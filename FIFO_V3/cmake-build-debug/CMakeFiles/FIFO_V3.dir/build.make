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
CMAKE_SOURCE_DIR = /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FIFO_V3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FIFO_V3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FIFO_V3.dir/flags.make

CMakeFiles/FIFO_V3.dir/main.c.o: CMakeFiles/FIFO_V3.dir/flags.make
CMakeFiles/FIFO_V3.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/FIFO_V3.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FIFO_V3.dir/main.c.o   -c /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/main.c

CMakeFiles/FIFO_V3.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FIFO_V3.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/main.c > CMakeFiles/FIFO_V3.dir/main.c.i

CMakeFiles/FIFO_V3.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FIFO_V3.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/main.c -o CMakeFiles/FIFO_V3.dir/main.c.s

CMakeFiles/FIFO_V3.dir/Reader.c.o: CMakeFiles/FIFO_V3.dir/flags.make
CMakeFiles/FIFO_V3.dir/Reader.c.o: ../Reader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/FIFO_V3.dir/Reader.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FIFO_V3.dir/Reader.c.o   -c /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/Reader.c

CMakeFiles/FIFO_V3.dir/Reader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FIFO_V3.dir/Reader.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/Reader.c > CMakeFiles/FIFO_V3.dir/Reader.c.i

CMakeFiles/FIFO_V3.dir/Reader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FIFO_V3.dir/Reader.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/Reader.c -o CMakeFiles/FIFO_V3.dir/Reader.c.s

CMakeFiles/FIFO_V3.dir/Writer.c.o: CMakeFiles/FIFO_V3.dir/flags.make
CMakeFiles/FIFO_V3.dir/Writer.c.o: ../Writer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/FIFO_V3.dir/Writer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FIFO_V3.dir/Writer.c.o   -c /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/Writer.c

CMakeFiles/FIFO_V3.dir/Writer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FIFO_V3.dir/Writer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/Writer.c > CMakeFiles/FIFO_V3.dir/Writer.c.i

CMakeFiles/FIFO_V3.dir/Writer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FIFO_V3.dir/Writer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/Writer.c -o CMakeFiles/FIFO_V3.dir/Writer.c.s

# Object files for target FIFO_V3
FIFO_V3_OBJECTS = \
"CMakeFiles/FIFO_V3.dir/main.c.o" \
"CMakeFiles/FIFO_V3.dir/Reader.c.o" \
"CMakeFiles/FIFO_V3.dir/Writer.c.o"

# External object files for target FIFO_V3
FIFO_V3_EXTERNAL_OBJECTS =

FIFO_V3: CMakeFiles/FIFO_V3.dir/main.c.o
FIFO_V3: CMakeFiles/FIFO_V3.dir/Reader.c.o
FIFO_V3: CMakeFiles/FIFO_V3.dir/Writer.c.o
FIFO_V3: CMakeFiles/FIFO_V3.dir/build.make
FIFO_V3: CMakeFiles/FIFO_V3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable FIFO_V3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FIFO_V3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FIFO_V3.dir/build: FIFO_V3

.PHONY : CMakeFiles/FIFO_V3.dir/build

CMakeFiles/FIFO_V3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FIFO_V3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FIFO_V3.dir/clean

CMakeFiles/FIFO_V3.dir/depend:
	cd /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3 /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3 /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug /home/bibi/Desktop/Programm/Operation_System/OS_Programm/FIFO_V3/cmake-build-debug/CMakeFiles/FIFO_V3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FIFO_V3.dir/depend

