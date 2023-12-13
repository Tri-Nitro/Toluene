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
#include "models/earth/polar_motion.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Polar motion matrix for time T.
 *
 * @param t Time in seconds since J2000.0.
 * @param matrix Output matrix.
 * */
void wobble(long double t, EOPTable* earth_orientation_parameter_table, Mat3* matrix) {

    if (matrix && earth_orientation_parameter_table) {
        EOPTableRecord record;
        eop_table_record_lookup(earth_orientation_parameter_table, double timestamp, &record);

        long double sin_x = sin(record.bulletin_a_PM_x * ARCSECONDS_TO_RADIANS);
        long double cos_x = cos(record.bulletin_a_PM_x * ARCSECONDS_TO_RADIANS);
        long double sin_y = sin(record.bulletin_a_PM_y * ARCSECONDS_TO_RADIANS);
        long double cos_y = cos(record.bulletin_a_PM_y * ARCSECONDS_TO_RADIANS);

        matrix->w11 = cos_x;
        matrix->w12 = sin_x * sin_y;
        matrix->w13 = -1.0 * sin_x * cos_y;
        matrix->w21 = 0.0;
        matrix->w22 = cos_y;
        matrix->w23 = sin_y;
        matrix->w31 = sin_x;
        matrix->w32 = -1.0 * cos_x * sin_y;
        matrix->w33 = cos_x * cos_y;
    }
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */