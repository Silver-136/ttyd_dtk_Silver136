#include "trk.h"

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

/* The functions are in reverse address order (-inline deferred emits a file's functions last to first). The C of
   TRKSuppAccessFile, TRKRequestSend and HandleOpenFileSupportRequest follows the matched GC 2.6 decompilations of
   pikmin2 and ac-decomp (CC0). */

/* Reads or writes *count bytes of a file in requests of up to 0x800 bytes; *count returns the bytes moved. */
DSError TRKSuppAccessFile(u32 file_handle, u8* data, size_t* count, DSIOResult* io_result, BOOL need_reply,
                          BOOL read) {
    DSError error;
    int replyBufferId;
    TRKBuffer* replyBuffer;
    u32 length;
    int bufferId;
    TRKBuffer* buffer;
    u32 i;
    u8 replyIOResult;
    u32 replyLength;
    BOOL exit;
    CommandReply reply;

    if (data == 0 || *count == 0)
        return 2;

    exit = 0;
    *io_result = DS_IONoError;
    i = 0;
    error = 0;
    while (!exit && i < *count && error == 0 && *io_result == DS_IONoError) {
        memset(&reply, 0, sizeof(CommandReply));

        if (*count - i <= 0x800)
            length = *count - i;
        else
            length = 0x800;

        reply.commandID = read ? DSMSG_ReadFile : DSMSG_WriteFile;

        if (read)
            reply.length = sizeof(CommandReply);
        else
            reply.length = length + sizeof(CommandReply);

        reply.r8.w = file_handle;
        reply.rC.h = length;

        TRKGetFreeBuffer(&bufferId, &buffer);
        error = TRKAppendBuffer_ui8(buffer, (u8*)&reply, sizeof(CommandReply));

        if (!read && error == 0)
            error = TRKAppendBuffer_ui8(buffer, data + i, length);

        if (error == 0) {
            if (need_reply) {
                // a console read waits without a time limit
                BOOL waitForever = read && file_handle == DS_Stdin;

                // the target code tests read for the third argument although both arms pass 5
                error = TRKRequestSend(buffer, &replyBufferId, read ? 5 : 5, 3, !waitForever);
                if (error == 0)
                    replyBuffer = TRKGetBuffer(replyBufferId);

                replyIOResult = *(u32*)(replyBuffer->data + 0x10);
                replyLength = *(u16*)(replyBuffer->data + 0x14);
                if (read && error == 0 && replyLength <= length) {
                    TRKSetBufferPosition(replyBuffer, sizeof(CommandReply));
                    error = TRKReadBuffer_ui8(replyBuffer, data + i, replyLength);
                    if (error == 0x302)
                        error = 0;
                }

                if (replyLength != length) {
                    length = replyLength;
                    exit = 1;
                }

                *io_result = replyIOResult;
                TRKReleaseBuffer(replyBufferId);
            } else {
                error = TRKMessageSend(buffer);
            }
        }

        TRKReleaseBuffer(bufferId);
        i += length;
    }

    *count = i;
    return error;
}

/* Sends msgBuf and waits for the reply, trying retryCount + 1 times; unused is never read; with useTimeout zero
   the wait for a packet has no limit. */
DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 unused, u32 retryCount, int useTimeout) {
    DSError error = 0;
    TRKBuffer* buffer;
    u32 counter;
    int count;
    u8 msgCmd;
    int msgReplyError;
    BOOL badReply = 1;

    *bufferId = -1;

    for (count = retryCount + 1; count != 0 && *bufferId == -1 && error == 0; count--) {
        MWTRACE(1, "Calling MessageSend\n");
        error = TRKMessageSend(msgBuf);
        if (error == 0) {
            if (useTimeout)
                counter = 0;

            while (1) {
                do {
                    *bufferId = TRKTestForPacket();
                    if (*bufferId != -1)
                        break;
                } while (!useTimeout || ++counter < 79999980);

                if (*bufferId == -1)
                    break;

                badReply = 0;

                buffer = TRKGetBuffer(*bufferId);
                TRKSetBufferPosition(buffer, 0);
                OutputData(&buffer->data[0], buffer->length);
                msgCmd = buffer->data[4];
                MWTRACE(1, "msg_command : 0x%02x hdr->cmdID 0x%02x\n", msgCmd, msgCmd);

                if (msgCmd >= DSMSG_ReplyACK)
                    break;

                TRKProcessInput(*bufferId);
                *bufferId = -1;
            }

            if (*bufferId != -1) {
                if (buffer->length < sizeof(CommandReply))
                    badReply = 1;

                if (error == 0 && !badReply) {
                    msgReplyError = buffer->data[8];
                    MWTRACE(1, "msg_error : 0x%02x\n", msgReplyError);
                }

                if (error == 0 && !badReply) {
                    if ((int)msgCmd != DSMSG_ReplyACK || msgReplyError != 0) {
                        MWTRACE(8, "RequestSend : Bad ack or non ack received msg_command : 0x%02x msg_error 0x%02x\n",
                                msgCmd, msgReplyError);
                        badReply = 1;
                    }
                }

                if (error != 0 || badReply) {
                    TRKReleaseBuffer(*bufferId);
                    *bufferId = -1;
                }
            }
        }
    }

    if (*bufferId == -1)
        error = 0x800;

    return error;
}

/* Sends an open request for path (requestArg goes in the byte at reply offset 8) and returns the reply's file
   handle in *fileHandle and its I/O result in *ioResult. */
DSError HandleOpenFileSupportRequest(const char* path, u8 requestArg, u32* fileHandle, DSIOResult* ioResult) {
    DSError error;
    int replyBufferId;
    int bufferId;
    TRKBuffer* replyBuffer;
    TRKBuffer* buffer;
    CommandReply reply;

    memset(&reply, 0, sizeof(CommandReply));
    *fileHandle = 0;
    reply.commandID = DSMSG_OpenFile;
    reply.length = strlen(path) + sizeof(CommandReply) + 1;
    reply.r8.b = requestArg;
    reply.rC.h = strlen(path) + 1;
    TRKGetFreeBuffer(&bufferId, &buffer);
    error = TRKAppendBuffer_ui8(buffer, (u8*)&reply, sizeof(CommandReply));

    if (error == 0)
        error = TRKAppendBuffer_ui8(buffer, (const u8*)path, strlen(path) + 1);

    if (error == 0) {
        *ioResult = DS_IONoError;
        error = TRKRequestSend(buffer, &replyBufferId, 7, 3, 0);

        if (error == 0)
            replyBuffer = TRKGetBuffer(replyBufferId);

        // BUG: on a send error replyBuffer is read without being set
        *ioResult = *(u32*)(replyBuffer->data + 0x10);
        *fileHandle = *(u32*)(replyBuffer->data + 0x8);
        TRKReleaseBuffer(replyBufferId);
    }

    TRKReleaseBuffer(bufferId);
    return error;
}

DSError HandleCloseFileSupportRequest(int file_handle, DSIOResult* ioResult) {
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

        *ioResult = DS_IONoError;
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

DSError HandlePositionFileSupportRequest(u32 file_handle, u32* position, u8 whence, DSIOResult* ioResult) {
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
        *ioResult = DS_IONoError;
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
