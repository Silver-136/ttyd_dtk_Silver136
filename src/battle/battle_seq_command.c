// Battle command menus: building and handling the action, weapon, item and target selection menus.

#include "version.h"
#include "battle/battle_seq_command.h"

#define CMD_DISABLE_NONE (0)
#define CMD_DISABLE_NO_FP (1)
#define CMD_DISABLE_NO_SP (2)
#define CMD_DISABLE_NOT_SELECTABLE (3)
#define CMD_DISABLE_NO_JUMP_TARGET (4)
#define CMD_DISABLE_NO_HAMMER_TARGET (5)
#define CMD_DISABLE_NO_ESCAPE (6)
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

#define BATTLE_COMMAND_UNIT_CAPACITY ((s32)(sizeof(((BattleWork*)0)->units) / sizeof(((BattleWork*)0)->units[0])))
#define BATTLE_COMMAND_WEAPON_CAPACITY ((s32)(sizeof(((BattleCommand*)0)->weapon) / sizeof(((BattleCommand*)0)->weapon[0])))
#define BATTLE_COMMAND_VISIBLE_WEAPON_ROWS (6)

void BattleCommandDisplay_ActMenuEnd(BattleWork* wp);
unsigned long strlen(const char* s);
static BOOL BattleCommandAttackAudienceCheck(void);
void BattleCommandDisplay_WeaponMultiItemCancelCheckDisp(CameraId camId, void* param);
static USER_FUNC(_get_msg);
void BattleCommandDisplay_ActMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_WeaponSelectMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_OperationMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_ChangePartySelectMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_MultiItemMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_TargetSelectMenuDisp(CameraId camId, void* param);
void BattleCommandDisplay_ProtectPartnerSelectMenuDisp(CameraId camId, void* param);

// local prototypes
void BattleDrawEnemyHP(CameraId camId, void* param);
static s32 _btlcmd_CheckWeaponTargetNum(BattleWork* wp, BattleUnit* unit, BattleWeapon* weapon);

typedef struct BattleCheckPartyEntry {
    s32 kind;          // 0x0
    const char* name;  // 0x4
    u16 icon;          // 0x8
    const char* help;  // 0xC
} BattleCheckPartyEntry;

static u16 _battleGetPartyIcon(s32 kind);
static s32 _btlcmd_MakePartyTable(BattleWork* wp);
static void _btlcmd_UpdateSelectWeaponTable(BattleWork* wp, s32 type);

/** Jump badges listed in Mario's Jump menu, in menu order, when equipped (_btlcmd_MakeSelectWeaponTable). */
static s32 _jump_badge_table[8] = {ITEM_POWER_JUMP, ITEM_MULTIBOUNCE, ITEM_SHRINK_STOMP, ITEM_SLEEPY_STOMP, ITEM_SOFT_STOMP, ITEM_TORNADO_JUMP, ITEM_POWER_BOUNCE, ITEM_MEGA_JUMP};

/** Hammer badges listed in Mario's Hammer menu, in menu order, when equipped (_btlcmd_MakeSelectWeaponTable). */
static s32 _hammer_badge_table[9] = {ITEM_POWER_SMASH, ITEM_PIERCING_BLOW, ITEM_HAMMER_THROW, ITEM_HEAD_RATTLE, ITEM_ICE_SMASH, ITEM_FIRE_DRIVE, ITEM_QUAKE_HAMMER, ITEM_MEGA_SMASH, ITEM_MEGA_QUAKE};

/**
 * Started by BattleCommandInput when the player picks a greyed-out command:
 * gets the reason message from _get_msg and announces it for 60 frames; the menu reopens when it ends.
 */
#if VERSION >= VERSION_JP
static EVT_BEGIN(_btl_cmd_error_msg_event)
    CALL(_get_msg, LW(0))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -30, LW(0), 60)
    RETURN
EVT_END()
#endif

void BattleCommandInit(BattleWork* wp) {
    wp->commandMenu.state = CMD_STATE_INIT;
    wp->flags |= BATTLE_FLAG_COMMAND_INPUT;
}

/**
 * Builds the action menu for the acting unit in cmd->act:
 * Jump, the partner's moves, Hammer, Items, Special (once a Star Power is obtained), Tactics,
 * or the tutorial's end-lecture entry.
 * Greys entries out for Jumpman/Hammerman, the battle's no-jump/no-hammer flags, the command-disable counters,
 * being veiled by Vivian or flipped, and the tutorial (lecture) locks.
 * The Jump and Hammer icons follow the boots and hammer level.
 */
static void _btlcmd_MakeActClassTable(BattleWork* wp) {
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

    // cmd = &wp->commandMenu is never NULL, but the test is in the retail code
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
        cmd->act[0].type = CMD_ACT_JUMP;
        cmd->act[0].enabled = 1;
        cmd->act[0].disableReason = CMD_DISABLE_NONE;
        if (unit->attributeFlags & kVeiled) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (pouch->jumpLevel < 1) {
            cmd->act[0].enabled = 0;
        }
        if (unit->badgesEquipped.Hammerman) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (wp->commandDisabled[0] > 0) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (info->battleFlags & NPC_BATTLE_NO_JUMP) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_SPECIAL)) {
            cmd->act[0].enabled = 0;
            cmd->act[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
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
        cmd->act[num].type = CMD_ACT_PARTY_MOVE;
        if (unit->flippedTurns) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        } else {
            cmd->act[num].enabled = 1;
            cmd->act[num].disableReason = CMD_DISABLE_NONE;
        }
        if (VERSION_PICK(wp->commandDisabled[3] > 0, unit->usCommandDisabled[0] != 0)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_SPECIAL)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_tech_party");
        cmd->act[num].icon = ICON_BTL_CMD_TECH_PARTY;
        num++;
    }
    if (unit->currentKind == UNIT_MARIO && pouch->hammerLevel >= 1) {
        cmd->act[num].type = CMD_ACT_HAMMER;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = CMD_DISABLE_NONE;
        if (unit->attributeFlags & kVeiled) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (pouch->hammerLevel < 1) {
            cmd->act[num].enabled = 0;
        }
        if (unit->badgesEquipped.Jumpman) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (wp->commandDisabled[1] > 0) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (info->battleFlags & NPC_BATTLE_NO_HAMMER) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_SPECIAL)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
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
        cmd->act[num].type = CMD_ACT_ITEM;
        if (unit->flippedTurns) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        } else {
            cmd->act[num].enabled = 1;
            cmd->act[num].disableReason = CMD_DISABLE_NONE;
        }
        if (unit->currentKind == UNIT_MARIO) {
            if (wp->commandDisabled[2] > 0) {
                cmd->act[num].enabled = 0;
                cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
            }
        } else {
            if (VERSION_PICK(wp->commandDisabled[4] > 0, unit->usCommandDisabled[1] != 0)) {
                cmd->act[num].enabled = 0;
                cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
            }
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_SPECIAL | BATTLE_FLAG_LECTURE_FREE | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_item");
        cmd->act[num].icon = ICON_BTL_CMD_ITEM;
        num++;
    }
    if (unit->currentKind == UNIT_MARIO && pouch->starPowersObtained != 0) {
        cmd->act[num].type = CMD_ACT_SPECIAL;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = CMD_DISABLE_NONE;
        if (unit->attributeFlags & kVeiled) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_FREE | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_super_action");
        cmd->act[num].icon = ICON_BTL_CMD_SUPER_ACTION;
        num++;
    }
    if (!(wp->flags & BATTLE_FLAG_LECTURE_FREE)) {
        cmd->act[num].type = CMD_ACT_OPERATION;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = CMD_DISABLE_NONE;
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_SPECIAL | BATTLE_FLAG_LECTURE_FREE | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        cmd->act[num].name = msgSearch("btl_cmd_act_operation");
        cmd->act[num].icon = ICON_BTL_CMD_OPERATION;
        num++;
    }
    if (wp->flags & BATTLE_FLAG_LECTURE_FREE) {
        cmd->act[num].type = CMD_ACT_LECTURE_END;
        cmd->act[num].enabled = 1;
        cmd->act[num].disableReason = CMD_DISABLE_NONE;
        cmd->act[num].name = msgSearch("btl_cmd_act_lecture_end");
        cmd->act[num].icon = ICON_BTL_CMD_OPERATION;
        if (wp->flags & (BATTLE_FLAG_LECTURE_NO_COMMAND | BATTLE_FLAG_LECTURE_ATTACK)) {
            cmd->act[num].enabled = 0;
            cmd->act[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        num++;
    }
    cursor->count = num;
    if (cursor->absPos >= cursor->count) {
        cursor->absPos = cursor->count - 1;
    }
}

/**
 * Sets *out to the cursor that remembers the position in a menu:
 * 14 the action menu (one each for Mario and the partner), 0 jumps, 1 hammers, 2 items,
 * 3 the Double/Triple Dip menu, 4 Specials, 5 Tactics (one each for Mario and the partner),
 * 6 the partner's moves, 7 and 8 two more weapon lists, 10 the partner list. Other types leave *out unchanged.
 */
static void _btlcmd_GetCursorPtr(BattleCommand* cmd, u32 type, BattleCursor** out) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, _battleWorkPointer->activeUnitId);

    switch (type) {
        case CMD_ACT_MENU:
            if (unit->currentKind == UNIT_MARIO) {
                *out = &cmd->cursor[0];
            } else {
                *out = &cmd->cursor[1];
            }
            break;
        case CMD_ACT_JUMP:
            *out = &cmd->cursor[2];
            break;
        case CMD_ACT_HAMMER:
            *out = &cmd->cursor[3];
            break;
        case CMD_ACT_ITEM:
            *out = &cmd->cursor[11];
            break;
        case CMD_ACT_MULTI_ITEM:
            *out = &cmd->cursor[12];
            break;
        case CMD_ACT_SPECIAL:
            *out = &cmd->cursor[7];
            break;
        case CMD_ACT_OPERATION:
            if (unit->currentKind == UNIT_MARIO) {
                *out = &cmd->cursor[8];
            } else {
                *out = &cmd->cursor[9];
            }
            break;
        case CMD_ACT_PARTY_MOVE:
            *out = &cmd->cursor[4];
            break;
        case 7:
            *out = &cmd->cursor[5];
            break;
        case 8:
            *out = &cmd->cursor[6];
            break;
        case CMD_ACT_PARTY:
            *out = &cmd->cursor[10];
            break;
    }
}

/** Icon of the jump moves in the weapon list: normal, Super or Ultra Boots by the pouch jumpLevel. */
static s32 _getJumpIconId(void) {
    switch (pouchGetPtr()->jumpLevel) {
        case 3:
            return ICON_ITEM_ULTRA_BOOTS;
        case 2:
            return ICON_ITEM_SUPER_BOOTS;
        default:
            return ICON_ITEM_BOOTS;
    }
}

/** Icon of the hammer moves in the weapon list: normal, Super or Ultra Hammer by the pouch hammerLevel. */
static s32 _getHammerIconId(void) {
    switch (pouchGetPtr()->hammerLevel) {
        case 3:
            return ICON_ITEM_ULTRA_HAMMER;
        case 2:
            return ICON_ITEM_SUPER_HAMMER;
        default:
            return ICON_ITEM_HAMMER;
    }
}

/**
 * Fills cmd->weapon with the list for menu type `type` and returns its length:
 * 0 Jump (plus Spin Jump and Spring Jump with the Super and Ultra Boots) and the equipped jump badges;
 * 1 Hammer (plus Super and Ultra Hammer) and the equipped hammer badges;
 * 2 the battle-usable items Mario carries (cooking items without battle data use ItemWeaponData_CookingItem;
 * a 'no items' dummy when empty); 4 the obtained Special moves; 6 the partner's moves from the unit's data hook.
 * In the tutorial the basic Jump or Hammer is swapped for the lecture unit's weapon.
 * Then marks which entries are usable.
 */
