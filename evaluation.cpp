#include "evaluation.h"

pair<eval_result_t, score_t> terminal_game_state(const context_t & ctx, const state_t & h) {
    DEBUG(cout << "$TERM start" << endl);
    // [BASE-LOSS]: Alice loses
    if (h.n_misses >= ctx.n_misses_for_loss) {
        DEBUG(cout << "$TERM leaf_loss" << endl);
        return make_pair(EVAL_RESULT_BASE_LOSS, SCORE_GUESSER_LOSE);
    }
    // [BASE-WIN]: Alice wins
    size_t n_words = h.live_word_indices.size();
    assert(n_words > 0);
    if (n_words == 1) {
        DEBUG(cout << "$TERM leaf_win" << endl);
        return make_pair(EVAL_RESULT_BASE_WIN, SCORE_GUESSER_WIN);
    }

    // [UPPER-BOUND-CHEAP]: Compute loose upper bound on
    // number of possible words left at end of this game.
    // This can detect some wins for Alice cheaply.
    // (justification: Alice can always eliminate at
    // least 1 possible word per guess, and she has at least
    // lives - 1 guesses left, and she wins if the number of
    // words is less than 2)
    size_t lives = ctx.n_misses_for_loss - h.n_misses;
    if (h.live_word_indices.size() <= lives) {
        DEBUG(cout << "$TERM upper_bound" << endl);
        return make_pair(EVAL_RESULT_UPPER_BOUND_CHEAP, SCORE_GUESSER_WIN);
    }

    // needed for following two estimates
    vector<size_t> unused_letter_indices = make_unused_letter_indices(
            ctx.letter_table, ctx.words.size(), h.guesses, h.live_word_indices);

    // only compute the expensive lower bound if a random forest classifier
    // predicts that it will succeed.
    size_t forest_clf_inputs[6];
    forest_clf_inputs[0] = n_words;
    forest_clf_inputs[1] = lives;
    forest_clf_inputs[2] = h.guesses.size();
    forest_clf_inputs[3] = unused_letter_indices.size();
    forest_clf_inputs[4] = ctx.word_length;
    forest_clf_inputs[5] = ctx.n_misses_for_loss;
    if (forest_clf(forest_clf_inputs)) {
        // [LOWER-BOUND-EXPENSIVE]: compute lower bound on the number of
        // possible words left if the foe claims no subsequent guessed
        // letters are in the unknown word (this is able to a detect
        // guesser loss scenario early in some circumstances)
        DEBUG(cout << "$TERM forest_clf_check" << endl);
        size_t lower_bound = lower_bound_on_remaining_words(ctx.letter_table,
                ctx.words.size(), h.live_word_indices, unused_letter_indices, lives);
        if (lower_bound > 1) {
            DEBUG(cout << "$TERM lower_bound" << endl);
            return make_pair(EVAL_RESULT_LOWER_BOUND_EXPENSIVE, SCORE_GUESSER_LOSE);
        }
    }

    // [NOT-TERMINAL] we haven't detected a terminal game state
    DEBUG(cout << "$TERM not_terminal" << endl);
    return make_pair(EVAL_RESULT_NOT_TERMINAL, SCORE_WHATEVER);
}
