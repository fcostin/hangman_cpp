#ifndef HANGMAN_DEBUG_H
#define HANGMAN_DEBUG_H

#include "stdio.h"

extern int HANGMAN_DEBUG_FLAG;

#define IF_DEBUG(s) do{if(HANGMAN_DEBUG_FLAG) s;} while(0)
#define DEBUG_PRINTF(...) \
            do { if (HANGMAN_DEBUG_FLAG) fprintf(stderr, __VA_ARGS__); } while (0)

#endif /* HANGMAN_DEBUG_H */
