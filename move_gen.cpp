#include "move_gen.h"

template <typename T>
bool compare_by_second(const T & a, const T & b) {
    return (a.second < b.second);
}

unordered_map<size_t, size_t> make_pattern_counts(const context_t & ctx,
        const unordered_set<size_t> & word_indices, const size_t & c_index) {
    unordered_map<size_t, size_t> pattern_counts;
    unordered_set<size_t>::const_iterator j;
    for (j = word_indices.begin(); j != word_indices.end(); ++j) {
        size_t pattern_id = ctx.vec_letter_word_to_pattern[c_index][*j];
        if (pattern_counts.count(pattern_id)) {
            pattern_counts[pattern_id] += 1;
        } else {
            pattern_counts[pattern_id] = 1;
        }
    }
    return pattern_counts;
}

pair<size_t, size_t> most_common_pattern(
        const unordered_map<size_t, size_t> & pattern_counts) {
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

    // -- figure out which letters are still useful options
    vector<size_t> unused_letter_indices = make_unused_letter_indices(
            ctx.letter_table, ctx.words.size(), h.guesses, h.live_word_indices);
    assert(unused_letter_indices.size() <= ALPHABET_SIZE - h.guesses.size());

    // -- compute Bob's maximum number of live words for each letter
    unordered_map<size_t, size_t> letter_max_pattern_counts;
    vector<size_t>::const_iterator i;
    for(i = unused_letter_indices.begin(); i != unused_letter_indices.end(); ++i) {
        unordered_map<size_t, size_t> pattern_counts = make_pattern_counts(ctx,
                h.live_word_indices, *i);
        letter_max_pattern_counts[*i] = most_common_pattern(pattern_counts).second;
    }
    // -- reel off our guesses in increasing order of max live words
    vector<pair<size_t, size_t> > items(letter_max_pattern_counts.begin(),
            letter_max_pattern_counts.end());
    sort(items.begin(), items.end(), compare_by_second<pair<size_t, size_t> >);
    vector<char> moves;
    vector<pair<size_t, size_t> >::const_iterator j;
    for (j = items.begin(); j != items.end(); ++j) {
        char move_c = static_cast<char>(j->first + 'a');
        moves.push_back(move_c);
    }
    return moves;
}

vector<size_t> generate_foe_moves(
        const context_t & ctx,
        const state_t & h,
        const string & h_key __attribute__((unused)),
        size_t depth __attribute__((unused))) {
    // count occurances of patterns for guessed letter in set of live words
    size_t c_index = (size_t)(h.last_guess - 'a');
    unordered_map<size_t, size_t> pattern_counts = make_pattern_counts(ctx,
            h.live_word_indices, c_index);
    // order patterns by frequency
    vector<pair<size_t, size_t> > items(pattern_counts.begin(),
            pattern_counts.end());
    sort(items.begin(), items.end(), compare_by_second<pair<size_t, size_t> >);
    // try patterns in decreasing order of frequency
    vector<size_t> moves;
    vector<pair<size_t, size_t> >::const_reverse_iterator i;
    for (i = items.rbegin(); i != items.rend(); ++i) {
        moves.push_back(i->first);
    }
    // BUT! always try the miss move first
    vector<size_t>::iterator j;
    for (j = moves.begin(); j != moves.end(); ++j) {
        if (ctx.miss_patterns[*j]) {
            swap(moves.front(), *j);
            break;
        }
    }
    return moves;
}
