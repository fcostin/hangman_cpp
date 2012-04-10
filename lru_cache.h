#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list>
#include <algorithm>
#include "ugly_hash_table.h"
#include "fnv_hash.h"

using namespace std;

template <typename key_t, typename value_t>
struct lru_cache_t {
    typedef pair<key_t, value_t> item_t;
    typedef list<item_t> item_list_t;
    typedef ugly_hash_table_t<key_t, typename item_list_t::iterator,
        fnv_1a_hash_t (*) (const string &)> cache_map_t;

    item_list_t item_list;
    cache_map_t cache_map;
    size_t max_item_count;
    size_t item_count;

    lru_cache_t(const size_t & max_item_count) : cache_map(max_item_count, fnv_1a_hash) {
        assert(max_item_count > 0);
        this->item_count = 0;
        this->max_item_count = max_item_count;
    }

    inline size_t size() const {
        return item_count;
    }

    inline typename item_list_t::const_iterator begin() const {
        return item_list.begin();
    }

    inline typename item_list_t::const_iterator end() const {
        return item_list.end();
    }

    inline typename item_list_t::const_iterator find(const key_t & k) {
        if (cache_map.contains(k)) {
            typename item_list_t::iterator i = cache_map.get(k);
            // move accessed item to the front of the list
            item_list.push_front(*i);
            item_list.erase(i);
            // update the map to point to the moved item
            cache_map.insert(k, item_list.begin());
            // return an iterator to the moved item
            return item_list.begin();
        } else {
            // return an iterator to the end of the list of
            // items to signal that the key was not found
            return item_list.end();
        }
    }

    inline void insert(const key_t & key, const value_t & value) {
        /* insert the key value pair into the lru cache
         * 
         * precondition: there is *no* entry already stored in the lru cache
         *               under the given key.
         */
        if (++item_count > max_item_count) {
            --item_count;
            // remove the oldest key from the map
            key_t oldest_key = item_list.back().first;
            cache_map.erase(oldest_key);
            // then remove the associated item from the list
            item_list.pop_back();
        }
        // insert a new item into the list and add a map
        // entry to point to it.
        item_t item = make_pair(key, value);
        item_list.push_front(item);
        cache_map.insert(key, item_list.begin());
    }
};

#endif /* LRU_CACHE_H */
