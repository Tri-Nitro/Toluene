#ifndef TOLUENE_BASE_H
#define TOLUENE_BASE_H

#define _USE_MATH_DEFINES
#include <math.h>


void ecef_from_lla(double semi_major_axis, double semi_minor_axis,
                   double latitude, double longitude, double altitude,
                   double* x, double* y, double* z);

double ellipsoid_radius(double latitude, double semi_major_axis, double inverse_flattening);

void lla_from_ecef(double semi_major_axis, double semi_minor_axis,
                   double x, double y, double z,
                   double *latitude, double *longitude, double *altitude);

#endif //TOLUENE_BASE_H
