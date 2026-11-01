// Bobbery (bomhei): partner init, follow and the bomb ability.

#include "version.h"
#include "driver/casedrv.h"
#include "dolphin/types.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_sbr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "party.h"
#include "pmario_sound.h"
#include "system.h"
#include "driver/animdrv.h"
#include "mario/mario_motion.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "party/party_hit.h"
#include "party/party_move.h"
#include "eff/eff_sandars.h"
#include "driver/npcdrv.h"
#include "evt/evt_cmd.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "evt/evt_cam.h"
#if VERSION <= VERSION_DEMO_US
#include "party/party_bomhei.h"
#endif

#if VERSION != VERSION_JP
// US has no local duplicate: this is the global bomhei_init
#define bomhei_init_800BFBB0 bomhei_init
#endif






/** Height bob of Bobbery over Mario's head per frame of Mario's carry walk (M_W_7), in quarter units. */
#if VERSION > VERSION_DEMO_US
s32 m_w_7_sync[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 8, 7, 5, 3, 1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 9, 8, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0};
#endif

/** Camera shake of 600 ms (camera 4), run when Bobbery's blast hits a bomb rock (isBombRock). */
EVT_BEGIN(longQuake_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(15.0f / 1024.0f), FLOAT(15.0f / 1024.0f), 600)
    RETURN
EVT_END()

/** Shorter 200 ms camera shake, run for every other Bobbery blast. */
static EVT_BEGIN(quake_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(15.0f / 1024.0f), FLOAT(15.0f / 1024.0f), 200)
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
s32 m_w_7_sync[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 8, 7, 5, 3, 1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 9, 8, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0};
#endif
static inline BOOL isBombRock(HitEntry* hit) {
    CaseEntry* c;
    if (hit == NULL) {
        return FALSE;
    }
    if (hitGetAttr(hit) & HIT_ATTR_MOBJ) {
        if (strcmp(mobjHitObjPtrToPtr(hit)->animName, "MOBJ_BombRock") == 0) {
            return TRUE;
        }
    } else {
        c = caseCheckHitObj2(hit);
        if (c != NULL && c->conditionId == CASE_BOBBERY) {
            return TRUE;
        }
    }
    return FALSE;
}


// Stops Bobbery's two looping sounds.
static inline void bomheiCleanSfx(PartyEntry* p) {
    if ((u32)p->work.bomhei.sfxId != 0xFFFFFFFFU) {
        psndSFXOff(p->work.bomhei.sfxId);
        p->work.bomhei.sfxId = -1;
    }
    if ((u32)p->work.bomhei.sfxId2 != 0xFFFFFFFFU) {
        psndSFXOff(p->work.bomhei.sfxId2);
        p->work.bomhei.sfxId2 = -1;
    }
}

/** Stops the sounds of a bomb use; the demos keep a third id at 0x17C. */
static inline void bomheiCleanUseSfx(PartyEntry* p) {
#if VERSION <= VERSION_DEMO_US
    if ((u32)p->work.bomhei.reserved_17C != 0xFFFFFFFFU) {
        psndSFXOff(p->work.bomhei.reserved_17C);
        p->work.bomhei.reserved_17C = -1;
    }
#endif
    bomheiCleanSfx(p);
}

static inline void bomheiCleanFlags(PartyEntry* p) {
    p->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    p->flags &= ~PARTY_FLAG_FORCE_FALL;
    p->flags &= ~PARTY_FLAG_IS_EVT_USE;
    p->flags &= ~PARTY_FLAG_IS_BEING_USED;
}

static inline void bomheiFree(PartyEntry* p) {
    if (p->useStruct != NULL) {
        __memFree(HEAP_DEFAULT, p->useStruct);
        p->useStruct = NULL;
    }
}

/**
 * Party table bye handler: plays the goodbye sequence when Bobbery leaves the party,
 * dropping Mario's carry flag and stopping the bomb sounds on the first frame,
 * then switches to run mode 14 when the sequence ends.
 */
void bomhei_bye(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyGoodbyeInit(party);
        party->playerPtr->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
        bomheiCleanUseSfx(party);
    }
    if (partyGoodbyeMain(party)) {
        partyChgRunMode(party, PARTY_RUN_DELETE);
    }
}

void bomhei_init_800BFBB0(PartyEntry* party) {
    party->memberKind = PARTY_MEMBER_BOBBERY;
    party->moveKind = 0;
    party->height = 28.0f;
    party->width = 28.0f;
    party->work.bomhei.state = 100;
    party->work.bomhei.reserved_17C = -1;
    party->work.bomhei.sfxId = -1;
    party->work.bomhei.sfxId2 = -1;
}

/**
 * Bobbery's state for scripts (evt_party.c): 0 when there is no partner or Bobbery is out
 * but not using his ability, 1 on the frame his bomb explodes (use motion 60),
 * 2 otherwise (bomb in progress, or another partner is out).
 */
u8 bomheiGetStatus(void) {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
    u8 status = 2;

    if (party == NULL || (party->currentMemberId == PARTY_MEMBER_BOBBERY && (party->flags & PARTY_FLAG_IS_BEING_USED) == 0)) {
        status = 0;
        return status;
    } else if (party->useMotionId == 60) {
        status = 1;
    }
    return status;
}

