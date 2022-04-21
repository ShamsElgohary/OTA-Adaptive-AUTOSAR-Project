cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)
project(Adaptive_platform)
###############################EM###############################

set(exec_source_dir "${CMAKE_SOURCE_DIR}/ara/exec")
set(em_install_dir "${CMAKE_SOURCE_DIR}/executables/em/bin")
find_package(Boost 1.50 COMPONENTS system filesystem program_options serialization REQUIRED)
file(GLOB_RECURSE SRC_FILES ${exec_source_dir}/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(em ${SRC_FILES})
target_include_directories(em PUBLIC  ${exec_source_dir}/include)
target_link_libraries(em Boost::filesystem Boost::program_options)
install(TARGETS em RUNTIME DESTINATION ${em_install_dir})

###############################EM_LIB###############################

add_library(execution_lib STATIC ${exec_source_dir}/src/execution_client.cpp ${exec_source_dir}/src/state_client.cpp ${exec_source_dir}/src/function_group_state.cpp ${exec_source_dir}/src/find_process_client.cpp)
target_include_directories(execution_lib PUBLIC  ${exec_source_dir}/include)

###############################EX1###############################

set(source_excutables_ex1_dir "${CMAKE_SOURCE_DIR}/executables/ex1/bin")
add_executable(ex1 ${source_excutables_ex1_dir}/ex1.cpp) 
target_link_libraries( ex1 execution_lib )
install(TARGETS ex1 RUNTIME DESTINATION ${source_excutables_ex1_dir})

###############################EX2###############################

set(source_excutables_ex2_dir  "${CMAKE_SOURCE_DIR}/executables/ex2/bin")
add_executable(ex2 ${source_excutables_ex2_dir}/ex2.cpp) 
target_link_libraries( ex2 execution_lib )
install(TARGETS ex2 RUNTIME DESTINATION ${source_excutables_ex2_dir})

###############################SM###############################

set(source_excutables_sm_dir "${CMAKE_SOURCE_DIR}/executables/sm/bin")
add_executable(sm ${source_excutables_sm_dir}/sm.cpp) 
target_link_libraries( sm execution_lib )
install(TARGETS sm RUNTIME DESTINATION ${source_excutables_sm_dir})

################################IAM#############################

set(iam_install_dir "${CMAKE_SOURCE_DIR}/executables/iam/bin")
set(iam_source_dir "${CMAKE_SOURCE_DIR}/ara/iam")
file(GLOB_RECURSE IAM_SRC_FILES ${iam_source_dir}/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(iam ${SRC_FILES} ${iam_source_dir}/lib/src/Grant.cpp) 
target_include_directories(iam PUBLIC  ${iam_source_dir}/include  ${iam_source_dir}/lib/include )
target_link_libraries(iam execution_lib Boost::filesystem Boost::program_options Boost::serialization)
install(TARGETS iam RUNTIME DESTINATION ${iam_install_dir})

################################IAM_LIB##########################

file(GLOB_RECURSE IAM_LIB_SRC_FILES ${iam_source_dir}/lib/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_library(iam_lib STATIC ${IAM_LIB_SRC_FILES} )
target_link_libraries(iam_lib  Boost::filesystem Boost::program_options Boost::serialization)

target_include_directories(iam_lib PUBLIC  ${iam_source_dir}/lib/include )

################################jsoncpp_lib##########################

set(jsoncpp_dir "${CMAKE_SOURCE_DIR}/jsoncpp")
add_library(jsoncpp_lib STATIC ${jsoncpp_dir}/src/jsoncpp.cpp)
target_include_directories(jsoncpp_lib PUBLIC ${jsoncpp_dir}/jsoncpp/header)

################################COM_LIB##########################
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(com_source_dir "${CMAKE_SOURCE_DIR}/ara/com")
file(GLOB_RECURSE COM_Proxy_SRC_FILES ${com_source_dir}/language_binding/proxy/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_Skeleton_SRC_FILES ${com_source_dir}/language_binding/skeleton/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_NetworkBinding_SRC_FILES ${com_source_dir}/network_binding/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_SomeIp_SRC_FILES ${com_source_dir}/network_binding/SomeIp/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_serviceDiscovery_SRC_FILES ${com_source_dir}/network_binding/SomeIp/servicediscovery/lib/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)

add_library(com_lib STATIC ${COM_Proxy_SRC_FILES} ${COM_Skeleton_SRC_FILES} ${COM_NetworkBinding_SRC_FILES} ${COM_SomeIp_SRC_FILES} ${COM_serviceDiscovery_SRC_FILES} )
target_link_libraries(com_lib Boost::filesystem Boost::program_options Boost::serialization jsoncpp_lib)

target_include_directories(
        com_lib PUBLIC  ${com_source_dir}/language_binding/proxy/include  
        ${com_source_dir}/language_binding/skeleton/include 
        ${com_source_dir}/network_binding
        ${com_source_dir}/network_binding/SomeIp
        ${com_source_dir}/network_binding/SomeIp/servicediscovery/lib/include
        ) 

################################SomeIP_SD##########################

set(someIp_SD_install_dir "${CMAKE_SOURCE_DIR}/executables/someIp_SD/bin")
set(COM_SERVICE_DiSCOVERY_MAIN_SRC_FILES "${com_source_dir}/network_binding/SomeIp/servicediscovery/src")

add_executable(someip_SD ${COM_serviceDiscovery_SRC_FILES} ${COM_SERVICE_DiSCOVERY_MAIN_SRC_FILES}/sd-main.cpp)
target_link_libraries(someip_SD com_lib)
target_include_directories(someip_SD PUBLIC ${com_source_dir}/network_binding/SomeIp/servicediscovery/lib/include ) 
install(TARGETS someip_SD RUNTIME DESTINATION ${someIp_SD_install_dir})

################################COM_test##########################

set(test_server_source_dir "${CMAKE_SOURCE_DIR}/ara/test/Server")
add_executable(test_server ${test_server_source_dir}/main.cpp ) 
target_link_libraries(test_server com_lib )
target_include_directories( test_server PUBLIC ${test_server_source_dir} )
install(TARGETS test_server RUNTIME DESTINATION ${test_server_source_dir})

set(test_client_source_dir "${CMAKE_SOURCE_DIR}/ara/test/Client")
add_executable(test_client ${test_client_source_dir}/main.cpp) 
target_include_directories( test_client PUBLIC ${test_client_source_dir})
target_link_libraries(test_client com_lib)
install(TARGETS test_client RUNTIME DESTINATION ${test_client_source_dir})

##################################################################

# project(sm)


# set(source_ara_exec_dir
#    "${CMAKE_SOURCE_DIR}/ara/exec")

# set(source_ara_sm_dir
#    "${CMAKE_SOURCE_DIR}/ara/sm")

# file(GLOB_RECURSE SRC_FILES_ara_exec_C ${source_ara_exec_dir}/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
# file(GLOB_RECURSE SRC_FILES_ara_exec_h ${source_ara_exec_dir}/include/*.hpp CMAKE_CONFIGURE_DEPENDS)
# file(GLOB_RECURSE SRC_FILES_ara_sm_C ${source_ara_sm_dir}/*.cpp CMAKE_CONFIGURE_DEPENDS)
# file(GLOB_RECURSE SRC_FILES_ara_sm_h ${source_ara_sm_dir}/*.hpp CMAKE_CONFIGURE_DEPENDS)

# list (REMOVE_ITEM SRC_FILES_ara_exec_C ${source_ara_exec_dir}/src/main.cpp)

# add_library(
#      ara_exec
#      ${SRC_FILES_ara_exec_C}
#      ${SRC_FILES_ara_exec_h} 
#    )

# # use SRC_FILES_ara_sm_C and SRC_FILES_ara_sm_h becouse we do not need main.c in add library
# add_library(
#      ara_sm
#      ${source_ara_sm_dir}/SM.hpp
#      ${source_ara_sm_dir}/SM.cpp
#)

# # named it from include_directories to target_include_directories also made it public 
#  target_include_directories(ara_exec PUBLIC ${source_ara_exec_dir}/include )
#  target_include_directories(ara_sm PUBLIC ${source_ara_sm_dir})

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

# add_executable(sm
#      ${SRC_FILES_ara_sm_C}
#      ${SRC_FILES_ara_sm_h}
#      ${SRC_FILES_ara_exec_C}
#      ${SRC_FILES_ara_exec_h}
#  )
#  target_link_libraries(
#   sm
#   ara_sm
#   ara_exec
#   Boost::filesystem
#   )
  
