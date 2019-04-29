Basic Usage of python-rocksdb
*****************************

Wrtie Batch
===========  

It is also possible to gather modifications and
apply them in a single operation ::

    update = pyrocksdb.WriteBatch()

    # collect the operations
    update.put('key1', 'value1')
    update.put('key2', 'value2')
    update.delete('key1')

    opts = pyrocksdb.WriteOptions()
    # Apply these operatios in a single operation
    s = db.write(opts, update)

    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, 'key1')
    assert blob.status.is_not_found()
    blob = db.get(opts, 'key2')
    assert blob.status.ok()
    assert blob.data == 'value2'

    db.close()

Iteration
=========

Iterators behave slightly different than expected. Per default they are not
valid. So you have to call one of its seek methods first ::

    opts = pyrocksdb.WriteOptions()
    s = {'key1': 'value1', 'key2': 'value2', 'key3': 'value3'}
    for k, v in s.items():
        db.put(opts, k, v)

    opts = pyrocksdb.ReadOptions()
    it = db.iterator(opts)
    # call this first
    it.seek_to_first()
    assert it.status().ok()
    assert it.valid()
    for k, v in s.items():
        assert it.key().data() == k
        assert it.value().data() == v
        it.next()

    assert not it.valid()

    it.seek('key1')
    assert it.valid()
    assert it.key().data() == 'key1'
    assert it.value().data() == 'value1'
    it.seek('key2')
    assert it.valid()
    assert it.key().data() == 'key2'
    assert it.value().data() == 'value2'

    it.seek('key4')
    assert not it.valid()

    it.seek_for_prev('key0')
    assert not it.valid()
    it.seek_for_prev('key4')
    assert it.valid()
