set(CONTAINERS_PROJECT_NAME RNGOEngine_Containers)
add_library(${CONTAINERS_PROJECT_NAME} INTERFACE
        include/Utilities/Containers/Graphs/DirectedGraph.h
        
        include/Utilities/Containers/QuadTree/QuadTree.h
        include/Utilities/Containers/QuadTree/QuadTree.tpp

        include/Utilities/Containers/FreeListVector/FreeListVector.h
        
        include/Utilities/Containers/GenerationalVector/GenerationalVector.h
        include/Utilities/Containers/GenerationalVector/GenerationalVector.tpp
        
        include/Utilities/Containers/GenerationalVector/MarkedIterator.tpp
        include/Utilities/Containers/GenerationalVector/UnmarkedIterator.tpp
        
        include/Utilities/Containers/TSQueue/TSQueue.h
)
target_include_directories(${CONTAINERS_PROJECT_NAME} INTERFACE include)
target_link_libraries(${CONTAINERS_PROJECT_NAME} INTERFACE ${MATH_PROJECT_NAME} 
        ${PROFILING_PROJECT_NAME} 
        ${UTILITIES_PROJECT_NAME} 
        ${CONCEPTS_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
)
