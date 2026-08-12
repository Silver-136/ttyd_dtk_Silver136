#include "driver/effdrv.h"

typedef struct EffWork {
    s32 count;
    EffEntry* entries;
    void* unk8;
    s32 unkC;
    void* unk10;
    s32 unk14;
} EffWork;

typedef struct EffGp {
    u8 pad0[0x14];
    s32 unk14;
} EffGp;

typedef struct EffSetEntry {
    s16 id;
    s16 pad;
    const char* name;
} EffSetEntry;

EffWork work;
EffWork* wp = &work;
extern EffGp* gp;

const char str_kemuri_802c1c80[] = "kemuri";
const char str_confetti_802c1c88[] = "confetti";
const char str_fukidashi_802c1c94[] = "fukidashi";
const char str_butterfly_802c1ca0[] = "butterfly";
const char str_damage_star_802c1cac[] = "damage_star";
const char str_small_star_802c1cb8[] = "small_star";
const char str_mario_balloon_802c1cc4[] = "mario_balloon";
const char str_stardust_802c1cd4[] = "stardust";
const char str_recovery_802c1ce0[] = "recovery";
const char str_miss_star_802c1cec[] = "miss_star";
const char str_breath_fire_802c1cf8[] = "breath_fire";
const char str_confusion_802c1d04[] = "confusion";
const char str_updown_802c1d10[] = "updown";
const char str_charge_802c1d18[] = "charge";
const char str_toge_flush_802c1d20[] = "toge_flush";
const char str_ripple_802c1d2c[] = "ripple";
const char str_coin_fukidashi_802c1d34[] = "coin_fukidashi";
const char str_star_point_802c1d44[] = "star_point";
const char str_puni_balloon_802c1d50[] = "puni_balloon";
const char str_p_fukidashi_802c1d60[] = "p_fukidashi";
const char str_kemuri_test_802c1d6c[] = "kemuri_test";
const char str_starstone_802c1d78[] = "starstone";
const char str_itemget_802c1d84[] = "itemget";
const char str_status_802c1d8c[] = "status";
const char str_pointget_802c1d94[] = "pointget";
const char str_levelup_802c1da0[] = "levelup";
const char str_stageclear_802c1da8[] = "stageclear";
const char str_fpdamage_802c1db4[] = "fpdamage";
const char str_mobj_broken_802c1dc0[] = "mobj_broken";
const char str_mahojin_802c1dcc[] = "mahojin";
const char str_mizutama_802c1dd4[] = "mizutama";
const char str_minigame_802c1de0[] = "minigame";
const char str_splash_802c1dec[] = "splash";
const char str_treasure_map_802c1df4[] = "treasure_map";
const char str_funemizu_802c1e04[] = "funemizu";
const char str_teresa_802c1e10[] = "teresa";
const char str_batten_802c1e18[] = "batten";
const char str_naniga_802c1e20[] = "naniga";
const char str_sandars_802c1e28[] = "sandars";
const char str_boomerang_802c1e30[] = "boomerang";
const char str_irekae_802c1e3c[] = "irekae";
const char str_rankup_802c1e44[] = "rankup";
const char str_scanning_802c1e4c[] = "scanning";
const char str_particle_802c1e58[] = "particle";
const char str_mahorn_802c1e64[] = "mahorn";
const char str_spirit_802c1e6c[] = "spirit";
const char str_indirect_802c1e74[] = "indirect";
const char str_syuryou_802c1e80[] = "syuryou";
const char str_uranoko_802c1e88[] = "uranoko";
const char str_hibashira_802c1e90[] = "hibashira";
const char str_number_802c1e9c[] = "number";
const char str_machingegun_802c1ea4[] = "machingegun";
const char str_las_mon_802c1eb0[] = "las_mon";
const char str_energy_802c1eb8[] = "energy";
const char str_biribirikinoko_802c1ec0[] = "biribirikinoko";
const char str_nokotarou_802c1ed0[] = "nokotarou";
const char str_queen2_802c1edc[] = "queen2";
const char str_gonbaba_breath_802c1ee4[] = "gonbaba_breath";
const char str_majinai_802c1ef4[] = "majinai";
const char str_mahorn2_802c1efc[] = "mahorn2";
const char str_ultra_hammer_802c1f04[] = "ultra_hammer";

