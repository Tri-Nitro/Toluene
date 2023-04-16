from toluene.image.tiff_pixel_data import TIFFPixelData

tiled_tiff_tags = ['TileWidth', 'TileLength', 'TileOffsets', 'TileByteCounts']


class TiledTiff(TIFFPixelData):
    """
    Defines pixel data for Tiled TIFFs

    Args:
        image_ifd (dict): The TIFF IFDs containing the tags
        image_data (bytes): The TIFF file data or stream data
    """

    def __init__(self, image_ifd: dict, image_data: bytes):

        super().__init__(image_ifd, image_data)

        tile_width = image_ifd['TileWidth']
        tile_length = image_ifd['TileLength']
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
                image_data[tile_offsets:tile_offsets+tile_byte_counts])

    def raw_pixel_data(self):
        return self._raw_pixel_data


def is_tiled_tiff(image_ifd: dict) -> bool:
    return all(tag in image_ifd for tag in tiled_tiff_tags)
