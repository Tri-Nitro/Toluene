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
#ifndef __OPENCL_CONTEXT_H__
#define __OPENCL_CONTEXT_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

typedef struct {
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
} OpenCLContext;

/**
 * @brief Creates a new opencl context
 */
static PyObject* new_opencl_context(PyObject* self, PyObject* args);

/**
 * @brief Deletes an opencl context
 */
static void delete_opencl_context(PyObject* obj);

/**
 * @brief Gets the device name
 */
static PyObject* get_device_name(PyObject* self, PyObject* args);

/**
 * @brief Gets the device version
 */
static PyObject* get_device_version(PyObject* self, PyObject* args);

/**
 * @brief Gets the driver version
 */
static PyObject* get_driver_version(PyObject* self, PyObject* args);

/**
 * @brief Gets the max compute units
 */
static PyObject* get_max_compute_units(PyObject* self, PyObject* args);

#ifdef __cplusplus
}   /* extern "C" */
#endif

#endif /* __OPENCL_CONTEXT_H__ */