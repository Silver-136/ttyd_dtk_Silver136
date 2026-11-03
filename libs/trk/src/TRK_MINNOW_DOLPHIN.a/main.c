typedef int BOOL;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct CircleBuffer {
    u8* read_ptr;
    u8* write_ptr;
    u8* start_ptr;
    u32 size;
    u32 mBytesToRead;
    u32 mBytesToWrite;
    unsigned int mCriticalSection;
} CircleBuffer;

void MWTRACE(u8 level, char* fmt, ...);
u32 CBGetBytesAvailableForRead(CircleBuffer* cb);
void CircleBufferInitialize(CircleBuffer* cb, u8* buf, u32 size);
int CircleBufferWriteBytes(CircleBuffer* cb, u8* buf, u32 size);
int CircleBufferReadBytes(CircleBuffer* cb, u8* buf, u32 size);
void EXI2_Init(void* inputPendingPtrRef, void* monitorCallback);
void EXI2_EnableInterrupts(void);
u32 EXI2_Poll(void);
int EXI2_ReadN(void* data, u32 size);
int EXI2_WriteN(const void* data, u32 size);
void EXI2_Reserve(void);
void EXI2_Unreserve(void);

#define DDH_BUF_SIZE 0x800

static CircleBuffer gRecvCB;
static u8 gRecvBuf[DDH_BUF_SIZE];
static BOOL gIsInitialized;

int ddh_cc_initialize(void* inputPendingPtrRef, void* monitorCallback) {
    MWTRACE(1, "CALLING EXI2_Init\n");
    EXI2_Init(inputPendingPtrRef, monitorCallback);
    MWTRACE(1, "DONE CALLING EXI2_Init\n");
    CircleBufferInitialize(&gRecvCB, gRecvBuf, DDH_BUF_SIZE);
    return 0;
}

int ddh_cc_shutdown(void) {
    return 0;
}

int ddh_cc_open(void) {
    if (gIsInitialized)
        return -10005;

    gIsInitialized = 1;
    return 0;
}

int ddh_cc_close(void) {
    return 0;
}

int ddh_cc_read(u8* dest, int size) {
    u8 buff[DDH_BUF_SIZE];
    u32 readResult;
    int poll;

    readResult = 0;

    if (!gIsInitialized)
        return -10001;

    MWTRACE(1, "Expected packet size : 0x%08x (%ld)\n", size, size);

    while (CBGetBytesAvailableForRead(&gRecvCB) < size) {
        readResult = 0;
        poll = EXI2_Poll();
        if (poll != 0) {
            readResult = EXI2_ReadN(buff, poll);
            if (readResult == 0)
                CircleBufferWriteBytes(&gRecvCB, buff, poll);
        }
    }

    if (readResult == 0)
        CircleBufferReadBytes(&gRecvCB, dest, size);
    else
        MWTRACE(8, "cc_read : error reading bytes from EXI2 %ld\n", readResult);

    return readResult;
}

int ddh_cc_write(const u8* bytes, int length) {
    int exi2Len;
    int n_copy;
    u32 hexCopy;

    hexCopy = (u32)bytes;
    n_copy = length;

    if (gIsInitialized == 0) {
        MWTRACE(8, "cc not initialized\n");
        return -10001;
    }

    MWTRACE(8, "cc_write : Output data 0x%08x %ld bytes\n", bytes, length);

    while (n_copy > 0) {
        MWTRACE(1, "cc_write sending %ld bytes\n", n_copy);
        exi2Len = EXI2_WriteN((const void*)hexCopy, n_copy);

        if (exi2Len == 0)
            break;

        hexCopy += exi2Len;
        n_copy -= exi2Len;
    }

    return 0;
}

int ddh_cc_pre_continue(void) {
    EXI2_Unreserve();
    return 0;
}

int ddh_cc_post_stop(void) {
    EXI2_Reserve();
    return 0;
}

int ddh_cc_peek(void) {
    int poll;
    u8 buff[DDH_BUF_SIZE];

    poll = EXI2_Poll();

    if (poll <= 0)
        return 0;

    if (EXI2_ReadN(buff, poll) == 0)
        CircleBufferWriteBytes(&gRecvCB, buff, poll);
    else
        return -10009;

    return poll;
}

int ddh_cc_initinterrupts(void) {
    EXI2_EnableInterrupts();
    return 0;
}
