#!/usr/bin/env python3

import os
import platform
import sys

import pkgconfig
from Cython.Build import cythonize
from setuptools import Extension, setup, find_packages

extra_compile_args = [
    '-std=c++11',
    '-O3',
    '-Wall',
    '-Wextra',
    '-Wconversion',
    '-fno-strict-aliasing',
    '-fno-rtti',
]

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

if platform.system() == 'Darwin':
    extra_compile_args += ['-mmacosx-version-min=10.9', '-stdlib=libc++']

if platform.system() == 'Windows':
    extra_compile_args.remove('-Wextra')
    extra_compile_args.remove('-Wconversion')
    ext_args['libraries'].remove('z')
    ext_args['libraries'].append('zlib')

rocksdb_extension = Extension(
    'rocksdb._rocksdb',
    [
        os.path.join('rocksdb','_rocksdb.pyx'),
    ],
    extra_compile_args=extra_compile_args,
    language='c++',
    **ext_args,
)

setup(
    name="faust-streaming-rocksdb",
    use_scm_version=True,
    description="Python bindings for RocksDB, primarily for use with Faust",
    keywords='rocksdb',
    author='William Barnhart',
    author_email="williambbarnhart@gmail.com",
    url="https://github.com/faust-streaming/python-rocksdb",
    license='BSD License',
    setup_requires=['setuptools>=25', 'Cython>=0.20', 'setuptools_scm'],
    install_requires=['setuptools>=25'],
    package_dir={'rocksdb': 'rocksdb'},
    packages=find_packages('.'),
    ext_modules=cythonize([rocksdb_extension]),
)
