import logging
from typing import Literal

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
        super().__init__(image_data)

        self._image_width = image_ifd['ImageWidth']
        self._image_length = image_ifd['ImageLength']
        self._bit_depth = image_ifd['BitsPerSample']
        self._color_depth = image_ifd['SamplesPerPixel']
        self._compression = image_ifd['Compression']
        self._endian = byte_order
