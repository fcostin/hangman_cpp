#include "move_apply.h"

state_t apply_guesser_move(const context_t & ctx __attribute__ ((unused)),
        const state_t & h, const char & chosen_c) {
    state_t next_h(h.guesses, h.n_misses, h.live_word_indices,
            h.partial_word, chosen_c);
    return next_h;
}

state_t apply_foe_move(const context_t & ctx, const state_t & h,
        const size_t & chosen_pattern_index) {
    /* arguments:
     *  ctx: game context (rules and word dictionary)
     *  h: current game state
     *  pattern_index: move made by foe (this is a pattern index of type size_t)
     */
    // update n_misses
    size_t is_miss = (size_t)ctx.miss_patterns[chosen_pattern_index];
    size_t next_n_misses = h.n_misses + is_miss;

    //  live_word_indices -- rebuild the live word indices
    char c = h.last_guess;
    unordered_set<size_t> next_live_word_indices;
    unordered_set<size_t>::const_iterator i;
    for (i = h.live_word_indices.begin(); i != h.live_word_indices.end(); ++i) {
        if (ctx.get_pattern_id(c, *i) == chosen_pattern_index) {
            next_live_word_indices.insert(*i);
        }
    }
    //  partial word -- what pattern does the pattern index m correspond to?
    string pattern = ctx.patterns[chosen_pattern_index];
    assert(pattern[0] == c);
    size_t j;
    vector<char> next_partial_word(h.partial_word);
    for (j = 2; j < pattern.size(); ++j) {
        if (pattern[j] == '1') {
            next_partial_word[j - 2] = c;
        }
    }

    vector<char> next_guesses(h.guesses);
    next_guesses.push_back(c);

    state_t next_h(next_guesses, next_n_misses, next_live_word_indices,
            next_partial_word, (char)0);
    return next_h;
}
