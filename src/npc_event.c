#include "npc_event.h"
#include "event/evt_cmd.h"
#include "effect/eff_fire.h"
#include "effect/n64/eff_magic1_n64.h"

extern const f32 float_0_80421f8c;
extern const f32 float_5_80421f84;
extern const f32 float_1_80421f88;
extern const f32 float_neg1_80421f90;
extern char* strcpy(char* dst, const char* src);
extern const f32 float_30_80421fb4;
extern s32 strcmp(const char* a, const char* b);
extern char str_kemuri_test_802ed438[];
extern const f32 float_neg75_80421fa0;
const char str_me_80421f30[] = "me";

/* Recovered target .rodata ASCII string prefix. */
const char str_SFX_ENEMY_DIE1_802ed444[] = "SFX_ENEMY_DIE1";
const char str_SFX_FLD_ENM_NOKO_MOV_802ed454[] = "SFX_FLD_ENM_NOKO_MOVE1";
const char str_NKT_A_1_802ed46c[] = "NKT_A_1";
const char str_SFX_FLD_ENM_NOKO_SHE_802ed474[] = "SFX_FLD_ENM_NOKO_SHELL1";
const char str_PTP_A_5_802ed48c[] = "PTP_A_5";
const char str_SFX_FLD_ENM_PATA_MOV_802ed494[] = "SFX_FLD_ENM_PATA_MOVE2";
const char str_SFX_FLD_ENM_PATA_MOV_802ed4ac[] = "SFX_FLD_ENM_PATA_MOVE3";
const char str_MET_S_1_802ed4c4[] = "MET_S_1";
const char str_TMT_S_1_802ed4cc[] = "TMT_S_1";
const char str_SMT_S_1_802ed4d4[] = "SMT_S_1";
const char str_STM_S_1_802ed4dc[] = "STM_S_1";
const char str_MET_W_1_802ed4e4[] = "MET_W_1";
const char str_TMT_W_1_802ed4ec[] = "TMT_W_1";
const char str_SMT_W_1_802ed4f4[] = "SMT_W_1";
const char str_STM_W_1_802ed4fc[] = "STM_W_1";
const char str_SFX_FLD_ENM_MET_ATTA_802ed504[] = "SFX_FLD_ENM_MET_ATTACK1";
const char str_MET_A_1_802ed51c[] = "MET_A_1";
const char str_TMT_A_1_802ed524[] = "TMT_A_1";
const char str_SMT_A_1_802ed52c[] = "SMT_A_1";
const char str_STM_A_1_802ed534[] = "STM_A_1";
const char str_SFX_FLD_ENM_PATAMET__802ed53c[] = "SFX_FLD_ENM_PATAMET_MOVE2";
const char str_SFX_FLD_ENM_PANZI_MO_802ed558[] = "SFX_FLD_ENM_PANZI_MOVE2";
const char str_PAN_S_1_802ed570[] = "PAN_S_1";
const char str_PAN_W_1_802ed578[] = "PAN_W_1";
const char str_PAN_S_1_K_802ed580[] = "PAN_S_1_K";
const char str_PAN_W_1_K_802ed58c[] = "PAN_W_1_K";
const char str_slave_0_802ed598[] = "slave_0";
const char str_KRN_H_1_802ed5a0[] = "KRN_H_1";
const char str_KRN_A_1_802ed5a8[] = "KRN_A_1";
const char str_SFX_FLD_ENM_KARON_BO_802ed5b0[] = "SFX_FLD_ENM_KARON_BONE_THROW1";
const char str_KRN_A_2_802ed5d0[] = "KRN_A_2";
const char str_KRN_S_1_802ed5d8[] = "KRN_S_1";
const char str_KRN_Z_2_802ed5e0[] = "KRN_Z_2";
const char str_EVENT_HONENOKO_MOVE_802ed5e8[] = "[EVENT] HONENOKO MOVE\n";
const char str_KRN_D_2_802ed600[] = "KRN_D_2";
const char str_EVENT_HONENOKO_FIND_802ed608[] = "[EVENT] HONENOKO FIND\n";
const char str_KRN_D_3_802ed620[] = "KRN_D_3";
const char str_KRN_D_4_802ed628[] = "KRN_D_4";
const char str_EVENT_HONENOKO_LOST_802ed630[] = "[EVENT] HONENOKO LOST\n";
const char str_EVENT_HONENOKO_RETUR_802ed648[] = "[EVENT] HONENOKO RETURN\n";
const char str_KRN_W_1_802ed664[] = "KRN_W_1";
const char str_KTA_S_1_802ed66c[] = "KTA_S_1";
const char str_KTA_A_1_802ed674[] = "KTA_A_1";
const char str_SFX_FLD_ENM_KILLER_C_802ed67c[] = "SFX_FLD_ENM_KILLER_CANNON1";
const char str_KIL_S_1_802ed698[] = "KIL_S_1";
const char str_SNB_S_1_802ed6a0[] = "SNB_S_1";
const char str_SIS_Z_3_802ed6a8[] = "SIS_Z_3";
const char str_SIS_S_1_802ed6b0[] = "SIS_S_1";
const char str_SIS_A_1a_802ed6b8[] = "SIS_A_1a";
const char str_SIS_W_1_802ed6c4[] = "SIS_W_1";
const char str_SIN_Z_3_802ed6cc[] = "SIN_Z_3";
const char str_SIN_S_1_802ed6d4[] = "SIN_S_1";
const char str_SIN_A_1a_802ed6dc[] = "SIN_A_1a";
const char str_SIN_W_1_802ed6e8[] = "SIN_W_1";
const char str_c_baria_802ed6f0[] = "c_baria";
const char str_SFX_FLD_ENM_BARRI_AT_802ed6f8[] = "SFX_FLD_ENM_BARRI_ATTACK1";
const char str_SFX_FLD_ENM_BARRI_AT_802ed714[] = "SFX_FLD_ENM_BARRI_ATTACK2";
const u32 vec3_802ed730[3] = {0, 0, 0};
const u32 vec3_802ed73c[3] = {0, 0, 0};
const char str_PAI_S_1_802ed748[] = "PAI_S_1";
const char str_SFX_FLD_ENM_PAIDA_MO_802ed750[] = "SFX_FLD_ENM_PAIDA_MOVE1";
const char str_SFX_FLD_ENM_PAIDA_MO_802ed768[] = "SFX_FLD_ENM_PAIDA_MOVE2";
const char str_SFX_FLD_ENM_PAKKUN_A_802ed780[] = "SFX_FLD_ENM_PAKKUN_ATTACK1";
const char str_PKF_A_1A_802ed79c[] = "PKF_A_1A";
const char str_PKF_A_1B_802ed7a8[] = "PKF_A_1B";
const char str_PKF_A_1C_802ed7b4[] = "PKF_A_1C";
const char str_c_pakflwr_802ed7c0[] = "c_pakflwr";

/* Forward declarations for recovered npc_event event bytecode. */
extern u32 enemy_common_dead_event[];
extern u32 enemy_event_dead_event_sub[];
extern u32 enemy_common_blow_event[];
extern u32 enemy_2d_common_dead_event[];
extern u32 _2d_dead_rotate_event[];
extern u32 unk_evt_8033f9b0[];
extern u32 npc_urouro_move_event[];
extern u32 npc_urouro_return_event[];
extern u32 kuriboo_move_event[];
extern u32 kuriboo_find_event[];
extern u32 kuriboo_lost_event[];
extern u32 kuriboo_return_event[];
extern u32 patakuri_move_event[];
extern u32 patakuri_find_event[];
extern u32 patakuri_lost_event[];
extern u32 patakuri_return_event[];
extern u32 nokonoko_shell_return_event[];
extern u32 nokonoko_init_event[];
extern u32 nokonoko_move_event[];
extern u32 nokonoko_find_event[];
extern u32 nokonoko_lost_event[];
extern u32 nokonoko_return_event[];
extern u32 togenoko_shell_return_event[];
extern u32 togenoko_init_event[];
extern u32 togenoko_move_event[];
extern u32 togenoko_find_event[];
extern u32 togenoko_lost_event[];
extern u32 togenoko_return_event[];
extern u32 patapata_move_event[];
extern u32 patapata_find_event[];
extern u32 patapata_lost_event[];
extern u32 patapata_return_event[];
extern u32 met_set_anim_wait_event[];
extern u32 met_set_anim_walk_event[];
extern u32 met_init_event[];
extern u32 met_init_event_ceil[];
extern u32 togemet_init_event[];
extern u32 togemet_init_event_ceil[];
extern u32 met_move_event[];
extern u32 met_find_event[];
extern u32 met_lost_event[];
extern u32 met_return_event[];
extern u32 patamet_move_event[];
extern u32 patamet_find_event[];
extern u32 patamet_lost_event[];
extern u32 patamet_return_event[];
extern u32 chorobon_move_event[];
extern u32 chorobon_find_event[];
extern u32 chorobon_lost_event[];
extern u32 chorobon_return_event[];
extern u32 pansy_move_event[];
extern u32 pansy_find_event[];
extern u32 pansy_lost_event[];
extern u32 pansy_return_event[];
extern u32 twinkling_pansy_move_event[];
extern u32 twinkling_pansy_find_event[];
extern u32 twinkling_pansy_lost_event[];
extern u32 twinkling_pansy_return_event[];
extern u32 karon_init_event[];
extern u32 karon_move_event[];
extern u32 karon_find_event[];
extern u32 karon_lost_event[];
extern u32 karon_return_event[];
extern u32 karon_blow_event[];
extern u32 honenoko2_init_event[];
extern u32 honenoko2_move_event[];
extern u32 honenoko2_find_event[];
extern u32 honenoko2_lost_event[];
extern u32 honenoko2_return_event[];
extern u32 killer_init_event[];
extern u32 killer_move_event[];
extern u32 killer_dead_event[];
extern u32 killer_cannon_init_event[];
extern u32 killer_cannon_move_event[];
extern u32 sambo_move_event[];
extern u32 sambo_find_event[];
extern u32 sambo_lost_event[];
extern u32 sambo_return_event[];
extern u32 sinnosuke_init_event[];
extern u32 sinnosuke_move_event[];
extern u32 sinnosuke_find_event[];
extern u32 sinnosuke_lost_event[];
extern u32 sinnosuke_return_event[];
extern u32 sinemon_init_event[];
extern u32 sinemon_move_event[];
extern u32 sinemon_find_event[];
extern u32 sinemon_lost_event[];
extern u32 sinemon_return_event[];
extern u32 togedaruma_move_event[];
extern u32 togedaruma_find_event[];
extern u32 togedaruma_lost_event[];
extern u32 togedaruma_return_event[];
extern u32 barriern_init_event[];
extern u32 barriern_move_event[];
extern u32 barriern2_move_event[];
extern u32 barriern_dead_event[];
extern u32 barriern_find_event[];
extern u32 barriern_lost_event[];
extern u32 barriern_return_event[];
extern u32 barriern_blow_event[];
extern u32 piders_move_event[];
extern u32 piders_find_event[];
extern u32 piders_blow_event[];
extern u32 pakkun_attack_event[];
extern u32 pakkun_init_event[];
extern u32 pakkun_move_event[];
extern u32 pakkun_find_event[];
extern u32 pakkun_return_event[];
extern u32 dokugassun_init_event[];
extern u32 dokugassun_move_event[];
extern u32 dokugassun_find_event[];
extern u32 dokugassun_lost_event[];
extern u32 dokugassun_return_event[];
extern u32 basabasa_init_event[];
extern u32 basabasa_move_event[];
extern u32 basabasa_find_event[];
extern u32 basabasa_lost_event[];
extern u32 basabasa_return_event[];
extern u32 basabasa2_move_event[];
extern u32 basabasa2_find_event[];
extern u32 basabasa2_lost_event[];
extern u32 basabasa2_return_event[];
extern u32 teresa_move_event[];
extern u32 teresa_find_event[];
extern u32 teresa_lost_event[];
extern u32 teresa_return_event[];
extern u32 bubble_move_event[];
extern u32 bubble_find_event[];
extern u32 bubble_lost_event[];
extern u32 bubble_return_event[];
extern u32 hbom_move_event[];
extern u32 hbom_find_event[];
extern u32 hbom_lost_event[];
extern u32 hbom_return_event[];
extern u32 zakowiz_init_event[];
extern u32 zakowiz_move_event[];
extern u32 zakowiz_find_event[];
extern u32 zakowiz_lost_event[];
extern u32 zakowiz_return_event[];
extern u32 zakowiz_blow_event[];
extern u32 hannya_move_event[];
extern u32 hannya_find_event[];
extern u32 hannya_lost_event[];
extern u32 hannya_return_event[];
extern u32 mahoon_init_event[];
extern u32 mahoon_move_event[];
extern u32 mahoon_dead_event[];
extern u32 mahoon_find_event[];
extern u32 mahoon_lost_event[];
extern u32 mahoon_return_event[];
extern u32 kamec_init_event[];
extern u32 kamec_move_event[];
extern u32 kamec_find_event[];
extern u32 kamec_lost_event[];
extern u32 kamec_return_event[];
extern u32 kamec_blow_event[];
extern u32 kamec2_init_event[];
extern u32 kamec2_move_event[];
extern u32 kamec2_find_event[];
extern u32 kamec2_lost_event[];
extern u32 kamec2_return_event[];
extern u32 kamec2_blow_event[];
extern u32 hbross_init_event[];
extern u32 hbross_move_event[];
extern u32 hbross_find_event[];
extern u32 hbross_lost_event[];
extern u32 hbross_return_event[];
extern u32 hbross_blow_event[];
extern u32 wanwan_init_event[];
extern u32 wanwan_init_event_sub[];
extern u32 wanwan_move_event[];
extern u32 wanwan_find_event[];
extern u32 kuriboo2D_init_event[];
extern u32 kuriboo2D_move_event[];
extern u32 kuriboo2D_find_event[];
extern u32 zako2D_init_event[];
extern u32 zako2D_move_event[];
extern u32 zako2D_find_event[];
extern u32 dokan2D_init_event[];
extern u32 dokan2D_regl_event[];
extern u32 fall2D_init_event[];
extern u32 fall2D_regl_event[];
extern u32 zakoM2D_init_event[];
extern u32 zakoM2D_move_event[];
extern u32 zakoM2D_find_event[];
extern u32 unk_evt_8034bdb4[];
extern u32 gesso2D_init_event[];
extern u32 gesso2D_move_event[];
extern u32 enemy_event_dead_event[];
extern u32 enemy_slave_common_dead_event[];
extern u32 enemy_common_flag_init_event[];
extern u32 testenemynpc_init_event[];
extern u32 npc_urouro_init_event[];
extern u32 kuriboo_init_event[];
extern u32 patakuri_init_event[];
extern u32 patapata_init_event[];
extern u32 patamet_init_event[];
extern u32 chorobon_init_event[];
extern u32 pansy_init_event[];
extern u32 twinkling_pansy_init_event[];
extern u32 karon_dead_event[];
extern u32 sambo_init_event[];
extern u32 togedaruma_init_event[];
extern u32 piders_init_event[];
extern u32 basabasa_dead_event[];
extern u32 basabasa2_init_event[];
extern u32 basabasa2_dead_event[];
extern u32 teresa_init_event[];
extern u32 teresa_dead_event[];
extern u32 bubble_init_event[];
extern u32 hbom_init_event[];
extern u32 zakowiz_dead_event[];
extern u32 hannya_init_event[];
extern u32 kamec_dead_event[];
extern u32 kamec2_dead_event[];
extern u32 hbross_dead_event[];
extern u32 zakoM2D_dead_event[];
extern u32 met_init_event_common[];
extern const char str_BRO_A_1A_802ed8e0[];
extern const char str_BRO_A_1B_802ed8cc[];
extern const char str_BRO_S_1_802ed8d8[];
extern const char str_BSA_S_2_802ed830[];
extern const char str_KMK_A_2A_802ed8b4[];
extern const char str_KMK_A_2B_802ed8c0[];
extern const char str_KMK_A_3A_802ed894[];
extern const char str_KMK_A_3B_802ed8a0[];
extern const char str_KMK_S_1_802ed8ac[];
extern const char str_KMK_S_2_802ed88c[];
extern const char str_KMK_Z_1_802ed884[];
extern const char str_MAH_S_2A_802ed860[];
extern const char str_PKF_E_1_802ed828[];
extern const char str_PKF_E_2_802ed804[];
extern const char str_PKF_S_1_802ed7e0[];
extern const char str_SFX_CONDITION_FIRE1_802ed9d8[];
extern const char str_SFX_ENM_3RD2_CRASH1_802ed9ec[];
extern const char str_SFX_FLD_ENM_HANNYA_M_802ed844[];
extern const char str_SFX_FLD_ENM_PAKKUN_M_802ed7e8[];
extern const char str_SFX_FLD_ENM_PAKKUN_M_802ed80c[];
extern const char str_SFX_FLD_ENM_WAN_ATTA_802ed984[];
extern const char str_SFX_FLD_ENM_WAN_LAND_802ed950[];
extern const char str_SFX_FLD_ENM_WAN_MOVE_802ed938[];
extern const char str_SFX_FLD_ENM_WAN_MOVE_802ed96c[];
extern const char str_SFX_KUPPA_GESSO_MOVE_802eda0c[];
extern const char str_WAN_A_1_802ed930[];
extern const char str_WAN_S_1A_802ed924[];
extern const char str_WAN_S_1B_802ed910[];
extern const char str_WAN_Z_2_802ed91c[];
extern const char str_c_zako_km_802ed9cc[];
extern const char str_c_zako_m_802ed838[];
extern const char str_glass_n64_802eda00[];
extern const char str_slave_1_802ed86c[];
extern const char str_slave_2_802ed874[];
extern const char str_slave_3_802ed87c[];
extern s32 evt_eff();
extern s32 evt_eff64();
extern s32 evt_eff_fukidashi();
extern s32 evt_fbat_trans_floor_position();
extern s32 evt_get_target_dir();
extern s32 evt_item_entry();
extern s32 evt_mario_get_mov_spd();
extern s32 evt_mario_get_pos();
extern s32 evt_npc_add_dirdist();
extern s32 evt_npc_add_rotate();
extern s32 evt_npc_blur_onoff();
extern s32 evt_npc_calc_score();
extern s32 evt_npc_change_interrupt();
extern s32 evt_npc_delete();
extern s32 evt_npc_facedirection_add();
extern s32 evt_npc_flag_onoff();
extern s32 evt_npc_get_btlsetup_work();
extern s32 evt_npc_get_dir();
extern s32 evt_npc_get_drop_coin();
extern s32 evt_npc_get_drop_fixitem();
extern s32 evt_npc_get_drop_flower();
extern s32 evt_npc_get_drop_heart();
extern s32 evt_npc_get_drop_item();
extern s32 evt_npc_get_home_position();
extern s32 evt_npc_get_kpencount_type();
extern s32 evt_npc_get_loiter_dir();
extern s32 evt_npc_get_position();
extern s32 evt_npc_get_unitwork();
extern s32 evt_npc_getback_item_entry();
extern s32 evt_npc_glide_position();
extern s32 evt_npc_homing_target();
extern s32 evt_npc_jump_position();
extern s32 evt_npc_move_position();
extern s32 evt_npc_pera_onoff();
extern s32 evt_npc_restart_regular_event();
extern s32 evt_npc_return_interrupt();
extern s32 evt_npc_reverse_ry();
extern s32 evt_npc_set_anim();
extern s32 evt_npc_set_attack_mode();
extern s32 evt_npc_set_btlsetup_work();
extern s32 evt_npc_set_color();
extern s32 evt_npc_set_confuse_anim();
extern s32 evt_npc_set_damage_anim();
extern s32 evt_npc_set_height();
extern s32 evt_npc_set_position();
extern s32 evt_npc_set_tribe();
extern s32 evt_npc_set_rotate();
extern s32 evt_npc_set_run_anim();
extern s32 evt_npc_set_ry();
extern s32 evt_npc_set_scale();
extern s32 evt_npc_set_stay_anim();
extern s32 evt_npc_set_talk_anim();
extern s32 evt_npc_set_unitwork();
extern s32 evt_npc_set_walk_anim();
extern s32 evt_npc_set_width();
extern s32 evt_npc_slave_entry();
extern s32 evt_npc_sound_data_reset();
extern s32 evt_npc_sound_data_set();
extern s32 evt_npc_status_check();
extern s32 evt_npc_status_onoff();
extern s32 evt_npc_wait_msec();
extern s32 evt_set_dir_to_home();
extern s32 evt_snd_sfxoff();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_random();
extern s32 unk_8004d180();
extern s32 urouro_init_func();
extern s32 urouro_main_func();

