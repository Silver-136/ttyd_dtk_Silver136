/* Entry points inside __save_fpr, __restore_fpr, __save_gpr and __restore_gpr below: _savefpr_N saves fN..f31,
   _restfpr_N restores them, and likewise for the general registers. */
void _savefpr_14(void);
void _savefpr_15(void);
void _savefpr_16(void);
void _savefpr_17(void);
void _savefpr_18(void);
void _savefpr_19(void);
void _savefpr_20(void);
void _savefpr_21(void);
void _savefpr_22(void);
void _savefpr_23(void);
void _savefpr_24(void);
void _savefpr_25(void);
void _savefpr_26(void);
void _savefpr_27(void);
void _savefpr_28(void);
void _savefpr_29(void);
void _savefpr_30(void);
void _savefpr_31(void);
void _restfpr_14(void);
void _restfpr_15(void);
void _restfpr_16(void);
void _restfpr_17(void);
void _restfpr_18(void);
void _restfpr_19(void);
void _restfpr_20(void);
void _restfpr_21(void);
void _restfpr_22(void);
void _restfpr_23(void);
void _restfpr_24(void);
void _restfpr_25(void);
void _restfpr_26(void);
void _restfpr_27(void);
void _restfpr_28(void);
void _restfpr_29(void);
void _restfpr_30(void);
void _restfpr_31(void);
void _savegpr_14(void);
void _savegpr_15(void);
void _savegpr_16(void);
void _savegpr_17(void);
void _savegpr_18(void);
void _savegpr_19(void);
void _savegpr_20(void);
void _savegpr_21(void);
void _savegpr_22(void);
void _savegpr_23(void);
void _savegpr_24(void);
void _savegpr_25(void);
void _savegpr_26(void);
void _savegpr_27(void);
void _savegpr_28(void);
void _savegpr_29(void);
void _savegpr_30(void);
void _savegpr_31(void);
void _restgpr_14(void);
void _restgpr_15(void);
void _restgpr_16(void);
void _restgpr_17(void);
void _restgpr_18(void);
void _restgpr_19(void);
void _restgpr_20(void);
void _restgpr_21(void);
void _restgpr_22(void);
void _restgpr_23(void);
void _restgpr_24(void);
void _restgpr_25(void);
void _restgpr_26(void);
void _restgpr_27(void);
void _restgpr_28(void);
void _restgpr_29(void);
void _restgpr_30(void);
void _restgpr_31(void);

static const double __constants[] = {0.0, 4294967296.0, 2147483648.0};

/* double to unsigned int */
asm void __cvt_fp2unsigned(void) {
    nofralloc
    stwu r1, -0x10(r1)
    lis r4, __constants@h
    ori r4, r4, __constants@l
    li r3, 0x0
    lfd f0, 0x0(r4)
    lfd f3, 0x8(r4)
    lfd f4, 0x10(r4)
    fcmpu cr0, f1, f0
    fcmpu cr6, f1, f3
    blt done
    subi r3, r3, 0x1
    bge cr6, done
    fcmpu cr7, f1, f4
    fmr f2, f1
    blt cr7, skip
    fsub f2, f1, f4
skip:
    fctiwz f2, f2
    stfd f2, 0x8(r1)
    lwz r3, 0xc(r1)
    blt cr7, done
    addis r3, r3, 0x8000
done:
    addi r1, r1, 0x10
    blr
}

asm void __save_fpr(void) {
    nofralloc
entry _savefpr_14
    stfd f14, -0x90(r11)
entry _savefpr_15
    stfd f15, -0x88(r11)
entry _savefpr_16
    stfd f16, -0x80(r11)
entry _savefpr_17
    stfd f17, -0x78(r11)
entry _savefpr_18
    stfd f18, -0x70(r11)
entry _savefpr_19
    stfd f19, -0x68(r11)
entry _savefpr_20
    stfd f20, -0x60(r11)
entry _savefpr_21
    stfd f21, -0x58(r11)
entry _savefpr_22
    stfd f22, -0x50(r11)
entry _savefpr_23
    stfd f23, -0x48(r11)
entry _savefpr_24
    stfd f24, -0x40(r11)
entry _savefpr_25
    stfd f25, -0x38(r11)
entry _savefpr_26
    stfd f26, -0x30(r11)
entry _savefpr_27
    stfd f27, -0x28(r11)
entry _savefpr_28
    stfd f28, -0x20(r11)
entry _savefpr_29
    stfd f29, -0x18(r11)
entry _savefpr_30
    stfd f30, -0x10(r11)
entry _savefpr_31
    stfd f31, -0x8(r11)
    blr
}

asm void __restore_fpr(void) {
    nofralloc
entry _restfpr_14
    lfd f14, -0x90(r11)
entry _restfpr_15
    lfd f15, -0x88(r11)
entry _restfpr_16
    lfd f16, -0x80(r11)
entry _restfpr_17
    lfd f17, -0x78(r11)
entry _restfpr_18
    lfd f18, -0x70(r11)
entry _restfpr_19
    lfd f19, -0x68(r11)
entry _restfpr_20
    lfd f20, -0x60(r11)
entry _restfpr_21
    lfd f21, -0x58(r11)
entry _restfpr_22
    lfd f22, -0x50(r11)
entry _restfpr_23
    lfd f23, -0x48(r11)
entry _restfpr_24
    lfd f24, -0x40(r11)
entry _restfpr_25
    lfd f25, -0x38(r11)
entry _restfpr_26
    lfd f26, -0x30(r11)
entry _restfpr_27
    lfd f27, -0x28(r11)
entry _restfpr_28
    lfd f28, -0x20(r11)
entry _restfpr_29
    lfd f29, -0x18(r11)
entry _restfpr_30
    lfd f30, -0x10(r11)
entry _restfpr_31
    lfd f31, -0x8(r11)
    blr
}

asm void __save_gpr(void) {
    nofralloc
entry _savegpr_14
    stw r14, -0x48(r11)
entry _savegpr_15
    stw r15, -0x44(r11)
entry _savegpr_16
    stw r16, -0x40(r11)
entry _savegpr_17
    stw r17, -0x3c(r11)
entry _savegpr_18
    stw r18, -0x38(r11)
entry _savegpr_19
    stw r19, -0x34(r11)
entry _savegpr_20
    stw r20, -0x30(r11)
entry _savegpr_21
    stw r21, -0x2c(r11)
entry _savegpr_22
    stw r22, -0x28(r11)
entry _savegpr_23
    stw r23, -0x24(r11)
entry _savegpr_24
    stw r24, -0x20(r11)
entry _savegpr_25
    stw r25, -0x1c(r11)
entry _savegpr_26
    stw r26, -0x18(r11)
entry _savegpr_27
    stw r27, -0x14(r11)
entry _savegpr_28
    stw r28, -0x10(r11)
entry _savegpr_29
    stw r29, -0xc(r11)
entry _savegpr_30
    stw r30, -0x8(r11)
entry _savegpr_31
    stw r31, -0x4(r11)
    blr
}

asm void __restore_gpr(void) {
    nofralloc
entry _restgpr_14
    lwz r14, -0x48(r11)
entry _restgpr_15
    lwz r15, -0x44(r11)
entry _restgpr_16
    lwz r16, -0x40(r11)
entry _restgpr_17
    lwz r17, -0x3c(r11)
entry _restgpr_18
    lwz r18, -0x38(r11)
entry _restgpr_19
    lwz r19, -0x34(r11)
entry _restgpr_20
    lwz r20, -0x30(r11)
entry _restgpr_21
    lwz r21, -0x2c(r11)
entry _restgpr_22
    lwz r22, -0x28(r11)
entry _restgpr_23
    lwz r23, -0x24(r11)
entry _restgpr_24
    lwz r24, -0x20(r11)
entry _restgpr_25
    lwz r25, -0x1c(r11)
entry _restgpr_26
    lwz r26, -0x18(r11)
entry _restgpr_27
    lwz r27, -0x14(r11)
entry _restgpr_28
    lwz r28, -0x10(r11)
entry _restgpr_29
    lwz r29, -0xc(r11)
entry _restgpr_30
    lwz r30, -0x8(r11)
entry _restgpr_31
    lwz r31, -0x4(r11)
    blr
}

/* 64-bit unsigned divide */
asm void __div2u(void) {
    nofralloc
    cmpwi r3, 0x0
    cntlzw r0, r3
    cntlzw r9, r4
    bne skip
    addi r0, r9, 0x20
skip:
    cmpwi r5, 0x0
    cntlzw r9, r5
    cntlzw r10, r6
    bne skip2
    addi r9, r10, 0x20
skip2:
    cmpw r0, r9
    subfic r10, r0, 0x40
    bgt done
    addi r9, r9, 0x1
    subfic r9, r9, 0x40
    add r0, r0, r9
    subf r9, r9, r10
    mtctr r9
    cmpwi r9, 0x20
    subi r7, r9, 0x20
    blt skip3
    srw r8, r3, r7
    li r7, 0x0
    b skip4
skip3:
    srw r8, r4, r9
    subfic r7, r9, 0x20
    slw r7, r3, r7
    or r8, r8, r7
    srw r7, r3, r9
skip4:
    cmpwi r0, 0x20
    subic r9, r0, 0x20
    blt skip5
    slw r3, r4, r9
    li r4, 0x0
    b skip6
skip5:
    slw r3, r3, r0
    subfic r9, r0, 0x20
    srw r9, r4, r9
    or r3, r3, r9
    slw r4, r4, r0
skip6:
    li r10, -0x1
    addic r7, r7, 0x0
loop:
    adde r4, r4, r4
    adde r3, r3, r3
    adde r8, r8, r8
    adde r7, r7, r7
    subfc r0, r6, r8
    subfe. r9, r5, r7
    blt skip7
    mr r8, r0
    mr r7, r9
    addic r0, r10, 0x1
skip7:
    bdnz loop
    adde r4, r4, r4
    adde r3, r3, r3
    blr
done:
    li r4, 0x0
    li r3, 0x0
    blr
}

/* 64-bit signed divide */
asm void __div2i(void) {
    nofralloc
    stwu r1, -0x10(r1)
    clrrwi. r9, r3, 31
    beq skip
    subfic r4, r4, 0x0
    subfze r3, r3
skip:
    stw r9, 0x8(r1)
    clrrwi. r10, r5, 31
    beq skip2
    subfic r6, r6, 0x0
    subfze r5, r5
skip2:
    stw r10, 0xc(r1)
    cmpwi r3, 0x0
    cntlzw r0, r3
    cntlzw r9, r4
    bne skip3
    addi r0, r9, 0x20
skip3:
    cmpwi r5, 0x0
    cntlzw r9, r5
    cntlzw r10, r6
    bne skip4
    addi r9, r10, 0x20
skip4:
    cmpw r0, r9
    subfic r10, r0, 0x40
    bgt skip10
    addi r9, r9, 0x1
    subfic r9, r9, 0x40
    add r0, r0, r9
    subf r9, r9, r10
    mtctr r9
    cmpwi r9, 0x20
    subi r7, r9, 0x20
    blt skip5
    srw r8, r3, r7
    li r7, 0x0
    b skip6
skip5:
    srw r8, r4, r9
    subfic r7, r9, 0x20
    slw r7, r3, r7
    or r8, r8, r7
    srw r7, r3, r9
skip6:
    cmpwi r0, 0x20
    subic r9, r0, 0x20
    blt skip7
    slw r3, r4, r9
    li r4, 0x0
    b skip8
skip7:
    slw r3, r3, r0
    subfic r9, r0, 0x20
    srw r9, r4, r9
    or r3, r3, r9
    slw r4, r4, r0
skip8:
    li r10, -0x1
    addic r7, r7, 0x0
loop:
    adde r4, r4, r4
    adde r3, r3, r3
    adde r8, r8, r8
    adde r7, r7, r7
    subfc r0, r6, r8
    subfe. r9, r5, r7
    blt skip9
    mr r8, r0
    mr r7, r9
    addic r0, r10, 0x1
skip9:
    bdnz loop
    adde r4, r4, r4
    adde r3, r3, r3
    lwz r9, 0x8(r1)
    lwz r10, 0xc(r1)
    xor. r7, r9, r10
    beq done
    cmpwi r9, 0x0
    subfic r4, r4, 0x0
    subfze r3, r3
    b done
skip10:
    li r4, 0x0
    li r3, 0x0
done:
    addi r1, r1, 0x10
    blr
}

