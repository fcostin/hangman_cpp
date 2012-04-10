#ifndef EXCLUSION_H
#define EXCLUSION_H

#include "assert.h"

#include <string>
#include <vector>
#include <algorithm>

#include "hangman_constants.h"
#include "hangman_types.h"
#include "hangman_algorithm.h"

using namespace std;

inline size_t letter_to_index(const char & c);

inline char index_to_letter(const size_t & i);

inline void write_letter_table(vector<bool> & table,
        const size_t & n_words,
        const size_t & letter_i, const size_t & word_i,
        const bool value);

inline bool read_letter_table(const vector<bool> & table,
        const size_t & n_words,
        const size_t& letter_i, const size_t & word_i);

vector<bool> make_letter_table(const vector<string> & words);

vector<size_t> make_unused_letter_indices(const vector<bool> & table,
        const size_t & n_words,
        const vector<char> & used_letters,
        const vector<size_t> & word_indices);

size_t lower_bound_on_remaining_words(const vector<bool> & table,
        const size_t & n_words,
        const vector<size_t> & word_indices,
        const vector<size_t> & unused_letter_indices,
        const size_t & lives);

size_t upper_bound_on_remaining_words(
        const vector<size_t> & word_indices,
        const vector<size_t> & unused_letter_indices,
        const context_t & ctx,
        const size_t & lives);

#endif /* EXCLUSION_H */
