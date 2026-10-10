#include "mapdata.h"

extern void* world_data[];
extern int strcmp(const char*, const char*);

#pragma opt_common_subs off
char** mapDataPtr(char* name) {
    void** outer;
    void** outerArea;
    void** scan;
    void** base;
    void** lookupBase;
    void** countBase;
    void** outerCountBase;
    void* area;
    void* map;
    char* areaName;
    s32 outerIndex;
    s32 mapIndex;
    s32 mapOffset;
    s32 lookupCount;
    s32 lookupIndex;
    s32 mapCount;
    s32 outerCount;

    outer = world_data;
    base = outer;
    lookupBase = outer;
    countBase = outer;
    outerCountBase = outer;
    outerIndex = 0;
    goto outer_check;

outer_body:
    outerArea = outer + 2;
    mapIndex = 0;
    mapOffset = 0;
    goto map_check;

map_body:
    map = *(void**)((s32)*outerArea + 0x10 + mapOffset);
    if (strcmp(*(char**)map, name) == 0) {
        return map;
    }
    mapOffset += 4;
    mapIndex++;

map_check:
    areaName = *(char**)*outerArea;
    scan = countBase;
    lookupCount = 0;
    while (scan[2] != 0) {
        scan++;
        lookupCount++;
    }
    scan = lookupBase;
    lookupIndex = 0;
    area = 0;
    while (lookupIndex < lookupCount) {
        if (strcmp(*(char**)scan[2], areaName) == 0) {
            area = base[lookupIndex + 2];
            break;
        }
        scan++;
        lookupIndex++;
    }
    mapCount = 0;
    while (*(void**)((s32)area + 0x10) != 0) {
        area = (void*)((s32)area + 4);
        mapCount++;
    }
    if (mapIndex < mapCount) {
        goto map_body;
    }
    outer++;
    outerIndex++;

outer_check:
    scan = outerCountBase;
    outerCount = 0;
    while (scan[2] != 0) {
        scan++;
        outerCount++;
    }
    if (outerIndex < outerCount) {
        goto outer_body;
    }
    return 0;
}
#pragma opt_common_subs reset

void* areaDataPtr(char* name) {
    char* key;
    void** base;
    void** scan;
    int count;
    int i;

    key = name;
    base = &world_data[2];
    scan = base;
    count = 0;
    while (*scan != 0) {
        scan++;
        count++;
    }

    scan = base;
    i = 0;
    while (i < count) {
        if (strcmp(*(const char**)*scan, key) == 0) {
            return base[i];
        }
        scan++;
        i++;
    }

    return 0;
}


#pragma opt_common_subs off
u8 relSetEvtAddr(char* name, void* evt) {
    typedef struct LocalMap { char* name; void* event; } LocalMap;
    typedef struct LocalArea { char* name; u8 pad[12]; LocalMap* maps[1]; } LocalArea;
    void** outer;
    void** outerArea;
    void** scan;
    void** base;
    void** lookupBase;
    void** countBase;
    void** outerCountBase;
    LocalArea* area;
    LocalMap* map;
    char* areaName;
    s32 outerIndex;
    s32 mapIndex;
    s32 mapOffset;
    s32 lookupCount;
    s32 lookupIndex;
    s32 mapCount;
    s32 outerCount;

    outer = world_data;
    base = outer;
    lookupBase = outer;
    countBase = outer;
    outerCountBase = outer;
    outerIndex = 0;
    goto outer_check;

outer_body:
    outerArea = outer + 2;
    mapIndex = 0;
    mapOffset = 0;
    goto map_check;

map_body:
    map = ((LocalArea*)*outerArea)->maps[mapIndex];
    if (strcmp(map->name, name) == 0) {
        goto done;
    }
    mapOffset += 4;
    mapIndex++;

map_check:
    areaName = *(char**)*outerArea;
    scan = countBase;
    lookupCount = 0;
    while (scan[2] != 0) {
        scan++;
        lookupCount++;
    }
    scan = lookupBase;
    lookupIndex = 0;
    while (lookupIndex < lookupCount) {
        if (strcmp(*(char**)scan[2], areaName) == 0) {
            area = base[lookupIndex + 2];
            goto area_found;
        }
        scan++;
        lookupIndex++;
    }
    area = 0;
area_found:
    mapCount = 0;
    while (area->maps[0] != 0) {
        area = (LocalArea*)((s32)area + 4);
        mapCount++;
    }
    if (mapIndex < mapCount) {
        goto map_body;
    }
    outer++;
    outerIndex++;

outer_check:
    scan = outerCountBase;
    outerCount = 0;
    while (scan[2] != 0) {
        scan++;
        outerCount++;
    }
    if (outerIndex < outerCount) {
        goto outer_body;
    }
    map = 0;

done:
    if (map != 0) {
        map->event = evt;
    }
}
#pragma opt_common_subs reset

