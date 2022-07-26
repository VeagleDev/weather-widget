cmake_minimum_required(VERSION 3.5)

project(WeathGet VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.hpp
        includes.hpp
        update.cpp
        tools.cpp
        ui.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/img/icon.rc")
    qt_add_executable(WeathGet
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${app_icon_resource_windows}
    )


else()
    if(ANDROID)
        add_library(WeathGet SHARED
            ${PROJECT_SOURCES}
        )
   else()
        add_executable(WeathGet
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(WeathGet PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(WeathGet PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WeathGet)
endif()


find_package(Qt6 REQUIRED COMPONENTS Network)
target_link_libraries(WeathGet PRIVATE Qt6::Network)

find_package(Qt6 REQUIRED COMPONENTS Sql)
target_link_libraries(WeathGet PRIVATE Qt6::Sql)
