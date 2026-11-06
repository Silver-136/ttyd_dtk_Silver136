#include "ctype_map.h"

#define EOF -1

/** toupper() via the __upper_map table; EOF passes through. Declared inline: as a plain function the object
    changes. */
inline int toupper(int c) {
    return (c == EOF) ? EOF : (int)__upper_map[(unsigned char)c];
}

/** Non-standard strupr(): converts a string to upper case in place and returns it. */
char* strupr(char* str) {
    char* p;

    for (p = str; *p; p++) {
        *p = toupper(*p);
    }
    return str;
}
