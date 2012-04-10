#ifndef HANGMAN_CONTEXT_H
#define HANGMAN_CONTEXT_H

#include "assert.h"
#include "hangman_types.h"
#include "hangman_constants.h"
#include "exclusion.h"

#include <fstream>

string make_pattern(char c, const string & word);

void make_letter_word_pattern_map(const vector<string> & words,
        vector<size_t> & patterns,
        vector<bool> & miss_patterns,
        map<string, size_t> & pattern_indices,
        vector<vector<size_t> > & vec_letter_word_to_pattern);

context_t make_hangman_context(const string & dictionary_file_name,
        size_t word_length, size_t n_misses_for_loss,
        size_t max_depth);

#endif // HANGMAN_CONTEXT_H
