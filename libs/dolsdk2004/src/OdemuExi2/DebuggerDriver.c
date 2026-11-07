#include <dolphin/os.h>
#include <macros.h>

typedef void (*InterruptHandlerLike)(s32, OSContext*);

u8 EXIInputFlag;
u8* pEXIInputFlag;
s32 RecvDataLeng;
u32 SendMailData;
InterruptHandlerLike DBGCallback;
InterruptHandlerLike MTRCallback;

u8 SendCount = 0x80;



static void DBGEXIInit(void)
{
    __OSMaskInterrupts(0x18000);
    __EXIRegs[EXI2_CSR] = 0;
}

static BOOL DBGEXISelect(u32 v)
{
    u32 regs = __EXIRegs[EXI2_CSR];
    regs &= 0x405;
    regs |= 0x80 | (v << 4);
    __EXIRegs[EXI2_CSR] = regs;
    return TRUE;
}

static BOOL DBGEXIDeselect(void)
{
    __EXIRegs[EXI2_CSR] &= 0x405;
    return TRUE;
}

static BOOL DBGEXISync(void)
{
    while (__EXIRegs[EXI2_CR] & 1)
        ;
    return TRUE;
}

static BOOL DBGEXIImm(void* data, s32 byte_size, u32 write)
{
    u32 writeVal;
    u32 readVal;
    int i;

    if (write) {
        writeVal = 0;
        for (i = 0; i < byte_size; ++i) {
            u8* nextWordPtr = (u8*)data + i;
            writeVal |= *nextWordPtr << ((3 - i) << 3);
        }
        __EXIRegs[EXI2_DATA] = writeVal;
    }

    __EXIRegs[EXI2_CR] = 1 | (write << 2) | ((byte_size - 1U) << 4);

    DBGEXISync();
    if (!write) {
        u8* dataPtr = (u8*)data;
        readVal     = __EXIRegs[EXI2_DATA];
        for (i = 0; i < byte_size; ++i) {
            *dataPtr++ = readVal >> ((3 - i) << 3);
        }
    }

    return TRUE;
}

static BOOL DBGReadMailbox(void* mailbox)
{
    BOOL error = FALSE;
    u32 command;

    if (!DBGEXISelect(4)) {
        return FALSE;
    }

    command = 0x60000000;
    error |= !DBGEXIImm((u8*)&command, 2, 1);
    error |= !DBGEXISync();
    error |= !DBGEXIImm(mailbox, sizeof(mailbox), 0); // sizeof a pointer: 4, the size of the mailbox word
    error |= !DBGEXISync();
    error |= !DBGEXIDeselect();

    return !error;
}

static BOOL DBGWriteMailbox(u32 mailbox)
{
    BOOL error = FALSE;
    u32 value;

    if (!DBGEXISelect(4)) {
        return FALSE;
    }

    value = (mailbox & 0x1fffffff) | 0xc0000000;
    error |= !DBGEXIImm((u8*)&value, sizeof(value), 1);
    error |= !DBGEXISync();
    error |= !DBGEXIDeselect();

    return !error;
}

static BOOL DBGRead(u32 param1, u32* data, s32 byte_size)
{
    BOOL error   = FALSE;
    u32* dataPtr = data;
    u32 writeValue;
    u32 readValue;

    if (!DBGEXISelect(4)) {
        return FALSE;
    }

    writeValue = (u32)(param1 << 8) & 0x1fffc00 | 0x20000000;
    error |= !DBGEXIImm((u8*)&writeValue, sizeof(writeValue), 1);
    error |= !DBGEXISync();

    while (byte_size != 0) {
        error |= !DBGEXIImm((u8*)&readValue, sizeof(readValue), 0);
        error |= !DBGEXISync();

        *dataPtr++ = readValue;

        byte_size -= 4;
        if (byte_size < 0) {
            byte_size = 0;
        }
    }
    error |= !DBGEXIDeselect();

    return !error;
}

