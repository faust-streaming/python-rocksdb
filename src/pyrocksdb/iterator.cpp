#include <pybind11/pybind11.h>
#include "pyrocksdb.hpp"
#include <iostream>

namespace py = pybind11;

// class PyIterator: public Iterator {
  // public:
    // using Iterator::Iterator;

    // bool Valid() const override {
        // PYBIND11_OVERLOAD_PURE(
            // bool,
            // rocksdb::Iterator,
            // Valid
        // );
    // }

    // void SeekToFirst() override {
        // PYBIND11_OVERLOAD_PURE(
            // void,
            // rocksdb::Iterator,
            // SeekToFirst
        // );
    // }

    // void SeekToLast() override {
        // PYBIND11_OVERLOAD_PURE(
            // void,
            // rocksdb::Iterator,
            // SeekToLast
        // );
    // }

    // void Seek(const Slice & target) override {
        // PYBIND11_OVERLOAD_PURE(
            // void,
            // rocksdb::Iterator,
            // Seek,
            // target
        // );
    // }

    // void SeekForPrev(const Slice & target) override {
        // PYBIND11_OVERLOAD_PURE(
            // void,
            // rocksdb::Iterator,
            // SeekForPrev,
            // target
        // );
    // }

    // void Next() override {
        // PYBIND11_OVERLOAD_PURE(
            // void,
            // rocksdb::Iterator,
            // Next
        // );
    // }

    // void Prev() override {
        // PYBIND11_OVERLOAD_PURE(
            // void,
            // rocksdb::Iterator,
            // Prev
        // );
    // }

    // Slice key() const override {
        // PYBIND11_OVERLOAD_PURE(
            // Slice,
            // rocksdb::Iterator,
            // key
        // );
    // }

    // Slice value() const override {
        // PYBIND11_OVERLOAD_PURE(
            // Slice,
            // rocksdb::Iterator,
            // value
        // );
    // }

    // Status status() const override {
        // PYBIND11_OVERLOAD_PURE(
            // Status,
            // rocksdb::Iterator,
            // status
        // );
    // }

    // // Status Refresh() const override {
        // // PYBIND11_OVERLOAD_PURE(
            // // Status,
            // // rocksdb::Iterator,
            // // Refresh
        // // );
    // // }

    // // Status GetProperty() const override {
        // // PYBIND11_OVERLOAD_PURE(
            // // Status,
            // // rocksdb::Iterator,
            // // GetProperty
        // // );
    // // }
// };


void init_iterator(py::module & m) {
}

