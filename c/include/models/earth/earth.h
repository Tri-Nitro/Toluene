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
#ifndef __MODELS_EARTH_EARTH_H__
#define __MODELS_EARTH_EARTH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "models/earth/earth_orientation_parameters.h"
#include "models/earth/ellipsoid.h"
#include "models/earth/geoid.h"
#include "models/earth/nutation.h"
#include "time/delta_t.h"

typedef struct {

    /* Earth Shape */
    Ellipsoid ellipsoid;
    Geoid geoid;

    /* Earth Motion */
    NutationSeries nutation_series;
    EOPTable earth_orientation_parameters;

    /* Earth Time */
    DeltaTTable delta_t_table;

} EarthModel;


#ifdef __compile_models_earth_earth__

/**
 * @brief Creates a new Earth Model object and makes it available to Python
 */
static PyObject* new_EarthModel(PyObject* self, PyObject* args);

/**
 * @brief Deletes the Earth Model object
 *
 * @param obj The Earth Model object
 */
static void delete_EarthModel(PyObject* obj);

/**
 * @brief Set the Earth Model's Ellipsoid
 */
static PyObject* earth_model_set_ellipsoid(PyObject* self, PyObject* args);

/**
 * @brief Set the Earth Model's Nutation Series
 */
static PyObject* earth_model_set_nutation_series(PyObject* self, PyObject* args);

/**
 * @brief Set the Earth Model's Earth Orientation Parameters
 */
static PyObject* earth_model_set_earth_orientation_parameters(PyObject* self, PyObject* args);

/**
 * @brief Get the Earth Model's Ellipsoid
 */
static PyObject* earth_model_get_ellipsoid(PyObject* self, PyObject* args);

/**
 * @brief Set the Earth Model's Geoid
 */
static PyObject* earth_model_set_geoid(PyObject* self, PyObject* args);

/**
 * @brief Get the Earth Model's Geoid
 */
static PyObject* earth_model_get_geoid(PyObject* self, PyObject* args);

/**
 * @brief Get the Earth Model's Nutation Series
 */
static PyObject* earth_model_get_nutation_series(PyObject* self, PyObject* args);

/**
 * @brief Get the Earth Model's Earth Orientation Parameters
 */
static PyObject* earth_model_get_earth_orientation_parameters(PyObject* self, PyObject* args);

/**
 * @brief Set the Earth Model's Delta T Table
 */
static PyObject* earth_model_set_delta_t_table(PyObject* self, PyObject* args);

/**
 * @brief Get the Earth Model's Delta T Table
 */
static PyObject* earth_model_get_delta_t_table(PyObject* self, PyObject* args);


#endif /* __compile_models_earth_earth__ */


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_PRECESSION_H__ */