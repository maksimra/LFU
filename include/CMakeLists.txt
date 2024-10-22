add_library(lfu_cache INTERFACE)
target_include_directories (lfu_cache INTERFACE $(CMAKE_CURRENT_SOURCE_DIR))

add_library(ideal_cache INTERFACE)
target_include_directories (ideal_cache INTERFACE $(CMAKE_CURRENT_SOURCE_DIR))
