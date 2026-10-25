#include "party/party_kuribo.h"
#include "evt/evt_cmd.h"
#include "mario/mario_party.h"
#include "party.h"
#include "pmario_sound.h"
#include <string.h>
#include "driver/hitdrv.h"
#include "mario/mario_pouch.h"
#include "driver/mobjdrv.h"
#include "party/party_move.h"

#ifdef VERSION_G8ME01
// US has no local duplicate: this is the global kuribo_init
#define kuribo_init_800C23B4 kuribo_init
#endif

// US MarioWork has 4 more bytes before these raw offsets
#ifdef VERSION_G8ME01
#define MW_US 4
#else
#define MW_US 0
#endif

s32 mobjCheckItemboxOpen(void* mobj);

//.sbss
u32 msg_ep; //unknown since it only gets written to once and never used

BOOL christineGetStatus(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (!party || party->currentMemberId != PARTY_MEMBER_GOOMBELLA || !(party->flags & 0x100)) {
        return FALSE;
    }
    return TRUE;
}

USER_FUNC(pre_kurio_use) {
    void* hit;
    char* msg;
    void* mario;
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (!party) {
        return EVT_RETURN_DONE;
    }

    if (party->flags2 & 0x4000000) {
        return EVT_RETURN_DONE;
    }

    psndSetFlag(PSND_FLAG_MUTE_SFX);
    party->flags |= 0x04000000;
    party->flags |= 0x02000000;

    mario = party->playerPtr;
    if (event->lwData[0]) {
        event->lwData[0] += 8;
        return EVT_RETURN_DONE;
    }
    {
        MapObjectEntry* mobj;
        hit = ((MarioWork*)mario)->hitobjPush;
        if (!hit || !(hitGetAttr(hit) & 0x80000000)) {
            hit = ((MarioWork*)mario)->hitobjFront;
            if (!hit || !(hitGetAttr(hit) & 0x80000000)) {
                hit = ((MarioWork*)mario)->hitobjStandOn;
                if (!hit || !(hitGetAttr(hit) & 0x80000000)) {
                    event->lwData[0] = (EvtWord)"msg_kuri_map";
                    return EVT_RETURN_DONE;
                }
            }
        }
        mobj = mobjHitObjPtrToPtr(hit);
#define name (mobj->animName)
        if (!strcmp(name, "MOBJ_Lv1Block")) {
            if (pouchGetHammerLv() < 1) msg = "obj_hlp_block_y_0"; else msg = "obj_hlp_block_y_1";
        } else if (!strcmp(name, "MOBJ_Lv1BigBlock")) {
            if (pouchGetHammerLv() < 1) msg = "obj_hlp_block_y_0"; else msg = "obj_hlp_block_y_1";
        } else if (!strcmp(name, "MOBJ_Lv1BigBigBlock")) {
            if (pouchGetHammerLv() < 1) msg = "obj_hlp_block_y_0";
            else if (pouchGetHammerLv() < 2) msg = "obj_hlp_block_y_2";
            else msg = "obj_hlp_block_y_3";
        } else if (!strcmp(name, "MOBJ_Lv2Block") || !strcmp(name, "MOBJ_Lv2BigBlock")) {
            if (pouchGetHammerLv() < 1) msg = "obj_hlp_block_s_0";
            else if (pouchGetHammerLv() < 3) msg = "obj_hlp_block_s_1";
            else msg = "obj_hlp_block_s_2";
        } else if (!strcmp(name, "MOBJ_Lv2BigBigBlock")) {
            if (pouchGetHammerLv() < 1) msg = "obj_hlp_block_s_0";
            else if (pouchGetHammerLv() < 3) msg = "obj_hlp_block_s_1";
            else msg = "obj_hlp_block_s_3";
        } else if (!strcmp(name, "MOBJ_Lv3BigBigBlock") || !strcmp(name, "MOBJ_Lv3BigBlock") || !strcmp(name, "MOBJ_Lv3Block")) {
            msg = "obj_hlp_redyellow_block";
        } else if (!strcmp(name, "MOBJ_Block") || !strcmp(name, "MOBJ_PinkBlock")) {
            msg = "obj_hlp_su_block";
        } else if (!strcmp(name, "MOBJ_PowerUpBlock")) {
            msg = "obj_hlp_shine";
        } else if (!strcmp(name, "MOBJ_HatenaBlock")) {
            msg = "obj_hlp_hatena";
        } else if (!strcmp(name, "MOBJ_BadgeBlock")) {
            msg = "obj_hlp_badge_hatena";
        } else if (!strcmp(name, "MOBJ_green_FlowBlock") || !strcmp(name, "MOBJ_green_big_FlowBlock") ||
                   !strcmp(name, "MOBJ_green_big02_FlowBlock") || !strcmp(name, "MOBJ_purple_FlowBlock") ||
                   !strcmp(name, "MOBJ_purple_big_FlowBlock") || !strcmp(name, "MOBJ_purple_big02_FlowBlock") ||
                   !strcmp(name, "MOBJ_orange_FlowBlock") || !strcmp(name, "MOBJ_orange_big_FlowBlock") ||
                   !strcmp(name, "MOBJ_orange_big02_FlowBlock")) {
            msg = "obj_hlp_huyuu";
        } else if (!strcmp(name, "MOBJ_green_FlowBlockSwitch") || !strcmp(name, "MOBJ_purple_FlowBlockSwitch") ||
                   !strcmp(name, "MOBJ_orange_FlowBlockSwitch")) {
            msg = "obj_hlp_huyuu_switch";
        } else if (!strcmp(name, "MOBJ_BlueTornadoSwitch") || !strcmp(name, "MOBJ_RedTornadoSwitch")) {
            msg = pouchGetJumpLv() < 2 ? "obj_hlp_big_switch_1" : "obj_hlp_big_switch_2";
        } else if (!strcmp(name, "MOBJ_TreasureBox") || !strcmp(name, "MOBJ_GrayTreasureBox") ||
                   !strcmp(name, "MOBJ_BigTreasureBox")) {
            msg = mobjCheckItemboxOpen((void*)mobj) == 2 ? "obj_hlp_takara_1" : "obj_hlp_takara_0";
        } else if (!strcmp(name, "MOBJ_BlackTreasureBox")) {
            msg = mobjCheckItemboxOpen((void*)mobj) == 2 ? "obj_hlp_blackbox_1" : "obj_hlp_blackbox_0";
        } else if (!strcmp(name, "MOBJ_RedJumpStand") || !strcmp(name, "MOBJ_RedJumpStand2") ||
                   !strcmp(name, "MOBJ_BlueJumpStand") || !strcmp(name, "MOBJ_BlueJumpStand2") ||
                   !strcmp(name, "MOBJ_MapJumpStand")) {
            msg = "obj_hlp_jump";
        } else if (!strcmp(name, "MOBJ_RedTimerSwitch") || !strcmp(name, "MOBJ_BlueTimerSwitch")) {
            msg = "obj_hlp_timer_switch";
        } else if (!strcmp(name, "MOBJ_RedSwitch") || !strcmp(name, "MOBJ_BlueSwitch") ||
                   !strcmp(name, "MOBJ_WhiteSwitch") || !strcmp(name, "MOBJ_BlackSwitch")) {
            msg = "obj_hlp_switch";
        } else if (!strcmp(name, "MOBJ_RideSwitchA") || !strcmp(name, "MOBJ_RideSwitchB") ||
                   !strcmp(name, "MOBJ_RideSwitchC")) {
            msg = "obj_hlp_yuka_switch";
        } else if (!strcmp(name, "MOBJ_BreakingFloor") || !strcmp(name, "MOBJ_BlackBreakingFloor")) {
            msg = "obj_hlp_hip_attack";
            if (pouchGetJumpLv() >= 2) msg = "obj_hlp_hip_attack_1";
        } else if (!strcmp(name, "MOBJ_BombRock")) {
            msg = "obj_hlp_bomb_rock";
        } else if (!strcmp(name, "MOBJ_Signboard")) {
            msg = "obj_hlp_kanban";
        } else if (!strcmp(name, "MOBJ_Arrow")) {
            msg = "obj_hlp_yajirushi";
        } else if (!strcmp(name, "MOBJ_Lock")) {
            msg = "obj_hlp_lock";
        } else if (!strcmp(name, "MOBJ_KururinFloor")) {
            msg = "msg_kuri_map";
        } else {
            msg = "msg_kuri_map";
        }
        if (msg) {
            event->lwData[0] = (EvtWord)msg;
            partyChgPoseId(party, 7);
        }
        return EVT_RETURN_DONE;
    }
#undef name
}

void marioStSystemLevel(s32 level);
void partyGoodbyeInit(PartyEntry* party);
BOOL partyGoodbyeMain(PartyEntry* party);

USER_FUNC(post_kurio_use) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (!party) {
        return EVT_RETURN_DONE;
    }
    party->useMotionId = 20;
    marioStSystemLevel(0);
    psndClearFlag(PSND_FLAG_MUTE_SFX);
    party->flags &= ~0x04000000;
    party->flags &= ~0x02000000;
    return EVT_RETURN_DONE;
}

void kuribo_init_800C23B4(PartyEntry* party) {
    party->memberKind = 1;
    party->moveKind = 0;
    party->height = 24.0f;
    party->width = 26.0f;
    msg_ep = 0;
}

USER_FUNC(evt_msg_print);
EVT_BEGIN(kurio_msg_evt)
    CALL(pre_kurio_use)
    IF_INT_EQUAL(4264967296, 0)
        CALL(post_kurio_use)
        RETURN
    END_IF
    CALL(evt_msg_print, 0, 4264967296, 0, STRING("party"))
    CALL(post_kurio_use)
    RETURN
EVT_END()
void* partySearchFrontWall(PartyEntry* party, Vec* pos, f32 dist, f32 angle);
BOOL partyGetAppearPos3(PartyEntry* party, Vec* pos);
s32 fbatNpcTalkCheck(void);
void partyChgMot(PartyEntry* party, s32 mot);
void partyChkGnd(PartyEntry* party);
f32 angleABf(f32, f32, f32, f32);
f32 distABf(f32, f32, f32, f32);
f32 PSVECDistance(const Vec*, const Vec*);


