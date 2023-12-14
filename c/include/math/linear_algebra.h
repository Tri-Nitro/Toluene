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
#endif /* __cplusplus */

/** @struct
 *  @brief A 3D vector
 *  @var Vec3::x
 *  Member 'x' is the x component of the vector
 *  @var Vec3::y
 *  Member 'y' is the y component of the vector
 *  @var Vec3::z
 *  Member 'z' is the z component of the vector
 */
typedef struct {
    long double x, y, z;
} Vec3;

/**
 * @struct
 * @brief A 3x3 matrix
 * @var Mat3::w11
 * Member 'w11' is the 1st row, 1st column element of the matrix
 * @var Mat3::w12
 * Member 'w12' is the 1st row, 2nd column element of the matrix
 * @var Mat3::w13
 * Member 'w13' is the 1st row, 3rd column element of the matrix
 * @var Mat3::w21
 * Member 'w21' is the 2nd row, 1st column element of the matrix
 * @var Mat3::w22
 * Member 'w22' is the 2nd row, 2nd column element of the matrix
 * @var Mat3::w23
 * Member 'w23' is the 2nd row, 3rd column element of the matrix
 * @var Mat3::w31
 * Member 'w31' is the 3rd row, 1st column element of the matrix
 * @var Mat3::w32
 * Member 'w32' is the 3rd row, 2nd column element of the matrix
 * @var Mat3::w33
 * Member 'w33' is the 3rd row, 3rd column element of the matrix
 */
typedef struct {
    long double w11, w12, w13;
    long double w21, w22, w23;
    long double w31, w32, w33;
} Mat3;


#ifdef __compile_math_linear_algebra__

/**
 * @brief Computes the dot product of a matrix and a vector
 * @param matrix The matrix
 * @param vector The vector
 * @param product The dot product of the matrix and vector
 */
void dot_product(Mat3* matrix, Vec3* vector, Vec3* product);

/**
 * @brief Computes the dot product of the transpose of a matrix and a vector
 * @param matrix The matrix
 * @param vector The vector
 * @param product The dot product of the matrix transpose and vector
 */
void dot_product_transpose(Mat3* matrix, Vec3* vector, Vec3* product);

/**
 * @brief Computes the cross product of two vectors
 * @param vector1 The first vector
 * @param vector2 The second vector
 * @param product The cross product of the two vectors
 */
void cross_product(Vec3* vector1, Vec3* vector2, Vec3* product);

/**
 * @brief Normalizes a vector
 * @param vector The vector to normalize
 */
void normalize(Vec3* vector);

/**
 * @brief Computes the magnitude of a vector
 * @param vector The vector
 * @param mag The magnitude of the vector
 */
void magnitude(Vec3* vector, long double* mag);


#endif /* __compile_math_linear_algebra__ */


#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */


#endif /* __MATH_LINEAR_ALGEBRA_H__ */