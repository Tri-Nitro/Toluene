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

#define __compile_models_earth_ellipsoid
#include "models/earth/ellipsoid.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* set_axes(PyObject* self, PyObject* args) {

    PyObject* capsule;
    Ellipsoid* ellipsoid;
    double a, b;

    if(!PyArg_ParseTuple(args, "Odd", &capsule, &a, &b)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for set_axes.");
        return PyErr_Occurred();
    }

    ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(capsule, "Ellipsoid");
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from capsule.");
        return PyErr_Occurred();
    }

    ellipsoid->a = a;
    ellipsoid->b = b;

    Py_RETURN_NONE;
}


static PyObject* get_axes(PyObject* self, PyObject* args) {

    PyObject* capsule;
    Ellipsoid* ellipsoid;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for set_axes.");
        return PyErr_Occurred();
    }

    ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(capsule, "Ellipsoid");
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from capsule.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("dd", ellipsoid->a, ellipsoid->b);
}


static PyObject* ellipsoid_radius(PyObject *self, PyObject *args) {

    PyObject* capsule = NULL;
    Ellipsoid* ellipsoid = NULL;
    double latitude = 0.0;

    if(!PyArg_ParseTuple(args, "Od", &capsule, &latitude)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ellipsoid_radius(Ellipsoid, latitude).");
        return PyErr_Occurred();
    }

    ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(capsule, "Ellipsoid");
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from capsule.");
        return PyErr_Occurred();
    }

    double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0;

    f3 = ellipsoid->a * cos(latitude * M_PI/180);
    f4 = ellipsoid->b * sin(latitude * M_PI/180);
    f1 = ellipsoid->a * f3;
    f1 = f1 * f1;
    f3 = f3 * f3;
    f2 = ellipsoid->b * f4;
    f2 = f2 * f2;
    f4 = f4 * f4;

    double retval = sqrt((f1 + f2) / (f3 + f4));
    return Py_BuildValue("d", retval);
}


static PyObject* new_Ellipsoid(PyObject* self, PyObject* args) {

    Ellipsoid* ellipsoid = (Ellipsoid*)malloc(sizeof(Ellipsoid));

    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_Ellipsoid.");
        return PyErr_Occurred();
    }

    ellipsoid->a = 0;
    ellipsoid->b = 0;

    return PyCapsule_New(ellipsoid, "Ellipsoid", delete_Ellipsoid);
}


static void delete_Ellipsoid(PyObject* obj) {

    Ellipsoid* ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(obj, "Ellipsoid");

    if(ellipsoid) {
        free(ellipsoid);
    }

}


static PyMethodDef tolueneModelsEarthEllipsoidMethods[] = {
    {"set_axes", set_axes, METH_VARARGS, "Sets the ellipsoid axes."},
    {"get_axes", get_axes, METH_VARARGS, "Gets the ellipsoid axes."},
    {"ellipsoid_radius", ellipsoid_radius, METH_VARARGS, "Gets the ellipsoid radius at a given latitude."},
    {"new_Ellipsoid", new_Ellipsoid, METH_VARARGS, "Create a new Ellipsoid object"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_ellipsoid = {
    PyModuleDef_HEAD_INIT,
    "models.earth.ellipsoid",
    "C Extensions to toluene models ellipsoid functions",
    -1,
    tolueneModelsEarthEllipsoidMethods
};


PyMODINIT_FUNC PyInit_ellipsoid(void) {
    return PyModule_Create(&models_earth_ellipsoid);
}


#ifdef __cplusplus
} /* extern "C" */
#endif