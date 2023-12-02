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
#ifndef __MODELS_EARTH_MODEL_H__
#define __MODELS_EARTH_MODEL_H__

#include "models/earth/coefficients.h"
#include "models/earth/ellipsoid.h"
#include "models/earth/geoid.h"
#include "models/earth/nutation.h"
#include "models/earth/polar_motion.h"
#include "models/earth/precession.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {

    /* Ellipsoid */
    Ellipsoid* ellipsoid;

    /* Geoid */
    Geoid* geoid;

    /* Rotation Models */
    CIRSCoefficients* cirs_coefficients;
    EOPTable* eop_table;
    double epoch;

} EarthModel;

static PyObject* get_ellipsoid(PyObject* self, PyObject* args);
static PyObject* set_ellipsoid(PyObject* self, PyObject* args);

static PyObject* get_cirs_coefficients(PyObject* self, PyObject* args);
static PyObject* set_cirs_coefficients(PyObject* self, PyObject* args);

static PyObject* get_eop_table(PyObject* self, PyObject* args);
static PyObject* set_eop_table(PyObject* self, PyObject* args);

static PyObject* new_EarthModel(PyObject* self, PyObject* args);
static void delete_EarthModel(PyObject* obj);


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_NUTATION_H__ */