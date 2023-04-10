from toluene.base.ellipsoid import Ellipsoid, wgs_84_ellipsoid
from toluene.base.lla import LLA


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
        self.ellipsoid = ellipsoid

    def __str__(self):
        """
        Gives the ECEF vector as a list of x,y,z with the addition of the EPSG number.
        """
        return f"[{self.x}, {self.y}, {self.z}, EPSG:={self.ellipsoid.epsg()}]"

    def to_lla(self) -> LLA:
        """
        Converts the ECEF vector into the approximate LLA coordinates type.

        Returns:
            The approximate LLA coordinates object. It's only an approximation because the height over the ellipsoid is
            unknown.
        """
        # return lla_from_ecef(self.x, self.y, self.z, self.ellipsoid)
        pass
