#include "version.h"
#include "win/win_main.h"
#include "win/win_party.h"
#include "battle/battle_database_common.h"
#include "driver/animdrv.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "data/item_data.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "mario/mariost.h"
#include "sdk/texPalette.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "icon_ids.h"
#include "unit/party/unit_party_christine.h"
#include "unit/party/unit_party_nokotarou.h"
#include "unit/party/unit_party_clauda.h"
#include "unit/party/unit_party_yoshi.h"
#include "unit/party/unit_party_vivian.h"
#include "unit/party/unit_party_sanders.h"
#include "unit/party/unit_party_chuchurina.h"
#include "libc_decls.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "sfx_name_word.h"
#include "win/win_mario.h"
#include "win/win_root.h"

extern ItemData itemDataTable[];

#ifdef VERSION_G8ME01
#endif

BattleWeapon* chri_waza[4] = {
    &partyWeapon_ChristineNormalAttack,
    &partyWeapon_ChristineMonosiri,
    &partyWeapon_ChristineRenzokuAttack,
    &partyWeapon_ChristineKiss,
};
BattleWeapon* noko_waza[4] = {
    &partyWeapon_NokotarouKouraAttack,
    &partyWeapon_NokotarouSyubibinKoura,
    &partyWeapon_NokotarouKouraGuard,
    &partyWeapon_NokotarouTsuranukiKoura,
};
BattleWeapon* cloud_waza[4] = {
    &partyWeapon_ClaudaNormalAttack,
    &partyWeapon_ClaudaBreathAttack,
    &partyWeapon_ClaudaPredationAttack,
    &partyWeapon_ClaudaKumogakureAttack,
};
BattleWeapon* yoshi_waza[4] = {
    &partyWeapon_YoshiNormalAttack,
    &partyWeapon_YoshiNomikomi_Shot,
    &partyWeapon_YoshiEggAttack_Minimini,
    &partyWeapon_YoshiCallGuard,
};
BattleWeapon* vivi_waza[4] = {
    &partyWeapon_VivianNormalAttack,
    &partyWeapon_VivianShadowGuard,
    &partyWeapon_VivianMagicalPowder,
    &partyWeapon_VivianCharmKissAttack,
};
BattleWeapon* sand_waza[4] = {
    &partyWeapon_SandersNormalAttack,
    &partyWeapon_SandersTimeBombSet,
    &partyWeapon_SandersCounterSet,
    &partyWeapon_SandersSuperBombAttack,
};
BattleWeapon* chu_waza[4] = {
    &partyWeapon_ChuchurinaNormalAttackLeft,
    &partyWeapon_ChuchurinaItemSteal,
    &partyWeapon_ChuchurinaMadowaseAttack,
    &partyWeapon_ChuchurinaKiss,
};

WindowPartyDt winPartyDt[7] = {
    {1, ICON_155, "name_party0", "msg_menu_party_kuri", "EFF_m_kuriko", "W_1", "d_mario", "R_2", chri_waza},
    {2, ICON_156, "name_party1", "msg_menu_party_noko", "EFF_m_noko", "W_1", "d_mario", "R_3", noko_waza},
    {5, ICON_15F, "name_party4", "msg_menu_party_cloud", "EFF_m_cloud", "W_1", "d_mario", "R_4", cloud_waza},
    {4, ICON_158, "name_party3", "msg_menu_party_yoshi", "EFF_m_yoshi", "W_1", "d_mario", "R_5", yoshi_waza},
    {6, ICON_160, "name_party5", "msg_menu_party_vivi", "c_vivian", "PTR_S_1", "d_mario", "R_12", vivi_waza},
    {3, ICON_157, "name_party2", "msg_menu_party_bom", "EFF_m_bomb", "W_1", "d_mario", "R_13", sand_waza},
    {7, ICON_161, "name_party6", "msg_menu_party_chu", "EFF_m_chu", "W_1", "d_mario", "R_14", chu_waza},
};

