#ifndef HANGMAN_TYPES_H
#define HANGMAN_TYPES_H

#include <vector>
#include <set>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

typedef unsigned int index_t;
typedef double score_t;
typedef char guesser_move_t;
typedef index_t foe_move_t;

class state_t {
public:
    vector<char> guesses;
    unsigned int n_misses;
    set<index_t> live_word_indices;
    vector<char> partial_word;

    state_t(const vector<char> & guesses, int n_misses,
            const set<index_t> & live_word_indices,
            const vector<char> & partial_word);
};

class cache_t {
public:
    map<string, score_t> move_cache;
    cache_t(const map<string, score_t> & move_cache);
};

#endif /* HANGMAN_TYPES_H */
