# TODO: This might be worth including as a submodule instead of fetching.

include(FetchContent)

FetchContent_Declare(
        glfw-fetcher
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw-fetcher)