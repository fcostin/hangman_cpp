#include "evaluation.h"

pair<eval_result_t, score_t> terminal_game_state(const context_t & ctx, const state_t & h) {
    // [BASE-LOSS]: Alice loses
    if (h.n_misses >= ctx.n_misses_for_loss) {
        return make_pair(EVAL_RESULT_BASE_LOSS, SCORE_GUESSER_LOSE);
    }
    // [BASE-WIN]: Alice wins
    unsigned int n_words = h.live_word_indices.size();
    assert(n_words > 0);
    if (n_words == 1) {
        return make_pair(EVAL_RESULT_BASE_WIN, SCORE_GUESSER_WIN);
    }

    // [UPPER-BOUND-CHEAP]: Compute loose upper bound on
    // number of possible words left at end of this game.
    // This can detect some wins for Alice cheaply.
    // (justification: Alice can always eliminate at
    // least 1 possible word per guess, and she has at least
    // lives - 1 guesses left, and she wins if the number of
    // words is less than 2)
    unsigned int lives = ctx.n_misses_for_loss - h.n_misses;
    if (h.live_word_indices.size() <= lives) {
        return make_pair(EVAL_RESULT_UPPER_BOUND_CHEAP, SCORE_GUESSER_WIN);
    }

    // needed for following two estimates
    vector<unsigned int> unused_letter_indices = make_unused_letter_indices(
            ctx.letter_table, ctx.words.size(), h.guesses, h.live_word_indices);

    // [LOWER-BOUND-EXPENSIVE]: compute lower bound on the number of
    // possible words left if the foe claims no subsequent guessed
    // letters are in the unknown word (this is able to a detect
    // guesser loss scenario early in some circumstances)
    unsigned int lower_bound = lower_bound_on_remaining_words(ctx.letter_table,
            ctx.words.size(), h.live_word_indices, unused_letter_indices, lives);
    if (lower_bound > 1) {
        return make_pair(EVAL_RESULT_LOWER_BOUND_EXPENSIVE, SCORE_GUESSER_LOSE);
    }

    // [NOT-TERMINAL] we haven't detected a terminal game state
    return make_pair(EVAL_RESULT_NOT_TERMINAL, SCORE_WHATEVER);
}
