# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /mnt/c/Users/Huawei/CLionProjects/llp_3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/client_exec.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client_exec.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client_exec.dir/flags.make

CMakeFiles/client_exec.dir/client_dir/client.c.o: CMakeFiles/client_exec.dir/flags.make
CMakeFiles/client_exec.dir/client_dir/client.c.o: ../client_dir/client.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/client_exec.dir/client_dir/client.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client_exec.dir/client_dir/client.c.o   -c /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/client.c

CMakeFiles/client_exec.dir/client_dir/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client_exec.dir/client_dir/client.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/client.c > CMakeFiles/client_exec.dir/client_dir/client.c.i

CMakeFiles/client_exec.dir/client_dir/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client_exec.dir/client_dir/client.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/client.c -o CMakeFiles/client_exec.dir/client_dir/client.c.s

CMakeFiles/client_exec.dir/client_dir/structures/structures.c.o: CMakeFiles/client_exec.dir/flags.make
CMakeFiles/client_exec.dir/client_dir/structures/structures.c.o: ../client_dir/structures/structures.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/client_exec.dir/client_dir/structures/structures.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client_exec.dir/client_dir/structures/structures.c.o   -c /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/structures/structures.c

CMakeFiles/client_exec.dir/client_dir/structures/structures.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client_exec.dir/client_dir/structures/structures.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/structures/structures.c > CMakeFiles/client_exec.dir/client_dir/structures/structures.c.i

CMakeFiles/client_exec.dir/client_dir/structures/structures.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client_exec.dir/client_dir/structures/structures.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/structures/structures.c -o CMakeFiles/client_exec.dir/client_dir/structures/structures.c.s

CMakeFiles/client_exec.dir/client_dir/parser.tab.c.o: CMakeFiles/client_exec.dir/flags.make
CMakeFiles/client_exec.dir/client_dir/parser.tab.c.o: ../client_dir/parser.tab.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/client_exec.dir/client_dir/parser.tab.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client_exec.dir/client_dir/parser.tab.c.o   -c /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/parser.tab.c

CMakeFiles/client_exec.dir/client_dir/parser.tab.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client_exec.dir/client_dir/parser.tab.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/parser.tab.c > CMakeFiles/client_exec.dir/client_dir/parser.tab.c.i

CMakeFiles/client_exec.dir/client_dir/parser.tab.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client_exec.dir/client_dir/parser.tab.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/parser.tab.c -o CMakeFiles/client_exec.dir/client_dir/parser.tab.c.s

CMakeFiles/client_exec.dir/client_dir/lex.yy.c.o: CMakeFiles/client_exec.dir/flags.make
CMakeFiles/client_exec.dir/client_dir/lex.yy.c.o: ../client_dir/lex.yy.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/client_exec.dir/client_dir/lex.yy.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client_exec.dir/client_dir/lex.yy.c.o   -c /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/lex.yy.c

CMakeFiles/client_exec.dir/client_dir/lex.yy.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client_exec.dir/client_dir/lex.yy.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/lex.yy.c > CMakeFiles/client_exec.dir/client_dir/lex.yy.c.i

CMakeFiles/client_exec.dir/client_dir/lex.yy.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client_exec.dir/client_dir/lex.yy.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/lex.yy.c -o CMakeFiles/client_exec.dir/client_dir/lex.yy.c.s

CMakeFiles/client_exec.dir/client_dir/serealizator.c.o: CMakeFiles/client_exec.dir/flags.make
CMakeFiles/client_exec.dir/client_dir/serealizator.c.o: ../client_dir/serealizator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/client_exec.dir/client_dir/serealizator.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client_exec.dir/client_dir/serealizator.c.o   -c /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/serealizator.c

CMakeFiles/client_exec.dir/client_dir/serealizator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client_exec.dir/client_dir/serealizator.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/serealizator.c > CMakeFiles/client_exec.dir/client_dir/serealizator.c.i

CMakeFiles/client_exec.dir/client_dir/serealizator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client_exec.dir/client_dir/serealizator.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Huawei/CLionProjects/llp_3/client_dir/serealizator.c -o CMakeFiles/client_exec.dir/client_dir/serealizator.c.s

CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.o: CMakeFiles/client_exec.dir/flags.make
CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.o: ../protobuf/protobuf-c/protobuf-c/protobuf-c.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.o   -c /mnt/c/Users/Huawei/CLionProjects/llp_3/protobuf/protobuf-c/protobuf-c/protobuf-c.c

CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Huawei/CLionProjects/llp_3/protobuf/protobuf-c/protobuf-c/protobuf-c.c > CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.i

CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Huawei/CLionProjects/llp_3/protobuf/protobuf-c/protobuf-c/protobuf-c.c -o CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.s

CMakeFiles/client_exec.dir/spec.pb-c.c.o: CMakeFiles/client_exec.dir/flags.make
CMakeFiles/client_exec.dir/spec.pb-c.c.o: ../spec.pb-c.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/client_exec.dir/spec.pb-c.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client_exec.dir/spec.pb-c.c.o   -c /mnt/c/Users/Huawei/CLionProjects/llp_3/spec.pb-c.c

CMakeFiles/client_exec.dir/spec.pb-c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client_exec.dir/spec.pb-c.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Huawei/CLionProjects/llp_3/spec.pb-c.c > CMakeFiles/client_exec.dir/spec.pb-c.c.i

CMakeFiles/client_exec.dir/spec.pb-c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client_exec.dir/spec.pb-c.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Huawei/CLionProjects/llp_3/spec.pb-c.c -o CMakeFiles/client_exec.dir/spec.pb-c.c.s

# Object files for target client_exec
client_exec_OBJECTS = \
"CMakeFiles/client_exec.dir/client_dir/client.c.o" \
"CMakeFiles/client_exec.dir/client_dir/structures/structures.c.o" \
"CMakeFiles/client_exec.dir/client_dir/parser.tab.c.o" \
"CMakeFiles/client_exec.dir/client_dir/lex.yy.c.o" \
"CMakeFiles/client_exec.dir/client_dir/serealizator.c.o" \
"CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.o" \
"CMakeFiles/client_exec.dir/spec.pb-c.c.o"

# External object files for target client_exec
client_exec_EXTERNAL_OBJECTS =

client_exec: CMakeFiles/client_exec.dir/client_dir/client.c.o
client_exec: CMakeFiles/client_exec.dir/client_dir/structures/structures.c.o
client_exec: CMakeFiles/client_exec.dir/client_dir/parser.tab.c.o
client_exec: CMakeFiles/client_exec.dir/client_dir/lex.yy.c.o
client_exec: CMakeFiles/client_exec.dir/client_dir/serealizator.c.o
client_exec: CMakeFiles/client_exec.dir/protobuf/protobuf-c/protobuf-c/protobuf-c.c.o
client_exec: CMakeFiles/client_exec.dir/spec.pb-c.c.o
client_exec: CMakeFiles/client_exec.dir/build.make
client_exec: CMakeFiles/client_exec.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable client_exec"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client_exec.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client_exec.dir/build: client_exec

.PHONY : CMakeFiles/client_exec.dir/build

CMakeFiles/client_exec.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client_exec.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client_exec.dir/clean

CMakeFiles/client_exec.dir/depend:
	cd /mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Huawei/CLionProjects/llp_3 /mnt/c/Users/Huawei/CLionProjects/llp_3 /mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug /mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug /mnt/c/Users/Huawei/CLionProjects/llp_3/cmake-build-debug/CMakeFiles/client_exec.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client_exec.dir/depend
