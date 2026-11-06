typedef int BOOL;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct DBCommTable {
    int (*initialize_func)(void*, void*);
    void (*init_interrupts_func)(void);
    int (*shutdown_func)(void);
    int (*peek_func)(void);
    int (*read_func)(u8*, int);
    int (*write_func)(const u8*, int);
    int (*open_func)(void);
    int (*close_func)(void);
    int (*pre_continue_func)(void);
    int (*post_stop_func)(void);
} DBCommTable;

void OSReport(const char* msg, ...);
void OSEnableScheduler(void);
void PPCHalt(void);
void ICInvalidateRange(void* addr, u32 nBytes);
void DCFlushRange(void* addr, u32 nBytes);
void* TRK_memcpy(void* dst, const void* src, unsigned int n);
void TRKInterruptHandler(void);
int Hu_IsStub(void);
int AMC_IsStub(void);

int udp_cc_initialize(void* flagOut, void* func);
int udp_cc_shutdown(void);
int udp_cc_open(void);
int udp_cc_close(void);
int udp_cc_read(u8* buf, int len);
int udp_cc_write(const u8* buf, int len);
int udp_cc_peek(void);
int udp_cc_pre_continue(void);
int udp_cc_post_stop(void);

int gdev_cc_initialize(void* inputPendingPtrRef, void* monitorCallback);
int gdev_cc_shutdown(void);
int gdev_cc_open(void);
int gdev_cc_close(void);
int gdev_cc_read(u8* dest, int size);
int gdev_cc_write(const u8* bytes, int length);
int gdev_cc_pre_continue(void);
int gdev_cc_post_stop(void);
int gdev_cc_peek(void);
int gdev_cc_initinterrupts(void);

int ddh_cc_initialize(void* inputPendingPtrRef, void* monitorCallback);
int ddh_cc_shutdown(void);
int ddh_cc_open(void);
int ddh_cc_close(void);
int ddh_cc_read(u8* dest, int size);
int ddh_cc_write(const u8* bytes, int length);
int ddh_cc_pre_continue(void);
int ddh_cc_post_stop(void);
int ddh_cc_peek(void);
int ddh_cc_initinterrupts(void);

DBCommTable gDBCommTable = {0};
BOOL _MetroTRK_Has_Framing;
u8 TRK_Use_BBA;

asm void TRKLoadContext(void* ctx, u32 val) {
    nofralloc
    lwz r0, 0x0(r3)
    lwz r1, 0x4(r3)
    lwz r2, 0x8(r3)
    lhz r5, 0x1a2(r3)
    rlwinm. r6, r5, 0, 30, 30
    beq skip
    rlwinm r5, r5, 0, 31, 29
    sth r5, 0x1a2(r3)
    lmw r5, 0x14(r3)
    b done
skip:
    lmw r13, 0x34(r3)
done:
    mr r31, r3
    mr r3, r4
    lwz r4, 0x80(r31)
    mtcrf 255, r4
    lwz r4, 0x84(r31)
    mtspr 8, r4
    lwz r4, 0x88(r31)
    mtspr 9, r4
    lwz r4, 0x8c(r31)
    mtspr 1, r4
    mfmsr r4
    rlwinm r4, r4, 0, 17, 15
    rlwinm r4, r4, 0, 31, 29
    mtmsr r4
    mtspr 273, r2
    lwz r4, 0xc(r31)
    mtspr 274, r4
    lwz r4, 0x10(r31)
    mtspr 275, r4
    lwz r2, 0x198(r31)
    lwz r4, 0x19c(r31)
    lwz r31, 0x7c(r31)
    b TRKInterruptHandler
}

void TRKEXICallBack(int interrupt, void* ctx) {
    OSEnableScheduler();
    TRKLoadContext(ctx, 0x500);
}

