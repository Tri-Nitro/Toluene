from typing import Literal

import numpy as np

from toluene.compression.deflate import deflate_compression
from toluene.image.image_c_library import image_c_library
from toluene.image.tiff_pixel_data import TIFFPixelData

striped_tiff_tags = ['RowsPerStripe', 'StripByteCount']


class StripedTiff(TIFFPixelData):
    """
    Defines pixel data for Tiled TIFFs

    Args:
        image_ifd (dict): The TIFF IFDs containing the tags
        image_data (bytes): The TIFF file data or stream data
        byte_order (Literal["little", "big"]): The byte order of the data
    """

    def __init__(self, image_ifd: dict, image_data: bytes,
                 byte_order: Literal["little", "big"]):
        super().__init__(image_ifd, image_data, byte_order)

    def image(self) -> np.array:
        return np.array([])


def is_striped_tiff(image_ifd: dict) -> bool:
    return all(tag in image_ifd for tag in striped_tiff_tags)
