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
CMAKE_SOURCE_DIR = /home/forrest/Documents/book-tracker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/forrest/Documents/book-tracker/build

# Utility rule file for ContinuousConfigure.

# Include the progress variables for this target.
include thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/progress.make

thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure:
	cd /home/forrest/Documents/book-tracker/build/thirdparty/nlohmann_json && /usr/bin/ctest -D ContinuousConfigure

ContinuousConfigure: thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure
ContinuousConfigure: thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/build.make

.PHONY : ContinuousConfigure

# Rule to build all files generated by this target.
thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/build: ContinuousConfigure

.PHONY : thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/build

thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/clean:
	cd /home/forrest/Documents/book-tracker/build/thirdparty/nlohmann_json && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousConfigure.dir/cmake_clean.cmake
.PHONY : thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/clean

thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/depend:
	cd /home/forrest/Documents/book-tracker/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/forrest/Documents/book-tracker /home/forrest/Documents/book-tracker/thirdparty/nlohmann_json /home/forrest/Documents/book-tracker/build /home/forrest/Documents/book-tracker/build/thirdparty/nlohmann_json /home/forrest/Documents/book-tracker/build/thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : thirdparty/nlohmann_json/CMakeFiles/ContinuousConfigure.dir/depend