void relSetBtlAddr(char* name, int btl1, int btl2) {
    void** base;
    void** scan;
    void* data;
    int count;
    int i;

    base = world_data;
    scan = base;
    count = 0;
    while (scan[2] != 0) {
        scan++;
        count++;
    }

    scan = base;
    i = 0;
    while (i < count) {
        if (strcmp(*(const char**)scan[2], name) == 0) {
            data = base[i + 2];
            goto done;
        }
        scan++;
        i++;
    }
    data = 0;

done:
    if (data != 0) {
        *(int*)((int)data + 8) = btl1;
        *(int*)((int)data + 0xC) = btl2;
    }
}

/* Target-backed room and area catalog. world_data is intentionally deferred. */
void* tst_bt00[2] = {"tst_bt00", 0};
void* tst_00[2] = {"tst_00", 0};
void* tst_01[2] = {"tst_01", 0};
void* tst_02[2] = {"tst_02", 0};
void* tst_moj_00[2] = {"tst_moj_00", 0};
void* tst_moj_01[2] = {"tst_moj_01", 0};
void* tst_moj_02[2] = {"tst_moj_02", 0};
void* tst_moj_03[2] = {"tst_moj_03", 0};
void* tst_off_01[2] = {"tst_off_01", 0};
void* tst_npc[2] = {"tst_npc", 0};
void* tst_nakayama[2] = {"tst_nakayama", 0};
void* tst_item_00[2] = {"tst_item_00", 0};
void* bti_01[2] = {"bti_01", 0};
void* bti_02[2] = {"bti_02", 0};
void* bti_03[2] = {"bti_03", 0};
void* bti_04[2] = {"bti_04", 0};
void* stg_01_0[2] = {"stg_01_0", 0};
void* stg_01_1[2] = {"stg_01_1", 0};
void* stg_01_2[2] = {"stg_01_2", 0};
void* stg_01_3[2] = {"stg_01_3", 0};
void* stg_01_4[2] = {"stg_01_4", 0};
void* aaa_00[2] = {"aaa_00", 0};
void* gor_00[2] = {"gor_00", 0};
void* gor_01[2] = {"gor_01", 0};
void* gor_02[2] = {"gor_02", 0};
void* gor_03[2] = {"gor_03", 0};
void* gor_04[2] = {"gor_04", 0};
void* gor_10[2] = {"gor_10", 0};
void* gor_11[2] = {"gor_11", 0};
void* gor_12[2] = {"gor_12", 0};
void* tik_00[2] = {"tik_00", 0};
void* tik_01[2] = {"tik_01", 0};
void* tik_02[2] = {"tik_02", 0};
void* tik_03[2] = {"tik_03", 0};
void* tik_04[2] = {"tik_04", 0};
void* tik_05[2] = {"tik_05", 0};
void* tik_06[2] = {"tik_06", 0};
void* tik_07[2] = {"tik_07", 0};
void* tik_08[2] = {"tik_08", 0};
void* tik_11[2] = {"tik_11", 0};
void* tik_12[2] = {"tik_12", 0};
void* tik_13[2] = {"tik_13", 0};
void* tik_15[2] = {"tik_15", 0};
void* tik_16[2] = {"tik_16", 0};
void* tik_17[2] = {"tik_17", 0};
void* tik_18[2] = {"tik_18", 0};
void* tik_19[2] = {"tik_19", 0};
void* tik_20[2] = {"tik_20", 0};
void* tik_21[2] = {"tik_21", 0};
void* bom_00[2] = {"bom_00", 0};
void* bom_01[2] = {"bom_01", 0};
void* bom_02[2] = {"bom_02", 0};
void* bom_03[2] = {"bom_03", 0};
void* bom_04[2] = {"bom_04", 0};
void* moo_00[2] = {"moo_00", 0};
void* moo_01[2] = {"moo_01", 0};
void* moo_02[2] = {"moo_02", 0};
void* moo_03[2] = {"moo_03", 0};
void* moo_04[2] = {"moo_04", 0};
void* moo_05[2] = {"moo_05", 0};
void* moo_06[2] = {"moo_06", 0};
void* moo_07[2] = {"moo_07", 0};
void* aji_00[2] = {"aji_00", 0};
void* aji_01[2] = {"aji_01", 0};
void* aji_02[2] = {"aji_02", 0};
void* aji_03[2] = {"aji_03", 0};
void* aji_04[2] = {"aji_04", 0};
void* aji_05[2] = {"aji_05", 0};
void* aji_06[2] = {"aji_06", 0};
void* aji_07[2] = {"aji_07", 0};
void* aji_08[2] = {"aji_08", 0};
void* aji_09[2] = {"aji_09", 0};
void* aji_10[2] = {"aji_10", 0};
void* aji_11[2] = {"aji_11", 0};
void* aji_12[2] = {"aji_12", 0};
void* aji_13[2] = {"aji_13", 0};
void* aji_14[2] = {"aji_14", 0};
void* aji_15[2] = {"aji_15", 0};
void* aji_16[2] = {"aji_16", 0};
void* aji_17[2] = {"aji_17", 0};
void* aji_18[2] = {"aji_18", 0};
void* aji_19[2] = {"aji_19", 0};
void* gon_00[2] = {"gon_00", 0};
void* gon_01[2] = {"gon_01", 0};
void* gon_02[2] = {"gon_02", 0};
void* gon_03[2] = {"gon_03", 0};
void* gon_04[2] = {"gon_04", 0};
void* gon_05[2] = {"gon_05", 0};
void* gon_06[2] = {"gon_06", 0};
void* gon_07[2] = {"gon_07", 0};
void* gon_08[2] = {"gon_08", 0};
void* gon_09[2] = {"gon_09", 0};
void* gon_10[2] = {"gon_10", 0};
void* gon_11[2] = {"gon_11", 0};
void* gon_12[2] = {"gon_12", 0};
void* gon_13[2] = {"gon_13", 0};
void* gon_bt00[2] = {"gon_bt00", 0};
void* nok_00[2] = {"nok_00", 0};
void* nok_01[2] = {"nok_01", 0};
void* hei_00[2] = {"hei_00", 0};
void* hei_01[2] = {"hei_01", 0};
void* hei_02[2] = {"hei_02", 0};
void* hei_03[2] = {"hei_03", 0};
void* hei_04[2] = {"hei_04", 0};
void* hei_05[2] = {"hei_05", 0};
void* hei_06[2] = {"hei_06", 0};
void* hei_07[2] = {"hei_07", 0};
void* hei_08[2] = {"hei_08", 0};
void* hei_09[2] = {"hei_09", 0};
void* hei_10[2] = {"hei_10", 0};
void* hei_11[2] = {"hei_11", 0};
void* hei_12[2] = {"hei_12", 0};
void* hei_13[2] = {"hei_13", 0};
void* hei_bt00[2] = {"hei_bt00", 0};
void* muj_00[2] = {"muj_00", 0};
void* muj_01[2] = {"muj_01", 0};
void* muj_02[2] = {"muj_02", 0};
void* muj_03[2] = {"muj_03", 0};
void* muj_04[2] = {"muj_04", 0};
void* muj_05[2] = {"muj_05", 0};
void* muj_10[2] = {"muj_10", 0};
void* muj_11[2] = {"muj_11", 0};
void* muj_12[2] = {"muj_12", 0};
void* muj_20[2] = {"muj_20", 0};
void* muj_21[2] = {"muj_21", 0};
void* dou_00[2] = {"dou_00", 0};
void* dou_01[2] = {"dou_01", 0};
void* dou_02[2] = {"dou_02", 0};
void* dou_03[2] = {"dou_03", 0};
void* dou_04[2] = {"dou_04", 0};
void* dou_05[2] = {"dou_05", 0};
void* dou_06[2] = {"dou_06", 0};
void* dou_07[2] = {"dou_07", 0};
void* dou_08[2] = {"dou_08", 0};
void* dou_09[2] = {"dou_09", 0};
void* dou_10[2] = {"dou_10", 0};
void* dou_11[2] = {"dou_11", 0};
void* dou_12[2] = {"dou_12", 0};
void* dou_13[2] = {"dou_13", 0};
void* rsh_00_a[2] = {"rsh_00_a", 0};
void* rsh_00_b[2] = {"rsh_00_b", 0};
void* rsh_00_c[2] = {"rsh_00_c", 0};
void* rsh_01_a[2] = {"rsh_01_a", 0};
void* rsh_01_b[2] = {"rsh_01_b", 0};
void* rsh_01_c[2] = {"rsh_01_c", 0};
void* rsh_02_a[2] = {"rsh_02_a", 0};
void* rsh_02_b[2] = {"rsh_02_b", 0};
void* rsh_02_c[2] = {"rsh_02_c", 0};
void* rsh_03_a[2] = {"rsh_03_a", 0};
void* rsh_03_b[2] = {"rsh_03_b", 0};
void* rsh_03_c[2] = {"rsh_03_c", 0};
void* rsh_04_a[2] = {"rsh_04_a", 0};
void* rsh_04_b[2] = {"rsh_04_b", 0};
void* rsh_04_c[2] = {"rsh_04_c", 0};
void* rsh_05_a[2] = {"rsh_05_a", 0};
void* rsh_06_a[2] = {"rsh_06_a", 0};
void* rsh_07_a[2] = {"rsh_07_a", 0};
void* rsh_07_b[2] = {"rsh_07_b", 0};
void* rsh_07_c[2] = {"rsh_07_c", 0};
void* rsh_08_a[2] = {"rsh_08_a", 0};
void* pik_00[2] = {"pik_00", 0};
void* pik_01[2] = {"pik_01", 0};
void* pik_02[2] = {"pik_02", 0};
void* pik_03[2] = {"pik_03", 0};
void* pik_04[2] = {"pik_04", 0};
void* hom_00[2] = {"hom_00", 0};
void* hom_10[2] = {"hom_10", 0};
void* hom_11[2] = {"hom_11", 0};
void* hom_12[2] = {"hom_12", 0};
void* eki_00[2] = {"eki_00", 0};
void* eki_01[2] = {"eki_01", 0};
void* eki_02[2] = {"eki_02", 0};
void* eki_03[2] = {"eki_03", 0};
void* eki_04[2] = {"eki_04", 0};
void* eki_05[2] = {"eki_05", 0};
void* eki_06[2] = {"eki_06", 0};
void* win_00[2] = {"win_00", 0};
void* win_01[2] = {"win_01", 0};
void* win_02[2] = {"win_02", 0};
void* win_03[2] = {"win_03", 0};
void* win_04[2] = {"win_04", 0};
void* win_05[2] = {"win_05", 0};
void* win_06[2] = {"win_06", 0};
void* mri_00[2] = {"mri_00", 0};
void* mri_01[2] = {"mri_01", 0};
void* mri_02[2] = {"mri_02", 0};
void* mri_03[2] = {"mri_03", 0};
void* mri_04[2] = {"mri_04", 0};
void* mri_05[2] = {"mri_05", 0};
void* mri_06[2] = {"mri_06", 0};
void* mri_07[2] = {"mri_07", 0};
void* mri_08[2] = {"mri_08", 0};
void* mri_09[2] = {"mri_09", 0};
void* mri_10[2] = {"mri_10", 0};
void* mri_11[2] = {"mri_11", 0};
void* mri_12[2] = {"mri_12", 0};
void* mri_13[2] = {"mri_13", 0};
void* mri_14[2] = {"mri_14", 0};
void* mri_15[2] = {"mri_15", 0};
void* mri_16[2] = {"mri_16", 0};
void* mri_17[2] = {"mri_17", 0};
void* mri_18[2] = {"mri_18", 0};
void* mri_19[2] = {"mri_19", 0};
void* mri_20[2] = {"mri_20", 0};
void* usu_00[2] = {"usu_00", 0};
void* usu_01[2] = {"usu_01", 0};
void* gra_00[2] = {"gra_00", 0};
void* gra_01[2] = {"gra_01", 0};
void* gra_02[2] = {"gra_02", 0};
void* gra_03[2] = {"gra_03", 0};
void* gra_04[2] = {"gra_04", 0};
void* gra_05[2] = {"gra_05", 0};
void* gra_06[2] = {"gra_06", 0};
void* jin_00[2] = {"jin_00", 0};
void* jin_01[2] = {"jin_01", 0};
void* jin_02[2] = {"jin_02", 0};
void* jin_03[2] = {"jin_03", 0};
void* jin_04[2] = {"jin_04", 0};
void* jin_05[2] = {"jin_05", 0};
void* jin_06[2] = {"jin_06", 0};
void* jin_07[2] = {"jin_07", 0};
void* jin_08[2] = {"jin_08", 0};
void* jin_09[2] = {"jin_09", 0};
void* jin_10[2] = {"jin_10", 0};
void* jin_11[2] = {"jin_11", 0};
void* tou_00[2] = {"tou_00", 0};
void* tou_01[2] = {"tou_01", 0};
void* tou_02[2] = {"tou_02", 0};
void* tou_03[2] = {"tou_03", 0};
void* tou_04[2] = {"tou_04", 0};
void* tou_05[2] = {"tou_05", 0};
void* tou_06[2] = {"tou_06", 0};
void* tou_07[2] = {"tou_07", 0};
void* tou_08[2] = {"tou_08", 0};
void* tou_09[2] = {"tou_09", 0};
void* tou_10[2] = {"tou_10", 0};
void* tou_11[2] = {"tou_11", 0};
void* tou_12[2] = {"tou_12", 0};
void* tou_13[2] = {"tou_13", 0};
void* tou_20[2] = {"tou_20", 0};
void* las_00[2] = {"las_00", 0};
void* las_01[2] = {"las_01", 0};
void* las_02[2] = {"las_02", 0};
void* las_03[2] = {"las_03", 0};
void* las_04[2] = {"las_04", 0};
void* las_05[2] = {"las_05", 0};
void* las_06[2] = {"las_06", 0};
void* las_07[2] = {"las_07", 0};
void* las_08[2] = {"las_08", 0};
void* las_09[2] = {"las_09", 0};
void* las_10[2] = {"las_10", 0};
void* las_11[2] = {"las_11", 0};
void* las_12[2] = {"las_12", 0};
void* las_13[2] = {"las_13", 0};
void* las_14[2] = {"las_14", 0};
void* las_15[2] = {"las_15", 0};
void* las_16[2] = {"las_16", 0};
void* las_17[2] = {"las_17", 0};
void* las_18[2] = {"las_18", 0};
void* las_19[2] = {"las_19", 0};
void* las_20[2] = {"las_20", 0};
void* las_21[2] = {"las_21", 0};
void* las_22[2] = {"las_22", 0};
void* las_23[2] = {"las_23", 0};
void* las_24[2] = {"las_24", 0};
void* las_25[2] = {"las_25", 0};
void* las_26[2] = {"las_26", 0};
void* las_27[2] = {"las_27", 0};
void* las_28[2] = {"las_28", 0};
void* las_29[2] = {"las_29", 0};
void* las_30[2] = {"las_30", 0};
void* kpa_00[2] = {"kpa_00", 0};
void* kpa_01[2] = {"kpa_01", 0};
void* kpa_02[2] = {"kpa_02", 0};
void* kpa_03[2] = {"kpa_03", 0};
void* kpa_04[2] = {"kpa_04", 0};
void* kpa_05[2] = {"kpa_05", 0};
void* kpa_06[2] = {"kpa_06", 0};
void* kpa_07[2] = {"kpa_07", 0};
void* jon_00[2] = {"jon_00", 0};
void* jon_01[2] = {"jon_01", 0};
void* jon_02[2] = {"jon_02", 0};
void* jon_03[2] = {"jon_03", 0};
void* jon_04[2] = {"jon_04", 0};
void* jon_05[2] = {"jon_05", 0};
void* jon_06[2] = {"jon_06", 0};
void* yuu_00[2] = {"yuu_00", 0};
void* yuu_01[2] = {"yuu_01", 0};
void* yuu_02[2] = {"yuu_02", 0};
void* yuu_03[2] = {"yuu_03", 0};
void* qiz_00[2] = {"qiz_00", 0};
void* sys_00[2] = {"sys_00", 0};
void* sys_01[2] = {"sys_01", 0};
void* dmo_00[2] = {"dmo_00", 0};
void* dig_00[2] = {"dig_00", 0};
void* end_00[2] = {"end_00", 0};

