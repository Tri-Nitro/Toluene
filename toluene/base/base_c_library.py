from ctypes import POINTER, byref, c_double
import logging

from typing import Tuple

from toluene.base.ellipsoid import Ellipsoid, wgs_84_ellipsoid
from toluene.util.c_loader import CLibrary
from toluene.util.exception import CLibraryNotFound, LatitudeOutOfRange

logger = logging.getLogger('toluene.base.base_c_library')

loaded = True

class BaseCLibrary(CLibrary):
    """
    Finds and loads the lib toluene-base and wraps its functions in python.

    Raises:
        ``toluene.util.exception.CLibraryNotFound``
    """

    def __init__(self):
        logger.debug("Initializing BaseCLibrary")
        super().__init__('toluene')
        if not self.found_library():
            logger.warning("Unable to locate toluene library.")
            raise CLibraryNotFound()

        self.library.ecef_from_lla.argtypes = [c_double, c_double, c_double,
                                               c_double, c_double,
                                               POINTER(c_double),
                                               POINTER(c_double),
                                               POINTER(c_double)]

        self.library.ellipsoid_radius.restype = c_double
        self.library.ellipsoid_radius.argtypes = [c_double, c_double, c_double]

        self.library.lla_from_ecef.argtypes = [c_double, c_double, c_double,
                                               c_double, c_double,
                                               POINTER(c_double),
                                               POINTER(c_double),
                                               POINTER(c_double)]

        logger.debug("Finished Initializing BaseCLibrary")

    def ecef_from_lla(self, latitude: float, longitude: float,
                      altitude: float = 0.0,
                      ellipsoid: Ellipsoid = wgs_84_ellipsoid) \
            -> Tuple[float, float, float]:
        """
        Converts geodetic coordinates into an ECEF vector.

        Args:
            :param latitude: the latitude of the geodetic coordinates.
            :param longitude: the longitude of the geodetic coordinates.
            :param altitude: that altitude of the geodetic coordinates.
                Default to ``0.0`` over the ellipsoid.
            :param ellipsoid: the ellipsoid the coordinates are in.

        Returns:
            :return: The equal ECEF vector as a tuple of displacement of X,
                displacement of Y, displacement of Z axes.
        """

        logger.debug(f'Entering BaseCLibrary.ecef_from_lla({latitude}, '
                     f'{longitude}, {altitude}, {ellipsoid})')

        x = c_double()
        y = c_double()
        z = c_double()

        self.library.ecef_from_lla(ellipsoid.semi_major_axis(),
                                   ellipsoid.semi_minor_axis(),
                                   latitude, longitude, altitude,
                                   byref(x), byref(y), byref(z))

        logger.debug(f'BaseCLibrary.ecef_from_lla({latitude}, {longitude}, '
                     f'{altitude}, {ellipsoid}) == [{x}, {y}, {z}]')

        return x.value, y.value, z.value

    def ellipsoid_radius(self, latitude: float, semi_major_axis: float,
                         inverse_flattening: float) -> float:
        """
        Gives ellipsoid's radius at a given latitude.

        Args:
            :param latitude: The latitude in which the desired radius is to be
                given.
            :param semi_major_axis: The semi-major axis of the ellipsoid.
            :param inverse_flattening: The inverse flattening factor of the
                ellipsoid.

        Raises:
              ``toluene.util.exception.LatitudeOutOfRange``

        Returns:
            :return: The radius at the given latitude.
        """

        logger.debug(f'Entering BaseCLibrary.ellipsoid_radius({latitude}, '
                     f'{semi_major_axis}, {inverse_flattening})')

        if latitude > 90 or latitude < -90:
            logger.warning(f'Unable to handle Latitudes < -90 or > 90, '
                           f'{latitude} was given')
            raise LatitudeOutOfRange

        return self.library.ellipsoid_radius(latitude, semi_major_axis,
                                             inverse_flattening)

    def lla_from_ecef(self, x: float, y: float, z: float,
                      ellipsoid: Ellipsoid = wgs_84_ellipsoid) \
            -> Tuple[float, float, float]:
        """
        Converts an ECEF vector into the approximate LLA coordinates type.

        Args:
            :param x: the displacement in the X-axis.
            :param y: the displacement in the Y-axis.
            :param z: the displacement in the Z-axis.
            :param ellipsoid: the ellipsoid the coordinates are in.

        Returns:
            :return: The approximate LLA coordinates as a tuple of Latitude,
                Longitude, and Altitude in degrees. It's only an approximation
                because the height over the ellipsoid is unknown.
        """

        logger.debug(f'Entering BaseCLibrary.lla_from_ecef({x}, {y}, {z}, {ellipsoid})')

        latitude = c_double()
        longitude = c_double()
        altitude = c_double()

        self.library.lla_from_ecef(ellipsoid.semi_major_axis(), ellipsoid.semi_minor_axis(),
                                   x, y, z,
                                   byref(latitude), byref(longitude), byref(altitude))

        logger.debug(f'BaseCLibrary.ecef_from_lla({x}, {y}, {z}, {ellipsoid}) == [{latitude}, {longitude}, {altitude}]')

        return latitude.value, longitude.value, altitude.value


try:
    """
    Singleton instance of BaseCLibrary
    """
    base_c_library = BaseCLibrary()
except CLibraryNotFound:
    loaded = False