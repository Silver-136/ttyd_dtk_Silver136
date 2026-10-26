#include "battle/battle_seq_command.h"
#include "battle/battle.h"
#include "battle/battle_monosiri.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/msgdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "data/item_data.h"
#include "battle/battle_audience.h"
#include "battle/battle_pad.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "icon_ids.h"
#include "battle/battle_mario.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_item_data.h"
#include "battle/battle_ac_help.h"
#include "battle/battle_menu_disp.h"
#include "battle/battle_status_effect.h"
#include "sfx_name_word.h"
#include "battle/battle_camera.h"
#include "manager/fontmgr.h"
#include "driver/icondrv.h"
#include "driver/windowdrv.h"
#include "libc_decls.h"
#include "battle/battle_disp.h"
#include "system.h"
#include "battle/battle_stage.h"
#include "battle/battle_actrecord.h"
#include "battle/battle_attack_audience.h"
#include "battle/battle_message.h"
#include "eff/eff_recovery.h"
#include "battle/battle_database_common.h"
#include "battle/battle_detect.h"


void BattleMenuDisp_ChangePartySelect_Init(BattleWork* wp);
void BattleCommandDisplay_ActMenuEnd(BattleWork* wp);
void _btlcmd_MakeMultiItemTable(BattleWork* wp);
void _btlcmd_MakeOperationTable(BattleWork* wp);
void _btlcmd_MakeActClassTable(BattleWork* wp);
void FontDrawString(f32 x, f32 y, const char* msg);
unsigned long strlen(const char* s);
BOOL BattleCommandAttackAudienceCheck(void);
void BattleCommandDisplay_WeaponMultiItemCancelCheckDisp(CameraId camId, void* param);
USER_FUNC(_get_msg);
void BattleCommandDisplay_ActMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_WeaponSelectMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_OperationMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_ChangePartySelectMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_MultiItemMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_TargetSelectMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_ProtectPartnerSelectMenuDisp(CameraId camId, void* param);


//local prototypes
extern ItemData itemDataTable[];
extern BattleWeapon marioDefaultWeapon_Jump;
extern BattleWeapon marioWeapon_KururinJump;
extern BattleWeapon marioDefaultWeapon_Hammer;
s32 BtlUnit_GetWeaponCost(BattleUnit* unit, BattleWeapon* weapon);
s32 BtlUnit_CheckWeaponCost(BattleUnit* unit, BattleWeapon* weapon);
void BattleDrawEnemyHP(CameraId camId, void* param);
s32 _btlcmd_CheckWeaponTargetNum(BattleWork* wp, BattleUnit* unit, BattleWeapon* weapon);

typedef struct BattleCheckPartyEntry {
    s32 kind;          // 0x0
    const char* name;  // 0x4
    u16 icon;          // 0x8
    const char* help;  // 0xC
} BattleCheckPartyEntry;

extern BattleCheckPartyEntry _checkPartyTable[8];

void _btlcmd_GetCursorPtr(BattleCommand* cmd, u32 type, BattleCursor** out);
u16 _battleGetPartyIcon(s32 kind);
s32 _btlcmd_MakePartyTable(BattleWork* wp);
s32 _btlcmd_MakeSelectWeaponTable(BattleWork* wp, s32 type);
void _btlcmd_UpdateSelectWeaponTable(BattleWork* wp, s32 type);
s32 _getJumpIconId(void);
s32 _getHammerIconId(void);

s32 _jump_badge_table[8] = {ITEM_POWER_JUMP, ITEM_MULTIBOUNCE, ITEM_SHRINK_STOMP, ITEM_SLEEPY_STOMP, ITEM_SOFT_STOMP, ITEM_TORNADO_JUMP, ITEM_POWER_BOUNCE, ITEM_MEGA_JUMP};
s32 _hammer_badge_table[9] = {ITEM_POWER_SMASH, ITEM_PIERCING_BLOW, ITEM_HAMMER_THROW, ITEM_HEAD_RATTLE, ITEM_ICE_SMASH, ITEM_FIRE_DRIVE, ITEM_QUAKE_HAMMER, ITEM_MEGA_SMASH, ITEM_MEGA_QUAKE};

EVT_BEGIN(_btl_cmd_error_msg_event)
    CALL(_get_msg, LW(0))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -30, LW(0), 60)
    RETURN
EVT_END()

void BattleCommandInit(BattleWork* wp) {
    wp->commandMenu.state = 0;
    wp->flags |= BATTLE_FLAG_COMMAND_INPUT;
}

void _btlcmd_MakeActClassTable(BattleWork* wp) {
    PouchData* pouch;
    BattleUnit* unit;
    BattleCommand* cmd;
    BattleCursor* cursor;
    s32 num;
    NpcBattleInfo* info;

    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    cmd = &wp->commandMenu;
    pouch = pouchGetPtr();
    info = wp->fbatInfo->setup;
    if (cmd == NULL) {
        return;
    }
    if (unit->currentKind == UNIT_MARIO) {
        cursor = &cmd->cursor[0];
    } else {
        cursor = &cmd->cursor[1];
    }
    num = 0;
    if (unit->currentKind == UNIT_MARIO) {
        cmd->act[0].type = 0;
        cmd->act[0].enabled = 1;
        cmd->act[0].disableReason = 0;
        if (unit->mAttributeFlags & kUnitInVeil) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = 3;
        }
        if (pouch->jumpLevel < 1) {
            cmd->act[0].enabled = 0;
        }
        if (unit->badgesEquipped.Hammerman) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = 3;
        }
        if (wp->commandDisabled[0] > 0) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = 3;
        }
        if (info->battleFlags & NPC_BATTLE_NO_JUMP) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = 3;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_SPECIAL)) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = 3;
        }
        cmd->act[0].name = msgSearch("btl_cmd_act_jump");
        switch (pouch->jumpLevel) {
            case 1:
                cmd->act[0].icon = ICON_BTL_CMD_JUMP_LV1;
                break;
            case 2:
                cmd->act[0].icon = ICON_BTL_CMD_JUMP_LV2;
                break;
            default:
                cmd->act[0].icon = ICON_BTL_CMD_JUMP_LV3;
                break;
        }
        num = 1;
    }
    if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
        cmd->act[num].type = 6;
        if (unit->flippedTurns) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        } else {
            cmd->act[num].enabled = 1;
            cmd->act[num].disableReason = 0;
        }
        if (VERSION_PICK(wp->commandDisabled[3] > 0, unit->usCommandDisabled[0] != 0)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_SPECIAL)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_tech_party");
        cmd->act[num].icon = ICON_BTL_CMD_TECH_PARTY;
        num++;
    }
    if (unit->currentKind == UNIT_MARIO && pouch->hammerLevel >= 1) {
        cmd->act[num].type = 1;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = 0;
        if (unit->mAttributeFlags & kUnitInVeil) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        if (pouch->hammerLevel < 1) {
            cmd->act[num].enabled = 0;
        }
        if (unit->badgesEquipped.Jumpman) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        if (wp->commandDisabled[1] > 0) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        if (info->battleFlags & NPC_BATTLE_NO_HAMMER) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_SPECIAL)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_hammer");
        switch (pouch->hammerLevel) {
            case 1:
                cmd->act[num].icon = ICON_BTL_CMD_HAMMER_LV1;
                break;
            case 2:
                cmd->act[num].icon = ICON_BTL_CMD_HAMMER_LV2;
                break;
            default:
                cmd->act[num].icon = ICON_BTL_CMD_HAMMER_LV3;
                break;
        }
        num++;
    }
    if (unit->currentKind == UNIT_MARIO || (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX)) {
        cmd->act[num].type = 2;
        if (unit->flippedTurns) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        } else {
            cmd->act[num].enabled = 1;
            cmd->act[num].disableReason = 0;
        }
        if (unit->currentKind == UNIT_MARIO) {
            if (wp->commandDisabled[2] > 0) {
                cmd->act[num].enabled = 0;
                cmd->act[num].disableReason = 3;
            }
        } else {
            if (VERSION_PICK(wp->commandDisabled[4] > 0, unit->usCommandDisabled[1] != 0)) {
                cmd->act[num].enabled = 0;
                cmd->act[num].disableReason = 3;
            }
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_SPECIAL | BATTLE_FLAG_LECTURE_FREE | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_item");
        cmd->act[num].icon = ICON_BTL_CMD_ITEM;
        num++;
    }
    if (unit->currentKind == UNIT_MARIO && pouch->mStarPowersObtained != 0) {
        cmd->act[num].type = 4;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = 0;
        if (unit->mAttributeFlags & kUnitInVeil) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_FREE | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_super_action");
        cmd->act[num].icon = ICON_BTL_CMD_SUPER_ACTION;
        num++;
    }
    if (!(wp->flags & BATTLE_FLAG_LECTURE_FREE)) {
        cmd->act[num].type = 5;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = 0;
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_SPECIAL | BATTLE_FLAG_LECTURE_FREE | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_operation");
        cmd->act[num].icon = ICON_BTL_CMD_OPERATION;
        num++;
    }
    if (wp->flags & BATTLE_FLAG_LECTURE_FREE) {
        cmd->act[num].type = 0xD;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = 0;
        cmd->act[num].name = msgSearch("btl_cmd_act_lecture_end");
        cmd->act[num].icon = ICON_BTL_CMD_OPERATION;
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = 3;
        }
        num++;
    }
    cursor->count = num;
    if (cursor->absPos >= cursor->count) {
        cursor->absPos = cursor->count - 1;
    }
}

void _btlcmd_GetCursorPtr(BattleCommand* cmd, u32 type, BattleCursor** out) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, _battleWorkPointer->activeUnitId);

    switch (type) {
        case 14:
            if (unit->currentKind == UNIT_MARIO) {
                *out = &cmd->cursor[0];
            } else {
                *out = &cmd->cursor[1];
            }
            break;
        case 0:
            *out = &cmd->cursor[2];
            break;
        case 1:
            *out = &cmd->cursor[3];
            break;
        case 2:
            *out = &cmd->cursor[11];
            break;
        case 3:
            *out = &cmd->cursor[12];
            break;
        case 4:
            *out = &cmd->cursor[7];
            break;
        case 5:
            if (unit->currentKind == UNIT_MARIO) {
                *out = &cmd->cursor[8];
            } else {
                *out = &cmd->cursor[9];
            }
            break;
        case 6:
            *out = &cmd->cursor[4];
            break;
        case 7:
            *out = &cmd->cursor[5];
            break;
        case 8:
            *out = &cmd->cursor[6];
            break;
        case 10:
            *out = &cmd->cursor[10];
            break;
    }
}

#pragma dont_inline on
s32 _getJumpIconId(void) {
    switch (pouchGetPtr()->jumpLevel) {
        case 3:
            return ICON_ITEM_ULTRA_BOOTS;
        case 2:
            return ICON_ITEM_SUPER_BOOTS;
        default:
            return ICON_ITEM_BOOTS;
    }
}
#pragma dont_inline reset

#pragma dont_inline on
s32 _getHammerIconId(void) {
    switch (pouchGetPtr()->hammerLevel) {
        case 3:
            return ICON_ITEM_ULTRA_HAMMER;
        case 2:
            return ICON_ITEM_SUPER_HAMMER;
        default:
            return ICON_ITEM_HAMMER;
    }
}
#pragma dont_inline reset

