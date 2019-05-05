#include "pyrocksdb.hpp"

#include <pybind11/pybind11.h>
namespace py = pybind11;

class PyMergeOperator: public MergeOperator {
  using MergeOperator::MergeOperator;
    const char* Name() const override {
        PYBIND11_OVERLOAD_PURE(
            const char*,
            rocksdb::MergeOperator,
            Name
        );
    }
};

void init_merge_operator(py::module & m) {
    py::class_<MergeOperator, PyMergeOperator /* <--- trampoline*/> mergeOperator(m, "MergeOperator");
    mergeOperator.def(py::init<>());
}
