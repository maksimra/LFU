#include <gtest/gtest.h>
#include <fstream>
#include <iostream>
#include "../include/lfu_cache.hpp"
#include "../include/ideal_cache.hpp"
#include "../include/get_page.hpp"

const size_t STANDART_CACHE_SIZE = 6;

size_t lfu_test (const char* filename)
{
    std::fstream test_file (filename);
    if (!test_file.is_open ())
    {
        throw std::runtime_error ("Не удалось открыть файл " + std::string (filename));
    }
    auto original_buffer = std::cin.rdbuf(test_file.rdbuf());

    size_t cache_capacity = 0, number_pages = 0;
    std::cin >> cache_capacity >> number_pages;

    LFUCache<PageInfo> cache (cache_capacity);
    size_t num_hits = 0;

    int key = 0;

    for (int page_number = 0;
         page_number < number_pages;
         page_number++)
    {
        std::cin >> key;

        assert (std::cin.good ());

        if (cache.lookup_update (key, slow_get_page))
            num_hits += 1;
    }

    std::cin.rdbuf(original_buffer);
    return num_hits;
}

size_t ideal_test (const char* filename)
{
    std::fstream test_file (filename);
    if (!test_file.is_open ())
    {
        throw std::runtime_error ("Не удалось открыть файл " + std::string (filename));
    }
    auto original_buffer = std::cin.rdbuf(test_file.rdbuf());

    size_t cache_capacity = 0, number_pages = 0;
    std::cin >> cache_capacity >> number_pages;

    IdealCache<PageInfo> cache (cache_capacity);
    int key = 0;

    for (size_t element_num = 0;
         element_num < number_pages;
         element_num++)
    {
        std::cin >> key;

        assert (std::cin.good ());

        cache.key_list.push_back (key);
        cache.sequency_map[key].push_back (element_num);
    }

    size_t num_hits = 0;

    for (size_t element_num = 0;
         element_num < number_pages;
         element_num++)
    {
        if (cache.lookup_update (slow_get_page))
            num_hits += 1;
    }

    std::cin.rdbuf(original_buffer);
    return num_hits;
}

size_t get_answer (size_t test_number, const char* filename)
{
    std::vector<size_t> answers;

    std::fstream answers_file(filename, std::ios::in);

    size_t answer;

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

TEST (LfuUnitTest, CacheInsertDelete)
{
    LFUCache<PageInfo> lfu_cache (STANDART_CACHE_SIZE);
    int key = 17156;

    lfu_cache.lookup_update (key, slow_get_page);
    EXPECT_EQ (lfu_cache.hash_[key]->key_, key);

    lfu_cache.delete_cache_element ();
    EXPECT_EQ (lfu_cache.element_exists (key), false);
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

    ideal_cache.key_list.push_back (key);

    ideal_cache.sequency_map[key].push_back (1);
    ideal_cache.sequency_map[key].push_back (2);

    ideal_cache.lookup_update (slow_get_page);
    EXPECT_EQ (ideal_cache.get_element (key).key, key);
}

TEST (LfuUnitTest, SetAndGet)
{
    LFUCache<PageInfo> lfu_cache (STANDART_CACHE_SIZE);
    int key = 8800;

    lfu_cache.lookup_update (key, slow_get_page);

    EXPECT_EQ (lfu_cache.hash_[key]->key_, key);
}

TEST (EndToEndTest, Test_1)
{
    size_t number_hits_lfu   = lfu_test   ("tests/1test.txt");
    size_t number_hits_ideal = ideal_test ("tests/1test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (1, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (1, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_2)
{
    size_t number_hits_lfu   = lfu_test   ("tests/2test.txt");
    size_t number_hits_ideal = ideal_test ("tests/2test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (2, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (2, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_3)
{
    size_t number_hits_lfu   = lfu_test   ("tests/3test.txt");
    size_t number_hits_ideal = ideal_test ("tests/3test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (3, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (3, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_4)
{
    size_t number_hits_lfu   = lfu_test   ("tests/4test.txt");
    size_t number_hits_ideal = ideal_test ("tests/4test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (4, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (4, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_5)
{
    size_t number_hits_lfu   = lfu_test   ("tests/5test.txt");
    size_t number_hits_ideal = ideal_test ("tests/5test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (5, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (5, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_6)
{
    size_t number_hits_lfu   = lfu_test   ("tests/6test.txt");
    size_t number_hits_ideal = ideal_test ("tests/6test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (6, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (6, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_7)
{
    size_t number_hits_lfu   = lfu_test   ("tests/7test.txt");
    size_t number_hits_ideal = ideal_test ("tests/7test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (7, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (7, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_8)
{
    size_t number_hits_lfu   = lfu_test   ("tests/8test.txt");
    size_t number_hits_ideal = ideal_test ("tests/8test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (8, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (8, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_9)
{
    size_t number_hits_lfu   = lfu_test   ("tests/9test.txt");
    size_t number_hits_ideal = ideal_test ("tests/9test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (9, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (9, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

TEST (EndToEndTest, Test_10)
{
    size_t number_hits_lfu   = lfu_test   ("tests/10test.txt");
    size_t number_hits_ideal = ideal_test ("tests/10test.txt");
    EXPECT_EQ (number_hits_lfu,   get_answer (10, "tests/lfu_answers.txt"));
    EXPECT_EQ (number_hits_ideal, get_answer (10, "tests/ideal_answers.txt"));

    std::cout << "LFU:   number hits   = " << number_hits_lfu << "\n";
    std::cout << "Ideal: number hits   = " << number_hits_ideal << "\n";
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
