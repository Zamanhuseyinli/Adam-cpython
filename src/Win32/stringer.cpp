#include <Python.h>
#include <cstring>   // C++ için standart başlık dosyası
#include <cstdlib>   // malloc, free için
#include <windows.h> // Windows API için

// String Kopyalama (strdup)
static PyObject* py_strdup(PyObject* self, PyObject* args) {
    const char* input_string;
    if (!PyArg_ParseTuple(args, "s", &input_string)) {
        return NULL;
    }
    char* copied_string = _strdup(input_string); // Windows'ta _strdup kullanılır
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
    char* result = (char*)malloc(strlen(str1) + num + 1);
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

// Alt String Arama (strstr)
static PyObject* py_strstr(PyObject* self, PyObject* args) {
    const char* haystack;
    const char* needle;
    if (!PyArg_ParseTuple(args, "ss", &haystack, &needle)) {
        return NULL;
    }
    char* result = strstr(haystack, needle);
    if (result) {
        return Py_BuildValue("s", result);
    } else {
        Py_RETURN_NONE;
    }
}

// String Tokenization (strtok)
static PyObject* py_strtok(PyObject* self, PyObject* args) {
    const char* str;
    const char* delimiter;
    if (!PyArg_ParseTuple(args, "ss", &str, &delimiter)) {
        return NULL;
    }
    char* str_copy = _strdup(str);  // strtok kullanırken orijinal diziyi değiştirdiği için kopya alıyoruz
    char* token = strtok(str_copy, delimiter);
    if (token) {
        return Py_BuildValue("s", token);
    } else {
        Py_RETURN_NONE;
    }
    free(str_copy);
}

// String Uzunluğu (strlen)
static PyObject* py_strlen(PyObject* self, PyObject* args) {
    const char* str;
    if (!PyArg_ParseTuple(args, "s", &str)) {
        return NULL;
    }
    size_t length = strlen(str);
    return Py_BuildValue("n", length);
}

// Python modülü ve fonksiyonları
static PyMethodDef stringer_methods[] = {
    {"strdup", py_strdup, METH_VARARGS, "Duplicate a string"},
    {"strncat", py_strncat, METH_VARARGS, "Concatenate two strings"},
    {"strncmp", py_strncmp, METH_VARARGS, "Compare two strings"},
    {"strstr", py_strstr, METH_VARARGS, "Find substring in string"},
    {"strtok", py_strtok, METH_VARARGS, "Tokenize a string"},
    {"strlen", py_strlen, METH_VARARGS, "Get string length"},
    {NULL, NULL, 0, NULL} // End marker
};

// Modül Tanımı
static struct PyModuleDef stringermodule = {
    PyModuleDef_HEAD_INIT,
    "stringer", // Modül ismi
    "A module for string operations", // Modül açıklaması
    -1, // Modül state'i, -1 global
    stringer_methods // Fonksiyonlar dizisi
};

// Modül başlatma fonksiyonu
PyMODINIT_FUNC PyInit_stringer(void) {
    return PyModule_Create(&stringermodule);
}
