#include "hangman_types.h"
#include "hangman_constants.h"
#include "hangman_context.h"
#include "hangman_utils.h"
#include "alpha_beta.h"

#include <iostream>

state_t make_initial_state(const unordered_set<index_t> & live_word_indices,
        unsigned int word_length) {
    vector<char> guesses;
    int n_misses = 0;
    vector<char> partial_word;
    while (word_length-- > 0) {
        partial_word.push_back('_');
    }
    state_t h(guesses, n_misses, live_word_indices, partial_word, (char)0);
    return h;
}

unordered_set<index_t> make_all_word_indices(const context_t & context) {
    index_t i = 0;
    vector<string>::const_iterator it;
    unordered_set<index_t> result;
    for (it = context.words.begin(); it != context.words.end(); ++it) {
        result.insert(i++);
    }
    return result;
}

int main(int n_args, char ** args) {
    if (n_args != 3) {
        cout << "usage: word_length n_misses_for_loss" << endl;
        return 1;
    }
    unsigned int word_length = 0;
    unsigned int n_misses_for_loss = 0;
    stringstream word_length_arg(args[1]);
    word_length_arg >> word_length;
    stringstream n_misses_for_loss_arg(args[2]);
    n_misses_for_loss_arg >> n_misses_for_loss;
    cout << "# got word_length := " << word_length << endl;
    cout << "# got n_misses_for_loss := " << n_misses_for_loss << endl;

    unsigned int max_depth = 2 * ALPHABET.size(); // full search

    cout << "# loading context..." << endl;
    context_t ctx = make_hangman_context("words.txt", word_length,
            n_misses_for_loss, max_depth);
    cout << "# loaded context, got " << ctx.words.size() << " words, and " <<
        ctx.patterns.size() << " patterns." << endl;

    state_t h_zero = make_initial_state(make_all_word_indices(ctx), word_length);
    cache_t cache;
    cout << "# searching.." << endl;
    score_t outcome = optimal_guesser_score(ctx, cache, h_zero,
            ctx.max_depth, SCORE_GUESSER_LOSE, SCORE_GUESSER_WIN);
    cout << "@ outcome: " << outcome << endl;
    cache.dump_summary(cout);
    return 0;
}
