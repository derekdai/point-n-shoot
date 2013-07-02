cmake_minimum_required(VERSION 2.8)

project(point-n-shoot)

set(MUSIC_DIR ${CMAKE_SOURCE_DIR}/music/)

find_package(PkgConfig)

pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

include_directories(${GTK_INCLUDE_DIRS}
                    ${GST_INCLUDE_DIRS}
                    ${CMAKE_BINARY_DIR}
                    ${CMAKE_SOURCE_DIR}/src)

link_directories(${GTK_LIBRARY_DIRS} ${GST_LIBRARY_DIRS})

add_definitions(-Wall)

set(SRC_PNS
    src/main
    src/base
    src/item
    src/arrow
    src/dot
    src/scene
    src/joystick
    src/platform
    src/point-n-shoot)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    list(APPEND SRC_PNS
         src/linux/linuxplat
         src/linux/linuxjs)
    set(PLATFORM_TYPE linux_plat_class)
    set(PLATFORM_HEADER \"linux/linuxplat.h\")
endif()

set(OBJ_PNS
    point-n-shoot)

add_executable(${OBJ_PNS} ${SRC_PNS})
target_link_libraries(${OBJ_PNS} ${GTK_LIBRARIES} ${GST_LIBRARIES} m)

set(SRC_TEST_JOYSTICK tests/test-joystick)
set(OBJ_TEST_JOYSTICK tests-joystick)
add_executable(${OBJ_TEST_JOYSTICK} ${SRC_TEST_JOYSTICK})
target_link_libraries(${OBJ_TEST_JOYSTICK} ${GTK_LIBRARIES})

configure_file(config.h.cmake ${CMAKE_BINARY_DIR}/config.h @ONLY)
