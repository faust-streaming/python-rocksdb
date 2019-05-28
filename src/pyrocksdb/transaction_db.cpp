#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

class Transaction_: public Transaction {
  public:
    using Transaction::Transaction;
    void SetSnapshot() override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            SetSnapshot
      );
    }

    void SetSnapshotOnNextOperation(std::shared_ptr<TransactionNotifier> notifier = nullptr) override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            SetSnapshotOnNextOperation,
            notifier
      );
    }

    const Snapshot* GetSnapshot() const override {
        PYBIND11_OVERLOAD_PURE(
            const Snapshot*,
            Transaction,
            GetSnapshot,
      );
    }

    void ClearSnapshot () override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            ClearSnapshot,
      );
    }

    Status Prepare() override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Prepare,
      );
    }

    Status Commit() override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Commit,
      );
    }

    Status Rollback() override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Rollback,
      );
    }

    void SetSavePoint() override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            SetSavePoint,
      );
    }

    Status RollbackToSavePoint() override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            RollbackToSavePoint
      );
    }

    Status PopSavePoint() override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            PopSavePoint
      );
    }

    // std::unique_ptr<Blob> XXX(const ReadOptions& options, const std::string& key) {
      // std::unique_ptr<Blob> blob;
      // blob->st = Get(options, key, &blob->data);
      // return blob;
    // }

    Status Get(const ReadOptions& options,
                     ColumnFamilyHandle* column_family, const Slice& key,
                     std::string* value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Get,
            options,
            column_family,
            key,
            value
      );
    }

    Status Get(const ReadOptions& options, const Slice& key,
                     std::string* value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Get,
            options,
            key,
            value
      );
    }

    std::vector<Status> MultiGet(const ReadOptions& options,
      const std::vector<ColumnFamilyHandle*>& column_family,
      const std::vector<Slice>& keys, std::vector<std::string>* values) override {
        PYBIND11_OVERLOAD_PURE(
            std::vector<Status>,
            Transaction,
            MultiGet,
            options,
            column_family,
            keys,
            values
      );
    }

    std::vector<Status> MultiGet(const ReadOptions& options,
                                         const std::vector<Slice>& keys,
                                         std::vector<std::string>* values) override {
        PYBIND11_OVERLOAD_PURE(
            std::vector<Status>,
            Transaction,
            MultiGet,
            options,
            keys,
            values
      );
      
    }

    Status GetForUpdate(const ReadOptions& options,
                                ColumnFamilyHandle* column_family,
                                const Slice& key, std::string* value,
                                bool exclusive = true,
                                const bool do_validate = true) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            GetForUpdate,
            options,
            column_family,
            key,
            value,
            exclusive,
            do_validate
      );
    }

    Status GetForUpdate(const ReadOptions& options, const Slice& key,
                                std::string* value, bool exclusive = true,
                                const bool do_validate = true) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            GetForUpdate,
            options,
            key,
            value,
            exclusive,
            do_validate
      );
    }

    std::vector<Status> MultiGetForUpdate(
        const ReadOptions& options,
        const std::vector<ColumnFamilyHandle*>& column_family,
        const std::vector<Slice>& keys, std::vector<std::string>* values) override {

        PYBIND11_OVERLOAD_PURE(
            std::vector<Status>,
            Transaction,
            MultigetForUpdate,
            options,
            column_family,
            keys,
            values
      );
    }

    std::vector<Status> MultiGetForUpdate(
        const ReadOptions& options, const std::vector<Slice>& keys,
        std::vector<std::string>* values) override {

        PYBIND11_OVERLOAD_PURE(
            std::vector<Status>,
            Transaction,
            MultigetForUpdate,
            options,
            keys,
            values
      );
    }

    Iterator* GetIterator(const ReadOptions& read_options) override {
        PYBIND11_OVERLOAD_PURE(
            Iterator*,
            Transaction,
            GetIterator,
            read_options
      );
    }

    Iterator* GetIterator(const ReadOptions& read_options,
                                  ColumnFamilyHandle* column_family) override {
        PYBIND11_OVERLOAD_PURE(
            Iterator*,
            Transaction,
            GetIterator,
            read_options,
            column_family
      );
    }

    Status Put(ColumnFamilyHandle* column_family, const Slice& key,
               const Slice& value, const bool assume_tracked = false) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Put,
            column_family,
            key,
            value,
            assume_tracked
      );
    }

    Status Put(const Slice& key, const Slice& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Put,
            key,
            value
      );
    }

    Status Put(ColumnFamilyHandle* column_family, const SliceParts& key,
               const SliceParts& value,
               const bool assume_tracked = false) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Put,
            column_family,
            key,
            value,
            assume_tracked
      );
    }

    Status Put(const SliceParts& key, const SliceParts& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Put,
            key,
            value
      );
    }

    Status Merge(ColumnFamilyHandle* column_family, const Slice& key,
                 const Slice& value,
                 const bool assume_tracked = false) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Merge,
            column_family,
            key,
            value,
            assume_tracked
      );
    }

    Status Merge(const Slice& key, const Slice& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Merge,
            key,
            value
      );
    }

    Status Delete(ColumnFamilyHandle* column_family, const Slice& key,
                  const bool assume_tracked = false) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Delete,
            column_family,
            key,
            assume_tracked
      );
    }

    Status Delete(const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Delete,
            key
      );
    }

    Status Delete(ColumnFamilyHandle* column_family,
                  const SliceParts& key,
                  const bool assume_tracked = false) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Delete,
            column_family,
            key,
            assume_tracked
      );
    }

    Status Delete(const SliceParts& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            Delete,
            key
      );
    }

    Status SingleDelete(ColumnFamilyHandle* column_family,
                        const Slice& key,
                        const bool assume_tracked = false) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            SingleDelete,
            column_family,
            key,
            assume_tracked
      );
    }

    Status SingleDelete(const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            SingleDelete,
            key
      );
    }

    Status SingleDelete(ColumnFamilyHandle* column_family,
                        const SliceParts& key,
                        const bool assume_tracked = false) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            SingleDelete,
            column_family,
            key,
            assume_tracked
      );
    }

    Status SingleDelete(const SliceParts& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            SingleDelete,
            key
      );
    }

    Status PutUntracked(ColumnFamilyHandle* column_family,
                        const Slice& key, const Slice& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            PutUntracked,
            column_family,
            key,
            value
      );
    }
      
    Status PutUntracked(const Slice& key, const Slice& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            PutUntracked,
            key,
            value
      );
    }

    Status PutUntracked(ColumnFamilyHandle* column_family,
                        const SliceParts& key,
                        const SliceParts& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            PutUntracked,
            column_family,
            key,
            value
      );
    }

    Status PutUntracked(const SliceParts& key,
                        const SliceParts& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            PutUntracked,
            key,
            value
      );
      
    }

    Status MergeUntracked(ColumnFamilyHandle* column_family,
                          const Slice& key, const Slice& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            MergeUntracked,
            column_family,
            key,
            value
      );
    }

    Status MergeUntracked(const Slice& key, const Slice& value) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            MergeUntracked,
            key,
            value
      );
    }

    Status DeleteUntracked(ColumnFamilyHandle* column_family,
                           const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            DeleteUntracked,
            column_family,
            key
      );
      
    }

    Status DeleteUntracked(const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            DeleteUntracked,
            key
      );
    }

    Status DeleteUntracked(ColumnFamilyHandle* column_family,
                           const SliceParts& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            DeleteUntracked,
            column_family,
            key
      );
    }

    Status DeleteUntracked(const SliceParts& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            DeleteUntracked,
            key
      );
    }

    Status SingleDeleteUntracked(ColumnFamilyHandle* column_family,
                                 const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            SingleDeleteUntracked,
            column_family,
            key
      );
    }

    Status SingleDeleteUntracked(const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            SingleDeleteUntracked,
            key
      );
    }

    void PutLogData(const Slice& blob) override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            PutLogData,
            blob
      );
    }

    void DisableIndexing() override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            DisableIndexing
      );
    }

    void EnableIndexing() override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            EnableIndexing
      );
    }

    uint64_t GetNumKeys() const override {
        PYBIND11_OVERLOAD_PURE(
            uint64_t,
            Transaction,
            GetNumKeys
      );
    }

    uint64_t GetNumPuts() const override {
        PYBIND11_OVERLOAD_PURE(
            uint64_t,
            Transaction,
            GetNumPuts
      );
    }

    uint64_t GetNumDeletes() const override {
        PYBIND11_OVERLOAD_PURE(
            uint64_t,
            Transaction,
            GetNumPuts
      );
    }

    uint64_t GetNumMerges() const override {
        PYBIND11_OVERLOAD_PURE(
            uint64_t,
            Transaction,
            GetNumMerges
      );
    }

    uint64_t GetElapsedTime() const override {
        PYBIND11_OVERLOAD_PURE(
            uint64_t,
            Transaction,
            GetElapsedTime
      );
    }

    WriteBatchWithIndex* GetWriteBatch() override {
        PYBIND11_OVERLOAD_PURE(
            WriteBatchWithIndex *,
            Transaction,
            GetWriteBatch
      );
    }

    void SetLockTimeout(int64_t timeout) override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            SetLockTimeout,
            timeout
      );
    }

    WriteOptions* GetWriteOptions() override {
        PYBIND11_OVERLOAD_PURE(
            WriteOptions*,
            Transaction,
            GetWriteOptions
      );
    }

    void SetWriteOptions(const WriteOptions& write_options) override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            SetWriteOptions
      );
    }

    void UndoGetForUpdate(ColumnFamilyHandle* column_family,
                          const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            UndoGetForUpdate,
            column_family,
            key
      );
    }

    void UndoGetForUpdate(const Slice& key) override {
        PYBIND11_OVERLOAD_PURE(
            void,
            Transaction,
            UndoGetForUpdate,
            key
      );
    }

    Status RebuildFromWriteBatch(WriteBatch* src_batch) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            RebuildFromWriteBatch,
            src_batch
      );
    }

    WriteBatch* GetCommitTimeWriteBatch() override {
        PYBIND11_OVERLOAD_PURE(
            WriteBatch*,
            Transaction,
            GetCommitTimeWriteBatch
      );
    }

    Status SetName(const TransactionName& name) override {
        PYBIND11_OVERLOAD_PURE(
            Status,
            Transaction,
            SetName
      );
    }
};

