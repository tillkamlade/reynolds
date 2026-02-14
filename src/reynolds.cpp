#define PY_SSIZE_T_CLEAN
#define _USE_MATH_DEFINES
#include <math.h>
#include <Python.h>
#include <structmember.h>
#include <numpy/arrayobject.h>
#include <new>

class Reynolds {
    public:
        Reynolds() : D(0), B(0), s(0), F(0), f(0), eta(0), p_amb(0), n_theta(0), n_z(0), theta_min(0), theta_max(2 * M_PI) {}
        Reynolds(
            const double D,
            const double B,
            const double s,
            const double F,
            const double f,
            const double eta,
            const double p_amb,
            const unsigned int n_theta,
            const unsigned int n_z,
            const double theta_min,
            const double theta_max
        ) : D(D), B(B), s(s), F(F), f(f), eta(eta), p_amb(p_amb), n_theta(n_theta), n_z(n_z), theta_min(theta_min), theta_max(theta_max) {}

        const double D, B, s, F, f, eta, p_amb, theta_min, theta_max;
        const unsigned int n_theta, n_z;
};

extern "C" {
    typedef struct {
        PyObject_HEAD
        Reynolds r;
    } ReynoldsObject;

    /* Prototype / stub: __new__ allocates and zero-inits fields */
    static PyObject *Reynolds_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
    {
        PyObject *self = type->tp_alloc(type, 0);
        if (!self) return NULL;
        return self;
    }

    /* Prototype / stub: __init__ (parsing/initialization to be implemented later) */
    static int Reynolds_init(ReynoldsObject *self, PyObject *args, PyObject *kwargs)
    {
        static const char *kwlist[] = {"D","B","s","F","f","eta","p_amb","n_theta","n_z","theta_min","theta_max", NULL};
        double D=0, B=0, s=0, F=0, f=0, eta=0, p_amb=0, theta_min=0, theta_max=2*M_PI;
        int n_theta=0, n_z=0;

        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dddddddii|dd", kwlist, &D,&B,&s,&F,&f,&eta,&p_amb,&n_theta,&n_z,&theta_min,&theta_max)) return -1;

        if (D < 0) {PyErr_SetString(PyExc_AttributeError, "D must be positive"); return -1;}
        if (B < 0) {PyErr_SetString(PyExc_AttributeError, "B must be positive"); return -1;}
        if (s < 0) {PyErr_SetString(PyExc_AttributeError, "s must be positive"); return -1;}
        if (F < 0) {PyErr_SetString(PyExc_AttributeError, "F must be positive"); return -1;}
        if (f < 0) {PyErr_SetString(PyExc_AttributeError, "f must be positive"); return -1;}
        if (eta < 0) {PyErr_SetString(PyExc_AttributeError, "eta must be positive"); return -1;}
        if (p_amb < 0) {PyErr_SetString(PyExc_AttributeError, "p_amb must be positive"); return -1;}
        if (n_theta < 0) {PyErr_SetString(PyExc_AttributeError, "n_theta must be positive"); return -1;}
        if (n_z < 0) {PyErr_SetString(PyExc_AttributeError, "n_z must be positive"); return -1;}
        if (theta_min < 0) {PyErr_SetString(PyExc_AttributeError, "theta_min must be positive"); return -1;}
        if (theta_max < 0) {PyErr_SetString(PyExc_AttributeError, "theta_max must be positive"); return -1;}

        new (&(self->r)) Reynolds(D, B, s, F, f, eta, p_amb, static_cast<unsigned int>(n_theta), static_cast<unsigned int>(n_z), theta_min, theta_max);

        return 0;
    }

    static PyObject *Reynolds_A(ReynoldsObject *self, PyObject *args)
    {
        double epsilon=0.0, beta=0.0;
        if (!PyArg_ParseTuple(args, "dd", &epsilon, &beta)) return NULL;

        PyErr_SetString(PyExc_NotImplementedError, "Reynolds.A is not implemented yet");
        return NULL;
    }

    static PyMethodDef Reynolds_methods[] = {
        {"A", (PyCFunction)Reynolds_A, METH_VARARGS, "A(epsilon, beta) -> ndarray (prototype)"},
        {NULL, NULL, 0, NULL}
    };

    static PyMemberDef Reynolds_members[] = {
        {"D", T_DOUBLE, offsetof(ReynoldsObject, r.D), Py_READONLY, "diameter"},
        {"B", T_DOUBLE, offsetof(ReynoldsObject, r.B), Py_READONLY, "width"},
        {"s", T_DOUBLE, offsetof(ReynoldsObject, r.s), Py_READONLY, "s"},
        {"F", T_DOUBLE, offsetof(ReynoldsObject, r.F), Py_READONLY, "F"},
        {"f", T_DOUBLE, offsetof(ReynoldsObject, r.f), Py_READONLY, "f"},
        {"eta", T_DOUBLE, offsetof(ReynoldsObject, r.eta), Py_READONLY, "viscosity"},
        {"p_amb", T_DOUBLE, offsetof(ReynoldsObject, r.p_amb), Py_READONLY, "ambient pressure"},
        {"n_theta", T_UINT, offsetof(ReynoldsObject, r.n_theta), Py_READONLY, "theta samples"},
        {"n_z", T_UINT, offsetof(ReynoldsObject, r.n_z), Py_READONLY, "z samples"},
        {"theta_min", T_DOUBLE, offsetof(ReynoldsObject, r.theta_min), Py_READONLY, "theta min"},
        {"theta_max", T_DOUBLE, offsetof(ReynoldsObject, r.theta_max), Py_READONLY, "theta max"},
        {NULL}  /* Sentinel */
    };

    static PyTypeObject ReynoldsType = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "reynolds.Reynolds",             /* tp_name */
        sizeof(ReynoldsObject),          /* tp_basicsize */
        0,                               /* tp_itemsize */
        0,                               /* tp_dealloc */
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
        0,                               /* tp_getset */
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
        return m;
    }
}