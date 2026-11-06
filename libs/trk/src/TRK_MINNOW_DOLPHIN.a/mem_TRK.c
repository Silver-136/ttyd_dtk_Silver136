asm void TRK_fill_mem(void* dst, int val, unsigned int n) {
    nofralloc
    cmplwi r5, 0x20
    clrlwi r4, r4, 24
    subi r6, r3, 0x1
    mr r7, r4
    blt skip5
    nor r0, r6, r6
    clrlwi. r3, r0, 30
    beq skip
    subf r5, r3, r5
loop:
    subic. r3, r3, 0x1
    stbu r7, 0x1(r6)
    bne loop
skip:
    cmplwi r7, 0x0
    beq skip2
    slwi r3, r7, 24
    slwi r0, r7, 16
    slwi r4, r7, 8
    or r0, r3, r0
    or r0, r4, r0
    or r7, r7, r0
skip2:
    srwi. r4, r5, 5
    subi r3, r6, 0x3
    beq skip3
loop2:
    stw r7, 0x4(r3)
    subic. r4, r4, 0x1
    stw r7, 0x8(r3)
    stw r7, 0xc(r3)
    stw r7, 0x10(r3)
    stw r7, 0x14(r3)
    stw r7, 0x18(r3)
    stw r7, 0x1c(r3)
    stwu r7, 0x20(r3)
    bne loop2
skip3:
    extrwi. r4, r5, 3, 27
    beq skip4
loop3:
    subic. r4, r4, 0x1
    stwu r7, 0x4(r3)
    bne loop3
skip4:
    addi r6, r3, 0x3
    clrlwi r5, r5, 30
skip5:
    cmplwi r5, 0x0
    beqlr
loop4:
    subic. r5, r5, 0x1
    stbu r7, 0x1(r6)
    bne loop4
    blr
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
