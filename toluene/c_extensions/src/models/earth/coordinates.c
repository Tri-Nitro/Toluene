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

#define __compile_models_earth_earth_rotation
#define __compile_models_earth_nutation
#define __compile_models_earth_polar_motion
#define __compile_models_earth_precession
#include "models/earth/bias.h"
#include "models/earth/coefficients.h"
#include "models/earth/coordinates.h"
#include "models/earth/earth_rotation.h"
#include "models/earth/ellipsoid.h"
#include "models/earth/equinox.h"
#include "models/earth/model.h"
#include "models/earth/nutation.h"
#include "models/earth/polar_motion.h"
#include "models/earth/precession.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* eci_to_ecef(PyObject *self, PyObject *args) {

    PyObject* capsule;
    EarthModel* model;

    double x, y, z, tt;
    double v_x, v_y, v_z;
    double a_x, a_y, a_z;

    if(!PyArg_ParseTuple(args, "ddddddddddO", &x, &y, &z, &v_x, &v_y, &v_z, &a_x, &a_y, &a_z, &tt, &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_to_eci()");
        return PyErr_Occurred();
    }

    printf("x: %f, y: %f, z: %f, v_x: %f, v_y: %f, v_z: %f, a_x: %f, a_y: %f, a_z: %f, tt: %f\n", x, y, z, v_x, v_y, v_z, a_x, a_y, a_z, tt);

    model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    printf("model: %p\n", model);
    if(!model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    Matrix matrix;
    matrix.nrows = 3;
    matrix.ncols = 3;
    matrix.elements = (double*)malloc(sizeof(double)*9);
    if(!matrix.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for matrix.");
        return PyErr_Occurred();
    }

    Vector vec_r, vec_v, vec_a;
    vec_r.nelements = 3;
    vec_v.nelements = 3;
    vec_a.nelements = 3;
    vec_r.elements = (double*)malloc(sizeof(double)*3);
    vec_v.elements = (double*)malloc(sizeof(double)*3);
    vec_a.elements = (double*)malloc(sizeof(double)*3);
    if(!vec_r.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for position vector.");
        return PyErr_Occurred();
    }
    if(!vec_v.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for velocity vector.");
        return PyErr_Occurred();
    }
    if(!vec_a.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for acceleration vector.");
        return PyErr_Occurred();
    }

    vec_r.elements[0] = x;
    vec_r.elements[1] = y;
    vec_r.elements[2] = z;
    vec_v.elements[0] = x;
    vec_v.elements[1] = y;
    vec_v.elements[2] = z;
    vec_a.elements[0] = x;
    vec_a.elements[1] = y;
    vec_a.elements[2] = z;

    Vector vec_r_prime, vec_v_prime, vec_a_prime;
    vec_r_prime.nelements = 3;
    vec_v_prime.nelements = 3;
    vec_a_prime.nelements = 3;
    vec_r_prime.elements = (double*)malloc(sizeof(double)*3);
    vec_v_prime.elements = (double*)malloc(sizeof(double)*3);
    vec_a_prime.elements = (double*)malloc(sizeof(double)*3);
    if(!vec_r_prime.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for position prime vector.");
        return PyErr_Occurred();
    }
    if(!vec_v_prime.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for velocity prime vector.");
        return PyErr_Occurred();
    }
    if(!vec_a_prime.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for acceleration prime vector.");
        return PyErr_Occurred();
    }

    tt = tt - model->epoch;
    double delta_psi, delta_epsilon, epsilon, eq_eq;
    get_delta_psi_delta_epsilon_epsilon_eq_eq(tt, model, &delta_psi, &delta_epsilon, &epsilon, &eq_eq);

    icrs_to_mean_j2000_bias_approximation(model->cirs_coefficients, &matrix);
    dot_product_matrix_transpose(&vec_r, &matrix, &vec_r_prime);
    dot_product_matrix_transpose(&vec_v, &matrix, &vec_v_prime);
    dot_product_matrix_transpose(&vec_a, &matrix, &vec_a_prime);

    iau_2000a_precession(tt, model, &matrix);
    dot_product_matrix_transpose(&vec_r_prime, &matrix, &vec_r);
    dot_product_matrix_transpose(&vec_v_prime, &matrix, &vec_v);
    dot_product_matrix_transpose(&vec_a_prime, &matrix, &vec_a);

    iau_2006_nutation(delta_psi, delta_epsilon, epsilon, &matrix);
    dot_product_matrix_transpose(&vec_r, &matrix, &vec_r_prime);
    dot_product_matrix_transpose(&vec_v, &matrix, &vec_v_prime);
    dot_product_matrix_transpose(&vec_a, &matrix, &vec_a_prime);

    tirs_to_true_equinox_equator_earth_rotation(tt, eq_eq, model, &matrix);
    dot_product_matrix_transpose(&vec_r_prime, &matrix, &vec_r);

    tirs_to_true_equinox_equator_earth_rotation_rate(tt, eq_eq, model, &matrix);
    dot_product_matrix_transpose(&vec_v_prime, &matrix, &vec_v);

    itrs_to_tirs_polar_motion_approximation(tt, model, &matrix);
    dot_product_matrix_transpose(&vec_r, &matrix, &vec_r_prime);
    dot_product_matrix_transpose(&vec_v, &matrix, &vec_v_prime);
    dot_product_matrix_transpose(&vec_a, &matrix, &vec_a_prime);

    x = vec_r_prime.elements[0];
    y = vec_r_prime.elements[1];
    z = vec_r_prime.elements[2];
    v_x += vec_v_prime.elements[0];
    v_y += vec_v_prime.elements[1];
    v_z += vec_v_prime.elements[2];
    a_x = vec_a_prime.elements[0];
    a_y = vec_a_prime.elements[1];
    a_z = vec_a_prime.elements[2];

    free(matrix.elements);
    free(vec_r.elements);
    free(vec_r_prime.elements);
    free(vec_v.elements);
    free(vec_v_prime.elements);
    free(vec_a.elements);
    free(vec_a_prime.elements);

    return Py_BuildValue("(ddddddddd)", x, y, z, v_x, v_y, v_z, a_x, a_y, a_z);
}


static PyObject* ecef_to_eci(PyObject *self, PyObject *args) {

    PyObject* capsule;
    EarthModel* model;

    double x, y, z, tt;
    double v_x, v_y, v_z;
    double a_x, a_y, a_z;

    if(!PyArg_ParseTuple(args, "ddddddddddO", &x, &y, &z, &v_x, &v_y, &v_z, &a_x, &a_y, &a_z, &tt, &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_to_eci()");
        return PyErr_Occurred();
    }

    printf("x: %f, y: %f, z: %f, v_x: %f, v_y: %f, v_z: %f, a_x: %f, a_y: %f, a_z: %f, tt: %f\n", x, y, z, v_x, v_y, v_z, a_x, a_y, a_z, tt);

    model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    printf("model: %p\n", model);
    if(!model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    Matrix matrix;
    matrix.nrows = 3;
    matrix.ncols = 3;
    matrix.elements = (double*)malloc(sizeof(double)*9);
    if(!matrix.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for matrix.");
        return PyErr_Occurred();
    }

    Vector vec_r, vec_v, vec_a;
    vec_r.nelements = 3;
    vec_v.nelements = 3;
    vec_a.nelements = 3;
    vec_r.elements = (double*)malloc(sizeof(double)*3);
    vec_v.elements = (double*)malloc(sizeof(double)*3);
    vec_a.elements = (double*)malloc(sizeof(double)*3);
    if(!vec_r.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for position vector.");
        return PyErr_Occurred();
    }
    if(!vec_v.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for velocity vector.");
        return PyErr_Occurred();
    }
    if(!vec_a.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for acceleration vector.");
        return PyErr_Occurred();
    }

    vec_r.elements[0] = x;
    vec_r.elements[1] = y;
    vec_r.elements[2] = z;
    vec_v.elements[0] = x;
    vec_v.elements[1] = y;
    vec_v.elements[2] = z;
    vec_a.elements[0] = x;
    vec_a.elements[1] = y;
    vec_a.elements[2] = z;

    Vector vec_r_prime, vec_v_prime, vec_a_prime;
    vec_r_prime.nelements = 3;
    vec_v_prime.nelements = 3;
    vec_a_prime.nelements = 3;
    vec_r_prime.elements = (double*)malloc(sizeof(double)*3);
    vec_v_prime.elements = (double*)malloc(sizeof(double)*3);
    vec_a_prime.elements = (double*)malloc(sizeof(double)*3);
    if(!vec_r_prime.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for position prime vector.");
        return PyErr_Occurred();
    }
    if(!vec_v_prime.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for velocity prime vector.");
        return PyErr_Occurred();
    }
    if(!vec_a_prime.elements) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for acceleration prime vector.");
        return PyErr_Occurred();
    }

    tt = tt - model->epoch;
    double delta_psi, delta_epsilon, epsilon, eq_eq;
    get_delta_psi_delta_epsilon_epsilon_eq_eq(tt, model, &delta_psi, &delta_epsilon, &epsilon, &eq_eq);

    itrs_to_tirs_polar_motion_approximation(tt, model, &matrix);
    dot_product(&vec_r, &matrix, &vec_r_prime);
    dot_product(&vec_v, &matrix, &vec_v_prime);
    dot_product(&vec_a, &matrix, &vec_a_prime);

    tirs_to_true_equinox_equator_earth_rotation(tt, eq_eq, model, &matrix);
    dot_product(&vec_r_prime, &matrix, &vec_r);

    tirs_to_true_equinox_equator_earth_rotation_rate(tt, eq_eq, model, &matrix);
    dot_product(&vec_v_prime, &matrix, &vec_v);

    iau_2006_nutation(delta_psi, delta_epsilon, epsilon, &matrix);
    dot_product(&vec_r, &matrix, &vec_r_prime);
    dot_product(&vec_v, &matrix, &vec_v_prime);
    dot_product(&vec_a, &matrix, &vec_a_prime);

    iau_2000a_precession(tt, model, &matrix);
    dot_product(&vec_r_prime, &matrix, &vec_r);
    dot_product(&vec_v_prime, &matrix, &vec_v);
    dot_product(&vec_a_prime, &matrix, &vec_a);

    icrs_to_mean_j2000_bias_approximation(model->cirs_coefficients, &matrix);
    dot_product(&vec_r, &matrix, &vec_r_prime);
    dot_product(&vec_v, &matrix, &vec_v_prime);
    dot_product(&vec_a, &matrix, &vec_a_prime);

    x = vec_r_prime.elements[0];
    y = vec_r_prime.elements[1];
    z = vec_r_prime.elements[2];
    v_x += vec_v_prime.elements[0];
    v_y += vec_v_prime.elements[1];
    v_z += vec_v_prime.elements[2];
    a_x = vec_a_prime.elements[0];
    a_y = vec_a_prime.elements[1];
    a_z = vec_a_prime.elements[2];

    free(matrix.elements);
    free(vec_r.elements);
    free(vec_r_prime.elements);
    free(vec_v.elements);
    free(vec_v_prime.elements);
    free(vec_a.elements);
    free(vec_a_prime.elements);

    return Py_BuildValue("(ddddddddd)", x, y, z, v_x, v_y, v_z, a_x, a_y, a_z);
}


static PyObject* ecef_to_lla(PyObject *self, PyObject *args) {

    PyObject* capsule;
    EarthModel* earth_model;
    Ellipsoid* ellipsoid;

    double x, y, z;

    if(!PyArg_ParseTuple(args, "dddO", &x, &y, &z, &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. lla_from_ecef()");
        return PyErr_Occurred();
    }
    
    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }
    ellipsoid = earth_model->ellipsoid;
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from EarthModel.");
        return PyErr_Occurred();
    }

    // Causes a divide by 0 bug because of p if x and y are 0. Just put a little offset so longitude can be set if
    // directly above the pole.
    if(x == 0 && y == 0) x = 0.000000001;

    double e_numerator = ellipsoid->a*ellipsoid->a - ellipsoid->b*ellipsoid->b;
    double e_2 = e_numerator/(ellipsoid->a*ellipsoid->a);
    double e_r2 = e_numerator/(ellipsoid->b*ellipsoid->b);
    double p = sqrt(x*x+y*y);
    double big_f = 54.0*ellipsoid->b*ellipsoid->b*z*z;
    double big_g = p*p+z*z*(1-e_2)-e_2*e_numerator;
    double c = (e_2*e_2*big_f*p*p)/(big_g*big_g*big_g);
    double s = cbrt(1+c+sqrt(c*c+2*c));
    double k = s+1+1/s;
    double big_p = big_f/(3*k*k*big_g*big_g);
    double big_q = sqrt(1 + 2 * e_2 * e_2 * big_p);
    double sqrt_r_0 = (ellipsoid->a*ellipsoid->a/2)*(1+1/big_q)-((big_p*(1-e_2)*z*z)/(big_q*(1+big_q)))
                 -(big_p*p*p)/2;
    sqrt_r_0 = (sqrt_r_0 < 0? 0 : sqrt(sqrt_r_0));
    double r_0 = ((-1* big_p*e_2*p)/(1+big_q)) + sqrt_r_0;
    double p_e_2_r_0 = p-e_2*r_0;
    double big_u = sqrt( p_e_2_r_0*p_e_2_r_0+z*z);
    double big_v = sqrt(p_e_2_r_0*p_e_2_r_0+(1-e_2)*z*z);
    double z_0 = (ellipsoid->b*ellipsoid->b*z)/(ellipsoid->a*big_v);

    double latitude = atan((z+(e_r2*z_0))/p) * 180/M_PI;
    double longitude = atan2(y,x) * 180/M_PI;
    double altitude = big_u * (1-(ellipsoid->b*ellipsoid->b)/(ellipsoid->a*big_v));

    return Py_BuildValue("(ddd)", latitude, longitude, altitude);
}


static PyObject* lla_to_ecef(PyObject *self, PyObject *args) {

    PyObject* capsule;
    EarthModel* earth_model;
    Ellipsoid* ellipsoid;

    double latitude, longitude, altitude;

    if(!PyArg_ParseTuple(args, "dddO", &latitude, &longitude, &altitude, &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. lla_to_ecef()");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }
    ellipsoid = earth_model->ellipsoid;
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from EarthModel.");
        return PyErr_Occurred();
    }

    double e_2 = 1 - ((ellipsoid->b*ellipsoid->b)/(ellipsoid->a*ellipsoid->a));
    double sin_of_latitude = sin((latitude * M_PI/180));
    double n_phi = ellipsoid->a/(sqrt(1-(e_2 * (sin_of_latitude*sin_of_latitude))));

    double x = (n_phi + altitude) * cos(latitude * M_PI/180) * cos(longitude * M_PI/180);
    double y = (n_phi + altitude) * cos(latitude * M_PI/180) * sin(longitude * M_PI/180);
    double z = ((1 - e_2) * n_phi + altitude) * sin(latitude * M_PI/180);

    return Py_BuildValue("(ddd)", x, y, z);
}


static PyMethodDef tolueneModelsEarthEllipsoidMethods[] = {
    {"eci_to_ecef", eci_to_ecef, METH_VARARGS, "Converts ECI to ECEF."},
    {"ecef_to_eci", ecef_to_eci, METH_VARARGS, "Converts ECEF to ECI."},
    {"lla_to_ecef", lla_to_ecef, METH_VARARGS, "Converts LLA to ECEF."},
    {"ecef_to_lla", ecef_to_lla, METH_VARARGS, "Converts ECEF to LLA."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_coordinates = {
    PyModuleDef_HEAD_INIT,
    "models.earth.coordinates",
    "C Extensions to toluene models ellipsoid functions",
    -1,
    tolueneModelsEarthEllipsoidMethods
};


PyMODINIT_FUNC PyInit_coordinates(void) {
    return PyModule_Create(&models_earth_coordinates);
}


#ifdef __cplusplus
} /* extern "C" */
#endif