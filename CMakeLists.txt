cmake_minimum_required(VERSION 2.8)

project(point-n-shoot)

set(MUSIC_DIR ${CMAKE_SOURCE_DIR}/music/)

find_package(PkgConfig)

pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

include_directories(${GTK_INCLUDE_DIRS} ${GST_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

link_directories(${GTK_LIBRARY_DIRS} ${GST_LIBRARY_DIRS})

add_definitions(-Wall)

configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h @ONLY)

set(SRC_PNS
    src/main
    src/base
    src/item
    src/arrow
    src/dot
    src/scene
    src/point-n-shoot)

set(OBJ_PNS
    point-n-shoot)

add_executable(${OBJ_PNS} ${SRC_PNS})
target_link_libraries(${OBJ_PNS} ${GTK_LIBRARIES} ${GST_LIBRARIES} m)
