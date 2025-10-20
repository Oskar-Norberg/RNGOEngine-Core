# TODO: QuadTree 100% depends on Math, add it as dependency.
set(CONTAINERS_PROJECT_NAME RNGOEngine_Containers)
add_library(${CONTAINERS_PROJECT_NAME} INTERFACE
        include/Utilities/Containers/Graphs/DirectedGraph.h
        
        include/Utilities/Containers/QuadTree/QuadTree.h
        include/Utilities/Containers/QuadTree/QuadTree.tpp
        
        include/Utilities/Containers/GenerationalVector/GenerationalVector.h
        include/Utilities/Containers/GenerationalVector/GenerationalVector.tpp
        include/Utilities/Containers/GenerationalVector/MarkedIterator.tpp
        include/Utilities/Containers/GenerationalVector/UnmarkedIterator.tpp
)
target_include_directories(${CONTAINERS_PROJECT_NAME} INTERFACE include)
target_link_libraries(${CONTAINERS_PROJECT_NAME} INTERFACE ${PROFILING_PROJECT_NAME} ${UTILITIES_PROJECT_NAME})
