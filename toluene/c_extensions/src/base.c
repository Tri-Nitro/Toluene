#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "nutation_series.h"
#include "iau_coefficients.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif

#ifdef __cplusplus
extern "C"
{
#endif

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


static void compute_iau_coefficients(double tt_seconds, double iau_coefficients[]) {

    // 3155760000.0 is the number of seconds in a century. t is the number of centuries since J2000.0.
    double t = tt_seconds / 3155760000.0;

    // zeta_a(t) = zeta_0 + zeta_1 * t + zeta_2 * t^2 + zeta_3 * t^3 + zeta_4 * t^4 + zeta_5 * t^5.
    iau_coefficients[0] = (((((ZETA_a[5]*t)+ZETA_a[4])*t+ZETA_a[3])*t+ZETA_a[2])*t+ZETA_a[1])*t+ZETA_a[0];

    // z_a(t) = z_0 + z_1 * t + z_2 * t^2 + z_3 * t^3 + z_4 * t^4 + z_5 * t^5.
    iau_coefficients[1] = (((((Z_a[5]*t)+Z_a[4])*t+Z_a[3])*t+Z_a[2])*t+Z_a[1])*t+Z_a[0];

    // theta_a(t) = theta_0 + theta_1 * t + theta_2 * t^2 + theta_3 * t^3 + theta_4 * t^4 + theta_5 * t^5.
    iau_coefficients[2] = (((((THETA_a[5]*t)+THETA_a[4])*t+THETA_a[3])*t+THETA_a[2])*t+THETA_a[1])*t+THETA_a[0];

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
     * Look in Explantory Supplement to the Astronomical Almanac 3rd Edition pg 217 for more information.
     */

    precession_matrix[0] = cos_z_a * cos_theta_a * cos_zeta_a -  sin_z_a * sin_zeta_a;
    precession_matrix[1] = -1 * cos_z_a * cos_theta_a * sin_zeta_a - sin_z_a * cos_zeta_a;
    precession_matrix[2] = -1 * cos_z_a * sin_theta_a;
    precession_matrix[3] = sin_z_a * cos_theta_a * cos_zeta_a + cos_z_a * sin_zeta_a;
    precession_matrix[4] = -1 * sin_z_a * cos_theta_a * sin_zeta_a + cos_z_a * cos_zeta_a;
    precession_matrix[5] = -1 * sin_z_a * sin_theta_a;
    precession_matrix[6] = sin_theta_a * cos_zeta_a;
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
    nutation_critical_arguments[8] = gen_p[0] + gen_p[1]*t;
    nutation_critical_arguments[9] = ((((l_[4])*t+l_[3])*t+l_[2])*t+l_[1])*t+l_[0];
    nutation_critical_arguments[10] = ((((l_prime[4])*t+l_prime[3])*t+l_prime[2])*t+l_prime[1])*t+l_prime[0];
    nutation_critical_arguments[11] = ((((f_[4])*t+f_[3])*t+f_[2])*t+f_[1])*t+f_[0];
    nutation_critical_arguments[12] = ((((d_[4])*t+d_[3])*t+d_[2])*t+d_[1])*t+d_[0];
    nutation_critical_arguments[13] = ((((omega[4])*t+omega[3])*t+omega[2])*t+omega[1])*t+omega[0];

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

    nutation_arguments[2] = (((((EPSILON_a[5]*t)+EPSILON_a[4])*t+EPSILON_a[3])*t+EPSILON_a[2])*t+EPSILON_a[1])*t+EPSILON_a[0];
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
     * Look in Explantory Supplement to the Astronomical Almanac 3rd Edition pg 225 for more information.
     */

    nutation_matrix[0] = cos_delta_psi;
    nutation_matrix[1] = -1 * sin_delta_psi * cos_epsilon_a;
    nutation_matrix[2] = -1 * sin_delta_psi * sin_epsilon_a;
    nutation_matrix[3] = sin_delta_psi * cos_epsilon;
    nutation_matrix[4] = cos_delta_psi * cos_epsilon * cos_epsilon_a + sin_epsilon * sin_epsilon_a;
    nutation_matrix[5] = cos_delta_psi * cos_epsilon * sin_epsilon_a - sin_epsilon * cos_epsilon_a;
    nutation_matrix[6] = sin_delta_psi * sin_epsilon;
    nutation_matrix[7] = cos_delta_psi * sin_epsilon * cos_epsilon_a - cos_epsilon * sin_epsilon_a;
    nutation_matrix[8] = cos_delta_psi * sin_epsilon * sin_epsilon_a + cos_epsilon * cos_epsilon_a;

    return;
}


static double lookup_closest_delta_t(double tt_seconds) {

    double best_delta_t = delta_t_list[(delta_t_length-1)*2];

    /* Because you're more likely to use coordinates closer to current day than J2000.0 start with end of list
     * Technically doesn't matter if more random but can drastically decrease looking through the list if this
     * assumption proves right.
    */
    for(int i = delta_t_length-1; i >= 0; --i) {
        if(tt_seconds >= delta_t_list[i*2+1]) {
           return best_delta_t;
        }
        best_delta_t = delta_t_list[i*2];
    }

    return best_delta_t;
}


static void compute_terrestrial_matrix(double tt_seconds, double equation_of_the_equinoxes, double terrestrial_matrix[]) {

    double delta_t = lookup_closest_delta_t(tt_seconds);
    double Dut = (tt_seconds-delta_t)/86400.0;
    double delta_t_days = delta_t/86400.0;

    double gast = fmod((((((gmst_coefficients[5] * Dut) + gmst_coefficients[4]) * Dut + gmst_coefficients[3]) * Dut
                    + gmst_coefficients[2]) * Dut + gmst_coefficients[1]) * Dut + gmst_coefficients[0]
                    + gmst_coefficients[6] * delta_t + equation_of_the_equinoxes/15.0, 86400.0);

    double gast_angle = gast * M_PI/43200.0;

    terrestrial_matrix[0] = cos(gast_angle);
    terrestrial_matrix[1] = sin(gast_angle);
    terrestrial_matrix[2] = 0.0;
    terrestrial_matrix[3] = -1 * sin(gast_angle);
    terrestrial_matrix[4] = cos(gast_angle);
    terrestrial_matrix[5] = 0.0;
    terrestrial_matrix[6] = 0.0;
    terrestrial_matrix[7] = 0.0;
    terrestrial_matrix[8] = 1.0;

    return;
}




static PyObject *
ecef_from_eci(PyObject *self, PyObject *args) {

    double x, y, z, tt_seconds;

    double precession_matrix[9];
    double iau_coefficients[3];

    double nutation_matrix[9];
    double nutation_arguments[4];

    double terrestrial_matrix[9];

    if(!PyArg_ParseTuple(args, "dddd", &x, &y, &z, &tt_seconds)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_from_eci(double x, double y, double z, double tt_seconds)");
        return PyErr_Occurred();
    }

    compute_iau_coefficients(tt_seconds, iau_coefficients);
    compute_precession_matrix(iau_coefficients, precession_matrix);

    compute_nutation_arguments(tt_seconds, nutation_arguments);
    compute_nutation_matrix(nutation_arguments, nutation_matrix);

    compute_terrestrial_matrix(tt_seconds, nutation_arguments[3], terrestrial_matrix);

    printf("precession_matrix: %f, %f, %f, %f, %f, %f, %f, %f, %f\n", precession_matrix[0], precession_matrix[1], precession_matrix[2], precession_matrix[3], precession_matrix[4], precession_matrix[5], precession_matrix[6], precession_matrix[7], precession_matrix[8]);
    printf("nutation_matrix: %f, %f, %f, %f, %f, %f, %f, %f, %f\n", nutation_matrix[0], nutation_matrix[1], nutation_matrix[2], nutation_matrix[3], nutation_matrix[4], nutation_matrix[5], nutation_matrix[6], nutation_matrix[7], nutation_matrix[8]);
    printf("terrestrial_matrix: %f, %f, %f, %f, %f, %f, %f, %f, %f\n", terrestrial_matrix[0], terrestrial_matrix[1], terrestrial_matrix[2], terrestrial_matrix[3], terrestrial_matrix[4], terrestrial_matrix[5], terrestrial_matrix[6], terrestrial_matrix[7], terrestrial_matrix[8]);

    /* Terrestrial rotation of earth from GAST */
    x = x*terrestrial_matrix[0] + y*terrestrial_matrix[1] + z*terrestrial_matrix[2];
    y = x*terrestrial_matrix[3] + y*terrestrial_matrix[4] + z*terrestrial_matrix[5];
    z = x*terrestrial_matrix[6] + y*terrestrial_matrix[7] + z*terrestrial_matrix[8];

    /* Nutation rotation of earth from J2000.0 */
    x = x*nutation_matrix[0] + y*nutation_matrix[1] + z*nutation_matrix[2];
    y = x*nutation_matrix[3] + y*nutation_matrix[4] + z*nutation_matrix[5];
    z = x*nutation_matrix[6] + y*nutation_matrix[7] + z*nutation_matrix[8];

    /* Precession rotation of earth from J2000.0 */
    x = x*precession_matrix[0] + y*precession_matrix[1] + z*precession_matrix[2];
    y = x*precession_matrix[3] + y*precession_matrix[4] + z*precession_matrix[5];
    z = x*precession_matrix[6] + y*precession_matrix[7] + z*precession_matrix[8];

    /* Bias rotation of earth from J2000.0 */
    x = x*FRAME_BIAS_ROTATION_MATRIX[0] + y*FRAME_BIAS_ROTATION_MATRIX[1] + z*FRAME_BIAS_ROTATION_MATRIX[2];
    y = x*FRAME_BIAS_ROTATION_MATRIX[3] + y*FRAME_BIAS_ROTATION_MATRIX[4] + z*FRAME_BIAS_ROTATION_MATRIX[5];
    z = x*FRAME_BIAS_ROTATION_MATRIX[6] + y*FRAME_BIAS_ROTATION_MATRIX[7] + z*FRAME_BIAS_ROTATION_MATRIX[8];

    return Py_BuildValue("(ddd)", x, y, z);
}


static PyObject *
ellipsoid_radius(PyObject *self, PyObject *args) {

    double semi_major_axis, inverse_flattening, latitude;

    if(!PyArg_ParseTuple(args, "ddd", &semi_major_axis, &inverse_flattening, &latitude)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ellipsoid_radius(double semi_major_axis, double inverse_flattening, double latitude)");
        return PyErr_Occurred();
    }

    double sin_of_latitude = sin(latitude * M_PI/180);
    double difference_of_flattening = 1 - 1 / inverse_flattening;

    return Py_BuildValue("d", sqrt((semi_major_axis * semi_major_axis) /
                    (1 + (1 / (difference_of_flattening*difference_of_flattening) - 1) * (sin_of_latitude * sin_of_latitude))));
}


static PyMethodDef tolueneBaseMethods[] = {
    {"ecef_from_lla", ecef_from_lla, METH_VARARGS, "Convert lla coordinates to ecef."},
    {"lla_from_ecef", lla_from_ecef, METH_VARARGS, "Convert ecef coordinates to lla using the none recursive method."},
    {"ecef_from_eci", ecef_from_eci, METH_VARARGS, "Convert eci coordinates to ecef."},
    {"ellipsoid_radius", ellipsoid_radius, METH_VARARGS, "Calculate the radius of the ellipsoid at a given latitude."},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef base_extensions = {
    PyModuleDef_HEAD_INIT,
    "base_extensions",
    "C Extensions to toluene base class functions",
    -1,
    tolueneBaseMethods
};

PyMODINIT_FUNC PyInit_base_extensions(void) {
    return PyModule_Create(&base_extensions);
}


#ifdef __cplusplus
}
#endif