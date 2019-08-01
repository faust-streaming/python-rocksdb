#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

void init_option(py::module & m) {
  py::class_<DBOptions, std::unique_ptr<DBOptions>>(m, "DBOptions")
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

  py::class_<rocksdb::AdvancedColumnFamilyOptions>(m, "AdvancedColumnFamilyOptions")
    .def(py::init<>())
    .def_readwrite("max_write_buffer_number", &rocksdb::AdvancedColumnFamilyOptions::max_write_buffer_number)
    .def_readwrite("target_file_size_base", &rocksdb::AdvancedColumnFamilyOptions::target_file_size_base)
    .def_readwrite("target_file_size_multiplier", &rocksdb::AdvancedColumnFamilyOptions::target_file_size_multiplier);

  py::class_<rocksdb::ColumnFamilyOptions, rocksdb::AdvancedColumnFamilyOptions>(m, "ColumnFamilyOptions")
    .def(py::init<>())
    .def("OptimizeLevelStyleCompaction", &rocksdb::ColumnFamilyOptions::OptimizeLevelStyleCompaction, py::arg("memtable_memory_budget")=512L * 1024L * 1024L, py::return_value_policy::reference_internal)
    .def_readwrite("write_buffer_size", &rocksdb::ColumnFamilyOptions::write_buffer_size)
    .def_readwrite("table_factory", &rocksdb::ColumnFamilyOptions::table_factory)
    .def_readwrite("sync", &rocksdb::ColumnFamilyOptions::merge_operator);

  py::class_<rocksdb::Options, rocksdb::DBOptions, rocksdb::ColumnFamilyOptions>(m, "Options")
    .def(py::init<>());

  py::class_<rocksdb::WriteOptions>(m, "WriteOptions")
    .def(py::init<>());

  py::class_<rocksdb::ReadOptions>(m, "ReadOptions")
    .def(py::init<>())
    .def_readwrite("snapshot", &ReadOptions::snapshot);

  py::class_<rocksdb::TransactionDBOptions>(m, "TransactionDBOptions")
    .def(py::init<>());

  py::class_<rocksdb::TransactionOptions>(m, "TransactionOptions")
    .def(py::init<>())
    .def_readwrite("set_snapshot", &TransactionOptions::set_snapshot);

  py::class_<rocksdb::LRUCacheOptions>(m, "LRUCacheOptions")
    .def(py::init<>())
    .def_readwrite("capacity", &rocksdb::LRUCacheOptions::capacity)
    .def_readwrite("num_shard_bits", &rocksdb::LRUCacheOptions::num_shard_bits)
    .def_readwrite("strict_capacity_limit", &rocksdb::LRUCacheOptions::strict_capacity_limit)
    .def_readwrite("high_pri_pool_ratio", &rocksdb::LRUCacheOptions::high_pri_pool_ratio)
    .def_readwrite("memory_allocator", &rocksdb::LRUCacheOptions::memory_allocator);

  py::class_<rocksdb::CompactRangeOptions>(m, "CompactRangeOptions")
    .def(py::init<>())
    .def_readwrite("exclusive_manual_compaction", &rocksdb::CompactRangeOptions::exclusive_manual_compaction)
    .def_readwrite("change_level", &rocksdb::CompactRangeOptions::change_level)
    .def_readwrite("target_level", &rocksdb::CompactRangeOptions::target_level);
}
