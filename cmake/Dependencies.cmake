CPMAddPackage(
        NAME glfw
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.4
        OPTIONS "GLFW_BUILD_DOCS OFF" "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_EXAMPLES OFF" "BUILD_SHARED_LIBS OFF"
)

CPMAddPackage(
        NAME glm
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 1.0.1
)

CPMAddPackage(
        NAME assimp
        GITHUB_REPOSITORY assimp/assimp
        GIT_TAG v6.0.2
        OPTIONS "ASSIMP_BUILD_ASSIMP_TOOLS OFF"
        "ASSIMP_BUILD_TESTS OFF"
        "BUILD_SHARED_LIBS OFF"
)

if (RNGOENGINE_TRACY_ENABLE)
    CPMAddPackage("gh:wolfpld/tracy@0.12.2")
endif ()
CPMAddPackage("gh:skypjack/entt@3.15.0")

add_subdirectory(./vendor/glad)
add_subdirectory(./vendor/stb_image)
