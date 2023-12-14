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

#define __compile_models_earth_earth__
#include "models/earth/earth.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
 * @brief Creates a new Earth Model object and makes it available to Python
 */
static PyObject* new_EarthModel(PyObject* self, PyObject* args) {

    EarthModel* model = (Ellipsoid*)malloc(sizeof(EarthModel));

    if(!model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_EarthModel.");
        return PyErr_Occurred();
    }

    model->ellipsoid.a = 0.0;
    model->ellipsoid.b = 0.0;
    model->nutation_series.nrecords = 0;
    model->nutation_series.nrecords_allocated = 0;
    model->nutation_series.records = NULL;
    model->earth_orientation_parameters.nrecords = 0;
    model->earth_orientation_parameters.nrecords_allocated = 0;
    model->earth_orientation_parameters.records = NULL;

    return PyCapsule_New(model, "EarthModel", delete_EarthModel);
}

/**
 * @brief Deletes the Earth Model object
 *
 * @param obj The Earth Model object
 */
static void delete_EarthModel(PyObject* obj) {
    EarthModel* model = (EarthModel*)PyCapsule_GetPointer(obj, "EarthModel");

    if(model) {
        if(model->nutation_series.records) {
            free(model->nutation_series.records);
        }
        if(model->earth_orientation_parameters.records) {
            free(model->earth_orientation_parameters.records);
        }
        free(model);
    }
    model = NULL;

}

/**
 * @brief Set the Earth Model's Ellipsoid
 */
static PyObject* earth_model_set_ellipsoid(PyObject* self, PyObject* args) {

    PyObject* capsule;
    EarthModel* model;
    double a, b;

    if(!PyArg_ParseTuple(args, "Odd", &capsule, &a, &b)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments passed to earth_model_set_ellipsoid.");
        return PyErr_Occurred();
    }

    model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");

    model->ellipsoid.a = a;
    model->ellipsoid.b = b;

    Py_RETURN_NONE;
}

/**
 * @brief Set the Earth Model's Nutation Series
 */
static PyObject* earth_model_set_nutation_series(PyObject* self, PyObject* args) {

    PyObject* model_capsule;
    PyObject* nutation_capsule;
    EarthModel* model;
    NutationSeries* series;


    if(!PyArg_ParseTuple(args, "OO", &model_capsule, &nutation_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments passed to earth_model_set_nutation_series.");
        return PyErr_Occurred();
    }

    model = (EarthModel*)PyCapsule_GetPointer(model_capsule, "EarthModel");
    series = (NutationSeries*)PyCapsule_GetPointer(nutation_capsule, "NutationSeries");

    model->nutation_series.nrecords = series->nrecords;
    model->nutation_series.nrecords_allocated = series->nrecords_allocated;
    model->nutation_series.records = series->records;

    /* Kill their version of the series because now it's managed by earth model */
    series->nrecords = 0;
    series->nrecords_allocated = 0;
    series->records = NULL;

    Py_RETURN_NONE;
}

/**
 * @brief Set the Earth Model's Earth Orientation Parameters
 */
static PyObject* earth_model_set_earth_orientation_parameters(PyObject* self, PyObject* args) {

    PyObject* model_capsule;
    PyObject* earth_orientation_parameters_capsule;
    EarthModel* model;
    EOPTable* earth_orientation_parameters;


    if(!PyArg_ParseTuple(args, "OO", &model_capsule, &earth_orientation_parameters_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments passed to earth_model_set_earth_orientation_parameters.");
        return PyErr_Occurred();
    }

    model = (EarthModel*)PyCapsule_GetPointer(model_capsule, "EarthModel");
    earth_orientation_parameters = (EOPTable*)PyCapsule_GetPointer(earth_orientation_parameters_capsule, "EOPTable");

    model->earth_orientation_parameters.nrecords = earth_orientation_parameters->nrecords;
    model->earth_orientation_parameters.nrecords_allocated = earth_orientation_parameters->nrecords_allocated;
    model->earth_orientation_parameters.records = earth_orientation_parameters->records;

    /* Kill their version of the table because now it's managed by earth model */
    earth_orientation_parameters->nrecords = 0;
    earth_orientation_parameters->nrecords_allocated = 0;
    earth_orientation_parameters->records = NULL;

    Py_RETURN_NONE;
}

/**
 * @brief Get the Earth Model's Ellipsoid
 */
static PyObject* earth_model_get_ellipsoid(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}

/**
 * @brief Get the Earth Model's Nutation Series
 */
static PyObject* earth_model_get_nutation_series(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}

/**
 * @brief Get the Earth Model's Earth Orientation Parameters
 */
static PyObject* earth_model_get_earth_orientation_parameters(PyObject* self, PyObject* args) {
    Py_RETURN_NONE;
}


static PyMethodDef tolueneModelsEarthEarthMethods[] = {
    {"new_EarthModel", new_EarthModel, METH_VARARGS, "Creates a new Earth Model object."},
    {"set_ellipsoid", earth_model_set_ellipsoid, METH_VARARGS, "Set the Earth Model's Ellipsoid."},
    {"set_nutation_series", earth_model_set_nutation_series, METH_VARARGS,
        "Set the Earth Model's Nutation Series."},
    {"set_earth_orientation_parameters", earth_model_set_earth_orientation_parameters, METH_VARARGS,
        "Set the Earth Model's Earth Orientation Parameters."},
    {"get_ellipsoid", earth_model_get_ellipsoid, METH_VARARGS, "Get the Earth Model's Ellipsoid."},
    {"get_nutation_series", earth_model_get_nutation_series, METH_VARARGS,
        "Get the Earth Model's Nutation Series."},
    {"get_earth_orientation_parameters", earth_model_get_earth_orientation_parameters, METH_VARARGS,
        "Get the Earth Model's Earth Orientation Parameters."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_earth = {
    PyModuleDef_HEAD_INIT,
    "models.earth.earth",
    "C Extensions for working with an earth model.",
    -1,
    tolueneModelsEarthEarthMethods
};


PyMODINIT_FUNC PyInit_earth(void) {
    return PyModule_Create(&models_earth_earth);
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */