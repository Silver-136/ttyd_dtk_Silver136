#ifndef _DOLPHIN_HW_REGS_H_
#define _DOLPHIN_HW_REGS_H_

#include <dolphin/types.h>

#ifdef __MWERKS__
volatile u16 __VIRegs[59]     AT_ADDRESS(0xCC002000);
volatile u32 __PIRegs[12]     AT_ADDRESS(0xCC003000);
volatile u16 __MEMRegs[64]    AT_ADDRESS(0xCC004000);
volatile u16 __DSPRegs[]      AT_ADDRESS(0xCC005000);
volatile u32 __DIRegs[]       AT_ADDRESS(0xCC006000);
volatile u32 __SIRegs[0x100]  AT_ADDRESS(0xCC006400);
volatile u32 __EXIRegs[0x40]  AT_ADDRESS(0xCC006800);
volatile u32 __AIRegs[8]      AT_ADDRESS(0xCC006C00);
#else
#define __VIRegs         ((volatile u16 *)0xCC002000)
#define __PIRegs         ((volatile u32 *)0xCC003000)
#define __MEMRegs        ((volatile u16 *)0xCC004000)
#define __DSPRegs        ((volatile u16 *)0xCC005000)
#define __DIRegs         ((volatile u32 *)0xCC006000)
#define __SIRegs         ((volatile u32 *)0xCC006400)
#define __EXIRegs        ((volatile u32 *)0xCC006800)
#define __AIRegs         ((volatile u32 *)0xCC006C00)
#endif

// Offsets for __VIRegs

// offsets for __VIRegs[i]
#define VI_VERT_TIMING        (0)
#define VI_DISP_CONFIG        (1)
#define VI_HORIZ_TIMING_0L    (2)
#define VI_HORIZ_TIMING_0U    (3)
#define VI_HORIZ_TIMING_1L    (4)
#define VI_HORIZ_TIMING_1U    (5)
#define VI_VERT_TIMING_ODD    (6)
#define VI_VERT_TIMING_ODD_U  (7)
#define VI_VERT_TIMING_EVEN   (8)
#define VI_VERT_TIMING_EVEN_U (9)

#define VI_BBI_ODD    (10) // burst blanking interval
#define VI_BBI_ODD_U  (11) // burst blanking interval
#define VI_BBI_EVEN   (12) // burst blanking interval
#define VI_BBI_EVEN_U (13) // burst blanking interval

#define VI_TOP_FIELD_BASE_LEFT   (14) // top in 2d, top of left pic in 3d
#define VI_TOP_FIELD_BASE_LEFT_U (15) // top in 2d, top of left pic in 3d

#define VI_TOP_FIELD_BASE_RIGHT   (16) // top of right pic in 3d
#define VI_TOP_FIELD_BASE_RIGHT_U (17) // top of right pic in 3d

#define VI_BTTM_FIELD_BASE_LEFT   (18) // bottom in 2d, bottom of left pic in 3d
#define VI_BTTM_FIELD_BASE_LEFT_U (19) // bottom in 2d, bottom of left pic in 3d

#define VI_BTTM_FIELD_BASE_RIGHT   (20) // bottom of right pic in 3d
#define VI_BTTM_FIELD_BASE_RIGHT_U (21) // bottom of right pic in 3d

#define VI_VERT_COUNT  (22) // vertical display position
#define VI_HORIZ_COUNT (23) // horizontal display position

#define VI_DISP_INT_0  (24) // display interrupt 0L
#define VI_DISP_INT_0U (25) // display interrupt 0U
#define VI_DISP_INT_1  (26) // display interrupt 1L
#define VI_DISP_INT_1U (27) // display interrupt 1U
#define VI_DISP_INT_2  (28) // display interrupt 2L
#define VI_DISP_INT_2U (29) // display interrupt 2U
#define VI_DISP_INT_3  (30) // display interrupt 3L
#define VI_DISP_INT_3U (31) // display interrupt 3U

#define VI_HSW (36) // horizontal scaling width
#define VI_HSR (37) // horizontal scaling register

#define VI_FCT_0  (38) // filter coefficient table 0L
#define VI_FCT_0U (39) // filter coefficient table 0U
#define VI_FCT_1  (40) // filter coefficient table 1L
#define VI_FCT_1U (41) // filter coefficient table 1U
#define VI_FCT_2  (42) // filter coefficient table 2L
#define VI_FCT_2U (43) // filter coefficient table 2U
#define VI_FCT_3  (44) // filter coefficient table 3L
#define VI_FCT_3U (45) // filter coefficient table 3U
#define VI_FCT_4  (46) // filter coefficient table 4L
#define VI_FCT_4U (47) // filter coefficient table 4U
#define VI_FCT_5  (48) // filter coefficient table 5L
#define VI_FCT_5U (49) // filter coefficient table 5U
#define VI_FCT_6  (50) // filter coefficient table 6L
#define VI_FCT_6U (51) // filter coefficient table 6U

#define VI_CLOCK_SEL (54) // clock select
#define VI_DTV_STAT  (55) // DTV status

#define VI_WIDTH (56)

// offsets for __DSPRegs[i]
#define DSP_MAILBOX_IN_HI  (0)
#define DSP_MAILBOX_IN_LO  (1)
#define DSP_MAILBOX_OUT_HI (2)
#define DSP_MAILBOX_OUT_LO (3)
#define DSP_CONTROL_STATUS (5)

// DSP_CONTROL_STATUS (DSP control register) bits. Names after Dolphin's UDSPControl (Source/Core/Core/HW/DSP.h):
// DSPReset, DSPAssertInt, DSPHalt, AID, AID_mask, ARAM, ARAM_mask, DSP, DSP_mask, DMAState, DSPInitCode, DSPInit.
// The interrupt bits are flags that are written back as 1 to acknowledge; each mask bit sits one above its flag.
#define DSP_CSR_RESET         (0x1)   // DSPReset
#define DSP_CSR_ASSERT_INT    (0x2)   // DSPAssertInt: raises the DSP's interrupt
#define DSP_CSR_HALT          (0x4)   // DSPHalt
#define DSP_CSR_AID_INT       (0x8)   // AID: audio interface DMA interrupt
#define DSP_CSR_AID_INT_MASK  (0x10)  // AID_mask: audio interface DMA interrupt enabled
#define DSP_CSR_ARAM_INT      (0x20)  // ARAM DMA interrupt; written back to acknowledge it
#define DSP_CSR_ARAM_INT_MASK (0x40)  // ARAM_mask
#define DSP_CSR_DSP_INT       (0x80)  // DSP: interrupt from the DSP (mailbox)
#define DSP_CSR_DSP_INT_MASK  (0x100) // DSP_mask
#define DSP_CSR_ARAM_DMA_BUSY (0x200) // DMAState: ARAM DMA in progress
#define DSP_CSR_INIT_CODE     (0x400) // DSPInitCode
#define DSP_CSR_INIT          (0x800) // DSPInit

#define DSP_ARAM_SIZE        (9)
#define DSP_ARAM_MODE        (11)
#define DSP_ARAM_REFRESH     (13)
#define DSP_ARAM_DMA_MM_HI   (16) // Main mem address
#define DSP_ARAM_DMA_MM_LO   (17)
#define DSP_ARAM_DMA_ARAM_HI (18) // ARAM address
#define DSP_ARAM_DMA_ARAM_LO (19)
#define DSP_ARAM_DMA_SIZE_HI (20) // DMA buffer size
#define DSP_ARAM_DMA_SIZE_LO (21)

#define DSP_DMA_START_HI    (24) // DMA start address
#define DSP_DMA_START_LO    (25)
#define DSP_DMA_CONTROL_LEN (27)
#define DSP_DMA_BYTES_LEFT  (29)

#define DSP_DMA_START_FLAG (0x8000) // set to start DSP

// Register and bit names below follow the public hardware documentation: YAGCD ("Yet Another GameCube
// Documentation", chapter 5) for the mnemonics and Dolphin's open-source emulator (Source/Core/Core/HW/) where
// YAGCD gives none. Indices are byte offset / 4 for the u32 arrays and byte offset / 2 for __MEMRegs.
// Sources and tables: cleanup_notes/hw_register_names.md.

// Drive commands: the top byte of DI_CMDBUF0. Names after Dolphin's DICommand enum
// (Source/Core/Core/HW/DVD/DVDInterface.h); values are shifted into the top byte here.
#define DI_CMD_INQUIRY            (0x12000000)
#define DI_CMD_READ               (0xA8000000)
#define DI_CMD_SEEK               (0xAB000000)
#define DI_CMD_REQUEST_ERROR      (0xE0000000)
#define DI_CMD_AUDIO_STREAM       (0xE1000000)
#define DI_CMD_REQUEST_AUDIO_STATUS (0xE2000000)
#define DI_CMD_STOP_MOTOR         (0xE3000000)
#define DI_CMD_AUDIO_BUFFER_CONFIG (0xE4000000)

