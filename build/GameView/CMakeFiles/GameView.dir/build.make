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
include GameView/CMakeFiles/GameView.dir/depend.make

# Include the progress variables for this target.
include GameView/CMakeFiles/GameView.dir/progress.make

# Include the compile flags for this target's objects.
include GameView/CMakeFiles/GameView.dir/flags.make

GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o: GameView/CMakeFiles/GameView.dir/flags.make
GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o: ../GameView/src/HumanView.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/GameView && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/GameView.dir/src/HumanView.cpp.o -c /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/GameView/src/HumanView.cpp

GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/GameView.dir/src/HumanView.cpp.i"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/GameView && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/GameView/src/HumanView.cpp > CMakeFiles/GameView.dir/src/HumanView.cpp.i

GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/GameView.dir/src/HumanView.cpp.s"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/GameView && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/GameView/src/HumanView.cpp -o CMakeFiles/GameView.dir/src/HumanView.cpp.s

GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.requires:

.PHONY : GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.requires

GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.provides: GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.requires
	$(MAKE) -f GameView/CMakeFiles/GameView.dir/build.make GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.provides.build
.PHONY : GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.provides

GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.provides.build: GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o


# Object files for target GameView
GameView_OBJECTS = \
"CMakeFiles/GameView.dir/src/HumanView.cpp.o"

# External object files for target GameView
GameView_EXTERNAL_OBJECTS =

GameView/libGameView.so: GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o
GameView/libGameView.so: GameView/CMakeFiles/GameView.dir/build.make
GameView/libGameView.so: GameView/CMakeFiles/GameView.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libGameView.so"
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/GameView && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GameView.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
GameView/CMakeFiles/GameView.dir/build: GameView/libGameView.so

.PHONY : GameView/CMakeFiles/GameView.dir/build

GameView/CMakeFiles/GameView.dir/requires: GameView/CMakeFiles/GameView.dir/src/HumanView.cpp.o.requires

.PHONY : GameView/CMakeFiles/GameView.dir/requires

GameView/CMakeFiles/GameView.dir/clean:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/GameView && $(CMAKE_COMMAND) -P CMakeFiles/GameView.dir/cmake_clean.cmake
.PHONY : GameView/CMakeFiles/GameView.dir/clean

GameView/CMakeFiles/GameView.dir/depend:
	cd /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/GameView /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/GameView /home/roger/Workspace/UPC/tfg-tarboragameengine/Tarbora/build/GameView/CMakeFiles/GameView.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : GameView/CMakeFiles/GameView.dir/depend

