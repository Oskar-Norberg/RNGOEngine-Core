CPMAddPackage(
        NAME glfw
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.4
        OPTIONS "GLFW_BUILD_DOCS OFF" "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_EXAMPLES OFF" "BUILD_SHARED_LIBS OFF"
)

if (RNGOENGINE_TRACY_ENABLE)
    CPMAddPackage("gh:wolfpld/tracy@0.12.2")
endif ()
CPMAddPackage("gh:skypjack/entt@3.15.0")

add_subdirectory(./vendor/glm)
add_subdirectory(./vendor/glad)
add_subdirectory(./vendor/stb_image)
add_subdirectory(./vendor/yaml-cpp)
add_subdirectory(./vendor/spdlog)
add_subdirectory(./vendor/magic_enum)

# Assimp Build Properties
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

# Build only required importers.
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

add_subdirectory(./vendor/assimp)
