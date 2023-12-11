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

#include "math/linear_algebra.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


//static PyObject* new_Vector(Vector* vector, int nelements) {
//
//    if(vector) {
//        if(vector->nelements == nelements) {
//            return PyCapsule_New(vector, "Vector", NULL);
//        }
//        free(vector->elements);
//    }
//
//    vector = (Vector*)malloc(sizeof(Vector));
//    vector->nelements = nelements;
//    vector->elements = (double*)malloc(sizeof(double) * nelements);
//
//    return PyCapsule_New(vector, "Vector", delete_Vector);
//}
//
//
//static PyObject* new_Matrix(Matrix* matrix, int nrows, int ncols) {
//
//    if(matrix->elements) {
//        if(matrix->nrows*matrix->ncols == nrows*ncols) {
//            matrix->nrows = nrows;
//            matrix->ncols = ncols;
//            return PyCapsule_New(matrix, "Matrix", NULL);
//        }
//        delete_Matrix(matrix->elements);
//    }
//
//    matrix = (Matrix*)malloc(sizeof(Matrix));
//    matrix->nrows = nrows;
//    matrix->ncols = ncols;
//    matrix->elements = (double*)malloc(sizeof(double) * nrows * ncols);
//
//    return PyCapsule_New(matrix, "Matrix", delete_Matrix);
//}
//
//
//void delete_Vector(PyObject* obj) {
//
//    Vector* vector = (Vector*)PyCapsule_GetPointer(obj, "Vector");
//
//    if(vector) {
//        if(vector->elements) {
//            free(vector->elements);
//        }
//        free(vector);
//    }
//
//}
//
//
//void delete_Matrix(PyObject* obj) {
//
//    Matrix* matrix = (Matrix*)PyCapsule_GetPointer(obj, "Matrix");
//
//    if(matrix) {
//        if(matrix->elements) {
//            free(matrix->elements);
//        }
//        free(matrix);
//    }
//
//}


void dot_product(Vector* vector, Matrix* matrix, Vector* product) {

    if(vector && matrix && vector->nelements == matrix->nrows) {

        for(int i = 0; i < vector->nelements; i++) {
            product->elements[i] = 0;
            for(int j = 0; j < matrix->ncols; j++) {
                product->elements[i] += vector->elements[j] * matrix->elements[i*matrix->ncols+j];
            }
        }
    }
}


void dot_product_matrix_transpose(Vector* vector, Matrix* matrix, Vector* product) {

    if(vector && matrix && vector->nelements == matrix->ncols) {

        for(int i = 0; i < vector->nelements; i++) {
            product->elements[i] = 0;
            for(int j = 0; j < matrix->nrows; j++) {
                product->elements[i] += vector->elements[j] * matrix->elements[j*matrix->nrows+i];
            }
        }
    }
}


void cross_product_3_3(Vector* vector1, Vector* vector2, Vector* product) {

    if(vector1 && vector2 && product && vector1->nelements == 3 && vector2->nelements == 3 && product->nelements == 3) {

        product->elements[0] = vector1->elements[1] * vector2->elements[2] - vector1->elements[2] * vector2->elements[1];
        product->elements[1] = vector1->elements[2] * vector2->elements[0] - vector1->elements[0] * vector2->elements[2];
        product->elements[2] = vector1->elements[0] * vector2->elements[1] - vector1->elements[1] * vector2->elements[0];
    }

}


void normalize(Vector* vector) {

    if(vector) {
        double mag = 0;
        magnitude(vector, &mag);
        for(int i = 0; i < vector->nelements; i++) {
            vector->elements[i] /= mag;
        }
    }

}


void magnitude(Vector* vector, double* mag) {

        if(vector) {
            *mag = 0;
            for(int i = 0; i < vector->nelements; i++) {
                *mag += vector->elements[i] * vector->elements[i];
            }
            *mag = sqrt(*mag);
        }

}


#ifdef __cplusplus
} /* extern "C" */
#endif