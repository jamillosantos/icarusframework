
project(minotaur)

find_package(Boost)

set(
	SOURCE_FILES
	src/ochainstream.hpp
)

add_library(minotaur
	${SOURCE_FILES}
)

add_executable(mntr
	src/compiler/templatebuilder.hpp
	src/compiler/cpptemplatebuilder.hpp
	src/compiler/compilefile.hpp
	src/compiler/compiler.hpp
	src/compiler/metainfo.hpp
	src/fileinfo.hpp
	src/compiler/exceptions.hpp

	src/main.cpp
)

target_link_libraries(mntr
	minotaur

	boost_program_options
	boost_filesystem
	boost_system
	boost_regex
)

set_target_properties(minotaur PROPERTIES LINKER_LANGUAGE CXX)
