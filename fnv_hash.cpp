#include "fnv_hash.h"

fnv_1a_hash_t fnv_1a_hash(const std::string & s) {
    std::string::const_iterator i = s.begin();
    fnv_1a_hash_t hash = FNV_1A_OFFSET_BIAS;
    while(i != s.end()) {
        hash ^= (fnv_1a_hash_t)*(i++);
        hash *= FNV_1A_PRIME;
    }
    return hash;
}
