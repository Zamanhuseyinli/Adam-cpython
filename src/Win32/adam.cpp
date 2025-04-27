#include <Python.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

// Simple GCD implementation
static int gcd(int a, int b) noexcept {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// calloc işlevini Python'a getirme
static PyObject* py_calloc(PyObject* self, PyObject* args) {
    (void)self;
    Py_ssize_t count, size;
    if (!PyArg_ParseTuple(args, "nn", &count, &size)) {
        return NULL;
    }
    void* ptr = calloc((size_t)count, (size_t)size);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// realloc işlevini Python'a getirme
static PyObject* py_realloc(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* obj;
    Py_ssize_t new_size;
    if (!PyArg_ParseTuple(args, "On", &obj, &new_size)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    ptr = realloc(ptr, (size_t)new_size);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// free işlevini Python'a getirme
static PyObject* py_free(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    free(ptr);
    Py_RETURN_NONE;
}

// bsearch işlevi
static int compare(const void* a, const void* b) noexcept {
    return (*(const int*)a - *(const int*)b);
}

static PyObject* py_bsearch(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* key_obj;
    PyObject* base_list;
    Py_ssize_t nmemb, size;
    if (!PyArg_ParseTuple(args, "OOnn", &key_obj, &base_list, &nmemb, &size)) {
        return NULL;
    }

    int key = (int)PyLong_AsLong(key_obj);
    int* base = (int*)malloc((size_t)(nmemb * size));
    if (!base) {
        PyErr_NoMemory();
        return NULL;
    }

    for (Py_ssize_t i = 0; i < nmemb; i++) {
        PyObject* item = PyList_GetItem(base_list, i);
        base[i] = (int)PyLong_AsLong(item);
    }

    int* result = (int*)bsearch(&key, base, (size_t)nmemb, (size_t)size, compare);
    PyObject* py_result = (result) ? PyLong_FromLong(*result) : Py_None;
    if (py_result == Py_None) Py_INCREF(Py_None);

    free(base);
    return py_result;
}

// qsort işlevi
static int compare_qsort(const void* a, const void* b) noexcept {
    return (*(const int*)a - *(const int*)b);
}

static PyObject* py_qsort(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* base_list;
    Py_ssize_t nmemb, size;
    if (!PyArg_ParseTuple(args, "Onn", &base_list, &nmemb, &size)) {
        return NULL;
    }

    int* base = (int*)malloc((size_t)(nmemb * size));
    if (!base) {
        PyErr_NoMemory();
        return NULL;
    }

    for (Py_ssize_t i = 0; i < nmemb; i++) {
        PyObject* item = PyList_GetItem(base_list, i);
        base[i] = (int)PyLong_AsLong(item);
    }

    qsort(base, (size_t)nmemb, (size_t)size, compare_qsort);
    PyObject* result_list = PyList_New(nmemb);
    for (Py_ssize_t i = 0; i < nmemb; i++) {
        PyList_SetItem(result_list, i, PyLong_FromLong(base[i]));
    }
    free(base);
    return result_list;
}

// memcpy işlevi
static PyObject* py_memcpy(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* dest_obj, * src_obj;
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "OOn", &dest_obj, &src_obj, &n)) {
        return NULL;
    }
    void* dest = PyBytes_AsString(dest_obj);
    const void* src = PyBytes_AsString(src_obj);
    memcpy(dest, src, (size_t)n);
    Py_RETURN_NONE;
}

// strtok işlevi
static PyObject* py_strtok(PyObject* self, PyObject* args) {
    (void)self;
    const char* str;
    const char* delimiters;
    static char* saved_ptr = NULL;

    if (!PyArg_ParseTuple(args, "ss", &str, &delimiters)) {
        return NULL;
    }

    if (str != NULL) {
        saved_ptr = (char*)str;
    }

    char* token = strtok(saved_ptr, delimiters);
    if (!token) {
        Py_RETURN_NONE;
    }

    return PyUnicode_FromString(token);
}

// asctime işlevi
static PyObject* py_asctime(PyObject* self, PyObject* args) {
    (void)self;
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

// localtime işlevi
static PyObject* py_localtime(PyObject* self, PyObject* args) {
    (void)self;
    time_t rawtime;
    if (!PyArg_ParseTuple(args, "l", &rawtime)) {
        return NULL;
    }
    struct tm* timeinfo = localtime(&rawtime);
    return Py_BuildValue("iiiiii", timeinfo->tm_sec, timeinfo->tm_min, timeinfo->tm_hour,
                         timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year);
}

// GetTickCount64 işlevi
static PyObject* py_gettickcount64(PyObject* self, PyObject* args) {
    (void)self;
    (void)args;
    return PyLong_FromUnsignedLongLong(GetTickCount64());
}

// VirtualAlloc işlevi
static PyObject* py_virtualalloc(PyObject* self, PyObject* args) {
    (void)self;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "n", &size)) {
        return NULL;
    }
    void* ptr = VirtualAlloc(NULL, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// VirtualFree işlevi
static PyObject* py_virtualfree(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    VirtualFree(ptr, 0, MEM_RELEASE);
    Py_RETURN_NONE;
}

// HeapAlloc işlevi
static PyObject* py_heapalloc(PyObject* self, PyObject* args) {
    (void)self;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "n", &size)) {
        return NULL;
    }
    void* ptr = HeapAlloc(GetProcessHeap(), 0, (SIZE_T)size);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// HeapFree işlevi
static PyObject* py_heapfree(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    HeapFree(GetProcessHeap(), 0, ptr);
    Py_RETURN_NONE;
}

// VirtualQuery işlevi
static PyObject* py_virtualquery(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(ptr, &mbi, sizeof(mbi));
    return Py_BuildValue("nnn", (Py_ssize_t)mbi.State, (Py_ssize_t)mbi.Protect, (Py_ssize_t)mbi.Type);
}

// LocalAlloc işlevi
static PyObject* py_localalloc(PyObject* self, PyObject* args) {
    (void)self;
    Py_ssize_t size;
    if (!PyArg_ParseTuple(args, "n", &size)) {
        return NULL;
    }
    void* ptr = LocalAlloc(LPTR, (SIZE_T)size);
    if (!ptr) {
        PyErr_NoMemory();
        return NULL;
    }
    return PyLong_FromVoidPtr(ptr);
}

// LocalFree işlevi
static PyObject* py_localfree(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* obj;
    if (!PyArg_ParseTuple(args, "O", &obj)) {
        return NULL;
    }
    void* ptr = PyLong_AsVoidPtr(obj);
    LocalFree(ptr);
    Py_RETURN_NONE;
}

// Metod tanımlamaları
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
    {NULL, NULL, 0, NULL}  // End marker
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "adam",
    "A module for memory operations.",
    -1,
    methods
};

PyMODINIT_FUNC PyInit_adam(void) {
    return PyModule_Create(&module);
}
