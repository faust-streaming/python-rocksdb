from libc.stdint cimport uint32_t, uint8_t, uint64_t
from libcpp.memory cimport shared_ptr
from libcpp.string cimport string
from libcpp cimport bool as cpp_bool
from libcpp.map cimport map
from .status cimport Status

cdef extern from "rocksdb/statistics.h" namespace "rocksdb":
    ctypedef enum StatsLevel:
        kExceptHistogramOrTimers
        kExceptTimers
        kExceptDetailedTimers
        kExceptTimeForMutex
        kAll

    cdef cppclass HistogramData:
        double median
        double percentile95
        double percentile99
        double average
        double standard_deviation
        double max
        uint64_t count
        uint64_t sum
        double min

    cdef cppclass Statistics:
        const char* Type() nogil except+
        uint64_t getTickerCount(uint32_t) nogil except+
        void histogramData(uint32_t type,
                           HistogramData* const) nogil except+
        string getHistogramString(uint32_t) nogil except+
        void recordTick(uint32_t, uint64_t) nogil except+
        void setTickerCount(uint32_t tickerType, uint64_t count) nogil except+
        uint64_t getAndResetTickerCount(uint32_t) nogil except+
        void reportTimeToHistogram(uint32_t, uint64_t) nogil except+
        void measureTime(uint32_t, uint64_t) nogil except+
        void recordInHistogram(uint32_t, uint64_t) nogil except+
        Status Reset() nogil except+
        string ToString() nogil except+
        cpp_bool getTickerMap(map[string, uint64_t]*) nogil except+
        cpp_bool HistEnabledForType(uint32_t type) nogil except+
        void set_stats_level(StatsLevel) nogil except+
        StatsLevel get_stats_level() nogil except+
