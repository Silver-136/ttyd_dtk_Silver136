#include <dolphin.h>
#include <dolphin/os.h>
#include <dolphin/hw_regs.h>

#include "__os.h"

#if DEBUG
u64 __OSSpuriousInterrupts = 0;
#endif
static __OSInterruptHandler* InterruptHandlerTable;

volatile OSTime __OSLastInterruptTime;
volatile __OSInterrupt __OSLastInterrupt;
volatile u32 __OSLastInterruptSrr0;

static OSInterruptMask InterruptPrioTable[] = {
    OS_INTERRUPTMASK_PI_ERROR,
    OS_INTERRUPTMASK_PI_DEBUG,
    OS_INTERRUPTMASK_MEM,
    OS_INTERRUPTMASK_PI_RSW,
    OS_INTERRUPTMASK_PI_VI,
    OS_INTERRUPTMASK_PI_PE,
    OS_INTERRUPTMASK_PI_HSP,
    OS_INTERRUPTMASK_DSP_ARAM | OS_INTERRUPTMASK_DSP_DSP | OS_INTERRUPTMASK_AI |
        OS_INTERRUPTMASK_EXI | OS_INTERRUPTMASK_PI_SI | OS_INTERRUPTMASK_PI_DI,
    OS_INTERRUPTMASK_DSP_AI,
    OS_INTERRUPTMASK_PI_CP,
    0xFFFFFFFF, // matches any cause left, ends the search in __OSDispatchInterrupt
};

#if DEBUG
char* __OSInterruptNames[33] = {
    "MEM_0",
    "MEM_1",
    "MEM_2",
    "MEM_3",
    "MEM_ADDRESS",
    "DSP_AI",
    "DSP_ARAM",
    "DSP_DSP",
    "AI_AI",
    "EXI_0_EXI",
    "EXI_0_TC",
    "EXI_0_EXT",
    "EXI_1_EXI",
    "EXI_1_TC",
    "EXI_1_EXT",
    "EXI_2_EXI",
    "EXI_2_TC",
    "PI_CP",
    "PI_PE_TOKEN",
    "PI_PE_FINISH",
    "PI_SI",
    "PI_DI",
    "PI_RSW",
    "PI_ERROR",
    "PI_VI",
    "PI_DEBUG",
    "PI_HSP",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown"
};

char* __OSPIErrors[8] = {
    "No Error",
    "Misaligned address for CPU request",
    "Incorrect transfer type (tt) from CPU",
    "Unsupported transfer size",
    "Address out of range",
    "Write to ROM address space",
    "Read from GX Fifo",
    "Reserved error code",
};
#endif

// prototypes
static void ExternalInterruptHandler(register __OSException exception, register OSContext* context);
extern void __RAS_OSDisableInterrupts_begin(void);
extern void __RAS_OSDisableInterrupts_end(void);

#ifdef __GEKKO__
asm BOOL OSDisableInterrupts(void) {
    nofralloc
entry    __RAS_OSDisableInterrupts_begin
    mfmsr   r3
    rlwinm  r4, r3, 0, 17, 15
    mtmsr   r4
entry    __RAS_OSDisableInterrupts_end
    rlwinm  r3, r3, 17, 31, 31
    blr
}

asm BOOL OSEnableInterrupts(void) {
    nofralloc

    mfmsr   r3
    ori     r4, r3, MSR_EE
    mtmsr   r4
    rlwinm  r3, r3, 17, 31, 31
    blr
}

asm BOOL OSRestoreInterrupts(register BOOL level) {
    nofralloc

    cmpwi   level, 0
    mfmsr   r4
    beq     _disable
    ori     r5, r4, MSR_EE
    b       _restore
_disable:
    rlwinm  r5, r4, 0, 17, 15
_restore:
    mtmsr   r5
    rlwinm  r3, r4, 17, 31, 31
    blr
}
#endif

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler) {
    __OSInterruptHandler oldHandler;
 
    ASSERTMSGLINE(411, InterruptHandlerTable, "__OSSetInterruptHandler(): OSInit() must be called in advance.");
    ASSERTMSGLINE(413, interrupt < __OS_INTERRUPT_MAX, "__OSSetInterruptHandler(): unknown interrupt.");

    oldHandler = InterruptHandlerTable[interrupt];
    InterruptHandlerTable[interrupt] = handler;
    return oldHandler;
}

__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt) {
    ASSERTMSGLINE(433, InterruptHandlerTable, "__OSGetInterruptHandler(): OSInit() must be called in advance.");
    ASSERTMSGLINE(435, interrupt < __OS_INTERRUPT_MAX, "__OSGetInterruptHandler(): unknown interrupt.");
    return InterruptHandlerTable[interrupt];
}

void __OSInterruptInit(void) {
    InterruptHandlerTable = (void*)OSPhysicalToCached(0x3040);

    memset(InterruptHandlerTable, 0, __OS_INTERRUPT_MAX * sizeof(__OSInterruptHandler));

    *(OSInterruptMask*)OSPhysicalToCached(0x00C4) = 0;
    *(OSInterruptMask*)OSPhysicalToCached(0x00C8) = 0;

    __PIRegs[PI_INTMR] = PI_INT_MEM | PI_INT_DSP | PI_INT_AI | PI_INT_EXI;

    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM | OS_INTERRUPTMASK_DSP | OS_INTERRUPTMASK_AI |
                        OS_INTERRUPTMASK_EXI | OS_INTERRUPTMASK_PI);

    __OSSetExceptionHandler(__OS_EXCEPTION_EXTERNAL_INTERRUPT, ExternalInterruptHandler);
    #if DEBUG
        __PIRegs[PI_INTSR] = PI_INT_ERROR;
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_ERROR);
    #endif
}

