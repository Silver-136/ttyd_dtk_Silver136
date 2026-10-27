#include "mario/mario.h"
#include "driver/casedrv.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "eff/eff_mario_balloon.h"
#include <string.h>
#include "mario/mario_hit.h"

extern NpcEntry* fbatNpcTalkCheck(void);

void marioBaloonInit(void) {
    marioGetPtr()->balloonEffect = NULL;
}

void marioBalloonOn(s32 type) {
    MarioWork* mario = marioGetPtr();

    if (!(mario->dispFlags & MARIO_DISP_FLAG_BALLOON)) {
        mario->dispFlags |= MARIO_DISP_FLAG_BALLOON;
        mario->balloonEffect = effMarioBalloonEntry(type, NULL);
    }
}

static inline void marioBalloonOnNpc(s32 type, NpcEntry* npc) {
    MarioWork* mario = marioGetPtr();

    if (!(mario->dispFlags & MARIO_DISP_FLAG_BALLOON)) {
        mario->dispFlags |= MARIO_DISP_FLAG_BALLOON;
        mario->balloonEffect = effMarioBalloonEntry(type, npc);
        mario->balloonNpc = npc;
    }
}

void marioBalloonOff(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->dispFlags & MARIO_DISP_FLAG_BALLOON) {
        mario->dispFlags &= ~MARIO_DISP_FLAG_BALLOON;
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
        if (attr & HIT_ATTR_BERO) {
            ret = FALSE;
        } else if (attr & HIT_ATTR_MOBJ) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                ret = TRUE;
            }
        } else {
            entry = caseCheckHitObj(hit);
            if (entry != NULL && (entry->flags & 4) && entry->conditionId == CASE_FRONT_A) {
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
        if (hitGetAttr(hit) & HIT_ATTR_MOBJ) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                ret = TRUE;
            }
        } else {
            entry = caseCheckHitObj(hit);
            if (entry != NULL) {
                if (entry->flags & 4) {
                    if (entry->conditionId == CASE_FRONT_A || entry->conditionId == CASE_PUSH_A) {
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
        if (attr & HIT_ATTR_BERO) {
            ret = FALSE;
        } else {
            if (attr & HIT_ATTR_MOBJ) {
                if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                    ret = TRUE;
                }
            } else {
                entry = caseCheckHitObj(hit);
                if (entry != NULL) {
                    if (entry->flags & 4) {
                        if (entry->conditionId == CASE_PUSH_A || entry->conditionId == CASE_STAND_ON_A) {
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
    if (!(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
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
    if (mario->dispFlags & MARIO_DISP_FLAG_EVT_BALLOON) {
        return;
    }
    show = FALSE;
    // FAKE: switch on !marioChkKey() (empty case 0, default turns the balloon off); the plain if changes the object
    switch (!marioChkKey()) {
        case 0:
            break;
        default:
            marioBalloonOff();
            return;
    }

    if (chkBalloonStand(mario->hitobjFrontA)) {
        type = 0;
        show = TRUE;
    }
    if (chkBalloonJump(mario->hitobjFront)) {
        type = 0;
        show = TRUE;
    }
    if (chkBalloonPush(mario->hitobjStandOn)) {
        type = 0;
        show = TRUE;
    }
    if (mario->hitobjFront != NULL) {
        if (chkItemBox(mario->hitobjFront) >= 0) {
            show = chkItemBox(mario->hitobjFront);
        }
    }

    npc = fbatNpcTalkCheck();
    if (npc != NULL) {
        if (!(npc->flags & NPC_FLAG_NO_TALK_BALLOON)) {
            type = npc->balloonType;
            show = TRUE;
        }
        if (mario->balloonNpc != npc) {
            marioBalloonOff();
        }
    }

    // FAKE: (u16)(currMotionId - 1) > 1 tests 'not MARIO_MOTION_WALK and not MARIO_MOTION_DASH'; the two plain compares change the object
    if (mario->airTimer != 0 || (mario->flags & MARIO_FLAG_PAPER_MODE) || (mario->dispFlags & MARIO_DISP_FLAG_SLIT) ||
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
        if (hitGetAttr(hit) & HIT_ATTR_MOBJ) {
            if (mobjGetHint(mobjHitObjPtrToPtr(hit)) == 1) {
                ret = TRUE;
            }
        } else {
            entry = caseCheckHitObj(hit);
            if (entry != NULL) {
                if (entry->flags & 4) {
                    if (entry->conditionId == CASE_FRONT_A || entry->conditionId == CASE_JUMP_FROM_A ||
                        entry->conditionId == CASE_PUSH_A || entry->conditionId == CASE_STAND_ON_A) {
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
