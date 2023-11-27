from __future__ import annotations

import logging

from toluene.core.ellipsoid import Ellipsoid, wgs_84_ellipsoid
from toluene.core.time import TerrestrialTimeJ2000
from toluene_extensions.core_extensions import eci_from_ecef, ecef_from_eci, ecef_from_lla, lla_from_ecef

logger = logging.getLogger('toluene.core.coordinates')


class Coordinates:
    def __init__(self):
        pass


class ECEF(Coordinates):
    """
    Defines a ECEF (Earth Centered Earth Fixed) vector. The ECEF vector is defined by the x, y, z coordinates in meters.
    Along with the ellipsoid if applicable which is used for conversion to :class:`LLA` coordinates and time which is
    used for conversion to :class:`ECI` coordinates.

    :param x: The x coordinate in meters.
    :type x: float
    :param y: The y coordinate in meters.
    :type y: float
    :param z: The z coordinate in meters.
    :type z: float
    :param ellipsoid: The ellipsoid the coordinates are in. Defaults to the WGS84 ellipsoid.
    :type ellipsoid: Ellipsoid, optional
    :param time: The time the coordinates are in. Defaults to time of ECEF vector initialization.
    :type time: TerrestrialTimeJ2000, optional
    """

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0,
                 ellipsoid: Ellipsoid = wgs_84_ellipsoid, time: TerrestrialTimeJ2000 = TerrestrialTimeJ2000()):
        logger.debug(f'Initializing ECEF({x}, {y}, {z}, {ellipsoid}, {time})')

        self.x = x
        self.y = y
        self.z = z
        self.__ellipsoid = ellipsoid
        self.__time = time

    def __sub__(self, other) -> float:
        """
        This documentation won't show up, but, it is used to get the distance between two :class:`ECEF` vectors. Always
        will be a positive number. The distance is calculated using the Pythagorean theorem. The distance is in meters.

        :param other: The other coordinate, can be ECEF, ECI, or LLA.
        :return: The distance between the two coordinates in meters.
        :rtype: float
        """
        if isinstance(other, ECEF):
            return ((self.x - other.x) ** 2 + (self.y - other.y) ** 2 + (self.z - other.z) ** 2) ** 0.5
        elif isinstance(other, (ECI, LLA)):
            other = other.to_ecef()
            return ((self.x - other.x) ** 2 + (self.y - other.y) ** 2 + (self.z - other.z) ** 2) ** 0.5
        else:
            raise TypeError(f'Cannot subtract {other} from {self}')

    def __str__(self) -> str:
        """
        :return: A string representation of the :class:`ECEF` coordinates.
        :rtype: str
        """
        return f'({self.x}, {self.y}, {self.z})'

    def ellipsoid(self) -> core.ellipsoid.Ellipsoid:
        """
        Getter for the ellipsoid in the :class:`ECEF` class. The ellipsoid is used for conversion to :class:`LLA` AKA
        geodetic coordinates. Otherwise, it is not used by ECEF directly.

        :return: The ellipsoid object in the :class:`ECEF` class.
        :rtype: :class:`core.ellipsoid.Ellipsoid`
        """
        logger.debug(f'Entering ECEF.ellipsoid()')
        return self.__ellipsoid

    def time(self) -> core.time.TerrestrialTimeJ2000:
        """
        Getter for the time in the :class:`ECEF` class. The time is used for conversion to :class:`ECI` AKA Earth

        :return: The :class:`core.time.TerrestrialTimeJ2000` object in the :class:`ECEF` class.
        :rtype: :class:`core.time.TerrestrialTimeJ2000`
        """
        logger.debug(f'Entering ECEF.time()')
        return self.__time

    def magnitude(self) -> float:
        """
        The magnitude of the :class:`ECEF` vector. Simple Pythagorean theorem measurement of the displacement from the
        origin.

        :return: The magnitude of the :class:`ECEF` vector.
        :rtype: float
        """
        logger.debug(f'Entering ECEF.magnitude()')
        return (self.x ** 2 + self.y ** 2 + self.z ** 2) ** 0.5

    def to_eci(self) -> ECI:
        """
        :class:`ECI` is incredibly useful when talking about orbiting bodies. Geostationary orbits defined in
        :class:`ECI` coordinates have velocity and acceleration while :class:`ECEF` coordinates appear unmoving. This is
        because the Earth is moving in relation to the Sun, moon, other planets, and the stars. The conversion is done
        using the time since J2000.0 which is definied as exactly 11:58:55.816 UTC on January 1, 2000. Accounting for
        leap seconds the complication of time is handled in :class:`core.time.TerrestrialTimeJ2000`. The conversion is
        done in 5 separate rotations. A simple quick not complete explanation of the formula.\n
        :math:`x_{crs} = BPNTWx_{trs}`\n
        :math:`W` being the rotation matrix from polar motion or the force applied by the solar system's motion through
        the space.\n
        :math:`T` being the rotation matrix from the Earth's rotation.\n
        :math:`N` being the rotation matrix from the nutation of the Earth's axis. This is caused by gravitational
        forces applied to the bulge of the Earth at the equator.\n
        :math:`P` being the rotation matrix from the precession of the Earth's axis. This is caused by the Earth's axis
        wobbling like a top.\n
        :math:`B` being the rotation matrix from the bias of the Earth's axis. This is caused by the Earth's axis
        swaying from J2000.0 in an observalbe way.\n

        More information and where the formulas were taken from can be found in Chapter 6 "Precession, Nutation, Polar
        Motion, and Earth Rotation" Explanatory Supplement to the Astronomical Almanac.


        :return: The equivalent :class:`ECI` vector with relation to x, y, z, and time displacement from J2000.0.
        :rtype: :class:`ECI`
        """
        x, y, z = eci_from_ecef(self.x, self.y, self.z, self.__time.seconds_since())
        return ECI(x, y, z, self.__ellipsoid, self.__time)

    def to_ecef(self) -> LLA:
        """
        This just returns self so you don't have a bug if you try to get ECEF from ECEF. It's not a bug, it's a feature.
        :return: self
        :rtype: ECEF
        """
        return self

    def to_lla(self) -> LLA:
        """
        Converts the :class:`ECEF` vector into the approximate :class:`LLA` coordinates. The conversion is done using
        the ellipsoid and must be set appropriately to get the correct coordinates. The conversion is done using
        Ferrari's solution to the inverse geodetic problem
        `<https://en.wikipedia.org/wiki/Geographic_coordinate_conversion>`_. Being computation heavy in toluene all this
        is done in a C extension. These series of equations are used to convert the :class:`ECEF` vector to the
        :class:`LLA` coordinates.

        :math:`a=` semi-major axis \n
        :math:`b=` semi-minor axis \n
        :math:`e^2 = \\frac{a^2-b^2}{a^2}` \n
        :math:`e^{r2} = \\frac{a^2-b^2}{b^2}` \n
        :math:`p = \\sqrt{x^2+y^2}` \n
        :math:`F = 54b^2z^2` \n
        :math:`G = p^2+(1-e^2)z^2-e^2(a^2-b^2)` \n
        :math:`c = \\frac{e^4Fp^2}{G^3}` \n
        :math:`s = (1+c+\\sqrt{c^2+2c})^\\frac{1}{3}` \n
        :math:`k = 1-s+\\frac{1}{s}` \n
        :math:`P = \\frac{F}{3k^2G^2}` \n
        :math:`Q = \\sqrt{1+2e^4P}` \n
        :math:`r_0 = -\\frac{Pep}{1+Q}+`
        :math:`\\sqrt{\\frac{1}{2}a^2(1+\\frac{1}{Q})-\\frac{P(1-e^2)Z^2}{Q(1+Q)}-\\frac{1}{2}Pp^2}` \n
        :math:`U = \\sqrt{(p-e^2r_0)^2+z^2}` \n
        :math:`V = \\sqrt{(p-e^2r_0)^2+(1-e^2)z^2}` \n
        :math:`z_0 = \\frac{b^2z}{aV}` \n
        :math:`h = U(1-\\frac{b^2}{aV}) =`  altitude over the ellipsoid \n
        :math:`\\phi = atan(\\frac{z+e^{r2}z_0}{p}) =` Latitude\n
        :math:`\\lambda = atan2[y,x] =` Longitude\n

        """
        logger.debug(f'Entering ECEF.to_lla()')

        # Faster in C than Python
        latitude, longitude, altitude = lla_from_ecef(self.__ellipsoid.semi_major_axis(),
                                                      self.__ellipsoid.semi_minor_axis(), self.x, self.y, self.z)
        return LLA(latitude, longitude, altitude, self.__ellipsoid, self.__time)


