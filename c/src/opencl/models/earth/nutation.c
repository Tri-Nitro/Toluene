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

#define __compile_opencl_models_earth_nutation__
#include "math/constants.h"
#include "models/earth/nutation.h"
#include "models/earth/constants.h"
#include "models/moon/constants.h"
#include "models/sun/constants.h"
#include "opencl/models/earth/nutation.h"
#include "time/constants.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Compute nutation values of date along with the equation of the equinoxes.
 */
void nutation_values_of_date_opencl(OpenCLKernel* kernel, long double t, NutationSeries* series,
        long double* nutation_longitude, long double* nutation_obliquity, long double* mean_obliquity_date,
        long double* equation_of_the_equinoxes) {
    *nutation_longitude = 0.0;
    *nutation_obliquity = 0.0;
    *mean_obliquity_date = 0.0;
    *equation_of_the_equinoxes = 0.0;

    t = (t-J2000_UNIX_TIME)/ SECONDS_PER_JULIAN_CENTURY;
    double nutation_critical_arguments[14];
    nutation_critical_arguments[0] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MERCURY[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MERCURY[1] * t;
    nutation_critical_arguments[1] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_VENUS[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_VENUS[1] * t;
    nutation_critical_arguments[2] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_EARTH[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_EARTH[1] * t;
    nutation_critical_arguments[3] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MARS[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MARS[1] * t;
    nutation_critical_arguments[4] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_JUPITER[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_JUPITER[1] * t;
    nutation_critical_arguments[5] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_SATURN[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_SATURN[1] * t;
    nutation_critical_arguments[6] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_URANUS[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_URANUS[1] * t;
    nutation_critical_arguments[7] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_NEPTUNE[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_NEPTUNE[1] * t;
    nutation_critical_arguments[8] = (GENERAL_PRECESSION_LONGITUDE[2] * t + GENERAL_PRECESSION_LONGITUDE[1]) * t;
    nutation_critical_arguments[9] = (((MEAN_ANOMALY_MOON[4] * t + MEAN_ANOMALY_MOON[3]) * t
        + MEAN_ANOMALY_MOON[2]) * t + MEAN_ANOMALY_MOON[1]) * t + MEAN_ANOMALY_MOON[0];
    nutation_critical_arguments[10] = (((MEAN_ANOMALY_SUN[4] * t + MEAN_ANOMALY_SUN[3]) * t
        + MEAN_ANOMALY_SUN[2]) * t + MEAN_ANOMALY_SUN[1]) * t + MEAN_ANOMALY_SUN[0];
    nutation_critical_arguments[11] = (((MEAN_ARGUMENT_LATITUDE_MOON[4] * t
        + MEAN_ARGUMENT_LATITUDE_MOON[3]) * t + MEAN_ARGUMENT_LATITUDE_MOON[2]) * t
        + MEAN_ARGUMENT_LATITUDE_MOON[1]) * t + MEAN_ARGUMENT_LATITUDE_MOON[0];
    nutation_critical_arguments[12] = (((MEAN_ELONGATION_MOON_FROM_SUN[4] * t
        + MEAN_ELONGATION_MOON_FROM_SUN[3]) * t + MEAN_ELONGATION_MOON_FROM_SUN[2]) * t
        + MEAN_ELONGATION_MOON_FROM_SUN[1]) * t + MEAN_ELONGATION_MOON_FROM_SUN[0];
    nutation_critical_arguments[13] = (((MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[4] * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[3]) * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[2]) * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[1]) * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[0];

    double* flattened_series = (double*)malloc(sizeof(double) * 20 * series->nrecords);
    double* nutation_values = (double*)malloc(sizeof(double) * 3);

    for (int i = 0; i < series->nrecords; i++) {
        flattened_series[i * 20] = series->records[i].heliocentric_elliptical_longitude_mercury_coefficient;
        flattened_series[i * 20 + 1] = series->records[i].heliocentric_elliptical_longitude_venus_coefficient;
        flattened_series[i * 20 + 2] = series->records[i].heliocentric_elliptical_longitude_earth_coefficient;
        flattened_series[i * 20 + 3] = series->records[i].heliocentric_elliptical_longitude_mars_coefficient;
        flattened_series[i * 20 + 4] = series->records[i].heliocentric_elliptical_longitude_jupiter_coefficient;
        flattened_series[i * 20 + 5] = series->records[i].heliocentric_elliptical_longitude_saturn_coefficient;
        flattened_series[i * 20 + 6] = series->records[i].heliocentric_elliptical_longitude_uranus_coefficient;
        flattened_series[i * 20 + 7] = series->records[i].heliocentric_elliptical_longitude_neptune_coefficient;
        flattened_series[i * 20 + 8] = series->records[i].general_precession_in_longitude_coefficient;
        flattened_series[i * 20 + 9] = series->records[i].mean_anomaly_moon_coefficient;
        flattened_series[i * 20 + 10] = series->records[i].mean_anomaly_sun_coefficient;
        flattened_series[i * 20 + 11] = series->records[i].mean_argument_of_latitude_moon_coefficient;
        flattened_series[i * 20 + 12] = series->records[i].mean_elongation_moon_from_the_sun_coefficient;
        flattened_series[i * 20 + 13] = series->records[i].mean_longitude_of_moon_mean_ascending_node_coefficient;
        flattened_series[i * 20 + 14] = series->records[i].S;
        flattened_series[i * 20 + 15] = series->records[i].S_dot;
        flattened_series[i * 20 + 16] = series->records[i].C_prime;
        flattened_series[i * 20 + 17] = series->records[i].C;
        flattened_series[i * 20 + 18] = series->records[i].C_dot;
        flattened_series[i * 20 + 19] = series->records[i].S_prime;
    }

    cl_int err;

    cl_mem nutation_critical_arguments_ocl, nutation_coefficients_ocl, time_ocl, nutation_values_ocl, size_ocl;
    nutation_critical_arguments_ocl = clCreateBuffer(kernel->context->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 14 * sizeof(double), nutation_critical_arguments, &err);
    nutation_coefficients_ocl = clCreateBuffer(kernel->context->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 20 * series->nrecords * sizeof(double), flattened_series, &err);
    time_ocl = clCreateBuffer(kernel->context->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double), &t, &err);
    nutation_values_ocl = clCreateBuffer(kernel->context->context, CL_MEM_WRITE_ONLY, 3 * series->nrecords * sizeof(double), NULL, &err);
    size_ocl = clCreateBuffer(kernel->context->context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), &series->nrecords, &err);

    clSetKernelArg(kernel->kernel, 0, sizeof(cl_mem), &nutation_critical_arguments_ocl);
    clSetKernelArg(kernel->kernel, 1, sizeof(cl_mem), &nutation_coefficients_ocl);
    clSetKernelArg(kernel->kernel, 2, sizeof(cl_mem), &time_ocl);
    clSetKernelArg(kernel->kernel, 3, sizeof(cl_mem), &nutation_values_ocl);
    clSetKernelArg(kernel->kernel, 4, sizeof(cl_mem), &size_ocl);

    size_t global_work_size = series->nrecords;
    err = clEnqueueNDRangeKernel(kernel->context->command_queue, kernel->kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
    clFinish(kernel->context->command_queue);
    err = clEnqueueReadBuffer(kernel->context->command_queue, nutation_values_ocl, CL_TRUE, 0, 3 * sizeof(double), nutation_values, 0, NULL, NULL);

    err = clFlush(kernel->context->command_queue);
    err = clFinish(kernel->context->command_queue);

    if(flattened_series) {
        free(flattened_series);
    }
    clReleaseMemObject(nutation_critical_arguments_ocl);
    clReleaseMemObject(nutation_coefficients_ocl);
    clReleaseMemObject(time_ocl);
    clReleaseMemObject(nutation_values_ocl);
    clReleaseMemObject(size_ocl);

    *nutation_longitude = nutation_values[0];
    *nutation_obliquity = nutation_values[1];

    *mean_obliquity_date = ((((MEAN_OBLIQUITY_EARTH[5] * t + MEAN_OBLIQUITY_EARTH[4]) * t
        + MEAN_OBLIQUITY_EARTH[3]) * t + MEAN_OBLIQUITY_EARTH[2]) * t + MEAN_OBLIQUITY_EARTH[1]) * t
        + MEAN_OBLIQUITY_EARTH[0];
    *equation_of_the_equinoxes = nutation_values[2] + *nutation_longitude * cosl(*nutation_obliquity * ARCSECONDS_TO_RADIANS) +
        0.00000087 * t * sinl(nutation_critical_arguments[13] * ARCSECONDS_TO_RADIANS);

    if (nutation_values) {
        free(nutation_values);
    }

}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */