#include "event/evt_lecture.h"

extern void* lect_work;
extern void evtSetValue();
extern s32 evtGetValue();
extern void marioStSystemLevel();
extern void marioStGetSystemLevel();
extern void* camGetPtr(s32 cameraId);
extern void* marioGetPtr(void);
extern s32 marioGetPartyId(void);
extern s32 marioGetExtraPartyId(void);
extern void* partyGetPtr(s32 id);

extern u32 stPt[3];
extern u32 stAt[3];
extern u32 stMpos[3];
extern u32 stPpos[3];
extern u32 stXpos[3];
extern f32 stMdir1;
extern f32 stMdir2;
extern f32 stMdir3;
extern f32 stMdir4;
extern f32 stPdir1;
extern f32 stPdir2;
extern f32 stPdir3;
extern f32 stPdir4;
extern f32 stXdir1;
extern f32 stXdir2;
extern f32 stXdir3;
extern f32 stXdir4;

s32 lect_juyoitem_rel_get(void* event) {
    evtSetValue(event, **(s32**)((s32)event + 0x18), lect_work);
    return 2;
}

s32 lect_juyoitem_rel_set(void* event) {
    lect_work = (void*)evtGetValue(event, **(s32**)((s32)event + 0x18));
    return 2;
}

s32 lect_set_systemlevel(void* event) {
    marioStSystemLevel(evtGetValue(event, **(s32**)((s32)event + 0x18)));
    return 2;
}

s32 lect_test_systemlevel(void) {
    marioStGetSystemLevel();
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 lect_cam_save(void) {
    void* cam = camGetPtr(4);
    void* mario = marioGetPtr();
    void* party = partyGetPtr(marioGetPartyId());
    void* extraParty = partyGetPtr(marioGetExtraPartyId());

    stPt[0] = *(u32*)((s32)cam + 0xC);
    stPt[1] = *(u32*)((s32)cam + 0x10);
    stPt[2] = *(u32*)((s32)cam + 0x14);
    stAt[0] = *(u32*)((s32)cam + 0x18);
    stAt[1] = *(u32*)((s32)cam + 0x1C);
    stAt[2] = *(u32*)((s32)cam + 0x20);

    stMpos[0] = *(u32*)((s32)mario + 0x8C);
    stMpos[1] = *(u32*)((s32)mario + 0x90);
    stMpos[2] = *(u32*)((s32)mario + 0x94);
    stMdir1 = *(f32*)((s32)mario + 0x1AC);
    stMdir2 = *(f32*)((s32)mario + 0x1B0);
    stMdir3 = *(f32*)((s32)mario + 0x1A4);
    stMdir4 = *(f32*)((s32)mario + 0x1A0);

    if (party != NULL) {
        stPpos[0] = *(u32*)((s32)party + 0x58);
        stPpos[1] = *(u32*)((s32)party + 0x5C);
        stPpos[2] = *(u32*)((s32)party + 0x60);
        stPdir1 = *(f32*)((s32)party + 0x10C);
        stPdir2 = *(f32*)((s32)party + 0x110);
        stPdir3 = *(f32*)((s32)party + 0x100);
        stPdir4 = *(f32*)((s32)party + 0xFC);
    }

    if (extraParty != NULL) {
        stXpos[0] = *(u32*)((s32)extraParty + 0x58);
        stXpos[1] = *(u32*)((s32)extraParty + 0x5C);
        stXpos[2] = *(u32*)((s32)extraParty + 0x60);
        stXdir1 = *(f32*)((s32)extraParty + 0x10C);
        stXdir2 = *(f32*)((s32)extraParty + 0x110);
        stXdir3 = *(f32*)((s32)extraParty + 0x100);
        stXdir4 = *(f32*)((s32)extraParty + 0xFC);
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
