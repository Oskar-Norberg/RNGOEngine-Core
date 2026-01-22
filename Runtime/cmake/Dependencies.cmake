if (RNGOENGINE_TRACY_ENABLE)
    add_subdirectory(./vendor/tracy)
endif ()

add_subdirectory(./vendor/glfw)
add_subdirectory(./vendor/entt)
add_subdirectory(./vendor/glm)
add_subdirectory(./vendor/glad)
add_subdirectory(./vendor/stb_image)
add_subdirectory(./vendor/yaml-cpp)
add_subdirectory(./vendor/spdlog)
add_subdirectory(./vendor/lua)
add_subdirectory(./vendor/sol2)
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
