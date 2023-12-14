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

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif

#define __compile_coordinates_state_vector__
#include "coordinates/state_vector.h"

/**
 * @brief Creates a new StateVector object and makes it available to Python
 */
static PyObject* new_StateVector(PyObject* self, PyObject* args){

    StateVector* state_vector = (StateVector*)malloc(sizeof(StateVector));

    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_StateVector.");
        return PyErr_Occurred();
    }

    double x, y, z, vx, vy, vz, ax, ay, az, time;
    int frame;

    if(!PyArg_ParseTuple(args, "ddddddddddi", &x, &y, &z, &vx, &vy, &vz, &ax, &ay, &az, &time, &frame)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. new_StateVector()");
        return PyErr_Occurred();
    }

    state_vector->r.x = (long double)x;
    state_vector->r.y = (long double)y;
    state_vector->r.z = (long double)z;
    state_vector->v.x = (long double)vx;
    state_vector->v.y = (long double)vy;
    state_vector->v.z = (long double)vz;
    state_vector->a.x = (long double)ax;
    state_vector->a.y = (long double)ay;
    state_vector->a.z = (long double)az;
    state_vector->time = (long double)time;
    state_vector->frame = frame;

    printf("AFTER --- x: %Lf, y: %Lf, z: %Lf, vx: %Lf, vy: %Lf, vz: %Lf, ax: %Lf, ay: %Lf, az: %Lf, time: %Lf, frame: %i\n", state_vector->r.x, state_vector->r.y, state_vector->r.z, state_vector->v.x, state_vector->v.y, state_vector->v.z, state_vector->a.x, state_vector->a.y, state_vector->a.z, state_vector->time, state_vector->frame);

    return PyCapsule_New(state_vector, "StateVector", delete_StateVector);
}

/**
 * @brief Deletes the StateVector object
 *
 * @param obj The StateVector object
 */
void delete_StateVector(PyObject* obj) {

    StateVector* state_vector = (StateVector*)PyCapsule_GetPointer(obj, "StateVector");

    if(state_vector) {
        free(state_vector);
    }
}

/**
 * @brief Sets the position vector.
 */
static PyObject* set_position(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;
    double x, y, z;

    if(!PyArg_ParseTuple(args, "Oddd", &capsule, &x, &y, &z)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for set_position.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    state_vector->r.x = x;
    state_vector->r.y = y;
    state_vector->r.z = z;

    Py_RETURN_NONE;
}

/**
 * @brief Sets the velocity vector.
 */
static PyObject* set_velocity(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;
    double x, y, z;

    if(!PyArg_ParseTuple(args, "Oddd", &capsule, &x, &y, &z)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for set_velocity.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    state_vector->v.x = x;
    state_vector->v.y = y;
    state_vector->v.z = z;

    Py_RETURN_NONE;
}

/**
 * @brief Sets the acceleration vector.
 */
static PyObject* set_acceleration(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;
    double x, y, z;

    if(!PyArg_ParseTuple(args, "Oddd", &capsule, &x, &y, &z)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for set_acceleration.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    state_vector->a.x = x;
    state_vector->a.y = y;
    state_vector->a.z = z;

    Py_RETURN_NONE;
}

/**
 * @brief Sets the timestamp.
 */
static PyObject* set_time(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;
    double time;

    if(!PyArg_ParseTuple(args, "Od", &capsule, &time)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for set_time.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    state_vector->time = time;

    Py_RETURN_NONE;
}

/**
 * @brief Sets the reference frame.
 */
static PyObject* set_frame(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;
    ReferenceFrame frame;

    if(!PyArg_ParseTuple(args, "Oi", &capsule, &frame)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for set_frame.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    state_vector->frame = frame;

    Py_RETURN_NONE;
}

/**
 * @brief Gets the position vector.
 */
static PyObject* get_position(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for get_position.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("(ddd)", (double)state_vector->r.x, (double)state_vector->r.y, (double)state_vector->r.z);
}

/**
 * @brief Gets the velocity vector.
 */
static PyObject* get_velocity(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for get_velocity.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("(ddd)", (double)state_vector->v.x, (double)state_vector->v.y, (double)state_vector->v.z);
}

/**
 * @brief Gets the acceleration vector.
 */
static PyObject* get_acceleration(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for get_acceleration.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("(ddd)", (double)state_vector->a.x, (double)state_vector->a.y, (double)state_vector->a.z);
}

/**
 * @brief Gets the timestamp.
 */
static PyObject* get_time(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for get_time.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("d", (double)state_vector->time);
}

/**
 * @brief Gets the reference frame.
 */
static PyObject* get_frame(PyObject* self, PyObject* args) {

    PyObject* capsule;
    StateVector* state_vector;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments for get_frame.");
        return PyErr_Occurred();
    }

    state_vector = (StateVector*)PyCapsule_GetPointer(capsule, "StateVector");
    if(!state_vector) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the StateVector from capsule.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("i", state_vector->frame);
}


static PyMethodDef tolueneCoordinatesStateVectorMethods[] = {
    {"new_StateVector", new_StateVector, METH_VARARGS, "Creates a new state vector."},
    {"set_position", set_position, METH_VARARGS, "Sets the position vector of the given statevector."},
    {"set_velocity", set_velocity, METH_VARARGS, "Sets the velocity vector of the given statevector."},
    {"set_acceleration", set_acceleration, METH_VARARGS, "Sets the acceleration vector of the given statevector."},
    {"set_time", set_time, METH_VARARGS, "Sets the time of the given statevector."},
    {"set_frame", set_frame, METH_VARARGS, "Sets the frame vector of the given statevector."},
    {"get_position", get_position, METH_VARARGS, "Gets the position vector of the given statevector."},
    {"get_velocity", get_velocity, METH_VARARGS, "Gets the velocity vector of the given statevector."},
    {"get_acceleration", get_acceleration, METH_VARARGS, "Gets the acceleration vector of the given statevector."},
    {"get_time", get_time, METH_VARARGS, "Gets the time of the given statevector."},
    {"get_frame", get_frame, METH_VARARGS, "Gets the frame vector of the given statevector."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef coordinates_state_vector = {
    PyModuleDef_HEAD_INIT,
    "coordinates.state_vector",
    "C Extensions work with state vectors within the c interface",
    -1,
    tolueneCoordinatesStateVectorMethods
};


PyMODINIT_FUNC PyInit_state_vector(void) {
    return PyModule_Create(&coordinates_state_vector);
}


#ifdef __cplusplus
} /* extern "C" */
#endif