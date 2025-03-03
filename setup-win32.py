import os
import platform
from setuptools import setup, Extension

# Windows derleyici ayarları
if platform.system() == 'Windows':
    # MSVC için yol ayarları
    os.environ['CC'] = 'cl'  # MSVC compiler
    os.environ['CXX'] = 'cl'  # MSVC compiler
    os.environ['LDSHARED'] = 'link'  # Linker

    # Ek kütüphaneleri belirtmek
    libraries = ['user32']  # user32.lib ile MessageBoxA'yı ekle

extensions = [
    Extension(
        'adamlibrary.adam',  # Modül adı
        sources=['src/Win32/adam.cpp'],  # Windows için kaynak dosya
        libraries=libraries,  # Kütüphaneler
    ),
    Extension(
        'adamlibrary.datasetname',  # Modül adı
        sources=['src/Win32/datasetname.cpp'],  # Windows için kaynak dosya
        libraries=libraries,  # Kütüphaneler
    ),
]

setup(
    name='adamlibrary',
    version='1.5',
    ext_modules=extensions,
)
