set(MATH_PROJECT_NAME RNGOEngine_Math)
add_library(${MATH_PROJECT_NAME} INTERFACE
        include/Math/2D/Point2D.h
        include/Math/2D/AABB2D.h

        include/Math/3D/Point3D.h
        include/Math/3D/AABB3D.h

)
target_include_directories(${MATH_PROJECT_NAME} INTERFACE include)
target_link_libraries(${MATH_PROJECT_NAME} INTERFACE glm)
target_link_libraries(${MATH_PROJECT_NAME} INTERFACE
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
        ${CONCEPTS_PROJECT_NAME}
)
