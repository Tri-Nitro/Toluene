from typing import Literal

from toluene.image.pixel_data import PixelData


class TIFFPixelData(PixelData):
    """
    Defines pixel data for TIFFs

    Args:
        image_ifd (dict): The TIFF IFDs containing the tags
        image_data (bytes): The TIFF file data or stream data
    """

    def __init__(self, image_ifd: dict, image_data: bytes,
                 byte_order: Literal["little", "big"]):
        super().__init__(image_data)

        self._image_width = image_ifd['ImageWidth']
        self._image_length = image_ifd['ImageLength']
        self._bit_depth = image_ifd['BitsPerSample']
        self._color_depth = image_ifd['SamplesPerPixel']
        self._compressions = image_ifd['Compression']
        self._endian = byte_order
