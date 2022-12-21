include_directories(
	${PROJECT_SOURCE_DIR}/include
)

list(REMOVE_ITEM ${SRC_FILES} ${PROJECT_SOURCE_DIR}/src/main.c)

### CPP Test Files ###
set(TEST_FILES
			# ${PROJECT_SOURCE_DIR}/test_files/
)

list(REMOVE_ITEM ${TEST_FILES} "${PROJECT_SOURCE_DIR}/tests/main.cpp")

### Create Test Executables ###
foreach(file ${TEST_FILE})
	set(name)
	get_filename_component(name ${file} NAME_WE)
	add_executable(
		${name}_tests
		${SRC_FILES}
		${file}
		${PROJECT_SOURCE_DIR}/test_files/main.cpp
	)
	target_link_libraries(${name}_tests ${CMAKE_SOURCE_DIR}/webserv_lib.a gtest_main)
	target_compile_options(${name}_tests PRIVATE "-Wextra -Wall -g")
	add_test(NAME ${name} COMMAND ${name}_tests)
endforeach()
