#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "driver/npcdrv.h"
#include "mario/mario_pouch.h"
#include "battle/battle_database_common.h"

ItemDropData battle_item_gundan_zako[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_FIRE_FLOWER, 20, 20},
    {ITEM_STOPWATCH, 10, 10},
    {ITEM_DIZZY_DIAL, 10, 10},
    {ITEM_HP_PLUS, 2, 2},
    {ITEM_FP_PLUS, 0, 2},
    {ITEM_SHRINK_STOMP, 0, 2},
    {0},
};

ItemDropData battle_item_gundan_zako_magician[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_SHOOTING_STAR, 20, 20},
    {ITEM_STOPWATCH, 10, 10},
    {ITEM_RUIN_POWDER, 10, 10},
    {ITEM_P_DOWN_D_UP, 2, 1},
    {ITEM_P_DOWN_D_UP_P, 2, 1},
    {ITEM_FLOWER_FINDER, 0, 2},
    {0},
};

ItemDropData battle_item_gundan_zako_elite[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_THUNDER_RAGE, 20, 20},
    {ITEM_STOPWATCH, 10, 10},
    {ITEM_BOOS_SHEET, 10, 10},
    {ITEM_P_UP_D_DOWN, 2, 1},
    {ITEM_P_UP_D_DOWN_P, 2, 1},
    {ITEM_HEART_FINDER, 0, 2},
    {0},
};

ItemDropData battle_item_hinnya[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_THUNDER_BOLT, 5, 5},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_FRIGHT_MASK, 0, 10},
    {ITEM_PRETTY_LUCKY, 2, 1},
    {ITEM_PRETTY_LUCKY_P, 2, 1},
    {ITEM_POWER_SMASH, 0, 1},
    {0},
};

ItemDropData battle_item_hannya[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_THUNDER_BOLT, 5, 5},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_MYSTERY, 0, 10},
    {ITEM_PRETTY_LUCKY, 2, 1},
    {ITEM_PRETTY_LUCKY_P, 2, 1},
    {ITEM_POWER_SMASH, 0, 1},
    {0},
};

ItemDropData battle_item_hennya[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_THUNDER_RAGE, 10, 10},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_SPITE_POUCH, 10, 10},
    {ITEM_PRETTY_LUCKY, 2, 2},
    {ITEM_PRETTY_LUCKY_P, 2, 2},
    {ITEM_POWER_SMASH, 0, 2},
    {0},
};

ItemDropData battle_item_kuriboo[] = {
    {0, 200, 300},
    {ITEM_DRIED_SHROOM, 10, 10},
    {ITEM_TASTY_TONIC, 0, 5},
    {ITEM_FIRE_FLOWER, 10, 10},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_CLOSE_CALL, 2, 1},
    {ITEM_CLOSE_CALL_P, 2, 1},
    {ITEM_POWER_JUMP, 0, 1},
    {0},
};

ItemDropData battle_item_togekuri[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 10, 10},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_CLOSE_CALL, 2, 1},
    {ITEM_CLOSE_CALL_P, 2, 1},
    {ITEM_SOFT_STOMP, 0, 1},
    {0},
};

ItemDropData battle_item_patakuri[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 10, 10},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_DIZZY_DIAL, 5, 5},
    {ITEM_CLOSE_CALL, 2, 1},
    {ITEM_CLOSE_CALL_P, 2, 1},
    {ITEM_MULTIBOUNCE, 0, 1},
    {0},
};

ItemDropData battle_item_hyper_kuriboo[] = {
    {0, 200, 300},
    {ITEM_LIFE_SHROOM, 5, 5},
    {ITEM_TASTY_TONIC, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_BOOS_SHEET, 20, 20},
    {ITEM_REPEL_CAPE, 10, 10},
    {ITEM_LAST_STAND, 2, 1},
    {ITEM_LAST_STAND_P, 2, 1},
    {ITEM_CHARGE, 0, 2},
    {0},
};

ItemDropData battle_item_hyper_togekuri[] = {
    {0, 200, 300},
    {ITEM_LIFE_SHROOM, 5, 5},
    {ITEM_TASTY_TONIC, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_BOOS_SHEET, 20, 20},
    {ITEM_POWER_PUNCH, 10, 10},
    {ITEM_LAST_STAND, 2, 1},
    {ITEM_LAST_STAND_P, 2, 1},
    {ITEM_CHARGE_P, 0, 2},
    {0},
};

