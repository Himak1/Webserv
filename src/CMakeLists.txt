set(BINARY ${CMAKE_PROJECT_NAME})

### Setting Source Files & Header Files ###
set(SRC_FILES
    server/Configuration.cpp
    server/TCPServer.cpp
    request/Request.cpp
    response/Response.cpp
	response/CGI.cpp
	utils/log.cpp
    main.cpp
    # !Add new src files here! #
    CACHE INTERNAL ""
)

### Compiling Static Library ###
set(LIB_NAME _unit)
set(UNIT_SRC ${SRC_FILES})
list(REMOVE_ITEM UNIT_SRC main.cpp)

add_library(${LIB_NAME} STATIC ${UNIT_SRC})
target_include_directories(${LIB_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/inc)
set_target_properties(${LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

### Compiling Executable ###
add_executable(${BINARY} ${SRC_FILES})
target_include_directories(${BINARY} PRIVATE ${PROJECT_SOURCE_DIR}/inc)
set_target_properties(${BINARY} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)