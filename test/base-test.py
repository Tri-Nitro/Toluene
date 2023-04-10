import math
import unittest

from base.ellipsoid import *

bad_latitudes = [91, -91.0, 135.0, 1999999.0, -20000000, 360, 90.00000000000001]

wgs_66_test_latitudes = {0: 6378145.0, 45: 6367425.45124896, 90: 6356759.769488684, -90: 6356759.769488684,
                         -45: 6367425.45124896, 78: 6357679.75027448, 38: 6370013.788134657,
                         29.8: 6372843.144040323, 55.9378: 6363445.224925261}

wgs_72_test_latitudes = {0: 6378135.0, 45: 6367415.828362414, 90: 6356750.520016094, -90: 6356750.520016094,
                         -45: 6367415.828362414, 78: 6357670.468663489, 38: 6370004.074357103,
                         29.8: 6372833.330786914, 55.9378: 6363435.741599941}

wgs_84_test_latitudes = {0: 6378137.0, 45: 6367417.7249666825, 90: 6356752.314245179, -90: 6356752.314245179,
                         -45: 6367417.7249666825, 78: 6357672.271703395, 38: 6370005.995880995,
                         29.8: 6372835.279584561, 55.9378: 6363437.599941369}

grs_80_test_latitudes = {0: 6378137.0, 45: 6367417.724914007, 90: 6356752.314140355, -90: 6356752.314140355,
                         -45: 6367417.724914007, 78: 6357672.271603059, 38: 6370005.995841013,
                         29.8: 6372835.279558474, 55.9378: 6363437.599869202}


class EllipsoidTestCase(unittest.TestCase):
    """
    Test for the Ellipsoid module in the base package.
    """
    def test_wgs_66_ellipsoid_radius_good(self):
        """
        Test the if WGS66 ellipsoid radius gives the right radius.
        """
        for test_latitude in wgs_66_test_latitudes:
            ellipsoid_class_answer = wgs_66_ellipsoid.ellipsoid_radius(math.radians(test_latitude))
            self.assertEqual(wgs_66_test_latitudes[test_latitude], ellipsoid_class_answer)

    def test_wgs_66_ellipsoid_radius_bad(self):
        """
        Test the if WGS66 ellipsoid raises the LatitudeOutOfRange if the given latitude is below -90 or above 90
        """
        for bad_latitude in bad_latitudes:
            with self.assertRaises(LatitudeOutOfRange):
                wgs_66_ellipsoid.ellipsoid_radius(bad_latitude)

    def test_wgs_72_ellipsoid_radius_good(self):
        """
        Test the if WGS72 ellipsoid radius gives the right radius.
        """
        for test_latitude in wgs_72_test_latitudes:
            ellipsoid_class_answer = wgs_72_ellipsoid.ellipsoid_radius(math.radians(test_latitude))
            self.assertEqual(wgs_72_test_latitudes[test_latitude], ellipsoid_class_answer)

    def test_wgs_72_ellipsoid_radius_bad(self):
        for bad_latitude in bad_latitudes:
            with self.assertRaises(LatitudeOutOfRange):
                wgs_72_ellipsoid.ellipsoid_radius(bad_latitude)

    def test_wgs_84_ellipsoid_radius_good(self):
        """
        Test the if WGS84 ellipsoid radius gives the right radius.
        """
        for test_latitude in wgs_84_test_latitudes:
            ellipsoid_class_answer = wgs_84_ellipsoid.ellipsoid_radius(math.radians(test_latitude))
            self.assertEqual(wgs_84_test_latitudes[test_latitude], ellipsoid_class_answer)

    def test_wgs_84_ellipsoid_radius_bad(self):
        for bad_latitude in bad_latitudes:
            with self.assertRaises(LatitudeOutOfRange):
                wgs_84_ellipsoid.ellipsoid_radius(bad_latitude)

    def test_grs_80_ellipsoid_radius_good(self):
        """
        Test the if GRS80 ellipsoid radius gives the right radius.
        """
        for test_latitude in grs_80_test_latitudes:
            ellipsoid_class_answer = grs_80_ellipsoid.ellipsoid_radius(math.radians(test_latitude))
            self.assertEqual(grs_80_test_latitudes[test_latitude], ellipsoid_class_answer)

    def test_grs_80_ellipsoid_radius_bad(self):
        for bad_latitude in bad_latitudes:
            with self.assertRaises(LatitudeOutOfRange):
                grs_80_ellipsoid.ellipsoid_radius(bad_latitude)

    def test_semi_major_axis_getter(self):
        """
        Test if the given ellipsoids give the right semi-major axis.
        """
        self.assertEqual(wgs_66_ellipsoid.semi_major_axis(), 6378145.0)
        self.assertEqual(wgs_72_ellipsoid.semi_major_axis(), 6378135.0)
        self.assertEqual(wgs_84_ellipsoid.semi_major_axis(), 6378137.0)
        self.assertEqual(grs_80_ellipsoid.semi_major_axis(), 6378137.0)

    def test_semi_minor_axis_getter(self):
        """
        Test if the given ellipsoids give the right semi-minor axis.
        """
        self.assertEqual(wgs_66_ellipsoid.semi_minor_axis(), 6356759.769488684)
        self.assertEqual(wgs_72_ellipsoid.semi_minor_axis(), 6356750.520016094)
        self.assertEqual(wgs_84_ellipsoid.semi_minor_axis(), 6356752.314245179)
        self.assertEqual(grs_80_ellipsoid.semi_minor_axis(), 6356752.314140355)

    def test_flattening_getter(self):
        """
        Test if the given ellipsoids give the right flattening factor.
        """
        self.assertEqual(wgs_66_ellipsoid.flattening(), 0.003352891869237217)
        self.assertEqual(wgs_72_ellipsoid.flattening(), 0.003352779454167505)
        self.assertEqual(wgs_84_ellipsoid.flattening(), 0.0033528106647474805)
        self.assertEqual(grs_80_ellipsoid.flattening(), 0.003352810681182319)

    def test_epsg_getter(self):
        """
        Test if the given ellipsoids give the right EPSG number.
        """
        self.assertEqual(wgs_66_ellipsoid.epsg(), 4890)
        self.assertEqual(wgs_72_ellipsoid.epsg(), 4322)
        self.assertEqual(wgs_84_ellipsoid.epsg(), 4326)
        self.assertEqual(grs_80_ellipsoid.epsg(), 7019)


if __name__ == '__main__':
    unittest.main()
