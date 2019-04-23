Installing
==========
.. highlight:: bash


With distro package and pypi
****************************

This requires librocksdb-dev>=5.0

.. code-block:: bash

    apt-get install python-virtualenv python-dev librocksdb-dev
    virtualenv venv
    source venv/bin/activate
    pip install python-rocksdb

From source
***********

Building python-rocksdb
-----------------------

.. code-block:: bash

    git clone https://github.com/twmht/python-rocksdb.git --recursive -b pybind11
    cd python-rocksdb
    python setup.py install
