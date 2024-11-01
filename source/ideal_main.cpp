#include "../include/ideal_cache.hpp"
#include <iostream>
#include <cassert>
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

        IdealCache<PageInfo> cache (cache_capacity);
        int key = 0;

        for (int element_num = 0;
             element_num < num_elements;
             element_num++)
        {
            get_smth_from_istream (&key, std::cin);
            cache.put_elem (element_num, key);
        }

        size_t num_hits = 0;

        for (int element_num = 0;
             element_num < num_elements;
             element_num++)
        {
            if (cache.lookup_update (slow_get_page))
                num_hits += 1;
        }
    
        std::cout << num_hits << "\n";
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << error.what () << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
