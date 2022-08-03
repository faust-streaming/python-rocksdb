from libcpp.string cimport string
from .db cimport DB
from .status cimport Status

cdef extern from "rocksdb/utilities/checkpoint.h" namespace "rocksdb":
    cdef cppclass Checkpoint:
        Status CreateCheckpoint(const string&) nogil except+

    cdef Status Checkpoint_Create "rocksdb::Checkpoint::Create"(
        DB*,
        Checkpoint**) nogil except+
