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
CMAKE_SOURCE_DIR = /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//adaptive_autosar_simulation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//build-adaptive_autosar_simulation-Desktop-Debug

# Utility rule file for adaptive_autosar_simulation_autogen.

# Include the progress variables for this target.
include CMakeFiles/adaptive_autosar_simulation_autogen.dir/progress.make

CMakeFiles/adaptive_autosar_simulation_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/shams/Github/OTA-Adaptive-AUTOSAR-Project//build-adaptive_autosar_simulation-Desktop-Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target adaptive_autosar_simulation"
	/usr/bin/cmake -E cmake_autogen /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//build-adaptive_autosar_simulation-Desktop-Debug/CMakeFiles/adaptive_autosar_simulation_autogen.dir/AutogenInfo.json Debug

adaptive_autosar_simulation_autogen: CMakeFiles/adaptive_autosar_simulation_autogen
adaptive_autosar_simulation_autogen: CMakeFiles/adaptive_autosar_simulation_autogen.dir/build.make

.PHONY : adaptive_autosar_simulation_autogen

# Rule to build all files generated by this target.
CMakeFiles/adaptive_autosar_simulation_autogen.dir/build: adaptive_autosar_simulation_autogen

.PHONY : CMakeFiles/adaptive_autosar_simulation_autogen.dir/build

CMakeFiles/adaptive_autosar_simulation_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/adaptive_autosar_simulation_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/adaptive_autosar_simulation_autogen.dir/clean

CMakeFiles/adaptive_autosar_simulation_autogen.dir/depend:
	cd /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//build-adaptive_autosar_simulation-Desktop-Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//adaptive_autosar_simulation /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//adaptive_autosar_simulation /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//build-adaptive_autosar_simulation-Desktop-Debug /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//build-adaptive_autosar_simulation-Desktop-Debug /home/shams/Github/OTA-Adaptive-AUTOSAR-Project//build-adaptive_autosar_simulation-Desktop-Debug/CMakeFiles/adaptive_autosar_simulation_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/adaptive_autosar_simulation_autogen.dir/depend

