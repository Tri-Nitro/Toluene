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

#define __compile_models_earth_model
#include "models/earth/model.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* get_ellipsoid(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. get_ellipsoid(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->ellipsoid, "Ellipsoid", NULL);
}


static PyObject* set_ellipsoid(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* ellipsoid_capsule;

    EarthModel* earth_model;
    Ellipsoid* ellipsoid;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &ellipsoid_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. set_ellipsoid(EarthModel, Ellipsoid)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(ellipsoid_capsule, "Ellipsoid");
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from capsule.");
        return PyErr_Occurred();
    }

    earth_model->ellipsoid = ellipsoid;

    Py_RETURN_NONE;
}


static PyObject* get_cirs_coefficients(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. cirs_coefficients(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->cirs_coefficients, "CIRSCoefficients", NULL);
}


static PyObject* set_cirs_coefficients(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* cirs_coefficients_capsule;

    EarthModel* earth_model;
    CIRSCoefficients* cirs_coefficients;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &cirs_coefficients_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. set_cirs_coefficients(EarthModel, CIRSCoefficients)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    cirs_coefficients = (CIRSCoefficients*)PyCapsule_GetPointer(cirs_coefficients_capsule, "CIRSCoefficients");
    if(!cirs_coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    earth_model->cirs_coefficients = cirs_coefficients;

    Py_RETURN_NONE;
}


static PyObject* get_eop_table(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. eop_table(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->eop_table, "EOPTable", NULL);
}


static PyObject* set_eop_table(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* eop_table_capsule;

    EarthModel* earth_model;
    EOPTable* eop_table;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &eop_table_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. set_eop_table(EarthModel, EOPTable)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    eop_table = (EOPTable*)PyCapsule_GetPointer(eop_table_capsule, "EOPTable");
    if(!eop_table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EOPTable from capsule.");
        return PyErr_Occurred();
    }

    earth_model->eop_table = eop_table;

    Py_RETURN_NONE;
}


static PyObject* new_EarthModel(PyObject* self, PyObject* args) {

    EarthModel* model = (EarthModel*)malloc(sizeof(EarthModel));

    if(model == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_EarthModel.");
        return PyErr_Occurred();
    }

    model->ellipsoid = NULL;
    model->geoid = NULL;
    model->cirs_coefficients = NULL;
    model->eop_table = NULL;

    return PyCapsule_New(model, "EarthModel", delete_EarthModel);
}


static void delete_EarthModel(PyObject* obj) {

    EarthModel* pointer = (EarthModel*)PyCapsule_GetPointer(obj, "EarthModel");

    if(pointer) free(pointer);

}


static PyMethodDef tolueneModelsEarthModelMethods[] = {
    {"get_ellipsoid", get_ellipsoid, METH_VARARGS, "Get the ellipsoid of the EarthModel"},
    {"set_ellipsoid", set_ellipsoid, METH_VARARGS, "Set the ellipsoid of the EarthModel"},
    {"get_cirs_coefficients", get_cirs_coefficients, METH_VARARGS, "Get the CIRSCoefficients of the EarthModel"},
    {"set_cirs_coefficients", set_cirs_coefficients, METH_VARARGS, "Set the CIRSCoefficients of the EarthModel"},
    {"get_eop_table", get_eop_table, METH_VARARGS, "Get the EOPTable of the EarthModel"},
    {"set_eop_table", set_eop_table, METH_VARARGS, "Set the EOPTable of the EarthModel"},
    {"new_EarthModel", new_EarthModel, METH_VARARGS, "Create a new EarthModel object"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_model = {
    PyModuleDef_HEAD_INIT,
    "models.earth.model",
    "C Extensions to toluene models earth functions",
    -1,
    tolueneModelsEarthModelMethods
};


PyMODINIT_FUNC PyInit_model(void) {
    return PyModule_Create(&models_earth_model);
}


#ifdef __cplusplus
} /* extern "C" */
#endif