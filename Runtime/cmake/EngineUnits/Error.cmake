set(ERROR_PROJECT_NAME RNGOEngine_ERROR)
add_library(${ERROR_PROJECT_NAME} STATIC
        include/Errors/EngineError.h
)
target_include_directories(${ERROR_PROJECT_NAME} PUBLIC include)
target_link_libraries(${ERROR_PROJECT_NAME} PUBLIC
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
)
