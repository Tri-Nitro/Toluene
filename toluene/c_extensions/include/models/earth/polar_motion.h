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
#ifndef __MODELS_EARTH_POLAR_MOTION_H__
#define __MODELS_EARTH_POLAR_MOTION_H__

#include "math/linear_algebra.h"
#include "models/earth/coefficients.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    double timestamp;

    bool is_bulletin_a_PM_predicted;
    double bulletin_a_PM_x;
    double bulletin_a_PM_x_error;
    double bulletin_a_PM_y;
    double bulletin_a_PM_y_error;

    bool is_bulletin_a_dut1_predicted;
    double bulletin_a_dut1;
    double bulletin_a_dut1_error;

    double bulletin_a_lod;
    double bulletin_a_lod_error;

    double bulletin_b_PM_x;
    double bulletin_b_PM_y;
    double bulletin_b_dut1;

} EOP_TableRecord;

typedef struct {
    int nrecords
    EOP_TableRecord* records;
} EOP_Table;

void itrs_to_tirs_polar_motion_approximation(double tt, Matrix* matrix);
void itrs_to_tirs_polar_motion_exact(double tt, Matrix* matrix);

static PyObject* new_EOP_Table(PyObject* self, PyObject* args);
static void delete_EOP_Table(PyObject* obj);


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_POLAR_MOTION_H__ */