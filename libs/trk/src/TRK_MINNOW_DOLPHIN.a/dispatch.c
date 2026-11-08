#include "trk.h"

typedef int DSError;

typedef struct TRKBuffer {
    unsigned int mutex;
    int fInUse;
    unsigned int length;
    unsigned int position;
    unsigned char data[0x880];
} TRKBuffer;

void MWTRACE(unsigned char level, char* fmt, ...);
DSError TRKSetBufferPosition(TRKBuffer* msg, unsigned int pos);
DSError TRKDoConnect(TRKBuffer* b);
DSError TRKDoDisconnect(TRKBuffer* b);
DSError TRKDoReset(TRKBuffer* b);
DSError TRKDoOverride(TRKBuffer* b);
DSError TRKDoVersions(TRKBuffer* b);
DSError TRKDoSupportMask(TRKBuffer* b);
DSError TRKDoReadMemory(TRKBuffer* b);
DSError TRKDoWriteMemory(TRKBuffer* b);
DSError TRKDoReadRegisters(TRKBuffer* b);
DSError TRKDoWriteRegisters(TRKBuffer* b);
DSError TRKDoContinue(TRKBuffer* b);
DSError TRKDoStep(TRKBuffer* b);
DSError TRKDoStop(TRKBuffer* b);
DSError TRKDoSetOption(TRKBuffer* b);

DSError TRKInitializeDispatcher(void) {
    return 0;
}

DSError TRKDispatchMessage(TRKBuffer* buffer) {
    DSError error = DS_DispatchError;

    TRKSetBufferPosition(buffer, 0);
    MWTRACE(1, "Dispatch command 0x%08x\n", buffer->data[4]);

    switch (buffer->data[4]) {
    case DSMSG_Connect:
        error = TRKDoConnect(buffer);
        break;
    case DSMSG_Disconnect:
        error = TRKDoDisconnect(buffer);
        break;
    case DSMSG_Reset:
        error = TRKDoReset(buffer);
        break;
    case DSMSG_Override:
        error = TRKDoOverride(buffer);
        break;
    case DSMSG_Versions:
        error = TRKDoVersions(buffer);
        break;
    case DSMSG_SupportMask:
        error = TRKDoSupportMask(buffer);
        break;
    case DSMSG_ReadMemory:
        error = TRKDoReadMemory(buffer);
        break;
    case DSMSG_WriteMemory:
        error = TRKDoWriteMemory(buffer);
        break;
    case DSMSG_ReadRegisters:
        error = TRKDoReadRegisters(buffer);
        break;
    case DSMSG_WriteRegisters:
        error = TRKDoWriteRegisters(buffer);
        break;
    case DSMSG_Continue:
        error = TRKDoContinue(buffer);
        break;
    case DSMSG_Step:
        error = TRKDoStep(buffer);
        break;
    case DSMSG_Stop:
        error = TRKDoStop(buffer);
        break;
    case DSMSG_SetOption:
        error = TRKDoSetOption(buffer);
        break;
    }

    MWTRACE(1, "Dispatch complete err = %ld\n", error);
    return error;
}
