include_directories(
	${PROJECT_SOURCE_DIR}/inc
)

### CPP Test Files ###
set(TEST_FILES
	${CMAKE_CURRENT_LIST_DIR}/configuration_tests/config_test.cpp
)

list(REMOVE_ITEM ${TEST_FILES} ${CMAKE_CURRENT_LIST_DIR}/main.cpp)

### Create Test Executables ###
foreach(file ${TEST_FILES})
	set(name)
	get_filename_component(name ${file} NAME_WE)
	add_executable(
		${name}
		${HPP_FILES}
		${file}
		${CMAKE_CURRENT_LIST_DIR}/main.cpp
	)
	target_link_libraries(${name} ${CMAKE_BINARY_DIR}/lib/lib_unit.a gtest_main)
	target_compile_options(${name} PRIVATE -Wextra -Wall -g)
	add_test(NAME ${name} COMMAND ${name})
endforeach()