s32 _btlcmd_MakeSelectWeaponTable(BattleWork* wp, s32 type) {
    s32 top;
    BattleUnit* unit;
    BattleCommand* cmd;
    PouchData* pouch;
    BattleCursor* cursor;
    s32 num;
    u32 i;
    s32 badge;
    u32 k;
    s32 badge2;
    s32 j;
    s16* items;
    void (*func)(BattleCommand*, s32*);
    static BattleWeapon defaultWeapon_Dummy_NoItem = {
        "btl_wn_dummy_noitem", 0, {0}, 0, "msg_dummy_noitem", 100, 0, 0, 0, 0.0f, 0, 0, 0, 0,
        NULL, {0}, NULL, {0}, 0x01000160, 0x20000002, 0, 0, 3, 2, NULL, 0,
        0, 0x80002000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* counterResist, targetWeight, sleep..confuse */
        0, 0, 0, 0, /* electric, dodgy */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* burn..payback */
        0, 0, 0, /* holdFast, invisible */
        0, 0, 0, 0, {0}, NULL, {0, 0, 0, 100}
    };

    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    cmd = &wp->commandMenu;
    pouch = pouchGetPtr();
    if (cmd == NULL) {
        return 0;
    }
    _btlcmd_GetCursorPtr(cmd, type, &cursor);
    num = 0;
    switch (type) {
        case 0:
            if (pouch->jumpLevel >= 1) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = 0;
                cmd->weapon[num].weapon = &marioDefaultWeapon_Jump;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = 0;
                cmd->weapon[num].icon = _getJumpIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                if (wp->flags & (BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_FREE) && wp->lectureUnitId != -1) {
                    cmd->weapon[num].weapon = BtlUnit_GetData(BattleGetUnitPtr(wp, wp->lectureUnitId), UNIT_DATA_LECTURE_JUMP_WEAPON);
                }
                num++;
            }
            if (pouch->jumpLevel >= 2) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = 0;
                cmd->weapon[num].weapon = &marioWeapon_KururinJump;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = 0;
                cmd->weapon[num].icon = _getJumpIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                num++;
            }
            if (pouch->jumpLevel >= 3) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = 0;
                cmd->weapon[num].weapon = &marioWeapon_JyabaraJump;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = 0;
                cmd->weapon[num].icon = _getJumpIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                num++;
            }
            break;
        case 1:
            if (pouch->hammerLevel >= 1) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = 0;
                cmd->weapon[num].weapon = &marioDefaultWeapon_Hammer;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = 0;
                cmd->weapon[num].icon = _getHammerIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                if (wp->flags & (BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_FREE) && wp->lectureUnitId != -1) {
                    cmd->weapon[num].weapon = BtlUnit_GetData(BattleGetUnitPtr(wp, wp->lectureUnitId), UNIT_DATA_LECTURE_HAMMER_WEAPON);
                }
                num++;
            }
            if (pouch->hammerLevel >= 2) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = 0;
                cmd->weapon[num].weapon = &marioWeapon_KaitenHammer;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = 0;
                cmd->weapon[num].icon = _getHammerIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                num++;
            }
            if (pouch->hammerLevel >= 3) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = 0;
                cmd->weapon[num].weapon = &marioWeapon_UltraHammer;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = 0;
                cmd->weapon[num].icon = _getHammerIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                num++;
            }
            break;
    }
    switch (type) {
        case 0:
            for (i = 0; i < 8; i++) {
                badge = _jump_badge_table[i];
                if (pouchEquipCheckBadge(badge) > 0) {
                    cmd->weapon[num].index = i;
                    cmd->weapon[num].itemId = badge;
                    cmd->weapon[num].weapon = itemDataTable[badge].battle_params;
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].disableReason = 0;
                    if (cmd->weapon[num].weapon->itemId) {
                        cmd->weapon[num].icon = itemDataTable[badge].icon_id;
                    } else {
                        cmd->weapon[num].icon = cmd->weapon[num].weapon->icon;
                    }
                    if (cmd->weapon[num].weapon->name) {
                        cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                    } else {
                        cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                    }
                    num++;
                }
            }
            break;
        case 1:
            for (k = 0; k < 9; k++) {
                badge2 = _hammer_badge_table[k];
                if (pouchEquipCheckBadge(badge2) > 0) {
                    cmd->weapon[num].index = k;
                    cmd->weapon[num].itemId = badge2;
                    cmd->weapon[num].weapon = itemDataTable[badge2].battle_params;
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].disableReason = 0;
                    if (cmd->weapon[num].weapon->itemId) {
                        cmd->weapon[num].icon = itemDataTable[badge2].icon_id;
                    } else {
                        cmd->weapon[num].icon = cmd->weapon[num].weapon->icon;
                    }
                    if (cmd->weapon[num].weapon->name) {
                        cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                    } else {
                        cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                    }
                    num++;
                }
            }
            break;
    }
    switch (type) {
        case 6:
            func = (void (*)(BattleCommand*, s32*))BtlUnit_GetData(unit, UNIT_DATA_TECH_MENU_FUNC);
            func(cmd, &num);
            break;
        case 2:
            items = pouch->heldItems;
            for (j = 0; j < 20; j++) {
                s32 id = items[j];
                ItemData* data = &itemDataTable[id];
                if (id != 0 && (data->usable_locations & 2)) {
                    cmd->weapon[num].index = j;
                    cmd->weapon[num].itemId = id;
                    cmd->weapon[num].weapon = data->battle_params;
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].disableReason = 0;
                    if (cmd->weapon[num].weapon == NULL) {
                        cmd->weapon[num].weapon = &ItemWeaponData_CookingItem;
                        cmd->weapon[num].name = msgSearch(data->name);
                        cmd->weapon[num].icon = data->icon_id;
                    } else {
                        cmd->weapon[num].icon = data->icon_id;
                        if (cmd->weapon[num].weapon->name) {
                            cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                        } else {
                            cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                        }
                    }
                    if (++num >= 21) {
                        break;
                    }
                }
            }
            if (num == 0) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = 0;
                cmd->weapon[num].index = 0;
                cmd->weapon[num].weapon = &defaultWeapon_Dummy_NoItem;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = 0;
                cmd->weapon[num].icon = ICON_UNAVAILABLE;
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                num++;
            }
            break;
        case 4: {
            u16 starPowers = pouch->mStarPowersObtained;
            s32 j;
            for (j = 0; j < 16; j++) {
                if ((starPowers & (1 << j)) && superActionTable[j]) {
                    cmd->weapon[num].index = -1;
                    cmd->weapon[num].itemId = 0;
                    cmd->weapon[num].weapon = superActionTable[j];
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].icon = superActionTable[j]->icon;
                    cmd->weapon[num].disableReason = 0;
                    if (cmd->weapon[num].weapon->name) {
                        cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                    } else {
                        cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                    }
                    if (++num >= 21) {
                        break;
                    }
                }
            }
            break;
        }
    }
    if (num <= 0) {
        return 0;
    }
    cursor->count = num;
    if (cursor->absPos >= cursor->count) {
        cursor->absPos = cursor->count - 1;
    }
    while (cursor->count < cursor->relPos + 6) {
        if ((top = cursor->relPos) <= 0) break;
        cursor->relPos--;
    }
    _btlcmd_UpdateSelectWeaponTable(wp, type);
    return num;
}

BattleCheckPartyEntry _checkPartyTable[8] = {
    {0, NULL, 0, NULL},
    {UNIT_GOOMBELLA, "btl_cmd_pt_christine", ICON_PARTY_GOOMBELLA, "btl_hlp_cmd_operation_party1"},
    {UNIT_KOOPS, "btl_cmd_pt_nokotarou", ICON_PARTY_KOOPS, "btl_hlp_cmd_operation_party2"},
    {UNIT_FLURRIE, "btl_cmd_pt_clauda", ICON_PARTY_FLURRIE, "btl_hlp_cmd_operation_party3"},
    {UNIT_YOSHI, "btl_cmd_pt_yoshi", ICON_PARTY_YOSHI, "btl_hlp_cmd_operation_party4"},
    {UNIT_VIVIAN, "btl_cmd_pt_vivian", ICON_PARTY_VIVIAN, "btl_hlp_cmd_operation_party5"},
    {UNIT_BOBBERY, "btl_cmd_pt_sanders", ICON_PARTY_BOBBERY, "btl_hlp_cmd_operation_party6"},
    {UNIT_MS_MOWZ, "btl_cmd_pt_chuchurina", ICON_PARTY_MS_MOWZ, "btl_hlp_cmd_operation_party7"},
};

void _btlcmd_UpdateSelectWeaponTable(BattleWork* wp, s32 type) {
    BattleUnit* unit;
    BattleCommand* cmd;
    BattleCursor* cursor;
    s32 i;
    s32 n;
    s32 result;
    BattleWeapon* weapon;

    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    cmd = &wp->commandMenu;
    _btlcmd_GetCursorPtr(cmd, type, &cursor);
    n = cursor->count;
    for (i = 0; i < n; i++) {
        weapon = cmd->weapon[i].weapon;
        cmd->weapon[i].enabled = 0;
        cmd->weapon[i].disableReason = 0;
        if (weapon) {
            result = _btlcmd_CheckWeaponTargetNum(wp, unit, weapon);
            if (result <= 0) {
                if (result == 0) {
                    switch (type) {
                        case 0:
                            cmd->weapon[i].disableReason = 4;
                            break;
                        case 1:
                            cmd->weapon[i].disableReason = 5;
                            break;
                        default:
                            cmd->weapon[i].disableReason = 3;
                            break;
                    }
                } else if (type != 4) {
                    cmd->weapon[i].disableReason = 1;
                } else {
                    cmd->weapon[i].disableReason = 2;
                }
            } else {
                cmd->weapon[i].enabled = 1;
                cmd->weapon[i].disableReason = 0;
            }
        }
    }
}

s32 _btlcmd_CheckWeaponTargetNum(BattleWork* wp, BattleUnit* unit, BattleWeapon* weapon) {
    BattleWorkWeaponTargets targets;

    if (weapon == NULL) {
        return 0;
    }
    if (weapon->mAttackEvtScript == NULL) {
        return 0;
    }
    if (!BtlUnit_CheckWeaponCost(unit, weapon)) {
        return -1;
    }
    {
        s32 belong = BtlUnit_GetEnemyBelong(unit);
        BattleSamplingEnemy(&targets, weapon, unit->unitId, belong, weapon->targetClassFlags,
            weapon->targetPropertyFlags, -wp->alliances[belong].attackDirection);
    }
    return targets.numTargets;
}

void _btlcmd_MakeOperationTable(BattleWork* wp) {
    PouchData* pouch;
    NpcBattleInfo* info;
    BattleUnit* unit;
    BattleCommand* cmd;
    s32 num;
    BattleUnit* party;
    BattleCursor* cursor;
    BattleWeapon* weapon;
    s32 idx;

    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    info = wp->fbatInfo->setup;
    cmd = &wp->commandMenu;
    pouch = pouchGetPtr();
    if (cmd == NULL) {
        return;
    }
    _btlcmd_GetCursorPtr(cmd, 5, &cursor);
    num = 0;
    if (_btlcmd_MakePartyTable(wp) >= 2) {
        cmd->operation[0].type = 0;
        if (unit->flippedTurns) {
            cmd->operation[0].enabled = 0;
            cmd->operation[0].disableReason = 3;
        } else {
            cmd->operation[0].enabled = 1;
            cmd->operation[0].disableReason = 0;
        }
        cmd->operation[0].name = msgSearch("btl_cmd_op_chg_party");
        party = unit;
        if (!(unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX)) {
            party = BattleGetPartnerPtr(wp, unit);
        }
        BtlUnit_CanActStatus(party);
        cmd->operation[0].icon = _battleGetPartyIcon(party->currentKind);
        cmd->operation[0].help = "btl_hlp_cmd_operation_changeparty";
        if (unit->mAttributeFlags & kUnitInVeil) {
            cmd->operation[0].enabled = 0;
            cmd->operation[0].disableReason = 3;
        }
        if (info->battleFlags & NPC_BATTLE_NO_PARTY_CHANGE) {
            cmd->operation[0].enabled = 0;
            cmd->operation[0].disableReason = 3;
        }
        cmd->operation[0].cost = 0;
        num = 1;
    }
    if (pouch->mStarPowersObtained != 0) {
        cmd->operation[num].type = 5;
        cmd->operation[num].enabled = 1;
        cmd->operation[num].name = msgSearch("btl_cmd_op_appeal");
        cmd->operation[num].icon = ICON_BTL_CMD_APPEAL;
        cmd->operation[num].disableReason = 0;
        cmd->operation[num].cost = 0;
        cmd->operation[num].help = "btl_hlp_cmd_operation_appeal";
        num++;
    }
    if (unit->badgesEquipped.Charge) {
        if (unit->currentKind == UNIT_MARIO) {
            weapon = &badgeWeapon_Charge;
        } else {
            weapon = &badgeWeapon_ChargeP;
        }
        cmd->operation[num].type = 1;
        cmd->operation[num].enabled = BtlUnit_CheckWeaponCost(unit, weapon);
        cmd->operation[num].cost = BtlUnit_GetWeaponCost(unit, weapon);
        if (cmd->operation[num].enabled) {
            cmd->operation[num].disableReason = 0;
        } else {
            cmd->operation[num].disableReason = 1;
        }
        cmd->operation[num].name = msgSearch(weapon->name);
        cmd->operation[num].icon = ICON_BTL_CMD_CHARGE;
        cmd->operation[num].help = weapon->description;
        if (unit->mAttributeFlags & kUnitInVeil) {
            cmd->operation[num].enabled = 0;
            cmd->operation[num].disableReason = 3;
        }
        num++;
    }
    if (unit->badgesEquipped.SuperCharge) {
        if (unit->currentKind == UNIT_MARIO) {
            weapon = &badgeWeapon_SuperCharge;
        } else {
            weapon = &badgeWeapon_SuperChargeP;
        }
        cmd->operation[num].type = 2;
        cmd->operation[num].enabled = BtlUnit_CheckWeaponCost(unit, weapon);
        cmd->operation[num].cost = BtlUnit_GetWeaponCost(unit, weapon);
        if (cmd->operation[num].enabled) {
            cmd->operation[num].disableReason = 0;
        } else {
            cmd->operation[num].disableReason = 1;
        }
        cmd->operation[num].name = msgSearch(weapon->name);
        cmd->operation[num].icon = ICON_BTL_CMD_SUPER_CHARGE;
        cmd->operation[num].help = weapon->description;
        if (unit->mAttributeFlags & kUnitInVeil) {
            cmd->operation[num].enabled = 0;
            cmd->operation[num].disableReason = 3;
        }
        num++;
    }
    cmd->operation[num].type = 6;
    cmd->operation[num].enabled = 1;
    cmd->operation[num].name = msgSearch("btl_cmd_op_defend");
    cmd->operation[num].icon = ICON_BTL_CMD_DEFEND;
    cmd->operation[num].disableReason = 0;
    cmd->operation[num].cost = 0;
    cmd->operation[num].help = "btl_hlp_cmd_operation_defend";
    if (unit->mAttributeFlags & kUnitInVeil) {
        cmd->operation[num].enabled = 0;
        cmd->operation[num].disableReason = 3;
    }
    idx = num + 1;
    cmd->operation[idx].type = 3;
    cmd->operation[idx].enabled = BtlUnit_CanActStatus(unit);
    cmd->operation[idx].name = msgSearch("btl_cmd_op_escape");
    cmd->operation[idx].icon = ICON_ARROW;
    cmd->operation[idx].cost = 0;
    if (cmd->operation[idx].enabled) {
        cmd->operation[idx].disableReason = 0;
    } else {
        cmd->operation[idx].disableReason = 1;
    }
    cmd->operation[idx].help = "btl_hlp_cmd_operation_escape";
    if (unit->mAttributeFlags & kUnitInVeil) {
        cmd->operation[idx].enabled = 0;
        cmd->operation[idx].disableReason = 3;
    }
    if (wp->fbatInfo->setup->battleFlags & NPC_BATTLE_NO_ESCAPE) {
        cmd->operation[idx].enabled = 0;
        cmd->operation[idx].disableReason = 3;
    }
    cursor->count = num + 2;
    if (cursor->absPos >= cursor->count) {
        cursor->absPos = cursor->count - 1;
    }
}

