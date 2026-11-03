#pragma defer_codegen off
typedef int BOOL;
typedef int DSError;
typedef unsigned long size_t;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct TRKBuffer {
    unsigned int mutex;
    BOOL isInUse;
    u32 length;
    u32 position;
    u8 data[0x880];
} TRKBuffer;

typedef enum MessageCommandID {
    DSMSG_ReplyACK = 0x80,
    DSMSG_WriteFile = 0xD0,
    DSMSG_ReadFile = 0xD1,
    DSMSG_OpenFile = 0xD2,
    DSMSG_CloseFile = 0xD3,
    DSMSG_PositionFile = 0xD4
} MessageCommandID;

typedef struct CommandReply {
    u32 length;
    u8 commandID;
    u8 pad5[3];
    union {
        u8 b;
        u32 w;
    } r8;
    union {
        u16 h;
        u32 w;
    } rC;
    u8 r10;
    u8 pad11[0x2F];
} CommandReply;

void* memset(void* dst, int val, size_t n);
size_t strlen(const char* s);
void MWTRACE(u8 level, char* fmt, ...);
DSError TRKMessageSend(TRKBuffer* b);
int TRKTestForPacket(void);
void* TRKGetBuffer(int idx);
DSError TRKGetFreeBuffer(int* bufIdx, TRKBuffer** buf);
void TRKReleaseBuffer(int idx);
DSError TRKSetBufferPosition(TRKBuffer* msg, u32 pos);
DSError TRKAppendBuffer_ui8(TRKBuffer* buffer, const u8* data, int count);
DSError TRKReadBuffer_ui8(TRKBuffer* buffer, u8* data, int count);
void OutputData(void* data, int length);
void TRKProcessInput(int bufferIdx);

DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 p1, u32 p2, int p3);

static const char sRequestSendStrings[] = "Calling MessageSend\n\0\0\0\0"
                                          "msg_command : 0x%02x hdr->cmdID 0x%02x\n\0"
                                          "msg_error : 0x%02x\n\0"
                                          "RequestSend : Bad ack or non ack received msg_command : 0x%02x msg_error 0x%02x\n";

DSError HandlePositionFileSupportRequest(u32 file_handle, u32* position, u8 whence, u32* ioResult) {
    int replyBufferId;
    int bufferId;
    TRKBuffer* buffer;
    TRKBuffer* replyBuffer;
    DSError error;
    CommandReply reply;

    memset(&reply, 0, sizeof(CommandReply));
    reply.commandID = DSMSG_PositionFile;
    reply.length = sizeof(CommandReply);
    reply.r8.w = file_handle;
    reply.rC.w = *position;
    reply.r10 = whence;

    error = TRKGetFreeBuffer(&bufferId, &buffer);

    if (error == 0)
        error = TRKAppendBuffer_ui8(buffer, (u8*)&reply, sizeof(CommandReply));

    if (error == 0) {
        *ioResult = 0;
        *position = -1;
        error = TRKRequestSend(buffer, &replyBufferId, 3, 3, 0);

        if (error == 0) {
            replyBuffer = TRKGetBuffer(replyBufferId);
            if (replyBuffer != 0) {
                *ioResult = *(u32*)(replyBuffer->data + 0x10);
                *position = *(u32*)(replyBuffer->data + 0x18);
            }
        }

        TRKReleaseBuffer(replyBufferId);
    }

    TRKReleaseBuffer(bufferId);
    return error;
}

