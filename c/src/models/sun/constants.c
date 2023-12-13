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

#include "models/sun/constants.h"

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
const long double mean_heliocentric_ecliptic_longitude_mercury[2] = {908103.259872, 538101628.688982};
const long double mean_heliocentric_ecliptic_longitude_venus[2] = {655127.28306, 210664136.433548};
const long double mean_heliocentric_ecliptic_longitude_earth[2] = {361679.244588, 129597742.283429};
const long double mean_heliocentric_ecliptic_longitude_mars[2] = {1279558.798488, 68905077.493988};
const long double mean_heliocentric_ecliptic_longitude_jupiter[2] = {123665.467464, 10925660.377991};
const long double mean_heliocentric_ecliptic_longitude_saturn[2] = {180278.79948, 4399609.855732};
const long double mean_heliocentric_ecliptic_longitude_uranus[2] = {1130598.018396, 1542481.193933};
const long double mean_heliocentric_ecliptic_longitude_neptune[2] = {1095655.195728, 786550.320744};

const long double mean_anomaly_sun[5] = {1287104.79305, 129596581.0481, -0.5532, 0.000136, -1.148e-5};

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */