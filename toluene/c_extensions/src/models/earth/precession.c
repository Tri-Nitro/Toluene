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
#define __compile_models_earth_precession
#include "math/algebra.h"
#include "models/earth/precession.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void iau_76_precession(double t, EarthModel* model, Matrix* matrix) {

    t = t / 3155760000.0;

    double zeta_a, z_a, theta_a;
    solve(model->cirs_coefficients->zeta_a, t, &zeta_a);
    solve(model->cirs_coefficients->z_a, t, &z_a);
    solve(model->cirs_coefficients->theta_a, t, &theta_a);

    double sin_zeta_a = sin(zeta_a * M_PI/648000);
    double cos_zeta_a = cos(zeta_a * M_PI/648000);
    double sin_z_a = sin(z_a * M_PI/648000);
    double cos_z_a = cos(z_a * M_PI/648000);
    double sin_theta_a = sin(theta_a * M_PI/648000);
    double cos_theta_a = cos(theta_a * M_PI/648000);

    if(matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = cos_z_a * cos_theta_a * cos_zeta_a -  sin_z_a * sin_zeta_a;
        matrix->elements[1] = cos_z_a * cos_theta_a * sin_zeta_a + sin_z_a * cos_zeta_a;
        matrix->elements[2] = cos_z_a * sin_theta_a;
        matrix->elements[3] = -1 * sin_z_a * cos_theta_a * cos_zeta_a - cos_z_a * sin_zeta_a;
        matrix->elements[4] = -1 * sin_z_a * cos_theta_a * sin_zeta_a + cos_z_a * cos_zeta_a;
        matrix->elements[5] = -1 * sin_z_a * sin_theta_a;
        matrix->elements[6] = -1 * sin_theta_a * cos_zeta_a;
        matrix->elements[7] = -1 * sin_theta_a * sin_zeta_a;
        matrix->elements[8] = cos_theta_a;

    }

}


void iau_2000a_precession(double t, EarthModel* model, Matrix* matrix) {

    t = t / 3155760000.0;

    double epsilon_0, psi_a, omega_a, chi_a;
    solve(model->cirs_coefficients->epsilon_a, 0, &epsilon_0);
    solve(model->cirs_coefficients->psi_a, t, &psi_a);
    solve(model->cirs_coefficients->omega_a, t, &omega_a);
    solve(model->cirs_coefficients->chi_a, t, &chi_a);

    double sin_epsilon_0 = sin(epsilon_0 * M_PI/648000);
    double cos_epsilon_0 = cos(epsilon_0 * M_PI/648000);
    double sin_psi_a = sin(-1 * psi_a * M_PI/648000);
    double cos_psi_a = cos(-1 * psi_a * M_PI/648000);
    double sin_omega_a = sin(-1 * omega_a * M_PI/648000);
    double cos_omega_a = cos(-1 * omega_a * M_PI/648000);
    double sin_chi_a = sin(chi_a * M_PI/648000);
    double cos_chi_a = cos(chi_a * M_PI/648000);

    if(matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = cos_chi_a * cos_psi_a - sin_psi_a * sin_chi_a * cos_omega_a;
        matrix->elements[1] = cos_chi_a * sin_psi_a * cos_epsilon_0 +
            sin_chi_a * cos_omega_a * cos_psi_a * cos_epsilon_0 - sin_epsilon_0 * sin_chi_a * sin_omega_a;
        matrix->elements[2] = cos_chi_a * sin_psi_a * sin_epsilon_0 +
            sin_chi_a * cos_omega_a * cos_psi_a * sin_epsilon_0 + cos_epsilon_0 * sin_chi_a * sin_omega_a;
        matrix->elements[3] = -1 * sin_chi_a * cos_omega_a - sin_psi_a * cos_chi_a * cos_omega_a;
        matrix->elements[4] = -1 * sin_chi_a * sin_psi_a * cos_epsilon_0 +
            cos_chi_a * cos_omega_a * cos_psi_a * cos_epsilon_0 - sin_epsilon_0 * cos_chi_a * sin_omega_a;
        matrix->elements[5] = -1 * sin_chi_a * sin_psi_a * sin_epsilon_0 +
            cos_chi_a * cos_omega_a * cos_psi_a * sin_epsilon_0 + cos_epsilon_0 * cos_chi_a * sin_omega_a;
        matrix->elements[6] = -1 * sin_psi_a * sin_omega_a;
        matrix->elements[7] = -1 * sin_omega_a * cos_psi_a * cos_epsilon_0 - sin_epsilon_0 * cos_omega_a;
        matrix->elements[8] = -1 * sin_omega_a * cos_psi_a * sin_epsilon_0 + cos_omega_a * cos_epsilon_0;

    }

}


#ifdef __cplusplus
} /* extern "C" */
#endif