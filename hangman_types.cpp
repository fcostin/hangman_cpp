#include "hangman_types.h"

state_t::state_t(const vector<char> & guesses, int n_misses,
            const set<index_t> & live_word_indices,
            const vector<char> & partial_word) {
    this->guesses = guesses;
    this->n_misses = n_misses;
    this->live_word_indices = live_word_indices;
    this->partial_word = partial_word;
}

cache_t::cache_t(const map<string, score_t> & move_cache) {
    this->move_cache = move_cache;
}

