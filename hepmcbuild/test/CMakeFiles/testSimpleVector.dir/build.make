# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/sf_darkphotons/hepmc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/sf_darkphotons/hepmcbuild

# Include any dependencies generated for this target.
include test/CMakeFiles/testSimpleVector.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/testSimpleVector.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/testSimpleVector.dir/flags.make

test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o: test/CMakeFiles/testSimpleVector.dir/flags.make
test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o: /media/sf_darkphotons/hepmc/test/testSimpleVector.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_darkphotons/hepmcbuild/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o"
	cd /media/sf_darkphotons/hepmcbuild/test && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o -c /media/sf_darkphotons/hepmc/test/testSimpleVector.cc

test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.i"
	cd /media/sf_darkphotons/hepmcbuild/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_darkphotons/hepmc/test/testSimpleVector.cc > CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.i

test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.s"
	cd /media/sf_darkphotons/hepmcbuild/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_darkphotons/hepmc/test/testSimpleVector.cc -o CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.s

test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.requires:
.PHONY : test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.requires

test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.provides: test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.requires
	$(MAKE) -f test/CMakeFiles/testSimpleVector.dir/build.make test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.provides.build
.PHONY : test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.provides

test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.provides.build: test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o
.PHONY : test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.provides.build

# Object files for target testSimpleVector
testSimpleVector_OBJECTS = \
"CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o"

# External object files for target testSimpleVector
testSimpleVector_EXTERNAL_OBJECTS =

test/testSimpleVector: test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o
test/testSimpleVector: lib/libHepMC.so.4.0.0
test/testSimpleVector: test/CMakeFiles/testSimpleVector.dir/build.make
test/testSimpleVector: test/CMakeFiles/testSimpleVector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable testSimpleVector"
	cd /media/sf_darkphotons/hepmcbuild/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testSimpleVector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/testSimpleVector.dir/build: test/testSimpleVector
.PHONY : test/CMakeFiles/testSimpleVector.dir/build

test/CMakeFiles/testSimpleVector.dir/requires: test/CMakeFiles/testSimpleVector.dir/testSimpleVector.cc.o.requires
.PHONY : test/CMakeFiles/testSimpleVector.dir/requires

test/CMakeFiles/testSimpleVector.dir/clean:
	cd /media/sf_darkphotons/hepmcbuild/test && $(CMAKE_COMMAND) -P CMakeFiles/testSimpleVector.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/testSimpleVector.dir/clean

test/CMakeFiles/testSimpleVector.dir/depend:
	cd /media/sf_darkphotons/hepmcbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/sf_darkphotons/hepmc /media/sf_darkphotons/hepmc/test /media/sf_darkphotons/hepmcbuild /media/sf_darkphotons/hepmcbuild/test /media/sf_darkphotons/hepmcbuild/test/CMakeFiles/testSimpleVector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/testSimpleVector.dir/depend
