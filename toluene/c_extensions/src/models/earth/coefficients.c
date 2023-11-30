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

#include "models/earth/coefficients.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* load_zeta_a(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_zeta_a(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->zeta_a) {
        if(coefficients->zeta_a->coefficients) free(coefficients->zeta_a->coefficients);
        free(coefficients->zeta_a);
    }

    coefficients->zeta_a = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->zeta_a) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->zeta_a.");
        return PyErr_Occurred();
    }

    coefficients->zeta_a->order = (int)PyList_Size(list);
    coefficients->zeta_a->coefficients = (double*)malloc(sizeof(double) * coefficients->zeta_a->order);
    if(!coefficients->zeta_a->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->zeta_a->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->zeta_a->order; i++) {
        coefficients->zeta_a->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_z_a(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_z_a(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->z_a) {
        if(coefficients->z_a->coefficients) free(coefficients->z_a->coefficients);
        free(coefficients->z_a);
    }

    coefficients->z_a = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->z_a) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->z_a.");
        return PyErr_Occurred();
    }

    coefficients->z_a->order = (int)PyList_Size(list);
    coefficients->z_a->coefficients = (double*)malloc(sizeof(double) * coefficients->z_a->order);
    if(!coefficients->z_a->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->z_a->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->z_a->order; i++) {
        coefficients->z_a->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_theta_a(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_theta_a(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->theta_a) {
        if(coefficients->theta_a->coefficients) free(coefficients->theta_a->coefficients);
        free(coefficients->theta_a);
    }

    coefficients->theta_a = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->theta_a) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->theta_a.");
        return PyErr_Occurred();
    }

    coefficients->theta_a->order = (int)PyList_Size(list);
    coefficients->theta_a->coefficients = (double*)malloc(sizeof(double) * coefficients->theta_a->order);
    if(!coefficients->theta_a->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->theta_a->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->theta_a->order; i++) {
        coefficients->theta_a->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_psi_a(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_psi_a(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->psi_a) {
        if(coefficients->psi_a->coefficients) free(coefficients->psi_a->coefficients);
        free(coefficients->psi_a);
    }

    coefficients->psi_a = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->psi_a) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->psi_a.");
        return PyErr_Occurred();
    }

    coefficients->psi_a->order = (int)PyList_Size(list);
    coefficients->psi_a->coefficients = (double*)malloc(sizeof(double) * coefficients->psi_a->order);
    if(!coefficients->psi_a->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->psi_a->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->psi_a->order; i++) {
        coefficients->psi_a->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_omega_a(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_omega_a(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->omega_a) {
        if(coefficients->omega_a->coefficients) free(coefficients->omega_a->coefficients);
        free(coefficients->omega_a);
    }

    coefficients->omega_a = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->omega_a) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->omega_a.");
        return PyErr_Occurred();
    }

    coefficients->omega_a->order = (int)PyList_Size(list);
    coefficients->omega_a->coefficients = (double*)malloc(sizeof(double) * coefficients->omega_a->order);
    if(!coefficients->omega_a->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->omega_a->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->omega_a->order; i++) {
        coefficients->omega_a->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_epsilon_a(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_epsilon_a(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->epsilon_a) {
        if(coefficients->epsilon_a->coefficients) free(coefficients->epsilon_a->coefficients);
        free(coefficients->epsilon_a);
    }

    coefficients->epsilon_a = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->epsilon_a) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->epsilon_a.");
        return PyErr_Occurred();
    }

    coefficients->epsilon_a->order = (int)PyList_Size(list);
    coefficients->epsilon_a->coefficients = (double*)malloc(sizeof(double) * coefficients->epsilon_a->order);
    if(!coefficients->epsilon_a->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->epsilon_a->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->epsilon_a->order; i++) {
        coefficients->epsilon_a->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_chi_a(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_chi_a(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->chi_a) {
        if(coefficients->chi_a->coefficients) free(coefficients->chi_a->coefficients);
        free(coefficients->chi_a);
    }

    coefficients->chi_a = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->chi_a) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->chi_a.");
        return PyErr_Occurred();
    }

    coefficients->chi_a->order = (int)PyList_Size(list);
    coefficients->chi_a->coefficients = (double*)malloc(sizeof(double) * coefficients->chi_a->order);
    if(!coefficients->chi_a->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->chi_a->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->chi_a->order; i++) {
        coefficients->chi_a->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l) {
        if(coefficients->l->coefficients) free(coefficients->l->coefficients);
        free(coefficients->l);
    }

    coefficients->l = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l.");
        return PyErr_Occurred();
    }

    coefficients->l->order = (int)PyList_Size(list);
    coefficients->l->coefficients = (double*)malloc(sizeof(double) * coefficients->l->order);
    if(!coefficients->l->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l->order; i++) {
        coefficients->l->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_prime(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_prime(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_prime) {
        if(coefficients->l_prime->coefficients) free(coefficients->l_prime->coefficients);
        free(coefficients->l_prime);
    }

    coefficients->l_prime = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_prime) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_prime.");
        return PyErr_Occurred();
    }

    coefficients->l_prime->order = (int)PyList_Size(list);
    coefficients->l_prime->coefficients = (double*)malloc(sizeof(double) * coefficients->l_prime->order);
    if(!coefficients->l_prime->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_prime->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_prime->order; i++) {
        coefficients->l_prime->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_F(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_F(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->F) {
        if(coefficients->F->coefficients) free(coefficients->F->coefficients);
        free(coefficients->F);
    }

    coefficients->F = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->F) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->F.");
        return PyErr_Occurred();
    }

    coefficients->F->order = (int)PyList_Size(list);
    coefficients->F->coefficients = (double*)malloc(sizeof(double) * coefficients->F->order);
    if(!coefficients->F->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->F->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->F->order; i++) {
        coefficients->F->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_D(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_D(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->D) {
        if(coefficients->D->coefficients) free(coefficients->D->coefficients);
        free(coefficients->D);
    }

    coefficients->D = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->D) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->D.");
        return PyErr_Occurred();
    }

    coefficients->D->order = (int)PyList_Size(list);
    coefficients->D->coefficients = (double*)malloc(sizeof(double) * coefficients->D->order);
    if(!coefficients->D->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->D->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->D->order; i++) {
        coefficients->D->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_Omega(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_Omega(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->Omega) {
        if(coefficients->Omega->coefficients) free(coefficients->Omega->coefficients);
        free(coefficients->Omega);
    }

    coefficients->Omega = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->Omega) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->Omega.");
        return PyErr_Occurred();
    }

    coefficients->Omega->order = (int)PyList_Size(list);
    coefficients->Omega->coefficients = (double*)malloc(sizeof(double) * coefficients->Omega->order);
    if(!coefficients->Omega->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->Omega->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->Omega->order; i++) {
        coefficients->Omega->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_me(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_me(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_me) {
        if(coefficients->l_me->coefficients) free(coefficients->l_me->coefficients);
        free(coefficients->l_me);
    }

    coefficients->l_me = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_me) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_me.");
        return PyErr_Occurred();
    }

    coefficients->l_me->order = (int)PyList_Size(list);
    coefficients->l_me->coefficients = (double*)malloc(sizeof(double) * coefficients->l_me->order);
    if(!coefficients->l_me->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_me->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_me->order; i++) {
        coefficients->l_me->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_v(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_v(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_v) {
        if(coefficients->l_v->coefficients) free(coefficients->l_v->coefficients);
        free(coefficients->l_v);
    }

    coefficients->l_v = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_v) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_v.");
        return PyErr_Occurred();
    }

    coefficients->l_v->order = (int)PyList_Size(list);
    coefficients->l_v->coefficients = (double*)malloc(sizeof(double) * coefficients->l_v->order);
    if(!coefficients->l_v->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_v->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_v->order; i++) {
        coefficients->l_v->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_e(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_e(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_e) {
        if(coefficients->l_e->coefficients) free(coefficients->l_e->coefficients);
        free(coefficients->l_e);
    }

    coefficients->l_e = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_e) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_e.");
        return PyErr_Occurred();
    }

    coefficients->l_e->order = (int)PyList_Size(list);
    coefficients->l_e->coefficients = (double*)malloc(sizeof(double) * coefficients->l_e->order);
    if(!coefficients->l_e->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_e->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_e->order; i++) {
        coefficients->l_e->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_ma(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_ma(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_ma) {
        if(coefficients->l_ma->coefficients) free(coefficients->l_ma->coefficients);
        free(coefficients->l_ma);
    }

    coefficients->l_ma = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_ma) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_ma.");
        return PyErr_Occurred();
    }

    coefficients->l_ma->order = (int)PyList_Size(list);
    coefficients->l_ma->coefficients = (double*)malloc(sizeof(double) * coefficients->l_ma->order);
    if(!coefficients->l_ma->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_ma->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_ma->order; i++) {
        coefficients->l_ma->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_j(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_j(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_j) {
        if(coefficients->l_j->coefficients) free(coefficients->l_j->coefficients);
        free(coefficients->l_j);
    }

    coefficients->l_j = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_j) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_j.");
        return PyErr_Occurred();
    }

    coefficients->l_j->order = (int)PyList_Size(list);
    coefficients->l_j->coefficients = (double*)malloc(sizeof(double) * coefficients->l_j->order);
    if(!coefficients->l_j->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_j->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_j->order; i++) {
        coefficients->l_j->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_s(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_s(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_s) {
        if(coefficients->l_s->coefficients) free(coefficients->l_s->coefficients);
        free(coefficients->l_s);
    }

    coefficients->l_s = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_s) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_s.");
        return PyErr_Occurred();
    }

    coefficients->l_s->order = (int)PyList_Size(list);
    coefficients->l_s->coefficients = (double*)malloc(sizeof(double) * coefficients->l_s->order);
    if(!coefficients->l_s->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_s->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_s->order; i++) {
        coefficients->l_s->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_u(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_u(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_u) {
        if(coefficients->l_u->coefficients) free(coefficients->l_u->coefficients);
        free(coefficients->l_u);
    }

    coefficients->l_u = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_u) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_u.");
        return PyErr_Occurred();
    }

    coefficients->l_u->order = (int)PyList_Size(list);
    coefficients->l_u->coefficients = (double*)malloc(sizeof(double) * coefficients->l_u->order);
    if(!coefficients->l_u->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_u->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_u->order; i++) {
        coefficients->l_u->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_l_n(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_l_n(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->l_n) {
        if(coefficients->l_n->coefficients) free(coefficients->l_n->coefficients);
        free(coefficients->l_n);
    }

    coefficients->l_n = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->l_n) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_n.");
        return PyErr_Occurred();
    }

    coefficients->l_n->order = (int)PyList_Size(list);
    coefficients->l_n->coefficients = (double*)malloc(sizeof(double) * coefficients->l_n->order);
    if(!coefficients->l_n->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->l_n->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->l_n->order; i++) {
        coefficients->l_n->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* load_p(PyObject* self, PyObject* args) {

    PyObject* capsule;
    CIRStoTIRSCoefficients* coefficients;
    PyObject* list;

    if(!PyArg_ParseTuple(args, "OO!", &capsule, &PyList_Type, &list)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for load_p(CIRStoTIRSCoefficients, double).");
        return PyErr_Occurred();
    }

    coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(capsule, "CIRStoTIRSCoefficients");
    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the CIRStoTIRSCoefficients from capsule.");
        return PyErr_Occurred();
    }

    if(coefficients->p) {
        if(coefficients->p->coefficients) free(coefficients->p->coefficients);
        free(coefficients->p);
    }

    coefficients->p = (Polynomial*)malloc(sizeof(Polynomial));
    if(!coefficients->p) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->p.");
        return PyErr_Occurred();
    }

    coefficients->p->order = (int)PyList_Size(list);
    coefficients->p->coefficients = (double*)malloc(sizeof(double) * coefficients->p->order);
    if(!coefficients->p->coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for coefficients->p->coefficients.");
        return PyErr_Occurred();
    }

    for(int i = 0; i < coefficients->p->order; i++) {
        coefficients->p->coefficients[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    Py_RETURN_NONE;
}


static PyObject* new_CIRStoTIRSCoefficients(PyObject* self, PyObject* args) {

    CIRStoTIRSCoefficients* coefficients = (CIRStoTIRSCoefficients*)malloc(sizeof(CIRStoTIRSCoefficients));

    if(!coefficients) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_CIRStoTIRSCoefficients.");
        return PyErr_Occurred();
    }

    coefficients->zeta_a = NULL;
    coefficients->z_a = NULL;
    coefficients->theta_a = NULL;
    coefficients->psi_a = NULL;
    coefficients->omega_a = NULL;
    coefficients->epsilon_a = NULL;
    coefficients->chi_a = NULL;

    coefficients->l = NULL;
    coefficients->l_prime = NULL;
    coefficients->F = NULL;
    coefficients->D = NULL;
    coefficients->Omega = NULL;
    coefficients->l_me = NULL;
    coefficients->l_v = NULL;
    coefficients->l_e = NULL;
    coefficients->l_ma = NULL;
    coefficients->l_j = NULL;
    coefficients->l_s = NULL;
    coefficients->l_u = NULL;
    coefficients->l_n = NULL;
    coefficients->p = NULL;

    return PyCapsule_New(coefficients, "CIRStoTIRSCoefficients", delete_CIRStoTIRSCoefficients);
}


