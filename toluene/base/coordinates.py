from __future__ import annotations
from math import atan, atan2, cos, degrees, radians, sin, sqrt
from numpy import cbrt

from toluene.base.ellipsoid import Ellipsoid, wgs_84_ellipsoid


class ECEF:
    """
    Defines an ECEF vector to give geocentric coordinates

    Args:
        x (float): The displacement on the X axis.
        y (float): The displacement on the Y axis.
        z (float): The displacement on the Z axis.
        ellipsoid (toluene.base.ellipsoid.Ellipsoid): The ellipsoid the coordinates are in.
    """

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0, ellipsoid: Ellipsoid = wgs_84_ellipsoid):
        self.x = x
        self.y = y
        self.z = z
        self.__ellipsoid = ellipsoid

    def __str__(self):
        """
        Gives the ECEF vector as a list of x,y,z with the addition of the EPSG number.
        """
        return f"[{self.x}, {self.y}, {self.z}, EPSG:={self.__ellipsoid.epsg()}]"

    def ellipsoid(self):
        """
        The ellipsoid the ECEF coordinates are in.

        Returns:
            The ellipsoid object in the ECEF vector
        """
        return self.__ellipsoid

    def to_lla(self) -> LLA:
        """
        Converts the ECEF vector into the approximate LLA coordinates type.

        Returns:
            The approximate LLA coordinates object. It's only an approximation because the height over the ellipsoid is
            unknown.
        """
        return lla_from_ecef(self.x, self.y, self.z, self.__ellipsoid)


class LLA:
    """
    Defines geodetic coordinates, I.E. Latitude, Longitude, Altitude over an ellipsoid.

    Args:
        latitude (float): The latitude in degrees.
        longitude (float): The longitude in degrees.
        altitude (float): The altitude in meters.
        ellipsoid (toluene.base.ellipsoid.Ellipsoid): The ellipsoid the coordinates are in.
    """

    def __init__(self, latitude: float = None, longitude: float = None, altitude: float = 0.0,
                 ellipsoid: Ellipsoid = wgs_84_ellipsoid):
        self.latitude = latitude
        self.longitude = longitude
        self.altitude = altitude
        self.__ellipsoid = ellipsoid

    def __str__(self):
        """
        Gives the geodetic coordinates as a list of lat, lon, altitude with the addition of the EPSG number.
        """
        return f"[{self.latitude}, {self.longitude}, {self.altitude}, epsg := {self.__ellipsoid.epsg()}]"

    def ellipsoid(self):
        """
        The ellipsoid the Geodetic coordinates are in.

        Returns:
            The ellipsoid object in the LLA coordinates
        """

    def to_ecef(self) -> ECEF:
        """
        Converts the geodetic coordinates into an ECEF vector.

        Returns:
            The equal ECEF vector object.
        """
        return ecef_from_lla(self.latitude, self.longitude, self.altitude, self.__ellipsoid)


def ecef_from_lla(latitude: float, longitude: float, altitude: float = 0.0,
                  ellipsoid: Ellipsoid = wgs_84_ellipsoid) -> ECEF:
    semi_major_axis = ellipsoid.semi_major_axis()
    semi_minor_axis = ellipsoid.semi_minor_axis()

    e_2 = 1 - (semi_minor_axis ** 2 / semi_major_axis ** 2)
    n_phi = semi_major_axis / sqrt(1 - (e_2 * (sin(radians(latitude)) ** 2)))

    x = (n_phi + altitude) * cos(radians(latitude)) * cos(radians(longitude))
    y = (n_phi + altitude) * cos(radians(latitude)) * sin(radians(longitude))
    z = ((1 - e_2) * n_phi + altitude) * sin(radians(latitude))

    return ECEF(x, y, z, ellipsoid)


def lla_from_ecef(x: float, y: float, z: float, ellipsoid: Ellipsoid) -> LLA:

    e_numerator = ellipsoid.semi_major_axis() ** 2 - ellipsoid.semi_minor_axis() ** 2
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
    r_0 = ((-1 * big_p * e_2 * p) / (1 + big_q)) + \
          sqrt((ellipsoid.semi_major_axis() ** 2 / 2) * (1 + 1 / big_q) - (
                      (big_p * (1 - e_2) * z * z) / (big_q * (1 + big_q))) - (big_p * p * p) / 2)
    p_e_2_r_0 = p - e_2 * r_0
    big_u = sqrt(p_e_2_r_0 * p_e_2_r_0 + z * z)
    big_v = sqrt(p_e_2_r_0 * p_e_2_r_0 + (1 - e_2) * z * z)
    z_0 = (ellipsoid.semi_minor_axis() ** 2 * z) / (ellipsoid.semi_major_axis() * big_v)

    latitude = degrees(atan((z + (e_r2 * z_0)) / p))
    longitude = degrees(atan2(y, x))
    altitude = big_u * (1 - (ellipsoid.semi_minor_axis() ** 2) / (ellipsoid.semi_major_axis() * big_v))

    return LLA(latitude, longitude, altitude, ellipsoid)
