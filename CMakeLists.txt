cmake_minimum_required (VERSION 2.6)

project (TerminalMines)

set (TerminalMines_VERSION_MAJOR 0)
set (TerminalMines_VERSION_MINOR 9)
set(CMAKE_CXX_STANDARD 14)

option(build_tests "build tests alongside the project" OFF)
option(debug "build with gdb debugging symbols" OFF)
option(profiling "build with gprof profiling enabled" OFF)

if (profiling)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -no-pie")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -no-pie")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -no-pie")
endif()

if (debug)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g")
endif()

find_package(Git)
if(GIT_FOUND)
    execute_process(COMMAND ${GIT_EXECUTABLE} "rev-parse" "HEAD"
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                    OUTPUT_VARIABLE GIT_COMMIT_HASH
                    RESULT_VARIABLE GIT_COMMIT_HASH_RETURN_VAR
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)
    if (NOT "0" EQUAL "${GIT_COMMIT_HASH_RETURN_VAR}")
        set(GIT_COMMIT_HASH "didn't build from git repo")
    endif()
else()
    set(GIT_COMMIT_HASH "no git found during build")
endif()

message(STATUS "Building git commit '${GIT_COMMIT_HASH}'")

configure_file(
    "${PROJECT_SOURCE_DIR}/src/config.h.in"
    "${PROJECT_BINARY_DIR}/src/config.h"
    )

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src")

add_library(minefield src/minefield.cpp)
add_library(controller src/controller.cpp)
add_library(display src/display.cpp)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(tmines src/mines.cpp)

add_subdirectory(doc)

target_link_libraries(tmines display)
target_link_libraries(tmines controller)
target_link_libraries(tmines minefield)
target_link_libraries(tmines ${CURSES_LIBRARIES})

install(TARGETS tmines DESTINATION bin)

if (build_tests)
    enable_testing()
    include_directories(extern/doctest)

    add_executable(minefield_test test/minefield.cpp)
    target_link_libraries(minefield_test minefield)
    add_test(minefield_test minefield_test)

    add_executable(controller_test test/controller.cpp)
    target_link_libraries(controller_test controller)
    target_link_libraries(controller_test minefield)
    add_test(controller_test controller_test)
endif()
