### CPP Test Files ###
set(TEST_FILES
	configure/Token_tests.cpp
	configure/tokenizer_tests.cpp
	configure/parser/parser_tests.cpp
	configure/parser/parseLocation_tests.cpp
	configure/parser/parseLocation_integration_tests.cpp
	configure/tokenizer_parser_integration_tests.cpp
	request/RequestTest.cpp
	response/ResponseTest.cpp
	response/CGITest.cpp
)

### Create Test Executables ###
foreach(file ${TEST_FILES})
	set(name)
	get_filename_component(name ${file} NAME_WE)
	add_executable(${name}
		${file}
		main_test.cpp
	)
	target_include_directories(${name} PRIVATE
		${PROJECT_SOURCE_DIR}/inc
	)
	target_link_libraries(${name} ${CMAKE_BINARY_DIR}/lib/lib_unit.a gtest_main)
	target_compile_options(${name} PRIVATE -Wextra -Wall -g)
	add_test(NAME ${name} COMMAND ${name})
endforeach()

add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/configure/dummy_config ${CMAKE_CURRENT_BINARY_DIR}/dummy_config
)