void winPartyInit(WindowWork* wp) {
    PouchData* pouch;
    s32 i;
    s32 j;
    const char* poseName;
    const char* animName;

    pouch = pouchGetPtr();
    wp->party.count = 0;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < PARTNER_COUNT; j++) {
            if (j == winPartyDt[i].partyId && (pouch->partyData[j].flags & 1) &&
                !(pouch->partyData[j].flags & 2)) {
                if (wp->flags & 0x2000) {
                    poseName = winPartyDt[i].poseName2;
                    switch (pouchGetPartyColor(PARTNER_YOSHI)) {
                        case 0:
                            animName = "R_5";
                            break;
                        case 1:
                            animName = "R_6";
                            break;
                        case 2:
                            animName = "R_7";
                            break;
                        case 3:
                            animName = "R_8";
                            break;
                        case 4:
                            animName = "R_9";
                            break;
                        case 5:
                            animName = "R_10";
                            break;
                        case 6:
                        default:
                            animName = "R_11";
                            break;
                    }
                } else if (j == 4) {
                    switch (pouchGetPartyColor(PARTNER_YOSHI)) {
                        case 0:
                            poseName = "EFF_m_yoshi";
                            break;
                        case 1:
                            poseName = "EFF_m_yoshi2";
                            break;
                        case 2:
                            poseName = "EFF_m_yoshi3";
                            break;
                        case 3:
                            poseName = "EFF_m_yoshi4";
                            break;
                        case 4:
                            poseName = "EFF_m_yoshi5";
                            break;
                        case 5:
                            poseName = "EFF_m_yoshi6";
                            break;
                        case 6:
                        default:
                            poseName = "EFF_m_yoshi7";
                            break;
                    }
                    animName = "W_1";
                } else {
                    poseName = winPartyDt[i].poseName;
                    animName = winPartyDt[i].animName;
                }
                wp->party.poseIds[wp->party.count] = animPoseEntry(poseName, 0);
                animPoseSetAnim(wp->party.poseIds[wp->party.count], animName, 1);
                animPoseSetMaterialFlagOn(wp->party.poseIds[wp->party.count], 0x1800);
                wp->party.partyIds[wp->party.count] = j;
                wp->party.count++;
            }
        }
    }

    wp->party.cursor = 0;
    for (i = 0; i < wp->party.count; i++) {
        s32 party;
        party = !marioBgmodeChk() ? marioGetParty() : marioGetPtr()->prevPartyMemberId[0];
        if (party == wp->party.partyIds[i]) {
            wp->party.cursor = i;
            break;
        }
    }

    for (i = 0; i < 8; i++) {
        if (wp->party.partyIds[wp->party.cursor] == winPartyDt[i].partyId) {
            wp->party.dtIdx = i;
            break;
        }
    }

    wp->party.startCursor = wp->party.cursor;
    wp->party.angle = wp->party.angleTarget = 360.0f / wp->party.count * wp->party.cursor;
    wp->party.mode = 0;
    wp->party.wazaCount = pouchGetPartyAttackLv(winPartyDt[wp->party.dtIdx].partyId) + 2;
    switch (pouchGetPartyColor(PARTNER_YOSHI)) {
        case 0:
            winPartyDt[3].nameMsgId = ICON_158;
            break;
        case 1:
            winPartyDt[3].nameMsgId = ICON_159;
            break;
        case 2:
            winPartyDt[3].nameMsgId = ICON_15A;
            break;
        case 3:
            winPartyDt[3].nameMsgId = ICON_15B;
            break;
        case 4:
            winPartyDt[3].nameMsgId = ICON_15C;
            break;
        case 5:
            winPartyDt[3].nameMsgId = ICON_15D;
            break;
        case 6:
        default:
            winPartyDt[3].nameMsgId = ICON_15E;
            break;
    }
}

void winPartyInit2(WindowWork* wp) {
    wp->party.angle = wp->party.angleTarget = 360.0f / wp->party.count * wp->party.cursor;
    wp->party.mode = 0;
    wp->helpTgtX = -200.0f;
    wp->helpTgtY = 20.0f;
    winMsgEntry(wp, 0, winPartyDt[wp->party.dtIdx].msg, 0);
}

void winPartyExit(WindowWork* wp) {
    s32 i;

    for (i = 0; i < wp->party.count; i++) {
        if (wp->party.poseIds[i] != -1) {
            animPoseRelease(wp->party.poseIds[i]);
        }
    }
}

