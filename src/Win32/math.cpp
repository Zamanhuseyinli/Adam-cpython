#include <Python.h>
#include <cmath>  // math kütüphanesini dahil ettik

// pow - Üs alma
static PyObject* py_pow(PyObject* self, PyObject* args) {
    double base, exponent;
    if (!PyArg_ParseTuple(args, "dd", &base, &exponent)) {
        return NULL;
    }
    double result = pow(base, exponent);
    return Py_BuildValue("d", result);
}

// sqrt - Karekök alma
static PyObject* py_sqrt(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = sqrt(value);
    return Py_BuildValue("d", result);
}

// exp - Üstel fonksiyon
static PyObject* py_exp(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = exp(value);
    return Py_BuildValue("d", result);
}

// log - Doğal logaritma
static PyObject* py_log(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = log(value);
    return Py_BuildValue("d", result);
}

// sin - Sinüs fonksiyonu
static PyObject* py_sin(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = sin(value);
    return Py_BuildValue("d", result);
}

// cos - Kosinüs fonksiyonu
static PyObject* py_cos(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = cos(value);
    return Py_BuildValue("d", result);
}

// tan - Tangent fonksiyonu
static PyObject* py_tan(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = tan(value);
    return Py_BuildValue("d", result);
}

// ceil - Sayıyı yukarı yuvarlama
static PyObject* py_ceil(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = ceil(value);
    return Py_BuildValue("d", result);
}

// floor - Sayıyı aşağı yuvarlama
static PyObject* py_floor(PyObject* self, PyObject* args) {
    double value;
    if (!PyArg_ParseTuple(args, "d", &value)) {
        return NULL;
    }
    double result = floor(value);
    return Py_BuildValue("d", result);
}

// mod - Bölmeden kalan hesaplama
static PyObject* py_mod(PyObject* self, PyObject* args) {
    double dividend, divisor;
    if (!PyArg_ParseTuple(args, "dd", &dividend, &divisor)) {
        return NULL;
    }
    double result = fmod(dividend, divisor);
    return Py_BuildValue("d", result);
}

// Python modülü ve fonksiyonları
static PyMethodDef math_methods[] = {
    {"pow", py_pow, METH_VARARGS, "Raise a number to a power"},
    {"sqrt", py_sqrt, METH_VARARGS, "Compute square root"},
    {"exp", py_exp, METH_VARARGS, "Compute exponential (e^x)"},
    {"log", py_log, METH_VARARGS, "Compute natural logarithm"},
    {"sin", py_sin, METH_VARARGS, "Compute sine"},
    {"cos", py_cos, METH_VARARGS, "Compute cosine"},
    {"tan", py_tan, METH_VARARGS, "Compute tangent"},
    {"ceil", py_ceil, METH_VARARGS, "Round number up"},
    {"floor", py_floor, METH_VARARGS, "Round number down"},
    {"mod", py_mod, METH_VARARGS, "Compute remainder of division"},
    {NULL, NULL, 0, NULL} // End marker
};

// Modül Tanımı
static struct PyModuleDef mathmodule = {
    PyModuleDef_HEAD_INIT,
    "math", // Modül ismi
    "A module for mathematical functions", // Modül açıklaması
    -1, // Modül state'i, -1 global
    math_methods // Fonksiyonlar dizisi
};

// Modül başlatma fonksiyonu
PyMODINIT_FUNC PyInit_math(void) {
    return PyModule_Create(&mathmodule);
}
