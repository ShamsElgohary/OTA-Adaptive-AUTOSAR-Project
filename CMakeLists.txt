cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0037 OLD)
# use 17 insted of 14
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project(sm)


set(source_ara_exec_dir
   "${CMAKE_SOURCE_DIR}/ara/exec")

set(source_ara_sm_dir
   "${CMAKE_SOURCE_DIR}/ara/sm")

file(GLOB_RECURSE SRC_FILES_ara_exec_C ${source_ara_exec_dir}/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE SRC_FILES_ara_exec_h ${source_ara_exec_dir}/include/*.hpp CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE SRC_FILES_ara_sm_C ${source_ara_sm_dir}/*.cpp CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE SRC_FILES_ara_sm_h ${source_ara_sm_dir}/*.hpp CMAKE_CONFIGURE_DEPENDS)


add_library(
     ara_exec
     ${SRC_FILES_ara_exec_C}
     ${SRC_FILES_ara_exec_h} 
   )

# use SRC_FILES_ara_sm_C and SRC_FILES_ara_sm_h becouse we do not need main.c in add library
add_library(
     ara_sm
     ${source_ara_sm_dir}/SM.hpp
     ${source_ara_sm_dir}/SM.cpp
   )

# named it from include_directories to target_include_directories also made it public 
 target_include_directories(ara_exec PUBLIC ${source_ara_exec_dir}/include )
 target_include_directories(ara_sm PUBLIC ${source_ara_sm_dir})

 ## for boost::filesystem at ara/exec
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")


if(NOT TARGET Boost::filesystem)
    add_library(Boost::filesystem IMPORTED INTERFACE)
    set_property(TARGET Boost::filesystem PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
    set_property(TARGET Boost::filesystem PROPERTY
        INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})
endif()
## generate exe

add_executable(sm
     ${SRC_FILES_ara_sm_C}
     ${SRC_FILES_ara_sm_h}
     ${SRC_FILES_ara_exec_C}
     ${SRC_FILES_ara_exec_h}
 )
 target_link_libraries(
  sm
  ara_sm
  ara_exec
  Boost::filesystem
  )
  