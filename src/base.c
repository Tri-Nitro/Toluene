#include "base.h"

void ecef_from_lla(double semi_major_axis, double semi_minor_axis,
                   double latitude, double longitude, double altitude,
                   double* x, double* y, double* z) {

    double e_2 = 1 - ((semi_minor_axis*semi_minor_axis)/(semi_major_axis*semi_major_axis));
    double sin_of_latitude = sin((latitude * M_PI/180));
    double n_phi = semi_major_axis/(sqrt(1-(e_2 * (sin_of_latitude*sin_of_latitude))));

    *x = (n_phi + altitude) * cos(latitude * M_PI/180) * cos(longitude * M_PI/180);
    *y = (n_phi + altitude) * cos(latitude * M_PI/180) * sin(longitude * M_PI/180);
    *z = ((1 - e_2) * n_phi + altitude) * sin(latitude * M_PI/180);

}

double ellipsoid_radius(double latitude, double semi_major_axis, double inverse_flattening) {

    double e = 1-1/inverse_flattening;
    double sin_latitude = sin(latitude);

    return sqrt((semi_major_axis*semi_major_axis)/(1+(1/(e*e)-1)*sin_latitude*sin_latitude));
}

void lla_from_ecef(double semi_major_axis, double semi_minor_axis, double x, double y, double z,
                   double *latitude, double *longitude, double *altitude) {

    // Causes a divide by 0 bug because of p if x and y are 0. Just put a little offset so longitude can be set if
    // directly above the pole.
    if(x == 0 && y == 0) x = 0.000000001;

    double e_numerator = semi_major_axis*semi_major_axis - semi_minor_axis*semi_minor_axis;
    double e_2 = e_numerator/(semi_major_axis*semi_major_axis);
    double e_r2 = e_numerator/(semi_minor_axis*semi_minor_axis);
    double p = sqrt(x*x+y*y);
    double big_f = 54.0*semi_minor_axis*semi_minor_axis*z*z;
    double big_g = p*p+z*z*(1-e_2)-e_2*e_numerator;
    double c = (e_2*e_2*big_f*p*p)/(big_g*big_g*big_g);
    double s = cbrt(1+c+sqrt(c*c+2*c));
    double k = s+1+1/s;
    double big_p = big_f/(3*k*k*big_g*big_g);
    double big_q = sqrt(1 + 2 * e_2 * e_2 * big_p);
    double r_0 = ((-1* big_p*e_2*p)/(1+big_q)) +
                 sqrt((semi_major_axis*semi_major_axis/2)*(1+1/big_q)-((big_p*(1-e_2)*z*z)/(big_q*(1+big_q)))
                 -(big_p*p*p)/2);
    double p_e_2_r_0 = p-e_2*r_0;
    double big_u = sqrt( p_e_2_r_0*p_e_2_r_0+z*z);
    double big_v = sqrt(p_e_2_r_0*p_e_2_r_0+(1-e_2)*z*z);
    double z_0 = (semi_minor_axis*semi_minor_axis*z)/(semi_major_axis*big_v);

    *latitude = atan((z+(e_r2*z_0))/p) * 180/M_PI;
    *longitude = atan2(y,x) * 180/M_PI;
    *altitude = big_u * (1-(semi_minor_axis*semi_minor_axis)/(semi_major_axis*big_v));

}