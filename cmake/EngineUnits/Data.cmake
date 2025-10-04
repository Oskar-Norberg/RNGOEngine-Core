set(DATA_PROJECT_NAME RNGOEngine_Data)
add_library(${DATA_PROJECT_NAME} INTERFACE
        include/Data/KeyCodes.h
        include/Data/MouseCodes.h
        include/Data/MeshData.h
        include/Data/MouseModes.h
)
target_include_directories(${DATA_PROJECT_NAME} INTERFACE include)
target_link_libraries(${DATA_PROJECT_NAME} INTERFACE glm)
