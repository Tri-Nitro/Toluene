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
#ifndef __MODELS_EARTH_ROTATION_H__
#define __MODELS_EARTH_ROTATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "math/linear_algebra.h"
#include "models/earth/earth.h"

/**
 * @brief Calculate the Greenwich Mean Sidereal Time (GMST).
 *
 * @param[in] t seconds since J2000.0.
 */
void gmst(long double t, EarthModel* model, long double* gmst);

/**
 * @brief Calculate the Earth rotation matrix.
 *
 * @param[in] angle angle of rotation.
 * @param[out] matrix rotation matrix.
 */
void earth_rotation_matrix(long double angle, Mat3* matrix);

/**
 * @brief Calculate the Earth rotation angle.
 *
 * @param[in] t Unix time
 * @param[in] model Earth model
 * @param[out] era the Earth rotation angle in rad.
 */
void earth_rotation_angle(long double t, EarthModel* model, long double* era);

/**
 * @brief Calculate the equation of origins.
 *
 * @param[in] t Unix time
 * @param[out] eo the equation of origins in rad.
 */
void equation_of_origins(long double t, long double nutation_longitude, long double mean_obliquity_date,
    long double* eo);

/**
 * @brief Calculate the Greenwich Apparent Sidereal Time (GAST).
 *
 * @param[in] t Unix time
 * @param[in] model Earth model
 * @param[out] gast the Greenwich Apparent Sidereal Time in rad.
 */
void gast_2000(long double t, EarthModel* model, long double nutation_longitude, long double mean_obliquity_date,
    long double* gast);

/**
 * @brief Calculate the Earth rotation matrix.
 *
 * @param[in] t Unix time
 * @param[in] model Earth model
 * @param[out] rate the rotation rate in rad/s.
 */
void rate_of_earth_rotation(long double t, EarthModel* model, long double* rate);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_ROTATION_H__ */