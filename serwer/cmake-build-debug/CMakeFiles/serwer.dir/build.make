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
CMAKE_COMMAND = /home/s17kf/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/181.5087.36/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/s17kf/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/181.5087.36/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/s17kf/Pulpit/projekt_tin/serwer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/serwer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/serwer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/serwer.dir/flags.make

CMakeFiles/serwer.dir/main.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/serwer.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/main.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/main.cpp

CMakeFiles/serwer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/main.cpp > CMakeFiles/serwer.dir/main.cpp.i

CMakeFiles/serwer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/main.cpp -o CMakeFiles/serwer.dir/main.cpp.s

CMakeFiles/serwer.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/main.cpp.o.requires

CMakeFiles/serwer.dir/main.cpp.o.provides: CMakeFiles/serwer.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/main.cpp.o.provides

CMakeFiles/serwer.dir/main.cpp.o.provides.build: CMakeFiles/serwer.dir/main.cpp.o


CMakeFiles/serwer.dir/consts.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/consts.cpp.o: ../consts.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/serwer.dir/consts.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/consts.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/consts.cpp

CMakeFiles/serwer.dir/consts.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/consts.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/consts.cpp > CMakeFiles/serwer.dir/consts.cpp.i

CMakeFiles/serwer.dir/consts.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/consts.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/consts.cpp -o CMakeFiles/serwer.dir/consts.cpp.s

CMakeFiles/serwer.dir/consts.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/consts.cpp.o.requires

CMakeFiles/serwer.dir/consts.cpp.o.provides: CMakeFiles/serwer.dir/consts.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/consts.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/consts.cpp.o.provides

CMakeFiles/serwer.dir/consts.cpp.o.provides.build: CMakeFiles/serwer.dir/consts.cpp.o


CMakeFiles/serwer.dir/connection.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/connection.cpp.o: ../connection.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/serwer.dir/connection.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/connection.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/connection.cpp

CMakeFiles/serwer.dir/connection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/connection.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/connection.cpp > CMakeFiles/serwer.dir/connection.cpp.i

CMakeFiles/serwer.dir/connection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/connection.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/connection.cpp -o CMakeFiles/serwer.dir/connection.cpp.s

CMakeFiles/serwer.dir/connection.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/connection.cpp.o.requires

CMakeFiles/serwer.dir/connection.cpp.o.provides: CMakeFiles/serwer.dir/connection.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/connection.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/connection.cpp.o.provides

CMakeFiles/serwer.dir/connection.cpp.o.provides.build: CMakeFiles/serwer.dir/connection.cpp.o


CMakeFiles/serwer.dir/RNG.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/RNG.cpp.o: ../RNG.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/serwer.dir/RNG.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/RNG.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/RNG.cpp

CMakeFiles/serwer.dir/RNG.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/RNG.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/RNG.cpp > CMakeFiles/serwer.dir/RNG.cpp.i

CMakeFiles/serwer.dir/RNG.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/RNG.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/RNG.cpp -o CMakeFiles/serwer.dir/RNG.cpp.s

CMakeFiles/serwer.dir/RNG.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/RNG.cpp.o.requires

CMakeFiles/serwer.dir/RNG.cpp.o.provides: CMakeFiles/serwer.dir/RNG.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/RNG.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/RNG.cpp.o.provides

CMakeFiles/serwer.dir/RNG.cpp.o.provides.build: CMakeFiles/serwer.dir/RNG.cpp.o


CMakeFiles/serwer.dir/packet.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/packet.cpp.o: ../packet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/serwer.dir/packet.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/packet.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/packet.cpp

CMakeFiles/serwer.dir/packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/packet.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/packet.cpp > CMakeFiles/serwer.dir/packet.cpp.i

CMakeFiles/serwer.dir/packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/packet.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/packet.cpp -o CMakeFiles/serwer.dir/packet.cpp.s

CMakeFiles/serwer.dir/packet.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/packet.cpp.o.requires

CMakeFiles/serwer.dir/packet.cpp.o.provides: CMakeFiles/serwer.dir/packet.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/packet.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/packet.cpp.o.provides

CMakeFiles/serwer.dir/packet.cpp.o.provides.build: CMakeFiles/serwer.dir/packet.cpp.o


CMakeFiles/serwer.dir/log.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/log.cpp.o: ../log.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/serwer.dir/log.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/log.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/log.cpp

