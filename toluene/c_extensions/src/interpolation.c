#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "interpolation.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

double bilinear_interpolation(double x, double y, double corners[]) {
    return 0.0;
}

#ifdef __cplusplus
extern "C"
{
#endif