DSError HandleCloseFileSupportRequest(int file_handle, u32* ioResult) {
    int replyBufferId;
    int bufferId;
    TRKBuffer* buffer;
    DSError error;
    CommandReply reply;

    memset(&reply, 0, sizeof(CommandReply));
    reply.commandID = DSMSG_CloseFile;
    reply.length = sizeof(CommandReply);
    reply.r8.w = file_handle;

    error = TRKGetFreeBuffer(&bufferId, &buffer);

    if (error == 0)
        error = TRKAppendBuffer_ui8(buffer, (u8*)&reply, sizeof(CommandReply));

    if (error == 0) {
        TRKBuffer* replyBuffer;

        *ioResult = 0;
        error = TRKRequestSend(buffer, &replyBufferId, 3, 3, 0);

        if (error == 0)
            replyBuffer = TRKGetBuffer(replyBufferId);

        if (error == 0)
            *ioResult = *(u32*)(replyBuffer->data + 0x10);

        TRKReleaseBuffer(replyBufferId);
    }

    TRKReleaseBuffer(bufferId);
    return error;
}

asm DSError HandleOpenFileSupportRequest(const char* path, u8 replyError, u32* param_3, u32* ioResult) {
    nofralloc
    stwu r1, -0x70(r1)
    mfspr r0, 8
    stw r0, 0x74(r1)
    stmw r27, 0x5c(r1)
    mr r27, r3
    mr r31, r4
    mr r28, r5
    mr r29, r6
    addi r3, r1, 0x14
    li r4, 0x0
    li r5, 0x40
    bl memset
    li r3, 0x0
    li r0, 0xd2
    stw r3, 0x0(r28)
    mr r3, r27
    stb r0, 0x18(r1)
    bl strlen
    addi r0, r3, 0x41
    stb r31, 0x1c(r1)
    mr r3, r27
    stw r0, 0x14(r1)
    bl strlen
    addi r0, r3, 0x1
    addi r3, r1, 0xc
    sth r0, 0x20(r1)
    addi r4, r1, 0x8
    bl TRKGetFreeBuffer
    lwz r3, 0x8(r1)
    addi r4, r1, 0x14
    li r5, 0x40
    bl TRKAppendBuffer_ui8
    mr. r31, r3
    bne L_8025AA50
    mr r3, r27
    bl strlen
    mr r5, r3
    lwz r3, 0x8(r1)
    mr r4, r27
    addi r5, r5, 0x1
    bl TRKAppendBuffer_ui8
    mr r31, r3
L_8025AA50:
    cmpwi r31, 0x0
    bne L_8025AAA4
    li r0, 0x0
    addi r4, r1, 0x10
    stw r0, 0x0(r29)
    li r5, 0x7
    li r6, 0x3
    li r7, 0x0
    lwz r3, 0x8(r1)
    bl TRKRequestSend
    mr. r31, r3
    bne L_8025AA8C
    lwz r3, 0x10(r1)
    bl TRKGetBuffer
    mr r30, r3
L_8025AA8C:
    lwz r0, 0x20(r30)
    stw r0, 0x0(r29)
    lwz r0, 0x18(r30)
    stw r0, 0x0(r28)
    lwz r3, 0x10(r1)
    bl TRKReleaseBuffer
L_8025AAA4:
    lwz r3, 0xc(r1)
    bl TRKReleaseBuffer
    mr r3, r31
    lmw r27, 0x5c(r1)
    lwz r0, 0x74(r1)
    mtspr 8, r0
    addi r1, r1, 0x70
    blr
}

