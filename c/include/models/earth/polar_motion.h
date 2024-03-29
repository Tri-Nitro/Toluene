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

#ifdef __cplusplus
extern "C" {
#endif

#define __compile_models_earth_earth_orientation_parameters__
#include "math/linear_algebra.h"
#include "models/earth/earth_orientation_parameters.h"

/**
 * @brief Polar motion matrix for time T.
 *
 * @param t Time in seconds since J2000.0.
 * @param matrix Output matrix.
 * */
void wobble(long double t, EOPTable* earth_orientation_parameter_table, Mat3* matrix);


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_POLAR_MOTION_H__ */