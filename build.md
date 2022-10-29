Build Instructions Windows
--------------------------

    py -3.10 -m cython -3 --cplus --fast-fail --annotate .\rocksdb\_rocksdb.pyx

### Modified the setup.py libraries list from z.lib -> zlib.lib

    py -3.10 ./setup.py build_ext --include-dirs="C:\vcpkg\installed\x64-windows\include\" --library-dirs="C:\vcpkg\installed\x64-windows\lib" --libraries 'shlwapi rpcrt4'