CMakeFiles/serwer.dir/log.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/log.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/log.cpp > CMakeFiles/serwer.dir/log.cpp.i

CMakeFiles/serwer.dir/log.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/log.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/log.cpp -o CMakeFiles/serwer.dir/log.cpp.s

CMakeFiles/serwer.dir/log.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/log.cpp.o.requires

CMakeFiles/serwer.dir/log.cpp.o.provides: CMakeFiles/serwer.dir/log.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/log.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/log.cpp.o.provides

CMakeFiles/serwer.dir/log.cpp.o.provides.build: CMakeFiles/serwer.dir/log.cpp.o


CMakeFiles/serwer.dir/privkey.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/privkey.cpp.o: ../privkey.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/serwer.dir/privkey.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/privkey.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/privkey.cpp

CMakeFiles/serwer.dir/privkey.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/privkey.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/privkey.cpp > CMakeFiles/serwer.dir/privkey.cpp.i

CMakeFiles/serwer.dir/privkey.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/privkey.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/privkey.cpp -o CMakeFiles/serwer.dir/privkey.cpp.s

CMakeFiles/serwer.dir/privkey.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/privkey.cpp.o.requires

CMakeFiles/serwer.dir/privkey.cpp.o.provides: CMakeFiles/serwer.dir/privkey.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/privkey.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/privkey.cpp.o.provides

CMakeFiles/serwer.dir/privkey.cpp.o.provides.build: CMakeFiles/serwer.dir/privkey.cpp.o


CMakeFiles/serwer.dir/pubkey.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/pubkey.cpp.o: ../pubkey.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/serwer.dir/pubkey.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/pubkey.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/pubkey.cpp

CMakeFiles/serwer.dir/pubkey.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/pubkey.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/pubkey.cpp > CMakeFiles/serwer.dir/pubkey.cpp.i

CMakeFiles/serwer.dir/pubkey.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/pubkey.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/pubkey.cpp -o CMakeFiles/serwer.dir/pubkey.cpp.s

CMakeFiles/serwer.dir/pubkey.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/pubkey.cpp.o.requires

CMakeFiles/serwer.dir/pubkey.cpp.o.provides: CMakeFiles/serwer.dir/pubkey.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/pubkey.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/pubkey.cpp.o.provides

CMakeFiles/serwer.dir/pubkey.cpp.o.provides.build: CMakeFiles/serwer.dir/pubkey.cpp.o


CMakeFiles/serwer.dir/sesskey.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/sesskey.cpp.o: ../sesskey.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/serwer.dir/sesskey.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/sesskey.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/sesskey.cpp

CMakeFiles/serwer.dir/sesskey.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/sesskey.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/sesskey.cpp > CMakeFiles/serwer.dir/sesskey.cpp.i

CMakeFiles/serwer.dir/sesskey.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/sesskey.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/sesskey.cpp -o CMakeFiles/serwer.dir/sesskey.cpp.s

CMakeFiles/serwer.dir/sesskey.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/sesskey.cpp.o.requires

CMakeFiles/serwer.dir/sesskey.cpp.o.provides: CMakeFiles/serwer.dir/sesskey.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/sesskey.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/sesskey.cpp.o.provides

CMakeFiles/serwer.dir/sesskey.cpp.o.provides.build: CMakeFiles/serwer.dir/sesskey.cpp.o


CMakeFiles/serwer.dir/androidClient.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/androidClient.cpp.o: ../androidClient.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/serwer.dir/androidClient.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/androidClient.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/androidClient.cpp

CMakeFiles/serwer.dir/androidClient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/androidClient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/androidClient.cpp > CMakeFiles/serwer.dir/androidClient.cpp.i

CMakeFiles/serwer.dir/androidClient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/androidClient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/androidClient.cpp -o CMakeFiles/serwer.dir/androidClient.cpp.s

CMakeFiles/serwer.dir/androidClient.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/androidClient.cpp.o.requires

CMakeFiles/serwer.dir/androidClient.cpp.o.provides: CMakeFiles/serwer.dir/androidClient.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/androidClient.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/androidClient.cpp.o.provides

CMakeFiles/serwer.dir/androidClient.cpp.o.provides.build: CMakeFiles/serwer.dir/androidClient.cpp.o


CMakeFiles/serwer.dir/AddQueue.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/AddQueue.cpp.o: ../AddQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/serwer.dir/AddQueue.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/AddQueue.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/AddQueue.cpp

