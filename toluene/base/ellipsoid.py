import logging
import math
from math import sin, sqrt

from toluene.util.exception import LatitudeOutOfRange

logger = logging.getLogger('toluene.base.ellipsoid')


class Ellipsoid:
    """
    Defines an Ellipsoid using the semi-major axis and the inverse flattening
    factor. There is also a definition for the epsg number.

    Args:
        :param semi_major_axis: the semi-major axis of the ellipsoid.
        :param inverse_flattening: the inverse flattening factor of the
            ellipsoid.
        :param epsg: The EPSG number that defines the ellipsoid.
    """

    def __init__(self, semi_major_axis: float, inverse_flattening: float, epsg: int = None):

        logger.debug(f'Initializing Ellipsoid({semi_major_axis}, {inverse_flattening}, {epsg})')

        self.__semi_major_axis = semi_major_axis
        self.__inverse_flattening = inverse_flattening
        self.__epsg = epsg
        self.__semi_minor_axis = self.ellipsoid_radius(90)

        logger.debug('Finished Initializing Ellipsoid')

    def __eq__(self, other) -> bool:

        logger.debug(f'Asserting If {self} == {other}')

        return self.__semi_major_axis == other.semi_major_axis() and 1 / self.__inverse_flattening == other.flattening()

    def __str__(self) -> str:

        return f'ellipsoid<{self.__semi_major_axis}, {self.__semi_minor_axis}>'

    def ellipsoid_radius(self, latitude: float = None) -> float:
        """
        Using the defined ellipsoid calculates the radius of the ellipsoid.

        Args:
            :param latitude: The latitude in which the desired radius is to be given.

        Raises:
            ``toluene.util.exception.LatitudeOutOfRange``

        Returns:
            :return: The radius at the given latitude.
        """

        logger.debug(f'Entering Ellipsoid.ellipsoid_radius({latitude})')

        if latitude > 90 or latitude < -90:
            logger.warning(f'Unable to handle Latitudes < -90 or > 90, {latitude} was given')
            raise LatitudeOutOfRange

        latitude = math.radians(latitude)

        return sqrt((self.__semi_major_axis ** 2) /
                    (1 + (1 / (1 - 1 / self.__inverse_flattening) ** 2 - 1) * (sin(latitude) ** 2)))

    def semi_major_axis(self) -> float:
        """
        Returns:
            :return: The semi-major axis of the ellipsoid.
        """

        logger.debug(f'Entering Ellipsoid.semi_major_axis()')

        return self.__semi_major_axis

    def semi_minor_axis(self) -> float:
        """
        Returns:
            :return: The semi-minor axis of the ellipsoid.
        """

        logger.debug(f'Entering Ellipsoid.semi_minor_axis()')

        return self.__semi_minor_axis

    def flattening(self) -> float:
        """
        Returns:
            :return: The flattening factor of the ellipsoid.
        """

        logger.debug(f'Entering Ellipsoid.flattening()')

        return 1 / self.__inverse_flattening

    def epsg(self) -> int:
        """
        Returns:
            :return: The EPSG number of the ellipsoid.
        """

        logger.debug(f'Entering Ellipsoid.epsg()')

        return self.__epsg


# Defined ellipsoids

wgs_66_ellipsoid = Ellipsoid(semi_major_axis=6378145.0, inverse_flattening=298.25, epsg=4890)
""" WGS66/EPSG:4890 ellipsoid `<https://epsg.io/4890>`_ """

wgs_72_ellipsoid = Ellipsoid(semi_major_axis=6378135.0, inverse_flattening=298.26, epsg=4322)
""" WGS72/EPSG:4322 ellipsoid `<https://epsg.io/4322>`_ """

wgs_84_ellipsoid = Ellipsoid(semi_major_axis=6378137.0, inverse_flattening=298.257223563, epsg=4326)
""" WGS84/EPSG:4326 ellipsoid `<https://epsg.io/4326>`_ """

grs_80_ellipsoid = Ellipsoid(semi_major_axis=6378137.0, inverse_flattening=298.257222101, epsg=7019)
""" GRS80/EPSG:7019 ellipsoid `<https://epsg.io/7019-ellipsoid>`_ """
