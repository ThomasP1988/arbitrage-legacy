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
CMAKE_SOURCE_DIR = /home/tom/Projects/ctrade

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tom/Projects/ctrade/build

# Include any dependencies generated for this target.
include CMakeFiles/ctrade.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ctrade.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ctrade.dir/flags.make

CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.o: CMakeFiles/ctrade.dir/flags.make
CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.o: ../src/platforms/binance/platform.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tom/Projects/ctrade/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.o"
	/bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.o -c /home/tom/Projects/ctrade/src/platforms/binance/platform.cc

CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.i"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tom/Projects/ctrade/src/platforms/binance/platform.cc > CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.i

CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.s"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tom/Projects/ctrade/src/platforms/binance/platform.cc -o CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.s

CMakeFiles/ctrade.dir/src/shared/markets/state.cc.o: CMakeFiles/ctrade.dir/flags.make
CMakeFiles/ctrade.dir/src/shared/markets/state.cc.o: ../src/shared/markets/state.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tom/Projects/ctrade/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ctrade.dir/src/shared/markets/state.cc.o"
	/bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ctrade.dir/src/shared/markets/state.cc.o -c /home/tom/Projects/ctrade/src/shared/markets/state.cc

CMakeFiles/ctrade.dir/src/shared/markets/state.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ctrade.dir/src/shared/markets/state.cc.i"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tom/Projects/ctrade/src/shared/markets/state.cc > CMakeFiles/ctrade.dir/src/shared/markets/state.cc.i

CMakeFiles/ctrade.dir/src/shared/markets/state.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ctrade.dir/src/shared/markets/state.cc.s"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tom/Projects/ctrade/src/shared/markets/state.cc -o CMakeFiles/ctrade.dir/src/shared/markets/state.cc.s

CMakeFiles/ctrade.dir/src/shared/markets/path.cc.o: CMakeFiles/ctrade.dir/flags.make
CMakeFiles/ctrade.dir/src/shared/markets/path.cc.o: ../src/shared/markets/path.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tom/Projects/ctrade/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ctrade.dir/src/shared/markets/path.cc.o"
	/bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ctrade.dir/src/shared/markets/path.cc.o -c /home/tom/Projects/ctrade/src/shared/markets/path.cc

CMakeFiles/ctrade.dir/src/shared/markets/path.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ctrade.dir/src/shared/markets/path.cc.i"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tom/Projects/ctrade/src/shared/markets/path.cc > CMakeFiles/ctrade.dir/src/shared/markets/path.cc.i

CMakeFiles/ctrade.dir/src/shared/markets/path.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ctrade.dir/src/shared/markets/path.cc.s"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tom/Projects/ctrade/src/shared/markets/path.cc -o CMakeFiles/ctrade.dir/src/shared/markets/path.cc.s

CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.o: CMakeFiles/ctrade.dir/flags.make
CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.o: ../src/strategies/simple_strategie.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tom/Projects/ctrade/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.o"
	/bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.o -c /home/tom/Projects/ctrade/src/strategies/simple_strategie.cc

CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.i"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tom/Projects/ctrade/src/strategies/simple_strategie.cc > CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.i

CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.s"
	/bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tom/Projects/ctrade/src/strategies/simple_strategie.cc -o CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.s

ctrade: CMakeFiles/ctrade.dir/src/platforms/binance/platform.cc.o
ctrade: CMakeFiles/ctrade.dir/src/shared/markets/state.cc.o
ctrade: CMakeFiles/ctrade.dir/src/shared/markets/path.cc.o
ctrade: CMakeFiles/ctrade.dir/src/strategies/simple_strategie.cc.o
ctrade: CMakeFiles/ctrade.dir/build.make

.PHONY : ctrade

# Rule to build all files generated by this target.
CMakeFiles/ctrade.dir/build: ctrade

.PHONY : CMakeFiles/ctrade.dir/build

CMakeFiles/ctrade.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ctrade.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ctrade.dir/clean

CMakeFiles/ctrade.dir/depend:
	cd /home/tom/Projects/ctrade/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tom/Projects/ctrade /home/tom/Projects/ctrade /home/tom/Projects/ctrade/build /home/tom/Projects/ctrade/build /home/tom/Projects/ctrade/build/CMakeFiles/ctrade.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ctrade.dir/depend

