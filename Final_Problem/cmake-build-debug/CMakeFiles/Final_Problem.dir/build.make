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
CMAKE_SOURCE_DIR = /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Final_Problem.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Final_Problem.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Final_Problem.dir/flags.make

CMakeFiles/Final_Problem.dir/main.c.o: CMakeFiles/Final_Problem.dir/flags.make
CMakeFiles/Final_Problem.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Final_Problem.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Final_Problem.dir/main.c.o   -c /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/main.c

CMakeFiles/Final_Problem.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Final_Problem.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/main.c > CMakeFiles/Final_Problem.dir/main.c.i

CMakeFiles/Final_Problem.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Final_Problem.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/main.c -o CMakeFiles/Final_Problem.dir/main.c.s

CMakeFiles/Final_Problem.dir/Source.c.o: CMakeFiles/Final_Problem.dir/flags.make
CMakeFiles/Final_Problem.dir/Source.c.o: ../Source.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Final_Problem.dir/Source.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Final_Problem.dir/Source.c.o   -c /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/Source.c

CMakeFiles/Final_Problem.dir/Source.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Final_Problem.dir/Source.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/Source.c > CMakeFiles/Final_Problem.dir/Source.c.i

CMakeFiles/Final_Problem.dir/Source.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Final_Problem.dir/Source.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/Source.c -o CMakeFiles/Final_Problem.dir/Source.c.s

# Object files for target Final_Problem
Final_Problem_OBJECTS = \
"CMakeFiles/Final_Problem.dir/main.c.o" \
"CMakeFiles/Final_Problem.dir/Source.c.o"

# External object files for target Final_Problem
Final_Problem_EXTERNAL_OBJECTS =

Final_Problem: CMakeFiles/Final_Problem.dir/main.c.o
Final_Problem: CMakeFiles/Final_Problem.dir/Source.c.o
Final_Problem: CMakeFiles/Final_Problem.dir/build.make
Final_Problem: CMakeFiles/Final_Problem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable Final_Problem"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Final_Problem.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Final_Problem.dir/build: Final_Problem

.PHONY : CMakeFiles/Final_Problem.dir/build

CMakeFiles/Final_Problem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Final_Problem.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Final_Problem.dir/clean

CMakeFiles/Final_Problem.dir/depend:
	cd /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug /home/bibi/Desktop/Programm/Operation_System/OS_Programm/Final_Problem/cmake-build-debug/CMakeFiles/Final_Problem.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Final_Problem.dir/depend

