#include "move_gen.h"

vector<pair<size_t, size_t> > make_pattern_counts(const context_t & ctx,
        const vector<size_t> & word_indices, const size_t & c_index) {

    // pattern_ids = map(f, word_indices)
    //      where f = lambda x : ctx.vec_letter_word_to_pattern[c_index][x]
    vector<size_t> pattern_ids;
    pattern_ids.reserve(word_indices.size());
    vector<size_t>::const_iterator j;
    for (j = word_indices.begin(); j != word_indices.end(); ++j) {
        pattern_ids.push_back(ctx.vec_letter_word_to_pattern[c_index][*j]);
    }
    return count_elements(pattern_ids);
}

pair<size_t, size_t> most_common_pattern(
        const vector<pair<size_t, size_t> > & pattern_counts) {
    return *max_element(pattern_counts.begin(), pattern_counts.end(),
            compare_by_second<pair<size_t, size_t> >);
}

vector<char> generate_guesser_moves(
        const context_t & ctx,
        const state_t & h,
        const string & h_key __attribute__((unused)),
        size_t depth __attribute__((unused))) {
    // Heuristic: assume Bob is trying to maximise the number of live words,
    // and play to minimise what he's doing (minimise the maximum number of
    // live words).
    DEBUG_PRINTF("$MOVEGEN Alice begin\n");

    // -- figure out which letters are still useful options
    vector<size_t> unused_letter_indices = make_unused_letter_indices(
            ctx.letter_table, ctx.words.size(), h.guesses, h.live_word_indices);
    assert(unused_letter_indices.size() <= ALPHABET_SIZE - h.guesses.size());

    // -- compute Bob's maximum number of live words for each letter
    vector<pair<size_t, size_t> > items;
    vector<size_t>::const_iterator i;
    for(i = unused_letter_indices.begin(); i != unused_letter_indices.end(); ++i) {
        vector<pair<size_t, size_t> > pattern_counts = make_pattern_counts(ctx,
                h.live_word_indices, *i);
        items.push_back(make_pair(*i, most_common_pattern(pattern_counts).second));
    }
    // -- reel off our guesses in increasing order of max live words
    sort(items.begin(), items.end(), compare_by_second<pair<size_t, size_t> >);
    vector<char> moves;
    vector<pair<size_t, size_t> >::const_iterator j;
    for (j = items.begin(); j != items.end(); ++j) {
        char move_c = static_cast<char>(j->first + 'a');
        DEBUG_PRINTF("$MOVEGEN Alice %c with weight %lu\n", move_c, j->second);
        moves.push_back(move_c);
    }
    DEBUG_PRINTF("$MOVEGEN Alice end\n");
    return moves;
}

vector<size_t> generate_foe_moves(
        const context_t & ctx,
        const state_t & h,
        const string & h_key __attribute__((unused)),
        size_t depth __attribute__((unused))) {
    DEBUG_PRINTF("$MOVEGEN Bob begin\n");
    // count occurances of patterns for guessed letter in set of live words
    size_t c_index = (size_t)(h.last_guess - 'a');
    // order patterns by frequency
    vector<pair<size_t, size_t> > items = make_pattern_counts(ctx,
            h.live_word_indices, c_index);
    sort(items.begin(), items.end(), compare_by_second<pair<size_t, size_t> >);
    // try patterns in decreasing order of frequency
    vector<size_t> moves;
    vector<pair<size_t, size_t> >::const_reverse_iterator i;
    for (i = items.rbegin(); i != items.rend(); ++i) {
        moves.push_back(i->first);
        if (ctx.miss_patterns[i->first]) {
            DEBUG_PRINTF("$MOVEGEN Bob %lu with weight %lu *** miss move\n", i->first, i->second);
        } else {
            DEBUG_PRINTF("$MOVEGEN Bob %lu with weight %lu\n", i->first, i->second);
        }
    }
    // BUT! always try the miss move first
    vector<size_t>::iterator j;
    for (j = moves.begin(); j != moves.end(); ++j) {
        if (ctx.miss_patterns[*j]) {
            swap(moves.front(), *j);
            break;
        }
    }
    DEBUG_PRINTF("$MOVEGEN Bob end\n");
    return moves;
}
