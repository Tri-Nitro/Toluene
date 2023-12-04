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

#define __compile_util_time
#include "util/time.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void delta_t_record_lookup(DeltaTTable* table, double timestamp, DeltaTTableRecord* record) {

    int lower = 0, upper = (table->nrecords)-1, pointer = upper;

    while(upper - lower > 1) {
        pointer = (upper + lower) / 2;
        if(table->records[pointer].timestamp > timestamp) {
            upper = pointer;
        } else {
            lower = pointer;
        }
        *record = table->records[pointer];
    }

}


static PyObject* delta_t_add_record(PyObject* self, PyObject* args) {

    PyObject* capsule;
    DeltaTTable* table;

    double timestamp;
    double deltaT;

    if(!PyArg_ParseTuple(args, "Odd", &capsule, &timestamp, &deltaT)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. add_record()");
        return PyErr_Occurred();
    }

    table = (DeltaTTable*)PyCapsule_GetPointer(capsule, "DeltaTTable");
    if(!table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the DeltaTTable from capsule.");
        return PyErr_Occurred();
    }

    if (table->nrecords_allocated < table->nrecords+1) {
        table->nrecords_allocated += 100;
        DeltaTTableRecord* new_table = (DeltaTTableRecord*)malloc(table->nrecords_allocated * sizeof(DeltaTTableRecord));
        if(!new_table) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for DeltaTTable records.");
            return PyErr_Occurred();
        }
        memcpy(new_table, table->records, table->nrecords * sizeof(DeltaTTableRecord));
        free(table->records);
        table->records = new_table;
    }

    table->records[table->nrecords].timestamp = timestamp;
    table->records[table->nrecords].deltaT = deltaT;

    return Py_BuildValue("i", table->records[table->nrecords].timestamp);
}


static PyObject* new_DeltaTTable(PyObject* self, PyObject* args)  {

    DeltaTTable* table = (DeltaTTable*)malloc(sizeof(DeltaTTable));

    if(!table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_DeltaTTable.");
        return PyErr_Occurred();
    }

    table->nrecords = 0;
    table->nrecords_allocated = 0;
    table->records = NULL;

    return PyCapsule_New(table, "DeltaTTable", delete_DeltaTTable);
}


static void delete_DeltaTTable(PyObject* obj) {

    DeltaTTable* table = (DeltaTTable*)PyCapsule_GetPointer(obj, "DeltaTTable");

    if(table) {
        if(table->records) {
            free(table->records);
        }
        free(table);
    }

}


static PyMethodDef tolueneUtilTimeMethods[] = {
    {"add_record", delta_t_add_record, METH_VARARGS, "Add a record to the DeltaTTable"},
    {"new_DeltaTTable", new_DeltaTTable, METH_VARARGS, "Create a new DeltaTTable"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef util_time = {
    PyModuleDef_HEAD_INIT,
    "util.time",
    "C Extensions to toluene models ellipsoid functions",
    -1,
    tolueneUtilTimeMethods
};


PyMODINIT_FUNC PyInit_time(void) {
    return PyModule_Create(&util_time);
}


#ifdef __cplusplus
} /* extern "C" */
#endif