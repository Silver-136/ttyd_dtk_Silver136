// Ms. Mowz (chuchu): partner init, follow and the treasure search ability.

#include "version.h"
#include "party/party_move.h"
#include "driver/itemdrv.h"
#include "driver/mobjdrv.h"
#include "evt/evt_cmd.h"
#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "mario/mario_party.h"
#include "mario/mario_sbr.h"
#include "mario/mariost.h"
#include "party.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "party/party_hit.h"
#include "party/party_move.h"
#include "manager/evtmgr_cmd.h"
#include "sfx_name_word.h"
#include "evt/evt_msg.h"
#include "eff/eff_fukidashi.h"

typedef struct ChuchuTreasure {
    const char* map;
    u32 flag;
    s32 x;
    s32 y;
    s32 z;
} ChuchuTreasure;

extern void partyWalkInit(PartyEntry* party, s32 mode);



/** Map objects Ms. Mowz can sniff out: blocks, and from JP on the treasure chests and the Kururin floor. */
char* mobj_list[] = {
#if VERSION > VERSION_DEMO_US
    "MOBJ_TreasureBox",
    "MOBJ_BigTreasureBox",
    "MOBJ_GrayTreasureBox",
    "MOBJ_BlackTreasureBox",
    "MOBJ_KururinFloor",
#endif
    "MOBJ_HatenaBlock",
    "MOBJ_BadgeBlock",
    "MOBJ_10CountBlock",
    "MOBJ_HiddenHatenaBlock",
    "MOBJ_HiddenBadgeBlock",
    "MOBJ_Hidden10CountBlock",
    "MOBJ_BrickHatenaBlock",
    "MOBJ_BrickBadgeBlock",
    "MOBJ_PowerUpBlock",
    NULL,
};

/** Zero vector that starts the position of a nearby item when Ms. Mowz searches for treasure. */
static const Vec vecA = {0.0f, 0.0f, 0.0f};

USER_FUNC(set_msg);
USER_FUNC(search_result);
USER_FUNC(post_msg);

/**
 * Ms. Mowz's search report, run by chuchu_use after her sniff:
 * set_msg finds the nearest hidden thing, search_result plays the reaction,
 * the message from the party group is printed, and post_msg lets chuchu_use end.
 */
EVT_BEGIN(msg_evt)
    CALL(set_msg)
    CALL(search_result)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, LW(0), 0, STRING("party"))
    END_IF
    CALL(post_msg)
    RETURN
EVT_END()

/**
 * Items hidden in the scenery that Ms. Mowz's sniffing can point at (in bushes, trees and the like): map,
 * the flag set once the item is taken, and the spot's position.
 */
#if VERSION > VERSION_DEMO_US
ChuchuTreasure chuchu_treasure_tbl[] = {
    {"hei_00", 0xF840715E, -468, 139, -127},
    {"hei_00", 0xF8406272, -408, 139, -127},
    {"hei_01", 0xF840628A, -200, 0, 120},
    {"hei_01", 0xF8406278, 190, 0, -70},
    {"hei_01", 0xF8406288, -285, 0, 85},
    {"hei_02", 0xF8407160, -645, 50, -70},
    {"hei_02", 0xF840627A, 192, 92, -120},
    {"hei_04", 0xF840627E, 364, 75, -95},
    {"hei_04", 0xF8406289, 320, 75, -55},
    {"hei_04", 0xF840627F, 17, 90, -122},
    {"hei_06", 0xF8406282, 200, 75, 60},
    {"nok_01", 0xF9718884, -407, 70, -148},
    {"win_00", 0xF84065FD, -694, 111, -130},
    {"win_01", 0xF84065FE, 312, 106, -122},
    {"win_03", 0xF8407169, -333, 72, -24},
    {"mri_01", 0xF840668C, -240, 0, -20},
    {"mri_04", 0xF8406696, 85, 0, 85},
    {"mri_05", 0xF840669C, 225, -309, -40},
    {"mri_06", 0xF840716E, -95, -310, -52},
    {"mri_12", 0xF84066AC, -190, -309, 65},
    {"mri_13", 0xF84066AE, -190, -309, 115},
    {"mri_15", 0xF8407170, -37, 0, 96},
    {"mri_18", 0xF84066B9, 50, -320, -35},
    {"mri_20", 0xF84066BB, 250, -320, 130},
    {"usu_00", 0xF840717C, 130, 0, 455},
    {"usu_00", 0xF840630D, 330, 0, 275},
    {"jin_01", 0xF9718882, 798, 50, -40},
    {"muj_02", 0xF84067D2, 323, 0, 50},
    {"muj_02", 0xF8407189, -500, 0, 110},
    {"muj_02", 0xF84067D0, -350, 90, -70},
    {"muj_03", 0xF84067D3, -250, 10, 95},
    {"muj_03", 0xF84067D4, 175, 210, -50},
    {"muj_05", 0xF84067D5, 235, 0, 90},
    {"rsh_02_a", 0xF8406906, -265, 0, -250},
    {"rsh_02_b", 0xF8406906, -265, 0, -250},
    {"rsh_02_c", 0xF8406906, -265, 0, -250},
    {"rsh_04_a", 0xF8407194, -265, 0, -250},
    {"rsh_04_b", 0xF8407194, -265, 0, -250},
    {"rsh_04_c", 0xF8407194, -265, 0, -250},
    {"tou_05", 0xF8407179, 100, 0, -28},
};
#endif

/** Zero vector that starts the position of a hidden scenery item during Ms. Mowz's treasure search. */
#if VERSION > VERSION_DEMO_US
static const Vec vecB = {0.0f, 0.0f, 0.0f};
#endif

/**
 * Script helper: reads set_msg's result (LW0 message, LW1 2 near / 1 far / 0 nothing) and,
 * if something was found, plays a 42-frame sniff (sound 0x945, pose PCH_Y_1) and
 * then a 1.5 s speech-bubble effect over her. In the retail versions a far find skips the sniff.
 */
#if VERSION <= VERSION_DEMO_US
static USER_FUNC(search_result) {
#else
USER_FUNC(search_result) {
#endif
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (isFirstCall) {
        if ((u32)event->lwData[0] == 0 || event->lwData[1] == 0) {
            return EVT_RETURN_DONE;
        }
#if VERSION <= VERSION_DEMO_US
psndSFXOn_3D((const char*)0x945, &party->position);
#else
        if (event->lwData[1] == 1) {
            if (party) {
                effFukidashiEntry(6, 0, (NpcEntry*)FUKIDASHI_PARTY, 0.0f, 18.0f, 0.0f, 0.0f, 0.0f, sysMsec2Frame(1500));
            }
            return EVT_RETURN_DONE;
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_CHURINA_SMELL_JUMP1), &party->position);
#endif
        event->userdata3 = 42;
        partyChgPose(party, "PCH_Y_1");
    }
    partyChkGnd(party);
    if ((s32)--event->userdata3 > 0) {
        return EVT_RETURN_BLOCK;
    }
    if (party) {
        effFukidashiEntry(6, 0, (NpcEntry*)FUKIDASHI_PARTY, 0.0f, 18.0f, 0.0f, 0.0f, 0.0f, sysMsec2Frame(1500));
    }
    return EVT_RETURN_DONE;
}

#if VERSION > VERSION_DEMO_US
static inline int chuchu_findTreasure2(PartyEntry* cpp, f32 range, Vec* bestPos) {
    u32 i;
    int found;
    MarioWork* mario;
    ChuchuTreasure* tbl;
    f32 minDist;

    found = 0;
    mario = cpp->playerPtr;
    minDist = 10000.0f;
    tbl = chuchu_treasure_tbl;
    for (i = 0; i < sizeof(chuchu_treasure_tbl) / sizeof(chuchu_treasure_tbl[0]); i++, tbl++) {
        if (strcmp(gp->currentMapName, tbl->map) == 0 && evtGetValue(NULL, tbl->flag) == 0) {
            Vec pos;
            Vec p = vecB;
            f32 d;
            p.x = tbl->x;
            p.y = tbl->y;
            p.z = tbl->z;
            pos = p;
            d = PSVECDistance(&mario->position, &pos);
            if (!(range < d) && minDist > d) {
                minDist = d;
                found = 1;
                bestPos->x = pos.x;
                bestPos->y = pos.y;
                bestPos->z = pos.z;
            }
        }
    }
    return found;
}
#endif

