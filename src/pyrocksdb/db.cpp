#include "pyrocksdb.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

void init_db(py::module & m) {
  py::class_<py_DB>(m, "DB")
    .def(py::init<>())
    .def("open", (Status (py_DB::*)(const Options& options, const std::string& name)) &py_DB::Open)
    .def("open",(py::tuple (py_DB::*)(const DBOptions& db_options, const std::string& name, const std::vector<ColumnFamilyDescriptor>& column_families)) &py_DB::Open)
    .def("open_for_readonly", (Status (py_DB::*)(const Options& options, const std::string& name,  bool error_if_log_file_exist)) &py_DB::OpenForReadOnly,py::arg("options"), py::arg("name"), py::arg("error_if_log_file_exist")=false)
    .def("open_for_readonly", (py::tuple (py_DB::*)(const DBOptions& db_options, const std::string& name, const std::vector<ColumnFamilyDescriptor>& column_families, bool error_if_log_file_exist)) &py_DB::OpenForReadOnly,py::arg("options"), py::arg("name"), py::arg("column_families"), py::arg("error_if_log_file_exist")=false)
    .def("open_as_secondary", (Status (py_DB::*)(const Options& options, const std::string& name, const std::string& secondary_path)) &py_DB::OpenAsSecondary)
    .def("open_as_secondary", (py::tuple (py_DB::*)(const DBOptions& db_options, const std::string& name, const std::string& secondary_path, const std::vector<ColumnFamilyDescriptor>& column_families)) &py_DB::OpenAsSecondary)
    .def("try_catch_up_with_primary", &py_DB::TryCatchUpWithPrimary)
    .def("put", (Status (py_DB::*) (const WriteOptions&, ColumnFamilyHandle*, const std::string&, const std::string& )) &py_DB::Put)
    .def("put", (Status (py_DB::*) (const WriteOptions&, const std::string&, const std::string&)) &py_DB::Put)
    .def("write", &py_DB::Write)
    .def("get", (std::unique_ptr<Blob> (py_DB::*) (const ReadOptions& options, const std::string&)) &py_DB::Get)
    .def("get", (std::unique_ptr<Blob> (py_DB::*) (const ReadOptions&, ColumnFamilyHandle*, const std::string&)) &py_DB::Get)
    .def("delete", (Status (py_DB::*) (const WriteOptions& options, const std::string& key)) &py_DB::Delete)
    .def("delete", (Status (py_DB::*) (const WriteOptions&, ColumnFamilyHandle*, const std::string&)) &py_DB::Delete)
    .def("merge", (Status (py_DB::*) (const WriteOptions& options, ColumnFamilyHandle* column_family, const std::string& key, const std::string& value)) &py_DB::Merge)
    .def("merge", (Status (py_DB::*) (const WriteOptions& options, const std::string& key, const std::string& value)) &py_DB::Merge)
    .def("close", &py_DB::Close)
    .def("create_column_family", &py_DB::CreateColumnFamily)
    .def("iterator", &py_DB::NewIterator);
    // .def_readonly("DefaultColumnFamilyName", &rocksdb::kDefaultColumnFamilyName);
  m.attr("DefaultColumnFamilyName") = rocksdb::kDefaultColumnFamilyName;

}
