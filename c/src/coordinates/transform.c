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

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


#define __compile_coordinates_state_vector__
#include "coordinates/transform.h"
#include "coordinates/state_vector.h"
#include "models/earth/earth.h"

/**
 * @brief Converts itrf coordinates to the equivalent gcrf coordinates.
 */
static PyObject* itrf_to_gcrf(PyObject *self, PyObject *args) {
    return NULL;
}

/**
 * @brief Converts gcrf coordinates to the equivalent itrf coordinates.
 */
static PyObject* gcrf_to_itrf(PyObject *self, PyObject *args) {
    return NULL;
}

/**
 * @brief Converts itrf coordinates to the equivalent geodetic coordinates.
 */
static PyObject* itrf_to_geodetic(PyObject *self, PyObject *args) {

    PyObject* state_vector_capsule;
    PyObject* model_capsule;
    StateVector* state_vector;
    EarthModel* model;

    long double x, y, z;

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

    x = state_vector->r.x;
    y = state_vector->r.y;
    z = state_vector->r.z;

    // Causes a divide by 0 bug because of p if x and y are 0. Just put a little offset so longitude can be set if
    // directly above the pole.
    if(x == 0 && y == 0) x = 0.000000001;

    long double e_numerator = model->ellipsoid.a*model->ellipsoid.a - model->ellipsoid.b*model->ellipsoid.b;
    long double e_2 = e_numerator/(model->ellipsoid.a*model->ellipsoid.a);
    long double e_r2 = e_numerator/(model->ellipsoid.b*model->ellipsoid.b);
    long double p = sqrt(x*x+y*y);
    long double big_f = 54.0*model->ellipsoid.b*model->ellipsoid.b*z*z;
    long double big_g = p*p+z*z*(1-e_2)-e_2*e_numerator;
    long double c = (e_2*e_2*big_f*p*p)/(big_g*big_g*big_g);
    long double s = cbrt(1+c+sqrt(c*c+2*c));
    long double k = s+1+1/s;
    long double big_p = big_f/(3*k*k*big_g*big_g);
    long double big_q = sqrt(1 + 2 * e_2 * e_2 * big_p);
    long double sqrt_r_0 = (model->ellipsoid.a*model->ellipsoid.a/2)*(1+1/big_q)-((big_p*(1-e_2)*z*z)/(big_q*(1+big_q)))
                 -(big_p*p*p)/2;
    sqrt_r_0 = (sqrt_r_0 < 0? 0 : sqrt(sqrt_r_0));
    long double r_0 = ((-1* big_p*e_2*p)/(1+big_q)) + sqrt_r_0;
    long double p_e_2_r_0 = p-e_2*r_0;
    long double big_u = sqrt( p_e_2_r_0*p_e_2_r_0+z*z);
    long double big_v = sqrt(p_e_2_r_0*p_e_2_r_0+(1-e_2)*z*z);
    long double z_0 = (model->ellipsoid.b*model->ellipsoid.b*z)/(model->ellipsoid.a*big_v);

    retval->r.x = atan((z+(e_r2*z_0))/p) * 180/M_PI;
    retval->r.y = atan2(y,x) * 180/M_PI;
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

    long double latitude, longitude, altitude;

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

    latitude = state_vector->r.x;
    longitude = state_vector->r.y;
    altitude = state_vector->r.z;

    long double e_2 = 1 - ((model->ellipsoid.b*model->ellipsoid.b)/(model->ellipsoid.a*model->ellipsoid.a));
    long double sin_of_latitude = sin((latitude * M_PI/180));
    long double n_phi = model->ellipsoid.a/(sqrt(1-(e_2 * (sin_of_latitude*sin_of_latitude))));

    retval->r.x = (n_phi + altitude) * cos(latitude * M_PI/180) * cos(longitude * M_PI/180);
    retval->r.y = (n_phi + altitude) * cos(latitude * M_PI/180) * sin(longitude * M_PI/180);
    retval->r.z = ((1 - e_2) * n_phi + altitude) * sin(latitude * M_PI/180);

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