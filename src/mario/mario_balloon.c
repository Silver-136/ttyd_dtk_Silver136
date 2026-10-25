#include "mario/mario.h"
#include "driver/casedrv.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include <string.h>
#include "mario/mario_hit.h"

extern NpcEntry* fbatNpcTalkCheck(void);
extern EffectEntry* effMarioBalloonEntry(s32 type, NpcEntry* npc);

void marioBaloonInit(void) {
    marioGetPtr()->balloonEffect = NULL;
}

void marioBalloonOn(s32 type) {
    MarioWork* mario = marioGetPtr();

    if (!(mario->dispFlags & 0x02000000)) {
        mario->dispFlags |= 0x02000000;
        mario->balloonEffect = effMarioBalloonEntry(type, NULL);
    }
}

static inline void marioBalloonOnNpc(s32 type, NpcEntry* npc) {
    MarioWork* mario = marioGetPtr();

    if (!(mario->dispFlags & 0x02000000)) {
        mario->dispFlags |= 0x02000000;
        mario->balloonEffect = effMarioBalloonEntry(type, npc);
        mario->balloonNpc = npc;
    }
}

void marioBalloonOff(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->dispFlags & 0x02000000) {
        mario->dispFlags &= ~0x02000000;
        effDelete(mario->balloonEffect);
        mario->balloonEffect = NULL;
        mario->balloonNpc = NULL;
    }
}

static inline BOOL chkBalloonStand(HitEntry* hit) {
    BOOL ret = FALSE;
    CaseEntry* entry;
    s32 attr;

    if (hit != NULL) {
        attr = hitGetAttr(hit);
        if (attr & 0x800000) {
            ret = FALSE;
        } else if (attr & 0x80000000) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                ret = TRUE;
            }
        } else {
            entry = caseCheckHitObj(hit);
            if (entry != NULL && (entry->flags & 4) && entry->conditionId == 9) {
                ret = TRUE;
            }
        }
    }
    return ret;
}

static inline BOOL chkBalloonJump(HitEntry* hit) {
    BOOL ret = FALSE;
    CaseEntry* entry;

    if (hit != NULL) {
        if (hitGetAttr(hit) & 0x80000000) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                ret = TRUE;
            }
        } else {
            entry = caseCheckHitObj(hit);
            if (entry != NULL) {
                if (entry->flags & 4) {
                    if (entry->conditionId == 9 || entry->conditionId == 12) {
                        ret = TRUE;
                    }
                }
            } else {
                ret = FALSE;
            }
        }
        if (marioChkBero(hit)) {
            ret = FALSE;
        }
    }
    return ret;
}

static inline BOOL chkBalloonPush(HitEntry* hit) {
    BOOL ret = FALSE;
    CaseEntry* entry;
    s32 attr;

    if (hit != NULL) {
        attr = hitGetAttr(hit);
        if (attr & 0x800000) {
            ret = FALSE;
        } else {
            if (attr & 0x80000000) {
                if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                    ret = TRUE;
                }
            } else {
                entry = caseCheckHitObj(hit);
                if (entry != NULL) {
                    if (entry->flags & 4) {
                        if (entry->conditionId == 12 || entry->conditionId == 5) {
                            ret = TRUE;
                        }
                    }
                } else {
                    ret = FALSE;
                }
            }
            if (marioChkBero(hit)) {
                ret = FALSE;
            }
        }
    }
    return ret;
}

static inline s32 chkItemBox(HitEntry* hit) {
    MapObjectEntry* mobj;
    s32 ret = -1;

    marioGetPtr();
    if (!(hitGetAttr(hit) & 0x80000000)) {
        ret = -1;
    } else {
        mobj = mobjHitObjPtrToPtr(hit);
        if (strcmp(mobj->animName, "MOBJ_TreasureBox") == 0 ||
            strcmp(mobj->animName, "MOBJ_BigTreasureBox") == 0) {
            ret = 1;
            if (mobjCheckItemboxOpen(mobj) != 1) {
                ret = 0;
            }
        }
    }
    return ret;
}

void marioBalloonMain(void) {
    MarioWork* mario;
    NpcEntry* npc;
    s32 type;
    BOOL show;

    mario = marioGetPtr();
    if (mario->dispFlags & 0x04000000) {
        return;
    }
    show = FALSE;
    switch (!marioChkKey()) {
        case 0:
            break;
        default:
            marioBalloonOff();
            return;
    }

    if (chkBalloonStand((HitEntry*)mario->hitobjFrontA)) {
        type = 0;
        show = TRUE;
    }
    if (chkBalloonJump((HitEntry*)mario->hitobjFront)) {
        type = 0;
        show = TRUE;
    }
    if (chkBalloonPush(mario->hitobjStandOn)) {
        type = 0;
        show = TRUE;
    }
    if ((HitEntry*)mario->hitobjFront != NULL) {
        if (chkItemBox((HitEntry*)mario->hitobjFront) >= 0) {
            show = chkItemBox((HitEntry*)mario->hitobjFront);
        }
    }

    npc = fbatNpcTalkCheck();
    if (npc != NULL) {
        if (!(npc->flags & 0x80000000)) {
            type = npc->balloonType;
            show = TRUE;
        }
        if (mario->balloonNpc != npc) {
            marioBalloonOff();
        }
    }

    if (mario->airTimer != 0 || (mario->flags & MARIO_FLAG_PAPER_MODE) || (mario->dispFlags & 0x01000000) ||
        (mario->currMotionId != MARIO_MOTION_STAY && (u16)(mario->currMotionId - 1) > 1)) {
        show = FALSE;
    }

    if (show) {
        if (type == 1) {
            marioBalloonOnNpc(type, npc);
        } else {
            marioBalloonOn(type);
            if (npc != NULL) {
                mario->balloonNpc = npc;
            }
        }
    } else {
        marioBalloonOff();
    }
}

BOOL chkBalloon(HitEntry* hit) {
    BOOL ret = FALSE;
    CaseEntry* entry;

    if (hit != NULL) {
        if (hitGetAttr(hit) & 0x80000000) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                ret = TRUE;
            }
        } else {
            entry = caseCheckHitObj(hit);
            if (entry != NULL) {
                if (entry->flags & 4) {
                    if (entry->conditionId == 9 || entry->conditionId == 4 || entry->conditionId == 12 || entry->conditionId == 5) {
                        ret = TRUE;
                    }
                }
            } else {
                ret = FALSE;
            }
        }
        if (marioChkBero(hit)) {
            ret = FALSE;
        }
    }
    return ret;
}
