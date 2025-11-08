set(RENDER_API_PROJECT_NAME RNGOEngine_RenderAPI)
add_library(${RENDER_API_PROJECT_NAME} STATIC
        include/Renderer/DrawQueue.h
        
        include/Renderer/API/RenderAPI.h
        src/Renderer/API/RenderAPI.cpp

        include/Renderer/API/RenderPass/RenderContext.h
        include/Renderer/API/RenderPass/RenderPass.h

        include/Renderer/API/RenderPass/RenderPassResources.h 
        src/Renderer/API/RenderPass/RenderPassResources.cpp

        include/Systems/Core/RenderSystem.h
        src/Systems/Core/RenderSystem.cpp
)
target_include_directories(${RENDER_API_PROJECT_NAME} PUBLIC include)
target_link_libraries(${RENDER_API_PROJECT_NAME} PUBLIC
        ${SCENE_PROJECT_NAME}
        ${SYSTEM_PROJECT_NAME}
        ${RENDERING_PROJECT_NAME}
        ${RESOURCE_MANAGEMENT_PROJECT_NAME}
        ${ASSET_MANAGER_PROJECT_NAME}
)
target_link_libraries(${RENDER_API_PROJECT_NAME} PUBLIC
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${DATA_PROJECT_NAME}
)
