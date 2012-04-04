#ifndef HANGMAN_CONSTANTS_H
#define HANGMAN_CONSTANTS_H

#include "hangman_types.h"
#include "math.h"

const string ALPHABET("etaonrishdlfcmugypwbvkxjqz");
const unsigned int ALPHABET_SIZE = 26;

const score_t SCORE_GUESSER_LOSE = -INFINITY;
const score_t SCORE_GUESSER_WIN = INFINITY;
const score_t SCORE_WHATEVER = 0.0f;

#endif
