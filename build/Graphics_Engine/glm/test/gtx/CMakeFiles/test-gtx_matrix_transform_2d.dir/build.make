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
CMAKE_SOURCE_DIR = /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build

# Include any dependencies generated for this target.
include Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/depend.make

# Include the progress variables for this target.
include Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/progress.make

# Include the compile flags for this target's objects.
include Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/flags.make

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o: Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/flags.make
Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o: ../Graphics_Engine/glm/test/gtx/gtx_matrix_transform_2d.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/gtx && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o -c /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/gtx/gtx_matrix_transform_2d.cpp

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.i"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/gtx && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/gtx/gtx_matrix_transform_2d.cpp > CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.i

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.s"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/gtx && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/gtx/gtx_matrix_transform_2d.cpp -o CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.s

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.requires:

.PHONY : Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.requires

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.provides: Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.requires
	$(MAKE) -f Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/build.make Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.provides.build
.PHONY : Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.provides

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.provides.build: Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o


# Object files for target test-gtx_matrix_transform_2d
test__gtx_matrix_transform_2d_OBJECTS = \
"CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o"

# External object files for target test-gtx_matrix_transform_2d
test__gtx_matrix_transform_2d_EXTERNAL_OBJECTS =

Graphics_Engine/glm/test/gtx/test-gtx_matrix_transform_2d: Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o
Graphics_Engine/glm/test/gtx/test-gtx_matrix_transform_2d: Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/build.make
Graphics_Engine/glm/test/gtx/test-gtx_matrix_transform_2d: Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test-gtx_matrix_transform_2d"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/gtx && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-gtx_matrix_transform_2d.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/build: Graphics_Engine/glm/test/gtx/test-gtx_matrix_transform_2d

.PHONY : Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/build

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/requires: Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/gtx_matrix_transform_2d.cpp.o.requires

.PHONY : Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/requires

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/clean:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/gtx && $(CMAKE_COMMAND) -P CMakeFiles/test-gtx_matrix_transform_2d.dir/cmake_clean.cmake
.PHONY : Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/clean

Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/depend:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/gtx /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/gtx /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Graphics_Engine/glm/test/gtx/CMakeFiles/test-gtx_matrix_transform_2d.dir/depend

