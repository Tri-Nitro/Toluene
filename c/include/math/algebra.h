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
#ifndef __MATH_ALGEBRA_H__
#define __MATH_ALGEBRA_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/** @struct
 * @brief A polynomial
 *
 * @var order The order of the polynomial
 * @var coefficients The coefficients of the polynomial
 */
typedef struct {
    int order;
    long double* coefficients;
} Polynomial;

#ifdef __compile_math_algebra__

/**
 * @brief Solve a polynomial for the value at a given x
 *
 * @param polynomial The polynomial
 * @param x The variable
 * @param value The value
 */
void solve(Polynomial* polynomial, double x, double* value);

/**
 * @brief Create a new polynomial available in python
 *
 * @param coefficients A python list of coefficients
 * @return Polynomial* The polynomial
 */
static PyObject* new_Polynomial(PyObject* self, PyObject* args);

/**
 * @brief Delete a polynomial
 *
 * @param obj The polynomial
 */
void delete_Polynomial(PyObject* obj);


#endif /* __compile_math_algebra__ */


#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */


#endif /* __MATH_ALGEBRA_H__ */