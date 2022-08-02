from libcpp cimport bool as cpp_bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.memory cimport unique_ptr
from libc.stdint cimport uint32_t
from .slice_ cimport Slice

cdef extern from "rocksdb/compaction_filter.h" namespace "rocksdb":
    cdef cppclass CompactionFilterContext:
        cpp_bool is_full_compaction
        cpp_bool is_manual_compaction

    cdef enum ValueType "rocksdb::CompactionFilter::ValueType":
        ValueType_kValue "rocksdb::CompactionFilter::ValueType::kValue"
        ValueType_kMergeOperand "rocksdb::CompactionFilter::ValueType::kMergeOperand"
        ValueType_kBlobIndex "rocksdb::CompactionFilter::ValueType::kBlobIndex"

    cdef enum Decision "rocksdb::CompactionFilter::Decision":
        Decision_kKeep "rocksdb::CompactionFilter::Decision::kKeep"
        Decision_kRemove "rocksdb::CompactionFilter::Decision::kRemove"
        Decision_kChangeValue "rocksdb::CompactionFilter::Decision::kChangeValue"
        Decision_kRemoveAndSkipUntil "rocksdb::CompactionFilter::Decision::kRemoveAndSkipUntil"

    cdef enum BlobDecision "rocksdb::CompactionFilter::BlobDecision":
        BlobDecision_kKeep "rocksdb::CompactionFilter::BlobDecision::kKeep"
        BlobDecision_kChangeValue "rocksdb::CompactionFilter::BlobDecision::kChangeValue"
        BlobDecision_kCorruption "rocksdb::CompactionFilter::BlobDecision::kCorruption"
        BlobDecision_kIOError "rocksdb::CompactionFilter::BlobDecision::kIOError"

    cdef cppclass Context "rocksdb::CompactionFilter::Context":
        cpp_bool is_full_compaction
        cpp_bool is_manual_compaction
        uint32_t column_family_id

    cdef cppclass CompactionFilter:
        cpp_bool Filter(int, const Slice&,
                        const Slice&,
                        string*,
                        cpp_bool*) nogil except+
        cpp_bool FilterMergeOperand(int, const Slice&,
                                    const Slice&) nogil except+
        Decision FilterV2(int level, const Slice&, ValueType,
                          const Slice&, string*,
                          string*)
        BlobDecision PrepareBlobOutput(const Slice&,
                                       const Slice&,
                                       string*)
        cpp_bool IgnoreSnapshots()
        const char* Name()

    cdef cppclass CompactionFilterFactory:
        unique_ptr[CompactionFilter] CreateCompactionFilter(
            const Context&)
        const char* Name()
