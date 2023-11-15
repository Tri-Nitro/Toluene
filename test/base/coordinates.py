from toluene.base.coordinates import LLA
import numpy as np

test_lla_coordinates = [LLA(latitude, longitude, altitude) for latitude in np.arange(-90, 90, 6.533242) for longitude in
                        np.arange(-180, 180, 12.3456) for altitude in np.arange(100)]


class TestCoordinates:

    def test_lla_and_ecef_conversion(self):
        for coordinates in test_lla_coordinates:
            old = coordinates
            new = coordinates.to_ecef().to_lla()
            # Just testing to get close enough because pinpoint accuracy isn't really possible
            # It's +/- 10 nanometers off which is super close.
            print(old-new)
            assert old - new < 0.00000001

