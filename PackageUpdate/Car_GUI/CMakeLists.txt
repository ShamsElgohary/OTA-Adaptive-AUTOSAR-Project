cmake_minimum_required(VERSION 3.5)

project(Car_GUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Boost 1.50 COMPONENTS system filesystem program_options serialization REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        commands.h
        ../../utility/general.hpp
        ../../SecureOTA_System_Configurations/GeneratedFiles/CarControlSkeleton.hpp
        ../../SecureOTA_System_Configurations/GeneratedFiles/types.hpp
        ../../utility/serialization/serialization_simple.hpp
        CarControlImpl.hpp
        CarControlImpl.cpp
)
###############################EM_LIB###############################

set(jsoncpp_dir "${CMAKE_SOURCE_DIR}/../../utility/jsoncpp")
add_library(jsoncpp_lib STATIC ${jsoncpp_dir}/src/jsoncpp.cpp)
target_include_directories(jsoncpp_lib PUBLIC ${jsoncpp_dir}/jsoncpp/header)


###############################EM_LIB###############################
set(exec_source_dir "${CMAKE_SOURCE_DIR}/../../ara/exec")
add_library(execution_lib STATIC ${exec_source_dir}/src/execution_client.cpp ${exec_source_dir}/src/state_client.cpp ${exec_source_dir}/src/function_group_state.cpp ${exec_source_dir}/src/find_process_client.cpp)
target_include_directories(execution_lib PUBLIC  ${exec_source_dir}/include)


###############################COM_LIB###############################

set(com_source_dir "${CMAKE_SOURCE_DIR}/../../ara/com")
file(GLOB_RECURSE COM_Proxy_SRC_FILES ${com_source_dir}/language_binding/proxy/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_Skeleton_SRC_FILES ${com_source_dir}/language_binding/skeleton/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_NetworkBinding_SRC_FILES ${com_source_dir}/network_binding/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_SomeIp_SRC_FILES ${com_source_dir}/network_binding/SomeIp/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_serviceDiscovery_SRC_FILES ${com_source_dir}/network_binding/SomeIp/servicediscovery/lib/src/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_someip_TLS_FILES ${com_source_dir}/network_binding/SomeIp/TLS/*.cpp  CMAKE_CONFIGURE_DEPENDS)
file(GLOB_RECURSE COM_GUI_Report_SRC_FILES ${com_source_dir}/*.cpp  CMAKE_CONFIGURE_DEPENDS)

add_library(com_lib STATIC ${COM_Proxy_SRC_FILES} ${COM_Skeleton_SRC_FILES} ${COM_NetworkBinding_SRC_FILES} ${COM_SomeIp_SRC_FILES} ${COM_serviceDiscovery_SRC_FILES} ${COM_someip_TLS_FILES} ${COM_GUI_Report_SRC_FILES})
target_link_libraries(com_lib Boost::filesystem Boost::program_options Boost::serialization jsoncpp_lib ssl crypto boost_thread pthread )

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

#######################################################################
set(iam_source_dir "${CMAKE_SOURCE_DIR}/../../ara/iam")
file(GLOB_RECURSE IAM_LIB_SRC_FILES ${iam_source_dir}/lib/src/*.cpp CMAKE_CONFIGURE_DEPENDS)
add_library(iam_lib STATIC ${IAM_LIB_SRC_FILES} )
target_link_libraries(iam_lib  Boost::filesystem Boost::program_options Boost::serialization)

target_include_directories(iam_lib PUBLIC  ${iam_source_dir}/lib/include )
#######################################################################

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Car_GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Car_GUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Car_GUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Car_GUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Car_GUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets execution_lib com_lib jsoncpp_lib iam_lib)

set_target_properties(Car_GUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Car_GUI)
endif()
