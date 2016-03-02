cmake_minimum_required(VERSION 3.0)
project(icarusframework VERSION 0.0.0.0)

add_definitions(-DVERSION_MAJOR="${PROJECT_VERSION_MAJOR}")
add_definitions(-DVERSION_MINOR="${PROJECT_VERSION_MINOR}")
add_definitions(-DVERSION_PATCH="${PROJECT_VERSION_PATCH}")
add_definitions(-DVERSION_TWEAK="${PROJECT_VERSION_TWEAK}")

#SET (CMAKE_C_COMPILER               "/usr/bin/clang")
#SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")

if(NOT WIN32)
	string(ASCII 27 Esc)
	set(ColourReset "${Esc}[m")
	set(ColourBold  "${Esc}[1m")
	set(Red         "${Esc}[31m")
	set(Green       "${Esc}[32m")
	set(Yellow      "${Esc}[33m")
	set(Blue        "${Esc}[34m")
	set(Magenta     "${Esc}[35m")
	set(Cyan        "${Esc}[36m")
	set(White       "${Esc}[37m")
	set(BoldRed     "${Esc}[1;31m")
	set(BoldGreen   "${Esc}[1;32m")
	set(BoldYellow  "${Esc}[1;33m")
	set(BoldBlue    "${Esc}[1;34m")
	set(BoldMagenta "${Esc}[1;35m")
	set(BoldCyan    "${Esc}[1;36m")
	set(BoldWhite   "${Esc}[1;37m")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(CMAKE_MODULE_PATH ${icarusframework_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

add_definitions(-DOUTPUT_DIRECTORY="${CMAKE_BINARY_DIR}")
add_definitions(-DSOURCE_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")

set(SOCI_LIBRARY_DIR "$ENV{HOME}/libs/soci3.2.3/usr/local")
find_library(soci_core soci_core REQUIRED PATHS "${SOCI_LIBRARY_DIR}" PATH_SUFFIXES lib lib64)
if("${soci_core}" STREQUAL "soci_core-NOTFOUND")
	message(FATAL_ERROR "Component not found ${Red}soci_core${ColourReset}.")
endif()

# find_library(soci_mysql soci_mysql REQUIRED PATHS "${SOCI_LIBRARY_DIR}" PATH_SUFFIXES lib lib64)
# if("${soci_mysql}" STREQUAL "soci_mysql-NOTFOUND")
# 	message(FATAL_ERROR "Component not found ${Red}soci_mysql${ColourReset}.")
# endif()

# find_library(soci_postgresql soci_postgresql REQUIRED PATHS "${SOCI_LIBRARY_DIR}" PATH_SUFFIXES lib lib64)
# if("${soci_postgresql}" STREQUAL "soci_postgresql-NOTFOUND")
# 	message(FATAL_ERROR "Component not found ${Red}soci_postgresql${ColourReset}.")
# endif()

message("      SOCI (core): ${soci_core}")

# message("     SOCI (mysql): ${soci_mysql}")
# message("SOCI (postgresql): ${soci_postgresql}")

include_directories(
	"${SOCI_LIBRARY_DIR}/include"
)

set(BOOST_ROOT "$ENV{HOME}/libs/libboost_1_60_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
find_package(Boost REQUIRED log_setup log system coroutine context filesystem program_options regex date_time)
message("Boost library:")
message("\t+ include: ${Boost_INCLUDE_DIRS}")
message("\t+ library: ${Boost_LIBRARY_DIRS}")
add_definitions(-DBOOST_LOG_DYN_LINK=1)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

find_package(OpenSSL REQUIRED)
message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL SSL libraries: ${OPENSSL_SSL_LIBRARIES}")
message("OpenSSL crypto libraries: ${OPENSSL_CRYPTO_LIBRARIES}")
include_directories(${OPENSSL_INCLUDE_DIR})


# cpp-netlib
find_package(cppnetlib 0.11.0 REQUIRED PATHS "$ENV{HOME}/libs/cpp-netlib-0.11.2/usr/local/lib64")
if("${cppnetlib}" STREQUAL "cppnetlib-NOTFOUND")
	message(FATAL_ERROR "Component not found ${Red}cppnetlib${ColourReset}.")
endif()
message("CPPNETLIB_INCLUDE_DIRS: ${CPPNETLIB_INCLUDE_DIRS}")
message("CPPNETLIB_LIBRARIES: ${CPPNETLIB_LIBRARIES}")
include_directories(${CPPNETLIB_INCLUDE_DIRS})

configure_file(
	src/config.h.in
	"${PROJECT_BINARY_DIR}/config.h"
)

set(SOURCE_FILES
	include/icarus/log.h
	src/icarus/log.cpp

	# ---

	include/icarus/http/headers.h
	include/icarus/statuses.h
	include/icarus/http/cookies.h
	include/icarus/http/exceptions.h
	include/icarus/http/request.h
	include/icarus/http/response.h
	include/icarus/http/client.h
	include/icarus/http/dataconversion.h

	src/icarus/http/headers.cpp
	src/icarus/statuses.cpp
	src/icarus/http/cookies.cpp
	src/icarus/http/exceptions.cpp
	src/icarus/http/request.cpp
	src/icarus/http/response.cpp
	src/icarus/http/client.cpp
	src/icarus/http/dataconversion.cpp

	# ---

	include/icarus/http/fcgi/request.h
	include/icarus/http/fcgi/response.h
	include/icarus/http/fcgi/application.h
	include/icarus/http/fcgi/client.h

	src/icarus/http/fcgi/request.cpp
	src/icarus/http/fcgi/response.cpp
	src/icarus/http/fcgi/application.cpp
	src/icarus/http/fcgi/client.cpp

	# ---

	include/icarus/routes/data.h
	include/icarus/routes/parser.h
	include/icarus/routes/compiler.h
	include/icarus/routes/routeswriter.h
	include/icarus/routes/to_url.h
	include/icarus/routes/exceptions.h
	include/icarus/routes/fieldtypes.h

	src/icarus/routes/data.cpp
	src/icarus/routes/parser.cpp
	src/icarus/routes/compiler.cpp
	src/icarus/routes/routeswriter.cpp
	src/icarus/routes/to_url.cpp
	src/icarus/routes/exceptions.cpp
	src/icarus/routes/fieldtypes.cpp

	# ---

	include/icarus/session/manager.h
	include/icarus/session/session.h
	include/icarus/session/memory.h

	src/icarus/session/manager.cpp
	src/icarus/session/session.cpp
	src/icarus/session/memory.cpp

	# ---

	include/icarus/interfaces.h

	include/icarus/action.h
	include/icarus/result.h
	include/icarus/exceptions.h
	include/icarus/action.h
	include/icarus/application.h
	include/icarus/config.h
	include/icarus/multithreadedapplication.h

	src/icarus/action.cpp
	src/icarus/result.cpp
	src/icarus/exceptions.cpp
	src/icarus/action.cpp
	src/icarus/application.cpp
	src/icarus/config.cpp
	src/icarus/multithreadedapplication.cpp

	# ---

	include/icarus/db/databases.h
	include/icarus/db/field.h
	include/icarus/db/migrations.h

	src/icarus/db/databases.cpp
	src/icarus/db/field.cpp
	src/icarus/db/migrations.cpp
)

include_directories("${CMAKE_SOURCE_DIR}/include")

set(ICARUS_LIBRARIES
	boost_log_setup
	boost_log
	boost_system
	boost_date_time
	boost_coroutine
	boost_context
	boost_filesystem
	boost_program_options
	boost_regex
	boost_locale
	boost_thread

	${soci_core}
	${soci_mysql}

	rt

	pthread

	${OPENSSL_SSL_LIBRARIES}
	${OPENSSL_CRYPTO_LIBRARIES}
)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(icarusframework SHARED
	${SOURCE_FILES}
)

target_link_libraries(icarusframework
	${ICARUS_LIBRARIES}
)

set_target_properties(icarusframework PROPERTIES LINKER_LANGUAGE CXX)

add_executable(icarus-fastcgi
	src/icarus/http/fcgi/main.cpp
)

target_link_libraries(icarus-fastcgi
	fcgi
	fcgi++

	icarusframework
	cppnetlib-uri


)

add_library(minotaur SHARED
	src/minotaur/fileinfo.hpp
	src/minotaur/compiler/compiler.hpp
	src/minotaur/compiler/compilefile.hpp
	src/minotaur/compiler/cpptemplatebuilder.hpp
	src/minotaur/compiler/exceptions.hpp
	src/minotaur/compiler/metainfo.hpp
	src/minotaur/compiler/templatebuilder.hpp
)

target_link_libraries(minotaur
	boost_system
	boost_filesystem
	boost_program_options
)

set_target_properties(minotaur PROPERTIES LINKER_LANGUAGE CXX)

add_executable(mntr
	src/minotaur/mntr.cpp
)

target_link_libraries(mntr
	boost_system
	boost_filesystem
	boost_program_options
	boost_regex

	minotaur
)

# Test configuration thanks to http://neyasystems.com/an-engineers-guide-to-unit-testing-cmake-and-boost-unit-tests/

enable_testing()

file(GLOB_RECURSE TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/icarus/*.cpp)
set(TEST_OUTPUT ${CMAKE_BINARY_DIR}/tests)
add_definitions(-DTEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test/resources")

message("\nTests\n=====")
foreach (testSrc ${TEST_SRCS})
	get_filename_component(testName ${testSrc} NAME_WE)
	string(REPLACE "test/" "" testPackage "${testSrc}")
	string(REPLACE "${testName}.cpp" "" testPackage "${testPackage}")
	string(REPLACE "/" "_" testPackage "${testPackage}")
	set(testName "test_${testPackage}${testName}")
	message(" + ${testSrc} as ${testName}")

	add_executable(${testName} ${testSrc})

	target_link_libraries(${testName} ${Boost_LIBRARIES} icarusframework)

	set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT})

	add_test(NAME ${testName}
		WORKING_DIRECTORY ${TEST_OUTPUT}
		COMMAND ${TEST_OUTPUT}/${testName}
		)
endforeach (testSrc)
message("")