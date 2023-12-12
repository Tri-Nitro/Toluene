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

#define __compile_models_earth_nutation__
#include "models/earth/nutation.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Compute nutation values of date along with the equation of the equinoxes.
 */
void nutation_values_of_date(long double t, NutationSeries* series, long double* nutation_longitude,
    long double* nutation_obliquity, long double* mean_obliquity_date, long double* equation_of_the_equinoxes) {

}

/**
 * @brief Compute the nutation series.
 */
void nutation_matrix(long double mean_obliquity_date, long double nutation_longitude, long double true_obliquity_date,
    Mat3* nutation_matrix) {

    long double sin_delta_psi, cos_delta_psi, sin_epsilon_prime, cos_epsilon_prime, sin_epsilon, cos_epsilon;

    sin_epsilon = sin(mean_obliquity_date * M_PI/648000);
    cos_epsilon = cos(mean_obliquity_date * M_PI/648000);
    sin_delta_psi = -1 * sin(nutation_longitude * M_PI/648000);
    cos_delta_psi = cos(nutation_longitude * M_PI/648000);
    sin_epsilon_prime = -1 * sin(true_obliquity_date * M_PI/648000);
    cos_epsilon_prime = cos(true_obliquity_date * M_PI/648000);

    if(nutation_matrix) {
        nutation_matrix->w11 = cos_delta_psi;
        nutation_matrix->w12 = sin_delta_psi * cos_epsilon;
        nutation_matrix->w13 = sin_delta_psi * sin_epsilon;
        nutation_matrix->w21 = -1.0 * sin_delta_psi * cos_epsilon_prime;
        nutation_matrix->w22 = cos_epsilon_prime * cos_delta_psi * cos_epsilon - sin_epsilon * sin_epsilon_prime;
        nutation_matrix->w23 = cos_epsilon_prime * cos_delta_psi * sin_epsilon + cos_epsilon * sin_epsilon_prime;
        nutation_matrix->w31 = sin_delta_psi * sin_epsilon_prime;
        nutation_matrix->w32 = -1.0 * sin_epsilon_prime * cos_delta_psi * cos_epsilon - sin_epsilon * cos_epsilon_prime;
        nutation_matrix->w33 = -1.0 * sin_epsilon_prime * cos_delta_psi * sin_epsilon + cos_epsilon_prime * cos_epsilon;
    }
}

/**
 * @brief Add a record to the nutation series.
 */
static PyObject* nutation_series_add_record(PyObject* self, PyObject* args) {

    PyObject* capsule;
    NutationSeries* table;

    int a, b, c, d, e, f, g, h, j, k, m, n, o, r;
    double S, S_dot, C_prime, C, C_dot, S_prime;

    if(!PyArg_ParseTuple(args, "Oiiiiiiiiiiiiiidddddd", &capsule, &a, &b, &c, &d, &e, &f, &g, &h, &j, &k, &m, &n, &o,
        &r, &S, &S_dot, &C_prime, &C, &C_dot, &S_prime)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. add_record()");
        return PyErr_Occurred();
    }

    table = (NutationSeries*)PyCapsule_GetPointer(capsule, "NutationSeries");
    if(!table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the NutationSeries from capsule.");
        return PyErr_Occurred();
    }

    if (table->nrecords_allocated < table->nrecords+1) {
        table->nrecords_allocated += 1365;
        NutationSeriesRecord* new_table = (NutationSeriesRecord*)malloc(table->nrecords_allocated *
            sizeof(NutationSeriesRecord));
        if(!new_table) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for NutationSeries records.");
            return PyErr_Occurred();
        }
        memcpy(new_table, table->records, table->nrecords * sizeof(NutationSeriesRecord));
        free(table->records);
        table->records = new_table;
    }

    table->records[table->nrecords].heliocentric_elliptical_longitude_mercury_coefficient = a;
    table->records[table->nrecords].heliocentric_elliptical_longitude_venus_coefficient = b;
    table->records[table->nrecords].heliocentric_elliptical_longitude_earth_coefficient = c;
    table->records[table->nrecords].heliocentric_elliptical_longitude_mars_coefficient = d;
    table->records[table->nrecords].heliocentric_elliptical_longitude_jupiter_coefficient = e;
    table->records[table->nrecords].heliocentric_elliptical_longitude_saturn_coefficient = f;
    table->records[table->nrecords].heliocentric_elliptical_longitude_uranus_coefficient = g;
    table->records[table->nrecords].heliocentric_elliptical_longitude_neptune_coefficient = h;
    table->records[table->nrecords].general_precession_in_longitude_coefficient = j;
    table->records[table->nrecords].mean_anomaly_moon_coefficient = k;
    table->records[table->nrecords].mean_anomaly_sun_coefficient = m;
    table->records[table->nrecords].mean_argument_of_latitude_moon_coefficient = n;
    table->records[table->nrecords].mean_elongation_moon_from_the_sun_coefficient = o;
    table->records[table->nrecords].mean_longitude_of_moon_mean_ascending_node_coefficient = r;
    table->records[table->nrecords].S = S;
    table->records[table->nrecords].S_dot = S_dot;
    table->records[table->nrecords].C_prime = C_prime;
    table->records[table->nrecords].C = C;
    table->records[table->nrecords].C_dot = C_dot;
    table->records[table->nrecords++].S_prime = S_prime;

    return Py_BuildValue("i", table->nrecords);
}

/**
 * @brief Create a new nutation series object available in Python.
 */
static PyObject* new_NutationSeries(PyObject* self, PyObject* args) {

    NutationSeries* table = (NutationSeries*)malloc(sizeof(NutationSeries));

    if(!table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_NutationSeries.");
        return PyErr_Occurred();
    }

    table->nrecords = 0;
    table->nrecords_allocated = 0;
    table->records = NULL;

    return PyCapsule_New(table, "NutationSeries", delete_NutationSeries);
}

/**
 * @brief Delete a nutation series object available in Python.
 */
static void delete_NutationSeries(PyObject* obj) {

    NutationSeries* table = (NutationSeries*)PyCapsule_GetPointer(obj, "NutationSeries");
    if(table) {
        if(table->records) free(table->records);
        free(table);
    }
}


static PyMethodDef tolueneModelsEarthNutationMethods[] = {
    {"add_record", nutation_series_add_record, METH_VARARGS, "Add a record to the NutationSeries"},
    {"new_NutationSeries", new_NutationSeries, METH_VARARGS, "Create a new NutationSeries"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_nutation = {
    PyModuleDef_HEAD_INIT,
    "models.earth.nutation",
    "C Extensions to toluene models ellipsoid functions",
    -1,
    tolueneModelsEarthNutationMethods
};


PyMODINIT_FUNC PyInit_nutation(void) {
    return PyModule_Create(&models_earth_nutation);
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */