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

#define __compile_models_earth_earth_orientation__
#include "models/earth/earth_orientation.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Look up the EOP table record for a given timestamp
 */
void eop_table_record_lookup(EOPTable* table, double timestamp, EOPTableRecord* record) {

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

/**
 * @brief Add a record to the EOP table
 */
static PyObject* eop_table_add_record(PyObject* self, PyObject* args) {

    PyObject* capsule;
    EOPTable* table;

    double timestamp;

    int is_bulletin_a_PM_predicted;
    double bulletin_a_PM_x, bulletin_a_PM_x_error, bulletin_a_PM_y, bulletin_a_PM_y_error;

    int is_bulletin_a_dut1_predicted;
    double bulletin_a_dut1, bulletin_a_dut1_error;

    double bulletin_a_lod, bulletin_a_lod_error;

    double bulletin_b_PM_x, bulletin_b_PM_y, bulletin_b_dut1;

    if(!PyArg_ParseTuple(args, "Odiddddiddddddd", &capsule, &timestamp, &is_bulletin_a_PM_predicted, &bulletin_a_PM_x,
                            &bulletin_a_PM_x_error, &bulletin_a_PM_y, &bulletin_a_PM_y_error,
                            &is_bulletin_a_dut1_predicted, &bulletin_a_dut1, &bulletin_a_dut1_error, &bulletin_a_lod,
                            &bulletin_a_lod_error, &bulletin_b_PM_x, &bulletin_b_PM_y, &bulletin_b_dut1)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. add_record()");
        return PyErr_Occurred();
    }

    table = (EOPTable*)PyCapsule_GetPointer(capsule, "EOPTable");
    if(!table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EOPTable from capsule.");
        return PyErr_Occurred();
    }

    if (table->nrecords_allocated < table->nrecords+1) {
        table->nrecords_allocated += 365;
        EOPTableRecord* new_table = (EOPTableRecord*)malloc(table->nrecords_allocated * sizeof(EOPTableRecord));
        if(!new_table) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for EOPTable records.");
            return PyErr_Occurred();
        }
        memcpy(new_table, table->records, table->nrecords * sizeof(EOPTableRecord));
        free(table->records);
        table->records = new_table;
    }

    int insertion_point = table->nrecords;
    while(insertion_point > 0 && table->records[insertion_point-1].timestamp > timestamp) {
        table->records[insertion_point] = table->records[insertion_point-1];
        insertion_point--;
    }
    table->nrecords++;

    table->records[insertion_point].timestamp = timestamp;
    table->records[insertion_point].is_bulletin_a_PM_predicted = is_bulletin_a_PM_predicted;
    table->records[insertion_point].bulletin_a_PM_x = bulletin_a_PM_x;
    table->records[insertion_point].bulletin_a_PM_x_error = bulletin_a_PM_x_error;
    table->records[insertion_point].bulletin_a_PM_y = bulletin_a_PM_y;
    table->records[insertion_point].bulletin_a_PM_y_error = bulletin_a_PM_y_error;
    table->records[insertion_point].is_bulletin_a_dut1_predicted = is_bulletin_a_dut1_predicted;
    table->records[insertion_point].bulletin_a_dut1 = bulletin_a_dut1;
    table->records[insertion_point].bulletin_a_dut1_error = bulletin_a_dut1_error;
    table->records[insertion_point].bulletin_a_lod = bulletin_a_lod;
    table->records[insertion_point].bulletin_a_lod_error = bulletin_a_lod_error;
    table->records[insertion_point].bulletin_b_PM_x = bulletin_b_PM_x;
    table->records[insertion_point].bulletin_b_PM_y = bulletin_b_PM_y;
    table->records[insertion_point].bulletin_b_dut1 = bulletin_b_dut1;

    return Py_BuildValue("i", table->nrecords);
}

/**
 * @brief Create a new EOP table
 */
static PyObject* new_EOPTable(PyObject* self, PyObject* args)  {

    EOPTable* table = (EOPTable*)malloc(sizeof(EOPTable));

    if(!table) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_EOPTable.");
        return PyErr_Occurred();
    }

    table->nrecords = 0;
    table->nrecords_allocated = 0;
    table->records = NULL;

    return PyCapsule_New(table, "EOPTable", delete_EOPTable);
}

/**
 * @brief Delete an EOP table
 */
static void delete_EOPTable(PyObject* obj) {

    EOPTable* table = (EOPTable*)PyCapsule_GetPointer(obj, "EOPTable");

    if(table) {
        if(table->records) {
            free(table->records);
        }
        free(table);
    }

}


static PyMethodDef tolueneModelsEarthEarthOrientationMethods[] = {
    {"add_record", eop_table_add_record, METH_VARARGS, "Add a record to the EOPTable"},
    {"new_EOPTable", new_EOPTable, METH_VARARGS, "Create a new EOPTable"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_earth_orientation = {
    PyModuleDef_HEAD_INIT,
    "models.earth.earth_orientation",
    "C Extensions to work with earth orientation parameters in C",
    -1,
    tolueneModelsEarthEarthOrientationMethods
};


PyMODINIT_FUNC PyInit_earth_orientation(void) {
    return PyModule_Create(&models_earth_earth_orientation);
}


#ifdef __cplusplus
} /* extern "C" */
#endif