#include "battle/battle_monosiri.h"
#include "driver/swdrv.h"

extern void* _battleWorkPointer;
typedef struct MonosiriMessageEntry {
    const char* fields[6];
} MonosiriMessageEntry;


s32 same_tbl_gesso[] = { 8, 9, 10, 0 };
s32 same_tbl_gesso_arm_l[] = { 9, 8, 10, 0 };
s32 same_tbl_gesso_arm_r[] = { 10, 8, 9, 0 };
s32 same_tbl_majolyne_1[] = { 31, 189, 192, 133, 0 };
s32 same_tbl_majolyne_2[] = { 189, 31, 192, 133, 0 };
s32 same_tbl_majolyne_3[] = { 192, 31, 189, 133, 0 };
s32 same_tbl_majolyne_4[] = { 133, 31, 189, 192, 0 };
s32 same_tbl_marilyn_1[] = { 32, 190, 193, 134, 0 };
s32 same_tbl_marilyn_2[] = { 190, 32, 193, 134, 0 };
s32 same_tbl_marilyn_3[] = { 193, 32, 190, 134, 0 };
s32 same_tbl_marilyn_4[] = { 134, 32, 190, 193, 0 };
s32 same_tbl_vivian_1[] = { 33, 191, 0 };
s32 same_tbl_vivian_2[] = { 191, 33, 0 };
s32 same_tbl_koopa_1[] = { 63, 144, 0 };
s32 same_tbl_koopa_2[] = { 144, 63, 0 };
s32 same_tbl_kamec[] = { 12, 13, 0 };
s32 same_tbl_kamec_bunshin[] = { 13, 12, 0 };
s32 same_tbl_black_peach[] = { 148, 150, 151, 152, 149, 0 };
s32 same_tbl_kamec_red[] = { 49, 50, 0 };
s32 same_tbl_kamec_red_bunshin[] = { 50, 49, 0 };
s32 same_tbl_kamec_white[] = { 51, 52, 0 };
s32 same_tbl_kamec_white_bunshin[] = { 52, 51, 0 };
s32 same_tbl_kamec_green[] = { 53, 54, 0 };
s32 same_tbl_kamec_green_bunshin[] = { 54, 53, 0 };
s32 same_tbl_super_mahorn[] = { 128, 129, 0 };
s32 same_tbl_super_mahorn_bunshin[] = { 129, 128, 0 };
s32 same_tbl_mahorn_custom[] = { 169, 170, 0 };
s32 same_tbl_mahorn_custom_bunshin[] = { 170, 169, 0 };
s32 same_tbl_kuriboo[] = { 1, 36, 0 };
s32 same_tbl_chorobon[] = { 16, 21, 0 };
s32 same_tbl_gundan_zako[] = { 28, 100, 101, 102, 0 };
s32 same_tbl_cortez[] = { 93, 94, 95, 96, 97, 98, 0 };
s32 same_tbl_magunum_battender[] = { 34, 35, 0 };
s32 same_tbl_magunum_battender_mkii[] = { 121, 122, 0 };
s32 same_tbl_moamoa[] = { 107, 110, 109, 108, 111, 0 };
s32 unk_80417170[] = { 61, 62, 0 };



MonosiriMessageEntry monosiriMessageTable[] = {
    { { 0, 0, 0, 0, 0, 0 } },
    { { "btl_un_kuriboo", "btl_hlp_kuriboo", "menu_enemy_001", "c_kuribo", "KUR_Z_2", "menu_monosiri_nara" } },
    { { "btl_un_patakuri", "btl_hlp_patakuri", "menu_enemy_003", "c_kuribo", "PTK_Z_2", "menu_monosiri_nara" } },
    { { "btl_un_togekuri", "btl_hlp_togekuri", "menu_enemy_002", "c_kuribo", "TGK_Z_2", "menu_monosiri_nara" } },
    { { "btl_un_hinnya", "btl_hlp_hinnya", "menu_enemy_020", "c_hannya_n", "Z_1", "menu_monosiri_tik" } },
    { { "btl_un_hannya", "btl_hlp_hannya", "menu_enemy_021", "c_hannya", "Z_1", "menu_monosiri_tik" } },
    { { "btl_un_kanbu1", "btl_hlp_kanbu1", 0, "c_kanbu", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_monban", "btl_hlp_monban", "menu_enemy_103", "c_monban", "Z_1", "menu_monosiri_gor" } },
    { { "btl_un_gesso", "btl_hlp_gesso", "menu_enemy_114", "c_gesso", "GSO_Z2_1", "menu_monosiri_tik" } },
    { { "btl_un_gesso_left_arm", 0, 0, "c_gesso3", "GSO_Z1_1", "menu_monosiri_tik" } },
    { { "btl_un_gesso_right_arm", 0, 0, "c_gesso2", "GSO_Z1_1", "menu_monosiri_tik" } },
    { { "btl_un_togenoko", "btl_hlp_togenoko", "menu_enemy_033", "c_togenoko", "Z_1", "menu_monosiri_tik" } },
    { { "btl_un_kamec", "btl_hlp_kamec", "menu_enemy_086", "c_kamek", "KMK_Z_1", "menu_monosiri_tik" } },
    { { "btl_un_kamec_bunsin", "btl_hlp_kamec_bunshin", 0, "c_kamek", "KMK_Z_1", "menu_monosiri_tik" } },
    { { "btl_un_nokonoko", "btl_hlp_nokonoko", "menu_enemy_004", "c_nokoteki", "NKT_Z_1", "menu_monosiri_hei" } },
    { { "btl_un_patapata", "btl_hlp_patapata", "menu_enemy_005", "c_nokoteki", "PTP_Z_1", "menu_monosiri_gon" } },
    { { "btl_un_chorobon", "btl_hlp_chorobon", "menu_enemy_006", "c_chorobon", "CBN_Z_1", "menu_monosiri_stn" } },
    { { "btl_un_honenoko", "btl_hlp_honenoko", "menu_enemy_007", "c_karon_g_h", "KRN_Z_1", "menu_monosiri_gon" } },
    { { "btl_un_sinnosuke", "btl_hlp_sinnosuke", "menu_enemy_008", "c_sinnosuke", "SIS_Z_1", "menu_monosiri_hei" } },
    { { "btl_un_togedaruma", "btl_hlp_togedaruma", "menu_enemy_009", "c_togedaruma", "TGD_Z_1", "menu_monosiri_hei" } },
    { { "btl_un_gold_chorobon", "btl_hlp_gold_chorobon", "menu_enemy_010", "c_chorobon_k", "CBN_Z_1", "menu_monosiri_stn" } },
    { { "btl_un_chorobon_gundan", "btl_hlp_chorobon_gundan", 0, "c_chorobon", "CBN_Z_1", "menu_monosiri_stn" } },
    { { "btl_un_red_honenoko", "btl_hlp_red_honenoko", "menu_enemy_012", "c_karon_r_h", "KRN_Z_1", "menu_monosiri_gon" } },
    { { "btl_un_gonbaba", "btl_hlp_gonbaba", "menu_enemy_117", "c_gonbaba", "GNB_Z_1", "menu_monosiri_gon" } },
    { { "btl_un_monochrome_kurokumorn", "btl_hlp_monochrome_kurokumorn", "menu_enemy_172", "c_kmoon_wb", "Z_1", "menu_monosiri_win" } },
    { { "btl_un_monochrome_pakkun", "btl_hlp_monochrome_pakkun", "menu_enemy_181", "c_pakflwr_t", "PKF_Z_1", "menu_monosiri_win" } },
    { { "btl_un_monochrome_sinemon", "btl_hlp_monochrome_sinemon", "menu_enemy_035", "c_sinemon_w", "SIN_Z_1", "menu_monosiri_win" } },
    { { "btl_un_piders", "btl_hlp_piders", "menu_enemy_015", "c_paid", "PAI_Z_1", "menu_monosiri_mri" } },
    { { "btl_un_gundan_zako", "btl_hlp_gundan_zako", "menu_enemy_018", "c_zako_n", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_barriern", "btl_hlp_barriern", "menu_enemy_016", "c_baria", "Z_1", "menu_monosiri_mri" } },
    { { "btl_un_barriern_satellite", "btl_hlp_barriern_petit", "menu_enemy_017", "c_baria", "Z_2", "menu_monosiri_mri" } },
    { { "btl_un_majolyne1", "btl_hlp_majyorin", "menu_enemy_104", "c_majyorin", "MJR_M_1", "menu_monosiri_win" } },
    { { "btl_un_marilyn1", "btl_hlp_maririn", "menu_enemy_108", "c_maririn", "MRR_M_1", "menu_monosiri_win" } },
    { { "btl_un_vivian1", "btl_hlp_vivian", "menu_enemy_112", "c_vivian", "PTR_M_1", "menu_monosiri_win" } },
    { { "btl_un_magnum_battender", "btl_hlp_magnum_battender", "menu_enemy_139", "c_mb_robo_1b", "MGN_Z_1", "menu_monosiri_mri" } },
    { { "btl_un_rocket_punch", 0, 0, 0, "MGN_A1_4", "menu_monosiri_mri" } },
    { { "btl_un_kuriboo", "btl_hlp_kuriboo", 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_nara" } },
    { { "btl_un_nokonoko_fighter", "btl_hlp_nokonoko_fighter", "menu_enemy_188", "c_touginoko", "NKT_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_patapata_fighter", "btl_hlp_patapata_fighter", "menu_enemy_189", "c_touginoko", "PTP_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_sambo", "btl_hlp_sambo", "menu_enemy_055", "c_sanbo", "SNB_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_jyugem", "btl_hlp_jyugem", "menu_enemy_084", "c_jugemu", "JGM_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_togezo", "btl_hlp_togezo", "menu_enemy_085", "c_togezo", "TGZ_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_hyper_sinnosuke", "btl_hlp_hyper_sinnosuke", "menu_enemy_036", "c_sinnosuke_h", "SIS_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_bomhei", "btl_hlp_bomhei", "menu_enemy_061", "c_bomhey", "BOM_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_borodo", "btl_hlp_borodo", "menu_enemy_077", "c_borodo", "Z_1", "menu_monosiri_tou" } },
    { { "btl_un_borodo_king", "btl_hlp_borodo_king", "menu_enemy_190", "c_borodo_g", "Z_1", "menu_monosiri_tou" } },
    { { "btl_un_crimson_togemet", "btl_hlp_crimson_togemet", "menu_enemy_191", "c_met_h", "TMT_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_ura_noko", "btl_hlp_ura_noko", "menu_enemy_031", "c_uranoko", "NKT_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_ura_pata", "btl_hlp_ura_pata", "menu_enemy_032", "c_uranoko", "PTP_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_kamec_red", "btl_hlp_kamec_red", "menu_enemy_169", "c_kamek_r", "KMR_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_kamec_red_bunsin", "btl_hlp_kamec_red_bunshin", 0, "c_kamek_r", "KMR_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_kamec_white", "btl_hlp_kamec_white", "menu_enemy_170", "c_kamek_w", "KMW_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_kamec_white_bunsin", "btl_hlp_kamec_white_bunshin", 0, "c_kamek_w", "KMW_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_kamec_green", "btl_hlp_kamec_green", "menu_enemy_171", "c_kamek_g", "KMG_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_kamec_green_bunsin", "btl_hlp_kamec_green_bunshin", 0, "c_kamek_g", "KMG_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_dark_keeper", "btl_hlp_dark_keeper", "menu_enemy_184", "c_monban_t", "Z_1", "menu_monosiri_tou" } },
    { { "btl_un_hammer_bros", "btl_hlp_hammer_bros", "menu_enemy_067", "c_burosu_h", "BRO_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_boomerang_bros", "btl_hlp_boomerang_bros", "menu_enemy_068", "c_burosu_b", "BRO_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_fire_bros", "btl_hlp_fire_bros", "menu_enemy_069", "c_burosu_f", "BRO_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_burst_wanwan", "btl_hlp_burst_wanwan", "menu_enemy_185", "c_wanwan_a", "WAN_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_togenoko_ace", "btl_hlp_togenoko_ace", "menu_enemy_175", "c_togenoko_b", "Z_1", "menu_monosiri_tou" } },
    { { "btl_un_iron_sinemon", "btl_hlp_iron_sinemon", "menu_enemy_038", "c_sinemon_a", "SIN_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_iron_sinemon2", "btl_hlp_iron_sinemon2", "menu_enemy_039", "c_sinemon_m", "SIN_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_koopa", "btl_hlp_koopa_tou", "menu_enemy_187", "c_koopa", "KPA_Z_1", "menu_monosiri_nara" } },
    { { "btl_un_champion", "btl_hlp_champion", "menu_enemy_142", "c_bred_h", "Z_1", "menu_monosiri_tou" } },
    { { "btl_un_macho_gance", "btl_hlp_macho_gansu", "menu_enemy_141", "c_m_gansu", "Z_1", "menu_monosiri_tou" } },
    { { "btl_un_hyper_kuriboo", "btl_hlp_hyper_kuriboo", "menu_enemy_023", "c_kuribo_h", "KUR_Z_2", "menu_monosiri_gra" } },
    { { "btl_un_hyper_patakuri", "btl_hlp_hyper_patakuri", "menu_enemy_025", "c_kuribo_h", "PTK_Z_2", "menu_monosiri_gra" } },
    { { "btl_un_hyper_togekuri", "btl_hlp_hyper_togekuri", "menu_enemy_024", "c_kuribo_h", "TGK_Z_2", "menu_monosiri_gra" } },
    { { "btl_un_pansy", "btl_hlp_pansy", "menu_enemy_082", "c_pansy", "PAN_Z_1", "menu_monosiri_gra" } },
    { { "btl_un_twinkling_pansy", "btl_hlp_twinkling_pansy", "menu_enemy_083", "c_kpansy", "PAN_Z_1", "menu_monosiri_gra" } },
    { { "btl_un_hyper_sinemon", "btl_hlp_hyper_sinemon", "menu_enemy_037", "c_sinemon_h", "SIN_Z_1", "menu_monosiri_gra" } },
    { { "btl_un_met", "btl_hlp_met", "menu_enemy_048", "c_met", "MET_Z_1", "menu_monosiri_jin" } },
    { { "btl_un_togemet", "btl_hlp_togemet", "menu_enemy_049", "c_met", "TMT_Z_1", "menu_monosiri_jin" } },
    { { "btl_un_basabasa", "btl_hlp_basabasa", "menu_enemy_040", "c_basabasa", "BSA_Z_1", "menu_monosiri_jin" } },
    { { "btl_un_teresa", "btl_hlp_teresa", "menu_enemy_052", "c_teresa", "Z_1", "menu_monosiri_jin" } },
    { { "btl_un_atmic_teresa", "btl_hlp_atomic_teresa", "menu_enemy_054", "c_atmic_trs", "Z_1", "menu_monosiri_jin" } },
    { { "btl_un_hatena", "btl_hlp_hatena", "menu_enemy_124", "c_ranpel", "Z_1", "menu_monosiri_jin" } },
    { { "btl_un_hatena", 0, 0, "c_ranpel", "Z_1", "menu_monosiri_gra" } },
    { { "btl_un_rampell", 0, 0, "c_ranpel", "Z_1", "menu_monosiri_jin" } },
    { { "btl_un_gullible_christine", 0, 0, "c_pkr", "PRK_Z_1", "menu_monosiri_jin" } },
    { { "btl_un_gullible_nokotarou", 0, 0, "c_pnk", "PNK_Z_1", "menu_monosiri_jin" } },
    { { "btl_un_gullible_yoshi", 0, 0, "c_babyyoshi", "PYS_Z_1", "menu_monosiri_jin" } },
    { { "btl_un_gullible_clauda", 0, 0, "c_windy", "PWD_Z_1", "menu_monosiri_jin" } },
    { { "btl_un_hermos", "btl_hlp_hermos", "menu_enemy_081", "c_elmos", "Z_1", "menu_monosiri_muj" } },
    { { "btl_un_bubble", "btl_hlp_bubble", "menu_enemy_080", "c_bubble", "Z_1", "menu_monosiri_dou" } },
    { { "btl_un_green_chorobon", "btl_hlp_green_chorobon", "menu_enemy_042", "c_chorobon_g", "CBN_Z_1", "menu_monosiri_muj" } },
    { { "btl_un_flower_chorobon", "btl_hlp_flower_chorobon", "menu_enemy_043", "c_chorobon_f", "CBN_Z_1", "menu_monosiri_muj" } },
    { { "btl_un_poison_pakkun", "btl_hlp_poison_pakkun", "menu_enemy_046", "c_pakflwr_p", "PKF_Z_1", "menu_monosiri_muj" } },
    { { "btl_un_patamet", "btl_hlp_patamet", "menu_enemy_050", "c_met", "PMT_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_killer_cannon", "btl_hlp_killer_cannon", "menu_enemy_059", "c_killtai", "KTA_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_killer", "btl_hlp_killer", "menu_enemy_057", "c_kilr", "KIL_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_heavy_bom", "btl_hlp_heavy_bom", "menu_enemy_064", "c_heavy", "Z_1", "menu_monosiri_dou" } },
    { { "btl_un_cortez", "btl_hlp_cortez", "menu_enemy_132", "c_korutesu1", "KRT_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_honeduka", 0, 0, "c_honezuka", "KRT_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_cortez_claw", 0, 0, "c_buki4", "KRT_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_cortez_rapier", 0, 0, "c_buki3", "KRT_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_cortez_sword", 0, 0, "c_buki2", "KRT_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_cortez_saber", 0, 0, "c_buki1", "KRT_Z_1", "menu_monosiri_dou" } },
    { { "btl_un_kanbu3", "btl_hlp_kanbu3", "menu_enemy_144", "c_kanbu", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_gundan_zako_group1", "btl_hlp_gundan_zako_group1", 0, "c_zako_n", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_gundan_zako_group2", "btl_hlp_gundan_zako_group1", 0, "c_zako_n", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_gundan_zako_group3", "btl_hlp_gundan_zako_group1", 0, "c_zako_n", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_kurokumorn", "btl_hlp_kurokumorn", "menu_enemy_065", "c_kmoon", "Z_1", "menu_monosiri_eki" } },
    { { "btl_un_sambo_mummy", "btl_hlp_sambo_mummy", "menu_enemy_056", "c_sanbo_m", "SNB_Z_1", "menu_monosiri_eki" } },
    { { "btl_un_patatogemet", "btl_hlp_patatogemet", "menu_enemy_051", "c_met", "PTM_Z_1", "menu_monosiri_eki" } },
    { { "btl_un_purple_teresa", "btl_hlp_purple_teresa", "menu_enemy_053", "c_teresa_p", "Z_1", "menu_monosiri_sin" } },
    { { "btl_un_moamoa", "btl_hlp_moamoa", "menu_enemy_118", "c_g_moamoa", "S_3", "menu_monosiri_rsh" } },
    { { "btl_un_moamoa_tentacle_a", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_rsh" } },
    { { "btl_un_moamoa_tentacl_b", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_rsh" } },
    { { "btl_un_moamoa_tentacle_c", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_rsh" } },
    { { "btl_un_moamoa_mouth", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_rsh" } },
    { { "btl_un_bllizard", "btl_hlp_bllizard", "menu_enemy_066", "c_kmoon_b", "Z_1", "menu_monosiri_bom" } },
    { { "btl_un_ice_pakkun", "btl_hlp_ice_pakkun", "menu_enemy_047", "c_pakflwr_a", "PKF_Z_1", "menu_monosiri_bom" } },
    { { "btl_un_sinemon", "btl_hlp_sinemon", "menu_enemy_034", "c_sinemon", "SIN_Z_1", "menu_monosiri_moo" } },
    { { "btl_un_barriern_z", "btl_hlp_barriern_z", "menu_enemy_073", "c_baria_z", "Z_1", "menu_monosiri_moo" } },
    { { "btl_un_barriern_satellite_z", "btl_hlp_barriern_z_petit", "menu_enemy_074", "c_baria_z", "Z_2", "menu_monosiri_moo" } },
    { { "btl_un_barriern_custom", "btl_hlp_barriern_custom", "menu_enemy_179", "c_baria_c", "Z_1", "menu_monosiri_aji" } },
    { { "btl_un_barriern_custom_satellite", "btl_hlp_barriern_custom_satellite", "menu_enemy_180", "c_baria_c", "Z_2", "menu_monosiri_aji" } },
    { { "btl_un_gundan_zako_magician", "btl_hlp_gundan_zako_magician", "menu_enemy_075", "c_zako_m", "Z_1", "menu_monosiri_aji" } },
    { { "btl_un_gundan_zako_elite", "btl_hlp_gundan_zako_elite", "menu_enemy_076", "c_zako_e", "Z_1", "menu_monosiri_aji" } },
    { { "btl_un_magnum_battender_mkii", "btl_hlp_magnum_battender_mkII", "menu_enemy_140", "c_mb_robo_2b", "MGN_Z_1", "menu_monosiri_aji" } },
    { { "btl_un_rocket_punch_mkii", 0, 0, "c_mb_robo_1b", "MGN_A1_4", "menu_monosiri_aji" } },
    { { "btl_un_basabasa_chururu", "btl_hlp_basabasa_chururu", "menu_enemy_041", "c_cyuru", "BSA_Z_1", "menu_monosiri_las" } },
    { { "btl_un_phantom", "btl_hlp_phantom", "menu_enemy_178", "c_phantom", "Z_1", "menu_monosiri_las" } },
    { { "btl_un_super_killer_cannon", "btl_hlp_super_killer_cannon", "menu_enemy_060", "c_killtai_g", "KTA_Z_1", "menu_monosiri_las" } },
    { { "btl_un_super_killer", "btl_hlp_super_killer", "menu_enemy_058", "c_kilr_g", "KIL_Z_1", "menu_monosiri_las" } },
    { { "btl_un_wanwan", "btl_hlp_wanwan", "menu_enemy_079", "c_wanwan", "WAN_Z_1", "menu_monosiri_las" } },
    { { "btl_un_super_mahorn", "btl_hlp_super_mahorn", "menu_enemy_071", "c_maho_b", "MAH_Z_M", "menu_monosiri_las" } },
    { { "btl_un_super_mahorn_bunsin", "btl_hlp_super_mahorn_bunsin", 0, "c_maho_b", "MAH_Z_M", "menu_monosiri_las" } },
    { { "btl_un_karon", "btl_hlp_karon", "menu_enemy_044", "c_karon_h", "KRN_Z_1", "menu_monosiri_las" } },
    { { "btl_un_black_karon", "btl_hlp_black_karon", "menu_enemy_045", "c_karon_b_h", "KRN_Z_1", "menu_monosiri_las" } },
    { { "btl_un_bunbaba", "btl_hlp_bunbaba", "menu_enemy_196", "c_gonbaba_b", "GNB_Z_1", "menu_monosiri_las" } },
    { { "btl_un_majolyne4", "btl_hlp_majyorin2", "menu_enemy_107", "c_majyorin", "MJR_M_1", "menu_monosiri_nara" } },
    { { "btl_un_marilyn4", "btl_hlp_maririn2", "menu_enemy_111", "c_maririn", "MRR_M_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", "menu_enemy_125", "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_rampell", "btl_hlp_ranperu_las", 0, "c_ranpel", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_koopa", "btl_hlp_koopa", "menu_enemy_149", "c_koopa", "KPA_Z_1", "menu_monosiri_nara" } },
    { { "btl_un_kamec_obaba", "btl_hlp_kamec_obaba", "menu_enemy_150", "c_kamek_bb", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_batten_leader", "btl_hlp_batsugalf", "menu_enemy_151", "c_shuryo", "Z_1", "menu_monosiri_las" } },
    { { "btl_un_batten_satellite", "btl_hlp_batsu_satellite", "menu_enemy_152", "c_shuryo_p", "Z_1", "menu_monosiri_las" } },
    { { "btl_un_black_peach", "btl_hlp_shadowqueen1", 0, "c_b_peach_b", "KRT_Z_1", "menu_monosiri_las" } },
    { { "btl_un_shadow_queen", 0, 0, "c_q_kage_m", "KRT_Z_1", "menu_monosiri_las" } },
    { { "btl_un_shadow_queen", "btl_hlp_shadowqueen2", "menu_enemy_154", "c_q_kage_m", "KRT_Z_1", "menu_monosiri_las" } },
    { { "btl_un_shadow_queen", 0, 0, "c_b_hand", "KRT_Z_1", "menu_monosiri_las" } },
    { { "btl_un_shadow_queen", 0, 0, "c_s_hand", "KRT_Z_1", "menu_monosiri_las" } },
    { { "btl_un_yami_kuriboo", "btl_hlp_yami_kuriboo", "menu_enemy_026", "c_kuribo_y", "KUR_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_yami_patakuri", "btl_hlp_yami_patakuri", "menu_enemy_028", "c_kuribo_y", "PTK_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_yami_togekuri", "btl_hlp_yami_togekuri", "menu_enemy_027", "c_kuribo_y", "TGK_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_yami_noko", "btl_hlp_yami_noko", "menu_enemy_029", "c_yaminoko", "NKT_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_yami_pata", "btl_hlp_yami_pata", "menu_enemy_030", "c_yaminoko", "PTP_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_badge_borodo", "btl_hlp_badge_borodo", "menu_enemy_078", "c_borodo_t", "Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_hyper_jyugem", "btl_hlp_hyper_jyugem", "menu_enemy_186", "c_jugemu_b", "JGM_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_hyper_togezo", "btl_hlp_hyper_togezo", "menu_enemy_173", "c_togezo_b", "TGZ_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_mahorn", "btl_hlp_mahorn", "menu_enemy_070", "c_maho", "MAH_Z_M", "menu_monosiri_shiga" } },
    { { "btl_un_pakkun_flower", "btl_hlp_pakkun_flower", "menu_enemy_014", "c_pakflwr", "PKF_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_hennya", "btl_hlp_hennya", "menu_enemy_022", "c_hannya_t", "Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_churantalar", "btl_hlp_churantalar", "menu_enemy_176", "c_paid_b", "PAI_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_yamitogedaruma", "btl_hlp_yamitogedaruma", "menu_enemy_174", "c_togedaruma_b", "TGD_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_dokugassun", "btl_hlp_dokugassun", "menu_enemy_013", "c_kmoon_g", "Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_basabasa_green", "btl_hlp_basabasa_green", "menu_enemy_177", "c_basabasa_g", "BSA_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_giant_bomb", "btl_hlp_giant_bomb", "menu_enemy_182", "c_giant", "Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_mahorn_custom", "btl_hlp_mahorn_custom", "menu_enemy_183", "c_maho_w", "MAH_Z_M", "menu_monosiri_shiga" } },
    { { "btl_un_mahorn_custom_bunsin", "btl_hlp_mahorn_custom_bunshin", 0, "c_maho_w", "MAH_Z_M", "menu_monosiri_shiga" } },
    { { "btl_un_zonbaba", "btl_hlp_zonbaba", "menu_enemy_195", "c_gonbaba_z", "GNB_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_crimson_met", "btl_hlp_crimson_met", 0, "c_met_h", "MET_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_crimson_patamet", "btl_hlp_crimson_patamet", 0, "c_met_h", "PMT_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_crimson_patatogemet", "btl_hlp_crimson_patatogemet", 0, "c_met_h", "PTM_Z_1", "menu_monosiri_tou" } },
    { { "btl_un_hyper_bomhei", "btl_hlp_hyper_bomhei", 0, "c_bomhey_h", "BOM_Z_1", "menu_monosiri_jon" } },
    { { "btl_un_ultra_bomhei", "btl_hlp_ultra_bomhei", 0, "c_bomhey_m", "BOM_Z_1", "menu_monosiri_jon" } },
    { { "btl_un_christine", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_frankli", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_frankli", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_frankli", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_act_mario", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_act_clauda", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_act_teresa", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_act_atmic_teresa", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_act_kinopio", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_act_kinopiko", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_test", "btl_hlp_kuriboo", 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gor" } },
    { { "btl_un_kanbu2", "btl_hlp_kanbu2", 0, "c_kanbu", "Z_1", "menu_monosiri_gon" } },
    { { "btl_un_majolyne2", "btl_hlp_majyorin", 0, "c_majyorin", "MJR_Z_1", "menu_monosiri_nara" } },
    { { "btl_un_marilyn2", "btl_hlp_maririn", 0, "c_maririn", "MRR_Z_1", "menu_monosiri_shiga" } },
    { { "btl_un_vivian2", "btl_hlp_vivian", 0, "c_vivian", "PTR_Z_1", "menu_monosiri_gor" } },
    { { "btl_un_majolyne3", "btl_hlp_majyorin", 0, "c_majyorin", "MJR_Z_1", "menu_monosiri_gon" } },
    { { "btl_un_marilyn3", "btl_hlp_maririn", 0, "c_maririn", "MRR_Z_1", "menu_monosiri_nara" } },
    { { "btl_un_mecha_kuri", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_mecha_kame", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gor" } },
    { { "btl_un_okorl", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gon" } },
    { { "btl_un_yowarl", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_nara" } },
    { { "btl_un_tuyonarl", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_wanawana", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gor" } },
    { { "btl_un_minarai_kamec", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gon" } },
    { { "btl_un_heiho", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_nara" } },
    { { "btl_un_dancing_heiho", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_fire_heiho", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gor" } },
    { { "btl_un_command_heiho", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gon" } },
    { { "btl_un_black_heiho", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_nara" } },
    { { "btl_un_hatty", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_kohatty", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gor" } },
    { { "btl_un_ufo", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_gon" } },
    { { "btl_un_powan", 0, 0, "c_powan", "Z_1", "menu_monosiri_nara" } },
    { { "btl_un_fighter", 0, 0, "c_kuribo", "KUR_Z_2", "menu_monosiri_shiga" } },
    { { "btl_un_nancy", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_master", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_reporter", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_hotdog_master", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_marco", 0, 0, "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_tree", 0, "menu_enemy_155", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_switch", 0, "menu_enemy_156", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_testnpc", 0, "menu_enemy_157", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_bomzou", "btl_hlp_bomzou", "menu_enemy_158", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_system", 0, "menu_enemy_159", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_christine", 0, "menu_enemy_162", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_mario", "btl_hlp_mario", "menu_enemy_160", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_koura", 0, "menu_enemy_161", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_christine", 0, "menu_enemy_162", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_nokotarou", 0, "menu_enemy_163", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_yoshi", 0, "menu_enemy_164", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_clauda", 0, "menu_enemy_165", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_vivian", "btl_hlp_vivian_party", "menu_enemy_166", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_sanders", 0, "menu_enemy_167", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
    { { "btl_un_chuchurina", 0, "menu_enemy_168", "c_kuribo", "KUR_Z_2", "KUR_Z_2" } },
};

s32 same_tbl_rampell_1[] = { 77, 135, 79, 78, 136, 137, 138, 139, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_2[] = { 135, 77, 79, 78, 136, 137, 138, 139, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_1[] = { 79, 77, 135, 78, 136, 137, 138, 139, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_2[] = { 78, 77, 135, 79, 136, 137, 138, 139, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_3[] = { 136, 77, 135, 79, 78, 137, 138, 139, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_4[] = { 137, 77, 135, 79, 78, 136, 138, 139, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_5[] = { 138, 77, 135, 79, 78, 136, 137, 139, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_6[] = { 139, 77, 135, 79, 78, 136, 137, 138, 140, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_7[] = { 140, 77, 135, 79, 78, 136, 137, 138, 139, 141, 142, 143, 0 };

s32 same_tbl_rampell_faker_8[] = { 141, 77, 135, 79, 78, 136, 137, 138, 139, 140, 142, 143, 0 };

s32 same_tbl_rampell_faker_9[] = { 142, 77, 135, 79, 78, 136, 137, 138, 139, 140, 141, 143, 0 };

s32 same_tbl_rampell_faker_10[] = { 143, 77, 135, 79, 78, 136, 137, 138, 139, 140, 141, 142, 0 };


s32* battle_monosiri_same_tbl[] = {
    same_tbl_gesso,
    same_tbl_gesso_arm_l,
    same_tbl_gesso_arm_r,
    same_tbl_rampell_1,
    same_tbl_rampell_2,
    same_tbl_rampell_faker_1,
    same_tbl_rampell_faker_2,
    same_tbl_rampell_faker_3,
    same_tbl_rampell_faker_4,
    same_tbl_rampell_faker_5,
    same_tbl_rampell_faker_6,
    same_tbl_rampell_faker_7,
    same_tbl_rampell_faker_8,
    same_tbl_rampell_faker_9,
    same_tbl_rampell_faker_10,
    same_tbl_majolyne_1,
    same_tbl_majolyne_2,
    same_tbl_majolyne_3,
    same_tbl_majolyne_4,
    same_tbl_marilyn_1,
    same_tbl_marilyn_2,
    same_tbl_marilyn_3,
    same_tbl_marilyn_4,
    same_tbl_vivian_1,
    same_tbl_vivian_2,
    same_tbl_koopa_1,
    same_tbl_koopa_2,
    same_tbl_kamec,
    same_tbl_kamec_bunshin,
    same_tbl_black_peach,
    same_tbl_kamec_red,
    same_tbl_kamec_red_bunshin,
    same_tbl_kamec_white,
    same_tbl_kamec_white_bunshin,
    same_tbl_kamec_green,
    same_tbl_kamec_green_bunshin,
    same_tbl_super_mahorn,
    same_tbl_super_mahorn_bunshin,
    same_tbl_mahorn_custom,
    same_tbl_mahorn_custom_bunshin,
    same_tbl_kuriboo,
    same_tbl_chorobon,
    same_tbl_gundan_zako,
    same_tbl_cortez,
    same_tbl_magunum_battender,
    same_tbl_magunum_battender_mkii,
    same_tbl_moamoa,
    unk_80417170,
    0,
};

void* battleGetUnitMonosiriPtr(s32 id) {
    return &monosiriMessageTable[id];
}


s32 battleCheckUnitMonosiriFlag(void* unit) {
    void* battleWork;
    u8* highWork;
    s32 id;
    s32 flags;
    s32 result;
    s32 word;
    s32 bit;
    s32 sameId;

    battleWork = _battleWorkPointer;
    id = *(s32*)((s32)unit + 8);

    if ((*(u32*)((s32)battleWork + 0x163F4) & 2) != 0) {
        return 1;
    }

    result = swGet(id + 0x117A);
    flags = *(s32*)((s32)unit + 0x1C);
    word = id / 32;
    bit = id % 32;
    highWork = (u8*)battleWork + 0x10000;
    result |= *(u32*)(highWork + 0x63D4 + word * 4) & (1 << bit);

    if ((flags & 4) != 0) {
        sameId = *(s32*)((s32)unit + 4);
        word = sameId / 32;
        bit = sameId % 32;
        result |= *(u32*)(highWork + 0x63D4 + word * 4) & (1 << bit);
    }

    if (result != 0) {
        return 1;
    }
    return 0;
}


#pragma opt_common_subs off
void battleSetUnitMonosiriFlag(void* unit) {
    void* wp = _battleWorkPointer;
    s32 kind = (s32)unit;
    s32** table = battle_monosiri_same_tbl;
    s32* list;
    s32 i = 0;
    s32 j;
    s32 k;
    s32 entry;
    s32 offset;
    while (1) {
        list = table[i];
        if (list == 0) break;
        j = 0;
        while ((entry = list[j]) != 0) {
            if (kind == entry) {
                k = 0;
                while ((kind = list[k]) != 0) {
                    swSet(kind + 0x117A);
                    offset = (kind / 32) * 4 + 0x163D4;
                    *(u32*)((u8*)wp + offset) |= 1 << (kind % 32);
                    k++;
                }
                return;
            }
            j++;
        }
        i++;
    }
    swSet(kind + 0x117A);
    *(u32*)((s32)wp + 0x163D4 + (kind / 32) * 4) |= 1 << (kind % 32);
}
#pragma opt_common_subs reset
