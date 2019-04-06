cmake_minimum_required(VERSION 2.8.12)
project(python_cpp_example)

SET(SOURCE_DIR "src/pyrocksdb")
include_directories(${SOURCE_DIR})
add_subdirectory(lib/pybind11)

SET ( WITH_TESTS OFF CACHE BOOL "BUILD ROCKSDB WITH TEST" FORCE)
SET ( WITH_SNAPPY ON CACHE BOOL "BUILD ROCKSDB WITH SNAPPY" FORCE)
SET ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
SET (USE_RTTI ON)
add_subdirectory(lib/rocksdb)

SET ( CMAKE_CXX_FLAGS "-std=c++11")
include_directories(lib/rocksdb/include)
pybind11_add_module(pyrocksdb ${SOURCE_DIR}/pyrocksdb.cpp ${SOURCE_DIR}/db.cpp ${SOURCE_DIR}/options.cpp ${SOURCE_DIR}/slice.cpp ${SOURCE_DIR}/status.cpp ${SOURCE_DIR}/write_batch.cpp ${SOURCE_DIR}/iterator.cpp)
target_link_libraries(pyrocksdb PRIVATE rocksdb)
