#include "pyrocksdb.hpp"

#include <pybind11/pybind11.h>
#include <iostream>

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

// class Child {
  // public:
    // Child() {std::cout << "cons child" << std::endl;}
    // ~Child(){std::cout << "dest child" << std::endl;}
// };

// class Parent {
// public:
   // // Parent() : child(std::make_shared<Child>()) { std::cout << "const parent" << std::endl;}
   // Parent() { std::cout << "const parent" << std::endl;}
   // Child* get_child() {
     // Child * child = new Child();
     // return child;
   // }  [> Hint: ** DON'T DO THIS ** <]
   // ~Parent() {std::cout << "dest parent" << std::endl;}
// // private:
    // // std::shared_ptr<Child> child;
// };


void init_merge_operator(py::module & m) {
    py::class_<MergeOperator, PyMergeOperator /* <--- trampoline*/> mergeOperator(m, "MergeOperator");
    mergeOperator.def(py::init<>());

    // py::class_<Child>(m, "Child");

    // py::class_<Parent>(m, "Parent")
       // .def(py::init<>())
       // .def("get_child", &Parent::get_child);
}
