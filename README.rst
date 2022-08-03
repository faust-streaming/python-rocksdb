faust-streaming-rocksdb
=======================

Python bindings for RocksDB.

See https://faust-streaming.github.io/python-rocksdb/ for a more comprehensive install and
usage description.

Quick Install
-------------

This library is built for Debian + Ubuntu Distributions.

.. code-block:: bash

    $ apt-get install build-essential libsnappy-dev zlib1g-dev libbz2-dev libgflags-dev liblz4-dev librocksdb-dev
    $ pip3 install faust-streaming-rocksdb



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
