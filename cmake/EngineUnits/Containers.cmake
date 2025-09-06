set(CONTAINERS_PROJECT_NAME RNGOEngine_Containers)
add_library(${CONTAINERS_PROJECT_NAME} INTERFACE
        include/Utilities/Containers/Graphs/DirectedGraph.h
        include/Utilities/Containers/QuadTree/QuadTree.h
        include/Utilities/Containers/QuadTree/QuadTree.tpp
)
target_include_directories(${CONTAINERS_PROJECT_NAME} INTERFACE include)