#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>

extern "C" {
    typedef struct {
        PyObject_HEAD
        PyObject *name;
    } reynolds;

    static void
    reynolds_dealloc(reynolds* self)
    {
        Py_XDECREF(self->name);
        Py_TYPE(self)->tp_free((PyObject*)self);
    }

    static PyObject*
    reynolds_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
    {
        reynolds *self = (reynolds*)type->tp_alloc(type, 0);
        if (!self) return NULL;
        self->name = NULL;
        return (PyObject*)self;
    }

    static int
    reynolds_init(reynolds *self, PyObject *args, PyObject *kwds)
    {
        static char *kwlist[] = { (char*)"name", NULL };
        const char *name_c = NULL;

        if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, &name_c))
            return -1;

        PyObject *tmp = PyUnicode_FromString(name_c);
        if (!tmp) return -1;

        Py_XDECREF(self->name);
        self->name = tmp;
        return 0;
    }

    static PyMemberDef reynolds_members[] = {
        {"name", T_OBJECT_EX, offsetof(reynolds, name), 0, "name"},
        {NULL}  /* Sentinel */
    };

    static PyTypeObject reynolds_type = {
        PyVarObject_HEAD_INIT(NULL, 0)
        "reynolds.Reynolds",             /* tp_name */
        sizeof(reynolds),                /* tp_basicsize */
        0,                               /* tp_itemsize */
        (destructor)reynolds_dealloc,    /* tp_dealloc */
        0,                               /* tp_vectorcall_offset / tp_print */
        0,                               /* tp_getattr */
        0,                               /* tp_setattr */
        0,                               /* tp_as_async */
        0,                               /* tp_repr */
        0,                               /* tp_as_number */
        0,                               /* tp_as_sequence */
        0,                               /* tp_as_mapping */
        0,                               /* tp_hash  */
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
        0,                               /* tp_methods */
        reynolds_members,                /* tp_members */
        0,                               /* tp_getset */
        0,                               /* tp_base */
        0,                               /* tp_dict */
        0,                               /* tp_descr_get */
        0,                               /* tp_descr_set */
        0,                               /* tp_dictoffset */
        (initproc)reynolds_init,         /* tp_init */
        0,                               /* tp_alloc */
        reynolds_new,                    /* tp_new */
    };

    static PyModuleDef reynoldsmodule = {
        PyModuleDef_HEAD_INIT,
        "reynolds",
        "Reynolds module",
        -1,
        NULL, NULL, NULL, NULL, NULL
    };

    PyMODINIT_FUNC
    PyInit_reynolds(void)
    {
        PyObject *m;
        if (PyType_Ready(&reynolds_type) < 0)
            return NULL;

        m = PyModule_Create(&reynoldsmodule);
        if (!m) return NULL;

        Py_INCREF(&reynolds_type);
        if (PyModule_AddObject(m, "Reynolds", (PyObject *)&reynolds_type) < 0) {
            Py_DECREF(&reynolds_type);
            Py_DECREF(m);
            return NULL;
        }

        return m;
    }
}