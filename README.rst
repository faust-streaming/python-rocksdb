Note
=========
The original pyrocksdb (https://pypi.python.org/pypi/pyrocksdb/0.4) has not been updated for long time. I update pyrocksdb to support the latest rocksdb. Please open issues in github if you have any problem.

News (2019/04/18)
=========
Currently I am refactoring the code, and more features like TTL are coming soon. And the installation with cmake will be much more easily.

News (2019/04/19)
=========
I have created a new branch(https://github.com/twmht/python-rocksdb/tree/pybind11) which provides the basic functions (`put`, `get` and `delete`) now. And the installtion is much more easily!  you can try it if you encounter any installtion issues in the current version of `python-rocksdb`.

The branch is under development and will be released to PypI after I migrate most of the existing features.

pyrocksdb
=========

Python bindings for RocksDB.

See https://rocksdb-tina.readthedocs.io/ for a more comprehensive install and
usage description.

Quick Install
-------------

Quick install for debian/ubuntu like linux distributions.

.. code-block:: bash

    $ apt-get install build-essential libsnappy-dev zlib1g-dev libbz2-dev libgflags-dev liblz4-dev
    $ git clone https://github.com/facebook/rocksdb.git
    $ cd rocksdb
    $ mkdir build && cd build
    $ cmake ..
    $ make
    $ cd ..
    $ export CPLUS_INCLUDE_PATH=${CPLUS_INCLUDE_PATH}${CPLUS_INCLUDE_PATH:+:}`pwd`/include/
    $ export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}${LD_LIBRARY_PATH:+:}`pwd`/build/
    $ export LIBRARY_PATH=${LIBRARY_PATH}${LIBRARY_PATH:+:}`pwd`/build/

    $ apt-get install python-virtualenv python-dev
    $ virtualenv pyrocks_test
    $ cd pyrocks_test
    $ . bin/active
    $ pip install python-rocksdb


Quick Usage Guide
-----------------

.. code-block:: python

    >>> import rocksdb
    >>> db = rocksdb.DB('test.db', rocksdb.Options(create_if_missing=True))
    >>> db.put(b'a', b'data')
    >>> print(db.get(b'a'))
    b'data'


Acknowledgements
----------------

This project attempts to collect the efforts put into different forks of the
`pyrocksdb`_ project that was originally written by `stephan-hof`_, as sadly
none seems to be actively maintained. In particular, the `python-rocksdb`_ fork
created by `twmht`_, but it also incorporates changes from other forks and
unfinished pull requests. The current state of this project has pulled all of
these commits from `NightTsarina`_'s `fork`_.

.. _python-rocksdb: https://github.com/twmht/python-rocksdb
.. _twmht: https://github.com/twmht
.. _pyrocksdb: https://github.com/stephan-hof/pyrocksdb
.. _stephan-hof: https://github.com/stephan-hof
.. _NightTsarina: https://github.com/NightTsarina
.. _fork: https://github.com/NightTsarina/python-rocksdb
