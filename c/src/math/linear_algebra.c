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

#define __compile_math_linear_algebra__
#include "math/linear_algebra.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Computes the dot product of a matrix and a vector
 * @param matrix The matrix
 * @param vector The vector
 * @param product The dot product of the matrix and vector
 */
void dot_product(Mat3* matrix, Vec3* vector, Vec3* product) {

    if(matrix && vector && product) {
        product->x = matrix->w11 * vector->x + matrix->w12 * vector->y + matrix->w13 * vector->z;
        product->y = matrix->w21 * vector->x + matrix->w22 * vector->y + matrix->w23 * vector->z;
        product->z = matrix->w31 * vector->x + matrix->w32 * vector->y + matrix->w33 * vector->z;
    }
}

/**
 * @brief Computes the dot product of the transpose of a matrix and a vector
 * @param matrix The matrix
 * @param vector The vector
 * @param product The dot product of the matrix transpose and vector
 */
void dot_product_transpose(Mat3* matrix, Vec3* vector, Vec3* product) {

    if(matrix && vector && product) {
        product->x = matrix->w11 * vector->x + matrix->w21 * vector->y + matrix->w31 * vector->z;
        product->y = matrix->w12 * vector->x + matrix->w22 * vector->y + matrix->w32 * vector->z;
        product->z = matrix->w13 * vector->x + matrix->w23 * vector->y + matrix->w33 * vector->z;
    }
}

/**
 * @brief Computes the cross product of two vectors
 * @param vector1 The first vector
 * @param vector2 The second vector
 * @param product The cross product of the two vectors
 */
void cross_product(Vec3* vector1, Vec3* vector2, Vec3* product) {

    if(vector1 && vector2 && product) {
        product->x = vector1->y * vector2->z - vector1->z * vector2->y;
        printf("CROSS PRODUCT: %Lf = %Lf * %Lf - %Lf * %Lf\n", product->x, vector1->y, vector2->z, vector1->z, vector2->y);
        product->y = vector1->z * vector2->x - vector1->x * vector2->z;
        printf("CROSS PRODUCT: %Lf = %Lf * %Lf - %Lf * %Lf\n", product->y, vector1->z, vector2->x, vector1->x, vector2->z);
        product->z = vector1->x * vector2->y - vector1->y * vector2->x;
        printf("CROSS PRODUCT: %Lf = %Lf * %Lf - %Lf * %Lf\n", product->z, vector1->x, vector2->y, vector1->y, vector2->x);
    }
}

/**
 * @brief Normalizes a vector
 * @param vector The vector to normalize
 */
void normalize(Vec3* vector) {

    if(vector) {
        long double mag = 0;
        magnitude(vector, &mag);
        vector->x /= mag;
        vector->y /= mag;
        vector->z /= mag;
    }
}

/**
 * @brief Computes the magnitude of a vector
 * @param vector The vector
 * @param mag The magnitude of the vector
 */
void magnitude(Vec3* vector, long double* mag) {

    if(vector && mag) {
        *mag = 0;
        *mag += vector->x * vector->x;
        *mag += vector->y * vector->y;
        *mag += vector->z * vector->z;
        *mag = sqrt(*mag);
    }
}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */