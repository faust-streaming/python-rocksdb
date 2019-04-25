import pyrocksdb
import tempfile
import pytest

#  s = pyrocksdb.Slice(b'key1')
#  print (type(s.data()))


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

def test_option():
    opts = pyrocksdb.Options()
    assert opts.create_if_missing == False
    assert opts.error_if_exists == False


def test_put_get(db):
    opts = pyrocksdb.WriteOptions()
    s = db.put(opts, "key1", "value1")
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, "key1")
    assert blob.status.ok()
    assert blob.data == 'value1'

def test_delete(db):
    opts = pyrocksdb.WriteOptions()
    s = db.put(opts, "key1", "value1")
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, "key1")
    assert blob.status.ok()
    assert blob.data == 'value1'
    opts = pyrocksdb.WriteOptions()
    s = db.delete(opts, "key1")
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, "key1")
    assert not blob.status.ok()

def test_iterator(db):
    opts = pyrocksdb.WriteOptions()
    s = {'key1': 'value1', 'key2': 'value2', 'key3': 'value3'}
    for k, v in s.items():
        db.put(opts, k, v)

    opts = pyrocksdb.ReadOptions()
    it = db.iterator(opts)
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

def test_write_batch(db):
    update = pyrocksdb.WriteBatch()

    update.put('key1', 'value1')
    update.put('key2', 'value2')
    update.delete('key1')
    opts = pyrocksdb.WriteOptions()
    s = db.write(opts, update)
    assert s.ok()
    opts = pyrocksdb.ReadOptions()
    blob = db.get(opts, 'key1')
    assert blob.status.is_not_found()
    blob = db.get(opts, 'key2')
    assert blob.status.ok()
    assert blob.data == 'value2'
