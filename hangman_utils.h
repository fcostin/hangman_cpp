#ifndef HANGMAN_UTILS_H
#define HANGMAN_UTILS_H

#include "hangman_types.h"

template <class T>
inline string to_string(const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

inline const char * small_int_to_string(const size_t & i) {
    switch(i) {
        case 0:
            return "0";
        case 1:
            return "1";
        case 2:
            return "2";
        case 3:
            return "3";
        case 4:
            return "4";
        case 5:
            return "5";
        case 6:
            return "6";
        case 7:
            return "7";
        case 8:
            return "8";
        case 9:
            return "9";
        case 10:
            return "10";
        case 11:
            return "11";
        case 12:
            return "12";
        case 13:
            return "13";
        case 14:
            return "14";
        case 15:
            return "15";
        case 16:
            return "16";
        case 17:
            return "17";
        case 18:
            return "18";
        case 19:
            return "19";
        case 20:
            return "20";
        case 21:
            return "21";
        case 22:
            return "22";
        case 23:
            return "23";
        case 24:
            return "24";
        case 25:
            return "25";
        case 26:
            return "26";
        case 27:
            return "27";
        case 28:
            return "28";
        case 29:
            return "29";
        default:
            return to_string(i).c_str();
    }
}


#endif // HANGMAN_UTILS_H
