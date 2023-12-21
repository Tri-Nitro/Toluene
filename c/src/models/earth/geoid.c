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

    geoid->interpolation_spacing = 0.0;
    geoid->interpolation = NULL;
    geoid->ncoefficients = 0;
    geoid->ncoefficients_allocated = 0;
    geoid->coefficients = NULL;

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

        PyObject* pList;
        PyObject* pItem;
        PyObject* capsule;
        Geoid* geoid = NULL;
        double spacing;

        if(!PyArg_ParseTuple(args, "OdO!", &capsule, &spacing, &PyList_Type, &pList)) {
            PyErr_SetString(PyExc_TypeError, "Unable to parse arguments.");
            return NULL;
        }

        geoid = (Geoid*)PyCapsule_GetPointer(capsule, "Geoid");

        if(geoid->interpolation) {
            free(geoid->interpolation);
        }

        geoid->interpolation_spacing = spacing;
        geoid->interpolation = (double*)malloc(sizeof(double) * 360/spacing * 180/spacing);

        geoid = (Geoid*)PyCapsule_GetPointer(capsule, "Geoid");

        Py_ssize_t n = PyList_Size(pList);
        if(n != (int)((360/spacing + 1) * (180/spacing + 1))) {
            PyErr_SetString(PyExc_TypeError, "list must be (360/spacing + 1) * (180/spacing + 1) in length.");
            return NULL;
        }
        for (int i = 0; i < n; ++i) {
            pItem = PyList_GetItem(pList, i);
            if(!PyFloat_Check(pItem)) {
                PyErr_SetString(PyExc_TypeError, "list items must be floats.");
                return NULL;
            }
            geoid->interpolation[i] = PyFloat_AsDouble(pItem);
        }

        return Py_None;
}

static PyObject* add_coefficients(PyObject* self, PyObject* args) {

}

static PyMethodDef tolueneModelsEarthGeoidMethods[] = {
    {"new_Geoid", new_Geoid, METH_VARARGS, "Create a new Geoid."},
    {"add_interpolation", add_interpolation, METH_VARARGS, "Add an interpolation point to the Geoid."},
    {"add_coefficients", add_coefficients, METH_VARARGS, "Add a coefficient to the Geoid."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_geoid = {
    PyModuleDef_HEAD_INIT,
    "models.earth.geoid",
    "C Extensions to working with Geoids in Toluene..",
    -1,
    tolueneModelsEarthGeoidMethods
};


PyMODINIT_FUNC PyInit_geoid(void) {
    return PyModule_Create(&models_earth_geoid);
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */