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

SET(LIBNAME FFLIB)
SET(FFLIB_DIR libs)
message("FFLIB_DIR : ${FFLIB_DIR}")

set(AVCODEC  libavcodec)
add_library(${AVCODEC} SHARED IMPORTED)
set_target_properties(${AVCODEC}
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        ${FFLIB_DIR}/avcodec-61.dll)
target_include_directories(${AVCODEC} INTERFACE
        include/libavcodec
        )


    set(AVDEVICE  libavdevice)
    add_library(${AVDEVICE} SHARED IMPORTED)
    set_target_properties(${AVDEVICE}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/avdevice-61.dll)
    target_include_directories(${AVDEVICE} INTERFACE ffmpeginc/libavdevice)

    set(AVFILTER libavfilter)
    add_library(${AVFILTER} SHARED IMPORTED)
    set_target_properties(${AVFILTER}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/avfilter-10.dll)
    target_include_directories(${AVFILTER} INTERFACE ffmpeginc/libavfilter)

    set(AVFORMAT libavformat)
    add_library(${AVFORMAT} SHARED IMPORTED)
    set_target_properties(${AVFORMAT}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/avformat-61.dll)
    target_include_directories(${AVFORMAT} INTERFACE ffmpeginc/libavformat)

    set(AVUTIL libavutil)
    add_library(${AVUTIL} SHARED IMPORTED)
    set_target_properties(${AVUTIL}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/avutil-59.dll)
    target_include_directories(${AVUTIL} INTERFACE ffmpeginc/libavutil)

    set(SWRESAMPLE libswresample)
    add_library(${SWRESAMPLE} SHARED IMPORTED)
    set_target_properties(${SWRESAMPLE}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/swresample-5.dll)
    target_include_directories(${SWRESAMPLE} INTERFACE ffmpeginc/libswresample)

    set(SWSCALE libswscale)
    add_library(${SWSCALE} SHARED IMPORTED)
    set_target_properties(${SWSCALE}
            # Specifies the parameter you want to define.
            PROPERTIES IMPORTED_LOCATION
            # Provides the path to the library you want to import.
            ${FFLIB_DIR}/swscale-8.dll)
    target_include_directories(${SWSCALE} INTERFACE ffmpeginc/libswscale)
