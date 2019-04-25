#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

void init_filter_policy(py::module & m) {
  m.def("NewBloomFilterPolicy", &NewBloomFilterPolicy, py::arg("bits_per_key")=10, py::arg("use_block_based_builder")=false);
}

