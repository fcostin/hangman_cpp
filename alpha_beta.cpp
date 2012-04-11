#include "alpha_beta.h"

inline string make_key_for_game_state(const state_t & h) {
    // return string key for game state structure
    // (for caching ...)
    vector<char> guesses = vector<char>(h.guesses);
    sort(guesses.begin(), guesses.end());

    string key;
    if (h.last_guess != (char)0) {
        key.push_back(h.last_guess);
    }
    key.push_back('|');
    key.append(guesses.begin(), guesses.end());
    key.push_back(';');
    key.append(h.partial_word.begin(), h.partial_word.end());
    key.append(small_int_to_string(h.n_misses));
    return key;
}

score_t optimal_guesser_score(const context_t & ctx, cache_t & cache, const state_t & h,
        size_t depth, score_t alpha, score_t beta, bool *abort_flag) {

    if (*abort_flag) {
        return SCORE_WHATEVER;
    }
    
    // check if we've already got the answer in the cache
    string h_key = make_key_for_game_state(h);
    lru_cache_t<string, score_t>::item_list_t::const_iterator cache_it = cache.move_cache.find(h_key);
    if (cache_it != cache.move_cache.end()) {
        DEBUG_PRINTF("$CACHE HIT %s %f\n", h_key.c_str(), cache_it->second);
        return cache_it->second;
    }
    DEBUG_PRINTF("$CACHE MISS %s\n", h_key.c_str());

    score_t node_score;

    pair<eval_result_t, score_t> term = terminal_game_state(ctx, h);
    cache.log_terminal_result(term.first, depth);
    if (term.first != EVAL_RESULT_NOT_TERMINAL) {
        node_score = term.second;
    } else {
        vector<char> moves = generate_guesser_moves(ctx, h, h_key, depth);
        vector<char>::iterator i;
        for (i = moves.begin(); i != moves.end(); i++) {
            state_t next_h = apply_guesser_move(ctx, h, *i);
            score_t score = optimal_foe_score(ctx, cache, next_h, depth - 1,
                    alpha, beta, abort_flag);
            alpha = (score > alpha) ? score : alpha;
            if (beta <= alpha) {
                break;
            }
        }
        node_score = alpha;
    }

    // stash the answer in the cache
    cache.move_cache.insert(h_key, node_score);
    // logging ...
    DEBUG_PRINTF("$CACHE STORE %s %f\n", h_key.c_str(), node_score);
    cache.log_evaluation_result(node_score, depth);
    return node_score;
}

score_t optimal_foe_score(const context_t & ctx, cache_t & cache, const state_t & h,
        size_t depth, score_t alpha, score_t beta, bool *abort_flag) {

    if (*abort_flag) {
        return SCORE_WHATEVER;
    }
    
    string h_key = make_key_for_game_state(h);

    score_t node_score;

    pair<eval_result_t, score_t> term = terminal_game_state(ctx, h);
    cache.log_terminal_result(term.first, depth);
    if (term.first != EVAL_RESULT_NOT_TERMINAL) {
        node_score = term.second;
    } else {
        vector<size_t> moves = generate_foe_moves(ctx, h, h_key, depth);
        vector<size_t>::iterator i;
        for (i = moves.begin(); i != moves.end(); i++) {
            state_t next_h = apply_foe_move(ctx, h, *i);
            score_t score = optimal_guesser_score(ctx, cache, next_h,
                    depth - 1, alpha, beta, abort_flag);
            beta = (score < beta) ? score : beta;
            if (beta <= alpha) {
                break;
            }
        }
        node_score = beta;
    }

    cache.log_evaluation_result(node_score, depth);
    
    return node_score;
}