#if VERSION > VERSION_DEMO_US
/** Party table exit handler (demo): drops Mario's carry flag, stops the bomb sounds and frees the use work. */
void bomhei_exit(PartyEntry* party) {
    PartyEntry* p;
    party->playerPtr->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
    p = partyGetPtr(marioGetPartyId());
    if (p != NULL && p->currentMemberId == PARTY_MEMBER_BOBBERY && (p->flags & PARTY_FLAG_IS_BEING_USED)) {
        p->playerPtr->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
        if (p->runMode != PARTY_RUN_FORCE_WALK) {
            partyChgRunMode(p, PARTY_RUN_MOVE);
            p->moveMode = PARTY_MOVE_MODE_STAY;
        }
        bomheiCleanFlags(p);
        bomheiCleanSfx(p);
        bomheiFree(p);
    }
}
#endif

/**
 * Cancels a bomb in progress (demo): back to run mode 2 unless in run mode 9,
 * clears the use flags and frees the use work.
 * Called before plane, ship and bellows rides and on run-mode changes (party_move.c).
 * Unlike the release versions it leaves Mario's carry flag and the sounds alone.
 */
#if VERSION <= VERSION_DEMO_US
/** Party table exit handler (demo): drops Mario's carry flag, stops the bomb sounds and frees the use work. */
void bomhei_exit(PartyEntry* party) {
    party->playerPtr->flags &= ~(MARIO_FLAG_ACTIVE << 13);
    bomheiCleanUseSfx(party);
    bomheiFree(party);
}
/**
 * Cancels a bomb in progress (demo): back to run mode 2 unless in run mode 9,
 * clears the use flags and frees the use work.
 * Called before plane, ship and bellows rides and on run-mode changes (party_move.c).
 * Unlike the release versions it leaves Mario's carry flag and the sounds alone.
 */
#endif
void bomhei_use_cancel(void) {
    PartyEntry* p = partyGetPtr(marioGetPartyId());
    if (p != NULL && p->currentMemberId == PARTY_MEMBER_BOBBERY && (p->flags & PARTY_FLAG_IS_BEING_USED)) {
#if VERSION > VERSION_DEMO_US
        p->playerPtr->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
#endif
        if (p->runMode != PARTY_RUN_FORCE_WALK) {
            partyChgRunMode(p, PARTY_RUN_MOVE);
            p->moveMode = PARTY_MOVE_MODE_STAY;
        }
        bomheiCleanFlags(p);
#if VERSION > VERSION_DEMO_US
        bomheiCleanSfx(p);
#endif
        bomheiFree(p);
    }
}

/**
 * Cancels a bomb only while Mario is carrying Bobbery (demo),
 * putting Mario back in his standing pose and stopping the sounds.
 * Called when Mario takes damage (mot_damage.c).
 */
void bomhei_use_cancel2(void) {
    PartyEntry* p = partyGetPtr(marioGetPartyId());
    if (p != NULL && p->currentMemberId == PARTY_MEMBER_BOBBERY && (p->flags & PARTY_FLAG_IS_BEING_USED) && (p->playerPtr->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
        p->playerPtr->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
        marioChgPose("M_S_1");
        if (p->runMode != PARTY_RUN_FORCE_WALK) {
            partyChgRunMode(p, PARTY_RUN_MOVE);
            p->moveMode = PARTY_MOVE_MODE_STAY;
        }
        bomheiCleanFlags(p);
        bomheiCleanUseSfx(p);
        bomheiFree(p);
    }
}

#if VERSION > VERSION_DEMO_US
/**
 * Party table post-use handler (demo): drops Mario's carry flag with a standing pose,
 * stops the sounds and frees the use work.
 */
void bomhei_use_post(PartyEntry* party) {
    MarioWork* player = party->playerPtr;
    const char* name;
    if (party->useMotionId >= 70 && party->useMotionId < 100) {
        partyGetAppearPos(party, &party->position);
    }
    if (player->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) {
        player->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
        if (player->currMotionId == MARIO_MOTION_WALK) {
            name = "M_W_1";
        } else if (player->currMotionId == MARIO_MOTION_DASH) {
            name = "M_R_1";
        } else if (player->flags & MARIO_FLAG_IS_JUMPING) {
            name = "M_J_1B";
        } else {
            name = "M_S_1";
        }
        marioChgPose(name);
    }
    bomheiCleanFlags(party);
    if (party->runMode != PARTY_RUN_FORCE_WALK) {
        party->moveMode = PARTY_MOVE_MODE_STAY;
        party->trigFlags |= PARTY_TRIG_CHG_RUN_MODE;
    }
    party->playerPtr->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
    bomheiCleanSfx(party);
    bomheiFree(party);
}
#endif

#if VERSION <= VERSION_DEMO_US
/**
 * Party table post-use handler (demo): drops Mario's carry flag with a standing pose,
 * stops the sounds and frees the use work.
 */
void bomhei_use_post(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    if (mario->flags & (MARIO_FLAG_ACTIVE << 13)) {
        mario->flags &= ~(MARIO_FLAG_ACTIVE << 13);
        marioChgPose("M_S_1");
    }
    bomheiCleanUseSfx(party);
    bomheiFree(party);
}
#endif
void bomhei_use_main(PartyEntry* party);
void mario_controll(PartyEntry* party);
HitEntry* check_front(PartyEntry* party);

/**
 * Party table use handler (demo), run every frame of the bomb ability:
 * steps bomhei_use_main, keeps Bobbery on the ground while he walks or blows,
 * faces him at the skull's tongue on Keelhaul Key's muj_05 (A_dokuro_bero),
 * and drives Mario's carry poses unless Mario is in a damage motion (state 0x66).
 */
void bomhei_use(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    MapObjectEntry* mobj;

#if VERSION > VERSION_DEMO_US
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
#endif
        bomhei_use_main(party);
        if ((party->useMotionId >= 30 && party->useMotionId <= 32) ||
            (party->useMotionId >= 50 && party->useMotionId <= 51) || party->useMotionId == 60) {
            partyChkGnd(party);
            if (party->flags & PARTY_FLAG_IS_GROUNDED) {
                partyChgMot(party, PARTY_MOTION_STAY);
                party->velY = 0.0f;
            }
            if (strcmp(gp->currentMapName, "muj_05") == 0 && party->hitobjStandOn != NULL &&
                strcmp(hitGetName(party->hitobjStandOn), "A_dokuro_bero") == 0) {
                party->targetDir = angleABf(party->position.x, party->position.z, 638.0f, 12.5f);
                party->moveDir = party->targetDir;
            }
        }
#if VERSION <= VERSION_DEMO_US
        if (party->flags & PARTY_FLAG_IS_BEING_USED) {
            mario = party->playerPtr;
#endif
        if (party->work.bomhei.state != 0x66) {
            if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
                party->work.bomhei.state = 0x66;
#if VERSION <= VERSION_DEMO_US
                } else {
#else
                return;
            }
#endif
            mario_controll(party);
#if VERSION > VERSION_DEMO_US
        }
        if (party->useMotionId <= 10 && mario->hitobjStandOn != NULL && (hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_MOBJ)) {
            mobj = mobjHitObjPtrToPtr(mario->hitobjStandOn);
            if (strcmp(mobj->animName, "MOBJ_RedJumpStand") == 0 ||
                strcmp(mobj->animName, "MOBJ_RedJumpStand2") == 0 ||
                strcmp(mobj->animName, "MOBJ_BlueJumpStand") == 0 ||
                strcmp(mobj->animName, "MOBJ_BlueJumpStand2") == 0) {
                bomhei_use_cancel();
                return;
            }
        }
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE) && marioKeyOffChk()) {
            bomhei_use_cancel();
        } else if (mario->forceMoveTimer != 0) {
            bomhei_use_cancel();
        } else if (party->useMotionId <= 10 && (mario->dispFlags & MARIO_DISP_FLAG_SLIT)) {
            bomhei_use_cancel();
#endif
        }
    }
}
#if VERSION <= VERSION_DEMO_US
}
#endif

typedef struct BomheiWork {
    u32 flags;  // 0x0
    s32 timer;  // 0x4
} BomheiWork;


// Bobbery's work (PartyEntry.useStruct). A macro, not an inline function: as a function the object changes.
#define BOMHEI_WORK(p) ((BomheiWork*)(p)->useStruct)

/**
 * The demo checks only the first hit object present.
 * Ends the fuse early when the first of Bobbery's front,
 * head or floor hits present has attribute 0xC00 (use motion 60, explode)
 * or his floor has 0x200 (use motion 40, the splash).
 * 0x200 is the hazard water that sinks Mario (mario_hit.c, MARIO_MOTION_DAMAGE) and 0x800 the spikes
 * (MARIO_MOTION_DAMAGE_TOGE). Guess: 0x400, the other bit of 0xC00, is a further hazard.
 */
static inline BOOL bomheiChkHitAttr(PartyEntry* party) {
    HitEntry* hit;
    u32 attr;

#if VERSION <= VERSION_DEMO_US
    if (party->hitobjFront != NULL) {
        if (hitGetAttr(party->hitobjFront) & (HIT_ATTR_DAMAGE_LIQUID | HIT_ATTR_SPIKES)) {
#else
    hit = party->hitobjFront;
    if (hit != NULL && (hitGetAttr(hit) & 0xC00)) {
#endif
        party->useMotionId = 60;
        return TRUE;
    }
#if VERSION <= VERSION_DEMO_US
    } else if (party->hitobjHead != NULL) {
        if (hitGetAttr(party->hitobjHead) & (HIT_ATTR_DAMAGE_LIQUID | HIT_ATTR_SPIKES)) {
#else
    hit = party->hitobjHead;
    if (hit != NULL && (hitGetAttr(hit) & 0xC00)) {
#endif
        party->useMotionId = 60;
        return TRUE;
    }
#if VERSION <= VERSION_DEMO_US
    } else if (party->hitobjStandOn != NULL) {
        attr = hitGetAttr(party->hitobjStandOn);
#else
    hit = party->hitobjStandOn;
    if (hit != NULL) {
        attr = hitGetAttr(hit);
#endif
        if (attr & (HIT_ATTR_DAMAGE_LIQUID | HIT_ATTR_SPIKES)) {
            party->useMotionId = 60;
#if VERSION <= VERSION_DEMO_US
            } else if (attr & HIT_ATTR_WATER_HAZARD) {
#else
            return TRUE;
        }
        if (attr & HIT_ATTR_WATER_HAZARD) {
#endif
            party->useMotionId = 40;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Holds Bobbery above Mario's head: 36 units up (plus a bob from m_w_7_sync while Mario plays the carry walk
 * M_W_7), 6 behind along his facing, facing the same way as Mario.
 */
static inline void bomheiSetHoldPos(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 height = 36.0f;

    if (strcmp(mario->animName, "M_W_7") == 0) {
        height += m_w_7_sync[party->work.bomhei.syncFrame] / 4.0f;
    }
    party->position.x = mario->position.x;
    party->position.y = mario->position.y + height;
    party->position.z = mario->position.z;
    movePos(&party->position.x, &party->position.z, -6.0f, toMovedirSimple(mario->displayDirectionTarget));
    party->position.z += 2.0f;
    party->displayDirectionTarget = mario->displayDirectionTarget;
    party->displayDirection = mario->displayDirection;
}

/**
 * The bomb ability's motion steps (party->useMotionId).
 * 0-2: Bobbery hops onto Mario's head; 10: held there with a 3 s fuse while X is held;
 * 20-22: thrown 38 units ahead on release; 30-32: walks forward slowly (X again cuts the fuse to 0.5 s);
 * 40-42: lands in water with a splash; 50-51: A_3 last-moment pose for 40 frames,
 * then check_front picks the hit; 60: the blast (Sanders effect at the partner attack level, camera shake,
 * field enemy hit); 70-71: flies off the top of the screen; 80-82: 0.8 s later drops back near Mario; 100: done.
 */
#if VERSION <= VERSION_DEMO_US
static void bomhei_use_main(PartyEntry* party) {
#else
void bomhei_use_main(PartyEntry* party) {
#endif
    MarioWork* mario = party->playerPtr;
    f32 x, z;
    f32 tmp;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
#if VERSION > VERSION_DEMO_US
        party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
        party->flags &= ~(PARTY_FLAG_DISABLE_FRONT_WALL_CHK | PARTY_FLAG_FORCE_FALL);
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
#endif
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(BomheiWork));
        memset(party->useStruct, 0, sizeof(BomheiWork));
        party->useMotionId = 0;
    }

    switch (party->useMotionId) {
    case 0:
        party->useMotionId = 1;
        party->work.bomhei.state = 100;
        party->work.bomhei.timer = 0;
        party->moveFrames = 4;
        BOMHEI_WORK(party)->timer = 0;
        BOMHEI_WORK(party)->flags = 0;
        partyChgPose(party, "R_1");
    case 1:
        party->moveTarget = mario->position;
        movePos(&party->moveTarget.x, &party->moveTarget.z, -6.0f, toMovedirSimple(mario->displayDirectionTarget));
        party->position.x += (party->moveTarget.x - party->position.x) / party->moveFrames;
        party->position.y += (party->moveTarget.y - party->position.y) / party->moveFrames;
        party->position.z += (party->moveTarget.z - party->position.z) / party->moveFrames;
        if (--party->moveFrames > 0) {
            break;
        }
        party->moveFrames = 9;
        x = mario->position.x;
        z = mario->position.z;
        movePos(&x, &z, -6.0f, toMovedirSimple(mario->displayDirectionTarget));
        party->moveSpeed = distABf(party->position.x, party->position.z, x, z) / 18.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, x, z);
        party->moveDir = party->targetDir;
        party->velY = 7.5f;
        // FAKE: the acceleration is negated from a spare temporary; party->accelY = -0.8333333f changes the object
        tmp = 0.8333333f;
        party->accelY = -tmp;
        partyChgPose(party, "J_1A");
        party->useMotionId = 2;
        party->work.bomhei.state = 10;
        break;
    case 2:
        party->position.y += party->velY;
        party->velY += party->accelY;
        if (--party->moveFrames > 0) {
            break;
        }
        party->flags2 |= PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        partyChgPose(party, "L_1");
        BOMHEI_WORK(party)->timer = (s16)sysMsec2Frame(3000);
        party->useMotionId = 10;
        party->work.bomhei.state = 20;
        break;
    case 10:
        if (BOMHEI_WORK(party)->flags & 2) {
            partyChgPose(party, "L_2");
        }
        bomheiSetHoldPos(party);
        bomheiChkHitAttr(party);
#if VERSION <= VERSION_DEMO_US
        if ((party->flags & PARTY_FLAG_IS_EVT_USE) || !(mario->buttonHold & PAD_BUTTON_X)) {
#else
        if ((party->flags & PARTY_FLAG_IS_EVT_USE) ||
            (mario->hitobjStandOn != NULL && mario->airTimer == 0 && !(mario->buttonHold & PAD_BUTTON_X))) {
#endif
            party->useMotionId = 20;
        }
        break;
    }

    switch (party->useMotionId) {
    case 20:
#if VERSION <= VERSION_DEMO_US
        party->work.bomhei.reserved_17C = psndSFXOn_3D((const char*)0x939, &party->playerPtr->position);
#else
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PBM_THROW1), &party->playerPtr->position);
#endif
        party->work.bomhei.state = 30;
        party->useMotionId = 21;
        x = party->position.x;
        z = party->position.z;
        movePos(&x, &z, 38.0f, toMovedirSimple(mario->displayDirectionTarget));
        party->moveSpeed = distABf(party->position.x, party->position.z, x, z) / 20.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, x, z);
        party->moveDir = party->targetDir;
        party->velY = 3.5555556f;
        // FAKE: the acceleration is negated from a spare temporary; party->accelY = -0.35555556f changes the object
        tmp = 0.35555556f;
        party->accelY = -tmp;
        // FAKE: 10.5f is converted to the frame count (10) at run time through tmp; party->moveFrames = 10 changes the object
        tmp = 10.5f;
        party->moveFrames = (s16)tmp;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags |= PARTY_FLAG_IS_JUMPING;
        bomheiChkHitAttr(party);
        break;
    case 21:
        partyMoveNoHosei(party);
        if (--party->moveFrames > 0) {
            bomheiChkHitAttr(party);
            break;
        }
        party->velY = 0.0f;
        party->accelY = -0.33333334f;
        // FAKE: 10.5f is converted to the frame count (10) at run time through tmp; party->moveFrames = 10 changes the object
        tmp = 10.5f;
        party->moveFrames = (s16)tmp;
        party->flags &= ~PARTY_FLAG_IS_JUMPING;
        if (party->hitobjStandOn != NULL) {
            party->useMotionId = 30;
        } else {
            party->useMotionId = 22;
        }
        bomheiChkHitAttr(party);
        break;
    case 22:
        partyMoveNoHosei(party);
#if VERSION <= VERSION_DEMO_US
        if (party->hitobjStandOn != NULL) {
#else
        if (party->hitobjStandOn != NULL && !bomheiChkHitAttr(party)) {
#endif
            party->useMotionId = !(BOMHEI_WORK(party)->flags & 4) ? 30 : 50;
        }
#if VERSION <= VERSION_DEMO_US
        bomheiChkHitAttr(party);
#endif
        break;
    case 30:
        party->moveFrames = sysMsec2Frame(200);
        party->useMotionId = 31;
    case 31:
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE) && (gp->buttonNew[0] & PAD_BUTTON_X)) {
            BOMHEI_WORK(party)->timer = (s16)(sysMsec2Frame(500) + 1);
            party->useMotionId = 50;
        }
        partyChgPose(party, "S_1");
        if (--party->moveFrames > 0) {
            bomheiChkHitAttr(party);
            break;
        }
        party->useMotionId = 32;
    case 32:
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE) && (gp->buttonNew[0] & PAD_BUTTON_X)) {
            BOMHEI_WORK(party)->timer = (s16)(sysMsec2Frame(500) + 1);
        }
        partyChgPose(party, !(BOMHEI_WORK(party)->flags & 2) ? "W_1" : "W_2");
        party->moveSpeed = 0.6f;
        partyMove(party);
        if (BOMHEI_WORK(party)->timer <= 0) {
            party->useMotionId = 50;
        }
        bomheiChkHitAttr(party);
        break;
    }
