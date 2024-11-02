#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

template <typename PageT, typename KeyT = int>
class IdealCache
{
private:
    size_t cache_sz_;
    std::unordered_map<KeyT, PageT> hash_;
    std::list<KeyT> key_list;
    std::unordered_map<KeyT, std::list<size_t>> sequency_map;

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
            std::list<size_t>& key_sequency_list = sequency_map[it->first];
            if (key_sequency_list.empty ())
            {
                key = it->first;
                break;
            }

            if (!key_sequency_list.empty () &&
                 key_sequency_list.front () > latest_appear)
            {
                latest_appear = key_sequency_list.front ();
                key = it->first;
            }
        }

        hash_.erase (key);
    }

    bool full () const
    {
        return (hash_.size () >= cache_sz_);
    }

    void put_elem (size_t element_num, KeyT key)
    {
        key_list.push_back (key);
        sequency_map[key].push_back (element_num);
    }

public:
    template <typename Iterator>
    IdealCache (size_t cache_size, Iterator begin, Iterator end): cache_sz_ (cache_size)
    {
        size_t element_num = 0;

        for (auto iter = begin; iter != end; ++iter)
        {
            put_elem (element_num++, *iter);
        }
    }

    using hash_it = typename std::unordered_map<KeyT, PageT>::iterator;

    PageT get_element (KeyT key)
    {
        return hash_.at (key);
    }

    template <typename F>
    bool lookup_update (F slow_get_page)
    {
        KeyT key = key_list.front ();
        auto hit = hash_.find (key);

        key_list.pop_front ();

        std::list<size_t>& key_sequency_list = sequency_map[key];
        key_sequency_list.pop_front ();

        if (hit == hash_.end ())
        {
            if (key_sequency_list.empty ())
                return false;

            if (full())
                delete_latest_element ();

            add_new_element (key, slow_get_page (key));
            return false;
        }
        return true;
    }
};
