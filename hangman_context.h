#ifndef HANGMAN_CONTEXT_H
#define HANGMAN_CONTEXT_H

#include "hangman_types.h"
#include "hangman_constants.h"
#include "assert.h"

struct context_t {
    unsigned int word_length;
    unsigned int n_misses_for_loss;
    unsigned int max_depth;
    vector<string> words;
    vector<string> patterns;
    vector<bool> miss_patterns;
    map<string, index_t> pattern_indices;
    map<pair<char, index_t>, index_t> letter_word_to_pattern;
};

string make_pattern(char c, const string & word);

void make_letter_word_pattern_map(const vector<string> & words,
        vector<index_t> & patterns,
        vector<bool> & miss_patterns,
        map<string, index_t> & pattern_indices,
        map<pair<char, index_t>, index_t> & char_word_to_pattern);

context_t make_hangman_context(const string & dictionary_file_name,
        unsigned int word_length, unsigned int n_misses_for_loss,
        unsigned int max_depth);

#endif // HANGMAN_CONTEXT_H
