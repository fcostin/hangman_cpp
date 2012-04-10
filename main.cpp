#include "stdlib.h"
#include "string.h"
#include <iostream>

#include "hangman_types.h"
#include "hangman_constants.h"
#include "hangman_context.h"
#include "hangman_utils.h"
#include "hangman_debug.h"
#include "alpha_beta.h"


state_t make_initial_state(const vector<size_t> & live_word_indices,
        size_t word_length) {
    vector<char> guesses;
    int n_misses = 0;
    vector<char> partial_word;
    while (word_length-- > 0) {
        partial_word.push_back('_');
    }
    state_t h(guesses, n_misses, live_word_indices, partial_word, (char)0);
    return h;
}

vector<size_t> make_all_word_indices(const context_t & context) {
    size_t i = 0;
    vector<string>::const_iterator it;
    vector<size_t> result;
    for (it = context.words.begin(); it != context.words.end(); ++it) {
        result.push_back(i++);
    }
    return result;
}

void print_usage_and_die() {
    cerr << "usage: [--debug] word_length n_misses_for_loss" << endl;
    exit(1);
}

int main(int n_args, char ** args) {
    size_t word_length = 0;
    size_t n_misses_for_loss = 0;
    // parse command line args
    if ((n_args != 3) && (n_args != 4)) {
        print_usage_and_die();
    }
    if (n_args == 3) {
        HANGMAN_DEBUG_FLAG = 0;
        stringstream word_length_arg(args[1]);
        stringstream n_misses_for_loss_arg(args[2]);
        word_length_arg >> word_length;
        n_misses_for_loss_arg >> n_misses_for_loss;
    } else if(strcmp(args[1], "--debug")) {
        print_usage_and_die();
    } else {
        HANGMAN_DEBUG_FLAG = 1;
        stringstream word_length_arg(args[2]);
        stringstream n_misses_for_loss_arg(args[3]);
        word_length_arg >> word_length;
        n_misses_for_loss_arg >> n_misses_for_loss;
    }

    cout << "# got word_length := " << word_length << endl;
    cout << "# got n_misses_for_loss := " << n_misses_for_loss << endl;

    // set up context (dictionary of words and derived constant structures)
    size_t max_depth = 2 * ALPHABET.size(); // full search
    cout << "# loading context..." << endl;
    context_t ctx = make_hangman_context("words.txt", word_length,
            n_misses_for_loss, max_depth);
    cout << "# loaded context, got " << ctx.words.size() << " words, and " <<
        ctx.patterns.size() << " patterns." << endl;
    if (!ctx.words.size()) {
        cerr << "There are no words there! Aborting." << endl;
        return 1;
    }
    
    // fire up the minimax search
    state_t h_zero = make_initial_state(make_all_word_indices(ctx), word_length);
    cache_t cache;
    cout << "# searching.." << endl;
    score_t outcome = optimal_guesser_score(ctx, cache, h_zero,
            ctx.max_depth, SCORE_GUESSER_LOSE, SCORE_GUESSER_WIN);
    cout << "@ outcome: " << outcome << endl;
    DEBUG(cache.dump_summary(cout));
    return 0;
}
