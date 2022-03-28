#!/usr/bin/env python3

import os
import platform
import sys

import pkgconfig
from Cython.Build import cythonize
from setuptools import Extension, setup

extra_compile_args = [
    '-std=c++11',
    '-O3',
    '-Wall',
    '-Wextra',
    '-Wconversion',
    '-fno-strict-aliasing',
    '-fno-rtti',
]

if platform.system() == 'Darwin':
    extra_compile_args += ['-mmacosx-version-min=10.7', '-stdlib=libc++']

if sys.version_info < (3 , 0):
    raise Exception('python-rocksdb requires Python 3.x')

try:
    ext_args = pkgconfig.parse('rocksdb')
except pkgconfig.PackageNotFoundError:
    include_path = os.environ.get('INCLUDE_PATH')
    library_path = os.environ.get('LIBRARY_PATH')

    ext_args = {
        'include_dirs': include_path.split(os.pathsep) if include_path else [],
        'library_dirs': library_path.split(os.pathsep) if library_path else [],
        'libraries': ['rocksdb', 'snappy', 'bz2', 'z', 'lz4'],
    }

rocksdb_extension = Extension(
    'rocksdb._rocksdb',
    [
        'rocksdb/_rocksdb.pyx',
    ],
    extra_compile_args=extra_compile_args,
    language='c++',
    **ext_args,
)

setup(
    ext_modules=cythonize([rocksdb_extension]),
)
