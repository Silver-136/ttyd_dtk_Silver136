typedef unsigned char u8;
typedef unsigned int u32;

/* Fills n bytes at dst with the byte val: single bytes up to a word boundary, then eight words per step, the rest
   in words and bytes. dst itself walks the buffer with pre-increment stores (stbu/stwu), so it always points one
   unit before the next address to write. The casts on the left are CodeWarrior's cast-as-lvalue extension.
   Form after the matched GC 2.6 decompilations of pikmin2 and ac-decomp (CC0). */
void TRK_fill_mem(void* dst, int val, unsigned int n) {
    u32 v, i, j;
    v = (u8)val;

    ((u8*)dst) = ((u8*)dst) - 1;

    if (n >= 32) {
        i = ~(u32)dst & 3;

        if (i) {
            n -= i;

            do {
                *++((u8*)dst) = v;
            } while (--i);
        }

        if (v)
            v |= v << 24 | v << 16 | v << 8;

        // one byte before the next word -> one word before it, in two steps (written as one step, or with a
        // second pointer variable, the compiler allocates other registers)
        ((u32*)dst) = ((u32*)(((u8*)dst) + 4)) - 1;
        ((u32*)dst) = ((u32*)(((u8*)dst) + 1)) - 1;

        i = n / 32;

        if (i) {
            do {
                for (j = 0; j < 8; j++)
                    *++((u32*)dst) = v;
            } while (--i);
        }

        i = (n / 4) % 8;

        if (i) {
            do {
                *++((u32*)dst) = v;
            } while (--i);
        }

        // one word before the next byte -> one byte before it
        ((u8*)dst) = ((u8*)(((u32*)dst) + 1)) - 1;

        n %= 4;
    }

    if (n) {
        do {
            *++((u8*)dst) = v;
        } while (--n);
    }
}

/* Forward byte copy only (the runtime memcpy in __mem.c also copies backward for overlaps). */
__declspec(section ".init") void* TRK_memcpy(void* dst, const void* src, unsigned int n) {
    const unsigned char* s = (const unsigned char*)src - 1;
    unsigned char* d = (unsigned char*)dst - 1;

    n++;
    while (--n != 0)
        *++d = *++s;

    return dst;
}

__declspec(section ".init") void* TRK_memset(void* dst, int val, unsigned int n) {
    TRK_fill_mem(dst, val, n);
    return dst;
}
