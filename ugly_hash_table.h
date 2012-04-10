#ifndef UGLY_HASH_TABLE_H
#define UGLY_HASH_TABLE_H

/* this is a really ugly home-brew hash table. it is not resizeable.
 * it is built out of a <vector> of <list>s of key, value <pair>s.
 *
 * it doesn't come with a defined hash function - it takes one
 * as a constructor arguments.
 *
 * it doesn't follow the same interface as <map> or <unordered_map>
 */

#include <vector>
#include <list>

template<typename k_t, typename v_t, typename hash_func_t>
struct ugly_hash_table_t {
public:
    typedef std::list<std::pair<k_t, v_t> > bucket_t;
    typedef std::vector<bucket_t> bucket_vector_t;

private:
    size_t n_buckets;
    bucket_vector_t buckets;
    hash_func_t hash_func;

public:
    ugly_hash_table_t(const size_t & size, hash_func_t hash_func) {
        n_buckets = size;
        buckets.resize(n_buckets);
        this->hash_func = hash_func;
    }

    bool contains(const k_t & key) const {
        size_t i = hash_func(key) % n_buckets;
        typename bucket_t::const_iterator j;
        for (j = buckets[i].begin(); j != buckets[i].end(); ++j) {
            if (j->first == key) {
                return true;
            }
        }
        return false;
    }

    v_t get(const k_t & key) const {
        size_t i = hash_func(key) % n_buckets;
        typename bucket_t::const_iterator j;
        v_t result;
        for (j = buckets[i].begin(); j != buckets[i].end(); ++j) {
            if (j->first == key) {
                result = j->second;
                break;
            }
        }
        return result;
    }

    void erase(const k_t & key) {
        size_t i = hash_func(key) % n_buckets;
        typename bucket_t::iterator j;
        for (j = buckets[i].begin(); j != buckets[i].end(); ++j) {
            if (j->first == key) {
                buckets[i].erase(j);
                return;
            }
        }
    }

    void insert(const k_t & k, const v_t & v) {
        size_t i = hash_func(k) % n_buckets;
        typename bucket_t::iterator j;
        for (j = buckets[i].begin(); j != buckets[i].end(); ++j) {
            if (j->first == k) {
                j->second = v;
                return;
            }
        }
        buckets[i].push_front(std::make_pair(k, v));
    }
};

#endif /* UGLY_HASH_TABLE_H */
