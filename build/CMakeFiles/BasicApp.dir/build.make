# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = C:\Users\danie\AppData\Local\Programs\Python\Python37\Lib\site-packages\cmake\data\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\danie\AppData\Local\Programs\Python\Python37\Lib\site-packages\cmake\data\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Repositories\Medit\MAseminar\ONNO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Repositories\Medit\MAseminar\ONNO\build

# Include any dependencies generated for this target.
include CMakeFiles/BasicApp.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/BasicApp.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/BasicApp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BasicApp.dir/flags.make

CMakeFiles/BasicApp.dir/main.cpp.obj: CMakeFiles/BasicApp.dir/flags.make
CMakeFiles/BasicApp.dir/main.cpp.obj: CMakeFiles/BasicApp.dir/includes_CXX.rsp
CMakeFiles/BasicApp.dir/main.cpp.obj: C:/Repositories/Medit/MAseminar/ONNO/main.cpp
CMakeFiles/BasicApp.dir/main.cpp.obj: CMakeFiles/BasicApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Repositories\Medit\MAseminar\ONNO\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BasicApp.dir/main.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BasicApp.dir/main.cpp.obj -MF CMakeFiles\BasicApp.dir\main.cpp.obj.d -o CMakeFiles\BasicApp.dir\main.cpp.obj -c C:\Repositories\Medit\MAseminar\ONNO\main.cpp

CMakeFiles/BasicApp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BasicApp.dir/main.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Repositories\Medit\MAseminar\ONNO\main.cpp > CMakeFiles\BasicApp.dir\main.cpp.i

CMakeFiles/BasicApp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BasicApp.dir/main.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Repositories\Medit\MAseminar\ONNO\main.cpp -o CMakeFiles\BasicApp.dir\main.cpp.s

CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj: CMakeFiles/BasicApp.dir/flags.make
CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj: CMakeFiles/BasicApp.dir/includes_CXX.rsp
CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj: C:/Repositories/Medit/MAseminar/ONNO/source/ControllerBox.cpp
CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj: CMakeFiles/BasicApp.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Repositories\Medit\MAseminar\ONNO\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj -MF CMakeFiles\BasicApp.dir\source\ControllerBox.cpp.obj.d -o CMakeFiles\BasicApp.dir\source\ControllerBox.cpp.obj -c C:\Repositories\Medit\MAseminar\ONNO\source\ControllerBox.cpp

CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Repositories\Medit\MAseminar\ONNO\source\ControllerBox.cpp > CMakeFiles\BasicApp.dir\source\ControllerBox.cpp.i

CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Repositories\Medit\MAseminar\ONNO\source\ControllerBox.cpp -o CMakeFiles\BasicApp.dir\source\ControllerBox.cpp.s

# Object files for target BasicApp
BasicApp_OBJECTS = \
"CMakeFiles/BasicApp.dir/main.cpp.obj" \
"CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj"

# External object files for target BasicApp
BasicApp_EXTERNAL_OBJECTS =

BasicApp.exe: CMakeFiles/BasicApp.dir/main.cpp.obj
BasicApp.exe: CMakeFiles/BasicApp.dir/source/ControllerBox.cpp.obj
BasicApp.exe: CMakeFiles/BasicApp.dir/build.make
BasicApp.exe: C:/msys64/mingw64/lib/libglfw3.dll.a
BasicApp.exe: C:/src/vcpkg/packages/imgui_x86-windows/debug/lib/imguid.lib
BasicApp.exe: CMakeFiles/BasicApp.dir/linkLibs.rsp
BasicApp.exe: CMakeFiles/BasicApp.dir/objects1.rsp
BasicApp.exe: CMakeFiles/BasicApp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Repositories\Medit\MAseminar\ONNO\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable BasicApp.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\BasicApp.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BasicApp.dir/build: BasicApp.exe
.PHONY : CMakeFiles/BasicApp.dir/build

CMakeFiles/BasicApp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\BasicApp.dir\cmake_clean.cmake
.PHONY : CMakeFiles/BasicApp.dir/clean

CMakeFiles/BasicApp.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Repositories\Medit\MAseminar\ONNO C:\Repositories\Medit\MAseminar\ONNO C:\Repositories\Medit\MAseminar\ONNO\build C:\Repositories\Medit\MAseminar\ONNO\build C:\Repositories\Medit\MAseminar\ONNO\build\CMakeFiles\BasicApp.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BasicApp.dir/depend

