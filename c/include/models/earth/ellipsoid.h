/*
 * MIT License
 *
 * Copyright (c) 2023 Tri-Nitro
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * */
#ifndef __MODELS_EARTH_ELLIPSOID_H__
#define __MODELS_EARTH_ELLIPSOID_H__

#ifdef __cplusplus
extern "C" {
#endif


/** @struct
 * @brief The Ellipsoid object
 *
 * @var Ellipsoid::a
 * Member 'a' contains the semi-major axis
 *
 * @var Ellipsoid::b
 * Member 'b' contains the semi-minor axis
 */
typedef struct {
    long double a;   /* Semi-major axis */
    long double b;   /* Semi-minor axis */
} Ellipsoid;


#ifdef __compile_models_earth_ellipsoid__

/**
 * @brief Sets the axes of the ellipsoid
 *
 * @param a The semi-major axis
 * @param b The semi-minor axis
 */
static PyObject* set_axes(PyObject* self, PyObject* args);

/**
 * @brief Gets the axes of the ellipsoid
 */
static PyObject* get_axes(PyObject* self, PyObject* args);

/**
 * @brief Sets the semi-major axis of the ellipsoid
 *
 * @param a The semi-major axis
 */
static PyObject* set_semi_major_axis(PyObject* self, PyObject* args);

/**
 * @brief Gets the semi-major axis of the ellipsoid
 */
static PyObject* get_semi_major_axis(PyObject* self, PyObject* args);

/**
 * @brief Sets the semi-minor axis of the ellipsoid
 *
 * @param b The semi-minor axis
 */
static PyObject* set_semi_minor_axis(PyObject* self, PyObject* args);

/**
 * @brief Gets the semi-minor axis of the ellipsoid
 */
static PyObject* get_semi_minor_axis(PyObject* self, PyObject* args);

/**
 * @brief Sets the flattening of the ellipsoid
 *
 * @param f The flattening
 */
static PyObject* get_flattening(PyObject* self, PyObject* args);

/**
 * @brief Calculates the eccentricity squared
 *
 * @return PyObject* The eccentricity squared
 */
static PyObject* eccentricity_squared(PyObject* self, PyObject* args);

/**
 * @brief Calculates the radius of curvature in the prime vertical
 *
 * @return PyObject* The radius of curvature in the prime vertical
 */
static PyObject* ellipsoid_radius(PyObject* self, PyObject* args);

/**
 * @brief Creates a new Ellipsoid object and makes it available to Python
 *
 * @return PyObject* The Ellipsoid object
 */
static PyObject* new_Ellipsoid(PyObject* self, PyObject* args);

/**
 * @brief Deletes the Ellipsoid object
 *
 * @param obj The Ellipsoid object
 */
static void delete_Ellipsoid(PyObject* obj);


#endif /* __compile_models_earth_ellipsoid__ */


#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */


#endif /* __MODELS_EARTH_ELLIPSOID_H__ */