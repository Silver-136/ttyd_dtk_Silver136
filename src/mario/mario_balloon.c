#include "mario/mario_balloon.h"


u8 marioBalloonMain(void) {
    return 0;
}


extern u32 hitGetAttr(void* hitObj);
extern void* mobjHitObjPtrToPtr(void* hitObj);
extern s32 mobjGetHint(void* mobj);
extern void* caseCheckHitObj(void* hitObj);
extern s32 marioChkBero(void* hitObj);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 chkBalloon(void* hitObj) {
    s32 result;
    void* entry;
    s32 kind;

    result = 0;
    if (hitObj != 0) {
        if ((hitGetAttr(hitObj) & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hitObj)) == 1) {
                result = 1;
            }
        } else {
            entry = caseCheckHitObj(hitObj);
            if (entry != 0) {
                if ((*(u16*)entry & 4) != 0) {
                    kind = *(s32*)((s32)entry + 4);
                    if (kind == 9 || kind == 4 || kind == 0xC || kind == 5) {
                        result = 1;
                    }
                }
            } else {
                result = 0;
            }
        }

        if (marioChkBero(hitObj) != 0) {
            result = 0;
        }
    }

    return result;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioBalloonOn(s32 type) {
    extern void* marioGetPtr(void);
    extern s32 effMarioBalloonEntry(s32 type, s32 arg1);
    void* mario;
    u32 flags;

    mario = marioGetPtr();
    flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x02000000) == 0) {
        *(u32*)((s32)mario + 4) = flags | 0x02000000;
        *(s32*)((s32)mario + 0x264) = effMarioBalloonEntry(type, 0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void marioBalloonOff(void) {
    extern void* marioGetPtr(void);
    extern void effDelete(s32 effect);
    void* mario;
    u32 flags;

    mario = marioGetPtr();
    flags = *(u32*)((s32)mario + 4);
    if ((flags & 0x02000000) != 0) {
        *(u32*)((s32)mario + 4) = flags & ~0x02000000;
        effDelete(*(s32*)((s32)mario + 0x264));
        *(s32*)((s32)mario + 0x264) = 0;
        *(s32*)((s32)mario + 0x268) = 0;
    }
}

void marioBaloonInit(void) {
    extern void* marioGetPtr(void);

    *(s32*)((s32)marioGetPtr() + 0x264) = 0;
}
