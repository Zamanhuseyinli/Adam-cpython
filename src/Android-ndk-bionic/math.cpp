#include <Python.h>
#include <cmath>

// pow
static PyObject* py_pow(PyObject* self, PyObject* args) {
    double base, exponent;
    if (!PyArg_ParseTuple(args, "dd", &base, &exponent)) {
        return NULL;
    }
    return Py_BuildValue("d", pow(base, exponent));
}

// sqrt
static PyObject* py_sqrt(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    return Py_BuildValue("d", sqrt(value));
}

// exp
static PyObject* py_exp(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    return Py_BuildValue("d", exp(value));
}

// log
static PyObject* py_log(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    return Py_BuildValue("d", log(value));
}

// Metodlar
static PyMethodDef MathMethods[] = {
    {"pow", py_pow, METH_VARARGS, "Üs alma"},
    {"sqrt", py_sqrt, METH_VARARGS, "Karekök"},
    {"exp", py_exp, METH_VARARGS, "Üstel fonksiyon"},
    {"log", py_log, METH_VARARGS, "Logaritma"},
    {NULL, NULL, 0, NULL}
};

// Modül tanımı
static struct PyModuleDef mathmodule = {
    PyModuleDef_HEAD_INIT,
    "mathnative",   // modül adı
    "C++ ile yazılmış matematik modülü",
    -1,
    MathMethods
};

PyMODINIT_FUNC PyInit_mathnative(void) {
    return PyModule_Create(&mathmodule);
}
