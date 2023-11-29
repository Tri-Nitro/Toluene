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

#include "models/earth/cirs_to_tirs_coefficients.h"
#include "models/earth/ellipsoid.h"
#include "models/earth/geoid.h"
#include "models/earth/nutation.h"
#include "models/earth/precession.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {

    /* CIRS to TIRS Rotation Models */
    CIRStoTIRSCoefficients cirs_to_tirs_coefficients;
    EarthNutationModel nutation_model;
    EarthPrecessionModel precession_model;
    double epoch;

    /* Ellipsoid */
    Ellipsoid* ellipsoid;

    /* Geoid */
    Geoid geoid;

    /* Tables */
    char* delta_t_table;
    char* earth_orientation_table;

} EarthModel;

static PyObject* get_ellipsoid(PyObject* self, PyObject* args);
static PyObject* set_ellipsoid(PyObject* self, PyObject* args);

static PyObject* new_EarthModel(PyObject* self, PyObject* args);
static void delete_EarthModel(PyObject* obj);


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_NUTATION_H__ */