class ECI(Coordinates):
    """
    Defines a ECI (Earth Centered Inertial) vector. The ECI vector is defined by the x, y, z coordinates in meters and
    time in seconds since J2000.0 using TT. The time is used for conversion to :class:`ECEF` coordinates. The ellipsoid
    is not used by ECI directly but used for conversion to :class:`LLA` coordinates. ECI is incredibly useful when
    talking about orbiting bodies as geostationary orbits defined in ECI coordinates still have velocity and
    acceleration however appear unmoving in ECEF coordinates.

    :param x: The x coordinate in meters.
    :type x: float
    :param y: The y coordinate in meters.
    :type y: float
    :param z: The z coordinate in meters.
    :type z: float
    :param ellipsoid: The ellipsoid the coordinates are in. Defaults to the WGS84 ellipsoid.
    :type ellipsoid: Ellipsoid, optional
    :param time: The time the coordinates are in. Defaults to time of ECI vector initialization.
    :type time: TerrestrialTimeJ2000, optional
    """

    def __init__(self, x, y, z,
                 ellipsoid: Ellipsoid = wgs_84_ellipsoid, time: TerrestrialTimeJ2000 = TerrestrialTimeJ2000()):
        self.x = x
        self.y = y
        self.z = z
        self.__ellipsoid = ellipsoid
        self.__time = time

    def __sub__(self, other) -> float:
        """
        This documentation won't show up, but, it is used to get the distance between two :class:`ECI` vectors. Always
        will be a positive number. The distance is calculated by first computing the ECEF vector and then using the
        Pythagorean theorem. The distance is in meters.

        :param other: The other coordinate, can be ECEF, ECI, or LLA.
        :return: The distance between the two coordinates in meters.
        :rtype: float
        """
        return self.to_ecef().__sub__(other)

    def __str__(self) -> str:
        """
        :return: A string representation of the :class:`ECI` coordinates.
        :rtype: str
        """
        return f'({self.x}, {self.y}, {self.z})'

    def ellipsoid(self) -> core.ellipsoid.Ellipsoid:
        """
        The ellipsoid the coordinates are in. Not used directly with ECI but is important if converting to :class:`LLA`
        coordinates. Defaults to the WGS84 ellipsoid, however others can be used if needed.
        :class:`core.ellipsoid.Ellipsoid` has more info on how to create a custom ellipsoid.

        :return: The ellipsoid in the :class:`ECI` class.
        """
        logger.debug(f'Entering ECI.ellipsoid()')
        return self.__ellipsoid

    def time(self) -> core.time.TerrestrialTimeJ2000:
        """
        The time the coordinates are in. The time is used for conversion to :class:`ECEF` coordinates. Defaults to time
        of :class:`ECI` vector initialization. This NEEDS to be set appropriately using the UTC timezone. If using
        :class:`ECI` you will probably need to convert to Earth Centered Earth Fixed coordinates at some point or
        geodetic coordinates to make sense as an observer on ground.

        :return: The time in the :class:`ECI` class.
        :rtype: :class:`core.time.TerrestrialTimeJ2000`
        """
        logger.debug(f'Entering ECI.time()')
        return self.__time

    def magnitude(self) -> float:
        """
        The magnitude of the :class:`ECI` vector. Simple Pythagorean theorem measurement of the displacement from the
        origin.

        :return: The magnitude of the :class:`ECI` vector.
        :rtype: float
        """
        logger.debug(f'Entering ECI.magnitude()')
        return (self.x ** 2 + self.y ** 2 + self.z ** 2) ** 0.5

    def to_eci(self) -> LLA:
        """
        This just returns self so you don't have a bug if you try to get ECI from ECI. It's not a bug, it's a feature.
        :return: self
        :rtype: ECI
        """
        return self

    def to_ecef(self) -> ECEF:
        """
        Converts the :class:`ECI` vector into the equivalent :class:`ECEF` coordinates. The conversion is done using
        the inverse of the rotation matricies used to convert from :class:`ECEF` to :class:`ECI`. The conversion is
        done in 5 separate rotations just the same however we use the transpose as they're orthaogonal matricies.
        Introduces some rounding error but is in the scale of micrometers so is negligible.

        :return: The equivalent :class:`ECEF` vector with relation to x, y, z, and time displacement from J2000.0.
        :rtype: :class:`ECEF`
        """
        x, y, z = ecef_from_eci(self.x, self.y, self.z, self.__time.seconds_since())
        return ECEF(x, y, z, self.__ellipsoid, self.__time)

    def to_lla(self) -> LLA:
        """
        Converts the :class:`ECI` vector into the equivalent :class:`LLA` coordinates. The conversion is done using
        the :class:`ECEF` as an intermediate step. Ensure that both time and ellipsoid are set appropriately to get the
        correct coordinates.

        :return: The equivalent :class:`LLA` vector with relation to latitude, longitude, altitude, and time
            displacement from J2000.0.
        :rtype: :class:`LLA`
        """
        return self.to_ecef().to_lla()


