from datetime import datetime, timezone
import pytest

from toluene.models.earth.coordinates import use_default_model, Lla, Ecef, Eci

use_default_model()

lla_test_points = [
    Lla(31.2304, 121.4737, 4, time=datetime(2000, 11, 20, 00, 15,00, 00, timezone.utc).timestamp()),    # Shanghai, China
    Lla(37.7749, -122.4194, 16, time=datetime(2001, 6, 20, 1, 15, 45, 00, timezone.utc).timestamp()),  # San Francisco, USA
    Lla(23.1291, 113.2644, 6, time=datetime(2011, 7, 14, 10, 15, 47, 400000, timezone.utc).timestamp()),    # Guangzhou, China
    Lla(40.7128, -74.0060, 10, time=datetime(2021, 7, 10, 10, 15, 47, 0, timezone.utc).timestamp()),   # New York City, USA
    Lla(35.6895, 139.6917, 40, time=datetime(2009, 6, 20, 10, 15, 45, 00, timezone.utc).timestamp()),   # Tokyo, Japan
    Lla(12.9716, 77.5946, 920, time=datetime(2019, 6, 23, 10, 15, 15, 5000, timezone.utc).timestamp()),   # Bangalore, India
    Lla(31.5497, 74.3436, 217, time=datetime(2013, 7, 15, 14, 43, 16, 50050, timezone.utc).timestamp()),   # Lahore, Pakistan
    Lla(41.8781, -87.6298, 182, time=datetime(2016, 4, 17, 14, 36, 16, 0, timezone.utc).timestamp()),  # Chicago, USA
    Lla(37.5665, 126.9780, 23, time=datetime(2000, 1, 1, 12, 0, 0, 0, timezone.utc).timestamp()),   # Seoul, South Korea
    Lla(28.6139, 77.2090, 216, time=datetime(2003, 3, 6, 23, 59, 59, 0, timezone.utc).timestamp()),   # Delhi, India
    Lla(41.4057, 2.1684, 12),     # Barcelona, Spain
    Lla(34.0522, -118.2437, 71),  # Los Angeles, USA
    Lla(23.6345, -102.5528, 1),   # León, Mexico
    Lla(51.5074, -0.1278, 35),    # London, UK
    Lla(25.2769, 51.5200, 7),     # Doha, Qatar
    Lla(13.7563, 100.5018, 2),    # Bangkok, Thailand
    Lla(40.4168, -3.7038, 667),   # Madrid, Spain
    Lla(23.8103, 90.4125, 4),     # Dhaka, Bangladesh
    Lla(41.9028, 12.4964, 21),    # Rome, Italy
    Lla(22.3964, 114.1095, 5),    # Hong Kong, SAR China
    Lla(14.5995, 120.9842, 16),   # Manila, Philippines
    Lla(51.1657, 10.4515, 113),   # Berlin, Germany
    Lla(55.7558, 37.6176, 155),   # Moscow, Russia
    Lla(48.8566, 2.3522, 35),     # Paris, France
    Lla(31.5497, 120.3107, 8),    # Suzhou, China
    Lla(55.9533, -3.1883, 47),    # Edinburgh, UK
    Lla(12.9714, 77.5946, 920),   # Bengaluru, India
    Lla(45.4215, -75.6993, 70),   # Ottawa, Canada
    Lla(32.7157, -117.1611, 20),  # San Diego, USA
    Lla(35.9686, -99.9018, 518),  # Abilene, USA
    Lla(42.3601, -71.0589, 43),   # Boston, USA
]

