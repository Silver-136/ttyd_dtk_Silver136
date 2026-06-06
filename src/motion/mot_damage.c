#include "motion/mot_damage.h"

extern s32 quake_bgmode_evt[8];
extern s32 quake_evt[8];
extern f32 float_1_80422b78;

void* marioGetPtr(void);
s32 marioBgmodeChk(void);
void* evtEntry(void* script, s32 priority, s32 flags);
void psndSFXOn_3D(s32 id, void* pos);
void effSplashEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
void marioGetScreenPos(void* pos, f32* x, f32* y, f32* z);
s32 marioChkInScreen(s32 x, s32 y);

typedef struct DamageVec {
    f32 x;
    f32 y;
    f32 z;
} DamageVec;

void set_damage_root_ypos(f32 y) {
    f32 temp = y;
    void* mario = marioGetPtr();
    *(f32*)((s32)mario + 0x2B8) = temp;
}

void marioDmgRetQuakeOn(void) {
    void* script;

    if (marioBgmodeChk() == 0) {
        script = quake_evt;
    } else {
        script = quake_bgmode_evt;
    }
    evtEntry(script, 0, 0);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioSetSplash(s32 type, f32* pos) {
    f32* p = pos;

    if (type == 0) {
        psndSFXOn_3D(0x194, p);
    }
    if (marioBgmodeChk() != 1) {
        effSplashEntry(((u32)(-type) | (u32)type) >> 31, p[0], p[1], p[2], float_1_80422b78);
    }
    if (type == 1) {
        psndSFXOn_3D(0xBA, p);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 mot_damage(void) {
    return 0;
}


u8 mot_damageToge(void) {
    return 0;
}


s32 marioWaitDamageTogeReturnFall(void) {
    s32 mario = (s32)marioGetPtr();
    DamageVec pos;
    f32 x;
    f32 y;
    f32 z;
    u32 px;
    u32 py;

    if (*(u16*)(mario + 0x2E) != 0x20) {
        return -1;
    }
    if (*(s32*)(mario + 0x44) >= 20) {
        px = *(u32*)(mario + 0x8C);
        py = *(u32*)(mario + 0x90);
        *(u32*)&pos.x = px;
        *(u32*)&pos.y = py;
        *(u32*)&pos.z = *(u32*)(mario + 0x94);
        marioGetScreenPos(&pos, &x, &y, &z);
        if (marioChkInScreen((s32)x, (s32)y) != 0) {
            return 1;
        }
    }
    return 0;
}


s32 marioWaitDamageReturnFall(void) {
    s32 mario = (s32)marioGetPtr();
    DamageVec pos;
    f32 x;
    f32 y;
    f32 z;
    u32 px;
    u32 py;

    if (*(u16*)(mario + 0x2E) != 0x1F) {
        return -1;
    }
    if (*(s32*)(mario + 0x44) >= 20) {
        px = *(u32*)(mario + 0x8C);
        py = *(u32*)(mario + 0x90);
        *(u32*)&pos.x = px;
        *(u32*)&pos.y = py;
        *(u32*)&pos.z = *(u32*)(mario + 0x94);
        marioGetScreenPos(&pos, &x, &y, &z);
        if (marioChkInScreen((s32)x, (s32)y) != 0) {
            return 1;
        }
    }
    return 0;
}


s32 gazigazi(void* pEvt) {
    return 0;
}


s32 marioChkDamageSink(void) {
    return 0;
}