s32 winPartyMain(WindowWork* wp) {
    s32 i;
    s32 prev;

    if (wp->party.count == 0) {
        return -1;
    }

    switch (wp->party.mode) {
        case 0:
            if (wp->buttonTrg & PAD_BUTTON_A) {
                wp->party.confirmed = 1;
                return -2;
            }
            if (wp->buttonTrg & PAD_BUTTON_X) {
                wp->party.mode = 1;
                wp->party.wazaCursor = 0;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_OK1 | PSND_SFX_BYPASS_MUTE));
            } else if (wp->buttonTrg & PAD_BUTTON_B) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
                wp->party.confirmed = 0;
                return -1;
            } else if (wp->buttonTrg & PAD_BUTTON_START) {
                wp->party.confirmed = 0;
                return -2;
            } else if (wp->dirRep & DEMO_STICK_LEFT) {
                prev = wp->party.cursor;
                wp->party.cursor = prev - 1;
                while (wp->party.cursor < 0) {
                    wp->party.cursor += wp->party.count;
                }
                wp->party.angleTarget = 360.0f / wp->party.count * wp->party.cursor;
                while (wp->party.angle < wp->party.angleTarget) {
                    wp->party.angle += 360.0f;
                }
                if (prev != wp->party.cursor) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
                for (i = 0; i < 8; i++) {
                    if (wp->party.partyIds[wp->party.cursor] == winPartyDt[i].partyId) {
                        wp->party.dtIdx = i;
                        break;
                    }
                }
            } else if (wp->dirRep & DEMO_STICK_RIGHT) {
                prev = wp->party.cursor;
                wp->party.cursor = prev + 1;
                while (wp->party.cursor > wp->party.count - 1) {
                    wp->party.cursor -= wp->party.count;
                }
                wp->party.angleTarget = 360.0f / wp->party.count * wp->party.cursor;
                while (wp->party.angle > wp->party.angleTarget) {
                    wp->party.angle -= 360.0f;
                }
                if (prev != wp->party.cursor) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
                }
                for (i = 0; i < 8; i++) {
                    if (wp->party.partyIds[wp->party.cursor] == winPartyDt[i].partyId) {
                        wp->party.dtIdx = i;
                        break;
                    }
                }
            }
            wp->helpTgtX = -200.0f;
            wp->helpTgtY = 20.0f;
            wp->party.wazaCount = pouchGetPartyAttackLv(winPartyDt[wp->party.dtIdx].partyId) + 2;
            winMsgEntry(wp, 0, winPartyDt[wp->party.dtIdx].msg, 0);
            break;
        case 1:
            if (wp->dirRep & DEMO_STICK_UP) {
                wp->party.wazaCursor--;
                if (wp->party.wazaCursor < 0) {
                    wp->party.wazaCursor = wp->party.wazaCount - 1;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (wp->dirRep & DEMO_STICK_DOWN) {
                wp->party.wazaCursor++;
                if (wp->party.wazaCursor >= wp->party.wazaCount) {
                    wp->party.wazaCursor = 0;
                }
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1 | PSND_SFX_BYPASS_MUTE));
            } else if (wp->buttonTrg & PAD_BUTTON_B) {
                wp->party.mode = 0;
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1 | PSND_SFX_BYPASS_MUTE));
            } else if (wp->buttonTrg & PAD_BUTTON_START) {
                wp->party.confirmed = 0;
                return -2;
            }
            wp->helpTgtX = -10.0f;
            wp->helpTgtY = -26.0f * wp->party.wazaCursor + 5.0f;
            winMsgEntry(wp, 0, winPartyDt[wp->party.dtIdx].waza[wp->party.wazaCursor]->description, 0);
            break;
    }
    return 0;
}

void winPartyMain2(WindowWork* wp) {
    wp->party.angle += (wp->party.angleTarget - wp->party.angle) / 4.0f;
}

