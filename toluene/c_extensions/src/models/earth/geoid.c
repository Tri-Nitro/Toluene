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

#define __compile_models_earth_geoid
#include "models/earth/geoid.h"

#if defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or 64 bit Windows systems */

#define _USE_MATH_DEFINES
#include <math.h>

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif


static PyObject* add_grid_point(PyObject* self, PyObject* args) {

    PyObject* capsule;
    Geoid* geoid;

    double lat;
    double lon;
    double height;

    if(!PyArg_ParseTuple(args, "Oddd", &capsule, &lat, &lon, &height)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. add_grid_point()");
        return PyErr_Occurred();
    }

    geoid = (Geoid*)PyCapsule_GetPointer(capsule, "Geoid");
    if(!geoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Geoid from capsule.");
        return PyErr_Occurred();
    }

    if (!geoid->points){
        geoid->points = (GeoidPoint*)malloc(360 * sizeof(GeoidPoint));
        if(!geoid->points) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for Geoid points.");
            return PyErr_Occurred();
        }
        geoid->npoints_allocated = 360;
    }

    if (geoid->npoints_allocated < geoid->npoints+1) {
        geoid->npoints_allocated += 360;
        GeoidPoint* new_geoid_points = (GeoidPoint*)malloc(geoid->npoints_allocated * sizeof(GeoidPoint));
        if(!new_geoid_points) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for Geoid points.");
            return PyErr_Occurred();
        }
        memcpy(new_geoid_points, geoid->points, geoid->npoints * sizeof(GeoidPoint));
        free(geoid->points);
        geoid->points = new_geoid_points;
    }

    int i = 0;
    for(i = geoid->npoints-1; i >= 0; --i) {
        if(geoid->points[i].lat == lat && geoid->points[i].lon == lon) {
            geoid->points[i].height = height;
            break;
        }
        if (geoid->points[i].lat < lat) {
            geoid->points[i+1].lat = geoid->points[i].lat;
            geoid->points[i+1].lon = geoid->points[i].lon;
            geoid->points[i+1].height = geoid->points[i].height;
        } else if (geoid->points[i].lat == lat && geoid->points[i].lon > lon) {
            geoid->points[i+1].lat = geoid->points[i].lat;
            geoid->points[i+1].lon = geoid->points[i].lon;
            geoid->points[i+1].height = geoid->points[i].height;
        } else if (geoid->points[i].lat >= lat && geoid->points[i].lon < lon) {
            geoid->points[i+1].lat = geoid->points[i].lat;
            geoid->points[i+1].lon = geoid->points[i].lon;
            geoid->points[i+1].height = geoid->points[i].height;
        } else {
            geoid->points[i+1].lat = lat;
            geoid->points[i+1].lon = lon;
            geoid->points[i+1].height = height;
            geoid->npoints++;
            break;
        }
    }
    if(i < 0) {
        geoid->points[0].lat = lat;
        geoid->points[0].lon = lon;
        geoid->points[0].height = height;
        geoid->npoints++;
    }

    return Py_BuildValue("i", 0);
}


