# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/adaptive_autosar_simulation_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/adaptive_autosar_simulation_autogen.dir/ParseCache.txt"
  "adaptive_autosar_simulation_autogen"
  )
endif()
