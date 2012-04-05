#ifndef EVALUATION_H
#define EVALUATION_H

#include "alpha_beta.h"
#include "assert.h"
#include "math.h"

pair<eval_result_t, score_t> terminal_game_state(const context_t & ctx, const state_t & h);

#endif /* EVALUATION_H */
