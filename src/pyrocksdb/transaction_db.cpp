#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"
#include <iostream>

namespace py = pybind11;

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

     // std::unique_ptr<const Snapshot> GetSnapshot() {
     const Snapshot* GetSnapshot() {
      // return std::unique_ptr<const Snapshot>(txn->GetSnapshot());
       // std::unique_ptr<const Snapshot> b;
       // return b;
       return txn->GetSnapshot();
    }
     void ClearSnapshot() {
       txn->ClearSnapshot();
     }

     void Rollback() {
       txn->Rollback();
     }

     void RollbackToSavePoint() {
       txn->RollbackToSavePoint();
     }

     void SetSavePoint() {
       txn->SetSavePoint();
     }

     void SetSnapshot() {
       txn->SetSnapshot();
     }

    std::unique_ptr<Blob> GetForUpdate(const ReadOptions& options, const std::string& key, bool exclusive = true,
                                const bool do_validate = true) {
      std::unique_ptr<Blob> b(new Blob);
      b->status = txn->GetForUpdate(options, key, &b->data, exclusive, do_validate);
      return b;
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

    void Close() {
      if (db_ptr == nullptr) {
        throw std::invalid_argument("db has been closed");
      }
      delete db_ptr;
      db_ptr = nullptr;
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
        throw std::invalid_argument("db has been closed");
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

    const Snapshot * GetSnapshot() {
      if (db_ptr == nullptr) {
        throw std::invalid_argument("db has been closed");
      }
      return db_ptr->GetSnapshot();
    }

    void ReleaseSnapshot(const Snapshot* snapshot) {
      return db_ptr->ReleaseSnapshot(snapshot);
    }

    std::unique_ptr<TransactionWrapper> BeginTransaction(const WriteOptions& write_options, const TransactionOptions& txn_options = TransactionOptions()) {
      return std::unique_ptr<TransactionWrapper>(new TransactionWrapper(db_ptr->BeginTransaction(write_options, txn_options)));
    }
  private:
    TransactionDB* db_ptr;

};


void init_transaction_db(py::module & m) {
  py::class_<TransactionWrapper>(m, "transaction_wrapper")
    .def("get", (std::unique_ptr<Blob> (TransactionWrapper::*) (const ReadOptions& options, const std::string& key)) &TransactionWrapper::Get)
    .def("put", (Status (TransactionWrapper::*) (const std::string&, const std::string &)) &TransactionWrapper::Put)
    .def("commit", &TransactionWrapper::Commit)
    .def("get_for_update", &TransactionWrapper::GetForUpdate, py::arg("options"), py::arg("key"), py::arg("exclusive")=true, py::arg("do_validate")=true)
    .def("get_snapshot", &TransactionWrapper::GetSnapshot)
    .def("clear_snapshot", &TransactionWrapper::ClearSnapshot)
    .def("set_snapshot", &TransactionWrapper::SetSnapshot)
    .def("set_savepoint", &TransactionWrapper::SetSavePoint)
    .def("rollback", &TransactionWrapper::Rollback)
    .def("rollback_to_savepoint", &TransactionWrapper::RollbackToSavePoint);

  py::class_<PyTransactionDB>(m, "transaction_db")
    .def(py::init<>())
    .def("open", &PyTransactionDB::Open)
    .def("close", &PyTransactionDB::Close)
    .def("get", &PyTransactionDB::Get)
    .def("put", &PyTransactionDB::Put)
    .def("get_snapshot", &PyTransactionDB::GetSnapshot)
    .def("release_snapshot", &PyTransactionDB::ReleaseSnapshot)
    .def("begin_transaction", &PyTransactionDB::BeginTransaction, py::arg("write_options"), py::arg("txn_options")=TransactionOptions());

}

