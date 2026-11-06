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

#endif
