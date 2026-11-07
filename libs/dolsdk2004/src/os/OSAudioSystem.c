#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/hw_regs.h>

#include "__os.h"

static u8 DSPInitCode[128] = {
    0x02, 0x9F, 0x00, 0x10, 0x02, 0x9F, 0x00, 0x33, 0x02, 0x9F, 0x00, 0x34, 0x02, 0x9F, 0x00, 0x35,
    0x02, 0x9F, 0x00, 0x36, 0x02, 0x9F, 0x00, 0x37, 0x02, 0x9F, 0x00, 0x38, 0x02, 0x9F, 0x00, 0x39,
    0x12, 0x06, 0x12, 0x03, 0x12, 0x04, 0x12, 0x05, 0x00, 0x80, 0x80, 0x00, 0x00, 0x88, 0xFF, 0xFF,
    0x00, 0x84, 0x10, 0x00, 0x00, 0x64, 0x00, 0x1D, 0x02, 0x18, 0x00, 0x00, 0x81, 0x00, 0x1C, 0x1E,
    0x00, 0x44, 0x1B, 0x1E, 0x00, 0x84, 0x08, 0x00, 0x00, 0x64, 0x00, 0x27, 0x19, 0x1E, 0x00, 0x00,
    0x00, 0xDE, 0xFF, 0xFC, 0x02, 0xA0, 0x80, 0x00, 0x02, 0x9C, 0x00, 0x28, 0x16, 0xFC, 0x00, 0x54,
    0x16, 0xFD, 0x43, 0x48, 0x00, 0x21, 0x02, 0xFF, 0x02, 0xFF, 0x02, 0xFF, 0x02, 0xFF, 0x02, 0xFF,
    0x02, 0xFF, 0x02, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define __DSPWorkBuffer (void*)0x81000000

void __OSInitAudioSystem(void) {
    u8 errFlag; // assigned once, never read; its stack slot is in the game code
    u16 reg16;
    u32 start_tick;

    // Save the work buffer below the arena top, then put the DSP init program there.
    memcpy((void*)((u32)OSGetArenaHi() - 0x80), __DSPWorkBuffer, sizeof(DSPInitCode));
    memcpy(__DSPWorkBuffer, (void*)DSPInitCode, sizeof(DSPInitCode));
    DCFlushRange(__DSPWorkBuffer, sizeof(DSPInitCode));

    __DSPRegs[DSP_ARAM_SIZE] = 0x43;
    ASSERTMSGLINE(113, !(__DSPRegs[DSP_CONTROL_STATUS] & 0x200), "__OSInitAudioSystem(): ARAM DMA already in progress");
    ASSERTMSGLINE(117, !(__DSPRegs[DSP_CONTROL_STATUS] & 0x400), "__OSInitAudioSystem(): DSP DMA already in progress");
    ASSERTMSGLINE(121, (__DSPRegs[DSP_CONTROL_STATUS] & 0x004), "__OSInitAudioSystem(): DSP already working");
    
    __DSPRegs[DSP_CONTROL_STATUS] = 0x8AC;
    __DSPRegs[DSP_CONTROL_STATUS] |= 1;

    while (__DSPRegs[DSP_CONTROL_STATUS] & 1);
    __DSPRegs[DSP_MAILBOX_IN_HI] = 0;

    while (((__DSPRegs[DSP_MAILBOX_OUT_HI] << 16) | __DSPRegs[DSP_MAILBOX_OUT_LO]) & 0x80000000);

    *(u32*)&__DSPRegs[DSP_ARAM_DMA_MM_HI] = 0x1000000;
    *(u32*)&__DSPRegs[DSP_ARAM_DMA_ARAM_HI] = 0;
    *(u32*)&__DSPRegs[DSP_ARAM_DMA_SIZE_HI] = 0x20;

    reg16 = __DSPRegs[DSP_CONTROL_STATUS];
    while (!(reg16 & 0x20))
        reg16 = __DSPRegs[DSP_CONTROL_STATUS];

    __DSPRegs[DSP_CONTROL_STATUS] = reg16;

    start_tick = OSGetTick();
    while ((s32)(OSGetTick() - start_tick) < 0x892);

    *(u32*)&__DSPRegs[DSP_ARAM_DMA_MM_HI] = 0x1000000;
    *(u32*)&__DSPRegs[DSP_ARAM_DMA_ARAM_HI] = 0;
    *(u32*)&__DSPRegs[DSP_ARAM_DMA_SIZE_HI] = 0x20;

    reg16 = __DSPRegs[DSP_CONTROL_STATUS];
    while (!(reg16 & 0x20)) {
        reg16 = __DSPRegs[DSP_CONTROL_STATUS];
    }
    __DSPRegs[DSP_CONTROL_STATUS] = reg16;

    __DSPRegs[DSP_CONTROL_STATUS] &= ~0x800;
    while ((__DSPRegs[DSP_CONTROL_STATUS]) & 0x400);
    
    __DSPRegs[DSP_CONTROL_STATUS] &= ~4;
    errFlag = 0;

    reg16 = __DSPRegs[DSP_MAILBOX_OUT_HI];

    while (!(reg16 & 0x8000)) {
        reg16 = __DSPRegs[DSP_MAILBOX_OUT_HI];
    }

    // The init program answers with the mail word 0x80544348 (0x4348 after adding 0x7FAC0000).
    if (((u32)((reg16 << 16) | __DSPRegs[DSP_MAILBOX_OUT_LO]) + 0x7FAC0000U) != 0x4348) {
        ASSERTMSGLINE(193, 0, "__OSInitAudioSystem(): DSP returns invalid message");
    }

    // FAKE: dead comparison, no effect; without it the register allocation differs from the game code
    reg16 != 0x81800;
    __DSPRegs[DSP_CONTROL_STATUS] |= 4;
    __DSPRegs[DSP_CONTROL_STATUS] = 0x8AC;
    __DSPRegs[DSP_CONTROL_STATUS] |= 1;
    while (__DSPRegs[DSP_CONTROL_STATUS] & 1);

    // Restore the saved work buffer.
    memcpy(__DSPWorkBuffer, (void*)((u32)OSGetArenaHi() - 0x80), sizeof(DSPInitCode));
}

void __OSStopAudioSystem(void) {
    u16 reg16;
    u32 start_tick;

    #define waitUntil(load, mask)  \
        reg16 = (load);            \
        while (reg16 & (mask)) {   \
            reg16 = (load);        \
        }

    __DSPRegs[DSP_CONTROL_STATUS] = 0x804;
    reg16 = __DSPRegs[DSP_DMA_CONTROL_LEN];
    __DSPRegs[DSP_DMA_CONTROL_LEN] = reg16 & ~DSP_DMA_START_FLAG;
    waitUntil(__DSPRegs[DSP_CONTROL_STATUS], 0x400);
    waitUntil(__DSPRegs[DSP_CONTROL_STATUS], 0x200);
    __DSPRegs[DSP_CONTROL_STATUS] = 0x8ac;
    __DSPRegs[DSP_MAILBOX_IN_HI] = 0;

    while (((__DSPRegs[DSP_MAILBOX_OUT_HI] << 16) | __DSPRegs[DSP_MAILBOX_OUT_LO]) & 0x80000000);

    start_tick = OSGetTick();
    while ((s32)(OSGetTick() - start_tick) < 0x2c);

    reg16 = __DSPRegs[DSP_CONTROL_STATUS];
    __DSPRegs[DSP_CONTROL_STATUS] = reg16 | 1;
    waitUntil(__DSPRegs[DSP_CONTROL_STATUS], 0x001);

    #undef waitUntil
}
