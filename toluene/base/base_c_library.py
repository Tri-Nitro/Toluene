from ctypes import POINTER, byref, c_double

from toluene.base.c_loader import CLibrary
from toluene.base.ellipsoid import Ellipsoid, wgs_84_ellipsoid
from toluene.base.exception import CLibraryNotFound, LatitudeOutOfRange


class BaseCLibrary(CLibrary):
    def __init__(self):
        super().__init__('toluene-base')
        if not self.found_library():
            raise CLibraryNotFound()

        self.library.ecef_from_lla.argtypes = [c_double, c_double, c_double, c_double, c_double,
                                               POINTER(c_double), POINTER(c_double), POINTER(c_double)]

        self.library.ellipsoid_radius.restype = c_double
        self.library.ellipsoid_radius.argtypes = [c_double, c_double, c_double]

        self.library.lla_from_ecef.argtypes = [c_double, c_double, c_double, c_double, c_double,
                                               POINTER(c_double), POINTER(c_double), POINTER(c_double)]

    def ecef_from_lla(self, latitude: float, longitude: float, altitude: float = 0.0,
                      ellipsoid: Ellipsoid = wgs_84_ellipsoid) -> tuple[float, float, float]:
        x = c_double()
        y = c_double()
        z = c_double()

        self.library.ecef_from_lla(ellipsoid.semi_major_axis(), ellipsoid.semi_minor_axis(),
                                   latitude, longitude, altitude,
                                   byref(x), byref(y), byref(z))

        return x.value, y.value, z.value

    def ellipsoid_radius(self, latitude: float, semi_major_axis: float, inverse_flattening: float) -> float:
        if latitude > 90 or latitude < -90:
            raise LatitudeOutOfRange
        return self.library.ellipsoid_radius(latitude, semi_major_axis, inverse_flattening)

    def lla_from_ecef(self, x: float, y: float, z: float, ellipsoid: Ellipsoid = wgs_84_ellipsoid) \
            -> tuple[float, float, float]:
        latitude = c_double()
        longitude = c_double()
        altitude = c_double()

        self.library.lla_from_ecef(ellipsoid.semi_major_axis(), ellipsoid.semi_minor_axis(),
                                   x, y, z,
                                   byref(latitude), byref(longitude), byref(altitude))

        return latitude.value, longitude.value, altitude.value


try:
    base_c_library = BaseCLibrary()
except CLibraryNotFound:
    pass
