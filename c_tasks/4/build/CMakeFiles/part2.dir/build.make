# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\C_progs\_c_training_git\training_tasks\c_tasks\4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\C_progs\_c_training_git\training_tasks\c_tasks\4\build

# Include any dependencies generated for this target.
include CMakeFiles/part2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/part2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/part2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/part2.dir/flags.make

CMakeFiles/part2.dir/src/part2.c.obj: CMakeFiles/part2.dir/flags.make
CMakeFiles/part2.dir/src/part2.c.obj: D:/C_progs/_c_training_git/training_tasks/c_tasks/4/src/part2.c
CMakeFiles/part2.dir/src/part2.c.obj: CMakeFiles/part2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\C_progs\_c_training_git\training_tasks\c_tasks\4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/part2.dir/src/part2.c.obj"
	C:\Users\Vlad\gcc\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/part2.dir/src/part2.c.obj -MF CMakeFiles\part2.dir\src\part2.c.obj.d -o CMakeFiles\part2.dir\src\part2.c.obj -c D:\C_progs\_c_training_git\training_tasks\c_tasks\4\src\part2.c

CMakeFiles/part2.dir/src/part2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/part2.dir/src/part2.c.i"
	C:\Users\Vlad\gcc\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\C_progs\_c_training_git\training_tasks\c_tasks\4\src\part2.c > CMakeFiles\part2.dir\src\part2.c.i

CMakeFiles/part2.dir/src/part2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/part2.dir/src/part2.c.s"
	C:\Users\Vlad\gcc\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\C_progs\_c_training_git\training_tasks\c_tasks\4\src\part2.c -o CMakeFiles\part2.dir\src\part2.c.s

# Object files for target part2
part2_OBJECTS = \
"CMakeFiles/part2.dir/src/part2.c.obj"

# External object files for target part2
part2_EXTERNAL_OBJECTS =

part2.exe: CMakeFiles/part2.dir/src/part2.c.obj
part2.exe: CMakeFiles/part2.dir/build.make
part2.exe: CMakeFiles/part2.dir/linkLibs.rsp
part2.exe: CMakeFiles/part2.dir/objects1.rsp
part2.exe: CMakeFiles/part2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\C_progs\_c_training_git\training_tasks\c_tasks\4\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable part2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\part2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/part2.dir/build: part2.exe
.PHONY : CMakeFiles/part2.dir/build

CMakeFiles/part2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\part2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/part2.dir/clean

CMakeFiles/part2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\C_progs\_c_training_git\training_tasks\c_tasks\4 D:\C_progs\_c_training_git\training_tasks\c_tasks\4 D:\C_progs\_c_training_git\training_tasks\c_tasks\4\build D:\C_progs\_c_training_git\training_tasks\c_tasks\4\build D:\C_progs\_c_training_git\training_tasks\c_tasks\4\build\CMakeFiles\part2.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/part2.dir/depend

