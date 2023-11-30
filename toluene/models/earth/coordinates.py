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

from toluene.models.earth.cirs_to_tirs_coefficients import CIRStoTIRSCoefficients
from toluene.models.earth.ellipsoid import Ellipsoid
from toluene.models.earth.geoid import Geoid
from toluene.util.file import configdir

import yaml

yaml_config = None
with open(configdir + '/config.yml') as f:
    yaml_config = yaml.safe_load(f)
default_cirs_to_tirs_coefficients = CIRStoTIRSCoefficients(yaml_config['cirs_to_tirs'])
default_ellipsoid = Ellipsoid(yaml_config['ellipsoid']['a'], yaml_config['ellipsoid']['b'])
default_geoid = Geoid()
default_epoch = (datetime.strptime(yaml_config['epoch'], "%Y-%m-%d %H:%M:%S.%f")
                 .replace(tzinfo=timezone.utc).timestamp())


class EarthCoordinates:

    def __init__(self, ellipsoid: Ellipsoid, geoid: Geoid, time: datetime):
        self.__ellipsoid = ellipsoid
        self.__geoid = geoid
        self.__time = time

    @property
    def ellipsoid(self) -> Ellipsoid:
        return self.__ellipsoid

    @property
    def geoid(self) -> Geoid:
        return self.__geoid

    @property
    def time(self) -> datetime:
        return self.__time

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

    def __init__(self, x: float, y: float, z: float,
                 ellipsoid: Ellipsoid = None, geoid: Geoid = None, time: datetime = None):
        super().__init__(ellipsoid, geoid, time)
        self.__x = x
        self.__y = y
        self.__z = z

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
        pass

    @property
    def lla(self) -> Lla:
        pass


class Eci(EarthCoordinates):

    def __init__(self, x: float, y: float, z: float,
                 ellipsoid: Ellipsoid = None, geoid: Geoid = None, time: datetime = None):
        super().__init__(ellipsoid, geoid, time)
        self.__x = x
        self.__y = y
        self.__z = z

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
                 ellipsoid: Ellipsoid = None, geoid: Geoid = None, time: datetime = None):
        super().__init__(ellipsoid, geoid, time)
        self.__latitude = latitude
        self.__longitude = longitude
        self.__altitude = altitude

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
        pass

    @property
    def eci(self) -> Eci:
        pass

    @property
    def lla(self) -> Lla:
        return self