#pragma once

struct PageInfo
{
    int key;
    int size;
    const char* data;
};

PageInfo slow_get_page (int key)
{
    PageInfo new_page = {key, 420, "I am page\n"};
    return new_page;
}
