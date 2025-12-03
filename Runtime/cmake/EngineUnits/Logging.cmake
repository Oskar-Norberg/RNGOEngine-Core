set(LOGGING_PROJECT_NAME RNGOEngine_Logging)
add_library(${LOGGING_PROJECT_NAME} STATIC
        include/Logging/LogLevel.h

        include/Logging/Logger.h
        src/Logging/Logger.cpp

        include/Logging/VectorSink.h
)
target_include_directories(${LOGGING_PROJECT_NAME} PUBLIC include)
target_link_libraries(${LOGGING_PROJECT_NAME} PUBLIC
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        spdlog::spdlog
)
