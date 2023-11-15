#define PY_SSIZE_T_CLEAN
#include <Python.h>

#ifdef __cplusplus
extern "C"
{
#endif

static PyObject *
ecef_from_lla(PyObject *self, PyObject *args) {

    double semi_major_axis, semi_minor_axis, latitude, longitude, altitude;

    if(!PyArg_ParseTuple(args, "ddddd", &semi_major_axis, &semi_minor_axis, &latitude, &longitude, &altitude)) {
        return -1;
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
        return -1;
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


static PyMethodDef tolueneBaseMethods[] = {
    {"lla_from_ecef", lla_from_ecef, METH_VARARGS, "Convert ecef coordinates to lla using the none recursive method."},
    {"ecef_from_lla", ecef_from_lla, METH_VARARGS, "Convert lla coordinates to ecef."},
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