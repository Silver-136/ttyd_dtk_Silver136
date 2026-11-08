// Goombella (kuribo): partner init, follow and the tattle ability.

#include "version.h"
#include "party/party_kuribo.h"
#include "evt/evt_cmd.h"
#include "mario/mario_party.h"
#include "party.h"
#include "pmario_sound.h"
#include <string.h>
#include "driver/hitdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "driver/mobjdrv.h"
#include "party/party_move.h"
#include "evt/evt_msg.h"
#include "mario/mariost.h"
#include "party/party_hit.h"
#include "system.h"
#include "dolphin/mtx.h"

#if VERSION != VERSION_JP
// Only JP has a local duplicate; elsewhere this is the global kuribo_init
#define kuribo_init_800C23B4 kuribo_init
#endif

//.sbss
u32 msg_ep; // cleared by kuribo_init; nothing reads it

/**
 * TRUE while Goombella is the active partner and her Tattle ability is in use;
 * marioChkKey checks it to keep Mario's controls locked while she talks.
 */
BOOL christineGetStatus(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (!party || party->currentMemberId != PARTY_GOOMBELLA || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return FALSE;
    }
    return TRUE;
}

/**
 * First step of the field Tattle script: picks the message Goombella says into LW(0).
 * If the script was handed an NPC Mario can talk to,
 * LW(0) is moved to that NPC's description (its tattle message);
 * otherwise it looks at the map object Mario pushes, faces or stands on and picks the hint for that block,
 * switch, chest or sign, falling back to the "msg_kuri_map" comment on the map itself.
 * Mutes sound effects for the duration; JP and later also lift her off the floor collision,
 * and returns early when she is in paper form (the early demos instead require her on the ground).
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(pre_kurio_use) {
#else
USER_FUNC(pre_kurio_use) {
#endif

    void* hit;
    const char* msg;
    MarioWork* mario;
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (!party) {
        return EVT_RETURN_DONE;
    }
#if VERSION <= VERSION_DEMO_JP
    if (!(party->flags & PARTY_FLAG_IS_GROUNDED)) {
#else
    if (party->flags2 & PARTY_DISP_FLAG_PAPER) {
#endif
        return EVT_RETURN_DONE;
    }

    psndSetFlag(PSND_FLAG_MUTE_SFX);
#if VERSION >= VERSION_JP
    party->flags |= PARTY_FLAG_DISABLE_FLOORS;
    party->flags |= PARTY_FLAG_FORCE_FALL;
#endif
    mario = party->playerPtr;
    if (event->lwData[0]) {
        event->lwData[0] += 8;
        return EVT_RETURN_DONE;
    }
    {
        MapObjectEntry* mobj;
        hit = mario->hitobjPush;
        if (!hit || !(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
            hit = mario->hitobjFront;
            if (!hit || !(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
                hit = mario->hitobjStandOn;
                if (!hit || !(hitGetAttr(hit) & HIT_ATTR_MOBJ)) {
                    event->lwData[0] = (EvtWord)"msg_kuri_map";
                    return EVT_RETURN_DONE;
                }
            }
        }
        mobj = mobjHitObjPtrToPtr(hit);
        if (!strcmp(mobj->animName, "MOBJ_Lv1Block")) {
            if (pouchGetHammerLv() < 1) {
                msg = "obj_hlp_block_y_0";
            } else {
                msg = "obj_hlp_block_y_1";
            }
        } else if (!strcmp(mobj->animName, "MOBJ_Lv1BigBlock")) {
            if (pouchGetHammerLv() < 1) {
                msg = "obj_hlp_block_y_0";
            } else {
                msg = "obj_hlp_block_y_1";
            }
        } else if (!strcmp(mobj->animName, "MOBJ_Lv1BigBigBlock")) {
            if (pouchGetHammerLv() < 1) {
                msg = "obj_hlp_block_y_0";
            } else if (pouchGetHammerLv() < 2) {
                msg = "obj_hlp_block_y_2";
            } else {
                msg = "obj_hlp_block_y_3";
            }
        } else if (!strcmp(mobj->animName, "MOBJ_Lv2Block") || !strcmp(mobj->animName, "MOBJ_Lv2BigBlock")) {
            if (pouchGetHammerLv() < 1) {
                msg = "obj_hlp_block_s_0";
            } else if (pouchGetHammerLv() < 3) {
                msg = "obj_hlp_block_s_1";
            } else {
                msg = "obj_hlp_block_s_2";
            }
        } else if (!strcmp(mobj->animName, "MOBJ_Lv2BigBigBlock")) {
            if (pouchGetHammerLv() < 1) {
                msg = "obj_hlp_block_s_0";
            } else if (pouchGetHammerLv() < 3) {
                msg = "obj_hlp_block_s_1";
            } else {
                msg = "obj_hlp_block_s_3";
            }
        } else if (!strcmp(mobj->animName, "MOBJ_Lv3BigBigBlock") || !strcmp(mobj->animName, "MOBJ_Lv3BigBlock") || !strcmp(mobj->animName, "MOBJ_Lv3Block")) {
            msg = "obj_hlp_redyellow_block";
        } else if (!strcmp(mobj->animName, "MOBJ_Block") || !strcmp(mobj->animName, "MOBJ_PinkBlock")) {
            msg = "obj_hlp_su_block";
        } else if (!strcmp(mobj->animName, "MOBJ_PowerUpBlock")) {
            msg = "obj_hlp_shine";
        } else if (!strcmp(mobj->animName, "MOBJ_HatenaBlock")) {
            msg = "obj_hlp_hatena";
        } else if (!strcmp(mobj->animName, "MOBJ_BadgeBlock")) {
            msg = "obj_hlp_badge_hatena";
        } else if (!strcmp(mobj->animName, "MOBJ_green_FlowBlock") || !strcmp(mobj->animName, "MOBJ_green_big_FlowBlock") ||
                   !strcmp(mobj->animName, "MOBJ_green_big02_FlowBlock") || !strcmp(mobj->animName, "MOBJ_purple_FlowBlock") ||
                   !strcmp(mobj->animName, "MOBJ_purple_big_FlowBlock") || !strcmp(mobj->animName, "MOBJ_purple_big02_FlowBlock") ||
                   !strcmp(mobj->animName, "MOBJ_orange_FlowBlock") || !strcmp(mobj->animName, "MOBJ_orange_big_FlowBlock") ||
                   !strcmp(mobj->animName, "MOBJ_orange_big02_FlowBlock")) {
            msg = "obj_hlp_huyuu";
        } else if (!strcmp(mobj->animName, "MOBJ_green_FlowBlockSwitch") || !strcmp(mobj->animName, "MOBJ_purple_FlowBlockSwitch") ||
                   !strcmp(mobj->animName, "MOBJ_orange_FlowBlockSwitch")) {
            msg = "obj_hlp_huyuu_switch";
        } else if (!strcmp(mobj->animName, "MOBJ_BlueTornadoSwitch") || !strcmp(mobj->animName, "MOBJ_RedTornadoSwitch")) {
            msg = pouchGetJumpLv() < 2 ? "obj_hlp_big_switch_1" : "obj_hlp_big_switch_2";
        } else if (!strcmp(mobj->animName, "MOBJ_TreasureBox") || !strcmp(mobj->animName, "MOBJ_GrayTreasureBox") ||
                   !strcmp(mobj->animName, "MOBJ_BigTreasureBox")) {
            msg = mobjCheckItemboxOpen(mobj) == 2 ? "obj_hlp_takara_1" : "obj_hlp_takara_0";
        } else if (!strcmp(mobj->animName, "MOBJ_BlackTreasureBox")) {
            msg = mobjCheckItemboxOpen(mobj) == 2 ? "obj_hlp_blackbox_1" : "obj_hlp_blackbox_0";
        } else if (!strcmp(mobj->animName, "MOBJ_RedJumpStand") || !strcmp(mobj->animName, "MOBJ_RedJumpStand2") ||
                   !strcmp(mobj->animName, "MOBJ_BlueJumpStand") || !strcmp(mobj->animName, "MOBJ_BlueJumpStand2") ||
                   !strcmp(mobj->animName, "MOBJ_MapJumpStand")) {
            msg = "obj_hlp_jump";
        } else if (!strcmp(mobj->animName, "MOBJ_RedTimerSwitch") || !strcmp(mobj->animName, "MOBJ_BlueTimerSwitch")) {
            msg = "obj_hlp_timer_switch";
        } else if (!strcmp(mobj->animName, "MOBJ_RedSwitch") || !strcmp(mobj->animName, "MOBJ_BlueSwitch") ||
                   !strcmp(mobj->animName, "MOBJ_WhiteSwitch") || !strcmp(mobj->animName, "MOBJ_BlackSwitch")) {
            msg = "obj_hlp_switch";
        } else if (!strcmp(mobj->animName, "MOBJ_RideSwitchA") || !strcmp(mobj->animName, "MOBJ_RideSwitchB") ||
                   !strcmp(mobj->animName, "MOBJ_RideSwitchC")) {
            msg = "obj_hlp_yuka_switch";
        } else if (!strcmp(mobj->animName, "MOBJ_BreakingFloor") || !strcmp(mobj->animName, "MOBJ_BlackBreakingFloor")) {
            msg = "obj_hlp_hip_attack";
            if (pouchGetJumpLv() >= 2) {
                msg = "obj_hlp_hip_attack_1";
            }
        } else if (!strcmp(mobj->animName, "MOBJ_BombRock")) {
            msg = "obj_hlp_bomb_rock";
        } else if (!strcmp(mobj->animName, "MOBJ_Signboard")) {
            msg = "obj_hlp_kanban";
        } else if (!strcmp(mobj->animName, "MOBJ_Arrow")) {
            msg = "obj_hlp_yajirushi";
        } else if (!strcmp(mobj->animName, "MOBJ_Lock")) {
            msg = "obj_hlp_lock";
        } else if (!strcmp(mobj->animName, "MOBJ_KururinFloor")) {
            msg = "msg_kuri_map";
        } else {
            msg = "msg_kuri_map";
        }

        // msg is always set by the chain above; the test is in the retail code all the same
        if (msg) {
            event->lwData[0] = (EvtWord)msg;
            partyChgPoseId(party, PARTY_POSE_TALK);
        }
        return EVT_RETURN_DONE;
    }
}

/**
 * Last step of the field Tattle script: advances kuribo_use to its wind-down state,
 * lets Mario move again and unmutes sound effects (JP and later also restore her floor collision).
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(post_kurio_use) {
#else
USER_FUNC(post_kurio_use) {
#endif
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (!party) {
        return EVT_RETURN_DONE;
    }
#if VERSION <= VERSION_DEMO_JP
    party->useMotionId = 10;
#else
    party->useMotionId = 20;
#endif
    marioStSystemLevel(0);
    psndClearFlag(PSND_FLAG_MUTE_SFX);
#if VERSION >= VERSION_JP
    party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
    party->flags &= ~PARTY_FLAG_FORCE_FALL;
#endif
    return EVT_RETURN_DONE;
}

void kuribo_init_800C23B4(PartyEntry* party) {
    party->memberKind = PARTY_GOOMBELLA;
    party->moveKind = 0;
    party->height = 24.0f;
    party->width = 26.0f;
    msg_ep = 0;
}

/**
 * Field Tattle event: runs pre_kurio_use, prints the chosen message in Goombella's party speech balloon,
 * then post_kurio_use. Started by kuribo_use with LW(0) set to the NPC Mario faces, if any.
 */
