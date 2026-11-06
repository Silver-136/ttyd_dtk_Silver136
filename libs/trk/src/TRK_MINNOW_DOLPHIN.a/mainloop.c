#include "trk.h"

typedef int BOOL;
typedef int DSError;

typedef struct TRKEvent {
    int eventType;
    int eventID;
    int msgBufID;
} TRKEvent;

extern void* gTRKInputPendingPtr;

BOOL TRKGetNextEvent(TRKEvent* event);
void* TRKGetBuffer(int idx);
void TRKDispatchMessage(void* msg);
void TRKTargetInterrupt(TRKEvent* event);
void TRKTargetSupportRequest(void);
void TRKDestructEvent(TRKEvent* event);
void TRKGetInput(void);
BOOL TRKTargetStopped(void);
DSError TRKTargetContinue(void);

void TRKNubMainLoop(void) {
    void* msg;
    TRKEvent event;
    BOOL isShutdownRequested = 0;
    BOOL isNewInput = 0;

    while (isShutdownRequested == 0) {
        if (TRKGetNextEvent(&event) != 0) {
            isNewInput = 0;

            switch (event.eventType) {
            case TRK_EVENT_NULL:
                break;
            case TRK_EVENT_REQUEST:
                msg = TRKGetBuffer(event.msgBufID);
                TRKDispatchMessage(msg);
                break;
            case TRK_EVENT_SHUTDOWN:
                isShutdownRequested = 1;
                break;
            case TRK_EVENT_BREAKPOINT:
            case TRK_EVENT_EXCEPTION:
                TRKTargetInterrupt(&event);
                break;
            case TRK_EVENT_SUPPORT:
                TRKTargetSupportRequest();
                break;
            }

            TRKDestructEvent(&event);
            continue;
        }

        // gTRKInputPendingPtr points at the comm driver's input-pending byte (set in TRKInitializeNub)
        if ((isNewInput == 0) || (*(unsigned char*)gTRKInputPendingPtr != '\0')) {
            isNewInput = 1;
            TRKGetInput();
            continue;
        }

        if (TRKTargetStopped() == 0)
            TRKTargetContinue();

        isNewInput = 0;
    }
}
