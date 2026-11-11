// Shadow Mario (kagemario) in Twilight Town: while Doopliss has Mario's name, Mario is drawn as a dark silhouette by
// a map TEV callback (gra.rel and jin.rel have the same code).

#include "battle/battle_unit.h"
#include "usu.h"
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

static EvtWord evt_check[4];
EvtWord evt_usu_kagemario_party_kill[42];

/**
 * Map TEV callback 10 that draws Shadow Mario: appends a TEV stage that outputs a flat color,
 * black when Mario is in the background (flipped) mode and dark purple (77, 9, 82) otherwise.
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
 * Turns the Shadow Mario look on (nonzero) or off:
 * sets up TEV callback 10 for Mario's three paper-anim groups and toggles MARIO_FLAG_IS_SHADOW_MARIO.
 * Always returns 0.
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

/**
 * Script helper that never finishes: on every call (each frame) it clears the Shadow Mario tint from the active
 * partner's pose, so the partner is not drawn dark.
 */
static USER_FUNC(check) {
    if (partyGetPtr(PARTY_SLOT_PARTY) != NULL) {
        animPoseSetMaterialFlagOff(partyGetPtr(PARTY_SLOT_PARTY)->paperAnimGroup, MAP_OBJ_TEV_CALLBACK(10));
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Called by the map init scripts: if the story progress byte is in the range 201 to 211 (Doopliss has Mario's
 * name) it turns Shadow Mario on and starts the partner-tint cleaner, otherwise it turns it off.
 */
USER_FUNC(evt_kagemario_init) {
    s32 sequence = swByteGet(GSW_ID(GSW_STORY_PROGRESS));

    if (sequence >= STG4_10_02_TRADING_PLACES && sequence < STG4_15_02_BACK_TO_NORMAL) {
        color_change_evt(1);
        evtEntry(evt_check, 0, 0);
    } else {
        color_change_evt(0);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: turns the Shadow Mario look on or off from args[0]. */
USER_FUNC(evt_kagemario_change) {
    color_change_evt(event->args[0]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: clears the two remembered previous-partner ids in Mario's work area,
 * so no partner is called back afterwards.
 */
static USER_FUNC(test) {
    marioGetPtr()->prevPartyMemberId[PARTY_SLOT_PARTY] = PARTY_NONE;
    marioGetPtr()->prevPartyMemberId[PARTY_SLOT_FOLLOWER] = PARTY_NONE;
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
 * Between STG4_10_02_TRADING_PLACES and STG4_12_01_SUPERBOMBBOMB_FOUND (Mario has swapped places
 * with Doopliss) it removes both partner slots:
 * stops them, moves them out of the map, disables control and clears them from the party.
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
