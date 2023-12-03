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

#define __compile_models_earth_earth_orientation
#define __compile_models_earth_polar_motion
#include "math/constants.h"
#include "models/earth/earth_orientation.h"
#include "models/earth/polar_motion.h"
#include "util/time.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void itrs_to_tirs_polar_motion_approximation(double tt, EarthModel* model, Matrix* matrix) {

    double julian_centuries = (tt - model->epoch) / SECONDS_PER_JULIAN_CENTURY;
    double s_prime = (model->cirs_coefficients->s_prime)*julian_centuries*ARCSECONDS_PER_RADIAN;
    EOPTableRecord record;
    record_lookup(model->eop_table, tt, &record);
    double x = record.bulletin_a_PM_x*ARCSECONDS_PER_RADIAN, y = record.bulletin_a_PM_y*ARCSECONDS_PER_RADIAN;

    printf("Dut1: %f\n", record.bulletin_a_dut1);

    if(matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = 1.0;
        matrix->elements[1] = -1.0 * s_prime;
        matrix->elements[2] = -1.0 * x;
        matrix->elements[3] = s_prime;
        matrix->elements[4] = 1.0;
        matrix->elements[5] = y;
        matrix->elements[6] = x;
        matrix->elements[7] = -1.0 * y;
        matrix->elements[8] = 1.0;

    }

}


void itrs_to_tirs_polar_motion_exact(double tt, EarthModel* model, Matrix* matrix) {

    double julian_centuries = (tt - model->epoch) / SECONDS_PER_JULIAN_CENTURY;
    double s_prime = (model->cirs_coefficients->s_prime)*julian_centuries*ARCSECONDS_PER_RADIAN;
    EOPTableRecord record;
    record_lookup(model->eop_table, tt, &record);
    double x = record.bulletin_a_PM_x*ARCSECONDS_PER_RADIAN, y = record.bulletin_a_PM_y*ARCSECONDS_PER_RADIAN;

    printf("Dut1: %f\n", record.bulletin_a_dut1);

    double cos_x = cos(x), sin_x = sin(x),
            cos_y = cos(y), sin_y = sin(y),
            cos_s_prime = cos(-1 * s_prime), sin_s_prime = sin(-1 * s_prime);

    if(matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = cos_x * cos_s_prime;
        matrix->elements[1] = sin_x * sin_y * cos_s_prime + cos_y * sin_s_prime;
        matrix->elements[2] = -1.0 * sin_x * cos_y * cos_s_prime + sin_y * sin_s_prime;
        matrix->elements[3] = -1.0 * cos_x * sin_s_prime;
        matrix->elements[4] = -1.0 * sin_x * sin_y * sin_s_prime + cos_y * cos_s_prime;
        matrix->elements[5] = sin_x * cos_y * sin_s_prime + sin_y * cos_s_prime;
        matrix->elements[6] = sin_x;
        matrix->elements[7] = -1.0 * cos_x * sin_y;
        matrix->elements[8] = cos_x * cos_y;

    }

}


#ifdef __cplusplus
} /* extern "C" */
#endif