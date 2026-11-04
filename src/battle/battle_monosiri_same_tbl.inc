// Tattle groups of battle_monosiri.c: one list for both data orders (the early demos keep them after the message table).
BattleUnitType same_tbl_gesso[] = {
    UNIT_BLOOPER,
    UNIT_BLOOPER_LEFT_TENTACLE,
    UNIT_BLOOPER_RIGHT_TENTACLE,
    UNIT_NULL
};

/** Tattle group for Blooper, listed from the left tentacle; same units as same_tbl_gesso. */
BattleUnitType same_tbl_gesso_arm_l[] = {
    UNIT_BLOOPER_LEFT_TENTACLE,
    UNIT_BLOOPER,
    UNIT_BLOOPER_RIGHT_TENTACLE,
    UNIT_NULL
};

/** Tattle group for Blooper, listed from the right tentacle; same units as same_tbl_gesso. */
BattleUnitType same_tbl_gesso_arm_r[] = {
    UNIT_BLOOPER_RIGHT_TENTACLE,
    UNIT_BLOOPER,
    UNIT_BLOOPER_LEFT_TENTACLE,
    UNIT_NULL
};

#if VERSION <= VERSION_DEMO_JP
#include "../src/battle/battle_monosiri_rampell.inc"
#endif

/**
 * Tattle group for Beldam: her chapter 2 and chapter 8 fights and two unused Beldam units.
 * Tattling any one marks them all; the next three tables list the same units in another order.
 */
BattleUnitType same_tbl_majolyne_1[] = {
    UNIT_BELDAM_CHAPTER2,
    UNIT_UNUSED_BELDAM2,
    UNIT_UNUSED_BELDAM3,
    UNIT_BELDAM_CHAPTER8,
    UNIT_NULL
};

/** Tattle group for Beldam, listed from the first unused unit; same units as same_tbl_majolyne_1. */
BattleUnitType same_tbl_majolyne_2[] = {
    UNIT_UNUSED_BELDAM2,
    UNIT_BELDAM_CHAPTER2,
    UNIT_UNUSED_BELDAM3,
    UNIT_BELDAM_CHAPTER8,
    UNIT_NULL
};

/** Tattle group for Beldam, listed from the second unused unit; same units as same_tbl_majolyne_1. */
BattleUnitType same_tbl_majolyne_3[] = {
    UNIT_UNUSED_BELDAM3,
    UNIT_BELDAM_CHAPTER2,
    UNIT_UNUSED_BELDAM2,
    UNIT_BELDAM_CHAPTER8,
    UNIT_NULL
};

/** Tattle group for Beldam, listed from the chapter 8 fight; same units as same_tbl_majolyne_1. */
BattleUnitType same_tbl_majolyne_4[] = {
    UNIT_BELDAM_CHAPTER8,
    UNIT_BELDAM_CHAPTER2,
    UNIT_UNUSED_BELDAM2,
    UNIT_UNUSED_BELDAM3,
    UNIT_NULL
};

/**
 * Tattle group for Marilyn: her chapter 2 and chapter 8 fights and two unused Marilyn units.
 * Tattling any one marks them all; the next three tables list the same units in another order.
 */
BattleUnitType same_tbl_marilyn_1[] = {
    UNIT_MARILYN_CHAPTER2,
    UNIT_UNUSED_MARILYN2,
    UNIT_UNUSED_MARILYN3,
    UNIT_MARILYN_CHAPTER8,
    UNIT_NULL
};

/** Tattle group for Marilyn, listed from the first unused unit; same units as same_tbl_marilyn_1. */
BattleUnitType same_tbl_marilyn_2[] = {
    UNIT_UNUSED_MARILYN2,
    UNIT_MARILYN_CHAPTER2,
    UNIT_UNUSED_MARILYN3,
    UNIT_MARILYN_CHAPTER8,
    UNIT_NULL
};

/** Tattle group for Marilyn, listed from the second unused unit; same units as same_tbl_marilyn_1. */
BattleUnitType same_tbl_marilyn_3[] = {
    UNIT_UNUSED_MARILYN3,
    UNIT_MARILYN_CHAPTER2,
    UNIT_UNUSED_MARILYN2,
    UNIT_MARILYN_CHAPTER8,
    UNIT_NULL
};

