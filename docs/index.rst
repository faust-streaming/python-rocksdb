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
    s = db.put(opts, "key1", "value1")
    assert (s.ok())
    # get
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, "key1")
    print (blob.data) # value1
    print (blob.status.ok()) # true
    #delete
    opts = pyrocksdb.WriteOptions()
    s = db.delete(opts, "key1")
    assert(s.ok())
    db.close()


Tested with python2.7 and python3.4 and RocksDB version 5.3.0

.. toctree::
    :maxdepth: 2

    Instructions how to install <installation>
    Tutorial <tutorial/index>
    API <api/index>
    Changelog <changelog>


Contributing
------------

Source can be found on `github <https://github.com/stephan-hof/pyrocksdb>`_.
Feel free to fork and send pull-requests or create issues on the
`github issue tracker <https://github.com/stephan-hof/pyrocksdb/issues>`_

RoadMap/TODO
------------

No plans so far. Please submit wishes to the github issues.

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