static u32 SetInterruptMask(OSInterruptMask mask, OSInterruptMask current) {
    u32 reg;

    switch (__cntlzw(mask)) {
    case __OS_INTERRUPT_MEM_0:
    case __OS_INTERRUPT_MEM_1:
    case __OS_INTERRUPT_MEM_2:
    case __OS_INTERRUPT_MEM_3:
    case __OS_INTERRUPT_MEM_ADDRESS:
        reg = 0;
        if (!(current & OS_INTERRUPTMASK_MEM_0))
            reg |= MI_IRQ_REG0;
        if (!(current & OS_INTERRUPTMASK_MEM_1))
            reg |= MI_IRQ_REG1;
        if (!(current & OS_INTERRUPTMASK_MEM_2))
            reg |= MI_IRQ_REG2;
        if (!(current & OS_INTERRUPTMASK_MEM_3))
            reg |= MI_IRQ_REG3;
        if (!(current & OS_INTERRUPTMASK_MEM_ADDRESS))
            reg |= MI_IRQ_ALL_REGS;
        __MEMRegs[MI_IRQMASK] = (u16)reg;
        mask &= ~OS_INTERRUPTMASK_MEM;
        break;
    case __OS_INTERRUPT_DSP_AI:
    case __OS_INTERRUPT_DSP_ARAM:
    case __OS_INTERRUPT_DSP_DSP:
        reg = __DSPRegs[DSP_CONTROL_STATUS];
        reg &= ~(DSP_CSR_AID_INT | DSP_CSR_AID_INT_MASK | DSP_CSR_ARAM_INT | DSP_CSR_ARAM_INT_MASK | DSP_CSR_DSP_INT | DSP_CSR_DSP_INT_MASK);
        if (!(current & OS_INTERRUPTMASK_DSP_AI))
            reg |= DSP_CSR_AID_INT_MASK;
        if (!(current & OS_INTERRUPTMASK_DSP_ARAM))
            reg |= DSP_CSR_ARAM_INT_MASK;
        if (!(current & OS_INTERRUPTMASK_DSP_DSP))
            reg |= DSP_CSR_DSP_INT_MASK;
        __DSPRegs[DSP_CONTROL_STATUS] = (u16)reg;
        mask &= ~OS_INTERRUPTMASK_DSP;
        break;
    case __OS_INTERRUPT_AI_AI:
        reg = __AIRegs[AI_CR];
        reg &= ~(AI_CR_SCRESET | AI_CR_AIINT | AI_CR_AIINTMSK);
        if (!(current & OS_INTERRUPTMASK_AI_AI))
            reg |= AI_CR_AIINTMSK;
        __AIRegs[AI_CR] = reg;
        mask &= ~OS_INTERRUPTMASK_AI;
        break;
    case __OS_INTERRUPT_EXI_0_EXI:
    case __OS_INTERRUPT_EXI_0_TC:
    case __OS_INTERRUPT_EXI_0_EXT:
        reg = __EXIRegs[EXI0_CSR];
        reg &= ~0x2C0F;
        if (!(current & OS_INTERRUPTMASK_EXI_0_EXI))
            reg |= EXI_CSR_EXIINTMASK;
        if (!(current & OS_INTERRUPTMASK_EXI_0_TC))
            reg |= EXI_CSR_TCINTMASK;
        if (!(current & OS_INTERRUPTMASK_EXI_0_EXT))
            reg |= EXI_CSR_EXTINTMASK;
        __EXIRegs[EXI0_CSR] = reg;
        mask &= ~OS_INTERRUPTMASK_EXI_0;
        break;
    case __OS_INTERRUPT_EXI_1_EXI:
    case __OS_INTERRUPT_EXI_1_TC:
    case __OS_INTERRUPT_EXI_1_EXT:
        reg = __EXIRegs[EXI1_CSR];
        reg &= ~0xC0F;

        if (!(current & OS_INTERRUPTMASK_EXI_1_EXI))
            reg |= EXI_CSR_EXIINTMASK;
        if (!(current & OS_INTERRUPTMASK_EXI_1_TC))
            reg |= EXI_CSR_TCINTMASK;
        if (!(current & OS_INTERRUPTMASK_EXI_1_EXT))
            reg |= EXI_CSR_EXTINTMASK;
        __EXIRegs[EXI1_CSR] = reg;
        mask &= ~OS_INTERRUPTMASK_EXI_1;
        break;
    case __OS_INTERRUPT_EXI_2_EXI:
    case __OS_INTERRUPT_EXI_2_TC:
        reg = __EXIRegs[EXI2_CSR];
        reg &= ~0xF;
        if (!(current & OS_INTERRUPTMASK_EXI_2_EXI))
            reg |= EXI_CSR_EXIINTMASK;
        if (!(current & OS_INTERRUPTMASK_EXI_2_TC))
            reg |= EXI_CSR_TCINTMASK;

        __EXIRegs[EXI2_CSR] = reg;
        mask &= ~OS_INTERRUPTMASK_EXI_2;
        break;
    case __OS_INTERRUPT_PI_CP:
    case __OS_INTERRUPT_PI_SI:
    case __OS_INTERRUPT_PI_DI:
    case __OS_INTERRUPT_PI_RSW:
    case __OS_INTERRUPT_PI_ERROR:
    case __OS_INTERRUPT_PI_VI:
    case __OS_INTERRUPT_PI_DEBUG:
    case __OS_INTERRUPT_PI_PE_TOKEN:
    case __OS_INTERRUPT_PI_PE_FINISH:
    case __OS_INTERRUPT_PI_HSP:
        reg = PI_INT_MEM | PI_INT_DSP | PI_INT_AI | PI_INT_EXI;

        if (!(current & OS_INTERRUPTMASK_PI_CP)) {
            reg |= PI_INT_CP;
        }
        if (!(current & OS_INTERRUPTMASK_PI_SI)) {
            reg |= PI_INT_SI;
        }
        if (!(current & OS_INTERRUPTMASK_PI_DI)) {
            reg |= PI_INT_DI;
        }
        if (!(current & OS_INTERRUPTMASK_PI_RSW)) {
            reg |= PI_INT_RSW;
        }
        if (!(current & OS_INTERRUPTMASK_PI_ERROR)) {
            reg |= PI_INT_ERROR;
        }
        if (!(current & OS_INTERRUPTMASK_PI_VI)) {
            reg |= PI_INT_VI;
        }
        if (!(current & OS_INTERRUPTMASK_PI_DEBUG)) {
            reg |= PI_INT_DEBUG;
        }
        if (!(current & OS_INTERRUPTMASK_PI_PE_TOKEN)) {
            reg |= PI_INT_PE_TOKEN;
        }
        if (!(current & OS_INTERRUPTMASK_PI_PE_FINISH)) {
            reg |= PI_INT_PE_FINISH;
        }
        if (!(current & OS_INTERRUPTMASK_PI_HSP)) {
            reg |= PI_INT_HSP;
        }
        __PIRegs[PI_INTMR] = reg;
        mask &= ~OS_INTERRUPTMASK_PI;
        break;
    default:
        break;
    }
    return mask;
}

OSInterruptMask OSGetInterruptMask(void) { 
    return *(OSInterruptMask*)OSPhysicalToCached(0x00C8);
}