// indices for __PIRegs[i] (processor interface, 0xCC003000)
#define PI_INTSR         (0)  // interrupt cause (YAGCD INTSR)
#define PI_INTMR         (1)  // interrupt mask (YAGCD INTMR)
#define PI_FIFO_BASE     (3)  // CPU FIFO base (Dolphin)
#define PI_FIFO_END      (4)  // CPU FIFO end (Dolphin)
#define PI_FIFO_WPTR     (5)  // CPU FIFO write pointer (Dolphin)
#define PI_FIFO_RESET    (6)  // FIFO reset (Dolphin; used by GXAbortFrame)
#define PI_ERROR_CAUSE   (7)  // error cause (Dolphin; OSReport prints it as PIESR)
#define PI_ERROR_ADDRESS (8)  // error address (Dolphin; OSReport prints it as PIEAR)
#define PI_RESET_CODE    (9)  // reset control and code (Dolphin; YAGCD "reset")
#define PI_FLIPPER_REV   (11) // console type / Flipper revision (Dolphin; YAGCD "console type")

// PI_INTSR / PI_INTMR bits (YAGCD names, Dolphin INT_CAUSE_* values)
#define PI_INT_ERROR     (0x1)    // GP runtime error
#define PI_INT_RSW       (0x2)    // reset switch pressed
#define PI_INT_DI        (0x4)
#define PI_INT_SI        (0x8)
#define PI_INT_EXI       (0x10)
#define PI_INT_AI        (0x20)
#define PI_INT_DSP       (0x40)
#define PI_INT_MEM       (0x80)
#define PI_INT_VI        (0x100)
#define PI_INT_PE_TOKEN  (0x200)
#define PI_INT_PE_FINISH (0x400)
#define PI_INT_CP        (0x800)
#define PI_INT_DEBUG     (0x1000) // debugger (development hardware)
#define PI_INT_HSP       (0x2000) // high speed port
#define PI_INTSR_RSWST (0x10000) // reset switch state: clear while the button is down (not maskable)

// indices for __MEMRegs[i] (memory interface, 0xCC004000; Dolphin MI_* names)
#define MI_REGION0_FIRST (0)  // protected region 0 start page (address >> 10); region n at + 2 * n
#define MI_REGION0_LAST  (1)  // protected region 0 end page
#define MI_PROT_TYPE     (8)  // protection type, 2 bits per region
#define MI_IRQMASK       (14) // interrupt mask
#define MI_IRQFLAG       (15) // interrupt cause

// MI_IRQMASK / MI_IRQFLAG bits (Dolphin MemoryInterface.h MIIRQMask / MIIRQFlag: reg0-reg3, all_regs)
#define MI_IRQ_REG0     (0x1)  // protected region 0 was accessed
#define MI_IRQ_REG1     (0x2)
#define MI_IRQ_REG2     (0x4)
#define MI_IRQ_REG3     (0x8)
#define MI_IRQ_ALL_REGS (0x10)
#define MI_PROT_ADDR_LO  (17) // address that failed protection, low half
#define MI_PROT_ADDR_HI  (18) // address that failed protection, high half

// indices for __DIRegs[i] (DVD interface, 0xCC006000; YAGCD names)
#define DI_SR      (0) // status
#define DI_CVR     (1) // cover status
#define DI_CMDBUF0 (2) // command buffer 0 (command)
#define DI_CMDBUF1 (3) // command buffer 1 (offset)
#define DI_CMDBUF2 (4) // command buffer 2 (length)
#define DI_MAR     (5) // DMA memory address
#define DI_LENGTH  (6) // DMA transfer length (counts down)
#define DI_CR      (7) // control
#define DI_IMMBUF  (8) // immediate data buffer
#define DI_CFG     (9) // configuration (latched at reset)

