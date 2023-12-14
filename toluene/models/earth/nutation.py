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
from typing import List

from toluene_extensions.models.earth.nutation import new_NutationSeries, add_record


class NutationSeries:

    def __init__(self, series: List[float] = None, capsule=None):
        if capsule is not None:
            self.__nutation_series = capsule
        else:
            self.__nutation_series = new_NutationSeries()
        if series is not None:
            self.load_from_list(series)

    def load_from_list(self, series: List[float]):
        for idx in range(0, len(series), 21):
            coefficients = series[idx:idx + 21]
            add_record(self.__nutation_series, coefficients[1], coefficients[2], coefficients[3], coefficients[4],
                       coefficients[5], coefficients[6], coefficients[7], coefficients[8], coefficients[9],
                       coefficients[10], coefficients[11], coefficients[12], coefficients[13], coefficients[14],
                       coefficients[15], coefficients[16], coefficients[17], coefficients[18], coefficients[19],
                       coefficients[20])

    @property
    def capsule(self):
        return self.__nutation_series
