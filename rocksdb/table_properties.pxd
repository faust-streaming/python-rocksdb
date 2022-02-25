from libc.stdint cimport uint32_t, uint64_t
from libcpp cimport bool as cpp_bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.map cimport map

from .slice_ cimport Slice
from .status cimport Status
from .types cimport SequenceNumber, EntryType


cdef extern from "rocksdb/table_properties.h" namespace "rocksdb":
    ctypedef map[string, string] UserCollectedProperties

    cdef cppclass TablePropertiesCollector:
        Status Add(const Slice&, const Slice&)
        Status AddUserKey(const Slice&, const Slice&,
                          EntryType, SequenceNumber,
                          uint64_t)
        void BlockAdd(uint64_t,
                      uint64_t,
                      uint64_t)

        Status Finish(UserCollectedProperties*)
        UserCollectedProperties GetReadableProperties()
        const char* Name()
        cpp_bool NeedCompact()

    cdef cppclass TablePropertiesCollectorFactory_Context "rocksdb::TablePropertiesCollectorFactory::Context":
        uint32_t column_family_id
        uint32_t kUnknownColumnFamily

    cdef cppclass TablePropertiesCollectorFactory:
        TablePropertiesCollector* CreateTablePropertiesCollector(
            TablePropertiesCollectorFactory_Context context)
        const char* Name()
        string ToString()

    cdef cppclass TableProperties:
        uint64_t data_size
        uint64_t index_size
        uint64_t index_partitions
        uint64_t top_level_index_size
        uint64_t index_key_is_user_key
        uint64_t index_value_is_delta_encoded
        uint64_t filter_size
        uint64_t raw_key_size
        uint64_t raw_value_size
        uint64_t num_data_blocks
        uint64_t num_entries
        uint64_t num_deletions
        uint64_t num_merge_operands
        uint64_t num_range_deletions
        uint64_t format_version
        uint64_t fixed_key_len
        uint64_t column_family_id
        uint64_t creation_time
        uint64_t oldest_key_time
        uint64_t file_creation_time

        string column_family_name
        string filter_policy_name
        string comparator_name
        string merge_operator_name
        string prefix_extractor_name
        string property_collectors_names
        string compression_name
        string compression_options
        UserCollectedProperties user_collected_properties
        UserCollectedProperties readable_properties
        map[string, uint64_t] properties_offsets
        string ToString(const string&,
                        const string&)
        void Add(const TableProperties&)

