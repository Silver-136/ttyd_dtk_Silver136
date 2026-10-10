#include "sequence/seq_gameover.h"
#include "countdown.h"
#include "cam_road.h"
#include "cam_shift.h"

#include "driver/fadedrv.h"
#include "statuswindow.h"

extern u8 psbgm[];
extern u8 sound[];

void psndStopAllFadeOut(void);
void marioStSystemLevel(s32 level);
void winReInit(void);
void L_camDispOn(s32 camId);
void psndSetReverb(s32 reverb);
void L_psndBGM_stop(void);
void psndClearFlag(s32 flag);
void N_pouchUnEquipBadgeID(s32 badgeId);

s32 fall_data[] = {
    200, 170, 140, 110, 80, 50, 20, 10, 26, 29, 30, 29, 26,
    10, 0, 12, 15, 16, 15, 11, 6, 0, 4, 3, 1, 0,
};

s32 fall_data2[] = {
    200, 170, 140, 110, 80, 50, 20, 0, 12, 15,
    16, 15, 11, 6, 0, 4, 3, 1, 0,
};

s32 N_evt_snd_bgm_unpause(void);
extern s32 evt_mapobj_trans();
extern s32 evt_mario_key_onoff();
extern s32 evt_mario_normalize();
extern s32 evt_mario_cont_onoff();
extern s32 evt_mario_set_pos();
extern s32 evt_cam_road_reset();
extern s32 evt_party_cont_onoff();
extern s32 evt_party_set_pos();
extern s32 evt_mario_kill_party();
extern s32 evt_mario_clear_party();
extern s32 evt_mario_dispflag_onoff();
extern s32 evt_mario_set_pose();
extern s32 evt_mario_set_dir();
extern s32 evt_mario_adjust_dir();
extern s32 evt_sub_get_language();
extern s32 evt_mapobj_flag_onoff();
extern s32 evt_cam_ctrl_onoff();
extern s32 evt_cam3d_evt_set();
extern s32 evt_map_playanim();
extern const char str_anm_hokori_802c2c00[];
extern const char str_g_80420448[];
extern const char str_a_8042044c[];
extern const char str_m_80420450[];
extern const char str_e_80420454[];
extern const char str_o_80420458[];
extern const char str_v_8042045c[];
extern const char str_e2_80420460[];
extern const char str_r_80420464[];
extern const char str_f_80420468[];
extern const char str_i_8042046c[];
extern const char str_n_80420470[];
extern const char str_d_80420474[];
extern const char str_e3_80420478[];
extern const char str_l_8042047c[];
extern const char str_j_80420480[];
extern const char str_u_80420484[];
extern const char str_e4_80420488[];
extern const char str_g1_8042048c[];
extern const char str_o289_80420490[];
extern const char str_M_D_2_80420498[];

s32 _fall_char[] = {
    0x0001003E, (s32)fall_data, 0x00010005, 0x0000001A,
    0x0001003F, 0xFE363C81, 0x0006005B, (s32)evt_mapobj_trans,
    0x00000001, 0xFE363C80, 0x00000000, 0xFE363C81,
    0x00000000, 0x00010009, 0x00000001, 0x00000006,
    0x00000002, 0x00000001,
};

s32 _fall_char_last[] = {
    0x0001003E, (s32)fall_data2, 0x00010005, 0x00000013,
    0x0001003F, 0xFE363C81, 0x0006005B, (s32)evt_mapobj_trans,
    0x00000001, 0xFE363C80, 0x00000000, 0xFE363C81,
    0x00000000, 0x00010009, 0x00000001, 0x00000006,
    0x00000002, 0x00000001,
};

s32 fall_gameover[] = {
    0x00020032, 0xFE363C80, (s32)(char*)str_g_80420448, 0x0001005C,
    (s32)_fall_char, 0x0001000A, 0x000000A6, 0x00020032,
    0xFE363C80, (s32)(char*)str_a_8042044c, 0x0001005C, (s32)_fall_char,
    0x0001000A, 0x0000014D, 0x00020032, 0xFE363C80,
    (s32)(char*)str_m_80420450, 0x0001005C, (s32)_fall_char, 0x0001000A,
    0x000000A6, 0x00020032, 0xFE363C80, (s32)(char*)str_e_80420454,
    0x0001005C, (s32)_fall_char, 0x0001000A, 0x000000A6,
    0x00020032, 0xFE363C80, (s32)(char*)str_o_80420458, 0x0001005C,
    (s32)_fall_char, 0x0001000A, 0x000000A6, 0x00020032,
    0xFE363C80, (s32)(char*)str_v_8042045c, 0x0001005C, (s32)_fall_char,
    0x0001000A, 0x000000A6, 0x00020032, 0xFE363C80,
    (s32)(char*)str_e2_80420460, 0x0001005C, (s32)_fall_char, 0x0001000A,
    0x000001F4, 0x00020032, 0xFE363C80, (s32)(char*)str_r_80420464,
    0x0002005D, (s32)_fall_char_last, 0xFE363C8A, 0x00010005,
    0x00000000, 0x0002006A, 0xFE363C8A, 0xFE363C8B,
    0x00020018, 0xFE363C8B, 0x00000000, 0x00000007,
    0x00000021, 0x00010009, 0x00000001, 0x00000006,
    0x0001000A, 0x000003E8, 0x00000002, 0x00000001,
};

s32 unk_evt_8030c674[] = {
    0x00020032, 0xFE363C80, (s32)(char*)str_f_80420468, 0x0001005C,
    (s32)_fall_char, 0x0001000A, 0x00000085, 0x00020032,
    0xFE363C80, (s32)(char*)str_i_8042046c, 0x0001005C, (s32)_fall_char,
    0x0001000A, 0x000000E9, 0x00020032, 0xFE363C80,
    (s32)(char*)str_n_80420470, 0x0001005C, (s32)_fall_char, 0x0001000A,
    0x00000085, 0x00020032, 0xFE363C80, (s32)(char*)str_d_80420474,
    0x0001005C, (s32)_fall_char, 0x0001000A, 0x00000085,
    0x00020032, 0xFE363C80, (s32)(char*)str_e3_80420478, 0x0001005C,
    (s32)_fall_char, 0x0001000A, 0x00000085, 0x00020032,
    0xFE363C80, (s32)(char*)str_l_8042047c, 0x0001005C, (s32)_fall_char,
    0x0001000A, 0x000000E9, 0x00020032, 0xFE363C80,
    (s32)(char*)str_j_80420480, 0x0001005C, (s32)_fall_char, 0x0001000A,
    0x00000085, 0x00020032, 0xFE363C80, (s32)(char*)str_u_80420484,
    0x0001005C, (s32)_fall_char, 0x0001000A, 0x00000085,
    0x00020032, 0xFE363C80, (s32)(char*)str_e4_80420488, 0x0001005C,
    (s32)_fall_char, 0x0001000A, 0x00000085, 0x00020032,
    0xFE363C80, (s32)(char*)str_g1_8042048c, 0x0001005C, (s32)_fall_char,
    0x0001000A, 0x00000190, 0x00020032, 0xFE363C80,
    (s32)(char*)str_o289_80420490, 0x0002005D, (s32)_fall_char_last, 0xFE363C8A,
    0x00010005, 0x00000000, 0x0002006A, 0xFE363C8A,
    0xFE363C8B, 0x00020018, 0xFE363C8B, 0x00000000,
    0x00000007, 0x00000021, 0x00010009, 0x00000001,
    0x00000006, 0x0001000A, 0x000003E8, 0x00000002,
    0x00000001,
};