void kuribo_use_post(PartyEntry* party) {
    party->flags &= ~0x04000000;
    party->flags &= ~0x02000000;
}

void kuribo_use(PartyEntry* party) {
    MarioWork* player = party->playerPtr;
    void* wall;
    Vec pos;
    EventEntry* evt;

    if (party->trigFlags & 8) {
        party->trigFlags &= ~8;
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
                partyChgPoseId(party, 1);
                party->moveSpeed = 0.0f;
                party->moveFrames = 100;
                party->useMotionId = 10;
            }
        }
    }

    switch (party->useMotionId) {
    case 0:
        if (!partyGetAppearPos3(party, &party->moveTarget)) {
            party->flags |= 0x04000000;
            party->flags |= 0x02000000;
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
            party->flags |= 0x04000000;
            party->flags |= 0x02000000;
        }
        party->position.x += (party->moveTarget.x - party->position.x) / party->moveFrames;
        party->position.y += (party->moveTarget.y - party->position.y) / party->moveFrames;
        party->position.z += (party->moveTarget.z - party->position.z) / party->moveFrames;
        if (--party->moveFrames > 0) {
            return;
        }
        partyChgPoseId(party, 1);
        party->moveSpeed = 0.0f;
        party->moveFrames = 100;
        party->useMotionId = 10;
        break;
    case 10:
        evt = evtEntryType(kurio_msg_evt, 0, 0, 26);
        marioStSystemLevel(1);
        evt->lwData[0] = fbatNpcTalkCheck();
        party->useMotionId = 15;
        break;
    case 20:
        party->moveFrames = 5;
        party->useMotionId = 21;
    case 21:
        if (--party->moveFrames > 0) {
            break;
        }
        partyChgRunMode(party, 2);
        partyChgMoveMode(party, 0);
        partyChgMot(party, 0);
        partyChgPoseId(party, 1);
        party->flags &= ~0x100;
        party->flags &= ~0x04000000;
        party->flags &= ~0x02000000;
        break;
    }
    partyChkGnd(party);
}

void kuribo_bye(PartyEntry* party) {
    if (party->trigFlags & 8) {
        party->trigFlags &= ~8;
        partyGoodbyeInit(party);
    }
    if (partyGoodbyeMain(party)) {
        partyChgRunMode(party, 14);
    }
}

