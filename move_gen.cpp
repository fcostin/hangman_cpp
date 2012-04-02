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
    set<foe_move_t> moves;
    char c = h.guesses.back();
    set<index_t>::const_iterator i;
    for (i = h.live_word_indices.begin(); i != h.live_word_indices.end(); ++i) {
        map<pair<char, index_t>, index_t>::const_iterator it;
        it = ctx.letter_word_to_pattern.find(make_pair(c, *i));
        assert(it != ctx.letter_word_to_pattern.end());
        index_t word_pattern_index = it->second;
        moves.insert(word_pattern_index);
    }
    vector<foe_move_t> result(moves.begin(), moves.end());
    return result;
}
