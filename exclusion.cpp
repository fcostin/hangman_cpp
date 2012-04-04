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
        if (find(used_letters.begin(), used_letters.end(), ALPHABET[i]) != used_letters.end()) {
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