static BOOL DBGWrite(u32 param1, u32* data, s32 byte_size)
{
    BOOL error   = FALSE;
    u32* dataPtr = data;
    u32 value;
    u32 nextWord;

    if (!DBGEXISelect(4)) {
        return FALSE;
    }

    value = (param1 & 0x1fffc) << 8 | 0xa0000000;
    error = !DBGEXIImm((u8*)&value, sizeof(value), 1);
    error |= !DBGEXISync();

    while (byte_size != 0) {
        nextWord = *dataPtr++;

        error |= !DBGEXIImm((u8*)&nextWord, sizeof(nextWord), 1);
        error |= !DBGEXISync();

        byte_size -= 4;
        if (byte_size < 0)
            byte_size = 0;
    }

    error |= !DBGEXIDeselect();

    return !error;
}

static BOOL DBGReadStatus(void* status)
{
    BOOL error = FALSE;
    u32 value;

    if (!DBGEXISelect(4)) {
        return FALSE;
    }

    value = 0x40000000;
    error |= !DBGEXIImm((u8*)&value, 2, 1);
    error |= !DBGEXISync();
    error |= !DBGEXIImm(status, sizeof(status), 0); // sizeof a pointer: 4, the size of the status word
    error |= !DBGEXISync();
    error |= !DBGEXIDeselect();

    return !error;
}

static void MWCallback(s32 interrupt, OSContext* context)
{
    EXIInputFlag = 1;
    if (MTRCallback) {
        (*MTRCallback)(0, context);
    }
}

static void DBGHandler(__OSInterrupt interrupt, OSContext* context)
{
    __PIRegs[PI_INTSR] = 0x1000;
    if (DBGCallback) {
        (*DBGCallback)(interrupt, context);
    }
}

static void CheckMailBox(void)
{
    u32 mail[2]; // the status word, then the mailbox word

    DBGReadStatus(mail);
    if (mail[0] & 1) {
        DBGReadMailbox(mail);
        mail[0] &= ~0xE0000000;
        if ((mail[0] & 0x1f000000) == 0x1f000000) {
            SendMailData = mail[0];
            RecvDataLeng = mail[0] & 0x7fff;
            EXIInputFlag = 1;
        }
    }
}

void DBInitComm(u8** inputPendingPtrRef, InterruptHandlerLike monitorCallback)
{
    BOOL enabled;

    enabled       = OSDisableInterrupts();
    pEXIInputFlag = &EXIInputFlag;
    *inputPendingPtrRef      = pEXIInputFlag;
    MTRCallback   = monitorCallback;
    DBGEXIInit();
    OSRestoreInterrupts(enabled);
}

void DBInitInterrupts(void)
{
    __OSMaskInterrupts(0x18000);
    __OSMaskInterrupts(0x40);
    DBGCallback = MWCallback;
    __OSSetInterruptHandler(0x19, DBGHandler);
    __OSUnmaskInterrupts(0x40);
}

u32 DBQueryData(void)
{
    BOOL enable;

    EXIInputFlag = 0;
    if (RecvDataLeng == 0) {
        enable = OSDisableInterrupts();
        CheckMailBox();
        OSRestoreInterrupts(enable);
    }
    return RecvDataLeng;
}

u32 DBRead(u32* data, s32 size)
{
    BOOL enabled;
    u32 bankOffset;

    enabled = OSDisableInterrupts();
    if (SendMailData & 0x10000) {
        bankOffset = 0x1000;
    } else {
        bankOffset = 0;
    }
    DBGRead(bankOffset + 0x1e000, data, size + 3U & 0xfffffffc);
    RecvDataLeng = 0;
    EXIInputFlag = 0;
    OSRestoreInterrupts(enabled);
    return 0;
}

BOOL DBWrite(void* data, u32 size)
{
    u32 value;
    u32 busyFlag;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    do {
        DBGReadStatus(&busyFlag);
    } while (busyFlag & 2);

    ++SendCount;

    value = ((SendCount & 1) ? 0x1000 : 0);
    while (!DBGWrite(value | 0x1c000, data, ALIGN_NEXT(size, 4)))
        ;

    do {
        DBGReadStatus(&busyFlag);
    } while (busyFlag & 2);

    value = (SendCount << 0x10) | 0x1f000000 | size;
    while (!DBGWriteMailbox(value))
        ;

    do {
        while (!DBGReadStatus(&busyFlag))
            ;
    } while (busyFlag & 2);

    OSRestoreInterrupts(enabled);

    return 0;
}

void DBOpen(void) { }

void DBClose(void) { }
