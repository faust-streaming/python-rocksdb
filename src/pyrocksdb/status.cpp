#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

void init_status(py::module & m) {

  py::class_<rocksdb::Status> status(m, "Status");

  status.def(py::init<>())
    .def("ok", &Status::ok)
    .def("is_not_found", &Status::IsNotFound)
    .def("is_corruption", &Status::IsCorruption)
    .def("is_not_supported", &Status::IsNotSupported)
    .def("is_invalid_argument", &Status::IsInvalidArgument)
    .def("is_io_error", &Status::IsIOError)
    .def("is_in_complete", &Status::IsIncomplete)
    .def("is_shutdown_in_progress", &Status::IsShutdownInProgress)
    .def("is_time_out", &Status::IsTimedOut)
    .def("is_aborted", &Status::IsAborted)
    .def("is_lock_limit", &Status::IsLockLimit)
    .def("is_busy", &Status::IsBusy)
    .def("is_deadlock", &Status::IsDeadlock)
    .def("is_expired", &Status::IsExpired)
    .def("is_try_again", &Status::IsTryAgain)
    .def("is_compaction_too_large", &Status::IsCompactionTooLarge)
    .def("is_no_space", &Status::IsNoSpace)
    .def("is_memory_limit", &Status::IsMemoryLimit)
    .def("is_path_not_found", &Status::IsPathNotFound)
    .def("to_string", &Status::ToString)
    .def("code", &Status::code);

  py::enum_<rocksdb::Status::Code>(status, "kCode")
      .value("kOk", rocksdb::Status::Code::kOk)
      .value("kNotFound", rocksdb::Status::Code::kNotFound)
      .value("kCorruption", rocksdb::Status::Code::kCorruption)
      .value("kNotSupported", rocksdb::Status::Code::kNotSupported)
      .value("kInvalidArgument", rocksdb::Status::Code::kInvalidArgument)
      .value("kIOError", rocksdb::Status::Code::kIOError)
      .value("kMergeInProgress", rocksdb::Status::Code::kMergeInProgress)
      .value("kIncomplete", rocksdb::Status::Code::kIncomplete)
      .value("kShutdownInProgress", rocksdb::Status::Code::kShutdownInProgress)
      .value("kTimedOut", rocksdb::Status::Code::kTimedOut)
      .value("kAborted", rocksdb::Status::Code::kAborted)
      .value("kBusy", rocksdb::Status::Code::kBusy)
      .value("kExpired", rocksdb::Status::Code::kExpired)
      .value("kTryAgain", rocksdb::Status::Code::kTryAgain)
      .value("kCompactionTooLarge", rocksdb::Status::Code::kCompactionTooLarge)
      .export_values();
}
