set(UTILITIES_PROJECT_NAME RNGOEngine_Utilities)
add_library(${UTILITIES_PROJECT_NAME} INTERFACE
        include/Utilities/IO/SimpleFileReader/SimpleFileReader.h
        include/Benchmarking/STDOUTStopwatch.h
        include/Profiling/Profiling.h
        include/Utilities/AssetCache/AssetCache.h
        include/Utilities/AssetCache/AssetCache.tpp
)
target_include_directories(${UTILITIES_PROJECT_NAME} INTERFACE include)