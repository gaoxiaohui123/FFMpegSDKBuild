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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.10.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.10.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_code_params.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_code_params.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_code_params.dir/flags.make

tests/CMakeFiles/test_code_params.dir/code_params_test.c.o: tests/CMakeFiles/test_code_params.dir/flags.make
tests/CMakeFiles/test_code_params.dir/code_params_test.c.o: tests/code_params_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/test_code_params.dir/code_params_test.c.o"
	cd /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_code_params.dir/code_params_test.c.o   -c /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests/code_params_test.c

tests/CMakeFiles/test_code_params.dir/code_params_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_code_params.dir/code_params_test.c.i"
	cd /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests/code_params_test.c > CMakeFiles/test_code_params.dir/code_params_test.c.i

tests/CMakeFiles/test_code_params.dir/code_params_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_code_params.dir/code_params_test.c.s"
	cd /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests/code_params_test.c -o CMakeFiles/test_code_params.dir/code_params_test.c.s

tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.requires:

.PHONY : tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.requires

tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.provides: tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.requires
	$(MAKE) -f tests/CMakeFiles/test_code_params.dir/build.make tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.provides.build
.PHONY : tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.provides

tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.provides.build: tests/CMakeFiles/test_code_params.dir/code_params_test.c.o


# Object files for target test_code_params
test_code_params_OBJECTS = \
"CMakeFiles/test_code_params.dir/code_params_test.c.o"

# External object files for target test_code_params
test_code_params_EXTERNAL_OBJECTS =

bin/Release/test_code_params: tests/CMakeFiles/test_code_params.dir/code_params_test.c.o
bin/Release/test_code_params: tests/CMakeFiles/test_code_params.dir/build.make
bin/Release/test_code_params: bin/Release/libopenfec.a
bin/Release/test_code_params: tests/CMakeFiles/test_code_params.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../bin/Release/test_code_params"
	cd /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_code_params.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_code_params.dir/build: bin/Release/test_code_params

.PHONY : tests/CMakeFiles/test_code_params.dir/build

tests/CMakeFiles/test_code_params.dir/requires: tests/CMakeFiles/test_code_params.dir/code_params_test.c.o.requires

.PHONY : tests/CMakeFiles/test_code_params.dir/requires

tests/CMakeFiles/test_code_params.dir/clean:
	cd /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_code_params.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_code_params.dir/clean

tests/CMakeFiles/test_code_params.dir/depend:
	cd /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2 /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2 /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests /home/gxh/works/huichang/svc_codec/bizconf_svc_codec/openfec_v1.4.2/tests/CMakeFiles/test_code_params.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_code_params.dir/depend