const char str_hit_804200e8[] = "hit";
const char str_mugi_804200ec[] = "mugi";
const char str_nice_804200f4[] = "nice";
const char str_torch_804200fc[] = "torch";
const char str_spark_80420104[] = "spark";
const char str_ice_8042010c[] = "ice";
const char str_fire_80420110[] = "fire";
const char str_bomb_80420118[] = "bomb";
const char str_fall_80420120[] = "fall";
const char str_break_80420128[] = "break";
const char str_nozle_80420130[] = "nozle";
const char str_kiss_80420138[] = "kiss";
const char str_mist_80420140[] = "mist";
const char str_sheep_80420148[] = "sheep";
const char str_toiki_80420150[] = "toiki";
const char str_mist2_80420158[] = "mist2";
const char str_ibuki_80420160[] = "ibuki";
const char str_laser_80420168[] = "laser";
const char str_sleep_80420170[] = "sleep";
const char str_queen_80420178[] = "queen";
const char str_jp_80420180[] = "jp";
const char str_us_80420184[] = "us";
const char str_ge_80420188[] = "ge";
const char str_fr_8042018c[] = "fr";
const char str_sp_80420190[] = "sp";
const char str_it_80420194[] = "it";
const char str_du_80420198[] = "du";

const char* prefix_tbl[] = {
    str_jp_80420180,
    str_us_80420184,
    str_ge_80420188,
    str_fr_8042018c,
    str_sp_80420190,
    str_it_80420194,
    str_du_80420198,
};

EffSetEntry eff_set_table[] = {
    { 0, 0, str_kemuri_802c1c80 },
    { 1, 0, str_confetti_802c1c88 },
    { 2, 0, str_fukidashi_802c1c94 },
    { 3, 0, str_butterfly_802c1ca0 },
    { 4, 0, str_hit_804200e8 },
    { 5, 0, str_damage_star_802c1cac },
    { 6, 0, str_small_star_802c1cb8 },
    { 7, 0, str_mario_balloon_802c1cc4 },
    { 8, 0, str_mugi_804200ec },
    { 9, 0, str_stardust_802c1cd4 },
    { 10, 0, str_recovery_802c1ce0 },
    { 11, 0, str_miss_star_802c1cec },
    { 12, 0, str_nice_804200f4 },
    { 13, 0, str_breath_fire_802c1cf8 },
    { 14, 0, str_torch_804200fc },
    { 15, 0, str_confusion_802c1d04 },
    { 16, 0, str_spark_80420104 },
    { 17, 0, str_updown_802c1d10 },
    { 18, 0, str_charge_802c1d18 },
    { 19, 0, str_toge_flush_802c1d20 },
    { 20, 0, str_ice_8042010c },
    { 21, 0, str_fire_80420110 },
    { 22, 0, str_bomb_80420118 },
    { 23, 0, str_ripple_802c1d2c },
    { 24, 0, str_coin_fukidashi_802c1d34 },
    { 25, 0, str_star_point_802c1d44 },
    { 26, 0, str_puni_balloon_802c1d50 },
    { 27, 0, str_p_fukidashi_802c1d60 },
    { 28, 0, str_kemuri_test_802c1d6c },
    { 29, 0, str_starstone_802c1d78 },
    { 30, 0, str_itemget_802c1d84 },
    { 31, 0, str_status_802c1d8c },
    { 32, 0, str_pointget_802c1d94 },
    { 33, 0, str_levelup_802c1da0 },
    { 34, 0, str_stageclear_802c1da8 },
    { 35, 0, str_fall_80420120 },
    { 36, 0, str_fpdamage_802c1db4 },
    { 37, 0, str_break_80420128 },
    { 38, 0, str_mobj_broken_802c1dc0 },
    { 39, 0, str_mahojin_802c1dcc },
    { 40, 0, str_nozle_80420130 },
    { 41, 0, str_mizutama_802c1dd4 },
    { 42, 0, str_minigame_802c1de0 },
    { 43, 0, str_splash_802c1dec },
    { 44, 0, str_treasure_map_802c1df4 },
    { 45, 0, str_kiss_80420138 },
    { 46, 0, str_funemizu_802c1e04 },
    { 47, 0, str_mist_80420140 },
    { 48, 0, str_teresa_802c1e10 },
    { 49, 0, str_batten_802c1e18 },
    { 50, 0, str_sheep_80420148 },
    { 51, 0, str_naniga_802c1e20 },
    { 52, 0, str_sandars_802c1e28 },
    { 53, 0, str_boomerang_802c1e30 },
    { 54, 0, str_irekae_802c1e3c },
    { 55, 0, str_rankup_802c1e44 },
    { 56, 0, str_scanning_802c1e4c },
    { 57, 0, str_toiki_80420150 },
    { 58, 0, str_mist2_80420158 },
    { 59, 0, str_particle_802c1e58 },
    { 60, 0, str_ibuki_80420160 },
    { 61, 0, str_mahorn_802c1e64 },
    { 62, 0, str_spirit_802c1e6c },
    { 63, 0, str_indirect_802c1e74 },
    { 64, 0, str_syuryou_802c1e80 },
    { 65, 0, str_uranoko_802c1e88 },
    { 66, 0, str_hibashira_802c1e90 },
    { 67, 0, str_number_802c1e9c },
    { 68, 0, str_laser_80420168 },
    { 69, 0, str_machingegun_802c1ea4 },
    { 70, 0, str_las_mon_802c1eb0 },
    { 71, 0, str_sleep_80420170 },
    { 72, 0, str_energy_802c1eb8 },
    { 73, 0, str_biribirikinoko_802c1ec0 },
    { 74, 0, str_nokotarou_802c1ed0 },
    { 75, 0, str_queen_80420178 },
    { 76, 0, str_queen2_802c1edc },
    { 77, 0, str_gonbaba_breath_802c1ee4 },
    { 78, 0, str_majinai_802c1ef4 },
    { 79, 0, str_mahorn2_802c1efc },
    { 80, 0, str_ultra_hammer_802c1f04 },
};