ecef_test_points = [
    Ecef(-2850075.294343253, 4655695.796924158, 3287765.2299773037, time=datetime(2000, 11, 20, 00, 15,00, 00, timezone.utc).timestamp()),  # Shanghai, China
    Ecef(-2706181.6267071473, -4261070.165026739, 3885735.290996956, time=datetime(2001, 6, 20, 1, 15, 45, 00, timezone.utc).timestamp()),  # San Francisco, USA
    Ecef(-2317914.5038877944, 5391355.734786994, 2489875.9948529475, time=datetime(2011, 7, 14, 10, 15, 47, 400000, timezone.utc).timestamp()),  # Guangzhou, China
    Ecef(1334000.5446860846, -4654052.12920688, 4138306.7613726556, time=datetime(2021, 7, 10, 10, 15, 47, 0, timezone.utc).timestamp()),
    Ecef(-3954869.0631549605, 3354957.9490679787, 3700288.1237203646, time=datetime(2009, 6, 20, 10, 15, 45, 00, timezone.utc).timestamp()),
    Ecef(1335650.794194833, 6072159.400646209, 1422550.1687721466, time=datetime(2019, 6, 23, 10, 15, 15, 5000, timezone.utc).timestamp()),
    Ecef(1468225.3552191756, 5238677.546124002, 3318097.9233186147, time=datetime(2013, 7, 15, 14, 43, 16, 50050, timezone.utc).timestamp()),
    Ecef(196696.71042015063, -4752114.554500811, 4235653.749994335, time=datetime(2016, 4, 17, 14, 36, 16, 0, timezone.utc).timestamp()),
    Ecef(-3044790.9362076046, 4043803.6753922133, 3867430.9995572227, time=datetime(2000, 1, 1, 12, 0, 0, 0, timezone.utc).timestamp()),
    Ecef(1240621.3815494243, 5464588.4359043995, 3036507.895895077, time=datetime(2003, 3, 6, 23, 59, 59, 0, timezone.utc).timestamp()),
    Ecef(4787489.766778159, 181272.54855436122, 4196330.517202016),
    Ecef(-2503385.149590922, -4660255.25606549, 3551285.115595811),
    Ecef(-1270629.2090853488, -5706546.240194934, 2541250.16991839),
    Ecef(3978016.0582352793, -8873.10199269117, 4968902.331366097),
    Ecef(3590949.0583496066, 4517677.680034685, 2706845.7276584287),
    Ecef(-1129388.4875065808, 6092566.778459311, 1506806.8879319832),
    Ecef(4853180.057032302, -314164.29855723435, 4113762.729820025),
    Ecef(-42033.56664680122, 5838313.591165916, 2559076.8097958085),
    Ecef(4641602.244591051, 1028712.4738301872, 4237588.573991904),
    Ecef(-2410004.3232879606, 5385236.433369752, 2415055.9083167086),
    Ecef(-3178140.633423443, 5292619.680537387, 1597260.3694744613),
    Ecef(3941265.3098854595, 727020.0670461959, 4945212.867743558),
    Ecef(2849538.5273332233, 2195834.844004245, 5249318.407671388),
    Ecef(4200937.804351269, 172560.72143258236, 4780107.699250289),
    Ecef(-2745687.8140138057, 4696667.817791838, 3317988.5665823524),
    Ecef(3573638.2482950804, -199065.37437001418, 5261571.8192865485),
    Ecef(1335651.8613013579, 6072164.251944874, 1422528.6042099572),
    Ecef(1107694.2065800726, -4345435.687023454, 4520399.686263348),
    Ecef(-2452102.125403971, -4779253.208882448, 3427484.159740133),
    Ecef(-888769.5164506885, -5091475.299490853, 3725676.658934424),
    Ecef(1532149.2696453626, -4464588.77258648, 4275273.369866783),
]