CMakeFiles/serwer.dir/AddQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/AddQueue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/AddQueue.cpp > CMakeFiles/serwer.dir/AddQueue.cpp.i

CMakeFiles/serwer.dir/AddQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/AddQueue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/AddQueue.cpp -o CMakeFiles/serwer.dir/AddQueue.cpp.s

CMakeFiles/serwer.dir/AddQueue.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/AddQueue.cpp.o.requires

CMakeFiles/serwer.dir/AddQueue.cpp.o.provides: CMakeFiles/serwer.dir/AddQueue.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/AddQueue.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/AddQueue.cpp.o.provides

CMakeFiles/serwer.dir/AddQueue.cpp.o.provides.build: CMakeFiles/serwer.dir/AddQueue.cpp.o


CMakeFiles/serwer.dir/ReadQueue.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/ReadQueue.cpp.o: ../ReadQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/serwer.dir/ReadQueue.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/ReadQueue.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/ReadQueue.cpp

CMakeFiles/serwer.dir/ReadQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/ReadQueue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/ReadQueue.cpp > CMakeFiles/serwer.dir/ReadQueue.cpp.i

CMakeFiles/serwer.dir/ReadQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/ReadQueue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/ReadQueue.cpp -o CMakeFiles/serwer.dir/ReadQueue.cpp.s

CMakeFiles/serwer.dir/ReadQueue.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/ReadQueue.cpp.o.requires

CMakeFiles/serwer.dir/ReadQueue.cpp.o.provides: CMakeFiles/serwer.dir/ReadQueue.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/ReadQueue.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/ReadQueue.cpp.o.provides

CMakeFiles/serwer.dir/ReadQueue.cpp.o.provides.build: CMakeFiles/serwer.dir/ReadQueue.cpp.o


CMakeFiles/serwer.dir/queuePacket.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/queuePacket.cpp.o: ../queuePacket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/serwer.dir/queuePacket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/queuePacket.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/queuePacket.cpp

CMakeFiles/serwer.dir/queuePacket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/queuePacket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/queuePacket.cpp > CMakeFiles/serwer.dir/queuePacket.cpp.i

CMakeFiles/serwer.dir/queuePacket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/queuePacket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/queuePacket.cpp -o CMakeFiles/serwer.dir/queuePacket.cpp.s

CMakeFiles/serwer.dir/queuePacket.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/queuePacket.cpp.o.requires

CMakeFiles/serwer.dir/queuePacket.cpp.o.provides: CMakeFiles/serwer.dir/queuePacket.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/queuePacket.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/queuePacket.cpp.o.provides

CMakeFiles/serwer.dir/queuePacket.cpp.o.provides.build: CMakeFiles/serwer.dir/queuePacket.cpp.o


CMakeFiles/serwer.dir/DevDescriptor.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/DevDescriptor.cpp.o: ../DevDescriptor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object CMakeFiles/serwer.dir/DevDescriptor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/DevDescriptor.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/DevDescriptor.cpp

CMakeFiles/serwer.dir/DevDescriptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/DevDescriptor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/DevDescriptor.cpp > CMakeFiles/serwer.dir/DevDescriptor.cpp.i

CMakeFiles/serwer.dir/DevDescriptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/DevDescriptor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/DevDescriptor.cpp -o CMakeFiles/serwer.dir/DevDescriptor.cpp.s

CMakeFiles/serwer.dir/DevDescriptor.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/DevDescriptor.cpp.o.requires

CMakeFiles/serwer.dir/DevDescriptor.cpp.o.provides: CMakeFiles/serwer.dir/DevDescriptor.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/DevDescriptor.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/DevDescriptor.cpp.o.provides

CMakeFiles/serwer.dir/DevDescriptor.cpp.o.provides.build: CMakeFiles/serwer.dir/DevDescriptor.cpp.o


CMakeFiles/serwer.dir/serwer.cpp.o: CMakeFiles/serwer.dir/flags.make
CMakeFiles/serwer.dir/serwer.cpp.o: ../serwer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object CMakeFiles/serwer.dir/serwer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/serwer.dir/serwer.cpp.o -c /home/s17kf/Pulpit/projekt_tin/serwer/serwer.cpp

CMakeFiles/serwer.dir/serwer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/serwer.dir/serwer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/s17kf/Pulpit/projekt_tin/serwer/serwer.cpp > CMakeFiles/serwer.dir/serwer.cpp.i

