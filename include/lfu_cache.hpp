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

        CacheNode (KeyT key, PageT page, freq_it freq_it):
                   key_ (key), page_ (page), freq_it_ (freq_it) {}
    };

    struct FreqNode
    {
        size_t frequency_ = 0;
        std::list<CacheNode> pages_ = {};

        FreqNode (size_t frequency):
                  frequency_ (frequency) {}
    };

    std::list<FreqNode> freq_list_ = {};

    void add_new_element (KeyT key, PageT page)
    {
        if (freq_list_.begin ()->frequency_ != 1)
        {
            FreqNode freq_node (1);
            freq_list_.push_front (freq_node);
        }

        CacheNode cache_node (key, page, freq_list_.begin ());
        freq_list_.begin ()->pages_.push_front (cache_node);
        hash_[key] = freq_list_.begin ()->pages_.begin ();
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
    std::unordered_map<KeyT, cache_node_it> hash_;

    void delete_cache_element ()
    {
        hash_.erase (freq_list_.begin()->pages_.back ().key_);
        freq_list_.begin ()->pages_.pop_back ();
    }
    
    bool element_exists (KeyT key)
    {
        if (hash_.find (key) != hash_.end ())
            return true;
        return false;
    }

    bool lookup_update (KeyT key, PageT (*slow_get_page) (KeyT key))
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
