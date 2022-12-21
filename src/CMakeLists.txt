set(BINARY ${CMAKE_PROJECT_NAME})

### Setting Source Files & Header Files ###
set(SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/BuildResponse.cpp
    ${PROJECT_SOURCE_DIR}/src/configuration/Configuration.cpp
    ${PROJECT_SOURCE_DIR}/src/HTTPRequest.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/TcpServer.cpp
    CACHE INTERNAL ""
)

set(HPP_FILES
    ${PROJECT_SOURCE_DIR}/inc/BuildResponse.hpp
    ${PROJECT_SOURCE_DIR}/inc/Configuration.hpp
    ${PROJECT_SOURCE_DIR}/inc/HTTPRequest.hpp
    ${PROJECT_SOURCE_DIR}/inc/main.hpp
    ${PROJECT_SOURCE_DIR}/inc/TcpServer.hpp
    CACHE INTERNAL ""
)

### Compiling Executable & Static Library ###
add_library(${BINARY}_lib STATIC ${SRC_FILES} ${HPP_FILES})
target_include_directories(${BINARY}_lib PRIVATE ${PROJECT_SOURCE_DIR}/inc)
set_target_properties(${BINARY}_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib/)

add_executable(${BINARY} ${SRC_FILES} ${HPP_FILES})
target_include_directories(${BINARY} PRIVATE ${PROJECT_SOURCE_DIR}/inc)