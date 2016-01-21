
# Installation

## Dependencies

### SOCI

Repo: 

HINT for custom boost library directory:

Edit the CMakeLists.txt and set `BOOST_ROOT`, `BOOST_INCLUDEDIR` and `BOOST_LIBRARYDIR` before set the dependencies.

### Boost

Version: 1.60.0

### libfcgi

### cpp-netlib

Version: 0.11.2
cmake ../.. -DBOOST_ROOT=$HOME/libs/libboost_1_59_0
make DESTDIR=$HOME/libs/cpp-netlib-0.11.2