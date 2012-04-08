#ifndef HANGMAN_CONSTANTS_H
#define HANGMAN_CONSTANTS_H

#include "math.h"
#include <string>

using namespace std;

const size_t MOVE_CACHE_SIZE = (size_t)10000000;

typedef double score_t;

const string ALPHABET("etaonrishdlfcmugypwbvkxjqz");
const size_t ALPHABET_SIZE = 26;

const score_t SCORE_GUESSER_LOSE = -INFINITY;
const score_t SCORE_GUESSER_WIN = INFINITY;
const score_t SCORE_WHATEVER = 0.0f;

#endif
