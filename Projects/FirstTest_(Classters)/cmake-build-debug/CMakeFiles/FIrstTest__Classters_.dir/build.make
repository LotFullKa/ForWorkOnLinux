# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /snap/clion/103/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/103/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/FIrstTest__Classters_.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FIrstTest__Classters_.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FIrstTest__Classters_.dir/flags.make

CMakeFiles/FIrstTest__Classters_.dir/main.cpp.o: CMakeFiles/FIrstTest__Classters_.dir/flags.make
CMakeFiles/FIrstTest__Classters_.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FIrstTest__Classters_.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FIrstTest__Classters_.dir/main.cpp.o -c "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/main.cpp"

CMakeFiles/FIrstTest__Classters_.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FIrstTest__Classters_.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/main.cpp" > CMakeFiles/FIrstTest__Classters_.dir/main.cpp.i

CMakeFiles/FIrstTest__Classters_.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FIrstTest__Classters_.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/main.cpp" -o CMakeFiles/FIrstTest__Classters_.dir/main.cpp.s

# Object files for target FIrstTest__Classters_
FIrstTest__Classters__OBJECTS = \
"CMakeFiles/FIrstTest__Classters_.dir/main.cpp.o"

# External object files for target FIrstTest__Classters_
FIrstTest__Classters__EXTERNAL_OBJECTS =

FIrstTest__Classters_: CMakeFiles/FIrstTest__Classters_.dir/main.cpp.o
FIrstTest__Classters_: CMakeFiles/FIrstTest__Classters_.dir/build.make
FIrstTest__Classters_: CMakeFiles/FIrstTest__Classters_.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FIrstTest__Classters_"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FIrstTest__Classters_.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FIrstTest__Classters_.dir/build: FIrstTest__Classters_

.PHONY : CMakeFiles/FIrstTest__Classters_.dir/build

CMakeFiles/FIrstTest__Classters_.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FIrstTest__Classters_.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FIrstTest__Classters_.dir/clean

CMakeFiles/FIrstTest__Classters_.dir/depend:
	cd "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)" "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)" "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/cmake-build-debug" "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/cmake-build-debug" "/home/kamil/Experements/ForWorkOnLinux/Projects/FIrstTest_(Classters)/cmake-build-debug/CMakeFiles/FIrstTest__Classters_.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/FIrstTest__Classters_.dir/depend

