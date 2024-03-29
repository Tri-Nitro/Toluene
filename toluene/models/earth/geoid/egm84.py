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

from toluene.models.earth.geoid import Geoid


class EGM84(Geoid):

    def __init__(self, interpolation_file_path: str = None, spherical_harmonics_file_path: str = None):
        super().__init__()
        if interpolation_file_path is not None:
            self.grid_from_file(interpolation_file_path)
        if spherical_harmonics_file_path is not None:
            self.coefficients_from_file(spherical_harmonics_file_path)

    def grid_from_file(self, file_path: str):
        with open(file_path, 'r') as file:
            height_list = []
            for line in file:
                line = line.split()
                height_list.append(float(line[2]))

        self.add_interpolation(0.5, height_list)

    def coefficients_from_file(self, file_path: str):
        with open(file_path, 'r') as file:
            for line in file:
                degree = int(line[:5])
                order = int(line[6:10])
                c = float(line[11:25])
                s = float(line[26:])
                self.add_coefficient(degree, order, c, s)



