pyrocksdb
=========

Python bindings for RocksDB.

This project is under development, and more features are coming soon.

Quick Install
-------------

Quick install for debian/ubuntu like linux distributions.


```
git clone https://github.com/twmht/python-rocksdb.git --recursive -b pybind11
cd python-rocksdb
python setup.py install
```

Quick Usage Guide
-----------------

```python
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
```
