
# Installation

## Dependencies

### google-glog

Website/Repo: https://github.com/google/glog
Method: Makefile

### gflags

Website: http://gflags.github.io/gflags/
Repo: https://github.com/gflags/gflags
Method: CMake

	cmake ../../../libs/gflags/ -DCMAKE_CXX_FLAGS=-fPIC

### double-convertion

Repo: git@github.com:google/double-conversion.git
Method: CMake

	cmake ../../../libs/gflags/ -DCMAKE_CXX_FLAGS=-fPIC

### folly

Repo: https://github.com/facebook/folly

	yum install libevent-devel

	LDFLAGS="-L/home/jsantos/projects/icarusframework/build/libs/google-glog/lib -L/home/jsantos/projects/icarusframework/build/libs/gflags/lib -L/home/jsantos/projects/icarusframework/build/libs/double-conversion/double-conversion" CPPFLAGS="-I/home/jsantos/projects/icarusframework/build/libs/google-glog/include/ -I/home/jsantos/projects/icarusframework/build/libs/gflags/include -I/home/jsantos/projects/icarusframework/libs/double-conversion" ./configure --prefix=/home/jsantos/projects/icarusframework/build/libs/folly

### wangle

Repo: https://github.com/facebook/wangle

	cmake ../../../libs/wangle/wangle "-DINCLUDE_DIR=/home/jsantos/projects/icarusframework/build/libs/google-glog/include;/home/jsantos/projects/icarusframework/libs/double-conversion;/home/jsantos/projects/icarusframework/build/libs/gflags/include" -DFOLLY_LIBRARYDIR=/home/jsantos/projects/icarusframework/build/libs/folly/lib -DFOLLY_INCLUDEDIR=/home/jsantos/projects/icarusframework/build/libs/folly/include

WARNING: bin/BootstrapTest will fail but it doesn't matter.

### proxygen

Repo: https://github.com/facebook/proxygen

	yum install autoconf-archive libcap-devel gperf

	LDFLAGS="-L/home/jsantos/projects/icarusframework/build/libs/wangle/lib -L/home/jsantos/projects/icarusframework/build/libs/google-glog/lib -L/home/jsantos/projects/icarusframework/build/libs/gflags/lib -L/home/jsantos/projects/icarusframework/build/libs/double-conversion/double-conversion -L/home/jsantos/projects/icarusframework/build/libs/folly/lib" CPPFLAGS="-I/home/jsantos/projects/icarusframework/libs/wangle -I/home/jsantos/projects/icarusframework/build/libs/google-glog/include/ -I/home/jsantos/projects/icarusframework/build/libs/gflags/include -I/home/jsantos/projects/icarusframework/libs/double-conversion -I/home/jsantos/projects/icarusframework/build/libs/folly" ./configure --prefix=/home/jsantos/projects/icarusframework/build/libs/proxygen

### SOCI

Repo: 