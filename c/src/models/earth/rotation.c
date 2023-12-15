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

#define __compile_models_earth_earth_orientation_parameters__
#define __compile_time_delta_t__

#include "models/earth/constants.h"
#include "models/earth/earth_orientation_parameters.h"
#include "models/earth/rotation.h"
#include "time/constants.h"
#include "time/delta_t.h"

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
    DeltaTTableRecord delta_t_record;
    eop_table_record_lookup(&model->earth_orientation_parameters, t, &record);
    delta_t_record_lookup(&model->delta_t_table, t, &delta_t_record);

    long double du = (t - J2000_UNIX_TIME + record.bulletin_a_dut1/1000.0) / SECONDS_PER_DAY;
    *gmst = ((((GMST_FUNCTION_JULIAN_DU[5] * du + GMST_FUNCTION_JULIAN_DU[4])* du + GMST_FUNCTION_JULIAN_DU[3]) * du +
        GMST_FUNCTION_JULIAN_DU[2]) * du + GMST_FUNCTION_JULIAN_DU[1]) * du + GMST_FUNCTION_JULIAN_DU[0];
    *gmst +=  GMST_DELTA_T * delta_t_record.deltaT/SECONDS_PER_DAY;

    *gmst = fmodl(*gmst, 86400.0);
}

/**
 * @brief Calculate the Earth rotation matrix.
 *
 * @param[in] angle angle of rotation.
 * @param[out] matrix rotation matrix.
 */
void earth_rotation_matrix(long double angle, Mat3* matrix) {

    if (matrix) {

        matrix->w11 = cosl(angle);
        matrix->w12 = -sinl(angle);
        matrix->w13 = 0.0;
        matrix->w21 = sinl(angle);
        matrix->w22 = cosl(angle);
        matrix->w23 = 0.0;
        matrix->w31 = 0.0;
        matrix->w32 = 0.0;
        matrix->w33 = 1.0;

    }

}

/**
 * @brief Calculate the Earth rotation matrix.
 *
 * @param[in] t Unix time
 * @param[in] model Earth model
 * @param[out] rate the rotation rate in rad/s.
 */
void rate_of_earth_rotation(double t, EarthModel* model, double* rate) {

    EOPTableRecord record;
    eop_table_record_lookup(&model->earth_orientation_parameters, t, &record);

    *rate = 2.0 * M_PI / (SECONDS_PER_DAY + record.bulletin_a_lod/1000.0);

}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */