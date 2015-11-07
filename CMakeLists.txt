cmake_minimum_required(VERSION 3.0)
project(icarusframework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

set(CMAKE_MODULE_PATH ${icarusframework_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

add_definitions(-DOUTPUT_DIRECTORY=${CMAKE_BINARY_DIR})
add_definitions(-DSOURCE_DIRECTORY=${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
	libs/soci/include
	libs/simpleweb
)

link_directories(
	build/libs/soci/lib
)

set(BOOST_ROOT "$ENV{HOME}/libs/libboost_1_59_0")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
find_package(Boost REQUIRED system coroutine context filesystem program_options regex date_time)
message("Boost library:")
message("\t+ include: ${Boost_INCLUDE_DIRS}")
message("\t+ library: ${Boost_LIBRARY_DIRS}")

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# find_package(Boost)

find_package(OpenSSL REQUIRED)
message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
message("OpenSSL SSL libraries: ${OPENSSL_SSL_LIBRARIES}")
message("OpenSSL crypto libraries: ${OPENSSL_CRYPTO_LIBRARIES}")
include_directories(${OPENSSL_INCLUDE_DIR})

add_library(simpleweb STATIC
	libs/simpleweb/simpleweb/server_http.hpp
	libs/simpleweb/simpleweb/server_https.hpp
	libs/simpleweb/simpleweb/crypto.hpp
	libs/simpleweb/simpleweb/server_ws.hpp
	libs/simpleweb/simpleweb/server_wss.hpp
)

set_target_properties(simpleweb PROPERTIES LINKER_LANGUAGE CXX)

set(SOURCE_FILES
		src/controllers/controller.hpp
		src/framework/http/headers.hpp
		src/framework/http/response.hpp
		src/framework/http/statuses.h
		src/framework/routes/data.hpp
		src/framework/routes/parser.hpp
		src/framework/routes/compiler.hpp
		src/framework/routes/routeswriter.hpp
		src/framework/result.hpp
		src/framework/routes/writers/simpleweb.hpp
		src/framework/exceptions.hpp
		src/framework/routes/exceptions.hpp
		src/test/routes_groups.cpp)

set(ICARUS_LIBRARIES
	soci_core
	soci_mysql

	boost_system
	boost_date_time
	boost_coroutine
	boost_context
	boost_filesystem
	boost_program_options
	boost_regex
	boost_locale

	rt

	pthread

	${OPENSSL_SSL_LIBRARIES}
	${OPENSSL_CRYPTO_LIBRARIES}

	simpleweb
)

add_library(icarusframework
	${SOURCE_FILES}
)

target_link_libraries(icarusframework
	${ICARUS_LIBRARIES}
)

set_target_properties(icarusframework PROPERTIES LINKER_LANGUAGE CXX)

add_library(minotaur
	src/minotaur/ochainstream.hpp
	src/minotaur/fileinfo.hpp
	src/minotaur/compiler/compiler.hpp
	src/minotaur/compiler/compilefile.hpp
	src/minotaur/compiler/cpptemplatebuilder.hpp
	src/minotaur/compiler/exceptions.hpp
	src/minotaur/compiler/metainfo.hpp
	src/minotaur/compiler/templatebuilder.hpp
)

set_target_properties(minotaur PROPERTIES LINKER_LANGUAGE CXX)

add_executable(mntr
	src/minotaur/mntr.cpp
)

target_link_libraries(mntr
	boost_program_options
	boost_filesystem
	boost_system
	boost_regex

	minotaur
)

# Test configuration thanks to http://neyasystems.com/an-engineers-guide-to-unit-testing-cmake-and-boost-unit-tests/

enable_testing()

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/test/*.cpp)
set(TEST_OUTPUT ${CMAKE_BINARY_DIR}/tests)
add_definitions(-DTEST_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/test/resources")

foreach(testSrc ${TEST_SRCS})
	get_filename_component(testName ${testSrc} NAME_WE)
	set(testName "test_${testName}")

	add_executable(${testName} ${testSrc})

	target_link_libraries(${testName} ${Boost_LIBRARIES} ${ICARUS_LIBRARIES})

	set_target_properties(${testName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT})

	add_test(NAME ${testName}
		WORKING_DIRECTORY ${TEST_OUTPUT}
		COMMAND ${TEST_OUTPUT}/${testName}
	)
endforeach(testSrc)
