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

#include "models/earth/constants.h"

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
const long double MEAN_OBLIQUITY_EARTH[6] = {84381.406, -46.836769, -0.0001831, 0.00200340, -5.76e-7, -4.34e-8};

const long double GENERAL_PRECESSION_LONGITUDE[3] = {0.0, 5028.82, 1.112022};
const long double PRECESSION_EQUATOR[6] = {0.0, 5038.481507, -1.0790069, -0.00114045, 0.000132851, -0.0000000951};
const long double OBLIQUITY_MEAN_EQUATOR[6] = {84381.406, -0.025754, 0.0512623, -0.00772503, -4.67e-7, -3.337e-7};
const long double PRECESSION_ECLIPTIC_MEAN_EQUATOR[6] = {0.0, 10.556403, -2.3814292, -0.00121197, 0.000170663, -5.4e-8};

const long double ICRS_X_POLE_OFFSET = 0.0;
const long double ICRS_Y_POLE_OFFSET = 0.0;
const long double ICRS_RIGHT_ASCENSION_OFFSET = 0.0;

const long double GMST_FUNCTION_JULIAN_DU[6] = {67310.54837708, 86636.555367405292, 6.9540371e-11, -6.02e-24,
    -1.1221e-24, -3.774e-32};
const long double ERA_DUT1[2] = {0.779057273264, 1.00273781191135448};
const long double EQUATION_OF_ORIGINS[544] = {
    2640.96, -0.39, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    63.52, -0.02, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    11.75, 0.01, 0, 0, 2, -2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    11.21, 0.01, 0, 0, 2, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -4.55, 0.00, 0, 0, 2, -2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2.02, 0.00, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1.98, 0.00, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -1.72, 0.00, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -1.41, -0.01, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -1.26, -0.01, 0, 1, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.63, 0.00, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.63, 0.00, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.46, 0.00, 0, 1, 2, -2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.45, 0.00, 0, 1, 2, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.36, 0.00, 0, 0, 4, -4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.24, -0.12, 0, 0, 1, -1, 1, 0, -8, 12, 0, 0, 0, 0, 0, 0,
    0.32, 0.00, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.28, 0.00, 0, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.27, 0.00, 1, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.26, 0.00, 1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.21, 0.00, 0, 0, 2, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.19, 0.00, 0, 1, -2, 2, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.18, 0.00, 0, 1, -2, 2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.10, 0.05, 0, 0, 0, 0, 0, 0, 8, -13, 0, 0, 0, 0, 0, -1,
    0.15, 0.00, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.14, 0.00, 2, 0, -2, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.14, 0.00, 1, 0, 0, -2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.14, 0.00, 0, 1, 2, -2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.14, 0.00, 1, 0, 0, -2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.13, 0.00, 0, 0, 4, -2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.11, 0.00, 0, 0, 2, -2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.11, 0.00, 1, 0, -2, 0, -3, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0.11, 0.00, 1, 0, -2, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    -0.87, 0.00, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const long double CHANDLER_WOBBLE = 0.12;
const long double ANNUAL_WOBBLE = 0.26;

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */