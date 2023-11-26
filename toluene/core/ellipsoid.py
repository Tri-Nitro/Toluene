import logging

from toluene.util.exception import LatitudeOutOfRange
from toluene_extensions.core_extensions import ellipsoid_radius

logger = logging.getLogger('toluene.core.ellipsoid')


class Ellipsoid:
    """
    Defines an ellipsoid for toluene. The ellipsoid is defined by the semi-major axis and the inverse flattening factor.
    For too complicated reasons to explain here, the earth is not a sphere, but an ellipsoid. An Ellipsoid is close to a
    sphere but bulges at the equator and is flattened at the poles. The semi-major axis is the radius of the ellipsoid
    at the equator and the inverse flattening factor is the flattening factor inverted. The flattening factor defines
    how much the ellipsoid is flattened at the poles.

    :param semi_major_axis: The semi-major axis of the ellipsoid in meters. The semi-major axis is the radius of the
        ellipsoid at the equator.
    :type semi_major_axis: float
    :param inverse_flattening: The inverse flattening factor of the ellipsoid. The inverse flattening factor is defined
        as the flattening factor inverted. :math:`\\frac{1}{f} = \\frac{a}{a-b}` where :math:`a` is the semi-major axis
        and :math:`b` is the semi-minor axis. The semi-minor axis is the radius of the ellipsoid at the poles in meters.
    :type inverse_flattening: float
    :param epsg: The EPSG number of the ellipsoid. More ellipsoids can be found at `<https://epsg.io>`_, Defaults to
        ``None``
    :type epsg: int, optional
    """

    def __init__(self, semi_major_axis: float, inverse_flattening: float, epsg: int = None):
        logger.debug(f'Initializing Ellipsoid({semi_major_axis}, {inverse_flattening}, {epsg})')
        self.__semi_major_axis = semi_major_axis
        self.__inverse_flattening = inverse_flattening
        self.__epsg = epsg
        self.__semi_minor_axis = semi_major_axis * (1 - 1 / inverse_flattening)

    def ellipsoid_radius(self, latitude: float = None) -> float:
        """
        Calculates the radius of the ellipsoid at a given latitude. The latitude must be between -90 and 90 degrees.
        The solution is given in meters. The formula used is
        :math:`R = \\sqrt{\\frac{a^2}{1+(\\frac{1}{(1-f)^2}-1)\\cdot sin^2(\\phi)}}`. Where :math:`a` is the semi-major
        axis, :math:`f` is the flattening factor and :math:`\\phi` is the latitude.

        :param latitude: The latitude of the point on the ellipsoid in degrees. Must be between -90 and 90 degrees.
            -90 is the South Pole and 90 is the North Pole and is just equal to the semi-minor axis. 0 is the equator
            and is equal to the semi-major axis.
        :type latitude: float
        :return: The radius of the ellipsoid at the given latitude in meters.
        :rtype: float
        """
        logger.debug(f'Entering Ellipsoid.ellipsoid_radius({latitude})')

        if latitude > 90 or latitude < -90:
            logger.warning(f'Unable to handle Latitudes < -90 or > 90, {latitude} was given')
            raise LatitudeOutOfRange

        return ellipsoid_radius(self.__semi_major_axis, self.__semi_minor_axis, latitude)

    def semi_major_axis(self) -> float:
        """
        The semi major axis of the ellipsoid. The semi-major axis is the radius of the ellipsoid at the equator. The
        mathematical symbol for the semi-major axis is :math:`a` in most cases.

        :return: The semi-major axis of the ellipsoid.
        :rtype: float
        """
        logger.debug(f'Entering Ellipsoid.semi_major_axis()')
        return self.__semi_major_axis

    def semi_minor_axis(self) -> float:
        """
        The semi minor axis of the ellipsoid. The semi-minor axis is the radius of the ellipsoid at the poles. The
        mathematical symbol for the semi-minor axis is :math:`b` in most cases. This is computed at the class'
        initialization using the formula :math:`b = a(1-f)` where :math:`a` is the semi-major axis and :math:`f` is the
        flattening factor.

        :return: The semi-minor axis of the ellipsoid.
        :rtype: float
        """
        logger.debug(f'Entering Ellipsoid.semi_minor_axis()')
        return self.__semi_minor_axis

    def flattening(self) -> float:
        """
        The flattening factor of the ellipsoid. The flattening factor is the relationship between the semi-major axis
        and the semi-minor axis. The mathematical symbol for the flattening factor is :math:`f` in most cases. This is
        computed with the formula :math:`f = \\frac{a}{a-b}` where :math:`a` is the semi-major axis and :math:`b` is the
        semi-minor axis. The flattening factor isn't saved in the class, but computed when the function is called with
        the formula :math:`f = \\frac{1}{\\frac{1}{f}}`.

        :return: The flattening factor of the ellipsoid.
        :rtype: float
        """
        logger.debug(f'Entering Ellipsoid.flattening()')
        return 1 / self.__inverse_flattening

    def epsg(self) -> int:
        """
        EPSG number of the ellipsoid. More ellipsoids than the ones defined here can be found at `<https://epsg.io>`_.
        There are plans to add more ellipsoids to this class, but for now, the user can define their own ellipsoid if
        the one's supplied don't fit their needs. The fact that the epsg number is saved in the class is just a for
        reference and has no effect on the calculations.

        :return: The EPSG number of the ellipsoid.
        :rtype: int
        """
        logger.debug(f'Entering Ellipsoid.epsg()')
        return self.__epsg


# Defined ellipsoids

wgs_66_ellipsoid = Ellipsoid(semi_major_axis=6378145.0, inverse_flattening=298.25, epsg=4890)
"""
WGS66/EPSG:4890 ellipsoid `<https://epsg.io/4890>`_ World Geodetic System 1966.
The semi-major axis is 6,378,145.0 meters and the inverse flattening is 298.25.
"""

wgs_72_ellipsoid = Ellipsoid(semi_major_axis=6378135.0, inverse_flattening=298.26, epsg=4322)
"""
WGS72/EPSG:4322 ellipsoid `<https://epsg.io/4322>`_ World Geodetic System 1972.
The semi-major axis is 6,378,135.0 meters and the inverse flattening is 298.26.
"""

wgs_84_ellipsoid = Ellipsoid(semi_major_axis=6378137.0, inverse_flattening=298.257223563, epsg=4326)
"""
WGS84/EPSG:4326 ellipsoid `<https://epsg.io/4326>`_ World Geodetic System 1984.
The semi-major axis is 6,378,137.0 meters and the inverse flattening is 298.257223563.
This is the most commonly used ellipsoid.
"""

grs_80_ellipsoid = Ellipsoid(semi_major_axis=6378137.0, inverse_flattening=298.257222101, epsg=7019)
""" 
GRS80/EPSG:7019 ellipsoid `<https://epsg.io/7019-ellipsoid>`_ Geodetic Reference System 1980.
The semi-major axis is 6,378,137.0 meters and the inverse flattening is 298.257222101.
"""