EffSetEntry gap_05_80309A30_data = { -1, 0, 0 };

void __memFree(s32 heap, void* ptr);
void UnpackTexPalette(void* tpl);
void DVDMgrClose(void* entry);
s32 strcmp(const char* s1, const char* s2);
char* strcpy(char* dst, const char* src);

void _callback_tpl(void* unk, void* dvdEntry) {
    void* closeEntry = *(void**)((s32)dvdEntry + 0x2C);

    UnpackTexPalette(wp->unk8);
    DVDMgrClose(closeEntry);
    wp->unkC = 1;
}

void effInit(void) {
    extern EffWork* wp;
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void effInit64(void);
    wp->count = 0x200;
    wp->entries = __memAlloc(0, wp->count * 0x28);
    memset(wp->entries, 0, wp->count * 0x28);
    *(s32*)((s32)wp + 0x10) = 0;
    *(s32*)((s32)wp + 0x14) = -1;
    wp->unk8 = 0;
    wp->unkC = 0;
    effInit64();
}

void effTexSetup(void) {
    extern void* arcOpen(const char*, s32, s32);
    extern void* getMarioStDvdRoot(void);
    extern s32 sprintf(char*, const char*, ...);
    extern void* DVDMgrOpen(char*, s32, s32);
    extern s32 DVDMgrGetLength(void*);
    extern void* __memAlloc(s32, u32);
    extern void DVDMgrReadAsync(void*, void*, u32, s32, void (*)(void*, void*));
    extern void _callback_tpl(void*, void*);
    extern void UnpackTexPalette(void*);
    extern void effTexSetupN64(void);
    char path[0x80];
    char* lang;
    void* dvd;
    u32 len;

    wp->unk8 = 0;
    wp->unkC = 0;
    wp->unk8 = arcOpen((const char*)0x802C1F38, 0, 0);
    if (wp->unk8 == 0) {
        lang = (char*)0x80420184;
        if (*(u32*)((s32)gp + 0x16C) == 0) {
            lang = (char*)0x80420180;
        }
        sprintf(path, (const char*)0x802C1F44, getMarioStDvdRoot(), lang);
        dvd = DVDMgrOpen(path, 2, 0);
        len = (DVDMgrGetLength(dvd) + 0x1F) & ~0x1F;
        wp->unk8 = __memAlloc(0, len);
        *(void**)((s32)dvd + 0x6C) = dvd;
        DVDMgrReadAsync(dvd, wp->unk8, len, 0, _callback_tpl);
    } else {
        UnpackTexPalette(wp->unk8);
        wp->unkC = 1;
    }
    effTexSetupN64();
}

