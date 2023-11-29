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

from toluene.models.earth.ellipsoid import Ellipsoid
from toluene.models.earth.geoid import Geoid

from toluene_extensions.models.earth.model import new_EarthModel, get_ellipsoid, set_ellipsoid


class EarthModel:

    def __init__(self):
        self.__model = new_EarthModel()
        self.__ellipsoid = None

    def ellipsoid(self) -> Ellipsoid:
        self.__ellipsoid = Ellipsoid(c_ellipsoid=get_ellipsoid(self.__model))
        return self.__ellipsoid

    def set_ellipsoid(self, ellipsoid: Ellipsoid):
        self.__ellipsoid = ellipsoid
        set_ellipsoid(self.__model, ellipsoid.c_ellipsoid())

    def geoid(self) -> Geoid:
        pass

    def set_geoid(self, geoid: Geoid):
        pass

    def epoch(self) -> float:
        pass

    def set_epoch(self, epoch: float):
        pass
