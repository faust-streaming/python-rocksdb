python-rocksdb
==============

Python bindings for RocksDB.

See https://rocksdb-tina.readthedocs.io/ for a more comprehensive install and
usage description.


Quick install
-------------

.. code-block:: bash

    $ pip install rocksdb


Quick usage guide
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
unfinished pull requests.

.. _python-rocksdb: https://github.com/twmht/python-rocksdb
.. _twmht: https://github.com/twmht
.. _pyrocksdb: https://github.com/stephan-hof/pyrocksdb
.. _stephan-hof: https://github.com/stephan-hof
