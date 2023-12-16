void nutation_values_of_date(long double t, NutationSeries* series, long double* nutation_longitude,
    long double* nutation_obliquity, long double* mean_obliquity_date, long double* equation_of_the_equinoxes) {

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


    long double ai, sin_ai, cos_ai;
    for(int i = 0; i < series->nrecords; ++i) {
        ai = series->records[i].heliocentric_elliptical_longitude_mercury_coefficient
                * nutation_critical_arguments[0] +
             series->records[i].heliocentric_elliptical_longitude_venus_coefficient
                * nutation_critical_arguments[1] +
             series->records[i].heliocentric_elliptical_longitude_earth_coefficient
                * nutation_critical_arguments[2] +
             series->records[i].heliocentric_elliptical_longitude_mars_coefficient
                * nutation_critical_arguments[3] +
             series->records[i].heliocentric_elliptical_longitude_jupiter_coefficient
                * nutation_critical_arguments[4] +
             series->records[i].heliocentric_elliptical_longitude_saturn_coefficient
                * nutation_critical_arguments[5] +
             series->records[i].heliocentric_elliptical_longitude_uranus_coefficient
                * nutation_critical_arguments[6] +
             series->records[i].heliocentric_elliptical_longitude_neptune_coefficient
                * nutation_critical_arguments[7] +
             series->records[i].general_precession_in_longitude_coefficient
                * nutation_critical_arguments[8] +
             series->records[i].mean_anomaly_moon_coefficient
                * nutation_critical_arguments[9] +
             series->records[i].mean_anomaly_sun_coefficient
                * nutation_critical_arguments[10] +
             series->records[i].mean_argument_of_latitude_moon_coefficient
                * nutation_critical_arguments[11] +
             series->records[i].mean_elongation_moon_from_the_sun_coefficient
                * nutation_critical_arguments[12] +
             series->records[i].mean_longitude_of_moon_mean_ascending_node_coefficient
                * nutation_critical_arguments[13];

        sin_ai = sinl(ai * ARCSECONDS_TO_RADIANS);
        cos_ai = cosl(ai * ARCSECONDS_TO_RADIANS);

        *nutation_longitude += (series->records[i].S + series->records[i].S_dot * t) * sin_ai +
            series->records[i].C_prime * cos_ai;
        *nutation_obliquity += (series->records[i].C + series->records[i].C_dot * t) * cos_ai +
            series->records[i].S_prime * sin_ai;
        *equation_of_the_equinoxes += series->records[i].C_prime * sin_ai +
            series->records[i].S_prime * cos_ai;
    }

    *mean_obliquity_date = ((((MEAN_OBLIQUITY_EARTH[5] * t + MEAN_OBLIQUITY_EARTH[4]) * t
        + MEAN_OBLIQUITY_EARTH[3]) * t + MEAN_OBLIQUITY_EARTH[2]) * t + MEAN_OBLIQUITY_EARTH[1]) * t
        + MEAN_OBLIQUITY_EARTH[0];
    *equation_of_the_equinoxes += *nutation_longitude * cosl(*nutation_obliquity * ARCSECONDS_TO_RADIANS) +
        0.00000087 * t * sinl(nutation_critical_arguments[13] * ARCSECONDS_TO_RADIANS);

}