OSInterruptMask OSSetInterruptMask(OSInterruptMask local) {
    BOOL enabled;
    OSInterruptMask global;
    OSInterruptMask prev;
    OSInterruptMask mask;

    enabled = OSDisableInterrupts();
    global = *(OSInterruptMask *)OSPhysicalToCached(0x00C4);
    prev = *(OSInterruptMask *)OSPhysicalToCached(0x00C8);
    mask = (global | prev) ^ local;
    *(OSInterruptMask *)OSPhysicalToCached(0x00C8) = local;
    while (mask) {
        mask = SetInterruptMask(mask, global | local);
    }
    OSRestoreInterrupts(enabled);
    return prev;
}

OSInterruptMask __OSMaskInterrupts(OSInterruptMask global) {
    BOOL enabled;
    OSInterruptMask prev;
    OSInterruptMask local;
    OSInterruptMask mask;

    enabled = OSDisableInterrupts();
    prev = *(OSInterruptMask *)OSPhysicalToCached(0x00C4);
    local = *(OSInterruptMask *)OSPhysicalToCached(0x00C8);
    mask = ~(prev | local) & global;
    global |= prev;
    *(OSInterruptMask *)OSPhysicalToCached(0x00C4) = global;
    while (mask) {
        mask = SetInterruptMask(mask, global | local);
    }
    OSRestoreInterrupts(enabled);
    return prev;
}

OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask global) {
    BOOL enabled;
    OSInterruptMask prev;
    OSInterruptMask local;
    OSInterruptMask mask;

    enabled = OSDisableInterrupts();
    prev = *(OSInterruptMask *)OSPhysicalToCached(0x00C4);
    local = *(OSInterruptMask *)OSPhysicalToCached(0x00C8);
    mask = (prev | local) & global;
    global = prev & ~global;
    *(OSInterruptMask *)OSPhysicalToCached(0x00C4) = global;
    while (mask) {
        mask = SetInterruptMask(mask, global | local);
    }
    OSRestoreInterrupts(enabled);
    return prev;
}