void* area_kwb[17] = {
    "tst", 0, 0, 0,
    tst_bt00, tst_00, tst_01, tst_02, tst_moj_00, tst_moj_01,
    tst_moj_02, tst_moj_03, tst_off_01, tst_npc, tst_nakayama, tst_item_00,
    0,
};

void* area_bti[9] = {
    "bti", 0, 0, 0,
    bti_01, bti_02, bti_03, bti_04,
    0,
};

void* area_stg[10] = {
    "stg", 0, 0, 0,
    stg_01_0, stg_01_1, stg_01_2, stg_01_3, stg_01_4,
    0,
};

void* area_aaa[6] = {
    "aaa", 0, 0, 0,
    aaa_00,
    0,
};

void* area_gor[13] = {
    "gor", 0, 0, 0,
    gor_00, gor_01, gor_02, gor_03, gor_04, gor_10,
    gor_11, gor_12,
    0,
};

void* area_tik[24] = {
    "tik", 0, 0, 0,
    tik_00, tik_01, tik_02, tik_03, tik_04, tik_05,
    tik_06, tik_07, tik_08, tik_11, tik_12, tik_13,
    tik_15, tik_16, tik_17, tik_18, tik_19, tik_20,
    tik_21,
    0,
};

void* area_gon[20] = {
    "gon", 0, 0, 0,
    gon_00, gon_01, gon_02, gon_03, gon_04, gon_05,
    gon_06, gon_07, gon_08, gon_09, gon_10, gon_11,
    gon_12, gon_13, gon_bt00,
    0,
};

