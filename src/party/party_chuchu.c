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

typedef struct ChuchuParty {
    u32 flags;                 // 0x0
    u8 pad4[0x8 - 0x4];        // 0x4
    u32 field_8;               // 0x8
    u8 padC[0x24 - 0xC];       // 0xC
    s32 timer;                 // 0x24
    u8 pad28[0x32 - 0x28];     // 0x28
    u8 field_32;               // 0x32
    u8 field_33;               // 0x33
    u8 pad34[0x39 - 0x34];     // 0x34
    u8 state;                  // 0x39
    u8 pad3A;                  // 0x3A
    u8 field_3B;               // 0x3B
    u8 pad3C[0x58 - 0x3C];     // 0x3C
    Vec position;              // 0x58
    u8 pad64[0x94 - 0x64];     // 0x64
    Vec target;                // 0x94
    u8 padA0[0xF0 - 0xA0];     // 0xA0
    f32 height;                // 0xF0
    f32 field_F4;              // 0xF4
    f32 field_F8;              // 0xF8
    f32 field_FC;              // 0xFC
    f32 field_100;             // 0x100
    f32 field_104;             // 0x104
    u8 pad108[0x10C - 0x108];  // 0x108
    f32 field_10C;             // 0x10C
    f32 field_110;             // 0x110
    f32 velY;                  // 0x114
    u8 pad118[0x160 - 0x118];  // 0x118
    MarioWork* player;         // 0x160
} ChuchuParty;

typedef struct ChuchuTreasure {
    const char* map;
    u32 flag;
    s32 x;
    s32 y;
    s32 z;
} ChuchuTreasure;

extern void partyWalkInit(PartyEntry* party, s32 mode);
extern void partyUpdMoveSpd(PartyEntry* party);


char* mobj_list[] = {
    "MOBJ_TreasureBox",
    "MOBJ_BigTreasureBox",
    "MOBJ_GrayTreasureBox",
    "MOBJ_BlackTreasureBox",
    "MOBJ_KururinFloor",
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

static const Vec vecA = {0.0f, 0.0f, 0.0f};

USER_FUNC(set_msg);
USER_FUNC(search_result);
USER_FUNC(post_msg);

EVT_BEGIN(msg_evt)
    CALL(set_msg)
    CALL(search_result)
    IF_INT_NOT_EQUAL(4264967296, 0)
        CALL(evt_msg_print, 0, 4264967296, 0, STRING("party"))
    END_IF
    CALL(post_msg)
    RETURN
EVT_END()

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
    {"tou_05", 0xF8407179, 100, 0, -28},};

static const Vec vecB = {0.0f, 0.0f, 0.0f};

USER_FUNC(search_result) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (isFirstCall) {
        if ((u32)event->lwData[0] == 0 || event->lwData[1] == 0) {
            return EVT_RETURN_DONE;
        }
        if (event->lwData[1] == 1) {
            if (party) {
                effFukidashiEntry(6, 0, (NpcEntry*)FUKIDASHI_PARTY, 0.0f, 18.0f, 0.0f, 0.0f, 0.0f, sysMsec2Frame(1500));
            }
            return EVT_RETURN_DONE;
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_CHURINA_SMELL_JUMP1), &party->position);
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

