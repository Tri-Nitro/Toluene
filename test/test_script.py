from toluene_extensions.base_extensions import ecef_from_lla
from toluene.base.coordinates import LLA
from toluene.base.ellipsoid import wgs_84_ellipsoid

from math import sqrt
#
# from timeit import timeit
#
# ecef = LLA(48.858093, 2.294694, 0)
# c = timeit(lambda: ecef_from_lla(wgs_84_ellipsoid.semi_major_axis(), wgs_84_ellipsoid.semi_minor_axis(), ecef.latitude, ecef.longitude, ecef.altitude), number=10000)
# python = timeit(lambda: python_ecef_from_lla(ecef.latitude, ecef.longitude, ecef.altitude, wgs_84_ellipsoid), number=10000)
#
# print("C took ", c)
#
# print("Python took ", python)

# def gravity (radius):
#     return 398601877000000/radius**2
#
#
# ecef = LLA(48.858093, 2.294694, 0).to_ecef()
#
# radius = sqrt(ecef.x**2+ecef.y**2+ecef.z**2)
# print(radius)
#
# print(gravity(radius))
# print(gravity(wgs_84_ellipsoid.semi_major_axis()))
# print(gravity(wgs_84_ellipsoid.semi_minor_axis()))

def precession(t):
    return 5028.796195*t + 1.1054348*t**2 + 0.00007964*t**3 - 0.000023857*t**4 - 0.0000000383*t**5

print(precession(260))