static PyObject* add_harmonic(PyObject* self, PyObject* args) {

    PyObject* capsule;
    Geoid* geoid;

    int order, degree;
    double C, S;

    if(!PyArg_ParseTuple(args, "Oiidd", &capsule, &order, &degree, &C, &S)) {
        PyErr_SetString(PyExc_TypeError, "Unable to parse arguments. add_harmonic()");
        return PyErr_Occurred();
    }

    geoid = (Geoid*)PyCapsule_GetPointer(capsule, "Geoid");
    if(!geoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to get the Geoid from capsule.");
        return PyErr_Occurred();
    }

    if (!geoid->harmonics){
        geoid->harmonics = (Harmonic*)malloc(41 * sizeof(Harmonic));
        if(!geoid->harmonics) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for Geoid harmonics.");
            return PyErr_Occurred();
        }
        geoid->nharmonics_allocated = 360;
    }

    if (geoid->nharmonics_allocated < geoid->nharmonics+1) {
        geoid->nharmonics_allocated += order*order;
        Harmonic* new_geoid_harmonics = (Harmonic*)malloc(geoid->nharmonics_allocated * sizeof(Harmonic));
        if(!new_geoid_harmonics) {
            PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for Geoid harmonics.");
            return PyErr_Occurred();
        }
        memcpy(new_geoid_harmonics, geoid->harmonics, geoid->nharmonics * sizeof(Harmonic));
        free(geoid->harmonics);
        geoid->harmonics = new_geoid_harmonics;
    }

    int i = 0;
    for(i = geoid->nharmonics-1; i >= 0; --i) {
        if (geoid->harmonics[i].order > order) {
            geoid->harmonics[i+1].order = geoid->harmonics[i].order;
            geoid->harmonics[i+1].degree = geoid->harmonics[i].degree;
            geoid->harmonics[i+1].C = geoid->harmonics[i].C;
            geoid->harmonics[i+1].S = geoid->harmonics[i].S;
        } else if (geoid->harmonics[i].order == order && geoid->harmonics[i].degree > degree) {
            geoid->harmonics[i+1].order = geoid->harmonics[i].order;
            geoid->harmonics[i+1].degree = geoid->harmonics[i].degree;
            geoid->harmonics[i+1].C = geoid->harmonics[i].C;
            geoid->harmonics[i+1].S = geoid->harmonics[i].S;
        } else {
            geoid->harmonics[i+1].order = order;
            geoid->harmonics[i+1].degree = degree;
            geoid->harmonics[i+1].C = C;
            geoid->harmonics[i+1].S = S;
            geoid->nharmonics++;
            break;
        }
    }
    if(i < 0) {
        geoid->harmonics[0].order = order;
        geoid->harmonics[0].degree = degree;
        geoid->harmonics[0].C = C;
        geoid->harmonics[0].S = S;
        geoid->nharmonics++;
    }

    return Py_BuildValue("i", 0);
}


static PyObject* new_Geoid(PyObject* self, PyObject* args) {

    Geoid* geoid = (Geoid*)malloc(sizeof(Geoid));

    if(!geoid) {
        PyErr_SetString(PyExc_MemoryError, "Unable to allocate memory for new_Geoid.");
        return PyErr_Occurred();
    }

    geoid->nharmonics = 0;
    geoid->npoints = 0;
    geoid->nharmonics_allocated = 0;
    geoid->npoints_allocated = 0;
    geoid->harmonics = NULL;
    geoid->points = NULL;

    return PyCapsule_New(geoid, "Geoid", delete_Geoid);
}


static void delete_Geoid(PyObject* obj) {

    Geoid* geoid = (Geoid*)PyCapsule_GetPointer(obj, "Geoid");

    if(geoid) {
        if(geoid->harmonics) {
            free(geoid->harmonics);
        }
        if(geoid->points) {
            free(geoid->points);
        }
        free(geoid);
    }

}


static PyMethodDef tolueneModelsEarthGeoidMethods[] = {
    {"add_grid_point", add_grid_point, METH_VARARGS, "Add a grid point to a Geoid object"},
    {"add_harmonic", add_harmonic, METH_VARARGS, "Add a harmonic to a Geoid object"},
    {"new_Geoid", new_Geoid, METH_VARARGS, "Create a new Geoid object"},
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef models_earth_geoid = {
    PyModuleDef_HEAD_INIT,
    "models.earth.geoid",
    "C Extensions to toluene models geoid functions",
    -1,
    tolueneModelsEarthGeoidMethods
};


PyMODINIT_FUNC PyInit_geoid(void) {
    return PyModule_Create(&models_earth_geoid);
}


#ifdef __cplusplus
} /* extern "C" */
#endif