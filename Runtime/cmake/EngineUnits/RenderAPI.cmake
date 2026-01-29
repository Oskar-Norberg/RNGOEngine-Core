set(RENDER_API_PROJECT_NAME RNGOEngine_RenderAPI)
add_library(${RENDER_API_PROJECT_NAME} STATIC
        include/Renderer/DrawQueue.h

        include/Renderer/API/RenderAPI.h
        src/Renderer/API/RenderAPI.cpp

        include/Renderer/API/RenderPass/RenderContext.h
        include/Renderer/API/RenderPass/RenderPass.h

        include/Renderer/API/RenderPass/RenderPassResources.h
        src/Renderer/API/RenderPass/RenderPassResources.cpp

        # Systems (TODO: Consider moving to separate cmake unit)
        include/Systems/Core/Rendering/BeginFrameSystem.h
        include/Systems/Core/Rendering/RenderSystem.h
        src/Systems/Core/Rendering/RenderSystem.cpp
        include/Systems/Core/Rendering/EndFrameSystem.h

        # Debug Systems (TODO: Consider moving to separate cmake unit)
        include/Systems/Core/Rendering/Debug/RenderDebugCollidersSystem.h
        src/Systems/Core/Rendering/Debug/RenderDebugCollidersSystem.cpp

        include/Systems/Core/Rendering/Debug/RenderDebugCollisionsSystem.h
        src/Systems/Core/Rendering/Debug/RenderDebugCollisionsSystem.cpp

        # Passes (TODO: Consider moving to separate cmake unit)
        include/Renderer/API/Passes/ForwardPass.h
        src/Renderer/API/Passes/ForwardPass.cpp

        include/Renderer/API/Passes/ForwardScreenPass.h
        src/Renderer/API/Passes/ForwardScreenPass.cpp

        include/Renderer/API/Passes/DirectionalShadowMapPass.h
        src/Renderer/API/Passes/DirectionalShadowMapPass.cpp

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
        ${LOGGING_PROJECT_NAME}
)
