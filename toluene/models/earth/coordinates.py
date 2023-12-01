# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#                                                                                   #
#   MIT License                                                                     #
#                                                                                   #
#   Copyright (c) 2023 Tri-Nitro                                                    #
#                                                                                   #
#   Permission is hereby granted, free of charge, to any person obtaining a copy    #
#   of this software and associated documentation files (the "Software"), to deal   #
#   in the Software without restriction, including without limitation the rights    #
#   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       #
#   copies of the Software, and to permit persons to whom the Software is           #
#   furnished to do so, subject to the following conditions:                        #
#                                                                                   #
#   The above copyright notice and this permission notice shall be included in all  #
#   copies or substantial portions of the Software.                                 #
#                                                                                   #
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      #
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        #
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     #
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          #
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   #
#   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   #
#   SOFTWARE.                                                                       #
#                                                                                   #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
from __future__ import annotations

from datetime import datetime, timezone
import yaml

from toluene.models.earth.cirs_coefficients import CIRSCoefficients
from toluene.models.earth.ellipsoid import Ellipsoid
from toluene.models.earth.geoid import Geoid
from toluene.util.file import configdir

from toluene_extensions.models.earth.coordinates import eci_to_ecef, ecef_to_eci, ecef_to_lla, lla_to_ecef

default_cirs_coefficients = None
default_ellipsoid = None
default_geoid = None
default_epoch = None


def use_defaults_cirs_parameters():
    global default_cirs_coefficients, default_ellipsoid, default_geoid, default_epoch
    yaml_config = None
    with open(configdir + '/config.yml') as f:
        yaml_config = yaml.safe_load(f)
    default_cirs_coefficients = CIRSCoefficients(yaml_config['cirs_coefficients'])
    default_ellipsoid = Ellipsoid(yaml_config['ellipsoid']['a'], yaml_config['ellipsoid']['b'])
    default_geoid = Geoid()
    default_epoch = (datetime.strptime(yaml_config['epoch'], "%Y-%m-%d %H:%M:%S.%f")
                     .replace(tzinfo=timezone.utc).timestamp())


class EarthCoordinates:

    def __init__(self, ellipsoid: Ellipsoid, geoid: Geoid,
                 time: float, epoch: float = None, cirs: CIRSCoefficients = None):

        if ellipsoid is None:
            self.__ellipsoid = default_ellipsoid
        else:
            self.__ellipsoid = ellipsoid

        if geoid is None:
            self.__geoid = default_geoid
        else:
            self.__geoid = geoid

        if time is None:
            self.__time = default_epoch
        else:
            self.__time = time

        if epoch is None:
            self.__epoch = default_epoch
        else:
            self.__epoch = epoch

        if cirs is None:
            self.__cirs = default_cirs_coefficients
        else:
            self.__cirs = cirs

    @property
    def ellipsoid(self) -> Ellipsoid:
        return self.__ellipsoid

    @property
    def geoid(self) -> Geoid:
        return self.__geoid

    @property
    def time(self) -> float:
        return self.__time

    @property
    def epoch(self) -> float:
        return self.__epoch

    @property
    def cirs(self) -> CIRSCoefficients:
        return self.__cirs

    @property
    def eci(self) -> Eci:
        raise NotImplementedError

    @property
    def ecef(self) -> Ecef:
        raise NotImplementedError

    @property
    def lla(self) -> Lla:
        raise NotImplementedError


class Ecef(EarthCoordinates):

    def __init__(self, x: float, y: float, z: float, ellipsoid: Ellipsoid = None, geoid: Geoid = None,
                 time: float = None, epoch: float = None, cirs: CIRSCoefficients = None):
        super().__init__(ellipsoid, geoid, time, epoch, cirs)
        self.__x = x
        self.__y = y
        self.__z = z

    def __str__(self):
        return f'[{self.x}, {self.y}, {self.z}]'

    @property
    def x(self) -> float:
        return self.__x

    @property
    def y(self) -> float:
        return self.__y

    @property
    def z(self) -> float:
        return self.__z

    @property
    def ecef(self) -> Ecef:
        return self

    @property
    def eci(self) -> Eci:
        x, y, z = ecef_to_eci(self.__x, self.__y, self.__z, self.time, self.epoch, self.cirs.coefficients)
        return Eci(x, y, z, self.ellipsoid, self.geoid, self.time, self.epoch, self.cirs)

    @property
    def lla(self) -> Lla:
        latitude, longitude, altitude = ecef_to_lla(self.__x, self.__y, self.__z, self.ellipsoid.c_ellipsoid())
        return Lla(latitude, longitude, altitude, self.ellipsoid, self.geoid, self.time, self.epoch)


class Eci(EarthCoordinates):

    def __init__(self, x: float, y: float, z: float, ellipsoid: Ellipsoid = None, geoid: Geoid = None,
                 time: float = None, epoch: float = None, cirs: CIRSCoefficients = None):
        super().__init__(ellipsoid, geoid, time, epoch, cirs)
        self.__x = x
        self.__y = y
        self.__z = z

    def __str__(self):
        return f'[{self.x}, {self.y}, {self.z}]'

    @property
    def x(self) -> float:
        return self.__x

    @property
    def y(self) -> float:
        return self.__y

    @property
    def z(self) -> float:
        return self.__z

    @property
    def ecef(self) -> Ecef:
        pass

    @property
    def eci(self) -> Eci:
        return self

    @property
    def lla(self) -> Lla:
        pass


class Lla(EarthCoordinates):

    def __init__(self, latitude: float, longitude: float, altitude: float,
                 ellipsoid: Ellipsoid = None, geoid: Geoid = None,
                 time: float = None, epoch: float = None, cirs: CIRSCoefficients = None):
        super().__init__(ellipsoid, geoid, time, epoch, cirs)
        self.__latitude = latitude
        self.__longitude = longitude
        self.__altitude = altitude

    def __str__(self):
        return f'[{self.latitude}, {self.longitude}, {self.altitude}]'

    @property
    def latitude(self) -> float:
        return self.__latitude

    @property
    def longitude(self) -> float:
        return self.__longitude

    @property
    def altitude(self) -> float:
        return self.__altitude

    @property
    def ecef(self) -> Ecef:
        x, y, z = lla_to_ecef(self.__latitude, self.__longitude, self.__altitude, self.ellipsoid.c_ellipsoid())
        return Ecef(x, y, z, self.ellipsoid, self.geoid, self.time, self.epoch)

    @property
    def eci(self) -> Eci:
        pass

    @property
    def lla(self) -> Lla:
        return self
