#include "trk.h"

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

/* A file support request (0x40 bytes): r8 = the file handle (as a byte, the open request's argument), rC = the
   file position (as a halfword, the byte count of a read or write). */
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
    u8 whence;
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

DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 unused, u32 retryCount, int useTimeout);

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
    reply.whence = whence;

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

/* Sends an open request for path (requestArg goes in the byte at reply offset 8) and returns the reply's file
   handle in *fileHandle and its I/O result in *ioResult. */
asm DSError HandleOpenFileSupportRequest(const char* path, u8 requestArg, u32* fileHandle, u32* ioResult) {
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
    bne skip
    mr r3, r27
    bl strlen
    mr r5, r3
    lwz r3, 0x8(r1)
    mr r4, r27
    addi r5, r5, 0x1
    bl TRKAppendBuffer_ui8
    mr r31, r3
skip:
    cmpwi r31, 0x0
    bne done
    li r0, 0x0
    addi r4, r1, 0x10
    stw r0, 0x0(r29)
    li r5, 0x7
    li r6, 0x3
    li r7, 0x0
    lwz r3, 0x8(r1)
    bl TRKRequestSend
    mr. r31, r3
    bne skip2
    lwz r3, 0x10(r1)
    bl TRKGetBuffer
    mr r30, r3
skip2:
    lwz r0, 0x20(r30)
    stw r0, 0x0(r29)
    lwz r0, 0x18(r30)
    stw r0, 0x0(r28)
    lwz r3, 0x10(r1)
    bl TRKReleaseBuffer
done:
    lwz r3, 0xc(r1)
    bl TRKReleaseBuffer
    mr r3, r31
    lmw r27, 0x5c(r1)
    lwz r0, 0x74(r1)
    mtspr 8, r0
    addi r1, r1, 0x70
    blr
}

/* Sends msgBuf and waits for the reply, trying retryCount + 1 times; unused (r5) is never read; with useTimeout
   zero the wait for a packet has no limit. */
