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
    vector<vector<index_t> > vec_letter_word_to_pattern;

    inline index_t get_pattern_id(const char & c, const index_t & word_id) const {
        index_t c_index = (index_t)(c - 'a');
        return this->vec_letter_word_to_pattern[c_index][word_id];
    }
};

string make_pattern(char c, const string & word);

void make_letter_word_pattern_map(const vector<string> & words,
        vector<index_t> & patterns,
        vector<bool> & miss_patterns,
        map<string, index_t> & pattern_indices,
        vector<vector<index_t> > & vec_letter_word_to_pattern);

context_t make_hangman_context(const string & dictionary_file_name,
        unsigned int word_length, unsigned int n_misses_for_loss,
        unsigned int max_depth);

#endif // HANGMAN_CONTEXT_H