static const GXColor winPartyWhiteC = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor winPartyBlackC = {0x00, 0x00, 0x00, 0xFF};
void winPartyDisp(s32 camId, void* param, s32 idx) {
    WindowWork* wp = param;
    GXColor white2;
    GXColor black2;
    GXColor red[1];
    GXColor white;
    GXColor black;
    GXColor white0;
    GXColor black0; 
    f32 scale;
    f32 tx;
    f32 bx;
    f32 fy;
    f32 by;
    f32 wy;
    f32 x;
    f32 y;
    f32 w1;
    f32 w2;
    f32 iy;
    s32 i;
    s32 pose;
    s32 j;
    AnimData* anim;
    const char* animName;
    s32 id;
    const char* name;
    PouchPartyData* pd;
    white0 = winPartyWhiteC;
    white = winPartyWhiteC;
    white2 = winPartyWhiteC;
    black0 = winPartyBlackC;
    black = winPartyBlackC;
    black2 = winPartyBlackC;
    x = wp->menu[idx].x;
    y = wp->menu[idx].y;
    winBgGX(wp, 1, x, y);
    if (wp->party.count == 0) {
        return;
    }

    pd = &pouchGetPtr()->partyData[winPartyDt[wp->party.dtIdx].partyId];
    fy = 20.0f;
    winKirinukiGX(wp, 0, x - 10.0f, fy + (116.0f + y), 240.0f, 76.0f);

    bx = 45.0f + (x - 10.0f);
    tx = bx - 2.0f;
    by = 110.0f + y;
    for (i = 0; i < 2; i++) {
        GXColor cols[2] = {{0xCB, 0x8C, 0xA2, 0xFE}, {0x9C, 0xC3, 0x81, 0xFE}};
        red[0] = (GXColor){0xA7, 0x3C, 0x3C, 0xFF};
        {
        const char* msgs[2] = {"msg_menu_party_hp", "msg_menu_party_rank"};
        const char* msgs2[2] = {"msg_menu_mario_ruby_hp", "msg_menu_mario_ruby_hp"};

        if (i == 0) {
            scale = 1.0f;
        } else {
            scale = 0.9f;
        }
        w1 = scale * FontGetMessageWidth(msgSearch(msgs[i]));
        w2 = 0.5f * FontGetMessageWidth(msgSearch(msgs2[i]));
        winTexInit(*wp->res->data);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
        winTexSet(0xAF, (Vec){tx, (fy = by - (( f32)(i * 32) )) - 4.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, cols[i]);
        winFontInit();
        winFontSet((Vec){bx - (w1 / 2.0f) , 12.0f + fy, 0.0f}, (Vec){scale, scale, scale}, black2, msgSearch(msgs[i]));
        if (i < 1) {
            winFontSet((Vec){bx - (w2 / 2.0f) , 21.0f + fy, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, red[i], msgSearch(msgs2[i]));
        }
        }
    }

    winIconInit();
    bx = 60.0f + bx;
    fy = 4.0f + by;
    winIconSet(ICON_19F, (Vec){bx, fy, 0.0f}, (Vec){0.82f, 0.82f, 0.82f}, white);
    for (i = 0; i < ( wp->party.wazaCount - 2) ; i++) {
        winIconSet(itemDataTable[0x57].icon_id, (Vec){bx + (( f32)(i * 34) ), iy = 4.0f + (by - 32.0f), 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white2);
    }

    winTexInit(*wp->res->data);
    winTexSet(0x10, (Vec){8.0f + (170.0f + (x - 10.0f)), fy, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    winFontInit();
    winFontSetR((Vec){10.0f + (110.0f + (x - 10.0f)), fy = 4.0f + (12.0f + by), 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pd->currentHP));
    winFontSet((Vec){8.0f + (180.0f + (x - 10.0f)), fy, 0.0f}, (Vec){0.9f, 0.9f, 0.9f}, black, "%s", winZenkakuStr(pd->maximumHP));
    winZClear();

    for (i = 0; i < wp->party.count; i++) {
        Vec v = {0.0f, 0.0f, 80.0f};
        Mtx rx;
        Mtx ry;
        PSMTXRotRad(rx, 'x', 0.5235988f);
        PSMTXRotRad(ry, 'y', 0.017453292f * ((( 360.0f / wp->party.count ) * i ) - wp->party.angle));
        PSMTXMultVec(ry, &v, &v);
        PSMTXMultVec(rx, &v, &v);
        winKageGX(wp, (x - 140.0f) + v.x, 20.0f + (y + v.y), v.z, (i == wp->party.cursor) ? 0.8f : 0.6f, (GXColor){0x00, 0x00, 0x00, 0x80});
    }

    for (i = 0; i < wp->party.count; i++) {
        Vec v = {0.0f, 0.0f, 80.0f};
        Mtx m;
        Mtx s;
        Mtx rx;
        Vec vp;
        Mtx ry;
        pose = wp->party.poseIds[i];
        if (pose == ( -1) ) {
            continue;
        }
        anim = animPoseGetAnimDataPtr(pose);
        PSMTXRotRad(rx, 'x', 0.5235988f);
        PSMTXRotRad(ry, 'y', 0.017453292f * ((( 360.0f / wp->party.count ) * i ) - wp->party.angle));
        PSMTXMultVec(ry, &v, &v);
        PSMTXMultVec(rx, &v, &v);
        if (i == wp->party.cursor) {
            PSMTXTrans(m, (x - 140.0f) + v.x, 20.0f + (y + v.y), v.z);
            PSMTXScale(s, 1.0f, 1.0f, -1.0f);
            PSMTXRotRad(ry, 'y', 3.1415927f);
            PSMTXConcat(m, s, m);
            PSMTXConcat(m, ry, m);
            animPoseSetMaterialFlagOff(pose, 0x40);
            for (j = 0; j < 7; j++) {
                if (wp->party.partyIds[i] == winPartyDt[j].partyId) {
                    break;
                }
            }
            if (wp->flags & 0x2000) {
                animName = winPartyDt[j].animName2;
                if (winPartyDt[j].partyId == 4) {
                    switch (pouchGetPartyColor(PARTNER_YOSHI)) {
                        case 0:
                            animName = "R_5";
                            break;
                        case 1:
                            animName = "R_6";
                            break;
                        case 2:
                            animName = "R_7";
                            break;
                        case 3:
                            animName = "R_8";
                            break;
                        case 4:
                            animName = "R_9";
                            break;
                        case 5:
                            animName = "R_10";
                            break;
                        case 6:
                        default:
                            animName = "R_11";
                            break;
                    }
                }
            } else {
                animName = winPartyDt[j].animName;
            }
            animPoseSetAnim(pose, animName, 0);
            animPoseSetLocalTime(pose, (f32)(gp->retraceCount % (s32)anim->loopInfo[2]));
        } else {
            PSMTXTrans(m, (x - 140.0f) + v.x, 20.0f + (y + v.y), v.z);
            PSMTXScale(s, 0.8f, 0.8f, -0.8f);
            PSMTXRotRad(ry, 'y', 3.1415927f);
            PSMTXConcat(m, s, m);
            PSMTXConcat(m, ry, m);
            animPoseSetMaterialFlagOn(pose, 0x40);
            animPoseSetMaterialEvtColor(pose, (GXColor){0x80, 0x80, 0x80, 0xFF});
            for (j = 0; j < 7; j++) {
                if (wp->party.partyIds[i] == winPartyDt[j].partyId) {
                    break;
                }
            }
            if (wp->flags & 0x2000) {
                animName = winPartyDt[j].animName2;
                if (winPartyDt[j].partyId == 4) {
                    switch (pouchGetPartyColor(PARTNER_YOSHI)) {
                        case 0:
                            animName = "Z_5";
                            break;
                        case 1:
                            animName = "Z_6";
                            break;
                        case 2:
                            animName = "Z_7";
                            break;
                        case 3:
                            animName = "Z_8";
                            break;
                        case 4:
                            animName = "Z_9";
                            break;
                        case 5:
                            animName = "Z_10";
                            break;
                        case 6:
                        default:
                            animName = "Z_11";
                            break;
                    }
                }
            } else {
                animName = winPartyDt[j].animName;
            }
            animPoseSetAnim(pose, animName, 0);
            animPoseSetLocalTime(pose, 0.0f);
        }
        animPoseMain(pose);
        if (wp->party.partyIds[i] == 6) {
            vp = (Vec){(x - 140.0f) + v.x, 20.0f + (y + v.y), v.z};
            animPoseVivianMain(pose, &vp);
        }
        animPoseDrawMtx(pose, m, 2, 0.0f, 5.0f);
    }

    if (wp->party.count != 0) {
        winNameGX(wp, 0, x - 230.0f, ( 20.0f + (y - 50.0f) ) - 10.0f, 200.0f, 32.0f);
        winFontInit();
        fy = ( 20.0f + ((y - 50.0f) - 4.0f) ) - 10.0f;
        for (i = 0; i < 7; i++) {
            id = winPartyDt[i].partyId;
            if (wp->party.partyIds[wp->party.cursor] == id) {
                if (id != 4) {
                    name = msgSearch(winPartyDt[i].name);
                } else {
                    name = pouchGetYoshiName();
                }
                w2 = FontGetMessageWidth(name);
                winFontSetEdge((Vec){((200.0f - w2) / 2.0f) + (x - 230.0f), fy, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white2, name);
            }
        }
    }

    if (wp->party.count > 1) {
        winFontInit();
        w1 = 0.8f * FontGetMessageWidth(msgSearch("msg_menu_party_henkou"));
        fy = ((-230.0f - w1) / 2.0f) + x;
        winFontSet((Vec){fy, y - 90.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, black, msgSearch("msg_menu_party_henkou"));
        winIconInit();
        winIconSet(ICON_064, (Vec){fy - 30.0f, y - 100.0f, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white);
        winTexInit(*wp->res->data);
        winTexSet(0xC0, (Vec){(x - 150.0f) - 50.0f, y - 16.0f, 0.0f}, (Vec){-0.6f, 0.6f, 0.6f}, white);
        winTexSet(0xC0, (Vec){50.0f + (x - 150.0f), y - 16.0f, 0.0f}, (Vec){0.6f, 0.6f, 0.6f}, white);
    }

    winFontInit();
    w1 = 0.7f * FontGetMessageWidth(msgSearch("msg_menu_party_waza_kakunin"));
    fy = x + (250.0f - w1);
    winFontSet((Vec){fy, y - 100.0f, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, black, msgSearch("msg_menu_party_waza_kakunin"));
    winIconInit();
    winIconSet(ICON_1F5, (Vec){fy - 20.0f, y - 110.0f, 0.0f}, (Vec){0.7f, 0.7f, 0.7f}, white);
    winWazaGX(wp, 0, x - 5.0f, wy = 50.0f + y, 260.0f, 144.0f);
    winFontInit();
    w1 = 0.8f * FontGetMessageWidth(msgSearch("msg_menu_party_waza"));
    fy = 45.0f + y;
    winFontSetEdge((Vec){((200.0f - w1) / 2.0f) + x, fy, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, msgSearch("msg_menu_party_waza"));
    bx = 230.0f + x;
    winFontSetEdge((Vec){bx - 20.0f, fy, 0.0f}, (Vec){0.8f, 0.8f, 0.8f}, white, VERSION_PICK("\x82\x65\x82\x6F", msgSearch("btl_cost_disp_FP")));
    for (i = 0; i < wp->party.wazaCount; i++) {
        char buf[0x40];
#ifdef VERSION_G8ME01
        winFontSetWidth((Vec){10.0f + x, fy = (wy - (( f32)(i * 26) )) - 30.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black2, 175.0f, msgSearch(winPartyDt[wp->party.dtIdx].waza[i]->name));
#else
        winFontSetLabel((Vec){10.0f + x, fy = (wy - (( f32)(i * 26) )) - 30.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black2, winPartyDt[wp->party.dtIdx].waza[i]->name);
#endif
        sprintf(buf, "%s", winZenkakuStr((u8)winPartyDt[wp->party.dtIdx].waza[i]->baseFpCost));
        winFontSet((Vec){(((238.0f + x) - 20.0f) - (( f32)(FontGetMessageWidth(buf) / 2) )) - 2.0f, fy, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black2, buf);
        winFontSet((Vec){2.0f + (2.0f + ((17.0f + bx) - 20.0f)), fy - 10.0f, 0.0f}, (Vec){0.5f, 0.5f, 0.5f}, black2, VERSION_PICK("\x82\x65\x82\x6F", msgSearch("btl_cost_disp_FP")));
    }
}
