Basic Usage of python-rocksdb
*****************************

Column Family
=============   

python-rocksdb also supports column family ::

    db = pyrocksdb.DB()
    opts = pyrocksdb.Options()
    opts.create_if_missing = True
    s = db.open(opts, 'path/to/db')
    # create new column family
    copts = pyrocksdb.ColumnFamilyOptions()
    s, cf = db.create_column_family(copts, "new_cf")
    del cf
    db.close()
    # open db with the column families
    cfd1 = pyrocksdb.ColumnFamilyDescriptor(pyrocksdb.DefaultColumnFamilyName, pyrocksdb.ColumnFamilyOptions())
    cfd2 = pyrocksdb.ColumnFamilyDescriptor("new_cf", pyrocksdb.ColumnFamilyOptions())
    cfds = pyrocksdb.VectorColumnFamilyDescriptor()
    cfds.append(cfd1)
    cfds.append(cfd2)
    db_opts = pyrocksdb.DBOptions()
    # column family handles
    s, cfhs = db.open(db_opts, 'path/to/db', cfds)
    assert(s.ok())
    assert(len(cfhs) == 2)
    assert(cfhs[0].get_name() == pyrocksdb.DefaultColumnFamilyName)
    assert(cfhs[1].get_name() =='new_cf')

    wopts = pyrocksdb.WriteOptions()
    ropts = pyrocksdb.ReadOptions()
    s = db.put(wopts, cfhs[0], b'key', b'value')
    assert(s.ok())
    b = db.get(ropts, cfhs[0], b'key')
    assert(b.status.ok())
    assert(b.data == b'value')
    b = db.get(ropts, cfhs[1], b'key')
    assert(b.status.is_not_found())

    s = db.put(wopts, cfhs[1], b'key', b'value2')
    assert(s.ok())
    b = db.get(ropts, cfhs[1], b'key')
    assert(b.status.ok())
    assert(b.data == b'value2')

    b = db.get(ropts, cfhs[0], b'key')
    assert(b.status.ok())
    assert(b.data == b'value')

    s = db.delete(wopts, cfhs[0], b'key')
    assert(s.ok())
    b = db.get(ropts, cfhs[0], b'key')
    assert(b.status.is_not_found())

    s = db.delete(wopts, cfhs[1], b'key')
    assert(s.ok())
    b = db.get(ropts, cfhs[1], b'key')
    assert(b.status.is_not_found())

    # write batch
    update = pyrocksdb.WriteBatch()
    update.put(cfhs[0], b'key1', b'value1')
    update.put(cfhs[0], b'key2', b'value2')
    update.delete(cfhs[0], 'key1')
    s = db.write(wopts, update)
    assert(s.ok())
    b = db.get(ropts, cfhs[0], b'key1')
    assert(b.status.is_not_found())
    b = db.get(ropts, cfhs[0], b'key2')
    assert(b.status.ok())
    assert(b.data == b'value2')

    for cfh in cfhs:
        del cfh
    db.close()


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
