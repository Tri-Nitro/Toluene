import logging

import numpy as np
from numpy import float32, float64, frombuffer
from typing import Literal

from toluene.compression.deflate import deflate_compression
from toluene.image.image import Image
from toluene.image.tiff_pixel_data import TIFFPixelData
from toluene.image.tiled_tiff import TiledTiff, is_tiled_tiff
from toluene.util.exception import MagicNumberError, UndefinedTagError

logger = logging.getLogger('toluene.image.tiff')


def rationalize(value: bytes, byte_order: Literal['little', 'big']) -> float:
    ret = int.from_bytes(value[:4], byte_order) \
          / int.from_bytes(value[4:], byte_order)
    return ret


def unpack_float(value: bytes, byte_order: Literal['little', 'big']) -> float:
    return frombuffer(value, dtype=float32)


def unpack_double(value: bytes, byte_order: Literal['little', 'big']) -> float:
    return frombuffer(value, dtype=float64)


def create_string(value: bytes, byte_order: Literal['little', 'big']) -> str:
    return str(value)


tiff_ifd_entry_type_length = {
    1: 1, 2: 1, 3: 2, 4: 4, 5: 8, 6: 1, 7: 1, 8: 2, 9: 4, 10: 8, 11: 4, 12: 8
}

tiff_ifd_entry_value_convert_function = {
    1: int.from_bytes, 2: create_string, 3: int.from_bytes, 4: int.from_bytes,
    5: rationalize, 6: create_string, 7: create_string, 8: int.from_bytes,
    9: int.from_bytes, 10: rationalize, 11: unpack_float, 12: unpack_double
}

baseline_tags = {
    254: 'NewSubfileType', 255: 'SubfileType', 256: 'ImageWidth',
    257: 'ImageLength', 258: 'BitsPerSample', 259: 'Compression',
    262: 'PhotometricInterpretation', 263: 'Threshholding', 264: 'CellWidth',
    265: 'CellLength', 266: 'FillOrder', 270: 'ImageDescription',
    271: 'Make', 272: 'Model', 273: 'StripOffsets', 274: 'Orientation',
    277: 'SamplesPerPixel', 278: 'RowsPerStripe', 279: 'StripByteCount',
    280: 'MinSampleValue', 281: 'MaxSampleValue', 282: 'XResolution',
    283: 'YResolution', 284: 'PlanarConfiguration', 288: 'FreeOffsets',
    289: 'FreeByteCounts', 290: 'GrayResponseUnit', 291: 'GrayResponseCurve',
    296: 'ResolutionUnit', 305: 'Software', 306: 'DateTime', 315: 'Artist',
    316: 'HostComputer', 320: 'ColorMap', 338: 'ExtraSamples',
    33432: 'Copyright'
}

extended_tags = {
    269: 'DocumentName', 285: 'PageName', 286: 'XPosition', 287: 'YPosition',
    292: 'T4Options', 293: 'T6Options', 297: 'PageNumber',
    301: 'TransferFunction', 317: 'Predictor', 318: 'WhitePoint',
    319: 'PrimaryChromaticities', 321: 'HalftoneHints', 322: 'TileWidth',
    323: 'TileLength', 324: 'TileOffsets', 325: 'TileByteCounts',
    326: 'BadFaxLines', 327: 'CleanFaxData', 328: 'ConsecutiveBadFaxLines',
    330: 'SubIFDs', 332: 'InkSet', 333: 'InkNames', 334: 'NumberOfInks',
    336: 'DotRange', 337: 'TargetPrinter', 339: 'SampleFormat',
    340: 'SMinSampleValue', 341: 'SMaxSampleValue', 342: 'TransferRange',
    343: 'ClipPath', 344: 'XClipPathUnits', 345: 'YClipPathUnits',
    346: 'Indexed'
}

basic_tags = {}

basic_tags.update(baseline_tags)
basic_tags.update(extended_tags)


class TIFF(Image):
    """
    Defines a TIFF image.

    Args:
        file (str): The tiff file path.
    """

    def __init__(self, file: str = None):

        logger.debug(f'Initializing TIFF({file})')

        super().__init__(file)

        logger.debug(f'Finished Initializing TIFF')

    def _parse(self, tags=None):
        """
        Overwritten Image._parse() loads the image header into memory.

        Raises:
            UndefinedTagError
        """

        logger.debug(f'Entering TIFF._parse({tags})')

        if tags is None:
            tags = {}
        tags.update(basic_tags)

        # Determine the byteorder
        byte_order_flag = self._image_file[:2]
        if byte_order_flag == b'II':
            self._byte_order: Literal["little", "big"] = 'little'
        elif byte_order_flag == b'MM':
            self._byte_order: Literal["little", "big"] = 'big'

        if int.from_bytes(self._image_file[2:4], self._byte_order) != 42:
            raise MagicNumberError()

        ifd_offset = int.from_bytes(self._image_file[4:8], self._byte_order)

        self._ifd_directories = []
        while ifd_offset != 0:
            ifd_entry = {}
            number_of_ifd_entries = int.from_bytes(
                self._image_file[ifd_offset:ifd_offset + 2], self._byte_order)
            ifd_offset += 2
            for entry_id in range(number_of_ifd_entries):
                tag = int.from_bytes(
                    self._image_file[ifd_offset:ifd_offset + 2],
                    self._byte_order)
                field_type = int.from_bytes(
                    self._image_file[ifd_offset + 2:ifd_offset + 4],
                    self._byte_order)
                num_of_values = int.from_bytes(
                    self._image_file[ifd_offset + 4:ifd_offset + 8],
                    self._byte_order)
                value_offset = self._image_file[ifd_offset + 8:ifd_offset + 12]
                conversion = tiff_ifd_entry_value_convert_function[field_type]
                value_size = tiff_ifd_entry_type_length[field_type]
                if num_of_values == 1:
                    if value_size > 4:
                        offset = int.from_bytes(value_offset, self._byte_order)
                        value_offset = \
                            self._image_file[offset:offset + value_size]
                    try:
                        ifd_entry[tags[tag]] = conversion(
                            value_offset, self._byte_order)
                    except KeyError:
                        raise UndefinedTagError
                else:
                    offset = int.from_bytes(value_offset, self._byte_order)
                    if value_size == 1:
                        ifd_entry[tags[tag]] = str(
                            self._image_file[offset:offset + num_of_values],
                            'UTF-8')
                    else:
                        value_list = []
                        for idx in range(num_of_values):
                            value_list.append(
                                conversion(
                                    self._image_file[offset:
                                                     offset + value_size],
                                    self._byte_order))
                            offset += value_size
                        ifd_entry[tags[tag]] = value_list

                ifd_offset += 12
            ifd_offset = int.from_bytes(
                self._image_file[ifd_offset:ifd_offset + 4],
                self._byte_order)
            self._ifd_directories.append(ifd_entry)
        self._pixel_data = [get_tiff_image_type(ifd)
                            (ifd, self._image_file, self._byte_order)
                            for ifd in self._ifd_directories]

    def ifd_directories(self):
        return self._ifd_directories

    def image(self, idx: int = 0):
        if idx > len(self._pixel_data):
            raise IndexError
        return self._pixel_data[idx].image()

    def __getitem__(self, item: int = 0):
        return self.image(item)


def get_tiff_image_type(image_ifd: dict) -> type(TIFFPixelData):
    if is_tiled_tiff(image_ifd):
        return TiledTiff