#if VERSION <= VERSION_DEMO_US
    if (BOMHEI_WORK(party)->timer > 0) {
#else
    if (party->useMotionId < 50 && BOMHEI_WORK(party)->timer > 0) {
#endif
        BOMHEI_WORK(party)->timer--;
        if (BOMHEI_WORK(party)->timer == (s16)sysMsec2Frame(2500)) {
            BOMHEI_WORK(party)->flags |= 2;
#if VERSION <= VERSION_DEMO_US
            party->work.bomhei.sfxId = psndSFXOn_3D((const char*)0x93C, &party->playerPtr->position);
#else
            if ((u32)party->work.bomhei.sfxId2 == 0xFFFFFFFFU) {
                party->work.bomhei.sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PBM_FUSE1), &party->playerPtr->position);
            }
#endif
        } else if (BOMHEI_WORK(party)->timer == (s16)sysMsec2Frame(500)) {
            party->useMotionId = 50;
            if (mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) {
                party->work.bomhei.state = 40;
            } else {
                party->useMotionId = 50;
            }
        }
    }

    switch (party->useMotionId) {
    case 40:
        if ((u32)party->work.bomhei.sfxId != 0xFFFFFFFFU) {
            psndSFXOff(party->work.bomhei.sfxId);
            party->work.bomhei.sfxId = -1;
        }
        marioSetSplash(0, party->position);
        BOMHEI_WORK(party)->flags |= 8;
        BOMHEI_WORK(party)->timer = (s16)(sysMsec2Frame(500) + 30);
        BOMHEI_WORK(party)->flags |= 2;
        party->flags |= PARTY_FLAG_FORCE_FALL;
        party->useMotionId = 41;
        party->moveFrames = 6;
        break;
    case 41:
        party->position.y += party->velY;
        party->velY += party->accelY;
        if (--party->moveFrames <= 0) {
            party->useMotionId = 42;
            party->velY = 0.0f;
            party->accelY = 0.0f;
        }
    case 42:
        // FAKE: the speed step goes through a spare temporary; adding the expression directly changes the object
        tmp = 0.1f * (-1.0f - party->moveSpeed);
        party->moveSpeed += tmp;
        if (party->moveSpeed <= 0.0f) {
            party->moveSpeed = 0.0f;
        }
        break;
    }

    switch (party->useMotionId) {
    case 50:
        if ((u32)party->work.bomhei.sfxId != 0xFFFFFFFFU) {
            psndSFXOff(party->work.bomhei.sfxId);
            party->work.bomhei.sfxId = -1;
        }
#if VERSION <= VERSION_DEMO_US
        party->work.bomhei.sfxId2 = psndSFXOn_3D((const char*)0x93D, &party->playerPtr->position);
#else
        if ((u32)party->work.bomhei.sfxId2 == 0xFFFFFFFFU) {
            party->work.bomhei.sfxId2 = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PBM_FUSE2), &party->playerPtr->position);
        }
#endif
        partyChgPose(party, "A_3");
        party->useMotionId = 51;
        party->moveFrames = 40;
    case 51:
        if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
            if (party->flags & PARTY_FLAG_IS_GROUNDED) {
                party->moveSpeed = 0.0f;
            }
            partyMove(party);
        } else {
            bomheiSetHoldPos(party);
        }
        if (--party->moveFrames > 0) {
            bomheiChkHitAttr(party);
            break;
        }
        if ((u32)party->work.bomhei.sfxId2 != 0xFFFFFFFFU) {
            psndSFXOff(party->work.bomhei.sfxId2);
            party->work.bomhei.sfxId2 = -1;
        }
        BOMHEI_WORK(party)->flags &= ~2;
        party->hitobjAction = check_front(party);
        if (mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) {
            mario->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
        }
        party->flags &= ~PARTY_FLAG_IS_FALLING;
        BOMHEI_WORK(party)->flags &= ~8;
        party->useMotionId = 60;
        break;
    }

    switch (party->useMotionId) {
    case 60: {
        EffectEntry* eff;
        bomheiCleanSfx(party);
#if VERSION > VERSION_DEMO_US
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PBM_BOMB1), &party->position);
#endif
        BOMHEI_WORK(party)->timer = 0;
        party->moveSpeed = 0.0f;
        partyMove(party);
        if (isBombRock(party->hitobjAction)) {
            evtEntryType(longQuake_evt, 0, 0, 0);
        } else {
            evtEntryType(quake_evt, 0, 0, 0);
        }
