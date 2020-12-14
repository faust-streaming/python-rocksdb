python-rocksdb
==============

Python bindings for RocksDB. See http://rocksdb.readthedocs.io for a more comprehensive install and usage description.


Quick install
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

    $ apt-get install python-dev
    $ python3 -m venv .venv
    $ source .venv/bin/activate
    $ pip install rocksdb


Quick usage guide
-----------------

.. code-block:: python

    >>> import rocksdb
    >>> db = rocksdb.DB("test.db", rocksdb.Options(create_if_missing=True))
    >>> db.put(b'a', b'data')
    >>> print db.get(b'a')
    b'data'


Acknowledgements
----------------

This project is a fork of `python-rocksdb`_ maintained by `twmht`_, which itself is a fork
of `pyrocksdb`_, that was originally written by `stephan-hof`_.

.. _python-rocksdb: https://github.com/twmht/python-rocksdb
.. _twmht: https://github.com/twmht
.. _pyrocksdb: https://github.com/stephan-hof/pyrocksdb
.. _stephan-hof: https://github.com/stephan-hof
