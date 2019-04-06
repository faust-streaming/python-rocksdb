#include "pyrocksdb.hpp"
#include <pybind11/pybind11.h>
#include <iostream>
#include <stdexcept>


namespace py = pybind11;

py_DB::py_DB(): db_ptr(nullptr) {
  
}
py_DB::~py_DB() {
  if (db_ptr != nullptr) {
    delete db_ptr;
  }
}
void py_DB::Close() {
  delete db_ptr;
  db_ptr = nullptr;
}
Status py_DB::Open(const Options& options, const std::string& name) {
  if (db_ptr != nullptr) {
    throw std::invalid_argument("db has been opened");
  }
  Status st =  DB::Open(options, name, &db_ptr);
  std::cout << st.ToString() << std::endl;
  return st;
}
Status py_DB::Put(const WriteOptions& options, const std::string& key,
                 const std::string& value) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return db_ptr->Put(options, key, value);
}

Blob py_DB::Get(const ReadOptions& options, const std::string& key) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  Blob blob;
  Status status =  db_ptr->Get(options, key, &blob.data);
  blob.st = status;
  return blob;
}

Status py_DB::Delete(const WriteOptions& options, const std::string& key) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return db_ptr->Delete(options, db_ptr->DefaultColumnFamily(), key);
}

std::unique_ptr<Iterator> py_DB::NewIterator(const ReadOptions& options) {
  if (db_ptr == nullptr) {
    throw std::invalid_argument("db has been closed");
  }
  return std::unique_ptr<Iterator>(db_ptr->NewIterator(options));
}


void init_db(py::module &);
void init_option(py::module &);
void init_slice(py::module &);
void init_status(py::module &);
void init_write_batch(py::module &);
void init_iterator(py::module &);

PYBIND11_MODULE(pyrocksdb, m) {
    // optional module docstring
  m.doc() = "python rocksdb";
  init_db(m);
  init_option(m);
  init_slice(m);
  init_status(m);
  init_write_batch(m);
  init_iterator(m);
  py::class_<Blob>(m, "Blob")
    .def(py::init<>())
    .def_readwrite("status", &Blob::st)
    .def_readwrite("data", &Blob::data);
}

