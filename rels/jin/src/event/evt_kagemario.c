#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"

#include "mario/mario.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "driver/animdrv.h"
#include "party.h"
#include "driver/swdrv.h"

static void callback(MapTevCallbackInfo* info);
static s32 color_change_evt(s32 on);

static USER_FUNC(check);
static USER_FUNC(test);
USER_FUNC(evt_kagemario_change);
USER_FUNC(evt_kagemario_init);

static EvtWord evt_check[4];
EvtWord evt_usu_kagemario_party_kill[42];

/**
 * Texture-environment callback for Mario while he is a shadow (Doopliss has taken his place):
 * adds a flat colour stage, purple (77, 9, 82) normally and black while Mario is in the background mode.
 */
static void callback(MapTevCallbackInfo* info) {
    s32 stage = info->tevStage;
    s32 texMap = info->texMap;
    s32 texCoord = info->texCoord;
    s32 indStage = info->indStage;

    if (marioGetPtr()->flags & MARIO_FLAG_BG_MODE) {
        GXColor color = {0, 0, 0, 255};
        GXSetTevColor(GX_TEVREG0, color);
    } else {
        GXColor color = {77, 9, 82, 255};
        GXSetTevColor(GX_TEVREG0, color);
    }
    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevSwapMode(stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
    info->tevStage = stage + 1;
    info->texCoord = texCoord;
    info->indStage = indStage;
    info->texMap = texMap;
}

/**
 * Turns the shadow-Mario look on or off: on installs the callback on Mario's three paper poses
 * and sets the shadow- Mario flag, off removes them and clears the flag. Returns 0.
 */
static s32 color_change_evt(s32 on) {
    s32 i;

    if (on == 0) {
        for (i = 0; i < 3; i++) {
            animPoseSetMaterialFlagOff(marioGetPtr()->paperAnimGroupId[i], MAP_OBJ_TEV_CALLBACK(10));
        }
        marioGetPtr()->flags &= ~MARIO_FLAG_IS_SHADOW_MARIO;
    } else {
        mapSetTevCallback(10, callback);
        for (i = 0; i < 3; i++) {
            animPoseSetMaterialFlagOn(marioGetPtr()->paperAnimGroupId[i], MAP_OBJ_TEV_CALLBACK(10));
        }
        marioGetPtr()->flags |= MARIO_FLAG_IS_SHADOW_MARIO;
    }
    return 0;
}

/** Script helper, blocks forever: keeps the partner's paper pose free of the shadow tint so only Mario is darkened. */
// Keep removing the shadow-material override from the current partner while this event is blocked.
static USER_FUNC(check) {
    if (partyGetPtr(0) != NULL) {
        animPoseSetMaterialFlagOff(partyGetPtr(0)->paperAnimGroup, MAP_OBJ_TEV_CALLBACK(10));
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper run at the start of every Creepy Steeple and Twilight Town map:
 * switches the shadow-Mario tint on while the story progress is 201 to 211 (Doopliss has stolen Mario's place)
 * and off otherwise.
 */
USER_FUNC(evt_kagemario_init) {
    s32 sequence = swByteGet(0); // GSW_STORY_PROGRESS

    if (sequence >= STG4_10_02_TRADING_PLACES && sequence < STG4_15_02_BACK_TO_NORMAL) {
        color_change_evt(1);
        evtEntry(evt_check, 0, 0);
    } else {
        color_change_evt(0);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: turns the shadow-Mario tint on (args[0] non-zero) or off. */
USER_FUNC(evt_kagemario_change) {
    color_change_evt(event->args[0]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: forgets the previous partner ids stored in Mario's work so no partner is restored
 * after the shadow scene.
 */
// Clear both remembered partner slots before the solo Shadow Mario section.
static USER_FUNC(test) {
    marioGetPtr()->prevPartyMemberId[0] = 0;
    marioGetPtr()->prevPartyMemberId[1] = 0;
    return EVT_RETURN_DONE;
}

/**
 * Script run once by evt_kagemario_init: calls the check helper,
 * which keeps the partner free of the Shadow Mario tint.
 */
static EVT_BEGIN(evt_check)
    CALL(check)
    RETURN
EVT_END()

/**
 * Map-init script for story steps from "Trading Places" up to just before the Superbombomb is found:
 * removes both partners from the map and disables their control, since Mario goes on alone as the shadow.
 */
EVT_BEGIN(evt_usu_kagemario_party_kill)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG4_10_02_TRADING_PLACES, STG4_12_01_SUPERBOMBBOMB_FOUND)
        CALL(evt_party_stop, 0)
        CALL(evt_party_set_pos, 0, 0, -1000, 0)
        CALL(evt_party_cont_onoff, 0, 0)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_party_stop, 1)
        CALL(evt_party_set_pos, 1, 0, -1000, 0)
        CALL(evt_party_cont_onoff, 1, 0)
        CALL(evt_mario_clear_party, 1)
        CALL(test)
    END_SWITCH
    RETURN
EVT_END()

