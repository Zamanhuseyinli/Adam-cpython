#include <Python.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <malloc.h>
#include <stdio.h>

// Simple GCD implementation
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// calloc işlevini Python'a getirme
static PyObject* py_calloc(PyObject* self, PyObject* args) {
    Py_ssize_t count, size;
    if (!PyArg_ParseTuple(args, "nn", &count, &size)) {
        return NULL;
    }
    void* ptr = calloc(count, size);
    if (ptr == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    PyObject* result = PyLong_FromVoidPtr(ptr);
    return result;
}

// realloc işlevini Python'a getirme
static PyObject* py_realloc(PyObject* self, PyObject* args) {
    PyObject* obj;
    Py_ssize_t new_size;
    if (!PyArg_ParseTuple(args, "On", &obj, &new_size)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    ptr = realloc(ptr, new_size);
    if (ptr == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    PyObject* result = PyLong_FromVoidPtr(ptr);
    return result;
}

// free işlevini Python'a getirme
static PyObject* py_free(PyObject* self, PyObject* args) {
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    free(ptr);
    Py_RETURN_NONE;
}

// bsearch işlevini Python'a getirme
static int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

static PyObject* py_bsearch(PyObject* self, PyObject* args) {
    PyObject* key_obj;
    PyObject* base_list;
    Py_ssize_t nmemb, size;
    if (!PyArg_ParseTuple(args, "OOnn", &key_obj, &base_list, &nmemb, &size)) {
        return NULL;
    }

    int key = (int)PyLong_AsLong(key_obj);
    int* base = (int*)malloc(nmemb * size);
    if (base == NULL) {
        PyErr_NoMemory();
        return NULL;
    }

    PyObject* item;
    for (Py_ssize_t i = 0; i < nmemb; i++) {
        item = PyList_GetItem(base_list, i);
        base[i] = (int)PyLong_AsLong(item);
    }

    int* result = (int*)bsearch(&key, base, nmemb, size, compare);
    free(base);

    if (result == NULL) {
        Py_RETURN_NONE;
    }
    return PyLong_FromLong(*result);
}

// qsort işlevini Python'a getirme
static int compare_qsort(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

static PyObject* py_qsort(PyObject* self, PyObject* args) {
    PyObject* base_list;
    Py_ssize_t nmemb, size;
    if (!PyArg_ParseTuple(args, "Onn", &base_list, &nmemb, &size)) {
        return NULL;
    }

    int* base = (int*)malloc(nmemb * size);
    if (base == NULL) {
        PyErr_NoMemory();
        return NULL;
    }

    PyObject* item;
    for (Py_ssize_t i = 0; i < nmemb; i++) {
        item = PyList_GetItem(base_list, i);
        base[i] = (int)PyLong_AsLong(item);
    }

    qsort(base, nmemb, size, compare_qsort);
    PyObject* result_list = PyList_New(nmemb);
    for (Py_ssize_t i = 0; i < nmemb; i++) {
        PyList_SetItem(result_list, i, PyLong_FromLong(base[i]));
    }
    free(base);
    return result_list;
}

// memcpy işlevini Python'a getirme
static PyObject* py_memcpy(PyObject* self, PyObject* args) {
    PyObject* dest_obj;
    PyObject* src_obj;
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "OOn", &dest_obj, &src_obj, &n)) {
        return NULL;
    }
    void* dest = PyBytes_AsString(dest_obj);
    const void* src = PyBytes_AsString(src_obj);
    memcpy(dest, src, n);
    Py_RETURN_NONE;
}

// strtok işlevini Python'a getirme
static PyObject* py_strtok(PyObject* self, PyObject* args) {
    const char* str;
    const char* delimiters;
    char* token;
    static char* saved_ptr = NULL;

    if (!PyArg_ParseTuple(args, "ss", &str, &delimiters)) {
        return NULL;
    }

    if (str != NULL) {
        saved_ptr = (char*)str;
    }

    token = strtok(saved_ptr, delimiters);  // strtok_r yerine strtok kullanıldı
    if (token == NULL) {
        Py_RETURN_NONE;
    }

    return PyUnicode_FromString(token);
}

// asctime işlevini Python'a getirme
static PyObject* py_asctime(PyObject* self, PyObject* args) {
    struct tm tm;
    if (!PyArg_ParseTuple(args, "iiiiii", &tm.tm_sec, &tm.tm_min, &tm.tm_hour,
                          &tm.tm_mday, &tm.tm_mon, &tm.tm_year)) {
        return NULL;
    }
    tm.tm_year += 1900;
    tm.tm_mon += 1;
    char* result = asctime(&tm);
    return PyUnicode_FromString(result);
}

// localtime işlevini Python'a getirme
static PyObject* py_localtime(PyObject* self, PyObject* args) {
    time_t rawtime;
    struct tm* timeinfo;
    if (!PyArg_ParseTuple(args, "l", &rawtime)) {
        return NULL;
    }
    timeinfo = localtime(&rawtime);
    return Py_BuildValue("iiiiii", timeinfo->tm_sec, timeinfo->tm_min, timeinfo->tm_hour,
                         timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year);
}

// GetTickCount64 işlevi Linux için
static PyObject* py_gettickcount64(PyObject* self, PyObject* args) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // CLOCK_MONOTONIC kullanıldı
    unsigned long long result = (unsigned long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return PyLong_FromUnsignedLongLong(result);
}

// VirtualAlloc işlevi Linux için
static PyObject* py_virtualalloc(PyObject* self, PyObject* args) {
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "n", &size)) {
        return NULL;
    }
    void* ptr = malloc(size);
    if (ptr == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// VirtualFree işlevi Linux için
static PyObject* py_virtualfree(PyObject* self, PyObject* args) {
    void* ptr;
    if (!PyArg_ParseTuple(args, "O", &ptr)) {
        return NULL;
    }
    free(ptr);
    Py_RETURN_NONE;
}

// HeapAlloc işlevi Linux için
static PyObject* py_heapalloc(PyObject* self, PyObject* args) {
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "n", &size)) {
        return NULL;
    }
    void* ptr = malloc(size);
    if (ptr == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// HeapFree işlevi Linux için
static PyObject* py_heapfree(PyObject* self, PyObject* args) {
    void* ptr;
    if (!PyArg_ParseTuple(args, "O", &ptr)) {
        return NULL;
    }
    free(ptr);
    Py_RETURN_NONE;
}

// VirtualQuery işlevi Linux için
static PyObject* py_virtualquery(PyObject* self, PyObject* args) {
    void* ptr;
    if (!PyArg_ParseTuple(args, "O", &ptr)) {
        return NULL;
    }
    return Py_BuildValue("n", 0);  // Linux'ta doğrudan karşılık yok
}

// LocalAlloc işlevi Linux için
static PyObject* py_localalloc(PyObject* self, PyObject* args) {
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "n", &size)) {
        return NULL;
    }
    void* ptr = malloc(size);
    if (ptr == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// LocalFree işlevi Linux için
static PyObject* py_localfree(PyObject* self, PyObject* args) {
    void* ptr;
    if (!PyArg_ParseTuple(args, "O", &ptr)) {
        return NULL;
    }
    free(ptr);
    Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
    {"calloc", py_calloc, METH_VARARGS, "Allocate memory"},
    {"realloc", py_realloc, METH_VARARGS, "Reallocate memory"},
    {"free", py_free, METH_VARARGS, "Free allocated memory"},
    {"bsearch", py_bsearch, METH_VARARGS, "Binary search"},
    {"qsort", py_qsort, METH_VARARGS, "Quick sort"},
    {"memcpy", py_memcpy, METH_VARARGS, "Copy memory"},
    {"strtok", py_strtok, METH_VARARGS, "Tokenize string"},
    {"asctime", py_asctime, METH_VARARGS, "Convert tm to string"},
    {"localtime", py_localtime, METH_VARARGS, "Local time"},
    {"gettickcount64", py_gettickcount64, METH_NOARGS, "Get tick count"},
    {"virtualalloc", py_virtualalloc, METH_VARARGS, "Allocate virtual memory"},
    {"virtualfree", py_virtualfree, METH_VARARGS, "Free virtual memory"},
    {"heapalloc", py_heapalloc, METH_VARARGS, "Allocate heap memory"},
    {"heapfree", py_heapfree, METH_VARARGS, "Free heap memory"},
    {"virtualquery", py_virtualquery, METH_VARARGS, "Query memory"},
    {"localalloc", py_localalloc, METH_VARARGS, "Allocate local memory"},
    {"localfree", py_localfree, METH_VARARGS, "Free local memory"},
    {NULL, NULL, 0, NULL} // End marker
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "adam", // Modül adı "adam" olarak değiştirildi
    "A module for memory operations.", // Modül açıklaması
    -1, // Global veri alanı
    methods // Fonksiyonlar
};

PyMODINIT_FUNC PyInit_adam(void) {
    return PyModule_Create(&module);
}