EVT_BEGIN(kurio_msg_evt)
    CALL(pre_kurio_use)
    IF_INT_EQUAL(LW(0), 0)
        CALL(post_kurio_use)
        RETURN
    END_IF
    CALL(evt_msg_print, 0, LW(0), 0, STRING("party"))
    CALL(post_kurio_use)
    RETURN
EVT_END()

/**
 * Party table post_func, run when Goombella leaves use mode (partyUsePost or a run-mode change out of mode 3):
 * JP and later clear the floor-collision overrides kuribo_use may have set; empty in the early demos.
 */
#if VERSION >= VERSION_JP
void kuribo_use_post(PartyEntry* party) {
    party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
    party->flags &= ~PARTY_FLAG_FORCE_FALL;
}
#endif

/**
 * The early demos start the message right where Goombella stands; JP and later first walk her next to Mario
 * when she is more than 125 units away or a wall lies between them.
 */
#if VERSION >= VERSION_JP
void kuribo_use(PartyEntry* party) {
    MarioWork* player = party->playerPtr;
    HitEntry* wall;
    Vec pos;
    EventEntry* evt;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        if (PSVECDistance(&player->position, &party->position) > 125.0f) {
            party->useMotionId = 0;
        } else {
            pos = party->position;
            wall = partySearchFrontWall(party, &pos, 150.0f,
                angleABf(party->position.x, party->position.z, player->position.x, player->position.z));
            if (wall) {
                if (party->frontWallDist < distABf(party->position.x, party->position.z, player->position.x, player->position.z)) {
                    party->useMotionId = 0;
                } else {
                    wall = NULL;
                }
            }
            if (!wall) {
                partyChgPoseId(party, PARTY_POSE_STAND);
                party->moveSpeed = 0.0f;
                party->moveFrames = 100;
                party->useMotionId = 10;
            }
        }
    }

    switch (party->useMotionId) {
    case 0:
        if (!partyGetAppearPos3(party, &party->moveTarget)) {
            party->flags |= PARTY_FLAG_DISABLE_FLOORS;
            party->flags |= PARTY_FLAG_FORCE_FALL;
        }
        party->moveFrames = 4;
        party->moveDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        party->targetDir = party->moveDir;
        party->displayDirection = player->displayDirection;
        party->displayDirectionTarget = player->displayDirection;
        party->moveSpeed = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z) / party->moveFrames;
        party->velY = 0.0f;
        party->useMotionId = 1;
        break;
    case 1:
        if (!partyGetAppearPos3(party, &party->moveTarget)) {
            party->flags |= PARTY_FLAG_DISABLE_FLOORS;
            party->flags |= PARTY_FLAG_FORCE_FALL;
        }
        party->position.x += (party->moveTarget.x - party->position.x) / party->moveFrames;
        party->position.y += (party->moveTarget.y - party->position.y) / party->moveFrames;
        party->position.z += (party->moveTarget.z - party->position.z) / party->moveFrames;
        if (--party->moveFrames > 0) {
            return;
        }
        partyChgPoseId(party, PARTY_POSE_STAND);
        party->moveSpeed = 0.0f;
        party->moveFrames = 100;
        party->useMotionId = 10;
        break;
    case 10:
        evt = evtEntryType(kurio_msg_evt, 0, 0, 26);
        marioStSystemLevel(1);
        evt->lwData[0] = (EvtWord)fbatNpcTalkCheck();
        party->useMotionId = 15;
        break;
    case 20:
        party->moveFrames = 5;
        party->useMotionId = 21;

        // fall through
    case 21:
        if (--party->moveFrames > 0) {
            break;
        }
        partyChgRunMode(party, PARTY_RUN_MOVE);
        partyChgMoveMode(party, PARTY_MOVE_MODE_STAY);
        partyChgMot(party, PARTY_MOTION_STAY);
        partyChgPoseId(party, PARTY_POSE_STAND);
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
        party->flags &= ~PARTY_FLAG_FORCE_FALL;
        break;
    }
    partyChkGnd(party);
}
#endif