/* 64-bit unsigned remainder */
asm void __mod2u(void) {
    nofralloc
    cmpwi r3, 0x0
    cntlzw r0, r3
    cntlzw r9, r4
    bne skip
    addi r0, r9, 0x20
skip:
    cmpwi r5, 0x0
    cntlzw r9, r5
    cntlzw r10, r6
    bne skip2
    addi r9, r10, 0x20
skip2:
    cmpw r0, r9
    subfic r10, r0, 0x40
    bgtlr
    addi r9, r9, 0x1
    subfic r9, r9, 0x40
    add r0, r0, r9
    subf r9, r9, r10
    mtctr r9
    cmpwi r9, 0x20
    subi r7, r9, 0x20
    blt skip3
    srw r8, r3, r7
    li r7, 0x0
    b skip4
skip3:
    srw r8, r4, r9
    subfic r7, r9, 0x20
    slw r7, r3, r7
    or r8, r8, r7
    srw r7, r3, r9
skip4:
    cmpwi r0, 0x20
    subic r9, r0, 0x20
    blt skip5
    slw r3, r4, r9
    li r4, 0x0
    b skip6
skip5:
    slw r3, r3, r0
    subfic r9, r0, 0x20
    srw r9, r4, r9
    or r3, r3, r9
    slw r4, r4, r0
skip6:
    li r10, -0x1
    addic r7, r7, 0x0
loop:
    adde r4, r4, r4
    adde r3, r3, r3
    adde r8, r8, r8
    adde r7, r7, r7
    subfc r0, r6, r8
    subfe. r9, r5, r7
    blt done
    mr r8, r0
    mr r7, r9
    addic r0, r10, 0x1
done:
    bdnz loop
    mr r4, r8
    mr r3, r7
    blr
    blr
}

/* 64-bit signed remainder */
asm void __mod2i(void) {
    nofralloc
    cmpwi cr7, r3, 0x0
    bge cr7, skip
    subfic r4, r4, 0x0
    subfze r3, r3
skip:
    cmpwi r5, 0x0
    bge skip2
    subfic r6, r6, 0x0
    subfze r5, r5
skip2:
    cmpwi r3, 0x0
    cntlzw r0, r3
    cntlzw r9, r4
    bne skip3
    addi r0, r9, 0x20
skip3:
    cmpwi r5, 0x0
    cntlzw r9, r5
    cntlzw r10, r6
    bne skip4
    addi r9, r10, 0x20
skip4:
    cmpw r0, r9
    subfic r10, r0, 0x40
    bgt done
    addi r9, r9, 0x1
    subfic r9, r9, 0x40
    add r0, r0, r9
    subf r9, r9, r10
    mtctr r9
    cmpwi r9, 0x20
    subi r7, r9, 0x20
    blt skip5
    srw r8, r3, r7
    li r7, 0x0
    b skip6
skip5:
    srw r8, r4, r9
    subfic r7, r9, 0x20
    slw r7, r3, r7
    or r8, r8, r7
    srw r7, r3, r9
skip6:
    cmpwi r0, 0x20
    subic r9, r0, 0x20
    blt skip7
    slw r3, r4, r9
    li r4, 0x0
    b skip8
skip7:
    slw r3, r3, r0
    subfic r9, r0, 0x20
    srw r9, r4, r9
    or r3, r3, r9
    slw r4, r4, r0
skip8:
    li r10, -0x1
    addic r7, r7, 0x0
loop:
    adde r4, r4, r4
    adde r3, r3, r3
    adde r8, r8, r8
    adde r7, r7, r7
    subfc r0, r6, r8
    subfe. r9, r5, r7
    blt skip9
    mr r8, r0
    mr r7, r9
    addic r0, r10, 0x1
skip9:
    bdnz loop
    mr r4, r8
    mr r3, r7
done:
    bgelr cr7
    subfic r4, r4, 0x0
    subfze r3, r3
    blr
}

/* 64-bit shift left */
asm void __shl2i(void) {
    nofralloc
    subfic r8, r5, 0x20
    subic r9, r5, 0x20
    slw r3, r3, r5
    srw r10, r4, r8
    or r3, r3, r10
    slw r10, r4, r9
    or r3, r3, r10
    slw r4, r4, r5
    blr
}

/* 64-bit logical shift right */
asm void __shr2u(void) {
    nofralloc
    subfic r8, r5, 0x20
    subic r9, r5, 0x20
    srw r4, r4, r5
    slw r10, r3, r8
    or r4, r4, r10
    srw r10, r3, r9
    or r4, r4, r10
    srw r3, r3, r5
    blr
}

/* 64-bit arithmetic shift right */
asm void __shr2i(void) {
    nofralloc
    subfic r8, r5, 0x20
    subic. r9, r5, 0x20
    srw r4, r4, r5
    slw r10, r3, r8
    or r4, r4, r10
    sraw r10, r3, r9
    ble done
    or r4, r4, r10
done:
    sraw r3, r3, r5
    blr
}

