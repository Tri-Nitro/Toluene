#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "earth_gravitational_model.h"
#include "interpolation.h"
#include "toluene_core_config.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif

#ifdef __cplusplus
extern "C"
{
#endif


static double compute_polynomial(const double coefficients[], double x, int highest_order) {

    double result = 0.0;

    for(int i = highest_order; i >= 0; --i) {
        result = result * x + coefficients[i];
    }

    return result;
}


static void compute_iau_coefficients(double tt_seconds, double iau_coefficients[]) {

    // 3155760000.0 is the number of seconds in a century. t is the number of centuries since J2000.0.
    double t = tt_seconds / 3155760000.0;

    // zeta_a(t) = zeta_0 + zeta_1 * t + zeta_2 * t^2 + zeta_3 * t^3 + zeta_4 * t^4 + zeta_5 * t^5.
    iau_coefficients[0] = compute_polynomial(ZETA_a, t, 5);

    // z_a(t) = z_0 + z_1 * t + z_2 * t^2 + z_3 * t^3 + z_4 * t^4 + z_5 * t^5.
    iau_coefficients[1] = compute_polynomial(Z_a, t, 5);

    // theta_a(t) = theta_0 + theta_1 * t + theta_2 * t^2 + theta_3 * t^3 + theta_4 * t^4 + theta_5 * t^5.
    iau_coefficients[2] = compute_polynomial(THETA_a, t, 5);

    return;
}


static void compute_precession_matrix(double iau_coefficients[], double precession_matrix[]) {

    double sin_zeta_a = sin(iau_coefficients[0] * M_PI/648000);
    double cos_zeta_a = cos(iau_coefficients[0] * M_PI/648000);
    double sin_z_a = sin(iau_coefficients[1] * M_PI/648000);
    double cos_z_a = cos(iau_coefficients[1] * M_PI/648000);
    double sin_theta_a = sin(iau_coefficients[2] * M_PI/648000);
    double cos_theta_a = cos(iau_coefficients[2] * M_PI/648000);

    /* Precession matrix for converting from ECI to ECEF.
     * [cos(z_a)cos(theta_a)cos(zeta_a)-sin(z_a)sin(zeta_a), -cos(z_a)cos(theta_a)sin(zeta_a)-sin(z_a)cos(zeta_a), -cos(z_a)sin(theta_a)]
     * [sin(z_a)cos(theta_a)cos(zeta_a)+cos(z_a)sin(zeta_a), -sin(z_a)cos(theta_a)sin(zeta_a)+cos(z_a)cos(zeta_a), -sin(z_a)sin(theta_a)]
     * [sin(theta_a)cos(zeta_a), -sin(theta_a)sin(zeta_a), cos(theta_a)]
     * Look in Explanatory Supplement to the Astronomical Almanac 3rd Edition pg 217 for more information.
     */

    precession_matrix[0] = cos_z_a * cos_theta_a * cos_zeta_a -  sin_z_a * sin_zeta_a;
    precession_matrix[1] = cos_z_a * cos_theta_a * sin_zeta_a + sin_z_a * cos_zeta_a;
    precession_matrix[2] = cos_z_a * sin_theta_a;
    precession_matrix[3] = -1 * sin_z_a * cos_theta_a * cos_zeta_a - cos_z_a * sin_zeta_a;
    precession_matrix[4] = -1 * sin_z_a * cos_theta_a * sin_zeta_a + cos_z_a * cos_zeta_a;
    precession_matrix[5] = -1 * sin_z_a * sin_theta_a;
    precession_matrix[6] = -1 * sin_theta_a * cos_zeta_a;
    precession_matrix[7] = -1 * sin_theta_a * sin_zeta_a;
    precession_matrix[8] = cos_theta_a;

    return;
}


static void compute_nutation_arguments(double tt_seconds, double nutation_arguments[]) {

    // 3155760000.0 is the number of seconds in a century. t is the number of centuries since J2000.0.
    double t = tt_seconds / 3155760000.0;
    double nutation_critical_arguments[14];

    nutation_critical_arguments[0] = l_me[0] + l_me[1]*t;
    nutation_critical_arguments[1] = l_v[0] + l_v[1]*t;
    nutation_critical_arguments[2] = l_e[0] + l_e[1]*t;
    nutation_critical_arguments[3] = l_ma[0] + l_ma[1]*t;
    nutation_critical_arguments[4] = l_j[0] + l_j[1]*t;
    nutation_critical_arguments[5] = l_s[0] + l_s[1]*t;
    nutation_critical_arguments[6] = l_u[0] + l_u[1]*t;
    nutation_critical_arguments[7] = l_n[0] + l_n[1]*t;
    nutation_critical_arguments[8] = compute_polynomial(gen_p, t, 5);
    nutation_critical_arguments[9] = compute_polynomial(l_, t, 4);
    nutation_critical_arguments[10] = compute_polynomial(l_prime, t, 4);
    nutation_critical_arguments[11] = compute_polynomial(f_, t, 4);
    nutation_critical_arguments[12] = compute_polynomial(d_, t, 4);
    nutation_critical_arguments[13] = compute_polynomial(omega, t, 4);

    double ai;
    double sin_ai;
    double cos_ai;

    // delta_psi
    nutation_arguments[0] = 0.0;
    // delta_epsilon
    nutation_arguments[1] = 0.0;
    // Equation_of_the_equinoxes
    nutation_arguments[3] = 0.0;

    for(size_t i = 0; i < nutation_series_rows*nutation_series_columns; i+=nutation_series_columns) {
        ai = nutation_series[i+1] * nutation_critical_arguments[0] + nutation_series[i+2] * nutation_critical_arguments[1] +
             nutation_series[i+3] * nutation_critical_arguments[2] + nutation_series[i+4] * nutation_critical_arguments[3] +
             nutation_series[i+5] * nutation_critical_arguments[4] + nutation_series[i+6] * nutation_critical_arguments[5] +
             nutation_series[i+7] * nutation_critical_arguments[6] + nutation_series[i+8] * nutation_critical_arguments[7] +
             nutation_series[i+9] * nutation_critical_arguments[8] + nutation_series[i+10] * nutation_critical_arguments[9] +
             nutation_series[i+11] * nutation_critical_arguments[10] + nutation_series[i+12] * nutation_critical_arguments[11] +
             nutation_series[i+13] * nutation_critical_arguments[12] + nutation_series[i+14] * nutation_critical_arguments[13];

        sin_ai = sin(ai * M_PI/180);
        cos_ai = cos(ai * M_PI/180);

        nutation_arguments[0] += (nutation_series[i+15] + nutation_series[i+16] * t) * sin_ai + nutation_series[i+18] * cos_ai;
        nutation_arguments[1] += (nutation_series[i+18] + nutation_series[i+19] * t) * cos_ai + nutation_series[i+15] * sin_ai;
        nutation_arguments[3] += nutation_series[i+17] * sin_ai + nutation_series[i+20] * cos_ai;
    }

    nutation_arguments[2] = compute_polynomial(EPSILON_a, t, 5);
    nutation_arguments[1] += nutation_arguments[2];

    nutation_arguments[3] += nutation_arguments[0] * cos(nutation_arguments[2] * M_PI/648000) +
                             0.00000087 * t * sin(nutation_critical_arguments[13] * M_PI/648000);

    return;
}


static void compute_nutation_matrix(double nutation_arguments[], double nutation_matrix[]) {

    double sin_delta_psi = sin(nutation_arguments[0] * M_PI/648000);
    double cos_delta_psi = cos(nutation_arguments[0] * M_PI/648000);
    double sin_epsilon = sin(nutation_arguments[1] * M_PI/648000);
    double cos_epsilon = cos(nutation_arguments[1] * M_PI/648000);
    double sin_epsilon_a = sin(nutation_arguments[2] * M_PI/648000);
    double cos_epsilon_a = cos(nutation_arguments[2] * M_PI/648000);

    /* Nutation matrix for converting from ECI to ECEF.
     * [cos(delta_psi), -sin(delta_psi)cos(epsilon), -sin(delta_psi)sin(epsilon)]
     * [sin(delta_psi)cos(epsilon)cos(epsilon_a)+cos(delta_psi)sin(epsilon_a), cos(delta_psi)cos(epsilon)cos(epsilon_a)-sin(delta_psi)sin(epsilon_a), -sin(epsilon)cos(epsilon_a)]
     * [sin(delta_psi)cos(epsilon)sin(epsilon_a)-cos(delta_psi)cos(epsilon_a), cos(delta_psi)cos(epsilon)sin(epsilon_a)+sin(delta_psi)cos(epsilon_a), cos(epsilon)cos(epsilon_a)]
     * Look in Explanatory Supplement to the Astronomical Almanac 3rd Edition pg 225 for more information.
     */

    nutation_matrix[0] = cos_delta_psi;
    nutation_matrix[1] = sin_delta_psi * cos_epsilon_a;
    nutation_matrix[2] = -1 * sin_delta_psi * sin_epsilon_a;
    nutation_matrix[3] = -1 * sin_delta_psi * cos_epsilon;
    nutation_matrix[4] = cos_delta_psi * cos_epsilon * cos_epsilon_a + sin_epsilon * sin_epsilon_a;
    nutation_matrix[5] = -1 * cos_delta_psi * cos_epsilon * sin_epsilon_a + sin_epsilon * cos_epsilon_a;
    nutation_matrix[6] = sin_delta_psi * sin_epsilon;
    nutation_matrix[7] = -1 * cos_delta_psi * sin_epsilon * cos_epsilon_a + cos_epsilon * sin_epsilon_a;
    nutation_matrix[8] = cos_delta_psi * sin_epsilon * sin_epsilon_a + cos_epsilon * cos_epsilon_a;

    return;
}


static double lookup_closest_delta_t(double tt_seconds) {

    double best_delta_t = delta_t_list[(delta_t_length-1)*2+1];

    /* Because you're more likely to use coordinates closer to current day than J2000.0 start with end of list
     * Technically doesn't matter if more random but can drastically decrease looking through the list if this
     * assumption proves right.
    */
    for(int i = delta_t_length-1; i >= 0; --i) {
        if(tt_seconds >= delta_t_list[i*2]) {
           return best_delta_t;
        }
        best_delta_t = delta_t_list[i*2+1];
    }

    return best_delta_t;
}


static void compute_terrestrial_matrix(double tt_seconds, double equation_of_the_equinoxes, double terrestrial_matrix[]) {

    double delta_t = lookup_closest_delta_t(tt_seconds);
    double Dut = (tt_seconds-delta_t)/86400.0;
    double delta_t_days = delta_t/86400.0;

    double gast = fmod(compute_polynomial(gmst_coefficients, Dut, 5)
                    + gmst_coefficients[6] * delta_t_days + equation_of_the_equinoxes/15.0, 86400.0);

    double gast_angle = gast * M_PI/43200.0;

    terrestrial_matrix[0] = cos(gast_angle);
    terrestrial_matrix[1] = -1 * sin(gast_angle);
    terrestrial_matrix[2] = 0.0;
    terrestrial_matrix[3] = sin(gast_angle);
    terrestrial_matrix[4] = cos(gast_angle);
    terrestrial_matrix[5] = 0.0;
    terrestrial_matrix[6] = 0.0;
    terrestrial_matrix[7] = 0.0;
    terrestrial_matrix[8] = 1.0;

    return;
}


static void compute_polar_motion_matrix(double tt_seconds, double polar_motion_matrix[]) {

    double delta_t = lookup_closest_delta_t(tt_seconds);
    tt_seconds = tt_seconds-delta_t;

    double best_x = polar_motion_list[(polar_motion_length-1)*3+1];
    double best_y = polar_motion_list[(polar_motion_length-1)*3+2];

    /* Because you're more likely to use coordinates closer to current day than J2000.0 start with end of list
     * Technically doesn't matter if more random but can drastically decrease looking through the list if this
     * assumption proves right.
    */
    for(int i = polar_motion_length-1; i >= 0; --i) {
        if(tt_seconds >= polar_motion_list[i*3]) {
           break;
        }
        best_x = polar_motion_list[i*3+1];
        best_y = polar_motion_list[i*3+2];
    }

    double s_prime = tio_locator_per_century * (tt_seconds)/3155760000.0;

    double sin_x = sin(best_x * M_PI/648000);
    double cos_x = cos(best_x * M_PI/648000);
    double sin_y = sin(best_y * M_PI/648000);
    double cos_y = cos(best_y * M_PI/648000);
    double sin_s_prime = sin(s_prime * M_PI/648000);
    double cos_s_prime = cos(s_prime * M_PI/648000);

    polar_motion_matrix[0] = cos_x;
    polar_motion_matrix[1] = -1 * sin_x * sin_y;
    polar_motion_matrix[2] = sin_x * cos_y;
    polar_motion_matrix[3] = -1 * sin_x * sin_s_prime;
    polar_motion_matrix[4] = cos_x * cos_s_prime - cos_x * sin_y * sin_s_prime;
    polar_motion_matrix[5] = sin_y * cos_s_prime + sin_s_prime * cos_y * cos_x;
    polar_motion_matrix[6] = -1 * sin_x * cos_s_prime;
    polar_motion_matrix[7] = -1 * cos_y * sin_s_prime - sin_y * cos_x * cos_s_prime;
    polar_motion_matrix[8] = cos_y * cos_x * cos_s_prime - sin_s_prime * sin_y;

    return;
}


static PyObject *
ecef_from_lla(PyObject *self, PyObject *args) {

    double semi_major_axis, semi_minor_axis, latitude, longitude, altitude;

    if(!PyArg_ParseTuple(args, "ddddd", &semi_major_axis, &semi_minor_axis, &latitude, &longitude, &altitude)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_from_lla(double semi_major_axis, double semi_minor_axis, double latitude, double longitude, double altitude)");
        return PyErr_Occurred();
    }

    double e_2 = 1 - ((semi_minor_axis*semi_minor_axis)/(semi_major_axis*semi_major_axis));
    double sin_of_latitude = sin((latitude * M_PI/180));
    double n_phi = semi_major_axis/(sqrt(1-(e_2 * (sin_of_latitude*sin_of_latitude))));

    double x = (n_phi + altitude) * cos(latitude * M_PI/180) * cos(longitude * M_PI/180);
    double y = (n_phi + altitude) * cos(latitude * M_PI/180) * sin(longitude * M_PI/180);
    double z = ((1 - e_2) * n_phi + altitude) * sin(latitude * M_PI/180);


    return Py_BuildValue("(ddd)", x, y, z);
}


static PyObject *
lla_from_ecef(PyObject *self, PyObject *args) {

    double semi_major_axis, semi_minor_axis;
    double x, y, z;

    if(!PyArg_ParseTuple(args, "ddddd", &semi_major_axis, &semi_minor_axis, &x, &y, &z)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. lla_from_ecef(double semi_major_axis, double semi_minor_axis, double x, double y, double z)");
        return PyErr_Occurred();
    }

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
    double sqrt_r_0 = (semi_major_axis*semi_major_axis/2)*(1+1/big_q)-((big_p*(1-e_2)*z*z)/(big_q*(1+big_q)))
                 -(big_p*p*p)/2;
    sqrt_r_0 = (sqrt_r_0 < 0? 0 : sqrt(sqrt_r_0));
    double r_0 = ((-1* big_p*e_2*p)/(1+big_q)) + sqrt_r_0;
    double p_e_2_r_0 = p-e_2*r_0;
    double big_u = sqrt( p_e_2_r_0*p_e_2_r_0+z*z);
    double big_v = sqrt(p_e_2_r_0*p_e_2_r_0+(1-e_2)*z*z);
    double z_0 = (semi_minor_axis*semi_minor_axis*z)/(semi_major_axis*big_v);

    double latitude = atan((z+(e_r2*z_0))/p) * 180/M_PI;
    double longitude = atan2(y,x) * 180/M_PI;
    double altitude = big_u * (1-(semi_minor_axis*semi_minor_axis)/(semi_major_axis*big_v));

    return Py_BuildValue("(ddd)", latitude, longitude, altitude);
}


static PyObject *
eci_from_ecef(PyObject *self, PyObject *args) {

    double x, y, z, tt_seconds;

    double precession_matrix[9];
    double iau_coefficients[3];

    double nutation_matrix[9];
    double nutation_arguments[4];

    double terrestrial_matrix[9];

    double polar_motion_matrix[9];

    if(!PyArg_ParseTuple(args, "dddd", &x, &y, &z, &tt_seconds)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_from_eci(double x, double y, double z, double tt_seconds)");
        return PyErr_Occurred();
    }

    compute_iau_coefficients(tt_seconds, iau_coefficients);
    compute_precession_matrix(iau_coefficients, precession_matrix);

    compute_nutation_arguments(tt_seconds, nutation_arguments);
    compute_nutation_matrix(nutation_arguments, nutation_matrix);

    compute_terrestrial_matrix(tt_seconds, nutation_arguments[3], terrestrial_matrix);

    compute_polar_motion_matrix(tt_seconds, polar_motion_matrix);

    /* Terrestrial rotation of earth from GAST */
    double x_prime = x*polar_motion_matrix[0] + y*polar_motion_matrix[1] + z*polar_motion_matrix[2];
    double y_prime = x*polar_motion_matrix[3] + y*polar_motion_matrix[4] + z*polar_motion_matrix[5];
    double z_prime = x*polar_motion_matrix[6] + y*polar_motion_matrix[7] + z*polar_motion_matrix[8];

    x = x_prime*terrestrial_matrix[0] + y_prime*terrestrial_matrix[1] + z_prime*terrestrial_matrix[2];
    y = x_prime*terrestrial_matrix[3] + y_prime*terrestrial_matrix[4] + z_prime*terrestrial_matrix[5];
    z = x_prime*terrestrial_matrix[6] + y_prime*terrestrial_matrix[7] + z_prime*terrestrial_matrix[8];

    /* Nutation rotation of earth from J2000.0 */
    x_prime = x*nutation_matrix[0] + y*nutation_matrix[1] + z*nutation_matrix[2];
    y_prime = x*nutation_matrix[3] + y*nutation_matrix[4] + z*nutation_matrix[5];
    z_prime = x*nutation_matrix[6] + y*nutation_matrix[7] + z*nutation_matrix[8];

    /* Precession rotation of earth from J2000.0 */
    x = x_prime*precession_matrix[0] + y_prime*precession_matrix[1] + z_prime*precession_matrix[2];
    y = x_prime*precession_matrix[3] + y_prime*precession_matrix[4] + z_prime*precession_matrix[5];
    z = x_prime*precession_matrix[6] + y_prime*precession_matrix[7] + z_prime*precession_matrix[8];

    /* Bias rotation of earth from J2000.0 */
    x_prime = x*bias_rotation_matrix[0] + y*bias_rotation_matrix[1] + z*bias_rotation_matrix[2];
    y_prime = x*bias_rotation_matrix[3] + y*bias_rotation_matrix[4] + z*bias_rotation_matrix[5];
    z_prime = x*bias_rotation_matrix[6] + y*bias_rotation_matrix[7] + z*bias_rotation_matrix[8];

    x = x_prime;
    y = y_prime;
    z = z_prime;

    return Py_BuildValue("(ddd)", x, y, z);
}


static PyObject *
ecef_from_eci(PyObject *self, PyObject *args) {

    double x, y, z, tt_seconds;

    double precession_matrix[9];
    double iau_coefficients[3];

    double nutation_matrix[9];
    double nutation_arguments[4];

    double terrestrial_matrix[9];

    double polar_motion_matrix[9];

    if(!PyArg_ParseTuple(args, "dddd", &x, &y, &z, &tt_seconds)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_from_eci(double x, double y, double z, double tt_seconds)");
        return PyErr_Occurred();
    }

    compute_iau_coefficients(tt_seconds, iau_coefficients);
    compute_precession_matrix(iau_coefficients, precession_matrix);

    compute_nutation_arguments(tt_seconds, nutation_arguments);
    compute_nutation_matrix(nutation_arguments, nutation_matrix);

    compute_terrestrial_matrix(tt_seconds, nutation_arguments[3], terrestrial_matrix);

    compute_polar_motion_matrix(tt_seconds, polar_motion_matrix);

    double x_prime = x*bias_rotation_matrix[0] + y*bias_rotation_matrix[3] + z*bias_rotation_matrix[6];
    double y_prime = x*bias_rotation_matrix[1] + y*bias_rotation_matrix[4] + z*bias_rotation_matrix[7];
    double z_prime = x*bias_rotation_matrix[2] + y*bias_rotation_matrix[5] + z*bias_rotation_matrix[8];

    x = x_prime*precession_matrix[0] + y_prime*precession_matrix[3] + z_prime*precession_matrix[6];
    y = x_prime*precession_matrix[1] + y_prime*precession_matrix[4] + z_prime*precession_matrix[7];
    z = x_prime*precession_matrix[2] + y_prime*precession_matrix[5] + z_prime*precession_matrix[8];

    x_prime = x*nutation_matrix[0] + y*nutation_matrix[3] + z*nutation_matrix[6];
    y_prime = x*nutation_matrix[1] + y*nutation_matrix[4] + z*nutation_matrix[7];
    z_prime = x*nutation_matrix[2] + y*nutation_matrix[5] + z*nutation_matrix[8];

    x = x_prime*terrestrial_matrix[0] + y_prime*terrestrial_matrix[3] + z_prime*terrestrial_matrix[6];
    y = x_prime*terrestrial_matrix[1] + y_prime*terrestrial_matrix[4] + z_prime*terrestrial_matrix[7];
    z = x_prime*terrestrial_matrix[2] + y_prime*terrestrial_matrix[5] + z_prime*terrestrial_matrix[8];

    x_prime = x*polar_motion_matrix[0] + y*polar_motion_matrix[3] + z*polar_motion_matrix[6];
    y_prime = x*polar_motion_matrix[1] + y*polar_motion_matrix[4] + z*polar_motion_matrix[7];
    z_prime = x*polar_motion_matrix[2] + y*polar_motion_matrix[5] + z*polar_motion_matrix[8];

    x = x_prime;
    y = y_prime;
    z = z_prime;

    return Py_BuildValue("(ddd)", x, y, z);
}


static PyObject *
egm84_height(PyObject *self, PyObject *args) {

    double latitude, longitude, grid_spacing, h = 0;
    char *egm84_interpolation_grid_file_path, *interpolation_method;

    if(!PyArg_ParseTuple(args, "ddsds", &latitude, &longitude,
        &egm84_interpolation_grid_file_path, &grid_spacing, &interpolation_method)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. egm84_height(double latitude, double longitude, char* egm84_interpolation_grid_file_path, double grid_spacing, char* interpolation_method)");
        return PyErr_Occurred();
    }

    if (strcmp(interpolation_method, "bilinear") == 0) {
        double four_corners[12]; // latitude, longitude, height X4
        find_egm84_four_corners(latitude, longitude, egm84_interpolation_grid_file_path, grid_spacing, four_corners);
        bilinear_interpolation(latitude, longitude, four_corners, &h);
    } else {
        PyErr_SetString(PyExc_TypeError, "Invalid interpolation method. Valid methods are 'bilinear'.");
        return PyErr_Occurred();
    }

    return Py_BuildValue("d", h);
}


static PyObject *
ellipsoid_radius(PyObject *self, PyObject *args) {

    double semi_major_axis, semi_minor_axis, latitude;

    if(!PyArg_ParseTuple(args, "ddd", &semi_major_axis, &semi_minor_axis, &latitude)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ellipsoid_radius(double semi_major_axis, double semi_minor_axis, double latitude)");
        return PyErr_Occurred();
    }

    double f3 = semi_major_axis * cos(latitude * M_PI/180);
    double f4 = semi_minor_axis * sin(latitude * M_PI/180);
    double f1 = semi_major_axis * f3;
    f1 = f1 * f1;
    f3 = f3 * f3;
    double f2 = semi_minor_axis * f4;
    f2 = f2 * f2;
    f4 = f4 * f4;

    return Py_BuildValue("d", sqrt((f1 + f2) / (f3 + f4)));
}


static PyMethodDef tolueneCoreMethods[] = {
    {"ecef_from_lla", ecef_from_lla, METH_VARARGS, "Convert lla coordinates to ecef."},
    {"lla_from_ecef", lla_from_ecef, METH_VARARGS, "Convert ecef coordinates to lla using the none recursive method."},
    {"eci_from_ecef", eci_from_ecef, METH_VARARGS, "Convert ecef coordinates to eci."},
    {"ecef_from_eci", ecef_from_eci, METH_VARARGS, "Convert eci coordinates to ecef."},
    {"egm84_height", egm84_height, METH_VARARGS, "Calculate the height of a point on the earth using the EGM84 geoid model."},
    {"ellipsoid_radius", ellipsoid_radius, METH_VARARGS, "Calculate the radius of the ellipsoid at a given latitude."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef core_extensions = {
    PyModuleDef_HEAD_INIT,
    "core_extensions",
    "C Extensions to toluene core class functions",
    -1,
    tolueneCoreMethods
};

PyMODINIT_FUNC PyInit_core_extensions(void) {
    return PyModule_Create(&core_extensions);
}


#ifdef __cplusplus
}
#endif