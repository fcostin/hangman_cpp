#include "move_gen.h"

vector<guesser_move_t> generate_guesser_moves(
        const context_t & ctx __attribute__((unused)),
        const state_t & h,
        const string & h_key __attribute__((unused)),
        unsigned int depth __attribute__((unused))) {

    // XXX FIXME this is a very entry-level way of generating moves.
    // We simply pick the first letter in the alphabet that we have not tried yet.
    vector<guesser_move_t> moves;
    string::const_iterator i;
    for (i = ALPHABET.begin(); i != ALPHABET.end(); ++i) {
        if(find(h.guesses.begin(), h.guesses.end(), *i) == h.guesses.end()) {
            moves.push_back(*i);
        }
    }
    return moves;
}

vector<foe_move_t> generate_foe_moves(
        const context_t & ctx,
        const state_t & h,
        const string & h_key __attribute__((unused)),
        unsigned int depth __attribute__((unused))) {
    
    // XXX FIXME this is an entry-level method of generating moves
    // We figure out what all the consistent moves are and try them
    // in an arbitrary order.
    unordered_set<foe_move_t> moves;
    char c = h.last_guess;
    unordered_set<index_t>::const_iterator i;
    for (i = h.live_word_indices.begin(); i != h.live_word_indices.end(); ++i) {
        moves.insert(ctx.get_pattern_id(c, *i));
    }
    // XXX heuristic : try all of the "no" moves first
    vector<foe_move_t> result;
    unordered_set<foe_move_t>::const_iterator j;
    for (j = moves.begin(); j != moves.end(); ++j) {
        if (ctx.miss_patterns[*j]) {
            result.push_back(*j);
        }
    }
    for (j = moves.begin(); j != moves.end(); ++j) {
        if (!(ctx.miss_patterns[*j])) {
            result.push_back(*j);
        }
    }
    return result;
}
