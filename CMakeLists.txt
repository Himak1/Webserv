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

enable_testing()
add_subdirectory(test)

### Setting Source Files & Header Files ###
set(SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/BuildResponse.cpp
    ${PROJECT_SOURCE_DIR}/src/Configuration.cpp
    ${PROJECT_SOURCE_DIR}/src/HTTPRequest.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/TcpServer.cpp
)

set(HPP_FILES
    ${PROJECT_SOURCE_DIR}/inc/BuildResponse.hpp
    ${PROJECT_SOURCE_DIR}/inc/Configuration.hpp
    ${PROJECT_SOURCE_DIR}/inc/HTTPRequest.hpp
    ${PROJECT_SOURCE_DIR}/inc/main.hpp
    ${PROJECT_SOURCE_DIR}/inc/TcpServer.hpp
)

add_library(webserv_lib STATIC ${SRC_FILES} ${HPP_FILES})

add_executable(webserv ${SRC_FILES} ${HPP_FILES})
target_include_directories(webserv_lib PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_include_directories(webserv PRIVATE ${PROJECT_SOURCE_DIR}/inc)
