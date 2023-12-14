import pytest

from toluene.coordinates.reference_frame import ReferenceFrame
from toluene.coordinates.state_vector import StateVector
from toluene.models.earth.model import EarthModel

geodetic_test_points = [
    StateVector(31.2304, 121.4737, 4, frame=ReferenceFrame.GeodeticReferenceFrame),  # Shanghai, China
    StateVector(37.7749, -122.4194, 16, frame=ReferenceFrame.GeodeticReferenceFrame),  # San Francisco, USA
    StateVector(23.1291, 113.2644, 6, frame=ReferenceFrame.GeodeticReferenceFrame),  # Guangzhou, China
    StateVector(40.7128, -74.0060, 10, frame=ReferenceFrame.GeodeticReferenceFrame),  # New York City, USA
    StateVector(35.6895, 139.6917, 40, frame=ReferenceFrame.GeodeticReferenceFrame),  # Tokyo, Japan
    StateVector(12.9716, 77.5946, 920, frame=ReferenceFrame.GeodeticReferenceFrame),  # Bangalore, India
    StateVector(31.5497, 74.3436, 217, frame=ReferenceFrame.GeodeticReferenceFrame),  # Lahore, Pakistan
    StateVector(41.8781, -87.6298, 182, frame=ReferenceFrame.GeodeticReferenceFrame),  # Chicago, USA
    StateVector(37.5665, 126.9780, 23, frame=ReferenceFrame.GeodeticReferenceFrame),  # Seoul, South Korea
    StateVector(28.6139, 77.2090, 216, frame=ReferenceFrame.GeodeticReferenceFrame),  # Delhi, India
    StateVector(41.4057, 2.1684, 12, frame=ReferenceFrame.GeodeticReferenceFrame),  # Barcelona, Spain
    StateVector(34.0522, -118.2437, 71, frame=ReferenceFrame.GeodeticReferenceFrame),  # Los Angeles, USA
    StateVector(23.6345, -102.5528, 1, frame=ReferenceFrame.GeodeticReferenceFrame),  # León, Mexico
    StateVector(51.5074, -0.1278, 35, frame=ReferenceFrame.GeodeticReferenceFrame),  # London, UK
    StateVector(25.2769, 51.5200, 7, frame=ReferenceFrame.GeodeticReferenceFrame),  # Doha, Qatar
    StateVector(13.7563, 100.5018, 2, frame=ReferenceFrame.GeodeticReferenceFrame),  # Bangkok, Thailand
    StateVector(40.4168, -3.7038, 667, frame=ReferenceFrame.GeodeticReferenceFrame),  # Madrid, Spain
    StateVector(23.8103, 90.4125, 4, frame=ReferenceFrame.GeodeticReferenceFrame),  # Dhaka, Bangladesh
    StateVector(41.9028, 12.4964, 21, frame=ReferenceFrame.GeodeticReferenceFrame),  # Rome, Italy
    StateVector(22.3964, 114.1095, 5, frame=ReferenceFrame.GeodeticReferenceFrame),  # Hong Kong, SAR China
    StateVector(14.5995, 120.9842, 16, frame=ReferenceFrame.GeodeticReferenceFrame),  # Manila, Philippines
    StateVector(51.1657, 10.4515, 113, frame=ReferenceFrame.GeodeticReferenceFrame),  # Berlin, Germany
    StateVector(55.7558, 37.6176, 155, frame=ReferenceFrame.GeodeticReferenceFrame),  # Moscow, Russia
    StateVector(48.8566, 2.3522, 35, frame=ReferenceFrame.GeodeticReferenceFrame),  # Paris, France
    StateVector(31.5497, 120.3107, 8, frame=ReferenceFrame.GeodeticReferenceFrame),  # Suzhou, China
    StateVector(55.9533, -3.1883, 47, frame=ReferenceFrame.GeodeticReferenceFrame),  # Edinburgh, UK
    StateVector(12.9714, 77.5946, 920, frame=ReferenceFrame.GeodeticReferenceFrame),  # Bengaluru, India
    StateVector(45.4215, -75.6993, 70, frame=ReferenceFrame.GeodeticReferenceFrame),  # Ottawa, Canada
    StateVector(32.7157, -117.1611, 20, frame=ReferenceFrame.GeodeticReferenceFrame),  # San Diego, USA
    StateVector(35.9686, -99.9018, 518, frame=ReferenceFrame.GeodeticReferenceFrame),  # Abilene, USA
    StateVector(42.3601, -71.0589, 43, frame=ReferenceFrame.GeodeticReferenceFrame),  # Boston, USA
]

