set(ASSET_MANAGER_PROJECT_NAME RNGOEngine_AssetManager)
add_library(${ASSET_MANAGER_PROJECT_NAME} STATIC
        # Asset Database
        include/AssetHandling/AssetDatabase/AssetDatabase.h
        src/AssetHandling/AssetDatabase/AssetDatabase.cpp

        include/AssetHandling/AssetDatabase/Databases/ModelDatabase.h
        src/AssetHandling/AssetDatabase/Databases/ModelDatabase.cpp

        include/AssetHandling/AssetDatabase/Databases/TextureDatabase.h
        src/AssetHandling/AssetDatabase/Databases/TextureDatabase.cpp


        include/AssetHandling/AssetManager/AssetManager.h
        src/AssetHandling/AssetManager/AssetManager.cpp
        
        include/Shader/ShaderPreprocessor.h
        src/Shader/ShaderPreprocessor.cpp
        
        include/AssetHandling/AssetFetcher/AssetFetcher.h
        src/AssetHandling/AssetFetcher/AssetFetcher.cpp
        
        include/AssetHandling/AssetLoaders/TextureLoader.h
        src/AssetHandling/AssetLoaders/TextureLoader.cpp
        
        include/AssetHandling/AssetLoaders/ShaderLoader.h
        src/AssetHandling/AssetLoaders/ShaderLoader.cpp
        
        include/AssetHandling/AssetLoaders/ModelLoader.h
        src/AssetHandling/AssetLoaders/ModelLoader.cpp
        
        include/AssetHandling/AssetManager/Managers/ModelManager.h
        src/AssetHandling/AssetManager/Managers/ModelManager.cpp

        include/AssetHandling/AssetManager/Managers/TextureManager.h
        src/AssetHandling/AssetManager/Managers/TextureManager.cpp

        include/AssetHandling/AssetManager/Managers/Material/Material.h
        include/AssetHandling/AssetManager/Managers/MaterialManager.h
        src/AssetHandling/AssetManager/Managers/MaterialManager.cpp

        include/AssetHandling/AssetManager/Managers/ShaderManager.h
        src/AssetHandling/AssetManager/Managers/ShaderManager.cpp
)
target_include_directories(${ASSET_MANAGER_PROJECT_NAME} PUBLIC include)
target_link_libraries(${ASSET_MANAGER_PROJECT_NAME} PUBLIC ${RENDERING_PROJECT_NAME} ${RESOURCE_MANAGEMENT_PROJECT_NAME} assimp stb_image)
target_link_libraries(${ASSET_MANAGER_PROJECT_NAME} PUBLIC ${PROFILING_PROJECT_NAME} ${UTILITIES_PROJECT_NAME})


# Define engine root path for assets.
set(ENGINE_ASSETS_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")
set(ENGINE_SHADERS_DIR "${ENGINE_ASSETS_DIR}/shaders")
set(ENGINE_SHADER_INCLUDE_DIR "${ENGINE_SHADERS_DIR}/include")
set(ENGINE_MODELS_DIR "${ENGINE_ASSETS_DIR}/models")
set(ENGINE_TEXTURES_DIR "${ENGINE_ASSETS_DIR}/textures")

target_compile_definitions(${ASSET_MANAGER_PROJECT_NAME} PRIVATE
        ENGINE_ASSETS_DIR="${ENGINE_ASSETS_DIR}"
        ENGINE_SHADERS_DIR="${ENGINE_SHADERS_DIR}"
        ENGINE_SHADER_INCLUDE_DIR="${ENGINE_SHADER_INCLUDE_DIR}"
        ENGINE_MODELS_DIR="${ENGINE_MODELS_DIR}"
        ENGINE_TEXTURES_DIR="${ENGINE_TEXTURES_DIR}"
)