CMakeFiles/serwer.dir/serwer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/serwer.dir/serwer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/s17kf/Pulpit/projekt_tin/serwer/serwer.cpp -o CMakeFiles/serwer.dir/serwer.cpp.s

CMakeFiles/serwer.dir/serwer.cpp.o.requires:

.PHONY : CMakeFiles/serwer.dir/serwer.cpp.o.requires

CMakeFiles/serwer.dir/serwer.cpp.o.provides: CMakeFiles/serwer.dir/serwer.cpp.o.requires
	$(MAKE) -f CMakeFiles/serwer.dir/build.make CMakeFiles/serwer.dir/serwer.cpp.o.provides.build
.PHONY : CMakeFiles/serwer.dir/serwer.cpp.o.provides

CMakeFiles/serwer.dir/serwer.cpp.o.provides.build: CMakeFiles/serwer.dir/serwer.cpp.o


# Object files for target serwer
serwer_OBJECTS = \
"CMakeFiles/serwer.dir/main.cpp.o" \
"CMakeFiles/serwer.dir/consts.cpp.o" \
"CMakeFiles/serwer.dir/connection.cpp.o" \
"CMakeFiles/serwer.dir/RNG.cpp.o" \
"CMakeFiles/serwer.dir/packet.cpp.o" \
"CMakeFiles/serwer.dir/log.cpp.o" \
"CMakeFiles/serwer.dir/privkey.cpp.o" \
"CMakeFiles/serwer.dir/pubkey.cpp.o" \
"CMakeFiles/serwer.dir/sesskey.cpp.o" \
"CMakeFiles/serwer.dir/androidClient.cpp.o" \
"CMakeFiles/serwer.dir/AddQueue.cpp.o" \
"CMakeFiles/serwer.dir/ReadQueue.cpp.o" \
"CMakeFiles/serwer.dir/queuePacket.cpp.o" \
"CMakeFiles/serwer.dir/DevDescriptor.cpp.o" \
"CMakeFiles/serwer.dir/serwer.cpp.o"

# External object files for target serwer
serwer_EXTERNAL_OBJECTS =

serwer: CMakeFiles/serwer.dir/main.cpp.o
serwer: CMakeFiles/serwer.dir/consts.cpp.o
serwer: CMakeFiles/serwer.dir/connection.cpp.o
serwer: CMakeFiles/serwer.dir/RNG.cpp.o
serwer: CMakeFiles/serwer.dir/packet.cpp.o
serwer: CMakeFiles/serwer.dir/log.cpp.o
serwer: CMakeFiles/serwer.dir/privkey.cpp.o
serwer: CMakeFiles/serwer.dir/pubkey.cpp.o
serwer: CMakeFiles/serwer.dir/sesskey.cpp.o
serwer: CMakeFiles/serwer.dir/androidClient.cpp.o
serwer: CMakeFiles/serwer.dir/AddQueue.cpp.o
serwer: CMakeFiles/serwer.dir/ReadQueue.cpp.o
serwer: CMakeFiles/serwer.dir/queuePacket.cpp.o
serwer: CMakeFiles/serwer.dir/DevDescriptor.cpp.o
serwer: CMakeFiles/serwer.dir/serwer.cpp.o
serwer: CMakeFiles/serwer.dir/build.make
serwer: CMakeFiles/serwer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Linking CXX executable serwer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/serwer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/serwer.dir/build: serwer

.PHONY : CMakeFiles/serwer.dir/build

CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/main.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/consts.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/connection.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/RNG.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/packet.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/log.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/privkey.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/pubkey.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/sesskey.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/androidClient.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/AddQueue.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/ReadQueue.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/queuePacket.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/DevDescriptor.cpp.o.requires
CMakeFiles/serwer.dir/requires: CMakeFiles/serwer.dir/serwer.cpp.o.requires

.PHONY : CMakeFiles/serwer.dir/requires

CMakeFiles/serwer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/serwer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/serwer.dir/clean

CMakeFiles/serwer.dir/depend:
	cd /home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/s17kf/Pulpit/projekt_tin/serwer /home/s17kf/Pulpit/projekt_tin/serwer /home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug /home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug /home/s17kf/Pulpit/projekt_tin/serwer/cmake-build-debug/CMakeFiles/serwer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/serwer.dir/depend

