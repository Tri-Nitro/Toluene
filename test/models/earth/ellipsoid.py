import pytest

from toluene.models.earth.ellipsoid import Ellipsoid

ellipsoids_test_cases = [
    ('Airy 1830', 6377563.396, 6356256.909237285),
    ('Bessel 1841', 6377397.155, 6356078.962818189),
    ('Clarke 1866', 6378206.4, 6356583.799998981),
    ('Clarke 1880', 6378249.145, 6356514.8695497755),
    ('Everest 1830', 6377276.345, 6356075.41314024),
    ('Fischer 1960', 6378160, 6356778.303721086),
    ('GRS 80', 6378137, 6356752.314140356),
    ('Hayford 1909', 6378388, 6356911.9461279465),
    ('Helmert 1906', 6378200, 6356818.169627891),
    ('IUGG 1967', 6378160, 6356774.50408554),
    ('Krasovsky 1940', 6378245, 6356863.018773047),
    ('South American 1969', 6378160, 6356774.719195305),
    ('WGS 72', 6378135, 6356750.520016094),
    ('WGS 84', 6378137, 6356752.314245179)
]

inverse_flattening_accepted = [
    299.3249646000043,
    299.15281280000335,
    294.9786981999987,
    293.46499999999986,
    300.8017000000069,
    298.29999999999956,
    298.2572221010041,
    297.000000000005,
    298.2999999999936,
    298.247,
    298.29999999999984,
    298.24999999999187,
    298.26000000000556,
    298.2572235629972,
]

ellipsoids = []


class TestEllipsoid:
    def test_new_ellipsoid(self):
        for test_case in ellipsoids_test_cases:
            ellipsoid = Ellipsoid(test_case[1], test_case[2])
            ellipsoids.append(ellipsoid)

    def test_get_axes(self):
        for idx in range(len(ellipsoids)):
            semi_major_axis, semi_minor_axis = ellipsoids[idx].axes
            assert semi_major_axis == ellipsoids_test_cases[idx][1]
            assert semi_minor_axis == ellipsoids_test_cases[idx][2]

    def test_get_semi_major_axis(self):
        for idx in range(len(ellipsoids)):
            semi_major_axis = ellipsoids[idx].semi_major_axis
            assert semi_major_axis == ellipsoids_test_cases[idx][1]

    def test_get_semi_minor_axis(self):
        for idx in range(len(ellipsoids)):
            semi_minor_axis = ellipsoids[idx].semi_minor_axis
            assert semi_minor_axis == ellipsoids_test_cases[idx][2]

    def test_inverse_flattening(self):
        for idx in range(len(ellipsoids)):
            inverse_flattening = 1. / ellipsoids[idx].flattening
            assert inverse_flattening == inverse_flattening_accepted[idx]
