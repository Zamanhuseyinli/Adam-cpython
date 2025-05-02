#include <Python.h>
#include <string.h>
#include <stdlib.h>

// `memchr` function
static PyObject* py_memchr(PyObject* self, PyObject* args) {
    const char* input;
    Py_ssize_t len;
    int c;

    if (!PyArg_ParseTuple(args, "y#i", &input, &len, &c)) {
        return NULL;
    }

    const void* result = memchr(input, c, len);
    if (result) {
        return PyBytes_FromStringAndSize((const char*)result, len - ((const char*)result - input));
    } else {
        Py_RETURN_NONE;
    }
}

// `memset` equivalent (safe memset)
static PyObject* py_memset(PyObject* self, PyObject* args) {
    PyObject* py_buffer;
    int value;
    Py_ssize_t size;

    if (!PyArg_ParseTuple(args, "O!ni", &PyByteArray_Type, &py_buffer, &size, &value)) {
        return NULL;
    }

    if (size > PyByteArray_Size(py_buffer)) {
        PyErr_SetString(PyExc_ValueError, "Size exceeds buffer length");
        return NULL;
    }

    void* buffer = PyByteArray_AsString(py_buffer);
    memset(buffer, value, size);

    Py_INCREF(Py_None);
    return Py_None;
}

// Module method table
static PyMethodDef MemReplicationMethods[] = {
    {"memchr", py_memchr, METH_VARARGS, "Find byte in memory block"},
    {"memset", py_memset, METH_VARARGS, "Set memory with a byte value"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef memreplicationmodule = {
    PyModuleDef_HEAD_INIT,
    "memreplication",
    "Memory operation utilities for Termux",
    -1,
    MemReplicationMethods
};

PyMODINIT_FUNC PyInit_memreplication(void) {
    return PyModule_Create(&memreplicationmodule);
}