#if VERSION <= VERSION_DEMO_US
        psndSFXOn_3D((const char*)0x93B, &party->position);
        eff = effSandarsEntry(pouchGetPartyAttackLv((PartyId)3), party->position.x,
#else
        eff = effSandarsEntry(pouchGetPartyAttackLv(PARTNER_BOBBERY), party->position.x,
#endif
                              party->position.y + party->height, party->position.z, 0.0f);
        ((EffSandarsWork*)eff->userdata)->camId = party->camId;
        fbatSandersBombTriggerOn();
        party->useMotionId = 70;
        break;
    }
    case 70:
        party->moveSpeed = 0.0f;
        party->velY = 8.0f;
        party->accelY = 0.0f;
        party->moveFrames = 60;
        party->useMotionId = 71;
    case 71: {
        f32 sx, sy, sz;
        party->position.y += party->velY;
        marioGetScreenPos(party->position, &sx, &sy, &sz);
        if (sy <= -300.0f) {
            party->moveFrames = sysMsec2Frame(800);
            party->useMotionId = 80;
        }
        break;
    }
    case 80:
        if (--party->moveFrames <= 0) {
            Vec pos;
#if VERSION <= VERSION_DEMO_US
            partyGetReturnPos(&pos);
#else
            partyGetAppearPos(party, &pos);
#endif
            party->position.x = pos.x;
            party->position.y = 200.0f + pos.y;
            party->position.z = pos.z;
            party->velY = 0.0f;
            party->accelY = -1.0f;
            party->moveSpeed = 0.0f;
            party->flags &= ~PARTY_FLAG_IS_JUMPING;
            party->flags |= PARTY_FLAG_IS_FALLING;
            party->flags |= PARTY_FLAG_FORCE_FALL;
            partyChgPose(party, "J_1B");
            party->useMotionId = 81;
        }
        break;
    case 81: {
        HitEntry* hit;
        f32 limit;
#if VERSION <= VERSION_DEMO_US
        f32 groundY, dropDepth;
#endif
        f32 sx, sy, sz;
        if (37.0f + mario->position.y > party->position.y) {
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
        }
#if VERSION <= VERSION_DEMO_US
        hit = _partySearchGround(party, &groundY, &dropDepth, 11.0f, party->velY);
#else
        hit = partySearchGround(party, 11.0f, party->velY);
#endif
        limit = mario->screenY - 25.0f;
        marioGetScreenPos(party->position, &sx, &sy, &sz);
        if (sy > limit) {
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
        } else if (hit != NULL) {
            party->hitobjStandOn = hit;
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
        }
        partyMove(party);
        if (party->velY <= -6.0f) {
            party->accelY = 0.0f;
            party->velY = -6.0f;
        }
        if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
            if (party->position.y < -2000.0f) {
                party->position = mario->position;
                party->position.y += 200.0f;
                party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
            }
            if (party->hitobjStandOn != NULL && !((hitGetAttr(party->hitobjStandOn) >> 9) & 1)) {
                partyChgPose(party, "F_1");
                party->useMotionId = 82;
                party->moveFrames = 30;
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PBM_DOWN1), &party->position);
            }
        }
        break;
    }
    case 82:
        if (--party->moveFrames <= 0) {
            party->useMotionId = 100;
        }
        partyMove(party);
        break;
    case 100:
        partyChgRunMode(party, PARTY_RUN_MOVE);
        party->moveMode = PARTY_MOVE_MODE_STAY;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags &= ~PARTY_FLAG_IS_EVT_USE;
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        bomheiFree(party);
        break;
    }
}

/**
 * Returns 0 while Bobbery's bomb is at a stage where marioCheckMenuDisable must keep the menu shut (being
 * lifted, carried, thrown, the blast, flying off); 1 otherwise or when Bobbery is not in use.
 */
