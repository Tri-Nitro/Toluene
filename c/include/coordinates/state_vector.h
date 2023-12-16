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
#ifndef __COORDINATES_STATE_VECTOR_H__
#define __COORDINATES_STATE_VECTOR_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "coordinates/reference_frame.h"
#include "math/linear_algebra.h"

/** @struct
 */
typedef struct {
    Vec3 r;
    Vec3 v;
    Vec3 a;
    long double time;
    ReferenceFrame frame;
} StateVector;


#ifdef __compile_coordinates_state_vector__

/**
 * @brief Creates a new StateVector object and makes it available to Python
 */
static PyObject* new_StateVector(PyObject* self, PyObject* args);

/**
 * @brief Deletes the StateVector object
 *
 * @param obj The StateVector object
 */
void delete_StateVector(PyObject* obj);

/**
 * @brief Sets the position vector.
 */
static PyObject* set_position(PyObject* self, PyObject* args);

/**
 * @brief Sets the velocity vector.
 */
static PyObject* set_velocity(PyObject* self, PyObject* args);

/**
 * @brief Sets the acceleration vector.
 */
static PyObject* set_acceleration(PyObject* self, PyObject* args);

/**
 * @brief Sets the timestamp.
 */
static PyObject* set_time(PyObject* self, PyObject* args);

/**
 * @brief Sets the reference frame.
 */
static PyObject* set_frame(PyObject* self, PyObject* args);

/**
 * @brief Gets the position vector.
 */
static PyObject* get_position(PyObject* self, PyObject* args);

/**
 * @brief Gets the velocity vector.
 */
static PyObject* get_velocity(PyObject* self, PyObject* args);

/**
 * @brief Gets the acceleration vector.
 */
static PyObject* get_acceleration(PyObject* self, PyObject* args);

/**
 * @brief Gets the timestamp.
 */
static PyObject* get_time(PyObject* self, PyObject* args);

/**
 * @brief Gets the reference frame.
 */
static PyObject* get_frame(PyObject* self, PyObject* args);

#endif /* __compile_coordinates_state_vector__ */


#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */


#endif /* __COORDINATES_STATE_VECTOR_H__ */