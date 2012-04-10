#ifndef HANGMAN_ALGORITHM_H
#define HANGMAN_ALGORITHM_H

#include <algorithm>
#include <vector>

template <typename T>
void sort_and_remove_nonunique_elements(T & t) {
    std::sort(t.begin(), t.end());
    t.resize(std::unique(t.begin(), t.end()) - t.begin());
}

template <typename T>
std::vector<std::pair<T, size_t> > count_elements(std::vector<T> & a) {
    std::vector<std::pair<T, size_t> > counts;
    std::pair<T, size_t> accumulator;
    typename vector<T>::iterator i;
    bool on_run = false;

    std::sort(a.begin(), a.end());

    for(i = a.begin(); i != a.end(); ++i) {
        if (on_run) {
            if (accumulator.first == *i) {
                ++accumulator.second;
            } else {
                on_run = false;
                counts.push_back(accumulator);
            }
        } else {
            accumulator.first = *i;
            accumulator.second = (size_t)1;
            on_run = true;
        }
    }
    if (on_run) {
        counts.push_back(accumulator);
    }
    return counts;
}

#endif /* HANGMAN_ALGORITHM_H */
