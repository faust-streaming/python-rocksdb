#include "pyrocksdb.hpp"

#include <pybind11/pybind11.h>
#include <iostream>

namespace py = pybind11;

class MergeDelegate: public AssociativeMergeOperator {
  public:
    // ~MergeDelegate() override {}
    virtual py::tuple merge(const std::string* existing_value, const std::string& value) const = 0;
};

class PyAssociativeMergeDelegate: public MergeDelegate  {
  public:
    using MergeDelegate::MergeDelegate;
    virtual py::tuple merge(const std::string* existing_value, const std::string& value) const {
        PYBIND11_OVERLOAD_PURE(
            py::tuple,
            MergeDelegate,
            merge,
            existing_value,
            value
        );
    }
    virtual bool Merge(const Slice& key, const Slice* existing_value,
                       const Slice& value, std::string* new_value,
                       Logger* logger) const override {
      // py::tuple result = merge(3, 2);
      py::tuple result;
      if (existing_value == nullptr) {
        std::string s_value(value.data());
        result = merge(nullptr, s_value);
      } else {
        std::string s_existing_value(existing_value->data());
        std::string s_value(value.data());
        result = merge(&s_existing_value, s_value);
      }
      bool success = result[0].cast<bool>();
      *new_value = result[1].cast<std::string>();
      return success;
    }

    virtual const char* Name() const override {
      PYBIND11_OVERLOAD_PURE (
          const char*,
          MergeDelegate,
          Name,
      );
    }
};

py::bytes slice_to_bytes(Slice & slice) {
  return py::bytes(slice.data());
}

// class SliceWrapper {
  // public:
    // SliceWrapper(const Slice &slice): slice_(std::make_unique<Slice>(slice.data())) {}
    // py::bytes data() const {
      // return py::bytes(slice_->data());
    // }
  // private:
    // std::unique_ptr <Slice> slice_;
// };


void init_merge_operator(py::module & m) {
    // py::class_<MergeOperator, PyMergeOperator, std::shared_ptr<MergeOperator>> (m, "MergeOperator");
    py::class_<MergeOperator, std::shared_ptr<MergeOperator>> (m, "MergeOperator");
    py::class_<AssociativeMergeOperator, MergeOperator, std::shared_ptr<AssociativeMergeOperator>> (m, "_AssociativeMergeOperator");

    py::class_<MergeDelegate, PyAssociativeMergeDelegate, AssociativeMergeOperator, std::shared_ptr<MergeDelegate>>(m, "AssociativeMergeOperator")
      .def(py::init<>());

    m.def("slice_to_bytes", &slice_to_bytes);

}