#pragma opt_propagation off
void _btlcmd_MakeMultiItemTable(BattleWork* wp) {
    BattleUnit* unit;
    BattleCommand* cmd;
    BattleCursor* cursor;
    s32 num;

    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    cmd = &wp->commandMenu;
    if (cmd == NULL) {
        return;
    }
    _btlcmd_GetCursorPtr(cmd, 3, &cursor);
    cmd->multiItem[0].padE[0] = 1;
    cmd->multiItem[0].enabled = 1;
    cmd->multiItem[0].name = msgSearch("btl_cmd_act_item");
    cmd->multiItem[0].icon = ICON_BTL_CMD_ITEM;
    cmd->multiItem[0].help = NULL;
    cmd->multiItem[0].cost = 0;
    num = 1;
    if (unit->badgesEquipped.DoubleDip) {
        cmd->multiItem[num].padE[0] = 2;
        if (unit->currentKind == UNIT_MARIO) {
            cmd->multiItem[num].enabled = BtlUnit_CheckWeaponCost(unit, &badgeWeapon_2KaiItem);
            cmd->multiItem[num].name = msgSearch(itemDataTable[ITEM_DOUBLE_DIP].name);
            cmd->multiItem[num].icon = itemDataTable[ITEM_DOUBLE_DIP].icon_id;
            cmd->multiItem[num].help = itemDataTable[ITEM_DOUBLE_DIP].description;
            cmd->multiItem[num].cost = (u8)BtlUnit_GetWeaponCost(unit, &badgeWeapon_2KaiItem);
        } else {
            cmd->multiItem[num].enabled = BtlUnit_CheckWeaponCost(unit, &badgeWeapon_2KaiItem);
            cmd->multiItem[num].name = msgSearch(itemDataTable[ITEM_DOUBLE_DIP_P].name);
            cmd->multiItem[num].icon = itemDataTable[ITEM_DOUBLE_DIP_P].icon_id;
            cmd->multiItem[num].help = itemDataTable[ITEM_DOUBLE_DIP_P].description;
            cmd->multiItem[num].cost = (u8)BtlUnit_GetWeaponCost(unit, &badgeWeapon_2KaiItem);
        }
        if (cmd->multiItem[num].enabled == 0) {
            cmd->multiItem[num].disableReason = 1;
        } else {
            cmd->multiItem[num].disableReason = 0;
        }
        num++;
    }
    if (unit->badgesEquipped.TripleDip) {
        cmd->multiItem[num].padE[0] = 3;
        if (unit->currentKind == UNIT_MARIO) {
            cmd->multiItem[num].enabled = BtlUnit_CheckWeaponCost(unit, &badgeWeapon_3KaiItem);
            cmd->multiItem[num].name = msgSearch(itemDataTable[ITEM_TRIPLE_DIP].name);
            cmd->multiItem[num].icon = itemDataTable[ITEM_TRIPLE_DIP].icon_id;
            cmd->multiItem[num].help = itemDataTable[ITEM_TRIPLE_DIP].description;
            cmd->multiItem[num].cost = (u8)BtlUnit_GetWeaponCost(unit, &badgeWeapon_3KaiItem);
        } else {
            cmd->multiItem[num].enabled = BtlUnit_CheckWeaponCost(unit, &badgeWeapon_3KaiItem);
            cmd->multiItem[num].name = msgSearch("in_3kai_item_p");
            cmd->multiItem[num].icon = ICON_TRIPLE_DIP_P;
            cmd->multiItem[num].help = "msg_3kai_item_p";
            cmd->multiItem[num].cost = (u8)BtlUnit_GetWeaponCost(unit, &badgeWeapon_3KaiItem);
        }
        if (cmd->multiItem[num].enabled == 0) {
            cmd->multiItem[num].disableReason = 1;
        } else {
            cmd->multiItem[num].disableReason = 0;
        }
        num++;
    }
    cursor->count = num;
    if (cursor->absPos >= cursor->count) {
        cursor->absPos = cursor->count - 1;
    }
}
#pragma opt_propagation reset

#pragma dont_inline on
u16 _battleGetPartyIcon(s32 kind) {
    BattleCheckPartyEntry* entry;
    s32 i;

    for (i = 0; i < 8; i++) {
        entry = &_checkPartyTable[i + 1];
        if (entry->kind == kind) {
            if (entry->kind == UNIT_YOSHI) {
                switch (pouchGetPartyColor(PARTNER_YOSHI)) {
                    case 1:
                        return ICON_PARTY_YOSHI_1;
                    case 2:
                        return ICON_PARTY_YOSHI_2;
                    case 3:
                        return ICON_PARTY_YOSHI_3;
                    case 4:
                        return ICON_PARTY_YOSHI_4;
                    case 5:
                        return ICON_PARTY_YOSHI_5;
                    case 6:
                        return ICON_PARTY_YOSHI_6;
                    case 0:
                    default:
                        return ICON_PARTY_YOSHI;
                }
            }
            return entry->icon;
        }
    }
    return 0;
}
#pragma dont_inline reset

s32 _btlcmd_MakePartyTable(BattleWork* wp) {
    BattleUnit* unit;
    BattleUnit* partner;
    BattleCommand* cmd;
    s32 num;
    s32 selected;
    PouchData* pouch;
    BattleCursor* cursor;
    PouchPartyData* party;
    s32 i;
    s32 kind;

    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    partner = BattleGetPartnerPtr(wp, unit);
    cmd = &wp->commandMenu;
    num = 0;
    selected = 0;
    pouch = pouchGetPtr();
    if (cmd == NULL) {
        return 0;
    }
    if (partner == NULL) {
        return 0;
    }
    _btlcmd_GetCursorPtr(cmd, 10, &cursor);
    for (i = 0; i < 8; i++) {
        kind = _checkPartyTable[i].kind;
        if (kind == 0) {
            continue;
        }
        party = &pouch->partyData[BattleTransPartyId(kind)];
        if (!(party->flags & 1)) {
            continue;
        }
        cmd->party[num].kind = kind;
        cmd->party[num].enabled = 1;
        cmd->party[num].name = (kind == UNIT_YOSHI) ? pouchGetYoshiName() : msgSearch(_checkPartyTable[i].name);
        cmd->party[num].icon = _battleGetPartyIcon(kind);
        cmd->party[num].help = _checkPartyTable[i].help;
        cmd->party[num].hp = party->currentHP;
        cmd->party[num].maxHp = party->maximumHP;
        if (unit->currentKind == kind || kind == BattleGetPartnerPtr(wp, unit)->currentKind) {
            cmd->party[num].enabled = 0;
            selected = num;
            if (unit->currentKind == kind) {
                cmd->party[num].hp = unit->currentHp;
            } else {
                cmd->party[num].hp = partner->currentHp;
            }
        } else if (party->currentHP <= 0) {
            cmd->party[num].enabled = 0;
        }
        if (party->flags & 2) {
            cmd->party[num].enabled = 0;
        }
        num++;
    }
    cursor->count = num;
    cursor->absPos = selected;
    return num;
}

#pragma dont_inline on
s32 _btlcmd_SelectWeaponDecide(BattleWork* wp, s32 type, s32 index) {
    BattleWorkWeaponTargets* targets;
    BattleWeapon* weapon;
    BattleUnit* unit;
    s32 selected;
    BattleWorkTarget* target;
    s32 i;
    s32 belong;

    targets = &wp->weaponTargets;
    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    _btlcmd_MakeSelectWeaponTable(wp, type);
    weapon = wp->commandMenu.weapon[index].weapon;
    BattleSamplingEnemy(targets, weapon, wp->activeUnitId, BtlUnit_GetEnemyBelong(unit), weapon->targetClassFlags,
        weapon->targetPropertyFlags, (s32)wp->alliances[unit->mAlliance].attackDirection);
    targets->currentTarget = 0;
    if (weapon->targetPropertyFlags & (TARGET_PROP_SAME_SIDE_FIRST | TARGET_PROP_OPPOSITE_SIDE_FIRST)) {
        selected = -1;
        belong = BtlUnit_GetBelong(unit);
        for (i = 0; i < targets->numTargets; i++) {
            target = &targets->targets[targets->targetIndices[i]];
            if (weapon->targetPropertyFlags & TARGET_PROP_SAME_SIDE_FIRST) {
                if (belong == BtlUnit_GetBelong(BattleGetUnitPtr(wp, target->unitIdx))) {
                    selected = i;
                }
            } else {
                if (belong != BtlUnit_GetBelong(BattleGetUnitPtr(wp, target->unitIdx))) {
                    selected = i;
                    break;
                }
            }
        }
        if (selected >= 0) {
            targets->currentTarget = selected;
        }
    }
    return 0;
}
#pragma dont_inline reset

void BtlUnit_PayWeaponCost(BattleUnit* unit, BattleWeapon* weapon);
void BattleCommandDisplay_WeaponSelectMenuSetup(BattleWork* wp, s32 type, s32 a);
s32 BattleCommandDisplay_WeaponSelectMenuMain(BattleWork* wp, s32 type);
void BattleCommandDisplay_WeaponMultiItemCancelCheckSetup(BattleWork* wp);
s32 BattleCommandDisplay_WeaponMultiItemCancelCheckMain(BattleWork* wp);
void BattleCommandDisplay_ActMenuSetup(BattleWork* wp, u32 a);
s32 BattleCommandDisplay_ActMenuMain(BattleWork* wp);
void BattleCommandDisplay_TargetSelectMenuSetup(BattleWork* wp);
void BattleCommandDisplay_TargetSelectMenuEnd(BattleWork* wp);
void BattleCommandDisplay_MultiItemMenuSetup(BattleWork* wp, s32 a);
s32 BattleCommandDisplay_MultiItemMenuMain(BattleWork* wp);
void BattleCommandDisplay_OperationMenuSetup(BattleWork* wp, s32 a);
s32 BattleCommandDisplay_OperationMenuMain(BattleWork* wp);
void BattleCommandDisplay_ChangePartySelectMenuSetup(BattleWork* wp);
s32 BattleCommandDisplay_ChangePartySelectMenuMain(BattleWork* wp);
void BattleCommandDisplay_ProtectPartnerSelectMenuSetup(BattleWork* wp);
void BattleCommandDisplay_ProtectPartnerSelectMenuEnd(BattleWork* wp);
void _commandRestoreRec(void);
s32 _check_present_item(void);
void _btlcmd_SetAttackEvent(BattleUnit* unit, BattleCommand* cmd);


