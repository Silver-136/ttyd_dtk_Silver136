#include "database.h"
#include "driver/arcdrv.h"

void* ptr;
u32 size;

void setupDataLoad(char* name) {
    extern void* ptr;
    extern u32 size;
    extern char* getMarioStDvdRoot(void);
    extern s32 sprintf(char* str, const char* fmt, ...);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 evtGetValue(void* event, s32 arg);
    extern void* DVDMgrOpen(const char* path, s32 mode, s32 unk);
    extern u32 DVDMgrGetLength(void* entry);
    extern void DVDMgrRead(void* entry, void* dst, u32 size, u32 offset);
    extern void DVDMgrClose(void* entry);
    extern void* _mapAlloc(void* heap, u32 size);
    extern void* mapalloc_base_ptr;
    extern const char str_PCTs_m_PCTs_s_802c3b20[];
    extern const char str_muj_20_802c3b2c[];
    char path[64];
    void* entry;

    ptr = NULL;
    sprintf(path, str_PCTs_m_PCTs_s_802c3b20, getMarioStDvdRoot(), name);
    if (strcmp(name, str_muj_20_802c3b2c) == 0 && evtGetValue(NULL, 0xF5DE0180) == 2) {
        return;
    }
    arcOpen(path, &ptr, &size);
    if (ptr != NULL) {
        return;
    }
    entry = DVDMgrOpen(path, 2, 0);
    if (entry != NULL) {
        size = DVDMgrGetLength(entry);
        if (size != 0) {
            ptr = _mapAlloc(mapalloc_base_ptr, (size + 0x1F) & ~0x1F);
            DVDMgrRead(entry, ptr, (size + 0x1F) & ~0x1F, 0);
        }
        DVDMgrClose(entry);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 setupDataCheck(void) {
    extern void* ptr;
    extern u32 size;
    extern void parseInit(void* data, u32 size);
    extern s32 parsePush(const char* tag);
    extern void parsePop(void);
    extern void parseTagGet1(const char* tag, s32 index, char* out);
    extern void parsePopNext(void);
    extern void* npcGetTribe(char* name);
    extern s32 animGroupBaseAsync(const char* name, s32 a, s32 b);
    extern void* gp;
    extern const char str_tribe_802c3a50[];
    extern char str_Npc_804207b8[6];
    s32 useLang;
    char name[64];

    if (ptr == NULL) {
        return 1;
    }
    parseInit(ptr, size);
    while (parsePush(str_Npc_804207b8) != 0) {
        parseTagGet1(str_tribe_802c3a50, 0, name);
        parsePopNext();
        useLang = (s32)((u32)(-*(s32*)((s32)gp + 0x14) | *(s32*)((s32)gp + 0x14)) >> 31);
        if (animGroupBaseAsync(*(char**)((s32)npcGetTribe(name) + 4), useLang, 0) == 0) {
            return 0;
        }
    }
    parsePop();
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



const char str_tik_00_802c3450[] = "tik_00";
const char str_tik_01_802c3458[] = "tik_01";
const char str_tik_02_802c3460[] = "tik_02";
const char str_tik_03_802c3468[] = "tik_03";
const char str_tik_04_802c3470[] = "tik_04";
const char str_tik_05_802c3478[] = "tik_05";
const char str_tik_06_802c3480[] = "tik_06";
const char str_tik_07_802c3488[] = "tik_07";
const char str_tik_08_802c3490[] = "tik_08";
const char str_tik_09_802c3498[] = "tik_09";
const char str_tik_10_802c34a0[] = "tik_10";
const char str_tik_11_802c34a8[] = "tik_11";
const char str_tik_12_802c34b0[] = "tik_12";
const char str_tik_13_802c34b8[] = "tik_13";
const char str_tik_14_802c34c0[] = "tik_14";
const char str_tik_15_802c34c8[] = "tik_15";
const char str_tik_16_802c34d0[] = "tik_16";
const char str_tik_17_802c34d8[] = "tik_17";
const char str_tik_18_802c34e0[] = "tik_18";
const char str_tik_19_802c34e8[] = "tik_19";
const char str_tik_20_802c34f0[] = "tik_20";
const char str_tik_21_802c34f8[] = "tik_21";
const char str_hei_00_802c3500[] = "hei_00";
const char str_hei_01_802c3508[] = "hei_01";
const char str_hei_02_802c3510[] = "hei_02";
const char str_hei_03_802c3518[] = "hei_03";
const char str_hei_04_802c3520[] = "hei_04";
const char str_hei_05_802c3528[] = "hei_05";
const char str_hei_06_802c3530[] = "hei_06";
const char str_hei_07_802c3538[] = "hei_07";
const char str_hei_08_802c3540[] = "hei_08";
const char str_hei_09_802c3548[] = "hei_09";
const char str_hei_10_802c3550[] = "hei_10";
const char str_hei_11_802c3558[] = "hei_11";
const char str_hei_12_802c3560[] = "hei_12";
const char str_hei_13_802c3568[] = "hei_13";
const char str_gon_00_802c3570[] = "gon_00";
const char str_gon_01_802c3578[] = "gon_01";
const char str_gon_02_802c3580[] = "gon_02";
const char str_gon_03_802c3588[] = "gon_03";
const char str_gon_04_802c3590[] = "gon_04";
const char str_gon_05_802c3598[] = "gon_05";
const char str_gon_06_802c35a0[] = "gon_06";
const char str_gon_07_802c35a8[] = "gon_07";
const char str_gon_08_802c35b0[] = "gon_08";
const char str_gon_09_802c35b8[] = "gon_09";
const char str_gon_10_802c35c0[] = "gon_10";
const char str_gon_11_802c35c8[] = "gon_11";
const char str_gon_12_802c35d0[] = "gon_12";
const char str_gon_13_802c35d8[] = "gon_13";
const char str_win_00_802c35e0[] = "win_00";
const char str_win_01_802c35e8[] = "win_01";
const char str_win_02_802c35f0[] = "win_02";
const char str_win_03_802c35f8[] = "win_03";
const char str_win_04_802c3600[] = "win_04";
const char str_win_05_802c3608[] = "win_05";
const char str_win_06_802c3610[] = "win_06";
const char str_mri_00_802c3618[] = "mri_00";
const char str_mri_01_802c3620[] = "mri_01";
const char str_mri_02_802c3628[] = "mri_02";
const char str_mri_03_802c3630[] = "mri_03";
const char str_mri_04_802c3638[] = "mri_04";
const char str_mri_05_802c3640[] = "mri_05";
const char str_mri_06_802c3648[] = "mri_06";
const char str_mri_07_802c3650[] = "mri_07";
const char str_mri_08_802c3658[] = "mri_08";
const char str_mri_09_802c3660[] = "mri_09";
const char str_mri_10_802c3668[] = "mri_10";
const char str_mri_11_802c3670[] = "mri_11";
const char str_mri_12_802c3678[] = "mri_12";
const char str_mri_13_802c3680[] = "mri_13";
const char str_mri_14_802c3688[] = "mri_14";
const char str_mri_15_802c3690[] = "mri_15";
const char str_mri_16_802c3698[] = "mri_16";
const char str_mri_17_802c36a0[] = "mri_17";
const char str_mri_18_802c36a8[] = "mri_18";
const char str_mri_19_802c36b0[] = "mri_19";
const char str_mri_20_802c36b8[] = "mri_20";
const char str_gra_00_802c36c0[] = "gra_00";
const char str_gra_01_802c36c8[] = "gra_01";
const char str_gra_02_802c36d0[] = "gra_02";
const char str_gra_03_802c36d8[] = "gra_03";
const char str_gra_04_802c36e0[] = "gra_04";
const char str_gra_05_802c36e8[] = "gra_05";
const char str_gra_06_802c36f0[] = "gra_06";
const char str_jin_00_802c36f8[] = "jin_00";
const char str_jin_01_802c3700[] = "jin_01";
const char str_jin_02_802c3708[] = "jin_02";
const char str_jin_03_802c3710[] = "jin_03";
const char str_jin_04_802c3718[] = "jin_04";
const char str_jin_05_802c3720[] = "jin_05";
const char str_jin_06_802c3728[] = "jin_06";
const char str_jin_07_802c3730[] = "jin_07";
const char str_jin_08_802c3738[] = "jin_08";
const char str_jin_09_802c3740[] = "jin_09";
const char str_jin_10_802c3748[] = "jin_10";
const char str_jin_11_802c3750[] = "jin_11";
const char str_muj_00_802c3758[] = "muj_00";
const char str_muj_01_802c3760[] = "muj_01";
const char str_muj_02_802c3768[] = "muj_02";
const char str_muj_03_802c3770[] = "muj_03";
const char str_muj_04_802c3778[] = "muj_04";
const char str_muj_05_802c3780[] = "muj_05";
const char str_muj_10_802c3788[] = "muj_10";
const char str_muj_11_802c3790[] = "muj_11";
const char str_muj_12_802c3798[] = "muj_12";
const char str_dou_00_802c37a0[] = "dou_00";
const char str_dou_01_802c37a8[] = "dou_01";
const char str_dou_02_802c37b0[] = "dou_02";
const char str_dou_03_802c37b8[] = "dou_03";
const char str_dou_04_802c37c0[] = "dou_04";
const char str_dou_05_802c37c8[] = "dou_05";
const char str_dou_06_802c37d0[] = "dou_06";
const char str_dou_07_802c37d8[] = "dou_07";
const char str_dou_08_802c37e0[] = "dou_08";
const char str_dou_09_802c37e8[] = "dou_09";
const char str_dou_10_802c37f0[] = "dou_10";
const char str_dou_11_802c37f8[] = "dou_11";
const char str_dou_12_802c3800[] = "dou_12";
const char str_dou_13_802c3808[] = "dou_13";
const char str_eki_00_802c3810[] = "eki_00";
const char str_eki_01_802c3818[] = "eki_01";
const char str_eki_02_802c3820[] = "eki_02";
const char str_eki_03_802c3828[] = "eki_03";
const char str_eki_04_802c3830[] = "eki_04";
const char str_eki_05_802c3838[] = "eki_05";
const char str_eki_06_802c3840[] = "eki_06";
const char str_pik_00_802c3848[] = "pik_00";
const char str_pik_01_802c3850[] = "pik_01";
const char str_pik_02_802c3858[] = "pik_02";
const char str_pik_03_802c3860[] = "pik_03";
const char str_pik_04_802c3868[] = "pik_04";
const char str_bom_03_802c3870[] = "bom_03";
const char str_bom_04_802c3878[] = "bom_04";
const char str_moo_00_802c3880[] = "moo_00";
const char str_moo_01_802c3888[] = "moo_01";
const char str_moo_02_802c3890[] = "moo_02";
const char str_moo_05_802c3898[] = "moo_05";
const char str_moo_06_802c38a0[] = "moo_06";
const char str_moo_07_802c38a8[] = "moo_07";
const char str_aji_00_802c38b0[] = "aji_00";
const char str_aji_01_802c38b8[] = "aji_01";
const char str_aji_02_802c38c0[] = "aji_02";
const char str_aji_03_802c38c8[] = "aji_03";
const char str_aji_04_802c38d0[] = "aji_04";
const char str_aji_05_802c38d8[] = "aji_05";
const char str_aji_06_802c38e0[] = "aji_06";
const char str_aji_07_802c38e8[] = "aji_07";
const char str_aji_08_802c38f0[] = "aji_08";
const char str_aji_09_802c38f8[] = "aji_09";
const char str_aji_10_802c3900[] = "aji_10";
const char str_aji_11_802c3908[] = "aji_11";
const char str_aji_12_802c3910[] = "aji_12";
const char str_aji_13_802c3918[] = "aji_13";
const char str_aji_14_802c3920[] = "aji_14";
const char str_aji_15_802c3928[] = "aji_15";
const char str_aji_16_802c3930[] = "aji_16";
const char str_aji_17_802c3938[] = "aji_17";
const char str_aji_18_802c3940[] = "aji_18";
const char str_aji_19_802c3948[] = "aji_19";
const char str_las_00_802c3950[] = "las_00";
const char str_las_01_802c3958[] = "las_01";
const char str_las_02_802c3960[] = "las_02";
const char str_las_03_802c3968[] = "las_03";
const char str_las_04_802c3970[] = "las_04";
const char str_las_05_802c3978[] = "las_05";
const char str_las_06_802c3980[] = "las_06";
const char str_las_07_802c3988[] = "las_07";
const char str_las_08_802c3990[] = "las_08";
const char str_las_09_802c3998[] = "las_09";
const char str_las_10_802c39a0[] = "las_10";
const char str_las_11_802c39a8[] = "las_11";
const char str_las_12_802c39b0[] = "las_12";
const char str_las_13_802c39b8[] = "las_13";
const char str_las_14_802c39c0[] = "las_14";
const char str_las_15_802c39c8[] = "las_15";
const char str_las_16_802c39d0[] = "las_16";
const char str_las_17_802c39d8[] = "las_17";
const char str_las_18_802c39e0[] = "las_18";
const char str_las_19_802c39e8[] = "las_19";
const char str_las_20_802c39f0[] = "las_20";
const char str_las_21_802c39f8[] = "las_21";
const char str_las_22_802c3a00[] = "las_22";
const char str_las_23_802c3a08[] = "las_23";
const char str_las_24_802c3a10[] = "las_24";
const char str_las_25_802c3a18[] = "las_25";
const char str_las_26_802c3a20[] = "las_26";
const char str_las_27_802c3a28[] = "las_27";
const char str_las_28_802c3a30[] = "las_28";
const char str_las_29_802c3a38[] = "las_29";
const char str_las_30_802c3a40[] = "las_30";

void* _map_attr_data[] = {
    (void*)(char*)str_tik_00_802c3450, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_01_802c3458, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_02_802c3460, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_03_802c3468, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_04_802c3470, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_05_802c3478, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_06_802c3480, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_07_802c3488, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_08_802c3490, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_09_802c3498, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_10_802c34a0, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_11_802c34a8, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_12_802c34b0, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_13_802c34b8, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_14_802c34c0, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_15_802c34c8, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_16_802c34d0, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_17_802c34d8, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_18_802c34e0, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_19_802c34e8, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_20_802c34f0, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_tik_21_802c34f8, (void*)0x00000010, (void*)0x00000007,
    (void*)(char*)str_hei_00_802c3500, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_01_802c3508, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_02_802c3510, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_03_802c3518, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_04_802c3520, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_05_802c3528, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_06_802c3530, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_07_802c3538, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_hei_08_802c3540, (void*)0x00000005, (void*)0x00000007,
    (void*)(char*)str_hei_09_802c3548, (void*)0x00000005, (void*)0x00000007,
    (void*)(char*)str_hei_10_802c3550, (void*)0x00000005, (void*)0x00000007,
    (void*)(char*)str_hei_11_802c3558, (void*)0x00000005, (void*)0x00000007,
    (void*)(char*)str_hei_12_802c3560, (void*)0x00000005, (void*)0x00000007,
    (void*)(char*)str_hei_13_802c3568, (void*)0x00000001, (void*)0x00000005,
    (void*)(char*)str_gon_00_802c3570, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_01_802c3578, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_02_802c3580, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_03_802c3588, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_04_802c3590, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_05_802c3598, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_06_802c35a0, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_07_802c35a8, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_08_802c35b0, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_09_802c35b8, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_10_802c35c0, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_11_802c35c8, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_12_802c35d0, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_gon_13_802c35d8, (void*)0x00000002, (void*)0x00000007,
    (void*)(char*)str_win_00_802c35e0, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_win_01_802c35e8, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_win_02_802c35f0, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_win_03_802c35f8, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_win_04_802c3600, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_win_05_802c3608, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_win_06_802c3610, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_mri_00_802c3618, (void*)0x00000003, (void*)0x00000005,
    (void*)(char*)str_mri_01_802c3620, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_02_802c3628, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_03_802c3630, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_04_802c3638, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_05_802c3640, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_06_802c3648, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_07_802c3650, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_08_802c3658, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_09_802c3660, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_10_802c3668, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_11_802c3670, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_12_802c3678, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_13_802c3680, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_14_802c3688, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_15_802c3690, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_16_802c3698, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_17_802c36a0, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_18_802c36a8, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_19_802c36b0, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_mri_20_802c36b8, (void*)0x00000004, (void*)0x00000007,
    (void*)(char*)str_gra_00_802c36c0, (void*)0x00000006, (void*)0x00000005,
    (void*)(char*)str_gra_01_802c36c8, (void*)0x00000006, (void*)0x00000005,
    (void*)(char*)str_gra_02_802c36d0, (void*)0x00000006, (void*)0x00000005,
    (void*)(char*)str_gra_03_802c36d8, (void*)0x00000006, (void*)0x00000005,
    (void*)(char*)str_gra_04_802c36e0, (void*)0x00000006, (void*)0x00000005,
    (void*)(char*)str_gra_05_802c36e8, (void*)0x00000006, (void*)0x00000005,
    (void*)(char*)str_gra_06_802c36f0, (void*)0x00000006, (void*)0x00000005,
    (void*)(char*)str_jin_00_802c36f8, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_01_802c3700, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_02_802c3708, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_03_802c3710, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_04_802c3718, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_05_802c3720, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_06_802c3728, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_07_802c3730, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_08_802c3738, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_09_802c3740, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_10_802c3748, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_jin_11_802c3750, (void*)0x00000007, (void*)0x00000007,
    (void*)(char*)str_muj_00_802c3758, (void*)0x00000008, (void*)0x00000005,
    (void*)(char*)str_muj_01_802c3760, (void*)0x00000008, (void*)0x00000005,
    (void*)(char*)str_muj_02_802c3768, (void*)0x00000008, (void*)0x00000005,
    (void*)(char*)str_muj_03_802c3770, (void*)0x00000008, (void*)0x00000005,
    (void*)(char*)str_muj_04_802c3778, (void*)0x00000008, (void*)0x00000005,
    (void*)(char*)str_muj_05_802c3780, (void*)0x00000008, (void*)0x00000005,
    (void*)(char*)str_muj_10_802c3788, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_muj_11_802c3790, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_muj_12_802c3798, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_00_802c37a0, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_01_802c37a8, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_02_802c37b0, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_03_802c37b8, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_04_802c37c0, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_05_802c37c8, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_06_802c37d0, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_07_802c37d8, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_08_802c37e0, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_09_802c37e8, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_10_802c37f0, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_11_802c37f8, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_12_802c3800, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_dou_13_802c3808, (void*)0x00000009, (void*)0x00000007,
    (void*)(char*)str_eki_00_802c3810, (void*)0x0000000A, (void*)0x00000007,
    (void*)(char*)str_eki_01_802c3818, (void*)0x0000000A, (void*)0x00000007,
    (void*)(char*)str_eki_02_802c3820, (void*)0x0000000A, (void*)0x00000007,
    (void*)(char*)str_eki_03_802c3828, (void*)0x0000000A, (void*)0x00000007,
    (void*)(char*)str_eki_04_802c3830, (void*)0x0000000A, (void*)0x00000007,
    (void*)(char*)str_eki_05_802c3838, (void*)0x0000000A, (void*)0x00000007,
    (void*)(char*)str_eki_06_802c3840, (void*)0x0000000A, (void*)0x00000007,
    (void*)(char*)str_pik_00_802c3848, (void*)0x00000011, (void*)0x00000007,
    (void*)(char*)str_pik_01_802c3850, (void*)0x00000011, (void*)0x00000007,
    (void*)(char*)str_pik_02_802c3858, (void*)0x00000011, (void*)0x00000007,
    (void*)(char*)str_pik_03_802c3860, (void*)0x00000011, (void*)0x00000007,
    (void*)(char*)str_pik_04_802c3868, (void*)0x00000011, (void*)0x00000007,
    (void*)(char*)str_bom_03_802c3870, (void*)0x0000000B, (void*)0x00000005,
    (void*)(char*)str_bom_04_802c3878, (void*)0x0000000B, (void*)0x00000005,
    (void*)(char*)str_eki_00_802c3810, (void*)0x0000000C, (void*)0x00000007,
    (void*)(char*)str_eki_01_802c3818, (void*)0x0000000C, (void*)0x00000007,
    (void*)(char*)str_eki_02_802c3820, (void*)0x0000000C, (void*)0x00000007,
    (void*)(char*)str_eki_03_802c3828, (void*)0x0000000C, (void*)0x00000007,
    (void*)(char*)str_eki_04_802c3830, (void*)0x0000000C, (void*)0x00000007,
    (void*)(char*)str_eki_05_802c3838, (void*)0x0000000C, (void*)0x00000007,
    (void*)(char*)str_eki_06_802c3840, (void*)0x0000000C, (void*)0x00000007,
    (void*)(char*)str_moo_00_802c3880, (void*)0x0000000D, (void*)0x00000005,
    (void*)(char*)str_moo_01_802c3888, (void*)0x0000000D, (void*)0x00000005,
    (void*)(char*)str_moo_02_802c3890, (void*)0x0000000D, (void*)0x00000005,
    (void*)(char*)str_moo_05_802c3898, (void*)0x0000000D, (void*)0x00000005,
    (void*)(char*)str_moo_06_802c38a0, (void*)0x0000000D, (void*)0x00000005,
    (void*)(char*)str_moo_07_802c38a8, (void*)0x0000000D, (void*)0x00000005,
    (void*)(char*)str_aji_00_802c38b0, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_01_802c38b8, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_02_802c38c0, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_03_802c38c8, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_04_802c38d0, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_05_802c38d8, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_06_802c38e0, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_07_802c38e8, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_08_802c38f0, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_09_802c38f8, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_10_802c3900, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_11_802c3908, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_12_802c3910, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_13_802c3918, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_14_802c3920, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_15_802c3928, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_16_802c3930, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_17_802c3938, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_18_802c3940, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_aji_19_802c3948, (void*)0x0000000E, (void*)0x00000007,
    (void*)(char*)str_las_00_802c3950, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_01_802c3958, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_02_802c3960, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_03_802c3968, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_04_802c3970, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_05_802c3978, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_06_802c3980, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_07_802c3988, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_08_802c3990, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_09_802c3998, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_10_802c39a0, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_11_802c39a8, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_12_802c39b0, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_13_802c39b8, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_14_802c39c0, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_15_802c39c8, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_16_802c39d0, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_17_802c39d8, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_18_802c39e0, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_19_802c39e8, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_20_802c39f0, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_21_802c39f8, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_22_802c3a00, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_23_802c3a08, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_24_802c3a10, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_25_802c3a18, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_26_802c3a20, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_27_802c3a28, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_28_802c3a30, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_29_802c3a38, (void*)0x0000000F, (void*)0x00000007,
    (void*)(char*)str_las_30_802c3a40, (void*)0x0000000F, (void*)0x00000007,
    (void*)0, (void*)0xFFFFFFFF, (void*)0x00000000, /* sentinel */
};

void setupDataBase(char* area, char* map) {
    typedef struct DeadInfo {
        u32 word0;
        u32 word1;
        u32 word2;
        u32 word3;
        u32 word4;
    } DeadInfo;
    typedef struct NpcAiTypeTable {
        char* name;
        u32 flags;
        void* initEvt;
        void* moveEvt;
        void* deadEvt;
        void* findEvt;
        void* lostEvt;
        void* returnEvt;
        void* blowEvt;
    } NpcAiTypeTable;
    extern void* fbatGetPointer(void);
    extern s32 strcmp(const char* a, const char* b);
    extern char* strcpy(char* dst, const char* src);
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern void parseInit(void* data, u32 size);
    extern u32 parsePush(const char* tag);
    extern void parseTagGet1(const char* tag, s32 type, void* out);
    extern void parsePopNext(void);
    extern void parsePop(void);
    extern void* npcGetTribe(char* name);
    extern s32 npcEntry(char* name, char* model);
    extern void* npcNameToPtr(char* name);
    extern void* dbGetDefData(void* table, char* name);
    extern void* areaDataPtr(char* area);
    extern void npcSetBattleInfo(void* npc, s32 info);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void* ptr;
    extern u32 size;
    extern NpcAiTypeTable npc_ai_type_table[];
    extern void* npc_define_territory_type[];
    extern char str_Npc_804207b8[];
    extern char str_dir_804207ac[];
    extern char str_BTLNO_INVALID_802c3b10[];
    extern char zero_804207a8;
    extern f32 float_0_804207b4;

    void* fbat = fbatGetPointer();
    u8* dead = (u8*)fbat + 0x4C;
    u8* previousDead = dead + 0x14;
    void** attr = _map_attr_data;
    void** currentAttr;
    void** previousAttr;
    void* tribe;
    void* npc;
    NpcAiTypeTable* ai;
    void* areaData;
    s32 battleInfo;
    s32 index;
    s32 checkDead;
    char name[76];
    char tribeName[64];
    char aiType[64];
    char territoryType[64];
    char battleSetup[64];
    u32 position[3];
    f32 direction;
    u32 territoryBase[3];
    u32 territoryLoiter[3];
    u32 territoryHoming[3];
    f32 searchRange;
    f32 searchAngle;
    f32 homingRange;
    f32 homingAngle;

    while (attr[0] != 0 && strcmp((char*)attr[0], map) != 0) {
        attr += 3;
    }
    currentAttr = attr;

    {
        u8* deadScan = (u8*)fbat;

        index = 0;
        do {
            if (strcmp((char*)deadScan + 0x4C, &zero_804207a8) == 0) {
                u8* entry = dead + index * 0x14;
                memset(entry, 0, 0x14);
                strcpy((char*)entry, map);
                break;
            }
            if (strcmp((char*)deadScan + 0x4C, map) == 0) {
                break;
            }
            index++;
            deadScan += 0x14;
        } while (index < 64);
    }

    if (index > 0) {
        DeadInfo* entry = (DeadInfo*)(dead + index * 0x14);
        DeadInfo saved;
        saved.word0 = entry->word0;
        saved.word1 = entry->word1;
        saved.word2 = entry->word2;
        saved.word3 = entry->word3;
        saved.word4 = entry->word4;
        while (index >= 4) {
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            index -= 4;
        }
        while (index > 0) {
            entry->word0 = (entry - 1)->word0;
            entry->word1 = (entry - 1)->word1;
            entry->word2 = (entry - 1)->word2;
            entry->word3 = (entry - 1)->word3;
            entry->word4 = (entry - 1)->word4;
            entry--;
            index--;
        }
        ((DeadInfo*)dead)->word0 = saved.word0;
        ((DeadInfo*)dead)->word1 = saved.word1;
        ((DeadInfo*)dead)->word2 = saved.word2;
        ((DeadInfo*)dead)->word3 = saved.word3;
        ((DeadInfo*)dead)->word4 = saved.word4;
    }

    if ((u32)currentAttr[2] & 1) {
        void** checkAttr = _map_attr_data;
        while (checkAttr[0] != 0 &&
               strcmp((char*)checkAttr[0], map) != 0) {
            checkAttr += 3;
        }
        previousAttr = _map_attr_data;
        while (previousAttr[0] != 0 &&
               strcmp((char*)previousAttr[0], (char*)previousDead) != 0) {
            previousAttr += 3;
        }
        if (checkAttr[1] == previousAttr[1]) {
            u32 flags = (u32)currentAttr[2];
            if ((flags & 2) == 0 && (flags & 4)) {
                memset(dead + 0x28, 0, 0x4D8);
            }
        } else {
            memset(dead, 0, 0x500);
            strcpy((char*)dead, map);
        }
    }

    checkDead = ((u32)currentAttr[2] & 1) != 0;
    index = 0;
    if (ptr != 0) {
        parseInit(ptr, size);
        while (parsePush(str_Npc_804207b8) != 0) {
            parseTagGet1("<name>", 0, name);
            parseTagGet1("<tribe>", 0, tribeName);
            parseTagGet1("<position>", 3, position);
            parseTagGet1(str_dir_804207ac, 2, &direction);
            parseTagGet1("<aitype>", 0, aiType);
            parseTagGet1("<territory_type>", 0, territoryType);
            parseTagGet1("<territory_base>", 3, territoryBase);
            parseTagGet1("<territory_loiter>", 3, territoryLoiter);
            parseTagGet1("<territory_homing>", 3, territoryHoming);
            parseTagGet1("<search_range>", 2, &searchRange);
            parseTagGet1("<search_angle>", 2, &searchAngle);
            parseTagGet1("<homing_range>", 2, &homingRange);
            parseTagGet1("<homing_angle>", 2, &homingAngle);
            parseTagGet1("<btl_setup_no>", 0, battleSetup);
            parsePopNext();

            if (!checkDead ||
                ((*(u32*)(dead + 0x10) & (1U << (index & 63))) == 0)) {
                tribe = npcGetTribe(tribeName);
                npcEntry(name, *(char**)((s32)tribe + 4));
                npc = npcNameToPtr(name);
                *(u8*)((s32)npc + 0x314) = (u8)index;
                *(u32*)npc |= 4;
                *(u32*)((s32)npc + 0x8C) = position[0];
                *(u32*)((s32)npc + 0x90) = position[1];
                *(u32*)((s32)npc + 0x94) = position[2];
                *(f32*)((s32)npc + 0x144) = direction;
                *(void**)((s32)npc + 0x28) = tribe;
                *(f32*)((s32)npc + 0x14C) = *(f32*)((s32)tribe + 0x28);
                *(f32*)((s32)npc + 0x150) = *(f32*)((s32)tribe + 0x2C);
                *(u32*)npc &= ~2U;

                for (ai = npc_ai_type_table; ai->name != 0; ai++) {
                    if (strcmp(ai->name, aiType) == 0) {
                        *(u32*)npc |= ai->flags;
                        *(void**)((u8*)npc + 0x120) = ai->initEvt;
                        *(void**)((u8*)npc + 0x124) = ai->moveEvt;
                        *(void**)((u8*)npc + 0x12C) = ai->deadEvt;
                        *(void**)((u8*)npc + 0x130) = ai->findEvt;
                        *(void**)((u8*)npc + 0x134) = ai->lostEvt;
                        *(void**)((u8*)npc + 0x138) = ai->returnEvt;
                        *(void**)((u8*)npc + 0x13C) = ai->blowEvt;
                        break;
                    }
                }

                *(s32*)((u8*)npc + 0x1F8) = (s32)dbGetDefData(npc_define_territory_type, territoryType);
                *(u32*)((u8*)npc + 0x1FC) = territoryBase[0];
                *(u32*)((u8*)npc + 0x200) = territoryBase[1];
                *(u32*)((u8*)npc + 0x204) = territoryBase[2];
                *(u32*)((u8*)npc + 0x208) = territoryLoiter[0];
                *(u32*)((u8*)npc + 0x20C) = territoryLoiter[1];
                *(u32*)((u8*)npc + 0x210) = territoryLoiter[2];
                *(u32*)((u8*)npc + 0x214) = territoryHoming[0];
                *(u32*)((u8*)npc + 0x218) = territoryHoming[1];
                *(u32*)((u8*)npc + 0x21C) = territoryHoming[2];
                *(f32*)((u8*)npc + 0x220) = searchRange;
                *(f32*)((u8*)npc + 0x224) = searchAngle;
                *(f32*)((u8*)npc + 0x228) = homingRange;
                *(f32*)((u8*)npc + 0x22C) = homingAngle;
                *(void**)((u8*)npc + 0x1E0) = *(void**)((u8*)tribe + 0x40);
                *(void**)((u8*)npc + 0x1E4) = *(void**)((u8*)tribe + 0x44);
                *(u16*)((u8*)npc + 0x1E8) = *(u16*)((u8*)tribe + 0x48);
                *(void**)((u8*)npc + 0x1EC) = *(void**)((u8*)tribe + 0x4C);
                *(void**)((u8*)npc + 0x1F0) = *(void**)((u8*)tribe + 0x50);
                *(u16*)((u8*)npc + 0x1D8) = 1;
                *(f32*)((u8*)npc + 0x1DC) = float_0_804207b4;

                areaData = areaDataPtr(area);
                battleInfo = -1;
                if (strcmp(battleSetup, str_BTLNO_INVALID_802c3b10) != 0) {
                    battleInfo = (s32)dbGetDefData(*(void***)((u8*)areaData + 0xC), battleSetup);
                }
                npcSetBattleInfo(npc, battleInfo);
                if (*(char**)((u8*)tribe + 0x10) != 0) {
                    strcpy((char*)npc + 0x4C, *(char**)((u8*)tribe + 0x10));
                }
                if (*(char**)((u8*)tribe + 0x14) != 0) {
                    strcpy((char*)npc + 0x6C, *(char**)((u8*)tribe + 0x14));
                }
                if (*(char**)((u8*)tribe + 8) != 0) {
                    strcpy((char*)npc + 0x2C, *(char**)((u8*)tribe + 8));
                    animPoseSetAnim(*(s32*)((u8*)npc + 0x104), (char*)npc + 0x2C, 1);
                }
            }
            index++;
        }
        parsePop();
    }
}

const char zero_804207a8 = 0;
const char str_dir_804207ac[] = "<dir>";
const f32 float_0_804207b4 = 0.0f;
const char str_Npc_804207b8[] = "<Npc>";

const char str_BTLNO_INVALID_802c3b10[] = "BTLNO_INVALID";

const char str_PCTs_m_PCTs_s_802c3b20[] = "%s/m/%s/s";
const char str_muj_20_802c3b2c[] = "muj_20";
