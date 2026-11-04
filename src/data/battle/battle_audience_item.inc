// Thrown-item tables: two groups apart in retail (AUD_ITEM_GROUP 1, 2), all together in the early demos (no group set).
#pragma once off
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 1
/**
 * Items a Toad in the audience can throw, with relative weights (the {0, 0} row ends each table):
 * healing items, mostly Mushrooms, and a few attack items.
 */
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
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items an X-Naut in the audience can throw: only junk (a rock or a can) that damages Mario. */
BattleAudienceItemKind audience_item_daisan[] = {
    {ITEM_AUDIENCE_ROCK, 10},
    {ITEM_AUDIENCE_CAN, 10},
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items a Boo in the audience can throw: Repel Cape, Boo's Sheet, Spite Pouch, or junk. */
BattleAudienceItemKind audience_item_teresa[] = {
    {ITEM_REPEL_CAPE, 10},
    {ITEM_BOOS_SHEET, 20},
    {ITEM_SPITE_POUCH, 10},
    {ITEM_AUDIENCE_ROCK, 10},
    {ITEM_AUDIENCE_CAN, 10},
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items a Hammer Bro in the audience can throw: only a hammer that damages Mario. */
BattleAudienceItemKind audience_item_hammer_bros[] = {
    {ITEM_AUDIENCE_HAMMER, 10},
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items a Dull Bones in the audience can throw: a bone, a rock or a can, all junk that damages Mario. */
BattleAudienceItemKind audience_item_honenoko[] = {
    {ITEM_AUDIENCE_BONE, 10},
    {ITEM_AUDIENCE_ROCK, 10},
    {ITEM_AUDIENCE_CAN, 10},
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 1
BattleAudienceItemKind audience_item_heiho[] = {
    {ITEM_ICE_STORM, 10},
    {ITEM_EARTH_QUAKE, 10},
    {ITEM_VOLT_SHROOM, 10},
    {ITEM_SLEEPY_SHEEP, 10},
    {ITEM_DIZZY_DIAL, 10},
    {ITEM_RUIN_POWDER, 10},
    {ITEM_POWER_PUNCH, 10},
    {ITEM_STOPWATCH, 10},
#if VERSION <= VERSION_DEMO_JP
    {ITEM_TRADE_OFF, 10},
#endif
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
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items a Crazee Dayzee in the audience can throw: none (empty table). */
BattleAudienceItemKind audience_item_pansy[] = {
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 1
/** Items a Puni in the audience can throw: coins, hearts, flowers and mostly healing or slowing items. */
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
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 1
/** Items a Koopa in the audience can throw: coins, hearts, flowers, Courage Shell, POW Block, or junk. */
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
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items a Bulky Bob-omb in the audience can throw: none (empty table). */
BattleAudienceItemKind audience_item_heavy_bomb[] = {
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items a Goomba in the audience can throw: only junk (a rock or a can) that damages Mario. */
BattleAudienceItemKind audience_item_kuriboo[] = {
    {ITEM_AUDIENCE_ROCK, 20},
    {ITEM_AUDIENCE_CAN, 20},
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 2
/** Items a Piranha Plant in the audience can throw: none (empty table). */
BattleAudienceItemKind audience_item_pakkun[] = {
    {0, 0},
};
#endif
#if !defined(AUD_ITEM_GROUP) || AUD_ITEM_GROUP == 1
/** Items Luigi throws from the audience: coins, hearts, flowers and good healing items, or a rock. */
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
#endif