ItemDropData battle_item_hyper_patakuri[] = {
    {0, 200, 300},
    {ITEM_LIFE_SHROOM, 5, 5},
    {ITEM_TASTY_TONIC, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_BOOS_SHEET, 20, 20},
    {ITEM_RUIN_POWDER, 10, 10},
    {ITEM_LAST_STAND, 2, 1},
    {ITEM_LAST_STAND_P, 2, 1},
    {ITEM_CHARGE, 0, 2},
    {0},
};

ItemDropData battle_item_yami_kuriboo[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 20, 20},
    {ITEM_SPITE_POUCH, 5, 5},
    {ITEM_VOLT_SHROOM, 10, 10},
    {ITEM_HP_PLUS, 2, 2},
    {ITEM_HP_PLUS_P, 2, 2},
    {ITEM_POWER_JUMP, 0, 2},
    {0},
};

ItemDropData battle_item_yami_togekuri[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 20, 20},
    {ITEM_SPITE_POUCH, 5, 5},
    {ITEM_HP_DRAIN_ITEM, 10, 10},
    {ITEM_HP_PLUS, 2, 2},
    {ITEM_HP_PLUS_P, 2, 2},
    {ITEM_SOFT_STOMP, 0, 2},
    {0},
};

ItemDropData battle_item_yami_patakuri[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 20, 20},
    {ITEM_SPITE_POUCH, 5, 5},
    {ITEM_DIZZY_DIAL, 10, 10},
    {ITEM_HP_PLUS, 2, 2},
    {ITEM_HP_PLUS_P, 2, 2},
    {ITEM_MULTIBOUNCE, 0, 2},
    {0},
};

ItemDropData battle_item_nokonoko[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_POW_BLOCK, 20, 20},
    {ITEM_COURAGE_SHELL, 10, 10},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_HAPPY_HEART, 2, 1},
    {ITEM_POWER_RUSH_P, 2, 1},
    {ITEM_HAPPY_FLOWER, 0, 1},
    {0},
};

ItemDropData battle_item_patapata[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_POW_BLOCK, 20, 20},
    {ITEM_COURAGE_SHELL, 10, 10},
    {ITEM_DIZZY_DIAL, 5, 5},
    {ITEM_POWER_RUSH, 2, 1},
    {ITEM_HAPPY_HEART_P, 2, 1},
    {ITEM_HAPPY_FLOWER, 0, 1},
    {0},
};

ItemDropData battle_item_yami_noko[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 10, 10},
    {ITEM_COURAGE_SHELL, 10, 10},
    {ITEM_MR_SOFTENER, 20, 20},
    {ITEM_HAPPY_HEART, 2, 1},
    {ITEM_MEGA_RUSH_P, 2, 1},
    {ITEM_HEART_FINDER, 0, 2},
    {0},
};

ItemDropData battle_item_ura_noko[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_EARTH_QUAKE, 20, 20},
    {ITEM_COURAGE_SHELL, 5, 5},
    {ITEM_VOLT_SHROOM, 10, 10},
    {ITEM_HAPPY_HEART, 2, 1},
    {ITEM_POWER_RUSH, 2, 1},
    {ITEM_HEART_FINDER, 0, 2},
    {0},
};

ItemDropData battle_item_ura_pata[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_EARTH_QUAKE, 20, 20},
    {ITEM_COURAGE_SHELL, 5, 5},
    {ITEM_DIZZY_DIAL, 10, 10},
    {ITEM_HAPPY_HEART_P, 2, 1},
    {ITEM_POWER_RUSH_P, 2, 1},
    {ITEM_FLOWER_FINDER, 0, 2},
    {0},
};

ItemDropData battle_item_togenoko[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_THUNDER_RAGE, 20, 20},
    {ITEM_COURAGE_SHELL, 5, 5},
    {ITEM_REPEL_CAPE, 10, 10},
    {ITEM_CLOSE_CALL, 2, 2},
    {ITEM_CLOSE_CALL_P, 2, 2},
    {ITEM_FIRE_DRIVE, 0, 1},
    {0},
};

