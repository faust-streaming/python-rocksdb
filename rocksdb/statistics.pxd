from libc.stdint cimport uint32_t, uint8_t
from std_memory cimport shared_ptr

cdef extern from "rocksdb/statistics.h" namespace "rocksdb":
    ctypedef enum StatsLevel:
        kExceptHistogramOrTimers
        kExceptTimers
        kExceptDetailedTimers
        kExceptTimeForMutex
        kAll

    cdef cppclass Statistics:
        void set_stats_level(StatsLevel) nogil except+
