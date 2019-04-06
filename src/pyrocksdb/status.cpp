#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

void init_status(py::module & m) {

  py::class_<rocksdb::Status> status(m, "Status");

  status.def(py::init<>())
    .def("code", &Status::code)
    .def("ok", &Status::ok)
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
