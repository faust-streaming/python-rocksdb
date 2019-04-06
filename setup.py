#! /usr/bin/env python3

import os
import re
import sys
import sysconfig
import platform
import subprocess

from distutils.version import LooseVersion
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from setuptools.command.test import test as TestCommand
from shutil import copyfile, copymode
import glob


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)',
                                         out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]
        #  build_args = []

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                cfg.upper(),
                extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            #  cmake_args = []
            build_args += ['--', '-j4']

        #  env = os.environ.copy()
        #  env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            #  env.get('CXXFLAGS', ''),
            #  self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        #  subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              #  cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp)
        print (cmake_args)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                              cwd=self.build_temp)
        # Copy *_test file to tests directory
        #  test_bin = os.path.join(self.build_temp, 'python_cpp_example_test')
        #  self.copy_test_file(test_bin)
        #  print()  # Add an empty line for cleaner output
        build_dir = os.path.join(self.build_lib, 'pyrocksdb')

        libs = glob.glob(os.path.join(build_dir, '*.so'))
        dst = 'tests'
        for lib in libs:
            copyfile(lib, os.path.join(dst, os.path.basename(lib)))
            copymode(lib, os.path.join(dst, os.path.basename(lib)))



    def copy_test_file(self, src_file):
        '''
        Copy ``src_file`` to ``dest_file`` ensuring parent directory exists.
        By default, message like `creating directory /path/to/package` and
        `copying directory /src/path/to/package -> path/to/package` are displayed on standard output. Adapted from scikit-build.
        '''
        # Create directory if needed
        dest_dir = os.path.join(os.path.dirname(
            os.path.abspath(__file__)), 'tests', 'bin')
        if dest_dir != "" and not os.path.exists(dest_dir):
            print("creating directory {}".format(dest_dir))
            os.makedirs(dest_dir)

        # Copy file
        dest_file = os.path.join(dest_dir, os.path.basename(src_file))
        print("copying {} -> {}".format(src_file, dest_file))
        copyfile(src_file, dest_file)
        copymode(src_file, dest_file)

setup(
    name='python-rocksdb',
    version='1.0',
    author='Ming Hsuan Tu',
    author_email='qrnnis2623891@gmail.com',
    description='python bindings to rocksdb',
    long_description='',
    packages=find_packages('src'),
    package_dir={'':'src'},

    ext_modules=[CMakeExtension('pyrocksdb/pyrocksdb')],
    cmdclass=dict(build_ext=CMakeBuild),
    tests_require=['pytest'],
    setup_requires=['pytest-runner'],
    #  test_suite='tests',
    zip_safe=False,
)