void effGetTexObj(s32 id, void* texObj) {
    extern void GXInitTexObj(void*, void*, s32, s32, s32, s32, s32, s32);
    extern void TEXGetGXTexObjFromPalette(void*, s32, void*);
    static u8 dummy[32] = {0};
    static u8 dummy2[8] = {0};

    if (id < 0x76) {
        if (wp->unkC == 0) {
            GXInitTexObj(texObj, dummy, 1, 1, 0, 0, 0, 0);
        } else {
            TEXGetGXTexObjFromPalette(wp->unk8, id, texObj);
        }
    } else {
        void* file = *(void**)((s32)wp + 0x10);
        if (file == 0) {
            GXInitTexObj(texObj, dummy2, 1, 1, 0, 0, 0, 0);
        } else {
            TEXGetGXTexObjFromPalette(*(void**)*(void**)((s32)file + 0xA0), id - 0x76, texObj);
        }
    }
}
void effAutoRelease(s32 value) {
    extern EffWork* wp;
    extern void __memFree(s32 heap, void* ptr);
    s32 i = 0;
    s32 zero = 0;
    s32 count = wp->count;
    EffEntry* entry = wp->entries;
    while (i < count) {
        if ((entry->flags & 1) && entry->unk4 == value) {
            if (entry != 0 && entry->flags != 0) {
                __memFree(3, entry->data);
                entry->flags = zero;
            }
        }
        i++;
        entry++;
    }
}

EffEntry* effEntry(void) {
    EffEntry* entry = wp->entries;
    s32 count = wp->count;
    s32 i;

    for (i = 0; i < count; i++) {
        if (entry->flags == 0) {
            break;
        }
        entry++;
    }

    entry->flags = 1;
    entry->unk4 = (u32)(-gp->unk14 | gp->unk14) >> 31;
    entry->unk14 = 0;
    entry->unk8 = 0;
    entry->data = 0;
    entry->unk10 = 0;
    entry->name[0] = 0;
    return entry;
}

void effSetName(EffEntry* entry, char* name) {
    s32 i = 0;
    s32 count = wp->count;
    EffEntry* scan = wp->entries;

    while (i < count) {
        if (scan->flags != 0 && strcmp(scan->name, name) == 0) {
            break;
        }
        i++;
        scan++;
    }

    strcpy(entry->name, name);
}
void effMain(void) {
    typedef void (*EffCallback)(void*);
    extern void fileFree(void*);
    extern void* getMarioStDvdRoot(void);
    extern void* fileAsyncf(s32, s32, const char*, ...);
    extern void* fileAllocf(s32, const char*, ...);
    s32 i;
    s32 count;
    void* entry;
    const char* prefix;

    count = wp->count;
    entry = wp->entries;
    for (i = 0; i < count; i++, entry = (void*)((s32)entry + 0x28)) {
        if (gp->unk14 != 0) {
            if (*(s32*)((s32)entry + 4) != 1) {
                continue;
            }
        } else if (*(s32*)((s32)entry + 4) != 0) {
            continue;
        }
        if ((*(u32*)entry & 1) && *(EffCallback*)((s32)entry + 0x10) != 0) {
            (*(EffCallback*)((s32)entry + 0x10))(entry);
        }
    }

    if (*(s32*)((s32)wp + 0x14) != *(s32*)((s32)gp + 0x16C)) {
        if (*(void**)((s32)wp + 0x10) != 0) {
            fileFree(*(void**)((s32)wp + 0x10));
            *(void**)((s32)wp + 0x10) = 0;
        }
        prefix = prefix_tbl[*(s32*)((s32)gp + 0x16C)];
        if (fileAsyncf(4, 0, (const char*)0x802C1F20, getMarioStDvdRoot(), prefix, prefix) != 0) {
            prefix = prefix_tbl[*(s32*)((s32)gp + 0x16C)];
            *(void**)((s32)wp + 0x10) = fileAllocf(4, (const char*)0x802C1F20, getMarioStDvdRoot(), prefix, prefix);
            *(s32*)((s32)wp + 0x14) = *(s32*)((s32)gp + 0x16C);
        }
    }
}

void effDelete(EffEntry* entry) {
    if (entry != 0 && entry->flags != 0) {
        __memFree(3, entry->data);
        entry->flags = 0;
    }
}

void effSoftDelete(EffEntry* entry) {
    u32 flags;

    if (entry != 0) {
        flags = entry->flags;
        if (flags != 0) {
            if (flags & 2) {
                entry->flags = flags | 4;
            } else {
                if (entry != 0 && flags != 0) {
                    __memFree(3, entry->data);
                    entry->flags = 0;
                }
            }
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effNameToPtr(char* name) {
    extern EffWork* wp;
    extern s32 strcmp(const char* s1, const char* s2);
    s32 i = 0;
    s32 count = wp->count;
    EffEntry* entry = wp->entries;
    while (i < count) {
        if (entry->flags != 0 && strcmp(entry->name, name) == 0) {
            break;
        }
        i++;
        entry++;
    }
    if (i >= count) {
        return 0;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effGetSet(char* name) {
    extern s32 strcmp(const char* s1, const char* s2);
    EffSetEntry* entry = eff_set_table;
    s32 i = 0;
    while (entry->id != -1) {
        if (strcmp(entry->name, name) == 0) {
            return &eff_set_table[i];
        }
        entry++;
        i++;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



u8 effCalcMayaAnimMatrix(int param_1, int* param_2, s32 param_3, float* param_4) {
    extern void PSMTXTrans(void*, double, double, double);
    extern void PSMTXRotRad(void*, double, char);
    extern void PSMTXScale(void*, float, float, float);
    extern void PSMTXConcat(void*, void*, void*);
    extern f32 float_0_804201a0;
    extern f32 dat_804201a4;
    extern f32 float_neg2_804201ac;
    extern f32 float_3_804201a8;
    extern f32 float_1_804201b0;
    extern f32 float_2_804201b4;
    f32 m0[3][4];
    f32 m1[3][4];
    f32 m2[3][4];
    f32 m3[3][4];
    f32 m4[3][4];
    f32 m5[3][4];
    f32 m6[3][4];
    f32 m7[3][4];
    f32 m8[3][4];
    f32 m9[3][4];
    f32 m10[3][4];
    f32 m11[3][4];
    f32 m12[3][4];
    f32 v[22];
    s32 i;
    s32 j;
    s32 last;
    s16* prev;
    s16* next;
    f32 time;
    f32 frameTime;
    f32 span;
    f32 t;
    f32 t2;
    f32 t3;
    f32 scale;

    time = *(f32*)(param_1 + 4);
    last = *param_2 - 1;
    for (i = 1; i < *param_2; i++) {
        next = (s16*)(param_2[1] + i * 0xB2);
        prev = (s16*)(param_2[1] + (i - 1) * 0xB2);
        frameTime = (f32)*next;
        if (time > frameTime && i != last) {
            continue;
        }
        if (time > frameTime) {
            time = frameTime;
        }
        for (j = 0; j < 22; j++) {
            s16* p = prev + j * 4;
            s16* n = next + j * 4;
            if (*(s8*)((s32)p + 9) != 0) {
                v[j] = (f32)p[1] * (1.0f / 250.0f);
            } else {
                span = (f32)*next - (f32)*prev;
                if (span == float_0_804201a0) {
                    span = dat_804201a4;
                }
                t = (time - (f32)*prev) / span;
                t2 = t * t;
                t3 = t2 * t;
                scale = 1.0f / 250.0f;
                v[j] = ((f32)p[1] * scale) * (float_1_804201b0 + (float_2_804201b4 * t3 - float_3_804201a8 * t2)) +
                       ((f32)n[1] * scale) * (float_neg2_804201ac * t3 + float_3_804201a8 * t2) +
                       ((f32)p[3] * scale) * (span * (t + -(float_2_804201b4 * t2 - t3))) +
                       ((f32)n[2] * scale) * (span * (t3 - t2));
            }
        }
        PSMTXTrans(m12, (double)v[0], (double)v[1], (double)v[2]);
        PSMTXRotRad(m0, (double)v[3], 'x');
        PSMTXRotRad(m1, (double)v[4], 'y');
        PSMTXRotRad(m2, (double)v[5], 'z');
        PSMTXScale(m11, v[6], v[7], v[8]);
        PSMTXTrans(m5, (double)v[9], (double)v[10], (double)v[11]);
        PSMTXTrans(m6, -(double)v[9], -(double)v[10], -(double)v[11]);
        PSMTXTrans(m7, (double)v[12], (double)v[13], (double)v[14]);
        PSMTXTrans(m3, (double)v[15], (double)v[16], (double)v[17]);
        PSMTXTrans(m4, -(double)v[15], -(double)v[16], -(double)v[17]);
        PSMTXTrans(m8, (double)v[18], (double)v[19], (double)v[20]);
        PSMTXConcat(m8, m3, m9);
        PSMTXConcat(m9, m11, m9);
        PSMTXConcat(m9, m4, m11);
        PSMTXConcat(m7, m5, m9);
        PSMTXConcat(m9, m2, m9);
        PSMTXConcat(m9, m1, m9);
        PSMTXConcat(m9, m0, m9);
        PSMTXConcat(m9, m6, m10);
        PSMTXConcat(m12, m10, m9);
        PSMTXConcat(m9, m11, (void*)param_3);
        if (v[21] > float_1_804201b0) {
            *param_4 = float_1_804201b0;
        } else if (v[21] < float_0_804201a0) {
            *param_4 = float_0_804201a0;
        } else {
            *param_4 = v[21];
        }
        return 0;
    }
    return 0;
}

int* effMayaAnimAlloc(void* effData) {
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* ptr, s32 value, u32 size);
    u32 size;
    void* anim;

    size = *(s32*)(*(s32*)((s32)effData + 8)) * 0x34 + 0x18;
    anim = __memAlloc(3, size);
    memset(anim, 0, size);
    *(void**)((s32)anim + 0x10) = effData;
    return anim;
}

void effDeleteMayaAnim(void* ptr) {
    __memFree(3, ptr);
}

void effPlayMayaAnim(void* anim) {
    extern EffGp* gp;
    extern const f32 float_0_804201a0;
    u64* t;
    u32 bus;
    u32 ticks;
    if (*(s32*)((s32)gp + 0x14) != 0) {
        t = (u64*)((s32)gp + 0x38);
    } else {
        t = (u64*)((s32)gp + 0x40);
    }
    bus = *(u32*)0x800000F8 >> 2;
    ticks = ((u64)bus * 0x10624DD3U) >> 38;
    *(s64*)((s32)anim + 8) = *t / ticks;
    *(f32*)((s32)anim + 4) = float_0_804201a0;
    *(u16*)anim |= 1;
}

u8 effCalcMayaAnim(void* anim) {
    extern s64 __div2i(s64 dividend, s64 divisor);
    extern u8 effCalcMayaAnimMatrix(s32 anim, s32* data, s32 node, f32* matrix);
    extern f32 float_1000_8042019c;
    u64 now;
    u32 ticks;
    s32 data;
    s32 i;
    u8* node;

    if ((*(u16*)anim & 1) != 0 && (*(u16*)anim & 2) == 0) {
        ticks = (*(u32*)0x800000F8 >> 2) / 1000;
        if (*(s32*)((s32)gp + 0x14) == 0) {
            now = (u64)__div2i(*(s64*)((s32)gp + 0x40), (s64)(s32)ticks);
        } else {
            now = (u64)__div2i(*(s64*)((s32)gp + 0x38), (s64)(s32)ticks);
        }

        *(f32*)((s32)anim + 4) =
            ((f32)*(s32*)((s32)gp + 0x1C) * (f32)(now - *(u64*)((s32)anim + 8))) /
            float_1000_8042019c;
        data = *(s32*)((s32)anim + 0x10);
        if (*(f32*)(data + 4) <= *(f32*)((s32)anim + 4)) {
            *(f32*)((s32)anim + 4) = *(f32*)(data + 4);
            *(u16*)anim |= 2;
        }

        node = (u8*)anim + 0x14;
        for (i = 0; i < **(s32**)(data + 8); i++, node += 0x34) {
            effCalcMayaAnimMatrix((s32)anim,
                                  (s32*)(*(s32*)(*(s32*)(data + 8) + 4) + i * 0x10),
                                  (s32)node,
                                  (f32*)(node + 0x30));
        }
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void effDrawMayaPoly(void* data) {
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(u8 attr, u8 type);
    extern void GXSetVtxAttrFmt(u32 vtxfmt, u32 attr, u32 compCnt, u32 compType, u32 frac);
    extern void GXSetCullMode(u32 mode);
    extern void GXBegin(u8 primitive, u8 vtxfmt, u16 nverts);
    volatile s16* fifo;
    s32 offset;
    s32 i;
    s16* poly;
    s32 count;
    s32 vertices;
    s16 a, b, c, d, e;

    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 9);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 9);
    GXSetCullMode(0);

    i = 0;
    offset = 0;
    fifo = (volatile s16*)0xCC008000;

    while (i < *(s32*)((s32)data + 8)) {
        poly = *(s16**)(*(s32*)((s32)data + 0xC) + offset);
        count = (u16)*poly;
        GXBegin(0xA0, 0, count);
        poly++;

        vertices = count;
        while (vertices > 0) {
            a = poly[0];
            b = poly[1];
            c = poly[2];
            fifo[0] = a;
            fifo[0] = b;
            fifo[0] = c;

            d = poly[3];
            e = poly[4];
            fifo[0] = d;
            fifo[0] = e;

            poly += 5;
            vertices--;
        }

        offset += 4;
        i++;
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