static void delete_CIRStoTIRSCoefficients(PyObject* obj) {

    CIRStoTIRSCoefficients* coefficients = (CIRStoTIRSCoefficients*)PyCapsule_GetPointer(obj, "CIRStoTIRSCoefficients");

    if(coefficients) {
        free(coefficients);
    }

}


static PyMethodDef tolueneModelsEarthCoefficientsMethods[] = {
    {"load_zeta_a", load_zeta_a, METH_VARARGS, "Load the zeta_a coefficients."},
    {"load_z_a", load_z_a, METH_VARARGS, "Load the z_a coefficients."},
    {"load_theta_a", load_theta_a, METH_VARARGS, "Load the theta_a coefficients."},
    {"load_psi_a", load_psi_a, METH_VARARGS, "Load the psi_a coefficients."},
    {"load_omega_a", load_omega_a, METH_VARARGS, "Load the omega_a coefficients."},
    {"load_epsilon_a", load_epsilon_a, METH_VARARGS, "Load the epsilon_a coefficients."},
    {"load_chi_a", load_chi_a, METH_VARARGS, "Load the chi_a coefficients."},
    {"load_l", load_l, METH_VARARGS, "Load the l coefficients."},
    {"load_l_prime", load_l_prime, METH_VARARGS, "Load the l_prime coefficients."},
    {"load_F", load_F, METH_VARARGS, "Load the F coefficients."},
    {"load_D", load_D, METH_VARARGS, "Load the D coefficients."},
    {"load_Omega", load_Omega, METH_VARARGS, "Load the Omega coefficients."},
    {"load_l_me", load_l_me, METH_VARARGS, "Load the l_me coefficients."},
    {"load_l_v", load_l_v, METH_VARARGS, "Load the l_v coefficients."},
    {"load_l_e", load_l_e, METH_VARARGS, "Load the l_e coefficients."},
    {"load_l_ma", load_l_ma, METH_VARARGS, "Load the l_ma coefficients."},
    {"load_l_j", load_l_j, METH_VARARGS, "Load the l_j coefficients."},
    {"load_l_s", load_l_s, METH_VARARGS, "Load the l_s coefficients."},
    {"load_l_u", load_l_u, METH_VARARGS, "Load the l_u coefficients."},
    {"load_l_n", load_l_n, METH_VARARGS, "Load the l_n coefficients."},
    {"load_p", load_p, METH_VARARGS, "Load the p coefficients."},
    {"new_CIRStoTIRSCoefficients", new_CIRStoTIRSCoefficients, METH_VARARGS, "Create a new CIRStoTIRSCoefficients object."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_coefficients = {
    PyModuleDef_HEAD_INIT,
    "models.earth.coefficients",
    "C Extensions to toluene models coefficients functions",
    -1,
    tolueneModelsEarthCoefficientsMethods
};


PyMODINIT_FUNC PyInit_coefficients(void) {
    return PyModule_Create(&models_earth_coefficients);
}


#ifdef __cplusplus
} /* extern "C" */
#endif