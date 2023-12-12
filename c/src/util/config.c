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

#include "util/config.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* load_delta_t_data(PyObject* self, PyObject* args) {
    return Py_BuildValue("d", 0);
}


static PyObject* load_iau_coefficients_data(PyObject* self, PyObject* args) {
    return Py_BuildValue("d", 0);
}


static PyObject* load_iers_earth_orientation_data(PyObject* self, PyObject* args) {
    return Py_BuildValue("d", 0);
}


static PyMethodDef tolueneUtilConfigMethods[] = {
    {"load_delta_t_data", load_delta_t_data, METH_VARARGS, "Load delta t data from a file"},
    {"load_iau_coefficients_data", load_iau_coefficients_data, METH_VARARGS, "Load iau coefficients data from a file"},
    {"load_iers_earth_orientation_data", load_iers_earth_orientation_data, METH_VARARGS, "Load iers earth orientation data from a file"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef util_config = {
    PyModuleDef_HEAD_INIT,
    "util.config",
    "C Extensions to toluene core class functions",
    -1,
    tolueneUtilConfigMethods
};


PyMODINIT_FUNC PyInit_config(void) {
    return PyModule_Create(&util_config);
}


#ifdef __cplusplus
} /* extern "C" */
#endif