static s32 _btlcmd_MakeSelectWeaponTable(BattleWork* wp, s32 type) {
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

    // cmd = &wp->commandMenu is never NULL, but the test is in the retail code
    if (cmd == NULL) {
        return 0;
    }
    _btlcmd_GetCursorPtr(cmd, type, &cursor);
    num = 0;
    switch (type) {
        case CMD_ACT_JUMP:
            if (pouch->jumpLevel >= 1) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = ITEM_NULL;
                cmd->weapon[num].weapon = &marioDefaultWeapon_Jump;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
                cmd->weapon[num].icon = _getJumpIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                if (wp->flags & (BATTLE_FLAG_LECTURE_JUMP | BATTLE_FLAG_LECTURE_FREE) && wp->lectureUnitId != -1) {
#if VERSION <= VERSION_DEMO_JP
                    cmd->weapon[num].weapon = BtlUnit_GetData(BattleGetUnitPtr(wp, wp->lectureUnitId), 0x43);
#else
                    cmd->weapon[num].weapon = BtlUnit_GetData(BattleGetUnitPtr(wp, wp->lectureUnitId), UNIT_DATA_LECTURE_JUMP_WEAPON);
#endif
                }
                num++;
            }
            if (pouch->jumpLevel >= 2) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = ITEM_NULL;
                cmd->weapon[num].weapon = &marioWeapon_KururinJump;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
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
                cmd->weapon[num].itemId = ITEM_NULL;
                cmd->weapon[num].weapon = &marioWeapon_JyabaraJump;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
                cmd->weapon[num].icon = _getJumpIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                num++;
            }
            break;
        case CMD_ACT_HAMMER:
            if (pouch->hammerLevel >= 1) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = ITEM_NULL;
                cmd->weapon[num].weapon = &marioDefaultWeapon_Hammer;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
                cmd->weapon[num].icon = _getHammerIconId();
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                if (wp->flags & (BATTLE_FLAG_LECTURE_HAMMER | BATTLE_FLAG_LECTURE_FREE) && wp->lectureUnitId != -1) {
#if VERSION <= VERSION_DEMO_JP
                    cmd->weapon[num].weapon = BtlUnit_GetData(BattleGetUnitPtr(wp, wp->lectureUnitId), 0x44);
#else
                    cmd->weapon[num].weapon = BtlUnit_GetData(BattleGetUnitPtr(wp, wp->lectureUnitId), UNIT_DATA_LECTURE_HAMMER_WEAPON);
#endif
                }
                num++;
            }
            if (pouch->hammerLevel >= 2) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = ITEM_NULL;
                cmd->weapon[num].weapon = &marioWeapon_KaitenHammer;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
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
                cmd->weapon[num].itemId = ITEM_NULL;
                cmd->weapon[num].weapon = &marioWeapon_UltraHammer;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
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
        case CMD_ACT_JUMP:
            for (i = 0; i < 8; i++) {
                badge = _jump_badge_table[i];
                if (pouchEquipCheckBadge(badge) > 0) {
                    cmd->weapon[num].index = i;
                    cmd->weapon[num].itemId = badge;
                    cmd->weapon[num].weapon = itemDataTable[badge].battle_params;
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
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
        case CMD_ACT_HAMMER:
            for (k = 0; k < 9; k++) {
                badge2 = _hammer_badge_table[k];
                if (pouchEquipCheckBadge(badge2) > 0) {
                    cmd->weapon[num].index = k;
                    cmd->weapon[num].itemId = badge2;
                    cmd->weapon[num].weapon = itemDataTable[badge2].battle_params;
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
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
        case CMD_ACT_PARTY_MOVE:
            func = (void (*)(BattleCommand*, s32*))BtlUnit_GetData(unit, UNIT_DATA_TECH_MENU_FUNC);
            func(cmd, &num);
            break;
        case CMD_ACT_ITEM:
            items = pouch->heldItems;
            for (j = 0; j < MAX_HELD_ITEMS; j++) {
                s32 id = items[j];
                ItemData* data = &itemDataTable[id];
                if (id != 0 && (data->usable_locations & 2)) {
                    cmd->weapon[num].index = j;
                    cmd->weapon[num].itemId = id;
                    cmd->weapon[num].weapon = data->battle_params;
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
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
                    if (++num >= BATTLE_COMMAND_WEAPON_CAPACITY) {
                        break;
                    }
                }
            }
            if (num == 0) {
                cmd->weapon[num].index = -1;
                cmd->weapon[num].itemId = ITEM_NULL;
                cmd->weapon[num].index = 0;
                cmd->weapon[num].weapon = &defaultWeapon_Dummy_NoItem;
                cmd->weapon[num].enabled = 0;
                cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
#if VERSION >= VERSION_JP
                cmd->weapon[num].icon = ICON_UNAVAILABLE;
#endif
                if (cmd->weapon[num].weapon->name) {
                    cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                } else {
                    cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                }
                num++;
            }
            break;
        case CMD_ACT_SPECIAL: {
            u16 starPowers = pouch->starPowersObtained;
            s32 j;
            for (j = 0; j < 16; j++) {
                if ((starPowers & (1 << j)) && superActionTable[j]) {
                    cmd->weapon[num].index = -1;
                    cmd->weapon[num].itemId = ITEM_NULL;
                    cmd->weapon[num].weapon = superActionTable[j];
                    cmd->weapon[num].enabled = 0;
                    cmd->weapon[num].icon = superActionTable[j]->icon;
                    cmd->weapon[num].disableReason = CMD_DISABLE_NONE;
                    if (cmd->weapon[num].weapon->name) {
                        cmd->weapon[num].name = msgSearch(cmd->weapon[num].weapon->name);
                    } else {
                        cmd->weapon[num].name = msgSearch(itemDataTable[cmd->weapon[num].weapon->itemId].name);
                    }
                    if (++num >= BATTLE_COMMAND_WEAPON_CAPACITY) {
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
    while (cursor->count < cursor->relPos + BATTLE_COMMAND_VISIBLE_WEAPON_ROWS) {
        const s32 top = cursor->relPos;

        if (top <= 0) {
            break;
        }
        cursor->relPos--;
    }
    _btlcmd_UpdateSelectWeaponTable(wp, type);
    return num;
}

/**
 * Partner list for the battle's change-partner menu: unit kind, name, menu icon and help message for each partner
 * (entry 0 empty). _btlcmd_MakePartyTable lists the ones Mario has; _battleGetPartyIcon reads the icons.
 */
static BattleCheckPartyEntry _checkPartyTable[8] = {
    {0, NULL, 0, NULL},
    {UNIT_GOOMBELLA, "btl_cmd_pt_christine", ICON_PARTY_GOOMBELLA, "btl_hlp_cmd_operation_party1"},
    {UNIT_KOOPS, "btl_cmd_pt_nokotarou", ICON_PARTY_KOOPS, "btl_hlp_cmd_operation_party2"},
    {UNIT_FLURRIE, "btl_cmd_pt_clauda", ICON_PARTY_FLURRIE, "btl_hlp_cmd_operation_party3"},
    {UNIT_YOSHI, "btl_cmd_pt_yoshi", ICON_PARTY_YOSHI, "btl_hlp_cmd_operation_party4"},
    {UNIT_VIVIAN, "btl_cmd_pt_vivian", ICON_PARTY_VIVIAN, "btl_hlp_cmd_operation_party5"},
    {UNIT_BOBBERY, "btl_cmd_pt_sanders", ICON_PARTY_BOBBERY, "btl_hlp_cmd_operation_party6"},
    {UNIT_MS_MOWZ, "btl_cmd_pt_chuchurina", ICON_PARTY_MS_MOWZ, "btl_hlp_cmd_operation_party7"},
};

/**
 * Started by BattleCommandInput when the player picks a greyed-out command:
 * gets the reason message from _get_msg and announces it for 60 frames; the menu reopens when it ends.
 */
#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(_btl_cmd_error_msg_event)
    CALL(_get_msg, LW(0))
    CALL(btlevtcmd_AnnounceMessage, 0, 0, -30, LW(0), 60)
    RETURN
EVT_END()
#endif

static void _btlcmd_UpdateSelectWeaponTable(BattleWork* wp, s32 type) {
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
        cmd->weapon[i].disableReason = CMD_DISABLE_NONE;
        if (weapon) {
            result = _btlcmd_CheckWeaponTargetNum(wp, unit, weapon);
            if (result <= 0) {
                if (result == 0) {
                    switch (type) {
                        case CMD_ACT_JUMP:
                            cmd->weapon[i].disableReason = CMD_DISABLE_NO_JUMP_TARGET;
                            break;
                        case CMD_ACT_HAMMER:
                            cmd->weapon[i].disableReason = CMD_DISABLE_NO_HAMMER_TARGET;
                            break;
                        default:
                            cmd->weapon[i].disableReason = CMD_DISABLE_NOT_SELECTABLE;
                            break;
                    }
                } else if (type != CMD_ACT_SPECIAL) {
                    cmd->weapon[i].disableReason = CMD_DISABLE_NO_FP;
                } else {
                    cmd->weapon[i].disableReason = CMD_DISABLE_NO_SP;
                }
            } else {
                cmd->weapon[i].enabled = 1;
                cmd->weapon[i].disableReason = CMD_DISABLE_NONE;
            }
        }
    }
}

/**
 * Number of targets the unit could pick with the weapon:
 * 0 with no weapon or no attack script, -1 when the unit cannot pay its cost.
 */
static s32 _btlcmd_CheckWeaponTargetNum(BattleWork* wp, BattleUnit* unit, BattleWeapon* weapon) {
    BattleWorkWeaponTargets targets;
    s32 belong;

    if (weapon == NULL) {
        return 0;
    }
    if (weapon->attackEvtScript == NULL) {
        return 0;
    }
    if (!BtlUnit_CheckWeaponCost(unit, weapon)) {
        return -1;
    }
    belong = BtlUnit_GetEnemyBelong(unit);
    BattleSamplingEnemy(&targets, weapon, unit->unitId, belong, weapon->targetClassFlags,
        weapon->targetPropertyFlags, -wp->alliances[belong].attackDirection);
    return targets.numTargets;
}

/**
 * Builds the Tactics menu: Change Partner (when another partner can be picked),
 * Appeal (once a Star Power is obtained), Charge and Super Charge (if equipped, with their FP cost),
 * Defend and Run Away.
 * Run Away is greyed out in no-escape battles or when the unit cannot act, and most entries while veiled.
 * The early demos leave the cost of the free entries unset.
 */
static void _btlcmd_MakeOperationTable(BattleWork* wp) {
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

    // cmd = &wp->commandMenu is never NULL, but the test is in the retail code
    if (cmd == NULL) {
        return;
    }
    _btlcmd_GetCursorPtr(cmd, 5, &cursor);
    num = 0;
    if (_btlcmd_MakePartyTable(wp) >= 2) {
        cmd->operation[0].type = CMD_OP_CHANGE_PARTY;
        if (unit->flippedTurns) {
            cmd->operation[0].enabled = 0;
            cmd->operation[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        } else {
            cmd->operation[0].enabled = 1;
            cmd->operation[0].disableReason = CMD_DISABLE_NONE;
        }
        cmd->operation[0].name = msgSearch("btl_cmd_op_chg_party");
        party = unit;
        if (!(unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX)) {
            party = BattleGetPartnerPtr(wp, unit);
        }
        BtlUnit_CanActStatus(party);
        cmd->operation[0].icon = _battleGetPartyIcon(party->currentKind);
        cmd->operation[0].help = "btl_hlp_cmd_operation_changeparty";
        if (unit->attributeFlags & kVeiled) {
            cmd->operation[0].enabled = 0;
            cmd->operation[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        if (info->battleFlags & NPC_BATTLE_NO_PARTY_CHANGE) {
            cmd->operation[0].enabled = 0;
            cmd->operation[0].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
#if VERSION >= VERSION_JP
        cmd->operation[0].cost = 0;
#endif
        num = 1;
    }
    if (pouch->starPowersObtained != 0) {
        cmd->operation[num].type = CMD_OP_APPEAL;
        cmd->operation[num].enabled = 1;
        cmd->operation[num].name = msgSearch("btl_cmd_op_appeal");
        cmd->operation[num].icon = ICON_BTL_CMD_APPEAL;
#if VERSION <= VERSION_DEMO_JP
        cmd->operation[num].disableReason = 0;
#else
        cmd->operation[num].disableReason = CMD_DISABLE_NONE;
        cmd->operation[num].cost = 0;
#endif
        cmd->operation[num].help = "btl_hlp_cmd_operation_appeal";
        num++;
    }
    if (unit->badgesEquipped.Charge) {
        if (unit->currentKind == UNIT_MARIO) {
            weapon = &badgeWeapon_Charge;
        } else {
            weapon = &badgeWeapon_ChargeP;
        }
        cmd->operation[num].type = CMD_OP_CHARGE;
        cmd->operation[num].enabled = BtlUnit_CheckWeaponCost(unit, weapon);
        cmd->operation[num].cost = BtlUnit_GetWeaponCost(unit, weapon);
        if (cmd->operation[num].enabled) {
            cmd->operation[num].disableReason = CMD_DISABLE_NONE;
        } else {
            cmd->operation[num].disableReason = CMD_DISABLE_NO_FP;
        }
        cmd->operation[num].name = msgSearch(weapon->name);
        cmd->operation[num].icon = ICON_BTL_CMD_CHARGE;
        cmd->operation[num].help = weapon->description;
        if (unit->attributeFlags & kVeiled) {
            cmd->operation[num].enabled = 0;
            cmd->operation[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        num++;
    }
    if (unit->badgesEquipped.SuperCharge) {
        if (unit->currentKind == UNIT_MARIO) {
            weapon = &badgeWeapon_SuperCharge;
        } else {
            weapon = &badgeWeapon_SuperChargeP;
        }
        cmd->operation[num].type = CMD_OP_SUPER_CHARGE;
        cmd->operation[num].enabled = BtlUnit_CheckWeaponCost(unit, weapon);
        cmd->operation[num].cost = BtlUnit_GetWeaponCost(unit, weapon);
        if (cmd->operation[num].enabled) {
            cmd->operation[num].disableReason = CMD_DISABLE_NONE;
        } else {
            cmd->operation[num].disableReason = CMD_DISABLE_NO_FP;
        }
        cmd->operation[num].name = msgSearch(weapon->name);
        cmd->operation[num].icon = ICON_BTL_CMD_SUPER_CHARGE;
        cmd->operation[num].help = weapon->description;
        if (unit->attributeFlags & kVeiled) {
            cmd->operation[num].enabled = 0;
            cmd->operation[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
        }
        num++;
    }
    cmd->operation[num].type = CMD_OP_DEFEND;
    cmd->operation[num].enabled = 1;
    cmd->operation[num].name = msgSearch("btl_cmd_op_defend");
    cmd->operation[num].icon = ICON_BTL_CMD_DEFEND;
    cmd->operation[num].disableReason = CMD_DISABLE_NONE;
#if VERSION >= VERSION_JP
    cmd->operation[num].cost = 0;
#endif
    cmd->operation[num].help = "btl_hlp_cmd_operation_defend";
    if (unit->attributeFlags & kVeiled) {
        cmd->operation[num].enabled = 0;
        cmd->operation[num].disableReason = CMD_DISABLE_NOT_SELECTABLE;
    }
    idx = num + 1;
    cmd->operation[idx].type = CMD_OP_ESCAPE;
    cmd->operation[idx].enabled = BtlUnit_CanActStatus(unit);
    cmd->operation[idx].name = msgSearch("btl_cmd_op_escape");
    cmd->operation[idx].icon = ICON_ARROW;
#if VERSION >= VERSION_JP
    cmd->operation[idx].cost = 0;
#endif
    if (cmd->operation[idx].enabled) {
        cmd->operation[idx].disableReason = CMD_DISABLE_NONE;
    } else {
        cmd->operation[idx].disableReason = CMD_DISABLE_NO_FP;
    }
    cmd->operation[idx].help = "btl_hlp_cmd_operation_escape";
    if (unit->attributeFlags & kVeiled) {
        cmd->operation[idx].enabled = 0;
        cmd->operation[idx].disableReason = CMD_DISABLE_NOT_SELECTABLE;
    }
    if (wp->fbatInfo->setup->battleFlags & NPC_BATTLE_NO_ESCAPE) {
        cmd->operation[idx].enabled = 0;
        cmd->operation[idx].disableReason = CMD_DISABLE_NOT_SELECTABLE;
    }
    cursor->count = num + 2;
    if (cursor->absPos >= cursor->count) {
        cursor->absPos = cursor->count - 1;
    }
}

// kept: without it _btlcmd_MakeMultiItemTable changes; -opt noprop for the whole file changes other functions
#pragma opt_propagation off

/**
 * Builds the menu shown before the item list when Double Dip or Triple Dip is equipped:
 * a single item, then Double Dip and Triple Dip with their FP cost (the P versions' names and icons for a
 * partner).
 */
static void _btlcmd_MakeMultiItemTable(BattleWork* wp) {
    BattleUnit* unit;
    BattleCommand* cmd;
    BattleCursor* cursor;
    s32 num;

    unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    cmd = &wp->commandMenu;

    // cmd = &wp->commandMenu is never NULL, but the test is in the retail code
    if (cmd == NULL) {
        return;
    }
    _btlcmd_GetCursorPtr(cmd, 3, &cursor);
    cmd->multiItem[0].dipMode = 1;
    cmd->multiItem[0].enabled = 1;
    cmd->multiItem[0].name = msgSearch("btl_cmd_act_item");
    cmd->multiItem[0].icon = ICON_BTL_CMD_ITEM;
    cmd->multiItem[0].help = NULL;
    cmd->multiItem[0].cost = 0;
    num = 1;
    if (unit->badgesEquipped.DoubleDip) {
        cmd->multiItem[num].dipMode = 2;
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
            cmd->multiItem[num].disableReason = CMD_DISABLE_NO_FP;
        } else {
            cmd->multiItem[num].disableReason = CMD_DISABLE_NONE;
        }
        num++;
    }
    if (unit->badgesEquipped.TripleDip) {
        cmd->multiItem[num].dipMode = 3;
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
            cmd->multiItem[num].disableReason = CMD_DISABLE_NO_FP;
        } else {
            cmd->multiItem[num].disableReason = CMD_DISABLE_NONE;
        }
        num++;
    }
    cursor->count = num;
    if (cursor->absPos >= cursor->count) {
        cursor->absPos = cursor->count - 1;
    }
}
#pragma opt_propagation reset

/**
 * Menu icon of a partner kind from _checkPartyTable; Yoshi's follows the colour he hatched with.
 * 0 for a kind that is not a partner.
 */
static u16 _battleGetPartyIcon(s32 kind) {
    BattleCheckPartyEntry* entry;
    s32 i;

    // BUG: i runs to 7, so the last pass reads _checkPartyTable[8], one past the end of the table, when kind
    // matches none of the seven partners
    for (i = 0; i < 8; i++) {
        entry = &_checkPartyTable[i + 1];
        if (entry->kind == kind) {
            if (entry->kind == UNIT_YOSHI) {
                switch (pouchGetPartyColor(PARTY_YOSHI)) {
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

/**
 * Fills the Change Partner list with each partner whose pouch entry carries POUCH_PARTY_FLAG_JOINED,
 * with name, icon and HP.
 * The partner already out and KO'd partners are greyed out, and the cursor starts on the current one.
 * Returns the count, 0 when there is no partner out.
 */
static s32 _btlcmd_MakePartyTable(BattleWork* wp) {
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

    // cmd = &wp->commandMenu is never NULL, but the test is in the retail code
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
        if (!(party->flags & POUCH_PARTY_FLAG_JOINED)) {
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
        if (party->flags & POUCH_PARTY_FLAG_AWAY) {
            cmd->party[num].enabled = 0;
        }
        num++;
    }
    cursor->count = num;
    cursor->absPos = selected;
    return num;
}

static s32 _btlcmd_SelectWeaponDecide(BattleWork* wp, s32 type, s32 index) {
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

    // FAKE: (s32) cast on the s8 attackDirection passed as s8; dropping it changes the object
    BattleSamplingEnemy(targets, weapon, wp->activeUnitId, BtlUnit_GetEnemyBelong(unit), weapon->targetClassFlags,
        weapon->targetPropertyFlags, (s32)wp->alliances[unit->alliance].attackDirection);
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
void BattleCommandDisplay_ChangePartySelectMenuSetup(BattleWork* wp, s32 a);
s32 BattleCommandDisplay_ChangePartySelectMenuMain(BattleWork* wp);
void BattleCommandDisplay_ProtectPartnerSelectMenuSetup(BattleWork* wp);
void BattleCommandDisplay_ProtectPartnerSelectMenuEnd(BattleWork* wp);
static void _commandRestoreRec(void);
static s32 _check_present_item(void);
static void _btlcmd_SetAttackEvent(BattleUnit* unit, BattleCommand* cmd);

/**
 * Per-frame command input for Mario's or the partner's turn, called from btlseqAct:
 * a state machine over the action, weapon, Double Dip, Tactics, partner and target menus,
 * the error message for greyed-out picks, the X-button audience attack and items thrown by the audience.
 * In the middle of a Double or Triple Dip it opens the item list directly.
 * Returns 1 once the action is decided, after _btlcmd_SetAttackEvent has stored its script in the unit.
 */
s32 BattleCommandInput(BattleWork* work) {
    BattleUnit* unit;
    BattleCommand* cmd;
    s32 loop;
    s32 type;
    s32 wtype;
    BattleWorkWeaponTargets* targets;
    BattleWork* const wp = work;
    s16 unitIdx;
    s16 partIdx;
    s32 state;
    BattleCursor* cursor;
    s32 i;
    BattleWorkTarget* t;

    unit = BattleGetUnitPtr(work, work->activeUnitId);
    cmd = &wp->commandMenu;

    // cmd = &wp->commandMenu is never NULL, but the test is in the retail code
    if (cmd == NULL) {
        return 1;
    }
    _btlcmd_GetCursorPtr(cmd, 14, &cursor);
    if (_check_present_item()) {
        BattleAcHelpSetDispType(AC_HELP_DISP_OFF);
    }
    _commandRestoreRec();
    loop = 1;
    while (loop) {
        loop = 0;
    again:
        switch (state = cmd->state) {
            case CMD_STATE_INIT:
                if (wp->commandMenu.attackAudience) {
                    switch (cmd->actType) {
                    case CMD_ACT_AUDIENCE_ATTACK:
                    case CMD_ACT_AUDIENCE_PRESENT:
                        cmd->actType = cmd->prevActType;
                        break;
                    }
                    if (wp->commandMenu.attackAudience == 2) {
                        wp->commandMenu.attackAudience = 0;
                    }
                }
                cmd->cancelled = 0;
                wp->dispFlags |= BATTLE_DISP_FLAG_COMMAND_MENU;
                BtlUnit_SetCommandAnimPose(unit);
                if (unit->moveState >= MOVE_STATE_TRIPLE_DIP_1 && unit->moveState <= MOVE_STATE_DOUBLE_DIP_2) {
                    BattleStageOnLightInCommand();
                    BattleStageOffLightFriendInCommand();
                    BattleStageOffLightEnemyInCommand();
                    cmd->state = CMD_STATE_WEAPON_SETUP;
                    break;
                }
                BattleCommandDisplay_ActMenuSetup(wp, 0);
                cmd->state = CMD_STATE_ACT_MENU;
                BattleStageOnLightInCommand();
                BattleStageOffLightFriendInCommand();
                BattleStageOffLightEnemyInCommand();
                break;
            case CMD_STATE_ACT_REOPEN:
                BattleCommandDisplay_ActMenuSetup(wp, 1);
                cmd->state = CMD_STATE_ACT_MENU;
                break;
            case CMD_STATE_ACT_MENU:
                if (BattleCommandDisplay_ActMenuMain(wp)) {
                    loop = 1;
                }
                break;
            case CMD_STATE_ACT_ERROR_OPEN:
                cmd->state = CMD_STATE_ACT_ERROR;
                cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
            case CMD_STATE_ACT_ERROR:
                if (!evtCheckID(cmd->errorEvtId)) {
                    cmd->errorEvtId = 0;
                    loop = 1;
                    cmd->state = CMD_STATE_ACT_REOPEN;
                }
                break;
            case CMD_STATE_WEAPON_SETUP:
            case CMD_STATE_WEAPON_REOPEN:
                type = cmd->actType;
                if ((type == CMD_ACT_JUMP && (wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) ||
                    (type == CMD_ACT_HAMMER && (wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                    _btlcmd_SelectWeaponDecide(wp, type, 0);
                    BattleCommandDisplay_TargetSelectMenuSetup(wp);
                    cmd->state = CMD_STATE_TARGET_SETUP;
                    BattleStageOnLightEnemyInCommand();
                    break;
                }
                if (state == 5) {
                    if (type == CMD_ACT_ITEM && (unit->badgesEquipped.DoubleDip || unit->badgesEquipped.TripleDip)) {
                        BattleCommandDisplay_WeaponSelectMenuSetup(wp, type, 0);
                    } else {
                        BattleCommandDisplay_WeaponSelectMenuSetup(wp, type, 1);
                    }
                } else {
                    BattleCommandDisplay_WeaponSelectMenuSetup(wp, type, 0);
                }
                BattleStageOffLightFriendInCommand();
                BattleStageOffLightEnemyInCommand();
                cmd->state = CMD_STATE_WEAPON_MENU;
                _commandRestoreRec();
            case CMD_STATE_WEAPON_MENU:
                if (BattleCommandDisplay_WeaponSelectMenuMain(wp, cmd->actType)) {
                    loop = 1;
                }
                break;
            case CMD_STATE_WEAPON_ERROR_OPEN:
                cmd->state = CMD_STATE_WEAPON_ERROR;
                cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
            case CMD_STATE_WEAPON_ERROR:
                if (!evtCheckID(cmd->errorEvtId)) {
                    cmd->errorEvtId = 0;
                    loop = 1;
                    cmd->state = CMD_STATE_WEAPON_REOPEN;
                }
                break;
            case CMD_STATE_MULTI_CANCEL_SETUP:
                cmd->state = CMD_STATE_MULTI_CANCEL;
                BattleCommandDisplay_WeaponMultiItemCancelCheckSetup(wp);
                _commandRestoreRec();
                break;
            case CMD_STATE_MULTI_CANCEL:
                if (BattleCommandDisplay_WeaponMultiItemCancelCheckMain(wp)) {
                    loop = 1;
                }
                break;
            case CMD_STATE_TARGET_SETUP:
                _btlcmd_GetCursorPtr(cmd, cmd->actType, &cursor);
                BattleAcHelpSetHelp(cmd->weapon[cursor->absPos].weapon->acHelpMsg);
                BattleAcHelpSetDispType(AC_HELP_DISP_XLU);
                cmd->state = CMD_STATE_TARGET_SELECT;
                break;
            case CMD_STATE_TARGET_SELECT:
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
                if (cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_SINGLE) {
                    if (BattlePadCheckRepeat(BATTLE_PAD_STICK_RIGHT)) {
                        targets->currentTarget++;
                        if (targets->currentTarget >= targets->numTargets) {
                            targets->currentTarget = 0;
                        }
                        if (targets->numTargets >= 2) {
                            psndSFXOn(SFX_NAME_WORD("SFX_BTL_CURSOR_MOVE2"));
                        }
                    }
                    if (BattlePadCheckRepeat(BATTLE_PAD_STICK_LEFT)) {
                        targets->currentTarget--;
                        if (targets->currentTarget < 0) {
                            targets->currentTarget = targets->numTargets - 1;
                        }
                        if (targets->numTargets >= 2) {
                            psndSFXOn(SFX_NAME_WORD("SFX_BTL_CURSOR_MOVE2"));
                        }
                    }
                }
                if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                    BattleCommandDisplay_TargetSelectMenuEnd(wp);
                    cmd->state = CMD_STATE_WEAPON_REOPEN;
                    wp->flags &= ~BATTLE_FLAG_ACT_DECIDED_UNUSED;
                    BattleAcHelpSetDispType(AC_HELP_DISP_OFF);
                    BtlUnit_SetCommandAnimPose(unit);
                    if ((cmd->actType == CMD_ACT_JUMP && (wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) ||
                        (cmd->actType == CMD_ACT_HAMMER && (wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                        cmd->state = CMD_STATE_ACT_REOPEN;
                        BattleStageOffLightEnemyInCommand();
                    }
                    break;
                }
                if (BattlePadCheckTrigger(PAD_BUTTON_A)) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                    BattleStageOffLightInCommand(1, 1);
                    cmd->state = CMD_STATE_DECIDED;
                    BattleCommandDisplay_TargetSelectMenuEnd(wp);
                    wp->flags |= BATTLE_FLAG_ACT_DECIDED_UNUSED;
                    if (wtype == CMD_ACT_ITEM && unit->moveState == MOVE_STATE_NORMAL) {
                        switch (cmd->dipMode) {
                        case 2:
                            unit->moveState = MOVE_STATE_DOUBLE_DIP_1;
                            BtlUnit_PayWeaponCost(unit, &badgeWeapon_2KaiItem);
                            break;
                        case 3:
                            unit->moveState = MOVE_STATE_TRIPLE_DIP_1;
                            BtlUnit_PayWeaponCost(unit, &badgeWeapon_3KaiItem);
                            break;
                        }
                    }
                    BattleAcHelpSetDispType(AC_HELP_DISP_ON);
                }
                if (BattleCommandAttackAudienceCheck()) {
                    wp->commandMenu.attackAudience = 1;
                    BattleCommandDisplay_TargetSelectMenuEnd(wp);
                    BattleAcHelpSetDispType(AC_HELP_DISP_OFF);
                }
                break;
            case CMD_STATE_MULTI_ITEM_SETUP:
            case CMD_STATE_MULTI_ITEM_REOPEN:
                if (state == 14) {
                    BattleCommandDisplay_MultiItemMenuSetup(wp, 1);
                } else {
                    BattleCommandDisplay_MultiItemMenuSetup(wp, 0);
                }
                cmd->state = CMD_STATE_MULTI_ITEM_MENU;
                _commandRestoreRec();
            case CMD_STATE_MULTI_ITEM_MENU:
                if (BattleCommandDisplay_MultiItemMenuMain(wp)) {
                    loop = 1;
                }
                break;
            case CMD_STATE_MULTI_ITEM_ERROR_OPEN:
                cmd->state = CMD_STATE_MULTI_ITEM_ERROR;
                cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
            case CMD_STATE_MULTI_ITEM_ERROR:
                if (!evtCheckID(cmd->errorEvtId)) {
                    cmd->errorEvtId = 0;
                    loop = 1;
                    cmd->state = CMD_STATE_MULTI_ITEM_REOPEN;
                }
                break;
            case CMD_STATE_OPERATION_SETUP:
            case CMD_STATE_OPERATION_REOPEN:
                if (state == 19) {
                    BattleCommandDisplay_OperationMenuSetup(wp, 1);
                } else {
                    BattleCommandDisplay_OperationMenuSetup(wp, 0);
                }
                cmd->state = CMD_STATE_OPERATION_MENU;
                _commandRestoreRec();
            case CMD_STATE_OPERATION_MENU:
                if (BattleCommandDisplay_OperationMenuMain(wp)) {
                    loop = 1;
                }
                break;
            case CMD_STATE_OPERATION_ERROR_OPEN:
                cmd->state = CMD_STATE_OPERATION_ERROR;
                cmd->errorEvtId = evtEntry(_btl_cmd_error_msg_event, 0, EVENT_START_IMMEDIATE)->eventId;
            case CMD_STATE_OPERATION_ERROR:
                if (!evtCheckID(cmd->errorEvtId)) {
                    cmd->errorEvtId = 0;
                    loop = 1;
                    cmd->state = CMD_STATE_OPERATION_REOPEN;
                }
                break;
            case CMD_STATE_PARTY_SETUP:
            case CMD_STATE_PARTY_REOPEN:
                if (state == 24) {
                    BattleCommandDisplay_ChangePartySelectMenuSetup(wp, TRUE);
                } else {
                    BattleCommandDisplay_ChangePartySelectMenuSetup(wp, FALSE);
                }
                cmd->state = CMD_STATE_PARTY_MENU;
                _commandRestoreRec();
            case CMD_STATE_PARTY_MENU:
                if (BattleCommandDisplay_ChangePartySelectMenuMain(wp)) {
                    loop = 1;
                }
                break;
            case CMD_STATE_PROTECT_SETUP:
                BattleCommandDisplay_ProtectPartnerSelectMenuSetup(wp);
                cmd->state = CMD_STATE_PROTECT_SELECT;
                _commandRestoreRec();
            case CMD_STATE_PROTECT_SELECT:
                if (BattlePadCheckTrigger(PAD_BUTTON_B)) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                    BattleCommandDisplay_ProtectPartnerSelectMenuEnd(wp);
                    cmd->state = CMD_STATE_OPERATION_REOPEN;
                } else if (BattlePadCheckTrigger(PAD_BUTTON_A)) {
                    cmd->state = CMD_STATE_DECIDED;
                    BattleCommandDisplay_ProtectPartnerSelectMenuEnd(wp);
                }
                break;
            case CMD_STATE_DECIDED:
                _btlcmd_SetAttackEvent(unit, cmd);
                cmd->flags &= ~CMD_MENU_ENEMY_HP;
                wp->dispFlags &= ~BATTLE_DISP_FLAG_COMMAND_MENU;
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
#if VERSION <= VERSION_DEMO_JP
        dispEntry(CAMERA_2D, 501.0f, DISP_MODE_OPAQUE, BattleDrawEnemyHP, wp);
#else
        dispEntry(CAMERA_2D, 902.0f, DISP_MODE_OPAQUE, BattleDrawEnemyHP, wp);
#endif
        if (cmd->flags & CMD_MENU_ACT) {
            dispEntry(CAMERA_2D, 500.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_ActMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_WEAPON_SELECT) {
            dispEntry(CAMERA_2D, 500.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_WeaponSelectMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_OPERATION) {
            dispEntry(CAMERA_2D, 500.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_OperationMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_CHANGE_PARTY) {
            dispEntry(CAMERA_2D, 500.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_ChangePartySelectMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_MULTI_ITEM) {
            dispEntry(CAMERA_2D, 500.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_MultiItemMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_TARGET_SELECT) {
            dispEntry(CAMERA_2D, 500.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_TargetSelectMenuDisp, wp);
        }
        if (cmd->flags & CMD_MENU_PROTECT_PARTNER) {
            dispEntry(CAMERA_2D, 500.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_ProtectPartnerSelectMenuDisp, wp);
        }
    }
}

/** Puts a menu cursor back on the first entry. */
static void _cursor_init(BattleCursor* cursor) {
    cursor->absPos = 0;
    cursor->relPos = 0;
}

/**
 * Opens the action menu: rebuilds its entries, starts the menu display (a = 1 when coming back from a submenu)
 * and shows all enemy HP.
 * Opening it fresh resets the weapon, item, Special and Tactics cursors unless an audience action is pending.
 */
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

/**
 * Per-frame input for the action menu: moves the cursor,
 * A picks the action (on to its list, the Double Dip menu, Tactics or the tutorial end)
 * or shows why it is greyed out, Y swaps Mario and the partner when allowed, X attacks the audience.
 * Greys out Jump or Hammer when Mario's single move has no target and lets the camera drift
 * after 900 idle frames. Returns 1 when the menu state changed.
 */
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
        cmd->actType = CMD_ACT_CHANGE_POSITION;
        cmd->state = CMD_STATE_DECIDED;
        BattleMenuDisp_ActSelect_Main(wp);
        return 1;
    }
    while (BattleMenuKeyOKInACT(wp)) {
        for (i = 0; i < cursor->count; i++) {
            type = cmd->act[i].type;
            switch (type) {
                case CMD_ACT_JUMP:
                case CMD_ACT_HAMMER:
                    if ((type == CMD_ACT_JUMP && (wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) || (type == CMD_ACT_HAMMER && (wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                        _btlcmd_MakeSelectWeaponTable(wp, type);
                        if (_btlcmd_CheckWeaponTargetNum(wp, unit, cmd->weapon[0].weapon) <= 0) {
                            cmd->act[i].enabled = 0;
                            if (!cmd->act[i].disableReason) {
                                if (type == CMD_ACT_JUMP) {
                                    cmd->act[i].disableReason = CMD_DISABLE_NO_JUMP_TARGET;
                                } else {
                                    cmd->act[i].disableReason = CMD_DISABLE_NO_HAMMER_TARGET;
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
            btl_camera_set_moveSpeedLv(BTL_CAM_PRI_NORMAL, 5);
        }
        if (wp->commandMenu.attackAudience) {
            type = cmd->act[cursor->absPos].type;
            if ((type == CMD_ACT_JUMP && (wp->flags & BATTLE_FLAG_JUMP_NO_SUBMENU)) || (type == CMD_ACT_HAMMER && (wp->flags & BATTLE_FLAG_HAMMER_NO_SUBMENU))) {
                wp->commandMenu.attackAudience = 0;
                wp->commandMenu.recCommand = 0;
            }
        }
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || wp->commandMenu.attackAudience) {
            if (cmd->act[cursor->absPos].enabled) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                cmd->actType = cmd->act[cursor->absPos].type;
                switch (cmd->actType) {
                    case CMD_ACT_JUMP:
                    case CMD_ACT_HAMMER:
                    case CMD_ACT_SPECIAL:
                    case CMD_ACT_PARTY_MOVE:
                    case 7:
                    case 8:
                        cmd->state = CMD_STATE_WEAPON_SETUP;
                        ret = 1;
                        break;
                    case CMD_ACT_ITEM:
                        cmd->state = CMD_STATE_WEAPON_SETUP;
                        if (!unit->moveState) {
                            cmd->dipMode = 1;
                            if (unit->badgesEquipped.DoubleDip || unit->badgesEquipped.TripleDip) {
                                cmd->state = CMD_STATE_MULTI_ITEM_SETUP;
                                ret = 1;
                            }
                        }
                        break;
                    case CMD_ACT_OPERATION:
                        cmd->state = CMD_STATE_OPERATION_SETUP;
                        ret = 1;
                        break;
                    case CMD_ACT_LECTURE_END:
                        cmd->state = CMD_STATE_DECIDED;
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
                cmd->state = CMD_STATE_ACT_ERROR_OPEN;
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
            cmd->actType = CMD_ACT_CHANGE_POSITION;
            ret = 1;
            cmd->state = CMD_STATE_DECIDED;
        }
        break;
    }
    BattleMenuDisp_ActSelect_Main(wp);
    return ret;
}

/**
 * TRUE when Mario and the partner may swap places with Y:
 * not in zero gravity or a locked tutorial, both able to act and still on their first move of the turn,
 * Mario not veiled and the partner not flipped. battle_menu_disp.c also checks it.
 */
BOOL BattleCommandCheckChangePositionEnable(BattleWork* wp) {
    BattleUnit* unit;

    if (wp->flags & BATTLE_FLAG_LECTURE_NO_COMMAND) {
        return FALSE;
    }
    if (wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED) {
        return FALSE;
    }
    unit = BattleGetMarioPtr(wp);
    if (!unit) {
        return FALSE;
    }
    if (unit->moveState) {
        return FALSE;
    }
    if (unit->maxMovesThisTurn <= 0 || unit->maxMovesThisTurn != unit->movesRemaining) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) ||
        BtlUnit_CheckStatus(unit, STATUS_FREEZE) || BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
        return FALSE;
    }
    if (unit->attributeFlags & kVeiled) {
        return FALSE;
    }
    unit = BattleGetPartyPtr(wp);
    if (!unit) {
        return FALSE;
    }
    if (unit->moveState) {
        return FALSE;
    }
    if (unit->maxMovesThisTurn <= 0 || unit->maxMovesThisTurn != unit->movesRemaining) {
        return FALSE;
    }
    if (BtlUnit_CheckStatus(unit, STATUS_SLEEP) || BtlUnit_CheckStatus(unit, STATUS_STOP) ||
        BtlUnit_CheckStatus(unit, STATUS_FREEZE) || BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
        return FALSE;
    }
    return unit->flippedTurns <= 0;
}

/** Draw callback for the action menu. */
void BattleCommandDisplay_ActMenuDisp(CameraId camId, void* param) {
    BattleWork* wp = param;
    BattleCursor* cursor;

    _btlcmd_GetCursorPtr(&wp->commandMenu, 14, &cursor);
    BattleMenuDisp_ActSelect_Disp(wp);
}

void BattleCommandDisplay_ActMenuEnd(BattleWork* wp) {
    BattleMenuDisp_ActSelect_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_ACT;
}

/** Opens the weapon list of menu type `type`: builds it, starts its display (a is passed on) and shows all enemy HP. */
void BattleCommandDisplay_WeaponSelectMenuSetup(BattleWork* wp, s32 type, s32 a) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;

    _btlcmd_MakeSelectWeaponTable(wp, type);
    _btlcmd_GetCursorPtr(cmd, type, &cursor);
    BattleMenuDisp_WeaponSelect_Init(wp, cursor, cmd->weapon, a);
    cmd->flags |= (CMD_MENU_WEAPON_SELECT | CMD_MENU_ENEMY_HP);
}

void BattleCommandDisplay_WeaponSelectMenuEnd(BattleWork* wp);

/**
 * Per-frame input for the weapon, item, Special or partner-move list:
 * A on a usable entry samples its targets and moves to target selection, lighting the side it can hit;
 * on a greyed one it shows why.
 * B goes back to the action menu, or for items with Double or Triple Dip to the dip menu or, mid-dip,
 * to the cancel prompt. Returns 1 when the menu state changed.
 */
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
                cmd->state = CMD_STATE_TARGET_SETUP;
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
                cmd->state = CMD_STATE_WEAPON_ERROR_OPEN;
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
            if (type == CMD_ACT_ITEM && (unit->badgesEquipped.DoubleDip || unit->badgesEquipped.TripleDip)) {
                if (!unit->moveState) {
                    psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                    BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                    cmd->state = CMD_STATE_MULTI_ITEM_REOPEN;
                } else {
                    BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                    cmd->state = CMD_STATE_MULTI_CANCEL_SETUP;
                }
            } else {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_CANCEL1));
                BattleCommandDisplay_WeaponSelectMenuEnd(wp);
                cmd->state = CMD_STATE_ACT_REOPEN;
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

/** Draw callback for the weapon list. */
void BattleCommandDisplay_WeaponSelectMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_WeaponSelect_Disp(param);
}

void BattleCommandDisplay_WeaponSelectMenuEnd(BattleWork* wp) {
    BattleMenuDisp_WeaponSelect_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_WEAPON_SELECT;
}

/**
 * Resets the Yes/No cursor of the prompt shown when B is pressed in the item list in the middle of a Double
 * or Triple Dip.
 */
void BattleCommandDisplay_WeaponMultiItemCancelCheckSetup(BattleWork* wp) {
    wp->commandMenu.cursor[13].absPos = 0;
    wp->commandMenu.cursor[13].relPos = 0;
    wp->commandMenu.cursor[13].count = 2;
}

/**
 * Per-frame input for that prompt: Yes gives up the remaining items
 * and ends the action (cmd->cancelled, the unit then just waits); No or B returns to the item list.
 * Returns 1 once answered.
 */
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
            cmd->state = CMD_STATE_DECIDED;
        } else {
            cmd->state = CMD_STATE_WEAPON_REOPEN;
        }
    }
    dispEntry(CAMERA_2D, 900.0f, DISP_MODE_OPAQUE, BattleCommandDisplay_WeaponMultiItemCancelCheckDisp, wp);
    return done;
}

/**
 * Draws the Double/Triple Dip cancel prompt: btl_msg_multi_item_cancel_check with Yes/No and the hand cursor.
 * Before the US release Yes/No are hardcoded Japanese;
 * the PAL demo and PAL size the window by the text's measured width instead of its length.
 */
void BattleCommandDisplay_WeaponMultiItemCancelCheckDisp(CameraId camId, void* param) {
    BattleWork* wp = param;
    char buf[36];
    Vec pos;
    f32 w;
    f32 x;
    u32 len;

    pouchGetPtr();
    sprintf(buf, msgSearch("btl_msg_multi_item_cancel_check"));
#if VERSION >= VERSION_DEMO_EU
    len = (f32)FontGetMessageWidth(buf);

    windowDispGX_Waku_col(-30.0f - (w = 0.5f * (f32)len), 30.0f, 65.0f + (f32)len, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#else
    len = strlen(buf);

    windowDispGX_Waku_col(-30.0f - (w = 6.25f * (f32)len), 30.0f, 12.5f * (f32)len + 65.0f, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
#endif
    windowDispGX_Waku_col(-70.0f, -10.0f, 130.0f, 60.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    FontDrawStart();

    // x is a variable: the retail code computes 0.0f - w at run time (lfs 0.0f, fsubs); a literal 0.0f folds to fneg
    x = 0.0f;
    FontDrawString(x - w, 27.0f, buf);
#if VERSION >= VERSION_US
    FontDrawString(-20.0f, -13.0f, msgSearch("btl_msg_lvup_yes"));
    FontDrawString(-20.0f, -38.0f, msgSearch("btl_msg_lvup_no"));
#else
    FontDrawString(-20.0f, -13.0f, "\202\315\202\242\201@");
    FontDrawString(-20.0f, -38.0f, "\202\242\202\242\202\246");
#endif
    pos.x = -40.0f;
    pos.y = -42 - wp->commandMenu.cursor[13].absPos * 25;
    pos.z = 0.0f;
    iconDispGx(&pos, ICON_FLAG_2D | ICON_FLAG_FLIP_X, ICON_YUBI_CURSOR, 1.0f);
}

/** Opens the Tactics menu: builds its entries, starts its display (a is passed on) and shows all enemy HP. */
void BattleCommandDisplay_OperationMenuSetup(BattleWork* wp, s32 a) {
    _btlcmd_MakeOperationTable(wp);
    BattleMenuDisp_Operation_Init(wp, a);
    wp->commandMenu.flags |= (CMD_MENU_OPERATION | CMD_MENU_ENEMY_HP);
}

void BattleCommandDisplay_OperationMenuEnd(BattleWork* wp);

/**
 * Per-frame input for the Tactics menu: A on Change Partner opens the partner list,
 * type 4 the protect-partner choice, and the other tactics end input; a greyed entry shows why.
 * B goes back to the action menu. Returns 1 when the menu state changed.
 */
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
                    case CMD_OP_CHARGE:
                    case CMD_OP_SUPER_CHARGE:
                    case CMD_OP_ESCAPE:
                    case CMD_OP_APPEAL:
                    case CMD_OP_DEFEND:
                    case CMD_OP_WAIT:
                    case CMD_OP_WAKEUP_EVENT:
                        BattleStageOffLightInCommand(0, 1);
                        cmd->state = CMD_STATE_DECIDED;
                        break;
                    case CMD_OP_CHANGE_PARTY:
                        cmd->state = CMD_STATE_PARTY_SETUP;
                        break;
                    case CMD_OP_PROTECT_PARTNER:
                        cmd->state = CMD_STATE_PROTECT_SETUP;
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
                cmd->state = CMD_STATE_OPERATION_ERROR_OPEN;
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
            cmd->state = CMD_STATE_ACT_REOPEN;
        } else if (BattleCommandAttackAudienceCheck()) {
            wp->commandMenu.attackAudience = 1;
            BattleCommandDisplay_OperationMenuEnd(wp);
        }
    }
end:
    BattleMenuDisp_Operation_Main(wp);
    return ret;
}

/** Draw callback for the Tactics menu. */
void BattleCommandDisplay_OperationMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_Operation_Disp(param);
}

void BattleCommandDisplay_OperationMenuEnd(BattleWork* wp) {
    BattleMenuDisp_Operation_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_OPERATION;
}

/** Opens the Double/Triple Dip menu: builds its entries and starts its display (a is passed on). */
void BattleCommandDisplay_MultiItemMenuSetup(BattleWork* wp, s32 a) {
    _btlcmd_MakeMultiItemTable(wp);
    BattleMenuDisp_MultiItem_Init(wp, a);
    wp->commandMenu.flags |= CMD_MENU_MULTI_ITEM;
}

void BattleCommandDisplay_MultiItemMenuEnd(BattleWork* wp);

/**
 * Per-frame input for the Double/Triple Dip menu:
 * A sets cmd->dipMode to 1, 2 or 3 items and opens the item list, or shows why the entry is greyed out;
 * B goes back to the action menu. Returns 1 when the menu state changed.
 */
s32 BattleCommandDisplay_MultiItemMenuMain(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    s32 ret = 0;
    BattleCursor* cursor;

    if (BattleMenuKeyOKInACT(wp)) {
        _btlcmd_GetCursorPtr(cmd, 3, &cursor);
        if (BattlePadCheckTrigger(PAD_BUTTON_A) || wp->commandMenu.attackAudience) {
            if (cmd->multiItem[cursor->absPos].enabled) {
                psndSFXOn(SFX_NAME_WORD(PSND_SFX_BTL_CURSOR_OK1));
                cmd->dipMode = cmd->multiItem[cursor->absPos].dipMode;
                cmd->state = CMD_STATE_WEAPON_SETUP;
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
                cmd->state = CMD_STATE_MULTI_ITEM_ERROR_OPEN;
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
            cmd->state = CMD_STATE_ACT_REOPEN;
        } else if (BattleCommandAttackAudienceCheck()) {
            wp->commandMenu.attackAudience = 1;
            BattleCommandDisplay_MultiItemMenuEnd(wp);
        }
    }
end:
    BattleMenuDisp_MultiItem_Main(wp);
    return ret;
}

/** Draw callback for the Double/Triple Dip menu. */
void BattleCommandDisplay_MultiItemMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_MultiItem_Disp(param);
}

void BattleCommandDisplay_MultiItemMenuEnd(BattleWork* wp) {
    BattleMenuDisp_MultiItem_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_MULTI_ITEM;
}

/** Opens the Change Partner list display and shows all enemy HP. */
void BattleCommandDisplay_ChangePartySelectMenuSetup(BattleWork* wp, s32 a) {
    BattleMenuDisp_ChangePartySelect_Init(wp, a);
    wp->commandMenu.flags |= (CMD_MENU_CHANGE_PARTY | CMD_MENU_ENEMY_HP);
}

void BattleCommandDisplay_ChangePartySelectMenuEnd(BattleWork* wp);

/**
 * Per-frame input for the Change Partner list: A on an available partner stores cmd->partyKind and ends input;
 * B goes back to Tactics. Returns 1 when the menu state changed.
 */
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
                cmd->state = CMD_STATE_DECIDED;
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
            cmd->state = CMD_STATE_OPERATION_REOPEN;
        } else if (BattleCommandAttackAudienceCheck()) {
            wp->commandMenu.attackAudience = 1;
            BattleCommandDisplay_ChangePartySelectMenuEnd(wp);
        }
    }
end:
    BattleMenuDisp_ChangePartySelect_Main(wp);
    return ret;
}

/** Draw callback for the Change Partner list. */
void BattleCommandDisplay_ChangePartySelectMenuDisp(CameraId camId, void* param) {
    BattleMenuDisp_ChangePartySelect_Disp(param);
}

void BattleCommandDisplay_ChangePartySelectMenuEnd(BattleWork* wp) {
    BattleMenuDisp_ChangePartySelect_End(wp);
    wp->commandMenu.flags &= ~CMD_MENU_CHANGE_PARTY;
}

void BattleCommandDisplay_TargetSelectMenuSetup(BattleWork* wp) {
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);

    wp->commandMenu.flags |= CMD_MENU_TARGET_SELECT;
    BtlUnit_SetAnimType(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)), POSE_TYPE_WALK);
}

/**
 * Draws target selection: the hand cursor on the current target with its name boxed at the top left,
 * or for multi-target moves a cursor on every target and their names stacked (flipped for ceiling units;
 * Yoshi shows the name he was given).
 */
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
    f32 nameWidth;

    _btlcmd_GetCursorPtr(cmd, wp->commandMenu.actType, &cursor);
    if (cmd->weapon[cursor->absPos].weapon->targetClassFlags & TARGET_CLASS_SINGLE) {
        wt = &wp->weaponTargets;
        t = &wt->targets[wt->targetIndices[wt->currentTarget]];
        unit = BattleGetUnitPtr(wp, t->unitIdx);
        part = BtlUnit_GetPartsPtr(unit, t->partIdx);
        pos1.x = t->posX;
        pos1.y = t->posY;
        pos1.z = t->posZ;
        if (!(unit->attributeFlags & kCeiling)) {
            btlGetScreenPoint(&pos1, &screen1);
            icon.x = screen1.x;
            icon.y = screen1.y;
            icon.z = 0.0f;
            iconDispGx(&icon, ICON_FLAG_2D, ICON_TARGET_CURSOR, 1.0f);
            if (unit->currentKind == UNIT_YOSHI) {
                name = pouchGetYoshiName();
            } else {
                name = msgSearch(part->partName);
            }
            nameWidth = FontGetMessageWidth(name);
            windowDispGX_Waku_col(-240.0f, 86.0f, 16.0f + nameWidth, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF});
            FontDrawStart();
            FontDrawString(-232.0f, 83.0f, name);
        } else {
            btlGetScreenPoint(&pos1, &screen1);
            icon.x = screen1.x;
            icon.y = screen1.y;
            icon.z = 0.0f;
            iconDispGx(&icon, ICON_FLAG_2D | ICON_FLAG_FLIP_Y, ICON_TARGET_CURSOR, 1.0f);
            if (unit->currentKind == UNIT_YOSHI) {
                name = pouchGetYoshiName();
            } else {
                name = msgSearch(part->partName);
            }
            nameWidth = FontGetMessageWidth(name);
            windowDispGX_Waku_col(-240.0f, 86.0f, 16.0f + nameWidth, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF});
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
            if (!(unit2->attributeFlags & kCeiling)) {
                btlGetScreenPoint(&pos2, &screen2);
                icon.x = screen2.x;
                icon.y = screen2.y;
                icon.z = 0.0f;
                iconDispGx(&icon, ICON_FLAG_2D, ICON_TARGET_CURSOR, 1.0f);
                if (unit2->currentKind == UNIT_YOSHI) {
                    name = pouchGetYoshiName();
                } else {
                    name = msgSearch(part2->partName);
                }
                nameWidth = FontGetMessageWidth(name);
                windowDispGX_Waku_col(-240.0f, 118.0f - (f32)(i * 32), 16.0f + nameWidth, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF});
                FontDrawStart();
                FontDrawString(-232.0f, 115.0f - (f32)(i * 32), name);
            } else {
                btlGetScreenPoint(&pos2, &screen2);
                icon.x = screen2.x;
                icon.y = screen2.y;
                icon.z = 0.0f;
                iconDispGx(&icon, ICON_FLAG_2D | ICON_FLAG_FLIP_Y, ICON_TARGET_CURSOR, 1.0f);
                if (unit2->currentKind == UNIT_YOSHI) {
                    name = pouchGetYoshiName();
                } else {
                    name = msgSearch(part2->partName);
                }
                nameWidth = FontGetMessageWidth(name);
                windowDispGX_Waku_col(-240.0f, 118.0f - (f32)(i * 32), 16.0f + nameWidth, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF});
                FontDrawStart();
                FontDrawString(-232.0f, 115.0f - (f32)(i * 32), name);
            }
        }
    }
    btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT);
}

/** Ends target selection: resets the battle camera and clears the target-select and show-all-enemy-HP flags. */
void BattleCommandDisplay_TargetSelectMenuEnd(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;

    _btlcmd_GetCursorPtr(cmd, wp->commandMenu.actType, &cursor);
    btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT);
    cmd->flags &= ~CMD_MENU_TARGET_SELECT;
    cmd->flags &= ~CMD_MENU_ENEMY_HP;
}

void BattleCommandDisplay_ProtectPartnerSelectMenuSetup(BattleWork* wp) {
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);
    BattleGetPartnerPtr(wp, unit);
    wp->commandMenu.flags |= CMD_MENU_PROTECT_PARTNER;
    wp->commandMenu.flags &= ~CMD_MENU_ENEMY_HP;
}

/** Draws the hand cursor and name box over the partner for the protect-partner choice. */
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
    iconDispGx(&icon, ICON_FLAG_2D, ICON_TARGET_CURSOR, 1.0f);
    len = strlen(msgSearch(unit->kindParams->name));
    w = 6.25f * len;
    windowDispGX_Waku_col(icon.x - w, 80.0f + icon.y, 12.5f * len, 32.0f, 10.0f, 0, (GXColor){0xFF, 0xFF, 0xFF, 0xCF});
    FontDrawStart();
    FontDrawString(3.0f + icon.x - w, 77.0f + icon.y, msgSearch(unit->kindParams->name));
}

void BattleCommandDisplay_ProtectPartnerSelectMenuEnd(BattleWork* wp) {
    wp->commandMenu.flags &= ~CMD_MENU_PROTECT_PARTNER;
}

/**
 * Hides every command menu except the Double Dip menu at once;
 * _check_present_item calls it when an audience item interrupts input.
 */
void BattleCommandDisplay_AllEnd(BattleWork* wp) {
    wp->commandMenu.flags &= ~(CMD_MENU_ACT | CMD_MENU_WEAPON_SELECT | CMD_MENU_OPERATION | CMD_MENU_CHANGE_PARTY | CMD_MENU_TARGET_SELECT | CMD_MENU_PROTECT_PARTNER);
}

static s32 _check_weapon_type_attack(BattleWeapon* weapon);

/**
 * Turns the decided command into the unit's attackEventCode and flags for btlseqAct
 * and counts it in the battle action record: the chosen move, item or Special script;
 * the Tactics (Defend, Appeal, Charge, Super Charge, Run Away, Change Partner,
 * which with Quick Change keeps the turn); the Y position swap;
 * the audience attack (X) and audience item scripts; and the tutorial end. A cancelled Double Dip just waits.
 */
static void _btlcmd_SetAttackEvent(BattleUnit* unit, BattleCommand* cmd) {
    BattleWork* wp = _battleWorkPointer;
    u32 type = cmd->actType;
    BattleCursor* cursor;
    BattleWeapon* weapon;

    if (cmd->cancelled) {
        unit->moveState = MOVE_STATE_NORMAL;
        unit->attackEventCode = btldefaultevt_Wait;
        unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
        return;
    }
    switch (type) {
        case CMD_ACT_OPERATION:
            switch (cmd->operationType) {
            case CMD_OP_DEFEND:
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
                if (unit->currentKind == UNIT_MARIO) {
                    unit->attackEventCode = btldefaultevt_Defend;
                } else {
                    unit->attackEventCode = btldefaultevt_DefendParty;
                }
                break;
            case CMD_OP_APPEAL:
                unit->attackEventCode = btldefaultevt_Appeal;
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
                if (unit->currentKind == UNIT_MARIO) {
                    BtlActRec_AddCount(&wp->actRecord.marioAppeals);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.partnerAppeals);
                }
                break;
            case CMD_OP_WAIT:
                unit->attackEventCode = btldefaultevt_Wait;
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
                if (unit->currentKind == UNIT_MARIO) {
                    BtlActRec_AddCount(&wp->actRecord.marioAppeals);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.partnerAppeals);
                }
                break;
            case CMD_OP_ESCAPE:
                unit->attackEventCode = btldefaultevt_Escape;
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
                if (unit->currentKind == UNIT_MARIO) {
                    BtlActRec_AddCount(&wp->actRecord.marioEscapes);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.partnerEscapes);
                }
                break;
            case CMD_OP_CHARGE:
#if VERSION <= VERSION_DEMO_JP
                unit->attackEventCode = badgeWeapon_Charge.attackEvtScript;
#endif
                unit->attackEventFlags = 0;
                if (unit->currentKind == UNIT_MARIO) {
#if VERSION >= VERSION_JP
                    unit->attackEventCode = badgeWeapon_Charge.attackEvtScript;
#endif
                    BtlActRec_AddCount(&wp->actRecord.marioCharges);
                } else {
#if VERSION >= VERSION_JP
                    unit->attackEventCode = badgeWeapon_ChargeP.attackEvtScript;
#endif
                    BtlActRec_AddCount(&wp->actRecord.partnerCharges);
                }
                break;
            case CMD_OP_SUPER_CHARGE:
#if VERSION <= VERSION_DEMO_JP
                unit->attackEventCode = badgeWeapon_SuperCharge.attackEvtScript;
#endif
                unit->attackEventFlags = 0;
                if (unit->currentKind == UNIT_MARIO) {
#if VERSION >= VERSION_JP
                    unit->attackEventCode = badgeWeapon_SuperCharge.attackEvtScript;
#endif
                    BtlActRec_AddCount(&wp->actRecord.marioSuperCharges);
                } else {
#if VERSION >= VERSION_JP
                    unit->attackEventCode = badgeWeapon_SuperChargeP.attackEvtScript;
#endif
                    BtlActRec_AddCount(&wp->actRecord.partnerSuperCharges);
                }
                break;
            case CMD_OP_CHANGE_PARTY:
                unit->attackEventCode = btldefaultevt_ChangeParty;
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
                cmd->cursor[1].absPos = 0;
                if (wp->badgeEquippedFlags & BATTLE_BADGE_QUICK_CHANGE) {
                    unit->moveState = MOVE_STATE_QUICK_CHANGE;
                }
                if (unit->currentKind == UNIT_MARIO) {
                    BtlActRec_AddCount(&wp->actRecord.marioPartyChanges);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.partnerPartyChanges);
                }
                break;
            case CMD_OP_PROTECT_PARTNER:
                unit->attackEventCode = btldefaultevt_ProtectPartner;
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
                break;
            case CMD_OP_WAKEUP_EVENT:
                unit->attackEventCode = BtlUnit_GetData(unit, UNIT_DATA_WAKEUP_EVENT);
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW | ACT_EVENT_IGNORE_STATUS;
                break;
            default:
                unit->attackEventCode = btldefaultevt_Wait;
                unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW;
                break;
            }
            break;
        case CMD_ACT_JUMP:
        case CMD_ACT_HAMMER:
        case CMD_ACT_ITEM:
        case CMD_ACT_SPECIAL:
        case CMD_ACT_PARTY_MOVE:
            _btlcmd_GetCursorPtr(cmd, type, &cursor);
            weapon = cmd->weapon[cursor->absPos].weapon;
            unit->attackEventCode = weapon->attackEvtScript;
            unit->attackEventFlags = 0;
            if (!unit->attackEventCode) {
                unit->attackEventCode = btldefaultevt_Wait;
            }
            switch (type) {
            case CMD_ACT_JUMP:
                BtlActRec_AddCount(&wp->actRecord.marioJumpMoves);
                break;
            case CMD_ACT_HAMMER:
                BtlActRec_AddCount(&wp->actRecord.marioHammerMoves);
                break;
            case CMD_ACT_SPECIAL:
#if VERSION <= VERSION_DEMO_JP
                if (weapon->hpDamageFn) {
#else
                if (_check_weapon_type_attack(weapon)) {
#endif
                    BtlActRec_AddCount(&wp->actRecord.marioSpecialAttacks);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.marioSpecialSupports);
                }
                break;
            case CMD_ACT_ITEM:
#if VERSION >= VERSION_JP
                // FAKE: compares the pointers as signed integers (cmpw); the plain pointer compare changes the object
                if ((EvtWord)weapon == (EvtWord)&ItemWeaponData_Naniga_Okorukana) {
                    break;
                }
#endif
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
            case CMD_ACT_PARTY_MOVE:
                if (_check_weapon_type_attack(weapon)) {
                    BtlActRec_AddCount(&wp->actRecord.partnerAttackMoves);
                } else {
                    BtlActRec_AddCount(&wp->actRecord.partnerSupportMoves);
                }
                break;
            }
            break;
        case CMD_ACT_CHANGE_POSITION:
            unit->moveState = MOVE_STATE_POSITION_CHANGE;
            unit->attackEventCode = btldefaultevt_ChangePosition;
            unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW | ACT_EVENT_IGNORE_STATUS | ACT_EVENT_SHELL_FOLLOWS;
            break;
        case CMD_ACT_AUDIENCE_ATTACK:
            switch (unit->moveState) {
            case MOVE_STATE_TRIPLE_DIP_2:
                unit->moveState = MOVE_STATE_TRIPLE_DIP_1;
                break;
            case MOVE_STATE_TRIPLE_DIP_3:
                unit->moveState = MOVE_STATE_TRIPLE_DIP_2;
                break;
            case MOVE_STATE_DOUBLE_DIP_2:
                unit->moveState = MOVE_STATE_DOUBLE_DIP_1;
                break;
            default:
                unit->moveState = MOVE_STATE_AUDIENCE_ATTACK;
                break;
            }
            unit->attackEventCode = btlataudGetAttackEventPtr(unit->unitId);
            unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW | ACT_EVENT_IGNORE_STATUS;
            if (unit->currentKind == UNIT_MARIO) {
                BtlActRec_AddCount(&wp->actRecord.marioAudienceAttacks);
            } else {
                BtlActRec_AddCount(&wp->actRecord.partnerAudienceAttacks);
            }
            if (wp->lectureFlags & LECTURE_FLAG_CONTINUE_WINDOW) {
                msgWindow_Continue(wp->lectureWindowId);
            }
            break;
        case CMD_ACT_AUDIENCE_PRESENT:
            switch (unit->moveState) {
            case MOVE_STATE_TRIPLE_DIP_2:
                unit->moveState = MOVE_STATE_TRIPLE_DIP_1;
                break;
            case MOVE_STATE_TRIPLE_DIP_3:
                unit->moveState = MOVE_STATE_TRIPLE_DIP_2;
                break;
            case MOVE_STATE_DOUBLE_DIP_2:
                unit->moveState = MOVE_STATE_DOUBLE_DIP_1;
                break;
            default:
                unit->moveState = MOVE_STATE_AUDIENCE_ATTACK;
                break;
            }
            unit->attackEventCode = btlataudGetMoveEventPtr(unit->unitId);
            unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW | ACT_EVENT_IGNORE_STATUS;
            break;
        case CMD_ACT_LECTURE_END:
            unit->attackEventCode = btldefaultevt_LectureEnd;
            unit->attackEventFlags = ACT_EVENT_KEEP_STATUS_WINDOW | ACT_EVENT_IGNORE_STATUS;
            break;
        default:
            unit->attackEventCode = btldefaultevt_Wait;
            unit->attackEventFlags = 0;
            break;
    }
}

/**
 * The weapon under the cursor of the current list, i.e. the move the player chose;
 * read by the btlevtcmd helpers in battle_event_cmd.c.
 */
BattleWeapon* BattleGetSelectWeapon(BattleWork* wp) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;

    _btlcmd_GetCursorPtr(cmd, wp->commandMenu.actType, &cursor);
    return cmd->weapon[cursor->absPos].weapon;
}

/**
 * Handles an item the audience threw (BattleAudience_GetPresentItemNo):
 * a Heart or Flower restores 1 HP or FP to its target on the spot;
 * any other item closes the menus and forces the audience-item action (actType 12) and returns 1.
 * Returns 0 otherwise.
 */
static s32 _check_present_item(void) {
    BattleWork* wp = _battleWorkPointer;
    s32 item = BattleAudience_GetPresentItemNo();
    s32 id;
    BattleUnit* unit;
    Vec pos;

    if (item >= ITEM_TYPE_MAX) {
        return 0;
    }
    id = BattleAudience_GetPresentTargetUnitId();
    if (id < -1 || id >= BATTLE_COMMAND_UNIT_CAPACITY) {
        return 0;
    }
    unit = BattleGetUnitPtr(wp, id);
    switch (item) {
        case ITEM_NULL:
            return 0;
        case ITEM_HEART:
            BtlUnit_RecoverHp(unit, 1);
            BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
#if VERSION <= VERSION_DEMO_JP
            pos.y += BtlUnit_GetHeight(unit);
#else
            pos.y += unit->sizeMultiplier * BtlUnit_GetHeight(unit) + 5.0f;
#endif
            effRecoveryEntry(0, 1, pos.x, pos.y, pos.z);
            BattleAudience_SetPresentItemNo(0);
            return 0;
        case ITEM_FLOWER:
            BtlUnit_RecoverFp(unit, 1);
            BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
#if VERSION <= VERSION_DEMO_JP
            pos.y += BtlUnit_GetHeight(unit);
#else
            pos.y += unit->sizeMultiplier * BtlUnit_GetHeight(unit) + 5.0f;
#endif
            effRecoveryEntry(1, 1, pos.x, pos.y, pos.z);
            BattleAudience_SetPresentItemNo(0);
            return 0;
        default:
            if (item < 0 || item >= ITEM_TYPE_MAX) {
                return 0;
            }
            wp->commandMenu.attackAudience = 1;
            if (wp->commandMenu.state == CMD_STATE_MULTI_CANCEL_SETUP || wp->commandMenu.state == CMD_STATE_MULTI_CANCEL) {
                wp->commandMenu.attackAudience = 2;
            }
            BattleCommandDisplay_AllEnd(wp);
            wp->commandMenu.prevActType = wp->commandMenu.actType;
            wp->commandMenu.actType = CMD_ACT_AUDIENCE_PRESENT;
            wp->commandMenu.state = CMD_STATE_DECIDED;
            return 1;
    }
}

void BattleDrawEnemyHPBar(Vec pos, BattleUnit* unit, GXColor color);

/**
 * Draw callback for the HP number and bar under each living unit that passes the Tattle/Peekaboo check and has a gauge:
 * all of them while a menu shows all enemy HP, otherwise only those with their gauge on,
 * fading over its last 20 frames. During command input the gauges sit at the units' home positions.
 */
void BattleDrawEnemyHP(CameraId camId, void* param) {
    BattleWork* wp = param;
    GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
    s32 i;
    BattleUnit* unit;
    Vec position;
    Vec screen;
    Mtx mtx;
    s32 ofs;

    camGetPtr(CAMERA_2D);

    for (i = 0; i < BATTLE_COMMAND_UNIT_CAPACITY; i++) {
        unit = wp->units[i];
        if (!unit) {
            continue;
        }
        if (BtlUnit_CheckStatus(unit, STATUS_DEAD)) {
            continue;
        }
        if (!battleCheckUnitMonosiriFlag(wp->units[i])) {
            continue;
        }
        if (unit->attributeFlags & kUnitNoHpGauge) {
            continue;
        }
        if (!(wp->commandMenu.flags & CMD_MENU_ENEMY_HP) && !(unit->flags & BATTLE_UNIT_FLAG_SHOW_HP)) {
            continue;
        }
        position.x = unit->position.x;
        position.y = unit->position.y;
        position.z = unit->position.z;
        if (wp->flags & BATTLE_FLAG_COMMAND_INPUT) {
            position.x = unit->homePosition.x;
            position.y = unit->homePosition.y;
            position.z = unit->homePosition.z;
        }
        position.x += (f32)unit->hpGaugeOffset[0];
        position.y += (f32)unit->hpGaugeOffset[1];
        btlGetScreenPoint(&position, &screen);
        if ((unit->flags & BATTLE_UNIT_FLAG_SHOW_HP) && (unit->hpGauge.flags & HP_GAUGE_FLAG_TIMED) && unit->hpGauge.visibleTimer < 20) {
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

/**
 * Draws one HP bar at screen position pos: the frame texture, then the fill by barRatio,
 * with a black trail for recent damage while it drains (mode 1).
 * US and later use other texture ids than JP and the early demos.
 */
void BattleDrawEnemyHPBar(Vec pos, BattleUnit* unit, GXColor color) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    f32 x, y;

    btlDispGXInit2DRasta();
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    pos.x -= 10.0f;
    btlDispTexPlane(19.0f + pos.x, pos.y - 3.0f, 0.0f, 1.0f, 1.0f, VERSION_PICK(10, 17), color, 0);
    switch (unit->hpGauge.mode) {
        case 1:
            if (unit->hpGauge.drainTimer > 0) {
                btlDispGXInit2DRasta();
                GXLoadPosMtxImm(cam->view, GX_PNMTX0);
                y = 2.0f + pos.y;
                x = 1.0f + pos.x;
                btlDispGXQuads2DRasta(x, y, x + 34.0f * unit->hpGauge.trailRatio, y - 8.0f, 0, 0, 0, color.a);
                btlDispGXInit2DRasta();
                GXLoadPosMtxImm(cam->view, GX_PNMTX0);
            }
            btlDispTexPlane(19.0f + pos.x - 20.0f * (1.0f - unit->hpGauge.barRatio), pos.y - 3.0f, 0.0f, unit->hpGauge.barRatio, 1.0f, VERSION_PICK(11, 18), color, 0);
            break;
        default:
            x = unit->hpGauge.barRatio;
            btlDispTexPlane(19.0f + pos.x - 20.0f * (1.0f - x), pos.y - 3.0f, 0.0f, x, 1.0f, VERSION_PICK(11, 18), color, 0);
            break;
    }
}

/**
 * Picks the random action of a Confused Mario or partner
 * and returns its script (btlevtcmd in battle_event_cmd.c retries while it returns NULL):
 * a random enabled command other than Items or Special,
 * then a random single-target move usable while confused aimed at a random target, or Defend,
 * Run Away or Change Partner from Tactics.
 * Enemies and some invalid picks get subsetevt_confuse_flustered; no eligible move returns NULL for a retry,
 * as does no eligible Change Partner choice in JP and later.
 * US and later first drop a Double or Triple Dip in progress.
 */
void* BattleSetConfuseAct(BattleWork* wp, BattleUnit* unit) {
    BattleCommand* cmd = &wp->commandMenu;
    BattleCursor* cursor;
    s16 list[24];
    BattleWeapon* weapon;
    u8 cnt;
    u8 i;

    if (unit->moveState == MOVE_STATE_QUICK_CHANGE) {
        unit->moveState = MOVE_STATE_NORMAL;
    }
    BattleAcHelpSetDispType(AC_HELP_DISP_OFF);
    if (unit->alliance) {
        return subsetevt_confuse_flustered;
    }
    if (unit->currentKind != UNIT_MARIO && (unit->currentKind < TYPE_PARTNER_MIN || unit->currentKind >= TYPE_PARTNER_MAX)) {
        return subsetevt_confuse_flustered;
    }
#if VERSION >= VERSION_US
    if (cmd->dipMode) {
        unit->moveState = MOVE_STATE_NORMAL;
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
            if (cmd->act[i].enabled && cmd->act[i].type != CMD_ACT_SPECIAL && cmd->act[i].type != CMD_ACT_ITEM) {
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
        case CMD_ACT_JUMP:
        case CMD_ACT_HAMMER:
        case CMD_ACT_ITEM:
        case CMD_ACT_SPECIAL:
        case CMD_ACT_PARTY_MOVE:
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
                    if (cmd->weapon[i].enabled && (weapon->special & SPECIAL_CONFUSED_ACTION) &&
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
            if (!(weapon->special & SPECIAL_CONFUSED_ACTION)) {
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
            return weapon->attackEvtScript;
        case CMD_ACT_OPERATION:
            _btlcmd_MakeOperationTable(wp);
            _btlcmd_GetCursorPtr(cmd, 5, &cursor);
            cnt = 0;
            for (i = 0; i < cursor->count; i++) {
                if (cmd->operation[i].enabled) {
                    if (cmd->operation[i].type == CMD_OP_CHANGE_PARTY || cmd->operation[i].type == CMD_OP_DEFEND || cmd->operation[i].type == CMD_OP_ESCAPE) {
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
            if (!cmd->operation[cursor->absPos].enabled) {
                return subsetevt_confuse_flustered;
            }
            cmd->operationType = cmd->operation[cursor->absPos].type;
            switch (cmd->operationType) {
            case CMD_OP_DEFEND:
                if (unit->currentKind == UNIT_MARIO) {
                    return btldefaultevt_Defend;
                }
                return btldefaultevt_DefendParty;
            case CMD_OP_APPEAL:
                return btldefaultevt_Appeal;
            case CMD_OP_WAIT:
                return btldefaultevt_Wait;
            case CMD_OP_ESCAPE:
                return btldefaultevt_Escape;
            case CMD_OP_CHARGE:
#if VERSION <= VERSION_DEMO_JP
                return badgeWeapon_Charge.attackEvtScript;
            case CMD_OP_SUPER_CHARGE:
                return badgeWeapon_SuperCharge.attackEvtScript;
            case CMD_OP_WAKEUP_EVENT:
                return BtlUnit_GetData(unit, UNIT_DATA_WAKEUP_EVENT);
            case CMD_OP_CHANGE_PARTY: {
#else
                if (unit->currentKind == UNIT_MARIO) {
                    return badgeWeapon_Charge.attackEvtScript;
                }
                return badgeWeapon_ChargeP.attackEvtScript;
            case CMD_OP_SUPER_CHARGE:
                if (unit->currentKind == UNIT_MARIO) {
                    return badgeWeapon_SuperCharge.attackEvtScript;
                }
                return badgeWeapon_SuperChargeP.attackEvtScript;
            case CMD_OP_WAKEUP_EVENT:
                return BtlUnit_GetData(unit, UNIT_DATA_WAKEUP_EVENT);
            case CMD_OP_CHANGE_PARTY:
                {
#endif
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
#if VERSION <= VERSION_DEMO_JP
                        return subsetevt_confuse_flustered;
#else
                        return NULL;
#endif
                    }
                }
                if (!cmd->party[cursor->absPos].enabled) {
                    return subsetevt_confuse_flustered;
                }
                cmd->partyKind = cmd->party[cursor->absPos].kind;
                cmd->cursor[1].absPos = 0;
                return btldefaultevt_ChangeParty;
            case CMD_OP_PROTECT_PARTNER:
                return subsetevt_confuse_flustered;
            default:
                return subsetevt_confuse_flustered;
            }
        default:
            return subsetevt_confuse_flustered;
    }
}

/**
 * Records the current menu state in cmd->recCommand;
 * while an audience action is pending, clears it
 * and the pending action once the menu is back at the recorded state.
 * The command loop calls it after each state while attackAudience is set (from an X audience attack or an audience item) and
 * then re-runs the states at once,
 * with BattleMenuKeyOKInACT letting input through, until it is back at the recorded state.
 * Guess: this rebuilds the menu where the player was after the audience interruption.
 */
static void _commandRestoreRec(void) {
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

/**
 * X during command input: unless in zero gravity or Mario is veiled,
 * targets the audience member BattleAudience_GetItemOn reports,
 * closes the menu and decides the audience-attack action (actType 11). TRUE when that happened.
 */
static BOOL BattleCommandAttackAudienceCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;

    if (BattlePadCheckTrigger(PAD_BUTTON_X) && !(wp->flags & BATTLE_FLAG_ZERO_GRAVITY_UNUSED)) {
        if (BattleGetMarioPtr(wp)->attributeFlags & kVeiled) {
            return FALSE;
        }
        BattleAudience_GetItemOn(&id, NULL, NULL, NULL, NULL);
        if (id != -1) {
            BattleAudience_SetTarget(id);
            BattleCommandDisplay_ActMenuEnd(wp);
            wp->commandMenu.prevActType = wp->commandMenu.actType;
            wp->commandMenu.actType = CMD_ACT_AUDIENCE_ATTACK;
            wp->commandMenu.state = CMD_STATE_DECIDED;
            BattleStageOffLightInCommand(0, 1);
            wp->flags |= BATTLE_FLAG_ACT_DECIDED_UNUSED;
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Script helper for _btl_cmd_error_msg_event: sets args[0] to the message id explaining why the picked command
 * is greyed out, by cmd->disableReason (no FP, no Star Power, no jump or hammer target, can't run,
 * or can't pick it now).
 */
// FAKE: auto_inline on for _get_msg alone in this -inline noauto file: as an auto-inline candidate it gets the
// front end's simple optimizer pass, which gives the target's string base register; without it r5/r6 swap
#pragma auto_inline on
static USER_FUNC(_get_msg) {
    EvtWord* args = event->args;
    s32 index = *args++;
    BattleWork* wp = _battleWorkPointer;
    const char* msg;

    switch (wp->commandMenu.disableReason) {
        case CMD_DISABLE_NO_FP:
            msg = "btl_msg_cmderror_no_fp";

            break;
        case CMD_DISABLE_NO_SP:
            msg = "btl_msg_cmderror_no_ap";
            break;
        case CMD_DISABLE_NO_JUMP_TARGET:
            msg = "btl_msg_cmderror_no_jump_target";
            break;
        case CMD_DISABLE_NO_HAMMER_TARGET:
            msg = "btl_msg_cmderror_no_hammer_target";
            break;
        case CMD_DISABLE_NO_ESCAPE:
            msg = "btl_msg_cmderror_no_escape";
            break;
        default:
            msg = "btl_msg_cmderror_no_select_now";
            break;
    }
    evtSetValue(event, index, (EvtWord)msg);
    return EVT_RETURN_DONE;
}
#pragma auto_inline reset

/**
 * TRUE if the weapon is offensive: it deals HP damage or inflicts a harmful status (sleep, stop, dizzy, poison,
 * confuse, burn, freeze, shrink, ATK or DEF down, OHKO, fright, gale force, slow).
 * Used to count attack against support moves and items in the action record.
 */
static s32 _check_weapon_type_attack(BattleWeapon* weapon) {
    if (!weapon) {
        return 0;
    }
    if (weapon->hpDamageFn) {
        return 1;
    }
    if ((s8)weapon->sleepChance > 0 && weapon->sleepTime > 0) {
        return 1;
    }
    if ((s8)weapon->stopChance > 0 && weapon->stopTime > 0) {
        return 1;
    }
    if ((s8)weapon->dizzyChance > 0 && weapon->dizzyTime > 0) {
        return 1;
    }
    if ((s8)weapon->poisonChance > 0 && weapon->poisonTime > 0) {
        return 1;
    }
    if ((s8)weapon->confuseChance > 0 && weapon->confuseTime > 0) {
        return 1;
    }
    if ((s8)weapon->burnChance > 0 && weapon->burnTime > 0) {
        return 1;
    }
    if ((s8)weapon->freezeChance > 0 && weapon->freezeTime > 0) {
        return 1;
    }
    if ((s8)weapon->sizeChangeChance > 0 && weapon->sizeChangeTime > 0 && weapon->sizeChangeStrength < 0) {
        return 1;
    }
    if ((s8)weapon->atkChangeChance > 0 && weapon->atkChangeTime > 0 && weapon->atkChangeStrength < 0) {
        return 1;
    }
    if ((s8)weapon->defChangeChance > 0 && weapon->defChangeTime > 0 && weapon->defChangeStrength < 0) {
        return 1;
    }
    if (weapon->ohkoChance > 0) {
        return 1;
    }
    if ((s8)weapon->frightChance > 0) {
        return 1;
    }
    if ((s8)weapon->galeForceChance > 0) {
        return 1;
    }
    if ((s8)weapon->slowChance > 0 && weapon->slowTime > 0) {
        return 1;
    }
    return 0;
}
