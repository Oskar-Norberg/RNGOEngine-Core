set(MATH_PROJECT_NAME RNGOEngine_Math)
add_library(${MATH_PROJECT_NAME} INTERFACE
        include/Math/Point.h
        include/Math/BoundingBox.h
)
target_include_directories(${MATH_PROJECT_NAME} INTERFACE include)
target_link_libraries(${MATH_PROJECT_NAME} INTERFACE glm)
target_link_libraries(${MATH_PROJECT_NAME} INTERFACE ${PROFILING_PROJECT_NAME} ${UTILITIES_PROJECT_NAME})
