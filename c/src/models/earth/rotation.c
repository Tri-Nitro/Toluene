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
 * @brief Calculate the Greenwich Mean Sidereal Time (GMST).
 *
 * @param[in] t seconds since J2000.0.
 */
void gmst(long double t, EarthModel* model, long double* gmst) {

    EOPTableRecord record;

    eop_table_record_lookup(model->eop_table, tt, &record);

    long double du = (tt + record.bulletin_a_dut1) / SECONDS_PER_DAY;
    *gmst = ((((GMST_FUNCTION_JULIAN_DU[5] * t + GMST_FUNCTION_JULIAN_DU[4])* t + GMST_FUNCTION_JULIAN_DU[3]) * t +
        GMST_FUNCTION_JULIAN_DU[2]) * t + GMST_FUNCTION_JULIAN_DU[1]) * t + GMST_FUNCTION_JULIAN_DU[0];

/**
 * @brief Calculate the Earth rotation matrix.
 *
 * @param[in] angle angle of rotation.
 * @param[out] matrix rotation matrix.
 */
void earth_rotation_matrix(long double angle, Mat3* matrix);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */