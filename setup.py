import platform
import setuptools


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


def _get_long_description():
    with open('README.rst') as readme_file:
        return readme_file.read()


setuptools.setup(
    name='rocksdb',
    version='0.8.0rc2',
    description='Python bindings for RocksDB',
    long_description=_get_long_description(),
    long_description_content_type='text/x-rst',
    author='Andrey Martyanov',
    author_email='andrey@martyanov.com',
    url='https://github.com/martyanov/python-rocksdb',
    license='BSD License',
    license_file='LICENSE',
    packages=setuptools.find_packages('.'),
    package_dir={
        'rocksdb': 'rocksdb',
    },
    include_package_data=True,
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
    setup_requires=[
        'cython>=0.20',
        'setuptools>=25',
    ],
    install_requires=[
        'setuptools>=25',
    ],
    extras_require={
        'doc': [
            'sphinx',
            'sphinx_rtd_theme',
        ],
        'test': [
            'pytest',
        ],
    },
    zip_safe=False,
    keywords='rocksdb bindings',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: BSD License',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
        'Topic :: Database',
    ],
    project_urls={
        'Bug Reports': 'https://github.com/martyanov/python-rocksdb/issues',
        'Repository': 'https://github.com/martyanov/python-rocksdb',
    },
)
