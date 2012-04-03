#include "hangman_context.h"


string make_pattern(char c, const string & word) {
    string pattern;
    pattern.push_back(c);
    pattern.push_back(';');
    string::const_iterator i;
    for (i = word.begin(); i != word.end(); ++i) {
        pattern.push_back((*i == c) ? '1' : '0');
    }
    return pattern;
}

bool is_miss_pattern(const string & pattern) {
    string::const_iterator i;
    for (i = pattern.begin() + 2; i != pattern.end(); ++i) {
        if (*i != '0') {
            return false;
        }
    }
    return true;
}

void make_letter_word_pattern_map(const vector<string> & words,
        vector<string> & patterns,
        vector<bool> & miss_patterns,
        map<string, index_t> & pattern_indices,
        vector<vector<index_t> > & vec_letter_word_to_pattern) {
    /* input arguments:
     *  words : vector of words
     * output arguments:
     *  pattern_indices : map of unique patterns to pattern indices
     *  vec_letter_word_to_pattern : vec {char index -> vec {word index -> pattern index }}
     * note:
     *  the words are implicitly indexed by their order in the input vector.
     */

    // loop over words
    // loop over characters in alphabet
    // make pattern for word w and character c
    // determine unique index for pattern (make it if necessary)
    // add entry to resulting map
    
    index_t word_index = 0;
    vector<string>::const_iterator i;

    string::const_iterator j;
    map<string, index_t>::iterator k;

    index_t pattern_index = 0;


    for (j = ALPHABET.begin(); j != ALPHABET.end(); ++j) {
        vector<index_t> v;
        v.resize(words.size());
        vec_letter_word_to_pattern.push_back(v);
    }

    for (i = words.begin(); i != words.end(); ++i) {
        for (j = ALPHABET.begin(); j != ALPHABET.end(); ++j) {
            string pattern = make_pattern(*j, *i);
            k = pattern_indices.find(pattern);
            if (k == pattern_indices.end()) {
                pattern_indices[pattern] = pattern_index++;
                patterns.push_back(pattern);
                miss_patterns.push_back(is_miss_pattern(pattern));
            }
            index_t c_id = (index_t)(*j - 'a');
            vec_letter_word_to_pattern[c_id][word_index] = pattern_indices[pattern];
        }
        word_index++;
    }
}

context_t make_hangman_context(const string & dictionary_file_name,
        unsigned int word_length, unsigned int n_misses_for_loss,
        unsigned int max_depth) {

    ifstream f(dictionary_file_name.c_str());
    assert(f);

    context_t ctx;
    ctx.word_length = word_length;
    ctx.n_misses_for_loss = n_misses_for_loss;
    ctx.max_depth = max_depth;

    string word;
    while (f >> word) {
        if (word.size() == ctx.word_length) {
            ctx.words.push_back(word);
        }
    }
    make_letter_word_pattern_map(ctx.words, ctx.patterns, ctx.miss_patterns,
            ctx.pattern_indices, ctx.vec_letter_word_to_pattern);
    return ctx;
}