/**
 * Party table post_func, run when Goombella leaves use mode (partyUsePost or a run-mode change out of mode 3):
 * JP and later clear the floor-collision overrides kuribo_use may have set; empty in the early demos.
 */
#if VERSION <= VERSION_DEMO_JP
void kuribo_use_post(PartyEntry* party) {
}
#endif

/**
 * The early demos start the message right where Goombella stands; JP and later first walk her next to Mario
 * when she is more than 125 units away or a wall lies between them.
 */
#if VERSION <= VERSION_DEMO_JP
void kuribo_use(PartyEntry* party) {
    EventEntry* evt;

    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyChgMoveMode(party, 0);
        partyChgMot(party, PARTY_MOTION_STAY);
        partyChgPoseId(party, PARTY_POSE_STAND);
        party->moveFrames = 100;
        party->useMotionId = 0;
    }

    switch (party->useMotionId) {
        case 0:
            evt = evtEntryType(kurio_msg_evt, 0, 0, 26);
            marioStSystemLevel(1);
            evt->lwData[0] = (EvtWord)fbatNpcTalkCheck();
            party->useMotionId = 1;
            break;
        case 10:
            party->moveFrames = 5;
            party->useMotionId = 11;
        case 11:
            if (--party->moveFrames > 0) {
                break;
            }
            partyChgRunMode(party, PARTY_RUN_MOVE);
            partyChgMoveMode(party, 0);
            partyChgMot(party, PARTY_MOTION_STAY);
            partyChgPoseId(party, PARTY_POSE_STAND);
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            break;
    }
    partyChkGnd(party);
}
#endif

/**
 * Goodbye routine shared by Goombella, Flurrie, Vivian, Ms. Mowz and the walking NPC followers:
 * plays the standard leave-the-party motion and switches to run mode 14 once it ends.
 */
void kuribo_bye(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyGoodbyeInit(party);
    }
    if (partyGoodbyeMain(party)) {
        partyChgRunMode(party, PARTY_RUN_DELETE);
    }
}
