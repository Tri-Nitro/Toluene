from typing import Literal
from zipfile import ZipFile

from toluene.core.coordinates import Coordinates
from toluene.util.file import tempdir


class Geoid:
    """
    The abstract base class for a Geoid. A geoid is a model of the earth's surface. It is defined by a grid of points
    and the spherical harmonics. The spherical harmonics are used to calculate gravity. The grid is used to interpolate
    the sea level at a point. The geoid is based on an ellipsoid. The ellipsoid is used to calculate the height of a
    perfect ellipsoid to model the earth. The geoid is then used to calculate the difference between the ellipsoid and
    the theoretical sea level.

    :param interpolation_grid_zip: The path to the interpolation grid zip file.
    :type interpolation_grid_zip: str
    :param spherical_harmonics_zip: The path to the spherical harmonics zip file. Defaults to ``None``.
    :type spherical_harmonics_zip: str, optional
    """

    def __init__(self, interpolation_grid_zip: str, spherical_harmonics_zip: str = None):
        with ZipFile(interpolation_grid_zip, 'r') as zObject:
            zObject.extractall(path=tempdir)

        if spherical_harmonics_zip is not None:
            with ZipFile(spherical_harmonics_zip, 'r') as zObject:
                zObject.extractall(path=tempdir)

        self._grid_spacing = None
        self._interpolation_grid = None


    def height(self, position: Coordinates, interpolation: Literal['bilinear'] = 'bilinear') -> float:
        """
        Calculates the height of the geoid at a point. This method is pure virtual and must be implemented by the
        subclass.

        :param position: The position to calculate the height at.
        :type position: Coordinates
        :param interpolation: The interpolation method to use for computing the value. Defaults to ``bilinear``.
        :type interpolation: Literal['bilinear'], optional. Defaults to bilinear.
        :return: The height of the geoid at the point.
        :rtype: float
        """
        raise NotImplementedError

