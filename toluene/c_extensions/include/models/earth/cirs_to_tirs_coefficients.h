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
#ifndef __MODELS_EARTH_COEFFICIENTS_H__
#define __MODELS_EARTH_COEFFICIENTS_H__

#include "math/algebra.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

    Polynomial* zeta_a;
    Polynomial* z_a;
    Polynomial* theta_a;
    Polynomial* psi_a;
    Polynomial* omega_a;
    Polynomial* epsilon_a;
    Polynomial* chi_a;

    Polynomial* l;
    Polynomial* l_prime;
    Polynomial* F;
    Polynomial* D;
    Polynomial* Omega;
    Polynomial* l_me;
    Polynomial* l_v;
    Polynomial* l_e;
    Polynomial* l_ma;
    Polynomial* l_j;
    Polynomial* l_s;
    Polynomial* l_u;
    Polynomial* l_n;
    Polynomial* p;

} CIRStoTIRSCoefficients;

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MODELS_EARTH_COEFFICIENTS_H__ */