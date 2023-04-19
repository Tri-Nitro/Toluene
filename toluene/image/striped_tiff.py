import logging
from typing import Literal

import numpy as np

from toluene.compression.multithreaded_codec_runner import MultiThreadedCodecRunner
from toluene.image.image_c_library import image_c_library
from toluene.image.tiff_pixel_data import TIFFPixelData

logger = logging.getLogger('toluene.image.striped_tiff')

striped_tiff_tags = ['RowsPerStripe', 'StripByteCount']


class StripedTIFF(TIFFPixelData):
    """
    Defines pixel data for Tiled TIFFs

    Args:
        image_ifd (dict): The TIFF IFDs containing the tags
        image_data (bytes): The TIFF file data or stream data
        byte_order (Literal["little", "big"]): The byte order of the data
    """

    def __init__(self, image_ifd: dict, image_data: bytes,
                 byte_order: Literal["little", "big"]):
        logger.debug(f'Initializing StripedTIFF({image_ifd}, '
                     f'bytesSize{len(image_data)}, {byte_order})')
        super().__init__(image_ifd, image_data, byte_order)
        logger.debug(f'Finished Initializing StripedTIFF')

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

        # threaded_codec = MultiThreadedCodecRunner(self._codec,
        #                                           self._raw_pixel_data,
        # #                                           'decode')
        # tiles = threaded_codec.run()

        self._uncompressed_pixel_data = []

        bytes_per_channel = self._bit_depth // 8
        #
        # raw_data = image_c_library.tiled_tiff_decoder(tiles,
        #                                               self._image_length,
        #                                               self._image_width,
        #                                               self._tile_length,
        #                                               self._tile_width,
        #                                               bytes_per_channel,
        #                                               self._color_depth)

        self._uncompressed_pixel_data = raw_data
        return self._uncompressed_pixel_data
        return np.array([])


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
