#define PY_SSIZE_T_CLEAN
#define _USE_MATH_DEFINES
#include <math.h>
#include <Python.h>
#include <structmember.h>
#include <numpy/arrayobject.h>
#include <new>

#include "reynolds.hpp"

extern "C" {
    typedef struct {
        PyObject_HEAD
        Reynolds r;
    } ReynoldsObject;

    static PyObject *Reynolds_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
    {
        PyObject *self = type->tp_alloc(type, 0);
        if (!self) return NULL;
        return self;
    }

    /* Prototype / stub: __init__ (parsing/initialization to be implemented later) */
    static int Reynolds_init(ReynoldsObject *self, PyObject *args, PyObject *kwargs)
    {
        /* definition of kwarg-names */
        static const char *kwlist[] = {"D","B","s","F","f","eta","p_amb","n_theta","n_z","theta_min","theta_max", NULL};
        /* buffers to load args and kwargs into */
        double D=0, B=0, s=0, F=0, f=0, eta=0, p_amb=0, theta_min=0, theta_max=2*M_PI;
        int n_theta=0, n_z=0;

        /* parsing arguments */
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dddddddii|dd", kwlist, &D,&B,&s,&F,&f,&eta,&p_amb,&n_theta,&n_z,&theta_min,&theta_max)) return -1;

        /* value checks (non-negative) */
        if (D < 0) {PyErr_SetString(PyExc_ValueError, "D must be positive"); return -1;}
        if (B < 0) {PyErr_SetString(PyExc_ValueError, "B must be positive"); return -1;}
        if (s < 0) {PyErr_SetString(PyExc_ValueError, "s must be positive"); return -1;}
        if (F < 0) {PyErr_SetString(PyExc_ValueError, "F must be positive"); return -1;}
        if (f < 0) {PyErr_SetString(PyExc_ValueError, "f must be positive"); return -1;}
        if (eta < 0) {PyErr_SetString(PyExc_ValueError, "eta must be positive"); return -1;}
        if (p_amb < 0) {PyErr_SetString(PyExc_ValueError, "p_amb must be positive"); return -1;}
        if (n_theta < 0) {PyErr_SetString(PyExc_ValueError, "n_theta must be positive"); return -1;}
        if (n_z < 0) {PyErr_SetString(PyExc_ValueError, "n_z must be positive"); return -1;}
        if (theta_min < 0) {PyErr_SetString(PyExc_ValueError, "theta_min must be positive"); return -1;}
        if (theta_max < 0) {PyErr_SetString(PyExc_ValueError, "theta_max must be positive"); return -1;}

        /* placement new to construct Reynolds-object at allocated location */
        new (&(self->r)) Reynolds(D, B, s, F, f, eta, p_amb, static_cast<unsigned int>(n_theta), static_cast<unsigned int>(n_z), theta_min, theta_max);

        return 0;
    }

    static void Reynolds_dealloc(ReynoldsObject *self) {
        /* deconstruct Reynolds-object */
        self->r.~Reynolds();
        /* deallocate the ReynoldsObject */
        Py_TYPE(self)->tp_free(reinterpret_cast<PyObject *>(self));
    }

    static PyObject *Reynolds_theta(ReynoldsObject *self, void *closure) {
        npy_intp dims[1] = {self->r.n_theta};

        PyObject *theta = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
        if (!theta) return NULL;

        double *data = (double *) PyArray_DATA((PyArrayObject *) theta);

        for (int i = 0; i < dims[0]; i++) {
            data[i] = self->r.theta[i];
        }

        return theta;
    }

    static PyObject *Reynolds_z(ReynoldsObject *self, void *closure) {
        npy_intp dims[1] = {self->r.n_z};

        PyObject *z = PyArray_SimpleNew(1, dims, NPY_DOUBLE);
        if (!z) return NULL;

        double *data = (double *) PyArray_DATA((PyArrayObject *) z);

        for (int i = 0; i < dims[0]; i++) {
            data[i] = self->r.z[i];
        }

        return z;
    }

    static PyObject *Reynolds_A(ReynoldsObject *self, PyObject *args, PyObject *kwargs)
    {
        static const char *kwlist[] = {"epsilon", "beta", NULL};
        double epsilon=0.0, beta=0.0;
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dd", kwlist, &epsilon, &beta)) return NULL;

        PyErr_SetString(PyExc_NotImplementedError, "Reynolds.A is not implemented yet");
        return NULL;
    }

    static PyMethodDef Reynolds_methods[] = {
        {"A", reinterpret_cast<PyCFunction>(Reynolds_A), METH_VARARGS | METH_KEYWORDS, "A(epsilon: double, beta: double) -> ndarray"},
        {NULL, NULL, 0, NULL}
    };

    static PyMemberDef Reynolds_members[] = {
        {"R", T_DOUBLE, offsetof(ReynoldsObject, r.R), Py_READONLY, "radius"},
        {"s", T_DOUBLE, offsetof(ReynoldsObject, r.s), Py_READONLY, "clearance"},
        {"F", T_DOUBLE, offsetof(ReynoldsObject, r.F), Py_READONLY, "load"},
        {"omega", T_DOUBLE, offsetof(ReynoldsObject, r.omega), Py_READONLY, "rotational frequency"},
        {"eta", T_DOUBLE, offsetof(ReynoldsObject, r.eta), Py_READONLY, "viscosity"},
        {"p_amb", T_DOUBLE, offsetof(ReynoldsObject, r.p_amb), Py_READONLY, "ambient pressure"},
        {"n_theta", T_UINT, offsetof(ReynoldsObject, r.n_theta), Py_READONLY, "number of theta samples"},
        {"n_z", T_UINT, offsetof(ReynoldsObject, r.n_z), Py_READONLY, "number of z samples"},
        {"n", T_UINT, offsetof(ReynoldsObject, r.n), Py_READONLY, "total number of points (n_theta * n_z)"},
        {"dtheta", T_DOUBLE, offsetof(ReynoldsObject, r.dtheta), Py_READONLY, "distance between samples in theta-direction"},
        {"dz", T_DOUBLE, offsetof(ReynoldsObject, r.dz), Py_READONLY, "distance between samples in z-direction"},
        {NULL}
    };

    static PyGetSetDef Reynolds_getset[] = {
        {"theta", reinterpret_cast<getter>(Reynolds_theta), NULL, "", NULL},
        {"z", reinterpret_cast<getter>(Reynolds_z), NULL, "", NULL},
        {NULL}
    };

    static PyTypeObject ReynoldsType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "reynolds.Reynolds",             /* tp_name */
        sizeof(ReynoldsObject),          /* tp_basicsize */
        0,                               /* tp_itemsize */
        reinterpret_cast<destructor>(Reynolds_dealloc),                               /* tp_dealloc */
        0,                               /* tp_vectorcall_offset / tp_print */
        0,                               /* tp_getattr */
        0,                               /* tp_setattr */
        0,                               /* tp_as_async */
        0,                               /* tp_repr */
        0,                               /* tp_as_number */
        0,                               /* tp_as_sequence */
        0,                               /* tp_as_mapping */
        0,                               /* tp_hash */
        0,                               /* tp_call */
        0,                               /* tp_str */
        0,                               /* tp_getattro */
        0,                               /* tp_setattro */
        0,                               /* tp_as_buffer */
        Py_TPFLAGS_DEFAULT,              /* tp_flags */
        "Reynolds objects",              /* tp_doc */
        0,                               /* tp_traverse */
        0,                               /* tp_clear */
        0,                               /* tp_richcompare */
        0,                               /* tp_weaklistoffset */
        0,                               /* tp_iter */
        0,                               /* tp_iternext */
        Reynolds_methods,                /* tp_methods */
        Reynolds_members,                /* tp_members */
        Reynolds_getset,                               /* tp_getset */
        0,                               /* tp_base */
        0,                               /* tp_dict */
        0,                               /* tp_descr_get */
        0,                               /* tp_descr_set */
        0,                               /* tp_dictoffset */
        reinterpret_cast<initproc>(Reynolds_init),         /* tp_init */
        0,                               /* tp_alloc */
        Reynolds_new,                    /* tp_new */
    };

    static struct PyModuleDef reynoldsmodule = {
        PyModuleDef_HEAD_INIT,
        "reynolds",
        "reynolds module (C++ extension)",
        -1,
        NULL, NULL, NULL, NULL, NULL
    };

    PyMODINIT_FUNC PyInit_reynolds(void)
    {
        PyObject *m;
        if (PyType_Ready(&ReynoldsType) < 0) return NULL;
        m = PyModule_Create(&reynoldsmodule);
        if (!m) return NULL;
        Py_INCREF(&ReynoldsType);
        if (PyModule_AddObjectRef(m, "Reynolds", reinterpret_cast<PyObject *>(&ReynoldsType)) < 0) {
            Py_DECREF(&ReynoldsType);
            Py_DECREF(m);
            return NULL;
        }

        import_array();

        return m;
    }
}