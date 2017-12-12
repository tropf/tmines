find_package(UnixCommands)

if (GZIP)
    execute_process(COMMAND ${GZIP} "-c" "${PROJECT_SOURCE_DIR}/man/tmines.6"
        OUTPUT_FILE "${PROJECT_BINARY_DIR}/man/tmines.6.gz")
    INSTALL(FILES ${PROJECT_BINARY_DIR}/man/tmines.6.gz DESTINATION share/man/man6)
else()
    INSTALL(FILES ${PROJECT_SOURCE_DIR}/man/tmines.6 DESTINATION share/man/man6)
endif()