static inline int chuchu_findTreasure2(ChuchuParty* cpp, f32 range, Vec* bestPos) {
    u32 i;
    int found;
    MarioWork* pl;
    ChuchuTreasure* tbl;
    f32 minDist;

    found = 0;
    pl = cpp->player;
    minDist = 10000.0f;
    tbl = chuchu_treasure_tbl;
    for (i = 0; i < 40; i++, tbl++) {
        if (strcmp(gp->currentMapName, tbl->map) == 0 && evtGetValue(NULL, tbl->flag) == 0) {
            Vec pos;
            Vec p = vecB;
            f32 d;
            p.x = tbl->x;
            p.y = tbl->y;
            p.z = tbl->z;
            pos = p;
            d = PSVECDistance(&pl->position, &pos);
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

USER_FUNC(set_msg) {
    MapObjectEntry* mobj;
    ItemEntry* item;
    int found;
    MarioWork* player;
    int pass;
    int type;
    ChuchuParty* cp = (ChuchuParty*)partyGetPtr(marioGetPartyId());
    PartyEntry* party = (PartyEntry*)cp;
    f32 range;
    f32 minDist;
    f32 angle;
    f32 d;
    Vec bestPos;
    Vec tmp;
    Vec ip;
    Vec p;
    Vec pos;

    player = cp->player;
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
                angle = revise360(angleABf(cp->position.x, cp->position.z, item->position.x, item->position.z) - cp->field_F8);
                type = 0;
            }
        }
        if (mobj) {
            if (minDist > PSVECDistance(&mobj->position, &player->position)) {
                minDist = PSVECDistance(&mobj->position, &player->position);
                angle = revise360(angleABf(cp->position.x, cp->position.z, mobj->position.x, mobj->position.z) - cp->field_F8);
                type = 1;
            }
        }
        if (found) {
            if (minDist > PSVECDistance(&bestPos, &player->position)) {
                minDist = PSVECDistance(&bestPos, &player->position);
                angle = revise360(angleABf(cp->position.x, cp->position.z, bestPos.x, bestPos.z) - cp->field_F8);
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
        cp->field_110 = (angle <= 180.0f) ? 180.0f : 0.0f;
        cp->field_FC = partyToMovedir(party, cp->field_110);
        cp->field_100 = cp->field_FC;
    } else {
        event->lwData[0] = (EvtWord)"map_no_exist";
        event->lwData[1] = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(post_msg) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    event->lwData[0] = 0;
    if (party) {
        party->useMotionId++;
    }
    return EVT_RETURN_DONE;
}

static inline int chuchu_findTreasure(ChuchuParty* cpp, f32 range) {
    u32 i;
    int found;
    MarioWork* player2;
    ChuchuTreasure* tbl;
    f32 minDist;

    found = 0;
    player2 = cpp->player;
    tbl = chuchu_treasure_tbl;
    minDist = 10000.0f;
    for (i = 0; i < 40; i++, tbl++) {
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

BOOL chuchu_searchObject(void) {
    ChuchuParty* cp = (ChuchuParty*)partyGetPtr(marioGetPartyId());
    MarioWork* player = cp->player;

    if (itemNearDistCheck(player->position.x, player->position.y, player->position.z, 150.0f)) {
        return TRUE;
    }
    if (mobjNearDistCheck2(player->position.x, player->position.y, player->position.z, 150.0f, mobj_list)) {
        return TRUE;
    }
    return chuchu_findTreasure(cp, 150.0f) != 0;
}

void chuchu_init(PartyEntry* party) {
    ChuchuParty* cp = (ChuchuParty*)party;

    cp->field_33 = 7;
    cp->field_32 = 0;
    cp->height = 20.0f;
    cp->field_F4 = 24.0f;
}

void chuchu_reinit(PartyEntry* party) {
}

void chuchu_exit(PartyEntry* party) {
}

void chuchu_move(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        partyWalkInit(party, 1);
    }
    partyUpdMoveSpd(party);
    partyWalkMain(party);
}

void chuchu_use(PartyEntry* party) {
    ChuchuParty* cp = (ChuchuParty*)party;
    MarioWork* player = cp->player;
    Vec pos;
    Vec pos1;

    if (cp->field_8 & 8) {
        cp->field_8 &= ~8;
        cp->state = 0;
        marioStSystemLevel(1);
    }

    switch (cp->state) {
    case 0:
        partyGetAppearPos3(party, &pos);
        cp->target = pos;
        cp->timer = 4;
        cp->field_FC = angleABf(cp->position.x, cp->position.z, cp->target.x, cp->target.z);
        cp->field_100 = cp->field_FC;
        cp->field_10C = player->displayDirection;
        cp->field_110 = player->displayDirection;
        cp->field_104 = distABf(cp->position.x, cp->position.z, cp->target.x, cp->target.z) / cp->timer;
        cp->velY = 0.0f;
        cp->state = 1;
        break;
    case 1:
        partyGetAppearPos3(party, &pos1);
        cp->target = pos1;
        cp->position.x += (cp->target.x - cp->position.x) / cp->timer;
        cp->position.y += (cp->target.y - cp->position.y) / cp->timer;
        cp->position.z += (cp->target.z - cp->position.z) / cp->timer;
        if (--cp->timer > 0) {
            return;
        }
        partyChgPoseId(party, 1);
        cp->field_104 = 0.0f;
        cp->state = 10;
        break;
    case 10:
        cp->field_110 = player->displayDirectionTarget;
        cp->field_FC = toMovedir(cp->field_110);
        cp->field_100 = cp->field_FC;
        cp->state = 20;
        cp->timer = 25;
        break;
    case 20:
        if (--cp->timer > 0) {
            break;
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_CHURINA_SMELL1), &cp->position);
        partyChgPose(party, "PCH_A2_1");
        cp->state = 21;
        cp->timer = 30;
    case 21:
        if (--cp->timer > 0) {
            break;
        }
        partyChgPoseId(party, 1);
        evtEntryType(msg_evt, 0, 0, 26);
        cp->state = 22;
        break;
    case 23:
        partyChgPoseId(party, 1);
        cp->state = 80;
        break;
    }

    switch (cp->state) {
    case 80:
        cp->flags &= ~0x100;
        partyChgRunMode(party, 2);
        cp->field_3B = 0;
        marioChgMot(MARIO_MOTION_STAY);
        marioStSystemLevel(0);
        break;
    }
    partyChkGnd(party);
}
