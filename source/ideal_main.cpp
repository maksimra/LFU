#include "../include/ideal_cache.hpp"
#include <iostream>
#include <cassert>
#include "../include/get_page.hpp"

int main ()
{
    size_t cache_capacity = 0;
    std::cin >> cache_capacity;

    size_t num_elements = 0;
    std::cin >> num_elements;

    IdealCache<PageInfo> cache (cache_capacity);

    for (size_t element_num = 0;
         element_num < num_elements;
         element_num++)
    {
        int key = 0;
        std::cin >> key;

        assert (std::cin.good ());

        cache.key_list.push_back (key);
        cache.sequency_map[key].push_back (element_num);
    }

    size_t num_hits = 0;

    for (size_t element_num = 0;
         element_num < num_elements;
         element_num++)
    {
        if (cache.lookup_update (slow_get_page))
            num_hits += 1;
    }

    std::cout << num_hits << "\n";
}
