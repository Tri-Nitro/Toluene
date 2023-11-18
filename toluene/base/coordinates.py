from __future__ import annotations

import logging
from math import cos, radians, sin, sqrt, tan

from toluene.base.ellipsoid import Ellipsoid, wgs_84_ellipsoid
from toluene_extensions.base_extensions import ecef_from_lla, lla_from_ecef

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


        # Faster in C than Python
        x, y, z = ecef_from_lla(self.__ellipsoid.semi_major_axis(), self.__ellipsoid.semi_minor_axis(), self.latitude, self.longitude, self.altitude)
        return ECEF(x, y, z, self.__ellipsoid)