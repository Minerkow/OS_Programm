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
CMAKE_SOURCE_DIR = /home/bibi/Desktop/Programm/Operation_System/3_problem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bibi/Desktop/Programm/Operation_System/3_problem

# Include any dependencies generated for this target.
include CMakeFiles/3_problem.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/3_problem.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/3_problem.dir/flags.make

CMakeFiles/3_problem.dir/main.c.o: CMakeFiles/3_problem.dir/flags.make
CMakeFiles/3_problem.dir/main.c.o: main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/3_problem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/3_problem.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/3_problem.dir/main.c.o   -c /home/bibi/Desktop/Programm/Operation_System/3_problem/main.c

CMakeFiles/3_problem.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/3_problem.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/3_problem/main.c > CMakeFiles/3_problem.dir/main.c.i

CMakeFiles/3_problem.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/3_problem.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/3_problem/main.c -o CMakeFiles/3_problem.dir/main.c.s

CMakeFiles/3_problem.dir/Reader.c.o: CMakeFiles/3_problem.dir/flags.make
CMakeFiles/3_problem.dir/Reader.c.o: Reader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/3_problem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/3_problem.dir/Reader.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/3_problem.dir/Reader.c.o   -c /home/bibi/Desktop/Programm/Operation_System/3_problem/Reader.c

CMakeFiles/3_problem.dir/Reader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/3_problem.dir/Reader.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/3_problem/Reader.c > CMakeFiles/3_problem.dir/Reader.c.i

CMakeFiles/3_problem.dir/Reader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/3_problem.dir/Reader.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/3_problem/Reader.c -o CMakeFiles/3_problem.dir/Reader.c.s

CMakeFiles/3_problem.dir/Writer.c.o: CMakeFiles/3_problem.dir/flags.make
CMakeFiles/3_problem.dir/Writer.c.o: Writer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/3_problem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/3_problem.dir/Writer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/3_problem.dir/Writer.c.o   -c /home/bibi/Desktop/Programm/Operation_System/3_problem/Writer.c

CMakeFiles/3_problem.dir/Writer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/3_problem.dir/Writer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/3_problem/Writer.c > CMakeFiles/3_problem.dir/Writer.c.i

CMakeFiles/3_problem.dir/Writer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/3_problem.dir/Writer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/3_problem/Writer.c -o CMakeFiles/3_problem.dir/Writer.c.s

# Object files for target 3_problem
3_problem_OBJECTS = \
"CMakeFiles/3_problem.dir/main.c.o" \
"CMakeFiles/3_problem.dir/Reader.c.o" \
"CMakeFiles/3_problem.dir/Writer.c.o"

# External object files for target 3_problem
3_problem_EXTERNAL_OBJECTS =

3_problem: CMakeFiles/3_problem.dir/main.c.o
3_problem: CMakeFiles/3_problem.dir/Reader.c.o
3_problem: CMakeFiles/3_problem.dir/Writer.c.o
3_problem: CMakeFiles/3_problem.dir/build.make
3_problem: CMakeFiles/3_problem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bibi/Desktop/Programm/Operation_System/3_problem/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable 3_problem"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/3_problem.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/3_problem.dir/build: 3_problem

.PHONY : CMakeFiles/3_problem.dir/build

CMakeFiles/3_problem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/3_problem.dir/cmake_clean.cmake
.PHONY : CMakeFiles/3_problem.dir/clean

CMakeFiles/3_problem.dir/depend:
	cd /home/bibi/Desktop/Programm/Operation_System/3_problem && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bibi/Desktop/Programm/Operation_System/3_problem /home/bibi/Desktop/Programm/Operation_System/3_problem /home/bibi/Desktop/Programm/Operation_System/3_problem /home/bibi/Desktop/Programm/Operation_System/3_problem /home/bibi/Desktop/Programm/Operation_System/3_problem/CMakeFiles/3_problem.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/3_problem.dir/depend

