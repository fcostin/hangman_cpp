#ifndef HANGMAN_UTILS_H
#define HANGMAN_UTILS_H

#include "hangman_types.h"

template <class T>
inline string to_string(const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

#endif // HANGMAN_UTILS_H
