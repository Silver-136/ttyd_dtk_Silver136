#include "lconv.h"

#define CHAR_MAX 127 /* plain char is signed */

struct lconv __lconv = {
    ".", "", "", "", "", "", "", "", "",
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
    "",
    CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
};

int strcmp(const char*, const char*);

/* Only the "C" locale exists; category is ignored. */
char* setlocale(int category, const char* locale) {
    if (locale == 0 || !strcmp(locale, "C") || !strcmp(locale, ""))
        return "C";
    return 0;
}
