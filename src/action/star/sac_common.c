#include "action/star/sac_common.h"
#include "battle/battle.h"
#include "battle/battle_ac_help.h"
#include "battle/battle_audience.h"
#include "manager/evtmgr.h"
#include "system.h"

extern s32 cheerEnable;
extern s32 handBeatEnable;
extern u32 enemy_slide_return_evt[21];
extern u32 enemy_slide_go_evt[24];
extern s32 count;
extern s32 count2;
extern u8 volPer;

/* Data-pass A1: exact identities referenced by the reconstructed Event family. */
extern const u8 zero_804283d0;
extern s32 btlevtcmd_AnimeChangePose(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_AnimeChangePoseType(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_CheckStatus(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_FaceDirectionAdd(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_GetHeight(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_GetHomePos(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_GetPartsPos(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_GetPos(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_GetStatusMg(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_MovePosition(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_SetMoveSpeed(EventEntry* event, BOOL isFirstCall);
extern s32 btlevtcmd_snd_se(EventEntry* event, BOOL isFirstCall);
extern s32 evt_audience_sound_cheer(EventEntry* event, BOOL isFirstCall);
extern s32 evt_btl_camera_set_homing_unit(EventEntry* event, BOOL isFirstCall);
extern s32 evt_btl_camera_set_mode(EventEntry* event, BOOL isFirstCall);
extern s32 evt_btl_camera_set_moveSpeedLv(EventEntry* event, BOOL isFirstCall);
extern s32 evt_btl_camera_set_zoom(EventEntry* event, BOOL isFirstCall);
extern s32 evt_btl_camera_set_zoomSpeedLv(EventEntry* event, BOOL isFirstCall);
extern s32 evt_eff(EventEntry* event, BOOL isFirstCall);
extern s32 evt_eff_delete(EventEntry* event, BOOL isFirstCall);

s32 sac_wao(void) {
    s32 audience = BattleAudience_GetAudienceNum();

    BattleAudienceSoundClap(0x78, 0x78);
    if (audience >= 1 && audience <= 0x95) {
        BattleAudienceSoundCheerKind(1, 0x78, 0x78);
    }
    if (audience >= 0x96) {
        BattleAudienceSoundCheerKind(4, 0x78, 0x78);
        BattleAudienceSoundWhistleKind(1);
    }
    return 2;
}

s32 sac_handbeat(void* event, s32 isFirstCall) {
    extern void BattleAudienceSoundHandBeat(void);

    s32 newVol;
    void* sound;

    if (isFirstCall != 0) {
        count = 0;
        count2 = 0x3C;
        handBeatEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(4) == 1) {
            BattleAudienceSoundStop(4);
        }
        if (BattleAudienceSoundCheck(5) == 1) {
            BattleAudienceSoundStop(5);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(4, 0, 1);
    }
    BattleAudienceSoundSetVol(4, volPer, 1);
    if (handBeatEnable == 0) {
        sound = BattleAudienceSoundGetPtr(4);
        *(s32*)((s32)sound + 0xC) = 0xB4;
        *(s32*)((s32)sound + 0x10) = 0xB4;
        return 2;
    }
    if (count < 0) {
        BattleAudienceSoundHandBeat();
        count = count2;
        count += irand(2);
        count2 -= 4;
        if (count2 < 0x14) {
            count2 = 0x14;
        }
        newVol = volPer + 5;
        volPer = newVol;
        if ((u8)newVol > 0x64) {
            volPer = 0x64;
        }
    }
    count--;
    return 0;
}

s32 sac_handbeat_end(void) {
    handBeatEnable = 0;
    return 2;
}

s32 sac_cheer(void* event, s32 isFirstCall) {
    extern s32 evtGetValue(void* event, s32 target);

    s32 value;
    s32 newVol;
    void* sound;

    value = evtGetValue(event, *(s32*)*(void**)((s32)event + 0x18));
    if (isFirstCall != 0) {
        count = 0;
        count2 = 0x3C;
        cheerEnable = 1;
        volPer = 0;
        if (BattleAudienceSoundCheck(4) == 1) {
            BattleAudienceSoundStop(4);
        }
        if (BattleAudienceSoundCheck(5) == 1) {
            BattleAudienceSoundStop(5);
        }
        BattleAudienceSoundCheer(-1, 0);
        BattleAudienceSoundSetVol(4, 0, 1);
    }
    if (value == 0) {
        BattleAudienceSoundSetVol(4, volPer, 1);
    }
    if (cheerEnable == 0) {
        sound = BattleAudienceSoundGetPtr(4);
        *(s32*)((s32)sound + 0xC) = 0xB4;
        *(s32*)((s32)sound + 0x10) = 0xB4;
        return 2;
    }
    if (count < 0) {
        count = count2;
        count += irand(2);
        count2 -= 4;
        if (count2 < 0x14) {
            count2 = 0x14;
        }
        newVol = volPer + 5;
        volPer = newVol;
        if ((u8)newVol > 0x64) {
            volPer = 0x64;
        }
    }
    count--;
    return 0;
}

s32 sac_cheer_end(void) {
    cheerEnable = 0;
    return 2;
}

s32 sac_enemy_slide_go(void) {
    s32 i;
    void* battleWork;
    void* script;

    script = enemy_slide_go_evt;
    i = 0;
    battleWork = _battleWorkPointer;
    for (; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);

        if (unit != NULL && *(s8*)((s32)unit + 0xC) == 1) {
            void* evt = evtEntry(script, 0, 0x20);

            *(s32*)((s32)evt + 0x160) = i;
        }
    }
    return 2;
}

s32 sac_enemy_slide_return(void) {
    s32 i;
    void* battleWork;
    void* script;

    script = enemy_slide_return_evt;
    i = 0;
    battleWork = _battleWorkPointer;
    for (; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);

        if (unit != NULL && *(s8*)((s32)unit + 0xC) == 1) {
            void* evt = evtEntry(script, 0, 0x20);

            *(s32*)((s32)evt + 0x160) = i;
        }
    }
    return 2;
}

s32 sac_ac_help_on(void) {
    BattleAcHelpSetDispType(2);
    return 2;
}

s32 sac_ac_help_off(void) {
    BattleAcHelpSetDispType(0);
    return 2;
}

const char str_SFX_VOICE_MARIO_HAPP_80300e40[] = "SFX_VOICE_MARIO_HAPPY1";
const char str_sac_treasure_80300e58[] = "sac_treasure";
const char str_treasure_map_80300e68[] = "treasure_map";
const char str_sac_itemget_80300e78[] = "sac_itemget";
const char str_itemget_80300e84[] = "itemget";
const char str_stardust_80300e8c[] = "stardust";

/*
 * Uninitialized small-data objects emit in reverse declaration order under
 * this unit's compiler settings.
 */
s32 count;
s32 count2;
s32 handBeatEnable;
s32 cheerEnable;
u8 volPer;

const char str_M_P_2_804283c8[] = "M_P_2";


/* Data-pass A1: complete target-backed sac_common Event family. */
u32 sac_start_camera_evt[37] = {
    0x0003005B, (u32)evt_btl_camera_set_mode, 0x00000000, 0x00000007,
    0x0004005B, (u32)evt_btl_camera_set_homing_unit, 0x00000000, 0xFFFFFFFE,
    0xFFFFFFFF, 0x0004005B, (u32)btlevtcmd_CheckStatus, 0xFFFFFFFE,
    0x0000000A, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0003005B, (u32)evt_btl_camera_set_zoom, 0x00000000,
    0x00000064, 0x00000020, 0x0003005B, (u32)evt_btl_camera_set_zoom,
    0x00000000, 0x0000012C, 0x00000021, 0x0003005B,
    (u32)evt_btl_camera_set_moveSpeedLv, 0x00000000, 0x00000002, 0x0003005B,
    (u32)evt_btl_camera_set_zoomSpeedLv, 0x00000000, 0x00000002, 0x00000002,
    0x00000001,
};

u32 sac_mario_item_on_evt[108] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)btlevtcmd_FaceDirectionAdd, 0xFFFFFFFE, 0xFE363C80, 0x0000000F,
    0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000005, 0xFFFFFFFF,
    0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_VOICE_MARIO_HAPP_80300e40, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0003005B, (u32)evt_audience_sound_cheer, 0x000000B4, 0x0000003C,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_M_P_2_804283c8, 0x0006005B, (u32)btlevtcmd_GetPartsPos, 0xFFFFFFFE,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)btlevtcmd_GetHeight, 0xFFFFFFFE, 0xFE363C85,
    0x0003005B, (u32)btlevtcmd_GetStatusMg, 0xFFFFFFFE, 0xFE363C86,
    0x0002003C, 0xFE363C85, 0xFE363C86, 0x00020035,
    0xFE363C81, 0xFE363C85, 0x00020035, 0xFE363C81,
    0x00000023, 0x00020035, 0xFE363C82, 0xFFFFFFFB,
    0x000F005B, (u32)evt_eff, (u32)(char*)str_sac_treasure_80300e58, (u32)(char*)str_treasure_map_80300e68,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00020035, 0xFE363C81, 0x00000002, 0x00020035,
    0xFE363C82, 0xFFFFFFF9, 0x000F005B, (u32)evt_eff,
    (u32)(char*)str_sac_itemget_80300e78, (u32)(char*)str_itemget_80300e84, 0x00000009, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000002, 0x00000001,
};

u32 sac_start_stardust[32] = {
    0x00010005, 0x00000006, 0x0005005B, (u32)btlevtcmd_GetPos,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0x00000032, 0x000F005B,
    (u32)evt_eff, (u32)(u8*)&zero_804283d0, (u32)(char*)str_stardust_80300e8c, 0x00000005,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000001E,
    0x0000001E, 0x0000000A, 0x0000001E, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x0000000A, 0x00000006, 0x00000002, 0x00000001,
};

u32 sac_mario_back_home[22] = {
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000028, 0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0008005B,
    (u32)btlevtcmd_MovePosition, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000005, 0xFFFFFFFF, 0x00000000,
    0x00000002, 0x00000001,
};

u32 enemy_slide_go_evt[24] = {
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C80,
    0x000001E0, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed, 0xFFFFFFFE,
    0xF24A8E80, 0x0008005B, (u32)btlevtcmd_MovePosition, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00000000, 0x00000001, 0x00000002, 0x00000001,
};

u32 enemy_slide_return_evt[21] = {
    0x0005005B, (u32)btlevtcmd_GetHomePos, 0xFFFFFFFE, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0003005B, (u32)btlevtcmd_SetMoveSpeed,
    0xFFFFFFFE, 0xF24A9680, 0x0008005B, (u32)btlevtcmd_MovePosition,
    0xFFFFFFFE, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000000, 0x00000001, 0x00000002,
    0x00000001,
};

u32 sac_mario_item_on_end_evt[8] = {
    0x0002005B, (u32)evt_eff_delete, (u32)(char*)str_sac_itemget_80300e78, 0x0002005B,
    (u32)evt_eff_delete, (u32)(char*)str_sac_treasure_80300e58, 0x00000002, 0x00000001,
};

/* Data-pass A2: target-backed one-byte .sdata2 zero object. */
const u8 zero_804283d0 = 0;
