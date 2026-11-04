// The kind table: ahead of the member tables in retail, last in the early demos.
/**
 * Every audience kind, indexed by member type (0 Toad .. 12 Luigi):
 * its animations and thrown items, where thrown items appear, its star power weight and its sprite scale.
 */
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

