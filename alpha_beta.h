#ifndef ALPHA_BETA_H
#define ALPHA_BETA_H

#include "hangman_types.h"
#include "hangman_constants.h"
#include "hangman_utils.h"
#include "hangman_context.h"
#include "assert.h"
#include <iostream> // for debugging

string make_key_for_game_state(const state_t & h);
bool is_terminal_game_state(const context_t & ctx, const state_t & h);

score_t evaluate_game_state(const context_t & ctx, const state_t & h);

vector<guesser_move_t> generate_guesser_moves(const context_t & ctx, const state_t & h,
        const string & h_key, unsigned int depth);

vector<foe_move_t> generate_foe_moves(const context_t & ctx, const state_t & h,
        const string & h_key, unsigned int depth);

state_t apply_guesser_move(const context_t & ctx, const state_t & h,
        const guesser_move_t & m);
state_t apply_foe_move(const context_t & ctx, const state_t & h,
        const foe_move_t & m);

score_t optimal_guesser_score(const context_t & ctx, cache_t & cache, const state_t & h,
        unsigned int depth, score_t alpha, score_t beta);

score_t optimal_foe_score(const context_t & ctx, cache_t & cache, const state_t & h,
        unsigned int depth, score_t alpha, score_t beta);

#endif /* ALPHA_BETA_H */
