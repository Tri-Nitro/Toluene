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
from toluene_extensions.models.earth import ellipsoid

from ctypes import py_object


class Ellipsoid:
    """
    Class to represent an ellipsoid model. Define your ellipsoid to create a model of a body that takes on an ellipsoid
    shape. This is not stored in python but in C. All computation using the classes are just wrappers for C functions.

    :param semi_major_axis: The semi-major axis of the ellipsoid.
    :type semi_major_axis: float
    :param semi_minor_axis: The semi-minor axis of the ellipsoid.
    :type semi_minor_axis: float
    """
    def __init__(self, semi_major_axis: float = 0.0, semi_minor_axis: float = 0.0):
        self.__ellipsoid = ellipsoid.new_Ellipsoid(semi_major_axis, semi_minor_axis)

    """
    Sets the axes of the Ellipsoid.
    
    :param a: The semi-major axis of the ellipsoid.
    :type a: float
    :param b: The semi-minor axis of the ellipsoid.
    :type b: float
    """
    def set_axes(self, a: float, b: float):
        ellipsoid.set_axes(self.__ellipsoid, a, b)

    """
    Gets the axes of the Ellipsoid as a tuple where the first is the semi-major axis and the second is the semi-minor.
    
    :returns: A tuple containing the semi-major axis and the semi-minor axis of the ellipsoid.
    :rtype: tuple(float, float)
    """
    @property
    def axes(self) -> (float, float):
        return ellipsoid.get_axes(self.__ellipsoid)

    """
    Sets the semi-major axis of the ellipsoid.
    
    :param a: The new semi-major axis of the ellipsoid.
    :type a: float
    """
    def set_semi_major_axis(self, a: float):
        ellipsoid.set_semi_major_axis(self.__ellipsoid, a)

    """
    Gets the semi-major axis of the ellipsoid.

    :returns: The semi-major axis of the ellipsoid.
    :rtype: float
    """
    @property
    def semi_major_axis(self) -> float:
        return ellipsoid.get_semi_major_axis(self.__ellipsoid)

    """
    Sets the semi-minor axis of the ellipsoid.
    
    :param b: The new semi-minor axis of the ellipsoid.
    :type b: float
    """
    def set_semi_minor_axis(self, b: float):
        ellipsoid.set_semi_minor_axis(self.__ellipsoid, b)

    """
    Gets the semi-minor axis of the ellipsoid.
    
    :returns: The semi-minor axis of the ellipsoid.
    :rtype: float
    """
    @property
    def semi_minor_axis(self) -> float:
        return ellipsoid.get_semi_minor_axis(self.__ellipsoid)

    """
    Gets the flattening of the ellipsoid. The flattening factor is defined as:
    :math:`f = \\frac{a-b}{a}`
    
    :return: The flattening of the ellipsoid.
    :rtype: float
    """
    @property
    def flattening(self) -> float:
        return ellipsoid.get_flattening(self.__ellipsoid)

    """
    Gets the eccentricity squared of the ellipsoid. The eccentricity squared is defined as:
    :math:`e^2 = \\frac{a^2-b^2}{a^2}`
    
    :return: The eccentricity squared of the ellipsoid.
    :rtype: float
    """
    @property
    def eccentricity_squared(self) -> float:
        return ellipsoid.get_eccentricity_squared(self.__ellipsoid)

    """
    Gets the radius of the ellipsoid at a given latitude. This is achieved through the formula
    :math`\\sqrt{\\frac{{a^2*cos(\\phi)}^2 + {b^2*sin(\\phi)}^2}{{a*cos(\\phi)}^2 + {b*sin(\\phi)}^2}}`
    
    :return: the radius of the ellipsoid at a given latitude.
    :rtype: float
    """
    def ellipsoid_radius(self, latitude) -> float:
        return ellipsoid.get_ellipsoid_radius(self.__ellipsoid, latitude)

    """
    Get a borrowed reference to the ellipsoid C struct inside the class.
    
    :return: the ellipsoid C struct's PyCapsule inside the class. Not a copy. If this is modified it will reflect inside
    the class.
    :rtype: ctypes.py_object
    """
    @property
    def capsule(self) -> py_object:
        return self.__ellipsoid
