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

#define __compile_models_earth_nutation
#include "models/earth/nutation.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void iau_2006_nutation(double delta_psi, double delta_epsilon, double epsilon, Matrix* matrix) {

    double sin_delta_psi, cos_delta_psi, sin_epsilon_prime, cos_epsilon_prime, sin_epsilon, cos_epsilon;

    sin_epsilon = sin(epsilon * M_PI/648000);
    cos_epsilon = cos(epsilon * M_PI/648000);
    sin_delta_psi = -1 * sin(delta_psi * M_PI/648000);
    cos_delta_psi = cos(delta_psi * M_PI/648000);
    sin_epsilon_prime = sin((-1 * epsilon - delta_epsilon) * M_PI/648000);
    cos_epsilon_prime = cos((-1 * epsilon - delta_epsilon) * M_PI/648000);

    if(matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = cos_delta_psi;
        matrix->elements[1] = -1.0 * sin_delta_psi * cos_epsilon_prime;
        matrix->elements[2] = sin_delta_psi * sin_epsilon_prime;
        matrix->elements[3] = sin_delta_psi * cos_epsilon;
        matrix->elements[4] = cos_epsilon_prime * cos_delta_psi * cos_epsilon - sin_epsilon_prime * sin_epsilon;
        matrix->elements[5] = -1.0 * sin_epsilon_prime * cos_delta_psi * cos_epsilon - sin_epsilon * cos_epsilon_prime;
        matrix->elements[6] = sin_delta_psi * sin_epsilon;
        matrix->elements[7] = cos_epsilon_prime * cos_delta_psi * sin_epsilon + cos_epsilon * sin_epsilon_prime;
        matrix->elements[8] = -1.0 * sin_epsilon_prime * cos_delta_psi * sin_epsilon + cos_epsilon_prime * cos_epsilon;

    }

}


static PyObject* nutation_series_add_record(PyObject* self, PyObject* args) {

    PyObject* capsule;
    NutationSeries* table;

    double a, b, c, d, e, f, g, h, j, k, m, n, o, r;
    double S, S_dot, C_prime, C, C_dot, S_prime;

    if(!PyArg_ParseTuple(args, "Odddddddddddddddddddd", &capsule, &a, &b, &c, &d, &e, &f, &g, &h, &j, &k, &m, &n, &o,
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
        table->nrecords_allocated += 100;
        NutationSeriesRecord* new_table = (NutationSeriesRecord*)malloc(table->nrecords_allocated * sizeof(NutationSeriesRecord));
        if(!new_table) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for NutationSeries records.");
            return PyErr_Occurred();
        }
        memcpy(new_table, table->records, table->nrecords * sizeof(NutationSeriesRecord));
        free(table->records);
        table->records = new_table;
    }

    table->records[table->nrecords].a = a;
    table->records[table->nrecords].b = b;
    table->records[table->nrecords].c = c;
    table->records[table->nrecords].d = d;
    table->records[table->nrecords].e = e;
    table->records[table->nrecords].f = f;
    table->records[table->nrecords].g = g;
    table->records[table->nrecords].h = h;
    table->records[table->nrecords].j = j;
    table->records[table->nrecords].k = k;
    table->records[table->nrecords].m = m;
    table->records[table->nrecords].n = n;
    table->records[table->nrecords].o = o;
    table->records[table->nrecords].r = r;
    table->records[table->nrecords].S = S;
    table->records[table->nrecords].S_dot = S_dot;
    table->records[table->nrecords].C_prime = C_prime;
    table->records[table->nrecords].C = C;
    table->records[table->nrecords].C_dot = C_dot;
    table->records[table->nrecords++].S_prime = S_prime;

    return Py_BuildValue("i", 0);
}


static PyObject* new_NutationSeries(PyObject* self, PyObject* args)  {

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


static void delete_NutationSeries(PyObject* obj) {

    NutationSeries* table = (NutationSeries*)PyCapsule_GetPointer(obj, "NutationSeries");

    if(table) {
        if(table->records) {
            free(table->records);
        }
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
#endif