#if VERSION <= VERSION_DEMO_US
static USER_FUNC(set_msg) {
    int pass;
    PartyEntry* cp = partyGetPtr(marioGetPartyId());
    MarioWork* player = cp->playerPtr;
    PartyEntry* party = (PartyEntry*)cp;
    ItemEntry* item;
    MapObjectEntry* mobj;
    f32 range;

    event->lwData[0] = 0;
    event->lwData[1] = 0;
    if (!party) {
        return EVT_RETURN_DONE;
    }
    for (pass = 0; pass < 2; pass++) {
        if (pass == 0) {
            range = 150.0f;
        } else {
            range = 10000.0f;
        }
        item = itemNearDistCheck(player->position.x, player->position.y, player->position.z, range);
        if (item) {
            event->lwData[0] = (EvtWord)(pass == 0 ? "map_exist_near" : "map_exist_far");
            event->lwData[1] = (pass == 0) + 1;
            cp->displayDirectionTarget = (revise360(angleABf(cp->position.x, cp->position.z, item->position.x, item->position.z) - cp->camDir) <= 180.0f) ? 180.0f : 0.0f;
            cp->moveDir = partyToMovedir(party, cp->displayDirectionTarget);
            cp->targetDir = cp->moveDir;
            return EVT_RETURN_DONE;
        }
        mobj = mobjNearDistCheck2(player->position.x, player->position.y, player->position.z, range, mobj_list);
        if (mobj) {
            event->lwData[0] = (EvtWord)(pass == 0 ? "map_exist_near" : "map_exist_far");
            event->lwData[1] = (pass == 0) + 1;
            cp->displayDirectionTarget = (revise360(angleABf(cp->position.x, cp->position.z, mobj->position.x, mobj->position.z) - cp->camDir) <= 180.0f) ? 180.0f : 0.0f;
            cp->moveDir = partyToMovedir(party, cp->displayDirectionTarget);
            cp->targetDir = cp->moveDir;
            return EVT_RETURN_DONE;
        }
    }
    event->lwData[0] = (EvtWord) "map_no_exist";
    event->lwData[1] = 0;
    return EVT_RETURN_DONE;
}
#else
USER_FUNC(set_msg) {
    MapObjectEntry* mobj;
    ItemEntry* item;
    int found;
    MarioWork* player;
    int pass;
    int type;
    PartyEntry* cp = partyGetPtr(marioGetPartyId());
    f32 range;
    f32 minDist;
    f32 angle;
    Vec bestPos;
    Vec tmp;
    Vec ip;

    player = cp->playerPtr;
    event->lwData[0] = 0;
    event->lwData[1] = 0;
    if (!cp) {
        return EVT_RETURN_DONE;
    }
    for (pass = 0; pass < 2; pass++) {
        if (pass == 0) {
            range = 150.0f;
        } else {
            range = 10000.0f;
        }
        type = -1;
        item = itemNearDistCheck(player->position.x, player->position.y, player->position.z, range);
        mobj = mobjNearDistCheck2(player->position.x, player->position.y, player->position.z, range, mobj_list);
        found = chuchu_findTreasure2(cp, range, &bestPos);

        minDist = 10000.0f;
        if (item) {
            ip = vecA;
            ip.x = item->position.x;
            ip.y = item->position.y;
            ip.z = item->position.z;
            tmp = ip;
            if (minDist > PSVECDistance(&tmp, &player->position)) {
                minDist = PSVECDistance(&tmp, &player->position);
                angle = revise360(angleABf(cp->position.x, cp->position.z, item->position.x, item->position.z) - cp->camDir);
                type = 0;
            }
        }
        if (mobj) {
            if (minDist > PSVECDistance(&mobj->position, &player->position)) {
                minDist = PSVECDistance(&mobj->position, &player->position);
                angle = revise360(angleABf(cp->position.x, cp->position.z, mobj->position.x, mobj->position.z) - cp->camDir);
                type = 1;
            }
        }
        if (found) {
            if (minDist > PSVECDistance(&bestPos, &player->position)) {
                minDist = PSVECDistance(&bestPos, &player->position);
                angle = revise360(angleABf(cp->position.x, cp->position.z, bestPos.x, bestPos.z) - cp->camDir);
                type = 2;
            }
        }
        if (type >= 0) {
            break;
        }
    }

    if (type >= 0) {
        if (range == 150.0f) {
            event->lwData[0] = (EvtWord)"map_exist_near";
            event->lwData[1] = 2;
        } else {
            event->lwData[0] = (EvtWord)"map_exist_far";
            event->lwData[1] = 1;
        }
        cp->displayDirectionTarget = (angle <= 180.0f) ? 180.0f : 0.0f;
        cp->moveDir = partyToMovedir(cp, cp->displayDirectionTarget);
        cp->targetDir = cp->moveDir;
    } else {
        event->lwData[0] = (EvtWord)"map_no_exist";
        event->lwData[1] = 0;
    }
    return EVT_RETURN_DONE;
}
#endif