ItemDropData battle_item_togenoko_ace[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_SHOOTING_STAR, 20, 20},
    {ITEM_COURAGE_SHELL, 5, 5},
    {ITEM_BOOS_SHEET, 10, 10},
    {ITEM_P_UP_D_DOWN, 2, 2},
    {ITEM_P_UP_D_DOWN_P, 2, 2},
    {ITEM_ITEM_HOG, 0, 2},
    {0},
};

ItemDropData battle_item_hammer_bros[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_THUNDER_RAGE, 15, 10},
    {ITEM_MINI_MR_MINI, 20, 20},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_MEGA_RUSH, 2, 2},
    {ITEM_MEGA_RUSH_P, 2, 2},
    {ITEM_HAMMER_THROW, 0, 2},
    {0},
};

ItemDropData battle_item_met[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 10, 10},
    {ITEM_GRADUAL_SYRUP, 0, 10},
    {ITEM_EARTH_QUAKE, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_POWER_JUMP, 0, 2},
    {ITEM_POWER_SMASH, 0, 2},
    {ITEM_SHRINK_STOMP, 0, 2},
    {0},
};

ItemDropData battle_item_togemet[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 10, 10},
    {ITEM_GRADUAL_SYRUP, 0, 10},
    {ITEM_EARTH_QUAKE, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_REPEL_CAPE, 5, 5},
    {ITEM_POWER_JUMP, 0, 2},
    {ITEM_POWER_SMASH, 0, 2},
    {ITEM_SHRINK_STOMP, 0, 2},
    {0},
};

ItemDropData battle_item_patamet[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 10, 10},
    {ITEM_GRADUAL_SYRUP, 0, 10},
    {ITEM_EARTH_QUAKE, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_DIZZY_DIAL, 5, 5},
    {ITEM_MULTIBOUNCE, 0, 2},
    {ITEM_QUAKE_HAMMER, 0, 2},
    {ITEM_ICE_SMASH, 0, 2},
    {0},
};

ItemDropData battle_item_patatogemet[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 10, 10},
    {ITEM_GRADUAL_SYRUP, 0, 10},
    {ITEM_EARTH_QUAKE, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_MR_SOFTENER, 5, 5},
    {ITEM_MULTIBOUNCE, 0, 2},
    {ITEM_QUAKE_HAMMER, 0, 2},
    {ITEM_PIERCING_BLOW, 0, 2},
    {0},
};

ItemDropData battle_item_hyper_togemet[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 10, 10},
    {ITEM_GRADUAL_SYRUP, 0, 10},
    {ITEM_EARTH_QUAKE, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_REPEL_CAPE, 5, 5},
    {0},
};

ItemDropData battle_item_jyugem[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_THUNDER_RAGE, 10, 10},
    {ITEM_SPITE_POUCH, 5, 5},
    {ITEM_VOLT_SHROOM, 10, 10},
    {ITEM_SUPER_APPEAL, 0, 2},
    {ITEM_SUPER_APPEAL_P, 0, 2},
    {ITEM_TORNADO_JUMP, 0, 2},
    {0},
};

ItemDropData battle_item_hyper_jyugem[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_THUNDER_RAGE, 20, 20},
    {ITEM_SPITE_POUCH, 5, 5},
    {ITEM_VOLT_SHROOM, 10, 10},
    {ITEM_SUPER_APPEAL, 0, 2},
    {ITEM_SUPER_APPEAL_P, 0, 2},
    {ITEM_TORNADO_JUMP, 0, 2},
    {0},
};

ItemDropData battle_item_togezo[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_EARTH_QUAKE, 10, 10},
    {ITEM_RUIN_POWDER, 5, 5},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_SHRINK_STOMP, 0, 2},
    {0},
};

ItemDropData battle_item_kamec[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_SHOOTING_STAR, 10, 10},
    {ITEM_STOPWATCH, 20, 20},
    {ITEM_MYSTERY, 0, 10},
    {ITEM_HP_PLUS_P, 2, 2},
    {ITEM_SUPER_APPEAL, 0, 2},
    {ITEM_SUPER_APPEAL_P, 0, 2},
    {0},
};

