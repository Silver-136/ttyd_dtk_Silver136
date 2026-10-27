typedef int BOOL;
typedef int DSError;

void MWTRACE(unsigned char level, char* fmt, ...);
void usr_put_initialize(void);
DSError TRKInitializeEventQueue(void);
DSError TRKInitializeMessageBuffers(void);
DSError TRKInitializeDispatcher(void);
void InitializeProgramEndTrap(void);
DSError TRKInitializeSerialHandler(void);
DSError TRKInitializeTarget(void);
DSError TRKInitializeIntDrivenUART(unsigned int param_0, unsigned int param_1, unsigned int param_2, void* param_3);
void TRKTargetSetInputPendingPtr(void* ptr);
DSError TRKTerminateSerialHandler(void);
void TRK_board_display(char* str);

extern void* gTRKInputPendingPtr;

BOOL gTRKBigEndian;

BOOL TRKInitializeEndian(void) {
    unsigned char bendian[4];
    BOOL result = 0;

    gTRKBigEndian = 1;

    bendian[0] = 0x12;
    bendian[1] = 0x34;
    bendian[2] = 0x56;
    bendian[3] = 0x78;

    if (*(unsigned long*)bendian == 0x12345678)
        gTRKBigEndian = 1;
    else if (*(unsigned long*)bendian == 0x78563412)
        gTRKBigEndian = 0;
    else
        result = 1;

    return result;
}

DSError TRKInitializeNub(void) {
    DSError result;
    DSError resultTemp;

    result = TRKInitializeEndian();

    MWTRACE(1, "Initialize NUB\n");

    if (result == 0)
        usr_put_initialize();

    if (result == 0)
        result = TRKInitializeEventQueue();

    if (result == 0)
        result = TRKInitializeMessageBuffers();

    if (result == 0)
        result = TRKInitializeDispatcher();

    InitializeProgramEndTrap();

    if (result == 0)
        result = TRKInitializeSerialHandler();

    if (result == 0)
        result = TRKInitializeTarget();

    if (result == 0) {
        resultTemp = TRKInitializeIntDrivenUART(0xE100, 1, 0, &gTRKInputPendingPtr);
        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
        if (resultTemp != 0)
            result = resultTemp;
    }

    return result;
}

DSError TRKTerminateNub(void) {
    TRKTerminateSerialHandler();
    return 0;
}

void TRKNubWelcome(void) {
    TRK_board_display("MetroTRK for GAMECUBE v2.6");
}