s32 BattleCommandInput(BattleWork* work) {
    const char* sfxA;
    const char* sfxB;
    BattleUnit* unit;
    BattleCommand* cmd;
    s32 loop;
    s32 type;
    s32 wtype;
    BattleWorkWeaponTargets* targets;
    BattleWork* wp;
    s16 unitIdx;
    s16 partIdx;
    s32 state;
    BattleCursor* cursor;
    s32 i;
    BattleWorkTarget* t;

    wp = (BattleWork*)(void*)work; // FAKE: the void* hop keeps wp its own copy of work (r24 as in the target)
    unit = BattleGetUnitPtr(work, work->activeUnitId);
    cmd = &wp->commandMenu;
    if (cmd == NULL) {
        return 1;
    }
    _btlcmd_GetCursorPtr(cmd, 0xe, &cursor);
    if (_check_present_item()) {
        BattleAcHelpSetDispType(0);
    }
    _commandRestoreRec();
    // FAKE: comma expression `(0, X)`: X becomes a separate compiler temp; the plain X changes the object
    sfxA = (0, "SFX_BTL_CURSOR_MOVE2");
    loop = 1;
    while (loop) {
        loop = 0;
        sfxB = sfxA;
    again:
        switch (state = cmd->state) {
        case 0:
            if (wp->commandMenu.attackAudience) {
                switch (cmd->actType) {
                case 11:
                case 12:
                    cmd->actType = cmd->prevActType;
                    break;
                }
                if (wp->commandMenu.attackAudience == 2) {
                    wp->commandMenu.attackAudience = 0;
                }
            }
            cmd->cancelled = 0;
            wp->dispFlags |= 1;
            BtlUnit_SetCommandAnimPose(unit);
            if ((s8)(u8)unit->moveState >= 5 && (s8)(u8)unit->moveState <= 9) {
                BattleStageOnLightInCommand();
                BattleStageOffLightFriendInCommand();
                BattleStageOffLightEnemyInCommand();
                cmd->state = 5;
                break;
            }
            BattleCommandDisplay_ActMenuSetup(wp, 0);
            cmd->state = 2;
            BattleStageOnLightInCommand();
            BattleStageOffLightFriendInCommand();
            BattleStageOffLightEnemyInCommand();
            break;
        case 1:
            BattleCommandDisplay_ActMenuSetup(wp, 1);
            cmd->state = 2;
            break;
        case 2:
            if (BattleCommandDisplay_ActMenuMain(wp)) {
                loop = 1;
            }
            break;
        case 3:
            cmd->state = 4;
            cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
        case 4:
            if (!evtCheckID(cmd->errorEvtId)) {
                cmd->errorEvtId = 0;
                loop = 1;
                cmd->state = 1;
            }
            break;
        case 5:
        case 6:
            type = cmd->actType;
            if ((type == 0 && ((s32)wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) ||
                (type == 1 && ((s32)wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                _btlcmd_SelectWeaponDecide(wp, type, 0);
                BattleCommandDisplay_TargetSelectMenuSetup(wp);
                cmd->state = 10;
                BattleStageOnLightEnemyInCommand();
                break;
            }
            if (state == 5) {
                if (type == 2 && (unit->badgesEquipped.DoubleDip || unit->badgesEquipped.TripleDip)) {
                    BattleCommandDisplay_WeaponSelectMenuSetup(wp, type, 0);
                } else {
                    BattleCommandDisplay_WeaponSelectMenuSetup(wp, type, 1);
                }
            } else {
                BattleCommandDisplay_WeaponSelectMenuSetup(wp, type, 0);
            }
            BattleStageOffLightFriendInCommand();
            BattleStageOffLightEnemyInCommand();
            cmd->state = 7;
            _commandRestoreRec();
        case 7:
            if (BattleCommandDisplay_WeaponSelectMenuMain(wp, cmd->actType)) {
                loop = 1;
            }
            break;
        case 8:
            cmd->state = 9;
            cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
        case 9:
            if (!evtCheckID(cmd->errorEvtId)) {
                cmd->errorEvtId = 0;
                loop = 1;
                cmd->state = 6;
            }
            break;
        case 12:
            cmd->state = 13;
            BattleCommandDisplay_WeaponMultiItemCancelCheckSetup(wp);
            _commandRestoreRec();
            break;
        case 13:
            if (BattleCommandDisplay_WeaponMultiItemCancelCheckMain(wp)) {
                loop = 1;
            }
            break;
        case 10:
            _btlcmd_GetCursorPtr(cmd, cmd->actType, &cursor);
            BattleAcHelpSetHelp(cmd->weapon[cursor->absPos].weapon->acHelpMsg);
            BattleAcHelpSetDispType(1);
            cmd->state = 11;
            break;
        case 11:
            wtype = cmd->actType;
            targets = &wp->weaponTargets;
            _btlcmd_GetCursorPtr(cmd, wtype, &cursor);
            t = &targets->targets[targets->targetIndices[targets->currentTarget]];
            unitIdx = t->unitIdx;
            partIdx = t->partIdx;
            BattleSamplingEnemyUpdate(targets);
            for (i = 0; i < targets->numTargets; i++) {
                t = &targets->targets[targets->targetIndices[i]];
                if (t->unitIdx == unitIdx && t->partIdx == partIdx) {
                    break;
                }
            }
            if (i >= targets->numTargets) {
                i = 0;
            }
            targets->currentTarget = i;
            if ((s32)cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_SINGLE) {
                if (BattlePadCheckRepeat(BATTLE_PAD_STICK_RIGHT)) {
                    targets->currentTarget++;
                    if (targets->currentTarget >= targets->numTargets) {
                        targets->currentTarget = 0;
                    }
                    if (targets->numTargets >= 2) {
                        psndSFXOn(sfxA);
                    }
                }
                if (BattlePadCheckRepeat(BATTLE_PAD_STICK_LEFT)) {
                    targets->currentTarget--;
                    if (targets->currentTarget < 0) {
                        targets->currentTarget = targets->numTargets - 1;
                    }
                    if (targets->numTargets >= 2) {
                        psndSFXOn(sfxB);
                    }
                }
            }
            if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                BattleCommandDisplay_TargetSelectMenuEnd(wp);
                cmd->state = 6;
                wp->flags &= ~BATTLE_FLAG_ACT_DECIDED_UNUSED;
                BattleAcHelpSetDispType(0);
                BtlUnit_SetCommandAnimPose(unit);
                if ((cmd->actType == 0 && ((s32)wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) ||
                    (cmd->actType == 1 && ((s32)wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                    cmd->state = 1;
                    BattleStageOffLightEnemyInCommand();
                }
                break;
            }
            if (BattlePadCheckTrigger(PAD_BUTTON_A)) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                BattleStageOffLightInCommand(1, 1);
                cmd->state = 0x1d;
                BattleCommandDisplay_TargetSelectMenuEnd(wp);
                wp->flags |= BATTLE_FLAG_ACT_DECIDED_UNUSED;
                if (wtype == 2 && (s8)(u8)unit->moveState == 0) {
                    switch (cmd->dipMode) {
                    case 2:
                        unit->moveState = 8;
                        BtlUnit_PayWeaponCost(unit, &badgeWeapon_2KaiItem);
                        break;
                    case 3:
                        unit->moveState = 5;
                        BtlUnit_PayWeaponCost(unit, &badgeWeapon_3KaiItem);
                        break;
                    }
                }
                BattleAcHelpSetDispType(2);
            }
            if (BattleCommandAttackAudienceCheck()) {
                wp->commandMenu.attackAudience = 1;
                BattleCommandDisplay_TargetSelectMenuEnd(wp);
                BattleAcHelpSetDispType(0);
            }
            break;
        case 14:
        case 15:
            if (state == 14) {
                BattleCommandDisplay_MultiItemMenuSetup(wp, 1);
            } else {
                BattleCommandDisplay_MultiItemMenuSetup(wp, 0);
            }
            cmd->state = 16;
            _commandRestoreRec();
        case 16:
            if (BattleCommandDisplay_MultiItemMenuMain(wp)) {
                loop = 1;
            }
            break;
        case 17:
            cmd->state = 18;
            cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
        case 18:
            if (!evtCheckID(cmd->errorEvtId)) {
                cmd->errorEvtId = 0;
                loop = 1;
                cmd->state = 15;
            }
            break;
        case 19:
        case 20:
            if (state == 19) {
                BattleCommandDisplay_OperationMenuSetup(wp, 1);
            } else {
                BattleCommandDisplay_OperationMenuSetup(wp, 0);
            }
            cmd->state = 21;
            _commandRestoreRec();
        case 21:
            if (BattleCommandDisplay_OperationMenuMain(wp)) {
                loop = 1;
            }
            break;
        case 22:
            cmd->state = 23;
            cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
        case 23:
            if (!evtCheckID(cmd->errorEvtId)) {
                cmd->errorEvtId = 0;
                loop = 1;
                cmd->state = 20;
            }
            break;
        case 24:
        case 25:
            if (state == 24) {
                ((void (*)(BattleWork*, s32))BattleCommandDisplay_ChangePartySelectMenuSetup)(wp, 1);
            } else {
                ((void (*)(BattleWork*, s32))BattleCommandDisplay_ChangePartySelectMenuSetup)(wp, 0);
            }
            cmd->state = 26;
            _commandRestoreRec();
        case 26:
            if (BattleCommandDisplay_ChangePartySelectMenuMain(wp)) {
                loop = 1;
            }
            break;
        case 27:
            BattleCommandDisplay_ProtectPartnerSelectMenuSetup(wp);
            cmd->state = 28;
            _commandRestoreRec();
        case 28:
            if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                BattleCommandDisplay_ProtectPartnerSelectMenuEnd(wp);
                cmd->state = 20;
            } else if (BattlePadCheckTrigger(PAD_BUTTON_A)) {
                cmd->state = 0x1d;
                BattleCommandDisplay_ProtectPartnerSelectMenuEnd(wp);
            }
            break;
        case 29:
            _btlcmd_SetAttackEvent(unit, cmd);
            cmd->flags &= ~CMD_MENU_ENEMY_HP;
            wp->dispFlags &= ~1;
            BattleStageOffLightInCommand(0, 0);
            return 1;
        }
        if (wp->commandMenu.attackAudience) {
            _commandRestoreRec();
            goto again;
        }
    }
    return 0;
}


void BattleCommandDisplay(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;

    if (cmd != NULL) {
        SelectedItemCoordinateColorUpDate();
        dispEntry(CAMERA_2D, 902.0f, 0, BattleDrawEnemyHP, wp);
        if (cmd->flags & CMD_MENU_ACT) {
            dispEntry(CAMERA_2D, 500.0f, 0, BattleCommandDisplay_ActMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_WEAPON_SELECT) {
            dispEntry(CAMERA_2D, 500.0f, 0, BattleCommandDisplay_WeaponSelectMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_OPERATION) {
            dispEntry(CAMERA_2D, 500.0f, 0, BattleCommandDisplay_OperationMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_CHANGE_PARTY) {
            dispEntry(CAMERA_2D, 500.0f, 0, BattleCommandDisplay_ChangePartySelectMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_MULTI_ITEM) {
            dispEntry(CAMERA_2D, 500.0f, 0, BattleCommandDisplay_MultiItemMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_TARGET_SELECT) {
            dispEntry(CAMERA_2D, 500.0f, 0, BattleCommandDisplay_TargetSelectMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_PROTECT_PARTNER) {
            dispEntry(CAMERA_2D, 500.0f, 0, BattleCommandDisplay_ProtectPartnerSelectMenuDisp, wp);
        }
    }
}

#pragma dont_inline on
void _cursor_init(BattleCursor* cursor) {
    cursor->absPos = 0;
    cursor->relPos = 0;
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleCommandDisplay_ActMenuSetup(BattleWork* wp, u32 a) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;

    _btlcmd_MakeActClassTable(wp);
    _btlcmd_GetCursorPtr(cmd, 14, &cursor);
    BattleMenuDisp_ActSelect_Init(wp, cursor, cmd->act, a);
    cmd->idleTimer = 0;
    cmd->flags |= (CMD_MENU_ACT | CMD_MENU_ENEMY_HP);
    if (!a && !cmd->attackAudience) {
        _cursor_init(&cmd->cursor[2]);
        _cursor_init(&cmd->cursor[3]);
        _cursor_init(&cmd->cursor[4]);
        _cursor_init(&cmd->cursor[5]);
        _cursor_init(&cmd->cursor[6]);
        _cursor_init(&cmd->cursor[7]);
        _cursor_init(&cmd->cursor[8]);
        _cursor_init(&cmd->cursor[9]);
        _cursor_init(&cmd->cursor[11]);
    }
}
#pragma dont_inline reset

#define CHECK_CHANGE_UNIT(unit) \
    if (!unit) { \
        return FALSE; \
    } \
    if (unit->moveState) { \
        return FALSE; \
    } \
    if (unit->maxMovesThisTurn <= 0 || unit->maxMovesThisTurn != unit->movesRemaining) { \
        return FALSE; \
    } \
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) || BtlUnit_CheckStatus(unit, STATUS_FREEZE) || \
        BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) { \
        return FALSE; \
    }

BOOL BattleCommandCheckChangePositionEnable(BattleWork* wp);

s32 BattleCommandDisplay_ActMenuMain(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    s32 ret = 0;
    s32 i;
    s32 type;
    BattleCursor* cursor;

    _btlcmd_GetCursorPtr(cmd, 14, &cursor);
    if (BattlePadCheckTrigger(PAD_BUTTON_Y) && BattleCommandCheckChangePositionEnable(wp)) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
        BattleCommandDisplay_ActMenuEnd(wp);
        cmd->actType = 9;
        cmd->state = 0x1d;
        BattleMenuDisp_ActSelect_Main(wp);
        return 1;
    }
    while (BattleMenuKeyOKInACT(wp)) {
        for (i = 0; i < cursor->count; i++) {
            type = cmd->act[i].type;
            switch (type) {
            case 0:
            case 1:
                if ((type == 0 && (wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) || (type == 1 && (wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                    _btlcmd_MakeSelectWeaponTable(wp, type);
                    if (_btlcmd_CheckWeaponTargetNum(wp, unit, cmd->weapon[0].weapon) <= 0) {
                        cmd->act[i].enabled = 0;
                        if (!cmd->act[i].disableReason) {
                            if (type == 0) {
                                cmd->act[i].disableReason = 4;
                            } else {
                                cmd->act[i].disableReason = 5;
                            }
                        }
                    }
                }
                break;
            }
        }
        if (BattlePadCheckNow(-1)) {
            cmd->idleTimer = 0;
            btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT);
        } else if (++cmd->idleTimer >= 900) {
            btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_AUDIENCE);
            btl_camera_set_moveSpeedLv(0, 5);
        }
        if (wp->commandMenu.attackAudience) {
            type = cmd->act[cursor->absPos].type;
            if ((type == 0 && (wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) || (type == 1 && (wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                wp->commandMenu.attackAudience = 0;
                wp->commandMenu.recCommand = 0;
            }
        }
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || wp->commandMenu.attackAudience) {
            if (cmd->act[cursor->absPos].enabled) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                cmd->actType = cmd->act[cursor->absPos].type;
                switch (cmd->actType) {
                case 0:
                case 1:
                case 4:
                case 6:
                case 7:
                case 8:
                    cmd->state = 5;
                    ret = 1;
                    break;
                case 2:
                    cmd->state = 5;
                    if (!unit->moveState) {
                        cmd->dipMode = 1;
                        if (unit->badgesEquipped.DoubleDip || unit->badgesEquipped.TripleDip) {
                            cmd->state = 0xe;
                            ret = 1;
                        }
                    }
                    break;
                case 5:
                    cmd->state = 0x13;
                    ret = 1;
                    break;
                case 13:
                    cmd->state = 0x1d;
                    ret = 1;
                    break;
                }
                BattleCommandDisplay_ActMenuEnd(wp);
                break;
            }
            if (!wp->commandMenu.attackAudience) {
                psndSFXOn("SFX_NOT_CHOICE1");
            }
            wp->commandMenu.attackAudience = 0;
            if (cmd->act[cursor->absPos].disableReason) {
                BattleCommandDisplay_ActMenuEnd(wp);
                ret = 1;
                cmd->state = 3;
                cmd->disableReason = cmd->act[cursor->absPos].disableReason;
                break;
            }
        }
        if (BattlePadCheckRepeat(BATTLE_PAD_STICK_RIGHT)) {
            if (cursor->absPos < cursor->count - 1) {
                cursor->absPos++;
                psndSFXOn("SFX_BTL_CURSOR_MOVE1");
            } else {
                psndSFXOn("SFX_BTL_CURSOR_MOVE1");
                cursor->absPos = 0;
            }
        } else if (BattlePadCheckRepeat(BATTLE_PAD_STICK_LEFT)) {
            if (cursor->absPos > 0) {
                cursor->absPos--;
                psndSFXOn("SFX_BTL_CURSOR_MOVE1");
            } else {
                psndSFXOn("SFX_BTL_CURSOR_MOVE1");
                cursor->absPos = cursor->count - 1;
            }
        } else if (!BattleCommandAttackAudienceCheck() && BattlePadCheckTrigger(PAD_BUTTON_Y) && BattleCommandCheckChangePositionEnable(wp)) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
            BattleCommandDisplay_ActMenuEnd(wp);
            cmd->actType = 9;
            ret = 1;
            cmd->state = 0x1d;
        }
        break;
    }
    BattleMenuDisp_ActSelect_Main(wp);
    return ret;
}

BOOL BattleCommandCheckChangePositionEnable(BattleWork* wp) {
    BattleUnit* unit;

    if (wp->flags & BATTLE_FLAG_LECTURE_NO_COMMAND) {
        return FALSE;
    }
    if (wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED) {
        return FALSE;
    }
    unit = BattleGetMarioPtr(wp);
    CHECK_CHANGE_UNIT(unit)
    if (unit->mAttributeFlags & kUnitInVeil) {
        return FALSE;
    }
    unit = BattleGetPartyPtr(wp);
    CHECK_CHANGE_UNIT(unit)
    return unit->flippedTurns <= 0;
}

void BattleCommandDisplay_ActMenuDisp(CameraId camId, void* param) {
    BattleWork* wp = param;
    BattleCursor* cursor;

    _btlcmd_GetCursorPtr(&wp->commandMenu, 14, &cursor);
    BattleMenuDisp_ActSelect_Disp(wp);
}

#pragma dont_inline on
void BattleCommandDisplay_ActMenuEnd(BattleWork* wp) {
    BattleMenuDisp_ActSelect_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_ACT;
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleCommandDisplay_WeaponSelectMenuSetup(BattleWork* wp, s32 type, s32 a) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;

    _btlcmd_MakeSelectWeaponTable(wp, type);
    _btlcmd_GetCursorPtr(cmd, type, &cursor);
    BattleMenuDisp_WeaponSelect_Init(wp, cursor, cmd->weapon, a);
    cmd->flags |= (CMD_MENU_WEAPON_SELECT | CMD_MENU_ENEMY_HP);
}
#pragma dont_inline reset

void BattleCommandDisplay_WeaponSelectMenuEnd(BattleWork* wp);

s32 BattleCommandDisplay_WeaponSelectMenuMain(BattleWork* wp, s32 type) {
    BattleCommand* cmd = &wp->commandMenu;
    s32 ret;
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    BattleCursor* cursor;

    ret = 0;
    while (BattleMenuKeyOKInACT(wp)) {
        _btlcmd_GetCursorPtr(cmd, type, &cursor);
        _btlcmd_UpdateSelectWeaponTable(wp, type);
        if (wp->commandMenu.attackAudience) {
            wp->commandMenu.attackAudience = 0;
            wp->commandMenu.recCommand = 0;
        }
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || wp->commandMenu.attackAudience) {
            if (cmd->weapon[cursor->absPos].enabled) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                _btlcmd_SelectWeaponDecide(wp, type, cursor->absPos);
                BattleCommandDisplay_TargetSelectMenuSetup(wp);
                cmd->state = 10;
                if (!(cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_NO_OWN_SIDE)) {
                    BattleStageOnLightFriendInCommand();
                }
                if (!(cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_ONLY_SELF)
                    && !(cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_NO_OPPOSITE_SIDE)
                    && !(cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_NO_ENEMY)) {
                    BattleStageOnLightEnemyInCommand();
                }
                ret = 1;
                break;
            }
            if (!wp->commandMenu.attackAudience) {
                psndSFXOn("SFX_NOT_CHOICE1");
            }
            wp->commandMenu.attackAudience = 0;
            if (cmd->weapon[cursor->absPos].disableReason) {
                BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                ret = 1;
                cmd->state = 8;
                cmd->disableReason = cmd->weapon[cursor->absPos].disableReason;
            }
            break;
        }
        if (cursor->absPos < cursor->count - 1) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos++;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                break;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos = 0;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                break;
            }
        }
        if (cursor->absPos > 0) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_UP)) {
                cursor->absPos--;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                break;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_UP)) {
                cursor->absPos = cursor->count - 1;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                break;
            }
        }
        if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
            if (type == 2 && (unit->badgesEquipped.DoubleDip || unit->badgesEquipped.TripleDip)) {
                if (!unit->moveState) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                    BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                    cmd->state = 0xf;
                } else {
                    BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                    cmd->state = 0xc;
                }
            } else {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                cmd->state = 1;
            }
            ret = 1;
        } else if (BattleCommandAttackAudienceCheck()) {
            BattleCommandDisplay_WeaponSelectMenuEnd(wp);
            wp->commandMenu.attackAudience = 1;
        }
        break;
    }
    BattleMenuDisp_WeaponSelect_Main(wp);
    return ret;
}

