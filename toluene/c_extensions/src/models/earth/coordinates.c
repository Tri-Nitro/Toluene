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

#include "models/earth/coordinates.h"
#include "models/earth/ellipsoid.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* ecef_to_lla(PyObject *self, PyObject *args) {

    PyObject* capsule;
    Ellipsoid* ellipsoid;

    double x, y, z;

    if(!PyArg_ParseTuple(args, "dddO", &x, &y, &z, &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. lla_from_ecef()");
        return PyErr_Occurred();
    }
    
    ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(capsule, "Ellipsoid");
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from capsule.");
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
    Ellipsoid* ellipsoid;

    double latitude, longitude, altitude;

    if(!PyArg_ParseTuple(args, "dddO", &latitude, &longitude, &altitude, &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. lla_to_ecef()");
        return PyErr_Occurred();
    }

    ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(capsule, "Ellipsoid");
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from capsule.");
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