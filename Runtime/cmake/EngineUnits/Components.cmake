set(COMPONENT_PROJECT_NAME RNGOEngine_Components)
add_library(${COMPONENT_PROJECT_NAME} INTERFACE
        include/Components/Components.h
)
target_include_directories(${COMPONENT_PROJECT_NAME} INTERFACE include)
target_link_libraries(${COMPONENT_PROJECT_NAME} INTERFACE EnTT)
target_link_libraries(${COMPONENT_PROJECT_NAME} INTERFACE ${PROFILING_PROJECT_NAME} ${UTILITIES_PROJECT_NAME})
