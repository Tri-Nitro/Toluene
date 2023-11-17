import datetime

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


from math import sin, cos
e_0 = 0.409092600601

def precession(t):
    psi = 5038.481507*t - 1.0790069*t**2 - 0.00114045*t**3 + 0.000132851*t**4
    chi = 10.556403*t - 2.3814292*t**2 - 0.00121197*t**3 + 0.000170663*t**4
    return psi*cos(e_0)**2 - chi*cos(e_0) + psi*sin(e_0)**2

print(precession(266)/3600)

from toluene.base.datetime import JulianDateTime

date = JulianDateTime(datetime.datetime(2100, 1, 1, 12,0,0))
print(date.century())
