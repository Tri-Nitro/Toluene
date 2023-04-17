from ctypes import POINTER, byref, c_int
from math import ceil

import numpy as np
from typing import List
import logging

from toluene.util.c_loader import CLibrary
from toluene.util.exception import CLibraryNotFound

logger = logging.getLogger('toluene.image.image_c_library')


class ImageCLibrary(CLibrary):
    """
    Finds and loads the lib toluene-image and wraps its functions in python.

    Raises:
        ``toluene.util.exception.CLibraryNotFound``
    """
    def __init__(self):
        logger.debug("Initializing ImageCLibrary")
        super().__init__('toluene-image')
        if not self.found_library():
            logger.warning("Unable to locate toluene-image library.")
            raise CLibraryNotFound()

        self.library.tiled_tiff_decoder.argtypes = [POINTER(c_int), c_int,
                                                    c_int, c_int, c_int, c_int,
                                                    c_int, POINTER(c_int)]

        logger.debug("Finished Initializing ImageCLibrary")

    def tiled_tiff_decoder(self, data: List[bytes], image_length: int,
                           image_width: int, tile_length: int, tile_width: int,
                           bytes_per_channel: int, color_depth: int) \
            -> np.array:

        """
        Converts the decompressed byte steam of a tiled tiff into a numpy
            array.

        Args:
            :param data: A list of decompressed tiles in the ``bytes`` format
            :param image_length: The length of the image. TAG ``'ImageLength'``
            :param image_width: The width of the image. TAG ``'ImageWidth'``
            :param tile_length: The length of a tile. TAG ``'TileLength'``
            :param tile_width: The width of a tile. TAG ``'TileWidth'``
            :param bytes_per_channel: The number of bytes per channel.
            :param color_depth: The number of channels per pixel.

        Returns:
            :return: a numpy array with the dimensions of [image_length,
                image_height,color_depth]
        """
        logger.debug(f'Entering ImageCLibrary(bytesSize{len(bytes)}, '
                     f'{image_length}, {image_width}, {tile_length}, '
                     f'{tile_width}, {bytes_per_channel}, {color_depth})')

        data = [byte for bytes_in_data in data for byte in bytes_in_data]
        data = (c_int * len(data))(*data)
        output = (c_int *
                  (image_width * image_length * color_depth))()

        self.library.tiled_tiff_decoder(data, image_length, image_width,
                                        tile_length, tile_width,
                                        bytes_per_channel, color_depth, output)

        retval = []

        for y in range(image_length):
            row = []
            for x in range(image_width):
                pixel_idx = (x + y * image_width) * color_depth
                pixel = [pixel
                         for pixel in
                         output[pixel_idx:pixel_idx+color_depth]]
                row.append(pixel)
            retval.append(row)

        return np.array(retval)


try:
    """
    Singleton instance of ImageCLibrary
    """
    image_c_library = ImageCLibrary()
except CLibraryNotFound:
    pass
