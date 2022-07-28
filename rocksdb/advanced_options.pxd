from libcpp cimport bool as cpp_bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libc.stdint cimport uint64_t
from libc.stdint cimport uint32_t
from libc.stdint cimport int64_t
from libc.stdint cimport int32_t
from .std_memory cimport shared_ptr
from .comparator cimport Comparator
from .merge_operator cimport MergeOperator
from .logger cimport Logger
from .slice_ cimport Slice
from .snapshot cimport Snapshot
from .slice_transform cimport SliceTransform
from .table_factory cimport TableFactory
from .memtablerep cimport MemTableRepFactory
from .universal_compaction cimport CompactionOptionsUniversal
from .cache cimport Cache
from .options cimport Options
from .options cimport CompressionType
from .table_properties cimport TablePropertiesCollectorFactory

cdef extern from "rocksdb/advanced_options.h" namespace "rocksdb":
    ctypedef enum CompactionStyle:
        kCompactionStyleLevel
        kCompactionStyleUniversal
        kCompactionStyleFIFO
        kCompactionStyleNone

    ctypedef enum CompactionPri:
        kByCompensatedSize
        kOldestLargestSeqFirst
        kOldestSmallestSeqFirst
        kMinOverlappingRatio

    cdef cppclass CompactionOptionsFIFO:
        uint64_t max_table_files_size
        cpp_bool allow_compaction
        CompactionOptionsFIFO()
        CompactionOptionsFIFO(uint64_t, cpp_bool)

    cdef cppclass CompressionOptions:
        int window_bits;
        int level;
        int strategy;
        uint32_t max_dict_bytes
        uint32_t zstd_max_train_bytes
        uint32_t parallel_threads
        cpp_bool enabled
        CompressionOptions() except +
        CompressionOptions(int, int, int, int,
                           int, int, cpp_bool) except +

    cdef enum UpdateStatus:
        UPDATE_FAILED
        UPDATED_INPLACE
        UPDATED

    cdef cppclass AdvancedColumnFamilyOptions:
        int max_write_buffer_number
        int min_write_buffer_number_to_merge
        int max_write_buffer_number_to_maintain
        int64_t max_write_buffer_size_to_maintain
        cpp_bool inplace_update_support
        size_t inplace_update_num_locks

        UpdateStatus (*inplace_callback)(char*,
                                         uint32_t*,
                                         Slice,
                                         string*)
        double memtable_prefix_bloom_size_ratio
        cpp_bool memtable_whole_key_filtering
        size_t memtable_huge_page_size
        shared_ptr[const SliceTransform] memtable_insert_with_hint_prefix_extractor
        uint32_t bloom_locality
        size_t arena_block_size
        vector[CompressionType] compression_per_level
        int num_levels
        int level0_slowdown_writes_trigger
        int level0_stop_writes_trigger
        uint64_t target_file_size_base
        int target_file_size_multiplier
        cpp_bool level_compaction_dynamic_level_bytes
        double max_bytes_for_level_multiplier

        vector[int] max_bytes_for_level_multiplier_additional
        uint64_t max_compaction_bytes
        uint64_t soft_pending_compaction_bytes_limit
        uint64_t hard_pending_compaction_bytes_limit

        CompactionStyle compaction_style
        CompactionPri compaction_pri
        CompactionOptionsUniversal compaction_options_universal

        CompactionOptionsFIFO compaction_options_fifo

        uint64_t max_sequential_skip_in_iterations
        shared_ptr[MemTableRepFactory] memtable_factory
        vector[shared_ptr[TablePropertiesCollectorFactory]] table_properties_collector_factories

        size_t max_successive_merges
        cpp_bool optimize_filters_for_hits

        cpp_bool paranoid_file_checks
        cpp_bool force_consistency_checks

        cpp_bool report_bg_io_stats
        uint64_t ttl

        uint64_t periodic_compaction_seconds
        uint64_t sample_for_compression

        AdvancedColumnFamilyOptions();
        AdvancedColumnFamilyOptions(const Options&);
        #  ---------------- OPTIONS NOT SUPPORTED ANYMORE ----------------
        # But kept for compatibality as they are still in the header files.
        int max_mem_compaction_level
        double soft_rate_limit
        double hard_rate_limit
        unsigned int rate_limit_delay_max_milliseconds
        cpp_bool purge_redundant_kvs_while_flush
