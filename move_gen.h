#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include "alpha_beta.h"
#include <algorithm>

using namespace std;

vector<char> generate_guesser_moves(const context_t & ctx, const state_t & h,
        const string & h_key, size_t depth);

vector<size_t> generate_foe_moves(const context_t & ctx, const state_t & h,
        const string & h_key, size_t depth);

#endif /* MOVE_GEN_H */
