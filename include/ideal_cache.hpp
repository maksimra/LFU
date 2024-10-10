#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

#include "get_page.hpp"

template <typename PageT, typename KeyT = int>
class IdealCache
{
    size_t cache_sz_;
    std::unordered_map<KeyT, PageT> hash_;

    void add_new_element (KeyT key, PageT page)
    {
        hash_[key] = page;
    }

    void delete_latest_element ()
    {
        size_t latest_appear = 0;
        KeyT key = 0;
        for (hash_it it = hash_.begin (); it != hash_.end (); it++)
        {
            if (!sequency_map[it->first].empty () &&
                 sequency_map[it->first].front () > latest_appear)
            {
                latest_appear = sequency_map[it->first].front ();
                key = it->first;
            }
        }

        hash_.erase (key);
    }

    bool full () const
    {
        return (hash_.size () >= cache_sz_);
    }

public:
    std::list<KeyT> key_list;
    std::unordered_map<KeyT, std::list<size_t>> sequency_map;

    IdealCache (size_t cache_size): cache_sz_ (cache_size) {}

    using hash_it = typename std::unordered_map<KeyT, PageT>::iterator;

    bool lookup_update (PageT (*slow_get_page) (KeyT key))
    {
        KeyT key = key_list.front ();
        auto hit = hash_.find (key);

        key_list.pop_front ();

        sequency_map[key].pop_front ();

        if (hit == hash_.end ())
        {
            if (sequency_map[key].empty ())
                return false;

            if (full())
                delete_latest_element ();

            add_new_element (key, slow_get_page (key));
            return false;
        }
        return true;
    }
};

// TODO: Достали первый элемент списка, ищем его в хэше, удаляем первую встречу элемента в списке
//       Если нет в хэше - если полный - удаляем тот, который встретится не скоро
//       Затем - добавляем элемент
//       Если есть в хэше -
