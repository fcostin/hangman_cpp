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
    
    // compute upper bound on number of possible words left with the
    // extremely loose bound: Alice (guessing player) can always
    // eliminate at least 1 possible word with each move she makes.
    // XXX TODO tighten this estimate (idea: count pattern frequencies
    // for patterns consistent with the live word set (these frequencies
    // are all at least one). sum the d smallest frequencies, where
    // d = (lives - 1). at least this many words must be excluded no
    // matter what Bob does (in reality the number is probably higher
    // since there are constraints preventing some combinations of
    // constraints (e.g. letter constraints and overlap constraints
    // for starters this would be similar to the lower bound used below).

    unsigned int lives = ctx.n_misses_for_loss - h.n_misses;
    if (h.live_word_indices.size() <= lives) {
        return make_pair(true, SCORE_GUESSER_WIN);
    }

    // compute lower bound on the number of possible words left if the
    // foe claims no subsequent guessed letters are in the unknown word
    // (this is able to a detect guesser loss scenario early in some
    // circumstances)
    vector<unsigned int> unused_letter_indices = make_unused_letter_indices(
            ctx.letter_table, ctx.words.size(), h.guesses, h.live_word_indices);
    unsigned int lower_bound = lower_bound_on_remaining_words(ctx.letter_table,
            ctx.words.size(), h.live_word_indices, unused_letter_indices, lives);
    if (lower_bound > 1) {
        return make_pair(true, SCORE_GUESSER_LOSE);
    }

    // we haven't detected a terminal game state - return false
    return make_pair(false, SCORE_WHATEVER);
}
