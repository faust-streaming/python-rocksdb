from . cimport options
from libc.stdint cimport uint64_t, uint32_t
from .status cimport Status
from libcpp cimport bool as cpp_bool
from libcpp.string cimport string
from libcpp.vector cimport vector
from .types cimport SequenceNumber
from libcpp.map cimport map
from libcpp.unordered_map cimport unordered_map
from libcpp.memory cimport shared_ptr
from .types cimport SequenceNumber
from .slice_ cimport Slice
from .snapshot cimport Snapshot
from .iterator cimport Iterator
from .env cimport Env
from .metadata cimport ColumnFamilyMetaData
from .metadata cimport LiveFileMetaData
from .metadata cimport ExportImportFilesMetaData
from .table_properties cimport TableProperties

cdef extern from "rocksdb/write_batch.h" namespace "rocksdb":
    cdef cppclass WriteBatch:
        WriteBatch() nogil except+
        WriteBatch(string) nogil except+
        void Put(const Slice&, const Slice&) nogil except+
        void Put(ColumnFamilyHandle*, const Slice&, const Slice&) nogil except+
        void Merge(const Slice&, const Slice&) nogil except+
        void Merge(ColumnFamilyHandle*, const Slice&, const Slice&) nogil except+
        void Delete(const Slice&) nogil except+
        void Delete(ColumnFamilyHandle*, const Slice&) nogil except+
        void PutLogData(const Slice&) nogil except+
        void Clear() nogil except+
        const string& Data() nogil except+
        int Count() nogil except+

cdef extern from "cpp/write_batch_iter_helper.hpp" namespace "py_rocks":
    cdef enum BatchItemOp "RecordItemsHandler::Optype":
        BatchItemOpPut "py_rocks::RecordItemsHandler::Optype::PutRecord"
        BatchItemOpMerge "py_rocks::RecordItemsHandler::Optype::MergeRecord"
        BatchItemOpDelte "py_rocks::RecordItemsHandler::Optype::DeleteRecord"

    cdef cppclass BatchItem "py_rocks::RecordItemsHandler::BatchItem":
        BatchItemOp op
        uint32_t column_family_id
        Slice key
        Slice value

    Status get_batch_items(WriteBatch* batch, vector[BatchItem]* items)


