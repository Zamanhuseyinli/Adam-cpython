import sys
import platform
from setuptools import setup, Extension, find_packages
import os

# README.md dosyasını okuyoruz
with open(os.path.join(os.path.dirname(__file__), 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

# Derleme ve linkleme argümanları
extra_compile_args = ['-O2', '-Wall']
extra_link_args = []
sources = [
    'src/Unix-posix/adam.cpp',
    'src/Unix-posix/datasetname.cpp',
    'src/Unix-posix/stringer.cpp',
    'src/Unix-posix/memreplication_cutline.cpp',
    'src/Unix-posix/math.cpp'
]

# GitHub Actions ortamına göre platform tespiti
if platform.system() == 'Windows':
    sources = [
        'src/Win32/adam.cpp',
        'src/Win32/datasetname.cpp',
        'src/Win32/stringer.cpp',
        'src/Win32/memreplication_cutline.cpp',
        'src/Win32/math.cpp'
    ]
  

# Extension'lar (C uzantıları)
extensions = [
    Extension(
        'adamlibrary.adam',
        sources=[sources[0]],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
        include_dirs=include_dirs,
        library_dirs=library_dirs,
    ),
    Extension(
        'adamlibrary.datasetname',
        sources=[sources[1]],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
        include_dirs=include_dirs,
        library_dirs=library_dirs,
    ),
    Extension(
        'adamlibrary.stringer',
        sources=[sources[2]],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
        include_dirs=include_dirs,
        library_dirs=library_dirs,
    ),
    Extension(
        'adamlibrary.memreplication_cutline',
        sources=[sources[3]],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
        include_dirs=include_dirs,
        library_dirs=library_dirs,
    ),
    Extension(
        'adamlibrary.math',
        sources=[sources[4]],
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
        include_dirs=include_dirs,
        library_dirs=library_dirs,
    ),
]

# Setup fonksiyonu ile kurulumu yapıyoruz
setup(
    name='adamlibrary',
    version='1.6.1',
    description='Improved library for Python to support C library extensions',
    long_description=long_description,
    long_description_content_type='text/markdown',
    ext_modules=extensions,
    packages=find_packages(),
    install_requires=[
        'cython',
        'setuptools',
        'wheel',
        'auditwheel',
    ],
    classifiers=[
        'Programming Language :: Python :: 3',
        'Programming Language :: C',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
)