ItemDropData battle_item_chorobon[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 5, 5},
    {ITEM_SLEEPY_SHEEP, 5, 5},
    {ITEM_HP_DRAIN_ITEM, 10, 10},
    {ITEM_SLEEPY_STOMP, 0, 1},
    {ITEM_HEAD_RATTLE, 0, 2},
    {0},
};

ItemDropData battle_item_green_chorobon[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 5, 5},
    {ITEM_GRADUAL_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_SLEEPY_SHEEP, 15, 15},
    {ITEM_RUIN_POWDER, 5, 5},
    {ITEM_LAST_STAND, 2, 1},
    {ITEM_HP_DRAIN_BADGE, 0, 1},
    {0},
};

ItemDropData battle_item_flower_chorobon[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 5, 5},
    {ITEM_GRADUAL_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_SLEEPY_SHEEP, 15, 15},
    {ITEM_SPITE_POUCH, 5, 5},
    {ITEM_LAST_STAND_P, 2, 1},
    {ITEM_FP_DRAIN, 0, 1},
    {0},
};

ItemDropData battle_item_honenoko[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 5, 5},
    {ITEM_SPITE_POUCH, 10, 10},
    {ITEM_POINT_SWAP, 0, 10},
    {ITEM_HP_PLUS, 0, 2},
    {ITEM_ITEM_HOG, 0, 1},
    {0},
};

ItemDropData battle_item_karon[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_SPITE_POUCH, 20, 20},
    {ITEM_POINT_SWAP, 0, 10},
    {ITEM_HP_PLUS, 0, 2},
    {ITEM_ITEM_HOG, 0, 1},
    {0},
};

ItemDropData battle_item_monochrome_kurokumorn[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_THUNDER_BOLT, 10, 10},
    {ITEM_DIZZY_DIAL, 5, 5},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_SLEEPY_STOMP, 0, 2},
    {ITEM_FP_DRAIN, 0, 1},
    {0},
};

ItemDropData battle_item_kurokumorn[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_THUNDER_RAGE, 15, 10},
    {ITEM_DIZZY_DIAL, 5, 5},
    {ITEM_MR_SOFTENER, 5, 5},
    {ITEM_HEAD_RATTLE, 0, 2},
    {ITEM_TORNADO_JUMP, 0, 2},
    {0},
};

ItemDropData battle_item_bllizard[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 15, 10},
    {ITEM_DIZZY_DIAL, 5, 5},
    {ITEM_RUIN_POWDER, 5, 5},
    {ITEM_SOFT_STOMP, 0, 2},
    {ITEM_ICE_SMASH, 0, 2},
    {0},
};

ItemDropData battle_item_dokugassun[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_THUNDER_RAGE, 15, 10},
    {ITEM_DIZZY_DIAL, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_ICE_SMASH, 0, 2},
    {ITEM_JUMPMAN, 0, 2},
    {0},
};

ItemDropData battle_item_pakkun_flower[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 5},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_FLOWER_SAVER, 0, 1},
    {ITEM_FLOWER_SAVER_P, 0, 1},
    {ITEM_PITY_FLOWER, 0, 1},
    {0},
};

ItemDropData battle_item_monochrome_sinemon[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_EARTH_QUAKE, 10, 10},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_QUAKE_HAMMER, 0, 2},
    {0},
};

ItemDropData battle_item_sinemon[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_EARTH_QUAKE, 10, 15},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_STOPWATCH, 5, 5},
    {ITEM_QUAKE_HAMMER, 0, 2},
    {0},
};

ItemDropData battle_item_hyper_sinnosuke[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_EARTH_QUAKE, 10, 10},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_REPEL_CAPE, 5, 5},
    {0},
};

ItemDropData battle_item_hyper_sinemon[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_EARTH_QUAKE, 15, 10},
    {ITEM_MINI_MR_MINI, 5, 5},
    {ITEM_REPEL_CAPE, 5, 5},
    {ITEM_CHARGE, 0, 2},
    {ITEM_CHARGE_P, 0, 2},
    {0},
};

ItemDropData battle_item_basabasa[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 5, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 5, 5},
    {ITEM_BOOS_SHEET, 5, 5},
    {ITEM_HP_DRAIN_ITEM, 15, 15},
    {ITEM_HAMMER_THROW, 0, 1},
    {ITEM_HP_DRAIN_BADGE, 0, 2},
    {ITEM_HP_DRAIN_P, 0, 2},
    {0},
};

ItemDropData battle_item_basabasa_chururu[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_BOOS_SHEET, 5, 5},
    {ITEM_DIZZY_DIAL, 15, 15},
    {ITEM_MULTIBOUNCE, 0, 2},
    {ITEM_HAMMER_THROW, 0, 2},
    {ITEM_HP_DRAIN_P, 0, 2},
    {0},
};

ItemDropData battle_item_basabasa_green[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_BOOS_SHEET, 5, 5},
    {ITEM_RUIN_POWDER, 15, 15},
    {ITEM_MULTIBOUNCE, 0, 2},
    {ITEM_HAMMER_THROW, 0, 2},
    {ITEM_LUCKY_DAY, 0, 1},
    {0},
};

ItemDropData battle_item_teresa[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 10, 10},
    {ITEM_GRADUAL_SYRUP, 0, 10},
    {ITEM_FIRE_FLOWER, 10, 10},
    {ITEM_DIZZY_DIAL, 10, 10},
    {ITEM_MINI_MR_MINI, 10, 10},
    {ITEM_SUPER_APPEAL, 0, 1},
    {ITEM_SUPER_APPEAL_P, 0, 1},
    {ITEM_SLEEPY_STOMP, 0, 2},
    {0},
};

ItemDropData battle_item_purple_teresa[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 10, 10},
    {ITEM_GRADUAL_SYRUP, 0, 10},
    {ITEM_THUNDER_RAGE, 10, 10},
    {ITEM_DIZZY_DIAL, 10, 10},
    {ITEM_POWER_PUNCH, 10, 10},
    {ITEM_SUPER_APPEAL, 0, 2},
    {ITEM_SUPER_APPEAL_P, 0, 2},
    {ITEM_MONEY_MONEY, 0, 1},
    {0},
};

ItemDropData battle_item_sambo[] = {
    {0, 200, 300},
    {ITEM_LIFE_SHROOM, 5, 5},
    {ITEM_TASTY_TONIC, 0, 15},
    {ITEM_EARTH_QUAKE, 5, 5},
    {ITEM_HP_DRAIN_ITEM, 5, 5},
    {ITEM_MR_SOFTENER, 5, 5},
    {0},
};

ItemDropData battle_item_sambo_mummy[] = {
    {0, 200, 300},
    {ITEM_LIFE_SHROOM, 5, 5},
    {ITEM_TASTY_TONIC, 0, 15},
    {ITEM_EARTH_QUAKE, 5, 5},
    {ITEM_HP_DRAIN_ITEM, 5, 5},
    {ITEM_MR_SOFTENER, 5, 5},
    {ITEM_FP_PLUS, 0, 2},
    {0},
};

ItemDropData battle_item_bomhei[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_STOPWATCH, 5, 5},
    {ITEM_REPEL_CAPE, 5, 5},
    {ITEM_HEAD_RATTLE, 0, 2},
    {ITEM_DAMAGE_DODGE_P, 0, 1},
    {ITEM_DAMAGE_DODGE, 0, 1},
    {0},
};

ItemDropData battle_item_heavy_bom[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 0, 5},
    {ITEM_STOPWATCH, 0, 5},
    {ITEM_DIZZY_DIAL, 0, 5},
    {ITEM_DAMAGE_DODGE, 0, 2},
    {ITEM_DAMAGE_DODGE_P, 0, 2},
    {0},
};

ItemDropData battle_item_giant_bomb[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 0, 5},
    {ITEM_STOPWATCH, 0, 5},
    {ITEM_DIZZY_DIAL, 0, 5},
    {ITEM_DEFEND_PLUS, 2, 2},
    {ITEM_DEFEND_PLUS_P, 2, 2},
    {0},
};

ItemDropData battle_item_borodo[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_SLEEPY_SHEEP, 10, 10},
    {ITEM_FRIGHT_MASK, 0, 15},
    {ITEM_PRETTY_LUCKY, 2, 1},
    {ITEM_PRETTY_LUCKY_P, 2, 1},
    {ITEM_REFUND, 0, 2},
    {0},
};