cdef extern from "rocksdb/db.h" namespace "rocksdb":
    ctypedef uint64_t SequenceNumber
    string kDefaultColumnFamilyName

    #todo TableProperties
    ctypedef unordered_map[string, shared_ptr[const TableProperties]] TablePropertiesCollection

    cdef struct GetMergeOperandsOptions:
        uint64_t expected_max_number_of_operands

    cdef struct LiveFileMetaData:
        string name
        int level
        uint64_t size
        string smallestkey
        string largestkey
        SequenceNumber smallest_seqno
        SequenceNumber largest_seqno

    # cdef struct SstFileMetaData:
    #     uint64_t size
    #     string name
    #     uint64_t file_number
    #     string db_path
    #     string smallestkey
    #     string largestkey
    #     SequenceNumber smallest_seqno
    #     SequenceNumber largest_seqno

    # cdef struct LevelMetaData:
    #     int level
    #     uint64_t size
    #     string largestkey
    #     LiveFileMetaData files

    cdef struct ColumnFamilyMetaData:
        uint64_t size
        uint64_t file_count
        # string largestkey
        # LevelMetaData levels

    cdef cppclass Range:
        Range(const Slice&, const Slice&)

    cdef cppclass DB:
        Status Put(
            const options.WriteOptions&,
            ColumnFamilyHandle*,
            const Slice&,
            const Slice&) nogil except+

        Status Delete(
            const options.WriteOptions&,
            ColumnFamilyHandle*,
            const Slice&) nogil except+

        Status DeleteRange(
            const options.WriteOptions&,
            ColumnFamilyHandle*,
            const Slice&,
            const Slice&) nogil except+

        Status Merge(
            const options.WriteOptions&,
            ColumnFamilyHandle*,
            const Slice&,
            const Slice&) nogil except+

        Status Write(
            const options.WriteOptions&,
            WriteBatch*) nogil except+

        Status Get(
            const options.ReadOptions&,
            ColumnFamilyHandle*,
            const Slice&,
            string*) nogil except+

        # Status GetMergeOperands(
        #     const options.ReadOptions&,
        #     ColumnFamilyHandle*,
        #     const Slice&,
        #     PinnableSlice*,
        #     GetMergeOperandsOptions*,
        #     uint64_t*) nogil except+;

        vector[Status] MultiGet(
            const options.ReadOptions&,
            const vector[ColumnFamilyHandle*]&,
            const vector[Slice]&,
            vector[string]*) nogil except+

        cpp_bool KeyMayExist(
            const options.ReadOptions&,
            ColumnFamilyHandle*,
            Slice&,
            string*,
            cpp_bool*) nogil except+

        cpp_bool KeyMayExist(
            const options.ReadOptions&,
            ColumnFamilyHandle*,
            Slice&,
            string*) nogil except+

        Iterator* NewIterator(
            const options.ReadOptions&,
            ColumnFamilyHandle*) nogil except+

        void NewIterators(
            const options.ReadOptions&,
            vector[ColumnFamilyHandle*]&,
            vector[Iterator*]*) nogil except+

        const Snapshot* GetSnapshot() nogil except+

        void ReleaseSnapshot(const Snapshot*) nogil except+

        cpp_bool GetProperty(
            ColumnFamilyHandle*,
            const Slice&,
            string*) nogil except+

        cpp_bool GetMapProperty(
            ColumnFamilyHandle*,
            const Slice&,
            map[string, string]*) nogil except+

        cpp_bool GetIntProperty(
            ColumnFamilyHandle*,
            const Slice&,
            uint64_t*) nogil except+

        Status ResetStats() nogil except+

        cpp_bool GetAggregatedIntProperty(
            const Slice&,
            uint64_t*) nogil except+

        void GetApproximateSizes(
            ColumnFamilyHandle*,
            const Range*
            int,
            uint64_t*) nogil except+

        Status CompactRange(
            const options.CompactRangeOptions&,
            ColumnFamilyHandle*,
            const Slice*,
            const Slice*) nogil except+

        Status SetOptions(
            ColumnFamilyHandle*,
            const unordered_map[string, string]&) nogil except+

        Status EnableAutoCompaction(
            const vector[ColumnFamilyHandle*]&) nogil except+

        void DisableManualCompaction() nogil except+
        void EnableManualCompaction() nogil except+

        Status CreateColumnFamily(
            const options.ColumnFamilyOptions&,
            const string&,
            ColumnFamilyHandle**) nogil except+

        Status CreateColumnFamilies(
            const options.ColumnFamilyOptions&,
            const vector[string]&,
            vector[ColumnFamilyHandle*]*) nogil except+

        Status CreateColumnFamilies(
            const vector[ColumnFamilyDescriptor]&,
            vector[ColumnFamilyHandle*]*) nogil except+

        Status DropColumnFamily(
            ColumnFamilyHandle*) nogil except+

        Status DropColumnFamilies(
            vector[ColumnFamilyHandle*]*) nogil except+

        Status DestroyColumnFamilyHandle(
            ColumnFamilyHandle*) nogil except+

        int NumberLevels(ColumnFamilyHandle*) nogil except+
        int MaxMemCompactionLevel(ColumnFamilyHandle*) nogil except+
        int Level0StopWriteTrigger(ColumnFamilyHandle*) nogil except+
        const string& GetName() nogil except+
        Env* GetEnv() nogil except+
        # TODO Mandar FileSystem* GetFileSystem()  nogil except+
        const options.Options& GetOptions(ColumnFamilyHandle*) nogil except+
        Status Flush(const options.FlushOptions&, ColumnFamilyHandle*) nogil except+
        Status Flush(
            const options.FlushOptions&,
            const vector[ColumnFamilyHandle*]&) nogil except+
        Status FlushWAL(bool_cpp) nogil except+
        Status SyncWAL() nogil except+
        Status LockWAL() nogil except+
        Status UnlockWAL() nogil except+

        SequenceNumber GetLatestSequenceNumber()
        cpp_bool SetPreserveDeletesSequenceNumber(SequenceNumber) nogil except+

        Status DisableFileDeletions() nogil except+
        Status EnableFileDeletions() nogil except+

        Status Close() nogil except+
        Status Resume() nogil except+
        Status PauseBackgroundWork() nogil except+
        Status ContinueBackgroundWork() nogil except+

        Status GetDbIdentity(string&) nogil except+
        ColumnFamilyHandle* DefaultColumnFamily()

        # Following defined for #ifndef ROCKDDB_LITE
        Status GetLiveFiles(vector[string]&,
                            uint64_t*,
                            cpp_bool) nogil except+
        # TODO: Status GetSortedWalFiles(VectorLogPtr& files)
        # TODO: Status GetCurrentWalFile(std::unique_ptr<LogFile>*)
        # TODO: Status GetUpdatesSince(
        #    SequenceNumber seq_number,
        #    unique_ptr[TransactionLogIterator]*,
        #    const TransactionLogIterator::ReadOptions&)
        Status GetCreationTimeOfOldestFile(uint64_t*) nogil except+
        Status DeleteFile(string) nogil except+
        void GetLiveFilesMetaData(vector[LiveFileMetaData]*) nogil except+
        void GetColumnFamilyMetaData(ColumnFamilyHandle*, ColumnFamilyMetaData*) nogil except+

        Status IngestExternalFile(
            ColumnFamilyHandle*,
            const vector[string]&,
            const options.IngestExternalFileOptions&) nogil except+
        Status CreateColumnFamilyWithImport(
            const options.ColumnFamilyOptions&,
            const string&,
            const options.ImportColumnFamilyOptions&,
            const ExportImportFilesMetaData&,
            ColumnFamilyHandle**) nogil except+
        Status VerifyChecksum(const options.ReadOptions&) nogil except+

        DB* GetRootDB() nogil except+
        Status GetPropertiesOfAllTables(
            ColumnFamilyHandle*,
            TablePropertiesCollection*) nogil except+
        Status GetPropertiesOfTablesInRange(
            ColumnFamilyHandle*, const Range*, size_t,
            TablePropertiesCollection*) nogil except+

        Status SuggestCompactRange(ColumnFamilyHandle*,
                                     const Slice*,
                                     const Slice*) nogil except+
        Status PromoteL0(ColumnFamilyHandle*, int) nogil except+
        #TODO Status StartTrace(const TraceOptions&,
        #                     std::unique_ptr<TraceWriter>&&) nogil except+
        Status EndTrace() nogil except+
        # TODO Status StartBlockCacheTrace(
        #     const TraceOptions&,
        #     std::unique_ptr<TraceWriter>&&) nogil except+
        Status EndBlockCacheTrace() nogil except+
        Status TryCatchUpWithPrimary() nogil except+

    cdef Status DB_Open "rocksdb::DB::Open"(
        const options.Options&,
        const string&,
        DB**) nogil except+

    cdef Status DB_Open_ColumnFamilies "rocksdb::DB::Open"(
        const options.Options&,
        const string&,
        const vector[ColumnFamilyDescriptor]&,
        vector[ColumnFamilyHandle*]*,
        DB**) nogil except+

    cdef Status DB_OpenForReadOnly "rocksdb::DB::OpenForReadOnly"(
        const options.Options&,
        const string&,
        DB**,
        cpp_bool) nogil except+

    cdef Status DB_OpenForReadOnly_ColumnFamilies "rocksdb::DB::OpenForReadOnly"(
        const options.Options&,
        const string&,
        const vector[ColumnFamilyDescriptor]&,
        vector[ColumnFamilyHandle*]*,
        DB**,
        cpp_bool) nogil except+

    cdef Status DestroyDB(
            const string&,
            const options.Options&,
            const vector[ColumnFamilyDescriptor]&) nogil except+

    cdef Status RepairDB(const string& dbname, const options.Options&)
    cdef Status RepairDB(const string& dbname, const options.Options&, const vector[ColumnFamilyDescriptor]&)

    cdef Status ListColumnFamilies "rocksdb::DB::ListColumnFamilies" (
        const options.Options&,
        const string&,
        vector[string]*) nogil except+

    cdef cppclass ColumnFamilyHandle:
        const string& GetName() nogil except+
        int GetID() nogil except+

    cdef cppclass ColumnFamilyDescriptor:
        ColumnFamilyDescriptor() nogil except+
        ColumnFamilyDescriptor(
	        const string&,
            const options.ColumnFamilyOptions&) nogil except+
        string name
        options.ColumnFamilyOptions options

cdef extern from "rocksdb/convenience.h" namespace "rocksdb":
    void CancelAllBackgroundWork(DB*, cpp_bool) nogil except+