asm DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 unused, u32 retryCount, int useTimeout) {
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
    b skip9
loop:
    addi r4, r31, 0x0
    li r3, 0x1
    crclr 6
    bl MWTRACE
    mr r3, r21
    bl TRKMessageSend
    mr. r30, r3
    bne skip8
    cmpwi r23, 0x0
    beq loop2
    li r28, 0x0
loop2:
    bl TRKTestForPacket
    stw r3, 0x0(r22)
    lwz r3, 0x0(r22)
    cmpwi r3, -0x1
    bne skip
    cmpwi r23, 0x0
    beq loop2
    lis r4, 0x4c5
    addi r28, r28, 0x1
    subi r0, r4, 0x4c14
    cmplw r28, r0
    blt loop2
skip:
    cmpwi r3, -0x1
    beq skip2
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
    bge skip2
    lwz r3, 0x0(r22)
    bl TRKProcessInput
    li r0, -0x1
    stw r0, 0x0(r22)
    b loop2
skip2:
    lwz r0, 0x0(r22)
    cmpwi r0, -0x1
    beq skip8
    lwz r0, 0x8(r29)
    cmplwi r0, 0x40
    bge skip3
    li r24, 0x1
skip3:
    cmpwi r30, 0x0
    bne skip4
    cmpwi r24, 0x0
    bne skip4
    lbz r25, 0x18(r29)
    addi r4, r31, 0x40
    li r3, 0x1
    mr r5, r25
    crclr 6
    bl MWTRACE
skip4:
    cmpwi r30, 0x0
    bne skip6
    cmpwi r24, 0x0
    bne skip6
    cmpwi r26, 0x80
    mr r5, r26
    bne skip5
    cmpwi r25, 0x0
    beq skip6
skip5:
    mr r6, r25
    addi r4, r31, 0x54
    li r3, 0x8
    crclr 6
    bl MWTRACE
    li r24, 0x1
skip6:
    cmpwi r30, 0x0
    bne skip7
    cmpwi r24, 0x0
    beq skip8
skip7:
    lwz r3, 0x0(r22)
    bl TRKReleaseBuffer
    li r0, -0x1
    stw r0, 0x0(r22)
skip8:
    subi r27, r27, 0x1
skip9:
    cmpwi r27, 0x0
    beq skip10
    lwz r0, 0x0(r22)
    cmpwi r0, -0x1
    bne skip10
    cmpwi r30, 0x0
    beq loop
skip10:
    lwz r0, 0x0(r22)
    cmpwi r0, -0x1
    bne done
    li r30, 0x800
done:
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
    beq skip
    lwz r0, 0x0(r25)
    cmplwi r0, 0x0
    bne skip2
skip:
    li r3, 0x2
    b done
skip2:
    li r0, 0x0
    li r29, 0x0
    stw r0, 0x0(r26)
    li r30, 0x0
    li r21, 0x0
    b skip13
loop:
    addi r3, r1, 0x14
    li r4, 0x0
    li r5, 0x40
    bl memset
    lwz r0, 0x0(r25)
    li r3, 0x800
    subf r0, r30, r0
    cmplwi r0, 0x800
    bgt skip3
    mr r3, r0
skip3:
    cmpwi r28, 0x0
    mr r31, r3
    li r0, 0xd0
    beq skip4
    li r0, 0xd1
skip4:
    cmpwi r28, 0x0
    stb r0, 0x18(r1)
    li r0, 0x40
    bne skip5
    addi r0, r31, 0x40
skip5:
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
    bne skip6
    cmpwi r21, 0x0
    bne skip6
    lwz r3, 0x8(r1)
    mr r5, r31
    add r4, r24, r30
    bl TRKAppendBuffer_ui8
    mr r21, r3
skip6:
    cmpwi r21, 0x0
    bne skip12
    cmpwi r27, 0x0
    beq skip11
    cmpwi r28, 0x0
    li r0, 0x0
    beq skip7
    cmplwi r23, 0x0
    bne skip7
    li r0, 0x1
skip7:
    cmpwi r28, 0x0
    lwz r3, 0x8(r1)
    addi r4, r1, 0x10
    li r5, 0x5
    cntlzw r0, r0
    li r6, 0x3
    srwi r7, r0, 5
    bl TRKRequestSend
    mr. r21, r3
    bne skip8
    lwz r3, 0x10(r1)
    bl TRKGetBuffer
    mr r22, r3
skip8:
    lwz r0, 0x20(r22)
    cmpwi r28, 0x0
    lhz r19, 0x24(r22)
    clrlwi r20, r0, 24
    beq skip9
    cmpwi r21, 0x0
    bne skip9
    cmplw r19, r31
    bgt skip9
    mr r3, r22
    li r4, 0x40
    bl TRKSetBufferPosition
    mr r3, r22
    mr r5, r19
    add r4, r24, r30
    bl TRKReadBuffer_ui8
    mr r21, r3
    cmpwi r21, 0x302
    bne skip9
    li r21, 0x0
skip9:
    cmplw r19, r31
    beq skip10
    mr r31, r19
    li r29, 0x1
skip10:
    stw r20, 0x0(r26)
    lwz r3, 0x10(r1)
    bl TRKReleaseBuffer
    b skip12
skip11:
    lwz r3, 0x8(r1)
    bl TRKMessageSend
    mr r21, r3
skip12:
    lwz r3, 0xc(r1)
    bl TRKReleaseBuffer
    add r30, r30, r31
skip13:
    cmpwi r29, 0x0
    bne skip14
    lwz r0, 0x0(r25)
    cmplw r30, r0
    bge skip14
    cmpwi r21, 0x0
    bne skip14
    lwz r0, 0x0(r26)
    cmpwi r0, 0x0
    beq loop
skip14:
    stw r30, 0x0(r25)
    mr r3, r21
done:
    lmw r19, 0x5c(r1)
    lwz r0, 0x94(r1)
    mtspr 8, r0
    addi r1, r1, 0x90
    blr
}
