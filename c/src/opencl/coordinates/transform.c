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
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#if defined(_WIN32) || defined(WIN32)       /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows
                                            systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif

#define __compile_coordinates_state_vector__
#define __compile_math_linear_algebra__
#define __compile_models_earth_nutation__


#include "coordinates/state_vector.h"
#include "math/linear_algebra.h"
#include "models/earth/bias.h"
#include "models/earth/earth.h"
#include "models/earth/nutation.h"
#include "models/earth/polar_motion.h"
#include "models/earth/precession.h"
#include "models/earth/rotation.h"
#include "opencl/coordinates/transform.h"
#include "opencl/context.h"
#include "time/constants.h"

/**
 * @brief Converts itrf coordinates to the equivalent gcrf coordinates.
 */
static PyObject* opencl_itrf_to_gcrf(PyObject *self, PyObject *args) {

    PyObject* opencl_kernel;
    PyObject* state_vector_capsule;
    PyObject* model_capsule;
    OpenCLKernel* kernel;
    StateVector* state_vector;
    EarthModel* model;

    if(!PyArg_ParseTuple(args, "OOO", &opencl_kernel, &state_vector_capsule, &model_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. itrf_to_geodetic()");
        return PyErr_Occurred();
    }

    kernel = (OpenCLKernel*)PyCapsule_GetPointer(opencl_kernel, "OpenCLKernel");
    if(!kernel) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the OpenCLKernel from Capsule.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(state_vector_capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from Capsule.");
        return PyErr_Occurred();
    }

    if(state_vector->frame != InternationalTerrestrialReferenceFrame) {
        PyErr_SetString(PyExc_TypeError, "itrf_to_geodetic() was expecting InternationalTerrestrialReferenceFrame");
        return PyErr_Occurred();
    }

    model = (EarthModel*)PyCapsule_GetPointer(model_capsule, "EarthModel");
    if(!model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from Capsule.");
        return PyErr_Occurred();
    }

    Mat3 matrix;
    Vec3 coriolis_velocity, coriolis_velocity_prime;
    Vec3 coriolis_acceleration, coriolis_acceleration_prime;
    Vec3 centrifugal_acceleration, centrifugal_acceleration_prime;

    StateVector* retval = (StateVector*)malloc(sizeof(StateVector));
    if(!retval) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_StateVector.");
        return PyErr_Occurred();
    }
    StateVector temp;

    temp.r.x = coriolis_velocity.x = state_vector->r.x;
    temp.r.y = coriolis_velocity.y = state_vector->r.y;
    temp.r.z = coriolis_velocity.z = state_vector->r.z;
    temp.v.x = coriolis_acceleration.x = state_vector->v.x;
    temp.v.y = coriolis_acceleration.y = state_vector->v.y;
    temp.v.z = coriolis_acceleration.z = state_vector->v.z;
    temp.a.x = state_vector->a.x;
    temp.a.y = state_vector->a.y;
    temp.a.z = state_vector->a.z;

    retval->time = state_vector->time;
    retval->frame = GeocentricCelestialReferenceFrame;

    long double gast;
    gmst(state_vector->time, model, &gast);

    long double nutation_longitude, nutation_obliquity, mean_obliquity_date, equation_of_the_equinoxes;
    nutation_values_of_date_opencl(kernel, state_vector->time, &model->nutation_series, &nutation_longitude, &nutation_obliquity,
        &mean_obliquity_date, &equation_of_the_equinoxes);

    gast += equation_of_the_equinoxes/15.0;

    wobble(state_vector->time, &model->earth_orientation_parameters, &matrix);
    dot_product(&matrix, &temp.r, &retval->r);
    dot_product(&matrix, &temp.v, &retval->v);
    dot_product(&matrix, &temp.a, &retval->a);
    dot_product(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product(&matrix, &coriolis_acceleration, &coriolis_acceleration_prime);

    long double rate;
    Vec3 coriolis_rotation;
    rate_of_earth_rotation(state_vector->time, model, &rate);
    coriolis_rotation.x = 0.0;
    coriolis_rotation.y = 0.0;
    coriolis_rotation.z = rate;

    cross_product(&coriolis_rotation, &coriolis_velocity_prime, &coriolis_velocity);

    cross_product(&coriolis_rotation, &coriolis_velocity, &coriolis_acceleration);
    cross_product(&coriolis_rotation, &coriolis_acceleration_prime, &centrifugal_acceleration);

    centrifugal_acceleration.x *= 2.0;
    centrifugal_acceleration.y *= 2.0;
    centrifugal_acceleration.z *= 2.0;

    earth_rotation_matrix(gast/SECONDS_PER_DAY * 2.0 * M_PI, &matrix);
    dot_product(&matrix, &retval->r, &temp.r);
    dot_product(&matrix, &retval->v, &temp.v);
    dot_product(&matrix, &retval->a, &temp.a);
    dot_product(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product(&matrix, &coriolis_acceleration, &coriolis_acceleration_prime);
    dot_product(&matrix, &centrifugal_acceleration, &centrifugal_acceleration_prime);

    nutation_matrix(mean_obliquity_date, nutation_longitude, mean_obliquity_date-nutation_obliquity, &matrix);
    dot_product(&matrix, &temp.r, &retval->r);
    dot_product(&matrix, &temp.v, &retval->v);
    dot_product(&matrix, &temp.a, &retval->a);
    dot_product(&matrix, &coriolis_velocity_prime, &coriolis_velocity);
    dot_product(&matrix, &coriolis_acceleration_prime, &coriolis_acceleration);
    dot_product(&matrix, &centrifugal_acceleration_prime, &centrifugal_acceleration);

    iau_2000a_precession(state_vector->time, &matrix);
    dot_product(&matrix, &retval->r, &temp.r);
    dot_product(&matrix, &retval->v, &temp.v);
    dot_product(&matrix, &retval->a, &temp.a);
    dot_product(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product(&matrix, &coriolis_acceleration, &coriolis_acceleration_prime);
    dot_product(&matrix, &centrifugal_acceleration, &centrifugal_acceleration_prime);

    icrs_frame_bias(&matrix);
    dot_product(&matrix, &temp.r, &retval->r);
    dot_product(&matrix, &temp.v, &retval->v);
    dot_product(&matrix, &temp.a, &retval->a);
    dot_product(&matrix, &coriolis_velocity_prime, &coriolis_velocity);
    dot_product(&matrix, &coriolis_acceleration_prime, &coriolis_acceleration);
    dot_product(&matrix, &centrifugal_acceleration_prime, &centrifugal_acceleration);

    retval->v.x += coriolis_velocity.x;
    retval->v.y += coriolis_velocity.y;
    retval->v.z += coriolis_velocity.z;
    retval->a.x += coriolis_acceleration.x + centrifugal_acceleration.x;
    retval->a.y += coriolis_acceleration.y + centrifugal_acceleration.y;
    retval->a.z += coriolis_acceleration.z + centrifugal_acceleration.z;

    return PyCapsule_New(retval, "StateVector", delete_StateVector);
}

/**
 * @brief Converts gcrf coordinates to the equivalent itrf coordinates.
 */
static PyObject* opencl_gcrf_to_itrf(PyObject *self, PyObject *args) {

    PyObject* state_vector_capsule;
    PyObject* model_capsule;
    StateVector* state_vector;
    EarthModel* model;

    if(!PyArg_ParseTuple(args, "OO", &state_vector_capsule, &model_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. itrf_to_geodetic()");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(state_vector_capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from Capsule.");
        return PyErr_Occurred();
    }

    if(state_vector->frame != GeocentricCelestialReferenceFrame) {
        PyErr_SetString(PyExc_TypeError, "itrf_to_geodetic() was expecting GeocentricCelestialReferenceFrame");
        return PyErr_Occurred();
    }

    model = (EarthModel*)PyCapsule_GetPointer(model_capsule, "EarthModel");
    if(!model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from Capsule.");
        return PyErr_Occurred();
    }

    Mat3 matrix;
    Vec3 coriolis_velocity, coriolis_velocity_prime;
    Vec3 coriolis_acceleration, coriolis_acceleration_prime;
    Vec3 centrifugal_acceleration, centrifugal_acceleration_prime;

    StateVector* retval = (StateVector*)malloc(sizeof(StateVector));
    if(!retval) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_StateVector.");
        return PyErr_Occurred();
    }
    StateVector temp;

    temp.r.x = coriolis_velocity.x = state_vector->r.x;
    temp.r.y = coriolis_velocity.y = state_vector->r.y;
    temp.r.z = coriolis_velocity.z = state_vector->r.z;
    temp.v.x = coriolis_acceleration.x = state_vector->v.x;
    temp.v.y = coriolis_acceleration.y = state_vector->v.y;
    temp.v.z = coriolis_acceleration.z = state_vector->v.z;
    temp.a.x = state_vector->a.x;
    temp.a.y = state_vector->a.y;
    temp.a.z = state_vector->a.z;

    retval->time = state_vector->time;
    retval->frame = InternationalTerrestrialReferenceFrame;

    long double gast;
    gmst(state_vector->time, model, &gast);

    long double nutation_longitude, nutation_obliquity, mean_obliquity_date, equation_of_the_equinoxes;
    nutation_values_of_date_opencl(state_vector->time, &model->nutation_series, &nutation_longitude, &nutation_obliquity,
        &mean_obliquity_date, &equation_of_the_equinoxes);

    gast += equation_of_the_equinoxes/15.0;

    icrs_frame_bias(&matrix);
    dot_product_transpose(&matrix, &temp.r, &retval->r);
    dot_product_transpose(&matrix, &temp.v, &retval->v);
    dot_product_transpose(&matrix, &temp.a, &retval->a);
    dot_product_transpose(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product_transpose(&matrix, &coriolis_acceleration, &coriolis_acceleration_prime);

    iau_2000a_precession(state_vector->time, &matrix);
    dot_product_transpose(&matrix, &retval->r, &temp.r);
    dot_product_transpose(&matrix, &retval->v, &temp.v);
    dot_product_transpose(&matrix, &retval->a, &temp.a);
    dot_product_transpose(&matrix, &coriolis_velocity_prime, &coriolis_velocity);
    dot_product_transpose(&matrix, &centrifugal_acceleration_prime, &centrifugal_acceleration);

    nutation_matrix(mean_obliquity_date, nutation_longitude, mean_obliquity_date-nutation_obliquity, &matrix);
    dot_product_transpose(&matrix, &temp.r, &retval->r);
    dot_product_transpose(&matrix, &temp.v, &retval->v);
    dot_product_transpose(&matrix, &temp.a, &retval->a);
    dot_product_transpose(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product_transpose(&matrix, &centrifugal_acceleration, &centrifugal_acceleration_prime);

    long double rate;
    Vec3 coriolis_rotation;
    rate_of_earth_rotation(state_vector->time, model, &rate);
    coriolis_rotation.x = 0.0;
    coriolis_rotation.y = 0.0;
    coriolis_rotation.z = rate;

    cross_product(&coriolis_rotation, &coriolis_velocity_prime, &coriolis_velocity);

    cross_product(&coriolis_rotation, &coriolis_velocity, &coriolis_acceleration);
    cross_product(&coriolis_rotation, &coriolis_acceleration_prime, &centrifugal_acceleration);

    centrifugal_acceleration.x *= 2.0;
    centrifugal_acceleration.y *= 2.0;
    centrifugal_acceleration.z *= 2.0;

    earth_rotation_matrix(gast/SECONDS_PER_DAY * 2.0 * M_PI, &matrix);
    dot_product_transpose(&matrix, &retval->r, &temp.r);
    dot_product_transpose(&matrix, &retval->v, &temp.v);
    dot_product_transpose(&matrix, &retval->a, &temp.a);
    dot_product_transpose(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product_transpose(&matrix, &coriolis_acceleration, &coriolis_acceleration_prime);
    dot_product_transpose(&matrix, &centrifugal_acceleration, &centrifugal_acceleration_prime);

    wobble(state_vector->time, &model->earth_orientation_parameters, &matrix);
    dot_product_transpose(&matrix, &temp.r, &retval->r);
    dot_product_transpose(&matrix, &temp.v, &retval->v);
    dot_product_transpose(&matrix, &temp.a, &retval->a);
    dot_product_transpose(&matrix, &coriolis_velocity_prime, &coriolis_velocity);
    dot_product_transpose(&matrix, &coriolis_acceleration_prime, &coriolis_acceleration);
    dot_product_transpose(&matrix, &centrifugal_acceleration_prime, &centrifugal_acceleration);

    retval->v.x -= coriolis_velocity.x;
    retval->v.y -= coriolis_velocity.y;
    retval->v.z -= coriolis_velocity.z;
    retval->a.x -= coriolis_acceleration.x - centrifugal_acceleration.x;
    retval->a.y -= coriolis_acceleration.y - centrifugal_acceleration.y;
    retval->a.z -= coriolis_acceleration.z - centrifugal_acceleration.z;

    return PyCapsule_New(retval, "StateVector", delete_StateVector);
}


static PyMethodDef tolueneCoordinatesTransformMethods[] = {
    {"itrf_to_gcrf", opencl_itrf_to_gcrf, METH_VARARGS, "Returns the equivalent coordinates in the GCRS frame."},
    {"gcrf_to_itrf", opencl_gcrf_to_itrf, METH_VARARGS, "Returns the equivalent coordinates in the ITRS frame."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef coordinates_transform = {
    PyModuleDef_HEAD_INIT,
    "opencl.coordinates.transform",
    "C Extensions for transforming between reference frames",
    -1,
    tolueneCoordinatesTransformMethods
};


PyMODINIT_FUNC PyInit_transform(void) {
    return PyModule_Create(&coordinates_transform);
}


#ifdef __cplusplus
} /* extern "C" */
#endif