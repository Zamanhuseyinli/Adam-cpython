#include <Python.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

// `memset` function
static PyObject* py_memset(PyObject* self, PyObject* args) {
    void *ptr;
    int value;
    int size;

    if (!PyArg_ParseTuple(args, "sii", &ptr, &value, &size)) {
        return NULL;
    }
    
    memset(ptr, value, size);
    Py_RETURN_NONE;
}

// `memcmp` function
static PyObject* py_memcmp(PyObject* self, PyObject* args) {
    const char *ptr1, *ptr2;
    int n;

    if (!PyArg_ParseTuple(args, "ssi", &ptr1, &ptr2, &n)) {
        return NULL;
    }

    int result = memcmp(ptr1, ptr2, n);
    return Py_BuildValue("i", result);
}

// `strncpy` function
static PyObject* py_strncpy(PyObject* self, PyObject* args) {
    char *dest;
    const char *src;
    int num;

    if (!PyArg_ParseTuple(args, "ssi", &dest, &src, &num)) {
        return NULL;
    }

    strncpy(dest, src, num);
    return Py_BuildValue("s", dest);
}

// `memmove` function
static PyObject* py_memmove(PyObject* self, PyObject* args) {
    void *dest, *src;
    int size;

    if (!PyArg_ParseTuple(args, "ssi", &dest, &src, &size)) {
        return NULL;
    }

    memmove(dest, src, size);
    Py_RETURN_NONE;
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

// `sprintf` function
static PyObject* py_sprintf(PyObject* self, PyObject* args) {
    const char *format;
    char buffer[1024];

    if (!PyArg_ParseTuple(args, "s", &format)) {
        return NULL;
    }

    snprintf(buffer, sizeof(buffer), format);
    return Py_BuildValue("s", buffer);
}

// `strchr` function
static PyObject* py_strchr(PyObject* self, PyObject* args) {
    const char *str;
    int c;

    if (!PyArg_ParseTuple(args, "si", &str, &c)) {
        return NULL;
    }

    const char *result = strchr(str, c);
    if (result) {
        return Py_BuildValue("s", result);
    } else {
        Py_RETURN_NONE;
    }
}

// `strrchr` function
static PyObject* py_strrchr(PyObject* self, PyObject* args) {
    const char *str;
    int c;

    if (!PyArg_ParseTuple(args, "si", &str, &c)) {
        return NULL;
    }

    const char *result = strrchr(str, c);
    if (result) {
        return Py_BuildValue("s", result);
    } else {
        Py_RETURN_NONE;
    }
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

// Windows API Usage - Example Function: MessageBox Display
static PyObject* py_message_box(PyObject* self, PyObject* args) {
    const char *message;
    const char *title;

    if (!PyArg_ParseTuple(args, "ss", &message, &title)) {
        return NULL;
    }

    MessageBoxA(NULL, message, title, MB_OK);
    Py_RETURN_NONE;
}

// Python Module Definition
static PyMethodDef AdamLibraryMethods[] = {
    {"memset", py_memset, METH_VARARGS, "Allocate memory with memset"},
    {"memcmp", py_memcmp, METH_VARARGS, "Memory comparison"},
    {"strncpy", py_strncpy, METH_VARARGS, "Copy string with strncpy"},
    {"memmove", py_memmove, METH_VARARGS, "Memory move"},
    {"itoa", py_itoa, METH_VARARGS, "Convert integer to string"},
    {"sprintf", py_sprintf, METH_VARARGS, "Formatted string output"},
    {"strchr", py_strchr, METH_VARARGS, "Find character in string"},
    {"strrchr", py_strrchr, METH_VARARGS, "Find last occurrence of character in string"},
    {"atoi", py_atoi, METH_VARARGS, "Convert string to integer"},
    {"abs", py_abs, METH_VARARGS, "Get absolute value"},
    {"gcd", py_gcd, METH_VARARGS, "Compute greatest common divisor"},
    {"message_box", py_message_box, METH_VARARGS, "Show a message box"},
    {NULL, NULL, 0, NULL}
};

// Module Initialization
static struct PyModuleDef AdamLibraryModule = {
    PyModuleDef_HEAD_INIT,
    "datasetname",   // Module name
    "Low-level C functions for memory and string operations", // Module description
    -1,
    AdamLibraryMethods
};

PyMODINIT_FUNC PyInit_datasetname(void) {
    return PyModule_Create(&AdamLibraryModule);
}
