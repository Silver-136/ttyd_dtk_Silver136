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


/* DSError values: what the TRK functions return. Each is named by where the code returns it. */
enum DSErrorCode {
    DSERR_EVENT_QUEUE_FULL = 0x100,   /* TRKPostEvent: both event slots are taken */
    DSERR_NO_FREE_BUFFER = 0x300,     /* TRKGetFreeBuffer: all three message buffers are in use */
    DSERR_BUFFER_OVERFLOW = 0x301,    /* TRKSetBufferPosition/TRKAppendBuffer: past the end of the buffer */
    DSERR_BUFFER_UNDERFLOW = 0x302,   /* TRKReadBuffer: more bytes asked than the message holds */
    DSERR_UNKNOWN_COMMAND = 0x500,    /* TRKDispatchMessage: no case for the command id */
    DSERR_INVALID_MEMORY = 0x700,     /* TRKValidMemory32 / TRKTargetReadInstruction: range not readable or writable */
    DSERR_INVALID_REGISTER = 0x701,   /* TRKTargetAccess* : last register number out of range */
    DSERR_EXCEPTION = 0x702,          /* TRKTargetAccess*: an exception was detected during the access */
    DSERR_UNSUPPORTED = 0x703        /* step-over requests, unknown register kind */
    /* 0x704-0x706 are only mapped in the handlers (replies 0x21, 0x22, 0x20); nothing here returns them */
};

/* Reply error byte of the DSMSG_ReplyACK message (TRKSendReply); the handlers map a DSError to it.
   Other values used: 1 (TRKDoStop default), 3 (any other error), 0x20-0x22 (for 0x706, 0x704, 0x705). */
enum DSReplyError {
    DSREPLY_OK = 0,
    DSREPLY_PACKET_SIZE = 2,         /* DSERR_BUFFER_UNDERFLOW */
    DSREPLY_INVALID_PARAMETER = 0x11, /* TRKDoStep: zero count or pc outside the range */
    DSREPLY_UNSUPPORTED = 0x12,      /* DSERR_UNSUPPORTED; unknown option bit or step kind */
    DSREPLY_INVALID_MEMORY = 0x13,   /* DSERR_INVALID_MEMORY */
    DSREPLY_INVALID_REGISTER = 0x14, /* DSERR_INVALID_REGISTER; first register after the last */
    DSREPLY_EXCEPTION = 0x15,        /* DSERR_EXCEPTION */
    DSREPLY_NOT_STOPPED = 0x16       /* TRKDoContinue/TRKDoStep: the target is running */
};

#endif
