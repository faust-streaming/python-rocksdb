#pragma once

#include <pybind11/pybind11.h>
#include <rocksdb/db.h>
#include <rocksdb/status.h>
#include <rocksdb/options.h>
#include <rocksdb/iterator.h>
#include <rocksdb/slice.h>
#include <rocksdb/write_batch.h>
#include <rocksdb/table.h>
#include <rocksdb/filter_policy.h>
#include <rocksdb/cache.h>
#include <rocksdb/snapshot.h>
#include <utilities/merge_operators.h>
#include <rocksdb/utilities/transaction_db.h>
#include <rocksdb/utilities/transaction.h>
#include <rocksdb/utilities/write_batch_with_index.h>
// #include <db/dbformat.h>
// #include <db/snapshot_impl.h>

using namespace rocksdb;

namespace py = pybind11;


class IteratorWrapper {
  public:
    IteratorWrapper(Iterator *iterator): iterator(iterator) {
      
    }
    ~IteratorWrapper() {
      
    }

    bool Valid() {
      return iterator->Valid();
    }

    void SeekToFirst() {
      iterator->SeekToFirst();
    }

    void SeekToLast() {
      iterator->SeekToLast();
    }

    void Seek(const std::string & target) {
      iterator->Seek(target);
    }

    void SeekForPrev(const std::string & target) {
      iterator->SeekForPrev(target);
    }

    void Next() {
      iterator->Next();
    }

    void Prev() {
      iterator->Prev();
    }

    py::bytes key() const {
      return py::bytes(iterator->key().ToString());
    }

    py::bytes value() const {
      return py::bytes(iterator->value().ToString());
    }

    Status status() const {
      return iterator->status();
    }
  private:
  std::unique_ptr<Iterator> iterator;
};

class Blob {
  public:
    Status status;
    std::string data;
    py::bytes get_data() {
      return py::bytes(data);
    }
};

class py_ColumnFamilyHandle:  public ColumnFamilyHandle {

  const std::string & GetName() const override {
    PYBIND11_OVERLOAD_PURE(
        const std::string &,
        rocksdb::ColumnFamilyHandle,
        GetName
    );
  }

  uint32_t GetID() const override {
    PYBIND11_OVERLOAD_PURE(
        uint32_t,
        rocksdb::ColumnFamilyHandle,
        GetID
    );
  }

  Status GetDescriptor(ColumnFamilyDescriptor* desc) override {
    PYBIND11_OVERLOAD_PURE(
        Status,
        rocksdb::ColumnFamilyHandle,
        GetDescriptor
    );
  }
  const Comparator* GetComparator() const override {
    PYBIND11_OVERLOAD_PURE(
        Comparator*,
        rocksdb::ColumnFamilyHandle,
        GetComparator
    );
  }
};


class py_DB {
  public:
    py_DB();
    Status Open(const Options& options, const std::string& name);
    Status OpenForReadOnly(const Options& options, const std::string& name,  bool error_if_log_file_exist=false);
    py::tuple Open(const DBOptions& db_options, const std::string& name, const std::vector<ColumnFamilyDescriptor>& column_families);

    Status Put(const WriteOptions& options,
                     ColumnFamilyHandle* column_family, const std::string& key, const std::string& value);


    Status Put(const WriteOptions& options, const std::string& key,
                     const std::string& value);

    std::unique_ptr<Blob> Get(const ReadOptions& options, const std::string& key);
    std::unique_ptr<Blob> Get(const ReadOptions& options,
                            ColumnFamilyHandle* column_family, const std::string& key);
                            
    Status Write(const WriteOptions& options, WriteBatch& updates);
    Status Delete(const WriteOptions& options, const std::string& key);
    Status Delete(const WriteOptions& options,
                        ColumnFamilyHandle* column_family,
                        const std::string& key);

    void Close();
    py::tuple CreateColumnFamily(const ColumnFamilyOptions& options, const std::string& column_family_name);
    std::unique_ptr<IteratorWrapper> NewIterator(const ReadOptions& options);
    //FIXME: python gc
    ~py_DB();
    const std::string default_column_familiy_name();
  private:
    DB* db_ptr;
};