itrf_test_points = [
    StateVector(-2850075.294343253, 4655695.796924158, 3287765.2299773037, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Shanghai, China
    StateVector(-2706181.6267071473, -4261070.165026739, 3885735.290996956, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # San Francisco, USA
    StateVector(-2317914.5038877944, 5391355.734786994, 2489875.9948529475, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Guangzhou, China
    StateVector(1334000.5446860846, -4654052.12920688, 4138306.7613726556, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # New York City, USA
    StateVector(-3954869.0631549605, 3354957.9490679787, 3700288.1237203646, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Tokyo, Japan
    StateVector(1335650.794194833, 6072159.400646209, 1422550.1687721466, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Bangalore, India
    StateVector(1468225.3552191756, 5238677.546124002, 3318097.9233186147, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Lahore, Pakistan
    StateVector(196696.71042015063, -4752114.554500811, 4235653.749994335, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Chicago, USA
    StateVector(-3044790.9362076046, 4043803.6753922133, 3867430.9995572227, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Seoul, South Korea
    StateVector(1240621.3815494243, 5464588.4359043995, 3036507.895895077, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Delhi, India
    StateVector(4787489.766778159, 181272.54855436122, 4196330.517202016, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Barcelona, Spain
    StateVector(-2503385.149590922, -4660255.25606549, 3551285.115595811, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Los Angeles, USA
    StateVector(-1270629.2090853488, -5706546.240194934, 2541250.16991839, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # León, Mexico
    StateVector(3978016.0582352793, -8873.10199269117, 4968902.331366097, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # London, UK
    StateVector(3590949.0583496066, 4517677.680034685, 2706845.7276584287, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Doha, Qatar
    StateVector(-1129388.4875065808, 6092566.778459311, 1506806.8879319832, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Bangkok, Thailand
    StateVector(4853180.057032302, -314164.29855723435, 4113762.729820025, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Madrid, Spain
    StateVector(-42033.56664680122, 5838313.591165916, 2559076.8097958085, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Dhaka, Bangladesh
    StateVector(4641602.244591051, 1028712.4738301872, 4237588.573991904, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Rome, Italy
    StateVector(-2410004.3232879606, 5385236.433369752, 2415055.9083167086, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Hong Kong, SAR China
    StateVector(-3178140.633423443, 5292619.680537387, 1597260.3694744613, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Manila, Philippines
    StateVector(3941265.3098854595, 727020.0670461959, 4945212.867743558, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Berlin, Germany
    StateVector(2849538.5273332233, 2195834.844004245, 5249318.407671388, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Moscow, Russia
    StateVector(4200937.804351269, 172560.72143258236, 4780107.699250289, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Paris, France
    StateVector(-2745687.8140138057, 4696667.817791838, 3317988.5665823524, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Suzhou, China
    StateVector(3573638.2482950804, -199065.37437001418, 5261571.8192865485, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Edinburgh, UK
    StateVector(1335651.8613013579, 6072164.251944874, 1422528.6042099572, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Bengaluru, India
    StateVector(1107694.2065800726, -4345435.687023454, 4520399.686263348, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Ottawa, Canada
    StateVector(-2452102.125403971, -4779253.208882448, 3427484.159740133, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # San Diego, USA
    StateVector(-888769.5164506885, -5091475.299490853, 3725676.658934424, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Abilene, USA
    StateVector(1532149.2696453626, -4464588.77258648, 4275273.369866783, frame=ReferenceFrame.InternationalTerrestrialReferenceFrame),  # Boston, USA
]




class TestStateVectorTransform:
    def test_itrf_to_geodetic(self):
        earth_model = EarthModel()
        for idx in range(len(geodetic_test_points)):
            print("Trying to convert from ITRF to Geodetic for", geodetic_test_points[idx])
            geodetic_point = itrf_test_points[idx].get_geodetic(earth_model)
            assert geodetic_point is not None
            print(geodetic_point.position, geodetic_test_points[idx].position)
            assert geodetic_point.position[0] == pytest.approx(geodetic_test_points[idx].position[0], abs=1e-8)
            assert geodetic_point.position[1] == pytest.approx(geodetic_test_points[idx].position[1], abs=1e-8)
            assert geodetic_point.position[2] == pytest.approx(geodetic_test_points[idx].position[2], abs=1e-8)
    def test_geodetic_to_itrf(self):
        earth_model = EarthModel()
        for idx in range(len(itrf_test_points)):
            print("Trying to convert from Geodetic to ITRF for", itrf_test_points[idx])
            itrf_point = geodetic_test_points[idx].get_itrs(earth_model)
            assert itrf_point is not None
            print(itrf_point.position, itrf_test_points[idx].position)
            assert itrf_point.position[0] == pytest.approx(itrf_test_points[idx].position[0], abs=1e-8)
            assert itrf_point.position[1] == pytest.approx(itrf_test_points[idx].position[1], abs=1e-8)
            assert itrf_point.position[2] == pytest.approx(itrf_test_points[idx].position[2], abs=1e-8)