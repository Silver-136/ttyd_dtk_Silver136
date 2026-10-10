#include "effect/n64/eff_n64.h"

static u32 work[2];
static void* wp = work;

extern void* gp;
extern void GXInitTexObj();
extern void TEXGetGXTexObjFromPalette();
extern void UnpackTexPalette();
extern void DVDMgrClose();
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32 attr, s32 type);
extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
extern void GXSetArray(s32 attr, void* base, s32 stride);
extern s32 strcmp(const char* s1, const char* s2);
extern void* arcOpen(const char* name, s32 unused0, s32 unused1);
extern char* getMarioStDvdRoot(void);
extern int sprintf(char* str, const char* format, ...);
extern s32 DVDMgrOpen(const char* path, s32 a, s32 b);
extern s32 DVDMgrGetLength(s32 entry);
extern void* __memAlloc(s32 heap, u32 size);
extern void DVDMgrReadAsync(s32 entry, void* dst, s32 size, s32 offset, void* callback);
static void _callback_tpl(void* unused, void* fileInfo);

typedef struct EffSetN64 {
    s16 id;
    s16 pad;
    const char* name;
} EffSetN64;

extern EffSetN64 eff_set_n64[];
extern s32 _rand_table[];
extern const char str_effect_n64_tpl_802fbafc[];
extern const char str_PCTs_e_PCTs_effect_n_802fbb0c[];
extern const char str_jp_80425b08[3];
extern const char str_us_80425b0c[3];

u8 dummy_425[1] __attribute__ ((aligned(32))) = {0};

static void _callback_tpl(void* unused, void* fileInfo) {
    void* file = *(void**)((s32)fileInfo + 0x2C);

    UnpackTexPalette(*(void**)wp);
    DVDMgrClose(file);
    *(s32*)((s32)wp + 4) = 1;
}

u8 effInit64(void) {
    memset(wp, 0, 8);
    *(s32*)((s32)wp + 4) = 0;
}

#pragma use_lmw_stmw off
void effTexSetupN64(void) {
    char path[0x80];
    char* language;
    s32 file;
    s32 size;

    *(void**)wp = NULL;
    *(s32*)((s32)wp + 4) = 0;
    *(void**)wp = arcOpen(str_effect_n64_tpl_802fbafc, 0, 0);

    if (*(void**)wp == NULL) {
        language = (char*)str_us_80425b0c;
        if (*(u32*)((s32)gp + 0x16C) == 0) {
            language = (char*)str_jp_80425b08;
        }

        sprintf(path, str_PCTs_e_PCTs_effect_n_802fbb0c, getMarioStDvdRoot(), language);
        file = DVDMgrOpen(path, 2, 0);
        size = (DVDMgrGetLength(file) + 0x1F) & ~0x1F;
        *(void**)wp = __memAlloc(0, size);
        *(s32*)((s32)file + 0x6C) = file;
        DVDMgrReadAsync(file, *(void**)wp, size, 0, _callback_tpl);
    } else {
        UnpackTexPalette(*(void**)wp);
        *(s32*)((s32)wp + 4) = 1;
    }
}
#pragma use_lmw_stmw on


u8 effGetTexObjN64(int param_1, u32* param_2) {
    void* work = wp;

    if (*(s32*)((s32)work + 4) == 0) {
        GXInitTexObj(param_2, dummy_425, 1, 1, 0, 0, 0, 0);
    } else {
        TEXGetGXTexObjFromPalette(*(void**)work, param_2, param_1);
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effGetSetN64(char* name) {
    EffSetN64* set;
    s32 i;

    set = eff_set_n64;
    i = 0;
    while (set->id != -1) {
        if (strcmp(set->name, name) == 0) {
            return &eff_set_n64[i];
        }
        set++;
        i++;
    }
    return NULL;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off



int effTblRandN64(int param_1, int param_2) {
    s32 value;

    value = _rand_table[param_2 % 128];
    if (value < 0) {
        value = -value;
    }
    return value % (param_1 + 1);
}

void effSetVtxDescN64(void* data) {
    GXClearVtxDesc();
    GXSetVtxDesc(9, 3);
    GXSetVtxDesc(0xB, 3);
    GXSetVtxDesc(0xD, 3);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 5);
    GXSetArray(9, data, 0xE);
    GXSetArray(0xB, (void*)((s32)data + 0xA), 0xE);
    GXSetArray(0xD, (void*)((s32)data + 0x6), 0xE);
}


void tri1(s16 param_1, s16 param_2, s16 param_3) {
    volatile u16* fifo = (volatile u16*)0xCC008000;

    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_3;
    *fifo = param_3;
    *fifo = param_3;
}


u8 tri2(s16 param_1, s16 param_2, s16 param_3, s32 param_4, s16 param_5, s16 param_6, s16 param_7) {
    volatile u16* fifo = (volatile u16*)0xCC008000;

    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_1;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_2;
    *fifo = param_3;
    *fifo = param_3;
    *fifo = param_3;
    *fifo = param_5;
    *fifo = param_5;
    *fifo = param_5;
    *fifo = param_6;
    *fifo = param_6;
    *fifo = param_6;
    *fifo = param_7;
    *fifo = param_7;
    *fifo = param_7;
}

/* Target-backed N64 effect registry/string catalog. */
const char str_expbom_n64_802fb4e0[] = "expbom_n64";
const char str_kemuri1_n64_802fb4ec[] = "kemuri1_n64";
const char str_kemuri2_n64_802fb4f8[] = "kemuri2_n64";
const char str_kemuri3_n64_802fb504[] = "kemuri3_n64";
const char str_kemuri4_n64_802fb510[] = "kemuri4_n64";
const char str_kemuri5_n64_802fb51c[] = "kemuri5_n64";
const char str_kemuri6_n64_802fb528[] = "kemuri6_n64";
const char str_kemuri7_n64_802fb534[] = "kemuri7_n64";
const char str_flower_n64_802fb540[] = "flower_n64";
const char str_snow_n64_802fb54c[] = "snow_n64";
const char str_sstar_n64_802fb558[] = "sstar_n64";
const char str_stardust_n64_802fb564[] = "stardust_n64";
const char str_magic1_n64_802fb574[] = "magic1_n64";
const char str_Pymagic2_n64_Pw_802fb580[] = "@Pymagic2_n64@Pw";
const char str_fall_leaf_n64_802fb594[] = "fall_leaf_n64";
const char str_hokori_n64_802fb5a4[] = "hokori_n64";
const char str_gareki_n64_802fb5b0[] = "gareki_n64";
const char str_kemuri8_n64_802fb5bc[] = "kemuri8_n64";
const char str_small_star_n64_802fb5c8[] = "small_star_n64";
const char str_bomb_n64_802fb5d8[] = "bomb_n64";
const char str_wangflush_n64_802fb5e4[] = "wangflush_n64";
const char str_magic3_n64_802fb5f4[] = "magic3_n64";
const char str_kemuri9_n64_802fb600[] = "kemuri9_n64";
const char str_kemuri10_n64_802fb60c[] = "kemuri10_n64";
const char str_Pyfire_n64_Pw_802fb61c[] = "@Pyfire_n64@Pw";
const char str_rappa_n64_802fb62c[] = "rappa_n64";
const char str_stardust2_n64_802fb638[] = "stardust2_n64";
const char str_ripple_n64_802fb648[] = "ripple_n64";
const char str_Pyexplosion_n64_Pw_802fb654[] = "@Pyexplosion_n64@Pw";
const char str_kemuri11_n64_802fb668[] = "kemuri11_n64";
const char str_onpu_n64_802fb678[] = "onpu_n64";
const char str_kemuri12_n64_802fb684[] = "kemuri12_n64";
const char str_sweat_n64_802fb694[] = "sweat_n64";
const char str_balloon_n64_802fb6a0[] = "balloon_n64";
const char str_cool_leaf_n64_802fb6ac[] = "cool_leaf_n64";
const char str_bush_n64_802fb6bc[] = "bush_n64";
const char str_rothammer_success_n6_802fb6c8[] = "rothammer_success_n64";
const char str_rothammer_fail_n64_802fb6e0[] = "rothammer_fail_n64";
const char str_guruguru_n64_802fb6f4[] = "guruguru_n64";
const char str_ice_n64_802fb704[] = "ice_n64";
const char str_stamp_n64_802fb70c[] = "stamp_n64";
const char str_Pythruhammer_n64_Pw_802fb718[] = "@Pythruhammer_n64@Pw";
const char str_radiation_n64_802fb730[] = "radiation_n64";
const char str_thunder_n64_802fb740[] = "thunder_n64";
const char str_breath_fire_n64_802fb74c[] = "breath_fire_n64";
const char str_broken_barrier_n64_802fb75c[] = "broken_barrier_n64";
const char str_fire_ring_n64_802fb770[] = "fire_ring_n64";
const char str_fire_dust_n64_802fb780[] = "fire_dust_n64";
const char str_Pyflame_n64_Pw_802fb790[] = "@Pyflame_n64@Pw";
const char str_blow_n64_802fb7a0[] = "blow_n64";
const char str_bomhei_n64_802fb7ac[] = "bomhei_n64";
const char str_fireflower_n64_802fb7b8[] = "fireflower_n64";
const char str_recover_heart_n64_802fb7c8[] = "recover_heart_n64";
const char str_pturn_n64_802fb7dc[] = "pturn_n64";
const char str_hit_n64_802fb7e8[] = "hit_n64";
const char str_confetti_n64_802fb7f0[] = "confetti_n64";
const char str_snowfall_n64_802fb800[] = "snowfall_n64";
const char str_whirlwind_n64_802fb810[] = "whirlwind_n64";
const char str_thunderflare_n64_802fb820[] = "thunderflare_n64";
const char str_coin_n64_802fb834[] = "coin_n64";
const char str_shock_n64_802fb840[] = "shock_n64";
const char str_balloon_heiho_n64_802fb84c[] = "balloon_heiho_n64";
const char str_rubble_n64_802fb860[] = "rubble_n64";
const char str_magic_horao_n64_802fb86c[] = "magic_horao_n64";
const char str_lensflare_n64_802fb87c[] = "lensflare_n64";
const char str_crystal_n64_802fb88c[] = "crystal_n64";
const char str_butterfly_n64_802fb898[] = "butterfly_n64";
const char str_akari_charge_n64_802fb8a8[] = "akari_charge_n64";
const char str_akaro_sflush_n64_802fb8bc[] = "akaro_sflush_n64";
const char str_opuku_wgun_n64_802fb8d0[] = "opuku_wgun_n64";
const char str_opuku_wprotect_n64_802fb8e0[] = "opuku_wprotect_n64";
const char str_opuku_wcurtain_n64_802fb8f4[] = "opuku_wcurtain_n64";
const char str_opuku_jetw_n64_802fb908[] = "opuku_jetw_n64";
const char str_opuku_jetwrain_n64_802fb918[] = "opuku_jetwrain_n64";
const char str_pokopi_thunder_n64_802fb92c[] = "pokopi_thunder_n64";
const char str_water_damage_n64_802fb940[] = "water_damage_n64";
const char str_snowman_n64_802fb954[] = "snowman_n64";
const char str_holography_n64_802fb960[] = "holography_n64";
const char str_timestamp_n64_802fb970[] = "timestamp_n64";
const char str_packn_bfire_n64_802fb980[] = "packn_bfire_n64";
const char str_pokopi_pipo_n64_802fb990[] = "pokopi_pipo_n64";
const char str_cloud_n64_802fb9a0[] = "cloud_n64";
const char str_dogabon_attack_n64_802fb9ac[] = "dogabon_attack_n64";
const char str_kameki_tornade_n64_802fb9c0[] = "kameki_tornade_n64";
const char str_stone_smoke_n64_802fb9d4[] = "stone_smoke_n64";
const char str_pokopi_pcharge_n64_802fb9e4[] = "pokopi_pcharge_n64";
const char str_hagetaka_flush_n64_802fb9f8[] = "hagetaka_flush_n64";
const char str_kumokumo_n64_802fba0c[] = "kumokumo_n64";
const char str_vapor_n64_802fba1c[] = "vapor_n64";
const char str_fire_spark_n64_802fba28[] = "fire_spark_n64";
const char str_horao_spell_n64_802fba38[] = "horao_spell_n64";
const char str_snow_dust_n64_802fba48[] = "snow_dust_n64";
const char str_akari_pose_n64_802fba58[] = "akari_pose_n64";
const char str_kumokumo_chip_n64_802fba68[] = "kumokumo_chip_n64";
const char str_fireworks_n64_802fba7c[] = "fireworks_n64";
const char str_glass_n64_802fba8c[] = "glass_n64";
const char str_booboo_n64_802fba98[] = "booboo_n64";
const char str_pikkyolo_n64_802fbaa4[] = "pikkyolo_n64";
const char str_freeze_n64_802fbab4[] = "freeze_n64";
const char str_sunlight_n64_802fbac0[] = "sunlight_n64";
const char str_release_n64_802fbad0[] = "release_n64";
const char str_peach_dust_n64_802fbadc[] = "peach_dust_n64";
const char str_star_power_n64_802fbaec[] = "star_power_n64";
const char str_effect_n64_tpl_802fbafc[] = "effect_n64.tpl";
const char str_PCTs_e_PCTs_effect_n_802fbb0c[] = "%s/e/%s/effect_n64.tpl";

const char str_jp_80425b08[3] = "jp";
const char str_us_80425b0c[3] = "us";

EffSetN64 eff_set_n64[104] = {
    {0, 0, str_expbom_n64_802fb4e0},
    {1, 0, str_kemuri1_n64_802fb4ec},
    {2, 0, str_kemuri2_n64_802fb4f8},
    {3, 0, str_kemuri3_n64_802fb504},
    {4, 0, str_kemuri4_n64_802fb510},
    {5, 0, str_kemuri5_n64_802fb51c},
    {6, 0, str_kemuri6_n64_802fb528},
    {7, 0, str_kemuri7_n64_802fb534},
    {8, 0, str_flower_n64_802fb540},
    {9, 0, str_snow_n64_802fb54c},
    {10, 0, str_sstar_n64_802fb558},
    {11, 0, str_stardust_n64_802fb564},
    {12, 0, str_magic1_n64_802fb574},
    {13, 0, str_Pymagic2_n64_Pw_802fb580},
    {14, 0, str_fall_leaf_n64_802fb594},
    {15, 0, str_hokori_n64_802fb5a4},
    {16, 0, str_gareki_n64_802fb5b0},
    {17, 0, str_kemuri8_n64_802fb5bc},
    {18, 0, str_small_star_n64_802fb5c8},
    {19, 0, str_bomb_n64_802fb5d8},
    {20, 0, str_wangflush_n64_802fb5e4},
    {21, 0, str_magic3_n64_802fb5f4},
    {22, 0, str_kemuri9_n64_802fb600},
    {23, 0, str_kemuri10_n64_802fb60c},
    {24, 0, str_Pyfire_n64_Pw_802fb61c},
    {25, 0, str_rappa_n64_802fb62c},
    {26, 0, str_stardust2_n64_802fb638},
    {27, 0, str_ripple_n64_802fb648},
    {28, 0, str_Pyexplosion_n64_Pw_802fb654},
    {29, 0, str_kemuri11_n64_802fb668},
    {30, 0, str_onpu_n64_802fb678},
    {31, 0, str_kemuri12_n64_802fb684},
    {32, 0, str_sweat_n64_802fb694},
    {33, 0, str_balloon_n64_802fb6a0},
    {34, 0, str_cool_leaf_n64_802fb6ac},
    {35, 0, str_bush_n64_802fb6bc},
    {36, 0, str_rothammer_success_n6_802fb6c8},
    {37, 0, str_rothammer_fail_n64_802fb6e0},
    {38, 0, str_guruguru_n64_802fb6f4},
    {39, 0, str_ice_n64_802fb704},
    {40, 0, str_stamp_n64_802fb70c},
    {41, 0, str_Pythruhammer_n64_Pw_802fb718},
    {42, 0, str_radiation_n64_802fb730},
    {43, 0, str_thunder_n64_802fb740},
    {44, 0, str_breath_fire_n64_802fb74c},
    {45, 0, str_broken_barrier_n64_802fb75c},
    {46, 0, str_fire_ring_n64_802fb770},
    {47, 0, str_fire_dust_n64_802fb780},
    {48, 0, str_Pyflame_n64_Pw_802fb790},
    {49, 0, str_blow_n64_802fb7a0},
    {50, 0, str_bomhei_n64_802fb7ac},
    {51, 0, str_fireflower_n64_802fb7b8},
    {52, 0, str_recover_heart_n64_802fb7c8},
    {53, 0, str_pturn_n64_802fb7dc},
    {54, 0, str_hit_n64_802fb7e8},
    {55, 0, str_confetti_n64_802fb7f0},
    {56, 0, str_snowfall_n64_802fb800},
    {57, 0, str_whirlwind_n64_802fb810},
    {58, 0, str_thunderflare_n64_802fb820},
    {59, 0, str_coin_n64_802fb834},
    {60, 0, str_shock_n64_802fb840},
    {61, 0, str_balloon_heiho_n64_802fb84c},
    {62, 0, str_rubble_n64_802fb860},
    {63, 0, str_magic_horao_n64_802fb86c},
    {64, 0, str_lensflare_n64_802fb87c},
    {65, 0, str_crystal_n64_802fb88c},
    {66, 0, str_butterfly_n64_802fb898},
    {67, 0, str_akari_charge_n64_802fb8a8},
    {68, 0, str_akaro_sflush_n64_802fb8bc},
    {69, 0, str_opuku_wgun_n64_802fb8d0},
    {70, 0, str_opuku_wprotect_n64_802fb8e0},
    {71, 0, str_opuku_wcurtain_n64_802fb8f4},
    {72, 0, str_opuku_jetw_n64_802fb908},
    {73, 0, str_opuku_jetwrain_n64_802fb918},
    {74, 0, str_pokopi_thunder_n64_802fb92c},
    {75, 0, str_water_damage_n64_802fb940},
    {76, 0, str_snowman_n64_802fb954},
    {77, 0, str_holography_n64_802fb960},
    {78, 0, str_timestamp_n64_802fb970},
    {79, 0, str_packn_bfire_n64_802fb980},
    {80, 0, str_pokopi_pipo_n64_802fb990},
    {81, 0, str_cloud_n64_802fb9a0},
    {82, 0, str_dogabon_attack_n64_802fb9ac},
    {83, 0, str_kameki_tornade_n64_802fb9c0},
    {84, 0, str_stone_smoke_n64_802fb9d4},
    {85, 0, str_pokopi_pcharge_n64_802fb9e4},
    {86, 0, str_hagetaka_flush_n64_802fb9f8},
    {87, 0, str_kumokumo_n64_802fba0c},
    {88, 0, str_vapor_n64_802fba1c},
    {89, 0, str_fire_spark_n64_802fba28},
    {90, 0, str_horao_spell_n64_802fba38},
    {91, 0, str_snow_dust_n64_802fba48},
    {92, 0, str_akari_pose_n64_802fba58},
    {93, 0, str_kumokumo_chip_n64_802fba68},
    {94, 0, str_fireworks_n64_802fba7c},
    {95, 0, str_glass_n64_802fba8c},
    {96, 0, str_booboo_n64_802fba98},
    {97, 0, str_pikkyolo_n64_802fbaa4},
    {98, 0, str_freeze_n64_802fbab4},
    {99, 0, str_sunlight_n64_802fbac0},
    {100, 0, str_release_n64_802fbad0},
    {101, 0, str_peach_dust_n64_802fbadc},
    {102, 0, str_star_power_n64_802fbaec},
    {-1, 0, NULL},
};

s32 _rand_table[128] = {
    269167349, 1169529124, 889801541, 1254073978, 374297457, 2065258565, 1482041942, 628309313,
    1207992583, 234901288, 1768143021, 1535290225, 1807873307, 1033140246, 963662197, 1145084505,
    249139303, 1601222392, 841330414, 146139516, 752776596, 1587436413, 1529489544, 276762448,
    1492895035, 1710661401, 989253363, 365136207, 1284708908, 967350249, 1497742220, 1553876257,
    2136879373, 240060114, 660466588, 363693183, 157835031, 2142508530, 992002496, 1365827614,
    229926170, 612661869, 753634191, 2037799477, 1645802115, 1717296388, 1035400335, 1894941419,
    1171035132, 1876730749, 2041080935, 1923811728, 1316683514, 1423086832, 53090528, 662094901,
    986264585, 1042343892, 1027231109, 123489845, 2009694141, 377489681, 1677366103, 1999089867,
    617549795, 190349043, 215299402, 775384826, 185373925, 1207301898, 2141212441, 415300096,
    1819963768, 747362984, 305615925, 1318282235, 317175725, 1341016260, 1065740006, 1488210857,
    1070263361, 959337294, 1264538938, 239463228, 234940478, 1317629466, 901558129, 1221205063,
    212489710, 1928789238, 1344694909, 74700204, 158795272, 874577364, 2073790071, 776345067,
    1064926407, 141605825, 1551729894, 1250300332, 1348907723, 1545458687, 1665600428, 1021387843,
    145338023, 1971216354, 192186431, 462513748, 1164748966, 1257926437, 1950724606, 87528680,
    69780083, 1067779896, 326991908, 304720561, 237925714, 1228550037, 1525925625, 450415425,
    1009855628, 723136886, 525115629, 1168650900, 1597714250, 451422052, 1944995967, 515157009,
};
