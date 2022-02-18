cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0037 OLD)
# project(sm)
set (CMAKE_CXX_STANDARD 14)

# set(source_ara_exec_dir
#    "${CMAKE_SOURCE_DIR}/ara/exec")

#  set(source_ara_sm_dir
#    "${CMAKE_SOURCE_DIR}/ara/sm")
# add_library(
#      ara_exec
#      ${source_ara_exec_dir}/include/application.hpp
#      ${source_ara_exec_dir}/include/applicationDependencyOrdering.hpp
#      ${source_ara_exec_dir}/include/applicationExecutionMgr.hpp
#      ${source_ara_exec_dir}/include/applicationManifest.hpp
#      ${source_ara_exec_dir}/include/execution_client.hpp
#      ${source_ara_exec_dir}/include/function_group.hpp
#      ${source_ara_exec_dir}/include/function_group_state.hpp
#      ${source_ara_exec_dir}/include/machineManifest.hpp
#      ${source_ara_exec_dir}/include/state_client.hpp
#      ${source_ara_exec_dir}/src/application.cpp
#      ${source_ara_exec_dir}/src/applicationDependencyOrdering.cpp
#      ${source_ara_exec_dir}/src/applicationExecutionMgr.cpp
#      ${source_ara_exec_dir}/src/applicationManifest.cpp
#      ${source_ara_exec_dir}/src/execution_client.cpp
#      ${source_ara_exec_dir}/src/function_group.cpp
#      ${source_ara_exec_dir}/src/function_group_state.cpp
#      ${source_ara_exec_dir}/src/machineManifest.cpp
#      ${source_ara_exec_dir}/src/state_client.cpp)
#    add_library(
#      ara_sm
#      ${source_ara_sm_dir}/SM.hpp
#      ${source_ara_sm_dir}/SM.cpp
#    )
#  include_directories("${CMAKE_SOURCE_DIR}/ara/exec/include" "${CMAKE_SOURCE_DIR}/ara/sm")

#  ## for boost::filesystem at ara/exec
# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
# find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
# message(STATUS "Boost version: ${Boost_VERSION}")


# if(NOT TARGET Boost::filesystem)
#     add_library(Boost::filesystem IMPORTED INTERFACE)
#     set_property(TARGET Boost::filesystem PROPERTY
#         INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
#     set_property(TARGET Boost::filesystem PROPERTY
#         INTERFACE_LINK_LIBRARIES ${Boost_LIBRARIES})
# endif()
# ## generate exe
#  add_executable(sm
#      ${source_ara_sm_dir}/main.cpp
#      ${source_ara_exec_dir}/include/application.hpp
#      ${source_ara_exec_dir}/include/applicationDependencyOrdering.hpp
#      ${source_ara_exec_dir}/include/applicationExecutionMgr.hpp
#      ${source_ara_exec_dir}/include/applicationManifest.hpp
#      ${source_ara_exec_dir}/include/execution_client.hpp
#      ${source_ara_exec_dir}/include/function_group.hpp
#      ${source_ara_exec_dir}/include/function_group_state.hpp
#      ${source_ara_exec_dir}/include/machineManifest.hpp
#      ${source_ara_exec_dir}/include/state_client.hpp
#      ${source_ara_exec_dir}/src/application.cpp
#      ${source_ara_exec_dir}/src/applicationDependencyOrdering.cpp
#      ${source_ara_exec_dir}/src/applicationExecutionMgr.cpp
#      ${source_ara_exec_dir}/src/applicationManifest.cpp
#      ${source_ara_exec_dir}/src/execution_client.cpp
#      ${source_ara_exec_dir}/src/function_group.cpp
#      ${source_ara_exec_dir}/src/function_group_state.cpp
#      ${source_ara_exec_dir}/src/machineManifest.cpp
#      ${source_ara_exec_dir}/src/state_client.cpp
#      ${source_ara_sm_dir}/SM.hpp
#      ${source_ara_sm_dir}/SM.cpp
#  )
#  target_link_libraries(
#   sm
#   ara_sm
#   ara_exec
#   Boost::filesystem
#   )



#####################################################

project(em)

set(source_ara_exec_dir
   "${CMAKE_SOURCE_DIR}/ara/exec")

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

  file(GLOB_RECURSE SRC_FILES ${source_ara_exec_dir}/*.cpp CMAKE_CONFIGURE_DEPENDS)
  add_executable(em ${SRC_FILES})
  target_include_directories(em PUBLIC  ${source_ara_exec_dir}/include)
 target_link_libraries(
  em
  Boost::filesystem
  )
