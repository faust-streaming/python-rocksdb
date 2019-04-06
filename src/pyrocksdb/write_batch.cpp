#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

void init_write_batch(py::module & m) {
  py::class_<rocksdb::WriteBatch>(m, "bbb")
    .def(py::init<>());
}

