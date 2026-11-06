#include "trk.h"

typedef int DSError;
typedef struct TRKBuffer TRKBuffer;

DSError TRKGetFreeBuffer(int* bufIdx, TRKBuffer** buf);
void TRKReleaseBuffer(int idx);
DSError TRKRequestSend(TRKBuffer* msg, int* bufIdx, unsigned int unused, unsigned int retryCount, int useTimeout);
DSError TRKTargetAddStopInfo(TRKBuffer* msg);
void TRKTargetAddExceptionInfo(TRKBuffer* msg);

DSError TRKDoNotifyStopped(int cmd) {
    DSError err;
    int reqIdx;
    int bufIdx;
    TRKBuffer* msg;

    err = TRKGetFreeBuffer(&bufIdx, &msg);

    if (err == 0) {
        // FAKE: the same test twice; removing the inner one changes the object
        if (err == 0) {
            if (cmd == DSMSG_NotifyStopped)
                TRKTargetAddStopInfo(msg);
            else
                TRKTargetAddExceptionInfo(msg);
        }

        err = TRKRequestSend(msg, &reqIdx, 2, 3, 1);

        if (err == 0)
            TRKReleaseBuffer(reqIdx);

        TRKReleaseBuffer(bufIdx);
    }

    return err;
}
