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

#define __compile_math_algebra /* Used to get prototypes so we don't get warnings about missing definitions. */
#include "math/algebra.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void solve(Polynomial* polynomial, double x, double* value) {

    if(polynomial && value) {
        *value = 0.0;
        for(int i = polynomial->order; i >= 0; --i) {
            *value = (*value)*x + polynomial->coefficients[i];
        }
    }

}


static PyObject* new_Polynomial(PyObject* self, PyObject* args) {

    Polynomial* polynomial = (Polynomial*)malloc(sizeof(Polynomial));

    if(!polynomial) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_Polynomial.");
        return PyErr_Occurred();
    }

    PyObject* list;

    if(!PyArg_ParseTuple(args, "O!", &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for new_Polynomial.");
        return PyErr_Occurred();
    }

    polynomial->order = (int)PyList_Size(list);
    polynomial->coefficients = (double*)malloc(sizeof(double) * polynomial->order);

    for(int i = 0; i < polynomial->order; i++) {
        polynomial->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    return PyCapsule_New(polynomial, "Polynomial", delete_Polynomial);
}


void delete_Polynomial(PyObject* obj) {

    Polynomial* polynomial = (Polynomial*)PyCapsule_GetPointer(obj, "Polynomial");

    if(polynomial) {
        if(polynomial->coefficients) free(polynomial->coefficients);
        free(polynomial);
    }

}


static PyMethodDef tolueneMathAlgebraMethods[] = {
    {"new_Polynomial", new_Polynomial, METH_VARARGS, "Create a new Polynomial object."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef math_algebra = {
    PyModuleDef_HEAD_INIT,
    "math.algebra",
    "C Extensions to toluene models polynomial functions",
    -1,
    tolueneMathAlgebraMethods
};


PyMODINIT_FUNC PyInit_algebra(void) {
    return PyModule_Create(&math_algebra);
}


#ifdef __cplusplus
} /* extern "C" */
#endif