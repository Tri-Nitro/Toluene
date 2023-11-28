#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "matrix.h"
#include "polynomial.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void matrix_dot_product_3_3_in_place(double a[], double b[]) {
    double spare_0, spare_1;
    spare_0 = a[0];
    spare_1 = a[1];
    a[0] = spare_0*b[0] + spare_1*b[3] + a[2]*b[6];
    a[1] = spare_0*b[1] + spare_1*b[4] + a[2]*b[7];
    a[2] = spare_0*b[2] + spare_1*b[5] + a[2]*b[8];
    spare_0 = a[3];
    spare_1 = a[4];
    a[3] = spare_0*b[0] + spare_1*b[3] + a[5]*b[6];
    a[4] = spare_0*b[1] + spare_1*b[4] + a[5]*b[7];
    a[5] = spare_0*b[2] + spare_1*b[5] + a[5]*b[8];
    spare_0 = a[6];
    spare_1 = a[7];
    a[6] = spare_0*b[0] + spare_1*b[3] + a[8]*b[6];
    a[7] = spare_0*b[1] + spare_1*b[4] + a[8]*b[7];
    a[8] = spare_0*b[2] + spare_1*b[5] + a[8]*b[8];
}


void matrix_transpose_3_3_in_place(double a[]) {
    double spare;
    spare = a[1];
    a[1] = a[3];
    a[3] = spare;
    spare = a[2];
    a[2] = a[6];
    a[6] = spare;
    spare = a[5];
    a[5] = a[7];
    a[7] = spare;
}


void matrix_dot_product_3_3_vector_in_place(double a[], double b[]) {
    double spare_0, spare_1;
    spare_0 = a[0];
    spare_1 = a[1];
    a[0] = spare_0*b[0] + spare_1*b[1] + a[2]*b[2];
    a[1] = spare_0*b[3] + spare_1*b[4] + a[2]*b[5];
    a[2] = spare_0*b[6] + spare_1*b[7] + a[2]*b[8];

}


#ifdef __cplusplus
extern "C"
{
#endif