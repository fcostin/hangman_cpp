#ifndef HANGMAN_TYPES_H
#define HANGMAN_TYPES_H

#include <vector>
#include <unordered_set>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

typedef unsigned int index_t;
typedef double score_t;
typedef char guesser_move_t;
typedef index_t foe_move_t;

struct state_t {
    vector<char> guesses;
    unsigned int n_misses;
    unordered_set<index_t> live_word_indices;
    vector<char> partial_word;

    inline state_t(const vector<char> & guesses,
            const unsigned int & n_misses,
            const unordered_set<index_t> & live_word_indices,
            const vector<char> & partial_word) {
        this->guesses = guesses;
        this->n_misses = n_misses;
        this->live_word_indices = live_word_indices;
        this->partial_word = partial_word;
    }
};

struct cache_t {
    unordered_map<string, score_t> move_cache;
};

#endif /* HANGMAN_TYPES_H */
