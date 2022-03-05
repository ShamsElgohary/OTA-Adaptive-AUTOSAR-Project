cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)


set(source_ara_exec_dir "${CMAKE_SOURCE_DIR}/ara/exec")

project(em)
find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
file(GLOB_RECURSE SRC_FILES ${source_ara_exec_dir}/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(em ${SRC_FILES})
target_include_directories(em PUBLIC  ${source_ara_exec_dir}/include)
target_link_libraries(em Boost::filesystem )


add_library(execution_lib STATIC ${source_ara_exec_dir}/src/execution_client.cpp ${source_ara_exec_dir}/src/state_client.cpp ${source_ara_exec_dir}/src/function_group_state.cpp)
target_include_directories(execution_lib PUBLIC  ${source_ara_exec_dir}/include)

project(ex1)
set(source_excutables_ex1_dir "${CMAKE_SOURCE_DIR}/executables/ex1/bin")
add_executable(ex1 ${source_excutables_ex1_dir}/ex1.cpp) 
target_link_libraries( ex1 execution_lib )
install(TARGETS ex1 RUNTIME DESTINATION ${source_excutables_ex1_dir})


project(ex2)
set(source_excutables_ex2_dir  "${CMAKE_SOURCE_DIR}/executables/ex2/bin")
add_executable(ex2 ${source_excutables_ex2_dir}/ex2.cpp) 
target_link_libraries( ex2 execution_lib )
install(TARGETS ex2 RUNTIME DESTINATION ${source_excutables_ex2_dir})

# project(sm)
# set(source_excutables_sm_dir   "${CMAKE_SOURCE_DIR}/executables/sm/bin")
# add_executable(sm ${source_excutables_sm_dir}/sm.cpp) 
# target_link_libraries( sm execution_lib )
# install(TARGETS sm RUNTIME DESTINATION ${source_excutables_sm_dir})
# cmake_policy(SET CMP0037 OLD)
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

list (REMOVE_ITEM SRC_FILES_ara_exec_C ${source_ara_exec_dir}/src/main.cpp)

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
  
