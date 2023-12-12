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
#ifndef __MODELS_EARTH_NUTATION_H__
#define __MODELS_EARTH_NUTATION_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @struct
 * @brief A record of the nutation series.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_mercury_coefficient
 * Member 'heliocentric_elliptical_longitude_mercury_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Mercury.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_venus_coefficient
 * Member 'heliocentric_elliptical_longitude_venus_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Venus.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_earth_coefficient
 * Member 'heliocentric_elliptical_longitude_earth_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Earth.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_mars_coefficient
 * Member 'heliocentric_elliptical_longitude_mars_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Mars.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_jupiter_coefficient
 * Member 'heliocentric_elliptical_longitude_jupiter_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Jupiter.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_saturn_coefficient
 * Member 'heliocentric_elliptical_longitude_saturn_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Saturn.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_uranus_coefficient
 * Member 'heliocentric_elliptical_longitude_uranus_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Uranus.
 * @var NutationSeriesRecord::heliocentric_elliptical_longitude_neptune_coefficient
 * Member 'heliocentric_elliptical_longitude_neptune_coefficient' is the coefficient of the heliocentric elliptical
 * longitude of Neptune.
 * @var NutationSeriesRecord::general_precession_in_longitude_coefficient
 * Member 'general_precession_in_longitude_coefficient' is the coefficient of the general precession in longitude.
 * @var NutationSeriesRecord::mean_anomaly_moon_coefficient
 * Member 'mean_anomaly_moon_coefficient' is the coefficient of the mean anomaly of the Moon.
 * @var NutationSeriesRecord::mean_anomaly_sun_coefficient
 * Member 'mean_anomaly_sun_coefficient' is the coefficient of the mean anomaly of the Sun.
 * @var NutationSeriesRecord::mean_argument_of_latitude_moon_coefficient
 * Member 'mean_argument_of_latitude_moon_coefficient' is the coefficient of the mean argument of latitude of the Moon.
 * @var NutationSeriesRecord::mean_elongation_moon_from_the_sun_coefficient
 * Member 'mean_elongation_moon_from_the_sun_coefficient' is the coefficient of the mean elongation of the Moon from the
 * Sun.
 * @var NutationSeriesRecord::mean_longitude_of_moon_mean_ascending_node_coefficient
 * Member 'mean_longitude_of_moon_mean_ascending_node_coefficient' is the coefficient of the mean longitude of the
 * Moon's mean ascending node.
 * @var NutationSeriesRecord::S
 * Member 'S' is the value of the S series.
 * @var NutationSeriesRecord::S_dot
 * Member 'S_dot' is the value of the S series derivative.
 * @var NutationSeriesRecord::C_prime
 * Member 'C_prime' is the value of the C' series.
 * @var NutationSeriesRecord::C
 * Member 'C' is the value of the C series.
 * @var NutationSeriesRecord::C_dot
 * Member 'C_dot' is the value of the C series derivative.
 * @var NutationSeriesRecord::S_prime
 * Member 'S_prime' is the value of the S' series.
 */
typedef struct {
    int heliocentric_elliptical_longitude_mercury_coefficient;
    int heliocentric_elliptical_longitude_venus_coefficient;
    int heliocentric_elliptical_longitude_earth_coefficient;
    int heliocentric_elliptical_longitude_mars_coefficient;
    int heliocentric_elliptical_longitude_jupiter_coefficient;
    int heliocentric_elliptical_longitude_saturn_coefficient;
    int heliocentric_elliptical_longitude_uranus_coefficient;
    int heliocentric_elliptical_longitude_neptune_coefficient;
    int general_precession_in_longitude_coefficient;
    int mean_anomaly_moon_coefficient;
    int mean_anomaly_sun_coefficient;
    int mean_argument_of_latitude_moon_coefficient;
    int mean_elongation_moon_from_the_sun_coefficient;
    int mean_longitude_of_moon_mean_ascending_node_coefficient;
    long double S, S_dot, C_prime, C, C_dot, S_prime;
} NutationSeriesRecord;

/** @struct
 * @brief A series of nutation records.
 * @var NutationSeries::nrecords
 * Member 'nrecords' is the number of records in the series.
 * @var NutationSeries::nrecords_allocated
 * Member 'nrecords_allocated' is the number of records allocated in the series.
 * @var NutationSeries::records
 * Member 'records' is the array of records.
 */
typedef struct {
    int nrecords;
    int nrecords_allocated;
    NutationSeriesRecord* records;
} NutationSeries;


#ifdef __compile_models_earth_nutation__

#include "math/linear_algebra.h"

/**
 * @brief Compute nutation values of date along with the equation of the equinoxes.
 */
void nutation_values_of_date(long double t, NutationSeries* series, long double* nutation_longitude,
    long double* nutation_obliquity, long double* mean_obliquity_date, long double* equation_of_the_equinoxes);

/**
 * @brief Compute the nutation series.
 */
void nutation_matrix(long double mean_obliquity_date, long double nutation_longitude, long double true_obliquity_date,
    Mat3* nutation_matrix);

/**
 * @brief Add a record to the nutation series.
 */
static PyObject* nutation_series_add_record(PyObject* self, PyObject* args);

/**
 * @brief Create a new nutation series object available in Python.
 */
static PyObject* new_NutationSeries(PyObject* self, PyObject* args);

/**
 * @brief Delete a nutation series object available in Python.
 */
static void delete_NutationSeries(PyObject* obj);


#endif /* __compile_models_earth_nutation */


#ifdef __cplusplus
}   /* extern "C" */
#endif /* __cplusplus */


#endif /* __MODELS_EARTH_NUTATION_H__ */