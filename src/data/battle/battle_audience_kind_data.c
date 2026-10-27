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
BattleAudienceItemKind audience_item_heiho[17];
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

s32 audience_kinopio_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(8, 4), AUD_FRAME(9, 4), AUD_FRAME(10, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(24, 4), AUD_FRAME(25, 4), AUD_FRAME(26, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(40, 4), AUD_FRAME(41, 4), AUD_FRAME(42, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(58, 4), AUD_FRAME(59, 4), AUD_FRAME(60, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(74, 4), AUD_FRAME(75, 4), AUD_FRAME(76, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(90, 4), AUD_FRAME(91, 4), AUD_FRAME(92, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(108, 4), AUD_FRAME(109, 4), AUD_FRAME(110, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(126, 4), AUD_FRAME(127, 4), AUD_FRAME(128, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(142, 4), AUD_FRAME(143, 4), AUD_FRAME(144, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(159, 4), AUD_FRAME(160, 4), AUD_FRAME(161, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(179, 4), AUD_FRAME(180, 4), AUD_FRAME(181, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(195, 4), AUD_FRAME(196, 4), AUD_FRAME(197, 4), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_run_front[] = {
    AUD_LABEL(0), AUD_FRAME(8, 4), AUD_FRAME(9, 4), AUD_FRAME(10, 4), AUD_GOTO(0), AUD_END,
};

s32* audience_anim_kinopio[] = {
    audience_kinopio_wait_front,
    audience_kinopio_wait_back,
    audience_kinopio_jump_front,
    audience_kinopio_jump_back,
    audience_kinopio_raise_back,
    audience_kinopio_itemon_back,
    audience_kinopio_itemthrow_back,
    audience_kinopio_run_front,
    audience_kinopio_tumble_front,
    audience_kinopio_damage_front,
    audience_kinopio_sleep_front,
    audience_kinopio_sleep_back,
    audience_kinopio_confuse_front,
    audience_kinopio_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_daisan[] = {
    audience_daisan_wait_front,
    audience_daisan_wait_back,
    audience_daisan_jump_front,
    audience_daisan_jump_back,
    audience_daisan_raise_back,
    audience_daisan_itemon_back,
    audience_daisan_itemthrow_back,
    audience_daisan_run_front,
    audience_daisan_tumble_front,
    audience_daisan_damage_front,
    audience_daisan_sleep_front,
    audience_daisan_sleep_back,
    audience_daisan_confuse_front,
    audience_daisan_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_teresa[] = {
    audience_teresa_wait_front,
    audience_teresa_wait_back,
    audience_teresa_jump_front,
    audience_teresa_jump_back,
    audience_teresa_raise_back,
    audience_teresa_itemon_back,
    audience_teresa_itemthrow_back,
    audience_teresa_run_front,
    audience_teresa_tumble_front,
    audience_teresa_damage_front,
    audience_teresa_sleep_front,
    audience_teresa_sleep_back,
    audience_teresa_confuse_front,
    audience_teresa_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_hammer_bros[] = {
    audience_hammer_bros_wait_front,
    audience_hammer_bros_wait_back,
    audience_hammer_bros_jump_front,
    audience_hammer_bros_jump_back,
    audience_hammer_bros_raise_back,
    audience_hammer_bros_itemon_back,
    audience_hammer_bros_itemthrow_back,
    audience_hammer_bros_run_front,
    audience_hammer_bros_tumble_front,
    audience_hammer_bros_damage_front,
    audience_hammer_bros_sleep_front,
    audience_hammer_bros_sleep_back,
    audience_hammer_bros_confuse_front,
    audience_hammer_bros_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_honenoko[] = {
    audience_honenoko_wait_front,
    audience_honenoko_wait_back,
    audience_honenoko_jump_front,
    audience_honenoko_jump_back,
    audience_honenoko_raise_back,
    audience_honenoko_itemon_back,
    audience_honenoko_itemthrow_back,
    audience_honenoko_run_front,
    audience_honenoko_tumble_front,
    audience_honenoko_damage_front,
    audience_honenoko_sleep_front,
    audience_honenoko_sleep_back,
    audience_honenoko_confuse_front,
    audience_honenoko_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_heiho[] = {
    audience_heiho_wait_front,
    audience_heiho_wait_back,
    audience_heiho_jump_front,
    audience_heiho_jump_back,
    audience_heiho_raise_back,
    audience_heiho_itemon_back,
    audience_heiho_itemthrow_back,
    audience_heiho_run_front,
    audience_heiho_tumble_front,
    audience_heiho_damage_front,
    audience_heiho_sleep_front,
    audience_heiho_sleep_back,
    audience_heiho_confuse_front,
    audience_heiho_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_pansy[] = {
    audience_pansy_wait_front,
    audience_pansy_wait_back,
    audience_pansy_jump_front,
    audience_pansy_jump_back,
    audience_pansy_raise_back,
    audience_pansy_itemon_back,
    audience_pansy_itemthrow_back,
    audience_pansy_run_front,
    audience_pansy_tumble_front,
    audience_pansy_damage_front,
    audience_pansy_sleep_front,
    audience_pansy_sleep_back,
    audience_pansy_confuse_front,
    audience_pansy_confuse_back,
    audience_pansy_sing_front,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_puni[] = {
    audience_puni_wait_front,
    audience_puni_wait_back,
    audience_puni_jump_front,
    audience_puni_jump_back,
    audience_puni_raise_back,
    audience_puni_itemon_back,
    audience_puni_itemthrow_back,
    audience_puni_run_front,
    audience_puni_tumble_front,
    audience_puni_damage_front,
    audience_puni_sleep_front,
    audience_puni_sleep_back,
    audience_puni_confuse_front,
    audience_puni_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_nokonoko[] = {
    audience_nokonoko_wait_front,
    audience_nokonoko_wait_back,
    audience_nokonoko_jump_front,
    audience_nokonoko_jump_back,
    audience_nokonoko_raise_back,
    audience_nokonoko_itemon_back,
    audience_nokonoko_itemthrow_back,
    audience_nokonoko_run_front,
    audience_nokonoko_tumble_front,
    audience_nokonoko_damage_front,
    audience_nokonoko_sleep_front,
    audience_nokonoko_sleep_back,
    audience_nokonoko_confuse_front,
    audience_nokonoko_confuse_back,
    NULL,
    audience_nokonoko_shell_back,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_heavy_bomb[] = {
    audience_heavy_bomb_wait_front,
    audience_heavy_bomb_wait_back,
    audience_heavy_bomb_jump_front,
    audience_heavy_bomb_jump_back,
    audience_heavy_bomb_raise_back,
    audience_heavy_bomb_itemon_back,
    audience_heavy_bomb_itemthrow_back,
    audience_heavy_bomb_run_front,
    audience_heavy_bomb_tumble_front,
    audience_heavy_bomb_damage_front,
    audience_heavy_bomb_sleep_front,
    audience_heavy_bomb_sleep_back,
    audience_heavy_bomb_confuse_front,
    audience_heavy_bomb_confuse_back,
    NULL,
    NULL,
    audience_heavy_bomb_ignite_front,
    audience_heavy_bomb_ignite_back,
    NULL,
};

s32* audience_anim_kuriboo[] = {
    audience_kuriboo_wait_front,
    audience_kuriboo_wait_back,
    audience_kuriboo_jump_front,
    audience_kuriboo_jump_back,
    audience_kuriboo_raise_back,
    audience_kuriboo_itemon_back,
    audience_kuriboo_itemthrow_back,
    audience_kuriboo_run_front,
    audience_kuriboo_tumble_front,
    audience_kuriboo_damage_front,
    audience_kuriboo_sleep_front,
    audience_kuriboo_sleep_back,
    audience_kuriboo_confuse_front,
    audience_kuriboo_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

s32* audience_anim_pakkun[] = {
    audience_pakkun_wait_front,
    audience_pakkun_wait_back,
    audience_pakkun_jump_front,
    audience_pakkun_jump_back,
    audience_pakkun_raise_back,
    audience_pakkun_itemon_back,
    audience_pakkun_itemthrow_back,
    audience_pakkun_run_front,
    audience_pakkun_tumble_front,
    audience_pakkun_damage_front,
    audience_pakkun_sleep_front,
    audience_pakkun_sleep_back,
    audience_pakkun_confuse_front,
    audience_pakkun_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    audience_pakkun_eat_front,
};

s32* audience_anim_luigi[] = {
    audience_luigi_wait_front,
    audience_luigi_wait_back,
    audience_luigi_jump_front,
    audience_luigi_jump_back,
    audience_luigi_raise_back,
    audience_luigi_itemon_back,
    audience_luigi_itemthrow_back,
    audience_luigi_run_front,
    audience_luigi_tumble_front,
    audience_luigi_damage_front,
    audience_luigi_sleep_front,
    audience_luigi_sleep_back,
    audience_luigi_confuse_front,
    audience_luigi_confuse_back,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

BattleAudienceItemKind audience_item_kinopio[] = {
    {ITEM_COIN, 10},
    {ITEM_HEART, 10},
    {ITEM_FLOWER, 10},
    {ITEM_MUSHROOM, 50},
    {ITEM_SUPER_SHROOM, 10},
    {ITEM_ULTRA_SHROOM, 1},
    {ITEM_LIFE_SHROOM, 5},
    {ITEM_HONEY_SYRUP, 10},
    {ITEM_FIRE_FLOWER, 10},
    {ITEM_THUNDER_RAGE, 5},
    {ITEM_SHOOTING_STAR, 5},
    {0, 0},
};

BattleAudienceItemKind audience_item_heiho[] = {
    {ITEM_ICE_STORM, 10},
    {ITEM_EARTH_QUAKE, 10},
    {ITEM_VOLT_SHROOM, 10},
    {ITEM_SLEEPY_SHEEP, 10},
    {ITEM_DIZZY_DIAL, 10},
    {ITEM_RUIN_POWDER, 10},
    {ITEM_POWER_PUNCH, 10},
    {ITEM_STOPWATCH, 10},
    {ITEM_MINI_MR_MINI, 10},
    {ITEM_MR_SOFTENER, 10},
    {ITEM_POINT_SWAP, 10},
    {ITEM_FRIGHT_MASK, 10},
    {ITEM_MYSTERY, 10},
    {ITEM_HP_DRAIN_ITEM, 10},
    {ITEM_AUDIENCE_ROCK, 30},
    {ITEM_AUDIENCE_CAN, 30},
    {0, 0},
};

BattleAudienceItemKind audience_item_puni[] = {
    {ITEM_COIN, 10},
    {ITEM_HEART, 10},
    {ITEM_FLOWER, 10},
    {ITEM_DRIED_SHROOM, 30},
    {ITEM_HONEY_SYRUP, 20},
    {ITEM_MAPLE_SYRUP, 5},
    {ITEM_JAMMIN_JELLY, 1},
    {ITEM_TASTY_TONIC, 10},
    {ITEM_SLOW_SHROOM, 10},
    {ITEM_GRADUAL_SYRUP, 10},
    {0, 0},
};

BattleAudienceItemKind audience_item_nokonoko[] = {
    {ITEM_COIN, 10},
    {ITEM_HEART, 10},
    {ITEM_FLOWER, 10},
    {ITEM_COURAGE_SHELL, 10},
    {ITEM_POW_BLOCK, 10},
    {ITEM_AUDIENCE_ROCK, 10},
    {ITEM_AUDIENCE_CAN, 10},
    {0, 0},
};

BattleAudienceItemKind audience_item_luigi[] = {
    {ITEM_COIN, 10},
    {ITEM_HEART, 10},
    {ITEM_FLOWER, 10},
    {ITEM_MUSHROOM, 10},
    {ITEM_SUPER_SHROOM, 10},
    {ITEM_ULTRA_SHROOM, 10},
    {ITEM_HONEY_SYRUP, 10},
    {ITEM_MAPLE_SYRUP, 10},
    {ITEM_JAMMIN_JELLY, 10},
    {ITEM_SHOOTING_STAR, 10},
    {ITEM_AUDIENCE_ROCK, 10},
    {0, 0},
};

BattleAudienceKind audience_kind[] = {
    {0, audience_anim_kinopio, audience_item_kinopio, 10.0f, 25.0f, 3.0f, 1.0f, 1.0f},
    {0, audience_anim_daisan, audience_item_daisan, 10.0f, 25.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_teresa, audience_item_teresa, 0.0f, 30.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_hammer_bros, audience_item_hammer_bros, 10.0f, 20.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_honenoko, audience_item_honenoko, 10.0f, 25.0f, 0.0f, 1.0f, 1.0f},
    {0, audience_anim_heiho, audience_item_heiho, 10.0f, 25.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_pansy, audience_item_pansy, 10.0f, 10.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_puni, audience_item_puni, 10.0f, 20.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_nokonoko, audience_item_nokonoko, 10.0f, 25.0f, 1.0f, 1.0f, 1.0f},
    {AUDIENCE_KIND_WIDE, audience_anim_heavy_bomb, audience_item_heavy_bomb, 0.0f, 50.0f, 1.0f, 1.5f, 1.5f},
    {0, audience_anim_kuriboo, audience_item_kuriboo, 10.0f, 25.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_pakkun, audience_item_pakkun, 0.0f, 20.0f, 1.0f, 1.0f, 1.0f},
    {0, audience_anim_luigi, audience_item_luigi, 0.0f, 40.0f, 1.0f, 1.2f, 1.2f},
};

s32 audience_kinopio_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(0, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(1, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(2, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(3, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(4, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(6, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_itemthrow_back[] = {
    AUD_FRAME(7, 32), AUD_END,
};

s32 audience_kinopio_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(11, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_damage_front[] = {
    AUD_FRAME(11, 32), AUD_END,
};

s32 audience_kinopio_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(12, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(13, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(14, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kinopio_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(15, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(16, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(17, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(18, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(19, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(21, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(22, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_itemthrow_back[] = {
    AUD_FRAME(23, 32), AUD_END,
};

s32 audience_daisan_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(27, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_damage_front[] = {
    AUD_FRAME(27, 32), AUD_END,
};

s32 audience_daisan_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(28, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(29, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(30, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_daisan_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(31, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(32, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(33, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(34, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(35, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(36, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(38, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_itemthrow_back[] = {
    AUD_FRAME(39, 32), AUD_END,
};

s32 audience_teresa_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(43, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_damage_front[] = {
    AUD_FRAME(43, 32), AUD_END,
};

s32 audience_teresa_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(44, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(45, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(46, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_teresa_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(47, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(50, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(51, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(52, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(53, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(55, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(56, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_itemthrow_back[] = {
    AUD_FRAME(57, 32), AUD_END,
};

s32 audience_hammer_bros_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(61, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_damage_front[] = {
    AUD_FRAME(61, 32), AUD_END,
};

s32 audience_hammer_bros_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(62, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(63, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(64, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_hammer_bros_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(65, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(66, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(67, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(68, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(69, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(71, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(72, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_itemthrow_back[] = {
    AUD_FRAME(73, 32), AUD_END,
};

s32 audience_honenoko_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(77, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_damage_front[] = {
    AUD_FRAME(77, 32), AUD_END,
};

s32 audience_honenoko_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(78, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(79, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(80, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_honenoko_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(81, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(82, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(83, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(84, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(85, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(87, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(88, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_itemthrow_back[] = {
    AUD_FRAME(89, 32), AUD_END,
};

s32 audience_heiho_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(93, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_damage_front[] = {
    AUD_FRAME(93, 32), AUD_END,
};

s32 audience_heiho_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(94, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(95, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(96, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heiho_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(97, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(100, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(101, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(102, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(103, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(104, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(106, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_itemthrow_back[] = {
    AUD_FRAME(107, 32), AUD_END,
};

s32 audience_pansy_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(111, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_damage_front[] = {
    AUD_FRAME(111, 32), AUD_END,
};

s32 audience_pansy_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(112, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(113, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(114, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(115, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pansy_sing_front[] = {
    AUD_LABEL(0), AUD_FRAME(116, 16), AUD_FRAME(117, 16), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(118, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(119, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(120, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(121, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(123, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(124, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_itemthrow_back[] = {
    AUD_FRAME(125, 32), AUD_END,
};

s32 audience_puni_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(129, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_damage_front[] = {
    AUD_FRAME(129, 32), AUD_END,
};

s32 audience_puni_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(130, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(131, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(132, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_puni_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(133, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(134, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(135, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(136, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(137, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(138, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(140, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_itemthrow_back[] = {
    AUD_FRAME(141, 32), AUD_END,
};

s32 audience_nokonoko_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(145, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_damage_front[] = {
    AUD_FRAME(145, 32), AUD_END,
};

s32 audience_nokonoko_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(146, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(147, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(148, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(149, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_nokonoko_shell_back[] = {
    AUD_LABEL(0), AUD_FRAME(150, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(151, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(152, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(153, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(154, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(156, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(157, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_itemthrow_back[] = {
    AUD_FRAME(158, 32), AUD_END,
};

s32 audience_heavy_bomb_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(162, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_damage_front[] = {
    AUD_FRAME(162, 32), AUD_END,
};

s32 audience_heavy_bomb_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(163, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(164, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(165, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(166, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_ignite_front[] = {
    AUD_LABEL(0), AUD_FRAME(167, 2), AUD_FRAME(168, 2), AUD_GOTO(0), AUD_END,
};

s32 audience_heavy_bomb_ignite_back[] = {
    AUD_LABEL(0), AUD_FRAME(169, 2), AUD_FRAME(170, 2), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(171, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(172, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(173, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(174, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(175, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(177, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_itemthrow_back[] = {
    AUD_FRAME(178, 32), AUD_END,
};

s32 audience_kuriboo_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(182, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_damage_front[] = {
    AUD_FRAME(182, 32), AUD_END,
};

s32 audience_kuriboo_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(183, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(184, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(185, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_kuriboo_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(186, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(187, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(188, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(189, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(190, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(192, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(193, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_itemthrow_back[] = {
    AUD_FRAME(194, 32), AUD_END,
};

s32 audience_pakkun_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(198, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_damage_front[] = {
    AUD_FRAME(198, 32), AUD_END,
};

s32 audience_pakkun_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(199, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(200, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(201, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(202, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_pakkun_eat_front[] = {
    AUD_FRAME(203, 10), AUD_FRAME(204, 10), AUD_END,
};

s32 audience_luigi_wait_front[] = {
    AUD_LABEL(0), AUD_FRAME(0, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_wait_back[] = {
    AUD_LABEL(0), AUD_FRAME(1, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_jump_front[] = {
    AUD_LABEL(0), AUD_FRAME(2, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_jump_back[] = {
    AUD_LABEL(0), AUD_FRAME(3, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_raise_back[] = {
    AUD_LABEL(0), AUD_FRAME(5, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_itemon_back[] = {
    AUD_LABEL(0), AUD_FRAME(6, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_itemthrow_back[] = {
    AUD_FRAME(7, 32), AUD_END,
};

s32 audience_luigi_tumble_front[] = {
    AUD_LABEL(0), AUD_FRAME(11, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_damage_front[] = {
    AUD_FRAME(11, 32), AUD_END,
};

s32 audience_luigi_sleep_front[] = {
    AUD_LABEL(0), AUD_FRAME(12, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_sleep_back[] = {
    AUD_LABEL(0), AUD_FRAME(13, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_confuse_front[] = {
    AUD_LABEL(0), AUD_FRAME(14, 8), AUD_GOTO(0), AUD_END,
};

s32 audience_luigi_confuse_back[] = {
    AUD_LABEL(0), AUD_FRAME(15, 8), AUD_GOTO(0), AUD_END,
};

BattleAudienceItemKind audience_item_daisan[] = {
    {ITEM_AUDIENCE_ROCK, 10},
    {ITEM_AUDIENCE_CAN, 10},
    {0, 0},
};

BattleAudienceItemKind audience_item_teresa[] = {
    {ITEM_REPEL_CAPE, 10},
    {ITEM_BOOS_SHEET, 20},
    {ITEM_SPITE_POUCH, 10},
    {ITEM_AUDIENCE_ROCK, 10},
    {ITEM_AUDIENCE_CAN, 10},
    {0, 0},
};

BattleAudienceItemKind audience_item_hammer_bros[] = {
    {ITEM_AUDIENCE_HAMMER, 10},
    {0, 0},
};

BattleAudienceItemKind audience_item_honenoko[] = {
    {ITEM_AUDIENCE_BONE, 10},
    {ITEM_AUDIENCE_ROCK, 10},
    {ITEM_AUDIENCE_CAN, 10},
    {0, 0},
};

BattleAudienceItemKind audience_item_pansy[] = {
    {0, 0},
};

BattleAudienceItemKind audience_item_heavy_bomb[] = {
    {0, 0},
};

BattleAudienceItemKind audience_item_kuriboo[] = {
    {ITEM_AUDIENCE_ROCK, 20},
    {ITEM_AUDIENCE_CAN, 20},
    {0, 0},
};

BattleAudienceItemKind audience_item_pakkun[] = {
    {0, 0},
};

