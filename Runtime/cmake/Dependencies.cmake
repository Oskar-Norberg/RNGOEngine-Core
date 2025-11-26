if (RNGOENGINE_TRACY_ENABLE)
    CPMAddPackage("gh:wolfpld/tracy@0.12.2")
endif ()
CPMAddPackage("gh:skypjack/entt@3.15.0")

# GLFW
## GLFW Build Properties
add_subdirectory(./vendor/glfw)

add_subdirectory(./vendor/glm)
add_subdirectory(./vendor/glad)
add_subdirectory(./vendor/stb_image)
add_subdirectory(./vendor/yaml-cpp)
add_subdirectory(./vendor/spdlog)
add_subdirectory(./vendor/magic_enum)

# Assimp
## Assimp Build Properties
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

### Build only required importers.
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

add_subdirectory(./vendor/assimp)
