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

#define __compile_models_earth_earth_orientation_parameters__
#define __compile_time_delta_t__

#include "math/constants.h"
#include "models/earth/constants.h"
#include "models/earth/earth_orientation_parameters.h"
#include "models/earth/rotation.h"
#include "models/moon/constants.h"
#include "models/sun/constants.h"
#include "time/constants.h"
#include "time/delta_t.h"

#if defined(_WIN32) || defined(WIN32)

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @brief Calculate the Greenwich Mean Sidereal Time (GMST).
 *
 * @param[in] t seconds since J2000.0.
 */
void gmst(long double t, EarthModel* model, long double* gmst) {

    EOPTableRecord record;
    DeltaTTableRecord delta_t_record;
    eop_table_record_lookup(&model->earth_orientation_parameters, t, &record);
    delta_t_record_lookup(&model->delta_t_table, t, &delta_t_record);

    long double du = (t - J2000_UNIX_TIME + record.bulletin_a_dut1/1000.0) / SECONDS_PER_DAY;
    *gmst = ((((GMST_FUNCTION_JULIAN_DU[5] * du + GMST_FUNCTION_JULIAN_DU[4])* du + GMST_FUNCTION_JULIAN_DU[3]) * du +
        GMST_FUNCTION_JULIAN_DU[2]) * du + GMST_FUNCTION_JULIAN_DU[1]) * du + GMST_FUNCTION_JULIAN_DU[0];
    *gmst +=  GMST_DELTA_T * delta_t_record.deltaT/SECONDS_PER_DAY;

    *gmst = fmodl(*gmst, 86400.0);
}

/**
 * @brief Calculate the Earth rotation angle.
 *
 * @param[in] t Unix time
 * @param[in] model Earth model
 * @param[out] era the Earth rotation angle in rad.
 */
void earth_rotation_angle(long double t, EarthModel* model, long double* era) {

    EOPTableRecord record;
    eop_table_record_lookup(&model->earth_orientation_parameters, t, &record);

    t = (t - J2000_UNIX_TIME + record.bulletin_a_dut1/1000.0) / SECONDS_PER_DAY;
    *era = (ERA_DUT1[0] + ERA_DUT1[1] * t) * 2.0 * M_PI;
}

/**
 * @brief Calculate the equation of origins.
 *
 * @param[in] t Unix time
 * @param[out] eo the equation of origins in rad.
 */
