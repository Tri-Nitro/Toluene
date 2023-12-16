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

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "opencl/context.h"

/**
 * @brief Creates a new opencl context
 */
static PyObject* new_opencl_context(PyObject* self, PyObject* args) {

    OpenCLContext* context = (OpenCLContext*)malloc(sizeof(OpenCLContext));

    if(!context) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_opencl_context.");
        return PyErr_Occurred();
    }

    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &context->platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(context->platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &context->device_id, &ret_num_devices);

    // Create an OpenCL context
    context->context = clCreateContext(NULL, 1, &context->device_id, NULL, NULL, &ret);

    // Create a command queue
    context->command_queue = clCreateCommandQueue(context->context, context->device_id, 0, &ret);


    return PyCapsule_New(context, "OpenCLContext", delete_opencl_context);
}

/**
 * @brief Deletes an opencl context
 */
static void delete_opencl_context(PyObject* obj) {

    OpenCLContext* context = (OpenCLContext*)PyCapsule_GetPointer(obj, "OpenCLContext");

    if(context) {
        clFlush(context->command_queue);
        clFinish(context->command_queue);

        clReleaseCommandQueue(context->command_queue);
        clReleaseContext(context->context);

        free(context);
    }
}

/**
 * @brief Gets the max compute units
 */
static PyObject* get_max_compute_units(PyObject* self, PyObject* args) {

    PyObject* capsule;
    OpenCLContext* context;

    if(!PyArg_ParseTuple(args, "O", &capsule)) {
        PyErr_SetString(PyExc_TypeError, "Invalid arguments passed to get_max_compute_units.");
        return PyErr_Occurred();
    }

    context = (OpenCLContext*)PyCapsule_GetPointer(capsule, "OpenCLContext");

    cl_uint max_compute_units;
    clGetDeviceInfo(context->device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_compute_units), &max_compute_units, NULL);

    return Py_BuildValue("i", max_compute_units);
}

/**
 * @brief Creates a new opencl program
 */
static PyMethodDef tolueneOpenCLContextMethods[] = {
    {"new_opencl_context", new_opencl_context, METH_VARARGS, "Creates a new OpenCL context."},
    {"get_max_compute_units", get_max_compute_units, METH_VARARGS, "Gets the max compute units."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef opencl_context = {
    PyModuleDef_HEAD_INIT,
    "opencl.context",
    "C Extensions to work with OpenCL accelerated code.",
    -1,
    tolueneOpenCLContextMethods
};


PyMODINIT_FUNC PyInit_context(void) {
    return PyModule_Create(&opencl_context);
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */