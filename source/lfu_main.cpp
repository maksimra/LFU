#include <iostream>
#include <cassert>
#include "../include/lfu_cache.hpp"
#include "../include/get_page.hpp"

int main ()
{
    size_t cache_capacity = 0;
    std::cin >> cache_capacity;

    size_t num_elements = 0;
    std::cin >> num_elements;

    LFUCache<PageInfo> cache (cache_capacity);
    size_t num_hits = 0;

    for (int element_num = 0;
         element_num < num_elements;
         element_num++)
    {
        int key;
        std::cin >> key;

        assert (std::cin.good ());

        if (cache.lookup_update (key, slow_get_page))
            num_hits += 1;
    }

    std::cout << num_hits << "\n";
}
