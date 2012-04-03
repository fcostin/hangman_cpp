#include "move_apply.h"

state_t apply_guesser_move(const context_t & ctx __attribute__ ((unused)),
        const state_t & h, const char & chosen_c) {
    vector<char> next_guesses(h.guesses);
    next_guesses.push_back(chosen_c);
    state_t next_h(next_guesses, h.n_misses, h.live_word_indices,
            h.partial_word);
    return next_h;
}

state_t apply_foe_move(const context_t & ctx, const state_t & h,
        const index_t & chosen_pattern_index) {
    /* arguments:
     *  ctx: game context (rules and word dictionary)
     *  h: current game state
     *  pattern_index: move made by foe (this is a pattern index of type index_t)
     */
    // update n_misses
    unsigned int is_miss = (unsigned int)ctx.miss_patterns[chosen_pattern_index];
    unsigned int next_n_misses = h.n_misses + is_miss;

    //  live_word_indices -- rebuild the live word indices
    char c = h.guesses.back();
    unordered_set<index_t> next_live_word_indices;
    unordered_set<index_t>::const_iterator i;
    for (i = h.live_word_indices.begin(); i != h.live_word_indices.end(); ++i) {
        if (ctx.get_pattern_id(c, *i) == chosen_pattern_index) {
            next_live_word_indices.insert(*i);
        }
    }
    //  partial word -- what pattern does the pattern index m correspond to?
    string pattern = ctx.patterns[chosen_pattern_index];
    assert(pattern[0] == c);
    unsigned int j;
    vector<char> next_partial_word(h.partial_word);
    for (j = 2; j < pattern.size(); ++j) {
        if (pattern[j] == '1') {
            next_partial_word[j - 2] = c;
        }
    }

    state_t next_h(h.guesses, next_n_misses, next_live_word_indices, next_partial_word);
    return next_h;
}
