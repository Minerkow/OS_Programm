# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /home/bibi/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/202.6948.80/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/bibi/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/202.6948.80/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bibi/Desktop/Programm/Operation_System/SecondProblem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SecondProblem.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SecondProblem.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SecondProblem.dir/flags.make

CMakeFiles/SecondProblem.dir/main.c.o: CMakeFiles/SecondProblem.dir/flags.make
CMakeFiles/SecondProblem.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SecondProblem.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SecondProblem.dir/main.c.o   -c /home/bibi/Desktop/Programm/Operation_System/SecondProblem/main.c

CMakeFiles/SecondProblem.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SecondProblem.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/SecondProblem/main.c > CMakeFiles/SecondProblem.dir/main.c.i

CMakeFiles/SecondProblem.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SecondProblem.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/SecondProblem/main.c -o CMakeFiles/SecondProblem.dir/main.c.s

CMakeFiles/SecondProblem.dir/msg.c.o: CMakeFiles/SecondProblem.dir/flags.make
CMakeFiles/SecondProblem.dir/msg.c.o: ../msg.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/SecondProblem.dir/msg.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SecondProblem.dir/msg.c.o   -c /home/bibi/Desktop/Programm/Operation_System/SecondProblem/msg.c

CMakeFiles/SecondProblem.dir/msg.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SecondProblem.dir/msg.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/SecondProblem/msg.c > CMakeFiles/SecondProblem.dir/msg.c.i

CMakeFiles/SecondProblem.dir/msg.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SecondProblem.dir/msg.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/SecondProblem/msg.c -o CMakeFiles/SecondProblem.dir/msg.c.s

# Object files for target SecondProblem
SecondProblem_OBJECTS = \
"CMakeFiles/SecondProblem.dir/main.c.o" \
"CMakeFiles/SecondProblem.dir/msg.c.o"

# External object files for target SecondProblem
SecondProblem_EXTERNAL_OBJECTS =

SecondProblem: CMakeFiles/SecondProblem.dir/main.c.o
SecondProblem: CMakeFiles/SecondProblem.dir/msg.c.o
SecondProblem: CMakeFiles/SecondProblem.dir/build.make
SecondProblem: CMakeFiles/SecondProblem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable SecondProblem"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SecondProblem.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SecondProblem.dir/build: SecondProblem

.PHONY : CMakeFiles/SecondProblem.dir/build

CMakeFiles/SecondProblem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SecondProblem.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SecondProblem.dir/clean

CMakeFiles/SecondProblem.dir/depend:
	cd /home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bibi/Desktop/Programm/Operation_System/SecondProblem /home/bibi/Desktop/Programm/Operation_System/SecondProblem /home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug /home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug /home/bibi/Desktop/Programm/Operation_System/SecondProblem/cmake-build-debug/CMakeFiles/SecondProblem.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SecondProblem.dir/depend

