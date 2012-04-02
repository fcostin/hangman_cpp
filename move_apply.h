#ifndef MOVE_APPLY_H
#define MOVE_APPLY_H

#include "hangman_types.h"
#include "hangman_constants.h"
#include "hangman_context.h"
#include "assert.h"

state_t apply_guesser_move(const context_t & ctx, const state_t & h,
        const char & chosen_c);
state_t apply_foe_move(const context_t & ctx, const state_t & h,
        const index_t & chosen_pattern_index);

#endif /* MOVE_APPLY_H */
