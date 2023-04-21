import logging
from typing import Literal

import numpy as np

from toluene.compression.multithreaded_codec_runner import MultiThreadedCodecRunner
from toluene.image.image_c_library import image_c_library
from toluene.image.tiff_pixel_data import TIFFPixelData

logger = logging.getLogger('toluene.image.striped_tiff')

striped_tiff_tags = ['RowsPerStripe', 'StripOffsets', 'StripByteCount']


class StripedTIFF(TIFFPixelData):
    """
    Defines pixel data for Tiled TIFFs

    Args:
        :param image_ifd: The TIFF IFDs containing the tags
        :param image_data: The TIFF file data or stream data
        :param byte_order: The byte order of the data
    """

    def __init__(self, image_ifd: dict, image_data: bytes,
                 byte_order: Literal["little", "big"]):

        logger.debug(f'Initializing StripedTIFF')

        super().__init__(image_ifd, image_data, byte_order)

        self.__rows_per_stripe = image_ifd['RowsPerStripe']

        self._uncompressed_pixel_data = None

        strip_offsets = image_ifd['StripOffsets']
        strip_byte_counts = image_ifd['StripByteCount']

        self._raw_pixel_data = []
        if isinstance(strip_offsets, list):

            for idx in range(len(strip_offsets)):
                strip_offset = strip_offsets[idx]
                strip_byte_count = strip_byte_counts[idx]
                self._raw_pixel_data.append(
                    image_data[strip_offset:strip_offset + strip_byte_count])
        else:
            self._raw_pixel_data.append(
                image_data[strip_offsets:strip_offsets + strip_byte_counts])

    def image(self) -> np.array:
        """
        Parses the pixel data into a numpy array.

        Returns:
            :return: numpy array of the pixel data.
        """

        logger.debug(f'Entering StripedTIFF.image()')

        self._uncompressed_pixel_data = None
        if self._uncompressed_pixel_data is not None:
            return self._uncompressed_pixel_data

        threaded_codec = MultiThreadedCodecRunner(self._codec,
                                                  self._raw_pixel_data,
                                                  'decode')
        strips = threaded_codec.run()

        self._uncompressed_pixel_data = []

        bytes_per_channel = self._bit_depth // 8

        raw_data = image_c_library.striped_tiff_decoder(strips,
                                                        self._image_length,
                                                        self._image_width,
                                                        bytes_per_channel,
                                                        self._color_depth)

        self._uncompressed_pixel_data = raw_data
        return self._uncompressed_pixel_data


def is_striped_tiff(image_ifd: dict) -> bool:
    """
    Asserts the image has the tags to be considered a striped tiff.

    Args:
        :param image_ifd: IFD of TIFF tags of the image in question.

    Returns:
        :return: True if the IFD has all the tags required to extract a
            StripedTIFF from the image data.

    """
    return all(tag in image_ifd for tag in striped_tiff_tags)
