Basic Usage of python-rocksdb
*****************************

Wrtie Batch
===========  

It is also possible to gather modifications and
apply them in a single operation ::

    update = pyrocksdb.WriteBatch()

    # collect the operations
    update.put(b'key1', b'value1')
    update.put(b'key2', b'value2')
    update.delete(b'key1')

    opts = pyrocksdb.WriteOptions()
    # Apply these operatios in a single operation
    s = db.write(opts, update)

    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, b'key1')
    assert blob.status.is_not_found()
    blob = db.get(opts, b'key2')
    assert blob.status.ok()
    assert blob.data == b'value2'

    db.close()

Iteration
=========

Iterators behave slightly different than expected. Per default they are not
valid. So you have to call one of its seek methods first ::

    opts = pyrocksdb.WriteOptions()
    s = {b'key1': b'value1', b'key2': b'value2', b'key3': b'value3'}
    for k, v in s.items():
        db.put(opts, k, v)

    opts = pyrocksdb.ReadOptions()
    it = db.iterator(opts)
    it.seek_to_first()
    assert it.status().ok()
    assert it.valid()
    for k, v in s.items():
        assert it.key() == k
        assert it.value() == v
        it.next()

    assert not it.valid()

    it.seek(b'key1')
    assert it.valid()
    assert it.key() == b'key1'
    assert it.value() == b'value1'
    it.seek(b'key2')
    assert it.valid()
    assert it.key() == b'key2'
    assert it.value() == b'value2'

    it.seek(b'key4')
    assert not it.valid()

    it.seek_for_prev(b'key0')
    assert not it.valid()
    it.seek_for_prev(b'key4')
    assert it.valid()
