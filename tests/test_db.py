import pyrocksdb
import tempfile
import pytest
import os

def test_options():
    opts = pyrocksdb.Options()
    assert(opts.target_file_size_multiplier == 1)

@pytest.fixture
def db():
    db = pyrocksdb.DB()
    opts = pyrocksdb.Options()
    opts.IncreaseParallelism()
    opts.OptimizeLevelStyleCompaction()
    opts.create_if_missing = True
    tmp = tempfile.TemporaryDirectory()
    s = db.open(opts, tmp.name)
    assert s.ok()
    yield db
    db.close()

@pytest.fixture
def transaction_db():
    opts = pyrocksdb.Options()
    opts.create_if_missing = True
    txn_db_opts = pyrocksdb.TransactionDBOptions()
    tmp = tempfile.TemporaryDirectory()
    db = pyrocksdb.transaction_db()
    s = db.open(opts, txn_db_opts, tmp.name)
    assert(s.ok())
    yield db
    db.close()


def test_put_get(db):
    opts = pyrocksdb.WriteOptions()
    s = db.put(opts, b"key1", b"value1")
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, b"key1")
    assert blob.status.ok()
    assert blob.data == b'value1'

def test_delete(db):
    opts = pyrocksdb.WriteOptions()
    s = db.put(opts, b"key1", b"value1")
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, b"key1")
    assert blob.status.ok()
    assert blob.data == b'value1'
    opts = pyrocksdb.WriteOptions()
    s = db.delete(opts, b"key1")
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, b"key1")
    assert not blob.status.ok()

def test_iterator(db):
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

def test_write_batch(db):
    update = pyrocksdb.WriteBatch()

    update.put(b'key1', b'value1')
    update.put(b'key2', b'value2')
    update.delete(b'key1')
    opts = pyrocksdb.WriteOptions()
    s = db.write(opts, update)
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, b'key1')
    assert blob.status.is_not_found()
    blob = db.get(opts, b'key2')
    assert blob.status.ok()
    assert blob.data == b'value2'

def test_transaction_db():
    opts = pyrocksdb.Options()
    opts.create_if_missing = True
    txn_db_opts = pyrocksdb.TransactionDBOptions()
    tmp = tempfile.TemporaryDirectory()
    db = pyrocksdb.transaction_db()
    s = db.open(opts, txn_db_opts, tmp.name)
    assert(s.ok())
    wopts = pyrocksdb.WriteOptions()
    txn = db.begin_transaction(wopts)
    assert(txn)
    ropts = pyrocksdb.ReadOptions()
    blob = txn.get(ropts, b'key1')
    assert(blob.status.is_not_found())
    del txn

    txn = db.begin_transaction(wopts)
    s = txn.put(b'key1', b'value1')
    assert(s.ok())
    s = txn.put(b'key2', b'value2')
    assert(s.ok())
    blob = txn.get(ropts, b'key1')
    assert(blob.status.ok())
    assert(blob.data == b'value1')

    # the data won't be written unitl the commit
    blob = db.get(ropts, b'key1')
    assert(blob.status.is_not_found())

    s = db.put(wopts, b'key3', b'value3')
    assert(s.ok())

    s = db.put(wopts, b'key1', b'value1')
    assert(not s.ok())

    s = txn.commit()
    assert(s.ok())
    del txn

    txn_opts = pyrocksdb.TransactionOptions()
    txn_opts.set_snapshot = True
    txn = db.begin_transaction(wopts, txn_opts)
    snapshot = txn.get_snapshot()
    del txn

