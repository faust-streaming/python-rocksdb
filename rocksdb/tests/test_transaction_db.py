import os
import sys
import shutil
import gc
import unittest
import rocksdb
from itertools import takewhile
import struct
import tempfile
from rocksdb.merge_operators import UintAddOperator, StringAppendOperator

from .test_db import TestHelper, TestDB

class TestTransactionDB(TestDB):
    """Re-run TestDB with the TransactionDB object"""

    def setUp(self):
        TestHelper.setUp(self)
        opts = rocksdb.Options(create_if_missing=True)
        tdb_opts = rocksdb.TransactionDBOptions()
        self.db = rocksdb.TransactionDB(
            os.path.join(self.db_loc, "test"),
            opts,
            tdb_opts=tdb_opts)

    def test_options_used_twice(self):
        expected = "Options object is already used by another DB"
        tdb_opts = rocksdb.TransactionDBOptions()
        with self.assertRaisesRegex(rocksdb.InvalidArgument, expected):
            rocksdb.TransactionDB(os.path.join(self.db_loc, "test2"),
                                  self.db.options,
                                  tdb_opts=tdb_opts)

    def test_transaction_options_used_twice(self):
        expected = "Transaction Options object is already used by another DB"
        opts = rocksdb.Options(create_if_missing=True)
        with self.assertRaisesRegex(rocksdb.InvalidArgument, expected):
            rocksdb.TransactionDB(os.path.join(self.db_loc, "test2"),
                                  opts,
                                  tdb_opts=self.db.transaction_options)
