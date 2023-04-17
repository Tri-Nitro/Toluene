import logging
from typing import Dict

from toluene.image.tiff import TIFF

logger = logging.getLogger('toluene.image.geotiff')


"""
tags for GeoTIFFs v1.0
"""
geotiff_1_0_tags = {
    33550: 'ModelPixelScaleTag', 33922: 'ModelTiepointTag',
    34264: 'ModelTransformationTag', 34735: 'GeoKeyDirectoryTag',
    34736: 'GeoDoubleParamsTag', 34737: 'GeoAsciiParamsTag'
}

"""
Tags for GDAL methods
"""
gdal_tags = {
    42112: 'GDAL_METADATA', 42113: 'GDAL_NODATA'
}


class GeoTIFF(TIFF):
    """
    Defines a GeoTIFF image object.

    Args:
        file (str): file of the GeoTIFF.
    """
    def __init__(self, file: str = None):
        logger.debug(f'Initializing GeoTIFF({file})')
        super().__init__(file)
        logger.debug(f'Finished Initializing GeoTIFF')

    def _parse(self, tags: Dict[int, str] = None):
        """
        Parses the GeoTIFF tags

        Args:
            tags (Dict[int,str]): Optional additional tags not in GeoTIFF v1.0
        """

        logger.debug(f'Entering GeoTIFF._parse({tags})')

        if tags is None:
            tags = {}
        tags.update(geotiff_1_0_tags)
        tags.update(gdal_tags)

        super()._parse(tags)
