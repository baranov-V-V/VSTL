# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/viktor/repos/VSTL/test/ExampleTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/viktor/repos/VSTL/test/ExampleTest/build

# Include any dependencies generated for this target.
include src/CMakeFiles/ALGO.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/ALGO.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/ALGO.dir/flags.make

src/CMakeFiles/ALGO.dir/example.cpp.o: src/CMakeFiles/ALGO.dir/flags.make
src/CMakeFiles/ALGO.dir/example.cpp.o: ../src/example.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/viktor/repos/VSTL/test/ExampleTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/ALGO.dir/example.cpp.o"
	cd /home/viktor/repos/VSTL/test/ExampleTest/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ALGO.dir/example.cpp.o -c /home/viktor/repos/VSTL/test/ExampleTest/src/example.cpp

src/CMakeFiles/ALGO.dir/example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ALGO.dir/example.cpp.i"
	cd /home/viktor/repos/VSTL/test/ExampleTest/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/viktor/repos/VSTL/test/ExampleTest/src/example.cpp > CMakeFiles/ALGO.dir/example.cpp.i

src/CMakeFiles/ALGO.dir/example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ALGO.dir/example.cpp.s"
	cd /home/viktor/repos/VSTL/test/ExampleTest/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/viktor/repos/VSTL/test/ExampleTest/src/example.cpp -o CMakeFiles/ALGO.dir/example.cpp.s

# Object files for target ALGO
ALGO_OBJECTS = \
"CMakeFiles/ALGO.dir/example.cpp.o"

# External object files for target ALGO
ALGO_EXTERNAL_OBJECTS =

src/libALGO.a: src/CMakeFiles/ALGO.dir/example.cpp.o
src/libALGO.a: src/CMakeFiles/ALGO.dir/build.make
src/libALGO.a: src/CMakeFiles/ALGO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/viktor/repos/VSTL/test/ExampleTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libALGO.a"
	cd /home/viktor/repos/VSTL/test/ExampleTest/build/src && $(CMAKE_COMMAND) -P CMakeFiles/ALGO.dir/cmake_clean_target.cmake
	cd /home/viktor/repos/VSTL/test/ExampleTest/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ALGO.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/ALGO.dir/build: src/libALGO.a

.PHONY : src/CMakeFiles/ALGO.dir/build

src/CMakeFiles/ALGO.dir/clean:
	cd /home/viktor/repos/VSTL/test/ExampleTest/build/src && $(CMAKE_COMMAND) -P CMakeFiles/ALGO.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/ALGO.dir/clean

src/CMakeFiles/ALGO.dir/depend:
	cd /home/viktor/repos/VSTL/test/ExampleTest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/viktor/repos/VSTL/test/ExampleTest /home/viktor/repos/VSTL/test/ExampleTest/src /home/viktor/repos/VSTL/test/ExampleTest/build /home/viktor/repos/VSTL/test/ExampleTest/build/src /home/viktor/repos/VSTL/test/ExampleTest/build/src/CMakeFiles/ALGO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/ALGO.dir/depend
