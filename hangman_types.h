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
#include "lru_cache.h"

using namespace std;

struct state_t {
    vector<char> guesses;
    size_t n_misses;
    unordered_set<size_t> live_word_indices;
    vector<char> partial_word;
    char last_guess;

    inline state_t(const vector<char> & guesses,
            const size_t & n_misses,
            const unordered_set<size_t> & live_word_indices,
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
    size_t word_length;
    size_t n_misses_for_loss;
    size_t max_depth;
    vector<string> words;
    vector<string> patterns;
    vector<bool> miss_patterns;
    map<string, size_t> pattern_indices;
    vector<vector<size_t> > vec_letter_word_to_pattern;
    vector<bool> letter_table;

    inline size_t get_pattern_id(const char & c, const size_t & word_id) const {
        size_t c_index = (size_t)(c - 'a');
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
    lru_cache_t<string, score_t> move_cache;

    vector<size_t> stat_not_terminal;
    vector<size_t> stat_base_loss;
    vector<size_t> stat_base_win;
    vector<size_t> stat_upper_bound_cheap;
    vector<size_t> stat_lower_bound_expensive;
    vector<size_t> stat_upper_bound_expensive;
        
    cache_t() : move_cache(MOVE_CACHE_SIZE) {
        stat_not_terminal.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_base_loss.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_base_win.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_upper_bound_cheap.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_lower_bound_expensive.resize(1 + (ALPHABET_SIZE * 2), 0);
        stat_upper_bound_expensive.resize(1 + (ALPHABET_SIZE * 2), 0);
    }

    inline void log_eval_result(const eval_result_t & er, const size_t & depth) {
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

    void dump_stats(ostream & out, const vector<size_t> & v) {
        vector<size_t>::const_reverse_iterator i;
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
