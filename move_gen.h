#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include "alpha_beta.h"
#include <algorithm>

using namespace std;

vector<guesser_move_t> generate_guesser_moves(const context_t & ctx, const state_t & h,
        const string & h_key, unsigned int depth);

vector<foe_move_t> generate_foe_moves(const context_t & ctx, const state_t & h,
        const string & h_key, unsigned int depth);

#endif /* MOVE_GEN_H */