asm DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 p1, u32 p2, int p3) {
    nofralloc
    stwu r1, -0x40(r1)
    mfspr r0, 8
    stw r0, 0x44(r1)
    li r0, -0x1
    stmw r21, 0x14(r1)
    mr r22, r4
    lis r4, sRequestSendStrings@ha
    mr r21, r3
    mr r23, r7
    addi r27, r6, 0x1
    addi r31, r4, sRequestSendStrings@l
    li r30, 0x0
    li r24, 0x1
    stw r0, 0x0(r22)
    b L_8025AC60
L_8025AB00:
    addi r4, r31, 0x0
    li r3, 0x1
    crclr 6
    bl MWTRACE
    mr r3, r21
    bl TRKMessageSend
    mr. r30, r3
    bne L_8025AC5C
    cmpwi r23, 0x0
    beq L_8025AB2C
    li r28, 0x0
L_8025AB2C:
    bl TRKTestForPacket
    stw r3, 0x0(r22)
    lwz r3, 0x0(r22)
    cmpwi r3, -0x1
    bne L_8025AB5C
    cmpwi r23, 0x0
    beq L_8025AB2C
    lis r4, 0x4c5
    addi r28, r28, 0x1
    subi r0, r4, 0x4c14
    cmplw r28, r0
    blt L_8025AB2C
L_8025AB5C:
    cmpwi r3, -0x1
    beq L_8025ABBC
    li r24, 0x0
    bl TRKGetBuffer
    li r4, 0x0
    mr r29, r3
    bl TRKSetBufferPosition
    lwz r4, 0x8(r29)
    addi r3, r29, 0x10
    bl OutputData
    lbz r26, 0x14(r29)
    addi r4, r31, 0x18
    li r3, 0x1
    mr r5, r26
    mr r6, r26
    crclr 6
    bl MWTRACE
    cmplwi r26, 0x80
    bge L_8025ABBC
    lwz r3, 0x0(r22)
    bl TRKProcessInput
    li r0, -0x1
    stw r0, 0x0(r22)
    b L_8025AB2C
L_8025ABBC:
    lwz r0, 0x0(r22)
    cmpwi r0, -0x1
    beq L_8025AC5C
    lwz r0, 0x8(r29)
    cmplwi r0, 0x40
    bge L_8025ABD8
    li r24, 0x1
L_8025ABD8:
    cmpwi r30, 0x0
    bne L_8025AC00
    cmpwi r24, 0x0
    bne L_8025AC00
    lbz r25, 0x18(r29)
    addi r4, r31, 0x40
    li r3, 0x1
    mr r5, r25
    crclr 6
    bl MWTRACE
L_8025AC00:
    cmpwi r30, 0x0
    bne L_8025AC3C
    cmpwi r24, 0x0
    bne L_8025AC3C
    cmpwi r26, 0x80
    mr r5, r26
    bne L_8025AC24
    cmpwi r25, 0x0
    beq L_8025AC3C
L_8025AC24:
    mr r6, r25
    addi r4, r31, 0x54
    li r3, 0x8
    crclr 6
    bl MWTRACE
    li r24, 0x1
L_8025AC3C:
    cmpwi r30, 0x0
    bne L_8025AC4C
    cmpwi r24, 0x0
    beq L_8025AC5C
L_8025AC4C:
    lwz r3, 0x0(r22)
    bl TRKReleaseBuffer
    li r0, -0x1
    stw r0, 0x0(r22)
L_8025AC5C:
    subi r27, r27, 0x1
L_8025AC60:
    cmpwi r27, 0x0
    beq L_8025AC7C
    lwz r0, 0x0(r22)
    cmpwi r0, -0x1
    bne L_8025AC7C
    cmpwi r30, 0x0
    beq L_8025AB00
L_8025AC7C:
    lwz r0, 0x0(r22)
    cmpwi r0, -0x1
    bne L_8025AC8C
    li r30, 0x800
L_8025AC8C:
    mr r3, r30
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtspr 8, r0
    addi r1, r1, 0x40
    blr
}

