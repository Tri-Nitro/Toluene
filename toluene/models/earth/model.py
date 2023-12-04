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
from toluene.models.earth.earth_orientation_table import EarthOrientationTable

from toluene.math.algebra import Polynomial
from toluene.models.earth.cirs_coefficients import CIRSCoefficients
from toluene.models.earth.ellipsoid import Ellipsoid
from toluene.models.earth.geoid import Geoid
from toluene.models.earth.nutation import NutationSeries
from toluene.util.time import DeltaTTable

from toluene_extensions.models.earth.model import (new_EarthModel, get_ellipsoid, set_ellipsoid, \
    get_cirs_coefficients, set_cirs_coefficients, get_nutation_series, set_nutation_series, get_eop_table,
    set_eop_table, get_delta_t_table, set_delta_t_table, get_gmst_polynomial, set_gmst_polynomial, get_epoch, set_epoch)


class EarthModel:

    def __init__(self):
        self.__model = new_EarthModel()
        self.__ellipsoid = None
        self.__geoid = None
        self.__cirs_coefficients = None
        self.__nutation_series = None
        self.__eop_table = None
        self.__gmst_polynomial = None
        self.__epoch = None

    @property
    def model(self):
        return self.__model

    @property
    def ellipsoid(self) -> Ellipsoid:
        self.__ellipsoid = Ellipsoid(c_ellipsoid=get_ellipsoid(self.__model))
        return self.__ellipsoid

    def set_ellipsoid(self, ellipsoid: Ellipsoid):
        self.__ellipsoid = ellipsoid
        set_ellipsoid(self.__model, ellipsoid.c_ellipsoid())

    @property
    def geoid(self) -> Geoid:
        return self.__geoid

    def set_geoid(self, geoid: Geoid):
        pass

    @property
    def cirs_coefficients(self) -> CIRSCoefficients:
        self.__cirs_coefficients = (
            CIRSCoefficients(cirs_dict=get_cirs_coefficients(self.__model)))
        return self.__cirs_coefficients

    def set_cirs_coefficients(self, cirs_coefficients: CIRSCoefficients):
        self.__cirs_coefficients = cirs_coefficients
        set_cirs_coefficients(self.__model, cirs_coefficients.coefficients)

    @property
    def nutation_series(self) -> NutationSeries:
        self.__nutation_series = NutationSeries(c_nutation_series=get_nutation_series(self.__model))
        return self.__nutation_series

    def set_nutation_series(self, nutation_series: NutationSeries):
        self.__nutation_series = nutation_series
        set_nutation_series(self.__model, nutation_series.series)

    @property
    def eop_table(self) -> EarthOrientationTable:
        self.__eop_table = EarthOrientationTable(get_eop_table(self.__model))
        return self.__eop_table

    def set_eop_table(self, eop_table: EarthOrientationTable):
        self.__eop_table = eop_table
        set_eop_table(self.__model, eop_table.table)

    @property
    def delta_t_table(self) -> DeltaTTable:
        self.__delta_t_table = EarthOrientationTable(get_eop_table(self.__model))
        return self.__delta_t_table

    def set_delta_t_table(self, delta_t_table: DeltaTTable):
        self.__delta_t_table = delta_t_table
        set_delta_t_table(self.__model, delta_t_table.table)

    @property
    def epoch(self) -> float:
        self.__epoch = get_epoch(self.__model)
        return self.__epoch

    def set_epoch(self, epoch: float):
        self.__epoch = epoch
        set_epoch(self.__model, epoch)

    @property
    def gmst_polynomial(self) -> Polynomial:
        return self.__gmst_polynomial

    def set_gmst_polynomial(self, gmst_du: Polynomial):
        self.__gmst_polynomial = gmst_du
        set_gmst_polynomial(self.__model, gmst_du.coefficients)
