set(COMPONENT_PROJECT_NAME RNGOEngine_Components)
add_library(${COMPONENT_PROJECT_NAME} INTERFACE
        include/Components/Components.h
        include/Components/ComponentSerializers.h
        include/Components/ComponentDeserializers.h
)
target_include_directories(${COMPONENT_PROJECT_NAME} INTERFACE include)
target_link_libraries(${COMPONENT_PROJECT_NAME} INTERFACE yaml-cpp)
target_link_libraries(${COMPONENT_PROJECT_NAME} INTERFACE
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${ASSET_MANAGER_PROJECT_NAME}
        ${MATH_PROJECT_NAME}
)
