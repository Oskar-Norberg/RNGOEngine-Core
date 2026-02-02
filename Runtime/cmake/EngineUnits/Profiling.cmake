set(PROFILING_PROJECT_NAME RNGOEngine_Profiling)
add_library(${PROFILING_PROJECT_NAME} STATIC
        include/Utilities/Profiling/Profiling.h
        src/Utilities/Profiling/Profiling.cpp
)
target_include_directories(${PROFILING_PROJECT_NAME} INTERFACE include)
if (RNGOENGINE_TRACY_ENABLE)
    target_link_libraries(${PROFILING_PROJECT_NAME} PUBLIC Tracy::TracyClient)
    target_compile_definitions(${PROFILING_PROJECT_NAME} PUBLIC RNGOENGINE_TRACY_ENABLE)
    target_compile_definitions(${PROFILING_PROJECT_NAME} PUBLIC TRACY_ENABLE)
endif ()
