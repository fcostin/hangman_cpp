#include "alpha_beta.h"

inline string make_key_for_game_state(const state_t & h) {
    // return string key for game state structure
    // (for caching ...)
    vector<char> guesses = vector<char>(h.guesses);
    sort(guesses.begin(), guesses.end());

    string key;
    key.push_back(h.last_guess);
    key.push_back(';');
    key.append(guesses.begin(), guesses.end());
    key.push_back(';');
    key.append(h.partial_word.begin(), h.partial_word.end());
    key.append(small_int_to_string(h.n_misses));
    return key;
}

score_t optimal_guesser_score(const context_t & ctx, cache_t & cache, const state_t & h,
        unsigned int depth, score_t alpha, score_t beta) {
    
    // check if we've already got the answer in the cache
    string h_key = make_key_for_game_state(h);
    unordered_map<string, score_t>::iterator cache_it = cache.move_cache.find(h_key);
    if (cache_it != cache.move_cache.end()) {
        return cache_it->second;
    }

    score_t node_score;

    pair<bool, score_t> term = terminal_game_state(ctx, h);
    if (term.first) {
        return term.second;
    } else {
        vector<guesser_move_t> moves = generate_guesser_moves(ctx, h, h_key, depth);
        vector<guesser_move_t>::iterator i;
        for (i = moves.begin(); i != moves.end(); i++) {
            state_t next_h = apply_guesser_move(ctx, h, *i);
            score_t score = optimal_foe_score(ctx, cache, next_h, depth - 1,
                    alpha, beta);
            alpha = (score > alpha) ? score : alpha;
            if (beta <= alpha) {
                break;
            }
        }
        node_score = alpha;
    }
    // stash the answer in the cache
    cache.move_cache[h_key] = node_score;
    return node_score;
}

score_t optimal_foe_score(const context_t & ctx, cache_t & cache, const state_t & h,
        unsigned int depth, score_t alpha, score_t beta) {
    
    // check if we've already got the answer in the cache
    string h_key = make_key_for_game_state(h);
    unordered_map<string, score_t>::iterator cache_it = cache.move_cache.find(h_key);
    if (cache_it != cache.move_cache.end()) {
        return cache_it->second;
    }

    score_t node_score;

    pair<bool, score_t> term = terminal_game_state(ctx, h);
    if (term.first) {
        return term.second;
    } else {
        vector<foe_move_t> moves = generate_foe_moves(ctx, h, h_key, depth);
        vector<foe_move_t>::iterator i;
        for (i = moves.begin(); i != moves.end(); i++) {
            state_t next_h = apply_foe_move(ctx, h, *i);
            score_t score = optimal_guesser_score(ctx, cache, next_h,
                    depth - 1, alpha, beta);
            beta = (score < beta) ? score : beta;
            if (beta <= alpha) {
                break;
            }
        }
        node_score = beta;
    }
    // stash the answer in the cache
    cache.move_cache[h_key] = node_score;
    return node_score;
}
