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

#include "math/constants.h"
#include "models/earth/constants.h"
#include "models/earth/precession.h"
#include "time/constants.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Precession matrix for the IAU 2000A model.
 *
 * @param t Time in seconds since J2000.0.
 * @param matrix Output matrix.
 * */
void iau_2000a_precession(long double t, Mat3* matrix) {

    if (matrix) {
        t = (t-J2000_UNIX_TIME)/ SECONDS_PER_JULIAN_CENTURY;
        long double mean_obliquity_j2000 = MEAN_OBLIQUITY_EARTH[0];
        long double precession_equator = ((((PRECESSION_EQUATOR[5] * t + PRECESSION_EQUATOR[4]) * t +
            PRECESSION_EQUATOR[3]) * t + PRECESSION_EQUATOR[2]) * t + PRECESSION_EQUATOR[1]) * t;
        long double obliquity_mean_equator = ((((OBLIQUITY_MEAN_EQUATOR[5] * t + OBLIQUITY_MEAN_EQUATOR[4]) * t +
            OBLIQUITY_MEAN_EQUATOR[3]) * t + OBLIQUITY_MEAN_EQUATOR[2]) * t + OBLIQUITY_MEAN_EQUATOR[1]) * t +
            OBLIQUITY_MEAN_EQUATOR[0];
        long double precession_ecliptic_mean_equator = ((((PRECESSION_ECLIPTIC_MEAN_EQUATOR[5] * t +
            PRECESSION_ECLIPTIC_MEAN_EQUATOR[4]) * t + PRECESSION_ECLIPTIC_MEAN_EQUATOR[3]) * t +
            PRECESSION_ECLIPTIC_MEAN_EQUATOR[2]) * t + PRECESSION_ECLIPTIC_MEAN_EQUATOR[1]) * t;

        long double sin_epsilon_0 = sin(mean_obliquity_j2000 * ARCSECONDS_TO_RADIANS);
        long double cos_epsilon_0 = cos(mean_obliquity_j2000 * ARCSECONDS_TO_RADIANS);
        long double sin_psi_a = -1 * sin(precession_equator * ARCSECONDS_TO_RADIANS);
        long double cos_psi_a = cos(precession_equator * ARCSECONDS_TO_RADIANS);
        long double sin_omega_a = -1 * sin(obliquity_mean_equator * ARCSECONDS_TO_RADIANS);
        long double cos_omega_a = cos(obliquity_mean_equator * ARCSECONDS_TO_RADIANS);
        long double sin_chi_a = sin(precession_ecliptic_mean_equator * ARCSECONDS_TO_RADIANS);
        long double cos_chi_a = cos(precession_ecliptic_mean_equator * ARCSECONDS_TO_RADIANS);

        matrix->w11 = cos_chi_a * cos_psi_a - sin_psi_a * sin_chi_a * cos_omega_a;
        matrix->w12 = cos_chi_a * sin_psi_a * cos_epsilon_0 + sin_chi_a * cos_omega_a * cos_psi_a * cos_epsilon_0 -
            sin_epsilon_0 * sin_chi_a * sin_omega_a;
        matrix->w13 = cos_chi_a * sin_psi_a * sin_epsilon_0 + sin_chi_a * cos_omega_a * cos_psi_a * sin_psi_a +
            cos_epsilon_0 * sin_chi_a * sin_omega_a;
        matrix->w21 = -1 * sin_chi_a * cos_psi_a - sin_psi_a * cos_chi_a * cos_omega_a;
        matrix->w22 = -1 * sin_chi_a * sin_psi_a * cos_epsilon_0 + cos_chi_a * cos_omega_a * cos_psi_a * cos_epsilon_0 -
            sin_epsilon_0 * cos_chi_a * sin_omega_a;
        matrix->w23 = -1 * sin_chi_a * sin_psi_a * sin_epsilon_0 + cos_chi_a * cos_omega_a * cos_psi_a * sin_epsilon_0 +
            cos_epsilon_0 * cos_chi_a * sin_omega_a;
        matrix->w31 = sin_psi_a * sin_omega_a;
        matrix->w32 = -1 * sin_omega_a * cos_psi_a * cos_epsilon_0 - sin_epsilon_0 * cos_omega_a;
        matrix->w33 = -1 * sin_omega_a * cos_psi_a * sin_epsilon_0 + cos_omega_a * cos_epsilon_0;
    }
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */