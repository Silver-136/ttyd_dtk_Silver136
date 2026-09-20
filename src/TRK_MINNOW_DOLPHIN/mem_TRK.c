#include "dolphin/types.h"

void TRK_fill_mem(void* dest, int value, unsigned long length);

__declspec(section ".init") void* TRK_memcpy(void* dst, const void* src, size_t n) {
    const char* p;
    char* q;

    for (p = (const char*)src - 1, q = (char*)dst - 1, n++; --n;) {
        *++q = *++p;
    }

    return dst;
}

__declspec(section ".init") void* TRK_memset(void* dst, int val, size_t n) {
    TRK_fill_mem(dst, val, n);
    return dst;
}
