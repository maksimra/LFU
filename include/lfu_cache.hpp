#pragma once

#include <iostream>
#include <list>
#include <unordered_map>

#include "get_page.hpp"

template <typename PageT, typename KeyT = int>
class LFUCache
{
    size_t cache_sz_;

    struct CacheNode;
    struct FreqNode;

    using cache_node_it = typename std::list<CacheNode>::iterator;
    using freq_it       = typename std::list<FreqNode>::iterator;

    struct CacheNode
    {
        KeyT key_;
        PageT page_;
        freq_it freq_it_;
    };

    struct FreqNode
    {
        size_t frequency_ = 1;
        std::list<CacheNode> pages_ = {};
    };

    std::list<FreqNode> freq_list_ = {};
    std::unordered_map<KeyT, cache_node_it> hash_;

    void add_new_element (KeyT key, PageT page)
    {
        if (freq_list_.begin ()->frequency_ != 1)
        {
            FreqNode freq_node;
            freq_list_.push_front (freq_node);
        }

        CacheNode cache_node (key, page, freq_list_.begin ());
        freq_list_.begin ()->pages_.push_front (cache_node);
        hash_[key] = freq_list_.begin ()->pages_.begin ();
    }

    void delete_cache_element ()
    {
        hash_.erase (freq_list_.begin()->pages_.back ().key_);
        freq_list_.begin ()->pages_.pop_back ();
    }

    void update_cache_list (cache_node_it cache_node_it)
    {
        cache_node_it->freq_it_->frequency_ += 1;
    }

    bool full () const
    {
        return (hash_.size () >= cache_sz_);
    }

public:
    LFUCache (size_t cache_size): cache_sz_ (cache_size) {}

    cache_node_it get_hash_value (KeyT key)
    {
        return hash_[key];
    }

    bool element_exists (KeyT key)
    {
        if (hash_.find (key) != hash_.end ())
            return true;
        return false; //
    }

    template <typename F>
    bool lookup_update (KeyT key, F slow_get_page)
    {
        auto hit = hash_.find (key);

        if (hit == hash_.end ())
        {
            if (full())
            {
                delete_cache_element ();
            }

            add_new_element (key, slow_get_page (key));
            return false;
        }

        update_cache_list (hit->second);
        return true;
    }
};
