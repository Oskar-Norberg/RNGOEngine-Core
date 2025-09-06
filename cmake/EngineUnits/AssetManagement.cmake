set(ASSET_MANAGER_PROJECT_NAME RNGOEngine_AssetManager)
add_library(${ASSET_MANAGER_PROJECT_NAME} STATIC
        include/AssetManager/AssetManager.h
        src/AssetManager/AssetManager.cpp
        include/Shader/ShaderPreprocessor.h
        src/Shader/ShaderPreprocessor.cpp
        include/AssetManager/AssetFileFetcher.h
        src/AssetManager/AssetFileFetcher.cpp
        include/AssetManager/AssetLoaders/TextureLoader.h
        src/AssetManager/AssetLoaders/TextureLoader.cpp
        include/AssetManager/AssetLoaders/ShaderLoader.h
        src/AssetManager/AssetLoaders/ShaderLoader.cpp
        include/AssetManager/AssetLoaders/MaterialLoader.h
        src/AssetManager/AssetLoaders/MaterialLoader.cpp
)
target_include_directories(${ASSET_MANAGER_PROJECT_NAME} PUBLIC include)
target_link_libraries(${ASSET_MANAGER_PROJECT_NAME} PUBLIC ${RENDERING_PROJECT_NAME} assimp stb_image)

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