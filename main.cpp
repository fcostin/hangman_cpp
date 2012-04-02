#include "hangman_types.h"
#include "hangman_constants.h"
#include "hangman_context.h"
#include "hangman_utils.h"
#include "alpha_beta.h"

#include <iostream>

state_t make_initial_state(const set<index_t> & live_word_indices,
        unsigned int word_length) {
    vector<char> guesses;
    int n_misses = 0;
    vector<char> partial_word;
    while (word_length-- > 0) {
        partial_word.push_back('_');
    }
    state_t h(guesses, n_misses, live_word_indices, partial_word);
    return h;
}

set<index_t> make_all_word_indices(const context_t & context) {
    index_t i = 0;
    vector<string>::const_iterator it;
    set<index_t> result;
    for (it = context.words.begin(); it != context.words.end(); ++it) {
        result.insert(i++);
    }
    return result;
}

cache_t make_cache() {
    map<string, score_t> move_cache;
    cache_t result(move_cache);
    return result;
}

int main() {
    unsigned int word_length = 10;
    unsigned int n_misses_for_loss = 8;

    cout << "loading context..." << endl;
    context_t ctx = make_hangman_context("words.txt", word_length, n_misses_for_loss);
    cout << "loaded context" << endl;

    state_t h_zero = make_initial_state(make_all_word_indices(ctx), word_length);

    cache_t cache = make_cache();

    unsigned int max_depth = ALPHABET.size(); // full search

    cout << "searching.." << endl;

    score_t outcome = optimal_guesser_score(ctx, cache, h_zero,
            max_depth, SCORE_GUESSER_LOSE, SCORE_GUESSER_WIN);

    cout << "outcome: " << outcome << endl;

    return 0;
}
