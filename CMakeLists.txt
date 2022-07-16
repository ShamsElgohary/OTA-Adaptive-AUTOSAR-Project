cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

project(Adaptive_platform)

################################jsoncpp_lib##########################

set(jsoncpp_dir "${CMAKE_SOURCE_DIR}/utility/jsoncpp")
add_library(jsoncpp_lib STATIC ${jsoncpp_dir}/src/jsoncpp.cpp)
target_include_directories(jsoncpp_lib PUBLIC ${jsoncpp_dir}/jsoncpp/header)

################################sim_sock_lib##########################

set(sim_sock_dir "${CMAKE_SOURCE_DIR}/utility/simulation")
add_library(sim_sock_lib STATIC ${sim_sock_dir}/src/simulation.cpp)
target_include_directories(sim_sock_lib PUBLIC ${sim_sock_dir}/include)

###############################EM###############################

set(exec_source_dir "${CMAKE_SOURCE_DIR}/ara/exec")
set(em_install_dir "${CMAKE_SOURCE_DIR}/executables/em/bin")
find_package(Boost 1.50 COMPONENTS system filesystem program_options serialization REQUIRED)
file(GLOB_RECURSE SRC_FILES ${exec_source_dir}/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(em ${SRC_FILES})
target_include_directories(em PUBLIC  ${exec_source_dir}/include)
target_link_libraries(em Boost::filesystem Boost::program_options jsoncpp_lib sim_sock_lib)
install(TARGETS em RUNTIME DESTINATION ${em_install_dir})

###############################EM_LIB###############################

add_library(execution_lib STATIC ${exec_source_dir}/src/execution_client.cpp ${exec_source_dir}/src/state_client.cpp ${exec_source_dir}/src/function_group_state.cpp ${exec_source_dir}/src/find_process_client.cpp)
target_include_directories(execution_lib PUBLIC  ${exec_source_dir}/include)


################################IAM#############################

set(iam_install_dir "${CMAKE_SOURCE_DIR}/executables/iam/0.1/bin")
set(iam_source_dir "${CMAKE_SOURCE_DIR}/ara/iam")
file(GLOB_RECURSE IAM_SRC_FILES ${iam_source_dir}/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE IAM_SRC_LIB_FILES ${iam_source_dir}/lib/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(iam ${IAM_SRC_FILES} ${IAM_SRC_LIB_FILES}) 
target_include_directories(iam PUBLIC  ${iam_source_dir}/include  ${iam_source_dir}/lib/include )
target_link_libraries(iam execution_lib Boost::filesystem Boost::program_options Boost::serialization boost_thread pthread boost_system jsoncpp_lib sim_sock_lib)
install(TARGETS iam RUNTIME DESTINATION ${iam_install_dir})

################################IAM_LIB##########################

file(GLOB_RECURSE IAM_LIB_SRC_FILES ${iam_source_dir}/lib/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_library(iam_lib STATIC ${IAM_LIB_SRC_FILES} )
target_link_libraries(iam_lib  Boost::filesystem Boost::program_options Boost::serialization)

target_include_directories(iam_lib PUBLIC  ${iam_source_dir}/lib/include )


################################COM_LIB##########################
set(com_source_dir "${CMAKE_SOURCE_DIR}/ara/com")
file(GLOB_RECURSE COM_Proxy_SRC_FILES ${com_source_dir}/language_binding/proxy/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_Skeleton_SRC_FILES ${com_source_dir}/language_binding/skeleton/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_NetworkBinding_SRC_FILES ${com_source_dir}/network_binding/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_SomeIp_SRC_FILES ${com_source_dir}/network_binding/SomeIp/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_serviceDiscovery_SRC_FILES ${com_source_dir}/network_binding/SomeIp/servicediscovery/lib/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_someip_TLS_FILES ${com_source_dir}/network_binding/SomeIp/TLS/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_GUI_Report_SRC_FILES ${com_source_dir}/*.cpp  CMAKE_CONFIGURE_DEPENDS)

add_library(com_lib STATIC ${COM_Proxy_SRC_FILES} ${COM_Skeleton_SRC_FILES} ${COM_NetworkBinding_SRC_FILES} ${COM_SomeIp_SRC_FILES} ${COM_serviceDiscovery_SRC_FILES} ${COM_someip_TLS_FILES} ${COM_GUI_Report_SRC_FILES})
target_link_libraries(com_lib Boost::filesystem Boost::program_options Boost::serialization jsoncpp_lib ssl crypto boost_thread pthread sim_sock_lib)


target_include_directories(
        com_lib PUBLIC  ${com_source_dir}/language_binding/proxy/include  
        ${com_source_dir}/language_binding/skeleton/include 
        ${com_source_dir}/network_binding
        ${com_source_dir}/network_binding/SomeIp
        ${com_source_dir}/network_binding/SomeIp/servicediscovery/lib/include
        ${com_source_dir}/include
        ${com_source_dir}/include 
        ${CMAKE_SOURCE_DIR}/utility/serialization
        ${com_source_dir}/network_binding/SomeIp/TLS
        ) 
        
################################SomeIP_SD##########################

set(someIp_SD_install_dir "${CMAKE_SOURCE_DIR}/executables/someip_sd/0.1/bin")
set(COM_SERVICE_DiSCOVERY_MAIN_SRC_FILES "${com_source_dir}/network_binding/SomeIp/servicediscovery/src")

add_executable(someip_sd ${COM_serviceDiscovery_SRC_FILES} ${COM_SERVICE_DiSCOVERY_MAIN_SRC_FILES}/sd-main.cpp)
target_link_libraries(someip_sd com_lib execution_lib)
target_include_directories(someip_sd  PUBLIC ${com_source_dir}/network_binding/SomeIp/servicediscovery/lib/include ) 
install(TARGETS someip_sd RUNTIME DESTINATION ${someIp_SD_install_dir})

###############################UCM#############################

set(UCM_FILES "${CMAKE_SOURCE_DIR}/ara/ucm/")
set(ucm_install_dir "${CMAKE_SOURCE_DIR}/executables/ucm/0.1/bin")

file(GLOB_RECURSE UCM_SRC_FILES ${UCM_FILES}/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_executable(ucm ${UCM_SRC_FILES}) 

target_link_libraries(ucm execution_lib Boost::filesystem Boost::program_options Boost::serialization com_lib iam_lib)
target_include_directories( ucm PUBLIC ${UCM_FILES}/includes )
install(TARGETS ucm RUNTIME DESTINATION ${ucm_install_dir})

###############################SM###############################

set(source_excutables_sm_dir "${CMAKE_SOURCE_DIR}/executables/sm/0.1/bin")
set(source_excutables_sm_dir_1 "${CMAKE_SOURCE_DIR}/ara/sm")
set(source_simulation_dir "${CMAKE_SOURCE_DIR}/utility/simulation/src")
add_executable(sm ${source_excutables_sm_dir_1}/main.cpp ${source_excutables_sm_dir_1}/SM.cpp ${source_excutables_sm_dir_1}/sm_logger.cpp ${source_simulation_dir}/simulation.cpp) 
# To Return Original Remove Nexe line & uncomment the previous 2 lines
# add_executable(sm ${source_excutables_sm_dir_1}/main.cpp ${source_excutables_sm_dir_1}/SM.cpp) 
target_link_libraries( sm execution_lib jsoncpp_lib com_lib iam_lib)
install(TARGETS sm RUNTIME DESTINATION ${source_excutables_sm_dir})



# ################################CRYPTO_LIB#################################

 set(crypto_source_dir "${CMAKE_SOURCE_DIR}/ara/crypto/cryp")
 file(GLOB_RECURSE CRYPTO_LIB_SRC_FILES ${crypto_source_dir}/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
 add_library(crypto_lib STATIC ${CRYPTO_LIB_SRC_FILES} )
 target_link_libraries(crypto_lib cryptopp)
 target_include_directories(crypto_lib PUBLIC  ${crypto_source_dir}/include)
 add_executable(crypto_test ${CRYPTO_LIB_SRC_FILES})
 target_link_libraries(crypto_test crypto_lib)

######################### ota ##########################

set(ota_source_dir "${CMAKE_SOURCE_DIR}/ara/ota/client")
set(ota_install_dir "${CMAKE_SOURCE_DIR}/executables/ota/0.1/bin")
add_executable(ota ${ota_source_dir}/main.cpp ) 
target_link_libraries(ota com_lib iam_lib execution_lib cpr crypto_lib)
install(TARGETS ota RUNTIME DESTINATION ${ota_install_dir})

