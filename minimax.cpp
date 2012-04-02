#include "assert.h"
#include <vector>
#include <set>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream> // only needed for printing output...

using namespace std;

typedef int score_t;
typedef unsigned int index_t;

/* CONSTANT GLOBAL VARIABLES */

const score_t SCORE_GUESSER_LOSE = -1;
const score_t SCORE_GUESSER_WIN = 1;
const int N_MISSES_FOR_LOSS = 8;
const int WORD_LENGTH = 10;
const string ALPHABET("etaonrishdlfcmugypwbvkxjqz");

/* MUTABLE GLOBAL VARIABLES */
map<string, score_t> GAME_MOVE_CACHE;
map<index_t, string> WORDS;

template <class T>
inline string to_string(const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

score_t best_for_guesser(score_t a, score_t b) {
    return (a > b) ? a : b;
}

score_t best_for_foe(score_t a, score_t b) {
    return (a > b) ? b : a;
}

class GameState {
public:
    vector<char> guesses;
    int n_misses;
    set<index_t> live_word_indices;
    vector<char> partial_word;

    GameState(const vector<char> & guesses, int n_misses,
            const set<index_t> & live_word_indices, const vector<char> & partial_word) {
        this->guesses = guesses;
        this->n_misses = n_misses;
        this->live_word_indices = live_word_indices;
        this->partial_word = partial_word;
    }
};

class Pattern {
public:
    char c;
    bool mask[WORD_LENGTH];
};

typedef Pattern pattern_t;



string make_key_for_game_state(const GameState & h) {
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

vector<char> gen_guesser_letters(const GameState & h) {
    // XXX do better than this
    vector<char> letters;
    string::const_iterator i;
    for (i = ALPHABET.begin(); i != ALPHABET.end(); i++) {
        if (find(h.guesses.begin(), h.guesses.end(), *i) != h.guesses.end()) {
            letters.push_back(*i);
        }
    }
    return letters;
}

GameState update_game_state_for_guess(const GameState & h, char c) {
    vector<char> next_guesses(h.guesses);
    next_guesses.push_back(c);
    GameState h_next(next_guesses, h.n_misses, h.live_word_indices,
            h.partial_word);
    return h_next;
}

bool is_terminal_game_state(const GameState & h) {
    if (h.n_misses >= N_MISSES_FOR_LOSS) {
        return true;
    } else {
        assert(h.live_word_indices.size() > 0);
        return h.live_word_indices.size() == 1;
    }
}

score_t terminal_score(const GameState & h) {
    if (h.n_misses >= N_MISSES_FOR_LOSS) {
        return SCORE_GUESSER_LOSE;
    } else {
        return SCORE_GUESSER_WIN;
    }
}

// forward declare these two since they are mutually recursive
score_t optimal_guesser_score(const GameState & h);
score_t optimal_foe_score(const GameState & h);

score_t optimal_guesser_score(const GameState & h) {
    // compute the best game outcome for guesser from this game state
    // (mutually recursive with optimal_foe_score)

    // check if we've already got the answer in the cache
    string h_key = make_key_for_game_state(h);
    map<string, score_t>::iterator cache_it = GAME_MOVE_CACHE.find(h_key);
    if (cache_it != GAME_MOVE_CACHE.end()) {
        return cache_it->second;
    }

    // try all possible moves and pick the best one
    score_t best_score = SCORE_GUESSER_LOSE;
    vector<char> letters = gen_guesser_letters(h);
    vector<char>::iterator i;
    for (i = letters.begin(); i != letters.end(); i++) {
        if (best_score == SCORE_GUESSER_WIN) {
            break;
        }
        GameState next_h = update_game_state_for_guess(h, *i);
        score_t score;
        if (is_terminal_game_state(next_h)) {
            score = terminal_score(next_h);    
        } else {
            score = optimal_foe_score(next_h);
        }
        best_score = best_for_guesser(score, best_score);
    }

    // stash the answer in the cache
    GAME_MOVE_CACHE[h_key] = best_score;
    return best_score;
}

vector<pattern_t> gen_foe_patterns(const GameState & h) {
    vector<pattern_t> result;
    // XXX TODO fairly complex logic in here
    return result;
}

GameState update_game_state_for_pattern(const GameState & h, const pattern_t & p) {

    vector<char> guesses;
    int n_misses;
    set<index_t> live_word_indices;
    vector<char> partial_word;

    int next_n_misses = h.n_misses + IS_MISS_PATTERN[p];

    set<index_t> next_live_word_indices =
            update_live_word_indices(h.live_word_indices, p);
    vector<char> next_partial_word = update_partial_word(h.partial_word, p);

    GameState result(h.guesses, next_n_misses, next_live_word_indices,
            next_partial_word);

    return result;
}

score_t optimal_foe_score(const GameState & h) {
    // compute the best game outcome for guesser from this game state
    // (mutually recursive with optimal_foe_score)

    // check if we've already got the answer in the cache
    string h_key = make_key_for_game_state(h);
    map<string, score_t>::iterator cache_it = GAME_MOVE_CACHE.find(h_key);
    if (cache_it != GAME_MOVE_CACHE.end()) {
        return cache_it->second;
    }

    // try all possible moves and pick the best one
    score_t best_score = SCORE_GUESSER_WIN;
    vector<pattern_t> patterns = gen_foe_patterns(h);
    vector<pattern_t>::iterator i;
    for (i = patterns.begin(); i != patterns.end(); i++) {
        if (best_score == SCORE_GUESSER_LOSE) {
            break;
        }
        GameState next_h = update_game_state_for_pattern(h, *i);
        score_t score;
        if (is_terminal_game_state(next_h)) {
            score = terminal_score(next_h);    
        } else {
            score = optimal_guesser_score(next_h);
        }
        best_score = best_for_foe(score, best_score);
    }

    // stash the answer in the cache
    GAME_MOVE_CACHE[h_key] = best_score;
    return best_score;
}



void test_print_key_game_state() {
    
    // test we can construct a game state and print its key
    vector<char> guesses;
    guesses.push_back('x');
    guesses.push_back('a');
    guesses.push_back('b');
    set<index_t> live_word_indices;
    vector<char> partial_word;
    partial_word.push_back('b');
    partial_word.push_back('a');
    partial_word.push_back('_');
    int n_misses = 99;
    GameState h(guesses, n_misses, live_word_indices, partial_word);

    cout << make_key_for_game_state(h) << endl;
}

int main() {
    test_print_key_game_state();
    return 0;
}
