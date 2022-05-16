cmake_minimum_required(VERSION 3.5)

project(adaptive_autosar_simulation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(jsoncpp_dir "${CMAKE_SOURCE_DIR}/jsoncpp")
add_library(jsoncpp_lib STATIC ${jsoncpp_dir}/src/jsoncpp.cpp)
target_include_directories(jsoncpp_lib PUBLIC ${jsoncpp_dir}/jsoncpp/header)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        exec.cpp
        exec.h
        sm.h
        sm.cpp
        ucm.cpp
        ucm.h
        ota.cpp
        ota.h
        iam.cpp
        iam.h
        simulation.hpp
        simulation.cpp
        sd.h
        sd.cpp
        com.h
        com.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(adaptive_autosar_simulation
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET adaptive_autosar_simulation APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(adaptive_autosar_simulation SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(adaptive_autosar_simulation
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(adaptive_autosar_simulation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets jsoncpp_lib)

set_target_properties(adaptive_autosar_simulation PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(adaptive_autosar_simulation)
endif()
