### CPP Test Files ###
set(TEST_FILES
<<<<<<< HEAD
	configuration_tests/config_test.cpp
=======
	request/RequestTest.cpp
	response/ResponseTest.cpp
	response/CGITest.cpp
>>>>>>> milan
	# !Add new test files here!
)

### Create Test Executables ###
foreach(file ${TEST_FILES})
	set(name)
	get_filename_component(name ${file} NAME_WE)
	add_executable(${name}
		${file}
		main_test.cpp
	)
	target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/inc)
	target_link_libraries(${name} ${CMAKE_BINARY_DIR}/lib/lib_unit.a gtest_main)
	target_compile_options(${name} PRIVATE -Wextra -Wall -g)
	add_test(NAME ${name} COMMAND ${name})
<<<<<<< HEAD
endforeach()
=======
endforeach()
>>>>>>> milan