eci_test_points = [
    Eci(-5441745.237752063, -429522.98068704456, 3287970.7565327426, time=datetime(2000, 11, 20, 00, 15,00, 00, timezone.utc).timestamp()),
    Eci(-4870732.943097642, 1323304.3697514043, 3886373.8944033403, time=datetime(2001, 6, 20, 1, 15, 45, 00, timezone.utc).timestamp()),
    Eci(-5544955.70243912, -1913200.0016413413, 2496411.2094237907, time=datetime(2011, 7, 14, 10, 15, 47, 400000, timezone.utc).timestamp()),
    Eci(4800401.841857878, 690944.7402699982, 4128448.252781357, time=datetime(2021, 7, 10, 10, 15, 47, 0, timezone.utc).timestamp()),
    Eci(-4794910.300098899, -1967325.0054090254, 3705042.415716683, time=datetime(2009, 6, 20, 10, 15, 45, 00, timezone.utc).timestamp()),
    Eci(-4924750.122931905, 3791408.596940791, 1432047.380128748, time=datetime(2019, 6, 23, 10, 15, 15, 5000, timezone.utc).timestamp()),
    Eci(-3592604.666450538, -4081707.4185223705, 3322961.7076838976, time=datetime(2013, 7, 15, 14, 43, 16, 50050, timezone.utc).timestamp()),
    Eci(4396365.506363477, -1830889.0483034858, 4228693.660743402, time=datetime(2016, 4, 17, 14, 36, 16, 0, timezone.utc).timestamp()),
    Eci(3425543.334677969, 3726262.2212037724, 3867912.2621600437, time=datetime(2000, 1, 1, 12, 0, 0, 0, timezone.utc).timestamp()),
    Eci(-2676783.7345566824, -4922647.07416093, 3037042.3971161866, time=datetime(2003, 3, 6, 23, 59, 59, 0, timezone.utc).timestamp()),
]

class TestCoordinates:

    def test_eci_to_ecef(self):

        for index in range(len(eci_test_points)):
            ecef_test = eci_test_points[index].ecef
            assert ((ecef_test.x - ecef_test_points[index].x)**2 + (ecef_test.y - ecef_test_points[index].y)**2 + (ecef_test.z - ecef_test_points[index].z)**2)**0.5 == pytest.approx(0, abs=5)

    def test_eci_to_lla(self):

        for index in range(len(eci_test_points)):
            lla_test = eci_test_points[index].lla
            assert lla_test.latitude == pytest.approx(lla_test_points[index].latitude, abs=0.1)
            assert lla_test.longitude == pytest.approx(lla_test_points[index].longitude, abs=0.1)
            assert lla_test.altitude == pytest.approx(lla_test_points[index].altitude, abs=5)

    def test_ecef_to_eci(self):

        for index in range(len(eci_test_points)):
            eci_test = ecef_test_points[index].eci
            assert ((eci_test.x - eci_test_points[index].x)**2 + (eci_test.y - eci_test_points[index].y)**2 + (eci_test.z - eci_test_points[index].z)**2)**0.5 == pytest.approx(0, abs=5)

    def test_ecef_to_lla(self):

            for index in range(len(ecef_test_points)):
                lla_test = ecef_test_points[index].lla
                assert lla_test.latitude == pytest.approx(lla_test_points[index].latitude, abs=1e-3)
                assert lla_test.longitude == pytest.approx(lla_test_points[index].longitude, abs=1e-3)
                assert lla_test.altitude == pytest.approx(lla_test_points[index].altitude, abs=5)

    def test_lla_to_eci(self):

        for index in range(len(eci_test_points)):
            eci_test = lla_test_points[index].eci
            print("Displacement LLA to ECI: ", ((eci_test.x - eci_test_points[index].x)**2 + (eci_test.y - eci_test_points[index].y)**2 + (eci_test.z - eci_test_points[index].z)**2)**0.5)
            assert ((eci_test.x - eci_test_points[index].x)**2 + (eci_test.y - eci_test_points[index].y)**2 + (eci_test.z - eci_test_points[index].z)**2)**0.5 == pytest.approx(0, abs=5)

    def test_lla_to_ecef(self):

        for index in range(len(lla_test_points)):
            ecef_test = lla_test_points[index].ecef
            assert ((ecef_test.x - ecef_test_points[index].x)**2 + (ecef_test.y - ecef_test_points[index].y)**2 + (ecef_test.z - ecef_test_points[index].z)**2)**0.5 == pytest.approx(0, abs=1)
