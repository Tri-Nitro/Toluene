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


static PyObject* get_nutation_series(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. nutation_series(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->nutation_series, "NutationSeries", NULL);
}


static PyObject* set_nutation_series(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* nutation_series_capsule;

    EarthModel* earth_model;
    NutationSeries* nutation_series;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &nutation_series_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. set_nutation_series(EarthModel, NutationSeries)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    nutation_series = (NutationSeries*)PyCapsule_GetPointer(nutation_series_capsule, "NutationSeries");
    if(!nutation_series) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the NutationSeries from capsule.");
        return PyErr_Occurred();
    }

    earth_model->nutation_series = nutation_series;

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


static PyObject* get_delta_t_table(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. delta_t_table(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->delta_t_table, "DeltaTTable", NULL);
}


static PyObject* set_delta_t_table(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* delta_t_table_capsule;

    EarthModel* earth_model;
    DeltaTTable* delta_t_table;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &delta_t_table_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. set_delta_t_table(EarthModel, DeltaTTable)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    delta_t_table = (DeltaTTable*)PyCapsule_GetPointer(delta_t_table_capsule, "DeltaTTable");
    if(!delta_t_table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the DeltaTTable from capsule.");
        return PyErr_Occurred();
    }

    earth_model->delta_t_table = delta_t_table;

    Py_RETURN_NONE;
}


static PyObject* get_gmst_polynomial(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. delta_t_table(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->greenwich_mean_sidereal_time_polynomial, "Polynomial", NULL);
}


static PyObject* set_gmst_polynomial(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* greenwich_mean_sidereal_time_polynomial_capsule;

    EarthModel* earth_model;
    Polynomial* greenwich_mean_sidereal_time_polynomial;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &greenwich_mean_sidereal_time_polynomial_capsule)) {
        PyErr_SetString(PyExc_TypeError,
            "Unable to parse arguments. set_greenwich_mean_sidereal_time_polynomial(EarthModel, Polynomial)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    greenwich_mean_sidereal_time_polynomial = (Polynomial*)PyCapsule_GetPointer(
        greenwich_mean_sidereal_time_polynomial_capsule, "Polynomial");
    if(!greenwich_mean_sidereal_time_polynomial) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Polynomial from capsule.");
        return PyErr_Occurred();
    }

    earth_model->greenwich_mean_sidereal_time_polynomial = greenwich_mean_sidereal_time_polynomial;

    Py_RETURN_NONE;
}


static PyObject* get_epoch(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. delta_t_table(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("d", earth_model->epoch);
}


static PyObject* set_epoch(PyObject* self, PyObject* args) {

    PyObject* capsule;
    double epoch;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "Od", &capsule, &epoch)) {
        PyErr_SetString(PyExc_TypeError,
            "Unable to parse arguments. set_epoch(EarthModel, double)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    earth_model->epoch = epoch;

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
    model->delta_t_table = NULL;
    model->greenwich_mean_sidereal_time_polynomial = NULL;
    model->epoch = 0.0;

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
    {"get_nutation_series", get_nutation_series, METH_VARARGS, "Get the NutationSeries of the EarthModel"},
    {"set_nutation_series", set_nutation_series, METH_VARARGS, "Set the NutationSeries of the EarthModel"},
    {"get_eop_table", get_eop_table, METH_VARARGS, "Get the EOPTable of the EarthModel"},
    {"set_eop_table", set_eop_table, METH_VARARGS, "Set the EOPTable of the EarthModel"},
    {"get_delta_t_table", get_delta_t_table, METH_VARARGS, "Get the Delta T Table of the EarthModel"},
    {"set_delta_t_table", set_delta_t_table, METH_VARARGS, "Set the Delta T Table of the EarthModel"},
    {"get_gmst_polynomial", get_gmst_polynomial, METH_VARARGS, "Get the GMST Polynomial of the EarthModel"},
    {"set_gmst_polynomial", set_gmst_polynomial, METH_VARARGS, "Set the GMST Polynomial of the EarthModel"},
    {"get_epoch", get_epoch, METH_VARARGS, "Get the epoch of the EarthModel"},
    {"set_epoch", set_epoch, METH_VARARGS, "Set the epoch of the EarthModel"},
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