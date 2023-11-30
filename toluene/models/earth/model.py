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
from toluene.models.earth.cirs_to_tirs_coefficients import CIRStoTIRSCoefficients
from toluene.models.earth.ellipsoid import Ellipsoid
from toluene.models.earth.geoid import Geoid

from toluene_extensions.models.earth.model import new_EarthModel, get_ellipsoid, set_ellipsoid, \
    get_cirs_to_tirs_coefficients, set_cirs_to_tirs_coefficients


class EarthModel:

    def __init__(self):
        self.__model = new_EarthModel()
        self.__ellipsoid = None
        self.__geoid = None
        self.__epoch = None
        self.__cirs_to_tirs_coefficients = None

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
    def epoch(self) -> float:
        return self.__epoch

    def set_epoch(self, epoch: float):
        pass

    @property
    def cirs_to_tirs_coefficients(self):
        self.__cirs_to_tirs_coefficients = (
            CIRStoTIRSCoefficients(cirs_to_tirs_dict=get_cirs_to_tirs_coefficients(self.__model)))
        return self.__cirs_to_tirs_coefficients

    def set_cirs_to_tirs_coefficients(self, cirs_to_tirs_coefficients: CIRStoTIRSCoefficients):
        self.__cirs_to_tirs_coefficients = cirs_to_tirs_coefficients
        set_cirs_to_tirs_coefficients(self.__model, cirs_to_tirs_coefficients.c_cirs_to_tirs_coefficients())