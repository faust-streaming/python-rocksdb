from libc.stdint cimport uint64_t, uint32_t
from .slice_ cimport Slice
from libcpp.string cimport string
from libcpp cimport bool as cpp_bool

cdef extern from "rocksdb/types.h" namespace "rocksdb":
    ctypedef uint64_t SequenceNumber

    cdef enum EntryType:
        kEntryPut
        kEntryDelete
        kEntrySingleDelete
        kEntryMerge
        kEntryRangeDeletion
        kEntryBlobIndex
        kEntryOther

    cdef cppclass FullKey:
        Slice user_key
        SequenceNumber sequence
        EntryType type

        FullKey() except+
        FullKey(const Slice&, const SequenceNumber&, EntryType) except+
        string DebugString(cpp_bool hex) nogil except+
        void clear() nogil except+

    cpp_bool ParseFullKey(const Slice&, FullKey*)
