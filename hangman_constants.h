#ifndef HANGMAN_CONSTANTS_H
#define HANGMAN_CONSTANTS_H

#include "math.h"
#include <string>

using namespace std;

typedef unsigned int index_t;
typedef double score_t;
typedef char guesser_move_t;
typedef index_t foe_move_t;

const string ALPHABET("etaonrishdlfcmugypwbvkxjqz");
const unsigned int ALPHABET_SIZE = 26;

const score_t SCORE_GUESSER_LOSE = -INFINITY;
const score_t SCORE_GUESSER_WIN = INFINITY;
const score_t SCORE_WHATEVER = 0.0f;

#endif
