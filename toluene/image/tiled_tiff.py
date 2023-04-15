from typing import List

from toluene.image.tiff_pixel_data import TiffPixelData

tags = ['TileWidth', 'TileLength', 'TileOffsets', 'TileByteCounts']


class TiledTiff(TiffPixelData):

    def __init__(self, image_ifd: List[dict], image_data: bytes):
        super().__init__(image_ifd, image_data)
        self.__raw_pixel_data = []
        for ifd in image_ifd:
            raw_pixel_data_for_ifd = []
            tile_offsets = ifd['TileOffsets']
            tile_byte_counts = ifd['TileByteCounts']
            if isinstance(tile_offsets, list):
                for idx in range(len(tile_offsets)):
                    offset = tile_offsets[idx]
                    byte_count = tile_byte_counts[idx]
                    raw_pixel_data_for_ifd.append(image_data[offset:offset + byte_count])
            else:
                offset = tile_offsets
                byte_count = tile_byte_counts
                raw_pixel_data_for_ifd.append(image_data[offset:offset + byte_count])
            self.__raw_pixel_data.append(raw_pixel_data_for_ifd)

    def get_pixel_data(self):
        return self.__raw_pixel_data[0]


def is_tiled_tiff(image_tags: dict) -> bool:
    return all(tag in image_tags for tag in tags)
