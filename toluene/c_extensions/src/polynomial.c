#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "polynomial.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void compute_polynomial(const double coefficients[], double x, int highest_order, double* value) {

    *value = 0.0;

    for(int i = highest_order; i >= 0; --i) {
        *value = *value * x + coefficients[i];
    }

}

#ifdef __cplusplus
extern "C"
{
#endif