/* signed long long to float */
asm void __cvt_sll_flt(void) {
    nofralloc
    stwu r1, -0x10(r1)
    clrrwi. r5, r3, 31
    beq skip
    subfic r4, r4, 0x0
    subfze r3, r3
skip:
    or. r7, r3, r4
    li r6, 0x0
    beq done
    cntlzw r7, r3
    cntlzw r8, r4
    extlwi r9, r7, 5, 26
    srawi r9, r9, 31
    and r9, r9, r8
    add r7, r7, r9
    subfic r8, r7, 0x20
    subic r9, r7, 0x20
    slw r3, r3, r7
    srw r10, r4, r8
    or r3, r3, r10
    slw r10, r4, r9
    or r3, r3, r10
    slw r4, r4, r7
    subf r6, r7, r6
    clrlwi r7, r4, 21
    cmpwi r7, 0x400
    addi r6, r6, 0x43e
    blt skip3
    bgt skip2
    rlwinm. r7, r4, 0, 20, 20
    beq skip3
skip2:
    addic r4, r4, 0x800
    addze r3, r3
    addze r6, r6
skip3:
    rotrwi r4, r4, 11
    rlwimi r4, r3, 21, 0, 10
    extrwi r3, r3, 20, 1
    slwi r6, r6, 20
    or r3, r6, r3
    or r3, r5, r3
done:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    lfd f1, 0x8(r1)
    frsp f1, f1
    addi r1, r1, 0x10
    blr
}

/* unsigned long long to float */
asm void __cvt_ull_flt(void) {
    nofralloc
    stwu r1, -0x10(r1)
    or. r7, r3, r4
    li r6, 0x0
    beq done
    cntlzw r7, r3
    cntlzw r8, r4
    extlwi r9, r7, 5, 26
    srawi r9, r9, 31
    and r9, r9, r8
    add r7, r7, r9
    subfic r8, r7, 0x20
    subic r9, r7, 0x20
    slw r3, r3, r7
    srw r10, r4, r8
    or r3, r3, r10
    slw r10, r4, r9
    or r3, r3, r10
    slw r4, r4, r7
    subf r6, r7, r6
    clrlwi r7, r4, 21
    cmpwi r7, 0x400
    addi r6, r6, 0x43e
    blt skip2
    bgt skip
    rlwinm. r7, r4, 0, 20, 20
    beq skip2
skip:
    addic r4, r4, 0x800
    addze r3, r3
    addze r6, r6
skip2:
    rotrwi r4, r4, 11
    rlwimi r4, r3, 21, 0, 10
    extrwi r3, r3, 20, 1
    slwi r6, r6, 20
    or r3, r6, r3
done:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    lfd f1, 0x8(r1)
    frsp f1, f1
    addi r1, r1, 0x10
    blr
}

/* double to unsigned long long */
asm void __cvt_dbl_usll(void) {
    nofralloc
    stwu r1, -0x10(r1)
    stfd f1, 0x8(r1)
    lwz r3, 0x8(r1)
    lwz r4, 0xc(r1)
    extrwi r5, r3, 11, 1
    cmplwi r5, 0x3ff
    bge skip
    li r3, 0x0
    li r4, 0x0
    b done
skip:
    mr r6, r3
    clrlwi r3, r3, 12
    oris r3, r3, 0x10
    subi r5, r5, 0x433
    cmpwi r5, 0x0
    bge skip2
    neg r5, r5
    subfic r8, r5, 0x20
    subic r9, r5, 0x20
    srw r4, r4, r5
    slw r10, r3, r8
    or r4, r4, r10
    srw r10, r3, r9
    or r4, r4, r10
    srw r3, r3, r5
    b skip5
skip2:
    cmpwi r5, 0xa
    ble+ skip4
    clrrwi. r6, r6, 31
    beq skip3
    lis r3, 0x8000
    li r4, 0x0
    b done
skip3:
    lis r3, 0x7fff
    ori r3, r3, 0xffff
    li r4, -0x1
    b done
skip4:
    subfic r8, r5, 0x20
    subic r9, r5, 0x20
    slw r3, r3, r5
    srw r10, r4, r8
    or r3, r3, r10
    slw r10, r4, r9
    or r3, r3, r10
    slw r4, r4, r5
skip5:
    clrrwi. r6, r6, 31
    beq done
    subfic r4, r4, 0x0
    subfze r3, r3
done:
    addi r1, r1, 0x10
    blr
}
#if defined(VERSION_G8MJ01_DEMO) || defined(VERSION_G8ME01_DEMO)
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
#endif

#ifdef VERSION_G8MP01
/* PAL only: setjmp and longjmp of the Metrowerks runtime, in assembly as in the original source. They
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
#endif
