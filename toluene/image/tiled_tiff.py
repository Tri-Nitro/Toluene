import logging
from typing import Literal

import numpy as np

from toluene.compression.multithreaded_codec_runner import MultiThreadedCodecRunner
from toluene.image.tiff_pixel_data import TIFFPixelData

try:
    from toluene.image.image_c_library import image_c_library
except ImportError:
    image_c_library = None

logger = logging.getLogger('toluene.image.tiled_tiff')

tiled_tiff_tags = ['TileWidth', 'TileLength', 'TileOffsets', 'TileByteCounts']


class TiledTIFF(TIFFPixelData):
    """
    Defines pixel data for Tiled TIFFs.

    Args:
        :param image_ifd: The TIFF IFDs containing the tags.
        :param image_data: The TIFF file data or stream data.
        :param byte_order: The byte order of the tiff.
    """

    def __init__(self, image_ifd: dict, image_data: bytes,
                 byte_order: Literal["little", "big"]):

        logger.debug(f'Initializing TiledTIFF')

        super().__init__(image_ifd, image_data, byte_order)

        self._tile_width = image_ifd['TileWidth']
        self._tile_length = image_ifd['TileLength']

        self._uncompressed_pixel_data = None

        tile_offsets = image_ifd['TileOffsets']
        tile_byte_counts = image_ifd['TileByteCounts']

        self._raw_pixel_data = []
        if isinstance(tile_offsets, list):

            for idx in range(len(tile_offsets)):
                tile_offset = tile_offsets[idx]
                tile_byte_count = tile_byte_counts[idx]
                self._raw_pixel_data.append(
                    image_data[tile_offset:tile_offset + tile_byte_count])
        else:
            self._raw_pixel_data.append(
                image_data[tile_offsets:tile_offsets + tile_byte_counts])

    def image(self) -> np.array:
        """
        Gets the uncompressed tiled tiff

        Returns:
            :return: the uncompressed tiled tiff in a numpy array.
        """

        logger.debug(f'Entering TiledTIFF.image()')

        self._uncompressed_pixel_data = None
        if self._uncompressed_pixel_data is not None:
            return self._uncompressed_pixel_data

        threaded_codec = MultiThreadedCodecRunner(self._codec,
                                                  self._raw_pixel_data,
                                                  'decode')
        tiles = threaded_codec.run()

        self._uncompressed_pixel_data = []

        try:
            bytes_per_channel = self._bit_depth // 8
        except TypeError:
            bytes_per_channel = self._bit_depth[0] // 8

        raw_data = image_c_library.tiled_tiff_decoder(tiles,
                                                      self._image_length,
                                                      self._image_width,
                                                      self._tile_length,
                                                      self._tile_width,
                                                      bytes_per_channel,
                                                      self._color_depth)

        self._uncompressed_pixel_data = raw_data
        return self._uncompressed_pixel_data


def is_tiled_tiff(image_ifd: dict) -> bool:
    """
    Asserts the image has the tags to be considered a tiled tiff.

    Args:
        :param image_ifd: IFD of TIFF tags of the image in question.

    Returns:
        :return: True if the IFD has all the tags required to extract a
            TiledTIFF from the image data.

    """
    return all(tag in image_ifd for tag in tiled_tiff_tags)
