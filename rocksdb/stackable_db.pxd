from libcpp.memory cimport shared_ptr
from .db cimport DB

cdef extern from "rocksdb/utilities/stackable_db.h" namespace "rocksdb":
    cdef cppclass StackableDB(DB):
        StackableDB(DB*) nogil except+
        StackableDB(shared_ptr[DB] db) nogil except+
        DB* GetBaseDB() nogil except+
