set(LAYER_PROJECT_NAME RNGOEngine_Layer)
add_library(${LAYER_PROJECT_NAME} STATIC
        include/Layer/Layer.h
        src/Layer/Layer.cpp
        
        include/Layer/LayerStack.h
        src/Layer/LayerStack.cpp
)
target_include_directories(${LAYER_PROJECT_NAME} PUBLIC include)
target_link_libraries(${LAYER_PROJECT_NAME} PUBLIC ${PROFILING_PROJECT_NAME} ${UTILITIES_PROJECT_NAME})
