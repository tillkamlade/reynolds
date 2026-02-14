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
            double D,
            double B,
            double s,
            double F,
            double f,
            double eta,
            double p_amb,
            int n_theta,
            int n_z,
            double theta_min,
            double theta_max
        ) : D(D), B(B), s(s), F(F), f(f), eta(eta), p_amb(p_amb), n_theta(n_theta), n_z(n_z), theta_min(theta_min), theta_max(theta_max) {}

        double D, B, s, F, f, eta, p_amb, theta_min, theta_max;
        int n_theta, n_z;
};

extern "C" {
    typedef struct {
        PyObject_HEAD
        Reynolds r;
    } ReynoldsObject;

    /* Prototype / stub: __new__ allocates and zero-inits fields */
    static PyObject *
    Reynolds_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
    {
        PyObject *self = type->tp_alloc(type, 0);
        if (!self) return NULL;
        return self;
    }

    /* Prototype / stub: __init__ (parsing/initialization to be implemented later) */
    static int
    Reynolds_init(ReynoldsObject *self, PyObject *args, PyObject *kwargs)
    {
        static const char *kwlist[] = {"D","B","s","F","f","eta","p_amb","n_theta","n_z","theta_min","theta_max", NULL};
        double D=0, B=0, s=0, F=0, f=0, eta=0, p_amb=0, theta_min=0, theta_max=2*M_PI;
        int n_theta=0, n_z=0;

        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "dddddddII|dd", kwlist,
                                         &D,&B,&s,&F,&f,&eta,&p_amb,&n_theta,&n_z,&theta_min,&theta_max)) {
            /* For now do not raise a custom error; propagate parse failure */
            return -1;
        }

        new (&(self->r)) Reynolds(D, B, s, F, f, eta, p_amb, n_theta, n_z, theta_min, theta_max);

        return 0;
    }

    /* Prototype / stub: A(epsilon, beta) -> raises NotImplementedError for now */
    static PyObject *
    Reynolds_A(ReynoldsObject *self, PyObject *args)
    {
        double epsilon=0.0, beta=0.0;
        if (!PyArg_ParseTuple(args, "dd", &epsilon, &beta)) {
            return NULL;
        }

        PyErr_SetString(PyExc_NotImplementedError, "Reynolds.A is not implemented yet");
        return NULL;
    }

    static PyMethodDef Reynolds_methods[] = {
        {"A", (PyCFunction)Reynolds_A, METH_VARARGS, "A(epsilon, beta) -> ndarray (prototype)"},
        {NULL, NULL, 0, NULL}
    };

    static PyMemberDef Reynolds_members[] = {
        {"D", T_DOUBLE, offsetof(ReynoldsObject, r.D), 0, "diameter"},
        {"B", T_DOUBLE, offsetof(ReynoldsObject, r.B), 0, "width"},
        {"s", T_DOUBLE, offsetof(ReynoldsObject, r.s), 0, "s"},
        {"F", T_DOUBLE, offsetof(ReynoldsObject, r.F), 0, "F"},
        {"f", T_DOUBLE, offsetof(ReynoldsObject, r.f), 0, "f"},
        {"eta", T_DOUBLE, offsetof(ReynoldsObject, r.eta), 0, "viscosity"},
        {"p_amb", T_DOUBLE, offsetof(ReynoldsObject, r.p_amb), 0, "ambient pressure"},
        {"n_theta", T_INT, offsetof(ReynoldsObject, r.n_theta), 0, "theta samples"},
        {"n_z", T_INT, offsetof(ReynoldsObject, r.n_z), 0, "z samples"},
        {"theta_min", T_DOUBLE, offsetof(ReynoldsObject, r.theta_min), 0, "theta min"},
        {"theta_max", T_DOUBLE, offsetof(ReynoldsObject, r.theta_max), 0, "theta max"},
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
        (initproc)Reynolds_init,         /* tp_init */
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

    PyMODINIT_FUNC
    PyInit_reynolds(void)
    {
        PyObject *m;
        if (PyType_Ready(&ReynoldsType) < 0) return NULL;
        m = PyModule_Create(&reynoldsmodule);
        if (!m) return NULL;
        Py_INCREF(&ReynoldsType);
        if (PyModule_AddObject(m, "Reynolds", (PyObject *)&ReynoldsType) < 0) {
            Py_DECREF(&ReynoldsType);
            Py_DECREF(m);
            return NULL;
        }
        return m;
    }
}