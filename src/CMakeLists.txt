set(BINARY ${CMAKE_PROJECT_NAME})

### Setting Source Files & Header Files ###
set(SRC_FILES
    configuration/Configuration.cpp
    configuration/config.cpp
    BuildResponse.cpp
    HTTPRequest.cpp
    main.cpp
    TcpServer.cpp
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

### Compiling Static Library ###
set(LIB_NAME _unit)
set(UNIT_SRC ${SRC_FILES})
list(REMOVE_ITEM UNIT_SRC main.cpp)

add_library(${LIB_NAME} STATIC ${UNIT_SRC} ${HPP_FILES})
target_include_directories(${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/inc)
set_target_properties(${LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

### Compiling Executable ###
add_executable(${BINARY} ${SRC_FILES} ${HPP_FILES})
target_include_directories(${BINARY} PRIVATE ${PROJECT_SOURCE_DIR}/inc)
set_target_properties(${BINARY} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)