#include <Python.h>

static PyObject* division(PyObject *self, PyObject *args) {
    long divident, divisor;
    if (!PyArg_ParseTuple(args, "ll", &divident, &divisor)) {
        return NULL;
    }
    if (0 == divisor) {
       PyErr_Format(PyExc_ZeroDivisionError, "Dividing %d by zero!", divident);
       return NULL;
    }
    return PyLong_FromLong(divident / divisor);
}

// exported methods are collected in a table
PyMethodDef method_table[] = {
    {"division", (PyCFunction) division, METH_VARARGS, "Method docstring"},
    {NULL, NULL, 0, NULL} // sentinel value ending the table
};

// a struct contains the definition of a module
PyModuleDef mymath_module = {
    PyModuleDef_HEAD_INIT,
    "mymath", // module name
    "This is the module docstring",
    -1, // optonal size of the module state memory
    method_table,
    NULL, // Optional slot defintions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

// the module init function
PyMODINIT_FUNC PyInit_mymath(void) {
    return PyModule_Create(&mymath_module);
}
