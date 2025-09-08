set(DO_TESTS off CACHE BOOL "Enable tests" FORCE)

if (DO_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()