include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
FetchContent_MakeAvailable(googletest)

set(PROJECT_NAME RNGOEngine_Tests)

add_executable(${PROJECT_NAME}
        QuadTree/QuadTreeTests.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        RNGOEngine_Core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
