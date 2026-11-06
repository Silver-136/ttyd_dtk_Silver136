#include <charPipeline/structures/dolphinString.h>

/* Writes str1 followed by str2 to dst (the destination is the last argument); dst's size is not checked. */
u8 Strcat(char* str1, char* str2, char* dst) {
    char* srcCursor = str1;
    char* dstCursor = dst;;

    if (!dst) {
        return 0;
    }
    if (!str1) {
        return 0;
    }
    if (!str2) {
        return 0;
    }

    while ((s8)*srcCursor != 0) {
        *dstCursor = *srcCursor;
        dstCursor++;
        srcCursor++;
    }

    srcCursor = str2;
    while ((s8)*srcCursor != 0) {
        *dstCursor = *srcCursor;
        dstCursor++;
        srcCursor++;
    }

    *dstCursor = 0;
    return 1;
}

/* BUG: the loop tests the next source character after each copy, so the terminating NUL is never copied (and an
   empty source still copies its NUL plus whatever follows it up to the next NUL). */
void Strcpy(char* dst, char* src) {
    do {
        *dst = *src;
        dst++;
        src++;
    } while ((s8)*src != 0);
}

/* Returns 1 when str1 sorts first, -1 when str2 does (the opposite sign of strcmp).
   BUG: returns 0 as soon as either string ends, so a string compares equal to any longer string it starts. */
s8 Strcmp(char* str1, char* str2) {
    char* cursor1 = str1;
    char* cursor2 = str2;
    while (1) {
        if ((s8)*cursor1 < (s8)*cursor2) {
            return 1;
        }
        if ((s8)*cursor1 > (s8)*cursor2) {
            return -1;
        }
        cursor1++;
        cursor2++;
        if ((s8)*cursor1 == 0 || (s8)*cursor2 == 0) {
            return 0;
        }
    }
}

u32 Strlen(char* str) {
    char* cursor = str;
    u32 counter = 0;

    if (!str) {
        return 0;
    }

    while ((s8)*cursor != 0) {
        cursor++;
        counter++;
    }

    return counter;
}
