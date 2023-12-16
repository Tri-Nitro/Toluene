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

#include "math/linear_algebra.h"
#include "coordinates/transform.h"
#include "coordinates/state_vector.h"
#include "models/earth/bias.h"
#include "models/earth/earth.h"
#include "models/earth/nutation.h"
#include "models/earth/polar_motion.h"
#include "models/earth/precession.h"
#include "models/earth/rotation.h"
#include "time/constants.h"

/**
 * @brief Converts itrf coordinates to the equivalent gcrf coordinates.
 */
static PyObject* itrf_to_gcrf(PyObject *self, PyObject *args) {

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

    temp.r.x = coriolis_velocity.x = centrifugal_acceleration.x = state_vector->r.x;
    temp.r.y = coriolis_velocity.y = centrifugal_acceleration.y = state_vector->r.y;
    temp.r.z = coriolis_velocity.z = centrifugal_acceleration.z = state_vector->r.z;
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
    nutation_values_of_date(state_vector->time, &model->nutation_series, &nutation_longitude, &nutation_obliquity,
        &mean_obliquity_date, &equation_of_the_equinoxes);

    gast += equation_of_the_equinoxes/15.0;

    wobble(state_vector->time, &model->earth_orientation_parameters, &matrix);
    dot_product(&matrix, &temp.r, &retval->r);
    dot_product(&matrix, &temp.v, &retval->v);
    dot_product(&matrix, &temp.a, &retval->a);
    dot_product(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product(&matrix, &centrifugal_acceleration, &centrifugal_acceleration_prime);

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
static PyObject* gcrf_to_itrf(PyObject *self, PyObject *args) {

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

    temp.r.x = coriolis_velocity.x = centrifugal_acceleration.x = state_vector->r.x;
    temp.r.y = coriolis_velocity.y = centrifugal_acceleration.y = state_vector->r.y;
    temp.r.z = coriolis_velocity.z = centrifugal_acceleration.z = state_vector->r.z;
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
    nutation_values_of_date(state_vector->time, &model->nutation_series, &nutation_longitude, &nutation_obliquity,
        &mean_obliquity_date, &equation_of_the_equinoxes);

    gast += equation_of_the_equinoxes/15.0;

    icrs_frame_bias(&matrix);
    dot_product_transpose(&matrix, &temp.r, &retval->r);
    dot_product_transpose(&matrix, &temp.v, &retval->v);
    dot_product_transpose(&matrix, &temp.a, &retval->a);
    dot_product_transpose(&matrix, &coriolis_velocity, &coriolis_velocity_prime);
    dot_product_transpose(&matrix, &centrifugal_acceleration, &centrifugal_acceleration_prime);

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

/**
 * @brief Converts itrf coordinates to the equivalent geodetic coordinates.
 */
static PyObject* itrf_to_geodetic(PyObject *self, PyObject *args) {

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

    if(state_vector->frame != InternationalTerrestrialReferenceFrame) {
        PyErr_SetString(PyExc_TypeError, "itrf_to_geodetic() was expecting InternationalTerrestrialReferenceFrame");
        return PyErr_Occurred();
    }

    model = (EarthModel*)PyCapsule_GetPointer(model_capsule, "EarthModel");
    if(!model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from Capsule.");
        return PyErr_Occurred();
    }

    StateVector* retval = (StateVector*)malloc(sizeof(StateVector));
    if(!retval) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_StateVector.");
        return PyErr_Occurred();
    }
    retval->r.x = state_vector->r.x;
    retval->r.y = state_vector->r.y;
    retval->r.z = state_vector->r.z;

    // Causes a divide by 0 bug because of p if x and y are 0. Just put a little offset so longitude can be set if
    // directly above the pole.
    if(retval->r.x == 0 && retval->r.y == 0) retval->r.x = 0.000000001;

    long double e_numerator = model->ellipsoid.a*model->ellipsoid.a - model->ellipsoid.b*model->ellipsoid.b;
    long double e_2 = e_numerator/(model->ellipsoid.a*model->ellipsoid.a);
    long double e_r2 = e_numerator/(model->ellipsoid.b*model->ellipsoid.b);
    long double p = sqrt(retval->r.x*retval->r.x+retval->r.y*retval->r.y);
    long double big_f = 54.0*model->ellipsoid.b*model->ellipsoid.b*retval->r.z*retval->r.z;
    long double big_g = p*p+retval->r.z*retval->r.z*(1-e_2)-e_2*e_numerator;
    long double c = (e_2*e_2*big_f*p*p)/(big_g*big_g*big_g);
    long double s = cbrt(1+c+sqrt(c*c+2*c));
    long double k = s+1+1/s;
    long double big_p = big_f/(3*k*k*big_g*big_g);
    long double big_q = sqrt(1 + 2 * e_2 * e_2 * big_p);
    long double sqrt_r_0 = (model->ellipsoid.a*model->ellipsoid.a/2)*(1+1/big_q)-
        ((big_p*(1-e_2)*retval->r.z*retval->r.z)/(big_q*(1+big_q))) -(big_p*p*p)/2;
    sqrt_r_0 = (sqrt_r_0 < 0? 0 : sqrt(sqrt_r_0));
    long double r_0 = ((-1* big_p*e_2*p)/(1+big_q)) + sqrt_r_0;
    long double p_e_2_r_0 = p-e_2*r_0;
    long double big_u = sqrt( p_e_2_r_0*p_e_2_r_0+retval->r.z*retval->r.z);
    long double big_v = sqrt(p_e_2_r_0*p_e_2_r_0+(1-e_2)*retval->r.z*retval->r.z);
    long double z_0 = (model->ellipsoid.b*model->ellipsoid.b*retval->r.z)/(model->ellipsoid.a*big_v);

    retval->r.x = atanl((retval->r.z+(e_r2*z_0))/p) * 180/M_PI;
    retval->r.y = atan2l(retval->r.y,state_vector->r.x) * 180/M_PI;
    retval->r.z = big_u * (1-(model->ellipsoid.b*model->ellipsoid.b)/(model->ellipsoid.a*big_v));

    retval->v.x = state_vector->v.x;
    retval->v.y = state_vector->v.y;
    retval->v.z = state_vector->v.z;
    retval->a.x = state_vector->a.x;
    retval->a.y = state_vector->a.y;
    retval->a.z = state_vector->a.z;
    retval->time = state_vector->time;
    retval->frame = GeodeticReferenceFrame;

    return PyCapsule_New(retval, "StateVector", delete_StateVector);
}

/**
 * @brief Converts geodetic coordinates to the equivalent itrf coordinates.
 */
static PyObject* geodetic_to_itrf(PyObject *self, PyObject *args) {

    PyObject* state_vector_capsule;
    PyObject* model_capsule;
    StateVector* state_vector;
    EarthModel* model;

    if(!PyArg_ParseTuple(args, "OO", &state_vector_capsule, &model_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. geodetic_to_itrf()");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(state_vector_capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from Capsule.");
        return PyErr_Occurred();
    }

    if(state_vector->frame != GeodeticReferenceFrame) {
        PyErr_SetString(PyExc_TypeError, "geodetic_to_itrf() was expecting GeodeticReferenceFrame");
        return PyErr_Occurred();
    }

    model = (EarthModel*)PyCapsule_GetPointer(model_capsule, "EarthModel");
    if(!model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from Capsule.");
        return PyErr_Occurred();
    }

    StateVector* retval = (StateVector*)malloc(sizeof(StateVector));

    long double e_2 = 1 - ((model->ellipsoid.b*model->ellipsoid.b)/(model->ellipsoid.a*model->ellipsoid.a));
    long double sin_of_latitude = sinl((state_vector->r.x * M_PI/180));
    long double n_phi = model->ellipsoid.a/(sqrt(1-(e_2 * (sin_of_latitude*sin_of_latitude))));

    retval->r.x = (n_phi + state_vector->r.z) * cosl(state_vector->r.x * M_PI/180) * cosl(state_vector->r.y
        * M_PI/180);
    retval->r.y = (n_phi + state_vector->r.z) * cosl(state_vector->r.x * M_PI/180) * sinl(state_vector->r.y
        * M_PI/180);
    retval->r.z = ((1 - e_2) * n_phi + state_vector->r.z) * sinl(state_vector->r.x * M_PI/180);

    retval->v.x = state_vector->v.x;
    retval->v.y = state_vector->v.y;
    retval->v.z = state_vector->v.z;
    retval->a.x = state_vector->a.x;
    retval->a.y = state_vector->a.y;
    retval->a.z = state_vector->a.z;
    retval->time = state_vector->time;
    retval->frame = InternationalTerrestrialReferenceFrame;

    return PyCapsule_New(retval, "StateVector", delete_StateVector);
}


static PyMethodDef tolueneCoordinatesTransformMethods[] = {
    {"itrf_to_gcrf", itrf_to_gcrf, METH_VARARGS, "Returns the equivalent coordinates in the GCRS frame."},
    {"gcrf_to_itrf", gcrf_to_itrf, METH_VARARGS, "Returns the equivalent coordinates in the ITRS frame."},
    {"itrf_to_geodetic", itrf_to_geodetic, METH_VARARGS, "Returns the equivalent coordinates in the Geodetic Datum."},
    {"geodetic_to_itrf", geodetic_to_itrf, METH_VARARGS, "Returns the equivalent coordinates in the ITRS frame."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef coordinates_transform = {
    PyModuleDef_HEAD_INIT,
    "coordinates.transform",
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