set(SCENE_PROJECT_NAME RNGOEngine_Scene)
add_library(${SCENE_PROJECT_NAME} STATIC
        include/World/World.h
        src/World/World.cpp
        include/Scene/Scene.h
        include/Scene/SceneManager/SceneManager.h
        include/Scene/SceneManager/SceneManager.tpp
        src/Scene/SceneManager/SceneManager.cpp
)
target_include_directories(${SCENE_PROJECT_NAME} PUBLIC include)
target_link_libraries(${SCENE_PROJECT_NAME} PUBLIC EnTT)
target_link_libraries(${SCENE_PROJECT_NAME} PUBLIC ${PROFILING_PROJECT_NAME} ${CONCEPTS_PROJECT_NAME})