/** Script helper: clears LW0 and advances Ms. Mowz's use step (22 to 23) so chuchu_use ends the search. */
USER_FUNC(post_msg) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    event->lwData[0] = 0;
    if (party) {
        party->useMotionId++;
    }
    return EVT_RETURN_DONE;
}

/** Whether any unfound chuchu_treasure_tbl entry on the current map is within range of Mario. */
#if VERSION > VERSION_DEMO_US
static inline int chuchu_findTreasure(PartyEntry* cpp, f32 range) {
    u32 i;
    int found;
    MarioWork* player2;
    ChuchuTreasure* tbl;
    f32 minDist;

    found = 0;
    player2 = cpp->playerPtr;
    tbl = chuchu_treasure_tbl;
    minDist = 10000.0f;
    for (i = 0; i < sizeof(chuchu_treasure_tbl) / sizeof(chuchu_treasure_tbl[0]); i++, tbl++) {
        if (strcmp(gp->currentMapName, tbl->map) == 0 && evtGetValue(NULL, tbl->flag) == 0) {
            Vec pos;
            Vec p = vecB;
            f32 d;
            p.x = tbl->x;
            p.y = tbl->y;
            p.z = tbl->z;
            pos = p;
            d = PSVECDistance(&player2->position, &pos);
            if (!(range < d) && minDist > d) {
                minDist = d;
                found = 1;
            }
        }
    }
    return found;
}
#endif

/**
 * Whether there is something to sniff within 150 units of Mario (an item, an unopened item box,
 * a Kururin floor item or a block from mobj_list);
 * party_motion.c and party_move.c play her sniffing pose while it is TRUE.
 */
