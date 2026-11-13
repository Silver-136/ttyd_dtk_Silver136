// The speech/action balloon over Mario's head ("!" and "?"): which NPC, item box or object it points at.

#include "mario/mario.h"
#include "driver/casedrv.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "eff/eff_mario_balloon.h"
#include <string.h>
#include "mario/mario_hit.h"

/** Clears the balloon effect pointer; run from marioInit and marioReInit (the "Baloon" spelling is the original's). */
void marioBaloonInit(void) {
    marioGetPtr()->balloonEffect = NULL;
}

/**
 * Spawns the balloon effect of the given type over Mario's head unless one is already showing.
 * Called each frame by marioBalloonMain and by the evt_mario balloon script commands (type 0 "!", type 1 the
 * "..." balloon).
 */
void marioBalloonOn(s32 type) {
    MarioWork* mario = marioGetPtr();

    if (!(mario->dispFlags & MARIO_DISP_FLAG_BALLOON)) {
        mario->dispFlags |= MARIO_DISP_FLAG_BALLOON;
        mario->balloonEffect = effMarioBalloonEntry(type, NULL);
    }
}

/**
 * Like marioBalloonOn, but anchors the balloon effect to the NPC Mario can talk to and remembers it in
 * mario->balloonNpc.
 */
// Original name from the JP demo's link map (UNUSED there): every call is inlined, so the linker strips it.
void marioBalloonOn_npc(s32 type, NpcEntry* npc) {
    MarioWork* mario = marioGetPtr();

    if (!(mario->dispFlags & MARIO_DISP_FLAG_BALLOON)) {
        mario->dispFlags |= MARIO_DISP_FLAG_BALLOON;
        mario->balloonEffect = effMarioBalloonEntry(type, npc);
        mario->balloonNpc = npc;
    }
}

/** Removes the balloon over Mario's head, if any, and forgets the NPC it pointed at. */
void marioBalloonOff(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->dispFlags & MARIO_DISP_FLAG_BALLOON) {
        mario->dispFlags &= ~MARIO_DISP_FLAG_BALLOON;
        effDelete(mario->balloonEffect);
        mario->balloonEffect = NULL;
        mario->balloonNpc = NULL;
    }
}

BOOL chkBalloonTypeTouch(HitEntry* hit);

BOOL chkBalloonTypeRide(HitEntry* hit);

BOOL chkBalloonTypeFront(HitEntry* hit);

s32 treasureBox(HitEntry* hit);

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

    // A switch with an empty case 0: a plain if compiles differently, so the original most likely had the switch too
    switch (!marioChkKey()) {
        case 0:
            break;
        default:
            marioBalloonOff();
            return;
    }

    if (chkBalloonTypeTouch(mario->hitobjFrontA)) {
        type = 0;
        show = TRUE;
    }
    if (chkBalloonTypeFront(mario->hitobjFront)) {
        type = 0;
        show = TRUE;
    }
    if (chkBalloonTypeRide(mario->hitobjStandOn)) {
        type = 0;
        show = TRUE;
    }
    if (mario->hitobjFront != NULL) {
        if (treasureBox(mario->hitobjFront) >= 0) {
            show = treasureBox(mario->hitobjFront);
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

    // (u16)(currMotionId - 1) > 1 tests 'not MARIO_MOTION_WALK and not MARIO_MOTION_DASH' (the subtract, clrlwi and cmplwi
    // are in the retail code)
    if (mario->airTimer != 0 || (mario->flags & MARIO_FLAG_PAPER_MODE) || (mario->dispFlags & MARIO_DISP_FLAG_SLIT) ||
        !(mario->currMotionId == MARIO_MOTION_STAY || mario->currMotionId == MARIO_MOTION_WALK ||
          mario->currMotionId == MARIO_MOTION_DASH)) {
        show = FALSE;
    }

    if (show) {
        if (type == 1) {
            marioBalloonOn_npc(type, npc);
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

/**
 * TRUE when the hit is something the A button acts on:
 * a map object with hint 1 or a balloon-flagged case event with any of the A conditions (front, jump from, push,
 * stand on). Pass-through hits never count. Used by searchFrontWall (mario_hit.c) and Bowser's collision code
 * (koopa_hit.c).
 */
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
                if (entry->flags & CASE_FLAG_SHOW_HINT) {
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

/**
 * TRUE when the surface in front of Mario (hitobjFrontA) is something A can act on:
 * a map object whose hint is 1 or a case-event hit flagged for the balloon with the "front + A" condition.
 * Hits with attribute 0x800000 never count.
 */
// Original name and size (0xa0) from the JP demo map (UNUSED there: every call is inlined).
BOOL chkBalloonTypeTouch(HitEntry* hit) {
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
            if (entry != NULL && (entry->flags & CASE_FLAG_SHOW_HINT) && entry->conditionId == CASE_FRONT_A) {
                ret = TRUE;
            }
        }
    }
    return ret;
}

/**
 * TRUE when the floor Mario stands on is a hinted map object or a balloon case event triggered by A
 * while pushing or standing on it. Attribute 0x800000 hits and pass-through hits (marioChkBero) never count.
 */
// Original name and size (0xc4) from the JP demo map (UNUSED there: every call is inlined).
BOOL chkBalloonTypeRide(HitEntry* hit) {
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
                    if (entry->flags & CASE_FLAG_SHOW_HINT) {
                        if (entry->conditionId == CASE_PUSH_A || entry->conditionId == CASE_STAND_ON_A) {
                            ret = TRUE;
                        }
                    }
                } else {
                    // Redundant (ret is already FALSE); the store and its branch are in the retail code
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

/**
 * TRUE when the hit directly in front of Mario is a hinted map object or a balloon case event triggered by A
 * while facing or pushing it. Hits Mario passes through (marioChkBero) never count.
 */
// Original name and size (0xb4) from the JP demo map (UNUSED there: every call is inlined).
BOOL chkBalloonTypeFront(HitEntry* hit) {
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
                if (entry->flags & CASE_FLAG_SHOW_HINT) {
                    if (entry->conditionId == CASE_FRONT_A || entry->conditionId == CASE_PUSH_A) {
                        ret = TRUE;
                    }
                }
            } else {
                // Redundant (ret is already FALSE); the store and its branch are in the retail code
                ret = FALSE;
            }
        }
        if (marioChkBero(hit)) {
            ret = FALSE;
        }
    }
    return ret;
}

/**
 * Checks whether the hit is a treasure chest (MOBJ_TreasureBox or MOBJ_BigTreasureBox):
 * -1 if it is not one, 1 if it is still closed, 0 once opened.
 * marioBalloonMain uses it so the "!" shows only over unopened chests.
 */
// Original name and size (0xa8) from the JP demo map (UNUSED there: every call is inlined).
s32 treasureBox(HitEntry* hit) {
    MapObjectEntry* mobj;
    s32 ret = -1;

    marioGetPtr();
    if (!(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
        // Redundant (ret is already -1); the store and its branch are in the retail code
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
