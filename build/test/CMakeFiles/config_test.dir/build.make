# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/mnweitenb/.local/lib/python3.8/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/mnweitenb/.local/lib/python3.8/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/Coding/Codam/Projecten/webserv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/Coding/Codam/Projecten/webserv/build

# Include any dependencies generated for this target.
include test/CMakeFiles/config_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/config_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/config_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/config_test.dir/flags.make

test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o: test/CMakeFiles/config_test.dir/flags.make
test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o: /mnt/d/Coding/Codam/Projecten/webserv/test/configuration_tests/config_test.cpp
test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o: test/CMakeFiles/config_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Coding/Codam/Projecten/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o"
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o -MF CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o.d -o CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o -c /mnt/d/Coding/Codam/Projecten/webserv/test/configuration_tests/config_test.cpp

test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.i"
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/Coding/Codam/Projecten/webserv/test/configuration_tests/config_test.cpp > CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.i

test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.s"
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/Coding/Codam/Projecten/webserv/test/configuration_tests/config_test.cpp -o CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.s

test/CMakeFiles/config_test.dir/main_test.cpp.o: test/CMakeFiles/config_test.dir/flags.make
test/CMakeFiles/config_test.dir/main_test.cpp.o: /mnt/d/Coding/Codam/Projecten/webserv/test/main_test.cpp
test/CMakeFiles/config_test.dir/main_test.cpp.o: test/CMakeFiles/config_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Coding/Codam/Projecten/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/config_test.dir/main_test.cpp.o"
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/config_test.dir/main_test.cpp.o -MF CMakeFiles/config_test.dir/main_test.cpp.o.d -o CMakeFiles/config_test.dir/main_test.cpp.o -c /mnt/d/Coding/Codam/Projecten/webserv/test/main_test.cpp

test/CMakeFiles/config_test.dir/main_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/config_test.dir/main_test.cpp.i"
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/Coding/Codam/Projecten/webserv/test/main_test.cpp > CMakeFiles/config_test.dir/main_test.cpp.i

test/CMakeFiles/config_test.dir/main_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/config_test.dir/main_test.cpp.s"
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/Coding/Codam/Projecten/webserv/test/main_test.cpp -o CMakeFiles/config_test.dir/main_test.cpp.s

# Object files for target config_test
config_test_OBJECTS = \
"CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o" \
"CMakeFiles/config_test.dir/main_test.cpp.o"

# External object files for target config_test
config_test_EXTERNAL_OBJECTS =

test/config_test: test/CMakeFiles/config_test.dir/configuration_tests/config_test.cpp.o
test/config_test: test/CMakeFiles/config_test.dir/main_test.cpp.o
test/config_test: test/CMakeFiles/config_test.dir/build.make
test/config_test: lib/lib_unit.a
test/config_test: lib/libgtest_main.a
test/config_test: lib/libgtest.a
test/config_test: test/CMakeFiles/config_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/Coding/Codam/Projecten/webserv/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable config_test"
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/config_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/config_test.dir/build: test/config_test
.PHONY : test/CMakeFiles/config_test.dir/build

test/CMakeFiles/config_test.dir/clean:
	cd /mnt/d/Coding/Codam/Projecten/webserv/build/test && $(CMAKE_COMMAND) -P CMakeFiles/config_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/config_test.dir/clean

test/CMakeFiles/config_test.dir/depend:
	cd /mnt/d/Coding/Codam/Projecten/webserv/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/Coding/Codam/Projecten/webserv /mnt/d/Coding/Codam/Projecten/webserv/test /mnt/d/Coding/Codam/Projecten/webserv/build /mnt/d/Coding/Codam/Projecten/webserv/build/test /mnt/d/Coding/Codam/Projecten/webserv/build/test/CMakeFiles/config_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/config_test.dir/depend