s32 mario_bomhei_keychk(void) {
    PartyEntry* p;
    u8 m;

    marioGetPtr();
    p = partyGetPtr(marioGetPartyId());
    if (p == NULL) {
        return 1;
    }
    if (p->currentMemberId != PARTY_MEMBER_BOBBERY) {
        return 1;
    }
    if ((p->flags & PARTY_FLAG_IS_BEING_USED) == 0) {
        return 1;
    }
    m = p->work.bomhei.state;
    if (m == 10 || (m >= 20 && m <= 21) || m == 101 || (m >= 40 && m <= 41) || (m >= 30 && m <= 36)) {
        return 0;
    }
    m = p->useMotionId;
    if ((m >= 50 && m <= 51) || m == 60 || (m >= 70 && m <= 71)) {
        return 0;
    }
    return 1;
}

/**
 * Returns 0 while Bobbery is being lifted or thrown, so marioChkKey ignores Mario's buttons;
 * 1 otherwise or when Bobbery is not in use.
 */
s32 mario_bomhei_keychk2(void) {
    PartyEntry* p;
    u8 m;

    marioGetPtr();
    p = partyGetPtr(marioGetPartyId());
    if (p == NULL) {
        return 1;
    }
    if (p->currentMemberId != PARTY_MEMBER_BOBBERY) {
        return 1;
    }
    if ((p->flags & PARTY_FLAG_IS_BEING_USED) == 0) {
        return 1;
    }
    m = p->work.bomhei.state;
    if (m == 10 || m == 101 || (m >= 30 && m <= 36)) {
        return 0;
    }
    return 1;
}

/**
 * Drives Mario's side of the bomb (party->work.bomhei.state):
 * 10 lift pose, 20-21 carrying (syncs the M_W_7 walk frame with the bob table),
 * 40-41 a 40-frame stand when the fuse runs out in his hands,
 * 30-36 the throw pose M_C_1 and its 50+10 frame recovery; 100 is idle.
 */
void mario_controll(PartyEntry* party) {
    MarioWork* player = party->playerPtr;
    s32 cnt;

    switch (party->work.bomhei.state) {
    case 10:
        marioAdjustMoveDir();
        marioChgPose("M_U_2");
        player->flags |= MARIO_FLAG_IS_CARRYING_BOBBERY;
        party->work.bomhei.state = 101;
        break;
    case 20:
        player->flags |= MARIO_FLAG_IS_CARRYING_BOBBERY;
        party->work.bomhei.state = 21;
        break;
    case 21:
        if (strcmp(player->animName, "M_W_7") == 0) {
            animPoseSetLocalTime(player->paperAnimGroupId[marioAnimeId()], party->work.bomhei.syncFrame);
            if (++party->work.bomhei.syncFrame >= 40) {
                party->work.bomhei.syncFrame = 0;
            }
        } else {
            party->work.bomhei.syncFrame = 0;
        }
        break;
    case 40:
        marioChgMot(MARIO_MOTION_STAY);
        party->work.bomhei.state = 41;
        party->work.bomhei.timer = 40;
        break;
    case 41:
        if (--party->work.bomhei.timer <= 0) {
            marioChgMot(MARIO_MOTION_STAY);
            party->work.bomhei.state = 100;
        }
        break;
    case 30:
        marioChgMot(MARIO_MOTION_STAY);
        party->work.bomhei.state = 31;
        break;
    case 31:
        marioChgPose("M_C_1");
        player->flags &= ~MARIO_FLAG_IS_CARRYING_BOBBERY;
        party->work.bomhei.timer = 50;
        party->work.bomhei.state = 32;
        break;
    case 32:
        if (--party->work.bomhei.timer <= 0) {
            party->work.bomhei.state = 36;
            party->work.bomhei.timer = 10;
        }
        break;
    case 36:
        animPoseSetLocalTime(player->paperAnimGroupId[marioAnimeId()], party->work.bomhei.timer);
        if (--party->work.bomhei.timer < 0) {
            marioChgMot(MARIO_MOTION_STAY);
            party->work.bomhei.state = 100;
        }
        break;
    }
}

/**
 * The demo probes straight down from the moved point and keeps the point itself.
 * Whether there is floor without attribute 0xE00 just below the point dist units from pos along angle;
 * if so moves pos there (the demo keeps the probed height, it does not snap to the floor).
 */
static inline BOOL bomheiChkReturnGround(Vec* pos, f32 dist, f32 angle) {
#if VERSION <= VERSION_DEMO_US
    f32 x, y, z;
    f32 hx, hy, hz, d, nx, ny, nz;
#else
    HitCheckQuery query;
#endif
    HitEntry* hit;
#if VERSION > VERSION_DEMO_US
    f32 x, z;
    f32 rf;
#endif

    marioGetPtr();
    x = pos->x;
#if VERSION <= VERSION_DEMO_US
    y = pos->y;
#endif
    z = pos->z;
    movePos(&x, &z, dist, angle);
#if VERSION <= VERSION_DEMO_US
    d = 4.0f;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &d, &nx, &ny, &nz, x, 2.0f + y, z, 0.0f, -1.0f, 0.0f);
