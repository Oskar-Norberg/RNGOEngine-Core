set(ASSET_MANAGER_PROJECT_NAME RNGOEngine_AssetManager)
add_library(${ASSET_MANAGER_PROJECT_NAME} STATIC
        # Asset Database
        include/Assets/Asset.h
        include/Assets/AssetMetadataTypes.h

        include/Assets/AssetTypes/TextureAsset.h
        include/Assets/AssetTypes/ModelAsset.h
        include/Assets/AssetTypes/ShaderAsset.h
        include/Assets/AssetTypes/MaterialAsset.h

        include/Assets/AssetMetadataStorage.h
        
        include/Assets/AssetDatabase/AssetDatabase.h
        include/Assets/AssetDatabase/AssetDatabase.tpp
        src/Assets/AssetDatabase/AssetDatabase.cpp

        include/Assets/AssetManager/AssetManager.h
        src/Assets/AssetManager/AssetManager.cpp
        
        include/Shader/ShaderPreprocessor.h
        src/Shader/ShaderPreprocessor.cpp
        
        include/Assets/AssetFetcher/AssetFetcher.h
        src/Assets/AssetFetcher/AssetFetcher.cpp
        
        include/Assets/AssetLoaders/TextureLoader.h
        src/Assets/AssetLoaders/TextureLoader.cpp
        
        include/Assets/AssetLoaders/ShaderLoader.h
        src/Assets/AssetLoaders/ShaderLoader.cpp
        
        include/Assets/AssetLoaders/ModelLoader.h
        src/Assets/AssetLoaders/ModelLoader.cpp
        
        include/Assets/AssetManager/Managers/ModelManager.h
        src/Assets/AssetManager/Managers/ModelManager.cpp

        include/Assets/AssetManager/Managers/TextureManager.h
        src/Assets/AssetManager/Managers/TextureManager.cpp

        include/Assets/AssetManager/Managers/Material/Material.h
        include/Assets/AssetManager/Managers/MaterialManager.h
        src/Assets/AssetManager/Managers/MaterialManager.cpp

        include/Assets/AssetManager/Managers/ShaderManager.h
        src/Assets/AssetManager/Managers/ShaderManager.cpp
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