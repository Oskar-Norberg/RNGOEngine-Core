set(UTILITIES_PROJECT_NAME RNGOEngine_Utilities)
add_library(${UTILITIES_PROJECT_NAME} STATIC
        include/Utilities/RNGOAsserts.h
        
        include/Utilities/IO/SimpleFileReader/SimpleFileReader.h
        include/Benchmarking/STDOUTStopwatch.h
        
        include/Utilities/AssetCache/AssetCache.h
        include/Utilities/AssetCache/AssetCache.tpp
        
        include/Utilities/UUID/UUID.h
        src/Utilities/UUID/UUID.cpp
        
        include/Utilities/Singleton/Singleton.h
)
target_include_directories(${UTILITIES_PROJECT_NAME} PUBLIC include)
target_link_libraries(${UTILITIES_PROJECT_NAME} PUBLIC ${PROFILING_PROJECT_NAME})