def test_transaction_snapshot(transaction_db):
    db = transaction_db

    wopts = pyrocksdb.WriteOptions()
    txn_opts = pyrocksdb.TransactionOptions()
    txn_opts.set_snapshot = True
    txn = db.begin_transaction(wopts, txn_opts)
    snapshot = txn.get_snapshot()

    # Write a key OUTSIDE of transaction
    s = db.put(wopts, b'abc', b'xyz')
    assert(s.ok())

    #  Attempt to read a key using the snapshot.  This will fail since
    #  the previous write outside this txn conflicts with this read.
    ropts = pyrocksdb.ReadOptions()
    ropts.snapshot = snapshot
    blob = txn.get_for_update(ropts, b"abc")
    assert(blob.status.is_busy());

    # Discard all batched writes in this transaction.

    txn.rollback()
    # be sure to clear the snapshot
    txn.clear_snapshot()

    del txn

def test_transaction_read_commited(transaction_db):


    #  "Read Committed" (Monotonic Atomic Views) Example
    #  --Using multiple Snapshots

    #  In this example, we set the snapshot multiple times.  This is probably
    #  only necessary if you have very strict isolation requirements to
    #  implement.

    #  opts = pyrocksdb.Options()
    #  opts.create_if_missing = True
    #  txn_db_opts = pyrocksdb.TransactionDBOptions()
    #  tmp = tempfile.TemporaryDirectory()
    #  db = pyrocksdb.transaction_db()
    #  s = db.open(opts, txn_db_opts, tmp.name)
    #  assert(s.ok())
    db = transaction_db

    # Set a snapshot at start of transaction
    wopts = pyrocksdb.WriteOptions()
    txn_opts = pyrocksdb.TransactionOptions()
    txn_opts.set_snapshot = True
    txn = db.begin_transaction(wopts, txn_opts)
    snapshot = txn.get_snapshot()

    #  Do some reads and writes to key "x"
    ropts = pyrocksdb.ReadOptions()
    ropts.snapshot = snapshot
    blob = txn.get(ropts, b"x");
    assert(blob.status.is_not_found())
    txn.put(b"x", b"x")
    # Do a write outside of the transaction to key "y"
    s = db.put(wopts, b"y", b"y");
    assert(s.ok())


    #  Set a new snapshot in the transaction
    txn.set_snapshot();
    txn.set_savepoint();
    ropts.snapshot = db.get_snapshot();


    # Do some reads and writes to key "y"
    #  Since the snapshot was advanced, the write done outside of the
    #  transaction does not conflict.
    blob = txn.get_for_update(ropts, b"y")
    assert(blob.status.ok())
    assert(blob.data == b'y')
    txn.put("y", "y");


    # Decide we want to revert the last write from this transaction.
    txn.rollback_to_savepoint()

    # Commit.
    s = txn.commit()
    assert(s.ok())
    del txn;
    # Clear snapshot from read options since it is no longer valid
    db.release_snapshot(ropts.snapshot)

def test_column_family():
    db = pyrocksdb.DB()
    opts = pyrocksdb.Options()
    opts.create_if_missing = True
    tmp = tempfile.TemporaryDirectory()
    s = db.open(opts, tmp.name)
    assert s.ok()

    copts = pyrocksdb.ColumnFamilyOptions()
    s, cf = db.create_column_family(copts, "new_cf")
    del cf
    db.close()

    cfd1 = pyrocksdb.ColumnFamilyDescriptor(pyrocksdb.DefaultColumnFamilyName, pyrocksdb.ColumnFamilyOptions())
    cfd2 = pyrocksdb.ColumnFamilyDescriptor("new_cf", pyrocksdb.ColumnFamilyOptions())
    cfds = pyrocksdb.VectorColumnFamilyDescriptor()
    cfds.append(cfd1)
    cfds.append(cfd2)
    db_opts = pyrocksdb.DBOptions()
    #  a = [1,2]
    s, cfhs = db.open(db_opts, tmp.name, cfds)
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

def test_readonly():
    db = pyrocksdb.DB()
    opts = pyrocksdb.Options()
    opts.IncreaseParallelism()
    opts.OptimizeLevelStyleCompaction()
    opts.create_if_missing = True
    tmp = tempfile.TemporaryDirectory()
    s = db.open(opts, tmp.name)
    assert s.ok()
    db.close()

    s = db.open_for_readonly(opts, tmp.name)
    assert(s.ok())

    wopts = pyrocksdb.WriteOptions()
    s = db.put(wopts, b'key1', b'value1')
    assert(not s.ok())

