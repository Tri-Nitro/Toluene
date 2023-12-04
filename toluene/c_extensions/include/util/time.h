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
#ifndef __UTIL_TIME_H__
#define __UTIL_TIME_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    double timestamp;
    double deltaT;

} DeltaTTableRecord;


typedef struct {
    int nrecords;
    int nrecords_allocated;
    DeltaTTableRecord* records;
} DeltaTTable;


#ifdef __compile_util_time

void delta_t_record_lookup(DeltaTTable* table, double timestamp, DeltaTTableRecord* record);

static PyObject* delta_t_add_record(PyObject* self, PyObject* args);

static PyObject* new_DeltaTTable(PyObject* self, PyObject* args);
static void delete_DeltaTTable(PyObject* obj);

#endif /* __compile_util_time */


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __UTIL_TIME_H__ */