void* area_hei[20] = {
    "hei", "\x95\xBD\x8C\xB4", 0, 0,
    hei_00, hei_01, hei_02, hei_03, hei_04, hei_05,
    hei_06, hei_07, hei_08, hei_09, hei_10, hei_11,
    hei_12, hei_13, hei_bt00,
    0,
};

void* area_nok[7] = {
    "nok", 0, 0, 0,
    nok_00, nok_01,
    0,
};

void* area_muj[16] = {
    "muj", 0, 0, 0,
    muj_00, muj_01, muj_02, muj_03, muj_04, muj_05,
    muj_10, muj_11, muj_12, muj_20, muj_21,
    0,
};

void* area_dou[19] = {
    "dou", "\x93\xB4\x8C\x41", 0, 0,
    dou_00, dou_01, dou_02, dou_03, dou_04, dou_05,
    dou_06, dou_07, dou_08, dou_09, dou_10, dou_11,
    dou_12, dou_13,
    0,
};

void* area_rsh[26] = {
    "rsh", "\x97\xF1\x8E\xD4", 0, 0,
    rsh_00_a, rsh_00_b, rsh_00_c, rsh_01_a, rsh_01_b, rsh_01_c,
    rsh_02_a, rsh_02_b, rsh_02_c, rsh_03_a, rsh_03_b, rsh_03_c,
    rsh_04_a, rsh_04_b, rsh_04_c, rsh_05_a, rsh_06_a, rsh_07_a,
    rsh_07_b, rsh_07_c, rsh_08_a,
    0,
};

