# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/roger/Workspace/Tarbora/Tarbora/Framework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roger/Workspace/Tarbora/Tarbora/Framework/Module

# Include any dependencies generated for this target.
include External/CMakeFiles/lua.dir/depend.make

# Include the progress variables for this target.
include External/CMakeFiles/lua.dir/progress.make

# Include the compile flags for this target's objects.
include External/CMakeFiles/lua.dir/flags.make

External/CMakeFiles/lua.dir/lua/onelua.o: External/CMakeFiles/lua.dir/flags.make
External/CMakeFiles/lua.dir/lua/onelua.o: ../External/lua/onelua.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/Tarbora/Framework/Module/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object External/CMakeFiles/lua.dir/lua/onelua.o"
	cd /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/External && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lua.dir/lua/onelua.o   -c /home/roger/Workspace/Tarbora/Tarbora/Framework/External/lua/onelua.c

External/CMakeFiles/lua.dir/lua/onelua.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lua.dir/lua/onelua.i"
	cd /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/External && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/External/lua/onelua.c > CMakeFiles/lua.dir/lua/onelua.i

External/CMakeFiles/lua.dir/lua/onelua.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lua.dir/lua/onelua.s"
	cd /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/External && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/External/lua/onelua.c -o CMakeFiles/lua.dir/lua/onelua.s

External/CMakeFiles/lua.dir/lua/onelua.o.requires:

.PHONY : External/CMakeFiles/lua.dir/lua/onelua.o.requires

External/CMakeFiles/lua.dir/lua/onelua.o.provides: External/CMakeFiles/lua.dir/lua/onelua.o.requires
	$(MAKE) -f External/CMakeFiles/lua.dir/build.make External/CMakeFiles/lua.dir/lua/onelua.o.provides.build
.PHONY : External/CMakeFiles/lua.dir/lua/onelua.o.provides

External/CMakeFiles/lua.dir/lua/onelua.o.provides.build: External/CMakeFiles/lua.dir/lua/onelua.o


# Object files for target lua
lua_OBJECTS = \
"CMakeFiles/lua.dir/lua/onelua.o"

# External object files for target lua
lua_EXTERNAL_OBJECTS =

External/liblua.so: External/CMakeFiles/lua.dir/lua/onelua.o
External/liblua.so: External/CMakeFiles/lua.dir/build.make
External/liblua.so: External/CMakeFiles/lua.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/roger/Workspace/Tarbora/Tarbora/Framework/Module/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library liblua.so"
	cd /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/External && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lua.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
External/CMakeFiles/lua.dir/build: External/liblua.so

.PHONY : External/CMakeFiles/lua.dir/build

External/CMakeFiles/lua.dir/requires: External/CMakeFiles/lua.dir/lua/onelua.o.requires

.PHONY : External/CMakeFiles/lua.dir/requires

External/CMakeFiles/lua.dir/clean:
	cd /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/External && $(CMAKE_COMMAND) -P CMakeFiles/lua.dir/cmake_clean.cmake
.PHONY : External/CMakeFiles/lua.dir/clean

External/CMakeFiles/lua.dir/depend:
	cd /home/roger/Workspace/Tarbora/Tarbora/Framework/Module && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roger/Workspace/Tarbora/Tarbora/Framework /home/roger/Workspace/Tarbora/Tarbora/Framework/External /home/roger/Workspace/Tarbora/Tarbora/Framework/Module /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/External /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/External/CMakeFiles/lua.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : External/CMakeFiles/lua.dir/depend

