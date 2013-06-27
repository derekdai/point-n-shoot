cmake_minimum_required(VERSION 2.8)

project(point-n-shoot)

find_package(PkgConfig)

pkg_check_modules(GTK REQUIRED gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})

link_directories(${GTK_LIBRARY_DIRS})

add_definitions(-Wall)

set(SRC_PNS
    main
    item
    arrow
    point-n-shoot)

set(OBJ_PNS
    point-n-shoot)

add_executable(${OBJ_PNS} ${SRC_PNS})
target_link_libraries(${OBJ_PNS} ${GTK_LIBRARIES}) 