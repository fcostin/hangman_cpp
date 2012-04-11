#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include <algorithm>

#include "alpha_beta.h"
#include "hangman_debug.h"
#include "hangman_algorithm.h"

using namespace std;

template <typename T>
bool compare_by_second(const T & a, const T & b) {
    return (a.second < b.second);
}

vector<pair<size_t, size_t> > make_pattern_counts(const context_t & ctx,
        const vector<size_t> & word_indices, const size_t & c_index);

pair<size_t, size_t> most_common_pattern(
        const vector<pair<size_t, size_t> > & pattern_counts);

vector<char> generate_guesser_moves(const context_t & ctx, const state_t & h,
        const string & h_key, size_t depth);

vector<size_t> generate_foe_moves(const context_t & ctx, const state_t & h,
        const string & h_key, size_t depth);

#endif /* MOVE_GEN_H */
