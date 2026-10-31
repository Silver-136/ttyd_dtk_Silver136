#define EOF -1

extern unsigned char __upper_map[];

/** toupper() via the __upper_map table; EOF passes through. */
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
