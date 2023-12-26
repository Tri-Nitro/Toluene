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

#include "models/moon/constants.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * These coefficients can be found here: https://aa.usno.navy.mil/downloads/Circular_179.pdf
 */
const long double MEAN_ANOMALY_MOON[5] = {485868.249036, 1717915923.2178, 31.8792, 0.051635, -0.00024470};
const long double MEAN_ARGUMENT_LATITUDE_MOON[5] = {335779.526232, 1739527262.8478, -12.7512, -0.001037, 0.00000417};
const long double MEAN_ELONGATION_MOON_FROM_SUN[5] = {1072260.703692, 1602961601.2090, -6.3706, 0.006593, -0.00003169};
const long double MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[5] = {450160.398036, -6962890.5431, 7.4722, 0.007702,
    -0.00005939};
const long double MEAN_LONGITUDE_MOON[3] = {785938.211998, 1732564371.16289, -4.06};

const long double MEAN_LUNAR_HORIZONTAL_PARALLAX = 0.016594591;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */