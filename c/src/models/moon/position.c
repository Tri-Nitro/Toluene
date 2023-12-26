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
#include "models/earth/earth.h"
#include "models/moon/constants.h"
#include "models/moon/position.h"
#include "models/sun/constants.h"
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
 * @brief Get the moon position object
 *
 * @param time The time in seconds
 * @param x The x position of the moon
 * @param y The y position of the moon
 * @param z The z position of the moon
 */
void moon_position(long double t, long double* x, long double* y, long double* z) {

    t = (t-J2000_UNIX_TIME)/ SECONDS_PER_JULIAN_CENTURY;

    long double mean_anomaly_moon = ((((MEAN_ANOMALY_MOON[4] * t + MEAN_ANOMALY_MOON[3]) * t + MEAN_ANOMALY_MOON[2]) * t
        + MEAN_ANOMALY_MOON[1]) * t + MEAN_ANOMALY_MOON[0]) * ARCSECONDS_TO_RADIANS;
    long double mean_anomaly_sun = ((((MEAN_ANOMALY_SUN[4] * t + MEAN_ANOMALY_SUN[3]) * t + MEAN_ANOMALY_SUN[2]) * t
        + MEAN_ANOMALY_SUN[1]) * t + MEAN_ANOMALY_SUN[0]) * ARCSECONDS_TO_RADIANS;
    long double mean_argument_latitude_moon = ((((MEAN_ARGUMENT_LATITUDE_MOON[4] * t + MEAN_ARGUMENT_LATITUDE_MOON[3]) *
        t + MEAN_ARGUMENT_LATITUDE_MOON[2]) * t + MEAN_ARGUMENT_LATITUDE_MOON[1]) * t + MEAN_ARGUMENT_LATITUDE_MOON[0])
        * ARCSECONDS_TO_RADIANS;
    long double mean_elongation_moon = ((((MEAN_ELONGATION_MOON_FROM_SUN[4] * t + MEAN_ELONGATION_MOON_FROM_SUN[3]) * t
        + MEAN_ELONGATION_MOON_FROM_SUN[2]) * t + MEAN_ELONGATION_MOON_FROM_SUN[1]) * t +
        MEAN_ELONGATION_MOON_FROM_SUN[0]) * ARCSECONDS_TO_RADIANS;
    long double e = (((((OBLIQUITY_MEAN_EQUATOR[5] * t + OBLIQUITY_MEAN_EQUATOR[4]) * t + OBLIQUITY_MEAN_EQUATOR[3]) * t
        + OBLIQUITY_MEAN_EQUATOR[2]) * t + OBLIQUITY_MEAN_EQUATOR[1]) * t + OBLIQUITY_MEAN_EQUATOR[0]) *
        ARCSECONDS_TO_RADIANS;

    long double eliptic_longitude = ((MEAN_LONGITUDE_MOON[2] * t + MEAN_LONGITUDE_MOON[1]) * t + MEAN_LONGITUDE_MOON[0]+
        6.29 * sinl(mean_anomaly_moon) - 1.27 * sinl(mean_anomaly_moon - 2 * mean_elongation_moon) +
        0.66 * sinl(2*mean_elongation_moon) + 0.21 * sinl(2*mean_anomaly_moon) -
        0.19 * sinl(mean_anomaly_sun) - 0.11 * sinl(2*mean_argument_latitude_moon)) * M_PI / 180.0;
        printf("eliptic_longitude: %Lf\n", fmod(eliptic_longitude * 180/M_PI, 360));
    long double eliptic_latitude = (5.13 * sinl(mean_elongation_moon) +
        0.28 * sinl(mean_anomaly_moon + mean_elongation_moon) -
        0.28 * sinl(mean_elongation_moon - mean_anomaly_moon) -
        0.17 * sinl(mean_elongation_moon - 2*mean_argument_latitude_moon)) * M_PI / 180.0;
        printf("eliptic_latitude: %Lf\n", fmod(eliptic_latitude * 180/M_PI, 360));
    long double horizontal_parallax = (MEAN_LUNAR_HORIZONTAL_PARALLAX * 180/M_PI +
        0.00518 * cosl(mean_anomaly_moon) +
        0.0095 * cosl(mean_anomaly_moon - 2*mean_elongation_moon) +
        0.0078 * cosl(2*mean_elongation_moon) + 0.0028 * cosl(2*mean_anomaly_moon)) * M_PI / 180.0;
        printf("horizontal_parallax: %Lf\n", fmod(horizontal_parallax * 180/M_PI, 360));

    long double magnitude = 1.0/ sinl(horizontal_parallax);

    *x = cosl(eliptic_latitude) * cosl(eliptic_longitude) * magnitude;
    *y = (cosl(e) * cosl(eliptic_latitude) * sinl(eliptic_longitude) - sinl(e) * sinl(eliptic_latitude)) * magnitude;
    *z = (sinl(e) * cosl(eliptic_latitude) * sinl(eliptic_longitude) + cosl(e) * sinl(eliptic_latitude)) * magnitude;

}

/**
 * @brief Get the moon position object
 */
static PyObject* get_moon_position(PyObject* self, PyObject* args) {

    PyObject* capsule;
    EarthModel* model;
    double time;
    long double x, y, z;

    if (!PyArg_ParseTuple(args, "Od", &capsule, &time)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. get_moon_position(double time)");
        return NULL;
    }

    model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if (!model) {
        PyErr_SetString(PyExc_TypeError, "Unable to get earth model from capsule");
        return NULL;
    }

    moon_position(time, &x, &y, &z);
    x *= model->ellipsoid.a;
    y *= model->ellipsoid.a;
    z *= model->ellipsoid.a;

    return Py_BuildValue("ddd", (double)x, (double)y, (double)z);
}

static PyMethodDef tolueneModelsMoonPositionMethods[] = {
    {"get_moon_position", get_moon_position, METH_VARARGS, "Get the position of the moon"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_moon_position = {
    PyModuleDef_HEAD_INIT,
    "models.moon.position",
    "C Extensions for computing the moon position",
    -1,
    tolueneModelsMoonPositionMethods
};


PyMODINIT_FUNC PyInit_position(void) {
    return PyModule_Create(&models_moon_position);
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */