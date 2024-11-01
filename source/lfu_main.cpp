#include <iostream>
#include <cassert>
#include "../include/lfu_cache.hpp"
#include "../include/get_page.hpp"
#include "../include/getting_value.hpp"

int main ()
{
    int cache_capacity = 0;
    int num_elements   = 0;

    try
    {
        get_positive_val_from_istream (&cache_capacity, std::cin);
        get_positive_val_from_istream (&num_elements,   std::cin);
    }
    catch (const char* error_message)
    {
        std::cerr << error_message;
        return 0;
    }

    LFUCache<PageInfo> cache (cache_capacity);
    size_t num_hits = 0;
    int key = 0;

    for (int element_num = 0;
         element_num < num_elements;
         element_num++)
    {
        try
        {
            get_smth_from_istream (&key, std::cin);
        }
        catch (const char* error_message)
        {
            std::cerr << error_message;
            return 0;
        }

        if (cache.lookup_update (key, slow_get_page))
            num_hits += 1;
    }

    std::cout << num_hits << "\n";
}
