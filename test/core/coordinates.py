from toluene.core.coordinates import LLA
from toluene.core.time import TerrestrialTimeJ2000

import numpy as np

test_lla_coordinates = [LLA(latitude, longitude, altitude, time=TerrestrialTimeJ2000())
                        for latitude in np.arange(-90, 90, 6.533242)
                        for longitude in np.arange(-180, 180, 12.3456)
                        for altitude in np.arange(100)]

test_ecef_coordinates = [coordinate.to_ecef() for coordinate in test_lla_coordinates]

class TestCoordinates:

    def test_lla_and_ecef_conversion(self):
        for coordinates in test_lla_coordinates:
            old = coordinates
            new = coordinates.to_ecef().to_lla()
            # Just testing to get close enough because pinpoint accuracy isn't really possible
            # It's +/- 10 nanometers off which is super close. Technically this is a few conversions
            # because the distance is only ever measured in ECEF.
            assert old - new < 0.00000001

    def test_ecef_and_eci_conversion(self):
        for coordinates in test_ecef_coordinates:
            old = coordinates
            new = coordinates.to_eci().to_ecef()
            assert old - new < 0.0000001

    def test_lla_and_eci_conversion(self):
        for coordinates in test_lla_coordinates:
            old = coordinates
            new = coordinates.to_eci().to_lla()
            assert old - new < 0.0000001

    def test_lla_magnitude(self):
        for coordinates in test_lla_coordinates:
            # This is kinda testing both lla_to_ecef and the magnitude function
            # But the magnitude in LLA is the altitude over the ellipsoid + the radius of ellipsoid at that latitude
            # So it's a good test. We're going for close enough because of rounding error not exact.
            assert abs(coordinates.magnitude() - coordinates.to_ecef().magnitude()) < 0.001
