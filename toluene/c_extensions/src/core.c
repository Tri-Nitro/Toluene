#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "earth_gravitational_model.h"
#include "interpolation.h"
#include "rotation_matrices.h"
#include "toluene_core_config.h"

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