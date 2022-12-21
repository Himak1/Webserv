cmake_minimum_required(VERSION 3.24)

### Defining the project and compilation flags ###
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wextra -Wall -Werror")

### Google Test ###
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download
)
execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download
)

add_subdirectory(
    ${CMAKE_BINARY_DIR}/googletest-src
	${CMAKE_BINARY_DIR}/googletest-build
)

### Subdirectories ###

include_directories(src)

enable_testing()
add_subdirectory(src)
add_subdirectory(test)
