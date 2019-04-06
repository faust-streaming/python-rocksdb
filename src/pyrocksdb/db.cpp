#include "pyrocksdb.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

void init_db(py::module & m) {
  py::class_<py_DB>(m, "DB")
    .def(py::init<>())
    .def("open", &py_DB::Open)
    .def("put", &py_DB::Put)
    .def("get", &py_DB::Get)
    .def("delete", &py_DB::Delete)
    .def("close", &py_DB::Close)
    .def("iterator", &py_DB::NewIterator);
}
