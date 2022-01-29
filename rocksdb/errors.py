class Error(Exception):
    pass

class NotFound(Error):
    pass

class Corruption(Error):
    pass

class NotSupported(Error):
    pass

class InvalidArgument(Error):
    pass

class RocksIOError(Error):
    pass

class MergeInProgress(Error):
    pass

class Incomplete(Error):
    pass
