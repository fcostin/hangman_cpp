#include "exclusion.h"

using namespace std;

inline unsigned int letter_to_index(const char & c) {
    return (unsigned int)c - (unsigned int)'a';
}

inline char index_to_letter(const unsigned int & i) {
    return static_cast<char>(i + 'a');
}

inline void write_letter_table(vector<bool> & table,
        const unsigned int & n_words,
        const unsigned int & letter_i, const unsigned int & word_i,
        const bool value) {
    table[(letter_i * n_words) + word_i] = value;
}

inline bool read_letter_table(const vector<bool> & table,
        const unsigned int & n_words,
        const unsigned int & letter_i, const unsigned int & word_i) {
    return table[(letter_i * n_words) + word_i];
}

vector<bool> make_letter_table(const vector<string> & words) {
    /* returns table such that
     *  read_letter_table(table, n_words, letter_i, word_i) is true
     *  iff word_i contains letter ~ letter_i */
    unsigned int n_words = words.size();
    unsigned int n_letters = ALPHABET.size();
    assert(n_letters == ALPHABET_SIZE);
    vector<bool> table;
    table.resize(n_letters * n_words, false);
    for (unsigned int i = 0; i < n_words; ++i) {
        for (unsigned int j = 0; j < words[i].size(); ++j) {
            unsigned int letter_i = letter_to_index(words[i][j]);
            write_letter_table(table, n_words, letter_i, i, true);
        }
    }
    return table;
}

vector<unsigned int> make_unused_letter_indices(const vector<bool> & table,
        const unsigned int & n_words,
        const vector<char> & used_letters,
        const unordered_set<unsigned int> & word_indices) {
    /* return indices of unused letters that appear in 1 or more words */
    vector<unsigned int> unused_letter_indices;
    for (unsigned int i = 0; i < ALPHABET_SIZE; ++i) {
        if (find(used_letters.begin(), used_letters.end(), index_to_letter(i)) != used_letters.end()) {
            continue;
        }
        unordered_set<unsigned int>::const_iterator j;
        for (j = word_indices.begin(); j != word_indices.end(); ++j) {
            if (read_letter_table(table, n_words, i, *j)) {
                unused_letter_indices.push_back(i);
                break;
            }
        }
    }
    return unused_letter_indices;
}

unsigned int lower_bound_on_remaining_words(const vector<bool> & table,
        const unsigned int & n_words,
        const unordered_set<unsigned int> & word_indices,
        const vector<unsigned int> & unused_letter_indices,
        const unsigned int & lives) {
    
    vector<unsigned int> letter_counts;
    letter_counts.resize(ALPHABET_SIZE, 0);
    unordered_set<unsigned int>::const_iterator i;
    vector<unsigned int>::const_iterator j;
    for (i = word_indices.begin(); i != word_indices.end(); ++i) {
        for (j = unused_letter_indices.begin(); j != unused_letter_indices.end(); ++j) {
            if (read_letter_table(table, n_words, *j, *i)) {
                // word *i contains letter *j
                letter_counts[*j] += 1;
            }
        }
    }
    // return max(0, (sum of the d highest letter counts) - n_possible_words)
    // where d = (lives - 1). the -1 here is because if you have only 1 life
    // then you cannot make any moves without immediately losing (even if
    // your 'dying move' reveals the unique hidden word...)

    // n.b. if you have no lives left then you should have
    // already lost, right?
    assert(lives > 0);
    sort(letter_counts.begin(), letter_counts.end());
    vector<unsigned int>::const_iterator k, k_0;
    if (lives > ALPHABET_SIZE + 1) {
        k_0 = letter_counts.begin();
    } else {
        k_0 = (letter_counts.end() + 1 - lives);
    }
    unsigned int sum = 0;
    for (k = k_0; k != letter_counts.end(); ++k) {
        sum += *k;
    }
    if (word_indices.size() > sum) {
        return word_indices.size() - sum;
    } else {
        return 0;
    }
}

unsigned int upper_bound_on_remaining_words(
        const unordered_set<unsigned int> & word_indices,
        const vector<unsigned int> & unused_letter_indices,
        const context_t & ctx,
        const unsigned int & lives) {
    /*
    figure out the subset C of letters that Alice may still guess
    for each letter c in C:
        find the c-pattern which occurs the most often in the live words
        store its frequency f_c

    how to get a (reasonably tight?) upper bound on the number of
    live words remaining after alice makes d moves?

    when patterns are revealed the subset of words featuring that pattern
    is intersected onto the set of live words.

    many patterns are mutually exclusive (but we drop that constraint to
    get an upper bound)

    therefore we can take the min of the d largest frequencies f_c

    actually, we can do better:

    every time alice guesses, she reduces the number of candidate words by at least 1
    this means we need only look at the unique frequencies f_c and can pick the
    d-th largest one of those for our upper bound (if there arent d unique values
    one... then we pick the k-th one, where 1 <= k < d, and subtract (d - k) from it.)

    if this value is less than 2 then Alice wins (since that means either there
    is only one word left, or that there are 0 words left (ie Alice has demonstrated
    that Bob is cheating...)
     */


    // lpc aka 'letter_index -> pattern_index -> count (frequency)'
    vector<unordered_map<index_t, unsigned int> > lpc;
    lpc.resize(ALPHABET_SIZE);

    vector<unsigned int>::const_iterator i;
    for (i = unused_letter_indices.begin(); i != unused_letter_indices.end(); ++i) {
        unordered_set<unsigned int>::const_iterator j;
        for (j = word_indices.begin(); j != word_indices.end(); ++j) {
            index_t pattern_id = ctx.vec_letter_word_to_pattern[*i][*j];
            if (lpc[*i].count(pattern_id)) {
                lpc[*i][pattern_id] += 1;
            } else {
                lpc[*i][pattern_id] = 1;
            }
        }
    }

    // form set of (max_p lcp[c][p]) for each unused letter c
    unordered_set<unsigned int> lc_max_unique;
    for (i = unused_letter_indices.begin(); i != unused_letter_indices.end(); ++i) {
        unordered_map<index_t, unsigned int>::const_iterator j;
        unsigned int acc = 0;
        for (j = lpc[*i].begin(); j != lpc[*i].end(); ++j) {
            acc = (j->second > acc) ? j->second : acc;
        }
        if (acc > 0) {
            lc_max_unique.insert(acc);
        }
    }
    // compute the sum of the n largest elements of lc_max_unique, where
    // n = min_alice_moves = lives - 1. If there are not n largest elements
    // then sum what elements exist and subtract (n - num_elements) from
    // the result. in both cases this yields an upper bound on the maximum
    // number of words remaining (it is probably not particularly tight).
    vector<unsigned int> lc_max_sorted(lc_max_unique.begin(), lc_max_unique.end());
    sort(lc_max_sorted.begin(), lc_max_sorted.end());

    unsigned int n_unique = lc_max_sorted.size();
    assert(n_unique > 0);
    assert(lives > 0);
    unsigned int min_alice_moves = lives - 1;

    unsigned int upper_bound = 0;
    if (min_alice_moves < n_unique) {
        vector<unsigned int>::const_iterator k;
        for (k = (lc_max_sorted.end() - min_alice_moves); k != lc_max_sorted.end(); ++k) {
            upper_bound += *k;
        }
    } else {
        vector<unsigned int>::const_iterator k;
        for (k = lc_max_sorted.begin(); k != lc_max_sorted.end(); ++k) {
            upper_bound += *k;
        }
        unsigned int defect = min_alice_moves - n_unique;
        assert(upper_bound >= defect);
        upper_bound -= defect;
    }
    return upper_bound;
}
