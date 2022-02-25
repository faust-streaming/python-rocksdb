import unittest
import sys
import rocksdb

class TestTransactionDBOptions(unittest.TestCase):
    def test_default_getter_setter(self):
        NOTNONE = object()
        UNSETTABLE = object()
        for option, def_value, new_value in (
                ('max_num_locks', -1, 100),
                ('max_num_deadlocks', 5, 10),
                ('num_stripes', 16, 10),
                ('transaction_lock_timeout', 1000, 100),
                ('default_lock_timeout', 1000, 10000),
                ('rollback_merge_operands', False, True),
                ('skip_concurrency_control', False, True),
                ('default_write_batch_flush_threshold', 0, 10)
        ):
            with self.subTest(option=option):
                opts = rocksdb.TransactionDBOptions()
                if def_value is NOTNONE:
                    self.assertIsNotNone(getattr(opts, option))
                else:
                    self.assertEqual(def_value, getattr(opts, option))
                if new_value is UNSETTABLE:
                    self.assertRaises(
                        Exception, setattr, opts, option, new_value)
                else:
                    setattr(opts, option, new_value)
                    self.assertEqual(getattr(opts, option), new_value)

    def test_write_policy(self):
        opts = rocksdb.TransactionDBOptions()
        self.assertEqual(opts.write_policy, 'write_committed')
        accepted_values = ('write_committed',
                           'write_prepared',
                           'write_unprepared')
        for value in accepted_values:
            opts.write_policy = value
            self.assertEqual(opts.write_policy, value)

        error_value = 'write_dummy'
        self.assertRaises(
            rocksdb.InvalidArgument, setattr, opts, 'write_policy', error_value)
