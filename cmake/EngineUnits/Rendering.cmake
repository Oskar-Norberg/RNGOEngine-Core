set(RENDERING_PROJECT_NAME RNGOEngine_Rendering)
add_library(${RENDERING_PROJECT_NAME} STATIC
        include/Renderer/IRenderer.h
        
        include/Renderer/DrawQueue.h
        include/Renderer/RenderID.h
        include/Renderer/Handles/MaterialHandle.h
        include/Renderer/Uniforms.h
        
        include/Renderer/Null/NullRenderer.h
        
        include/Renderer/GLFW/GLFWRenderer.h
        src/Renderer/GLFW/GLFWRenderer.cpp
)
target_include_directories(${RENDERING_PROJECT_NAME} PUBLIC include)
target_link_libraries(${RENDERING_PROJECT_NAME} PUBLIC glm)
target_link_libraries(${RENDERING_PROJECT_NAME} PRIVATE glad glfw)
target_link_libraries(${RENDERING_PROJECT_NAME} PUBLIC ${PROFILING_PROJECT_NAME} ${UTILITIES_PROJECT_NAME})
