set(RENDER_API_PROJECT_NAME RNGOEngine_RenderAPI)
add_library(${RENDER_API_PROJECT_NAME} STATIC
        include/Renderer/API/RenderAPI.h
        src/Renderer/API/RenderAPI.cpp
)
target_include_directories(${RENDER_API_PROJECT_NAME} PUBLIC include)
target_link_libraries(${RENDER_API_PROJECT_NAME} PUBLIC ${RENDERING_PROJECT_NAME} ${RESOURCE_MANAGEMENT_PROJECT_NAME})
target_link_libraries(${RENDER_API_PROJECT_NAME} PUBLIC
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${DATA_PROJECT_NAME}
)