int InitMetroTRKCommTable(int hwId) {
    int result = 1;

    OSReport("Devkit set to : %ld\n", hwId);
    TRK_Use_BBA = 0;

    if (hwId == 2) {
        OSReport("MetroTRK : Set to BBA\n");
        TRK_Use_BBA = 1;
        gDBCommTable.initialize_func = udp_cc_initialize;
        gDBCommTable.open_func = udp_cc_open;
        gDBCommTable.close_func = udp_cc_close;
        gDBCommTable.read_func = udp_cc_read;
        gDBCommTable.write_func = udp_cc_write;
        gDBCommTable.shutdown_func = udp_cc_shutdown;
        gDBCommTable.peek_func = udp_cc_peek;
        gDBCommTable.pre_continue_func = udp_cc_pre_continue;
        gDBCommTable.post_stop_func = udp_cc_post_stop;
        gDBCommTable.init_interrupts_func = 0;
        return 0;
    } else if (hwId == 1) {
        OSReport("MetroTRK : Set to GDEV hardware\n");
        result = Hu_IsStub();
        gDBCommTable.initialize_func = gdev_cc_initialize;
        gDBCommTable.open_func = gdev_cc_open;
        gDBCommTable.close_func = gdev_cc_close;
        gDBCommTable.read_func = gdev_cc_read;
        gDBCommTable.write_func = gdev_cc_write;
        gDBCommTable.shutdown_func = gdev_cc_shutdown;
        gDBCommTable.peek_func = gdev_cc_peek;
        gDBCommTable.pre_continue_func = gdev_cc_pre_continue;
        gDBCommTable.post_stop_func = gdev_cc_post_stop;
        gDBCommTable.init_interrupts_func = (void (*)(void))gdev_cc_initinterrupts;
    } else if (hwId == 0) {
        OSReport("MetroTRK : Set to AMC DDH hardware\n");
        result = AMC_IsStub();
        gDBCommTable.initialize_func = ddh_cc_initialize;
        gDBCommTable.open_func = ddh_cc_open;
        gDBCommTable.close_func = ddh_cc_close;
        gDBCommTable.read_func = ddh_cc_read;
        gDBCommTable.write_func = ddh_cc_write;
        gDBCommTable.shutdown_func = ddh_cc_shutdown;
        gDBCommTable.peek_func = ddh_cc_peek;
        gDBCommTable.pre_continue_func = ddh_cc_pre_continue;
        gDBCommTable.post_stop_func = ddh_cc_post_stop;
        gDBCommTable.init_interrupts_func = (void (*)(void))ddh_cc_initinterrupts;
    } else {
        OSReport("MetroTRK : Set to UNKNOWN hardware. (%ld)\n", hwId);
        OSReport("MetroTRK : Invalid hardware ID passed from OS\n");
        OSReport("MetroTRK : Defaulting to GDEV Hardware\n");
    }

    return result;
}

/* The first three arguments are not used. */
int TRKInitializeIntDrivenUART(u32 param_0, u32 param_1, u32 param_2, void* inputPendingPtrRef) {
    gDBCommTable.initialize_func(inputPendingPtrRef, TRKEXICallBack);
    gDBCommTable.open_func();
    return 0;
}

void EnableEXI2Interrupts(void) {
    if (!TRK_Use_BBA) {
        if (gDBCommTable.init_interrupts_func != 0)
            gDBCommTable.init_interrupts_func();
    }
}

int TRKPollUART(void) {
    return gDBCommTable.peek_func();
}

int TRKReadUARTN(void* bytes, u32 length) {
    int readErr = gDBCommTable.read_func(bytes, length);
    return readErr == 0 ? 0 : -1;
}

int TRKWriteUARTN(const void* bytes, u32 length) {
    int writeErr = gDBCommTable.write_func(bytes, length);
    return writeErr == 0 ? 0 : -1;
}

void ReserveEXI2Port(void) {
    gDBCommTable.post_stop_func();
}

void UnreserveEXI2Port(void) {
    gDBCommTable.pre_continue_func();
}

void TRK_board_display(char* str) {
    OSReport("%s\n", str);
}

void InitializeProgramEndTrap(void) {
    static const u8 EndofProgramInstruction[4] = {0x00, 0x45, 0x4E, 0x44};
    u8* endOfProgramInstructionBytes = (u8*)EndofProgramInstruction;
    u8* ppcHaltPtr = (u8*)PPCHalt;

    TRK_memcpy(ppcHaltPtr + 4, endOfProgramInstructionBytes, sizeof(EndofProgramInstruction));
    ICInvalidateRange(ppcHaltPtr + 4, sizeof(EndofProgramInstruction));
    DCFlushRange(ppcHaltPtr + 4, sizeof(EndofProgramInstruction));
}

/* Empty: called from the TRK interrupt code, nothing to do here. */
void TRKUARTInterruptHandler(void) {}
