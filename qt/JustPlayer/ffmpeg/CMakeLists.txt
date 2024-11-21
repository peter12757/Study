cmake_minimum_required(VERSION 3.16)

project(JustPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES JustPlayer_zh_CN.ts)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    add_subdirectory(win64)
else()
    if(ANDROID)
        add_subdirectory(armv7)
    else()
        message("error here")
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