void* evtNpcNameToPtr(EventEntry* event, const void* name);
void* effNameToPtr(void* name);
void* camGetPtr(s32 camId);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(mahoon_get_groupname) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetValue(event, args[0], *(s32*)(*(s32*)((s32)npc + 0x28) + 4));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(npc_check_wall_stop) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    if (*(u32*)((s32)npc + 0x2FC) != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 _wait_blow_end(void* pEvt) {
    extern s32 npcCheckBlow(void* npc);
    extern f32 float_18_80421fe4;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 name = evtGetValue(pEvt, args[0]);
    void* npc = evtNpcNameToPtr(pEvt, (void*)name);

    if (npcCheckBlow(npc) != 0) {
        *(f32*)((s32)npc + 0xF0) += float_18_80421fe4;
        return 0;
    }
    *(f32*)((s32)npc + 0xF0) = float_0_80421f8c;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 pressCheck(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    extern s32 kpaGetLevel(void);
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 a;
    f32 b;
    f32 c;
    f32 radius = float_5_80421f84;
    f32 d;
    f32 e;
    f32 f;
    void* hit;

    if (kpaGetLevel() == 0) {
        evtSetValue((void*)param_1, args[0], 0);
        return 2;
    }
    hit = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                         *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                         *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                         float_neg1_80421f90, float_0_80421f8c,
                         &a, &b, &c, &radius, &d, &e, &f);
    if (hit != NULL) {
        evtSetValue((void*)param_1, args[0], 1);
    } else {
        evtSetValue((void*)param_1, args[0], 0);
    }
    return 2;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _2d_dead_jump(EventEntry* event, s32 first) {
    extern void* evtNpcNameToPtr(EventEntry* event, const void* name);
    extern void PSVECSubtract(void* a, void* b, void* out);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern double sin(double);
    extern double cos(double);
    extern f32 float_1000_80421fd0;
    extern f32 float_6p2832_80421fdc;
    extern f32 float_360_80421fe0;
    extern f32 float_0p5_80421fd4;
    extern f32 float_neg980_80421fd8;
    s32* args = event->args;
    s32 name = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 time = (f32)evtGetValue(event, args[4]) / float_1000_80421fd0;
    f32 speed = evtGetFloat(event, args[5]);
    f32 height = evtGetFloat(event, args[6]);
    f32 floorY = (f32)evtGetValue(event, args[7]);
    void* npc = evtNpcNameToPtr(event, (void*)name);
    f32 h = height;

    *(u32*)((s32)npc + 0x1D4) |= 0x10;
    *(u32*)((s32)npc + 0x1D4) |= 0x1000;

    if (first == 0) {
        goto update;
    }

    {
        f32 diff[3];
        f32 dy;

        *(u32*)npc |= 0x20000;
        *(u32*)npc &= 0xBFFFFFEF;
        *(s32*)((s32)npc + 0x18C) = 0;
        *(s32*)((s32)npc + 0x188) = 0;
        *(s32*)((s32)npc + 0x17C) = 0;
        *(s32*)((s32)npc + 0x178) = 0;
        *(s32*)((s32)npc + 0x108) = 0;
        *(f32*)((s32)npc + 0x164) = x;
        *(f32*)((s32)npc + 0x168) = y;
        *(f32*)((s32)npc + 0x16C) = z;
        *(u32*)((s32)npc + 0x158) = *(u32*)((s32)npc + 0x8C);
        *(u32*)((s32)npc + 0x15C) = *(u32*)((s32)npc + 0x90);
        *(u32*)((s32)npc + 0x160) = *(u32*)((s32)npc + 0x94);
        PSVECSubtract((void*)((s32)npc + 0x164), (void*)((s32)npc + 0x158), diff);
        *(f32*)((s32)npc + 0x1AC) = angleABf(float_0_80421f8c, float_0_80421f8c, diff[0], diff[2]);
        if (time <= float_0_80421f8c) {
            *(f32*)((s32)npc + 0x1B0) = speed;
        } else {
            f32 dx = x - *(f32*)((s32)npc + 0x8C);
            if (dx < float_0_80421f8c) {
                dx = -dx;
            }
            *(f32*)((s32)npc + 0x1B0) = dx / time;
        }
        *(f32*)((s32)npc + 0x1C0) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1B8) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1C4) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1C8) = float_1_80421f88;
        dy = *(f32*)((s32)npc + 0x168) - *(f32*)((s32)npc + 0x15C);
        if (height == float_0_80421f8c) {
            if (float_0_80421f8c <= dy) {
                return 1;
            }
        } else {
            if (float_0_80421f8c < dy) {
                h = height + dy;
            }
            *(f32*)((s32)npc + 0x1C4) = float_0_80421f8c;
            if (time == float_0_80421f8c) {
                *(f32*)((s32)npc + 0x1C4) = height;
            } else {
                f32 half = float_0p5_80421fd4 * time;
                f32 accel = -(time * float_neg980_80421fd8 * *(f32*)((s32)npc + 0x1CC) * time) / time;
                f32 denom;
                *(f32*)((s32)npc + 0x1C4) = accel;
                denom = accel * half + half * float_neg980_80421fd8 * *(f32*)((s32)npc + 0x1CC) * half;
                if (float_0_80421f8c != denom) {
                    *(f32*)((s32)npc + 0x1C8) = h / denom;
                }
            }
        }
        return 0;
    }

update:
    *(f32*)((s32)npc + 0x1B4) += *(f32*)((s32)npc + 0x1A4) * *(f32*)((s32)npc + 0x1B0) *
                                     (f32)sin((float_6p2832_80421fdc * *(f32*)((s32)npc + 0x1AC)) / float_360_80421fe0);
    *(f32*)((s32)npc + 0x1BC) += *(f32*)((s32)npc + 0x1A4) * *(f32*)((s32)npc + 0x1B0) *
                                     -(f32)cos((float_6p2832_80421fdc * *(f32*)((s32)npc + 0x1AC)) / float_360_80421fe0);
    if (*(f32*)((s32)npc + 0x90) < floorY) {
        *(f32*)((s32)npc + 0x1B0) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1C4) = float_0_80421f8c;
        *(f32*)((s32)npc + 0x1C8) = float_1_80421f88;
        *(f32*)((s32)npc + 0x1CC) = float_1_80421f88;
        return 1;
    }
    return 0;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _2d_get_dead_jump_offset(int param_1) {
    extern void* marioGetPtr(void);
    extern f32 float_20_80421fcc;
    s32* args = *(s32**)(param_1 + 0x18);
    s32 npcName = evtGetValue((void*)param_1, args[0]);
    void* npc = evtNpcNameToPtr((void*)param_1, (void*)npcName);
    s32 dst = args[1];
    void* mario = marioGetPtr();
    f32 dir;

    if (*(f32*)((s32)mario + 0x8C) == *(f32*)((s32)npc + 0x8C)) {
        dir = float_0_80421f8c;
    } else if (*(f32*)((s32)mario + 0x8C) > *(f32*)((s32)npc + 0x8C)) {
        dir = float_neg1_80421f90;
    } else {
        dir = float_1_80421f88;
    }
    evtSetValue((void*)param_1, dst, (s32)(float_20_80421fcc * dir));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 killer_make_name(void* pEvt) {
    extern s32 strlen(const char* str);
    static char name[32];
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* npc = evtNpcNameToPtr(pEvt, str_me_80421f30);
    s32 index = evtGetValue(pEvt, args[0]);
    s32 len;

    strcpy(name, (char*)((s32)npc + 8));
    len = strlen(name);
    name[len] = index + 0x31;
    name[len + 1] = 0;
    evtSetValue(pEvt, args[1], (s32)name);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(piders_get_height) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetFloat(event, args[0], *(f32*)((s32)npc + 0x210));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void piders_draw_yarn_sub(s32 unused, void* npc) {
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 comptype, s32 compsize, s32 frac);
    extern void GXSetNumChans(s32 nChans);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambsrc, s32 matsrc, s32 lightmask, s32 diff_fn, s32 attn_fn);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXSetNumTexGens(s32 nTexGens);
    extern void GXSetNumTevStages(s32 nStages);
    extern void GXSetTevColorOp(s32 tevstage, s32 tevop, s32 tevbias, s32 tevscale, s32 clamp, s32 tevregid);
    extern void GXSetTevColorIn(s32 tevstage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaOp(s32 tevstage, s32 tevop, s32 tevbias, s32 tevscale, s32 clamp, s32 tevregid);
    extern void GXSetTevAlphaIn(s32 tevstage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevOrder(s32 tevstage, s32 texcoord, s32 texmap, s32 color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);
    extern const u32 vec3_802ed730[3];
    extern const u32 vec3_802ed73c[3];
    extern u32 dat_80421f5c;
    u32 posA[3];
    u32 posB[3];
    u32 a[3];
    u32 b[3];
    u32 color;
    void* cam;

    cam = camGetPtr(4);
    posB[0] = vec3_802ed730[0];
    posB[1] = vec3_802ed730[1];
    posB[2] = vec3_802ed730[2];
    posA[0] = vec3_802ed73c[0];
    posA[1] = vec3_802ed73c[1];
    posA[2] = vec3_802ed73c[2];
    *(f32*)&posB[0] = *(f32*)((s32)npc + 0x1FC);
    *(f32*)&posB[1] = *(f32*)((s32)npc + 0x200) + float_30_80421fb4;
    *(f32*)&posB[2] = *(f32*)((s32)npc + 0x204);
    *(f32*)&posA[0] = *(f32*)((s32)npc + 0x8C);
    *(f32*)&posA[1] = *(f32*)((s32)npc + 0x90) + float_30_80421fb4;
    *(f32*)&posA[2] = *(f32*)((s32)npc + 0x94);
    b[0] = posB[0];
    b[1] = posB[1];
    b[2] = posB[2];
    a[0] = posA[0];
    a[1] = posA[1];
    a[2] = posA[2];

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    color = dat_80421f5c;
    GXSetChanMatColor(4, &color);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 7, 7, 5);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = *(f32*)&b[0] - float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&b[1];
    *(volatile f32*)0xCC008000 = *(f32*)&b[2] - float_5_80421f84;
    *(volatile f32*)0xCC008000 = *(f32*)&b[0] + float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&b[1];
    *(volatile f32*)0xCC008000 = *(f32*)&b[2] - float_5_80421f84;
    *(volatile f32*)0xCC008000 = *(f32*)&a[0] + float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&a[1];
    *(volatile f32*)0xCC008000 = *(f32*)&a[2] - float_5_80421f84;
    *(volatile f32*)0xCC008000 = *(f32*)&a[0] - float_1_80421f88;
    *(volatile f32*)0xCC008000 = *(f32*)&a[1];
    *(volatile f32*)0xCC008000 = *(f32*)&a[2] - float_5_80421f84;
}

s32 piders_draw_yawn(int param_1) {
    extern s32 dispEntry(s32 camera, s32 order, void* callback, void* data, f32 priority);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);

    dispEntry(4, 1, piders_draw_yarn_sub, npc, float_0_80421f8c);
    return 2;
}

s32 kamec_fire_magic(int param_1) {
    extern void effSetName(void* eff, void* name);
    s32* args = *(s32**)(param_1 + 0x18);
    s32 x = evtGetValue((void*)param_1, args[0]);
    s32 y = evtGetValue((void*)param_1, args[1]);
    s32 z = evtGetValue((void*)param_1, args[2]);
    s32 a = evtGetValue((void*)param_1, args[3]);
    s32 b = evtGetValue((void*)param_1, args[4]);
    s32 c = evtGetValue((void*)param_1, args[5]);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    void* eff = effMagic1N64Entry(0, 60, (f32)x, (f32)y, (f32)z, (f32)a, (f32)b, (f32)c);

    effSetName(eff, (void*)((s32)npc + 8));
    evtSetValue((void*)param_1, args[6], (s32)eff);
    return 2;
}

USER_FUNC(kamec_fire_magic_delete) {
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    void* eff = effNameToPtr((void*)((s32)npc + 8));
    if (eff != 0) {
        *(s32*)(*(s32*)((s32)eff + 0xC) + 0x30) = 1;
    }
    return 2;
}

