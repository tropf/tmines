cmake_minimum_required (VERSION 2.6)

project (TerminalMines)

set (TerminalMines_VERSION_MAJOR 0)
set (TerminalMines_VERSION_MINOR 9)
set(CMAKE_CXX_STANDARD 14)

configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/src/config.h"
    )

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")

add_library(minefield src/minefield.cpp)
add_library(controller src/controller.cpp)
add_library(display src/display.cpp)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(tmines src/mines.cpp)

target_link_libraries(tmines display)
target_link_libraries(tmines controller)
target_link_libraries(tmines minefield)
target_link_libraries(tmines ${CURSES_LIBRARIES})

install(TARGETS tmines DESTINATION bin)
