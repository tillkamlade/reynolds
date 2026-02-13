#define PY_SSIZE_T_CLEAN
#define _USE_MATH_DEFINES
#include <math.h>
#include <Python.h>
#include <structmember.h>

extern "C" {
    typedef struct {
        PyObject_HEAD
        double D, B, s, F, f, eta, p_amb;
        int n_theta, n_z;
        double theta_min;
        double theta_max;
    } ReynoldsObject;

    /* Prototype / stub: __new__ allocates and zero-inits fields */
    static PyObject *
    Reynolds_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
    {
        ReynoldsObject *self = (ReynoldsObject *)type->tp_alloc(type, 0);
        if (!self) return NULL;
        self->D = self->B = self->s = self->F = self->f = self->eta = self->p_amb = 0.0;
        self->n_theta = 0;
        self->n_z = 0;
        self->theta_min = 0.0;
        self->theta_max = 0.0;
        return (PyObject *)self;
    }

    /* Prototype / stub: __init__ (parsing/initialization to be implemented later) */
    static int
    Reynolds_init(ReynoldsObject *self, PyObject *args, PyObject *kwds)
    {
        static const char *kwlist[] = {"D","B","s","F","f","eta","p_amb","n_theta","n_z","theta_min","theta_max", NULL};
        double D=0, B=0, s=0, F=0, f=0, eta=0, p_amb=0, theta_min=0, theta_max=2*M_PI;
        int n_z=0, n_theta=0;

        if (!PyArg_ParseTupleAndKeywords(args, kwds, "dddddddii|dd", (char **)kwlist,
                                         &D,&B,&s,&F,&f,&eta,&p_amb,&n_theta,&n_z,&theta_min,&theta_max)) {
            /* For now do not raise a custom error; propagate parse failure */
            return -1;
        }

        /* Store parsed values (actual validation/usage to be implemented later) */
        self->D = D;
        self->B = B;
        self->s = s;
        self->F = F;
        self->f = f;
        self->eta = eta;
        self->p_amb = p_amb;
        self->n_theta = n_theta;
        self->n_z = n_z;
        self->theta_min = theta_min;
        self->theta_max = theta_max;

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
        {"D", T_DOUBLE, offsetof(ReynoldsObject, D), 0, "diameter"},
        {"B", T_DOUBLE, offsetof(ReynoldsObject, B), 0, "width"},
        {"s", T_DOUBLE, offsetof(ReynoldsObject, s), 0, "s"},
        {"F", T_DOUBLE, offsetof(ReynoldsObject, F), 0, "F"},
        {"f", T_DOUBLE, offsetof(ReynoldsObject, f), 0, "f"},
        {"eta", T_DOUBLE, offsetof(ReynoldsObject, eta), 0, "viscosity"},
        {"p_amb", T_DOUBLE, offsetof(ReynoldsObject, p_amb), 0, "ambient pressure"},
        {"n_theta", T_INT, offsetof(ReynoldsObject, n_theta), 0, "theta samples"},
        {"n_z", T_INT, offsetof(ReynoldsObject, n_z), 0, "z samples"},
        {"theta_min", T_DOUBLE, offsetof(ReynoldsObject, theta_min), 0, "theta min"},
        {"theta_max", T_DOUBLE, offsetof(ReynoldsObject, theta_max), 0, "theta max"},
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