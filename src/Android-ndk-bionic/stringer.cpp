#include <Python.h>
#include <cstring>
#include <cstdlib>

// String Kopyalama (strdup)
static PyObject* py_strdup(PyObject* self, PyObject* args) {
    const char* input_string;
    if (!PyArg_ParseTuple(args, "s", &input_string)) {
        return NULL;
    }
    char* copied_string = strdup(input_string);
    if (!copied_string) return PyErr_NoMemory();
    PyObject* result = Py_BuildValue("s", copied_string);
    free(copied_string);
    return result;
}

// String Birleştirme (strncat)
static PyObject* py_strncat(PyObject* self, PyObject* args) {
    const char* str1;
    const char* str2;
    int num;
    if (!PyArg_ParseTuple(args, "ssi", &str1, &str2, &num)) {
        return NULL;
    }
    size_t len = strlen(str1) + num + 1;
    char* result = (char*)malloc(len);
    if (!result) return PyErr_NoMemory();
    strcpy(result, str1);
    strncat(result, str2, num);
    PyObject* result_obj = Py_BuildValue("s", result);
    free(result);
    return result_obj;
}

// String Karşılaştırma (strncmp)
static PyObject* py_strncmp(PyObject* self, PyObject* args) {
    const char* str1;
    const char* str2;
    int num;
    if (!PyArg_ParseTuple(args, "ssi", &str1, &str2, &num)) {
        return NULL;
    }
    int cmp_result = strncmp(str1, str2, num);
    return Py_BuildValue("i", cmp_result);
}

// Python modül metod tanımı
static PyMethodDef StringerMethods[] = {
    {"strdup", py_strdup, METH_VARARGS, "String kopyalama"},
    {"strncat", py_strncat, METH_VARARGS, "String birleştirme"},
    {"strncmp", py_strncmp, METH_VARARGS, "String karşılaştırma"},
    {NULL, NULL, 0, NULL}
};

// Python modül tanımı
static struct PyModuleDef stringer_module = {
    PyModuleDef_HEAD_INIT,
    "stringer",
    "String işlemleri modülü",
    -1,
    StringerMethods
};

// Modül başlatma
PyMODINIT_FUNC PyInit_stringer(void) {
    return PyModule_Create(&stringer_module);
}
