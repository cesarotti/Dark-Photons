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
include fio/CMakeFiles/HepMCfioS.dir/depend.make

# Include the progress variables for this target.
include fio/CMakeFiles/HepMCfioS.dir/progress.make

# Include the compile flags for this target's objects.
include fio/CMakeFiles/HepMCfioS.dir/flags.make

fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o: fio/CMakeFiles/HepMCfioS.dir/flags.make
fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o: /media/sf_darkphotons/hepmc/fio/HEPEVT_Wrapper.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_darkphotons/hepmcbuild/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o -c /media/sf_darkphotons/hepmc/fio/HEPEVT_Wrapper.cc

fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.i"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_darkphotons/hepmc/fio/HEPEVT_Wrapper.cc > CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.i

fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.s"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_darkphotons/hepmc/fio/HEPEVT_Wrapper.cc -o CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.s

fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.requires:
.PHONY : fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.requires

fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.provides: fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.requires
	$(MAKE) -f fio/CMakeFiles/HepMCfioS.dir/build.make fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.provides.build
.PHONY : fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.provides

fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.provides.build: fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o
.PHONY : fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.provides.build

fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o: fio/CMakeFiles/HepMCfioS.dir/flags.make
fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o: /media/sf_darkphotons/hepmc/fio/HerwigWrapper.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_darkphotons/hepmcbuild/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o -c /media/sf_darkphotons/hepmc/fio/HerwigWrapper.cc

fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.i"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_darkphotons/hepmc/fio/HerwigWrapper.cc > CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.i

fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.s"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_darkphotons/hepmc/fio/HerwigWrapper.cc -o CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.s

fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.requires:
.PHONY : fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.requires

fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.provides: fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.requires
	$(MAKE) -f fio/CMakeFiles/HepMCfioS.dir/build.make fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.provides.build
.PHONY : fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.provides

fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.provides.build: fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o
.PHONY : fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.provides.build

fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o: fio/CMakeFiles/HepMCfioS.dir/flags.make
fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o: /media/sf_darkphotons/hepmc/fio/IO_HEPEVT.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_darkphotons/hepmcbuild/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o -c /media/sf_darkphotons/hepmc/fio/IO_HEPEVT.cc

fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.i"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_darkphotons/hepmc/fio/IO_HEPEVT.cc > CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.i

fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.s"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_darkphotons/hepmc/fio/IO_HEPEVT.cc -o CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.s

fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.requires:
.PHONY : fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.requires

fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.provides: fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.requires
	$(MAKE) -f fio/CMakeFiles/HepMCfioS.dir/build.make fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.provides.build
.PHONY : fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.provides

fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.provides.build: fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o
.PHONY : fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.provides.build

fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o: fio/CMakeFiles/HepMCfioS.dir/flags.make
fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o: /media/sf_darkphotons/hepmc/fio/IO_HERWIG.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /media/sf_darkphotons/hepmcbuild/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o -c /media/sf_darkphotons/hepmc/fio/IO_HERWIG.cc

fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.i"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /media/sf_darkphotons/hepmc/fio/IO_HERWIG.cc > CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.i

fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.s"
	cd /media/sf_darkphotons/hepmcbuild/fio && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /media/sf_darkphotons/hepmc/fio/IO_HERWIG.cc -o CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.s

fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.requires:
.PHONY : fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.requires

fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.provides: fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.requires
	$(MAKE) -f fio/CMakeFiles/HepMCfioS.dir/build.make fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.provides.build
.PHONY : fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.provides

fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.provides.build: fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o
.PHONY : fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.provides.build

# Object files for target HepMCfioS
HepMCfioS_OBJECTS = \
"CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o" \
"CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o" \
"CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o" \
"CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o"

# External object files for target HepMCfioS
HepMCfioS_EXTERNAL_OBJECTS =

lib/libHepMCfio.a: fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o
lib/libHepMCfio.a: fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o
lib/libHepMCfio.a: fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o
lib/libHepMCfio.a: fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o
lib/libHepMCfio.a: fio/CMakeFiles/HepMCfioS.dir/build.make
lib/libHepMCfio.a: fio/CMakeFiles/HepMCfioS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../lib/libHepMCfio.a"
	cd /media/sf_darkphotons/hepmcbuild/fio && $(CMAKE_COMMAND) -P CMakeFiles/HepMCfioS.dir/cmake_clean_target.cmake
	cd /media/sf_darkphotons/hepmcbuild/fio && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HepMCfioS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
fio/CMakeFiles/HepMCfioS.dir/build: lib/libHepMCfio.a
.PHONY : fio/CMakeFiles/HepMCfioS.dir/build

fio/CMakeFiles/HepMCfioS.dir/requires: fio/CMakeFiles/HepMCfioS.dir/HEPEVT_Wrapper.cc.o.requires
fio/CMakeFiles/HepMCfioS.dir/requires: fio/CMakeFiles/HepMCfioS.dir/HerwigWrapper.cc.o.requires
fio/CMakeFiles/HepMCfioS.dir/requires: fio/CMakeFiles/HepMCfioS.dir/IO_HEPEVT.cc.o.requires
fio/CMakeFiles/HepMCfioS.dir/requires: fio/CMakeFiles/HepMCfioS.dir/IO_HERWIG.cc.o.requires
.PHONY : fio/CMakeFiles/HepMCfioS.dir/requires

fio/CMakeFiles/HepMCfioS.dir/clean:
	cd /media/sf_darkphotons/hepmcbuild/fio && $(CMAKE_COMMAND) -P CMakeFiles/HepMCfioS.dir/cmake_clean.cmake
.PHONY : fio/CMakeFiles/HepMCfioS.dir/clean

fio/CMakeFiles/HepMCfioS.dir/depend:
	cd /media/sf_darkphotons/hepmcbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/sf_darkphotons/hepmc /media/sf_darkphotons/hepmc/fio /media/sf_darkphotons/hepmcbuild /media/sf_darkphotons/hepmcbuild/fio /media/sf_darkphotons/hepmcbuild/fio/CMakeFiles/HepMCfioS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : fio/CMakeFiles/HepMCfioS.dir/depend

