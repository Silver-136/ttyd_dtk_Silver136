#include "event/evt_paper.h"

s32 evt_paper_delete(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 animPaperPoseGetId(s32 name, s32 flag);
    extern void animPaperPoseRelease(s32 id);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 flag = *(s32*)((s32)gp + 0x14);

    animPaperPoseRelease(animPaperPoseGetId(name, ((u32)(-flag) | (u32)flag) >> 31));
    return 2;
}

s32 evt_paper_entry(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 animGroupBaseAsync(s32 name, s32 flag, s32 value);
    extern void animPaperPoseEntry(s32 name, s32 flag);
    extern void* gp;
    s32* args = *(s32**)((s32)event + 0x18);
    s32 name = evtGetValue(event, args[0]);
    s32 flag = *(s32*)((s32)gp + 0x14);
    s32 normalized = ((u32)(-flag) | (u32)flag) >> 31;

    if (animGroupBaseAsync(name, normalized, 0) == 0) {
        return 0;
    }
    animPaperPoseEntry(name, normalized);
    return 2;
}
