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
include Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/depend.make

# Include the progress variables for this target.
include Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/progress.make

# Include the compile flags for this target's objects.
include Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/flags.make

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/flags.make
Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o: ../Graphics_Engine/glfw/tests/monitors.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/monitors.dir/monitors.c.o   -c /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/tests/monitors.c

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/monitors.dir/monitors.c.i"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/tests/monitors.c > CMakeFiles/monitors.dir/monitors.c.i

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/monitors.dir/monitors.c.s"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/tests/monitors.c -o CMakeFiles/monitors.dir/monitors.c.s

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.requires:

.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.requires

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.provides: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.requires
	$(MAKE) -f Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/build.make Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.provides.build
.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.provides

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.provides.build: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o


Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/flags.make
Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o: ../Graphics_Engine/glfw/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/monitors.dir/__/deps/getopt.c.o   -c /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/deps/getopt.c

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/monitors.dir/__/deps/getopt.c.i"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/deps/getopt.c > CMakeFiles/monitors.dir/__/deps/getopt.c.i

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/monitors.dir/__/deps/getopt.c.s"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/deps/getopt.c -o CMakeFiles/monitors.dir/__/deps/getopt.c.s

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.requires:

.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.requires

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.provides: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.requires
	$(MAKE) -f Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/build.make Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.provides.build
.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.provides

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.provides.build: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o


Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/flags.make
Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o: ../Graphics_Engine/glfw/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/monitors.dir/__/deps/glad.c.o   -c /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/deps/glad.c

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/monitors.dir/__/deps/glad.c.i"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/deps/glad.c > CMakeFiles/monitors.dir/__/deps/glad.c.i

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/monitors.dir/__/deps/glad.c.s"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/deps/glad.c -o CMakeFiles/monitors.dir/__/deps/glad.c.s

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.requires:

.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.requires

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.provides: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.requires
	$(MAKE) -f Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/build.make Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.provides.build
.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.provides

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.provides.build: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o


# Object files for target monitors
monitors_OBJECTS = \
"CMakeFiles/monitors.dir/monitors.c.o" \
"CMakeFiles/monitors.dir/__/deps/getopt.c.o" \
"CMakeFiles/monitors.dir/__/deps/glad.c.o"

# External object files for target monitors
monitors_EXTERNAL_OBJECTS =

Graphics_Engine/glfw/tests/monitors: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o
Graphics_Engine/glfw/tests/monitors: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o
Graphics_Engine/glfw/tests/monitors: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o
Graphics_Engine/glfw/tests/monitors: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/build.make
Graphics_Engine/glfw/tests/monitors: Graphics_Engine/glfw/src/libglfw3.a
Graphics_Engine/glfw/tests/monitors: /usr/lib/x86_64-linux-gnu/libm.so
Graphics_Engine/glfw/tests/monitors: /usr/lib/x86_64-linux-gnu/librt.so
Graphics_Engine/glfw/tests/monitors: /usr/lib/x86_64-linux-gnu/libm.so
Graphics_Engine/glfw/tests/monitors: /usr/lib/x86_64-linux-gnu/libX11.so
Graphics_Engine/glfw/tests/monitors: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable monitors"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/monitors.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/build: Graphics_Engine/glfw/tests/monitors

.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/build

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/requires: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/monitors.c.o.requires
Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/requires: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/getopt.c.o.requires
Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/requires: Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/__/deps/glad.c.o.requires

.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/requires

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/clean:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/monitors.dir/cmake_clean.cmake
.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/clean

Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/depend:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/Graphics_Engine/glfw/tests /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Graphics_Engine/glfw/tests/CMakeFiles/monitors.dir/depend