void* area_pik[10] = {
    "pik", 0, 0, 0,
    pik_00, pik_01, pik_02, pik_03, pik_04,
    0,
};

void* area_hom[9] = {
    "hom", 0, 0, 0,
    hom_00, hom_10, hom_11, hom_12,
    0,
};

void* area_eki[12] = {
    "eki", "\x89\x77", 0, 0,
    eki_00, eki_01, eki_02, eki_03, eki_04, eki_05,
    eki_06,
    0,
};

void* area_win[12] = {
    "win", 0, 0, 0,
    win_00, win_01, win_02, win_03, win_04, win_05,
    win_06,
    0,
};

void* area_mri[26] = {
    "mri", 0, 0, 0,
    mri_00, mri_01, mri_02, mri_03, mri_04, mri_05,
    mri_06, mri_07, mri_08, mri_09, mri_10, mri_11,
    mri_12, mri_13, mri_14, mri_15, mri_16, mri_17,
    mri_18, mri_19, mri_20,
    0,
};

void* area_usu[7] = {
    "usu", 0, 0, 0,
    usu_00, usu_01,
    0,
};

void* area_gra[12] = {
    "gra", 0, 0, 0,
    gra_00, gra_01, gra_02, gra_03, gra_04, gra_05,
    gra_06,
    0,
};

void* area_jin[17] = {
    "jin", 0, 0, 0,
    jin_00, jin_01, jin_02, jin_03, jin_04, jin_05,
    jin_06, jin_07, jin_08, jin_09, jin_10, jin_11,
    0,
};

void* area_tou[20] = {
    "tou", 0, 0, 0,
    tou_00, tou_01, tou_02, tou_03, tou_04, tou_05,
    tou_06, tou_07, tou_08, tou_09, tou_10, tou_11,
    tou_12, tou_13, tou_20,
    0,
};

void* area_bom[10] = {
    "bom", 0, 0, 0,
    bom_00, bom_01, bom_02, bom_03, bom_04,
    0,
};

void* area_moo[13] = {
    "moo", "\x8C\x8E", 0, 0,
    moo_00, moo_01, moo_02, moo_03, moo_04, moo_05,
    moo_06, moo_07,
    0,
};

void* area_aji[25] = {
    "aji", 0, 0, 0,
    aji_00, aji_01, aji_02, aji_03, aji_04, aji_05,
    aji_06, aji_07, aji_08, aji_09, aji_10, aji_11,
    aji_12, aji_13, aji_14, aji_15, aji_16, aji_17,
    aji_18, aji_19,
    0,
};

void* area_las[36] = {
    "las", 0, 0, 0,
    las_00, las_01, las_02, las_03, las_04, las_05,
    las_06, las_07, las_08, las_09, las_10, las_11,
    las_12, las_13, las_14, las_15, las_16, las_17,
    las_18, las_19, las_20, las_21, las_22, las_23,
    las_24, las_25, las_26, las_27, las_28, las_29,
    las_30,
    0,
};

