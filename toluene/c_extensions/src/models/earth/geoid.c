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

#define __compile_models_earth_geoid
#include "models/earth/geoid.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* new_Geoid(PyObject* self, PyObject* args) {

    Geoid* geoid = (Geoid*)malloc(sizeof(Geoid));

    if(!geoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_Geoid.");
        return PyErr_Occurred();
    }

    geoid->harmonics = NULL;
    geoid->grid = NULL;

    return PyCapsule_New(geoid, "Geoid", delete_Geoid);
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