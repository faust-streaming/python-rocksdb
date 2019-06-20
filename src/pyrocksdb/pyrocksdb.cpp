#include "pyrocksdb.hpp"
#include <iostream>
#include <stdexcept>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>



PYBIND11_MAKE_OPAQUE(std::vector<ColumnFamilyHandle*>);
PYBIND11_MAKE_OPAQUE(std::vector<ColumnFamilyDescriptor, std::allocator<ColumnFamilyDescriptor>>);


// class ColumnFamilyHandleWrapper {
  // public:
    // ColumnFamilyHandleWrapper() {
      
    // }
    // ~ColumnFamilyHandleWrapper() {
      // delete handle;
    // }
    // ColumnFamilyHandle* handle;
// };


py_DB::py_DB(): db_ptr(nullptr) {
  
}
py_DB::~py_DB() {
  if (db_ptr != nullptr) {
    delete db_ptr;
  }
}
void py_DB::Close() {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  delete db_ptr;
  db_ptr = nullptr;
}
Status py_DB::Open(const Options& options, const std::string& name) {
  if (db_ptr != nullptr) {
    throw std::invalid_argument("db has been opened");
  }
  Status st =  DB::Open(options, name, &db_ptr);
  return st;
}

py::tuple py_DB::Open(const DBOptions& db_options, const std::string& name, const std::vector<ColumnFamilyDescriptor>& column_families) {
  if (db_ptr != nullptr) {
    throw std::invalid_argument("db has been opened");
  }
  std::vector<ColumnFamilyHandle*> handles;
  Status st =  DB::Open(db_options, name, column_families, &handles, &db_ptr);
  return py::make_tuple(st, handles);
}

Status py_DB::Put(const WriteOptions& options, const std::string& key,
                 const std::string& value) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return db_ptr->Put(options, key, value);
}

Status py_DB::Put(const WriteOptions& options,
                     ColumnFamilyHandle* column_family, const std::string& key, const std::string& value) {

  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return db_ptr->Put(options, column_family, key, value);
}

Status py_DB::Write(const WriteOptions& options, WriteBatch& updates) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return db_ptr->Write(options, &updates);
}

std::unique_ptr<Blob> py_DB::Get(const ReadOptions& options, const std::string& key) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  std::unique_ptr<Blob> blob(new Blob());
  blob->status =  db_ptr->Get(options, key, &blob->data);
  return blob;
}

std::unique_ptr<Blob> py_DB::Get(const ReadOptions& options,
                          ColumnFamilyHandle* column_family, const std::string& key) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  std::unique_ptr<Blob> blob(new Blob());
  blob->status =  db_ptr->Get(options, column_family, key, &blob->data);
  return blob;
  }

Status py_DB::Delete(const WriteOptions& options, const std::string& key) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return db_ptr->Delete(options, db_ptr->DefaultColumnFamily(), key);
}

Status py_DB::Delete(const WriteOptions& options,
                    ColumnFamilyHandle* column_family,
                    const std::string& key) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return db_ptr->Delete(options, column_family, key);
  
}

py::tuple py_DB::CreateColumnFamily(const ColumnFamilyOptions& options, const std::string& column_family_name) {
  // std::unique_ptr<ColumnFamilyHandleWrapper> cf_handle(new ColumnFamilyHandleWrapper());
  // ColumnFamilyHandleWrapper *cf_handle = new ColumnFamilyHandleWrapper();
  ColumnFamilyHandle* cfh;
  Status s = db_ptr->CreateColumnFamily(options, column_family_name, &cfh);
  return py::make_tuple(s, cfh);
}

std::unique_ptr<IteratorWrapper> py_DB::NewIterator(const ReadOptions& options) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return std::unique_ptr<IteratorWrapper>(new IteratorWrapper(db_ptr->NewIterator(options)));
}



void init_db(py::module &);
void init_option(py::module &);
void init_slice(py::module &);
void init_status(py::module &);
void init_write_batch(py::module &);
void init_iterator(py::module &);
void init_filter_policy(py::module &);
void init_merge_operator(py::module &);
void init_transaction_db(py::module &);
void init_snapshot(py::module &);

PYBIND11_MODULE(pyrocksdb, m) {
    // optional module docstring
  m.doc() = "python rocksdb";
  init_db(m);
  init_option(m);
  init_slice(m);
  init_status(m);
  init_write_batch(m);
  init_iterator(m);
  init_filter_policy(m);
  init_merge_operator(m);
  init_transaction_db(m);
  init_snapshot(m);
  py::class_<Blob>(m, "Blob")
    .def(py::init<>())
    .def_readwrite("status", &Blob::status)
    // .def_readwrite("data", &Blob::data);
    .def_property_readonly("data", &Blob::get_data);

  // py::class_<ColumnFamilyHandleWrapper, std::unique_ptr<ColumnFamilyHandleWrapper>>(m, "ColumnFamilyHandleWrapper")
    // .def(py::init<>());

  py::class_<ColumnFamilyHandle, py_ColumnFamilyHandle, std::unique_ptr<ColumnFamilyHandle>>(m, "ColumnFamilyHandle")
    .def(py::init<>())
    .def("get_name", &ColumnFamilyHandle::GetName);

  py::class_<ColumnFamilyDescriptor, std::unique_ptr<ColumnFamilyDescriptor>>(m, "ColumnFamilyDescriptor")
    .def(py::init<const std::string&, const ColumnFamilyOptions&>());

  py::bind_vector<std::vector<ColumnFamilyDescriptor>>(m, "VectorColumnFamilyDescriptor");

  py::bind_vector<std::vector<ColumnFamilyHandle*>>(m, "VectorColumnFamilyHandle");


  py::class_<IteratorWrapper /* <--- trampoline*/> iterator(m, "IteratorWrapper");
  iterator
      .def("valid", &IteratorWrapper::Valid)
      .def("seek_to_first", &IteratorWrapper::SeekToFirst)
      .def("seek_to_last", &IteratorWrapper::SeekToLast)
      // .def("seek_for_prev", (void (IteratorWrapper::*) (const std::string &)) &Iterator::SeekForPrev)
      .def("seek_for_prev", &IteratorWrapper::SeekForPrev)
      .def("seek", &IteratorWrapper::Seek)
      .def("next", &IteratorWrapper::Next)
      .def("prev", &IteratorWrapper::Prev)
      .def("key", &IteratorWrapper::key)
      .def("value", &IteratorWrapper::value)
      .def("status", &IteratorWrapper::status);

  // m.def("append_1", &append_1);
}

