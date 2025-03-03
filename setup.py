from setuptools import setup
import os
import platform

# Read the content of README.md
with open(os.path.join(os.path.dirname(__file__), 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

# Setup function with updated configuration
setup(
    name='adamlibrary',
    version='1.5.1',
    description='Improved library for Python to support C library extensions',
    long_description=long_description,
    long_description_content_type='text/markdown',
    # ext_modules=extensions,  # Comment out or remove this line
    packages=['adamlibrary'],  # Package name
    include_package_data=True,
    install_requires=[  # Required Python libraries
        'cython',
    ],
    classifiers=[
        'Programming Language :: Python :: 3',
        'Programming Language :: C',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',  # Minimum Python version
)
