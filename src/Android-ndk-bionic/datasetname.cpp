#include <Python.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#ifdef __ANDROID__
#include <android/log.h>
#define LOG_TAG "datasetname"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#define LOGI(...) printf(__VA_ARGS__)
#endif

// `memset` function
static PyObject* py_memset(PyObject* self, PyObject* args) {
    Py_buffer buffer;
    int value, size;

    if (!PyArg_ParseTuple(args, "y*ii", &buffer, &value, &size)) {
        return NULL;
    }

    memset(buffer.buf, value, size);
    PyBuffer_Release(&buffer);
    Py_RETURN_NONE;
}

// `memcmp` function
static PyObject* py_memcmp(PyObject* self, PyObject* args) {
    const char *ptr1, *ptr2;
    int n;

    if (!PyArg_ParseTuple(args, "y#y#i", &ptr1, &n, &ptr2, &n, &n)) {
        return NULL;
    }

    int result = memcmp(ptr1, ptr2, n);
    return Py_BuildValue("i", result);
}

// `itoa` function
static PyObject* py_itoa(PyObject* self, PyObject* args) {
    int value;
    char str[32];

    if (!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }

    snprintf(str, sizeof(str), "%d", value);
    return Py_BuildValue("s", str);
}

// `atoi` function
static PyObject* py_atoi(PyObject* self, PyObject* args) {
    const char *str;

    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }

    int result = atoi(str);
    return Py_BuildValue("i", result);
}

// `abs` function
static PyObject* py_abs(PyObject* self, PyObject* args) {
    int value;

    if (!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }

    return Py_BuildValue("i", abs(value));
}

// `gcd` function (Greatest Common Divisor)
static int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

static PyObject* py_gcd(PyObject* self, PyObject* args) {
    int a, b;

    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    int result = gcd(a, b);
    return Py_BuildValue("i", result);
}

// Log message instead of stdout on Android
static PyObject* py_display_message(PyObject* self, PyObject* args) {
    const char *message;

    if (!PyArg_ParseTuple(args, "s", &message)) {
        return NULL;
    }

    LOGI("%s", message);
    Py_RETURN_NONE;
}

static PyMethodDef AdamLibraryMethods[] = {
    {"memset", py_memset, METH_VARARGS, "Memory set"},
    {"memcmp", py_memcmp, METH_VARARGS, "Memory compare"},
    {"itoa", py_itoa, METH_VARARGS, "Integer to string"},
    {"atoi", py_atoi, METH_VARARGS, "String to integer"},
    {"abs", py_abs, METH_VARARGS, "Absolute value"},
    {"gcd", py_gcd, METH_VARARGS, "Greatest common divisor"},
    {"display_message", py_display_message, METH_VARARGS, "Print or log a message"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef AdamLibraryModule = {
    PyModuleDef_HEAD_INIT,
    "datasetname",
    "Low-level Android-safe C functions",
    -1,
    AdamLibraryMethods
};

PyMODINIT_FUNC PyInit_datasetname(void) {
    return PyModule_Create(&AdamLibraryModule);
}
