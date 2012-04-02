#include "alpha_beta.h"

string make_key_for_game_state(const state_t & h) {
    // return string key for game state structure
    // (for caching ...)
    vector<char> guesses = vector<char>(h.guesses);
    sort(guesses.begin(), guesses.end());

    string key;
    key.append(guesses.begin(), guesses.end());
    key.push_back(';');
    key.append(h.partial_word.begin(), h.partial_word.end());
    key.push_back(';');
    key.append(to_string(h.n_misses));
    return key;
}

bool is_terminal_game_state(const context_t & ctx, const state_t & h) {
    if (h.n_misses >= ctx.n_misses_for_loss) {
        return true;
    } else {
        assert(h.live_word_indices.size() > 0);
        return h.live_word_indices.size() == 1;
    }
}

void spam_debug_message(const string & who, unsigned int indent,
        score_t alpha, score_t beta, const vector<char> & partial_word) {
    for (unsigned int i = 0; i < indent; ++i) {
        cout << " ";
    }
    vector<char>::const_iterator j;
    for (j = partial_word.begin(); j != partial_word.end(); ++j) {
        cout << *j;
    }
    cout << "; player = " << who << "; alpha = " << alpha << "; beta = " << beta << endl;
}

score_t optimal_guesser_score(const context_t & ctx, cache_t & cache, const state_t & h,
        unsigned int depth, score_t alpha, score_t beta) {
    
    // check if we've already got the answer in the cache
    string h_key = make_key_for_game_state(h);
    map<string, score_t>::iterator cache_it = cache.move_cache.find(h_key);
    if (cache_it != cache.move_cache.end()) {
        return cache_it->second;
    }

    // spam_debug_message("G", ctx.max_depth - depth, alpha, beta, h.partial_word);

    score_t node_score;

    if ((depth == 0) or is_terminal_game_state(ctx, h)) {
        node_score = evaluate_game_state(ctx, h);
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
    map<string, score_t>::iterator cache_it = cache.move_cache.find(h_key);
    if (cache_it != cache.move_cache.end()) {
        return cache_it->second;
    }

    // spam_debug_message("F", ctx.max_depth - depth, alpha, beta, h.partial_word);

    score_t node_score;

    if ((depth == 0) or is_terminal_game_state(ctx, h)) {
        node_score = evaluate_game_state(ctx, h);
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
