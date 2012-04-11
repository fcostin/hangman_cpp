#ifndef HANGMAN_DEBUG_H
#define HANGMAN_DEBUG_H

#include "stdio.h"

extern int HANGMAN_DEBUG_FLAG;

#define HANGMAN_DEBUG_LEVEL_DISABLED 0
#define HANGMAN_DEBUG_LEVEL_SUMMARY 1
#define HANGMAN_DEBUG_LEVEL_ALL 2

#define DEBUG_PRINTF(...) \
            do { if (HANGMAN_DEBUG_FLAG >= HANGMAN_DEBUG_LEVEL_ALL) fprintf(stderr, __VA_ARGS__); } while (0)
#define DEBUG_SUMMARY_PRINTF(...) \
            do { if (HANGMAN_DEBUG_FLAG >= HANGMAN_DEBUG_LEVEL_SUMMARY) fprintf(stderr, __VA_ARGS__); } while (0)

#endif /* HANGMAN_DEBUG_H */
