from toluene.base.ellipsoid import wgs_84_ellipsoid
from toluene_extensions.base_extensions import lla_from_ecef

print(wgs_84_ellipsoid.semi_major_axis(), wgs_84_ellipsoid.semi_minor_axis(), 45.0, 92.0, 0.0)
print(lla_from_ecef(wgs_84_ellipsoid.semi_major_axis(), wgs_84_ellipsoid.semi_minor_axis(), 45.0, 92.0, 0.0))
