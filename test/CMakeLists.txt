enable_testing()

add_executable(minefield_test ${PROJECT_SOURCE_DIR}/test/minefield.cpp)
target_link_libraries(minefield_test minefield)
add_test(minefield_test minefield_test)

add_executable(controller_test ${PROJECT_SOURCE_DIR}/test/controller.cpp)
target_link_libraries(controller_test controller)
target_link_libraries(controller_test minefield)
add_test(controller_test controller_test)

add_executable(iodevice_simulation_test ${PROJECT_SOURCE_DIR}/test/iodevice_simulation.cpp)
target_link_libraries(iodevice_simulation_test iodevice_simulation)
add_test(iodevice_simulation_test iodevice_simulation_test)

add_executable(display_test ${PROJECT_SOURCE_DIR}/test/display.cpp)
target_link_libraries(display_test display)
target_link_libraries(display_test controller)
target_link_libraries(display_test minefield)
target_link_libraries(display_test iodevice_simulation)
target_link_libraries(display_test ${CURSES_LIBRARIES})
add_test(display_test display_test)
