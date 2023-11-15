from toluene_extensions.base_extensions import lla_from_ecef
from toluene.base.coordinates import python_lla_from_ecef, LLA
from toluene.base.ellipsoid import wgs_84_ellipsoid

from timeit import timeit

ecef = LLA(48.858093, 2.294694, 0).to_ecef()
c = timeit(lambda: lla_from_ecef(wgs_84_ellipsoid.semi_major_axis(), wgs_84_ellipsoid.semi_minor_axis(), ecef.x, ecef.y, ecef.z), number=10000)
python = timeit(lambda: python_lla_from_ecef(ecef.x, ecef.y, ecef.z, wgs_84_ellipsoid), number=10000)

print("C took ", c)

print("Python took ", python)