def test_readonly_column_family():
    db = pyrocksdb.DB()
    opts = pyrocksdb.Options()
    opts.create_if_missing = True
    tmp = tempfile.TemporaryDirectory()
    s = db.open(opts, tmp.name)
    assert s.ok()

    copts = pyrocksdb.ColumnFamilyOptions()
    s, cf = db.create_column_family(copts, "new_cf")
    del cf
    db.close()

    cfd1 = pyrocksdb.ColumnFamilyDescriptor(pyrocksdb.DefaultColumnFamilyName, pyrocksdb.ColumnFamilyOptions())
    cfd2 = pyrocksdb.ColumnFamilyDescriptor("new_cf", pyrocksdb.ColumnFamilyOptions())
    cfds = pyrocksdb.VectorColumnFamilyDescriptor()
    cfds.append(cfd1)
    cfds.append(cfd2)
    db_opts = pyrocksdb.DBOptions()
    #  a = [1,2]
    s, cfhs = db.open_for_readonly(db_opts, tmp.name, cfds)
    assert(s.ok())
    assert(len(cfhs) == 2)
    assert(cfhs[0].get_name() == pyrocksdb.DefaultColumnFamilyName)
    assert(cfhs[1].get_name() =='new_cf')

    wopts = pyrocksdb.WriteOptions()
    s = db.put(wopts, cfhs[1], b'key1', b'value1')
    assert(not s.ok())

#  def test_open_secondary():
    #  db = pyrocksdb.DB()
    #  opts = pyrocksdb.Options()
    #  opts.create_if_missing = True
    #  tmp = tempfile.TemporaryDirectory()
    #  s = db.open(opts, tmp.name)
    #  assert s.ok()

    #  db2 = pyrocksdb.DB()
    #  tmp2 = tempfile.TemporaryDirectory()
    #  s = db2.open_as_secondary(opts, tmp.name, tmp2.name)
    #  assert s.ok()

    #  s = db2.try_catch_up_with_primary()
    #  assert s.ok()

    #  wopts = pyrocksdb.WriteOptions()
    #  s = db.put(wopts, b'key1', b'value1')
    #  assert(s.ok())
    #  ropts = pyrocksdb.ReadOptions()
    #  blob = db2.get(ropts, b'key1')
    #  assert(blob.status.ok())
    #  assert(blob.data == b'value1')

def test_merge_operator():
    class AddMergeOperator(pyrocksdb.AssociativeMergeOperator):
        def merge(self, existing_value, value):
            # TODO: can we skip slice_to_byte and keep the efficiency?
            print ('payyyaaaaa merge {}'.format(existing_value))
            print ('payyyaaaaa merge {}'.format(value))
            if existing_value:
                s = int(value) + int(existing_value)
                return (True, str(s).encode())
            else:
                return (True, str(int(value)).encode())

        def Name(self):
            return b"AddMergeOperator";

    opts = pyrocksdb.Options()
    # FIXME: we have to hold this reference otherwise the python object would be destroyed
    merge_operator = AddMergeOperator()
    db = pyrocksdb.DB()
    opts.IncreaseParallelism()
    opts.OptimizeLevelStyleCompaction()
    opts.create_if_missing = True
    opts.merge_operator = merge_operator
    tmp = tempfile.TemporaryDirectory()
    s = db.open(opts, tmp.name)
    assert(s.ok())
    wopts = pyrocksdb.WriteOptions()
    ropts = pyrocksdb.ReadOptions()
    s = db.merge(wopts, b"a", b"2")
    assert(s.ok())
    print('get1')
    blob = db.get(ropts, b"a")
    s = db.merge(wopts, b"a", b"3")
    assert(s.ok())
    print('get2')
    blob = db.get(ropts, b"a")
    #  print ('a')
    #  while (True):
        #  pass



    #  assert blob.data == b'5'