void __OSDispatchInterrupt(__OSException exception, OSContext* context) {
    u32 intsr;
    u32 reg;
    OSInterruptMask cause;
    OSInterruptMask unmasked;
    OSInterruptMask* prio;
    __OSInterrupt interrupt;
    __OSInterruptHandler handler;

    intsr = __PIRegs[PI_INTSR];
    intsr &= ~PI_INTSR_RSWST;

    if (intsr == 0 || (intsr & __PIRegs[PI_INTMR]) == 0) {
    #if DEBUG
        __OSSpuriousInterrupts++;
    #endif
        OSLoadContext(context);
    }

    cause = 0;

    if (intsr & PI_INT_MEM) {
        reg = __MEMRegs[MI_IRQFLAG];
        if (reg & MI_IRQ_REG0)
            cause |= OS_INTERRUPTMASK_MEM_0;
        if (reg & MI_IRQ_REG1)
            cause |= OS_INTERRUPTMASK_MEM_1;
        if (reg & MI_IRQ_REG2)
            cause |= OS_INTERRUPTMASK_MEM_2;
        if (reg & MI_IRQ_REG3)
            cause |= OS_INTERRUPTMASK_MEM_3;
        if (reg & MI_IRQ_ALL_REGS)
            cause |= OS_INTERRUPTMASK_MEM_ADDRESS;
    }

    if (intsr & PI_INT_DSP) {
        reg = __DSPRegs[DSP_CONTROL_STATUS];
        if (reg & DSP_CSR_AID_INT)
            cause |= OS_INTERRUPTMASK_DSP_AI;
        if (reg & DSP_CSR_ARAM_INT)
            cause |= OS_INTERRUPTMASK_DSP_ARAM;
        if (reg & DSP_CSR_DSP_INT)
            cause |= OS_INTERRUPTMASK_DSP_DSP;
    }

    if (intsr & PI_INT_AI) {
        reg = __AIRegs[AI_CR];
        if (reg & AI_CR_AIINT)
            cause |= OS_INTERRUPTMASK_AI_AI;
    }

    if (intsr & PI_INT_EXI) {
        reg = __EXIRegs[EXI0_CSR];
        if (reg & EXI_CSR_EXIINT)
            cause |= OS_INTERRUPTMASK_EXI_0_EXI;
        if (reg & EXI_CSR_TCINT)
            cause |= OS_INTERRUPTMASK_EXI_0_TC;
        if (reg & EXI_CSR_EXTINT)
            cause |= OS_INTERRUPTMASK_EXI_0_EXT;
        reg = __EXIRegs[EXI1_CSR];
        if (reg & EXI_CSR_EXIINT)
            cause |= OS_INTERRUPTMASK_EXI_1_EXI;
        if (reg & EXI_CSR_TCINT)
            cause |= OS_INTERRUPTMASK_EXI_1_TC;
        if (reg & EXI_CSR_EXTINT)
            cause |= OS_INTERRUPTMASK_EXI_1_EXT;
        reg = __EXIRegs[EXI2_CSR];
        if (reg & EXI_CSR_EXIINT)
            cause |= OS_INTERRUPTMASK_EXI_2_EXI;
        if (reg & EXI_CSR_TCINT)
            cause |= OS_INTERRUPTMASK_EXI_2_TC;
    }

    if (intsr & PI_INT_HSP)
        cause |= OS_INTERRUPTMASK_PI_HSP;
    if (intsr & PI_INT_DEBUG)
        cause |= OS_INTERRUPTMASK_PI_DEBUG;
    if (intsr & PI_INT_PE_FINISH)
        cause |= OS_INTERRUPTMASK_PI_PE_FINISH;
    if (intsr & PI_INT_PE_TOKEN)
        cause |= OS_INTERRUPTMASK_PI_PE_TOKEN;
    if (intsr & PI_INT_VI)
        cause |= OS_INTERRUPTMASK_PI_VI;
    if (intsr & PI_INT_SI)
        cause |= OS_INTERRUPTMASK_PI_SI;
    if (intsr & PI_INT_DI)
        cause |= OS_INTERRUPTMASK_PI_DI;
    if (intsr & PI_INT_RSW)
        cause |= OS_INTERRUPTMASK_PI_RSW;
    if (intsr & PI_INT_CP)
        cause |= OS_INTERRUPTMASK_PI_CP;
    if (intsr & PI_INT_ERROR)
        cause |= OS_INTERRUPTMASK_PI_ERROR;

    #if DEBUG
    if (cause & OS_INTERRUPTMASK_PI_ERROR) {
        OSReport("PI ERROR\n");
        OSDumpContext(context);
        OSReport("\nPIESR = 0x%08x                  PIEAR  = 0x%08x\n", __PIRegs[PI_ERROR_CAUSE], __PIRegs[PI_ERROR_ADDRESS]);
        __PIRegs[PI_INTSR] = PI_INT_ERROR;
        OSReport("PI Error = %s\n", __OSPIErrors[__PIRegs[PI_ERROR_CAUSE]]);
        OSReport("Offending address = 0x%x (from PIEAR)\n", __PIRegs[PI_ERROR_ADDRESS]);
    }
    #endif

    unmasked = cause & ~(*(OSInterruptMask *)OSPhysicalToCached(0x00C4) |
                        *(OSInterruptMask *)OSPhysicalToCached(0x00C8));
    if (unmasked) {
        for (prio = InterruptPrioTable;; ++prio) {
            if (unmasked & *prio) {
                interrupt = (__OSInterrupt)__cntlzw(unmasked & *prio);
                break;
            }
        }

        handler = __OSGetInterruptHandler(interrupt);
        if (handler) {
            if (__OS_INTERRUPT_MEM_ADDRESS < interrupt) {
                __OSLastInterrupt = interrupt;
                __OSLastInterruptTime = OSGetTime();
                __OSLastInterruptSrr0 = context->srr0;
            }
            OSDisableScheduler();
            handler(interrupt, context);
            OSEnableScheduler();
            __OSReschedule();
            OSLoadContext(context);
        }
    }

    #if DEBUG
    OSReport("Unhandled Interrupt(s): cause %08x  intsr %08x\n", cause, intsr);
    while (cause) {
        interrupt = __cntlzw(cause);
        cause &= ~(1 << (0x1F - __cntlzw(cause)));
        OSReport("    %s\n", __OSInterruptNames[interrupt]);
    }
    #endif

    OSLoadContext(context);
}

#ifdef __GEKKO__
static asm void ExternalInterruptHandler(register __OSException exception,
                                         register OSContext* context) {
#pragma unused(exception)
    nofralloc 
    OS_EXCEPTION_SAVE_GPRS(context)

    stwu r1, -0x8(r1)
    b __OSDispatchInterrupt
}
#endif
