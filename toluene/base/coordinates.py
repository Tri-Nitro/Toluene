from __future__ import annotations

import logging
from math import atan, atan2, cos, degrees, radians, sin, sqrt
from numpy import cbrt

from toluene.base.ellipsoid import Ellipsoid, wgs_84_ellipsoid
from toluene_extensions.base_extensions import lla_from_ecef

logger = logging.getLogger('toluene.base.coordinates')


class ECEF:
    """
    Defines an ECEF vector to give geocentric coordinates

    Args:
        :param x: The displacement on the X-axis. (Defaults to ``0.0``)
        :param y: The displacement on the Y-axis. (Defaults to ``0.0``)
        :param z: The displacement on the Z-axis. (Defaults to ``0.0``)
        :param ellipsoid: The ellipsoid the coordinates are in.
    """

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0, ellipsoid: Ellipsoid = wgs_84_ellipsoid):
        logger.debug(f'Initializing ECEF({x}, {y}, {z}, {ellipsoid})')

        self.x = x
        self.y = y
        self.z = z
        self.__ellipsoid = ellipsoid

        logger.debug(f'Finished Initializing ECEF')

    def __str__(self):
        """
        Gives the ECEF vector as a list of x,y,z with the addition of the EPSG number.
        """
        return f"[{self.x}, {self.y}, {self.z}, EPSG:={self.__ellipsoid.epsg()}]"

    def __sub__(self, other):
        """
        Gives the distance between two points in space in meters.

        Args:
            :param other: the other coordinate

        Returns:
            :return: The distance between the two points in space in meters.
        """

        if isinstance(other, LLA):
            other = other.to_ecef()

        return sqrt(abs((self.x-other.x)**2+(self.y-other.y)**2+(self.z-other.z)**2))

    def ellipsoid(self):
        """
        The ellipsoid the ECEF coordinates are in.

        Returns:
            :return: The ellipsoid object in the ECEF vector
        """

        logger.debug(f'Entering ECEF.ellipsoid()')

        return self.__ellipsoid

    def to_lla(self) -> LLA:
        """
        Converts the ECEF vector into the approximate LLA coordinates type.

        Returns:
            :return: The approximate LLA coordinates object. It's only an
                approximation because the height over the ellipsoid is unknown.
        """

        logger.debug(f'Entering ECEF.to_lla()')

        # Faster in C than Python
        latitude, longitude, altitude = lla_from_ecef(self.__ellipsoid.semi_major_axis(), self.__ellipsoid.semi_minor_axis(), self.x, self.y, self.z)
        return LLA(latitude, longitude, altitude, self.__ellipsoid)


class LLA:
    """
    Defines geodetic coordinates, I.E. Latitude, Longitude, Altitude over an ellipsoid.

    Args:
        :param latitude: The latitude in degrees.
        :param longitude: The longitude in degrees.
        :param altitude: The altitude.
        :param ellipsoid: The ellipsoid the coordinates are in.
    """

    def __init__(self, latitude: float = None, longitude: float = None, altitude: float = 0.0,
                 ellipsoid: Ellipsoid = wgs_84_ellipsoid):
        logger.debug(f'Initializing LLA({latitude}, {longitude}, {altitude}, {ellipsoid})')

        self.latitude = latitude
        self.longitude = longitude
        self.altitude = altitude
        self.__ellipsoid = ellipsoid

        logger.debug(f'Finished Initializing LLA')

    def __str__(self):
        """
        Gives the geodetic coordinates as a list of lat, lon, altitude with the addition of the EPSG number.
        """
        return f"[{self.latitude}, {self.longitude}, {self.altitude}, ESPG:={self.__ellipsoid.epsg()}]"

    def __sub__(self, other):
        """
        Gives the distance between two points in space in meters.

        Args:
            :param other: the other coordinate

        Returns:
            :return: The distance between the two points in space in meters.
        """

        temp_self = self.to_ecef()
        if isinstance(other, LLA):
            other = other.to_ecef()

        return temp_self - other

    def ellipsoid(self):
        """
        The ellipsoid the Geodetic coordinates are in.

        Returns:
            :return: The ellipsoid object in the LLA coordinates
        """

        logger.debug('Entering LLA.ellipsoid()')

        return self.__ellipsoid

    def to_ecef(self) -> ECEF:
        """
        Converts the geodetic coordinates into an ECEF vector.

        Returns:
            :return: The equal ECEF vector object.
        """

        logger.debug('Entering LLA.to_ecef()')

        return ecef_from_lla(self.latitude, self.longitude, self.altitude, self.__ellipsoid)


