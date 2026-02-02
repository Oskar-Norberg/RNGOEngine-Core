set(CONCEPTS_PROJECT_NAME RNGOEngine_Concepts)
add_library(${CONCEPTS_PROJECT_NAME} INTERFACE
        include/Utilities/Concepts/Concepts.h
)
target_include_directories(${CONCEPTS_PROJECT_NAME} INTERFACE include)
target_link_libraries(${CONCEPTS_PROJECT_NAME} INTERFACE ${PROFILING_PROJECT_NAME})
