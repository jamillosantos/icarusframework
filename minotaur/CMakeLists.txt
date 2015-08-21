
project(minotaur)

find_package(Boost)

set(
	SOURCE_FILES

	src/compiler/templatebuilder.hpp
	src/compiler/cpptemplatebuilder.hpp
	src/compiler/compilefile.hpp
	src/compiler/parser.hpp
	src/compiler/metainfo.hpp
	src/fileinfo.hpp
	src/compiler/exceptions.hpp
)

add_library(minotaur
	${SOURCE_FILES}
)

add_executable(mtaur
	src/main.cpp
)

target_link_libraries(mtaur
	minotaur

	boost_filesystem
	boost_system
	boost_regex
)

set_target_properties(minotaur PROPERTIES LINKER_LANGUAGE CXX)
