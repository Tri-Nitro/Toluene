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

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#define __compile_math_algebra
#define __compile_models_earth_earth_orientation
#define __compile_models_earth_earth_rotation
#define __compile_util_time
#include "math/algebra.h"
#include "models/earth/earth_orientation.h"
#include "models/earth/earth_rotation.h"
#include "util/time.h"

#ifdef __cplusplus
extern "C"
{
#endif


void greenwich_mean_sidereal_time(double tt, EarthModel* model, double* gmst_rad) {

    EOPTableRecord record;
    DeltaTTableRecord delta_t_record;
    eop_table_record_lookup(model->eop_table, tt, &record);
    delta_t_record_lookup(model->delta_t_table, tt, &delta_t_record);

    double du = (tt + record.bulletin_a_dut1) / 86400.0;
    Polynomial* gmst_poly = model->greenwich_mean_sidereal_time_polynomial;

    solve(gmst_poly, du, gmst_rad);
    *gmst_rad += 0.008418264265 * delta_t_record.deltaT / 86400.0;
    *gmst_rad = fmod(*gmst_rad, 86400.0);
    *gmst_rad = *gmst_rad * 2.0 * M_PI / 86400.0;

}


void tirs_to_true_equinox_equator_earth_rotation(double tt, double eq_eq, EarthModel* model, Matrix* matrix) {

    double gmst_rad = 0.0;
    greenwich_mean_sidereal_time(tt, model, &gmst_rad);
    gmst_rad += eq_eq * M_PI / 86400.0;

    double cos_gmst = cos(gmst_rad);
    double sin_gmst = sin(gmst_rad);

    if(matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = cos_gmst;
        matrix->elements[1] = -1.0 * sin_gmst;
        matrix->elements[2] = 0.0;
        matrix->elements[3] = sin_gmst;
        matrix->elements[4] = cos_gmst;
        matrix->elements[5] = 0.0;
        matrix->elements[6] = 0.0;
        matrix->elements[7] = 0.0;
        matrix->elements[8] = 1.0;

    }

    return;
}


void rate_of_earth_rotation(double tt, EarthModel* model, double* rate) {

    EOPTableRecord record;
    eop_table_record_lookup(model->eop_table, tt, &record);

    *rate = 2.0 * M_PI / (86400.0 + record.bulletin_a_lod/1000.0);

}


void tirs_to_true_equinox_equator_earth_rotation_rate(double tt, double eq_eq, EarthModel* model, Matrix* matrix) {

    double rate = 0.0;
    rate_of_earth_rotation(tt, model, &rate);

    double gmst_rad = 0.0;
    greenwich_mean_sidereal_time(tt, model, &gmst_rad);
    gmst_rad += eq_eq * M_PI / 86400.0;

    double cos_gmst = cos(gmst_rad);
    double sin_gmst = sin(gmst_rad);


    if(matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = -1.0 * rate * sin_gmst;
        matrix->elements[1] = -1.0 * rate * cos_gmst;
        matrix->elements[2] = 0.0;
        matrix->elements[3] = rate * cos_gmst;
        matrix->elements[4] = -1.0 * rate * sin_gmst;
        matrix->elements[5] = 0.0;
        matrix->elements[6] = 0.0;
        matrix->elements[7] = 0.0;
        matrix->elements[8] = 0.0;

    }

    return;
}


#ifdef __cplusplus
} /* extern "C" */
#endif