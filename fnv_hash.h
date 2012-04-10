#ifndef FNV_HASH_H
#define FNV_HASH_H

#include <string>

/* my very own (slow as hell? buggy?) version of the fvn 1a hash
 * see: http://www.isthe.com/chongo/tech/comp/fnv/ for the real deal... */

typedef unsigned long fnv_1a_hash_t;
#define FNV_1A_OFFSET_BIAS (fnv_1a_hash_t)14695981039346656037UL
#define FNV_1A_PRIME (fnv_1a_hash_t)1099511628211UL

fnv_1a_hash_t fnv_1a_hash(const std::string & s);

#endif /* FNV_HASH_H */
