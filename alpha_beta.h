#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

#include "assert.h"

#include "hangman_debug.h"

#include "hangman_types.h"
#include "hangman_constants.h"
#include "hangman_utils.h"
#include "hangman_context.h"

inline string make_key_for_game_state(const state_t & h);

pair<eval_result_t, score_t> terminal_game_state(const context_t & ctx, const state_t & h);

vector<char> generate_guesser_moves(const context_t & ctx, const state_t & h,
        const string & h_key, size_t depth);

vector<size_t> generate_foe_moves(const context_t & ctx, const state_t & h,
        const string & h_key, size_t depth);

state_t apply_guesser_move(const context_t & ctx, const state_t & h,
        const char & m);
state_t apply_foe_move(const context_t & ctx, const state_t & h,
        const size_t & m);

score_t optimal_guesser_score(const context_t & ctx, cache_t & cache, const state_t & h,
        size_t depth, score_t alpha, score_t beta);

score_t optimal_foe_score(const context_t & ctx, cache_t & cache, const state_t & h,
        size_t depth, score_t alpha, score_t beta);

#endif /* ALPHA_BETA_H */