// DI_SR bits
#define DI_SR_BRKINT     (0x40) // break complete interrupt
#define DI_SR_BRKINTMASK (0x20)
#define DI_SR_TCINT      (0x10) // transfer complete interrupt
#define DI_SR_TCINTMASK  (0x08)
#define DI_SR_DEINT      (0x04) // device error interrupt
#define DI_SR_DEINTMASK  (0x02)
#define DI_SR_BRK        (0x01) // break request

// DI_CVR bits
#define DI_CVR_CVRINT     (0x4) // cover interrupt
#define DI_CVR_CVRINTMASK (0x2)
#define DI_CVR_CVR        (0x1) // cover open

// DI_CR bits
#define DI_CR_RW     (0x4) // 1 = write
#define DI_CR_DMA    (0x2) // 1 = DMA, 0 = immediate
#define DI_CR_TSTART (0x1) // start transfer / transfer pending

// indices for __SIRegs[i] (serial interface, 0xCC006400; YAGCD names). SI_COMCSR_IDX (13) and SI_STATUS_IDX (14)
// are in dolphin/si.h.
#define SI_CHAN_REGS   (3)    // registers per channel: channel n at n * SI_CHAN_REGS
#define SI_OUTBUF_IDX  (0)    // SICnOUTBUF: command
#define SI_INBUFH_IDX  (1)    // SICnINBUFH: response, high word
#define SI_INBUFL_IDX  (2)    // SICnINBUFL: response, low word
#define SI_POLL_IDX    (12)   // SIPOLL
#define SI_EXILK_IDX   (15)   // SIEXILK: EXI clock lock
#define SI_IOBUF_IDX   (0x20) // SI I/O buffer (0x80 bytes)

// indices for __EXIRegs[i] (external interface, 0xCC006800; YAGCD names). Five registers per channel.
#define EXI_CHAN_REGS (5)
#define EXI_CSR    (0) // parameter / status
#define EXI_MAR    (1) // DMA start address
#define EXI_LENGTH (2) // DMA transfer length
#define EXI_CR     (3) // control
#define EXI_DATA   (4) // immediate data
#define EXI0_CSR (0 * EXI_CHAN_REGS + EXI_CSR)
#define EXI0_CR  (0 * EXI_CHAN_REGS + EXI_CR)
#define EXI1_CSR (1 * EXI_CHAN_REGS + EXI_CSR)
#define EXI2_CSR (2 * EXI_CHAN_REGS + EXI_CSR)
#define EXI2_CR   (2 * EXI_CHAN_REGS + EXI_CR)
#define EXI2_DATA (2 * EXI_CHAN_REGS + EXI_DATA)

// EXI_CSR bits (Dolphin field names; YAGCD gives the same layout)
#define EXI_CSR_EXIINTMASK (0x1)
#define EXI_CSR_EXIINT     (0x2)    // EXI interrupt
#define EXI_CSR_TCINTMASK  (0x4)
#define EXI_CSR_TCINT      (0x8)    // transfer complete interrupt
#define EXI_CSR_CLK        (0x70)   // clock frequency field
#define EXI_CSR_CS         (0x380)  // device select field
#define EXI_CSR_EXTINTMASK (0x400)
#define EXI_CSR_EXTINT     (0x800)  // device insertion / removal interrupt
#define EXI_CSR_EXT        (0x1000) // device present
#define EXI_CSR_ROMDIS     (0x2000) // boot ROM disable (channel 0 only)

// EXI_CR bits
#define EXI_CR_DMA    (0x2) // 1 = DMA, 0 = immediate
#define EXI_CR_TSTART (0x1) // start transfer / transfer pending

// indices for __AIRegs[i] (audio streaming interface, 0xCC006C00; YAGCD names)
#define AI_CR   (0) // control
#define AI_VR   (1) // volume (left bits 0-7, right bits 8-15)
#define AI_SCNT (2) // sample counter
#define AI_IT   (3) // interrupt timing

// AI_CR bits
#define AI_CR_PSTAT    (0x01) // 1 = play
#define AI_CR_AFR      (0x02) // stream frequency, 1 = 48 kHz (Dolphin AISFR: YAGCD has the rates inverted)
#define AI_CR_AIINTMSK (0x04)
#define AI_CR_AIINT    (0x08) // interrupt status; write 1 to clear
#define AI_CR_AIINTVLD (0x10)
#define AI_CR_SCRESET  (0x20) // reset sample counter
#define AI_CR_DSPSR    (0x40) // DSP sample rate, 1 = 32 kHz (Dolphin AIDFR)

#endif
