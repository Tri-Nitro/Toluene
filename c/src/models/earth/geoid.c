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

#define __compile_models_earth_geoid__
#include "models/earth/geoid.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

static PyObject* new_Geoid(PyObject* self, PyObject* args) {

    Geoid* geoid = (Geoid*)malloc(sizeof(Geoid));

    if(!geoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_Geoid.");
        return PyErr_Occurred();
    }

    geoid->ninterpolation = 0;
    geoid->ncoefficients = 0;
    geoid->ninterpolation_allocated = 0;
    geoid->ncoefficients_allocated = 0;
    geoid->coefficients = NULL;
    geoid->interpolation = NULL;

    return PyCapsule_New(geoid, "Geoid", delete_Geoid);
}

void delete_Geoid(PyObject* self) {

    Geoid* geoid = (Geoid*)PyCapsule_GetPointer(self, "Geoid");

    if(geoid) {
        if(geoid->coefficients) {
            free(geoid->coefficients);
        }
        if(geoid->interpolation) {
            free(geoid->interpolation);
        }
        free(geoid);
    }
}

static PyObject* add_interpolation(PyObject* self, PyObject* args) {

    PyObject* capsule;
    Geoid* geoid;

    double latitude, longitude, height;

    if(!PyArg_ParseTuple(args, "Oddd", &capsule, &latitude, &longitude, &height)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. add_interpolation()");
        return PyErr_Occurred();
    }

    geoid = (Geoid*)PyCapsule_GetPointer(capsule, "Geoid");
    if(!geoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Geoid from capsule.");
        return PyErr_Occurred();
    }

    if(geoid->ninterpolation_allocated < geoid->ninterpolation + 1) {
        geoid->ninterpolation_allocated += 1000;
        InterpolationGrid* new_interpolation =
            (InterpolationGrid*)malloc(sizeof(InterpolationGrid) * geoid->ninterpolation_allocated);
        memcpy(new_interpolation, geoid->interpolation, sizeof(InterpolationGrid) * geoid->ninterpolation);
        free(geoid->interpolation);
        geoid->interpolation = new_interpolation;
    }

    int insertion_point = geoid->ninterpolation;
    while(insertion_point > 0 && geoid->interpolation[insertion_point-1].lat > latitude) {
        geoid->interpolation[insertion_point] = geoid->interpolation[insertion_point-1];
        insertion_point--;
    }
    while(insertion_point > 0 && geoid->interpolation[insertion_point-1].lat == latitude && geoid->interpolation[insertion_point-1].lon > longitude) {
        geoid->interpolation[insertion_point] = geoid->interpolation[insertion_point-1];
        insertion_point--;
    }

    geoid->interpolation[insertion_point].lat = latitude;
    geoid->interpolation[insertion_point].lon = longitude;
    geoid->interpolation[insertion_point].height = height;
    geoid->ninterpolation++;

    return Py_BuildValue("i", geoid->ninterpolation);
}

static PyObject* add_coefficients(PyObject* self, PyObject* args) {

    PyObject* capsule;
    Geoid* geoid;

    int degree, order;
    double C, S, C_dot, S_dot;

    if(!PyArg_ParseTuple(args, "Oiidddd", &capsule, &degree, &order, &C, &S, &C_dot, &S_dot)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. add_interpolation()");
        return PyErr_Occurred();
    }

    geoid = (Geoid*)PyCapsule_GetPointer(capsule, "Geoid");
    if(!geoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Geoid from capsule.");
        return PyErr_Occurred();
    }

    if(geoid->ncoefficients_allocated < geoid->ncoefficients + 1) {
        geoid->ncoefficients_allocated += 1000;
        GeoidCoefficient* new_coefficients =
            (GeoidCoefficient*)malloc(sizeof(GeoidCoefficient) * geoid->ncoefficients_allocated);
        memcpy(new_coefficients, geoid->coefficients, sizeof(GeoidCoefficient) * geoid->ncoefficients);
        free(geoid->coefficients);
        geoid->coefficients = new_coefficients;
    }

    int insertion_point = geoid->ncoe;
    while(insertion_point > 0 && geoid->interpolation[insertion_point-1].lat > latitude) {
        geoid->interpolation[insertion_point] = geoid->interpolation[insertion_point-1];
        insertion_point--;
    }
    while(insertion_point > 0 && geoid->interpolation[insertion_point-1].lat == latitude && geoid->interpolation[insertion_point-1].lon > longitude) {
        geoid->interpolation[insertion_point] = geoid->interpolation[insertion_point-1];
        insertion_point--;
    }
}

static PyMethodDef tolueneModelsEarthEllipsoidMethods[] = {
    {"set_axes", set_axes, METH_VARARGS, "Sets the ellipsoid axes."},
    {"get_axes", get_axes, METH_VARARGS, "Gets the ellipsoid axes."},
    {"set_semi_major_axis", set_semi_major_axis, METH_VARARGS, "Sets the ellipsoid semi-major axis."},
    {"get_semi_major_axis", get_semi_major_axis, METH_VARARGS, "Gets the ellipsoid semi-major axis."},
    {"set_semi_minor_axis", set_semi_minor_axis, METH_VARARGS, "Sets the ellipsoid semi-minor axis."},
    {"get_semi_minor_axis", get_semi_minor_axis, METH_VARARGS, "Gets the ellipsoid semi-minor axis."},
    {"get_flattening", get_flattening, METH_VARARGS, "Gets the ellipsoid flattening."},
    {"get_eccentricity_squared", get_eccentricity_squared, METH_VARARGS, "Calculates the eccentricity squared."},
    {"get_ellipsoid_radius", get_ellipsoid_radius, METH_VARARGS, "Gets the ellipsoid radius at a given latitude."},
    {"new_Ellipsoid", new_Ellipsoid, METH_VARARGS, "Create a new Ellipsoid object"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_ellipsoid = {
    PyModuleDef_HEAD_INIT,
    "models.earth.ellipsoid",
    "C Extensions to working with an ellipsoid model.",
    -1,
    tolueneModelsEarthEllipsoidMethods
};


PyMODINIT_FUNC PyInit_ellipsoid(void) {
    return PyModule_Create(&models_earth_ellipsoid);
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */