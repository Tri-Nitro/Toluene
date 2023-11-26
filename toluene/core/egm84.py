import os
from typing import Literal

from toluene.core.coordinates import Coordinates
from toluene.core.geoid import Geoid
from toluene.util.file import tempdir

from toluene_extensions.core_extensions import egm84_height

class EGM84(Geoid):
    """
    The EGM84 geoid. The EGM84 geoid is a geoid model of the earth. It is based on the WGS84 ellipsoid and is defined
    by a grid of points and the spherical harmonics. The spherical harmonics are used to calculate gravity. The grid is
    used to interpolate the sea level at a point. Created by the National Geospatial-Intelligence Agency (NGA) and the
    data is available at `<https://earth-info.nga.mil/#wgs84-data>`_. It does not come with toluene and must be
    downloaded to use the EGM84 geoid. Opens the interpolation grid zip file and sphereical harmonics zip file into
    the temp dir of this run. The file WWGRID can be found there after super().__init__.

    :param interpolation_grid_zip: The path to the interpolation grid zip file.
    :type interpolation_grid_zip: str
    :param spherical_harmonics_zip: The path to the spherical harmonics zip file. Defaults to ``None``.
    :type spherical_harmonics_zip: str, optional
    """

    def __init__(self, interpolation_grid_zip: str, spherical_harmonics_zip: str = None):
        super().__init__(interpolation_grid_zip=interpolation_grid_zip, spherical_harmonics_zip=spherical_harmonics_zip)

        if os.path.exists(tempdir + '/WWGRID.TXT'):
            self._grid_spacing = .5
            self._interpolation_grid = tempdir + '/WWGRID.TXT'

    def height(self, position: Coordinates, interpolation: Literal['bilinear'] = 'bilinear') -> float:
        """
        Calculates the height of the EGM84 geoid at a point. Uses interpolation to calculate the height. The method of
        interpolation is defined by the interpolation parameter.

        :param position: The position to calculate the height at.
        :type position: Coordinates
        :param interpolation: The interpolation method to use for computing the value. Defaults to ``bilinear``.
        :type interpolation: Literal['bilinear'], optional. Defaults to bilinear.
        :return: The height of the EGM84 geoid at the point.
        :rtype: float
        """
        position = position.to_lla()
        return egm84_height(position.latitude, position.longitude,
                            self._interpolation_grid, self._grid_spacing, interpolation)
