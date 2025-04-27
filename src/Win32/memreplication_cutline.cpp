#include <Python.h>
#include <cstring>
#include <cstdlib>
#include <malloc.h>
#include <memory.h>  // Windows için uygun bellek yönetim kütüphanesi
#include <windows.h> // Windows'a özel API'ler için

// memchr - Hafızada bir byte'ı bulma
static PyObject* py_memchr(PyObject* self, PyObject* args) {
    const char* memory;
    int c;
    size_t size;
    if (!PyArg_ParseTuple(args, "sni", &memory, &size, &c)) {
        return NULL;
    }
    void* result = memchr(memory, c, size);
    if (result) {
        return Py_BuildValue("s", (char*)result);
    } else {
        Py_RETURN_NONE;
    }
}

// memset_s - Güvenli hafıza ayarı
static PyObject* py_memset_s(PyObject* self, PyObject* args) {
    char* memory;
    int value;
    size_t size;
    if (!PyArg_ParseTuple(args, "sni", &memory, &size, &value)) {
        return NULL;
    }
    int result = memset_s(memory, size, value, size);
    if (result == 0) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

// malloc_usable_size - Ayrılmış bir hafıza bloğunun boyutunu belirleme
static PyObject* py_malloc_usable_size(PyObject* self, PyObject* args) {
    void* ptr;
    if (!PyArg_ParseTuple(args, "O", &ptr)) {
        return NULL;
    }
    size_t size = _msize(ptr);  // Windows'ta _msize kullanılır
    return Py_BuildValue("n", size);
}

// aligned_alloc - Belirli bir hizalama ile hafıza ayırma
static PyObject* py_aligned_alloc(PyObject* self, PyObject* args) {
    size_t alignment;
    size_t size;
    if (!PyArg_ParseTuple(args, "nn", &alignment, &size)) {
        return NULL;
    }
    void* ptr = _aligned_malloc(size, alignment);  // Windows'ta _aligned_malloc kullanılır
    if (ptr == NULL) {
        Py_RETURN_NONE;
    }
    return Py_BuildValue("O", ptr);
}

// valloc - Sayfa hizalı hafıza ayırma
static PyObject* py_valloc(PyObject* self, PyObject* args) {
    size_t size;
    if (!PyArg_ParseTuple(args, "n", &size)) {
        return NULL;
    }
    void* ptr = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);  // Windows API
    if (ptr == NULL) {
        Py_RETURN_NONE;
    }
    return Py_BuildValue("O", ptr);
}

// posix_memalign - Belirli bir hizalamayla hafıza ayırma
static PyObject* py_posix_memalign(PyObject* self, PyObject* args) {
    size_t alignment;
    size_t size;
    void* ptr;
    if (!PyArg_ParseTuple(args, "nn", &alignment, &size)) {
        return NULL;
    }
    int result = _aligned_malloc(&ptr, alignment, size);  // Windows'ta _aligned_malloc kullanılabilir
    if (result != 0) {
        PyErr_SetString(PyExc_MemoryError, "Memory allocation failed");
        return NULL;
    }
    return Py_BuildValue("O", ptr);
}

// Python modülü ve fonksiyonları
static PyMethodDef memreplication_methods[] = {
    {"memchr", py_memchr, METH_VARARGS, "Find byte in memory"},
    {"memset_s", py_memset_s, METH_VARARGS, "Securely set memory"},
    {"malloc_usable_size", py_malloc_usable_size, METH_VARARGS, "Get the usable size of a memory block"},
    {"aligned_alloc", py_aligned_alloc, METH_VARARGS, "Allocate memory with alignment"},
    {"valloc", py_valloc, METH_VARARGS, "Allocate page-aligned memory"},
    {"posix_memalign", py_posix_memalign, METH_VARARGS, "Allocate memory with posix alignment"},
    {NULL, NULL, 0, NULL} // End marker
};

// Modül Tanımı
static struct PyModuleDef memreplicationmodule = {
    PyModuleDef_HEAD_INIT,
    "memreplication", // Modül ismi
    "A module for memory management functions", // Modül açıklaması
    -1, // Modül state'i, -1 global
    memreplication_methods // Fonksiyonlar dizisi
};

// Modül başlatma fonksiyonu
PyMODINIT_FUNC PyInit_memreplication(void) {
    return PyModule_Create(&memreplicationmodule);
}
