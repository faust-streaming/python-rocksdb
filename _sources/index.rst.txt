Welcome to python-rocksdb's documentation!
==========================================

Overview
--------
Python bindings to the C++ interface of http://rocksdb.org/ using pybind11::

    import pyrocksdb
    db = pyrocksdb.DB()
    opts = pyrocksdb.Options()
    # for multi-thread
    opts.IncreaseParallelism()
    opts.OptimizeLevelStyleCompaction()
    opts.create_if_missing = True
    s = db.open(opts, '/path/to/db')
    assert(s.ok())
    # put
    opts = pyrocksdb.WriteOptions()
    s = db.put(opts, b"key1", b"value1")
    assert (s.ok())
    # get
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, b"key1")
    print (blob.data) # b"value1"
    print (blob.status.ok()) # true
    #delete
    opts = pyrocksdb.WriteOptions()
    s = db.delete(opts, b"key1")
    assert(s.ok())
    db.close()


Tested with python3.7

.. toctree::
    :maxdepth: 2

    python_rocksdb

    Instructions how to install <installation>
    Tutorial <tutorial/index>
    Changelog <changelog>


Contributing
------------

Source can be found on `github <https://github.com/twmht/python-rocksdb/tree/pybind11>`_.
Feel free to fork and send pull-requests or create issues on the
`github issue tracker <https://github.com/twmht/python-rocksdb/issues>`_


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
