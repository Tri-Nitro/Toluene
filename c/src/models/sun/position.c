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

#include "math/constants.h"
#include "models/earth/constants.h"
#include "models/sun/constants.h"
#include "models/sun/position.h"
#include "time/constants.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
 * @brief Get the sun position object
 *
 * @param time The time in seconds
 * @param x The x position of the sun
 * @param y The y position of the sun
 * @param z The z position of the sun
 */
void sun_position(long double t, long double* x, long double* y, long double* z) {

    t = (t-J2000_UNIX_TIME)/ SECONDS_PER_JULIAN_CENTURY;
    printf("t: %Lf\n", t);

    long double L = ((MEAN_LONGITUDE_SUN[2] * t + MEAN_LONGITUDE_SUN[1]) * t + MEAN_LONGITUDE_SUN[0]) * M_PI / 180.0;
    long double M = ((((MEAN_ANOMALY_SUN[4] * t + MEAN_ANOMALY_SUN[3]) * t + MEAN_ANOMALY_SUN[2]) * t +
        MEAN_ANOMALY_SUN[1]) * t + MEAN_ANOMALY_SUN[0]) * ARCSECONDS_TO_RADIANS;
    long double e = (((((OBLIQUITY_MEAN_EQUATOR[5] * t + OBLIQUITY_MEAN_EQUATOR[4]) * t + OBLIQUITY_MEAN_EQUATOR[3]) * t
        + OBLIQUITY_MEAN_EQUATOR[2]) * t + OBLIQUITY_MEAN_EQUATOR[1]) * t + OBLIQUITY_MEAN_EQUATOR[0]) *
        ARCSECONDS_TO_RADIANS;

    L += 0.033423055 * sin(M) + 0.0003490659 * sin(2 * M);
    long double magnitude = (1.00014 - 0.01671 * cosl(M) - 0.00014 * cosl(2*M)) * ASTRO_UNIT_TO_METERS;

    *x = magnitude * cosl(L);
    *y = magnitude * cosl(e) * sinl(L);
    *z = magnitude * sinl(e) * sinl(L);

}

/**
 * @brief Get the sun position object
 */
static PyObject* get_sun_position(PyObject* self, PyObject* args) {

    double time;
    long double x, y, z;

    if (!PyArg_ParseTuple(args, "d", &time)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. get_sun_position(double time)");
        return NULL;
    }

    sun_position(time, &x, &y, &z);

    return Py_BuildValue("ddd", (double)x, (double)y, (double)z);
}

static PyMethodDef tolueneModelsSunPositionMethods[] = {
    {"get_sun_position", get_sun_position, METH_VARARGS, "Get the position of the sun"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_sun_position = {
    PyModuleDef_HEAD_INIT,
    "models.sun.position",
    "C Extensions for computing the sun position",
    -1,
    tolueneModelsSunPositionMethods
};


PyMODINIT_FUNC PyInit_position(void) {
    return PyModule_Create(&models_sun_position);
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */