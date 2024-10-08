add_library(lfu_cache
            lfu_cache.hpp
            get_page.hpp
)

add_library(ideal_cache
            ideal_cache.hpp
            get_page.hpp
)

set_target_properties(lfu_cache PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(ideal_cache PROPERTIES LINKER_LANGUAGE CXX)