#if VERSION <= VERSION_DEMO_US
BOOL chuchu_searchObject(void) {
    char** name;
    PartyEntry* cp = partyGetPtr(marioGetPartyId());
    MapObjectEntry* mobj;
    MarioWork* mario = cp->playerPtr;

    if (itemNearDistCheck(mario->position.x, mario->position.y, mario->position.z, 150.0f)) {
        return TRUE;
    }
    mobj = mobjNearDistCheck(mario->position.x, mario->position.y, mario->position.z, 150.0f);
    if (mobj) {
        if (mobjCheckItemboxOpen(mobj) == 1) {
            return TRUE;
        }
        if (mobjCheckKururingFloorItem(mobj) == 2) {
            return TRUE;
        }
        for (name = mobj_list; *name != NULL; name++) {
            if (strcmp(mobj->animName, *name) == 0) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
#else
BOOL chuchu_searchObject(void) {
    PartyEntry* cp = partyGetPtr(marioGetPartyId());
    MarioWork* player = cp->playerPtr;

    if (itemNearDistCheck(player->position.x, player->position.y, player->position.z, 150.0f)) {
        return TRUE;
    }
    if (mobjNearDistCheck2(player->position.x, player->position.y, player->position.z, 150.0f, mobj_list)) {
        return TRUE;
    }
    return chuchu_findTreasure(cp, 150.0f) != 0;
}
#endif

/** Party table init handler: Ms. Mowz is member kind 7, 20 units tall and 24 wide. */
void chuchu_init(PartyEntry* party) {
    party->memberKind = PARTY_MEMBER_MS_MOWZ;
    party->moveKind = 0;
    party->height = 20.0f;
    party->width = 24.0f;
}

/** Called from partyReInit when Ms. Mowz is the partner; does nothing. */
void chuchu_reinit(PartyEntry* party) {
}

/** Party table exit handler; does nothing. */
void chuchu_exit(PartyEntry* party) {
}

/**
 * Party table move handler: on a run-mode change clears the in-use flag and starts walking;
 * then follows Mario (the retail versions also update her walk speed first).
 */
void chuchu_move(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
#if VERSION <= VERSION_DEMO_US
        // FAKE: the call passes a second argument partyWalkInit does not take (no effect); the cast keeps it.
        ((void (*)(PartyEntry*, s32))partyWalkInit)(party, 1);
#else
        partyWalkInit(party, 1);
#endif
    }
#if VERSION > VERSION_DEMO_US
    partyUpdMoveSpd(party);
#endif
    partyWalkMain(party);
}

/**
 * Party table use handler for her search ability:
 * freezes the game at system level 1, glides beside Mario over 4 frames, faces his way, waits 25 frames,
 * sniffs (sound 0x944, pose PCH_A2_1) for 30, runs msg_evt,
 * and once post_msg advances the step ends the use and unfreezes.
 */
void chuchu_use(PartyEntry* party) {
    PartyEntry* cp = party;
    MarioWork* player = cp->playerPtr;
    Vec pos;
    Vec pos1;

    if (cp->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        cp->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        cp->useMotionId = 0;
        marioStSystemLevel(1);
    }

    switch (cp->useMotionId) {
    case 0:
        partyGetAppearPos3(party, &pos);
        cp->moveTarget = pos;
        cp->moveFrames = 4;
        cp->moveDir = angleABf(cp->position.x, cp->position.z, cp->moveTarget.x, cp->moveTarget.z);
        cp->targetDir = cp->moveDir;
        cp->displayDirection = player->displayDirection;
        cp->displayDirectionTarget = player->displayDirection;
        cp->moveSpeed = distABf(cp->position.x, cp->position.z, cp->moveTarget.x, cp->moveTarget.z) / cp->moveFrames;
        cp->velY = 0.0f;
        cp->useMotionId = 1;
        break;
    case 1:
        partyGetAppearPos3(party, &pos1);
        cp->moveTarget = pos1;
        cp->position.x += (cp->moveTarget.x - cp->position.x) / cp->moveFrames;
        cp->position.y += (cp->moveTarget.y - cp->position.y) / cp->moveFrames;
        cp->position.z += (cp->moveTarget.z - cp->position.z) / cp->moveFrames;
        if (--cp->moveFrames > 0) {
            return;
        }
        partyChgPoseId(party, 1);
        cp->moveSpeed = 0.0f;
        cp->useMotionId = 10;
        break;
    case 10:
        cp->displayDirectionTarget = player->displayDirectionTarget;
        cp->moveDir = toMovedir(cp->displayDirectionTarget);
        cp->targetDir = cp->moveDir;
        cp->useMotionId = 20;
        cp->moveFrames = 25;
        break;
    case 20:
        if (--cp->moveFrames > 0) {
            break;
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_CHURINA_SMELL1), &cp->position);
        partyChgPose(party, "PCH_A2_1");
        cp->useMotionId = 21;
        cp->moveFrames = 30;
    case 21:
        if (--cp->moveFrames > 0) {
            break;
        }
        partyChgPoseId(party, 1);
        evtEntryType(msg_evt, 0, 0, 26);
        cp->useMotionId = 22;
        break;
    case 23:
        partyChgPoseId(party, 1);
        cp->useMotionId = 80;
        break;
    }

    switch (cp->useMotionId) {
    case 80:
        cp->flags &= ~PARTY_FLAG_IS_BEING_USED;
        partyChgRunMode(party, PARTY_RUN_MOVE);
        cp->moveMode = PARTY_MOVE_MODE_STAY;
        marioChgMot(MARIO_MOTION_STAY);
        marioStSystemLevel(0);
        break;
    }
    partyChkGnd(party);
}