class TransactionWrapper {
  public:
    TransactionWrapper(Transaction* p){txn = p;}
    ~TransactionWrapper(){delete txn;}
    std::unique_ptr<Blob> Get(const ReadOptions& options, const Slice& key) {
      std::unique_ptr<Blob> blob(new Blob());
      blob->status = txn->Get(options, key, &blob->data);
      return blob;
    }

    Status Put(const Slice& key, const Slice& value) {
      return txn->Put(key, value);
    }

    Status Commit() {
      return txn->Commit();
    }

    Status GetForUpdate(const ReadOptions& options, const Slice& key,
                                std::string* value, bool exclusive = true,
                                const bool do_validate = true) {
      return txn->GetForUpdate(options, key, value, exclusive, do_validate);
    }
  private:
    Transaction* txn;

};

class PyTransactionDB {
  public:

    PyTransactionDB(): db_ptr(nullptr) {
      
    }
    ~PyTransactionDB() {
      if (db_ptr != nullptr) {
        delete db_ptr;
      }
    }
    Status Open(const Options& options, const TransactionDBOptions& txn_db_options,
                       const std::string& dbname) {
      if (db_ptr != nullptr) {
        throw std::invalid_argument("db has been opened");
      }
      return TransactionDB::Open(options,  txn_db_options, dbname, &db_ptr);
    }

