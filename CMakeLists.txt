cmake_minimum_required(VERSION 2.8)
PROJECT(hurricane)

#The version number.
set(HURRICANE_VERSION_MAJOR 0)
set(HURRICANE_VERSION_MINOR 0)
set(HURRICANE_VERSION_MINOR_FIX 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
	"${PROJECT_SOURCE_DIR}/src/hurricane/config.hpp.in"
	"${PROJECT_BINARY_DIR}/src/hurricane/config.hpp"
)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

INCLUDE_DIRECTORIES(
	${PROJECT_BINARY_DIR}
	${PROJECT_BINARY_DIR}/src
)

FILE(GLOB HURRICANE_SOURCES 
	"src/*.cpp"
	"src/hurricane/*.cpp"
)

ADD_EXECUTABLE(hurricane
	${HURRICANE_SOURCES}
)
target_link_libraries(hurricane "-lboost_system -lboost_program_options -lpthread -pthread")

if(UNIX)
	add_custom_target(distclean @echo cleaning for source distribution)
	add_custom_command(
		COMMENT	"distribution clean"
		DEPENDS	clean
		COMMAND	sh
		ARGS		-c '(for x in cmake.depends cmake_install.cmake cmake.check_depends CMakeCache.txt cmake.check_cache Makefile gmon.out '*~' CMakeTmp .backups CMakeFiles config.h \; do find . -iname \"$$x\" \; done ) | xargs -n1 rm -rf'
		TARGET  distclean
	)
ENDIF(UNIX)

