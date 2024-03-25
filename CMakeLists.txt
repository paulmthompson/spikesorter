cmake_minimum_required(VERSION 3.24)

project(spikesorter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#[[
Best practice for looking for cmake modules cmake folder in root of the project
Currently, this project does not have any cmake modules
From Professional CMake Chapter 11 Modules
]]
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#[[
Manifest mode from VCPKG will make all dependancies specified in vcpkg.json be associated and installed
with this particular project
]]
set(VCPKG_MANIFEST_MODE ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS UiTools REQUIRED)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGL)

find_package(nlohmann_json CONFIG REQUIRED)

set(PROJECT_SOURCES
        spikesorter/main.cpp

        spikesorter/mainwindow/mainwindow.cpp
        spikesorter/mainwindow/mainwindow.hpp
        spikesorter/mainwindow/mainwindow.ui
        spikesorter.qrc
        my_stylesheet.qss

        spikesorter/AnalogRenderArea/analogrenderarea.hpp
        spikesorter/AnalogRenderArea/analogrenderarea.cpp
        spikesorter/AnalogRenderArea/AnalogRenderXAxisProps.hpp
        spikesorter/AnalogRenderArea/AnalogRenderYAxisProps.hpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(spikesorter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET spikesorter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(spikesorter SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(spikesorter
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(spikesorter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(spikesorter PRIVATE Qt${QT_VERSION_MAJOR}::Multimedia)
target_link_libraries(spikesorter PRIVATE Qt${QT_VERSION_MAJOR}::MultimediaWidgets)
target_link_libraries(spikesorter PRIVATE Qt${QT_VERSION_MAJOR}::OpenGL)
target_link_libraries(spikesorter PRIVATE Qt${QT_VERSION_MAJOR}::UiTools)

target_link_libraries(spikesorter PRIVATE nlohmann_json::nlohmann_json)


target_include_directories(spikesorter PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/spikesorter/mainwindow>"

    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set_target_properties(spikesorter PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(spikesorter)
endif()
