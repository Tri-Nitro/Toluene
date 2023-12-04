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

#define __compile_math_algebra
#include "math/algebra.h"
#include "models/earth/equinox.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void get_delta_psi_delta_epsilon_epsilon_eq_eq(double t, EarthModel* model, double* delta_psi, double* delta_epsilon,
    double* epsilon, double* eq_eq) {

    t = t / 3155760000.0;
    double nutation_critical_arguments[14];

    solve(model->cirs_coefficients->l_me, t, nutation_critical_arguments);
    solve(model->cirs_coefficients->l_v, t, nutation_critical_arguments+1);
    solve(model->cirs_coefficients->l_e, t, nutation_critical_arguments+2);
    solve(model->cirs_coefficients->l_ma, t, nutation_critical_arguments+3);
    solve(model->cirs_coefficients->l_j, t, nutation_critical_arguments+4);
    solve(model->cirs_coefficients->l_s, t, nutation_critical_arguments+5);
    solve(model->cirs_coefficients->l_u, t, nutation_critical_arguments+6);
    solve(model->cirs_coefficients->l_n, t, nutation_critical_arguments+7);
    solve(model->cirs_coefficients->p, t, nutation_critical_arguments+8);
    solve(model->cirs_coefficients->l, t, nutation_critical_arguments+9);
    solve(model->cirs_coefficients->l_prime, t, nutation_critical_arguments+10);
    solve(model->cirs_coefficients->F, t, nutation_critical_arguments+11);
    solve(model->cirs_coefficients->D, t, nutation_critical_arguments+12);
    solve(model->cirs_coefficients->Omega, t, nutation_critical_arguments+13);

    int n_series = model->nutation_series->nrecords;
    NutationSeries* nutation_series = model->nutation_series;

    double ai, sin_ai, cos_ai;

    printf("Nutation Series: %d\n", n_series);

    for(size_t i = 0; i < n_series; ++i) {
        ai = nutation_series->records[i].a * nutation_critical_arguments[0] +
             nutation_series->records[i].b * nutation_critical_arguments[1] +
             nutation_series->records[i].c * nutation_critical_arguments[2] +
             nutation_series->records[i].d * nutation_critical_arguments[3] +
             nutation_series->records[i].e * nutation_critical_arguments[4] +
             nutation_series->records[i].f * nutation_critical_arguments[5] +
             nutation_series->records[i].g * nutation_critical_arguments[6] +
             nutation_series->records[i].h * nutation_critical_arguments[7] +
             nutation_series->records[i].j * nutation_critical_arguments[8] +
             nutation_series->records[i].k * nutation_critical_arguments[9] +
             nutation_series->records[i].m * nutation_critical_arguments[10] +
             nutation_series->records[i].n * nutation_critical_arguments[11] +
             nutation_series->records[i].o * nutation_critical_arguments[12] +
             nutation_series->records[i].r * nutation_critical_arguments[13];

        sin_ai = sin(ai * M_PI/648000);
        cos_ai = cos(ai * M_PI/648000);

        *delta_psi += (nutation_series->records[i].S + nutation_series->records[i].S_dot * t) * sin_ai +
                                    nutation_series->records[i].C * cos_ai;
        *delta_epsilon += (nutation_series->records[i].C + nutation_series->records[i].C_dot * t) * cos_ai +
                                    nutation_series->records[i].S * sin_ai;
        *eq_eq += nutation_series->records[i].C_prime * sin_ai + nutation_series->records[i].S_prime * cos_ai;
    }

    solve(model->cirs_coefficients->epsilon_a, t, epsilon);
    *eq_eq += *delta_psi * cos(*epsilon * M_PI/648000) +
        0.00000087 * t * sin(nutation_critical_arguments[13] * M_PI/648000);
    *epsilon += *delta_epsilon;

}


#ifdef __cplusplus
} /* extern "C" */
#endif