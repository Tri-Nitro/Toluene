import logging
import matplotlib.pyplot as plt
import numpy as np

from toluene.image.tiff import TIFF, basic_tags

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


# geo = GeoTIFF("C:/Users/DTCan/Downloads/LC08_CU_031006_20220808_20220821_02_BP.TIF")
geo = GeoTIFF("/home/dylancannisi/Downloads/LE07_L2SP_147045_20230318_20230413_02_T1_SR_B1.TIF")
# for ifd_directory in geo.ifd_directories():
#     print(ifd_directory.keys())

pixels = geo.get_pixels()
# print(pixels)
plt.imshow(np.array([[[1], [2]], [[3], [4]]]))
plt.show()