s32 chain_main(EventEntry* event) {
    extern f32 float_0p25_80421fc0;
    extern f32 float_10_80421fc4;
    extern f32 float_300_80421fc8;
    extern double sqrt(double);
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    void* tail = *(void**)((s32)npc + 0x33C);
    f32 dx = *(f32*)((s32)npc + 0x8C) - *(f32*)((s32)tail + 0x8C);
    f32 dy = *(f32*)((s32)npc + 0x90) - *(f32*)((s32)tail + 0x90);
    f32 dz = *(f32*)((s32)npc + 0x94) - *(f32*)((s32)tail + 0x94);
    f32 len = (f32)sqrt(dx * dx + dy * dy + dz * dz);
    f32 step = len * float_0p25_80421fc0;
    f32 fall;
    s32 i;
    void* base;

    if (step < float_10_80421fc4) {
        step = float_10_80421fc4;
    }
    fall = float_300_80421fc8 / (step * step);

    {
        void* seg = *(void**)((s32)npc + 0x330);
        f32 x = *(f32*)((s32)seg + 0x8C);
        f32 y = *(f32*)((s32)seg + 0x90) - fall;
        f32 z = *(f32*)((s32)seg + 0x94);
        f32 vx = *(f32*)((s32)npc + 0x8C) - x;
        f32 vy = *(f32*)((s32)npc + 0x90) - y;
        f32 vz = *(f32*)((s32)npc + 0x94) - z;
        f32 dist = (f32)sqrt(vx * vx + vy * vy + vz * vz);
        f32 scale = float_1_80421f88 - step / dist;
        y += vy * scale;
        if (y < *(f32*)((s32)npc + 0x200)) {
            y = *(f32*)((s32)npc + 0x200);
        }
        *(f32*)((s32)seg + 0x8C) = x + vx * scale;
        *(f32*)((s32)seg + 0x90) = y;
        *(f32*)((s32)seg + 0x94) = z + vz * scale;
    }

    base = npc;
    for (i = 0; i < 2; i++, base = (void*)((s32)base + 4)) {
        void* prev = *(void**)((s32)base + 0x330);
        void* seg = *(void**)((s32)base + 0x334);
        f32 x = *(f32*)((s32)seg + 0x8C);
        f32 y = *(f32*)((s32)seg + 0x90) - fall;
        f32 z = *(f32*)((s32)seg + 0x94);
        f32 vx = *(f32*)((s32)prev + 0x8C) - x;
        f32 vy = *(f32*)((s32)prev + 0x90) - y;
        f32 vz = *(f32*)((s32)prev + 0x94) - z;
        f32 dist = (f32)sqrt(vx * vx + vy * vy + vz * vz);
        f32 scale = float_1_80421f88 - step / dist;
        y += vy * scale;
        if (y < *(f32*)((s32)npc + 0x200)) {
            y = *(f32*)((s32)npc + 0x200);
        }
        *(f32*)((s32)seg + 0x8C) = x + vx * scale;
        *(f32*)((s32)seg + 0x90) = y;
        *(f32*)((s32)seg + 0x94) = z + vz * scale;
    }

    base = (void*)((s32)npc + 0xC);
    for (i = 3; i > 0; i--, base = (void*)((s32)base - 4)) {
        void* seg = *(void**)((s32)base + 0x32C);
        void* prev = *(void**)((s32)base + 0x330);
        f32 x = *(f32*)((s32)seg + 0x8C);
        f32 y = *(f32*)((s32)seg + 0x90) - fall;
        f32 z = *(f32*)((s32)seg + 0x94);
        f32 vx = *(f32*)((s32)prev + 0x8C) - x;
        f32 vy = *(f32*)((s32)prev + 0x90) - y;
        f32 vz = *(f32*)((s32)prev + 0x94) - z;
        f32 dist = (f32)sqrt(vx * vx + vy * vy + vz * vz);
        f32 scale = float_1_80421f88 - step / dist;
        y += vy * scale;
        if (y < *(f32*)((s32)npc + 0x200)) {
            y = *(f32*)((s32)npc + 0x200);
        }
        *(f32*)((s32)seg + 0x8C) = x + vx * scale;
        *(f32*)((s32)seg + 0x90) = y;
        *(f32*)((s32)seg + 0x94) = z + vz * scale;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(camGetPos) {
    s32* args = event->args;
    void* cam = camGetPtr(4);
    evtSetFloat(event, args[0], *(f32*)((s32)cam + 0xC));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 cliffCheck(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    extern f32 npcTransRytoFaceDir(void* npc);
    extern f32 float_90_80421fbc;
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 dir;
    f32 a;
    f32 b;
    f32 c;
    f32 radius;
    f32 d;
    f32 e;
    f32 f;
    void* down;
    void* ahead;

    if (npcTransRytoFaceDir(npc) == float_90_80421fbc) {
        dir = float_1_80421f88;
    } else {
        dir = float_neg1_80421f90;
    }
    radius = float_5_80421f84;
    down = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                          *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                          *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                          float_neg1_80421f90, float_0_80421f8c,
                          &a, &b, &c, &radius, &d, &e, &f);
    radius = float_5_80421f84;
    ahead = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                           *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                           *(f32*)((s32)npc + 0x94), dir, float_neg1_80421f90,
                           float_0_80421f8c, &a, &b, &c, &radius, &d, &e, &f);
    if (down != NULL && ahead != NULL) {
        evtSetValue((void*)param_1, args[0], 0);
    } else if (down != NULL && ahead == NULL) {
        evtSetValue((void*)param_1, args[0], 1);
    } else {
        evtSetValue((void*)param_1, args[0], 2);
    }
    return 2;
}

s32 upperCheck(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    extern f32 float_15_80421fb0;
    extern f32 float_2_80421fb8;
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 a;
    f32 b;
    f32 c;
    f32 radius = float_5_80421f84;
    f32 d;
    f32 e;
    f32 f;
    void* hit1;
    void* hit2;

    hit1 = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C) - float_15_80421fb0,
                          *(f32*)((s32)npc + 0x90) + float_30_80421fb4,
                          *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                          float_2_80421fb8, float_0_80421f8c,
                          &a, &b, &c, &radius, &d, &e, &f);
    hit2 = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C) + float_15_80421fb0,
                          *(f32*)((s32)npc + 0x90) + float_30_80421fb4,
                          *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                          float_2_80421fb8, float_0_80421f8c,
                          &a, &b, &c, &radius, &d, &e, &f);
    evtSetValue((void*)param_1, args[0], hit1 != NULL || hit2 != NULL);
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(getSpdWalk) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetFloat(event, args[0], *(f32*)((s32)npc + 0x210));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(getSpdRun) {
    s32* args = event->args;
    void* npc = evtNpcNameToPtr(event, str_me_80421f30);
    evtSetFloat(event, args[0], *(f32*)((s32)npc + 0x21C));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 limitSpd(int param_1) {
    s32* args = *(s32**)(param_1 + 0x18);
    f32 speed = evtGetFloat((void*)param_1, args[0]);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);

    *(f32*)((s32)npc + 0x1D0) = speed;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 zakoEntryDokan(EventEntry* event) {
    extern void* npcNameToPtr(const char* name);
    extern void* evtEntry(void* script, s32 order, u32 flags);
    extern void* marioGetPtr(void);
    extern char* hitGetName(void* hit);
    extern u32 vec3_802ed9a8[];
    extern f32 float_35_80421fac;
    void* owner;
    void* npc;
    void* child;
    char mobjName[32];
    char names[56];
    char* name;
    char* strings;
    void* mario;
    s32 i;

    strings = str_kemuri_test_802ed438;
    owner = evtNpcNameToPtr(event, str_me_80421f30);
    if (strcmp((char*)owner + 8, strings + 0x690) == 0) {
        strcpy(names, strings + 0x6C8);
        strcpy(names + 0x10, strings + 0x6D8);
        strcpy(names + 0x20, strings + 0x6E8);
        strcpy(mobjName, strings + 0x6F8);
    }
    if (strcmp((char*)owner + 8, strings + 0x6A8) == 0) {
        strcpy(names, strings + 0x704);
        strcpy(names + 0x10, strings + 0x714);
        strcpy(names + 0x20, strings + 0x724);
        strcpy(mobjName, strings + 0x734);
    }

    mario = marioGetPtr();
    name = hitGetName(*(void**)((s32)mario + 0x1E8));
    if (name != NULL && strcmp(name, mobjName) == 0) {
        return 2;
    }
    mario = marioGetPtr();
    name = hitGetName(*(void**)((s32)mario + 0x1EC));
    if (name != NULL && strcmp(name, mobjName) == 0) {
        return 2;
    }
    mario = marioGetPtr();
    name = hitGetName(*(void**)((s32)mario + 0x1E4));
    if (name != NULL && strcmp(name, mobjName) == 0) {
        return 2;
    }

    name = names;
    for (i = 0; i < 3; i++, name += 0x10) {
        npc = npcNameToPtr(name);
        if (*(f32*)((s32)npc + 0x90) <= float_neg75_80421fa0) {
            *(u32*)npc |= 0x20410;
            *(u32*)npc &= ~0x20;
            *(u32*)((s32)npc + 0x1D4) |= 0x1002;
            *(u32*)((s32)npc + 0x1D4) &= 0xEFAFFFFB;
            *(u32*)((s32)npc + 0x8C) = *(u32*)((s32)owner + 0x8C);
            *(f32*)((s32)npc + 0x90) = *(f32*)((s32)owner + 0x90) - float_35_80421fac;
            *(u32*)((s32)npc + 0x94) = *(u32*)((s32)owner + 0x94);
            *(u32*)((s32)npc + 0xEC) = vec3_802ed9a8[0];
            *(u32*)((s32)npc + 0xF0) = vec3_802ed9a8[1];
            *(u32*)((s32)npc + 0xF4) = vec3_802ed9a8[2];
            *(s32*)((s32)npc + 0x304) = 1;
            if (*(void**)((s32)npc + 0x124) == NULL) {
                return 2;
            }
            if (*(s32*)((s32)npc + 0x11C) != 0) {
                evtDeleteID(*(s32*)((s32)npc + 0x11C));
            }
            child = evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
            *(s8*)((s32)child + 0x10) = 1;
            *(void**)((s32)child + 0x170) = npc;
            *(s32*)((s32)npc + 0x11C) = *(s32*)((s32)child + 0x15C);
            return 2;
        }
    }
    return 2;
}

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 dokanCheck(int param_1) {
    extern void* mobjNameToPtrNoAssert(char* name);
    extern char str_mobj9_80421fa4[];
    char name[32];
    char* strings = str_kemuri_test_802ed438;
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    void* mobj;

    if (strcmp((char*)npc + 8, strings + 0x690) == 0) {
        strcpy(name, str_mobj9_80421fa4);
    }
    if (strcmp((char*)npc + 8, strings + 0x6A8) == 0) {
        strcpy(name, strings + 0x6C0);
    }
    mobj = mobjNameToPtrNoAssert(name);
    if (mobj != NULL) {
        evtSetValue((void*)param_1, args[0], 1);
    } else {
        evtSetValue((void*)param_1, args[0], 0);
    }
    return 2;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 zakoEntryFall(EventEntry* event) {
    extern void* npcNameToPtr_NoAssert(const char* name);
    extern void* evtEntry(void* script, s32 order, u32 flags);
    extern u32 vec3_802ed9c0[];
    extern const char str_none_80421f98[];
    void* owner;
    void* npc;
    void* child;
    char names[56];
    char* name;
    char* strings;
    s32 i;

    strings = str_kemuri_test_802ed438;
    owner = evtNpcNameToPtr(event, str_me_80421f30);
    if (strcmp((char*)owner + 8, strings + 0x5EC) == 0) {
        strcpy(names, strings + 0x600);
        strcpy(names + 0x10, str_none_80421f98);
        strcpy(names + 0x20, str_none_80421f98);
    }
    if (strcmp((char*)owner + 8, strings + 0x610) == 0) {
        strcpy(names, strings + 0x600);
        strcpy(names + 0x10, strings + 0x628);
        strcpy(names + 0x20, strings + 0x638);
    }
    if (strcmp((char*)owner + 8, strings + 0x648) == 0) {
        strcpy(names, strings + 0x660);
        strcpy(names + 0x10, strings + 0x670);
        strcpy(names + 0x20, strings + 0x680);
    }

    name = names;
    for (i = 0; i < 3; i++, name += 0x10) {
        npc = npcNameToPtr_NoAssert(name);
        if (npc != NULL && *(f32*)((s32)npc + 0x90) <= float_neg75_80421fa0) {
            *(u32*)npc |= 0x20410;
            *(u32*)npc &= ~0x20;
            *(u32*)((s32)npc + 0x1D4) |= 0x1002;
            *(u32*)((s32)npc + 0x1D4) &= 0xEF8FFFFB;
            *(u32*)((s32)npc + 0x8C) = *(u32*)((s32)owner + 0x8C);
            *(u32*)((s32)npc + 0x90) = *(u32*)((s32)owner + 0x90);
            *(u32*)((s32)npc + 0x94) = *(u32*)((s32)owner + 0x94);
            *(u32*)((s32)npc + 0xEC) = vec3_802ed9c0[0];
            *(u32*)((s32)npc + 0xF0) = vec3_802ed9c0[1];
            *(u32*)((s32)npc + 0xF4) = vec3_802ed9c0[2];
            *(s32*)((s32)npc + 0x304) = 2;
            if (*(void**)((s32)npc + 0x124) != NULL) {
                if (*(s32*)((s32)npc + 0x11C) != 0) {
                    evtDeleteID(*(s32*)((s32)npc + 0x11C));
                }
                child = evtEntry(*(void**)((s32)npc + 0x124), 0, 0x20);
                *(s8*)((s32)child + 0x10) = 1;
                *(void**)((s32)child + 0x170) = npc;
                *(s32*)((s32)npc + 0x11C) = *(s32*)((s32)child + 0x15C);
            }
            return 2;
        }
    }
    return 2;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset


/* CHATGPT FALLBACK MISSING STUBS: main/npc_event 20260624_191429 */

/* fallback stub-fill: map=unk_800f1ac8 addr=0x800f1ac8 size=0x00000184 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unk_800f1ac8(EventEntry* event, s32 first) {
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void effDelete(void* eff);
    extern f32 float_255_80421f94;
    s32* args;
    void* npc;
    f32 x;
    f32 y;
    f32 z;
    s32 frames;
    void* eff;
    f32 value;
    f32 zero;
    f32 alpha;

    npc = evtNpcNameToPtr(event, str_me_80421f30);
    args = event->args;
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    frames = evtGetValue(event, args[3]);
    if (first != 0) {
        *(void**)((s32)npc + 0x304) = effFireEntry(x, y, z, float_1_80421f88, 5, 1000);
        *(s32*)((s32)npc + 0x308) = frames;
    }
    frames = *(s32*)((s32)npc + 0x308) - 1;
    eff = *(void**)((s32)npc + 0x304);
    *(s32*)((s32)npc + 0x308) = frames;
    if (*(s32*)((s32)npc + 0x308) <= 30) {
        value = intplGetValue(1, 30 - *(s32*)((s32)npc + 0x308), 30, float_1_80421f88, float_0_80421f8c);
        zero = float_0_80421f8c;
        *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x78) = value;
        *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x7C) = value;
        alpha = float_255_80421f94;
        frames = *(s32*)((s32)npc + 0x308);
        value = intplGetValue(0, 30 - frames, 30, alpha, zero);
        *(u8*)((s32)*(void**)((s32)eff + 0xC) + 0x10) = (u8)(s32)value;
    }
    if (*(s32*)((s32)npc + 0x308) <= 0) {
        effDelete(eff);
        return 2;
    }
    return 0;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(set_bottle_flag_init) {
    void* npc = evtNpcNameToPtr(event, str_slave_0_802ed598);
    *(u32*)npc |= 0x28020020;
    *(u16*)((s32)npc + 0x31E) |= 0x1F;
    return 2;
}

s32 gesso_ground_check(int param_1) {
    extern void* hitCheckFilter(s32 flags, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz,
                                f32* outX, f32* outY, f32* outZ, f32* radius,
                                f32* outA, f32* outB, f32* outC);
    s32* args = *(s32**)(param_1 + 0x18);
    void* npc = evtNpcNameToPtr((void*)param_1, str_me_80421f30);
    f32 a;
    f32 b;
    f32 c;
    f32 radius = float_5_80421f84;
    f32 d;
    f32 e;
    f32 f;
    void* hit = hitCheckFilter(0, *(f32*)((s32)npc + 0x8C),
                               *(f32*)((s32)npc + 0x90) + float_1_80421f88,
                               *(f32*)((s32)npc + 0x94), float_0_80421f8c,
                               float_neg1_80421f90, float_0_80421f8c,
                               &a, &b, &c, &radius, &d, &e, &f);

    evtSetValue((void*)param_1, args[0], hit != NULL);
    return 2;
}

const char str_mario_80421f34[] = "mario";
const char zero_80421f3c[] = "";
const char str_A_1_80421f40[] = "A_1";
const char str_S_1_80421f44[] = "S_1";
const char str_S_3_80421f48[] = "S_3";
const char str_A_2_80421f4c[] = "A_2";
const char str_A_3_80421f50[] = "A_3";
const char str_A_4_80421f54[] = "A_4";
const char str_Z_3_80421f58[] = "Z_3";
const u32 dat_80421f5c = 0xFFFFFFFF;
const char str_W_1_80421f60[] = "W_1";
const char str_B_1A_80421f64[] = "B_1A";
const char str_A_1B_80421f6c[] = "A_1B";
const char str_B_2_80421f74[] = "B_2";
const char str_R_1_80421f78[] = "R_1";
const char str_Z_1_80421f7c[] = "Z_1";
const char str_B_1_80421f80[] = "B_1";
const f32 float_5_80421f84 = 5.0f;
const f32 float_1_80421f88 = 1.0f;
const f32 float_0_80421f8c = 0.0f;
const f32 float_neg1_80421f90 = -1.0f;
const f32 float_255_80421f94 = 255.0f;
const char str_none_80421f98[] = "\x82\xC8\x82\xB5";
const f32 float_neg75_80421fa0 = -75.0f;
const char str_mobj9_80421fa4[] = "mobj9";
const f32 float_35_80421fac = 35.0f;
const f32 float_15_80421fb0 = 15.0f;
const f32 float_30_80421fb4 = 30.0f;
const f32 float_2_80421fb8 = 2.0f;
const f32 float_90_80421fbc = 90.0f;
const f32 float_0p25_80421fc0 = 0.25f;
const f32 float_10_80421fc4 = 10.0f;
const f32 float_300_80421fc8 = 300.0f;
const f32 float_20_80421fcc = 20.0f;
const f32 float_1000_80421fd0 = 1000.0f;
const f32 float_0p5_80421fd4 = 0.5f;
const f32 float_neg980_80421fd8 = -980.0f;
const f32 float_6p2832_80421fdc = 6.2831855f;
const f32 float_360_80421fe0 = 360.0f;
const f32 float_18_80421fe4 = 18.0f;
/* Recovered npc_event event bytecode (.data). */
u32 enemy_common_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_set_damage_anim, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)str_kemuri_test_802ed438,
    0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7DB3, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_ENEMY_DIE1_802ed444, 0x00000000, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)evt_npc_get_drop_coin,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002001B, 0xFE363C83, 0x00000000, 0x00010005,
    0xFE363C83, 0x0009005B, (u32)evt_item_entry, 0x00000000, 0x00000079, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x00000000, 0x00000006,
    0x00000021, 0x0003005B, (u32)evt_npc_get_drop_heart, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002001B,
    0xFE363C83, 0x00000000, 0x00010005, 0xFE363C83, 0x0009005B, (u32)evt_item_entry,
    0x00000000, 0x0000007B, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A,
    0xFFFFFFFF, 0x00000000, 0x00000006, 0x00000021, 0x0003005B, (u32)evt_npc_get_drop_flower,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002001B, 0xFE363C83, 0x00000000, 0x00010005,
    0xFE363C83, 0x0009005B, (u32)evt_item_entry, 0x00000000, 0x0000007C, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0xFFFFFFFF, 0x00000000, 0x00000006,
    0x00000021, 0x0003005B, (u32)evt_npc_get_drop_item, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020019,
    0xFE363C83, 0x00000000, 0x0009005B, (u32)evt_item_entry, 0x00000000, 0xFE363C83,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000009, 0xFFFFFFFF, 0x00000000,
    0x00000021, 0x0002005B, (u32)evt_npc_getback_item_entry, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_drop_fixitem,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020019, 0xFE363C83, 0x00000000, 0x0009005B,
    (u32)evt_item_entry, 0x00000000, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000009, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0002005B, (u32)evt_npc_delete,
    (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 enemy_event_dead_event_sub[] = {
    0x0002005B, (u32)evt_npc_set_damage_anim, 0xFE363C8A, 0x0001000A, 0x000001F4, 0x0005005B,
    (u32)evt_npc_get_position, 0xFE363C8A, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B,
    (u32)evt_eff, 0x00000000, (u32)str_kemuri_test_802ed438, 0x00000004, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A7DB3, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_ENEMY_DIE1_802ed444,
    0x00000000, 0x0002005B, (u32)evt_npc_delete, 0xFE363C8A, 0x00000002, 0x00000001,
};

u32 enemy_common_blow_event[] = {
    0x0002005B, (u32)evt_npc_set_confuse_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)_wait_blow_end, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000007D0, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 enemy_2d_common_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x10000000, 0x0002005B, (u32)pressCheck,
    0xFE363C8F, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0x000000C8, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)str_kemuri_test_802ed438, 0x00000004,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7DB3, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_snd_sfxon, (u32)(char*)str_SFX_ENEMY_DIE1_802ed444, 0x00000000, 0x0002005B, (u32)evt_npc_set_damage_anim, (u32)(char*)str_me_80421f30,
    0x0003005B, (u32)evt_npc_get_kpencount_type, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x00010022, 0xFE363C80,
    0x0001002D, 0x00000001, 0x00020018, 0xFE363C8F, 0x00000000, 0x0002005B,
    (u32)evt_npc_calc_score, (u32)(char*)str_me_80421f30, 0x0001005C, (u32)_2d_dead_rotate_event, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)_2d_get_dead_jump_offset,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020035, 0xFE363C80, 0xFE363C83, 0x0009005B,
    (u32)_2d_dead_jump, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000258,
    0xF24B1A80, 0xF24A7A80, 0xFFFFFED4, 0x00000020, 0x0002005B, (u32)evt_npc_calc_score,
    (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00020000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000002, 0x0002005B, (u32)evt_npc_set_talk_anim,
    (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020036, 0xFE363C81, 0x00000005, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00010009, 0x0000001E,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000006E, 0x00010005, 0x0000001E, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000000, 0x00000001, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x00020036, 0xFE363C80, 0x00000028, 0x00020036, 0xFE363C81,
    0x00000014, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xFFFFFFF6, 0x00000004, 0x00000000,
    0x00010005, 0x00000000, 0x0000006E, 0x00010005, 0x0000003C, 0x0005005B,
    (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0xFFFFFFFF, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C80, 0x00000050,
    0x00020036, 0xFE363C81, 0x00000014, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000003E8, 0x00000000, 0xFFFFFFF6,
    0x00000004, 0x00000000, 0x0000006E, 0x00010005, 0x0000003C, 0x0005005B,
    (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000001, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020036, 0xFE363C80, 0x00000050,
    0x00020036, 0xFE363C81, 0x00000014, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000003E8, 0x00000000, 0xFFFFFFF6,
    0x00000004, 0x00000000, 0x0002001C, 0xFE363C81, 0xFFFFFF38, 0x00000007,
    0x00000021, 0x00000006, 0x00000021, 0x0001002D, 0x00000002, 0x0002005B,
    (u32)evt_npc_calc_score, (u32)(char*)str_me_80421f30, 0x0001005C, (u32)_2d_dead_rotate_event, 0x0004005B, (u32)evt_npc_status_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00400000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)_2d_get_dead_jump_offset, (u32)(char*)str_me_80421f30,
    0xFE363C83, 0x00020035, 0xFE363C80, 0xFE363C83, 0x0009005B, (u32)_2d_dead_jump,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000258, 0xF24B1A80,
    0xF24B7E80, 0xFFFFFED4, 0x0001002D, 0x00000004, 0x0002005B, (u32)evt_npc_calc_score,
    (u32)(char*)str_me_80421f30, 0x0001005C, (u32)_2d_dead_rotate_event, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)_2d_get_dead_jump_offset, (u32)(char*)str_me_80421f30,
    0xFE363C83, 0x00020035, 0xFE363C80, 0xFE363C83, 0x0009005B, (u32)_2d_dead_jump,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000258, 0xF24B1A80,
    0xF24B7E80, 0xFFFFFED4, 0x0001002D, 0x00000020, 0x0001005C, (u32)_2d_dead_rotate_event,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)_2d_get_dead_jump_offset, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020035, 0xFE363C80,
    0xFE363C83, 0x0009005B, (u32)_2d_dead_jump, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000258, 0xF24B1A80, 0xF24B7E80, 0xFFFFFED4, 0x0001002D,
    0x00000008, 0x0002005B, (u32)evt_npc_calc_score, (u32)(char*)str_me_80421f30, 0x0001005C, (u32)_2d_dead_rotate_event,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0003005B, (u32)_2d_get_dead_jump_offset, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020035, 0xFE363C80,
    0xFE363C83, 0x0009005B, (u32)_2d_dead_jump, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000258, 0xF24B1A80, 0xF24B7E80, 0xFFFFFED4, 0x0001002D,
    0x00000010, 0x0002005B, (u32)evt_npc_calc_score, (u32)(char*)str_me_80421f30, 0x0001005C, (u32)_2d_dead_rotate_event,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00400000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)_2d_get_dead_jump_offset, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020035, 0xFE363C80, 0xFE363C83,
    0x0009005B, (u32)_2d_dead_jump, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000258, 0xF24B1A80, 0xF24B7E80, 0xFFFFFED4, 0x0000002A, 0x0001000A,
    0x000001F4, 0x00000031, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 _2d_dead_rotate_event[] = {
    0x00020032, 0xFE363C80, 0xFFFFFFF4, 0x00010005, 0x0000000F, 0x0005005B,
    (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0xFE363C80, 0x00010009,
    0x00000001, 0x00000006, 0x00000002, 0x00000001,
};

u32 unk_evt_8033f9b0[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x00010003,
    0x00000000, 0x0001000A, 0x000001F4, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 npc_urouro_move_event[] = {
    0x0001005E, (u32)npc_urouro_init_event, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0007005B, (u32)urouro_init_func, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0xF24C0A80, 0xF24AF280, 0x00000000, 0x0002005B, (u32)urouro_main_func,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 npc_urouro_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 kuriboo_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x00010003, 0x00000000, 0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir,
    0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000004, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 kuriboo_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000012C, 0x00000000,
    0xF24ACA80, 0x00000000, 0x0008005B, (u32)evt_npc_homing_target, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34,
    0x00000000, 0xF24C5A80, 0xF24AA280, 0xF24B6A80, 0x00000004, 0x0001000A,
    0x000001F4, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 kuriboo_lost_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 kuriboo_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 patakuri_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x00010003, 0x00000000, 0x0002005B,
    (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim,
    (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A5280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24AA280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A5280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24AA280, 0x00000000, 0x00000003,
    0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 patakuri_find_event[] = {
    0x0002005B, (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0004005B, (u32)evt_get_target_dir,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C81,
    0xFE363C83, 0xFE363C80, 0xF24C0A80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00000000, 0x0000004B, 0xF249B280, 0x00000000,
    0x00000003, 0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_set_stay_anim,
    (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x000003E8, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 patakuri_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 patakuri_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000032, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x000003E8, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x0002005B,
    (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24A5280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24AA280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24A5280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24AA280, 0x00000000, 0x00000003, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 nokonoko_shell_return_event[] = {
    0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000001, 0xFE363C8F, 0x00020018,
    0xFE363C8F, 0x00000001, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000001, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim,
    (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000012C, 0x00000000, 0xF24ACA80, 0x00000000, 0x00000021,
    0x00000002, 0x00000001,
};

u32 nokonoko_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00120018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30,
    0x00000001, 0x00000000, 0x00000002, 0x00000001,
};

u32 nokonoko_move_event[] = {
    0x0001005E, (u32)nokonoko_shell_return_event, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0002005B, (u32)evt_npc_sound_data_reset,
    (u32)(char*)str_me_80421f30, 0x00010003, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000004,
    0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 nokonoko_find_event[] = {
    0x0001005E, (u32)nokonoko_shell_return_event, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x000C005B,
    (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001E,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000012C, 0x00000000, 0xF24ACA80, 0x00000000, 0x0007005B, (u32)evt_npc_sound_data_set,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SFX_FLD_ENM_NOKO_MOV_802ed454, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000001, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x0000000B, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_NKT_A_1_802ed46c, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C83,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_FLD_ENM_NOKO_SHE_802ed474, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24E7A80, 0x0000006E,
    0x00010005, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000001, 0x00000007, 0x00000021,
    0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000028, 0x00000000,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0001000A, 0x00000064,
    0x0001006F, 0xFE363C8E, 0x00010005, 0x00000000, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81,
    0x0000000A, 0x000F005B, (u32)evt_eff, (u32)(char*)zero_80421f3c, (u32)str_kemuri_test_802ed438, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7BB3, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x00000003, 0x00000006, 0x00000070, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x00000320, 0xF250BA80, 0x00000004, 0x0001005F,
    0xFE363C8E, 0x0001005E, (u32)nokonoko_shell_return_event, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 nokonoko_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0001005E, (u32)nokonoko_shell_return_event, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 nokonoko_return_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0001005E, (u32)nokonoko_shell_return_event, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 togenoko_shell_return_event[] = {
    0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000001, 0xFE363C8F, 0x00020018,
    0xFE363C8F, 0x00000001, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000001, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim,
    (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000012C, 0x00000000, 0xF24ACA80, 0x00000000, 0x00000021,
    0x00000002, 0x00000001,
};

u32 togenoko_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00120018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30,
    0x00000001, 0x00000000, 0x00000002, 0x00000001,
};

u32 togenoko_move_event[] = {
    0x0001005E, (u32)togenoko_shell_return_event, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0002005B, (u32)evt_npc_sound_data_reset,
    (u32)(char*)str_me_80421f30, 0x00010003, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000004,
    0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 togenoko_find_event[] = {
    0x0001005E, (u32)togenoko_shell_return_event, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x000C005B,
    (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001E,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000012C, 0x00000000, 0xF24ACA80, 0x00000000, 0x0007005B, (u32)evt_npc_sound_data_set,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SFX_FLD_ENM_NOKO_MOV_802ed454, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000001, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x0000000B, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_A_1_80421f40, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C83,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_FLD_ENM_NOKO_SHE_802ed474, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24E7A80, 0x0000006E,
    0x00010005, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000001, 0x00000007, 0x00000021,
    0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000028, 0x00000000,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0001000A, 0x00000064,
    0x0001006F, 0xFE363C8E, 0x00010005, 0x00000000, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81,
    0x0000000A, 0x000F005B, (u32)evt_eff, (u32)(char*)zero_80421f3c, (u32)str_kemuri_test_802ed438, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7BB3, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010009,
    0x00000003, 0x00000006, 0x00000070, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x00000320, 0xF250BA80, 0x00000004, 0x0001005F,
    0xFE363C8E, 0x0001005E, (u32)togenoko_shell_return_event, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 togenoko_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0001005E, (u32)togenoko_shell_return_event, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 togenoko_return_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0001005E, (u32)togenoko_shell_return_event, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 patapata_move_event[] = {
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0003005B, (u32)evt_sub_random,
    0x00000168, 0xFE363C83, 0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x00010003,
    0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00010005, 0x00000000, 0x00020018, 0xFE363C8A, 0x00000000, 0x0004005B,
    (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24A2A80, 0xF24ACA80, 0x00000021, 0x00020018,
    0xFE363C8A, 0x00000002, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24A2A80,
    0xF24ACA80, 0x00000021, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24AF280, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A5E80,
    0x00000000, 0x00000002, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24AF280, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A9680,
    0x00000000, 0x00000002, 0x0002001D, 0xFE363C8A, 0x00000004, 0x00000007,
    0x00000021, 0x00020035, 0xFE363C8A, 0x00000001, 0x00000006, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 patapata_find_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0003005B, (u32)evt_npc_pera_onoff,
    (u32)(char*)str_me_80421f30, 0x00000001, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00010005, 0x00000012, 0x0005005B, (u32)evt_npc_add_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x0000000F, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PTP_A_5_802ed48c, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PATA_MOV_802ed494, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x00010005, 0x00000006, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0000000F, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x0000000A, 0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_SFX_FLD_ENM_PATA_MOV_802ed4ac, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0004005B,
    (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C81, 0xFE363C83, 0xFE363C80, 0xF24D9A80, 0x0003005B, (u32)evt_npc_set_ry,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x000003E8, 0x00000000, 0xF249DA80,
    0x00000004, 0x00000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 patapata_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30,
    0x00000000, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B,
    (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 patapata_return_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000000, 0x00000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30,
    0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30,
    0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24AF280, 0x0006005B, (u32)evt_fbat_trans_floor_position,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B,
    (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4,
    0x00000000, 0xF24A9680, 0x00000000, 0x00000002, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24AF280, 0x0006005B, (u32)evt_fbat_trans_floor_position,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B,
    (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4,
    0x00000000, 0xF24A5E80, 0x00000000, 0x00000002, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24AF280, 0x0006005B, (u32)evt_fbat_trans_floor_position,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B,
    (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4,
    0x00000000, 0xF24A9680, 0x00000000, 0x00000002, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24AF280, 0x0006005B, (u32)evt_fbat_trans_floor_position,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B,
    (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4,
    0x00000000, 0xF24A5E80, 0x00000000, 0x00000002, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 met_init_event_common[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000008,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100010,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
};

u32 met_set_anim_wait_event[] = {
    0x0004005B, (u32)evt_npc_get_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F, 0x00020018,
    0xFE363C8F, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_MET_S_1_802ed4c4, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_TMT_S_1_802ed4cc, 0x00000021, 0x00000020, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30,
    0x00000000, 0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SMT_S_1_802ed4d4, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_STM_S_1_802ed4dc, 0x00000021, 0x00000021, 0x00000002, 0x00000001,
};

u32 met_set_anim_walk_event[] = {
    0x0004005B, (u32)evt_npc_get_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F, 0x00020018,
    0xFE363C8F, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_MET_W_1_802ed4e4, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_TMT_W_1_802ed4ec, 0x00000021, 0x00000020, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30,
    0x00000000, 0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SMT_W_1_802ed4f4, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_STM_W_1_802ed4fc, 0x00000021, 0x00000021, 0x00000002, 0x00000001,
};

u32 met_init_event[] = {
    0x0001005E, (u32)met_init_event_common, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_set_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000002,
    0x00000001,
};

u32 met_init_event_ceil[] = {
    0x0001005E, (u32)met_init_event_common, 0x0004005B, (u32)evt_npc_set_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000001, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000002, 0x00000001,
};

u32 togemet_init_event[] = {
    0x0001005E, (u32)met_init_event_common, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_set_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000001, 0x00000002,
    0x00000001,
};

u32 togemet_init_event_ceil[] = {
    0x0001005E, (u32)met_init_event_common, 0x0004005B, (u32)evt_npc_set_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000001, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000001,
    0x00000002, 0x00000001,
};

u32 met_move_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0001005E, (u32)met_set_anim_wait_event,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0002005B, (u32)evt_npc_sound_data_reset,
    (u32)(char*)str_me_80421f30, 0x00010003, 0x00000000, 0x0001000A, 0x000001F4, 0x0001005E,
    (u32)met_set_anim_walk_event, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000007D0,
    0xF24B1A80, 0x00000004, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 met_find_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)evt_npc_sound_data_reset,
    (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_get_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F,
    0x00020018, 0xFE363C8F, 0x00000000, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000012C, 0x00000000,
    0xF24ACA80, 0x00000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x0000000B,
    0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30, (u32)(char*)str_SFX_FLD_ENM_MET_ATTA_802ed504, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_MET_A_1_802ed51c, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_TMT_A_1_802ed524, 0x00000021, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34,
    0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24E7A80, 0x0000006E,
    0x00020032, 0xFE363C84, 0x00000000, 0x00020032, 0xFE363C8F, 0x00000000,
    0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x00020035, 0xFE363C8F,
    0x00000001, 0x0002001D, 0xFE363C8F, 0x00000037, 0x00000007, 0x00000021,
    0x00020035, 0xFE363C84, 0x00000028, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30,
    0x00000000, 0xFE363C84, 0x00000000, 0x00000006, 0x00000070, 0x0001000A,
    0x00000064, 0x0001006F, 0xFE363C8E, 0x00010005, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x0000000A, 0x000F005B, (u32)evt_eff, (u32)(char*)zero_80421f3c, (u32)str_kemuri_test_802ed438,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7BB3, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00010009, 0x00000003, 0x00000006, 0x00000070, 0x0007005B, (u32)evt_npc_move_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000320, 0xF250BA80, 0x00000004,
    0x0001005F, 0xFE363C8E, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0001005E, (u32)met_set_anim_wait_event, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000012C, 0x00000000, 0xF24ACA80, 0x00000000,
    0x00000020, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x0000000B, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00020000, 0x0004005B, (u32)evt_npc_get_unitwork,
    (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SMT_A_1_802ed52c, 0x00000020, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_STM_A_1_802ed534, 0x00000021, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0x00000000, 0xFE363C82, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x0004005B, (u32)evt_npc_set_btlsetup_work, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x0001005E,
    (u32)met_set_anim_wait_event, 0x0001000A, 0x000003E8, 0x00000021, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 met_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30,
    0x00000000, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0000001E, 0x0001005E, (u32)met_set_anim_wait_event, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 met_return_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30,
    0x00000000, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000000, 0x0001005E, (u32)met_set_anim_wait_event, 0x0001000A, 0x000001F4, 0x0001005E,
    (u32)met_set_anim_walk_event, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x00000000, 0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 patamet_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30,
    0x00010003, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_get_loiter_dir,
    0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24A5280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24AA280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24A5280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24AA280, 0x00000000, 0x00000003, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 patamet_find_event[] = {
    0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30, (u32)(char*)str_SFX_FLD_ENM_PATAMET__802ed53c, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0002005B, (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30, 0x000C005B,
    (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001E,
    0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C81, 0xFE363C83, 0xFE363C80, 0xF24C0A80, 0x0003005B,
    (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000000, 0x0000004B,
    0xF249B280, 0x00000000, 0x00000003, 0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30,
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000001F4, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82,
    0xFE363C83, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x000003E8, 0x00000000, 0x00000000, 0x00000000,
    0x00000001, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 patamet_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 patamet_return_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24A5280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24AA280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24A5280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24AA280, 0x00000000, 0x00000003,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 chorobon_move_event[] = {
    0x0005005B, (u32)evt_npc_set_scale, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000001, 0x00000001,
    0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x00010003, 0x00000000, 0x0003005B,
    (u32)evt_sub_random, 0x00000003, 0xFE363C84, 0x00020035, 0xFE363C84, 0x00000002,
    0x00010005, 0xFE363C84, 0x00010009, 0x00000001, 0x0003005B, (u32)evt_sub_random,
    0x00000168, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0x00000032, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000012C, 0x00000000, 0xF24AF280, 0x00000000,
    0x00000006, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C83,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0003005B, (u32)evt_sub_random,
    0x000001F4, 0xFE363C84, 0x00020035, 0xFE363C84, 0x0000000A, 0x0001000A,
    0xFE363C84, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 chorobon_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x00010005, 0x00000003, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0x00000032,
    0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000012C, 0x00000000, 0xF24AF280, 0x00000000, 0x0001000A, 0x00000032,
    0x00000006, 0x0005005B, (u32)evt_npc_set_scale, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000001,
    0x00000001, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 chorobon_lost_event[] = {
    0x0005005B, (u32)evt_npc_set_scale, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000001, 0x00000001,
    0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 chorobon_return_event[] = {
    0x0005005B, (u32)evt_npc_set_scale, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000001, 0x00000001,
    0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30,
    0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0x00000032, 0x0009005B, (u32)evt_npc_jump_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000012C, 0x00000000,
    0xF24AF280, 0x00000000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 pansy_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x00010003, 0x00000000, 0x0001000A,
    0x000001F4, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000007D0,
    0xF24B1A80, 0x00000004, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 pansy_find_event[] = {
    0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30, (u32)(char*)str_SFX_FLD_ENM_PANZI_MO_802ed558, (u32)(char*)str_SFX_FLD_ENM_PANZI_MO_802ed558, 0x00007530,
    0x00000000, 0x00000000, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x0008005B, (u32)evt_npc_homing_target, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0x00000000, 0xF24C0A80,
    0xF24AA280, 0xF24B6A80, 0x00000004, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 pansy_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_PAN_S_1_802ed570, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 pansy_return_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_PAN_S_1_802ed570, 0x0001000A, 0x000001F4, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_PAN_W_1_802ed578, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x00000000, 0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 twinkling_pansy_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x00010003, 0x00000000, 0x0001000A,
    0x000001F4, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000007D0,
    0xF24B1A80, 0x00000004, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 twinkling_pansy_find_event[] = {
    0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30, (u32)(char*)str_SFX_FLD_ENM_PANZI_MO_802ed558, (u32)(char*)str_SFX_FLD_ENM_PANZI_MO_802ed558, 0x00007530,
    0x00000000, 0x00000000, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C83, 0x00020035, 0xFE363C83, 0x000000B4, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24F2A80, 0x0007005B,
    (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000003E8, 0xF24F2A80,
    0x00000006, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 twinkling_pansy_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_PAN_S_1_K_802ed580, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 twinkling_pansy_return_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_PAN_S_1_K_802ed580, 0x0001000A, 0x000001F4, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_PAN_W_1_K_802ed58c, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x00000000, 0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 karon_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0002005B, (u32)mahoon_get_groupname, 0xFE363C8A, 0x0006005B,
    (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x20000000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_KRN_H_1_802ed5a0, 0x0003005B, (u32)evt_npc_set_width, (u32)(char*)str_slave_0_802ed598, 0x00000018,
    0x0003005B, (u32)evt_npc_set_height, (u32)(char*)str_slave_0_802ed598, 0x00000008, 0x00000002, 0x00000001,
};

u32 karon_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598,
    0x00000080, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00200000, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x00010003, 0x00000000, 0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir,
    0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000004, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 karon_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0000006E, 0x00010003, 0x00000000,
    0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x00000000, 0x0000001E,
    0x00010009, 0x00000001, 0x00010004, 0x00000000, 0x00000070, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_A_1_802ed5a8, 0x0001000A, 0x000000FA, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_KARON_BO_802ed5b0, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0x00000000,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_A_2_802ed5d0, 0x0001000A, 0x00000096,
    0x0000006E, 0x0001000A, 0x000003E8, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_KRN_S_1_802ed5d8, 0x00000070, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_slave_0_802ed598, 0x00000000,
    0x00000000, 0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000019, 0x0005005B,
    (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598,
    0x0000000C, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000003E8, 0x00000000, 0xF24BA680, 0x00000000, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0x00000000,
    0xFFFFFC18, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 karon_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 karon_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 karon_blow_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0x00000000, 0xFFFFFC18, 0x00000000, 0x0001005E, (u32)enemy_common_blow_event, 0x00000002,
    0x00000001,
};

u32 honenoko2_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000001,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_Z_2_802ed5e0, 0x00000002, 0x00000001,
};

u32 honenoko2_move_event[] = {
    0x00010071, (u32)(char*)str_EVENT_HONENOKO_MOVE_802ed5e8, 0x00010003, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork,
    (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0003005B, (u32)evt_npc_wait_msec, (u32)(char*)str_me_80421f30, 0x000003E8, 0x00010004, 0x00000000,
    0x00000021, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000001, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_D_2_802ed600, 0x0003005B, (u32)evt_npc_wait_msec, (u32)(char*)str_me_80421f30,
    0x000000AA, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_Z_2_802ed5e0, 0x0003005B,
    (u32)evt_npc_wait_msec, (u32)(char*)str_me_80421f30, 0x0000000A, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 honenoko2_find_event[] = {
    0x00010071, (u32)(char*)str_EVENT_HONENOKO_FIND_802ed608, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_D_3_802ed620, 0x0003005B,
    (u32)evt_npc_wait_msec, (u32)(char*)str_me_80421f30, 0x000000AA, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_D_4_802ed628,
    0x0003005B, (u32)evt_npc_wait_msec, (u32)(char*)str_me_80421f30, 0x000000AA, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00000400, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x0008005B, (u32)evt_npc_homing_target, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0x00000000, 0xF24C5A80,
    0xF24AA280, 0xF24B6A80, 0x00000004, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 honenoko2_lost_event[] = {
    0x00010071, (u32)(char*)str_EVENT_HONENOKO_LOST_802ed630, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0003005B, (u32)evt_npc_wait_msec, (u32)(char*)str_me_80421f30, 0x000003E8, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0004005B, (u32)evt_npc_set_unitwork,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000001, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_KRN_D_2_802ed600, 0x00010009, 0x0000000A, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_KRN_Z_2_802ed5e0, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 honenoko2_return_event[] = {
    0x00010071, (u32)(char*)str_EVENT_HONENOKO_RETUR_802ed648, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_S_1_802ed5d8,
    0x0003005B, (u32)evt_npc_wait_msec, (u32)(char*)str_me_80421f30, 0x000003E8, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_KRN_W_1_802ed664, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x00000000, 0xF24B1A80, 0x00000004, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 killer_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000040,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000004, 0x00000002,
    0x00000001,
};

u32 killer_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000004,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x0000000B, 0x0003005B, (u32)evt_npc_get_dir,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0x000007D0, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x00000000, 0x000000C8, 0x00000004, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81,
    0x0000000F, 0x000F005B, (u32)evt_eff, (u32)(char*)zero_80421f3c, (u32)str_kemuri_test_802ed438, 0x00000011,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)evt_npc_set_position, (u32)(char*)str_me_80421f30, 0x00000000, 0xFFFFFC18, 0x00000000, 0x00000002,
    0x00000001,
};

u32 killer_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_set_damage_anim, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)str_kemuri_test_802ed438,
    0x00000004, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7DB3, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_snd_sfxon, (u32)(char*)str_SFX_ENEMY_DIE1_802ed444, 0x00000000, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B, (u32)evt_npc_get_drop_coin,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002001B, 0xFE363C83, 0x00000000, 0x00010005,
    0xFE363C83, 0x0009005B, (u32)evt_item_entry, 0x00000000, 0x00000079, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00000000, 0x00000006,
    0x00000021, 0x0003005B, (u32)evt_npc_get_drop_heart, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002001B,
    0xFE363C83, 0x00000000, 0x00010005, 0xFE363C83, 0x0009005B, (u32)evt_item_entry,
    0x00000000, 0x0000007B, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000008,
    0xFFFFFFFF, 0x00000000, 0x00000006, 0x00000021, 0x0003005B, (u32)evt_npc_get_drop_flower,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002001B, 0xFE363C83, 0x00000000, 0x00010005,
    0xFE363C83, 0x0009005B, (u32)evt_item_entry, 0x00000000, 0x0000007C, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000008, 0xFFFFFFFF, 0x00000000, 0x00000006,
    0x00000021, 0x0003005B, (u32)evt_npc_get_drop_item, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020019,
    0xFE363C83, 0x00000000, 0x0009005B, (u32)evt_item_entry, 0x00000000, 0xFE363C83,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000009, 0xFFFFFFFF, 0x00000000,
    0x00000021, 0x0002005B, (u32)evt_npc_getback_item_entry, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_drop_fixitem,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x00020019, 0xFE363C83, 0x00000000, 0x0009005B,
    (u32)evt_item_entry, 0x00000000, 0xFE363C83, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000009, 0xFFFFFFFF, 0x00000000, 0x00000021, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_me_80421f30, 0x00000000, 0xFFFFFC18, 0x00000000, 0x00000002, 0x00000001,
};

u32 killer_cannon_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C80, 0x00000002,
    0x00000001,
};

u32 killer_cannon_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KTA_S_1_802ed66c, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00010003, 0x00000000, 0x0001000A, 0x00000BB8, 0x00010003, 0x00000001,
    0x0001000A, 0x00000064, 0x00020035, 0xFE363C8A, 0x00000001, 0x00020018,
    0xFE363C8A, 0x00000002, 0x00020032, 0xFE363C8A, 0x00000000, 0x00000021,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KTA_A_1_802ed674, 0x0001000A, 0x000000C8,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_KILLER_C_802ed67c, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0x00000000, 0x0001000A, 0x000000C8, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30,
    0xFE363C81, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C82, 0xFE363C84, 0xFE363C81,
    0x0000001E, 0x00020035, 0xFE363C83, 0x0000001E, 0x000F005B, (u32)evt_eff,
    (u32)(char*)zero_80421f3c, (u32)str_kemuri_test_802ed438, 0x00000012, 0xFE363C82, 0xFE363C83, 0xFE363C84,
    0xF24A8080, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0x0003005B, (u32)killer_make_name, 0xFE363C8A, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry,
    0xFE363C80, 0xFE363C81, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C82,
    0xFE363C83, 0xFE363C84, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C82, 0xFE363C84,
    0xFE363C81, 0x0000001E, 0x00020035, 0xFE363C83, 0x0000000B, 0x0003005B,
    (u32)killer_make_name, 0xFE363C8A, 0xFE363C80, 0x0005005B, (u32)evt_npc_set_position, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xFE363C84, 0x0003005B, (u32)killer_make_name, 0xFE363C8A,
    0xFE363C80, 0x0003005B, (u32)evt_npc_set_anim, 0xFE363C80, (u32)(char*)str_KIL_S_1_802ed698, 0x0003005B,
    (u32)killer_make_name, 0xFE363C8A, 0xFE363C80, 0x0004005B, (u32)evt_npc_change_interrupt, 0xFE363C80,
    0x00000001, (u32)killer_move_event, 0x0003005B, (u32)killer_make_name, 0xFE363C8A, 0xFE363C80,
    0x0002005B, (u32)evt_npc_restart_regular_event, 0xFE363C80, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 sambo_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100010, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SNB_S_1_802ed6a0, 0x00010003, 0x00000000, 0x00010009, 0x00000001,
    0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 sambo_find_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SNB_S_1_802ed6a0, 0x000C005B, (u32)evt_eff_fukidashi,
    0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0008005B,
    (u32)evt_npc_homing_target, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0x00000000, 0xF24C0A80, 0xF24A9680,
    0xF24B6A80, 0x00000004, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 sambo_lost_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SNB_S_1_802ed6a0, 0x000C005B, (u32)evt_eff_fukidashi,
    0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 sambo_return_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SNB_S_1_802ed6a0, 0x0002005B, (u32)evt_set_dir_to_home,
    (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B,
    (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000, 0xF24C0A80,
    0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 sinnosuke_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000008, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100410, 0x0004005B, (u32)evt_npc_status_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_me_80421f30, 0x10000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000002, 0x00000001,
};

u32 sinnosuke_move_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00100410, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_SIS_Z_3_802ed6a8, 0x00010003, 0x00000000, 0x00010009, 0x00000001, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 sinnosuke_find_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIS_Z_3_802ed6a8, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00020032, 0xFE363C8F,
    0x00000000, 0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x00020035,
    0xFE363C8F, 0x0000001E, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C8F, 0x00000000, 0x0002001D, 0xFE363C8F, 0x0000010E, 0x00000007,
    0x00000021, 0x00000006, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIS_S_1_802ed6b0,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x0000010E, 0x00000000,
    0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x00020035, 0xFE363C8F,
    0x0000001E, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F,
    0x00000000, 0x0002001D, 0xFE363C8F, 0x00000168, 0x00000007, 0x00000021,
    0x00000006, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000000, 0x00010005, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_SIS_A_1a_802ed6b8, 0x0001000A, 0x0000012C, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30,
    0x0000000D, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C83,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000, 0xF24D9A80, 0x00000006,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIS_S_1_802ed6b0, 0x0003005B, (u32)evt_npc_set_attack_mode,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x0001000A, 0x000000C8, 0x00000006, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 sinnosuke_lost_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SIS_S_1_802ed6b0, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 sinnosuke_return_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SIS_W_1_802ed6c4, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x00000000, 0xF24AF280, 0x00000004, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SIS_S_1_802ed6b0, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00020032, 0xFE363C8F, 0x00000000, 0x00010005,
    0x00000000, 0x00010009, 0x00000001, 0x00020035, 0xFE363C8F, 0x0000001E,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F, 0x00000000,
    0x0002001D, 0xFE363C8F, 0x0000010E, 0x00000007, 0x00000021, 0x00000006,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIS_Z_3_802ed6a8, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x0000010E, 0x00000000, 0x00010005, 0x00000000,
    0x00010009, 0x00000001, 0x00020035, 0xFE363C8F, 0x0000001E, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F, 0x00000000, 0x0002001D,
    0xFE363C8F, 0x00000168, 0x00000007, 0x00000021, 0x00000006, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 sinemon_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000008, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100410, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00020000, 0x0004005B, (u32)evt_npc_status_onoff, 0x00000001,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000002, 0x00000001,
};

u32 sinemon_move_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00100410, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_me_80421f30, 0x00020000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_Z_3_802ed6cc,
    0x00010003, 0x00000000, 0x00010009, 0x00000001, 0x00010004, 0x00000000,
    0x00000002, 0x00000001,
};

u32 sinemon_find_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_Z_3_802ed6cc, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00020032, 0xFE363C8F,
    0x00000000, 0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x00020035,
    0xFE363C8F, 0x0000001E, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFE363C8F, 0x00000000, 0x0002001D, 0xFE363C8F, 0x0000010E, 0x00000007,
    0x00000021, 0x00000006, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_S_1_802ed6d4,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x0000010E, 0x00000000,
    0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x00020035, 0xFE363C8F,
    0x0000001E, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F,
    0x00000000, 0x0002001D, 0xFE363C8F, 0x00000168, 0x00000007, 0x00000021,
    0x00000006, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000000, 0x00010005, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_SIN_A_1a_802ed6dc, 0x0001000A, 0x0000012C, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30,
    0x0000000E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C83,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000, 0xF24D9A80, 0x00000006,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_S_1_802ed6d4, 0x0003005B, (u32)evt_npc_set_attack_mode,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x0001000A, 0x000000C8, 0x00000006, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 sinemon_lost_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_S_1_802ed6d4, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 sinemon_return_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_W_1_802ed6e8, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x00000000, 0xF24AF280, 0x00000004, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_S_1_802ed6d4, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00020032, 0xFE363C8F, 0x00000000, 0x00010005,
    0x00000000, 0x00010009, 0x00000001, 0x00020035, 0xFE363C8F, 0x0000001E,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F, 0x00000000,
    0x0002001D, 0xFE363C8F, 0x0000010E, 0x00000007, 0x00000021, 0x00000006,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_SIN_Z_3_802ed6cc, 0x0005005B, (u32)evt_npc_set_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x0000010E, 0x00000000, 0x00010005, 0x00000000,
    0x00010009, 0x00000001, 0x00020035, 0xFE363C8F, 0x0000001E, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8F, 0x00000000, 0x0002001D,
    0xFE363C8F, 0x00000168, 0x00000007, 0x00000021, 0x00000006, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 togedaruma_move_event[] = {
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0001005E, (u32)enemy_common_flag_init_event,
    0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x00010003, 0x00000000,
    0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80,
    0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82,
    0x000007D0, 0xF24B1A80, 0x00000004, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 togedaruma_find_event[] = {
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_attack_mode,
    (u32)(char*)str_me_80421f30, 0x0000000F, 0x0000006E, 0x00010005, 0x00000000, 0x00010009,
    0x00000001, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x0000002D,
    0x00000000, 0x00000006, 0x00000070, 0x0008005B, (u32)evt_npc_homing_target, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0x00000000, 0xF24C5A80, 0xF24AA280, 0xF24B6A80, 0x00000004,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 togedaruma_lost_event[] = {
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x000C005B, (u32)evt_eff_fukidashi,
    0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0002005B,
    (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 togedaruma_return_event[] = {
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30,
    0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82,
    0x00000000, 0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 barriern_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000,
    (u32)(char*)str_c_baria_802ed6f0, 0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    0xFE363C80, 0x20000000, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0x00000000,
    0xFFFFFC18, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00020000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x00000002,
    0x00000001,
};

u32 barriern_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00200000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00020000,
    0x00010003, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_3_80421f48,
    0x00010009, 0x0000003C, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44,
    0x00010009, 0x0000003C, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80,
    0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24C0A80, 0x0007005B, (u32)unk_8004d180, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0x00000032, 0x0000003C, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000000,
    0x00000000, 0x00000001, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 barriern2_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00200000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00020000,
    0x00010003, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_3_80421f48,
    0x00010009, 0x0000003C, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44,
    0x00010009, 0x0000003C, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80,
    0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24C0A80, 0x0007005B, (u32)unk_8004d180, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0x00000032, 0x0000003C, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000003E8, 0xF24C5A80, 0x00000000,
    0x00000000, 0x00000001, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 barriern_dead_event[] = {
    0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A, 0x0002005B,
    (u32)evt_snd_sfxoff, 0xFE363C8A, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598,
    0x00000080, 0x0001005E, (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 barriern_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000400, 0x00010003, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_A_2_80421f4c, 0x00010009, 0x0000001E, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_A_3_80421f50, 0x00010009, 0x0000000A, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_A_4_80421f54, 0x0000006E, 0x00020034, 0xFE363C8F, 0x00000000,
    0x00010005, 0x0000003C, 0x0002003A, 0xFE363C8F, 0xF24A7A9E, 0x0005005B,
    (u32)evt_npc_set_scale, (u32)(char*)str_slave_0_802ed598, 0x00000001, 0xFE363C8F, 0x00000001, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598,
    (u32)(char*)str_Z_3_80421f58, 0x0003005B, (u32)evt_npc_blur_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_FLD_ENM_BARRI_AT_802ed6f8, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_BARRI_AT_802ed714, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C8A,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A, 0x0004005B,
    (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF2524A80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598,
    0x00000011, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000004E2, 0x0000000A, 0x00000000, 0x00000000, 0x00000001,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30,
    0x00000000, 0xFE363C8A, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8A, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_3_80421f48, 0x00010009, 0x0000001E, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x00010009, 0x0000001E, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 barriern_lost_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_3_80421f48, 0x00010009, 0x0000003C,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x00010009, 0x0000003C,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 barriern_return_event[] = {
    0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30,
    0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24C0A80, 0x0007005B, (u32)unk_8004d180, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000001E, 0x00000032, 0x0000003C, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000000,
    0x00000000, 0x00000001, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 barriern_blow_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30,
    0x00000000, 0xFE363C8A, 0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C8A, 0x0001005E,
    (u32)enemy_common_blow_event, 0x00000002, 0x00000001,
};

u32 piders_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PAI_S_1_802ed748, 0x0000006E, 0x00010005, 0x00000000,
    0x00010009, 0x00000001, 0x0001005B, (u32)piders_draw_yawn, 0x00000006, 0x00000070,
    0x0000006E, 0x00010005, 0x00000000, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020032, 0xFE363C83, 0xFE363C81,
    0x00020036, 0xFE363C81, 0x0000000A, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000096, 0x00000000, 0x00000000,
    0x00000004, 0x00000000, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C83, 0xFE363C82, 0x00000096, 0x00000000, 0x00000000, 0x00000004,
    0x00000000, 0x00010009, 0x00000064, 0x00000006, 0x00000070, 0x0000006E,
    0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x00010005, 0x00000014, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000002, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00010005,
    0x00000000, 0x00010005, 0x00000028, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30,
    0x00000000, 0xFFFFFFFE, 0x00000000, 0x00010009, 0x00000001, 0x00000006,
    0x00010005, 0x00000028, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000002, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00000006,
    0x00000070, 0x00010003, 0x00000000, 0x00010009, 0x00000001, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 piders_find_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000400, 0x0000006E,
    0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x0001005B, (u32)piders_draw_yawn,
    0x00000006, 0x00000070, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x0000006B, 0x00010005, 0x0000000A, 0x0005005B,
    (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000006, 0x00000000, 0x00010009,
    0x00000001, 0x00000006, 0x00010005, 0x00000016, 0x0005005B, (u32)evt_npc_add_rotate,
    (u32)(char*)str_me_80421f30, 0x00000000, 0xFFFFFFFB, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x00010005, 0x00000014, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000004, 0x00000000, 0x00010009, 0x00000001, 0x00000006,
    0x00010005, 0x00000010, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000,
    0xFFFFFFFD, 0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00010005,
    0x0000000E, 0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000002,
    0x00000000, 0x00010009, 0x00000001, 0x00000006, 0x00010005, 0x0000000A,
    0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_me_80421f30, 0x00000000, 0xFFFFFFFF, 0x00000000,
    0x00010009, 0x00000001, 0x00000006, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000000, 0x00000000, 0x0000006D, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d,
    (u32)(char*)str_SFX_FLD_ENM_PAIDA_MO_802ed750, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0002005B,
    (u32)piders_get_height, 0xFE363C8F, 0x00020032, 0xFE363C8E, 0xFE363C8F, 0x00020038,
    0xFE363C8E, 0x00000003, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020032, 0xFE363C83, 0xFE363C81, 0x00020036,
    0xFE363C81, 0xFE363C8F, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0x00000000, 0x00000004,
    0x00000000, 0x00010009, 0x0000003C, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PAIDA_MO_802ed768,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00020035, 0xFE363C81,
    0xFE363C8E, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000002BC, 0x00000000, 0x00000014, 0x00000004, 0x00000000,
    0x00020035, 0xFE363C81, 0xFE363C8E, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PAIDA_MO_802ed768,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000002BC, 0x00000000,
    0x00000014, 0x00000004, 0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PAIDA_MO_802ed768,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C83, 0xFE363C82, 0x00000258, 0x00000000,
    0x00000000, 0x00000004, 0x00000000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 piders_blow_event[] = {
    0x0000006E, 0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x0001005B,
    (u32)piders_draw_yawn, 0x00000006, 0x00000070, 0x0001005E, (u32)enemy_common_blow_event, 0x00000002,
    0x00000001,
};

u32 pakkun_attack_event[] = {
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PAKKUN_A_802ed780, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_A_1A_802ed79c, 0x0001000A,
    0x000000C8, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_A_1B_802ed7a8, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B, (u32)evt_npc_set_attack_mode,
    (u32)(char*)str_slave_0_802ed598, 0x00000012, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x0000001E, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0001000A, 0x000000C8, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_PKF_A_1C_802ed7b4, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0001000A, 0x000000C8,
    0x00000002, 0x00000001,
};

u32 pakkun_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100018,
    0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000,
    (u32)(char*)str_c_pakflwr_802ed7c0, 0xFE363C80, (u32)enemy_slave_common_dead_event,
    0x0003005B, (u32)evt_npc_set_tribe, (u32)(char*)str_slave_0_802ed598,
    (u32)(char*)"\x83\x70\x83\x62\x83\x4E\x83\x93\x83\x74\x83\x89\x83\x8F\x81\x5B",
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18,
    0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598,
    0x00000020, 0x00000002, 0x00000001,
};

/* Recovered target .rodata ASCII string suffix. */
const char str_PKF_S_1_802ed7e0[] = "PKF_S_1";
const char str_SFX_FLD_ENM_PAKKUN_M_802ed7e8[] = "SFX_FLD_ENM_PAKKUN_MOVE1";
const char str_PKF_E_2_802ed804[] = "PKF_E_2";
const char str_SFX_FLD_ENM_PAKKUN_M_802ed80c[] = "SFX_FLD_ENM_PAKKUN_MOVE2";
const char str_PKF_E_1_802ed828[] = "PKF_E_1";
const char str_BSA_S_2_802ed830[] = "BSA_S_2";
const char str_c_zako_m_802ed838[] = "c_zako_m";
const char str_SFX_FLD_ENM_HANNYA_M_802ed844[] = "SFX_FLD_ENM_HANNYA_MOVE1";
const char str_MAH_S_2A_802ed860[] = "MAH_S_2A";
const char str_slave_1_802ed86c[] = "slave_1";
const char str_slave_2_802ed874[] = "slave_2";
const char str_slave_3_802ed87c[] = "slave_3";
const char str_KMK_Z_1_802ed884[] = "KMK_Z_1";
const char str_KMK_S_2_802ed88c[] = "KMK_S_2";
const char str_KMK_A_3A_802ed894[] = "KMK_A_3A";
const char str_KMK_A_3B_802ed8a0[] = "KMK_A_3B";
const char str_KMK_S_1_802ed8ac[] = "KMK_S_1";
const char str_KMK_A_2A_802ed8b4[] = "KMK_A_2A";
const char str_KMK_A_2B_802ed8c0[] = "KMK_A_2B";
const char str_BRO_A_1B_802ed8cc[] = "BRO_A_1B";
const char str_BRO_S_1_802ed8d8[] = "BRO_S_1";
const char str_BRO_A_1A_802ed8e0[] = "BRO_A_1A";
const u32 vec3_802ed8ec[3] = {0, 0, 0};
const u32 vec3_802ed8f8[3] = {0, 0, 0};
const u32 vec3_802ed904[3] = {0, 0, 0};
const char str_WAN_S_1B_802ed910[] = "WAN_S_1B";
const char str_WAN_Z_2_802ed91c[] = "WAN_Z_2";
const char str_WAN_S_1A_802ed924[] = "WAN_S_1A";
const char str_WAN_A_1_802ed930[] = "WAN_A_1";
const char str_SFX_FLD_ENM_WAN_MOVE_802ed938[] = "SFX_FLD_ENM_WAN_MOVE1";
const char str_SFX_FLD_ENM_WAN_LAND_802ed950[] = "SFX_FLD_ENM_WAN_LANDING1";
const char str_SFX_FLD_ENM_WAN_MOVE_802ed96c[] = "SFX_FLD_ENM_WAN_MOVE2";
const char str_SFX_FLD_ENM_WAN_ATTA_802ed984[] = "SFX_FLD_ENM_WAN_ATTACK2";
const u32 vec3_802ed99c[3] = {0, 0, 0};
const u32 vec3_802ed9b4[3] = {0, 0, 0};
const char str_c_zako_km_802ed9cc[] = "c_zako_km";
const char str_SFX_CONDITION_FIRE1_802ed9d8[] = "SFX_CONDITION_FIRE1";
const char str_SFX_ENM_3RD2_CRASH1_802ed9ec[] = "SFX_ENM_3RD2_CRASH1";
const char str_glass_n64_802eda00[] = "glass_n64";
const char str_SFX_KUPPA_GESSO_MOVE_802eda0c[] = "SFX_KUPPA_GESSO_MOVE1";
const char str_jp_802eda24[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x92\x6E\x93\x5F\x82\x50";
const char str_jp_802eda38[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x82\x50";
const char str_jp_802eda48[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x92\x6E\x93\x5F\x82\x51\x81\x51\x82\x50";
const char str_jp_802eda60[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x82\x51";
const char str_jp_802eda70[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x82\x52";
const char str_jp_802eda80[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x92\x6E\x93\x5F\x82\x51\x81\x51\x82\x51";
const char str_jp_802eda98[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x82\x53";
const char str_jp_802edaa8[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x82\x54";
const char str_jp_802edab8[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x8D\x7E\x89\xBA\x82\x55";
const char str_jp_802edac8[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x82\xAA\x82\xC5\x82\xE9\x93\x79\x8A\xC7\x82\x50";
const char str_jp_802edae0[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x82\xAA\x82\xC5\x82\xE9\x93\x79\x8A\xC7\x82\x51";
const char str_mobj14_802edaf8[] = "mobj14";
const char str_jp_802edb00[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x93\x79\x8A\xC7\x82\x50";
const char str_jp_802edb10[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x93\x79\x8A\xC7\x82\x51";
const char str_jp_802edb20[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x93\x79\x8A\xC7\x82\x52";
const char str_MOBJ_mobj9_802edb30[] = "MOBJ_mobj9";
const char str_jp_802edb3c[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x93\x79\x8A\xC7\x82\x53";
const char str_jp_802edb4c[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x93\x79\x8A\xC7\x82\x54";
const char str_jp_802edb5c[] = "\x8C\x52\x92\x63\x83\x55\x83\x52\x93\x79\x8A\xC7\x82\x55";
const char str_MOBJ_mobj14_802edb6c[] = "MOBJ_mobj14";

u32 pakkun_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_S_1_802ed7e0, 0x00010003, 0x00000000, 0x00010009,
    0x00000001, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 pakkun_find_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PAKKUN_M_802ed7e8, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000020, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_E_2_802ed804, 0x00010009, 0x0000000A, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)evt_sub_random, 0x0000000A, 0xFE363C83, 0x0002005B, (u32)evt_mario_get_mov_spd, 0xFE363C84,
    0x0002003C, 0xFE363C84, 0xF24A7C80, 0x0002003A, 0xFE363C83, 0xFE363C84,
    0x0002003A, 0xFE363C83, 0x0000001E, 0x0004005B, (u32)evt_npc_facedirection_add, (u32)(char*)str_mario_80421f34,
    0xFE363C80, 0xFE363C83, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x000000C8, 0x00000000, 0x00000004, 0x0004005B, (u32)evt_get_target_dir,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PAKKUN_M_802ed80c, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_E_1_802ed828,
    0x00010009, 0x0000000A, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000020, 0x00010009, 0x00000014, 0x0001005E, (u32)pakkun_attack_event, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 pakkun_return_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_PAKKUN_M_802ed80c, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_E_1_802ed828, 0x00010009, 0x0000000A,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000020, 0x00010009,
    0x00000014, 0x0001005E, (u32)pakkun_attack_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_me_80421f30, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_E_2_802ed804,
    0x00010009, 0x0000000A, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C82, 0x000000C8, 0x00000000, 0x00000004, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_PKF_E_1_802ed828, 0x00010009, 0x0000000A, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000020, 0x00010009, 0x00000019, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 dokugassun_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x10000000,
    0x00000002, 0x00000001,
};

u32 dokugassun_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x00010003, 0x00000000, 0x0001000A,
    0x000001F4, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80,
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000001,
    0xF24A9280, 0x00000000, 0x00000002, 0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000001F4, 0x00000001, 0xF24A6280, 0x00000000, 0x00000002, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000001F4, 0x00000001, 0xF24A9280, 0x00000000, 0x00000002, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000000A, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000001F4, 0x00000001, 0xF24A9280, 0x00000000, 0x00000002, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 dokugassun_find_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_me_80421f30, 0x00000400, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000001E, 0x0001000A, 0x000000C8, 0x0004005B,
    (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C81, 0xFE363C83, 0xFE363C80, 0xF24D9A80, 0x0006005B, (u32)evt_fbat_trans_floor_position,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0000000A, 0x000001F4, 0x0003005B,
    (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x000003E8, 0x00000001, 0x00000000,
    0x00000004, 0x00000001, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 dokugassun_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 dokugassun_return_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0001000A, 0x000001F4, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_W_1_80421f60, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30,
    0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002005B, (u32)evt_npc_sound_data_reset,
    (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000A, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000001, 0xF24A6280, 0x00000000,
    0x00000002, 0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000001,
    0xF24A9280, 0x00000000, 0x00000002, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000001,
    0xF24A6280, 0x00000000, 0x00000002, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0000000A, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000001,
    0xF24A9280, 0x00000000, 0x00000002, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 basabasa_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100418, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00020000, 0x00000002, 0x00000001,
};

u32 basabasa_move_event[] = {
    0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C83, 0xFE363C84, 0xFE363C85,
    0x0002001A, 0xFE363C84, 0xFE363C81, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30,
    0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30,
    0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C84, 0xFE363C81,
    0xFE363C85, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF249B280, 0x00000000, 0x00000002,
    0x00000021, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000,
    0x00010003, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_BSA_S_2_802ed830,
    0x00010009, 0x0000003C, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 basabasa_find_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B,
    (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C84,
    0xFE363C85, 0xFE363C86, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C81, 0xFE363C83,
    0xFE363C80, 0xF24C0A80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C85, 0xFE363C83,
    0x00000000, 0x0000004B, 0xF2484A80, 0x00000000, 0x00000000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 basabasa_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 basabasa_return_event[] = {
    0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30,
    0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0005005B, (u32)evt_npc_get_home_position,
    (u32)(char*)str_me_80421f30, 0xFE363C84, 0xFE363C81, 0xFE363C85, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF249B280, 0x00000000, 0x00000002, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 basabasa2_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x00010003, 0x00000000, 0x0002005B,
    (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim,
    (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000050,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A5280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000050,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24AA280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000050,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A5280, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000050,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24AA280, 0x00000000, 0x00000003,
    0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 basabasa2_find_event[] = {
    0x0002005B, (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0004005B, (u32)evt_get_target_dir,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C81,
    0xFE363C83, 0xFE363C80, 0xF24C0A80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00000050, 0x000001F4, 0x0003005B, (u32)evt_npc_set_ry,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00000000, 0x0000004B, 0xF2493A80, 0x00000000,
    0x00000000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 basabasa2_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 basabasa2_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000050, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24A5280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000050, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24AA280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000050, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24A5280, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000050, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000,
    0xF24AA280, 0x00000000, 0x00000003, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 teresa_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000020, 0x0006005B, (u32)evt_npc_set_color, (u32)(char*)str_me_80421f30,
    0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 0x0003005B, (u32)evt_sub_random,
    0x00000168, 0xFE363C83, 0x00010003, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30,
    0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000032, 0xF24A5280, 0x00000000, 0x00000003, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000032, 0xF24AA280, 0x00000000, 0x00000003, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000032, 0xF24A5280, 0x00000000, 0x00000003, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000032, 0xF24AA280, 0x00000000, 0x00000003, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 teresa_find_event[] = {
    0x0002005B, (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0004005B, (u32)evt_get_target_dir,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C81,
    0xFE363C83, 0xFE363C80, 0xF24C0A80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00000000, 0x0000004B, 0xF249DA80, 0x00000000,
    0x00000000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 teresa_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 teresa_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_me_80421f30, 0x00200000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x00000020, 0x00020032, 0xFE363C80, 0x000000FF, 0x00010005, 0x00000010,
    0x0006005B, (u32)evt_npc_set_color, (u32)(char*)str_me_80421f30, 0x000000FF, 0x000000FF, 0x000000FF,
    0xFE363C80, 0x00020036, 0xFE363C80, 0x00000010, 0x0001000A, 0x0000001E,
    0x00000006, 0x0006005B, (u32)evt_npc_set_color, (u32)(char*)str_me_80421f30, 0x000000FF, 0x000000FF,
    0x000000FF, 0x00000000, 0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_home_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020032, 0xFE363C80,
    0x00000000, 0x00010005, 0x00000010, 0x00020035, 0xFE363C80, 0x00000010,
    0x0002001B, 0xFE363C80, 0x000000FF, 0x00020032, 0xFE363C80, 0x000000FF,
    0x00000021, 0x0006005B, (u32)evt_npc_set_color, (u32)(char*)str_me_80421f30, 0x000000FF, 0x000000FF,
    0x000000FF, 0xFE363C80, 0x0001000A, 0x0000001E, 0x00000006, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000020, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 bubble_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x00010003, 0x00000000, 0x0002005B,
    (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_sound_data_reset,
    (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_get_loiter_dir,
    0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24A6680, 0x00000000, 0x00000003, 0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000032, 0xF24A8E80, 0x00000000, 0x00000003, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000032, 0xF24A6680, 0x00000000, 0x00000003, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B,
    (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000032, 0xF24A8E80, 0x00000000, 0x00000003, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 bubble_find_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C81, 0xFE363C83, 0xFE363C80, 0xF24C0A80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0006005B, (u32)evt_fbat_trans_floor_position,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000014, 0x000001F4, 0x000A005B,
    (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000000,
    0x0000005A, 0xF24A7A80, 0x00000000, 0x00000002, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 bubble_lost_event[] = {
    0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 bubble_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0002005B, (u32)evt_npc_sound_data_reset, (u32)(char*)str_me_80421f30,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24A6680, 0x00000000, 0x00000003,
    0x0007005B, (u32)evt_npc_sound_data_set, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A8E80,
    0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A6680,
    0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A8E80,
    0x00000000, 0x00000003, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 hbom_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x00010003, 0x00000000, 0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir,
    0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24ACA80, 0x00000004, 0x0001000A,
    0x000003E8, 0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 hbom_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0000012C, 0x00000000,
    0xF24ACA80, 0x00000000, 0x0008005B, (u32)evt_npc_homing_target, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34,
    0x00000000, 0xF24B6A80, 0xF24AA280, 0xF24B6A80, 0x00000004, 0x0001000A,
    0x000001F4, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 hbom_lost_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 hbom_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24AF280, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 zakowiz_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000,
    (u32)(char*)str_c_zako_m_802ed838, 0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    0xFE363C80, 0x20000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80,
    0x00000020, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_B_1A_80421f64, 0x0003005B,
    (u32)evt_npc_set_width, (u32)(char*)str_slave_0_802ed598, 0x0000000A, 0x0003005B, (u32)evt_npc_set_height, (u32)(char*)str_slave_0_802ed598,
    0x0000000A, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000001,
    0x00000002, 0x00000001,
};

u32 zakowiz_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598,
    0x00000080, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00200000, 0x00010003, 0x00000000, 0x0001000A, 0x000001F4,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B,
    (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B,
    (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24B1A80,
    0x00000004, 0x0002005B, (u32)npc_check_wall_stop, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x00000021,
    0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 zakowiz_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_A_1B_80421f6c, 0x0001000A, 0x000001F4, 0x0000006E, 0x0001000A, 0x000002BC,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x00000070, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x00020035, 0xFE363C81, 0x00000019,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_B_1A_80421f64, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x0000000C,
    0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000003E8, 0x00000000, 0xF24BA680, 0x00000000, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B, (u32)evt_eff,
    (u32)(char*)zero_80421f3c, (u32)str_kemuri_test_802ed438, 0x00000010, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0xF24A7E80, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_B_2_80421f74, 0x0001000A, 0x0000012C, 0x0005005B,
    (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 zakowiz_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 zakowiz_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 zakowiz_blow_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0x00000000, 0xFFFFFC18, 0x00000000, 0x0001005E, (u32)enemy_common_blow_event, 0x00000002,
    0x00000001,
};

u32 hannya_move_event[] = {
    0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x00010003, 0x00000000,
    0x0001000A, 0x000003E8, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80,
    0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82,
    0x000007D0, 0xF24B1A80, 0x00000004, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 hannya_find_event[] = {
    0x0002005B, (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0004005B, (u32)evt_get_target_dir,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_HANNYA_M_802ed844,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000000, 0x0005005B, (u32)evt_npc_get_home_position,
    (u32)(char*)str_me_80421f30, 0xFE363C84, 0xFE363C85, 0xFE363C86, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C81, 0xFE363C83, 0xFE363C80, 0xF24C0A80, 0x0003005B, (u32)evt_npc_set_attack_mode,
    (u32)(char*)str_me_80421f30, 0x0000000B, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_A_3_80421f50, 0x0007005B,
    (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C83, 0x00000000, 0xF24D4A80,
    0x00000006, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000400,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 hannya_lost_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 hannya_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000002, 0x0005005B, (u32)evt_npc_get_home_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)evt_npc_move_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000BB8, 0xF24B1A80, 0x00000004,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 mahoon_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0002005B, (u32)mahoon_get_groupname, 0xFE363C8A, 0x0006005B,
    (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_MAH_S_2A_802ed860, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0006005B, (u32)evt_npc_slave_entry,
    (u32)(char*)str_me_80421f30, 0x00000001, 0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_slave_1_802ed86c, (u32)(char*)str_MAH_S_2A_802ed860, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_1_802ed86c,
    0x00000000, 0xFFFFFC18, 0x00000000, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30,
    0x00000002, 0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_slave_2_802ed874, (u32)(char*)str_MAH_S_2A_802ed860, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_2_802ed874, 0x00000000,
    0xFFFFFC18, 0x00000000, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000003,
    0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_slave_3_802ed87c,
    (u32)(char*)str_MAH_S_2A_802ed860, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_3_802ed87c, 0x00000000, 0xFFFFFC18,
    0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00020000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00020000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_1_802ed86c, 0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_slave_2_802ed874, 0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_slave_3_802ed87c, 0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598,
    0x00000020, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_1_802ed86c, 0x00000020,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_2_802ed874, 0x00000020, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_3_802ed87c, 0x00000020, 0x00000002, 0x00000001,
};

u32 mahoon_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0000006E,
    0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x0003005B, (u32)evt_npc_get_dir,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_0_802ed598, 0xFE363C83,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_1_802ed86c, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry,
    (u32)(char*)str_slave_2_802ed874, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_3_802ed87c, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x00020035, 0xFE363C81, 0x00000005, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81, 0x00000005,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_1_802ed86c, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x00020035, 0xFE363C81, 0x0000001E, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_2_802ed874,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81, 0x0000001E,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_3_802ed87c, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000006, 0x00000070, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x00010003, 0x00000000, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_get_loiter_dir,
    0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24A6680, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24A8E80, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24A6680, 0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000014, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x00000032,
    0xF24A8E80, 0x00000000, 0x00000003, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 mahoon_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_1_802ed86c, 0x00000080, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_2_802ed874, 0x00000080, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_slave_3_802ed87c, 0x00000080, 0x0001005E, (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 mahoon_find_event[] = {
    0x0000006E, 0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_0_802ed598,
    0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_1_802ed86c, 0xFE363C83, 0x0003005B,
    (u32)evt_npc_set_ry, (u32)(char*)str_slave_2_802ed874, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_3_802ed87c,
    0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x00020035, 0xFE363C81, 0x00000005, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81,
    0x00000005, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_1_802ed86c, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x00020035, 0xFE363C81, 0x0000001E, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_2_802ed874, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81,
    0x0000001E, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_3_802ed87c, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000006, 0x00000070, 0x0002005B, (u32)evt_npc_set_run_anim, (u32)(char*)str_me_80421f30,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C84, 0xFE363C85, 0xFE363C86,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C81, 0xFE363C83, 0xFE363C80, 0xF24C0A80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000014,
    0x000001F4, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x000A005B,
    (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0x00000000,
    0x00000087, 0xF24A7A80, 0x00000000, 0x00000000, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 mahoon_lost_event[] = {
    0x0000006E, 0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_0_802ed598,
    0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_1_802ed86c, 0xFE363C83, 0x0003005B,
    (u32)evt_npc_set_ry, (u32)(char*)str_slave_2_802ed874, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_3_802ed87c,
    0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x00020035, 0xFE363C81, 0x00000005, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81,
    0x00000005, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_1_802ed86c, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x00020035, 0xFE363C81, 0x0000001E, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_2_802ed874, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81,
    0x0000001E, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_3_802ed87c, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000006, 0x00000070, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000003E8, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 mahoon_return_event[] = {
    0x0000006E, 0x00010005, 0x00000000, 0x00010009, 0x00000001, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_0_802ed598,
    0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_1_802ed86c, 0xFE363C83, 0x0003005B,
    (u32)evt_npc_set_ry, (u32)(char*)str_slave_2_802ed874, 0xFE363C83, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_slave_3_802ed87c,
    0xFE363C83, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x00020035, 0xFE363C81, 0x00000005, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81,
    0x00000005, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_1_802ed86c, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83,
    0xF24ADE80, 0x00020035, 0xFE363C81, 0x0000001E, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_2_802ed874, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24A1680, 0x00020035, 0xFE363C81,
    0x0000001E, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_3_802ed87c, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000006, 0x00000070, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24A6680, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24A8E80, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24A6680, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000014,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000001F4, 0x00000000, 0xF24A8E80, 0x00000000, 0x00000003,
    0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 kamec_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x0002005B, (u32)mahoon_get_groupname,
    0xFE363C8A, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A,
    0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80,
    0x20000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x00000020,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, 0xFE363C80, 0x00020000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_KMK_Z_1_802ed884, 0x0003005B, (u32)evt_npc_set_width, (u32)(char*)str_slave_0_802ed598,
    0x0000000A, 0x0003005B, (u32)evt_npc_set_height, (u32)(char*)str_slave_0_802ed598, 0x0000000A, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x00000002, 0x00000001,
};

u32 kamec_move_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598,
    0x00000020, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KMK_S_2_802ed88c, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x00010003, 0x00000000,
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0004005B, (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80,
    0xF24E3A80, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A6680, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A8E80, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A6680, 0x00000000, 0x00000003,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ADE80,
    0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000032,
    0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00000032, 0xF24A8E80, 0x00000000, 0x00000003,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000400, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 kamec_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_KMK_A_3A_802ed894, 0x0001000A, 0x000002BC, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_KMK_A_3B_802ed8a0, 0x0001000A, 0x00000096, 0x0000006E, 0x0001000A, 0x000003E8,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KMK_S_2_802ed88c, 0x00000070, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24ACA80, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C83, 0xFE363C84, 0xFE363C85, 0x00020035, 0xFE363C84, 0x00000014,
    0x0008005B, (u32)kamec_fire_magic, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0xFE363C85, 0xFE363C8A, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000014,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x0000000C, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_slave_0_802ed598,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000003E8, 0x00000000, 0xF24A7A80,
    0x00000000, 0x00000003, 0x0002005B, (u32)kamec_fire_magic_delete, 0xFE363C8A, 0x0003005B,
    (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0x00000000,
    0xFFFFFC18, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 kamec_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000001, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 kamec_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_npc_set_walk_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B,
    (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000400, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A6680,
    0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A8E80,
    0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A6680,
    0x00000000, 0x00000003, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ADE80, 0x0006005B, (u32)evt_fbat_trans_floor_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000032, 0x000001F4, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000001F4, 0x00000000, 0xF24A8E80,
    0x00000000, 0x00000003, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 kamec_blow_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0x00000000, 0xFFFFFC18, 0x00000000, 0x0001005E, (u32)enemy_common_blow_event, 0x00000002,
    0x00000001,
};

u32 kamec2_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100418, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x0002005B, (u32)mahoon_get_groupname,
    0xFE363C8A, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A,
    0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80,
    0x20000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x00000020,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, 0xFE363C80, 0x00020000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_KMK_Z_1_802ed884, 0x0003005B, (u32)evt_npc_set_width, (u32)(char*)str_slave_0_802ed598,
    0x0000000A, 0x0003005B, (u32)evt_npc_set_height, (u32)(char*)str_slave_0_802ed598, 0x0000000A, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x00000002, 0x00000001,
};

u32 kamec2_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_KMK_S_1_802ed8ac, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00200000, 0x00010003, 0x00000000, 0x0001000A, 0x000001F4, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_get_loiter_dir, 0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist,
    0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000004,
    0x00010004, 0x00000000, 0x00000002, 0x00000001,
};

u32 kamec2_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_KMK_A_2A_802ed8b4, 0x0001000A, 0x000002BC, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_KMK_A_2B_802ed8c0, 0x0001000A, 0x00000096, 0x0000006E, 0x0001000A, 0x000003E8,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_KMK_S_1_802ed8ac, 0x00000070, 0x0005005B,
    (u32)evt_npc_set_rotate, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x00000000, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x0000001E, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24ACA80,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C83, 0xFE363C84, 0xFE363C85,
    0x00020035, 0xFE363C84, 0x00000014, 0x0008005B, (u32)kamec_fire_magic, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0xFE363C85, 0xFE363C8A,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020035, 0xFE363C81, 0x00000014, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x0000000C,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000003E8, 0x00000000, 0xF24A7A80, 0x00000000, 0x00000003, 0x0002005B,
    (u32)kamec_fire_magic_delete, 0xFE363C8A, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B,
    (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 kamec2_lost_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_reverse_ry,
    (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 kamec2_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 kamec2_blow_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0x00000000, 0xFFFFFC18, 0x00000000, 0x0001005E, (u32)enemy_common_blow_event, 0x00000002,
    0x00000001,
};

u32 hbross_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0002005B, (u32)mahoon_get_groupname, 0xFE363C8A, 0x0006005B,
    (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x20000000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x00000020, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_BRO_A_1B_802ed8cc, 0x0003005B, (u32)evt_npc_set_width, (u32)(char*)str_slave_0_802ed598, 0x0000000A,
    0x0003005B, (u32)evt_npc_set_height, (u32)(char*)str_slave_0_802ed598, 0x0000000A, 0x00000002, 0x00000001,
};

u32 hbross_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598,
    0x00000080, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_BRO_S_1_802ed8d8, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00200000, 0x0003005B, (u32)evt_sub_random, 0x00000168, 0xFE363C83,
    0x00010003, 0x00000000, 0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_get_loiter_dir,
    0xFE363C83, 0xF24C5A80, 0xF24E3A80, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0xF24C0A80, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x000007D0, 0xF24B1A80, 0x00000004, 0x00010004,
    0x00000000, 0x00000002, 0x00000001,
};

u32 hbross_find_event[] = {
    0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0000001E, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80,
    0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0000006E, 0x00010003, 0x00000000,
    0x0005005B, (u32)evt_npc_add_rotate, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x00000000, 0x0000001E,
    0x00010009, 0x00000001, 0x00010004, 0x00000000, 0x00000070, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_BRO_A_1A_802ed8e0, 0x0001000A, 0x00000190, 0x0000006E,
    0x0001000A, 0x000001F4, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_BRO_S_1_802ed8d8,
    0x00000070, 0x0005005B, (u32)evt_npc_set_rotate, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x00000000,
    0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C81, 0x00000019, 0x0003005B, (u32)evt_npc_get_dir,
    (u32)(char*)str_me_80421f30, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82,
    0xFE363C83, 0xF24ACA80, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598,
    0x00000080, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000020,
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x0000000C, 0x0009005B, (u32)evt_npc_jump_position,
    (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000003E8, 0x00000000,
    0xF24BA680, 0x00000000, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B,
    (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000, 0x0002005B, (u32)evt_npc_return_interrupt,
    (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 hbross_lost_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x0001000A, 0x000001F4, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0001000A,
    0x000003E8, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 hbross_return_event[] = {
    0x0002005B, (u32)evt_npc_set_stay_anim, (u32)(char*)str_me_80421f30, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)evt_set_dir_to_home, (u32)(char*)str_me_80421f30, 0x0003005B, (u32)evt_npc_get_dir, (u32)(char*)str_me_80421f30, 0xFE363C83,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0xF24C0A80,
    0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000000,
    0xF24B1A80, 0x00000004, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002,
    0x00000001,
};

u32 hbross_blow_event[] = {
    0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_slave_0_802ed598, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0x00000000, 0xFFFFFC18, 0x00000000, 0x0001005E, (u32)enemy_common_blow_event, 0x00000002,
    0x00000001,
};

u32 wanwan_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0002005B, (u32)mahoon_get_groupname, 0xFE363C8A, 0x0006005B, (u32)evt_npc_slave_entry,
    (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0003005B,
    (u32)evt_npc_set_anim, 0xFE363C80, (u32)(char*)str_WAN_S_1B_802ed910, 0x0001005E, (u32)wanwan_init_event_sub, 0x0006005B,
    (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000001, 0xFE363C8A, 0xFE363C80, (u32)enemy_slave_common_dead_event,
    0x0003005B, (u32)evt_npc_set_anim, 0xFE363C80, (u32)(char*)str_WAN_S_1B_802ed910, 0x0001005E, (u32)wanwan_init_event_sub,
    0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000002, 0xFE363C8A, 0xFE363C80,
    (u32)enemy_slave_common_dead_event, 0x0003005B, (u32)evt_npc_set_anim, 0xFE363C80, (u32)(char*)str_WAN_S_1B_802ed910, 0x0001005E,
    (u32)wanwan_init_event_sub, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000003, 0xFE363C8A,
    0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0003005B, (u32)evt_npc_set_anim, 0xFE363C80, (u32)(char*)str_WAN_Z_2_802ed91c,
    0x0001005E, (u32)wanwan_init_event_sub, 0x00000002, 0x00000001,
};

u32 wanwan_init_event_sub[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x00000020, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, 0xFE363C80, 0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, 0xFE363C80, 0x00000010, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    0xFE363C80, 0x00000200, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, 0xFE363C80,
    0x00000400, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, 0xFE363C80, 0x00000800,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x01000000, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80, 0x00200000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, 0xFE363C80, 0x00000040, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    0xFE363C80, 0x00040000, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x0005005B, (u32)evt_npc_set_position, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xFE363C83, 0x00000002, 0x00000001,
};

u32 wanwan_move_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000400, 0x0003005B, (u32)evt_npc_set_attack_mode, (u32)(char*)str_me_80421f30, 0x00000000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_WAN_S_1A_802ed924, 0x0000006E, 0x00010005, 0x00000000,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x0002001D, 0xFE363C80, 0xFE363C8A, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30,
    0x0000010E, 0x00000020, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0x0000005A,
    0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0000006E,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0001000A,
    0x000005DC, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00200000,
    0x00000070, 0x0000006E, 0x00010005, 0x00000000, 0x0001005B, (u32)chain_main,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0000006E, 0x00010005,
    0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_WAN_A_1_802ed930, 0x0001000A,
    0x000003E8, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_WAN_S_1A_802ed924, 0x0001000A,
    0x000001F4, 0x00000006, 0x00000070, 0x00010003, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0003005B,
    (u32)evt_sub_random, 0x00000168, 0xFE363C83, 0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80,
    0xFE363C82, 0xFE363C83, 0x00000032, 0x0003005B, (u32)evt_sub_random, 0x0000000F,
    0xFE363C84, 0x00020035, 0xFE363C84, 0x0000000F, 0x00020032, 0xFE363C85,
    0xFE363C84, 0x00020032, 0xFE363C86, 0xFE363C84, 0x00020037, 0xFE363C86,
    0x00000011, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_WAN_MOVE_802ed938, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0xFE363C86, 0x00000050, 0xFE363C85, 0x00000001,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_WAN_LAND_802ed950, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00010009, 0x00000001, 0x00010004, 0x00000000, 0x00000002,
    0x00000001,
};

u32 wanwan_find_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_WAN_A_1_802ed930, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x0003005B, (u32)evt_npc_set_attack_mode,
    (u32)(char*)str_me_80421f30, 0x0000000C, 0x0000006E, 0x00010005, 0x00000000, 0x0001005B,
    (u32)chain_main, 0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_WAN_MOVE_802ed96c, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_FLD_ENM_WAN_ATTA_802ed984, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0005005B, (u32)evt_npc_get_home_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C83,
    0x0005005B, (u32)evt_npc_add_dirdist, 0xFE363C80, 0xFE363C82, 0xFE363C83, 0x00000064,
    0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x00000258, 0xF24A7A80, 0x00000000, 0x00000006, 0x0001000A,
    0x000005DC, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 kuriboo2D_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x08000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000004, 0x0002005B,
    (u32)limitSpd, 0xF250BA80, 0x00000002, 0x00000001,
};

u32 kuriboo2D_move_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x00010003, 0x00000000,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00001000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0002005B,
    (u32)camGetPos, 0xFE363C81, 0x00020036, 0xFE363C80, 0xFE363C81, 0x0002001C,
    0xFE363C80, 0x000000FA, 0x00010004, 0x00000001, 0x00000021, 0x00010009,
    0x00000001, 0x00010004, 0x00000000, 0x00010003, 0x00000001, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_W_1_80421f60, 0x00010003, 0x00000002, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x000003E8, 0x0002005B, (u32)getSpdWalk,
    0xFE363C83, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82,
    0x00000000, 0xFE363C83, 0x00000004, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x00010009, 0x00000001, 0x00010004, 0x00000002, 0x00000002, 0x00000001,
};

u32 kuriboo2D_find_event[] = {
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0002001D, 0xFE363C81, 0xFFFFFF9C, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x0000001E, 0x00000021, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_R_1_80421f78, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0x000003E8, 0x0002005B, (u32)getSpdRun, 0xFE363C83, 0x0007005B,
    (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82, 0x00000FA0, 0xFE363C83,
    0x00000002, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 zako2D_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x08000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000004, 0x0002005B, (u32)limitSpd,
    0xF250BA80, 0x00000002, 0x00000001,
};

u32 zako2D_move_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x00010003, 0x00000000, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00001000, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0002005B, (u32)camGetPos,
    0xFE363C81, 0x00020036, 0xFE363C80, 0xFE363C81, 0x0002001C, 0xFE363C80,
    0x000000FA, 0x00010004, 0x00000001, 0x00000021, 0x00010009, 0x00000001,
    0x00010004, 0x00000000, 0x00010003, 0x00000001, 0x0004005B, (u32)evt_npc_get_unitwork,
    (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_Z_1_80421f7c, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000010, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00020000, 0x00010005, 0x00000023,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00001000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035,
    0xFE363C81, 0x00000001, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00010009, 0x00000001, 0x00000006, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000010, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00020000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_W_1_80421f60, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x00000032,
    0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x000001F4, 0xF24C0A80, 0xF24AF280, 0x00000001, 0x00000021, 0x0004005B,
    (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000002, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00020000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFFFFFF9C, 0xFE363C82, 0x00000000, 0xF24C0A80, 0xF24A7A80,
    0x00000001, 0x00000021, 0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00001000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_W_1_80421f60, 0x00010003,
    0x0000000A, 0x0001006F, 0xFE363C8F, 0x00010005, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x000003E8, 0x0002005B, (u32)getSpdWalk,
    0xFE363C83, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82,
    0x00000000, 0xFE363C83, 0x00000004, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30,
    0x00010009, 0x00000001, 0x00000006, 0x00000070, 0x00010003, 0x0000000B,
    0x0002005B, (u32)cliffCheck, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x0001005F, 0xFE363C8F, 0x0002005B, (u32)evt_npc_reverse_ry, (u32)(char*)str_me_80421f30, 0x00010009,
    0x00000001, 0x00010004, 0x0000000A, 0x00000021, 0x00010009, 0x00000001,
    0x00010004, 0x0000000B, 0x00000002, 0x00000001,
};

u32 zako2D_find_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0002001D,
    0xFE363C81, 0xFFFFFF9C, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000001, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000001E, 0x00000021, 0x0003005B, (u32)evt_npc_set_anim,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_A_1_80421f40, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34,
    0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x000003E8, 0x0002005B, (u32)getSpdRun,
    0xFE363C83, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82,
    0x00000BB8, 0xFE363C83, 0x00000002, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_S_1_80421f44, 0x00010003, 0x00000063, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 dokan2D_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x000000A0,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000004, 0x00000002,
    0x00000001,
};

u32 dokan2D_regl_event[] = {
    0x00010003, 0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xF1194D80, 0xF1194D80, 0x0002005B, (u32)camGetPos, 0xFE363C81, 0x00020036,
    0xFE363C80, 0xFE363C81, 0x0002001C, 0xFE363C80, 0x000000FA, 0x00010004,
    0x00000001, 0x00000021, 0x00010009, 0x00000001, 0x00010004, 0x00000000,
    0x00010003, 0x00000001, 0x0002005B, (u32)dokanCheck, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x00010004, 0x00000063, 0x00000021, 0x0001005B,
    (u32)zakoEntryDokan, 0x00010009, 0x00000078, 0x00010004, 0x00000001, 0x00010003,
    0x00000063, 0x0002005B, (u32)evt_npc_delete, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 fall2D_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x000000A0,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000004, 0x00000002,
    0x00000001,
};

u32 fall2D_regl_event[] = {
    0x00010003, 0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xF1194D80, 0xF1194D80, 0x0002005B, (u32)camGetPos, 0xFE363C81, 0x00020036,
    0xFE363C80, 0xFE363C81, 0x0002001C, 0xFE363C80, 0x000000FA, 0x00010004,
    0x00000001, 0x00000021, 0x00010009, 0x00000001, 0x00010004, 0x00000000,
    0x00010003, 0x00000001, 0x0001005B, (u32)zakoEntryFall, 0x00010009, 0x00000078,
    0x00010004, 0x00000001, 0x00000002, 0x00000001,
};

u32 zakoM2D_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x08000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000004, 0x0002005B,
    (u32)limitSpd, 0xF250BA80, 0x0006005B, (u32)evt_npc_slave_entry, (u32)(char*)str_me_80421f30, 0x00000000,
    (u32)(char*)str_c_zako_km_802ed9cc, 0xFE363C80, (u32)enemy_slave_common_dead_event, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598,
    0x00000000, 0xFFFFFC18, 0x00000000, 0x0001005B, (u32)set_bottle_flag_init, 0x00000002,
    0x00000001,
};

u32 zakoM2D_move_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x00010003, 0x00000002,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00001000, 0x0004005B,
    (u32)evt_npc_status_check, (u32)(char*)str_me_80421f30, 0x00000004, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x00010004, 0x00000003, 0x00000021, 0x00010009, 0x00000001,
    0x00010004, 0x00000002, 0x00010003, 0x00000003, 0x0000006E, 0x00010005,
    0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C82,
    0x00000064, 0x00000000, 0x00000000, 0x00010009, 0x00000001, 0x00000006,
    0x00000070, 0x00010003, 0x00000000, 0x0004005B, (u32)evt_npc_status_onoff, 0x00000001,
    (u32)(char*)str_me_80421f30, 0x00001000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xF1194D80, 0xF1194D80, 0x0002005B, (u32)camGetPos, 0xFE363C81, 0x00020036,
    0xFE363C80, 0xFE363C81, 0x0002001C, 0xFE363C80, 0x000000FA, 0x00010004,
    0x00000001, 0x00000021, 0x00010009, 0x00000001, 0x00010004, 0x00000000,
    0x00010003, 0x00000001, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34,
    0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x00010009,
    0x00000001, 0x00010004, 0x00000001, 0x00000002, 0x00000001,
};

u32 zakoM2D_find_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598, (u32)(char*)str_B_1_80421f80, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_npc_facedirection_add,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0x0000000A, 0x00020035, 0xFE363C81, 0x00000014,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0002001D, 0xFE363C8A,
    0x000000C8, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C80, 0x00000064,
    0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0xF24F2A80, 0xF24F2A80, 0x00000000, 0x00000020, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C8A, 0x0002003C, 0xFE363C8A,
    0xF24A8080, 0x0009005B, (u32)evt_npc_jump_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0xFE363C8A, 0xF24F2A80, 0x00000000, 0x00000021,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_slave_0_802ed598, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_CONDITION_FIRE1_802ed9d8, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_ENM_3RD2_CRASH1_802ed9ec, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x00020036, 0xFE363C81, 0x00000005, 0x00020035,
    0xFE363C82, 0x000000C8, 0x000F005B, (u32)evt_eff, (u32)(char*)zero_80421f3c, (u32)str_kemuri_test_802ed438,
    0x00000010, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00010005, 0x0000000C, 0x000F005B, (u32)evt_eff64, (u32)(char*)zero_80421f3c, (u32)(char*)str_glass_n64_802eda00,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF24A7E80, 0x0000001E,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000006, 0x0000006B, 0x0005005B, (u32)unk_800f1ac8, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0000003C, 0x0000006D, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_slave_0_802ed598,
    (u32)(char*)str_B_2_80421f74, 0x00010009, 0x0000000A, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001,
    (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x00010009, 0x0000003C, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_slave_0_802ed598, 0x00000000, 0xFFFFFC18, 0x00000000, 0x0004005B, (u32)evt_npc_flag_onoff,
    0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000020, 0x00000002, 0x00000001,
};

u32 unk_evt_8034bdb4[] = {
    0x0000006E, 0x00010005, 0x00000000, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B, (u32)evt_npc_move_position, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0xFE363C82, 0x00000064, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x0004005B, (u32)evt_get_target_dir, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x0002003B, 0xFE363C8A, 0xFE363C80, 0x0002001A, 0xFE363C8A, 0x00000000,
    0x0002003C, 0xFE363C8A, 0xF24A7680, 0x00000021, 0x0002001A, 0xFE363C8A,
    0x0000001E, 0x00020034, 0xFE363C8A, 0xF24AF280, 0x00000021, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_A_1_80421f40, 0x00010009, 0x00000040, 0x0003005B,
    (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_A_2_80421f4c, 0x00010009, 0x00000018, 0x0000006E,
    0x00010009, 0x00000018, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44,
    0x00000070, 0x0001005C, (u32)zakoM2D_find_event, 0x00010009, 0x00000064, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 gesso2D_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30,
    0x08000000, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00000400,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30, 0x00020000, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x10000000, 0x0004005B, (u32)evt_npc_status_onoff,
    0x00000001, (u32)(char*)str_me_80421f30, 0x00200000, 0x0004005B, (u32)evt_npc_status_onoff, 0x00000000,
    (u32)(char*)str_me_80421f30, 0x00000004, 0x00000002, 0x00000001,
};

u32 gesso2D_move_event[] = {
    0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44, 0x00010003, 0x00000000,
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00001000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xF1194D80, 0xF1194D80, 0x0002005B,
    (u32)camGetPos, 0xFE363C81, 0x00020036, 0xFE363C80, 0xFE363C81, 0x0002001C,
    0xFE363C80, 0x000000FA, 0x00010004, 0x00000001, 0x00000021, 0x00010009,
    0x00000001, 0x00010004, 0x00000000, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30,
    (u32)(char*)str_S_1_80421f44, 0x00010003, 0x00000001, 0x00020034, 0xFE363C81, 0xF24A7A80,
    0x00020034, 0xFE363C8D, 0xF24A7A80, 0x00010005, 0x00000000, 0x0005005B,
    (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0005005B,
    (u32)evt_mario_get_pos, 0x00000000, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C, 0x00010009,
    0x00000001, 0x00020036, 0xFE363C80, 0xFE363C8A, 0x0002001D, 0xFE363C80,
    0x0000012C, 0x00010004, 0x00000005, 0x00000021, 0x0002001C, 0xFE363C80,
    0xFFFFFED4, 0x00010004, 0x00000005, 0x00000021, 0x0002001A, 0xFE363C81,
    0xFE363C8B, 0x00000007, 0x00000021, 0x0002001A, 0xFE363C81, 0x0000005A,
    0x00000007, 0x00000021, 0x00010003, 0x00000005, 0x0002005B, (u32)gesso_ground_check,
    0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000, 0x0005005B, (u32)evt_npc_get_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020034, 0xFE363C8E,
    0xFE363C81, 0x0002003B, 0xFE363C8E, 0xFE363C8D, 0x0002003A, 0xFE363C8D,
    0xF24A7AE6, 0x0002001D, 0xFE363C8D, 0xF24A7E80, 0x00020034, 0xFE363C8D,
    0xF24A7E80, 0x00000021, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_me_80421f30, 0xFE363C80,
    0xFE363C8E, 0xFE363C82, 0x00000021, 0x00000006, 0x0004005B, (u32)evt_get_target_dir,
    (u32)(char*)str_me_80421f30, (u32)(char*)str_mario_80421f34, 0xFE363C80, 0x0003005B, (u32)evt_npc_set_ry, (u32)(char*)str_me_80421f30,
    0xFE363C80, 0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C8A, 0xFE363C8B,
    0xFE363C8C, 0x0004005B, (u32)evt_npc_facedirection_add, (u32)(char*)str_me_80421f30, 0xFE363C8A, 0x00000064,
    0x00020035, 0xFE363C8B, 0x00000032, 0x0002001B, 0xFE363C8B, 0x0000015E,
    0x00020032, 0xFE363C8B, 0x0000015E, 0x00000021, 0x00010005, 0x0000003C,
    0x0002005B, (u32)gesso_ground_check, 0xFE363C8F, 0x00020018, 0xFE363C8F, 0x00000000,
    0x0005005B, (u32)evt_npc_get_position, (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00020034, 0xFE363C8E, 0xFE363C81, 0x0002003B, 0xFE363C8E, 0xFE363C8D,
    0x0002003A, 0xFE363C8D, 0xF24A7AE6, 0x0002001D, 0xFE363C8D, 0xF24A7E80,
    0x00020034, 0xFE363C8D, 0xF24A7E80, 0x00000021, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_me_80421f30, 0xFE363C80, 0xFE363C8E, 0xFE363C82, 0x00000021, 0x00010009,
    0x00000001, 0x00000006, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_A_1_80421f40,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_KUPPA_GESSO_MOVE_802eda0c, 0xFE363C8A, 0xFE363C8B, 0xFE363C8C,
    0x00000000, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000000,
    0x0001006F, 0xFE363C8F, 0x000A005B, (u32)evt_npc_glide_position, (u32)(char*)str_me_80421f30, 0xFE363C8A,
    0xFE363C8B, 0xFE363C8C, 0x000007D0, 0xF24B4280, 0x0000000A, 0x00000004,
    0x00000000, 0x0004005B, (u32)evt_npc_set_unitwork, (u32)(char*)str_me_80421f30, 0x00000000, 0x00000001,
    0x00000070, 0x00010005, 0x00000000, 0x0004005B, (u32)evt_npc_get_unitwork, (u32)(char*)str_me_80421f30,
    0x00000000, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x00000007,
    0x00000021, 0x0002005B, (u32)upperCheck, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000001, 0x0001005F, 0xFE363C8F, 0x00000007, 0x00000021, 0x00010009,
    0x00000001, 0x00000006, 0x0003005B, (u32)evt_npc_set_anim, (u32)(char*)str_me_80421f30, (u32)(char*)str_S_1_80421f44,
    0x00010004, 0x00000001, 0x00000002, 0x00000001,
};

/* Recovered npc_event event bytecode (.sdata). */
u32 enemy_event_dead_event[] = {
    0x00020032, 0xFE363C8A, (u32)(char*)str_me_80421f30, 0x0001005E, (u32)enemy_event_dead_event_sub, 0x0002005B,
    (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000001,
};

u32 enemy_slave_common_dead_event[] = {
    0x0002005B, (u32)evt_npc_delete, (u32)(char*)str_me_80421f30, 0x0002005B, (u32)evt_npc_return_interrupt, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000001,
};

u32 enemy_common_flag_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x10120418, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 testenemynpc_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x00000002, 0x00000001,
};

u32 npc_urouro_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000600, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 kuriboo_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x00000002, 0x00000001,
};

u32 patakuri_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 patapata_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00110018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 patamet_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 chorobon_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00120018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 pansy_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x00000002, 0x00000001,
};

u32 twinkling_pansy_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x00000002, 0x00000001,
};

u32 karon_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0001005E,
    (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 sambo_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000008, 0x0004005B,
    (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100410, 0x00000002, 0x00000001,
};

u32 togedaruma_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x00000002, 0x00000001,
};

u32 piders_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00020000, 0x00000002, 0x00000001,
};

u32 basabasa_dead_event[] = {
    0x0001005E, (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 basabasa2_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 basabasa2_dead_event[] = {
    0x0001005E, (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 teresa_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 teresa_dead_event[] = {
    0x0001005E, (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 bubble_init_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00100018, 0x0004005B,
    (u32)evt_npc_status_onoff, 0x00000001, (u32)(char*)str_me_80421f30, 0x00000002, 0x00000002, 0x00000001,
};

u32 hbom_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x00000002, 0x00000001,
};

u32 zakowiz_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0001005E,
    (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 hannya_init_event[] = {
    0x0001005E, (u32)enemy_common_flag_init_event, 0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_me_80421f30,
    0x00000002, 0x00000002, 0x00000001,
};

u32 kamec_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0001005E,
    (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 kamec2_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0001005E,
    (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 hbross_dead_event[] = {
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_slave_0_802ed598, 0x00000080, 0x0001005E,
    (u32)enemy_common_dead_event, 0x00000002, 0x00000001,
};

u32 zakoM2D_dead_event[] = {
    0x0004005B, (u32)evt_npc_status_onoff, 0x00000000, (u32)(char*)str_slave_0_802ed598, 0x00000004, 0x0001005E,
    (u32)enemy_2d_common_dead_event, 0x00000002, 0x00000001,
};
