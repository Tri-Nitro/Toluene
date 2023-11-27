#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "interpolation.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


void bilinear_interpolation(double x, double y, double corners[], double* value) {
    double f1 = (corners[6]-x)/(corners[6]-corners[0])*(corners[2]) + (x-corners[0])/(corners[6]-corners[0])*(corners[8]);
    double f2 = (corners[6]-x)/(corners[6]-corners[0])*(corners[5]) + (x-corners[0])/(corners[6]-corners[0])*(corners[11]);
    *value = ((corners[4]-y)/(corners[4]-corners[1]))*f1 + ((y-corners[1])/(corners[4]-corners[1]))*f2;
}

#ifdef __cplusplus
extern "C"
{
#endif