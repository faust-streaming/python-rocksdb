#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"
#include <iostream>
#include <tests/constructor_stats.h>

namespace py = pybind11;
using namespace std;

class PySnapshot: public Snapshot {
  public:
    using Snapshot::Snapshot;

    SequenceNumber GetSequenceNumber() const override {
        PYBIND11_OVERLOAD_PURE(
            SequenceNumber,
            rocksdb::Snapshot,
            GetSequenceNumber
        );
    }
};


class Animal {
public:
    virtual ~Animal() { }
    virtual std::string go(int n_times, std::string *a) = 0;
    virtual std::string vvv() = 0;
};

// class Dog : public Animal {
// public:
    // std::string go(int n_times) override {
        // std::string result;
        // for (int i=0; i<n_times; ++i)
            // result += "woof! ";
        // return result;
    // }
// };

class PyAnimal : public Animal {
public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times, std::string *a) override {
      std::cout << "gaaao" << std::endl;
        PYBIND11_OVERLOAD_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,          /* Name of function in C++ (must match Python name) */
            n_times      /* Argument(s) */
        );
    }

    std::string vvv() override {
        PYBIND11_OVERLOAD_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            vvv,          /* Name of function in C++ (must match Python name) */
        );
    }
};

std::string call_go(Animal *animal) {
  std::cout<< "call go" << std::endl;
  std::string b;
  return animal->go(3, &b);
}


void init_snapshot(py::module & m) {
    py::class_<Snapshot, PySnapshot, std::unique_ptr<Snapshot, py::nodelete>> snapshot(m, "Snapshot");
    snapshot
        .def(py::init<>());

    py::class_<Animal, PyAnimal /* <--- trampoline*/> animal(m, "Animal");
    animal
        .def(py::init<>())
        .def("go", &Animal::go)
        .def("vvv", &Animal::vvv);

   // py::class_<Dog>(m, "Dog", animal)
        // .def(py::init<>());

    m.def("call_go", &call_go);
}


