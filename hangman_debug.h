#ifndef HANGMAN_DEBUG_H
#define HANGMAN_DEBUG_H

extern int HANGMAN_DEBUG_FLAG;

#define DEBUG(s) do{if(HANGMAN_DEBUG_FLAG) s;} while(0)

#endif /* HANGMAN_DEBUG_H */
