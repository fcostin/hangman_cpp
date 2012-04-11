#include "exclusion.h"

using namespace std;

inline size_t letter_to_index(const char & c) {
    return (size_t)c - (size_t)'a';
}

inline char index_to_letter(const size_t & i) {
    return static_cast<char>(i + 'a');
}

inline void write_letter_table(vector<bool> & table,
        const size_t & n_words,
        const size_t & letter_i, const size_t & word_i,
        const bool value) {
    table[(letter_i * n_words) + word_i] = value;
}

inline bool read_letter_table(const vector<bool> & table,
        const size_t & n_words,
        const size_t & letter_i, const size_t & word_i) {
    return table[(letter_i * n_words) + word_i];
}

vector<bool> make_letter_table(const vector<string> & words) {
    /* returns table such that
     *  read_letter_table(table, n_words, letter_i, word_i) is true
     *  iff word_i contains letter ~ letter_i */
    size_t n_words = words.size();
    size_t n_letters = ALPHABET.size();
    assert(n_letters == ALPHABET_SIZE);
    vector<bool> table;
    table.resize(n_letters * n_words, false);
    for (size_t i = 0; i < n_words; ++i) {
        for (size_t j = 0; j < words[i].size(); ++j) {
            size_t letter_i = letter_to_index(words[i][j]);
            write_letter_table(table, n_words, letter_i, i, true);
        }
    }
    return table;
}

vector<size_t> make_unused_letter_indices(const vector<bool> & table,
        const size_t & n_words,
        const vector<char> & used_letters,
        const vector<size_t> & word_indices) {
    /* return indices of unused letters that appear in 1 or more words */
    vector<size_t> unused_letter_indices;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (find(used_letters.begin(), used_letters.end(), index_to_letter(i)) != used_letters.end()) {
            continue;
        }
        vector<size_t>::const_iterator j;
        for (j = word_indices.begin(); j != word_indices.end(); ++j) {
            if (read_letter_table(table, n_words, i, *j)) {
                unused_letter_indices.push_back(i);
                break;
            }
        }
    }
    return unused_letter_indices;
}

size_t lower_bound_on_remaining_words(const vector<bool> & table,
        const size_t & n_words,
        const vector<size_t> & word_indices,
        const vector<size_t> & unused_letter_indices,
        const size_t & lives) {
    
    vector<size_t> letter_counts;
    letter_counts.resize(ALPHABET_SIZE, 0);
    vector<size_t>::const_iterator i;
    vector<size_t>::const_iterator j;
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
    vector<size_t>::const_iterator k, k_0;
    if (lives > ALPHABET_SIZE + 1) {
        k_0 = letter_counts.begin();
    } else {
        k_0 = (letter_counts.end() + 1 - lives);
    }
    size_t sum = 0;
    for (k = k_0; k != letter_counts.end(); ++k) {
        sum += *k;
    }
    if (word_indices.size() > sum) {
        return word_indices.size() - sum;
    } else {
        return 0;
    }
}

size_t upper_bound_on_remaining_words(
        const vector<size_t> & word_indices,
        const vector<size_t> & unused_letter_indices,
        const context_t & ctx,
        const size_t & lives) {

    vector<size_t>::const_iterator i;
    vector<size_t>::const_iterator j;
    vector<pair<size_t, size_t> >::const_iterator k;

    vector<size_t> patterns;
    patterns.reserve(word_indices.size());
    vector<pair<size_t, size_t> > pattern_counts;
    pattern_counts.reserve(unused_letter_indices.size());

    size_t d = lives - 1;
    // Alice trivially wins if she has as least as many moves as guesses.
    // We assume we've already checked for that prior to calling
    // this expensive routine ...
    assert(d < unused_letter_indices.size());

    // Alice chooses d letters to guess via heuristic: for each letter,
    // max patterns for that letter over word freq and store the max freq.
    // then pick the d letters with minimal max freq.
    vector<pair<size_t, size_t> > letter_pattern_counts;
    letter_pattern_counts.reserve(unused_letter_indices.size());
    for (i = unused_letter_indices.begin(); i != unused_letter_indices.end(); ++i) {
        // map words to pattern id wrt chosen letter
        for (j = word_indices.begin(); j != word_indices.end(); ++j) {
            patterns.push_back(ctx.vec_letter_word_to_pattern[*i][*j]);
        }
        // compute vector of pattern counts
        pattern_counts = count_elements(patterns);
        // compute max pattern count
        size_t acc = 0;
        for (k = pattern_counts.begin(); k != pattern_counts.end(); ++k) {
            acc = (k->second > acc) ? k->second : acc;
        }
        // store it
        letter_pattern_counts.push_back(make_pair(*i, acc));
        // clean working data stuctures for next letter
        patterns.clear();
        pattern_counts.clear();
    }
    sort(letter_pattern_counts.begin(), letter_pattern_counts.end(),
        compare_by_second<pair<size_t, size_t> >);

    // XXX FIXME Heuristic: Alice chooses the first d letters with the minimal maximal pattern
    // frequencies -- can this be improved without much more compute?
    vector<size_t> letters;
    size_t n_patterns = (size_t)1;
    for(k = letter_pattern_counts.begin(); k != (letter_pattern_counts.begin() + d); ++k) {
        letters.push_back(k->first);
        n_patterns *= k->second;
    }

    // now we compute what Bob's optimal moves are by mapping each live word to its
    // length-d "fingerprint" of pattern ids wrt the chosen d letters. Then we
    // count how many occurences there are of each combination of pattern ids and
    // pick the biggest class. This is the exact number of words remaining wrt Alice's
    // fixed heuristic strategy. if the number is lower than 2 then Bob loses.
    
    vector<size_t> fingerprint;
    fingerprint.reserve(letters.size());
    vector<vector<size_t> > fingerprints;
    fingerprints.reserve(word_indices.size());
    for (j = word_indices.begin(); j != word_indices.end(); ++j) {
        for (i = letters.begin(); i != letters.end(); ++i) {
            fingerprint.push_back(ctx.vec_letter_word_to_pattern[*i][*j]);
        }
        fingerprints.push_back(fingerprint);
        fingerprint.clear();
    }
    // XXX TODO : we can do better here - we dont have to count all the elements and
    // report the largest count, if any element count is >= 2 we can immediately
    // return that instead, as that proves that a win for Alice is impossible with
    // respect to her heuristic strategy.
    vector<pair<vector<size_t>, size_t> > fingerprint_counts = count_elements(fingerprints);
    pair<vector<size_t>, size_t> max_fingerprint = *max_element(
            fingerprint_counts.begin(), fingerprint_counts.end(),
            compare_by_second<pair<vector<size_t>, size_t> >);
    return max_fingerprint.second;
}