class LLA(Coordinates):
    """
    Defines geodetic coordinates, I.E. Latitude, Longitude, Altitude over an ellipsoid. These coordinates are defined
    by the ellipsoid they are on meaning unlike :class:`ECEF` coordinates if you want to convert to any other coordinate
    frame, it is suggested that you ensure the ellipsoid is the proper one defaulting to the WGS84 ellipsoid. The time
    is also used when converting to :class:`ECI` coordinates.

    :param latitude: The latitude of the point on the ellipsoid in degrees. Must be between -90 and 90 degrees. -90 is
        the South Pole and 90 is the North Pole and is just equal to the semi-minor axis. 0 is the equator and is equal
        to the semi-major axis. Represented in math as :math:`\\phi`.
    :type latitude: float
    :param longitude: The longitude of the point on the ellipsoid in degrees. Must be between -180 and 180 degrees. -180
        is the International Date Line and 180 is the Prime Meridian. Represented in math as :math:`\\lambda`.
    :type longitude: float
    :param altitude: The altitude of the point over the ellipsoid in meters. Represented in math as :math:`h`. Defaults
        to 0 meters.
    :type altitude: float, optional
    :param ellipsoid: The ellipsoid the coordinates are on. Defaults to the WGS84 ellipsoid.
    :type ellipsoid: :class:`core.ellipsoid.Ellipsoid`, optional
    :param time: The time the coordinates are in. Defaults to time of LLA coordinates initialization.
    :type time: :class:`core.time.TerrestrialTimeJ2000`, optional
    """

    def __init__(self, latitude: float = None, longitude: float = None, altitude: float = 0.0,
                 ellipsoid: Ellipsoid = wgs_84_ellipsoid, time: TerrestrialTimeJ2000 = TerrestrialTimeJ2000()):
        logger.debug(f'Initializing LLA({latitude}, {longitude}, {altitude}, {ellipsoid}, {time})')

        self.latitude = latitude
        self.longitude = longitude
        self.altitude = altitude
        self.__ellipsoid = ellipsoid
        self.__time = time

    def __sub__(self, other) -> float:
        """
        This documentation won't show up, but, it is used to get the distance between two :class:`LLA` vectors. Always
        will be a positive number. The distance is calculated by first computing the ECEF vector and then using the
        Pythagorean theorem. The distance is in meters.

        :param other: The other coordinate, can be ECEF, ECI, or LLA.
        :return: The distance between the two coordinates in meters.
        :rtype: float
        """
        return self.to_ecef().__sub__(other)

    def __str__(self) -> str:
        """
        :return: A string representation of the :class:`LLA` coordinates.
        :rtype: str
        """
        return f'({self.latitude}, {self.longitude}, {self.altitude})'

    def ellipsoid(self) -> core.ellipsoid.Ellipsoid:
        """
        Getter for the ellipsoid in the :class:`LLA` class. The ellipsoid is used for conversion to :class:`ECEF` AKA
        Earth Centered Earth Fixed coordinates. It could also be used to measure the distance from one point to another
        in meters. It defines the shape of the Earth and how the radius changes with respect to latitude.

        :return: The ellipsoid object in the :class:`LLA` class.
        :rtype: Ellipsoid
        """
        logger.debug('Entering LLA.ellipsoid()')
        return self.__ellipsoid

    def time(self) -> core.time.TerrestrialTimeJ2000:
        """
        Getter for the time in the :class:`LLA` class. The time is used for conversion to :class:`ECI` AKA Earth
        Centered Inertial coordinates. It could be used to store time when the coordinates were taken.

        :return: The time object in the :class:`LLA` class.
        :rtype: :class:`core.time.TerrestrialTimeJ2000`
        """
        logger.debug('Entering LLA.time()')
        return self.__time

    def magnitude(self) -> float:
        """
        The magnitude of the :class:`LLA` vector. Measurement of the displacement from the center of the Earth. This
        is the same as the altitude over the ellipsoid plus the radius of the ellipsoid at the latitude of the point.
        Should be equal to the Pythagorean theorem of the corresponding :class:`ECEF` vector.

        :return: The magnitude of the :class:`LLA` vector.
        :rtype: float
        """
        logger.debug('Entering LLA.magnitude()')
        return self.altitude + self.__ellipsoid.ellipsoid_radius(self.latitude)

    def to_eci(self) -> ECI:
        """
        Converts the :class:`LLA` coordinates to the equivalent :class:`ECI` coordinates. The conversion is done using
        :class:`ECEF` as an intermediate step. Ensure that both time and ellipsoid are set appropriately to get the
        correct coordinates.

        :return: The equivalent :class:`ECI` vector with relation to x, y, z, and time displacement from J2000.0.
        :rtype: :class:`ECI`
        """
        logger.debug('Entering LLA.to_eci()')
        return self.to_ecef().to_eci()

    def to_ecef(self) -> ECEF:
        """
        Converts the Geodetic coordinates to the equivalent :class:`ECEF` coordinates. The conversion is done using the
        definied ellipsoid and must be set appropriately to get the correct coordinates. The conversion is much simpler
        than the other way around and could be computed with the equation. Still preformed in a C extension for speed.

        :math:`x = (N(\\phi)+h)cos(\\phi)cos(\\lambda)` \n
        :math:`y = (N(\\phi)+h)cos(\\phi)sin(\\lambda)` \n
        :math:`z = (N(\\phi)(1-e^2)+h)sin(\\phi)` \n
        Where :math:`N(\\phi) = \\frac{a}{\\sqrt{1-e^2sin^2(\\phi)}}` \n
        :math:`a=` semi-major axis \n
        :math:`b=` semi-minor axis \n
        :math:`e^2 = \\frac{a^2-b^2}{a^2}` \n
        """

        logger.debug('Entering LLA.to_ecef()')

        x, y, z = ecef_from_lla(self.__ellipsoid.semi_major_axis(), self.__ellipsoid.semi_minor_axis(),
                                self.latitude, self.longitude, self.altitude)
        return ECEF(x, y, z, self.__ellipsoid, self.__time)

    def to_lla(self) -> LLA:
        """
        This just returns self so you don't have a bug if you try to get LLA from LLA. It's not a bug, it's a feature.
        :return: self
        :rtype: LLA
        """
        return self
