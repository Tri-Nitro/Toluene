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
import sys
import yaml

from toluene.math.algebra import Polynomial
from toluene.models.earth.cirs_coefficients import CIRSCoefficients
from toluene.models.earth.earth_orientation_table import EarthOrientationTable
from toluene.models.earth.ellipsoid import Ellipsoid
from toluene.models.earth.geoid import Geoid
from toluene.models.earth.model import EarthModel
from toluene.models.earth.nutation import NutationSeries
from toluene.util.file import datadir, configdir
from toluene.util.time import DeltaTTable

from toluene_extensions.models.earth.coordinates import eci_to_ecef, ecef_to_eci, ecef_to_lla, lla_to_ecef
from toluene_extensions.models.earth.ellipsoid import ellipsoid_radius

default_earth_model = None


def use_default_model() -> EarthModel:
    global default_earth_model
    default_earth_model = EarthModel()

    yaml_config = None
    with open(configdir + '/config.yml') as f:
        yaml_config = yaml.safe_load(f)

    default_cirs_coefficients = CIRSCoefficients(yaml_config['cirs_coefficients'])
    default_ellipsoid = Ellipsoid(yaml_config['ellipsoid']['a'], yaml_config['ellipsoid']['b'])
    default_geoid = Geoid()
    default_epoch = (datetime.strptime(yaml_config['time']['epoch'], "%Y-%m-%d %H:%M:%S.%f")
                     .replace(tzinfo=timezone.utc).timestamp())
    default_eop_table = EarthOrientationTable(datadir + '/finals2000A.all')
    default_delta_t_table = DeltaTTable(datadir + '/deltat.data')
    default_gmst_polynomial = Polynomial(yaml_config['time']['gmst_du'])
    default_nutation_series = NutationSeries(yaml_config['nutation']['series'])

    default_earth_model.set_cirs_coefficients(default_cirs_coefficients)
    default_earth_model.set_ellipsoid(default_ellipsoid)
    default_earth_model.set_geoid(default_geoid)
    default_earth_model.set_epoch(default_epoch)
    default_earth_model.set_eop_table(default_eop_table)
    default_earth_model.set_delta_t_table(default_delta_t_table)
    default_earth_model.set_gmst_polynomial(default_gmst_polynomial)
    default_earth_model.set_nutation_series(default_nutation_series)
    return default_earth_model


class EarthCoordinates:

    def __init__(self, time: float, model: EarthModel = None):
        self.__time = time
        if model is None:
            self.__model = default_earth_model
        else:
            self.__model = model

    @property
    def time(self) -> float:
        return self.__time

    @property
    def model(self) -> EarthModel:
        return self.__model

    @property
    def eci(self) -> Eci:
        raise NotImplementedError

    @property
    def ecef(self) -> Ecef:
        raise NotImplementedError

    @property
    def lla(self) -> Lla:
        raise NotImplementedError

    @property
    def magnitude(self) -> float:
        raise NotImplementedError


class Ecef(EarthCoordinates):

    def __init__(self, x: float, y: float, z: float, time: float = None, model: EarthModel = None):
        super().__init__(time, model)
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
        x, y, z, _, _, _, _, _, _ = ecef_to_eci(self.__x, self.__y, self.__z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                                self.time, self.model.model)
        return Eci(x, y, z, self.time, self.model)

    @property
    def lla(self) -> Lla:
        latitude, longitude, altitude = ecef_to_lla(self.__x, self.__y, self.__z, self.model.model)
        return Lla(latitude, longitude, altitude, self.time, self.model)

    @property
    def magnitude(self) -> float:
        return (self.__x ** 2 + self.__y ** 2 + self.__z ** 2) ** 0.5


class Eci(EarthCoordinates):

    def __init__(self, x: float, y: float, z: float, time: float = None, model: EarthModel = None):
        super().__init__(time, model)
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
        x, y, z,  _, _, _, _, _, _ = eci_to_ecef(self.__x, self.__y, self.__z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                                self.time, self.model.model)
        return Ecef(x, y, z, self.time, self.model)

    @property
    def eci(self) -> Eci:
        return self

    @property
    def lla(self) -> Lla:
        x, y, z, _, _, _, _, _, _ = eci_to_ecef(self.__x, self.__y, self.__z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                                self.time, self.model.model)
        latitude, longitude, altitude = ecef_to_lla(x, y, z, self.model.model)
        return Lla(latitude, longitude, altitude, self.time, self.model)

    @property
    def magnitude(self) -> float:
        return (self.__x ** 2 + self.__y ** 2 + self.__z ** 2) ** 0.5


class Lla(EarthCoordinates):

    def __init__(self, latitude: float, longitude: float, altitude: float,
                 time: float = None, model: EarthModel = None):
        super().__init__(time, model)
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
        x, y, z = lla_to_ecef(self.__latitude, self.__longitude, self.__altitude, self.model.model)
        return Ecef(x, y, z, self.time, self.model)

    @property
    def eci(self) -> Eci:
        x, y, z = lla_to_ecef(self.__latitude, self.__longitude, self.__altitude, self.model.model)
        x, y, z, vx, vy, vz, ax, ay, az = ecef_to_eci(x, y, z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                                self.time, self.model.model)
        return Eci(x, y, z, self.time, self.model)

    @property
    def lla(self) -> Lla:
        return self

    @property
    def magnitude(self) -> float:
        return ellipsoid_radius(self.model.ellipsoid.c_ellipsoid, self.__latitude) + self.__altitude
