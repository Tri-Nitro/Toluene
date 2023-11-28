#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "earth_gravitational_model.h"
#include "egm84.h"
#include "interpolation.h"
#include "matrix.h"
#include "rotation_matrices.h"

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

    double vector[3];
    double arguments[4];
    double matrix[9];


    if(!PyArg_ParseTuple(args, "dddd", &x, &y, &z, &tt_seconds)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_from_eci(double x, double y, double z, double tt_seconds)");
        return PyErr_Occurred();
    }

    vector[0] = x;
    vector[1] = y;
    vector[2] = z;

    compute_polar_motion_matrix(tt_seconds, matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_nutation_arguments(tt_seconds, arguments);

    compute_terrestrial_matrix(tt_seconds, arguments[3],  matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_nutation_matrix(arguments, matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_precession_arguments(tt_seconds, arguments);
    compute_precession_matrix(arguments, matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_bias_matrix(matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    x = vector[0];
    y = vector[1];
    z = vector[2];

    return Py_BuildValue("(ddd)", x, y, z);
}


static PyObject *
ecef_from_eci(PyObject *self, PyObject *args) {

    double x, y, z, tt_seconds;

    double vector[3];
    double arguments[4];
    double matrix[9];

    if(!PyArg_ParseTuple(args, "dddd", &x, &y, &z, &tt_seconds)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. ecef_from_eci(double x, double y, double z, double tt_seconds)");
        return PyErr_Occurred();
    }

    vector[0] = x;
    vector[1] = y;
    vector[2] = z;

    compute_bias_matrix(matrix);
    matrix_transpose_3_3_in_place(matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_precession_arguments(tt_seconds, arguments);
    compute_precession_matrix(arguments, matrix);
    matrix_transpose_3_3_in_place(matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_nutation_arguments(tt_seconds, arguments);
    compute_nutation_matrix(arguments, matrix);
    matrix_transpose_3_3_in_place(matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_terrestrial_matrix(tt_seconds, arguments[3], matrix);
    matrix_transpose_3_3_in_place(matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    compute_polar_motion_matrix(tt_seconds, matrix);
    matrix_transpose_3_3_in_place(matrix);
    matrix_dot_product_3_3_vector_in_place(vector, matrix);

    x = vector[0];
    y = vector[1];
    z = vector[2];

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



static void delete_egm84_coefficients(PyObject * obj) {
    EGM84_Coefficients* pointer = PyCapsule_GetPointer(obj, "EGM84_Coefficients");
    free(pointer->coefficients);
    free(pointer);
}


static PyObject*
load_egm84_coefficients(PyObject* self, PyObject* args) {


    const char* filename;

    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    EGM84_Coefficients* egm84 = (EGM84_Coefficients*)malloc(sizeof(EGM84_Coefficients));
    egm84->coefficients = (GravityCoefficients*)malloc(sizeof(GravityCoefficients) * 16467);
    if(egm84->coefficients == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for EGM84 coefficients.");
        return PyErr_Occurred();
    } else {
        printf("coefficients: %p\n", egm84->coefficients);
    }

    egm84->num_coefficients = 16467;

    egm84->coefficients[0].n = 1;
    egm84->coefficients[0].m = 2;
    egm84->coefficients[0].C = 3;
    egm84->coefficients[0].S = 4;

    PyObject* capsule = PyCapsule_New(egm84, "EGM84_Coefficients", delete_egm84_coefficients);
    return capsule;
}


static PyObject* test_egm84(PyObject* self, PyObject* args) {

    PyObject* capsule;
    EGM84_Coefficients* test;

    if (!PyArg_ParseTuple(args, "O", &capsule)) {
        return NULL;
    }
    test = (EGM84_Coefficients*)PyCapsule_GetPointer(capsule, "EGM84_Coefficients");

    printf("n: %d, m: %d, C: %f, S: %f\n", test->coefficients[0].n, test->coefficients[0].m, test->coefficients[0].C, test->coefficients[0].S);

    return Py_BuildValue("d", 0);
}

static PyMethodDef tolueneCoreMethods[] = {
    {"load_egm84_coefficients", load_egm84_coefficients, METH_VARARGS, "Load the EGM84 geoid model coefficients into memory."},
    {"test_egm84", test_egm84, METH_VARARGS, "Test the EGM84 geoid model."},
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