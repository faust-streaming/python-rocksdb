from . cimport options
from libc.stdint cimport uint64_t, uint32_t, int64_t
from .status cimport Status
from libcpp cimport bool as cpp_bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.memory cimport shared_ptr
from .types cimport SequenceNumber
from .db cimport DB, WriteBatch, ColumnFamilyDescriptor, ColumnFamilyHandle
from .stackable_db cimport StackableDB

cdef extern from "rocksdb/utilities/transaction_db.h" namespace "rocksdb":
    cpdef enum TxnDBWritePolicy:
        WRITE_COMMITTED
        WRITE_PREPARED
        WRITE_UNPREPARED

    cdef cppclass TransactionDBOptions:
        int64_t max_num_locks
        uint32_t max_num_deadlocks
        size_t num_stripes
        int64_t transaction_lock_timeout
        int64_t default_lock_timeout
        # TODO shared_ptr[TransactionDBMutexFactory] custom_mutex_factory
        TxnDBWritePolicy write_policy
        cpp_bool rollback_merge_operands
        cpp_bool skip_concurrency_control
        int64_t default_write_batch_flush_threshold

    cdef cppclass TransactionOptions:
        cpp_bool set_snapshot
        cpp_bool deadlock_detect
        cpp_bool use_only_the_last_commit_time_batch_for_recovery
        int64_t lock_timeout
        int64_t expiration
        int64_t deadlock_detect_depth
        size_t max_write_batch_size
        cpp_bool skip_concurrency_control
        cpp_bool skip_prepare
        int64_t write_batch_flush_threshold

    cdef cppclass TransactionDBWriteOptimizations:
        cpp_bool skip_concurrency_control
        cpp_bool skip_duplicate_key_check

    cdef cppclass TransactionDB(StackableDB):
        Status Write(const options.WriteOptions&,
                     const TransactionDBWriteOptimizations&,
                     WriteBatch*) nogil except+

    cdef Status TransactionDB_Open "rocksdb::TransactionDB::Open"(
        const options.Options&,
        const TransactionDBOptions&,
        const string&,
        TransactionDB**) nogil except+

    cdef Status TransactionDB_Open_ColumnFamilies "rocksdb::TransactionDB::Open"(
        const options.DBOptions&,
        const TransactionDBOptions&,
        const string&,
        const vector[ColumnFamilyDescriptor]&,
        vector[ColumnFamilyHandle*]*,
        TransactionDB**) nogil except+