    std::unique_ptr<Blob> Get(const ReadOptions& options, const std::string& key) {
      if (db_ptr == nullptr) {
        throw std::invalid_argument("db has been opened");
      }
      std::unique_ptr<Blob> blob(new Blob());
      blob->status = db_ptr->Get(options, key, &blob->data);
      return blob;
    }

    Status Put(const WriteOptions& options, const std::string& key,
                     const std::string& value) {
      if (db_ptr == nullptr) {
        throw std::invalid_argument("db has been closed");
      }
      return db_ptr->Put(options, key, value);
    }

    std::unique_ptr<TransactionWrapper> BeginTransaction(const WriteOptions& write_options, const TransactionOptions& txn_options = TransactionOptions()) {
      return std::unique_ptr<TransactionWrapper>(new TransactionWrapper(db_ptr->BeginTransaction(write_options, txn_options)));
    }
  private:
    TransactionDB* db_ptr;

};

void init_transaction_db(py::module & m) {
    py::class_<Transaction, Transaction_ /* <--- trampoline*/> transaction(m, "transaction");
    transaction
        .def(py::init<>());
        // .def("get", &Transaction::ggg);
        //
  py::class_<TransactionWrapper>(m, "transaction_wrapper")
    .def("get", (std::unique_ptr<Blob> (TransactionWrapper::*) (const ReadOptions& options, const std::string& key)) &TransactionWrapper::Get)
    .def("put", (Status (TransactionWrapper::*) (const std::string&, const std::string &)) &TransactionWrapper::Put)
    .def("commit", &TransactionWrapper::Commit)
    .def("get_for_update", &TransactionWrapper::GetForUpdate, py::arg("options"), py::arg("key"), py::arg("value"), py::arg("exclusive")=true, py::arg("do_validate")=true);

  py::class_<PyTransactionDB>(m, "transaction_db")
    .def(py::init<>())
    .def("open", &PyTransactionDB::Open)
    .def("get", &PyTransactionDB::Get)
    .def("put", &PyTransactionDB::Put)
    .def("begin_transaction", &PyTransactionDB::BeginTransaction, py::arg("write_options"), py::arg("txn_options")=TransactionOptions());
}

