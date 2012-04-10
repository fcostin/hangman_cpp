#ifndef HANGMAN_ALGORITHM_H
#define HANGMAN_ALGORITHM_H

#include <algorithm>

template <typename T>
void sort_and_remove_nonunique_elements(T & t) {
    std::sort(t.begin(), t.end());
    t.resize(std::unique(t.begin(), t.end()) - t.begin());
}

#endif /* HANGMAN_ALGORITHM_H */
