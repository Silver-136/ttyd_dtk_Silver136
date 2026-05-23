#include "battle/battle_message.h"

s32 btlevtcmd_AnnounceSetParam(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32* params = (s32*)((s32)battleWork + 0x18C8C);
    s32 index = evtGetValue(evt, args[0]);

    params[index + 1] = evtGetValue(evt, args[1]);
    return 2;
}
