#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

class PyWrtieBatch: public WriteBatch{

  public:
    Status Put(const std::string& key, const std::string& value) {
      return WriteBatch::Put(nullptr, key, value);
    }
    Status Put(ColumnFamilyHandle* column_family, const std::string& key, const std::string& value) {
      return WriteBatch::Put(column_family, key, value);
    }
    Status Delete(const std::string& key) {
      return WriteBatch::Delete(nullptr, key);
    }
    Status Delete(ColumnFamilyHandle* column_family, const std::string& key) { return WriteBatch::Delete(column_family, key);}

    Status Merge(const std::string& key, const std::string& value) {
      return WriteBatch::Merge(nullptr, key, value);
    }
};

void init_write_batch(py::module & m) {
  py::class_<WriteBatch>(m, "_WriteBatch");
  py::class_<PyWrtieBatch, WriteBatch>(m, "WriteBatch")
    .def(py::init<>())
    .def("put", (Status (PyWrtieBatch::*) (const std::string&, const std::string&)) &PyWrtieBatch::Put)
    .def("put", (Status (PyWrtieBatch::*) (ColumnFamilyHandle*, const std::string&, const std::string&)) &PyWrtieBatch::Put)
    .def("delete", (Status (PyWrtieBatch::*) (const std::string&)) &PyWrtieBatch::Delete)
    .def("delete", (Status (PyWrtieBatch::*) (ColumnFamilyHandle*, const std::string&)) &PyWrtieBatch::Delete);
    // .def("merge", &PyWrtieBatch::Merge);
}
