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
include Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/depend.make

# Include the progress variables for this target.
include Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/progress.make

# Include the compile flags for this target's objects.
include Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/flags.make

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o: Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/flags.make
Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o: ../Graphics_Engine/glm/test/ext/ext_scalar_int_sized.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/ext && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o -c /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/ext/ext_scalar_int_sized.cpp

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.i"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/ext && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/ext/ext_scalar_int_sized.cpp > CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.i

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.s"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/ext && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/ext/ext_scalar_int_sized.cpp -o CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.s

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.requires:

.PHONY : Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.requires

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.provides: Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.requires
	$(MAKE) -f Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/build.make Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.provides.build
.PHONY : Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.provides

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.provides.build: Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o


# Object files for target test-ext_scalar_int_sized
test__ext_scalar_int_sized_OBJECTS = \
"CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o"

# External object files for target test-ext_scalar_int_sized
test__ext_scalar_int_sized_EXTERNAL_OBJECTS =

Graphics_Engine/glm/test/ext/test-ext_scalar_int_sized: Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o
Graphics_Engine/glm/test/ext/test-ext_scalar_int_sized: Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/build.make
Graphics_Engine/glm/test/ext/test-ext_scalar_int_sized: Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test-ext_scalar_int_sized"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/ext && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-ext_scalar_int_sized.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/build: Graphics_Engine/glm/test/ext/test-ext_scalar_int_sized

.PHONY : Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/build

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/requires: Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/ext_scalar_int_sized.cpp.o.requires

.PHONY : Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/requires

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/clean:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/ext && $(CMAKE_COMMAND) -P CMakeFiles/test-ext_scalar_int_sized.dir/cmake_clean.cmake
.PHONY : Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/clean

Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/depend:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glm/test/ext /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/ext /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Graphics_Engine/glm/test/ext/CMakeFiles/test-ext_scalar_int_sized.dir/depend

