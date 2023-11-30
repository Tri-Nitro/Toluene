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

#include "models/earth/model.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* get_ellipsoid(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. get_ellipsoid(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->ellipsoid, "Ellipsoid", NULL);
}


static PyObject* set_ellipsoid(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* ellipsoid_capsule;

    EarthModel* earth_model;
    Ellipsoid* ellipsoid;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &ellipsoid_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. set_ellipsoid(EarthModel, Ellipsoid)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    ellipsoid = (Ellipsoid*)PyCapsule_GetPointer(ellipsoid_capsule, "Ellipsoid");
    if(!ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Ellipsoid from capsule.");
        return PyErr_Occurred();
    }

    earth_model->ellipsoid = ellipsoid;

    Py_RETURN_NONE;
}


static PyObject* get_cirs_to_tirs_coefficients(PyObject* self, PyObject* args) {

    PyObject* capsule;

    EarthModel* earth_model;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. cirs_to_tirs_coefficients(EarthModel)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model && earth_model->ellipsoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule. Or the ellipsoid is NULL.");
        return PyErr_Occurred();
    }

    return PyCapsule_New(earth_model->cirs_to_tirs_coefficients, "CIRStoTIRSCoefficients", NULL);
}


static PyObject* set_cirs_to_tirs_coefficients(PyObject* self, PyObject* args) {

    PyObject* capsule;
    PyObject* cirs_to_tirs_coefficients_capsule;

    EarthModel* earth_model;
    CIRStoTIRSCoefficients* cirs_to_tirs_coefficients;

    if(!PyArg_ParseTuple(args, "OO", &capsule, &cirs_to_tirs_coefficients_capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. set_cirs_to_tirs_coefficients(EarthModel, CIRStoTIRSCoefficients)");
        return PyErr_Occurred();
    }

    earth_model = (EarthModel*)PyCapsule_GetPointer(capsule, "EarthModel");
    if(!earth_model) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the EarthModel from capsule.");
        return PyErr_Occurred();
    }

    cirs_to_tirs_coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(cirs_to_tirs_coefficients_capsule, "CIRStoTIRSCoefficients");
    if(!cirs_to_tirs_coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    earth_model->cirs_to_tirs_coefficients = cirs_to_tirs_coefficients;

    Py_RETURN_NONE;
}


static PyObject* new_EarthModel(PyObject* self, PyObject* args) {

    EarthModel* model = (EarthModel*)malloc(sizeof(EarthModel));

    if(model == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_EarthModel.");
        return PyErr_Occurred();
    }

    model->cirs_to_tirs_coefficients = NULL;
    model->ellipsoid = NULL;

    return PyCapsule_New(model, "EarthModel", delete_EarthModel);
}


static void delete_EarthModel(PyObject* obj) {

    EarthModel* pointer = (EarthModel*)PyCapsule_GetPointer(obj, "EarthModel");

    if(pointer) {

        if(pointer->cirs_to_tirs_coefficients) {

            if(pointer->cirs_to_tirs_coefficients->zeta_a) {
                if(pointer->cirs_to_tirs_coefficients->zeta_a->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->zeta_a->coefficients);
                free(pointer->cirs_to_tirs_coefficients->zeta_a);
            }

            if(pointer->cirs_to_tirs_coefficients->z_a) {
                if(pointer->cirs_to_tirs_coefficients->z_a->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->z_a->coefficients);
                free(pointer->cirs_to_tirs_coefficients->z_a);
            }

            if(pointer->cirs_to_tirs_coefficients->theta_a) {
                if(pointer->cirs_to_tirs_coefficients->theta_a->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->theta_a->coefficients);
                free(pointer->cirs_to_tirs_coefficients->theta_a);
            }

            if(pointer->cirs_to_tirs_coefficients->psi_a) {
                if(pointer->cirs_to_tirs_coefficients->psi_a->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->psi_a->coefficients);
                free(pointer->cirs_to_tirs_coefficients->psi_a);
            }

            if(pointer->cirs_to_tirs_coefficients->omega_a) {
                if(pointer->cirs_to_tirs_coefficients->omega_a->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->omega_a->coefficients);
                free(pointer->cirs_to_tirs_coefficients->omega_a);
            }

            if(pointer->cirs_to_tirs_coefficients->epsilon_a) {
                if(pointer->cirs_to_tirs_coefficients->epsilon_a->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->epsilon_a->coefficients);
                free(pointer->cirs_to_tirs_coefficients->epsilon_a);
            }

            if(pointer->cirs_to_tirs_coefficients->chi_a) {
                if(pointer->cirs_to_tirs_coefficients->chi_a->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->chi_a->coefficients);
                free(pointer->cirs_to_tirs_coefficients->chi_a);
            }

            if(pointer->cirs_to_tirs_coefficients->l) {
                if(pointer->cirs_to_tirs_coefficients->l->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l);
            }

            if(pointer->cirs_to_tirs_coefficients->l_prime) {
                if(pointer->cirs_to_tirs_coefficients->l_prime->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_prime->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_prime);
            }

            if(pointer->cirs_to_tirs_coefficients->F) {
                if(pointer->cirs_to_tirs_coefficients->F->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->F->coefficients);
                free(pointer->cirs_to_tirs_coefficients->F);
            }

            if(pointer->cirs_to_tirs_coefficients->D) {
                if(pointer->cirs_to_tirs_coefficients->D->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->D->coefficients);
                free(pointer->cirs_to_tirs_coefficients->D);
            }

            if(pointer->cirs_to_tirs_coefficients->Omega) {
                if(pointer->cirs_to_tirs_coefficients->Omega->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->Omega->coefficients);
                free(pointer->cirs_to_tirs_coefficients->Omega);
            }

            if(pointer->cirs_to_tirs_coefficients->l_me) {
                if(pointer->cirs_to_tirs_coefficients->l_me->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_me->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_me);
            }

            if(pointer->cirs_to_tirs_coefficients->l_v) {
                if(pointer->cirs_to_tirs_coefficients->l_v->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_v->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_v);
            }

            if(pointer->cirs_to_tirs_coefficients->l_e) {
                if(pointer->cirs_to_tirs_coefficients->l_e->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_e->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_e);
            }

            if(pointer->cirs_to_tirs_coefficients->l_ma) {
                if(pointer->cirs_to_tirs_coefficients->l_ma->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_ma->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_ma);
            }

            if(pointer->cirs_to_tirs_coefficients->l_j) {
                if(pointer->cirs_to_tirs_coefficients->l_j->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_j->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_j);
            }

            if(pointer->cirs_to_tirs_coefficients->l_s) {
                if(pointer->cirs_to_tirs_coefficients->l_s->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_s->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_s);
            }

            if(pointer->cirs_to_tirs_coefficients->l_u) {
                if(pointer->cirs_to_tirs_coefficients->l_u->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_u->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_u);
            }

            if(pointer->cirs_to_tirs_coefficients->l_n) {
                if(pointer->cirs_to_tirs_coefficients->l_n->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->l_n->coefficients);
                free(pointer->cirs_to_tirs_coefficients->l_n);
            }

            if(pointer->cirs_to_tirs_coefficients->p) {
                if(pointer->cirs_to_tirs_coefficients->p->coefficients)
                    free(pointer->cirs_to_tirs_coefficients->p->coefficients);
                free(pointer->cirs_to_tirs_coefficients->p);
            }

            free(pointer->cirs_to_tirs_coefficients);
        }

        free(pointer);
    }

}


static PyMethodDef tolueneModelsEarthModelMethods[] = {
    {"get_ellipsoid", get_ellipsoid, METH_VARARGS, "Get the ellipsoid of the EarthModel"},
    {"set_ellipsoid", set_ellipsoid, METH_VARARGS, "Set the ellipsoid of the EarthModel"},
    {"new_EarthModel", new_EarthModel, METH_VARARGS, "Create a new EarthModel object"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_model = {
    PyModuleDef_HEAD_INIT,
    "models.earth.model",
    "C Extensions to toluene models earth functions",
    -1,
    tolueneModelsEarthModelMethods
};


PyMODINIT_FUNC PyInit_model(void) {
    return PyModule_Create(&models_earth_model);
}


#ifdef __cplusplus
} /* extern "C" */
#endif