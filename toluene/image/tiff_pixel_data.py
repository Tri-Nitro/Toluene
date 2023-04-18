import logging
from typing import Literal

from toluene.compression.codec import Codec
from toluene.compression.deflate import Deflate, deflate_compression
from toluene.image.pixel_data import PixelData

logger = logging.getLogger('toluene.image.tiff_pixel_data')


class TIFFPixelData(PixelData):
    """
    Defines pixel data for TIFFs

    Args:
        :param image_ifd: The TIFF IFDs containing the tags.
        :param image_data: The TIFF file data or stream data.
        :param byte_order: The byte order of the TIFF.
    """

    def __init__(self, image_ifd: dict, image_data: bytes,
                 byte_order: Literal["little", "big"]):
        logger.debug(f'Initializing TIFFPixelData({image_ifd}, image_data, '
                     f'{byte_order}')

        super().__init__(image_data)

        self._image_width = image_ifd['ImageWidth']
        self._image_length = image_ifd['ImageLength']
        self._bit_depth = image_ifd['BitsPerSample']
        self._color_depth = image_ifd['SamplesPerPixel']
        self._compression = image_ifd['Compression']
        self._endian = byte_order
        self._codec = get_codec(self._compression)

        logger.debug(f'Finished Initializing TIFFPixelData()')


def get_codec(compression: int) -> Codec:
    """
    Determines the codec to use for the compression in the tiff.

    Args:
        :param compression: The compression of the tiff as an int.

    Returns:
        :return: The codec to use on the tiff pixel data.
    """

    logger.debug(f'Entering get_codec({compression})')

    if compression == 8:
        return deflate_compression
