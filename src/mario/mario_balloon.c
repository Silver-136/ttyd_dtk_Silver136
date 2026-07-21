#include "mario/mario_balloon.h"

extern u32 hitGetAttr(void* hitObj);
extern void* mobjHitObjPtrToPtr(void* hitObj);
extern s32 mobjGetHint(void* mobj);
extern void* caseCheckHitObj(void* hitObj);
extern s32 marioChkBero(void* hitObj);

void marioBaloonInit(void) {
    extern void* marioGetPtr(void);

    *(s32*)((s32)marioGetPtr() + 0x264) = 0;
}

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

u8 marioBalloonMain(void) {
    extern void* marioGetPtr(void);
    extern s32 marioChkKey(void);
    extern u32 hitGetAttr(void* hitObj);
    extern void* mobjHitObjPtrToPtr(void* hitObj);
    extern s32 mobjGetHint(void* mobj);
    extern void* caseCheckHitObj(void* hitObj);
    extern s32 marioChkBero(void* hitObj);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 mobjCheckItemboxOpen(void* mobj);
    extern void* fbatNpcTalkCheck(void);
    extern void effDelete(void* eff);
    extern void* effMarioBalloonEntry(s32 type, void* npc);
    extern char str_MOBJ_TreasureBox_802f35d0[];
    extern char str_MOBJ_BigTreasureBox_802f35e4[];

    void* player;
    void* hitObj;
    void* entry;
    void* mobj;
    void* npc;
    u32 attr;
    s32 show;
    s32 type;
    s32 check;

    player = marioGetPtr();
    if ((*(u32*)((s32)player + 4) & 0x04000000) != 0) {
        return 0;
    }

    show = 0;
    if (marioChkKey() == 0) {
        player = marioGetPtr();
        if ((*(u32*)((s32)player + 4) & 0x02000000) != 0) {
            *(u32*)((s32)player + 4) &= ~0x02000000;
            effDelete(*(void**)((s32)player + 0x264));
            *(void**)((s32)player + 0x264) = 0;
            *(void**)((s32)player + 0x268) = 0;
        }
        return 0;
    }

    type = 0;
    hitObj = *(void**)((s32)player + 0x1E0);
    check = 0;
    if (hitObj != 0) {
        attr = hitGetAttr(hitObj);
        if ((attr & 0x00800000) == 0) {
            if ((attr & 0x80000000) != 0) {
                if (mobjGetHint(mobjHitObjPtrToPtr(hitObj)) == 1) {
                    check = 1;
                }
            } else {
                entry = caseCheckHitObj(hitObj);
                if (entry != 0 && (*(u16*)entry & 4) != 0 && *(s32*)((s32)entry + 4) == 9) {
                    check = 1;
                }
            }
        }
    }
    if (check != 0) {
        type = 0;
        show = 1;
    }

    hitObj = *(void**)((s32)player + 0x1E4);
    check = 0;
    if (hitObj != 0) {
        attr = hitGetAttr(hitObj);
        if ((attr & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hitObj)) == 1) {
                check = 1;
            }
        } else {
            entry = caseCheckHitObj(hitObj);
            if (entry != 0 && (*(u16*)entry & 4) != 0) {
                if (*(s32*)((s32)entry + 4) == 9 || *(s32*)((s32)entry + 4) == 0xC) {
                    check = 1;
                }
            }
        }
        if (marioChkBero(hitObj) != 0) {
            check = 0;
        }
    }
    if (check != 0) {
        type = 0;
        show = 1;
    }

    hitObj = *(void**)((s32)player + 0x1E8);
    check = 0;
    if (hitObj != 0) {
        attr = hitGetAttr(hitObj);
        if ((attr & 0x00800000) == 0) {
            if ((attr & 0x80000000) != 0) {
                if (mobjGetHint(mobjHitObjPtrToPtr(hitObj)) == 1) {
                    check = 1;
                }
            } else {
                entry = caseCheckHitObj(hitObj);
                if (entry != 0 && (*(u16*)entry & 4) != 0) {
                    if (*(s32*)((s32)entry + 4) == 0xC || *(s32*)((s32)entry + 4) == 5) {
                        check = 1;
                    }
                }
            }
            if (marioChkBero(hitObj) != 0) {
                check = 0;
            }
        }
    }
    if (check != 0) {
        type = 0;
        show = 1;
    }

    hitObj = *(void**)((s32)player + 0x1E4);
    if (hitObj != 0) {
        check = -1;
        marioGetPtr();
        attr = hitGetAttr(hitObj);
        if ((attr & 0x80000000) != 0) {
            mobj = mobjHitObjPtrToPtr(hitObj);
            if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_TreasureBox_802f35d0) == 0 ||
                strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BigTreasureBox_802f35e4) == 0) {
                check = 1;
                if (mobjCheckItemboxOpen(mobj) != 1) {
                    check = 0;
                }
            }
        }
        if (check > -1) {
            hitObj = *(void**)((s32)player + 0x1E4);
            type = -1;
            marioGetPtr();
            attr = hitGetAttr(hitObj);
            if ((attr & 0x80000000) != 0) {
                mobj = mobjHitObjPtrToPtr(hitObj);
                if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_TreasureBox_802f35d0) == 0 ||
                    strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BigTreasureBox_802f35e4) == 0) {
                    type = 1;
                    if (mobjCheckItemboxOpen(mobj) != 1) {
                        type = 0;
                    }
                }
            }
            show = type;
        }
    }

    npc = fbatNpcTalkCheck();
    if (npc != 0) {
        if ((*(u32*)npc & 0x80000000) == 0) {
            type = *(s8*)((s32)npc + 0x317);
            show = 1;
        }
        if (*(void**)((s32)player + 0x268) != npc) {
            void* p = marioGetPtr();
            if ((*(u32*)((s32)p + 4) & 0x02000000) != 0) {
                *(u32*)((s32)p + 4) &= ~0x02000000;
                effDelete(*(void**)((s32)p + 0x264));
                *(void**)((s32)p + 0x264) = 0;
                *(void**)((s32)p + 0x268) = 0;
            }
        }
    }

    if (*(s16*)((s32)player + 0x50) != 0 || (*(u32*)player & 0x00100000) != 0 ||
        (*(u32*)((s32)player + 4) & 0x01000000) != 0 ||
        (*(u16*)((s32)player + 0x2E) != 0 && (u16)(*(u16*)((s32)player + 0x2E) - 1) > 1)) {
        show = 0;
    }

    if (show != 0) {
        if (type == 1) {
            player = marioGetPtr();
            if ((*(u32*)((s32)player + 4) & 0x02000000) == 0) {
                *(u32*)((s32)player + 4) |= 0x02000000;
                *(void**)((s32)player + 0x264) = effMarioBalloonEntry(type, npc);
                *(void**)((s32)player + 0x268) = npc;
            }
        } else {
            void* p = marioGetPtr();
            if ((*(u32*)((s32)p + 4) & 0x02000000) == 0) {
                *(u32*)((s32)p + 4) |= 0x02000000;
                *(void**)((s32)p + 0x264) = effMarioBalloonEntry(type, 0);
            }
            if (npc != 0) {
                *(void**)((s32)player + 0x268) = npc;
            }
        }
    } else {
        player = marioGetPtr();
        if ((*(u32*)((s32)player + 4) & 0x02000000) != 0) {
            *(u32*)((s32)player + 4) &= ~0x02000000;
            effDelete(*(void**)((s32)player + 0x264));
            *(void**)((s32)player + 0x264) = 0;
            *(void**)((s32)player + 0x268) = 0;
        }
    }

    return 0;
}

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
