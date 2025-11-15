set(PROFILING_PROJECT_NAME RNGOEngine_Profiling)
add_library(${PROFILING_PROJECT_NAME} INTERFACE
        include/Profiling/Profiling.h
)
target_include_directories(${PROFILING_PROJECT_NAME} INTERFACE include)
if (RNGOENGINE_TRACY_ENABLE)
    target_link_libraries(${PROFILING_PROJECT_NAME} INTERFACE Tracy::TracyClient)
endif ()