#!/usr/bin/env python3

import os
import platform
import sys

import pkgconfig
from Cython.Build import cythonize
from pkgconfig import PackageNotFoundError
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
    raise Exception("python-rocksdb requires Python 3.x")

rocksdb_extension = Extension(
    'rocksdb._rocksdb',
    [
        'rocksdb/_rocksdb.pyx',
    ],
    extra_compile_args=extra_compile_args,
    language='c++',
    libraries=['rocksdb'],
)

try:
    pkgconfig.configure_extension(rocksdb_extension, "rocksdb")
except PackageNotFoundError:
    include_path = os.environ.get("INCLUDE_PATH")
    library_path = os.environ.get("LIBRARY_PATH")

    rocksdb_extension.include_dirs += include_path.split(os.pathsep) if include_path else []
    rocksdb_extension.library_dirs += library_path.split(os.pathsep) if library_path else []
    rocksdb_extension.libraries += [
        'snappy',
        'bz2',
        'z',
        'lz4',
    ]

setup(
    ext_modules=cythonize([rocksdb_extension]),
)
