#include <Python.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

// `memset` function
static PyObject* py_memset(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    PyObject *buffer_obj;
    int value;
    Py_ssize_t size;

    if (!PyArg_ParseTuple(args, "Oin", &buffer_obj, &value, &size)) {
        return NULL;
    }

    char *buffer = PyByteArray_AsString(buffer_obj);
    if (!buffer) {
        PyErr_SetString(PyExc_TypeError, "Expected a bytearray object.");
        return NULL;
    }

    memset(buffer, value, (size_t)size);  // explicit cast to size_t
    Py_RETURN_NONE;
}

// `memcmp` function
static PyObject* py_memcmp(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    Py_buffer buf1, buf2;
    int n;

    if (!PyArg_ParseTuple(args, "y*y*i", &buf1, &buf2, &n)) {
        return NULL;
    }

    int result = memcmp(buf1.buf, buf2.buf, n);

    PyBuffer_Release(&buf1);
    PyBuffer_Release(&buf2);

    return Py_BuildValue("i", result);
}

// `strncpy` function
static PyObject* py_strncpy(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    char dest[1024];
    const char *src;
    int num;

    if (!PyArg_ParseTuple(args, "si", &src, &num)) {
        return NULL;
    }

    strncpy(dest, src, num);
    dest[num] = '\0';  // Sonlandırıcı ekle

    return Py_BuildValue("s", dest);
}

// `memmove` function
static PyObject* py_memmove(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    PyObject *dest_obj, *src_obj;
    Py_ssize_t size;

    if (!PyArg_ParseTuple(args, "OOn", &dest_obj, &src_obj, &size)) {
        return NULL;
    }

    char *dest = PyByteArray_AsString(dest_obj);
    char *src = PyByteArray_AsString(src_obj);

    if (!dest || !src) {
        PyErr_SetString(PyExc_TypeError, "Expected bytearray objects.");
        return NULL;
    }

    memmove(dest, src, (size_t)size);  // explicit cast to size_t
    Py_RETURN_NONE;
}

// `itoa` function
static PyObject* py_itoa(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    int value;
    char str[32];

    if (!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }

    snprintf(str, sizeof(str), "%d", value);
    return Py_BuildValue("s", str);
}

// `sprintf` function
static PyObject* py_sprintf(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    const char *format;
    PyObject *args_tuple;
    char buffer[1024];

    if (!PyArg_ParseTuple(args, "sO", &format, &args_tuple)) {
        return NULL;
    }

    if (!PyTuple_Check(args_tuple)) {
        PyErr_SetString(PyExc_TypeError, "Second argument must be a tuple.");
        return NULL;
    }

    // Şu anda desteklemiyor, sadece formatı döndür
    snprintf(buffer, sizeof(buffer), "%s", format);
    return Py_BuildValue("s", buffer);
}

// `strchr` function
static PyObject* py_strchr(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    const char *str;
    int c;

    if (!PyArg_ParseTuple(args, "si", &str, &c)) {
        return NULL;
    }

    const char *result = strchr(str, c);
    if (result) {
        return PyUnicode_FromString(result);
    } else {
        Py_RETURN_NONE;
    }
}

// `strrchr` function
static PyObject* py_strrchr(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    const char *str;
    int c;

    if (!PyArg_ParseTuple(args, "si", &str, &c)) {
        return NULL;
    }

    const char *result = strrchr(str, c);
    if (result) {
        return PyUnicode_FromString(result);
    } else {
        Py_RETURN_NONE;
    }
}

// `atoi` function
static PyObject* py_atoi(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    const char *str;

    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }

    int result = atoi(str);
    return Py_BuildValue("i", result);
}

// `abs` function
static PyObject* py_abs(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    int value;

    if (!PyArg_ParseTuple(args, "i", &value)) {
        return NULL;
    }

    return PyLong_FromLong(abs(value));
}

// `gcd` function
static int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

static PyObject* py_gcd(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    int a, b;

    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    int result = gcd(a, b);
    return Py_BuildValue("i", result);
}

// MessageBoxA kullanımı
static PyObject* py_message_box(PyObject* self, PyObject* args) {
    Py_UNUSED(self);  // 'self' kullanılmıyor

    const char *message;
    const char *title;

    if (!PyArg_ParseTuple(args, "ss", &message, &title)) {
        return NULL;
    }

    MessageBoxA(NULL, message, title, MB_OK);
    Py_RETURN_NONE;
}

// Python Module Method Definitions
static PyMethodDef AdamLibraryMethods[] = {
    {"memset", py_memset, METH_VARARGS, "Fill memory with a constant byte"},
    {"memcmp", py_memcmp, METH_VARARGS, "Compare memory areas"},
    {"strncpy", py_strncpy, METH_VARARGS, "Copy string with specified limit"},
    {"memmove", py_memmove, METH_VARARGS, "Move memory areas"},
    {"itoa", py_itoa, METH_VARARGS, "Convert integer to string"},
    {"sprintf", py_sprintf, METH_VARARGS, "Format a string"},
    {"strchr", py_strchr, METH_VARARGS, "Locate first occurrence of character in string"},
    {"strrchr", py_strrchr, METH_VARARGS, "Locate last occurrence of character in string"},
    {"atoi", py_atoi, METH_VARARGS, "Convert string to integer"},
    {"abs", py_abs, METH_VARARGS, "Absolute value of integer"},
    {"gcd", py_gcd, METH_VARARGS, "Greatest common divisor"},
    {"message_box", py_message_box, METH_VARARGS, "Show a message box"},
    {NULL, NULL, 0, NULL}
};

// Module definition
static struct PyModuleDef AdamLibraryModule = {
    PyModuleDef_HEAD_INIT,
    "datasetname",   // Module name
    "Low-level C functions for memory and string operations.", // Description
    -1,
    AdamLibraryMethods
};

// Initialization function
PyMODINIT_FUNC PyInit_datasetname(void) {
    return PyModule_Create(&AdamLibraryModule);
}
