#ifndef EXCLUSION_H
#define EXCLUSION_H

#include "assert.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include "hangman_constants.h"
#include "hangman_types.h"

using namespace std;

inline unsigned int letter_to_index(const char & c);

inline char index_to_letter(const unsigned int & i);

inline void write_letter_table(vector<bool> & table,
        const unsigned int & n_words,
        const unsigned int & letter_i, const unsigned int & word_i,
        const bool value);

inline bool read_letter_table(const vector<bool> & table,
        const unsigned int & n_words,
        const unsigned int & letter_i, const unsigned int & word_i);

vector<bool> make_letter_table(const vector<string> & words);

vector<unsigned int> make_unused_letter_indices(const vector<bool> & table,
        const unsigned int & n_words,
        const vector<char> & used_letters,
        const unordered_set<unsigned int> & word_indices);

unsigned int lower_bound_on_remaining_words(const vector<bool> & table,
        const unsigned int & n_words,
        const unordered_set<unsigned int> & word_indices,
        const vector<unsigned int> & unused_letter_indices,
        const unsigned int & lives);

unsigned int upper_bound_on_remaining_words(
        const unordered_set<unsigned int> & word_indices,
        const vector<unsigned int> & unused_letter_indices,
        const context_t & ctx,
        const unsigned int & lives);

#endif /* EXCLUSION_H */
