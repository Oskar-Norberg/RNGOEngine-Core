set(RESOURCE_MANAGEMENT_PROJECT_NAME RNGOEngine_ResourceManagement)
add_library(${RESOURCE_MANAGEMENT_PROJECT_NAME} STATIC
        include/ResourceManager/ResourceCollection.h
        
        include/ResourceManager/ResourceManager.h
        src/ResourceManager/ResourceManager.cpp

        include/ResourceManager/ModelResourceManager/ModelResourceManager.h
        src/ResourceManager/ModelResourceManager/ModelResourceManager.cpp
        
        include/ResourceManager/ShaderResourceManager/ShaderResourceManager.h
        src/ResourceManager/ShaderResourceManager/ShaderResourceManager.cpp
        
        include/ResourceManager/TextureResourceManager/TextureResourceManager.h
        src/ResourceManager/TextureResourceManager/TextureResourceManager.cpp
        
        include/ResourceManager/ResourceTracker.h
        src/ResourceManager/ResourceTracker.cpp
)
target_include_directories(${RESOURCE_MANAGEMENT_PROJECT_NAME} PUBLIC include)
target_link_libraries(${RESOURCE_MANAGEMENT_PROJECT_NAME} PUBLIC ${RENDERING_PROJECT_NAME})
target_link_libraries(${RESOURCE_MANAGEMENT_PROJECT_NAME} PUBLIC
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${DATA_PROJECT_NAME}
)
