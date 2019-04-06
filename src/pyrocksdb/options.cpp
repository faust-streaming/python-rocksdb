#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

void init_option(py::module & m) {
  py::class_<rocksdb::DBOptions>(m, "DBOptions")
    .def(py::init<>())
    .def("IncreaseParallelism", &rocksdb::DBOptions::IncreaseParallelism, py::arg("total_threads")=16, py::return_value_policy::reference_internal)
    .def_readwrite("create_if_missing", &rocksdb::DBOptions::create_if_missing)
    .def_readwrite("error_if_exists", &rocksdb::DBOptions::error_if_exists)
    .def_readwrite("paranoid_checks", &rocksdb::DBOptions::paranoid_checks)
    // .def_readwrite("rate_limiter", &rocksdb::DBOptions::rate_limiter)
    // .def_readwrite("sst_file_manager", &rocksdb::DBOptions::sst_file_manager)
    .def_readwrite("max_open_files", &rocksdb::DBOptions::max_open_files)
    .def_readwrite("max_file_opening_threads", &rocksdb::DBOptions::max_file_opening_threads)
    .def_readwrite("max_total_wal_size", &rocksdb::DBOptions::max_total_wal_size)
    .def_readwrite("statistics", &rocksdb::DBOptions::statistics)
    .def_readwrite("use_fsync", &rocksdb::DBOptions::use_fsync);

  py::class_<rocksdb::ColumnFamilyOptions>(m, "ColumnFamilyOptions")
    .def(py::init<>())
    .def("OptimizeLevelStyleCompaction", &rocksdb::ColumnFamilyOptions::OptimizeLevelStyleCompaction, py::arg("memtable_memory_budget")=512L * 1024L * 1024L, py::return_value_policy::reference_internal);

  py::class_<rocksdb::Options, rocksdb::DBOptions, rocksdb::ColumnFamilyOptions>(m, "Options")
    .def(py::init<>());

  py::class_<rocksdb::WriteOptions>(m, "WriteOptions")
    .def(py::init<>());

  py::class_<rocksdb::ReadOptions>(m, "ReadOptions")
    .def(py::init<>());
}
