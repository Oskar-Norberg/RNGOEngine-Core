include(FetchContent)

FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG v0.12.2
)
FetchContent_MakeAvailable(tracy)