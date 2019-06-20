#include "pyrocksdb.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

void init_db(py::module & m) {
  py::class_<py_DB>(m, "DB")
    .def(py::init<>())
    .def("open", (Status (py_DB::*)(const Options& options, const std::string& name)) &py_DB::Open)
    .def("open",(py::tuple (py_DB::*)(const DBOptions& db_options, const std::string& name, const std::vector<ColumnFamilyDescriptor>& column_families)) &py_DB::Open)
    .def("put", (Status (py_DB::*) (const WriteOptions&, ColumnFamilyHandle*, const std::string&, const std::string& )) &py_DB::Put)
    .def("put", (Status (py_DB::*) (const WriteOptions&, const std::string&, const std::string&)) &py_DB::Put)
    .def("write", &py_DB::Write)
    .def("get", (std::unique_ptr<Blob> (py_DB::*) (const ReadOptions& options, const std::string&)) &py_DB::Get)
    .def("get", (std::unique_ptr<Blob> (py_DB::*) (const ReadOptions&, ColumnFamilyHandle*, const std::string&)) &py_DB::Get)
    .def("delete", (Status (py_DB::*) (const WriteOptions& options, const std::string& key)) &py_DB::Delete)
    .def("delete", (Status (py_DB::*) (const WriteOptions&, ColumnFamilyHandle*, const std::string&)) &py_DB::Delete)
    .def("close", &py_DB::Close)
    .def("create_column_family", &py_DB::CreateColumnFamily)
    .def("iterator", &py_DB::NewIterator);
    // .def_readonly("DefaultColumnFamilyName", &rocksdb::kDefaultColumnFamilyName);
  m.attr("DefaultColumnFamilyName") = rocksdb::kDefaultColumnFamilyName;

}
