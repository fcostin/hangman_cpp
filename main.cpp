#include "stdlib.h"
#include "string.h"
#include "stdio.h"

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
    fprintf(stderr, "usage: [--debug | --debug-summary] word_length n_misses_for_loss\n");
    exit(1);
}

void debug_printf_vector(const vector<size_t> & v) {
    vector<size_t>::const_reverse_iterator i;
    for(i = v.rbegin(); i != v.rend(); ++i) {
        DEBUG_SUMMARY_PRINTF("%lu", *i);
        if (i + 1 != v.rend()) {
            DEBUG_SUMMARY_PRINTF(",");
        }
    }
    DEBUG_SUMMARY_PRINTF("\n");
}

void debug_printf_cache(const cache_t & cache) {
    DEBUG_SUMMARY_PRINTF("-------------\n");
    DEBUG_SUMMARY_PRINTF("Cache summary\n");
    DEBUG_SUMMARY_PRINTF("-------------\n");
    DEBUG_SUMMARY_PRINTF("\tmove_cache size %lu \n", cache.move_cache.size());
    DEBUG_SUMMARY_PRINTF("\tstat_not_terminal ");
    debug_printf_vector(cache.stat_not_terminal);
    DEBUG_SUMMARY_PRINTF("\tstat_base_loss ");
    debug_printf_vector(cache.stat_base_loss);
    DEBUG_SUMMARY_PRINTF("\tstat_base_win ");
    debug_printf_vector(cache.stat_base_win);
    DEBUG_SUMMARY_PRINTF("\tstat_upper_bound_cheap ");
    debug_printf_vector(cache.stat_upper_bound_cheap);
    DEBUG_SUMMARY_PRINTF("\tstat_lower_bound_expensive ");
    debug_printf_vector(cache.stat_lower_bound_expensive);
    DEBUG_SUMMARY_PRINTF("\tstat_upper_bound_expensive ");
    debug_printf_vector(cache.stat_upper_bound_expensive);
}

int main(int n_args, char ** args) {
    size_t word_length = 0;
    size_t n_misses_for_loss = 0;
    // parse command line args
    if ((n_args != 3) && (n_args != 4)) {
        print_usage_and_die();
    }
    if (n_args == 3) {
        HANGMAN_DEBUG_FLAG = HANGMAN_DEBUG_LEVEL_DISABLED;
        stringstream word_length_arg(args[1]);
        stringstream n_misses_for_loss_arg(args[2]);
        word_length_arg >> word_length;
        n_misses_for_loss_arg >> n_misses_for_loss;
    } else {
        if (strcmp(args[1], "--debug") == 0) {
            HANGMAN_DEBUG_FLAG = HANGMAN_DEBUG_LEVEL_ALL;
        } else if (strcmp(args[1], "--debug-summary") == 0) {
            HANGMAN_DEBUG_FLAG = HANGMAN_DEBUG_LEVEL_SUMMARY;
        } else {
            print_usage_and_die();
        }
        stringstream word_length_arg(args[2]);
        stringstream n_misses_for_loss_arg(args[3]);
        word_length_arg >> word_length;
        n_misses_for_loss_arg >> n_misses_for_loss;
    }

    printf("# got word_length = %lu\n", word_length);
    printf("# got n_misses_for_loss = %lu\n", n_misses_for_loss);

    // set up context (dictionary of words and derived constant structures)
    size_t max_depth = 2 * ALPHABET.size(); // full search
    printf("#  loading context...\n");
    context_t ctx = make_hangman_context("words.txt", word_length,
            n_misses_for_loss, max_depth);
    printf("# loaded context, got %lu words and %lu patterns\n",
            ctx.words.size(), ctx.patterns.size());
    if (!ctx.words.size()) {
        fprintf(stderr, "There are no words there! Aborting.\n");
        return 1;
    }
    
    // fire up the minimax search
    state_t h_zero = make_initial_state(make_all_word_indices(ctx), word_length);
    cache_t cache;
    printf("# searching...\n");
    score_t outcome = optimal_guesser_score(ctx, cache, h_zero,
            ctx.max_depth, SCORE_GUESSER_LOSE, SCORE_GUESSER_WIN);
    printf("@ outcome: %f\n", outcome);
    debug_printf_cache(cache);
    return 0;
}