void equation_of_origins(long double t, long double nutation_longitude, long double mean_obliquity_date,
    long double* eo) {

    t = (t-J2000_UNIX_TIME)/ SECONDS_PER_JULIAN_CENTURY;
    double nutation_critical_arguments[14];

    nutation_critical_arguments[0] = (((MEAN_ANOMALY_MOON[4] * t + MEAN_ANOMALY_MOON[3]) * t
        + MEAN_ANOMALY_MOON[2]) * t + MEAN_ANOMALY_MOON[1]) * t + MEAN_ANOMALY_MOON[0];
    nutation_critical_arguments[1] = (((MEAN_ANOMALY_SUN[4] * t + MEAN_ANOMALY_SUN[3]) * t
        + MEAN_ANOMALY_SUN[2]) * t + MEAN_ANOMALY_SUN[1]) * t + MEAN_ANOMALY_SUN[0];
    nutation_critical_arguments[2] = (((MEAN_ARGUMENT_LATITUDE_MOON[4] * t
        + MEAN_ARGUMENT_LATITUDE_MOON[3]) * t + MEAN_ARGUMENT_LATITUDE_MOON[2]) * t
        + MEAN_ARGUMENT_LATITUDE_MOON[1]) * t + MEAN_ARGUMENT_LATITUDE_MOON[0];
    nutation_critical_arguments[3] = (((MEAN_ELONGATION_MOON_FROM_SUN[4] * t
        + MEAN_ELONGATION_MOON_FROM_SUN[3]) * t + MEAN_ELONGATION_MOON_FROM_SUN[2]) * t
        + MEAN_ELONGATION_MOON_FROM_SUN[1]) * t + MEAN_ELONGATION_MOON_FROM_SUN[0];
    nutation_critical_arguments[4] = (((MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[4] * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[3]) * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[2]) * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[1]) * t
        + MEAN_LONGITUDE_MOON_MEAN_ASCENDING_NODE[0];
    nutation_critical_arguments[5] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MERCURY[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MERCURY[1] * t;
    nutation_critical_arguments[6] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_VENUS[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_VENUS[1] * t;
    nutation_critical_arguments[7] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_EARTH[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_EARTH[1] * t;
    nutation_critical_arguments[8] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MARS[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_MARS[1] * t;
    nutation_critical_arguments[9] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_JUPITER[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_JUPITER[1] * t;
    nutation_critical_arguments[10] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_SATURN[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_SATURN[1] * t;
    nutation_critical_arguments[11] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_URANUS[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_URANUS[1] * t;
    nutation_critical_arguments[12] = MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_NEPTUNE[0] +
        MEAN_HELIOCENTRIC_ECLIPTIC_LONGITUDE_NEPTUNE[1] * t;
    nutation_critical_arguments[13] = (GENERAL_PRECESSION_LONGITUDE[2] * t + GENERAL_PRECESSION_LONGITUDE[1]) * t;

    *eo = ((((-3.68e-8 * t - 0.000029956) * t + 4.4e-7) * t - 1.391581) * t - 4612.156534) * t - 0.014506 -
        nutation_longitude*cosl(mean_obliquity_date * ARCSECONDS_TO_RADIANS);

    int i = 0;
    for(i = 0; i < 33; ++i) {
        long double ai = EQUATION_OF_ORIGINS[i*16+2] * nutation_critical_arguments[0] +
            EQUATION_OF_ORIGINS[i*16+3] * nutation_critical_arguments[1] +
            EQUATION_OF_ORIGINS[i*16+4] * nutation_critical_arguments[2] +
            EQUATION_OF_ORIGINS[i*16+5] * nutation_critical_arguments[3] +
            EQUATION_OF_ORIGINS[i*16+6] * nutation_critical_arguments[4] +
            EQUATION_OF_ORIGINS[i*16+7] * nutation_critical_arguments[5] +
            EQUATION_OF_ORIGINS[i*16+8] * nutation_critical_arguments[6] +
            EQUATION_OF_ORIGINS[i*16+9] * nutation_critical_arguments[7] +
            EQUATION_OF_ORIGINS[i*16+10] * nutation_critical_arguments[8] +
            EQUATION_OF_ORIGINS[i*16+11] * nutation_critical_arguments[9] +
            EQUATION_OF_ORIGINS[i*16+12] * nutation_critical_arguments[10] +
            EQUATION_OF_ORIGINS[i*16+13] * nutation_critical_arguments[11] +
            EQUATION_OF_ORIGINS[i*16+14] * nutation_critical_arguments[12] +
            EQUATION_OF_ORIGINS[i*16+15] * nutation_critical_arguments[13];

           *eo -= (EQUATION_OF_ORIGINS[i*16] * sinl(ai) + EQUATION_OF_ORIGINS[i*16+1] * cosl(ai));
    }

    long double ai = EQUATION_OF_ORIGINS[i*16+2] * nutation_critical_arguments[0] +
            EQUATION_OF_ORIGINS[i*16+3] * nutation_critical_arguments[1] +
            EQUATION_OF_ORIGINS[i*16+4] * nutation_critical_arguments[2] +
            EQUATION_OF_ORIGINS[i*16+5] * nutation_critical_arguments[3] +
            EQUATION_OF_ORIGINS[i*16+6] * nutation_critical_arguments[4] +
            EQUATION_OF_ORIGINS[i*16+7] * nutation_critical_arguments[5] +
            EQUATION_OF_ORIGINS[i*16+8] * nutation_critical_arguments[6] +
            EQUATION_OF_ORIGINS[i*16+9] * nutation_critical_arguments[7] +
            EQUATION_OF_ORIGINS[i*16+10] * nutation_critical_arguments[8] +
            EQUATION_OF_ORIGINS[i*16+11] * nutation_critical_arguments[9] +
            EQUATION_OF_ORIGINS[i*16+12] * nutation_critical_arguments[10] +
            EQUATION_OF_ORIGINS[i*16+13] * nutation_critical_arguments[11] +
            EQUATION_OF_ORIGINS[i*16+14] * nutation_critical_arguments[12] +
            EQUATION_OF_ORIGINS[i*16+15] * nutation_critical_arguments[13];

    *eo -= (EQUATION_OF_ORIGINS[i*16] * sinl(ai * ARCSECONDS_TO_RADIANS) +
        EQUATION_OF_ORIGINS[i*16+1] * cosl(ai * ARCSECONDS_TO_RADIANS));

}

/**
 * @brief Calculate the Greenwich Apparent Sidereal Time (GAST).
 *
 * @param[in] t Unix time
 * @param[in] model Earth model
 * @param[out] gast the Greenwich Apparent Sidereal Time in rad.
 */
void gast_2000(long double t, EarthModel* model, long double nutation_longitude, long double mean_obliquity_date,
    long double* gast) {

    *gast = 0;
    earth_rotation_angle(t, model, gast);

    long double eo;
    equation_of_origins(t, nutation_longitude, mean_obliquity_date, &eo);
    *gast -= eo * ARCSECONDS_TO_RADIANS;

}

/**
 * @brief Calculate the Earth rotation matrix.
 *
 * @param[in] angle angle of rotation.
 * @param[out] matrix rotation matrix.
 */
void earth_rotation_matrix(long double angle, Mat3* matrix) {

    if (matrix) {

        matrix->w11 = cosl(angle);
        matrix->w12 = sinl(angle);
        matrix->w13 = 0.0;
        matrix->w21 = -sinl(angle);
        matrix->w22 = cosl(angle);
        matrix->w23 = 0.0;
        matrix->w31 = 0.0;
        matrix->w32 = 0.0;
        matrix->w33 = 1.0;

    }

}

/**
 * @brief Calculate the Earth rotation matrix.
 *
 * @param[in] t Unix time
 * @param[in] model Earth model
 * @param[out] rate the rotation rate in rad/s.
 */
void rate_of_earth_rotation(long double t, EarthModel* model, long double* rate) {

    EOPTableRecord record;
    eop_table_record_lookup(&model->earth_orientation_parameters, t, &record);

    *rate = 2.0 * M_PI / (SECONDS_PER_DAY + record.bulletin_a_lod/1000.0);

}


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */