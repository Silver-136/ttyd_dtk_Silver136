// Audience data: the item and animation tables for each kind of audience member.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "mario/mario_pouch.h"
#include "battle/battle_audience_kind_data.h"
#include "battle/battle_audience_anim.h"


s32 audience_kinopio_run_front[14];
s32 audience_daisan_run_front[14];
s32 audience_teresa_run_front[14];
s32 audience_hammer_bros_run_front[14];
s32 audience_honenoko_run_front[14];
s32 audience_heiho_run_front[14];
s32 audience_pansy_run_front[14];
s32 audience_puni_run_front[14];
s32 audience_nokonoko_run_front[14];
s32 audience_heavy_bomb_run_front[14];
s32 audience_kuriboo_run_front[14];
s32 audience_pakkun_run_front[14];
s32 audience_luigi_run_front[14];
s32* audience_anim_kinopio[19];
s32* audience_anim_daisan[19];
s32* audience_anim_teresa[19];
s32* audience_anim_hammer_bros[19];
s32* audience_anim_honenoko[19];
s32* audience_anim_heiho[19];
s32* audience_anim_pansy[19];
s32* audience_anim_puni[19];
s32* audience_anim_nokonoko[19];
s32* audience_anim_heavy_bomb[19];
s32* audience_anim_kuriboo[19];
s32* audience_anim_pakkun[19];
s32* audience_anim_luigi[19];
BattleAudienceItemKind audience_item_kinopio[12];
#if VERSION <= VERSION_DEMO_US
BattleAudienceItemKind audience_item_heiho[18];
#else
BattleAudienceItemKind audience_item_heiho[17];
#endif
BattleAudienceItemKind audience_item_puni[11];
BattleAudienceItemKind audience_item_nokonoko[8];
BattleAudienceItemKind audience_item_luigi[12];
BattleAudienceKind audience_kind[13];
s32 audience_kinopio_wait_front[8];
s32 audience_kinopio_wait_back[8];
s32 audience_kinopio_jump_front[8];
s32 audience_kinopio_jump_back[8];
s32 audience_kinopio_raise_back[8];
s32 audience_kinopio_itemon_back[8];
s32 audience_kinopio_itemthrow_back[4];
s32 audience_kinopio_tumble_front[8];
s32 audience_kinopio_damage_front[4];
s32 audience_kinopio_sleep_front[8];
s32 audience_kinopio_sleep_back[8];
s32 audience_kinopio_confuse_front[8];
s32 audience_kinopio_confuse_back[8];
s32 audience_daisan_wait_front[8];
s32 audience_daisan_wait_back[8];
s32 audience_daisan_jump_front[8];
s32 audience_daisan_jump_back[8];
s32 audience_daisan_raise_back[8];
s32 audience_daisan_itemon_back[8];
s32 audience_daisan_itemthrow_back[4];
s32 audience_daisan_tumble_front[8];
s32 audience_daisan_damage_front[4];
s32 audience_daisan_sleep_front[8];
s32 audience_daisan_sleep_back[8];
s32 audience_daisan_confuse_front[8];
s32 audience_daisan_confuse_back[8];
s32 audience_teresa_wait_front[8];
s32 audience_teresa_wait_back[8];
s32 audience_teresa_jump_front[8];
s32 audience_teresa_jump_back[8];
s32 audience_teresa_raise_back[8];
s32 audience_teresa_itemon_back[8];
s32 audience_teresa_itemthrow_back[4];
s32 audience_teresa_tumble_front[8];
s32 audience_teresa_damage_front[4];
s32 audience_teresa_sleep_front[8];
s32 audience_teresa_sleep_back[8];
s32 audience_teresa_confuse_front[8];
s32 audience_teresa_confuse_back[8];
s32 audience_hammer_bros_wait_front[8];
s32 audience_hammer_bros_wait_back[8];
s32 audience_hammer_bros_jump_front[8];
s32 audience_hammer_bros_jump_back[8];
s32 audience_hammer_bros_raise_back[8];
s32 audience_hammer_bros_itemon_back[8];
s32 audience_hammer_bros_itemthrow_back[4];
s32 audience_hammer_bros_tumble_front[8];
s32 audience_hammer_bros_damage_front[4];
s32 audience_hammer_bros_sleep_front[8];
s32 audience_hammer_bros_sleep_back[8];
s32 audience_hammer_bros_confuse_front[8];
s32 audience_hammer_bros_confuse_back[8];
s32 audience_honenoko_wait_front[8];
s32 audience_honenoko_wait_back[8];
s32 audience_honenoko_jump_front[8];
s32 audience_honenoko_jump_back[8];
s32 audience_honenoko_raise_back[8];
s32 audience_honenoko_itemon_back[8];
s32 audience_honenoko_itemthrow_back[4];
s32 audience_honenoko_tumble_front[8];
s32 audience_honenoko_damage_front[4];
s32 audience_honenoko_sleep_front[8];
s32 audience_honenoko_sleep_back[8];
s32 audience_honenoko_confuse_front[8];
s32 audience_honenoko_confuse_back[8];
s32 audience_heiho_wait_front[8];
s32 audience_heiho_wait_back[8];
s32 audience_heiho_jump_front[8];
s32 audience_heiho_jump_back[8];
s32 audience_heiho_raise_back[8];
s32 audience_heiho_itemon_back[8];
s32 audience_heiho_itemthrow_back[4];
s32 audience_heiho_tumble_front[8];
s32 audience_heiho_damage_front[4];
s32 audience_heiho_sleep_front[8];
s32 audience_heiho_sleep_back[8];
s32 audience_heiho_confuse_front[8];
s32 audience_heiho_confuse_back[8];
s32 audience_pansy_wait_front[8];
s32 audience_pansy_wait_back[8];
s32 audience_pansy_jump_front[8];
s32 audience_pansy_jump_back[8];
s32 audience_pansy_raise_back[8];
s32 audience_pansy_itemon_back[8];
s32 audience_pansy_itemthrow_back[4];
s32 audience_pansy_tumble_front[8];
s32 audience_pansy_damage_front[4];
s32 audience_pansy_sleep_front[8];
s32 audience_pansy_sleep_back[8];
s32 audience_pansy_confuse_front[8];
s32 audience_pansy_confuse_back[8];
s32 audience_pansy_sing_front[11];
s32 audience_puni_wait_front[8];
s32 audience_puni_wait_back[8];
s32 audience_puni_jump_front[8];
s32 audience_puni_jump_back[8];
s32 audience_puni_raise_back[8];
s32 audience_puni_itemon_back[8];
s32 audience_puni_itemthrow_back[4];
s32 audience_puni_tumble_front[8];
s32 audience_puni_damage_front[4];
s32 audience_puni_sleep_front[8];
s32 audience_puni_sleep_back[8];
s32 audience_puni_confuse_front[8];
s32 audience_puni_confuse_back[8];
s32 audience_nokonoko_wait_front[8];
s32 audience_nokonoko_wait_back[8];
s32 audience_nokonoko_jump_front[8];
s32 audience_nokonoko_jump_back[8];
s32 audience_nokonoko_raise_back[8];
s32 audience_nokonoko_itemon_back[8];
s32 audience_nokonoko_itemthrow_back[4];
s32 audience_nokonoko_tumble_front[8];
s32 audience_nokonoko_damage_front[4];
s32 audience_nokonoko_sleep_front[8];
s32 audience_nokonoko_sleep_back[8];
s32 audience_nokonoko_confuse_front[8];
s32 audience_nokonoko_confuse_back[8];
s32 audience_nokonoko_shell_back[8];
s32 audience_heavy_bomb_wait_front[8];
s32 audience_heavy_bomb_wait_back[8];
s32 audience_heavy_bomb_jump_front[8];
s32 audience_heavy_bomb_jump_back[8];
s32 audience_heavy_bomb_raise_back[8];
s32 audience_heavy_bomb_itemon_back[8];
s32 audience_heavy_bomb_itemthrow_back[4];
s32 audience_heavy_bomb_tumble_front[8];
s32 audience_heavy_bomb_damage_front[4];
s32 audience_heavy_bomb_sleep_front[8];
s32 audience_heavy_bomb_sleep_back[8];
s32 audience_heavy_bomb_confuse_front[8];
s32 audience_heavy_bomb_confuse_back[8];
s32 audience_heavy_bomb_ignite_front[11];
s32 audience_heavy_bomb_ignite_back[11];
s32 audience_kuriboo_wait_front[8];
s32 audience_kuriboo_wait_back[8];
s32 audience_kuriboo_jump_front[8];
s32 audience_kuriboo_jump_back[8];
s32 audience_kuriboo_raise_back[8];
s32 audience_kuriboo_itemon_back[8];
s32 audience_kuriboo_itemthrow_back[4];
s32 audience_kuriboo_tumble_front[8];
s32 audience_kuriboo_damage_front[4];
s32 audience_kuriboo_sleep_front[8];
s32 audience_kuriboo_sleep_back[8];
s32 audience_kuriboo_confuse_front[8];
s32 audience_kuriboo_confuse_back[8];
s32 audience_pakkun_wait_front[8];
s32 audience_pakkun_wait_back[8];
s32 audience_pakkun_jump_front[8];
s32 audience_pakkun_jump_back[8];
s32 audience_pakkun_raise_back[8];
s32 audience_pakkun_itemon_back[8];
s32 audience_pakkun_itemthrow_back[4];
s32 audience_pakkun_tumble_front[8];
s32 audience_pakkun_damage_front[4];
s32 audience_pakkun_sleep_front[8];
s32 audience_pakkun_sleep_back[8];
s32 audience_pakkun_confuse_front[8];
s32 audience_pakkun_confuse_back[8];
s32 audience_pakkun_eat_front[7];
s32 audience_luigi_wait_front[8];
s32 audience_luigi_wait_back[8];
s32 audience_luigi_jump_front[8];
s32 audience_luigi_jump_back[8];
s32 audience_luigi_raise_back[8];
s32 audience_luigi_itemon_back[8];
s32 audience_luigi_itemthrow_back[4];
s32 audience_luigi_tumble_front[8];
s32 audience_luigi_damage_front[4];
s32 audience_luigi_sleep_front[8];
s32 audience_luigi_sleep_back[8];
s32 audience_luigi_confuse_front[8];
s32 audience_luigi_confuse_back[8];
BattleAudienceItemKind audience_item_daisan[3];
BattleAudienceItemKind audience_item_teresa[6];
BattleAudienceItemKind audience_item_hammer_bros[2];
BattleAudienceItemKind audience_item_honenoko[4];
BattleAudienceItemKind audience_item_pansy[1];
BattleAudienceItemKind audience_item_heavy_bomb[1];
BattleAudienceItemKind audience_item_kuriboo[3];
BattleAudienceItemKind audience_item_pakkun[1];

#if VERSION > VERSION_DEMO_US
#include "../src/data/battle/battle_audience_run_front.inc"
#include "../src/data/battle/battle_audience_anim.inc"
#define AUD_ITEM_GROUP 1
#include "../src/data/battle/battle_audience_item.inc"
#undef AUD_ITEM_GROUP
#include "../src/data/battle/battle_audience_kind.inc"
#endif

/** Toad audience member facing the camera: idle loop (slot 0). */
s32 audience_kinopio_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(0, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member seen from behind: idle loop (slot 1). */
s32 audience_kinopio_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(1, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_kinopio_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(2, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_kinopio_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(3, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_kinopio_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(4, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_kinopio_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(6, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_kinopio_itemthrow_back[] = {
    AUD_FRAME(7, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 1
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Toad audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_kinopio_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(11, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_kinopio_damage_front[] = {
    AUD_FRAME(11, 32), AUD_END,
};

/** Toad audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_kinopio_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(12, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_kinopio_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(13, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member facing the camera: confused loop (slot 12). */
s32 audience_kinopio_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(14, 8), AUD_GOTO(0), AUD_END,
};

/** Toad audience member seen from behind: confused loop (slot 13). */
s32 audience_kinopio_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(15, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member facing the camera: idle loop (slot 0). */
s32 audience_daisan_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(16, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member seen from behind: idle loop (slot 1). */
s32 audience_daisan_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(17, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_daisan_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(18, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_daisan_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(19, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_daisan_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(21, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_daisan_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(22, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_daisan_itemthrow_back[] = {
    AUD_FRAME(23, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 2
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** X-Naut audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_daisan_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(27, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_daisan_damage_front[] = {
    AUD_FRAME(27, 32), AUD_END,
};

/** X-Naut audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_daisan_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(28, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_daisan_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(29, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member facing the camera: confused loop (slot 12). */
s32 audience_daisan_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(30, 8), AUD_GOTO(0), AUD_END,
};

/** X-Naut audience member seen from behind: confused loop (slot 13). */
s32 audience_daisan_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(31, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member facing the camera: idle loop (slot 0). */
s32 audience_teresa_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(32, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member seen from behind: idle loop (slot 1). */
s32 audience_teresa_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(33, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_teresa_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(34, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_teresa_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(35, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_teresa_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(36, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_teresa_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(38, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_teresa_itemthrow_back[] = {
    AUD_FRAME(39, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 3
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Boo audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_teresa_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(43, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_teresa_damage_front[] = {
    AUD_FRAME(43, 32), AUD_END,
};

/** Boo audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_teresa_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(44, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_teresa_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(45, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member facing the camera: confused loop (slot 12). */
s32 audience_teresa_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(46, 8), AUD_GOTO(0), AUD_END,
};

/** Boo audience member seen from behind: confused loop (slot 13). */
s32 audience_teresa_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(47, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member facing the camera: idle loop (slot 0). */
s32 audience_hammer_bros_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(50, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member seen from behind: idle loop (slot 1). */
s32 audience_hammer_bros_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(51, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_hammer_bros_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(52, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_hammer_bros_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(53, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_hammer_bros_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(55, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_hammer_bros_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(56, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_hammer_bros_itemthrow_back[] = {
    AUD_FRAME(57, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 4
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Hammer Bro audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_hammer_bros_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(61, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_hammer_bros_damage_front[] = {
    AUD_FRAME(61, 32), AUD_END,
};

/** Hammer Bro audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_hammer_bros_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(62, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_hammer_bros_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(63, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member facing the camera: confused loop (slot 12). */
s32 audience_hammer_bros_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(64, 8), AUD_GOTO(0), AUD_END,
};

/** Hammer Bro audience member seen from behind: confused loop (slot 13). */
s32 audience_hammer_bros_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(65, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member facing the camera: idle loop (slot 0). */
s32 audience_honenoko_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(66, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member seen from behind: idle loop (slot 1). */
s32 audience_honenoko_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(67, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_honenoko_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(68, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_honenoko_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(69, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_honenoko_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(71, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_honenoko_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(72, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_honenoko_itemthrow_back[] = {
    AUD_FRAME(73, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 5
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Dull Bones audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_honenoko_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(77, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_honenoko_damage_front[] = {
    AUD_FRAME(77, 32), AUD_END,
};

/** Dull Bones audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_honenoko_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(78, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_honenoko_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(79, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member facing the camera: confused loop (slot 12). */
s32 audience_honenoko_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(80, 8), AUD_GOTO(0), AUD_END,
};

/** Dull Bones audience member seen from behind: confused loop (slot 13). */
s32 audience_honenoko_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(81, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member facing the camera: idle loop (slot 0). */
s32 audience_heiho_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(82, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member seen from behind: idle loop (slot 1). */
s32 audience_heiho_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(83, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_heiho_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(84, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_heiho_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(85, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_heiho_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(87, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_heiho_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(88, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_heiho_itemthrow_back[] = {
    AUD_FRAME(89, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 6
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Shy Guy audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_heiho_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(93, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_heiho_damage_front[] = {
    AUD_FRAME(93, 32), AUD_END,
};

/** Shy Guy audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_heiho_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(94, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_heiho_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(95, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member facing the camera: confused loop (slot 12). */
s32 audience_heiho_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(96, 8), AUD_GOTO(0), AUD_END,
};

/** Shy Guy audience member seen from behind: confused loop (slot 13). */
s32 audience_heiho_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(97, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member facing the camera: idle loop (slot 0). */
s32 audience_pansy_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(100, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member seen from behind: idle loop (slot 1). */
s32 audience_pansy_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(101, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_pansy_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(102, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_pansy_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(103, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_pansy_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(104, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_pansy_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(106, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_pansy_itemthrow_back[] = {
    AUD_FRAME(107, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 7
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Crazee Dayzee audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_pansy_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(111, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_pansy_damage_front[] = {
    AUD_FRAME(111, 32), AUD_END,
};

/** Crazee Dayzee audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_pansy_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(112, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_pansy_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(113, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member facing the camera: confused loop (slot 12). */
s32 audience_pansy_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(114, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member seen from behind: confused loop (slot 13). */
s32 audience_pansy_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(115, 8), AUD_GOTO(0), AUD_END,
};

/** Crazee Dayzee audience member facing the camera: singing its sleep song in its phase event (slot 14). */
s32 audience_pansy_sing_front[] = {
    AUD_LABEL(0), AUD_FRAME(116, 16), AUD_FRAME(117, 16), AUD_GOTO(0), AUD_END,
};

/** Puni audience member facing the camera: idle loop (slot 0). */
s32 audience_puni_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(118, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member seen from behind: idle loop (slot 1). */
s32 audience_puni_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(119, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_puni_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(120, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_puni_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(121, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_puni_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(123, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_puni_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(124, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_puni_itemthrow_back[] = {
    AUD_FRAME(125, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 8
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Puni audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_puni_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(129, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_puni_damage_front[] = {
    AUD_FRAME(129, 32), AUD_END,
};

/** Puni audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_puni_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(130, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_puni_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(131, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member facing the camera: confused loop (slot 12). */
s32 audience_puni_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(132, 8), AUD_GOTO(0), AUD_END,
};

/** Puni audience member seen from behind: confused loop (slot 13). */
s32 audience_puni_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(133, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member facing the camera: idle loop (slot 0). */
s32 audience_nokonoko_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(134, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: idle loop (slot 1). */
s32 audience_nokonoko_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(135, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_nokonoko_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(136, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_nokonoko_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(137, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_nokonoko_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(138, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_nokonoko_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(140, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_nokonoko_itemthrow_back[] = {
    AUD_FRAME(141, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 9
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Koopa audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_nokonoko_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(145, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_nokonoko_damage_front[] = {
    AUD_FRAME(145, 32), AUD_END,
};

/** Koopa audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_nokonoko_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(146, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_nokonoko_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(147, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member facing the camera: confused loop (slot 12). */
s32 audience_nokonoko_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(148, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: confused loop (slot 13). */
s32 audience_nokonoko_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(149, 8), AUD_GOTO(0), AUD_END,
};

/** Koopa audience member seen from behind: hiding in its shell in its phase event (slot 15). */
s32 audience_nokonoko_shell_back[] = {
    AUD_LABEL(0), AUD_FRAME(150, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member facing the camera: idle loop (slot 0). */
s32 audience_heavy_bomb_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(151, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: idle loop (slot 1). */
s32 audience_heavy_bomb_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(152, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_heavy_bomb_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(153, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_heavy_bomb_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(154, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_heavy_bomb_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(156, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_heavy_bomb_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(157, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_heavy_bomb_itemthrow_back[] = {
    AUD_FRAME(158, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 10
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Bulky Bob-omb audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_heavy_bomb_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(162, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_heavy_bomb_damage_front[] = {
    AUD_FRAME(162, 32), AUD_END,
};

/** Bulky Bob-omb audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_heavy_bomb_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(163, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_heavy_bomb_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(164, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member facing the camera: confused loop (slot 12). */
s32 audience_heavy_bomb_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(165, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: confused loop (slot 13). */
s32 audience_heavy_bomb_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(166, 8), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member facing the camera: fuse lit before it explodes (slot 16). */
s32 audience_heavy_bomb_ignite_front[] = {
    AUD_LABEL(0), AUD_FRAME(167, 2), AUD_FRAME(168, 2), AUD_GOTO(0), AUD_END,
};

/** Bulky Bob-omb audience member seen from behind: fuse lit before it explodes (slot 17). */
s32 audience_heavy_bomb_ignite_back[] = {
    AUD_LABEL(0), AUD_FRAME(169, 2), AUD_FRAME(170, 2), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member facing the camera: idle loop (slot 0). */
s32 audience_kuriboo_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(171, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member seen from behind: idle loop (slot 1). */
s32 audience_kuriboo_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(172, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_kuriboo_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(173, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_kuriboo_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(174, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_kuriboo_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(175, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_kuriboo_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(177, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_kuriboo_itemthrow_back[] = {
    AUD_FRAME(178, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 11
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Goomba audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_kuriboo_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(182, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_kuriboo_damage_front[] = {
    AUD_FRAME(182, 32), AUD_END,
};

/** Goomba audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_kuriboo_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(183, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_kuriboo_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(184, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member facing the camera: confused loop (slot 12). */
s32 audience_kuriboo_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(185, 8), AUD_GOTO(0), AUD_END,
};

/** Goomba audience member seen from behind: confused loop (slot 13). */
s32 audience_kuriboo_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(186, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member facing the camera: idle loop (slot 0). */
s32 audience_pakkun_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(187, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member seen from behind: idle loop (slot 1). */
s32 audience_pakkun_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(188, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_pakkun_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(189, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_pakkun_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(190, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_pakkun_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(192, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_pakkun_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(193, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_pakkun_itemthrow_back[] = {
    AUD_FRAME(194, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 12
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Piranha Plant audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_pakkun_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(198, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_pakkun_damage_front[] = {
    AUD_FRAME(198, 32), AUD_END,
};

/** Piranha Plant audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_pakkun_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(199, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_pakkun_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(200, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member facing the camera: confused loop (slot 12). */
s32 audience_pakkun_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(201, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member seen from behind: confused loop (slot 13). */
s32 audience_pakkun_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(202, 8), AUD_GOTO(0), AUD_END,
};

/** Piranha Plant audience member facing the camera: eating a nearby member in its phase event (slot 18). */
s32 audience_pakkun_eat_front[] = {
    AUD_FRAME(203, 10), AUD_FRAME(204, 10), AUD_END,
};

/** Luigi audience member facing the camera: idle loop (slot 0). */
s32 audience_luigi_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(0, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member seen from behind: idle loop (slot 1). */
s32 audience_luigi_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(1, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member facing the camera: hopping, for idle hops and cheers (slot 2). */
s32 audience_luigi_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(2, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member seen from behind: hopping, for idle hops and cheers (slot 3). */
s32 audience_luigi_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(3, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member seen from behind: arms up, for cheering and booing (slot 4). */
s32 audience_luigi_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(5, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member seen from behind: holding up an item before throwing it (slot 5). */
s32 audience_luigi_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(6, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member seen from behind: throwing the item, one 32-frame pose (slot 6). */
s32 audience_luigi_itemthrow_back[] = {
    AUD_FRAME(7, 32), AUD_END,
};

#if VERSION <= VERSION_DEMO_US
#define AUD_RUN_FRONT 13
#include "../src/data/battle/battle_audience_run_front.inc"
#undef AUD_RUN_FRONT
#endif

/** Luigi audience member facing the camera: knocked-over loop (slot 8). */
s32 audience_luigi_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(11, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member facing the camera: hit by Mario or a partner, one 32-frame pose (slot 9). */
s32 audience_luigi_damage_front[] = {
    AUD_FRAME(11, 32), AUD_END,
};

/** Luigi audience member facing the camera: asleep after a Crazee Dayzee's song (slot 10). */
s32 audience_luigi_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(12, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member seen from behind: asleep after a Crazee Dayzee's song (slot 11). */
s32 audience_luigi_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(13, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member facing the camera: confused loop (slot 12). */
s32 audience_luigi_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(14, 8), AUD_GOTO(0), AUD_END,
};

/** Luigi audience member seen from behind: confused loop (slot 13). */
s32 audience_luigi_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(15, 8), AUD_GOTO(0), AUD_END,
};

#if VERSION > VERSION_DEMO_US
#define AUD_ITEM_GROUP 2
#include "../src/data/battle/battle_audience_item.inc"
#undef AUD_ITEM_GROUP
#else
#include "../src/data/battle/battle_audience_anim.inc"
#include "../src/data/battle/battle_audience_item.inc"
#include "../src/data/battle/battle_audience_kind.inc"
#endif
