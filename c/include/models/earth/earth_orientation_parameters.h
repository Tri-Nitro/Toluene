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
#ifndef __MODELS_EARTH_EARTH_ORIENTATION_PARAMETERS_H__
#define __MODELS_EARTH_EARTH_ORIENTATION_PARAMETERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @struct
 * @brief Earth Orientation Parameters Table Record
 * */
typedef struct {

    long double timestamp;

    int is_bulletin_a_PM_predicted;
    long double bulletin_a_PM_x;
    long double bulletin_a_PM_x_error;
    long double bulletin_a_PM_y;
    long double bulletin_a_PM_y_error;

    int is_bulletin_a_dut1_predicted;
    long double bulletin_a_dut1;
    long double bulletin_a_dut1_error;

    long double bulletin_a_lod;
    long double bulletin_a_lod_error;

    long double bulletin_b_PM_x;
    long double bulletin_b_PM_y;
    long double bulletin_b_dut1;

} EOPTableRecord;

/** @struct
 * @brief Earth Orientation Parameters Table
 * */
typedef struct {
    int nrecords;
    int nrecords_allocated;
    EOPTableRecord* records;
} EOPTable;


#ifdef __compile_models_earth_earth_orientation_parameters__

/**
 * @brief Look up the EOP table record for a given timestamp
 */
void eop_table_record_lookup(EOPTable* table, double timestamp, EOPTableRecord* record);

/**
 * @brief Add a record to the EOP table
 */
static PyObject* eop_table_add_record(PyObject* self, PyObject* args);

/**
 * @brief Create a new EOP table
 */
static PyObject* new_EOPTable(PyObject* self, PyObject* args);

/**
 * @brief Delete an EOP table
 */
static void delete_EOPTable(PyObject* obj);


#endif /* __compile_models_earth_earth_orientation */

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_EARTH_ORIENTATION_H__ */