# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "MinSizeRel")
  file(REMOVE_RECURSE
  "CMakeFiles/Car_GUI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Car_GUI_autogen.dir/ParseCache.txt"
  "CMakeFiles/execution_lib_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/execution_lib_autogen.dir/ParseCache.txt"
  "Car_GUI_autogen"
  "execution_lib_autogen"
  )
endif()
