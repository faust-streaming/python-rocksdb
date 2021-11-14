from libcpp cimport bool as cpp_bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libc.stdint cimport uint64_t
from libc.stdint cimport int64_t
from .logger cimport Logger
from .types cimport SequenceNumber

cdef extern from "rocksdb/metadata.h" namespace "rocksdb":
    cdef cppclass ColumnFamilyMetaData:
        ColumnFamilyMetaData() except+
        ColumnFamilyMetaData(const string&, uint64_t,
                             const vector[LevelMetaData]&&) except+
        uint64_t size
        uint64_t file_count
        string name
        vector[LevelMetaData] levels

    cdef cppclass LevelMetaData:
        int level
        uint64_t size
        vector[SstFileMetaData] files

    cdef cppclass SstFileMetaData:
        uint64_t size
        string name
        uint64_t file_number
        string db_path
        SequenceNumber smallest_seqno
        SequenceNumber largest_seqno
        string smallestkey
        string largestkey
        uint64_t num_reads_sampled
        cpp_bool being_compacted
        uint64_t num_entries
        uint64_t num_deletions
        uint64_t oldest_blob_file_number
        uint64_t oldest_ancester_time
        uint64_t file_creation_time
        string file_checksum
        string file_checksum_func_name

    cdef cppclass LiveFileMetaData(SstFileMetaData):
        string column_family_name
        int level
        uint64_t size
        string smallestkey
        string largestkey
        SequenceNumber smallest_seqno
        SequenceNumber largest_seqno

    cdef cppclass ExportImportFilesMetaData:
        string db_comparator_name
        vector[LiveFileMetaData] files

