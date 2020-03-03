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
CMAKE_SOURCE_DIR = /home/roger/Workspace/Tarbora

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roger/Workspace/Tarbora/build

# Include any dependencies generated for this target.
include Tarbora/Framework/CMakeFiles/TarboraCore.dir/depend.make

# Include the progress variables for this target.
include Tarbora/Framework/CMakeFiles/TarboraCore.dir/progress.make

# Include the compile flags for this target's objects.
include Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make

Tarbora/Framework/messages.pb.cc: ../Tarbora/Framework/MessageManager/messages.proto
Tarbora/Framework/messages.pb.cc: /usr/local/bin/protoc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running C++ protocol buffer compiler on MessageManager/messages.proto"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/local/bin/protoc --cpp_out=/home/roger/Workspace/Tarbora/build/Tarbora/Framework -I /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/messages.proto

Tarbora/Framework/messages.pb.h: Tarbora/Framework/messages.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate Tarbora/Framework/messages.pb.h

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o: ../Tarbora/Framework/Logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/Logger.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/Logger.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/Logger.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/Logger.cpp > CMakeFiles/TarboraCore.dir/Logger.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/Logger.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/Logger.cpp -o CMakeFiles/TarboraCore.dir/Logger.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o: ../Tarbora/Framework/Module/AbstractModule.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/AbstractModule.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/AbstractModule.cpp > CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/AbstractModule.cpp -o CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o: ../Tarbora/Framework/Module/Module.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/Module/Module.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/Module.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/Module/Module.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/Module.cpp > CMakeFiles/TarboraCore.dir/Module/Module.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/Module/Module.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/Module.cpp -o CMakeFiles/TarboraCore.dir/Module/Module.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o: ../Tarbora/Framework/Module/ModuleComponent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/ModuleComponent.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/ModuleComponent.cpp > CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/Module/ModuleComponent.cpp -o CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o: ../Tarbora/Framework/ResourceManager/ResourceManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/ResourceManager/ResourceManager.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/ResourceManager/ResourceManager.cpp > CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/ResourceManager/ResourceManager.cpp -o CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o: ../Tarbora/Framework/ResourceManager/Resource.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/ResourceManager/Resource.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/ResourceManager/Resource.cpp > CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/ResourceManager/Resource.cpp -o CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o: ../Tarbora/Framework/Math/Math.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/Math/Math.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/Math/Math.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/Math/Math.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/Math/Math.cpp > CMakeFiles/TarboraCore.dir/Math/Math.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/Math/Math.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/Math/Math.cpp -o CMakeFiles/TarboraCore.dir/Math/Math.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o: ../Tarbora/Framework/MessageManager/MessageClient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageClient.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageClient.cpp > CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageClient.cpp -o CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o: ../Tarbora/Framework/MessageManager/MessageHub.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageHub.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageHub.cpp > CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageHub.cpp -o CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o: ../Tarbora/Framework/MessageManager/MessageManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o -c /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageManager.cpp

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageManager.cpp > CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/Tarbora/Framework/MessageManager/MessageManager.cpp -o CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o


Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o: Tarbora/Framework/CMakeFiles/TarboraCore.dir/flags.make
Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o: Tarbora/Framework/messages.pb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TarboraCore.dir/messages.pb.cc.o -c /home/roger/Workspace/Tarbora/build/Tarbora/Framework/messages.pb.cc

Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TarboraCore.dir/messages.pb.cc.i"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roger/Workspace/Tarbora/build/Tarbora/Framework/messages.pb.cc > CMakeFiles/TarboraCore.dir/messages.pb.cc.i

Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TarboraCore.dir/messages.pb.cc.s"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roger/Workspace/Tarbora/build/Tarbora/Framework/messages.pb.cc -o CMakeFiles/TarboraCore.dir/messages.pb.cc.s

Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.requires:

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.provides: Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.requires
	$(MAKE) -f Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.provides.build
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.provides

Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.provides.build: Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o


# Object files for target TarboraCore
TarboraCore_OBJECTS = \
"CMakeFiles/TarboraCore.dir/Logger.cpp.o" \
"CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o" \
"CMakeFiles/TarboraCore.dir/Module/Module.cpp.o" \
"CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o" \
"CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o" \
"CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o" \
"CMakeFiles/TarboraCore.dir/Math/Math.cpp.o" \
"CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o" \
"CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o" \
"CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o" \
"CMakeFiles/TarboraCore.dir/messages.pb.cc.o"

# External object files for target TarboraCore
TarboraCore_EXTERNAL_OBJECTS =

Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/build.make
Tarbora/Framework/libTarboraCore.a: Tarbora/Framework/CMakeFiles/TarboraCore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/roger/Workspace/Tarbora/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX static library libTarboraCore.a"
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && $(CMAKE_COMMAND) -P CMakeFiles/TarboraCore.dir/cmake_clean_target.cmake
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TarboraCore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Tarbora/Framework/CMakeFiles/TarboraCore.dir/build: Tarbora/Framework/libTarboraCore.a

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/build

Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Logger.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/AbstractModule.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/Module.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Module/ModuleComponent.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/ResourceManager.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/ResourceManager/Resource.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/Math/Math.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageClient.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageHub.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/MessageManager/MessageManager.cpp.o.requires
Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires: Tarbora/Framework/CMakeFiles/TarboraCore.dir/messages.pb.cc.o.requires

.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/requires

Tarbora/Framework/CMakeFiles/TarboraCore.dir/clean:
	cd /home/roger/Workspace/Tarbora/build/Tarbora/Framework && $(CMAKE_COMMAND) -P CMakeFiles/TarboraCore.dir/cmake_clean.cmake
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/clean

Tarbora/Framework/CMakeFiles/TarboraCore.dir/depend: Tarbora/Framework/messages.pb.cc
Tarbora/Framework/CMakeFiles/TarboraCore.dir/depend: Tarbora/Framework/messages.pb.h
	cd /home/roger/Workspace/Tarbora/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roger/Workspace/Tarbora /home/roger/Workspace/Tarbora/Tarbora/Framework /home/roger/Workspace/Tarbora/build /home/roger/Workspace/Tarbora/build/Tarbora/Framework /home/roger/Workspace/Tarbora/build/Tarbora/Framework/CMakeFiles/TarboraCore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Tarbora/Framework/CMakeFiles/TarboraCore.dir/depend

