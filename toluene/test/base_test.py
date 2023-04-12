import math
import unittest

from toluene.base.coordinates import *
from toluene.base.ellipsoid import *


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
            self.assertAlmostEqual(wgs_66_test_latitudes[test_latitude], ellipsoid_class_answer)

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
            self.assertAlmostEqual(wgs_72_test_latitudes[test_latitude], ellipsoid_class_answer)

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
            self.assertAlmostEqual(wgs_84_test_latitudes[test_latitude], ellipsoid_class_answer)

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
            self.assertAlmostEqual(grs_80_test_latitudes[test_latitude], ellipsoid_class_answer)

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
        self.assertAlmostEqual(wgs_66_ellipsoid.semi_minor_axis(), 6356759.769488684)
        self.assertAlmostEqual(wgs_72_ellipsoid.semi_minor_axis(), 6356750.520016094)
        self.assertAlmostEqual(wgs_84_ellipsoid.semi_minor_axis(), 6356752.314245179)
        self.assertAlmostEqual(grs_80_ellipsoid.semi_minor_axis(), 6356752.314140355)

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


ecef_lla_test_examples = {
    LLA(64.0, 29.0): ECEF(2452068.321971885, 1359203.6656496185, 5709714.935455258),
    LLA(90, 0): ECEF(0.0, 0.0, 6356752.314245179),
    LLA(-14, 123): ECEF(-3371256.686406562, 5191280.055423112, -1532981.8295057514),
    LLA(45, -176.0): ECEF(-4506586.254521379, -315131.20952712646, 4487348.40886592),
    LLA(45.67890, 144.323043, 8000): ECEF(-3630666.472126026, 2606685.03830834, 4546107.080585875),
}


class LLATestCase(unittest.TestCase):

    def test_to_ecef(self):
        """
        Test to ensure proper conversion between LLA and ECEF
        """
        for lla in ecef_lla_test_examples:
            ecef = lla.to_ecef()
            accepted_ecef = ecef_lla_test_examples[lla]
            self.assertAlmostEqual(ecef.x, accepted_ecef.x)
            self.assertAlmostEqual(ecef.y, accepted_ecef.y)
            self.assertAlmostEqual(ecef.z, accepted_ecef.z)
            self.assertEqual(ecef.ellipsoid(), accepted_ecef.ellipsoid())


class ECEFTestCase(unittest.TestCase):

    def test_to_lla(self):
        """
        Test to ensure estimated ECEF from LLA is within tolerance.
        """
        for accepted_lla in ecef_lla_test_examples:
            lla = ecef_lla_test_examples[accepted_lla].to_lla()
            self.assertAlmostEqual(lla.latitude, accepted_lla.latitude)
            self.assertAlmostEqual(lla.longitude, accepted_lla.longitude)
            self.assertAlmostEqual(lla.altitude, accepted_lla.altitude)
            self.assertEqual(lla.ellipsoid(), accepted_lla.ellipsoid())


class TolueneBaseCLibraryTestCase(unittest.TestCase):

    def test_toluene_base_library_found(self):
        """
        Test to ensure Toluene Base C Library was built and found.
        """
        try:
            from toluene.base.base_c_library import base_c_library
            self.assertTrue(base_c_library.found_library())
        except ImportError:
            self.assertFalse(True)


if __name__ == '__main__':
    unittest.main()
