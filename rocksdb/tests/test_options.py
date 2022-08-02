import sys
import unittest

import rocksdb


class TestFilterPolicy(rocksdb.interfaces.FilterPolicy):
    def create_filter(self, keys):
        return b'nix'

    def key_may_match(self, key, fil):
        return True

    def name(self):
        return b'testfilter'

class TestMergeOperator(rocksdb.interfaces.MergeOperator):
    def full_merge(self, *args, **kwargs):
        return (False, None)

    def partial_merge(self, *args, **kwargs):
        return (False, None)

    def name(self):
        return b'testmergeop'

class TestOptions(unittest.TestCase):
    #  def test_default_merge_operator(self):
        #  opts = rocksdb.Options()
        #  self.assertEqual(True, opts.paranoid_checks)
        #  opts.paranoid_checks = False
        #  self.assertEqual(False, opts.paranoid_checks)

        #  self.assertIsNone(opts.merge_operator)
        #  opts.merge_operator = "uint64add"
        #  self.assertIsNotNone(opts.merge_operator)
        #  self.assertEqual(opts.merge_operator, "uint64add")
        #  with self.assertRaises(TypeError):
            #  opts.merge_operator = "not an operator"

    # FIXME: travis test should include the latest version of rocksdb
    def test_compaction_pri(self):
        opts = rocksdb.Options()
        # default compaction_pri
        self.assertEqual(opts.compaction_pri, rocksdb.CompactionPri.min_overlapping_ratio)
        opts.compaction_pri = rocksdb.CompactionPri.by_compensated_size
        self.assertEqual(opts.compaction_pri, rocksdb.CompactionPri.by_compensated_size)
        opts.compaction_pri = rocksdb.CompactionPri.oldest_largest_seq_first
        self.assertEqual(opts.compaction_pri, rocksdb.CompactionPri.oldest_largest_seq_first)
        opts.compaction_pri = rocksdb.CompactionPri.min_overlapping_ratio
        self.assertEqual(opts.compaction_pri, rocksdb.CompactionPri.min_overlapping_ratio)

    def test_enable_write_thread_adaptive_yield(self):
        opts = rocksdb.Options()
        self.assertEqual(opts.enable_write_thread_adaptive_yield, True)
        opts.enable_write_thread_adaptive_yield = False
        self.assertEqual(opts.enable_write_thread_adaptive_yield, False)

    def test_allow_concurrent_memtable_write(self):
        opts = rocksdb.Options()
        self.assertEqual(opts.allow_concurrent_memtable_write, True)
        opts.allow_concurrent_memtable_write = False
        self.assertEqual(opts.allow_concurrent_memtable_write, False)

    def test_compression_opts(self):
        opts = rocksdb.Options()
        compression_opts = opts.compression_opts
        # default value
        self.assertEqual(isinstance(compression_opts, dict), True)
        self.assertEqual(compression_opts['window_bits'], -14)
        self.assertEqual(compression_opts['level'], 2**15 - 1)
        self.assertEqual(compression_opts['strategy'], 0)
        self.assertEqual(compression_opts['max_dict_bytes'], 0)
        self.assertEqual(compression_opts['zstd_max_train_bytes'], 0)
        self.assertEqual(compression_opts['parallel_threads'], 1)
        self.assertEqual(compression_opts['enabled'], False)

        with self.assertRaises(TypeError):
            opts.compression_opts = list(1, 2)

        new_opts = {
            'window_bits': 1,
            'level': 2,
            'strategy': 3,
            'max_dict_bytes': 4,
            'zstd_max_train_bytes': 15,
            'parallel_threads': 4,
            'enabled': True}
        opts.compression_opts = new_opts
        self.assertIsNot(new_opts, opts.compression_opts)
        for key, value in new_opts.items():
            self.assertEqual(opts.compression_opts[key], value)

    def test_bottommost_compression_opts(self):
        opts = rocksdb.Options()
        bottommost_compression_opts = opts.bottommost_compression_opts
        # default value
        self.assertEqual(isinstance(bottommost_compression_opts, dict), True)
        self.assertEqual(bottommost_compression_opts['window_bits'], -14)
        self.assertEqual(bottommost_compression_opts['level'], 2**15 - 1)
        self.assertEqual(bottommost_compression_opts['strategy'], 0)
        self.assertEqual(bottommost_compression_opts['max_dict_bytes'], 0)
        self.assertEqual(bottommost_compression_opts['zstd_max_train_bytes'], 0)
        self.assertEqual(bottommost_compression_opts['parallel_threads'], 1)
        self.assertEqual(bottommost_compression_opts['enabled'], False)

        with self.assertRaises(TypeError):
            opts.compression_opts = list(1, 2)

        new_opts = {
            'window_bits': 1,
            'level': 2,
            'strategy': 3,
            'max_dict_bytes': 4,
            'zstd_max_train_bytes': 15,
            'parallel_threads': 4,
            'enabled': True,
        }
        opts.bottommost_compression_opts = new_opts
        self.assertIsNot(new_opts, opts.bottommost_compression_opts)
        for key, value in new_opts.items():
            self.assertEqual(opts.bottommost_compression_opts[key], value)

    def test_simple(self):
        opts = rocksdb.Options()
        self.assertEqual(True, opts.paranoid_checks)
        opts.paranoid_checks = False
        self.assertEqual(False, opts.paranoid_checks)

        self.assertIsNone(opts.merge_operator)
        ob = TestMergeOperator()
        opts.merge_operator = ob
        self.assertEqual(opts.merge_operator, ob)

        self.assertIsInstance(
            opts.comparator,
            rocksdb.BytewiseComparator)

        self.assertIn(opts.compression,
                      (rocksdb.CompressionType.no_compression,
                      rocksdb.CompressionType.snappy_compression))

        opts.compression = rocksdb.CompressionType.zstd_compression
        self.assertEqual(rocksdb.CompressionType.zstd_compression, opts.compression)

    def test_block_options(self):
        rocksdb.BlockBasedTableFactory(
            block_size=4096,
            filter_policy=TestFilterPolicy(),
            block_cache=rocksdb.LRUCache(100))

    def test_unicode_path(self):
        name = b'/tmp/M\xc3\xbcnchen'.decode('utf8')
        opts = rocksdb.Options()
        opts.db_log_dir = name
        opts.wal_dir = name

        self.assertEqual(name, opts.db_log_dir)
        self.assertEqual(name, opts.wal_dir)

    def test_table_factory(self):
        opts = rocksdb.Options()
        self.assertIsNone(opts.table_factory)

        opts.table_factory = rocksdb.BlockBasedTableFactory()
        opts.table_factory = rocksdb.PlainTableFactory()

    def test_compaction_style(self):
        opts = rocksdb.Options()
        self.assertEqual('level', opts.compaction_style)

        opts.compaction_style = 'universal'
        self.assertEqual('universal', opts.compaction_style)

        opts.compaction_style = 'level'
        self.assertEqual('level', opts.compaction_style)

        if sys.version_info[0] == 3:
            assertRaisesRegex = self.assertRaisesRegex
        else:
            assertRaisesRegex = self.assertRaisesRegexp

        with assertRaisesRegex(Exception, 'Unknown compaction style'):
            opts.compaction_style = 'foo'

    def test_compaction_opts_universal(self):
        opts = rocksdb.Options()
        uopts = opts.compaction_options_universal
        self.assertEqual(-1, uopts['compression_size_percent'])
        self.assertEqual(200, uopts['max_size_amplification_percent'])
        self.assertEqual('total_size', uopts['stop_style'])
        self.assertEqual(1, uopts['size_ratio'])
        self.assertEqual(2, uopts['min_merge_width'])
        self.assertGreaterEqual(4294967295, uopts['max_merge_width'])

        new_opts = {'stop_style': 'similar_size', 'max_merge_width': 30}
        opts.compaction_options_universal = new_opts
        uopts = opts.compaction_options_universal

        self.assertEqual(-1, uopts['compression_size_percent'])
        self.assertEqual(200, uopts['max_size_amplification_percent'])
        self.assertEqual('similar_size', uopts['stop_style'])
        self.assertEqual(1, uopts['size_ratio'])
        self.assertEqual(2, uopts['min_merge_width'])
        self.assertEqual(30, uopts['max_merge_width'])

    def test_rocksdb_options(self):
        NOTNONE = object()
        UNSETTABLE = object()
        for option, def_value, new_value in (
                ('max_open_files', NOTNONE, 10),
                ('row_cache', None, rocksdb.LRUCache(2*1024*1024)),
                ('max_file_opening_threads', NOTNONE, 10),
                ('max_total_wal_size', NOTNONE, 10),
                ('max_background_jobs', NOTNONE, 10),
                ('base_background_compactions', NOTNONE, 10),
                ('max_background_compactions', NOTNONE, 10),
                ('max_subcompactions', NOTNONE, 10),
                ('max_background_flushes', NOTNONE, 10),
                ('max_log_file_size', NOTNONE, 10),
                ('log_file_time_to_roll', NOTNONE, 10),
                ('keep_log_file_num', 1000, 10),
                ('recycle_log_file_num', NOTNONE, 10),
                ('max_manifest_file_size', NOTNONE, 10),
                ('table_cache_numshardbits', NOTNONE, 10),
                ('wal_ttl_seconds', NOTNONE, 10),
                ('wal_size_limit_mb', NOTNONE, 10),
                ('manifest_preallocation_size', NOTNONE, 10),
                ('allow_mmap_reads', False, True),
                ('allow_mmap_writes', False, True),
                ('use_direct_reads', False, True),
                ('use_direct_io_for_flush_and_compaction', False, True),
                ('allow_fallocate', True, False),
                ('is_fd_close_on_exec', True, False),
                ('stats_dump_period_sec', 600, 3600),
                ('stats_persist_period_sec', 600, 3600),
                ('persist_stats_to_disk', False, True),
                ('stats_history_buffer_size', 1024*1024, 1024),
                ('advise_random_on_open', True, False),
                ('db_write_buffer_size', 0, 100),
                ('new_table_reader_for_compaction_inputs', False, True),
                ('compaction_readahead_size', 0, 10),
                ('random_access_max_buffer_size', 1024*1024, 100),
                ('writable_file_max_buffer_size', 1024*1024, 100),
                ('use_adaptive_mutex', False, True),
                ('bytes_per_sync', 0, 10),
                ('wal_bytes_per_sync', 0, 10),
                ('strict_bytes_per_sync', False, True),
                ('enable_thread_tracking', False, True),
                ('delayed_write_rate', 0, 10),
                ('enable_pipelined_write', False, True),
                ('unordered_write', False, True),
                ('allow_concurrent_memtable_write', True, False),
                ('enable_write_thread_adaptive_yield', True, False),
                ('max_write_batch_group_size_bytes', 1 << 20, 10),
                ('write_thread_max_yield_usec', 100, 200),
                ('write_thread_slow_yield_usec', 3, 2000),
                ('skip_stats_update_on_db_open', False, True),
                ('skip_checking_sst_file_sizes_on_db_open', False, True),
                ('allow_2pc', False, True),
                ('fail_if_options_file_error', False, True),
                ('dump_malloc_stats', False, True),
                ('avoid_flush_during_recovery', False, True),
                ('avoid_flush_during_shutdown', False, True),
                ('allow_ingest_behind', False, True),
                ('preserve_deletes', False, True),
                ('two_write_queues', False, True),
                ('manual_wal_flush', False, True),
                ('atomic_flush', False, True),
                ('avoid_unnecessary_blocking_io', False, True),
                ('write_dbid_to_manifest', False, True),
                ('log_readahead_size', 0, 10),
                ('best_efforts_recovery', False, True),
        ):
            with self.subTest(option=option):
                opts = rocksdb.Options()
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
