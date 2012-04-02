#ifndef EVALUATION_H
#define EVALUATION_H

#include "alpha_beta.h"
#include "assert.h"
#include "math.h"

score_t evaluate_game_state(const context_t & ctx, const state_t & h);

#endif /* EVALUATION_H */
