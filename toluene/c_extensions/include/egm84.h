#ifndef __EGM84_H__
#define __EGM84_H__
#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GravityCoefficients {
    int n, m;
    double C, S;
} GravityCoefficients;

typedef struct EGM84_Coefficients {
    char* filename;
    int num_coefficients;
    GravityCoefficients *coefficients;
} EGM84_Coefficients;

static void delete_egm84_coefficients(PyObject* obj);
static PyObject* load_egm84_coefficients(PyObject* self, PyObject* args);
static PyObject* test_egm84(PyObject* self, PyObject* args);


#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __MATRIX_H__ */