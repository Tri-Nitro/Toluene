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

from ctypes import py_object

from toluene.coordinates.reference_frame import ReferenceFrame
from toluene.models.earth.model import EarthModel
from toluene_extensions.coordinates import state_vector, transform


class StateVector:
    """
    StateVector representation in Python. State Vectors are used to represent coordinates in various implemented
    reference frames. The three most common being Geodetic which is Latitude, Longitude, and altitude. These require
    an ellipsoid model to be defined to do anything interesting with. InternationalTerrestrialReferenceFrame coordinates
    which is a fancy name for ecef coordinates given in meters from the earth center in cartisian coordinates.
    GeocentricCelestialReferenceFrame coordinates are similar to ecef but the reference frame is non-rotating as the
    earth rotates under them. This requires the UTC timestamp of the coordinates to mean anything to an observer on
    earth. This is the backbone to anything to do with position, velocity, and/or acceleration data.

    :param x: x-axis displacement this is in meters for cartisian coordinates or latitude for spherical coordinates.
    :type x: float
    :param y: y-axis displacement this is in meters for cartisian coordinates or longitude for spherical
    coordinates.
    :param z: z-axis displacement this is in meters for both or altitude over the reference ellipsoid.
    :type z: float
    :param vx: velocity along x-axis in meters per second. Meaningless to Geodetic coordinates and is unneeded.
    :type vx: float
    :param vy: velocity along y-axis in meters per second. Meaningless to Geodetic coordinates and is unneeded.
    :type vy: float
    :param vz: velocity along z-axis in meters per second. Meaningless to Geodetic coordinates and is unneeded.
    :type vz: float
    :param time: time in seconds since January 1, 1970
    :type time: float
    :param frame: The reference frame of the coordinates.
    :type frame: :class:`ReferenceFrame`
    """

    def __init__(self, x: float, y: float, z: float = 0.0, vx: float = 0.0, vy: float = 0.0, vz: float = 0.0,
                 ax: float = 0.0, ay: float = 0.0, az: float = 0.0,
                 time: float = 0.0, frame: ReferenceFrame = ReferenceFrame.GeodeticReferenceFrame,
                 capsule: py_object = None):
        if capsule is None:
            self.__state_vector = state_vector.new_StateVector(x, y, z, vx, vy, vz, ax, ay, az, time, int(frame))
        else:
            self.__state_vector = capsule

    """
    Gets the position vector as a tuple of floats.
    
    :return: The position vector.
    :rtype: tuple(float, float, float)
    """

    @property
    def position(self) -> (float, float, float):
        return state_vector.get_position(self.__state_vector)

    """
    Gets the velocity vector as a tuple of floats.
    
    :return: The velocity vector.
    :rtype: tuple(float, float, float)
    """

    @property
    def velocity(self) -> (float, float, float):
        return state_vector.get_velocity(self.__state_vector)

    """
    Gets the acceleration vector as a tuple of floats.
    
    :return: The acceleration vector.
    :rtype: tuple(float, float, float)
    """

    @property
    def acceleration(self) -> (float, float, float):
        return state_vector.get_acceleration(self.__state_vector)

    """
    Gets the time of the vector. This is the time since the UNIX epoch not since J2000.0.
    
    :return: The time of the vector.
    :rtype: float
    """

    @property
    def time(self) -> float:
        return state_vector.get_time(self.__state_vector)

    """
    Gets the frame the vector is in.
    
    :return: The frame the vector is in.
    :rtype: :class:`ReferenceFrame`
    """

    @property
    def reference_frame(self) -> ReferenceFrame:
        return ReferenceFrame(state_vector.get_frame(self.__state_vector))

    """
    Creates a copy of the vector.
    
    :return: A new instance of the vector in it's current state.
    :rtype: :class:`StateVector`
    """

    @property
    def __copy(self) -> StateVector:
        frame = state_vector.get_frame(self.__state_vector)
        position = state_vector.get_position(self.__state_vector)
        velocity = state_vector.get_velocity(self.__state_vector)
        acceleration = state_vector.get_acceleration(self.__state_vector)
        time = state_vector.get_time(self.__state_vector)
        retval = state_vector.new_StateVector(position[0], position[1], position[2],
                                              velocity[0], velocity[1], velocity[2], acceleration[0], acceleration[1],
                                              acceleration[2], time, frame)
        return StateVector(None, None, capsule=retval)

    """
    Creates a copy of the current vector but doing the appropriate translation to be in the GeodeticReferenceFrame.
    
    :param model: The earth model to use for the conversion.
    :type model: :class:`toluene.models.earth.EarthModel`
    :return: A copy of the state vector in the GeodeticReferenceFrame.
    :rtype: :class:`StateVector`
    """
    def get_geodetic(self, model: EarthModel) -> StateVector:
        frame = state_vector.get_frame(self.__state_vector)
        if frame is int(ReferenceFrame.InternationalTerrestrialReferenceFrame):
            return StateVector(None, None,
                               capsule=transform.itrf_to_geodetic(self.__state_vector, model.capsule))
        elif frame is int(ReferenceFrame.InternationalCelestialReferenceFrame):
            return None
        elif frame is int(ReferenceFrame.GeocentricCelestialReferenceFrame):
            return None
        elif frame is int(ReferenceFrame.GeodeticReferenceFrame):
            return self.__copy
        return None

    """
    Creates a copy of the current vector but doing the appropriate translation to be in the
    InternationalTerrestrialReferenceFrame.

    :param model: The earth model to use for the conversion.
    :type model: :class:`toluene.models.earth.EarthModel`
    :return: A copy of the state vector in the GeodeticReferenceFrame.
    :rtype: :class:`StateVector`
    """
    def get_itrs(self, model: EarthModel) -> StateVector:
        frame = state_vector.get_frame(self.__state_vector)
        if frame is int(ReferenceFrame.InternationalTerrestrialReferenceFrame):
            return self.__copy
        elif frame is int(ReferenceFrame.InternationalCelestialReferenceFrame):
            return None
        elif frame is int(ReferenceFrame.GeocentricCelestialReferenceFrame):
            return StateVector(None, None,
                               capsule=transform.gcrf_to_itrf(self.__state_vector, model.capsule))
        elif frame is int(ReferenceFrame.GeodeticReferenceFrame):
            return StateVector(None, None,
                               capsule=transform.geodetic_to_itrf(self.__state_vector, model.capsule))
        return None


    """
    Creates a copy of the current vector but doing the appropriate translation to be in the
    GeocentricCelestialReferenceFrame. This is a non-rotating reference frame that is fixed to the earth's center of
    mass. This requires the UTC timestamp of the coordinates to mean anything to an observer on earth.
    
    :param model: The earth model to use for the conversion.
    :type model: :class:`toluene.models.earth.EarthModel`
    :return: A copy of the state vector in the GeodeticReferenceFrame.
    :rtype: :class:`StateVector`
    """
    def get_gcrs(self, model: EarthModel) -> StateVector:
        frame = state_vector.get_frame(self.__state_vector)
        if frame is int(ReferenceFrame.InternationalTerrestrialReferenceFrame):
            return StateVector(None, None,
                                 capsule=transform.itrf_to_gcrf(self.__state_vector, model.capsule))
        elif frame is int(ReferenceFrame.InternationalCelestialReferenceFrame):
            return None
        elif frame is int(ReferenceFrame.GeocentricCelestialReferenceFrame):
            return self.__copy
        elif frame is int(ReferenceFrame.GeodeticReferenceFrame):
            return None

    @property
    def capsule(self) -> py_object:
        return self.__state_vector
