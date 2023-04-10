from math import radians, sin, sqrt

from base.exception import LatitudeOutOfRange


class Ellipsoid:
    """
    Defines an Ellipsoid using the semi-major axis and the inverse flattening factor.
    There is also a definition for the epsg number.

    Args:
        semi_major_axis (float): the semi-major axis of the ellipsoid.
        inverse_flattening (float): the inverse flattening factor of the ellipsoid.
        epsg (int): The EPSG number that defines the ellipsoid.
    """

    def __init__(self, semi_major_axis: float, inverse_flattening: float, epsg: int = None):
        self.__semi_major_axis = semi_major_axis
        self.__inverse_flattening = inverse_flattening
        self.__epsg = epsg

        self.__semi_minor_axis = self.ellipsoid_radius(radians(90))

    def ellipsoid_radius(self, latitude: float = None) -> float:
        """
        Using the defined ellipsoid calculates the radius of the ellipsoid.

        Args:
            latitude (float): The latitude in which the desired radius is to be given.

        Returns:
            The radius at the given latitude.
        """
        if latitude > 90 or latitude < -90:
            raise LatitudeOutOfRange
        return sqrt((self.__semi_major_axis ** 2) /
                    (1 + (1 / (1 - 1 / self.__inverse_flattening) ** 2 - 1) * (sin(latitude) ** 2)))

    def semi_major_axis(self) -> float:
        """
        Returns:
            The semi-major axis of the ellipsoid.
        """
        return self.__semi_major_axis

    def semi_minor_axis(self) -> float:
        """
        Returns:
            The semi-minor axis of the ellipsoid.
        """
        return self.__semi_minor_axis

    def flattening(self) -> float:
        """
        Returns:
            The flattening factor of the ellipsoid.
        """
        return 1 / self.__inverse_flattening

    def epsg(self) -> int:
        """
        Returns:
            The EPSG number of the ellipsoid.
        """
        return self.__epsg


# Defined ellipsoids

wgs_66_ellipsoid = Ellipsoid(semi_major_axis=6378145.0, inverse_flattening=298.25, epsg=4890)
""" WGS66/EPSG:4890 ellipsoid https://epsg.io/4890 """

wgs_72_ellipsoid = Ellipsoid(semi_major_axis=6378135.0, inverse_flattening=298.26, epsg=4322)
""" WGS72/EPSG:4322 ellipsoid https://epsg.io/4322 """

wgs_84_ellipsoid = Ellipsoid(semi_major_axis=6378137.0, inverse_flattening=298.257223563, epsg=4326)
""" WGS84/EPSG:4326 ellipsoid https://epsg.io/4326 """

grs_80_ellipsoid = Ellipsoid(semi_major_axis=6378137.0, inverse_flattening=298.257222101, epsg=7019)
""" GRS80/EPSG:7019 ellipsoid https://epsg.io/7019-ellipsoid """
