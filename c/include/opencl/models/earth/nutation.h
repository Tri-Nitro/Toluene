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
#ifndef __OPENCL_MODELS_EARTH_NUTATION_H__
#define __OPENCL_MODELS_EARTH_NUTATION_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef __compile_opencl_models_earth_nutation__

/**
 * @brief Compute nutation values of date along with the equation of the equinoxes.
 */
void nutation_values_of_date_opencl(long double t, NutationSeries* series, long double* nutation_longitude,
    long double* nutation_obliquity, long double* mean_obliquity_date, long double* equation_of_the_equinoxes);


#endif /* __compile_opencl_models_earth_nutation */


#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */


#endif /* __OPENCL_MODELS_EARTH_NUTATION_H__ */