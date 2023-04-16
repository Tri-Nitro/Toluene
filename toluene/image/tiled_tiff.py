from math import ceil
from typing import Literal

import numpy as np

from toluene.compression.deflate import deflate_compression
from toluene.image.image_c_library import image_c_library
from toluene.image.tiff_pixel_data import TIFFPixelData

tiled_tiff_tags = ['TileWidth', 'TileLength', 'TileOffsets', 'TileByteCounts']


class TiledTiff(TIFFPixelData):
    """
    Defines pixel data for Tiled TIFFs

    Args:
        image_ifd (dict): The TIFF IFDs containing the tags
        image_data (bytes): The TIFF file data or stream data
    """

    def __init__(self, image_ifd: dict, image_data: bytes,
                 byte_order: Literal["little", "big"]):

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

        self._uncompressed_pixel_data = None
        if self._uncompressed_pixel_data is not None:
            return self._uncompressed_pixel_data

        tiles = [deflate_compression.decode(tile)
                 for tile in self._raw_pixel_data]
        self._uncompressed_pixel_data = []

        bytes_per_channel = self._bit_depth // 8
        bytes_in_pixel = self._color_depth * bytes_per_channel

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
    return all(tag in image_ifd for tag in tiled_tiff_tags)
