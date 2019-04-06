#include <pybind11/pybind11.h>
#include <iostream>
#include "pyrocksdb.hpp"

namespace py = pybind11;

void init_slice(py::module & m) {
  py::class_<Slice>(m, "Slice")
    .def(py::init<const std::string&>())
    .def("data", &Slice::ToString, py::arg("hex")=false);
}