void BattleCommandDisplay_WeaponSelectMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_WeaponSelect_Disp(param);
}

#pragma dont_inline on
void BattleCommandDisplay_WeaponSelectMenuEnd(BattleWork* wp) {
    BattleMenuDisp_WeaponSelect_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_WEAPON_SELECT;
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleCommandDisplay_WeaponMultiItemCancelCheckSetup(BattleWork* wp) {
    wp->commandMenu.cursor[13].absPos = 0;
    wp->commandMenu.cursor[13].relPos = 0;
    wp->commandMenu.cursor[13].count = 2;
}
#pragma dont_inline reset

s32 BattleCommandDisplay_WeaponMultiItemCancelCheckMain(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor = &cmd->cursor[13];
    s32 done = 0;
    s32 cancel = 0;

    if (BattleCommandAttackAudienceCheck()) {
        wp->commandMenu.attackAudience = 1;
        return 0;
    }
    if (cursor->absPos < cursor->count - 1) {
        if (BattlePadCheckRepeat(BATTLE_PAD_STICK_DOWN)) {
            cursor->absPos++;
            psndSFXOn("SFX_BTL_CURSOR_MOVE2");
            goto check;
        }
    } else {
        if (BattlePadCheckTrigger(BATTLE_PAD_STICK_DOWN)) {
            cursor->absPos = 0;
            psndSFXOn("SFX_BTL_CURSOR_MOVE2");
            goto check;
        }
    }
    if (cursor->absPos > 0) {
        if (BattlePadCheckRepeat(BATTLE_PAD_STICK_UP)) {
            cursor->absPos--;
            psndSFXOn("SFX_BTL_CURSOR_MOVE2");
        }
    } else {
        if (BattlePadCheckTrigger(BATTLE_PAD_STICK_UP)) {
            cursor->absPos = cursor->count - 1;
            psndSFXOn("SFX_BTL_CURSOR_MOVE2");
        }
    }
check:
    if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
        done = 1;
        cancel = 0;
    } else if (BattlePadCheckTrigger(PAD_BUTTON_A)) {
        done = 1;
        if (cursor->absPos == 0) {
            cancel = 1;
        }
    }
    if (done) {
        if (cancel) {
            wp->commandMenu.cancelled = 1;
            cmd->state = 0x1d;
        } else {
            cmd->state = 6;
        }
    }
    dispEntry(CAMERA_2D, 900.0f, 0, BattleCommandDisplay_WeaponMultiItemCancelCheckDisp, wp);
    return done;
}

