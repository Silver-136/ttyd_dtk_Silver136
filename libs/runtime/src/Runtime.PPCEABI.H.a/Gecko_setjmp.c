// Gecko_setjmp.c of the Metrowerks runtime library (its own object in the JP demo map). Linked only by the
// versions that use setjmp: the early demos, PAL and the PAL demo; JP, US and the US September demo leave
// it out.
/* setjmp and longjmp of the Metrowerks runtime, in assembly as in the original source. They
 * save and restore the return address, the condition register, r1, r2, r13-r31, f14-f31 (each with its
 * paired-single half) and the FPSCR. */
asm int __setjmp(register void* env) {
    nofralloc
    mflr r5
    mfcr r6
    stw r5, 0x0(r3)
    stw r6, 0x4(r3)
    stw r1, 0x8(r3)
    stw r2, 0xc(r3)
    stmw r13, 0x14(r3)
    mffs f0
    stfd f14, 0x60(r3)
    addi r4, r3, 0x68
    psq_stx f14, 0, r4, 0, 0
    stfd f15, 0x70(r3)
    addi r4, r3, 0x78
    psq_stx f15, 0, r4, 0, 0
    stfd f16, 0x80(r3)
    addi r4, r3, 0x88
    psq_stx f16, 0, r4, 0, 0
    stfd f17, 0x90(r3)
    addi r4, r3, 0x98
    psq_stx f17, 0, r4, 0, 0
    stfd f18, 0xa0(r3)
    addi r4, r3, 0xa8
    psq_stx f18, 0, r4, 0, 0
    stfd f19, 0xb0(r3)
    addi r4, r3, 0xb8
    psq_stx f19, 0, r4, 0, 0
    stfd f20, 0xc0(r3)
    addi r4, r3, 0xc8
    psq_stx f20, 0, r4, 0, 0
    stfd f21, 0xd0(r3)
    addi r4, r3, 0xd8
    psq_stx f21, 0, r4, 0, 0
    stfd f22, 0xe0(r3)
    addi r4, r3, 0xe8
    psq_stx f22, 0, r4, 0, 0
    stfd f23, 0xf0(r3)
    addi r4, r3, 0xf8
    psq_stx f23, 0, r4, 0, 0
    stfd f24, 0x100(r3)
    addi r4, r3, 0x108
    psq_stx f24, 0, r4, 0, 0
    stfd f25, 0x110(r3)
    addi r4, r3, 0x118
    psq_stx f25, 0, r4, 0, 0
    stfd f26, 0x120(r3)
    addi r4, r3, 0x128
    psq_stx f26, 0, r4, 0, 0
    stfd f27, 0x130(r3)
    addi r4, r3, 0x138
    psq_stx f27, 0, r4, 0, 0
    stfd f28, 0x140(r3)
    addi r4, r3, 0x148
    psq_stx f28, 0, r4, 0, 0
    stfd f29, 0x150(r3)
    addi r4, r3, 0x158
    psq_stx f29, 0, r4, 0, 0
    stfd f30, 0x160(r3)
    addi r4, r3, 0x168
    psq_stx f30, 0, r4, 0, 0
    stfd f31, 0x170(r3)
    addi r4, r3, 0x178
    psq_stx f31, 0, r4, 0, 0
    stfd f0, 0x180(r3)
    li r3, 0x0
    blr
}

/**
 * Restores the registers saved by __setjmp (return address, condition register, r1, r2, r13-r31,
 * f14-f31 and FPSCR) and resumes at the setjmp call, which returns the given value (1 if it is 0).
 */
asm void longjmp(register void* env, register int val) {
    nofralloc
    lwz r5, 0x0(r3)
    lwz r6, 0x4(r3)
    mtlr r5
    mtcrf 255, r6
    lwz r1, 0x8(r3)
    lwz r2, 0xc(r3)
    lmw r13, 0x14(r3)
    lfd f14, 0x60(r3)
    addi r7, r3, 0x68
    psq_lx f14, 0, r7, 0, 0
    lfd f15, 0x70(r3)
    addi r7, r3, 0x78
    psq_lx f15, 0, r7, 0, 0
    lfd f16, 0x80(r3)
    addi r7, r3, 0x88
    psq_lx f16, 0, r7, 0, 0
    lfd f17, 0x90(r3)
    addi r7, r3, 0x98
    psq_lx f17, 0, r7, 0, 0
    lfd f18, 0xa0(r3)
    addi r7, r3, 0xa8
    psq_lx f18, 0, r7, 0, 0
    lfd f19, 0xb0(r3)
    addi r7, r3, 0xb8
    psq_lx f19, 0, r7, 0, 0
    lfd f20, 0xc0(r3)
    addi r7, r3, 0xc8
    psq_lx f20, 0, r7, 0, 0
    lfd f21, 0xd0(r3)
    addi r7, r3, 0xd8
    psq_lx f21, 0, r7, 0, 0
    lfd f22, 0xe0(r3)
    addi r7, r3, 0xe8
    psq_lx f22, 0, r7, 0, 0
    lfd f23, 0xf0(r3)
    addi r7, r3, 0xf8
    psq_lx f23, 0, r7, 0, 0
    lfd f24, 0x100(r3)
    addi r7, r3, 0x108
    psq_lx f24, 0, r7, 0, 0
    lfd f25, 0x110(r3)
    addi r7, r3, 0x118
    psq_lx f25, 0, r7, 0, 0
    lfd f26, 0x120(r3)
    addi r7, r3, 0x128
    psq_lx f26, 0, r7, 0, 0
    lfd f27, 0x130(r3)
    addi r7, r3, 0x138
    psq_lx f27, 0, r7, 0, 0
    lfd f28, 0x140(r3)
    addi r7, r3, 0x148
    psq_lx f28, 0, r7, 0, 0
    lfd f29, 0x150(r3)
    addi r7, r3, 0x158
    psq_lx f29, 0, r7, 0, 0
    lfd f30, 0x160(r3)
    addi r7, r3, 0x168
    psq_lx f30, 0, r7, 0, 0
    lfd f31, 0x170(r3)
    addi r7, r3, 0x178
    psq_lx f31, 0, r7, 0, 0
    lfd f0, 0x180(r3)
    cmpwi r4, 0x0
    mr r3, r4
    mtfsf 255, f0
    bnelr
    li r3, 0x1
    blr
}