void* area_kpa[13] = {
    "kpa", 0, 0, 0,
    kpa_00, kpa_01, kpa_02, kpa_03, kpa_04, kpa_05,
    kpa_06, kpa_07,
    0,
};

void* area_jon[12] = {
    "jon", 0, 0, 0,
    jon_00, jon_01, jon_02, jon_03, jon_04, jon_05,
    jon_06,
    0,
};

void* area_yuu[9] = {
    "yuu", 0, 0, 0,
    yuu_00, yuu_01, yuu_02, yuu_03,
    0,
};

void* area_qiz[6] = {
    "qiz", 0, 0, 0,
    qiz_00,
    0,
};

void* area_sys[7] = {
    "sys", 0, 0, 0,
    sys_00, sys_01,
    0,
};

void* area_dmo[6] = {
    "dmo", 0, 0, 0,
    dmo_00,
    0,
};

void* area_dig[6] = {
    "dig", 0, 0, 0,
    dig_00,
    0,
};

void* area_end[6] = {
    "end", 0, 0, 0,
    end_00,
    0,
};

/* Remaining target-backed world/display catalog. */
const char str_sjis_000009d4[] = "\203\145\203\130\203\147\227\160";
const char str_sjis_000009e0[] = "\225\221\221\344\203\132\203\142\203\147";
const char str_sjis_000009ec[] = "\224\167\214\151\203\132\203\142\203\147";
const char str_sjis_000009f8[] = "\203\175\203\212\203\111\202\314\211\306";
const char str_sjis_00000a04[] = "\203\123\203\215\203\143\203\114\203\136\203\105\203\223";
const char str_sjis_00000a14[] = "\226\204\226\166\202\265\202\275\223\163\216\163";
const char str_sjis_00000a24[] = "\203\123\203\223\203\157\203\157";
const char str_sjis_00000a30[] = "\203\155\203\122\221\272";
const char str_sjis_00000a38[] = "\226\263\220\154\223\207";
const char str_sjis_00000a40[] = "\203\163\203\112\203\212\201\133\203\161\203\213\203\131";
const char str_sjis_00000a50[] = "\223\162\222\206\211\167\203\172\201\133\203\200";
const char str_sjis_00000a60[] = "\203\105\203\102\203\223\203\146\203\102\202\361\211\306\216\374\225\323";
const char str_sjis_00000a74[] = "\213\220\226\330\202\314\222\206";
const char str_sjis_00000a80[] = "\203\105\203\130\203\117\203\211\203\136\203\105\203\223";
const char str_sjis_00000a90[] = "\203\105\203\130\203\117\203\211\202\314\223\271";
const char str_sjis_00000aa0[] = "\203\137\203\103\203\200\216\233\211\100";
const char str_sjis_00000aac[] = "\223\254\213\132\217\352";
const char str_sjis_00000ab4[] = "\203\173\203\200\221\272";
const char str_sjis_00000abc[] = "\223\107\202\314\203\101\203\127\203\147";
const char str_sjis_00000ac8[] = "\203\211\203\130\203\147\203\137\203\223\203\127\203\207\203\223";
const char str_sjis_00000adc[] = "\203\116\203\142\203\160\225\322";
const char str_sjis_00000ae8[] = "\202\120\202\117\202\117\212\113\203\137\203\223\203\127\203\207\203\223";
const char str_sjis_00000afc[] = "\227\126\213\132\217\352";
const char str_sjis_00000b04[] = "\202\315\202\304\202\310\202\336\202\265\203\116\203\103\203\131";
const char str_sjis_00000b18[] = "\203\126\203\130\203\145\203\200";
const char str_sjis_00000b24[] = "\203\111\201\133\203\166\203\152\203\223\203\117\203\146\203\202";
const char str_sjis_00000b38[] = "\203\137\203\103\203\127\203\106\203\130\203\147";
const char str_sjis_00000b48[] = "\203\107\203\223\203\146\203\102\203\223\203\117";
const char str_MarioSt_802c0d00[] = "MarioSt";
const char str_sjis_00000b60[] = "\203\175\203\212\203\111\202\314\220\242\212\105";

void* world_data[66] = {
    (void*)str_MarioSt_802c0d00, (void*)str_sjis_00000b60, area_kwb, area_bti, area_stg, area_aaa,
    area_gor, area_tik, area_gon, area_hei, area_nok, area_win,
    area_mri, area_tou, area_usu, area_gra, area_jin, area_muj,
    area_dou, area_pik, area_rsh, area_hom, area_eki, area_bom,
    area_moo, area_aji, area_las, area_kpa, area_jon, area_yuu,
    area_qiz, area_sys, area_dmo, area_dig, area_end,
    0,
};

__declspec(section ".data") u8 gap_05_803058EC_data[20] = {0};

__declspec(section ".sdata2") const u32 gap_09_8041FB04_sdata2 = 0;