void BattleCommandDisplay_WeaponMultiItemCancelCheckDisp(CameraId camId, void* param) {
    BattleWork* wp = param;
    char buf[36];
    Vec pos;
    f32 w;
    f32 x;
    u32 len;

    pouchGetPtr();
    sprintf(buf, msgSearch("btl_msg_multi_item_cancel_check"));
    len = strlen(buf);
    ((void (*)(f32, f32, f32, f32, f32, u16, GXColor))windowDispGX_Waku_col)(-30.0f - (w = 6.25f * (f32)len), 30.0f, 12.5f * (f32)len + 65.0f, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, -70.0f, -10.0f, 130.0f, 60.0f, 10.0f);
    FontDrawStart();
    x = 0.0f;
    FontDrawString(x - w, 27.0f, buf);
#ifdef VERSION_G8ME01
    FontDrawString(-20.0f, -13.0f, msgSearch("btl_msg_lvup_yes"));
    FontDrawString(-20.0f, -38.0f, msgSearch("btl_msg_lvup_no"));
#else
    FontDrawString(-20.0f, -13.0f, "\202\315\202\242\201@");
    FontDrawString(-20.0f, -38.0f, "\202\242\202\242\202\246");
#endif
    pos.x = -40.0f;
    pos.y = -42 - wp->commandMenu.cursor[13].absPos * 25;
    pos.z = 0.0f;
    iconDispGx(&pos, 0x14, ICON_YUBI_CURSOR, 1.0f);
}

#pragma dont_inline on
void BattleCommandDisplay_OperationMenuSetup(BattleWork* wp, s32 a) {
    _btlcmd_MakeOperationTable(wp);
    BattleMenuDisp_Operation_Init(wp, a);
    wp->commandMenu.flags |= (CMD_MENU_OPERATION | CMD_MENU_ENEMY_HP);
}
#pragma dont_inline reset

void BattleCommandDisplay_OperationMenuEnd(BattleWork* wp);

s32 BattleCommandDisplay_OperationMenuMain(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    s32 ret = 0;
    BattleCursor* cursor;

    if (wp->commandMenu.attackAudience) {
        wp->commandMenu.attackAudience = 0;
        wp->commandMenu.recCommand = 0;
    }
    if (BattleMenuKeyOKInACT(wp)) {
        _btlcmd_GetCursorPtr(cmd, 5, &cursor);
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || wp->commandMenu.attackAudience) {
            if (cmd->operation[cursor->absPos].enabled) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                cmd->operationType = cmd->operation[cursor->absPos].type;
                switch (cmd->operationType) {
                case 1:
                case 2:
                case 3:
                case 5:
                case 6:
                case 7:
                case 8:
                    BattleStageOffLightInCommand(0, 1);
                    cmd->state = 0x1d;
                    break;
                case 0:
                    cmd->state = 0x18;
                    break;
                case 4:
                    cmd->state = 0x1b;
                    break;
                }
                BattleCommandDisplay_OperationMenuEnd(wp);
                ret = 1;
                goto end;
            }
            if (!wp->commandMenu.attackAudience) {
                psndSFXOn("SFX_NOT_CHOICE1");
            }
            wp->commandMenu.attackAudience = 0;
            if (cmd->operation[cursor->absPos].disableReason) {
                BattleCommandDisplay_OperationMenuEnd(wp);
                ret = 1;
                cmd->state = 0x16;
                cmd->disableReason = cmd->operation[cursor->absPos].disableReason;
            }
            goto end;
        }
        if (cursor->absPos < cursor->count - 1) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos++;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos = 0;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        }
        if (cursor->absPos > 0) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_UP)) {
                cursor->absPos--;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_UP)) {
                cursor->absPos = cursor->count - 1;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        }
        if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
            BattleCommandDisplay_OperationMenuEnd(wp);
            ret = 1;
            cmd->state = 1;
        } else if (BattleCommandAttackAudienceCheck()) {
            wp->commandMenu.attackAudience = 1;
            BattleCommandDisplay_OperationMenuEnd(wp);
        }
    }
end:
    BattleMenuDisp_Operation_Main(wp);
    return ret;
}

void BattleCommandDisplay_OperationMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_Operation_Disp(param);
}

#pragma dont_inline on
void BattleCommandDisplay_OperationMenuEnd(BattleWork* wp) {
    BattleMenuDisp_Operation_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_OPERATION;
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleCommandDisplay_MultiItemMenuSetup(BattleWork* wp, s32 a) {
    _btlcmd_MakeMultiItemTable(wp);
    BattleMenuDisp_MultiItem_Init(wp, a);
    wp->commandMenu.flags |= CMD_MENU_MULTI_ITEM;
}
#pragma dont_inline reset

void BattleCommandDisplay_MultiItemMenuEnd(BattleWork* wp);

s32 BattleCommandDisplay_MultiItemMenuMain(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    s32 ret = 0;
    BattleCursor* cursor;

    if (BattleMenuKeyOKInACT(wp)) {
        _btlcmd_GetCursorPtr(cmd, 3, &cursor);
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || wp->commandMenu.attackAudience) {
            if (cmd->multiItem[cursor->absPos].enabled) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                cmd->dipMode = cmd->multiItem[cursor->absPos].padE[0];
                cmd->state = 5;
                BattleCommandDisplay_MultiItemMenuEnd(wp);
                ret = 1;
                goto end;
            }
            if (!wp->commandMenu.attackAudience) {
                psndSFXOn("SFX_NOT_CHOICE1");
            }
            wp->commandMenu.attackAudience = 0;
            if (cmd->multiItem[cursor->absPos].disableReason) {
                BattleCommandDisplay_MultiItemMenuEnd(wp);
                ret = 1;
                cmd->state = 0x11;
                cmd->disableReason = cmd->multiItem[cursor->absPos].disableReason;
            }
            goto end;
        }
        if (cursor->absPos < cursor->count - 1) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos++;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos = 0;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        }
        if (cursor->absPos > 0) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_UP)) {
                cursor->absPos--;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_UP)) {
                cursor->absPos = cursor->count - 1;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        }
        if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
            BattleCommandDisplay_MultiItemMenuEnd(wp);
            ret = 1;
            cmd->state = 1;
        } else if (BattleCommandAttackAudienceCheck()) {
            wp->commandMenu.attackAudience = 1;
            BattleCommandDisplay_MultiItemMenuEnd(wp);
        }
    }
end:
    BattleMenuDisp_MultiItem_Main(wp);
    return ret;
}

void BattleCommandDisplay_MultiItemMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_MultiItem_Disp(param);
}

#pragma dont_inline on
void BattleCommandDisplay_MultiItemMenuEnd(BattleWork* wp) {
    BattleMenuDisp_MultiItem_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_MULTI_ITEM;
}
#pragma dont_inline reset

void BattleCommandDisplay_ChangePartySelectMenuSetup(BattleWork* wp) {
    BattleMenuDisp_ChangePartySelect_Init(wp);
    wp->commandMenu.flags |= (CMD_MENU_CHANGE_PARTY | CMD_MENU_ENEMY_HP);
}

void BattleCommandDisplay_ChangePartySelectMenuEnd(BattleWork* wp);

s32 BattleCommandDisplay_ChangePartySelectMenuMain(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    s32 ret = 0;
    BattleCursor* cursor;

    if (BattleMenuKeyOKInACT(wp)) {
        _btlcmd_GetCursorPtr(cmd, 10, &cursor);
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || wp->commandMenu.attackAudience) {
            if (cmd->party[cursor->absPos].enabled) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                BattleStageOffLightInCommand(0, 1);
                cmd->partyKind = cmd->party[cursor->absPos].kind;
                cmd->state = 0x1d;
                BattleCommandDisplay_ChangePartySelectMenuEnd(wp);
                ret = 1;
                goto end;
            }
            if (!wp->commandMenu.attackAudience) {
                psndSFXOn("SFX_NOT_CHOICE1");
            }
            wp->commandMenu.attackAudience = 0;
        }
        if (cursor->absPos < cursor->count - 1) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos++;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_DOWN)) {
                cursor->absPos = 0;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        }
        if (cursor->absPos > 0) {
            if (BattlePadCheckRepeat(BATTLE_PAD_STICK_UP)) {
                cursor->absPos--;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        } else {
            if (BattlePadCheckTrigger(BATTLE_PAD_STICK_UP)) {
                cursor->absPos = cursor->count - 1;
                psndSFXOn("SFX_BTL_CURSOR_MOVE2");
                goto end;
            }
        }
        if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
            BattleCommandDisplay_ChangePartySelectMenuEnd(wp);
            ret = 1;
            cmd->state = 0x14;
        } else if (BattleCommandAttackAudienceCheck()) {
            wp->commandMenu.attackAudience = 1;
            BattleCommandDisplay_ChangePartySelectMenuEnd(wp);
        }
    }
end:
    BattleMenuDisp_ChangePartySelect_Main(wp);
    return ret;
}

void BattleCommandDisplay_ChangePartySelectMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_ChangePartySelect_Disp(param);
}

#pragma dont_inline on
void BattleCommandDisplay_ChangePartySelectMenuEnd(BattleWork* wp) {
    BattleMenuDisp_ChangePartySelect_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_CHANGE_PARTY;
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleCommandDisplay_TargetSelectMenuSetup(BattleWork* wp) {
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);

    wp->commandMenu.flags |= CMD_MENU_TARGET_SELECT;
    BtlUnit_SetAnimType(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), 0x28);
}
#pragma dont_inline reset


void BattleCommandDisplay_TargetSelectMenuDisp(CameraId camId, void* param) {
    BattleWork* wp = param;
    Vec icon;
    Vec pos1;
    Vec screen1;
    Vec pos2;
    Vec screen2;
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;
    BattleWorkTarget* t;
    BattleWorkWeaponTargets* wt;
    BattleUnit* unit;
    BattleUnitPart* part;
    const char* name;
    BattleWorkTarget* t2;
    BattleWorkWeaponTargets* wt2;
    s32 i;
    BattleUnit* unit2;
    BattleUnitPart* part2;

    _btlcmd_GetCursorPtr(cmd, wp->commandMenu.actType, &cursor);
    if (cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_SINGLE) {
        wt = &wp->weaponTargets;
        t = &wt->targets[wt->targetIndices[wt->currentTarget]];
        unit = BattleGetUnitPtr(wp, t->unitIdx);
        part = BtlUnit_GetPartsPtr(unit, t->partIdx);
        pos1.x = t->posX;
        pos1.y = t->posY;
        pos1.z = t->posZ;
        if (!(unit->mAttributeFlags & kUnitOnCeiling)) {
            btlGetScreenPoint(&pos1, &screen1);
            icon.x = screen1.x;
            icon.y = screen1.y;
            icon.z = 0.0f;
            iconDispGx(&icon, 0x10, ICON_TARGET_CURSOR, 1.0f);
            if (unit->currentKind == UNIT_YOSHI) {
                name = pouchGetYoshiName();
            } else {
                name = msgSearch(part->partName);
            }
            { f32 ww = FontGetMessageWidth(name); windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF}, -240.0f, 86.0f, 16.0f + ww, 32.0f, 10.0f); }
            FontDrawStart();
            FontDrawString(-232.0f, 83.0f, name);
        } else {
            btlGetScreenPoint(&pos1, &screen1);
            icon.x = screen1.x;
            icon.y = screen1.y;
            icon.z = 0.0f;
            iconDispGx(&icon, 0x18, ICON_TARGET_CURSOR, 1.0f);
            if (unit->currentKind == UNIT_YOSHI) {
                name = pouchGetYoshiName();
            } else {
                name = msgSearch(part->partName);
            }
            { f32 ww = FontGetMessageWidth(name); windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF}, -240.0f, 86.0f, 16.0f + ww, 32.0f, 10.0f); }
            FontDrawStart();
            FontDrawString(-232.0f, 83.0f, name);
        }
    } else {
        wt2 = &wp->weaponTargets;
        for (i = 0; i < wt2->numTargets; i++) {
            t2 = &wt2->targets[wt2->targetIndices[i]];
            unit2 = BattleGetUnitPtr(wp, t2->unitIdx);
            part2 = BtlUnit_GetPartsPtr(unit2, t2->partIdx);
            pos2.x = t2->posX;
            pos2.y = t2->posY;
            pos2.z = t2->posZ;
            if (!(unit2->mAttributeFlags & kUnitOnCeiling)) {
                btlGetScreenPoint(&pos2, &screen2);
                icon.x = screen2.x;
                icon.y = screen2.y;
                icon.z = 0.0f;
                iconDispGx(&icon, 0x10, ICON_TARGET_CURSOR, 1.0f);
                if (unit2->currentKind == UNIT_YOSHI) {
                    name = pouchGetYoshiName();
                } else {
                    name = msgSearch(part2->partName);
                }
                { f32 ww = FontGetMessageWidth(name); windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF}, -240.0f, 118.0f - (f32)(i * 32), 16.0f + ww, 32.0f, 10.0f); }
                FontDrawStart();
                FontDrawString(-232.0f, 115.0f - (f32)(i * 32), name);
            } else {
                btlGetScreenPoint(&pos2, &screen2);
                icon.x = screen2.x;
                icon.y = screen2.y;
                icon.z = 0.0f;
                iconDispGx(&icon, 0x18, ICON_TARGET_CURSOR, 1.0f);
                if (unit2->currentKind == UNIT_YOSHI) {
                    name = pouchGetYoshiName();
                } else {
                    name = msgSearch(part2->partName);
                }
                { f32 ww = FontGetMessageWidth(name); windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF}, -240.0f, 118.0f - (f32)(i * 32), 16.0f + ww, 32.0f, 10.0f); }
                FontDrawStart();
                FontDrawString(-232.0f, 115.0f - (f32)(i * 32), name);
            }
        }
    }
    btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT);
}

