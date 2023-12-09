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

from toluene_extensions.models.earth.ellipsoid import new_Ellipsoid, set_axes, get_axes, ellipsoid_radius


class Ellipsoid:
    """
    Representation of an ellipsoid model of the Earth based on semi-major and semi-minor axis.

    :param a: The semi-major axis of the ellipsoid in meters.
    :type a: float
    :param b: The semi-minor axis of the ellipsoid in meters.
    :type b: float
    """

    def __init__(self, a: float = None, b: float = None, c_ellipsoid=None):
        if c_ellipsoid is not None:
            self.__ellipsoid = c_ellipsoid
            a, b = get_axes(self.__ellipsoid)
        else:
            self.__ellipsoid = new_Ellipsoid()
        if a is not None and b is not None:
            self.set_axes(a, b)

    def set_axes(self, a: float, b: float):
        """
        Sets the semi-major and semi-minor axes of the ellipsoid.

        :param a: The semi-major axis of the ellipsoid in meters.
        :type a: float
        :param b: The semi-minor axis of the ellipsoid in meters.
        :type b: float
        """
        set_axes(self.__ellipsoid, a, b)

    @property
    def axes(self) -> (float, float):
        """
        Gets the semi-major and semi-minor axes of the ellipsoid.

        :return: The semi-major and semi-minor axes of the ellipsoid in meters.
        :rtype: (float, float)
        """
        return get_axes(self.__ellipsoid)

    @property
    def a(self) -> float:
        """
        :return: The semi-major axis of the ellipsoid in meters.
        :rtype: float
        """
        return self.axes[0]

    @property
    def b(self) -> float:
        """
        :return: The semi-minor axis of the ellipsoid in meters.
        :rtype: float
        """
        return self.axes[1]

    def radius(self, latitude: float) -> float:
        """
        Calculates the radius of the ellipsoid at a given latitude.

        :param latitude: The latitude in degrees.
        :type latitude: float
        :return: The radius of the ellipsoid in meters.
        :rtype: float
        """
        return ellipsoid_radius(self.__ellipsoid, latitude)

    @property
    def c_ellipsoid(self):
        """
        :return: The C ellipsoid object.
        """
        return self.__ellipsoid
