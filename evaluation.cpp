#include "evaluation.h"

pair<bool, score_t> terminal_game_state(const context_t & ctx, const state_t & h) {
    // base case of guesser loss
    if (h.n_misses >= ctx.n_misses_for_loss) {
        return make_pair(true, SCORE_GUESSER_LOSE);
    }
    // base case of guesser win
    unsigned int n_words = h.live_word_indices.size();
    assert(n_words > 0);
    if (n_words == 1) {
        return make_pair(true, SCORE_GUESSER_WIN);
    }
    // compute lower bound on the number of possible words left if the
    // foe claims no subsequent guessed letters are in the unknown word
    // (this is able to a detect guesser loss scenario early in some
    // circumstances)
    vector<unsigned int> unused_letter_indices = make_unused_letter_indices(
            ctx.letter_table, ctx.words.size(), h.guesses, h.live_word_indices);
    unsigned int lives = ctx.n_misses_for_loss - h.n_misses;
    unsigned int lower_bound = lower_bound_on_remaining_words(ctx.letter_table,
            ctx.words.size(), h.live_word_indices, unused_letter_indices, lives);
    if (lower_bound > 1) {
        return make_pair(true, SCORE_GUESSER_LOSE);
    }
    // we haven't detected a terminal game state - return false
    return make_pair(false, SCORE_WHATEVER);
}