asm DSError TRKSuppAccessFile(u32 file_handle, u8* data, size_t* count, u32* io_result, BOOL need_reply, BOOL read) {
    nofralloc
    stwu r1, -0x90(r1)
    mfspr r0, 8
    stw r0, 0x94(r1)
    stmw r19, 0x5c(r1)
    mr. r24, r4
    mr r23, r3
    mr r25, r5
    mr r26, r6
    mr r27, r7
    mr r28, r8
    beq L_8025ACDC
    lwz r0, 0x0(r25)
    cmplwi r0, 0x0
    bne L_8025ACE4
L_8025ACDC:
    li r3, 0x2
    b L_8025AEB0
L_8025ACE4:
    li r0, 0x0
    li r29, 0x0
    stw r0, 0x0(r26)
    li r30, 0x0
    li r21, 0x0
    b L_8025AE80
L_8025ACFC:
    addi r3, r1, 0x14
    li r4, 0x0
    li r5, 0x40
    bl memset
    lwz r0, 0x0(r25)
    li r3, 0x800
    subf r0, r30, r0
    cmplwi r0, 0x800
    bgt L_8025AD24
    mr r3, r0
L_8025AD24:
    cmpwi r28, 0x0
    mr r31, r3
    li r0, 0xd0
    beq L_8025AD38
    li r0, 0xd1
L_8025AD38:
    cmpwi r28, 0x0
    stb r0, 0x18(r1)
    li r0, 0x40
    bne L_8025AD4C
    addi r0, r31, 0x40
L_8025AD4C:
    stw r0, 0x14(r1)
    addi r3, r1, 0xc
    addi r4, r1, 0x8
    stw r23, 0x1c(r1)
    sth r31, 0x20(r1)
    bl TRKGetFreeBuffer
    lwz r3, 0x8(r1)
    addi r4, r1, 0x14
    li r5, 0x40
    bl TRKAppendBuffer_ui8
    cmpwi r28, 0x0
    mr r21, r3
    bne L_8025AD9C
    cmpwi r21, 0x0
    bne L_8025AD9C
    lwz r3, 0x8(r1)
    mr r5, r31
    add r4, r24, r30
    bl TRKAppendBuffer_ui8
    mr r21, r3
L_8025AD9C:
    cmpwi r21, 0x0
    bne L_8025AE74
    cmpwi r27, 0x0
    beq L_8025AE68
    cmpwi r28, 0x0
    li r0, 0x0
    beq L_8025ADC4
    cmplwi r23, 0x0
    bne L_8025ADC4
    li r0, 0x1
L_8025ADC4:
    cmpwi r28, 0x0
    lwz r3, 0x8(r1)
    addi r4, r1, 0x10
    li r5, 0x5
    cntlzw r0, r0
    li r6, 0x3
    srwi r7, r0, 5
    bl TRKRequestSend
    mr. r21, r3
    bne L_8025ADF8
    lwz r3, 0x10(r1)
    bl TRKGetBuffer
    mr r22, r3
L_8025ADF8:
    lwz r0, 0x20(r22)
    cmpwi r28, 0x0
    lhz r19, 0x24(r22)
    clrlwi r20, r0, 24
    beq L_8025AE48
    cmpwi r21, 0x0
    bne L_8025AE48
    cmplw r19, r31
    bgt L_8025AE48
    mr r3, r22
    li r4, 0x40
    bl TRKSetBufferPosition
    mr r3, r22
    mr r5, r19
    add r4, r24, r30
    bl TRKReadBuffer_ui8
    mr r21, r3
    cmpwi r21, 0x302
    bne L_8025AE48
    li r21, 0x0
L_8025AE48:
    cmplw r19, r31
    beq L_8025AE58
    mr r31, r19
    li r29, 0x1
L_8025AE58:
    stw r20, 0x0(r26)
    lwz r3, 0x10(r1)
    bl TRKReleaseBuffer
    b L_8025AE74
L_8025AE68:
    lwz r3, 0x8(r1)
    bl TRKMessageSend
    mr r21, r3
L_8025AE74:
    lwz r3, 0xc(r1)
    bl TRKReleaseBuffer
    add r30, r30, r31
L_8025AE80:
    cmpwi r29, 0x0
    bne L_8025AEA8
    lwz r0, 0x0(r25)
    cmplw r30, r0
    bge L_8025AEA8
    cmpwi r21, 0x0
    bne L_8025AEA8
    lwz r0, 0x0(r26)
    cmpwi r0, 0x0
    beq L_8025ACFC
L_8025AEA8:
    stw r30, 0x0(r25)
    mr r3, r21
L_8025AEB0:
    lmw r19, 0x5c(r1)
    lwz r0, 0x94(r1)
    mtspr 8, r0
    addi r1, r1, 0x90
    blr
}
