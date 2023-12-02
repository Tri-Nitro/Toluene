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
#ifndef __MATH_LINEAR_ALGEBRA_H__
#define __MATH_LINEAR_ALGEBRA_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int nrows, ncols;
    double* elements;
} Matrix;

typedef struct {
    int nelements;
    double* elements;
} Vector;

static PyObject* new_Vector(Vector* vector, int nelements);
static PyObject* new_Matrix(Matrix* matrix, int nrows, int ncols);

void delete_Vector(PyObject* obj);
void delete_Matrix(PyObject* obj);

void dot_product(Vector* vector, Matrix* matrix, Vector* product);
void dot_product_matrix_transpose(Vector* vector, Matrix* matrix, Vector* product);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MATH_LINEAR_ALGEBRA_H__ */