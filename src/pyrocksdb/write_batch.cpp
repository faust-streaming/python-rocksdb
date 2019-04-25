#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

class PyWrtieBatch: public WriteBatch{

  public:
    Status Put(const std::string& key, const std::string& value) {
      return WriteBatch::Put(nullptr, key, value);
    }
    Status Delete(const std::string& key) { return WriteBatch::Delete(nullptr, key);}

    Status Merge(const std::string& key, const std::string& value) {
      return WriteBatch::Merge(nullptr, key, value);
    }
};

void init_write_batch(py::module & m) {
  py::class_<WriteBatch>(m, "_WriteBatch");
  py::class_<PyWrtieBatch, WriteBatch>(m, "WriteBatch")
    .def(py::init<>())
    .def("put", &PyWrtieBatch::Put)
    .def("delete", &PyWrtieBatch::Delete)
    .def("Merge", &PyWrtieBatch::Merge);
}
