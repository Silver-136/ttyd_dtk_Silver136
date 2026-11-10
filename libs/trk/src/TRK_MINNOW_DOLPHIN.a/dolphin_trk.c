typedef int BOOL;
typedef int DSError;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#include "targimpl.h"

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

extern ProcessorState_PPC gTRKCPUState;
extern TRKState_PPC gTRKState;

static u32 TRK_ISR_OFFSETS[15] = {0x100, 0x200, 0x300, 0x400, 0x500, 0x600, 0x700, 0x800,
                                  0x900, 0xC00, 0xD00, 0xF00, 0x1300, 0x1400, 0x1700};

static u32 lc_base;

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
    bne done
    // BUG: r3 is 1 here (the compared result), not gTRKCPUState, so this loads from address 0x85
    lwz r4, 0x84(r3)
    mtlr r4
    opword 0xB8030000
    blr
done:
    b TRK_main
    blr
}

/* Differs from InitMetroTRK: sets MSR bit 0x8000 (EE) instead of clearing it, and passes 2 to
   InitMetroTRKCommTable instead of r5. */
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
    bne done
    // BUG: as in InitMetroTRK, r3 is 1 here
    lwz r4, 0x84(r3)
    mtlr r4
    opword 0xB8030000
    blr
done:
    b TRK_main
    blr
}

void EnableMetroTRKInterrupts(void) {
    EnableEXI2Interrupts();
}

u32 TRKTargetTranslate(u32 addr) {
    if (addr >= lc_base) {
        // 0x4000 = the 16 KB locked cache at lc_base, mapped when DBAT3 is valid
        if ((addr < lc_base + 0x4000) && ((gTRKCPUState.Extended1.DBAT3U & 3) != 0))
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

    // the word at 0x44 is a mask: bit i set = copy the vector at TRK_ISR_OFFSETS[i] (4, the 0x500 external
    // interrupt, is never copied); lc_base is always 0xE0000000 (TRKInitializeTarget), so the first branch is
    // never taken
    if (lc_base <= 0x44 && lc_base + 0x4000 > 0x44 && (gTRKCPUState.Extended1.DBAT3U & 3))
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
    gTRKState.MSR = __TRK_get_MSR();
    lc_base = 0xE0000000;
    return 0;
}

void TRK__read_aram(register void* dst, register u32 src, register u32* n) {
    u32 start;
    u32 length;
    register u32 i;
    u16 status;

    // BUG: an out-of-range request returns without an error and leaves *n unchanged
    if (src < 0x4000 || src + *n > 0x8000000)
        return;

    start = src & ~31;
    length = *n + (src & 31);
    length = (length + 31) & ~31;

    // dcbi has no C form
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

/* Writes *n bytes to ARAM at dst with one DMA of whole 32-byte blocks: length bytes from src to the first block
   (dst & ~31), so src stands for that block. When the range does not start or end on a block boundary, the ARAM
   block there is read into buf first and its bytes outside the range are copied into main memory, so the DMA
   writes them back unchanged. Form after the matched GC 2.6 decompilations of pikmin2 and ac-decomp (CC0). */
void TRK__write_aram(void* src, u32 dst, u32* n) {
    u8 buf[32] __attribute__((aligned(32)));
    u32 status;
    register u32 bufAddr;
    u32 start;
    u32 length;
    u16 oldStatus;
    u32 firstBlock;
    int offset;
    u32 i;
    u8* tail;

    // BUG: an out-of-range request returns without an error and writes nothing
    if (dst < 0x4000 || dst + *n > 0x8000000)
        return;

    start = dst & ~31;
    offset = 0;
    length = *n + (dst & 31);
    length = (length + 31) & ~31;

    for (i = 0; i < length; i += 32) {
        __dcbf((void*)offset, (int)src);
        offset += 32;
    }

    do {
        status = ARGetDMAStatus();
    } while (status);

    oldStatus = __ARGetInterruptStatus();
    firstBlock = 0x8000000;

    offset = dst & 31;
    if (offset) {
        // the first block starts before dst: copy the ARAM bytes in front of dst to src
        firstBlock = start;
        bufAddr = (u32)buf;
        asm { dcbi r0, bufAddr }
        __ARClearInterrupt();
        ARStartDMA(1, bufAddr, start, 32);
        while (__ARGetInterruptStatus() == 0) {}

        TRK_memcpy(src, buf, offset);
        __dcbf(src, 0);
    }

    dst += *n;
    offset = dst & 31;
    if (offset) {
        // the last block ends after the range: the ARAM bytes behind it (buf already holds the block when the
        // range starts and ends in the same one)
        u32 lastBlock = dst & ~31;
        if (lastBlock != firstBlock) {
            bufAddr = (u32)buf;
            asm { dcbi r0, bufAddr }
            __ARClearInterrupt();
            ARStartDMA(1, bufAddr, lastBlock, 32);
            while (__ARGetInterruptStatus() == 0) {}
        }

        // BUG: dst is the ARAM end address here, so the bytes go to src plus that address, not to the end of the
        // range in main memory (src + dst - start)
        tail = (u8*)src + dst;
        TRK_memcpy(tail, buf + offset, 32 - offset);
        __dcbf(tail, 0);
    }

    __sync();
    __ARClearInterrupt();
    ARStartDMA(0, (u32)src, start, length);
    if (oldStatus == 0) {
        while (__ARGetInterruptStatus() == 0) {}
        __ARClearInterrupt();
    }
}

__declspec(section ".init") void __TRK_reset(void) {
    OSResetSystem(0, 0, 0);
}
