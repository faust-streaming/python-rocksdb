#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"

namespace py = pybind11;

class PyCache: public Cache {
  public:
    using Cache::Cache;
}

void init_cache(py::module & m) {
  py::class_<Cache>(m, "Cache")
    .def(py::init<>());

  m.def("NewLRUCache", (std::shared_ptr<Cache> (*)(const LRUCacheOptions &)) &NewLRUCache);
}
