#ifndef _DOLPHIN_AR_H_
#define _DOLPHIN_AR_H_

#include <dolphin/arq.h>
#ifdef __cplusplus
extern "C" {
#endif

#define ARAM_DIR_MRAM_TO_ARAM 0x00
#define ARAM_DIR_ARAM_TO_MRAM 0x01

// ARClear flags
#define AR_CLEAR_INTERNAL_ALL  0x00 // the whole internal ARAM
#define AR_CLEAR_INTERNAL_USER 0x01 // internal ARAM above AR_BASE_ADDRESS
#define AR_CLEAR_EXPANSION     0x02 // the expansion ARAM

#define AR_BASE_ADDRESS 0x4000 // first ARAM address the allocator hands out (ARGetBaseAddress)

#define ARStartDMARead(mmem, aram, len) \
    ARStartDMA(ARAM_DIR_ARAM_TO_MRAM, mmem, aram, len)
#define ARStartDMAWrite(mmem, aram, len) \
    ARStartDMA(ARAM_DIR_MRAM_TO_ARAM, mmem, aram, len)

ARQCallback ARRegisterDMACallback(ARQCallback callback);
u32 ARGetDMAStatus(void);
void ARStartDMA(u32 type, u32 mainmem_addr, u32 aram_addr, u32 length);
u32 ARAlloc(u32 length);
u32 ARFree(u32* length);
BOOL ARCheckInit(void);
u32 ARInit(u32* stack_index_addr, u32 num_entries);
void ARReset(void);
void ARSetSize(void);
u32 ARGetBaseAddress(void);
u32 ARGetSize(void);
u32 ARGetInternalSize(void);
void ARClear(u32 flag);

#ifdef __cplusplus
}
#endif

#endif
