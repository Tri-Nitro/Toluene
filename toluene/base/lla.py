from toluene.base.ecef import ECEF
from toluene.base.ellipsoid import Ellipsoid, wgs_84_ellipsoid


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
        self.ellipsoid = ellipsoid

    def __str__(self):
        """
        Gives the geodetic coordinates as a list of lat, lon, altitude with the addition of the EPSG number.
        """
        return f"[{self.latitude}, {self.longitude}, {self.altitude}, epsg := {self.ellipsoid.epsg()}]"

    def to_ecef(self) -> ECEF:
        """
        Converts the geodetic coordinates into an ECEF vector.

        Returns:
            The equal ECEF vector object.
        """
        # return ecef_from_lla(self.latitude, self.longitude, self.altitude, self.ellipsoid)
        pass
