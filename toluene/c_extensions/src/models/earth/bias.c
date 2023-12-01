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

#include "models/earth/bias.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void icrs_to_mean_j2000_bias_approximation(CIRSCoefficients* coefficients, Matrix* matrix) {

    if(coefficients && matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        matrix->elements[0] = 1.0;
        matrix->elements[1] = coefficients->d_alpha_0;
        matrix->elements[2] = -1.0 * coefficients->xi_0;
        matrix->elements[3] = -1.0 * coefficients->d_alpha_0;
        matrix->elements[4] = 1.0;
        matrix->elements[5] = -1.0 * coefficients->eta_0;
        matrix->elements[6] = coefficients->xi_0;
        matrix->elements[7] = coefficients->eta_0;
        matrix->elements[8] = 1.0;

    }
}


void icrs_to_mean_j2000_bias_exact(CIRSCoefficients* coefficients, Matrix* matrix) {

    if(coefficients && matrix && matrix->ncols == 3 && matrix->nrows == 3) {

        double d_alpha_squared = coefficients->d_alpha_0 * coefficients->d_alpha_0;
        double xi_squared = coefficients->xi_0 * coefficients->xi_0;
        double eta_squared = coefficients->eta_0 * coefficients->eta_0;

        matrix->elements[0] = 1.0 - 0.5*(d_alpha_squared + xi_squared);
        matrix->elements[1] = coefficients->d_alpha_0;
        matrix->elements[2] = -1.0 * coefficients->xi_0;
        matrix->elements[3] = -1.0 * coefficients->d_alpha_0 - coefficients->eta_0 * coefficients->xi_0;
        matrix->elements[4] = 1.0 - 0.5*(d_alpha_squared + eta_squared);
        matrix->elements[5] = -1.0 * coefficients->eta_0;
        matrix->elements[6] = coefficients->xi_0 - coefficients->eta_0 * coefficients->d_alpha_0;
        matrix->elements[7] = coefficients->eta_0 + coefficients->xi_0 * coefficients->d_alpha_0;
        matrix->elements[8] = 1.0 - 0.5*(eta_squared + xi_squared);

    }
}


#ifdef __cplusplus
} /* extern "C" */
#endif