#pragma dont_inline on
void BattleCommandDisplay_TargetSelectMenuEnd(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;

    _btlcmd_GetCursorPtr(cmd, wp->commandMenu.actType, &cursor);
    btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT);
    cmd->flags &= ~CMD_MENU_TARGET_SELECT;
    cmd->flags &= ~CMD_MENU_ENEMY_HP;
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleCommandDisplay_ProtectPartnerSelectMenuSetup(BattleWork* wp) {
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    BattleGetPartnerPtr(wp, unit);
    wp->commandMenu.flags |= CMD_MENU_PROTECT_PARTNER;
    wp->commandMenu.flags &= ~CMD_MENU_ENEMY_HP;
}
#pragma dont_inline reset

void BattleCommandDisplay_ProtectPartnerSelectMenuDisp(CameraId camId, void* param) {
    BattleWork* wp = param;
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    Vec icon;
    Vec pos;
    Vec screen;
    f32 len;
    f32 w;

    unit = BattleGetPartnerPtr(wp, unit);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.y += 60.0f;
    btlGetScreenPoint(&pos, &screen);
    icon.x = screen.x;
    icon.y = screen.y;
    icon.z = 0.0f;
    iconDispGx(&icon, 0x10, ICON_TARGET_CURSOR, 1.0f);
    len = strlen(msgSearch(unit->mKindParams->name));
    w = 6.25f * len;
    windowDispGX_Waku_col(0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF}, icon.x - w, 80.0f + icon.y, 12.5f * len, 32.0f, 10.0f);
    FontDrawStart();
    FontDrawString(3.0f + icon.x - w, 77.0f + icon.y, msgSearch(unit->mKindParams->name));
}

#pragma dont_inline on
void BattleCommandDisplay_ProtectPartnerSelectMenuEnd(BattleWork* wp) {
    wp->commandMenu.flags &= ~CMD_MENU_PROTECT_PARTNER;
}
#pragma dont_inline reset

#pragma dont_inline on
void BattleCommandDisplay_AllEnd(BattleWork* wp) {
    wp->commandMenu.flags &= ~(CMD_MENU_ACT | CMD_MENU_WEAPON_SELECT | CMD_MENU_OPERATION | CMD_MENU_CHANGE_PARTY | CMD_MENU_TARGET_SELECT | CMD_MENU_PROTECT_PARTNER);
}
#pragma dont_inline reset

s32 _check_weapon_type_attack(BattleWeapon* weapon);
#define UEVT(u) ((u)->attackEventCode)
#define UEVTMODE(u) ((u)->attackEventFlags)
#define WEVT(w) ((w)->mAttackEvtScript)

void _btlcmd_SetAttackEvent(BattleUnit* unit, BattleCommand* cmd) {
    BattleWork* wp = _battleWorkPointer;
    u32 type = cmd->actType;
    BattleCursor* cursor;
    BattleWeapon* weapon;

    if (cmd->cancelled) {
        unit->moveState = 0;
        UEVT(unit) = btldefaultevt_Wait;
        unit->attackEventFlags = 1;
        return;
    }
    switch (type) {
    case 5:
        switch ((u32)cmd->operationType) {
        case 6:
            unit->attackEventFlags = 1;
            if (unit->currentKind == UNIT_MARIO) {
                UEVT(unit) = btldefaultevt_Defend;
            } else {
                UEVT(unit) = btldefaultevt_DefendParty;
            }
            break;
        case 5:
            UEVT(unit) = btldefaultevt_Appeal;
            unit->attackEventFlags = 1;
            if (unit->currentKind == UNIT_MARIO) {
                BtlActRec_AddCount(&wp->actRecord.marioAppeals);
            } else {
                BtlActRec_AddCount(&wp->actRecord.partnerAppeals);
            }
            break;
        case 7:
            UEVT(unit) = btldefaultevt_Wait;
            unit->attackEventFlags = 1;
            if (unit->currentKind == UNIT_MARIO) {
                BtlActRec_AddCount(&wp->actRecord.marioAppeals);
            } else {
                BtlActRec_AddCount(&wp->actRecord.partnerAppeals);
            }
            break;
        case 3:
            UEVT(unit) = btldefaultevt_Escape;
            unit->attackEventFlags = 1;
            if (unit->currentKind == UNIT_MARIO) {
                BtlActRec_AddCount(&wp->actRecord.marioEscapes);
            } else {
                BtlActRec_AddCount(&wp->actRecord.partnerEscapes);
            }
            break;
        case 1:
            unit->attackEventFlags = 0;
            if (unit->currentKind == UNIT_MARIO) {
                UEVT(unit) = WEVT(&badgeWeapon_Charge);
                BtlActRec_AddCount(&wp->actRecord.marioCharges);
            } else {
                UEVT(unit) = WEVT(&badgeWeapon_ChargeP);
                BtlActRec_AddCount(&wp->actRecord.partnerCharges);
            }
            break;
        case 2:
            unit->attackEventFlags = 0;
            if (unit->currentKind == UNIT_MARIO) {
                UEVT(unit) = WEVT(&badgeWeapon_SuperCharge);
                BtlActRec_AddCount(&wp->actRecord.marioSuperCharges);
            } else {
                UEVT(unit) = WEVT(&badgeWeapon_SuperChargeP);
                BtlActRec_AddCount(&wp->actRecord.partnerSuperCharges);
            }
            break;
        case 0:
            UEVT(unit) = btldefaultevt_ChangeParty;
            unit->attackEventFlags = 1;
            cmd->cursor[1].absPos = 0;
            if (wp->badgeEquippedFlags & 1) {
                unit->moveState = 3;
            }
            if (unit->currentKind == UNIT_MARIO) {
                BtlActRec_AddCount(&wp->actRecord.marioPartyChanges);
            } else {
                BtlActRec_AddCount(&wp->actRecord.partnerPartyChanges);
            }
            break;
        case 4:
            UEVT(unit) = btldefaultevt_ProtectPartner;
            unit->attackEventFlags = 1;
            break;
        case 8:
            UEVT(unit) = BtlUnit_GetData(unit, UNIT_DATA_WAKEUP_EVENT);
            unit->attackEventFlags = 3;
            break;
        default:
            UEVT(unit) = btldefaultevt_Wait;
            unit->attackEventFlags = 1;
            break;
        }
        break;
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
        _btlcmd_GetCursorPtr(cmd, type, &cursor);
        weapon = cmd->weapon[cursor->absPos].weapon;
        UEVT(unit) = WEVT(weapon);
        unit->attackEventFlags = 0;
        if (!UEVT(unit)) {
            UEVT(unit) = btldefaultevt_Wait;
        }
        switch (type) {
        case 0:
            BtlActRec_AddCount(&wp->actRecord.marioJumpMoves);
            break;
        case 1:
            BtlActRec_AddCount(&wp->actRecord.marioHammerMoves);
            break;
        case 4:
            if (_check_weapon_type_attack(weapon)) {
                BtlActRec_AddCount(&wp->actRecord.marioSpecialAttacks);
            } else {
                BtlActRec_AddCount(&wp->actRecord.marioSpecialSupports);
            }
            break;
        case 2:
            if ((EvtWord)weapon == (EvtWord)&ItemWeaponData_Naniga_Okorukana) break; // FAKE: integer compare gives the target's signed cmpw
            if (unit->currentKind == UNIT_MARIO) {
                if (_check_weapon_type_attack(weapon)) {
                    BtlActRec_AddCount(&wp->actRecord.marioAttackItems);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.marioSupportItems);
                }
            } else {
                if (_check_weapon_type_attack(weapon)) {
                    BtlActRec_AddCount(&wp->actRecord.partnerAttackItems);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.partnerSupportItems);
                }
            }
            break;
        case 6:
            if (_check_weapon_type_attack(weapon)) {
                BtlActRec_AddCount(&wp->actRecord.partnerAttackMoves);
            } else {
                BtlActRec_AddCount(&wp->actRecord.partnerSupportMoves);
            }
            break;
        }
        break;
    case 9:
        unit->moveState = 1;
        UEVT(unit) = btldefaultevt_ChangePosition;
        unit->attackEventFlags = 7;
        break;
    case 11:
        switch (unit->moveState) {
        case 6: unit->moveState = 5; break;
        case 7: unit->moveState = 6; break;
        case 9: unit->moveState = 8; break;
        default: unit->moveState = 4; break;
        }
        UEVT(unit) = btlataudGetAttackEventPtr(unit->unitId);
        unit->attackEventFlags = 3;
        if (unit->currentKind == UNIT_MARIO) {
            BtlActRec_AddCount(&wp->actRecord.marioAudienceAttacks);
        } else {
            BtlActRec_AddCount(&wp->actRecord.partnerAudienceAttacks);
        }
        if (wp->lectureFlags & 1) {
            msgWindow_Continue(wp->lectureWindowId);
        }
        break;
    case 12:
        switch (unit->moveState) {
        case 6: unit->moveState = 5; break;
        case 7: unit->moveState = 6; break;
        case 9: unit->moveState = 8; break;
        default: unit->moveState = 4; break;
        }
        UEVT(unit) = btlataudGetMoveEventPtr(unit->unitId);
        unit->attackEventFlags = 3;
        break;
    case 13:
        UEVT(unit) = btldefaultevt_LectureEnd;
        unit->attackEventFlags = 3;
        break;
    default:
        UEVT(unit) = btldefaultevt_Wait;
        unit->attackEventFlags = 0;
        break;
    }
}

BattleWeapon* BattleGetSelectWeapon(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;

    _btlcmd_GetCursorPtr(cmd, wp->commandMenu.actType, &cursor);
    return cmd->weapon[cursor->absPos].weapon;
}


s32 _check_present_item(void) {
    BattleWork* wp = _battleWorkPointer;
    s32 item = BattleAudience_GetPresentItemNo();
    s32 id;
    BattleUnit* unit;
    Vec pos;

    if (item >= 0x153) return 0;
    id = BattleAudience_GetPresentTargetUnitId();
    if (id < -1 || id >= 64) return 0;
    unit = BattleGetUnitPtr(wp, id);
    switch (item) {
    case 0:
        return 0;
    case 0x7b:
        BtlUnit_RecoverHp(unit, 1);
        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
        pos.y += unit->mSizeMultiplier * BtlUnit_GetHeight(unit) + 5.0f;
        effRecoveryEntry(0, 1, pos.x, pos.y, pos.z);
        BattleAudience_SetPresentItemNo(0);
        return 0;
    case 0x7c:
        BtlUnit_RecoverFp(unit, 1);
        BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
        pos.y += unit->mSizeMultiplier * BtlUnit_GetHeight(unit) + 5.0f;
        effRecoveryEntry(1, 1, pos.x, pos.y, pos.z);
        BattleAudience_SetPresentItemNo(0);
        return 0;
    default:
        if (item < 0 || item >= 0x153) return 0;
        wp->commandMenu.attackAudience = 1;
        if (wp->commandMenu.state == 12 || wp->commandMenu.state == 13) {
            wp->commandMenu.attackAudience = 2;
        }
        BattleCommandDisplay_AllEnd(wp);
        wp->commandMenu.prevActType = wp->commandMenu.actType;
        wp->commandMenu.actType = 12;
        wp->commandMenu.state = 0x1d;
        return 1;
    }
}

void BattleDrawEnemyHPBar(Vec pos, BattleUnit* unit, GXColor color);