s32 evt_gameover[] = {
    0x0002005B, (s32)evt_mario_key_onoff, 0x00000000, 0x0001005B,
    (s32)evt_mario_normalize, 0x0002005B, (s32)evt_mario_cont_onoff, 0x00000000,
    0x0004005B, (s32)evt_mario_set_pos, 0x00000000, 0x00000000,
    0x00000000, 0x0001005B, (s32)evt_cam_road_reset, 0x0003005B,
    (s32)evt_party_cont_onoff, 0x00000000, 0x00000000, 0x0005005B,
    (s32)evt_party_set_pos, 0x00000000, 0x00000000, 0xFFFFFC18,
    0x00000000, 0x0002005B, (s32)evt_mario_kill_party, 0x00000000,
    0x0003005B, (s32)evt_party_cont_onoff, 0x00000001, 0x00000000,
    0x0005005B, (s32)evt_party_set_pos, 0x00000001, 0x00000000,
    0xFFFFFC18, 0x00000000, 0x0002005B, (s32)evt_mario_clear_party,
    0x00000001, 0x00010009, 0x00000001, 0x0001005B,
    (s32)N_evt_snd_bgm_unpause, 0x0003005B, (s32)evt_mario_dispflag_onoff, 0x00000001,
    0x00000002, 0x0002005B, (s32)evt_mario_set_pose, (s32)(char*)str_M_D_2_80420498,
    0x0004005B, (s32)evt_mario_set_dir, 0x000000B4, 0x00000000,
    0x00000000, 0x0001005B, (s32)evt_mario_adjust_dir, 0x0002005B,
    (s32)evt_sub_get_language, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000004, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_g_80420448, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_a_8042044c,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_m_80420450, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_e_80420454,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_o_80420458, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_v_8042045c,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_e2_80420460, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_r_80420464,
    0x00000001, 0x0006005B, (s32)evt_mapobj_trans, 0x00000001,
    (s32)(char*)str_f_80420468, 0x00000000, 0x000000C8, 0x00000000,
    0x0006005B, (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_i_8042046c,
    0x00000000, 0x000000C8, 0x00000000, 0x0006005B,
    (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_n_80420470, 0x00000000,
    0x000000C8, 0x00000000, 0x0006005B, (s32)evt_mapobj_trans,
    0x00000001, (s32)(char*)str_d_80420474, 0x00000000, 0x000000C8,
    0x00000000, 0x0006005B, (s32)evt_mapobj_trans, 0x00000001,
    (s32)(char*)str_e3_80420478, 0x00000000, 0x000000C8, 0x00000000,
    0x0006005B, (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_l_8042047c,
    0x00000000, 0x000000C8, 0x00000000, 0x0006005B,
    (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_j_80420480, 0x00000000,
    0x000000C8, 0x00000000, 0x0006005B, (s32)evt_mapobj_trans,
    0x00000001, (s32)(char*)str_u_80420484, 0x00000000, 0x000000C8,
    0x00000000, 0x0006005B, (s32)evt_mapobj_trans, 0x00000001,
    (s32)(char*)str_e4_80420488, 0x00000000, 0x000000C8, 0x00000000,
    0x0006005B, (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_g1_8042048c,
    0x00000000, 0x000000C8, 0x00000000, 0x0006005B,
    (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_o289_80420490, 0x00000000,
    0x000000C8, 0x00000000, 0x00000020, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_f_80420468,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_i_8042046c, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_n_80420470,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_d_80420474, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_e3_80420478,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_l_8042047c, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_j_80420480,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_u_80420484, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_e4_80420488,
    0x00000001, 0x0005005B, (s32)evt_mapobj_flag_onoff, 0x00000001,
    0x00000001, (s32)(char*)str_g1_8042048c, 0x00000001, 0x0005005B,
    (s32)evt_mapobj_flag_onoff, 0x00000001, 0x00000001, (s32)(char*)str_o289_80420490,
    0x00000001, 0x0006005B, (s32)evt_mapobj_trans, 0x00000001,
    (s32)(char*)str_g_80420448, 0x00000000, 0x000000C8, 0x00000000,
    0x0006005B, (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_a_8042044c,
    0x00000000, 0x000000C8, 0x00000000, 0x0006005B,
    (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_m_80420450, 0x00000000,
    0x000000C8, 0x00000000, 0x0006005B, (s32)evt_mapobj_trans,
    0x00000001, (s32)(char*)str_e_80420454, 0x00000000, 0x000000C8,
    0x00000000, 0x0006005B, (s32)evt_mapobj_trans, 0x00000001,
    (s32)(char*)str_o_80420458, 0x00000000, 0x000000C8, 0x00000000,
    0x0006005B, (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_v_8042045c,
    0x00000000, 0x000000C8, 0x00000000, 0x0006005B,
    (s32)evt_mapobj_trans, 0x00000001, (s32)(char*)str_e2_80420460, 0x00000000,
    0x000000C8, 0x00000000, 0x0006005B, (s32)evt_mapobj_trans,
    0x00000001, (s32)(char*)str_r_80420464, 0x00000000, 0x000000C8,
    0x00000000, 0x00000021, 0x0003005B, (s32)evt_cam_ctrl_onoff,
    0x00000004, 0x00000001, 0x0009005B, (s32)evt_cam3d_evt_set,
    0x00000000, 0x000000D0, 0x0000028D, 0x00000000,
    0x0000005B, 0xFFFFFFE2, 0x00000000, 0x0000000B,
    0x00010009, 0x00000001, 0x0004005B, (s32)evt_map_playanim,
    (s32)(char*)str_anm_hokori_802c2c00, 0x00000001, 0x00000000, 0x0002005B,
    (s32)evt_sub_get_language, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000004, 0x0001005E, (s32)unk_evt_8030c674, 0x00000020,
    0x0001005E, (s32)fall_gameover, 0x00000021, 0x00000002,
    0x00000001,
};

void seq_gameOverInit(void) {
    winReInit();
    L_camDispOn(4);
    psndStopAllFadeOut();
    psndSetReverb(0);
    L_psndBGM_stop();
    psndClearFlag(0x80);
    psndClearFlag(0x20);
    psndClearFlag(0x40);
    N_pouchUnEquipBadgeID(0x143);
    marioStSystemLevel(4);
}

void seq_gameOverExit(void) {
    psndStopAllFadeOut();
    marioStSystemLevel(0);
}

s32 N_evt_snd_bgm_unpause(void) {
    s32 id = *(s32*)(psbgm + 4);
    if ((u32)(id + 0x10000) != 0xFFFF) {
        u16* table = *(u16**)(sound + 0x100);
        table[id * 0x9C] &= 0x7FFF;
    }
    return 2;
}

static void* wp;

#pragma use_lmw_stmw off
void seq_gameOverMain(void* param_1, s32 param_2, s32 param_3, s32 param_4, u32 param_5, u32 param_6) {
    extern void fadeEntry(s32 type, s32 duration, void* color);
    extern s32 fadeIsFinish(void);
    extern s32 mapPreLoad(char* map);
    extern void mapLoad(char* map);
    extern void _relUnLoad(void);
    extern void _unload(char* map, char* bero, s32 unk);
    extern char* strcpy(char* dst, const char* src);
    extern void* camGetPtr(s32 camId);
    extern void npcClearDeadInfo(void);
    extern void nameEntReInit(void);
    extern void* marioGetPtr(void);
    extern void marioReset(void);
    extern void marioEntry(void);
    extern void psndBGMOn(u32 id, char* flag);
    extern void psndBGMOff_f_d(u32 id, u32 fade, s32 mode);
    extern void* evtEntryType(void* code, s8 priority, u32 flags, u32 type);
    extern s32 evtCheckID(s32 id);
    extern u32 keyGetButtonTrg(s32 pad);
    extern void seqSetSeq(s32 seq, char* map, char* bero);
    extern u32 dat_804204a0;
    extern u32 dat_804204a4;
    extern u32 dat_804204a8;
    extern u32 dat_804204ac;
    extern f32 float_0_804204b0;
    extern f32 float_0p01_804204b4;
    extern u32 vec3_802c2c0c[3];
    extern char str_sys_00_802c2c18[];
    extern char zero_804204b8;
    extern void* wp;
    extern void* gp;
    void* gpLocal;
    void* cam;
    void* mario;
    void* evt;
    s32 bgmId;
    u16* bgmTable;

    switch (*(s32*)((s32)param_1 + 4)) {
        case 0: {
            u32 color;
            if (*(void**)((s32)param_1 + 8) == 0) {
                color = dat_804204a0;
                fadeEntry(2, 1000, &color);
                *(s32*)((s32)gp + 0x110) = 10;
                *(s32*)((s32)gp + 0x114) = 300;
            }
            mapPreLoad(str_sys_00_802c2c18);
            *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            break;
        }
        case 1: {
            f32 zero;
            if (fadeIsFinish() != 0) {
                _relUnLoad();
                gpLocal = gp;
                _unload((char*)((s32)gpLocal + 0x12C), 0, 0);
                gpLocal = gp;
                strcpy((char*)((s32)gpLocal + 0x14C), (char*)((s32)gpLocal + 0x12C));
                statusWinForceOff();
                statusWinDispOff();
                cam = camGetPtr(4);
                zero = float_0_804204b0;
                *(f32*)((s32)cam + 0xFC) = zero;
                *(f32*)((s32)cam + 0x100) = zero;
                *(f32*)((s32)cam + 0x104) = (f32)*(u16*)((s32)gp + 0x170);
                *(f32*)((s32)cam + 0x108) = (f32)*(u16*)((s32)gp + 0x172);
                cam = camGetPtr(8);
                zero = float_0_804204b0;
                *(f32*)((s32)cam + 0xFC) = zero;
                *(f32*)((s32)cam + 0x100) = zero;
                *(f32*)((s32)cam + 0x104) = (f32)*(u16*)((s32)gp + 0x170);
                *(f32*)((s32)cam + 0x108) = (f32)*(u16*)((s32)gp + 0x172);
                cam = camGetPtr(8);
                *(u16*)cam &= ~0x100;
                cam = camGetPtr(8);
                *(u16*)cam |= 0x400;
                cam = camGetPtr(8);
                *(u16*)cam |= 0x200;
                fadeTecOff();
                fadeSoftFocusOff();
                npcClearDeadInfo();
                countDownEnd();
                nameEntReInit();
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        }
        case 2:
            if (mapPreLoad(str_sys_00_802c2c18) != 0) {
                mapLoad(str_sys_00_802c2c18);
                cam = camGetPtr(4);
                mario = marioGetPtr();
                *(f32*)((s32)cam + 0x94) = float_0_804204b0;
                *(f32*)((s32)cam + 0x98) = float_0p01_804204b4;
                *(f32*)((s32)cam + 0x9C) = float_0_804204b0;
                *(u32*)((s32)cam + 0xAC) = *(u32*)((s32)cam + 0x94);
                *(u32*)((s32)cam + 0xB0) = *(u32*)((s32)cam + 0x98);
                *(u32*)((s32)cam + 0xB4) = *(u32*)((s32)cam + 0x9C);
                *(u32*)((s32)mario + 0x8C) = vec3_802c2c0c[0];
                *(u32*)((s32)mario + 0x90) = vec3_802c2c0c[1];
                *(u32*)((s32)mario + 0x94) = vec3_802c2c0c[2];
                *(u32*)((s32)mario + 0xEC) = *(u32*)((s32)mario + 0x8C);
                *(u32*)((s32)mario + 0xF0) = *(u32*)((s32)mario + 0x90);
                *(u32*)((s32)mario + 0xF4) = *(u32*)((s32)mario + 0x94);
                camRoadReset();
                camShiftReset();
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        case 3: {
            u32 color;
            color = dat_804204a4;
            fadeEntry(0x1F, 200, &color);
            *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            break;
        }
        case 4: {
            u32 color;
            if (fadeIsFinish() != 0) {
                color = dat_804204a8;
                fadeEntry(9, 1000, &color);
                marioReset();
                marioEntry();
                psndBGMOn(0x200, (char*)0x93);
                bgmId = *(s32*)(psbgm + 4);
                if ((u32)(bgmId + 0x10000) != 0xFFFF) {
                    bgmTable = *(u16**)(sound + 0x100);
                    *(u16*)((u8*)bgmTable + bgmId * 0x138) |= 0x8000;
                }
                evt = evtEntryType(evt_gameover, 0, 0, 0);
                *(void**)wp = evt;
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        }
        case 5: {
            u32 color;
            if (evtCheckID(*(s32*)((s32)*(void**)wp + 0x15C)) == 0 || (keyGetButtonTrg(0) & 0x1100) != 0) {
                color = dat_804204ac;
                fadeEntry(0x24, 0, &color);
                psndBGMOff_f_d(0x200, 4000, 0);
                *(s32*)((s32)param_1 + 4) = *(s32*)((s32)param_1 + 4) + 1;
            }
            break;
        }
        case 6:
            if (fadeIsFinish() != 0) {
                _relUnLoad();
                _unload(str_sys_00_802c2c18, 0, 0);
                strcpy((char*)((s32)gp + 0x11C), &zero_804204b8);
                strcpy((char*)((s32)gp + 0x12C), &zero_804204b8);
                strcpy((char*)((s32)gp + 0x13C), &zero_804204b8);
                statusWinDispOn();
                cam = camGetPtr(8);
                *(u16*)cam &= ~0x200;
                if ((*(u32*)gp & 0x1000) == 0) {
                    seqSetSeq(1, 0, 0);
                } else {
                    seqSetSeq(7, 0, 0);
                }
            }
            break;
    }
}
#pragma use_lmw_stmw on

const char str_anm_hokori_802c2c00[] = "anm_hokori";
const u32 vec3_802c2c0c[3] = { 0, 0, 0 };
const char str_sys_00_802c2c18[] = "sys_00";

extern char str_n_804204c0[];
extern char str_ne_804204c4[];
extern char str_e_804204c8[];
extern char str_se_804204cc[];
extern char str_s_804204d0[];
extern char str_sw_804204d4[];
extern char str_w_804204d8[];
extern char str_nw_804204dc[];

const char* const lbl_802C2C28[] = {
    str_n_804204c0,
    str_ne_804204c4,
    str_e_804204c8,
    str_se_804204cc,
    str_s_804204d0,
    str_sw_804204d4,
    str_w_804204d8,
    str_nw_804204dc,
};

static void* work;
static void* wp = &work;

const char str_g_80420448[] = "g";
const char str_a_8042044c[] = "a";
const char str_m_80420450[] = "m";
const char str_e_80420454[] = "e";
const char str_o_80420458[] = "o";
const char str_v_8042045c[] = "v";
const char str_e2_80420460[] = "e2";
const char str_r_80420464[] = "r";
const char str_f_80420468[] = "f";
const char str_i_8042046c[] = "i";
const char str_n_80420470[] = "n";
const char str_d_80420474[] = "d";
const char str_e3_80420478[] = "e3";
const char str_l_8042047c[] = "l";
const char str_j_80420480[] = "j";
const char str_u_80420484[] = "u";
const char str_e4_80420488[] = "e4";
const char str_g1_8042048c[] = "g1";
const char str_o289_80420490[] = "o289";
const char str_M_D_2_80420498[] = "M_D_2";
const u32 dat_804204a0 = 0xFF;
const u32 dat_804204a4 = 0xFF;
const u32 dat_804204a8 = 0xFF;
const u32 dat_804204ac = 0xFF;
const f32 float_0_804204b0 = 0.0f;
const f32 float_0p01_804204b4 = 0.01f;
const char zero_804204b8 = '\0';