/** Tattle group for Marilyn, listed from the chapter 8 fight; same units as same_tbl_marilyn_1. */
BattleUnitType same_tbl_marilyn_4[] = {
    UNIT_MARILYN_CHAPTER8,
    UNIT_MARILYN_CHAPTER2,
    UNIT_UNUSED_MARILYN2,
    UNIT_UNUSED_MARILYN3,
    UNIT_NULL
};

/** Tattle group for Vivian: her chapter 2 fight and an unused second Vivian unit. */
BattleUnitType same_tbl_vivian_1[] = {
    UNIT_VIVIAN_CHAPTER2,
    UNIT_UNUSED_VIVIAN2,
    UNIT_NULL
};

/** Tattle group for Vivian, listed from the unused unit; same units as same_tbl_vivian_1. */
BattleUnitType same_tbl_vivian_2[] = {
    UNIT_UNUSED_VIVIAN2,
    UNIT_VIVIAN_CHAPTER2,
    UNIT_NULL
};

/** Tattle group for Bowser: his chapter 3 and chapter 8 fights. */
BattleUnitType same_tbl_koopa_1[] = {
    UNIT_BOWSER_CHAPTER3,
    UNIT_BOWSER_CHAPTER8,
    UNIT_NULL
};

/** Tattle group for Bowser, listed from the chapter 8 fight; same units as same_tbl_koopa_1. */
BattleUnitType same_tbl_koopa_2[] = {
    UNIT_BOWSER_CHAPTER8,
    UNIT_BOWSER_CHAPTER3,
    UNIT_NULL
};

/** Tattle group for Magikoopa: Tattling the Magikoopa or one of its clones marks both. */
BattleUnitType same_tbl_kamec[] = {
    UNIT_MAGIKOOPA,
    UNIT_MAGIKOOPA_CLONE,
    UNIT_NULL
};

/** Tattle group for Magikoopa, listed from the clone; same units as same_tbl_kamec. */
BattleUnitType same_tbl_kamec_bunshin[] = {
    UNIT_MAGIKOOPA_CLONE,
    UNIT_MAGIKOOPA,
    UNIT_NULL
};

/**
 * Tattle group for the Shadow Queen: the possessed Peach, the Shadow Queen herself, her hands
 * and her invincible form.
 */
BattleUnitType same_tbl_black_peach[] = {
    UNIT_SHADOW_QUEEN_PEACH,
    UNIT_SHADOW_QUEEN_DEMON,
    UNIT_SHADOW_QUEEN_LEFT_RIGHT_HAND,
    UNIT_SHADOW_QUEEN_DEAD_HANDS,
    UNIT_SHADOW_QUEEN_INVINCIBLE,
    UNIT_NULL
};

/** Tattle group for Red Magikoopa and its clones. */
BattleUnitType same_tbl_kamec_red[] = {
    UNIT_RED_MAGIKOOPA,
    UNIT_RED_MAGIKOOPA_CLONE,
    UNIT_NULL
};

/** Tattle group for Red Magikoopa, listed from the clone; same units as same_tbl_kamec_red. */
BattleUnitType same_tbl_kamec_red_bunshin[] = {
    UNIT_RED_MAGIKOOPA_CLONE,
    UNIT_RED_MAGIKOOPA,
    UNIT_NULL
};

/** Tattle group for White Magikoopa and its clones. */
BattleUnitType same_tbl_kamec_white[] = {
    UNIT_WHITE_MAGIKOOPA,
    UNIT_WHITE_MAGIKOOPA_CLONE,
    UNIT_NULL
};

/** Tattle group for White Magikoopa, listed from the clone; same units as same_tbl_kamec_white. */
BattleUnitType same_tbl_kamec_white_bunshin[] = {
    UNIT_WHITE_MAGIKOOPA_CLONE,
    UNIT_WHITE_MAGIKOOPA,
    UNIT_NULL
};

/** Tattle group for Green Magikoopa and its clones. */
BattleUnitType same_tbl_kamec_green[] = {
    UNIT_GREEN_MAGIKOOPA,
    UNIT_GREEN_MAGIKOOPA_CLONE,
    UNIT_NULL
};

/** Tattle group for Green Magikoopa, listed from the clone; same units as same_tbl_kamec_green. */
BattleUnitType same_tbl_kamec_green_bunshin[] = {
    UNIT_GREEN_MAGIKOOPA_CLONE,
    UNIT_GREEN_MAGIKOOPA,
    UNIT_NULL
};

/** Tattle group for Dark Wizzerd and its clones. */
BattleUnitType same_tbl_super_mahorn[] = {
    UNIT_DARK_WIZZERD,
    UNIT_DARK_WIZZERD_CLONE,
    UNIT_NULL
};

/** Tattle group for Dark Wizzerd, listed from the clone; same units as same_tbl_super_mahorn. */
BattleUnitType same_tbl_super_mahorn_bunshin[] = {
    UNIT_DARK_WIZZERD_CLONE,
    UNIT_DARK_WIZZERD,
    UNIT_NULL
};

/** Tattle group for Elite Wizzerd and its clones. */
BattleUnitType same_tbl_mahorn_custom[] = {
    UNIT_ELITE_WIZZERD,
    UNIT_ELITE_WIZZERD_CLONE,
    UNIT_NULL
};

/** Tattle group for Elite Wizzerd, listed from the clone; same units as same_tbl_mahorn_custom. */
BattleUnitType same_tbl_mahorn_custom_bunshin[] = {
    UNIT_ELITE_WIZZERD_CLONE,
    UNIT_ELITE_WIZZERD,
    UNIT_NULL
};

/** Tattle group for Goomba: the normal Goomba and the Glitz Pit's Goomba unit. */
BattleUnitType same_tbl_kuriboo[] = {
    UNIT_GOOMBA,
    UNIT_GOOMBA_GLITZVILLE,
    UNIT_NULL
};

/** Tattle group for Fuzzy: a single Fuzzy and the Fuzzy Horde. */
BattleUnitType same_tbl_chorobon[] = {
    UNIT_FUZZY,
    UNIT_FUZZY_HORDE,
    UNIT_NULL
};

/** Tattle group for X-Naut: the single X-Naut and the three X-Naut formations of Lord Crump's fight. */
BattleUnitType same_tbl_gundan_zako[] = {
    UNIT_X_NAUT,
    UNIT_X_NAUTS_CRUMP_FORMATION1,
    UNIT_X_NAUTS_CRUMP_FORMATION2,
    UNIT_X_NAUTS_CRUMP_FORMATION3,
    UNIT_NULL
};

/** Tattle group for Cortez: his main body, his bone pile and his four weapons. */
BattleUnitType same_tbl_cortez[] = {
    UNIT_CORTEZ,
    UNIT_CORTEZ_BONE_PILE,
    UNIT_CORTEZ_HOOK,
    UNIT_CORTEZ_RAPIER,
    UNIT_CORTEZ_SWORD,
    UNIT_CORTEZ_SABER,
    UNIT_NULL
};

/** Tattle group for Magnus von Grapple and its X-Fist. */
BattleUnitType same_tbl_magunum_battender[] = {
    UNIT_MAGNUS,
    UNIT_X_FIST,
    UNIT_NULL
};

/** Tattle group for Magnus von Grapple 2.0 and its X-Punch. */
BattleUnitType same_tbl_magunum_battender_mkii[] = {
    UNIT_MAGNUS2_0,
    UNIT_X_PUNCH,
    UNIT_NULL
};

BattleUnitType same_tbl_moamoa[] = {
    UNIT_SMORG,
    UNIT_SMORG_MIASMA_TENTACLE_C,
    UNIT_SMORG_MIASMA_TENTACLE_B,
    UNIT_SMORG_MIASMA_TENTACLE_A,
#if VERSION >= VERSION_JP
    UNIT_SMORG_MIASMA_CLAW,
#endif
    UNIT_NULL
};

#if VERSION >= VERSION_JP
BattleUnitType same_tbl_iron_sinemon[] = {
    UNIT_IRON_CLEFT_RED,
    UNIT_IRON_CLEFT_GREEN,
    UNIT_NULL
};
#endif