void BattleDrawEnemyHP(CameraId camId, void* param) {
    BattleWork* wp = param;
    GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
    int i;
    BattleUnit* unit;
    Vec position;
    Vec screen;
    Mtx mtx;
    s32 ofs;

    camGetPtr(CAMERA_2D);

    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (!unit) continue;
        if (BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) continue;
        if (!battleCheckUnitMonosiriFlag(wp->units[i])) continue;
        if (unit->mAttributeFlags & kUnitNoHpGauge) continue;
        if (!(wp->commandMenu.flags & CMD_MENU_ENEMY_HP) && !(unit->flags & BATTLE_UNIT_FLAG_SHOW_HP)) continue;
        position.x = unit->mPosition.x;
        position.y = unit->mPosition.y;
        position.z = unit->mPosition.z;
        if (wp->flags & BATTLE_FLAG_COMMAND_INPUT) {
            position.x = unit->mHomePosition.x;
            position.y = unit->mHomePosition.y;
            position.z = unit->mHomePosition.z;
        }
        position.x += (f32)unit->mHpGaugeOffset[0];
        position.y += (f32)unit->mHpGaugeOffset[1];
        btlGetScreenPoint(&position, &screen);
        if ((unit->flags & BATTLE_UNIT_FLAG_SHOW_HP) && (unit->hpGauge.flags & 1) && unit->hpGauge.visibleTimer < 20) {
            color.a = intplGetValue(EASING_LINEAR, 0.0f, 255.0f, unit->hpGauge.visibleTimer, 20);
        }
        ofs = 20;
        if (wp->units[i]->currentHp >= 100) {
            ofs = 12;
        } else if (wp->units[i]->currentHp >= 10) {
            ofs = 16;
        }
        PSMTXTrans(mtx, screen.x + (f32)ofs, screen.y - 32.0f, 0.0f);
        iconNumberDispGx(mtx, wp->units[i]->currentHp, 1, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
        BattleDrawEnemyHPBar(screen, unit, color);
    }
}


#pragma dont_inline on
void BattleDrawEnemyHPBar(Vec pos, BattleUnit* unit, GXColor color) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    f32 x, y;

    btlDispGXInit2DRasta();
    GXLoadPosMtxImm(cam->view, 0);
    pos.x -= 10.0f;
    btlDispTexPlane(19.0f + pos.x, pos.y - 3.0f, 0.0f, 1.0f, 1.0f, VERSION_PICK(10, 17), color, 0);
    switch (unit->hpGauge.mode) {
    case 1:
        if (unit->hpGauge.drainTimer > 0) {
            btlDispGXInit2DRasta();
            GXLoadPosMtxImm(cam->view, 0);
            y = 2.0f + pos.y;
            x = 1.0f + pos.x;
            btlDispGXQuads2DRasta(x, y, x + 34.0f * unit->hpGauge.trailRatio, y - 8.0f, 0, 0, 0, color.a);
            btlDispGXInit2DRasta();
            GXLoadPosMtxImm(cam->view, 0);
        }
        btlDispTexPlane(19.0f + pos.x - 20.0f * (1.0f - unit->hpGauge.barRatio), pos.y - 3.0f, 0.0f, unit->hpGauge.barRatio, 1.0f, VERSION_PICK(11, 18), color, 0);
        break;
    default:
        x = unit->hpGauge.barRatio;
        btlDispTexPlane(19.0f + pos.x - 20.0f * (1.0f - x), pos.y - 3.0f, 0.0f, x, 1.0f, VERSION_PICK(11, 18), color, 0);
        break;
    }
}
#pragma dont_inline reset

void* BattleSetConfuseAct(BattleWork* wp, BattleUnit* unit) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;
    s16 list[24];
    BattleWeapon* weapon;

    if (unit->moveState == 3) {
        unit->moveState = 0;
    }
    BattleAcHelpSetDispType(0);
    if (unit->mAlliance) {
        return subsetevt_confuse_flustered;
    }
    if (unit->currentKind != UNIT_MARIO && (unit->currentKind < TYPE_PARTNER_MIN || unit->currentKind >= TYPE_PARTNER_MAX)) {
        return subsetevt_confuse_flustered;
    }
#ifdef VERSION_G8ME01
    if (cmd->dipMode) {
        unit->moveState = 0;
        cmd->dipMode = 0;
    }
#endif
    _btlcmd_MakeActClassTable(wp);
    {
    u8 cnt;
    u8 i;
    _btlcmd_GetCursorPtr(cmd, 14, &cursor);
    cnt = 0;
    for (i = 0; i < cursor->count; i++) {
        if (cmd->act[i].enabled && cmd->act[i].type != 4 && cmd->act[i].type != 2) {
            list[cnt++] = i;
        }
    }
    list[cnt] = -1;
    if (cnt) {
        cursor->absPos = list[irand(cnt)];
    } else {
        return subsetevt_confuse_flustered;
    }
    }
    cmd->actType = cmd->act[cursor->absPos].type;
    if (!cmd->act[cursor->absPos].enabled) {
        return subsetevt_confuse_flustered;
    }
    switch (cmd->actType) {
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
        _btlcmd_MakeSelectWeaponTable(wp, cmd->actType);
        {
        u8 cnt;
        u8 i;
        _btlcmd_GetCursorPtr(cmd, cmd->actType, &cursor);
        cnt = 0;
        for (i = 0; i < cursor->count; i++) {
            weapon = cmd->weapon[i].weapon;
            if (cmd->weapon[i].enabled && ((u32)weapon->special & SPECIAL_CONFUSED_ACTION) &&
                !(weapon->targetClassFlags & TARGET_CLASS_MULTIPLE) && (weapon->targetClassFlags & TARGET_CLASS_SINGLE)) {
                list[cnt++] = i;
            }
        }
        list[cnt] = -1;
        if (cnt) {
            cursor->absPos = list[irand(cnt)];
        } else {
            return NULL;
        }
        }
        if (!cmd->weapon[cursor->absPos].enabled) {
            return subsetevt_confuse_flustered;
        }
        _btlcmd_SelectWeaponDecide(wp, cmd->actType, cursor->absPos);
        weapon = cmd->weapon[cursor->absPos].weapon;
        if (!((u32)weapon->special & SPECIAL_CONFUSED_ACTION)) {
            return subsetevt_confuse_flustered;
        }
        if ((weapon->targetClassFlags & TARGET_CLASS_MULTIPLE) || !(weapon->targetClassFlags & TARGET_CLASS_SINGLE)) {
            return subsetevt_confuse_flustered;
        }
        if (wp->weaponTargets.numTargets <= 0) {
            return subsetevt_confuse_flustered;
        }
        wp->weaponTargets.currentTarget = irand(wp->weaponTargets.numTargets);
        wp->flags |= BATTLE_FLAG_ACT_DECIDED_UNUSED;
        return WEVT(weapon);
    case 5:
        _btlcmd_MakeOperationTable(wp);
        {
        u8 cnt;
        u8 i;
        _btlcmd_GetCursorPtr(cmd, 5, &cursor);
        cnt = 0;
        for (i = 0; i < cursor->count; i++) {
            if (cmd->operation[i].enabled) {
                if (cmd->operation[i].type == 0 || cmd->operation[i].type == 6 || cmd->operation[i].type == 3) {
                    list[cnt++] = i;
                }
            }
        }
        list[cnt] = -1;
        if (cnt) {
            cursor->absPos = list[irand(cnt)];
        } else {
            return subsetevt_confuse_flustered;
        }
        }
        if (!cmd->operation[cursor->absPos].enabled) {
            return subsetevt_confuse_flustered;
        }
        cmd->operationType = cmd->operation[cursor->absPos].type;
        switch ((u32)cmd->operationType) {
        case 6:
            if (unit->currentKind == UNIT_MARIO) {
                return btldefaultevt_Defend;
            }
            return btldefaultevt_DefendParty;
        case 5:
            return btldefaultevt_Appeal;
        case 7:
            return btldefaultevt_Wait;
        case 3:
            return btldefaultevt_Escape;
        case 1:
            if (unit->currentKind == UNIT_MARIO) {
                return WEVT(&badgeWeapon_Charge);
            }
            return WEVT(&badgeWeapon_ChargeP);
        case 2:
            if (unit->currentKind == UNIT_MARIO) {
                return WEVT(&badgeWeapon_SuperCharge);
            }
            return WEVT(&badgeWeapon_SuperChargeP);
        case 8:
            return BtlUnit_GetData(unit, UNIT_DATA_WAKEUP_EVENT);
        case 0: {
            u8 cnt;
            u8 i;
            _btlcmd_GetCursorPtr(cmd, 10, &cursor);
            cnt = 0;
            for (i = 0; i < cursor->count; i++) {
                if (cmd->party[i].enabled) {
                    list[cnt++] = i;
                }
            }
            list[cnt] = -1;
            if (cnt) {
                cursor->absPos = list[irand(cnt)];
            } else {
                return NULL;
            }
            }
            if (!cmd->party[cursor->absPos].enabled) {
                return subsetevt_confuse_flustered;
            }
            cmd->partyKind = cmd->party[cursor->absPos].kind;
            cmd->cursor[1].absPos = 0;
            return btldefaultevt_ChangeParty;
        case 4:
            return subsetevt_confuse_flustered;
        default:
            return subsetevt_confuse_flustered;
        }
    default:
        return subsetevt_confuse_flustered;
    }
}

#pragma dont_inline on
void _commandRestoreRec(void) {
    BattleWork* wp = _battleWorkPointer;

    if (!wp->commandMenu.attackAudience) {
        wp->commandMenu.recCommand = wp->commandMenu.state;
        return;
    }
    if (wp->commandMenu.recCommand == wp->commandMenu.state) {
        wp->commandMenu.recCommand = 0;
        wp->commandMenu.attackAudience = 0;
    }
}
#pragma dont_inline reset


#pragma dont_inline on
BOOL BattleCommandAttackAudienceCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;

    if (BattlePadCheckTrigger(PAD_BUTTON_X) && !(wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED)) {
        if (BattleGetMarioPtr(wp)->mAttributeFlags & kUnitInVeil) {
            return FALSE;
        }
        BattleAudience_GetItemOn(&id, NULL, NULL, NULL, NULL);
        if (id != -1) {
            BattleAudience_SetTarget(id);
            BattleCommandDisplay_ActMenuEnd(wp);
            wp->commandMenu.prevActType = wp->commandMenu.actType;
            wp->commandMenu.actType = 11;
            wp->commandMenu.state = 0x1d;
            BattleStageOffLightInCommand(0, 1);
            wp->flags |= BATTLE_FLAG_ACT_DECIDED_UNUSED;
            return TRUE;
        }
    }
    return FALSE;
}
#pragma dont_inline reset

USER_FUNC(_get_msg) {
    s32 index = event->args[0];
    BattleWork* wp = _battleWorkPointer;
    const char* msg;

    switch (wp->commandMenu.disableReason) {
        case 1:
            msg = "btl_msg_cmderror_no_fp";
            // FAKE: empty if that only reads event->args (twice); without it case 1 is scheduled differently
            if (!event->args && !event->args) {}
            break;
        case 2:
            msg = "btl_msg_cmderror_no_ap";
            break;
        case 4:
            msg = "btl_msg_cmderror_no_jump_target";
            break;
        case 5:
            msg = "btl_msg_cmderror_no_hammer_target";
            break;
        case 6:
            msg = "btl_msg_cmderror_no_escape";
            break;
        default:
            msg = "btl_msg_cmderror_no_select_now";
            break;
    }
    evtSetValue(event, index, (EvtWord)msg);
    return 2;
}

s32 _check_weapon_type_attack(BattleWeapon* weapon) {
    if (!weapon) return 0;
    if (weapon->hpDamageFn) return 1;
    if ((s8)weapon->sleepChance > 0 && weapon->sleepTime > 0) return 1;
    if ((s8)weapon->stopChance > 0 && weapon->stopTime > 0) return 1;
    if ((s8)weapon->dizzyChance > 0 && weapon->dizzyTime > 0) return 1;
    if ((s8)weapon->poisonChance > 0 && weapon->poisonTime > 0) return 1;
    if ((s8)weapon->confuseChance > 0 && weapon->confuseTime > 0) return 1;
    if ((s8)weapon->burnChance > 0 && weapon->burnTime > 0) return 1;
    if ((s8)weapon->freezeChance > 0 && weapon->freezeTime > 0) return 1;
    if ((s8)weapon->sizeChangeChance > 0 && weapon->sizeChangeTime > 0 && weapon->sizeChangeStrength < 0) return 1;
    if ((s8)weapon->atkChangeChance > 0 && weapon->atkChangeTime > 0 && weapon->atkChangeStrength < 0) return 1;
    if ((s8)weapon->defChangeChance > 0 && weapon->defChangeTime > 0 && weapon->defChangeStrength < 0) return 1;
    if (weapon->ohkoChance > 0) return 1;
    if ((s8)weapon->frightChance > 0) return 1;
    if ((s8)weapon->galeForceChance > 0) return 1;
    if ((s8)weapon->slowChance > 0 && weapon->slowTime > 0) return 1;
    return 0;
}

