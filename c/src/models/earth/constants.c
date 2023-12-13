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

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */