python-rocksdb
==============

Python bindings for RocksDB. See http://rocksdb.readthedocs.io for a more comprehensive install and usage description.


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

This project is a fork of `python-rocksdb`_ maintained by `twmht`_, which itself is a fork
of `pyrocksdb`_, that was originally written by `stephan-hof`_.

.. _python-rocksdb: https://github.com/twmht/python-rocksdb
.. _twmht: https://github.com/twmht
.. _pyrocksdb: https://github.com/stephan-hof/pyrocksdb
.. _stephan-hof: https://github.com/stephan-hof
