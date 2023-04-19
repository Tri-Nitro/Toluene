import logging
from typing import Dict

from toluene.image.tiff import TIFF

logger = logging.getLogger('toluene.image.geotiff')

geotiff_1_0_tags = {
    33550: 'ModelPixelScaleTag', 33922: 'ModelTiepointTag',
    34264: 'ModelTransformationTag', 34735: 'GeoKeyDirectoryTag',
    34736: 'GeoDoubleParamsTag', 34737: 'GeoAsciiParamsTag'
}
"""
tags for GeoTIFFs v1.0
"""

gdal_tags = {
    42112: 'GDAL_METADATA', 42113: 'GDAL_NODATA'
}
"""
Tags for GDAL methods
"""


class GeoTIFF(TIFF):
    """
    Defines a GeoTIFF image object.

    Args:
        :param file: file of the GeoTIFF.
    """

    def __init__(self, file: str = None):
        logger.debug(f'Initializing GeoTIFF({file})')
        super().__init__(file)
        logger.debug(f'Finished Initializing GeoTIFF')

    def _parse(self, tags: Dict[int, str] = None):
        """
        Parses the GeoTIFF tags

        Args:
            :param tags: Optional additional tags not in GeoTIFF v1.0
        """

        logger.debug(f'Entering GeoTIFF._parse({tags})')

        if tags is None:
            tags = {}
        tags.update(geotiff_1_0_tags)
        tags.update(gdal_tags)

        super()._parse(tags)


geo = GeoTIFF("C:/Users/DTCan/Downloads/tjpeg.tif")


import matplotlib.pyplot as plt

plt.imshow(geo.image(), cmap='gray')
plt.show()
