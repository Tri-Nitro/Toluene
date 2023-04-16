import logging
from time import time
from timeit import timeit

from toluene.image.tiff import TIFF

logger = logging.getLogger('toluene.image.geotiff')

geotiff_1_0_tags = {
    33550: 'ModelPixelScaleTag', 33922: 'ModelTiepointTag',
    34264: 'ModelTransformationTag', 34735: 'GeoKeyDirectoryTag',
    34736: 'GeoDoubleParamsTag', 34737: 'GeoAsciiParamsTag'
}

gdal_tags = {
    42112: 'GDAL_METADATA', 42113: 'GDAL_NODATA'
}


class GeoTIFF(TIFF):
    def __init__(self, file: str = None):
        logger.debug(f'Initializing GeoTIFF({file})')
        super().__init__(file)
        logger.debug(f'Finished Initializing GeoTIFF')

    def _parse(self, tags=None):
        logger.debug(f'Entering GeoTIFF._parse(tags)')

        if tags is None:
            tags = {}
        tags.update(geotiff_1_0_tags)
        tags.update(gdal_tags)

        super()._parse(tags)
