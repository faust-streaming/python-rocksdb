from libcpp cimport bool as cpp_bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libc.stdint cimport uint64_t
from libc.stdint cimport uint32_t
from .std_memory cimport shared_ptr
from .comparator cimport Comparator
from .merge_operator cimport MergeOperator
from .logger cimport Logger
from .slice_ cimport Slice
from .snapshot cimport Snapshot
from .slice_transform cimport SliceTransform
from .table_factory cimport TableFactory
from .statistics cimport Statistics
from .memtablerep cimport MemTableRepFactory
from .universal_compaction cimport CompactionOptionsUniversal
from .cache cimport Cache
from . cimport advanced_options
from .advanced_options cimport CompressionOptions
from .advanced_options cimport AdvancedColumnFamilyOptions
from .env cimport Env
from .types cimport SequenceNumber
from .compaction_filter cimport CompactionFilter, CompactionFilterFactory
from .concurrent_task_limiter cimport ConcurrentTaskLimiter

cdef extern from "rocksdb/options.h" namespace "rocksdb":
    ctypedef enum CpuPriority:
        kIdle
        kLow
        kNormal
        kHigh

    ctypedef enum CompressionType:
        kNoCompression
        kSnappyCompression
        kZlibCompression
        kBZip2Compression
        kLZ4Compression
        kLZ4HCCompression
        kXpressCompression
        kZSTD
        kZSTDNotFinalCompression
        kDisableCompressionOption

    cdef cppclass ColumnFamilyOptions(AdvancedColumnFamilyOptions):
        ColumnFamilyOptions* OldDefaults(int, int)
        ColumnFamilyOptions* OptimizeForSmallDb(shared_ptr[Cache]*)
        ColumnFamilyOptions* OptimizeForPointLookup(uint64_t)
        ColumnFamilyOptions* OptimizeLevelStyleCompaction(uint64_t)
        ColumnFamilyOptions* OptimizeUniversalStyleCompaction(uint64_t)
        const Comparator* comparator
        shared_ptr[MergeOperator] merge_operator
        CompactionFilter* compaction_filter
        shared_ptr[CompactionFilterFactory] compaction_filter_factory
        size_t write_buffer_size
        advanced_options.CompressionType compression
        advanced_options.CompressionType bottommost_compression
        CompressionOptions bottommost_compression_opts
        advanced_options.CompactionPri compaction_pri
        CompressionOptions compression_opts
        int level0_file_num_compaction_trigger
        shared_ptr[SliceTransform] prefix_extractor
        uint64_t max_bytes_for_level_base
        # Deprecated but kept here since it is in the header.
        uint64_t snap_refresh_nanos
        cpp_bool disable_auto_compactions
        shared_ptr[TableFactory] table_factory

        vector[DbPath] cf_paths
        shared_ptr[ConcurrentTaskLimiter] compaction_thread_limiter
        ColumnFamilyOptions()
        ColumnFamilyOptions(const Options& options)
        void Dump(Logger*)

    # This needs to be in _rocksdb.pxd so it will export into python
    cpdef enum AccessHint "rocksdb::DBOptions::AccessHint":
       NONE,
       NORMAL,
       SEQUENTIAL,
       WILLNEED

    cpdef enum WALRecoveryMode:
        kTolerateCorruptedTailRecords
        kAbsoluteConsistency
        kPointInTimeRecovery
        kSkipAnyCorruptedRecords

    cdef cppclass DbPath:
        string path
        uint64_t target_size

        DbPath() except +
        DbPath(const string&, uint64_t) except +

    cdef cppclass DBOptions:
        DBOptions* OldDefaults(int, int) nogil except+
        DBOptions* OptimizeForSmallDb(shared_ptr[Cache]*) nogil except+
        void IncreaseParallelism(int) nogil except+
        cpp_bool create_if_missing
        cpp_bool create_missing_column_families
        cpp_bool error_if_exists
        cpp_bool paranoid_checks
        Env* env
        # TODO shared_ptr[RateLimiter] rate_limiter
        # TODO shared_ptr[SstFileManager] sst_file_manager
        shared_ptr[Logger] info_log
        # TODO InfoLogLevel info_log_level
        int max_open_files
        int max_file_opening_threads
        uint64_t max_total_wal_size
        shared_ptr[Statistics] statistics
        cpp_bool use_fsync
        vector[DbPath] db_paths
        string db_log_dir
        string wal_dir
        uint64_t delete_obsolete_files_period_micros
        int max_background_jobs
        int base_background_compactions
        int max_background_compactions
        uint32_t max_subcompactions
        int max_background_flushes
        size_t max_log_file_size
        size_t log_file_time_to_roll
        size_t keep_log_file_num
        size_t recycle_log_file_num
        uint64_t max_manifest_file_size
        int table_cache_numshardbits
        uint64_t WAL_ttl_seconds
        uint64_t WAL_size_limit_MB
        size_t manifest_preallocation_size
        cpp_bool allow_mmap_reads
        cpp_bool allow_mmap_writes
        cpp_bool use_direct_reads
        cpp_bool use_direct_io_for_flush_and_compaction
        cpp_bool allow_fallocate
        cpp_bool is_fd_close_on_exec
        unsigned int stats_dump_period_sec
        unsigned int stats_persist_period_sec
        cpp_bool persist_stats_to_disk
        size_t stats_history_buffer_size
        cpp_bool advise_random_on_open
        size_t db_write_buffer_size
        # TODO shared_ptr[WriteBufferManager] write_buffer_manager
        AccessHint access_hint_on_compaction_start
        cpp_bool new_table_reader_for_compaction_inputs
        size_t compaction_readahead_size
        size_t random_access_max_buffer_size
        size_t writable_file_max_buffer_size
        cpp_bool use_adaptive_mutex
        DBOptions() nogil except+
        DBOptions(const Options&) nogil except+
        void Dump(Logger*) nogil except+

        uint64_t bytes_per_sync
        uint64_t wal_bytes_per_sync
        cpp_bool strict_bytes_per_sync
        # TODO vector[shared_ptr[EventListener]] listeners
        cpp_bool enable_thread_tracking
        uint64_t delayed_write_rate
        cpp_bool enable_pipelined_write
        cpp_bool unordered_write
        cpp_bool allow_concurrent_memtable_write
        cpp_bool enable_write_thread_adaptive_yield
        uint64_t max_write_batch_group_size_bytes
        uint64_t write_thread_max_yield_usec
        uint64_t write_thread_slow_yield_usec
        cpp_bool skip_stats_update_on_db_open
        cpp_bool skip_checking_sst_file_sizes_on_db_open
        WALRecoveryMode wal_recovery_mode
        cpp_bool allow_2pc
        shared_ptr[Cache] row_cache
        # TODO WalFilter* wal_filter
        cpp_bool fail_if_options_file_error
        cpp_bool dump_malloc_stats
        cpp_bool avoid_flush_during_recovery
        cpp_bool avoid_flush_during_shutdown
        cpp_bool allow_ingest_behind
        cpp_bool preserve_deletes
        cpp_bool two_write_queues
        cpp_bool manual_wal_flush
        cpp_bool atomic_flush
        cpp_bool avoid_unnecessary_blocking_io
        cpp_bool write_dbid_to_manifest
        size_t log_readahead_size
        # TODO shared_ptr[FileChecksumGenFactory] file_checksum_gen_factory
        cpp_bool best_efforts_recovery

    cdef cppclass Options(DBOptions, ColumnFamilyOptions):
        Options() except+
        Options(const DBOptions&, const ColumnFamilyOptions&) except+
        Options* OldDefaults(int, int) nogil except+
        void Dump(Logger*) nogil except+
        void DumpCFOptions(Logger*) nogil except+
        Options* PrepareForBulkLoad() nogil except+
        Options* OptimizeForSmallDb() nogil except+

    ctypedef enum ReadTier:
        kReadAllTier
        kBlockCacheTier
        kPersistedTier
        kMemtableTier

    cdef cppclass ReadOptions:
        const Snapshot* snapshot
        const Slice* iterate_lower_bound
        const Slice* iterate_upper_bound
        size_t readahead_size
        uint64_t max_skippable_internal_keys
        ReadTier read_tier
        cpp_bool verify_checksums
        cpp_bool fill_cache
        cpp_bool tailing
        cpp_bool managed
        cpp_bool total_order_seek
        cpp_bool auto_prefix_mode
        cpp_bool prefix_same_as_start
        cpp_bool pin_data
        cpp_bool background_purge_on_iterator_cleanup
        cpp_bool ignore_range_deletions
        # TODO std::function<bool(const TableProperties&)> table_filter
        SequenceNumber iter_start_seqnum
        const Slice* timestamp
        const Slice* iter_start_ts
        # TODO std::chrono::microseconds deadline
        uint64_t value_size_soft_limit
        ReadOptions() nogil except+
        ReadOptions(cpp_bool, cpp_bool) nogil except+

    cdef cppclass WriteOptions:
        cpp_bool sync
        cpp_bool disableWAL
        cpp_bool ignore_missing_column_families
        cpp_bool no_slowdown
        cpp_bool low_pri
        cpp_bool memtable_insert_hint_per_batch
        const Slice* timestamp
        WriteOptions() nogil except+

    cdef cppclass FlushOptions:
        cpp_bool wait
        cpp_bool allow_write_stall
        FlushOptions() nogil except+

    cdef cppclass CompactionOptions:
        CompressionType compression
        uint64_t output_file_size_limit
        uint32_t max_subcompactions
        CompactionOptions() nogil except+

    ctypedef enum BottommostLevelCompaction:
        blc_skip "rocksdb::BottommostLevelCompaction::kSkip"
        blc_is_filter "rocksdb::BottommostLevelCompaction::kIfHaveCompactionFilter"
        blc_force "rocksdb::BottommostLevelCompaction::kForce"
        blc_force_optimized "rocksdb::BottommostLevelCompaction::kForceOptimized"

    cdef cppclass CompactRangeOptions:
        cpp_bool exclusive_manual_compaction
        cpp_bool change_level
        int target_level
        uint32_t target_path_id
        BottommostLevelCompaction bottommost_level_compaction
        cpp_bool allow_write_stall
        uint32_t max_subcompactions

    cdef cppclass IngestExternalFileOptions:
        cpp_bool move_files
        cpp_bool failed_move_fall_back_to_copy
        cpp_bool snapshot_consistency
        cpp_bool allow_global_seqno
        cpp_bool allow_blocking_flush
        cpp_bool ingest_behind
        cpp_bool write_global_seqno
        cpp_bool verify_checksums_before_ingest
        size_t verify_checksums_readahead_size
        cpp_bool verify_file_checksum

    ctypedef enum TraceFilterType:
        kTraceFilterNone
        kTraceFilterGet
        kTraceFilterWrite

    cdef cppclass TraceOptions:
        uint64_t max_trace_file_size
        uint64_t sampling_frequency
        uint64_t filter

    cdef cppclass ImportColumnFamilyOptions:
        cpp_bool move_files

    cdef cppclass SizeApproximationOptions:
        cpp_bool include_memtabtles
        cpp_bool include_files
        double files_size_error_margin