def ecef_from_lla(latitude: float, longitude: float, altitude: float = 0.0,
                  ellipsoid: Ellipsoid = wgs_84_ellipsoid) -> ECEF:
    """
    Static version of ECEF to geodetic coordinates.

    Args:
        :param latitude: The input latitude.
        :param longitude: The input longitude.
        :param altitude: The input altitude. (Defaults to ``0.0``)
        :param ellipsoid: The input ellipsoid. (Defaults to wgs84)

    Returns:
        :return: ECEF object of equivalent input.
    """
    semi_major_axis = ellipsoid.semi_major_axis()
    semi_minor_axis = ellipsoid.semi_minor_axis()

    e_2 = 1 - (semi_minor_axis ** 2 / semi_major_axis ** 2)
    n_phi = semi_major_axis / sqrt(1 - (e_2 * (sin(radians(latitude)) ** 2)))

    x = (n_phi + altitude) * cos(radians(latitude)) * cos(radians(longitude))
    y = (n_phi + altitude) * cos(radians(latitude)) * sin(radians(longitude))
    z = ((1 - e_2) * n_phi + altitude) * sin(radians(latitude))

    return ECEF(x, y, z, ellipsoid)


def python_lla_from_ecef(x: float, y: float, z: float,
                  ellipsoid: Ellipsoid = wgs_84_ellipsoid) -> LLA:
    """
    Static version geodetic to ECEF coordinates.

    Args:
        :param x: The input displacement in the X-axis.
        :param y: The input displacement in the Y-axis.
        :param z: The input displacement in the Z-axis.
        :param ellipsoid: The input ellipsoid. (Defaults to wgs84)

    Returns:
         :return: LLA object of equivalent input.
    """
    if x == 0 and y == 0:
        x = 0.000000001
    e_numerator = ellipsoid.semi_major_axis() ** 2 - \
                  ellipsoid.semi_minor_axis() ** 2
    e_2 = e_numerator / ellipsoid.semi_major_axis() ** 2
    e_r2 = e_numerator / ellipsoid.semi_minor_axis() ** 2
    p = sqrt(x * x + y * y)
    big_f = 54.0 * ellipsoid.semi_minor_axis() ** 2 * z * z
    big_g = p * p + z * z * (1 - e_2) - e_2 * e_numerator
    c = (e_2 * e_2 * big_f * p * p) / (big_g * big_g * big_g)
    s = cbrt(1 + c + sqrt(c * c + 2 * c))
    k = s + 1 + 1 / s
    big_p = big_f / (3 * k * k * big_g * big_g)
    big_q = sqrt(1 + 2 * e_2 * e_2 * big_p)
    sqrt_r_0 = (ellipsoid.semi_major_axis() ** 2 / 2) * (1 + 1 / big_q) - ((big_p * (1 - e_2) * z * z) / (big_q * (1 + big_q))) -(big_p * p * p) / 2
    sqrt_r_0 = sqrt_r_0 if sqrt_r_0 > 0 else 0
    sqrt_r_0 = sqrt(sqrt_r_0)
    r_0 = ((-1 * big_p * e_2 * p) / (1 + big_q)) + sqrt_r_0
    p_e_2_r_0 = p - e_2 * r_0
    big_u = sqrt(p_e_2_r_0 * p_e_2_r_0 + z * z)
    big_v = sqrt(p_e_2_r_0 * p_e_2_r_0 + (1 - e_2) * z * z)
    z_0 = (ellipsoid.semi_minor_axis() ** 2 * z) / \
          (ellipsoid.semi_major_axis() * big_v)

    latitude = degrees(atan((z + (e_r2 * z_0)) / p))
    longitude = degrees(atan2(y, x))
    altitude = big_u * (1 - (ellipsoid.semi_minor_axis() ** 2) /
                        (ellipsoid.semi_major_axis() * big_v))

    return LLA(latitude, longitude, altitude, ellipsoid)
