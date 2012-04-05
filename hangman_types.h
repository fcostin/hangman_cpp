#ifndef HANGMAN_TYPES_H
#define HANGMAN_TYPES_H

#include "assert.h"
#include <vector>
#include <unordered_set>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

#include "hangman_constants.h"

using namespace std;

struct state_t {
    vector<char> guesses;
    unsigned int n_misses;
    unordered_set<index_t> live_word_indices;
    vector<char> partial_word;
    char last_guess;

    inline state_t(const vector<char> & guesses,
            const unsigned int & n_misses,
            const unordered_set<index_t> & live_word_indices,
            const vector<char> & partial_word,
            const char & last_guess) {
        this->guesses = guesses;
        this->n_misses = n_misses;
        this->live_word_indices = live_word_indices;
        this->partial_word = partial_word;
        this->last_guess = last_guess;
    }
};

struct context_t {
    unsigned int word_length;
    unsigned int n_misses_for_loss;
    unsigned int max_depth;
    vector<string> words;
    vector<string> patterns;
    vector<bool> miss_patterns;
    map<string, index_t> pattern_indices;
    vector<vector<index_t> > vec_letter_word_to_pattern;
    vector<bool> letter_table;

    inline index_t get_pattern_id(const char & c, const index_t & word_id) const {
        index_t c_index = (index_t)(c - 'a');
        return this->vec_letter_word_to_pattern[c_index][word_id];
    }
};

enum eval_result_t {
    EVAL_RESULT_NOT_TERMINAL,
    EVAL_RESULT_BASE_LOSS,
    EVAL_RESULT_BASE_WIN,
    EVAL_RESULT_UPPER_BOUND_CHEAP,
    EVAL_RESULT_LOWER_BOUND_EXPENSIVE,
    EVAL_RESULT_UPPER_BOUND_EXPENSIVE
};

struct cache_t {
    unordered_map<string, score_t> move_cache;

    vector<unsigned int> stat_not_terminal;
    vector<unsigned int> stat_base_loss;
    vector<unsigned int> stat_base_win;
    vector<unsigned int> stat_upper_bound_cheap;
    vector<unsigned int> stat_lower_bound_expensive;
    vector<unsigned int> stat_upper_bound_expensive;
        
    inline cache_t() {
        stat_not_terminal.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_base_loss.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_base_win.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_upper_bound_cheap.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_lower_bound_expensive.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_upper_bound_expensive.resize(1 + (ALPHABET_SIZE * 2), 0);
    }

    inline void log_eval_result(const eval_result_t & er, const unsigned int & depth) {
        assert(depth <= (ALPHABET_SIZE * 2));
        switch(er) {
            case EVAL_RESULT_NOT_TERMINAL:
                stat_not_terminal[depth] += 1;
                break;
            case EVAL_RESULT_BASE_LOSS:
                stat_base_loss[depth] += 1;
                break;
            case EVAL_RESULT_BASE_WIN:
                stat_base_win[depth] += 1;
                break;
            case EVAL_RESULT_UPPER_BOUND_CHEAP:
                stat_upper_bound_cheap[depth] += 1;
                break;
            case EVAL_RESULT_UPPER_BOUND_EXPENSIVE:
                stat_upper_bound_expensive[depth] += 1;
                break;
            case EVAL_RESULT_LOWER_BOUND_EXPENSIVE:
                stat_lower_bound_expensive[depth] += 1;
                break;
            default:
                assert(0); // wut?!
        }
    }

    void dump_stats(ostream & out, const vector<unsigned int> & v) {
        vector<unsigned int>::const_reverse_iterator i;
        for(i = v.rbegin(); i != v.rend(); ++i) {
            out << *i;
            if (i + 1 != v.rend()) {
                out << ", ";
            }
        }
        out << endl;
    }

    void dump_summary(ostream & out) {
        out << "-------------" << endl;
        out << "Cache summary" << endl;
        out << "-------------" << endl;
        out << "\tmove_cache size " << move_cache.size() << endl;
        out << "\tstat_not_terminal ";
        dump_stats(out, stat_not_terminal);
        out << "\tstat_base_loss ";
        dump_stats(out, stat_base_loss);
        out << "\tstat_base_win ";
        dump_stats(out, stat_base_win);
        out << "\tstat_upper_bound_cheap ";
        dump_stats(out, stat_upper_bound_cheap);
        // out << "\tstat_upper_bound_expensive ";
        // dump_stats(out, stat_upper_bound_expensive);
        out << "\tstat_lower_bound_expensive ";
        dump_stats(out, stat_lower_bound_expensive);
    }

};

#endif /* HANGMAN_TYPES_H */
