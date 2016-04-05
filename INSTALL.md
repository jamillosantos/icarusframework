
# Installation

## Dependencies

### SOCI

HINT for custom boost library directory:

Edit the CMakeLists.txt and set `BOOST_ROOT`, `BOOST_INCLUDEDIR` and `BOOST_LIBRARYDIR` before set the dependencies.

```
set(BOOST_ROOT "$ENV{HOME}/libs/libboost_1_60_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
```

```bash
cmake -DWITH_BOOST=On -DSOCI_STATIC=Off -DSOCI_POSTGRESQL=Off -DCMAKE_INSTALL_PREFIX=$HOME/libs/soci3.2.3/ ../..
make all
make install
```

### libemcached

```bash
mkdir -p $HOME/libs/libmemcached
./configure --prefix=$HOME/libs/libmemcached
```

### Boost

Version: 1.60.0

```bash
wget "http://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.bz2?r=&ts=1459024057&use_mirror=ufpr" -O boost_1_60_0.tar.bz2
tar xvf boost_1_60_0.tar.bz2
cd boost_1_60_0
./bootstrap.sh
mkdir -p $HOME/libs/libboost_1_60_0
./b2 --prefix=$HOME/libs/libboost_1_60_0 install
```

### libfcgi

It looks like the library was discontinued. Really sad about it. Try installing
it from repository.