#else
    query.targetDistance = 4.0f;
    {
        Vec start = {0.0f, 0.0f, 0.0f};
        Vec dir = {0.0f, -1.0f, 0.0f};
        start.x = x;
        start.y = 2.0f + pos->y;
        start.z = z;
        query.targetPos = start;
        query.targetDir = dir;
    }
    hit = hitCheckVecFilter(&query, NULL);
#endif
    if (hit != NULL && !(hitGetAttr(hit) & (HIT_ATTR_DEEP_WATER | HIT_ATTR_DAMAGE_LIQUID | HIT_ATTR_SPIKES))) {
#if VERSION <= VERSION_DEMO_US
        pos->x = x;
        pos->y = y;
        pos->z = z;
#else
        if (query.hitPos.x >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        pos->x = (s32)((1000.0f * query.hitPos.x) + rf) / 1000.0f;
        if (query.hitPos.y >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        pos->y = (s32)((1000.0f * query.hitPos.y) + rf) / 1000.0f;
        if (query.hitPos.z >= 0.0f) {
            rf = 0.5f;
        } else {
            rf = -0.5f;
        }
        pos->z = (s32)((1000.0f * query.hitPos.z) + rf) / 1000.0f;
#endif
        return TRUE;
    }
    return FALSE;
}

/**
 * Where a partner reappears next to Mario after leaving the screen (Bobbery after his blast in the demo,
 * Koops and the party_motion fall-ins): 6 units along Mario's yaw,
 * else behind or in front of him at his collision radius,
 * whichever has no wall and floor that passes bomheiChkReturnGround; else behind him at that radius.
 */
void partyGetReturnPos(Vec* out) {
    MarioWork* mario = marioGetPtr();
    f32 x, z, s, c;
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 y;
    f32 angle;
    Vec p;

    x = mario->position.x;
    y = 2.0f + mario->position.y;
    z = mario->position.z;
    dist = mario->wPlayerCollisionBox.x;
    angle = mario->targetYaw;
    sincosf(&s, &c, angle);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c) == NULL) {
        p = mario->position;
        if (bomheiChkReturnGround(&p, 6.0f, angle)) {
            out->x = p.x;
            out->y = p.y;
            out->z = p.z;
            return;
        }
    }

    x = mario->position.x;
    y = 2.0f + mario->position.y;
    z = mario->position.z;
    dist = mario->wPlayerCollisionBox.x;
    angle = revise360(toMovedirSimple(mario->displayDirectionTarget) - 180.0f);
    sincosf(&s, &c, angle);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c) == NULL) {
        p = mario->position;
        if (bomheiChkReturnGround(&p, mario->wPlayerCollisionBox.x, angle)) {
            out->x = p.x;
            out->y = p.y;
            out->z = p.z;
            return;
        }
    }

    angle = toMovedirSimple(mario->displayDirectionTarget);
    sincosf(&s, &c, angle);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c) == NULL) {
        p = mario->position;
        if (bomheiChkReturnGround(&p, mario->wPlayerCollisionBox.x, angle)) {
            out->x = p.x;
            out->y = p.y;
            out->z = p.z;
            return;
        }
    }

    x = mario->position.x;
    z = mario->position.z;
    movePos(&x, &z, mario->wPlayerCollisionBox.x, revise360(180.0f + mario->targetYaw));
    out->x = x;
    out->y = mario->position.y;
    out->z = z;
}

/**
 * Picks the hit object Bobbery's blast acts on (party->hitobjAction, read by the case triggers):
 * a bomb rock within 30 units (50 when the direction is sideways to the camera) in eight directions around his
 * facing, else the floor below if it is a bomb rock or any floor, else his floor hit.
 */
HitEntry* check_front(PartyEntry* party) {
    const s32 angles[8] = {0, 45, -45, 90, -90, 135, -135, 180};
    static const Vec down = {0.0f, -1.0f, 0.0f};
    HitCheckQuery q;
    f32 height;
    HitEntry* hit;
    HitEntry* found = NULL;
    HitEntry* last = NULL;
    f32 movedir;
    f32 ang;
    f32 lim;
    u32 i;

    height = 0.8f * party->height;
    movedir = partyToMovedir(party, party->displayDirection);
    for (i = 0; i < 8; i++) {
        sincosf(&q.targetDir.x, &q.targetDir.z, movedir + (f32)angles[i]);
        q.targetDir.y = 0.0f;
        q.targetPos = party->position;
        q.targetPos.y = q.targetPos.y + height;
        q.targetDistance = 100.0f;
        hit = hitCheckVecFilter(&q, NULL);
        if (hit != NULL) {
            ang = revise360(movedir + (f32)angles[i] - camGetPtr(CAMERA_3D)->viewYaw);
            if ((ang >= 60.0f && ang <= 120.0f) || (ang >= 240.0f && ang <= 300.0f)) {
                lim = 50.0f;
            } else {
                lim = 30.0f;
            }
            if (q.targetDistance <= lim && isBombRock(hit)) {
                return hit;
            }
            found = NULL;
        }
    }

    q.targetPos = party->position;
    q.targetDir = down;
    q.targetDistance = 2.0f + height;
    hit = hitCheckVecFilter(&q, NULL);
    if (hit != NULL) {
        if (isBombRock(hit)) {
            return hit;
        }
        last = hit;
    }

    hit = party->hitobjStandOn;
    if (hit != NULL && isBombRock(hit)) {
        return hit;
    }
    if (found != NULL) {
        return found;
    }
    if (last != NULL) {
        return last;
    }
    return hit;
}
