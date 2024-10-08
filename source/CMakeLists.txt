add_executable(LFUCache
                lfu_main.cpp
                ../include/lfu_cache.hpp
                ../include/get_page.hpp
)

add_executable(IdealCache
                ideal_main.cpp
                ../include/ideal_cache.hpp
                ../include/get_page.hpp
)

target_link_libraries(LFUCache PRIVATE lfu_cache)
target_link_libraries(IdealCache PRIVATE ideal_cache)

target_include_directories(LFUCache PUBLIC ${PROJECT_SOURCE_DIR})
target_include_directories(IdealCache PUBLIC ${PROJECT_SOURCE_DIR})