ItemDropData battle_item_badge_borodo[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_ICE_STORM, 5, 5},
    {ITEM_SLEEPY_SHEEP, 10, 10},
    {ITEM_MYSTERY, 0, 15},
    {ITEM_PRETTY_LUCKY, 2, 2},
    {ITEM_PRETTY_LUCKY_P, 2, 2},
    {ITEM_REFUND, 0, 2},
    {0},
};

ItemDropData battle_item_mahorn[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_THUNDER_RAGE, 10, 10},
    {ITEM_STOPWATCH, 10, 10},
    {ITEM_BOOS_SHEET, 5, 5},
    {ITEM_JUMPMAN, 0, 1},
    {ITEM_HAMMERMAN, 0, 1},
    {0},
};

ItemDropData battle_item_super_mahorn[] = {
    {0, 200, 300},
    {ITEM_LIFE_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_THUNDER_RAGE, 10, 10},
    {ITEM_STOPWATCH, 10, 10},
    {ITEM_BOOS_SHEET, 5, 5},
    {ITEM_POWER_PLUS, 1, 1},
    {ITEM_POWER_PLUS_P, 1, 1},
    {0},
};

ItemDropData battle_item_mahorn_custom[] = {
    {0, 200, 300},
    {ITEM_ULTRA_SHROOM, 10, 10},
    {ITEM_JAMMIN_JELLY, 0, 10},
    {ITEM_SHOOTING_STAR, 10, 10},
    {ITEM_STOPWATCH, 10, 10},
    {ITEM_BOOS_SHEET, 5, 5},
    {ITEM_POWER_PLUS, 2, 2},
    {ITEM_POWER_PLUS_P, 2, 2},
    {0},
};

ItemDropData battle_item_wanwan[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_POWER_PUNCH, 10, 10},
    {ITEM_DEFEND_PLUS, 2, 1},
    {ITEM_DEFEND_PLUS_P, 2, 1},
    {0},
};

ItemDropData battle_item_burst_wanwan[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_POWER_PUNCH, 10, 10},
    {ITEM_DAMAGE_DODGE, 2, 1},
    {ITEM_DAMAGE_DODGE_P, 2, 1},
    {0},
};

ItemDropData battle_item_bubble[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 0, 15},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_HP_DRAIN_ITEM, 10, 10},
    {ITEM_FIRE_DRIVE, 0, 2},
    {0},
};

ItemDropData battle_item_hermos[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 0, 15},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_SPITE_POUCH, 10, 10},
    {ITEM_FIRE_DRIVE, 0, 2},
    {0},
};

ItemDropData battle_item_phantom[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 5, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_FIRE_FLOWER, 0, 15},
    {ITEM_POWER_PUNCH, 5, 5},
    {ITEM_RUIN_POWDER, 10, 10},
    {ITEM_FIRE_DRIVE, 0, 2},
    {0},
};

ItemDropData battle_item_pansy[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 10, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_THUNDER_BOLT, 10, 10},
    {ITEM_POINT_SWAP, 0, 15},
    {ITEM_FRIGHT_MASK, 0, 10},
    {ITEM_HAPPY_FLOWER, 0, 2},
    {ITEM_FLOWER_SAVER_P, 0, 1},
    {ITEM_FLOWER_SAVER, 0, 1},
    {0},
};

ItemDropData battle_item_twinkling_pansy[] = {
    {0, 200, 300},
    {ITEM_ULTRA_SHROOM, 10, 10},
    {ITEM_JAMMIN_JELLY, 0, 10},
    {ITEM_SHOOTING_STAR, 10, 10},
    {ITEM_POINT_SWAP, 0, 15},
    {ITEM_MYSTERY, 0, 10},
    {ITEM_HAPPY_FLOWER, 0, 2},
    {ITEM_FLOWER_SAVER_P, 0, 2},
    {ITEM_FLOWER_SAVER, 0, 2},
    {0},
};

ItemDropData battle_item_dark_keeper[] = {
    {0, 200, 300},
    {ITEM_LIFE_SHROOM, 10, 10},
    {ITEM_TASTY_TONIC, 0, 10},
    {ITEM_SHOOTING_STAR, 5, 5},
    {ITEM_RUIN_POWDER, 5, 5},
    {ITEM_SPITE_POUCH, 5, 5},
    {ITEM_P_DOWN_D_UP, 2, 2},
    {ITEM_P_DOWN_D_UP_P, 2, 2},
    {ITEM_ALL_OR_NOTHING, 0, 1},
    {0},
};

PointDropData battle_heart_drop_param_default[] = {
    {20, 70, 2, 50},
    {30, 60, 2, 50},
    {50, 50, 2, 40},
    {80, 40, 2, 40},
    {100, 30, 2, 30},
    {20, 70, 3, 50},
    {30, 60, 3, 50},
    {50, 50, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 30},
    {20, 70, 4, 50},
    {30, 60, 4, 50},
    {50, 50, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 30},
    {20, 70, 5, 50},
    {30, 60, 5, 50},
    {50, 50, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 30},
    {20, 70, 6, 50},
    {30, 60, 6, 50},
    {50, 50, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 30},
    {20, 40, 2, 40},
    {30, 40, 2, 40},
    {50, 40, 2, 40},
    {80, 40, 2, 40},
    {100, 30, 2, 40},
    {20, 40, 3, 40},
    {30, 40, 3, 40},
    {50, 40, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 40},
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_heart_drop_param_default2[] = {
    {20, 70, 3, 50},
    {30, 60, 3, 50},
    {50, 50, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 30},
    {20, 70, 4, 50},
    {30, 60, 4, 50},
    {50, 50, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 30},
    {20, 70, 5, 50},
    {30, 60, 5, 50},
    {50, 50, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 30},
    {20, 70, 6, 50},
    {30, 60, 6, 50},
    {50, 50, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 30},
    {20, 40, 2, 40},
    {30, 40, 2, 40},
    {50, 40, 2, 40},
    {80, 40, 2, 40},
    {100, 30, 2, 40},
    {20, 40, 3, 40},
    {30, 40, 3, 40},
    {50, 40, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 40},
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_heart_drop_param_default3[] = {
    {20, 70, 4, 50},
    {30, 60, 4, 50},
    {50, 50, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 30},
    {20, 70, 5, 50},
    {30, 60, 5, 50},
    {50, 50, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 30},
    {20, 70, 6, 50},
    {30, 60, 6, 50},
    {50, 50, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 30},
    {20, 40, 2, 40},
    {30, 40, 2, 40},
    {50, 40, 2, 40},
    {80, 40, 2, 40},
    {100, 30, 2, 40},
    {20, 40, 3, 40},
    {30, 40, 3, 40},
    {50, 40, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 40},
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_heart_drop_param_default4[] = {
    {20, 70, 5, 50},
    {30, 60, 5, 50},
    {50, 50, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 30},
    {20, 70, 6, 50},
    {30, 60, 6, 50},
    {50, 50, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 30},
    {20, 40, 2, 40},
    {30, 40, 2, 40},
    {50, 40, 2, 40},
    {80, 40, 2, 40},
    {100, 30, 2, 40},
    {20, 40, 3, 40},
    {30, 40, 3, 40},
    {50, 40, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 40},
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_heart_drop_param_default5[] = {
    {20, 70, 6, 50},
    {30, 60, 6, 50},
    {50, 50, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 30},
    {20, 40, 2, 40},
    {30, 40, 2, 40},
    {50, 40, 2, 40},
    {80, 40, 2, 40},
    {100, 30, 2, 40},
    {20, 40, 3, 40},
    {30, 40, 3, 40},
    {50, 40, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 40},
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_flower_drop_param_default[] = {
    {20, 40, 2, 40},
    {30, 40, 2, 40},
    {50, 40, 2, 40},
    {80, 40, 2, 40},
    {100, 30, 2, 40},
    {20, 40, 3, 40},
    {30, 40, 3, 40},
    {50, 40, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 40},
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_flower_drop_param_default2[] = {
    {20, 40, 3, 40},
    {30, 40, 3, 40},
    {50, 40, 3, 40},
    {80, 40, 3, 40},
    {100, 30, 3, 40},
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_flower_drop_param_default3[] = {
    {20, 40, 4, 40},
    {30, 40, 4, 40},
    {50, 40, 4, 40},
    {80, 40, 4, 40},
    {100, 30, 4, 40},
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_flower_drop_param_default4[] = {
    {20, 40, 5, 40},
    {30, 40, 5, 40},
    {50, 40, 5, 40},
    {80, 40, 5, 40},
    {100, 30, 5, 40},
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

PointDropData battle_flower_drop_param_default5[] = {
    {20, 40, 6, 40},
    {30, 40, 6, 40},
    {50, 40, 6, 40},
    {80, 40, 6, 40},
    {100, 30, 6, 40},
};

ItemDropData battle_item_NONE[] = {
    {0},
};

ItemDropData battle_item_nokonoko_fighter[] = {
    {0},
};

ItemDropData battle_item_patapata_fighter[] = {
    {0},
};

ItemDropData battle_item_boomerang_bros[] = {
    {0},
};

ItemDropData battle_item_fire_bros[] = {
    {0},
};

ItemDropData battle_item_red_honenoko[] = {
    {0},
};

ItemDropData battle_item_poison_pakkun[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 5},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_FP_PLUS, 0, 1},
    {0},
};

ItemDropData battle_item_ice_pakkun[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 5},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_ICE_SMASH, 0, 1},
    {0},
};

ItemDropData battle_item_monochrome__pakkun[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 0, 5},
    {ITEM_HONEY_SYRUP, 0, 10},
    {ITEM_FP_PLUS, 0, 1},
    {0},
};

ItemDropData battle_item_piders[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 0, 10},
    {ITEM_GRADUAL_SYRUP, 0, 5},
    {0},
};

ItemDropData battle_item_churantalar[] = {
    {0, 200, 300},
    {ITEM_SLOW_SHROOM, 0, 10},
    {ITEM_GRADUAL_SYRUP, 0, 5},
    {ITEM_PITY_FLOWER, 0, 2},
    {ITEM_ALL_OR_NOTHING, 0, 2},
    {0},
};

ItemDropData battle_item_barriern[] = {
    {0},
};

ItemDropData battle_item_barriern_z[] = {
    {0},
};

ItemDropData battle_item_barriern_custom[] = {
    {0},
};

ItemDropData battle_item_iron_sinemon[] = {
    {0},
};

ItemDropData battle_item_iron_sinemon2[] = {
    {0},
};

ItemDropData battle_item_atmic_teresa[] = {
    {0},
};

ItemDropData battle_item_killer[] = {
    {0},
};

ItemDropData battle_item_super_killer[] = {
    {0},
};

ItemDropData battle_item_killer_cannon[] = {
    {0, 200, 300},
    {ITEM_MUSHROOM, 0, 5},
    {ITEM_HONEY_SYRUP, 0, 5},
    {ITEM_MONEY_MONEY, 0, 2},
    {0},
};

ItemDropData battle_item_super_killer_cannon[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 5},
    {ITEM_MAPLE_SYRUP, 0, 5},
    {ITEM_MONEY_MONEY, 0, 2},
    {0},
};

ItemDropData battle_item_borodo_king[] = {
    {0},
};

ItemDropData battle_item_monban[] = {
    {0},
};

ItemDropData battle_item_gold_chorobon[] = {
    {0},
};

ItemDropData battle_item_sinnosuke[] = {
    {0},
};

ItemDropData battle_item_togedaruma[] = {
    {0},
};

ItemDropData battle_item_yamitogedaruma[] = {
    {0, 200, 300},
    {ITEM_SUPER_SHROOM, 0, 10},
    {ITEM_MAPLE_SYRUP, 0, 10},
    {ITEM_HAMMERMAN, 0, 2},
    {0},
};

ItemDropData battle_item_vivian1[] = {
    {0},
};

ItemDropData battle_item_gesso[] = {
    {0},
};

ItemDropData battle_item_gonbaba[] = {
    {0},
};

ItemDropData battle_item_moamoa[] = {
    {0},
};

ItemDropData battle_item_rampell[] = {
    {0},
};

ItemDropData battle_item_faker_mario[] = {
    {0},
};

ItemDropData battle_item_cortez[] = {
    {0},
};

ItemDropData battle_item_kurikuri[] = {
    {0},
};

ItemDropData battle_item_magnum_battender[] = {
    {0},
};

ItemDropData battle_item_macho_gance[] = {
    {0},
};

ItemDropData battle_item_champion[] = {
    {0},
};

ItemDropData battle_item_kanbu1[] = {
    {0},
};

ItemDropData battle_item_zonbaba[] = {
    {0},
};

ItemDropData battle_item_default_param_list[] = {
    {0},
};

