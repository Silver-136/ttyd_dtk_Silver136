typedef int BOOL;
typedef int DSError;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu);
void TRKSaveExtended1Block(void);
int InitMetroTRKCommTable(int hwId);
void TRK_main(void);
void EnableEXI2Interrupts(void);
u32 __TRK_get_MSR(void);
void* TRK_memcpy(void* dst, const void* src, unsigned int n);
void TRK_flush_cache(void* addr, int length);
u32 ARGetDMAStatus(void);
u16 __ARGetInterruptStatus(void);
void __ARClearInterrupt(void);
void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length);

extern u8 _db_stack_addr[];
extern u8 gTRKInterruptVectorTable[];

typedef struct ProcessorState_PPC {
    u8 pad[0x238];
    u32 DBAT3U;
} ProcessorState_PPC;

typedef struct TRKState {
    u8 pad[0x8C];
    u32 msr;
    u8 pad2[0x98 - 0x90];
    BOOL isStopped;
} TRKState;

extern ProcessorState_PPC gTRKCPUState;
extern TRKState gTRKState;

static u32 TRK_ISR_OFFSETS[15] = {0x100, 0x200, 0x300, 0x400, 0x500, 0x600, 0x700, 0x800,
                                  0x900, 0xC00, 0xD00, 0xF00, 0x1300, 0x1400, 0x1700};

u32 lc_base;

asm void InitMetroTRK(void) {
    nofralloc
    subi r1, r1, 0x4
    stw r3, 0x0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, 0x0(r3)
    lwz r4, 0x0(r1)
    addi r1, r1, 0x4
    stw r1, 0x4(r3)
    stw r4, 0xc(r3)
    mflr r4
    stw r4, 0x84(r3)
    stw r4, 0x80(r3)
    mfcr r4
    stw r4, 0x88(r3)
    mfmsr r4
    ori r3, r4, 0x8000
    xori r3, r3, 0x8000
    mtmsr r3
    mtsrr1 r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    opword 0xB8030000
    li r0, 0x0
    mtspr 0x3F2, r0
    mtspr 0x3F5, r0
    lis r1, _db_stack_addr@h
    ori r1, r1, _db_stack_addr@l
    mr r3, r5
    bl InitMetroTRKCommTable
    cmpwi r3, 0x1
    bne L_8025D010
    lwz r4, 0x84(r3)
    mtlr r4
    opword 0xB8030000
    blr
L_8025D010:
    b TRK_main
    blr
}

#pragma weak InitMetroTRK_BBA
asm void InitMetroTRK_BBA(void) {
    nofralloc
    subi r1, r1, 0x4
    stw r3, 0x0(r1)
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    stmw r0, 0x0(r3)
    lwz r4, 0x0(r1)
    addi r1, r1, 0x4
    stw r1, 0x4(r3)
    stw r4, 0xc(r3)
    mflr r4
    stw r4, 0x84(r3)
    stw r4, 0x80(r3)
    mfcr r4
    stw r4, 0x88(r3)
    mfmsr r4
    ori r3, r4, 0x8000
    mtmsr r3
    mtsrr1 r4
    bl TRKSaveExtended1Block
    lis r3, gTRKCPUState@h
    ori r3, r3, gTRKCPUState@l
    opword 0xB8030000
    li r0, 0x0
    mtspr 0x3F2, r0
    mtspr 0x3F5, r0
    lis r1, _db_stack_addr@h
    ori r1, r1, _db_stack_addr@l
    li r3, 0x2
    bl InitMetroTRKCommTable
    cmpwi r3, 0x1
    bne L_8025D0A4
    lwz r4, 0x84(r3)
    mtlr r4
    opword 0xB8030000
    blr
L_8025D0A4:
    b TRK_main
    blr
}

asm void TRK__write_aram(register void* src, register u32 dst, register u32* n) {
    nofralloc
    clrlwi r11, r1, 27
    mr r12, r1
    subfic r11, r11, -0x80
    stwux r1, r1, r11
    mflr r0
    stw r0, 0x4(r12)
    stmw r23, -0x24(r12)
    mr r31, r4
    cmplwi r31, 0x4000
    mr r30, r3
    mr r23, r5
    blt L_8025D280
    lwz r4, 0x0(r23)
    lis r0, 0x800
    add r3, r31, r4
    cmplw r3, r0
    ble L_8025D0F4
    b L_8025D280
L_8025D0F4:
    clrlwi r0, r31, 27
    clrrwi r27, r31, 5
    add r26, r4, r0
    li r24, 0x0
    addi r0, r26, 0x1f
    clrrwi r26, r0, 5
    cmplwi r26, 0x0
    addi r3, r26, 0x1f
    srwi r3, r3, 5
    ble L_8025D184
    srwi. r0, r3, 3
    mtctr r0
    beq L_8025D174
L_8025D128:
    dcbf r24, r30
    addi r24, r24, 0x20
    dcbf r24, r30
    addi r24, r24, 0x20
    dcbf r24, r30
    addi r24, r24, 0x20
    dcbf r24, r30
    addi r24, r24, 0x20
    dcbf r24, r30
    addi r24, r24, 0x20
    dcbf r24, r30
    addi r24, r24, 0x20
    dcbf r24, r30
    addi r24, r24, 0x20
    dcbf r24, r30
    addi r24, r24, 0x20
    bdnz L_8025D128
    andi. r3, r3, 0x7
    beq L_8025D184
L_8025D174:
    mtctr r3
L_8025D178:
    dcbf r24, r30
    addi r24, r24, 0x20
    bdnz L_8025D178
L_8025D184:
    bl ARGetDMAStatus
    cmplwi r3, 0x0
    bne L_8025D184
    bl __ARGetInterruptStatus
    clrlwi. r24, r31, 27
    mr r29, r3
    lis r25, 0x800
    beq L_8025D1E8
    addi r28, r1, 0x20
    mr r25, r27
    dcbi r0, r28
    bl __ARClearInterrupt
    mr r4, r28
    mr r5, r27
    li r3, 0x1
    li r6, 0x20
    bl ARStartDMA
L_8025D1C8:
    bl __ARGetInterruptStatus
    clrlwi. r0, r3, 16
    beq L_8025D1C8
    mr r3, r30
    mr r5, r24
    addi r4, r1, 0x20
    bl TRK_memcpy
    dcbf r0, r30
L_8025D1E8:
    lwz r0, 0x0(r23)
    add r31, r31, r0
    clrlwi. r24, r31, 27
    beq L_8025D24C
    clrrwi r23, r31, 5
    cmplw r23, r25
    beq L_8025D230
    addi r28, r1, 0x20
    dcbi r0, r28
    bl __ARClearInterrupt
    mr r4, r28
    mr r5, r23
    li r3, 0x1
    li r6, 0x20
    bl ARStartDMA
L_8025D224:
    bl __ARGetInterruptStatus
    clrlwi. r0, r3, 16
    beq L_8025D224
L_8025D230:
    add r25, r30, r31
    addi r4, r1, 0x20
    mr r3, r25
    subfic r5, r24, 0x20
    add r4, r4, r24
    bl TRK_memcpy
    dcbf r0, r25
L_8025D24C:
    sync
    bl __ARClearInterrupt
    mr r4, r30
    mr r5, r27
    mr r6, r26
    li r3, 0x0
    bl ARStartDMA
    clrlwi. r0, r29, 16
    bne L_8025D280
L_8025D270:
    bl __ARGetInterruptStatus
    clrlwi. r0, r3, 16
    beq L_8025D270
    bl __ARClearInterrupt
L_8025D280:
    lwz r10, 0x0(r1)
    lmw r23, -0x24(r10)
    lwz r0, 0x4(r10)
    mtlr r0
    mr r1, r10
    blr
}

void EnableMetroTRKInterrupts(void) {
    EnableEXI2Interrupts();
}

u32 TRKTargetTranslate(u32 addr) {
    if (addr >= lc_base) {
        if ((addr < lc_base + 0x4000) && ((gTRKCPUState.DBAT3U & 3) != 0))
            return addr;
    }

    if (addr >= 0x7E000000 && addr <= 0x80000000)
        return addr;

    return (addr & 0x3FFFFFFF) | 0x80000000;
}

void TRK_copy_vector(u32 offset) {
    void* destPtr = (void*)TRKTargetTranslate(offset);
    TRK_memcpy(destPtr, gTRKInterruptVectorTable + offset, 0x100);
    TRK_flush_cache(destPtr, 0x100);
}

void __TRK_copy_vectors(void) {
    int i;
    u32 mask;
    u32* maskPtr;

    if (lc_base <= 0x44 && lc_base + 0x4000 > 0x44 && (gTRKCPUState.DBAT3U & 3))
        maskPtr = (u32*)0x44;
    else
        maskPtr = (u32*)0x80000044;

    mask = *maskPtr;

    for (i = 0; i <= 14; ++i) {
        if ((mask & (1 << i)) && i != 4)
            TRK_copy_vector(TRK_ISR_OFFSETS[i]);
    }
}

DSError TRKInitializeTarget(void) {
    gTRKState.isStopped = 1;
    gTRKState.msr = __TRK_get_MSR();
    lc_base = 0xE0000000;
    return 0;
}

void TRK__read_aram(register void* dst, register u32 src, register u32* n) {
    u32 start;
    u32 length;
    register u32 i;
    u16 status;

    if (src < 0x4000 || src + *n > 0x8000000)
        return;

    start = src & ~31;
    length = *n + (src & 31);
    length = (length + 31) & ~31;

    for (i = 0; i < length; i += 32) {
        asm { dcbi i, dst }
    }

    while (ARGetDMAStatus()) {}

    status = __ARGetInterruptStatus();
    __ARClearInterrupt();
    ARStartDMA(1, (u32)dst, start, length);
    while (__ARGetInterruptStatus() == 0) {}

    if (status == 0)
        __ARClearInterrupt();
}

__declspec(section ".init") void __TRK_reset(void) {
    OSResetSystem(0, 0, 0);
}
