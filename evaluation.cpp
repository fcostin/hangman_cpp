#include "evaluation.h"

score_t evaluate_game_state(const context_t & ctx, const state_t & h) {
    if (h.n_misses >= ctx.n_misses_for_loss) {
        return SCORE_GUESSER_LOSE;
    }
    unsigned int n_words = h.live_word_indices.size();
    assert(n_words);
    if (n_words == 1) {
        return SCORE_GUESSER_WIN;
    }

    // first stab in the dark at an evaluation heuristic
    float gamma = 0.5f;
    float lives_left = (float)(ctx.n_misses_for_loss - h.n_misses);
    return -1.0f * ((float)n_words) * pow(gamma, lives_left);
}
