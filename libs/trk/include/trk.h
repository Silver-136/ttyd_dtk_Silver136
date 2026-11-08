#ifndef TRK_TRK_H
#define TRK_TRK_H

/* Message command ids (byte 4 of a message). The request ids are the TRKDo* handler each one selects in
   TRKDispatchMessage; 0x90/0x91 are the stop and exception notifications (TRKTargetAddStopInfo /
   TRKTargetAddExceptionInfo); 0xD0-0xD4 are the file support requests of TRKTargetSupportRequest. */
typedef enum MessageCommandID {
    DSMSG_Connect = 0x01,
    DSMSG_Disconnect = 0x02,
    DSMSG_Reset = 0x03,
    DSMSG_Versions = 0x04,
    DSMSG_SupportMask = 0x05,
    DSMSG_Override = 0x07,
    DSMSG_ReadMemory = 0x10,
    DSMSG_WriteMemory = 0x11,
    DSMSG_ReadRegisters = 0x12,
    DSMSG_WriteRegisters = 0x13,
    DSMSG_SetOption = 0x17,
    DSMSG_Continue = 0x18,
    DSMSG_Step = 0x19,
    DSMSG_Stop = 0x1A,
    DSMSG_ReplyACK = 0x80,
    DSMSG_NotifyStopped = 0x90,
    DSMSG_NotifyException = 0x91,
    DSMSG_WriteFile = 0xD0,
    DSMSG_ReadFile = 0xD1,
    DSMSG_OpenFile = 0xD2,
    DSMSG_CloseFile = 0xD3,
    DSMSG_PositionFile = 0xD4
} MessageCommandID;

/* TRKEvent.eventType, as posted and handled in TRKNubMainLoop. */
enum TRKEventType {
    TRK_EVENT_NULL,       /* no action */
    TRK_EVENT_SHUTDOWN,   /* TRKDoDisconnect: leave the main loop */
    TRK_EVENT_REQUEST,    /* TRKTestForPacket: a message is waiting in a buffer */
    TRK_EVENT_BREAKPOINT, /* TRKPostInterruptEvent: program or trace exception that is no support request */
    TRK_EVENT_EXCEPTION,  /* TRKPostInterruptEvent: any other exception */
    TRK_EVENT_SUPPORT     /* TRKPostInterruptEvent: the support-request instruction 0x0FE00000 */
};

/* DSError values: what the TRK functions return. The names are MetroTRK's own spellings, as the community TRK
   decompilations (pikmin2, ac-decomp, zeldaret/tp) share them; the comments say where this code returns each. */
enum DSErrorCode {
    DS_NoError = 0,
    DS_ParameterError = 2,             /* TRKSuppAccessFile: no buffer or a zero count */
    DS_EventQueueFull = 0x100,         /* TRKPostEvent: both event slots are taken */
    DS_NoMessageBufferAvailable = 0x300, /* TRKGetFreeBuffer: all three message buffers are in use */
    DS_MessageBufferOverflow = 0x301,  /* TRKSetBufferPosition/TRKAppendBuffer: past the end of the buffer */
    DS_MessageBufferReadError = 0x302, /* TRKReadBuffer: more bytes asked than the message holds */
    DS_DispatchError = 0x500,          /* TRKDispatchMessage: no case for the command id */
    DS_InvalidMemory = 0x700,          /* TRKValidMemory32 / TRKTargetReadInstruction: range not readable or writable */
    DS_InvalidRegister = 0x701,        /* TRKTargetAccess* : last register number out of range */
    DS_CWDSException = 0x702,          /* TRKTargetAccess*: an exception was detected during the access */
    DS_UnsupportedError = 0x703,       /* step-over requests, unknown register kind */
    DS_InvalidProcessID = 0x704,       /* 0x704-0x706: only mapped in the handlers; nothing here returns them */
    DS_InvalidThreadID = 0x705,
    DS_OSError = 0x706,
    DS_Error800 = 0x800                /* TRKRequestSend: no reply after all tries */
};

/* Reply error byte of the DSMSG_ReplyACK message (TRKSendReply); the handlers map a DSError to it. MetroTRK
   spellings, as above. */
enum DSReplyError {
    DSREPLY_NoError = 0,
    DSREPLY_Error = 1,                    /* TRKDoStop: any other stop result */
    DSREPLY_PacketSizeError = 2,          /* DS_MessageBufferReadError */
    DSREPLY_CWDSError = 3,                /* any other error */
    DSREPLY_ParameterError = 0x11,        /* TRKDoStep: zero count or pc outside the range */
    DSREPLY_UnsupportedOptionError = 0x12, /* DS_UnsupportedError; unknown option bit or step kind */
    DSREPLY_InvalidMemoryRange = 0x13,    /* DS_InvalidMemory */
    DSREPLY_InvalidRegisterRange = 0x14,  /* DS_InvalidRegister; first register after the last */
    DSREPLY_CWDSException = 0x15,         /* DS_CWDSException */
    DSREPLY_NotStopped = 0x16,            /* TRKDoContinue/TRKDoStep: the target is running */
    DSREPLY_OSError = 0x20,               /* DS_OSError */
    DSREPLY_InvalidProcessID = 0x21,      /* DS_InvalidProcessID */
    DSREPLY_InvalidThreadID = 0x22        /* DS_InvalidThreadID */
};

/* TRKDoStep options (byte 8 of the step request): step a count of instructions or until the pc leaves a range,
   into or over calls. */
enum DSMessageStepOptions {
    DSSTEP_IntoCount = 0x00,
    DSSTEP_IntoRange = 0x01,
    DSSTEP_OverCount = 0x10,
    DSSTEP_OverRange = 0x11
};

/* I/O result of a file support request (the reply word at data + 0x10), as the MSL console hooks return it. */
typedef enum DSIOResult {
    DS_IONoError,
    DS_IOError,
    DS_IOEOF
} DSIOResult;

/* File handles of the console streams (the file_handle of a file support request). */
typedef enum DSFileHandle {
    DS_Stdin,
    DS_Stdout,
    DS_Stderr
} DSFileHandle;

#endif
