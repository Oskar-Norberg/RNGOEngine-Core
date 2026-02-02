set(JOBS_PROJECT_NAME RNGOEngine_Jobs)
add_library(${JOBS_PROJECT_NAME} STATIC
        include/Utilities/ThreadPool/ThreadPool.h
        include/Utilities/ThreadPool/ThreadPool.inl
        src/Utilities/ThreadPool.cpp
        include/Utilities/JobSystem/JobSystem.h
        include/Utilities/JobSystem/JobSystem.inl
        include/Utilities/JobSystem/Handle/JobHandle.h
        include/Utilities/JobSystem/Handle/JobHandle.inl
)
target_include_directories(${JOBS_PROJECT_NAME} PUBLIC include)
target_link_libraries(${JOBS_PROJECT_NAME} PUBLIC 
        ${PROFILING_PROJECT_NAME} 
        ${UTILITIES_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
)
