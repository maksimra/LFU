#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include "../include/lfu_cache.hpp"
#include "../include/ideal_cache.hpp"
#include "../include/get_page.hpp"
#include "../include/getting_value.hpp"

const size_t STANDART_CACHE_SIZE = 6;
const int    NUMBER_TESTS = 10;

size_t lfu_test (const std::string& filename)
{
    std::fstream test_file (filename);
    if (!test_file.is_open ())
    {
        throw std::runtime_error ("Failed to open file " + std::string (filename));
    }
    auto original_buffer = std::cin.rdbuf (test_file.rdbuf());

    int cache_capacity = 0, number_pages = 0;
    std::cin >> cache_capacity >> number_pages;

    LFUCache<PageInfo> cache (cache_capacity);
    size_t num_hits = 0;

    int key = 0;

    for (int page_number = 0;
         page_number < number_pages;
         page_number++)
    {
        try
        {
            get_smth_from_cin (&key);
        }
        catch (const char* error_message)
        {
            std::cerr << error_message;
            return 0;
        }

        if (cache.lookup_update (key, slow_get_page))
            num_hits += 1;
    }

    std::cin.rdbuf(original_buffer);
    return num_hits;
}

size_t ideal_test (const std::string& filename)
{
    std::fstream test_file (filename);
    if (!test_file.is_open ())
    {
        throw std::runtime_error ("Failed to open file " + std::string (filename));
    }
    auto original_buffer = std::cin.rdbuf (test_file.rdbuf());

    int cache_capacity = 0, number_pages = 0;
    std::cin >> cache_capacity >> number_pages;

    IdealCache<PageInfo> cache (cache_capacity);
    int key = 0;

    for (int element_num = 0;
         element_num < number_pages;
         element_num++)
    {
        try
        {
            get_smth_from_cin (&key);
        }
        catch (const char* error_message)
        {
            std::cerr << error_message;
            return 0;
        }

        cache.put_elem (element_num, key);
    }

    size_t num_hits = 0;

    for (int element_num = 0;
         element_num < number_pages;
         element_num++)
    {
        if (cache.lookup_update (slow_get_page))
            num_hits += 1;
    }

    std::cin.rdbuf(original_buffer);
    return num_hits;
}

size_t get_answer (int test_number, const char* filename)
{
    std::vector<size_t> answers;

    std::fstream answers_file (filename, std::ios::in);
    if (!answers_file.is_open ())
    {
        throw std::runtime_error ("Failed to open file " + std::string (filename));
    }

    size_t answer = 0;

    for (int i = 0; i < test_number; i++)
    {
        answers_file >> answer;
        answers.push_back(answer);
    }

    return answers[test_number - 1];
}

TEST (LfuUnitTest, CacheEviction)
{
    LFUCache<PageInfo> lfu_cache (2);
    int keys[4] = {10, 10, 20, 30};

    for (int element_num = 0; element_num < 4; ++element_num)
    {
        lfu_cache.lookup_update (keys[element_num], slow_get_page);
    }

    EXPECT_EQ (lfu_cache.element_exists (20), false);
}

TEST (LfuUnitTest, CacheMiss)
{
    LFUCache<PageInfo> lfu_cache (STANDART_CACHE_SIZE);
    int key = 1999;

    EXPECT_EQ (lfu_cache.element_exists (key), false);
}

TEST (IdealUnitTest, SetAndGet)
{
    IdealCache<PageInfo> ideal_cache (STANDART_CACHE_SIZE);
    int key = 2005;

    ideal_cache.put_elem (1, key);
    ideal_cache.put_elem (2, key);

    ideal_cache.lookup_update (slow_get_page);
    EXPECT_EQ (ideal_cache.get_element (key).key, key);
}

TEST (LfuUnitTest, SetAndGet)
{
    LFUCache<PageInfo> lfu_cache (STANDART_CACHE_SIZE);
    int key = 8800;

    lfu_cache.lookup_update (key, slow_get_page);

    EXPECT_EQ (lfu_cache.get_hash_value (key)->key_, key);
}

TEST (EndToEndTests, CheckNumberHits)
{
    size_t number_hits_lfu   = 0;
    size_t number_hits_ideal = 0;
    const char* test_file_name = nullptr;

    for (int test_number = 1; test_number <= NUMBER_TESTS; ++test_number)
    {
        std::cout << test_number << " Test:" << std::endl;
        std::string test_file_name = "tests/" + std::to_string (test_number) + "test.txt";

        number_hits_lfu   = lfu_test   (test_file_name);
        number_hits_ideal = ideal_test (test_file_name);

        EXPECT_EQ (number_hits_lfu,   get_answer (test_number, "tests/lfu_answers.txt"));
        EXPECT_EQ (number_hits_ideal, get_answer (test_number, "tests/ideal_answers.txt"));

        std::cout << "LFU:   number hits   = " << number_hits_lfu   << std::endl;
        std::cout << "Ideal: number hits   = " << number_hits_ideal << std::endl;
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
