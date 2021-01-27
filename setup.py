#!/usr/bin/env python3

import platform
import setuptools
import sys


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

setuptools.setup(
    ext_modules=[
        setuptools.Extension(
            'rocksdb._rocksdb',
            [
                'rocksdb/_rocksdb.pyx',
            ],
            extra_compile_args=extra_compile_args,
            language='c++',
            libraries=[
                'rocksdb',
                'snappy',
                'bz2',
                'z',
                'lz4',
            ],
        ),
    ],
)
