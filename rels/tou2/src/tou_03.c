#include "version.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#define GXSetTexCoordGen GXSetTexCoordGen_inline_hidden
#include "mario/mario.h"
#include "eff/n64/eff_pokopi_pcharge_n64.h"
#include "eff/eff_starstone.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "driver/npcdrv.h"
#include "driver/mapdrv.h"
#include "driver/animdrv.h"
#include "dolphin/gx.h"
#include "driver/itemdrv.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "pmario_sound.h"
#include "dolphin/mtx.h"
#include "stdio.h"
#include "string.h"
#include "driver/camdrv.h"
#include "dolphin/os.h"
#include "stdlib.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "mario/mariost.h"
#include "driver/hitdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_sub.h"
#include "tou2.h"
#include "data_tables.h"
#include "driver/extdrv.h"
#include "driver/seqdrv.h"
#include "evt/evt_eff.h"
#ifdef VERSION_G8ME01
USER_FUNC(evt_mario_balloon_off);
USER_FUNC(evt_mario_balloon_tenten);
USER_FUNC(evt_mario_get_name_hitobj_front);
USER_FUNC(evt_mario_party_use_check);
#endif
#ifdef VERSION_G8ME01
USER_FUNC(evt_msg_togelr);
#endif
#undef GXSetTexCoordGen
static void callback(MapTevCallbackInfo* info);
static f32 cosf(f32 x);
static f32 sinf(f32 x);
double cos(double x);
double sin(double x);
typedef struct ScreenWork {
    void* buf;
    s32 ready;
    f32 scale;
    f32 scaleTarget;
    f32 x;
    f32 xTarget;
    f32 y;
    f32 yTarget;
    s32 count;
    s32 mode;
    f32 field_0x28;
    u8* noise;
} ScreenWork;
static f32 randf(f32 min, f32 max);
void GXSetTexCoordGen(GXTexCoordID dst, GXTexGenType func, GXTexGenSrc src, u32 mtx);
static void screen_capture(CameraId cameraId, void* param);
void kyaku_disp(CameraId camera, void* param);
static void pose_main(void);
typedef struct Kyaku {
    u16 flags;
    u8 pad_0x2[2];
    Vec pos;
    s16 animIdx;
    u8 pad_0x12[6];
    u64 time;
} Kyaku;
typedef struct KyakuAnim {
    char name[16];
    char anim[8];
    s16 loop;
    f32 frames; // length of the pose in frames
    s32 start;
    s32 count;
} KyakuAnim;
typedef struct KyakuWork {
    s32 field_0x0;
    s32 num;
    Kyaku* kyakus;
    s32 animNum;
    KyakuAnim anim[32];
} KyakuWork;
static s32 tou_03_bss_0xC;
KyakuWork* kyaku_wp;
static ScreenWork* screen_wp;
s32 gans_alpha;
static void pose_init(void);
static void pose_change(Kyaku* kyaku, const char* name, const char* anim);
u32 __tou_rand(void);
void __tou_srand(u32 seed);

s32 _firstattack_setting(EventEntry* event, BOOL isFirstCall);
s32 burst_chain(EventEntry* event, BOOL isFirstCall);
s32 confupose(EventEntry* event, BOOL isFirstCall);
s32 eff_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_bero_exec_wait(EventEntry* event, BOOL isFirstCall);
s32 evt_bero_get_entername(EventEntry* event, BOOL isFirstCall);
s32 evt_bero_get_info(EventEntry* event, BOOL isFirstCall);
s32 evt_bero_mapchange(EventEntry* event, BOOL isFirstCall);
s32 evt_boss_stone_r(EventEntry* event, BOOL isFirstCall);
s32 evt_cam3d_evt_off(EventEntry* event, BOOL isFirstCall);
s32 evt_cam3d_evt_set(EventEntry* event, BOOL isFirstCall);
s32 evt_cam_shake(EventEntry* event, BOOL isFirstCall);
s32 evt_eff(EventEntry* event, BOOL isFirstCall);
s32 evt_eff64(EventEntry* event, BOOL isFirstCall);
s32 evt_eff_delete(EventEntry* event, BOOL isFirstCall);
s32 evt_eff_fukidashi(EventEntry* event, BOOL isFirstCall);
s32 evt_ext_entry(EventEntry* event, BOOL isFirstCall);
s32 evt_fade_set_mapchange_type(EventEntry* event, BOOL isFirstCall);
s32 evt_hitobj_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_item_entry(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_position(EventEntry* event, BOOL isFirstCall);
s32 evt_map_checkanim(EventEntry* event, BOOL isFirstCall);
s32 evt_map_pauseanim(EventEntry* event, BOOL isFirstCall);
s32 evt_map_playanim(EventEntry* event, BOOL isFirstCall);
s32 evt_map_replayanim(EventEntry* event, BOOL isFirstCall);
s32 evt_map_set_playrate(EventEntry* event, BOOL isFirstCall);
s32 evt_map_set_tevcallback(EventEntry* event, BOOL isFirstCall);
s32 evt_mapobj_flag_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_mapobj_get_position(EventEntry* event, BOOL isFirstCall);
s32 evt_mapobj_rotate(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_cont_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_dispflag_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_get_name_hitobj_ride(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_get_party(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_get_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_jump_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_key_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_kill_party(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_mov_pos2(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_normalize(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_set_dir(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_set_dir_npc(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_set_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_set_pose(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_print(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_print_add(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_print_add_insert(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_print_insert(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_print_party(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_select(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_toge(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_battle_start(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_check(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_clear_paper(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_entry(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_flag_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_get_battle_result(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_get_battle_rule_keep_result(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_get_position(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_jump_position_nohit(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_move_position(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_anim(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_attack_mode(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_autotalkpose(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_battle_info(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_battle_rule(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_height(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_paper(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_paper_anim(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_position(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_rotate(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_ry(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_scale(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_stay_anim(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_tribe(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_set_width(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_setup(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_status_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_wait_battle_end(EventEntry* event, BOOL isFirstCall);
s32 evt_paper_entry(EventEntry* event, BOOL isFirstCall);
s32 evt_party_cont_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_party_get_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_party_jump_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_party_move_pos2(EventEntry* event, BOOL isFirstCall);
s32 evt_party_run(EventEntry* event, BOOL isFirstCall);
s32 evt_party_set_dir(EventEntry* event, BOOL isFirstCall);
s32 evt_party_set_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_party_set_pose(EventEntry* event, BOOL isFirstCall);
s32 evt_party_stop(EventEntry* event, BOOL isFirstCall);
s32 evt_run_case_evt(EventEntry* event, BOOL isFirstCall);
s32 evt_seq_wait(EventEntry* event, BOOL isFirstCall);
s32 evt_set_dir_to_target(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_bgm_scope(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_bgmoff(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_bgmoff_f(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_bgmon(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_bgmon_f(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_envoff(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_envon(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_sfx_pit(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_sfx_vol(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_sfxoff(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_sfxon(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_sfxon_3d(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_get_sincos(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_get_system_flag(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_intpl_get_value(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_intpl_init(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_intpl_msec_get_value(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_intpl_msec_init(EventEntry* event, BOOL isFirstCall);
s32 evt_sub_random(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_get_fighter_battle_msg(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_get_fighter_info(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_get_fighter_name2(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_get_fighter_num(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_get_ranking(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_set_ranking_win_lose(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_set_rule_result(EventEntry* event, BOOL isFirstCall);
s32 gans_white_off(EventEntry* event, BOOL isFirstCall);
s32 gans_white_on(EventEntry* event, BOOL isFirstCall);
s32 getY(EventEntry* event, BOOL isFirstCall);
s32 item_jump(EventEntry* event, BOOL isFirstCall);
static s32 screen_evt(EventEntry* event, BOOL isFirstCall);
s32 screen_init(EventEntry* event, BOOL isFirstCall);
s32 snd_sfx_stop(EventEntry* event, BOOL isFirstCall);
s32 starstone_sound_onoff(EventEntry* event, BOOL isFirstCall);
s32 tou_champion_belt(EventEntry* event, BOOL isFirstCall);
s32 tou_champion_belt_onoff(EventEntry* event, BOOL isFirstCall);
s32 tribe_chk(EventEntry* event, BOOL isFirstCall);
void screen_tev(MapTevCallbackInfo* info);
void ext_kyaku_dispent(void);
void ext_kyaku_main(void);
void ext_kyaku_init(void);

ExtTexInfo ext_kyaku_data[51];
EvtWord x[1];
EvtWord evt_kyaku[138];
static s32 inta_tex[8];
EvtWord open_main[53];
EvtWord close_main[37];
static EvtWord w_door_open[22];
static EvtWord w_door_close[22];
static EvtWord w_open[5];
static EvtWord w_close[5];
static EvtWord e_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_open[5];
static EvtWord e_close[5];
EvtWord s1_door_open[22];
EvtWord s1_door_close[22];
EvtWord s1_door_open2[19];
static EvtWord s1_open[18];
static EvtWord s1_close[17];
EvtWord s2_door_open[22];
EvtWord s2_door_close[22];
EvtWord s2_door_open2[19];
static EvtWord s2_open[18];
static EvtWord s2_close[17];
static BeroEntry bero_data[5];
EvtWord evt_tougi[777];
EvtWord init_kinosikowa[13];
static NpcSetupInfo npcEnt[5];
EvtWord evt_damage_pose[63];
static EvtWord evt_kime_pose[56];
EvtWord npc_entry[623];
EvtWord evt_random_battle[VERSION_PICK(335, 338)];
EvtWord evt_random_battle2[VERSION_PICK(317, 326)];
EvtWord evt_teki_nyuujyou[107];
EvtWord evt_teki_nyuujyou_default[183];
EvtWord burst_chain_entry[289];
EvtWord burst_wangwang[69];
EvtWord evt_match[VERSION_PICK(362, 381)];
EvtWord evt_kurikuri_opening[442];
EvtWord evt_kurikuri_win[231];
EvtWord evt_kurikuri_lose[146];
EvtWord evt_shinemon_opening[647];
EvtWord evt_shinemon_win[199];
EvtWord evt_shinemon_lose[84];
EvtWord evt_champ_opening[556];
EvtWord evt_champ_opening_re[767];
EvtWord evt_champ_win[335];
EvtWord evt_champ_lose[124];
EvtWord evt_default_opening[748];
EvtWord evt_default_win[201];
EvtWord evt_default_lose[VERSION_PICK(137, 138)];
static EvtWord evt_champion[VERSION_PICK(365, 376)];
EvtWord evt_champion2[VERSION_PICK(178, 189)];
const char* block_data[9];
static EvtWord evt_shake[39];
EvtWord evt_boss_stone[63];
static EvtWord evt_boss[VERSION_PICK(1916, 1919)];
EvtWord evt_watch_match[515];
EvtWord evt_kyaku_1[56];
EvtWord evt_kyaku_2[56];
EvtWord evt_kyaku_3[56];
EvtWord evt_kyaku_4[88];
EvtWord evt_kyaku_5[56];
EvtWord evt_kyaku_6[56];
EvtWord evt_kyaku_7[56];
EvtWord evt_kyaku_8[88];
#ifdef VERSION_G8ME01
EvtWord evt_kyaku_check[105];
#endif
EvtWord tou_03_init_evt[VERSION_PICK(370, 372)];

ExtTexInfo ext_kyaku_data[] = {
    {("c_kinopio"), ("KNP_S_1"), 0.0f},
    {("c_kinopio"), ("KNP_S_1"), 10.0f},
    {("c_kinopio"), ("KNP_S_1"), 20.0f},
    {("c_kinopio"), ("KNP_S_1"), 30.0f},
    {("c_kinopio"), ("KNP_S_1"), 40.0f},
    {("c_kinopio"), ("KNP_S_1"), 50.0f},
    {("c_kinopio"), ("KNP_S_1"), 60.0f},
    {("c_kinopio"), ("KNP_S_1"), 70.0f},
    {("c_kinopio"), ("KNP_S_1"), 80.0f},
    {("c_kinopio"), ("KNP_S_1"), 90.0f},
    {("c_kinopio"), ("KNP_S_1"), 100.0f},
    {("c_kinopio"), ("KNP_S_1"), 110.0f},
    {("c_kinopio"), ("KNP_S_1"), 120.0f},
    {("c_kinopio"), ("KNP_S_1"), 130.0f},
    {("c_kinopio"), ("KNP_S_1"), 140.0f},
    {("c_kinopio"), ("KNP_S_1"), 150.0f},
    {("c_kinopio"), ("KNP_S_1"), 160.0f},
    {("c_kinopio"), ("KNP_S_1"), 170.0f},
    {("c_kinopio"), ("KNP_S_1"), 180.0f},
    {("c_kinopio"), ("KNP_S_1"), 190.0f},
    {("c_kinopiko"), ("KNF_S_1"), 0.0f},
    {("c_kinopiko"), ("KNF_S_1"), 10.0f},
    {("c_kinopiko"), ("KNF_S_1"), 20.0f},
    {("c_kinopiko"), ("KNF_S_1"), 30.0f},
    {("c_kinopiko"), ("KNF_S_1"), 40.0f},
    {("c_kinopiko"), ("KNF_S_1"), 50.0f},
    {("c_kinopiko"), ("KNF_S_1"), 60.0f},
    {("c_kinopiko"), ("KNF_S_1"), 70.0f},
    {("c_kinopiko"), ("KNF_S_1"), 80.0f},
    {("c_kinopiko"), ("KNF_S_1"), 90.0f},
    {("c_kinopiko"), ("KNF_S_1"), 100.0f},
    {("c_kinopiko"), ("KNF_S_1"), 110.0f},
    {("c_kinopiko"), ("KNF_S_1"), 120.0f},
    {("c_kinopiko"), ("KNF_S_1"), 130.0f},
    {("c_kinopiko"), ("KNF_S_1"), 140.0f},
    {("c_kinopiko"), ("KNF_S_1"), 150.0f},
    {("c_kinopiko"), ("KNF_S_1"), 160.0f},
    {("c_roten"), ("S_1"), 0.0f},
    {("c_roten"), ("S_1"), 10.0f},
    {("c_roten"), ("S_1"), 20.0f},
    {("c_roten"), ("S_1"), 30.0f},
    {("c_roten"), ("S_1"), 40.0f},
    {("c_roten"), ("S_1"), 50.0f},
    {("c_jugemu"), ("JGM_S_1"), 0.0f},
    {("c_jugemu"), ("JGM_S_1"), 10.0f},
    {("c_jugemu"), ("JGM_S_1"), 20.0f},
    {("c_jugemu"), ("JGM_S_1"), 30.0f},
    {("c_jugemu"), ("JGM_S_1"), 40.0f},
    {("c_jugemu"), ("JGM_S_1"), 50.0f},
    {("c_jugemu"), ("JGM_S_1"), 60.0f},
    {0},
};

EVT_BEGIN(x)
EVT_END()

s32 _firstattack_setting(EventEntry* event, BOOL isFirstCall) {
    FieldBattleData* fbat = fbatGetPointer();

    fbat->hitKind = 0x10000000;
    fbat->firstAttackParam = 0x10;
    fbat->firstAttackFlags &= ~1;
    fbatSetAttackAnnounce(0x10000000);
    fbatSetAttackAnnounceEnable();
    return EVT_RETURN_DONE;
}

#pragma dont_inline on
static void pose_init(void) {
    char name[32];
    char anim[32];
    ExtTexInfo* d = ext_kyaku_data;
    s32 i = 0;
    s32 start;

    strcpy(name, "");
    strcpy(anim, "");
    kyaku_wp->animNum = -1;
    do {
        if (strcmp(d->group, name) != 0) {
            strcpy(name, d->group);
            strcpy(anim, "");
        }
        if (strcmp(d->anim, anim) != 0) {
            strcpy(anim, d->anim);
            start = i;
            kyaku_wp->animNum++;
        }
        strcpy(kyaku_wp->anim[kyaku_wp->animNum].name, name);
        strcpy(kyaku_wp->anim[kyaku_wp->animNum].anim, anim);
        kyaku_wp->anim[kyaku_wp->animNum].start = start;
        kyaku_wp->anim[kyaku_wp->animNum].count = i - start + 1;
        kyaku_wp->anim[kyaku_wp->animNum].frames = d->frame;
        kyaku_wp->anim[kyaku_wp->animNum].loop = 1;
        i++;
        d++;
    } while (d->group != NULL);
    kyaku_wp->animNum++;
}
#pragma dont_inline reset

#pragma dont_inline on
static void pose_main(void) {
    s32 i;
    ExtdrvPose* pose;
    Kyaku* kyaku;
    KyakuAnim* a;
    f32 t;

    pose = extGetPosePtr();
    kyaku = kyaku_wp->kyakus;

    for (i = 0; i < kyaku_wp->num; i++, kyaku++, pose++) {
        a = &kyaku_wp->anim[kyaku->animIdx];
        t = 60.0f * OSTicksToMilliseconds((u64)(gp->renderTime - kyaku->time)) / 1000.0f / a->frames;
        if (t >= 1.0f) {
            kyaku->time = gp->renderTime;
            t = 0.0f;
        }
        pose->texId = a->start + (s32)((a->count - 1) * t);
    }
}
#pragma dont_inline reset

#pragma dont_inline on
static void pose_change(Kyaku* kyaku, const char* name, const char* anim) {
    KyakuAnim* a;
    s32 i;

    for (i = 0, a = kyaku_wp->anim; i < kyaku_wp->animNum; i++, a++) {
        if (strcmp(a->name, name) == 0 && strcmp(a->anim, anim) == 0) {
            break;
        }
    }
    if (i < kyaku_wp->animNum && kyaku->animIdx != i) {
        kyaku->animIdx = i;
        kyaku->time = gp->renderTime + rand();
    }
}
#pragma dont_inline reset

#pragma dont_inline on
void __tou_srand(u32 seed) {
    x[0] = seed;
}
#pragma dont_inline reset

#pragma dont_inline on
u32 __tou_rand(void) {
    x[0] = x[0] * 1103515245 + 12345;
    return ((u32)x[0] >> 16) & 0x7FFF;
}
#pragma dont_inline reset

#pragma dont_inline on
static f32 randf(f32 min, f32 max) {
    f32 r = __tou_rand() / 32767.0f;

    return r * (max - min) + min;
}
#pragma dont_inline reset

void ext_kyaku_init(void) {
    s32 far;
    s32 i;
    ExtdrvPose* pose;
    Kyaku* kyaku;
    s32 side;
    f32 dist;
    f32 angle;
    f32 x;
    f32 z;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 hd;
    f32 nx;
    f32 ny;
    f32 nz;

    kyaku_wp = _mapAlloc(sizeof(KyakuWork));
    memset(kyaku_wp, 0, sizeof(KyakuWork));
    kyaku_wp->num = extGetPoseNum();
    kyaku_wp->kyakus = _mapAlloc(kyaku_wp->num * sizeof(Kyaku));
    memset(kyaku_wp->kyakus, 0, kyaku_wp->num * sizeof(Kyaku));
    pose_init();
    __tou_srand(0x25B0E2B);
    pose = extGetPosePtr();
    kyaku = kyaku_wp->kyakus;
    for (i = 0; i < kyaku_wp->num; i++, pose++, kyaku++) {
        pose->flags |= (EXT_POSE_FLAG_VISIBLE | EXT_POSE_FLAG_BILLBOARD | EXT_POSE_FLAG_NO_SHADOW);
        PSMTXIdentity(*pose->mtx);
        while (TRUE) {
            far = __tou_rand() % 100 >= 30;
            angle = randf(0.0f, 6.283184f);
            if (!far) {
                dist = randf(250.0f, 500.0f);
            } else {
                side = angle < 0.7853982f || angle > 5.4977875f;
                dist = side ? randf(550.0f, 650.0f) : randf(550.0f, 850.0f);
            }
            x = dist * sinf(angle);
            z = dist * cosf(angle);
            if (!far) {
                if (!(__fabs(z) > 70.0)) {
                    continue;
                }
                hd = 100000.0f;
                if (hitCheckFilter(NULL, &hx, &hy, &hz, &hd, &nx, &ny, &nz, x, 100.0f, z, 0.0f, -1.0f, 0.0f) == NULL) {
                    continue;
                }
                if (0.0f != hy) {
                    continue;
                }
                (*pose->mtx)[0][3] = hx;
                (*pose->mtx)[2][3] = hz;
                pose->dir = angleABf(0.0f, 0.0f, hx, hz);
            } else {
                hd = 100000.0f;
                if (hitCheckFilter(NULL, &hx, &hy, &hz, &hd, &nx, &ny, &nz, x, 500.0f, z, 0.0f, -1.0f, 0.0f) == NULL) {
                    continue;
                }
                (*pose->mtx)[0][3] = hx;
                (*pose->mtx)[1][3] = hy;
                (*pose->mtx)[2][3] = hz;
                if (hz > 570.0f && __fabsf(hx) > 40.0f && __fabsf(hx) < 60.0f) {
                    (*pose->mtx)[0][3] += hx > 0.0f ? -15 : 15;
                }
                pose->dir = angleABf(0.0f, 0.0f, hx, hz);
            }
            break;
        }
        if (strcmp(gp->beroEnterName, "s_bero_2") != 0 && (*pose->mtx)[2][3] > 350.0f) {
            pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
        }
        if (0.0f != (*pose->mtx)[1][3]) {
            kyaku->flags |= 1;
        }
        kyaku->pos = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};
        switch (irand(1000) % 4) {
            case 0:
                pose_change(kyaku, "c_kinopio", "KNP_S_1");
                break;
            case 1:
                pose_change(kyaku, "c_kinopiko", "KNF_S_1");
                break;
            case 2:
                pose_change(kyaku, "c_roten", "S_1");
                break;
            case 3:
                pose_change(kyaku, "c_jugemu", "JGM_S_1");
                break;
        }
    }
}

EVT_BEGIN(evt_kyaku)
    CALL(evt_npc_entry, STRING(("kyaku1")), STRING(("c_kinopio")))
    CALL(evt_npc_entry, STRING(("kyaku2")), STRING(("c_kinopiko")))
    CALL(evt_npc_entry, STRING(("kyaku3")), STRING(("c_roten")))
    CALL(evt_npc_entry, STRING(("kyaku4")), STRING(("c_jugemu")))
    CALL(evt_npc_set_tribe, STRING(("kyaku1")), STRING(("\203L\203m\203s\203I")))
    CALL(evt_npc_set_tribe, STRING(("kyaku2")), STRING(("\203L\203m\203s\203I\217\227")))
    CALL(evt_npc_set_tribe, STRING(("kyaku3")), STRING(("\203\215\203e\203\223")))
    CALL(evt_npc_set_tribe, STRING(("kyaku4")), STRING(("\203W\203\205\203Q\203\200")))
    CALL(evt_npc_set_ry, STRING(("kyaku1")), 90)
    CALL(evt_npc_set_ry, STRING(("kyaku2")), 90)
    CALL(evt_npc_set_ry, STRING(("kyaku3")), 270)
    CALL(evt_npc_set_ry, STRING(("kyaku4")), 270)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING(("s_bero_2")))
        CALL(evt_npc_set_position, STRING(("kyaku1")), -40, 120, 680)
        CALL(evt_npc_set_position, STRING(("kyaku2")), -15, 120, 670)
        CALL(evt_npc_set_position, STRING(("kyaku3")), 15, 120, 680)
        CALL(evt_npc_set_position, STRING(("kyaku4")), 40, 120, 670)
        ELSE
        CALL(evt_npc_set_position, STRING(("kyaku1")), -50, 0, 370)
        CALL(evt_npc_set_position, STRING(("kyaku2")), -20, 0, 360)
        CALL(evt_npc_set_position, STRING(("kyaku3")), 20, 0, 370)
        CALL(evt_npc_set_position, STRING(("kyaku4")), 50, 0, 360)
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING(("kyaku1")), 1073743360)
    CALL(evt_npc_flag_onoff, 1, STRING(("kyaku2")), 1073743360)
    CALL(evt_npc_flag_onoff, 1, STRING(("kyaku3")), 1073743360)
    CALL(evt_npc_flag_onoff, 1, STRING(("kyaku4")), 1073743360)
    CALL(evt_ext_entry, 1000, PTR(ext_kyaku_data), PTR(ext_kyaku_init), PTR(ext_kyaku_main), PTR(ext_kyaku_dispent))
    CALL(evt_hitobj_onoff, STRING(("group246")), 1, 0)
    RETURN
EVT_END()

static s32 inta_tex[] __attribute__((aligned(32))) = {
    -1, 0, -1, 0, -1, 0, -1, 0,
};

EVT_BEGIN(open_main)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 180, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, LW(8), LW(0), 0, 0)
    CALL(evt_mapobj_rotate, 1, LW(9), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_DOOR_FALL1")), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(close_main)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 180, 0, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, LW(8), LW(0), 0, 0)
    CALL(evt_mapobj_rotate, 1, LW(9), LW(0), 0, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("aka_door_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("aka_door_b")), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("aka_door_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("aka_door_b")), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_open)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_close)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("ao_door_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("ao_ddor_b")), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("ao_door_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("ao_ddor_b")), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_open)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_close)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(s1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_b")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(s1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_b")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(s1_door_open2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_a")), LW(0), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_b")), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(s1_open)
    BEGIN_INLINE_EVENT
        CALL(evt_bero_exec_wait, 65536)
        SET_VALUE(LW(8), STRING(("door_2_a")))
        SET_VALUE(LW(9), STRING(("door_2_b")))
        CALL_CHILD(PTR(open_main))
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s1_close)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(8), STRING(("door_2_a")))
        SET_VALUE(LW(9), STRING(("door_2_b")))
        CALL_CHILD(PTR(close_main))
    END_INLINE_EVENT
    WAIT_MS(500)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(s2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_b")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(s2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_a")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_b")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(s2_door_open2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_a")), LW(0), 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_b")), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(s2_open)
    BEGIN_INLINE_EVENT
        CALL(evt_bero_exec_wait, 65536)
        SET_VALUE(LW(8), STRING(("door_3_a")))
        SET_VALUE(LW(9), STRING(("door_3_b")))
        CALL_CHILD(PTR(open_main))
    END_INLINE_EVENT
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(s2_close)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(8), STRING(("door_3_a")))
        SET_VALUE(LW(9), STRING(("door_3_b")))
        CALL_CHILD(PTR(close_main))
    END_INLINE_EVENT
    WAIT_MS(500)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        ("w_bero"), // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        w_open, // enterEvent
        -1, // caseType
        w_close, // exitEvent
        "tou_04", // nextMapName
        ("n_bero_2"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        ("e_bero"), // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        e_open, // enterEvent
        -1, // caseType
        e_close, // exitEvent
        "tou_04", // nextMapName
        ("n_bero_1"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {
        ("s_bero_1"), // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        s1_open, // enterEvent
        -1, // caseType
        s1_close, // exitEvent
        "tou_02", // nextMapName
        ("n_bero_1"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s1_door_close, // enterAnimeArg
        s1_door_open, // exitAnimeArg
    },
    {
        ("s_bero_2"), // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        s2_open, // enterEvent
        -1, // caseType
        s2_close, // exitEvent
        "tou_02", // nextMapName
        ("n_bero_2"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s2_door_close, // enterAnimeArg
        s2_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_tougi)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SET_VALUE(LF(0), 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), PTR("s_bero_2"))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_mario_set_pos, -25, 0, 635)
        CALL(evt_party_set_pos, 0, 25, 0, 635)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
    ELSE
        CALL(evt_mario_set_pos, -25, 120, 980)
        CALL(evt_party_set_pos, 0, 25, 120, 980)
        CALL(evt_mario_set_dir, 90, 0, 0)
        CALL(evt_party_set_dir, 0, 270, 0)
    END_IF
    CALL(evt_mario_set_dir_npc, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"))
    CALL(evt_party_set_dir_npc, 0, STRING(/*"相手"*/"\221\212\216\350"))
    CALL(evt_set_dir_to_target, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), STRING(/*"相手"*/"\221\212\216\350"))
    CALL(evt_cam3d_evt_set, 0, 563, 962, 0, 554, 412, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_sound_data_set, STRING(/*"相手"*/"\221\212\216\350"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, PTR("SFX_STG3_TOGENOKO_DOWN1"))
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    IF_INT_EQUAL(LF(0), 0)
        CALL_CHILD(s1_door_open)
    ELSE
        CALL_CHILD(s2_door_open)
    END_IF
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_cam3d_evt_set, 0, 258, 962, 0, 215, 412, 5000, EASING_LINEAR)
    WAIT_MS(5000)
    WAIT_MS(2000)
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam3d_evt_set, 0, 130, 840, 0, 50, 397, 1500, EASING_SINE_IN_OUT)
    ELSE
        CALL(evt_cam3d_evt_set, 0, 240, 1187, 0, 170, 742, 1500, EASING_SINE_IN_OUT)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SUBTRACT(LW(2), 230)
        CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(80.0f))
    END_INLINE_EVENT
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 230)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(80.0f))
    SET_VALUE(LW(1), 2)
    SPAWN_EVENT(bero_close_door_play_se)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    IF_INT_EQUAL(LF(0), 0)
        CALL_CHILD(s1_door_close)
    ELSE
        CALL_CHILD(s2_door_close)
    END_IF
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 180, 500)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    IF_INT_EQUAL(LF(0), 0)
        CALL_CHILD(s1_door_open2)
    ELSE
        CALL_CHILD(s2_door_open2)
    END_IF
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, PTR("stg3_tou_25"))
    WAIT_MS(300)
    CALL(evt_mario_set_dir_npc, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 85, 172, 326, 85, 96, -104, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, PTR("stg3_tou_26"), 0, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -85, 172, 326, -85, 96, -104, 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_get_position, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(10), LW(11), LW(12))
    CALL(evt_npc_status_onoff, 1, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_get_position, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 50)
    CALL(evt_npc_move_position, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(0), LW(2), 0, FLOAT(100.0f), 1)
    CALL(evt_npc_set_rotate, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), 0, 0, -30)
    CALL(evt_npc_get_position, STRING(/*"相手"*/"\221\212\216\350"), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(evt_npc_jump_position_nohit, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(0), LW(1), LW(2), 700, FLOAT(40.0f))
    CALL(evt_snd_sfxon_3d, PTR("SFX_STG3_BLED_ATTACK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff, 0, PTR("hit"), 0, LW(0), LW(1), LW(2), FLOAT(3.0f), 10, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 300)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), -30)
        LOOP(10)
            CALL(evt_npc_set_rotate, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), 0, 0, LW(0))
            ADD(LW(0), 3)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_rotate, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), 0, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_sound_data_set, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, 0, 0)
        ADD(LW(0), 30)
        SUBTRACT(LW(1), 20)
        CALL(evt_npc_jump_position_nohit, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(0), LW(1), LW(2), 300, FLOAT(20.0f))
        ADD(LW(0), 30)
        CALL(evt_npc_jump_position_nohit, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(0), LW(1), LW(2), 300, FLOAT(10.0f))
        CALL(evt_npc_jump_position_nohit, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(10), LW(11), LW(12), 300, FLOAT(10.0f))
        CALL(evt_npc_sound_data_reset, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")))
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, PTR(/*("\221\212\216\350")*/("\221\212\216\350")), PTR("K_2"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING(/*"相手"*/"\221\212\216\350"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 30)
        CALL(evt_npc_jump_position_nohit, STRING(/*"相手"*/"\221\212\216\350"), LW(0), LW(1), LW(2), 300, FLOAT(20.0f))
        SUBTRACT(LW(0), 30)
        CALL(evt_npc_jump_position_nohit, STRING(/*"相手"*/"\221\212\216\350"), LW(0), LW(1), LW(2), 300, FLOAT(10.0f))
    END_INLINE_EVENT
    CALL(evt_msg_print, 0, PTR("stg3_tou_27"), 0, PTR(/*("\221\212\216\350")*/("\221\212\216\350")))
    CALL(evt_cam3d_evt_set, 65, 172, 326, 65, 96, -104, 1000, EASING_SINE_IN_OUT)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING(/*"ガンス"*/"\203K\203\223\203X"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_set_position, STRING(/*"ガンス"*/"\203K\203\223\203X"), 160, 60, -65)
        CALL(evt_npc_move_position, STRING(/*"ガンス"*/"\203K\203\223\203X"), 45, -65, 0, FLOAT(50.0f), 1)
        CALL(evt_npc_move_position, STRING(/*"ガンス"*/"\203K\203\223\203X"), 20, 0, 0, FLOAT(50.0f), 1)
        CALL(evt_set_dir_to_target, STRING(/*"ガンス"*/"\203K\203\223\203X"), STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(1200)
    CALL(evt_npc_set_anim, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")), PTR("S_4"))
    CALL(evt_npc_set_autotalkpose, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")), PTR("S_4"), PTR("T_4"))
    CALL(evt_msg_print, 0, PTR("stg3_tou_28"), 0, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")))
    WAIT_MS(300)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_msg_print, 0, PTR("stg3_tou_29"), 0, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("M_5"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("M_6"))
    CALL(evt_msg_print, 0, PTR("stg3_tou_30"), 0, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")))
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("M_7"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("S_2"))
    CALL(evt_npc_set_anim, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")), PTR("H_1"))
    CALL(evt_npc_set_autotalkpose, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")), PTR("S_2"), PTR("T_2"))
    CALL(evt_msg_continue)
    WAIT_MS(1500)
    CALL(evt_npc_set_anim, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")), PTR("H_2"))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")), PTR("S_3"))
    CALL(evt_npc_entry, PTR(/*("\203x\203\213\203g")*/("\203x\203\213\203g")), PTR("c_bred_h"))
    CALL(evt_npc_get_position, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 5)
    ADD(LW(1), 65)
    CALL(evt_npc_set_position, STRING(/*"ベルト"*/"\203x\203\213\203g"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_anim, PTR(/*("\203x\203\213\203g")*/("\203x\203\213\203g")), PTR("C_1"))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, PTR("starstone"), 9, LW(0), LW(1), LW(2), FLOAT(0.75f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, PTR("SFX_STG3_WIN_AUDIENCE2"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(2000)
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_cam3d_evt_set, 0, 95, 692, 0, 43, 397, 0, EASING_SINE_IN_OUT)
    ELSE
        CALL(evt_cam3d_evt_set, 0, 218, 1037, 0, 165, 742, 0, EASING_SINE_IN_OUT)
    END_IF
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir_npc, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"))
    CALL(evt_party_set_dir_npc, 0, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, PTR("eff"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_msg_print_party, PTR("stg3_tou_31"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 80, 67, 225, 80, 172, -161, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print_party, PTR("stg3_tou_32"))
    WAIT_MS(300)
    CALL(evt_fade_set_mapchange_type, 0, 2, 500, 1, 500)
    SET_VALUE(GSWF(2374), 1)
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_bero_mapchange, PTR("tou_02"), PTR("n_bero_1"))
    ELSE
        CALL(evt_bero_mapchange, PTR("tou_02"), PTR("n_bero_2"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_kinosikowa)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_npc_set_tribe, STRING(("\203L\203m\203V\203R\203\217")), STRING(("\203L\203m\203V\203R\203\217\201i\203}\203C\203N\201j")))
        CALL(evt_npc_set_stay_anim, STRING(("\203L\203m\203V\203R\203\217")))
    END_IF
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ガンス"*/"\203K\203\223\203X", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノシコワ"*/"\203L\203m\203V\203R\203\217", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kinosikowa, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マッチョガンス"*/"\203}\203b\203`\203\207\203K\203\223\203X", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノシチョフ"*/"\203L\203m\203V\203`\203\207\203t", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_damage_pose)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING(("M_D_2")))
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING(("PKR_D_3")))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING(("PNK_D_3")))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING(("D_3")))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING(("PYS_D_3")))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING(("PWD_D_3")))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING(("PTR_D_3")))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING(("PCH_D_3")))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(evt_kime_pose)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_party_set_pose, 0, STRING(("PKR_Y_1")))
        CASE_EQUAL(2)
        CALL(evt_party_set_pose, 0, STRING(("PNK_Y_1")))
        CASE_EQUAL(3)
        CALL(evt_party_set_pose, 0, STRING(("Y_1")))
        CASE_EQUAL(4)
        CALL(evt_party_set_pose, 0, STRING(("PYS_Y_1")))
        CASE_EQUAL(5)
        CALL(evt_party_set_pose, 0, STRING(("PWD_Y_1")))
        CASE_EQUAL(6)
        CALL(evt_party_set_pose, 0, STRING(("PTR_Y_1")))
        CASE_EQUAL(7)
        CALL(evt_party_set_pose, 0, STRING(("PCH_Y_1")))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(npc_entry)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_01_ARENA_INTRO_1F)
        CALL(evt_npc_entry, PTR(/*("\203`\203\203\203\223\203s\203\207\203\223")*/("\203`\203\203\203\223\203s\203\207\203\223")), PTR("c_bred_h"))
        CALL(evt_npc_set_position, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), 80, 60, 0)
        CALL(evt_npc_entry, STRING(/*"相手"*/"\221\212\216\350"), PTR("c_togenoko_b"))
        CALL(evt_npc_set_position, PTR(/*("\221\212\216\350")*/("\221\212\216\350")), -80, 60, 0)
        CALL(evt_npc_set_tribe, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), STRING(/*"ブレッドハート"*/"\203u\203\214\203b\203h\203n\201[\203g"))
        CALL(evt_npc_set_tribe, STRING(/*"相手"*/"\221\212\216\350"), STRING(/*"トゲノコエース"*/"\203g\203Q\203m\203R\203G\201[\203X"))
        CALL(evt_npc_flag_onoff, 1, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_flag_onoff, 1, STRING(/*"相手"*/"\221\212\216\350"), NPC_FLAG_NO_WALL_COLLISION)
        CALL(evt_npc_set_ry, STRING(/*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223"), 270)
        CALL(evt_npc_set_ry, STRING(/*"相手"*/"\221\212\216\350"), 90)
        RETURN
    END_IF
    SET_VALUE(LF(0), 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG3_01_ARENA_INTRO_1F, STG3_30_04_AIR_DUCT_EXIT)
        SET_VALUE(LF(0), 1)
        CASE_LESS(STG3_32_BOSS_BATTLE)
        CASE_DEFAULT
        SET_VALUE(LF(0), 1)
    END_SWITCH
    IF_INT_EQUAL(LF(0), 1)
        IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 0)
            CALL(evt_sub_random, 10000, LW(10))
            IF_INT_LESS(LW(10), 5000)
                LOOP(0)
                    CALL(evt_sub_random, 3000, LW(10))
                    SWITCH(LW(10))
                        CASE_LESS(1000)
                        SET_VALUE(LW(0), PTR("c_bomhey"))
                        SET_VALUE(LW(1), PTR(/*("\203{\203\200\225\272")*/("\203{\203\200\225\272")))
                        CASE_LESS(2000)
                        SET_VALUE(LW(0), PTR("c_kuribo"))
                        SET_VALUE(LW(1), PTR(/*("\203N\203\212\203{\201[")*/("\203N\203\212\203{\201[")))
                        CASE_DEFAULT
                        SET_VALUE(LW(0), PTR("c_kmoon_wb"))
                        SET_VALUE(LW(1), STRING(/*"モノクロクロクモーン"*/"\203\202\203m\203N\203\215\203N\203\215\203N\203\202\201[\203\223"))
                    END_SWITCH
                    CALL(evt_sub_random, 3000, LW(10))
                    SWITCH(LW(10))
                        CASE_LESS(1000)
                        SET_VALUE(LW(2), PTR("c_bomhey"))
                        SET_VALUE(LW(3), PTR(/*("\203{\203\200\225\272")*/("\203{\203\200\225\272")))
                        CASE_LESS(2000)
                        SET_VALUE(LW(2), PTR("c_kuribo"))
                        SET_VALUE(LW(3), PTR(/*("\203N\203\212\203{\201[")*/("\203N\203\212\203{\201[")))
                        CASE_DEFAULT
                        SET_VALUE(LW(2), PTR("c_kmoon_wb"))
                        SET_VALUE(LW(3), PTR(/*("\203\202\203m\203N\203\215\203N\203\215\203N\203\202\201[\203\223")*/("\203\202\203m\203N\203\215\203N\203\215\203N\203\202\201[\203\223")))
                    END_SWITCH
                    IF_STR_NOT_EQUAL(LW(0), LW(2))
                        BREAK_LOOP
                    END_IF
                END_LOOP
            ELSE
                LOOP(0)
                    CALL(evt_sub_random, 4000, LW(10))
                    SWITCH(LW(10))
                        CASE_LESS(1000)
                        SET_VALUE(LW(0), PTR("c_chorobon"))
                        SET_VALUE(LW(1), PTR(/*("\203`\203\207\203\215\203{\203\223")*/("\203`\203\207\203\215\203{\203\223")))
                        CASE_LESS(2000)
                        SET_VALUE(LW(0), PTR("c_monban_t"))
                        SET_VALUE(LW(1), PTR(/*("\203_\201[\203N\203L\201[\203p\201[")*/("\203_\201[\203N\203L\201[\203p\201[")))
                        CASE_LESS(3000)
                        SET_VALUE(LW(0), PTR("c_togedaruma"))
                        SET_VALUE(LW(1), PTR(/*("\203g\203Q\203_\203\213\203}\201[")*/("\203g\203Q\203_\203\213\203}\201[")))
                        CASE_DEFAULT
                        SET_VALUE(LW(0), PTR("c_wanwan_a"))
                        SET_VALUE(LW(1), PTR(/*("\203o\201[\203X\203g\203\217\203\223\203\217\203\223")*/("\203o\201[\203X\203g\203\217\203\223\203\217\203\223")))
                    END_SWITCH
                    CALL(evt_sub_random, 4000, LW(10))
                    SWITCH(LW(10))
                        CASE_LESS(1000)
                        SET_VALUE(LW(2), PTR("c_chorobon"))
                        SET_VALUE(LW(3), PTR(/*("\203`\203\207\203\215\203{\203\223")*/("\203`\203\207\203\215\203{\203\223")))
                        CASE_LESS(2000)
                        SET_VALUE(LW(2), PTR("c_monban_t"))
                        SET_VALUE(LW(3), PTR(/*("\203_\201[\203N\203L\201[\203p\201[")*/("\203_\201[\203N\203L\201[\203p\201[")))
                        CASE_LESS(3000)
                        SET_VALUE(LW(2), PTR("c_togedaruma"))
                        SET_VALUE(LW(3), PTR(/*("\203g\203Q\203_\203\213\203}\201[")*/("\203g\203Q\203_\203\213\203}\201[")))
                        CASE_DEFAULT
                        SET_VALUE(LW(2), PTR("c_wanwan_a"))
                        SET_VALUE(LW(3), PTR(/*("\203o\201[\203X\203g\203\217\203\223\203\217\203\223")*/("\203o\201[\203X\203g\203\217\203\223\203\217\203\223")))
                    END_SWITCH
                    IF_STR_NOT_EQUAL(LW(0), LW(2))
                        BREAK_LOOP
                    END_IF
                END_LOOP
            END_IF
            CALL(evt_npc_entry, STRING(/*"選手1"*/"\221I\216\3501"), LW(0))
            CALL(evt_npc_set_position, STRING(/*"選手1"*/"\221I\216\3501"), -80, 60, 0)
            CALL(evt_npc_entry, STRING(/*"選手2"*/"\221I\216\3502"), LW(2))
            CALL(evt_npc_set_position, STRING(/*"選手2"*/"\221I\216\3502"), 80, 60, 0)
            CALL(evt_npc_set_tribe, STRING(/*"選手1"*/"\221I\216\3501"), LW(1))
            CALL(evt_npc_set_tribe, STRING(/*"選手2"*/"\221I\216\3502"), LW(3))
            CALL(evt_npc_flag_onoff, 1, STRING(/*"選手1"*/"\221I\216\3501"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_flag_onoff, 1, STRING(/*"選手2"*/"\221I\216\3502"), NPC_FLAG_NO_HIT | NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_set_ry, STRING(/*"選手1"*/"\221I\216\3501"), 90)
            CALL(evt_npc_set_ry, STRING(/*"選手2"*/"\221I\216\3502"), 270)
            IF_STR_EQUAL(LW(1), STRING(/*"バーストワンワン"*/"\203o\201[\203X\203g\203\217\203\223\203\217\203\223"))
                CALL(evt_npc_set_anim, STRING(/*"選手1"*/"\221I\216\3501"), PTR("WAN_A_1"))
                SET_VALUE(LW(0), 6)
                SPAWN_EVENT(burst_chain_entry)
            END_IF
            IF_STR_EQUAL(LW(3), PTR(/*("\203o\201[\203X\203g\203\217\203\223\203\217\203\223")*/("\203o\201[\203X\203g\203\217\203\223\203\217\203\223")))
                CALL(evt_npc_set_anim, PTR(/*("\221I\216\3501")*/("\221I\216\3501")), PTR("WAN_A_1"))
                SET_VALUE(LW(0), 7)
                SPAWN_EVENT(burst_chain_entry)
            END_IF
            RETURN
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
        CALL(evt_tou_get_fighter_num, GSW_GLITZ_PIT_OPPONENT, LW(0))
        SET_VALUE(LW(1), 0)
        LOOP(0)
            IF_INT_EQUAL(LW(0), LW(1))
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(GSW_GLITZ_PIT_OPPONENT, 11)
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    SET_VALUE(LW(2), STRING(/*"敵３"*/"\223G\202R"))
                    CASE_EQUAL(1)
                    SET_VALUE(LW(2), STRING(/*"敵４"*/"\223G\202S"))
                    CASE_EQUAL(2)
                    SET_VALUE(LW(2), STRING(/*"敵１"*/"\223G\202P"))
                    CASE_EQUAL(3)
                    SET_VALUE(LW(2), STRING(/*"敵２"*/"\223G\202Q"))
                    CASE_EQUAL(4)
                    SET_VALUE(LW(2), STRING(/*"敵５"*/"\223G\202T"))
                    CASE_DEFAULT
                    DEBUG_PUT_MSG(STRING(/*"おかしい"*/"\202\250\202\251\202\265\202\242"))
                    BREAK_LOOP
                END_SWITCH
            ELSE
                SWITCH(LW(1))
                    CASE_EQUAL(0)
                    SET_VALUE(LW(2), STRING(/*"敵１"*/"\223G\202P"))
                    CASE_EQUAL(1)
                    SET_VALUE(LW(2), STRING(/*"敵２"*/"\223G\202Q"))
                    CASE_EQUAL(2)
                    SET_VALUE(LW(2), STRING(/*"敵３"*/"\223G\202R"))
                    CASE_EQUAL(3)
                    SET_VALUE(LW(2), STRING(/*"敵４"*/"\223G\202S"))
                    CASE_EQUAL(4)
                    SET_VALUE(LW(2), STRING(/*"敵５"*/"\223G\202T"))
                    CASE_DEFAULT
                    DEBUG_PUT_MSG(STRING(/*"おかしい"*/"\202\250\202\251\202\265\202\242"))
                    BREAK_LOOP
                END_SWITCH
            END_IF
            CALL(evt_tou_get_fighter_tribe, GSW_GLITZ_PIT_OPPONENT, LW(1), LW(3), LW(4))
            CALL(evt_npc_entry, LW(2), LW(3))
            CALL(evt_npc_set_tribe, LW(2), LW(4))
            CALL(evt_npc_set_position, LW(2), 0, -1000, 0)
            CALL(evt_npc_flag_onoff, 1, LW(2), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_set_ry, LW(2), 270)
            BEGIN_INLINE_EVENT
                CALL(evt_npc_get_height, LW(2), LW(0))
                IF_INT_GREATER(LW(0), 50)
                    CALL(evt_npc_set_height, LW(2), 50)
                END_IF
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                IF_INT_EQUAL(GSW_GLITZ_PIT_OPPONENT, 2)
                    SWITCH(LW(1))
                        CASE_EQUAL(0)
                        SET_VALUE(LW(0), 0)
                        SPAWN_EVENT(burst_chain_entry)
                        CASE_EQUAL(1)
                        SET_VALUE(LW(0), 1)
                        SPAWN_EVENT(burst_chain_entry)
                    END_SWITCH
                END_IF
            END_INLINE_EVENT
            ADD(LW(1), 1)
        END_LOOP
        CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
        IF_INT_EQUAL(LW(1), 10)
            CALL(evt_npc_set_scale, STRING(/*"敵１"*/"\223G\202P"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
            CALL(evt_npc_set_scale, STRING(/*"敵２"*/"\223G\202Q"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
            CALL(evt_npc_sound_data_set, PTR(/*("\223G\202P")*/("\223G\202P")), PTR("SFX_STG3_ISIN_MOVE1L"), PTR("SFX_STG3_ISIN_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CALL(evt_npc_sound_data_set, PTR(/*("\223G\202Q")*/("\223G\202Q")), PTR("SFX_STG3_ISIN_MOVE1L"), PTR("SFX_STG3_ISIN_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        END_IF
        SET_VALUE(LF(0), 0)
        CALL(evt_tou_get_ranking, LW(0))
        IF_INT_EQUAL(LW(0), 6)
            CALL(evt_npc_entry, PTR(/*("\227\220")*/("\227\220")), PTR("c_koopa"))
            CALL(evt_npc_set_position, PTR(/*("\227\220")*/("\227\220")), 0, -1000, 0)
            CALL(evt_npc_set_tribe, STRING(/*"乱"*/"\227\220"), STRING(/*"クッパ"*/"\203N\203b\203p"))
            CALL(evt_npc_flag_onoff, 1, STRING(/*"乱"*/"\227\220"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_set_ry, STRING(/*"乱"*/"\227\220"), 270)
            CALL(evt_npc_sound_data_set, PTR(/*("\227\220")*/("\227\220")), PTR("SFX_STG3_KOOPA_MOVE1L"), PTR("SFX_STG3_KOOPA_MOVE1R"), 12000, PTR("SFX_STG3_KOOPA_JUMP1"), EVT_NULLPTR)
        ELSE
            CALL(evt_npc_entry, PTR(/*("\227\220")*/("\227\220")), PTR("c_sinemon_a"))
            CALL(evt_npc_set_position, PTR(/*("\227\220")*/("\227\220")), 0, -1000, 0)
            CALL(evt_npc_set_tribe, STRING(/*"乱"*/"\227\220"), STRING(/*"アイアンシンエモン"*/"\203A\203C\203A\203\223\203V\203\223\203G\203\202\203\223"))
            CALL(evt_npc_flag_onoff, 1, STRING(/*"乱"*/"\227\220"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_set_ry, STRING(/*"乱"*/"\227\220"), 270)
            CALL(evt_npc_set_scale, STRING(/*"乱"*/"\227\220"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
            CALL(evt_npc_sound_data_set, PTR(/*("\227\220")*/("\227\220")), PTR("SFX_STG3_ISIN_MOVE1L"), PTR("SFX_STG3_ISIN_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
            CALL(evt_npc_entry, PTR(/*("\227\220\202Q")*/("\227\220\202Q")), PTR("c_sinemon_m"))
            CALL(evt_npc_set_position, PTR(/*("\227\220\202Q")*/("\227\220\202Q")), 0, -1000, 0)
            CALL(evt_npc_set_tribe, STRING(/*"乱２"*/"\227\220\202Q"), STRING(/*"アイアンシンエモン2"*/"\203A\203C\203A\203\223\203V\203\223\203G\203\202\203\2232"))
            CALL(evt_npc_flag_onoff, 1, STRING(/*"乱２"*/"\227\220\202Q"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION)
            CALL(evt_npc_set_ry, STRING(/*"乱２"*/"\227\220\202Q"), 270)
            CALL(evt_npc_set_scale, STRING(/*"乱２"*/"\227\220\202Q"), FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
            CALL(evt_npc_sound_data_set, PTR(/*("\227\220\202Q")*/("\227\220\202Q")), PTR("SFX_STG3_ISIN_MOVE1L"), PTR("SFX_STG3_ISIN_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_random_battle)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING("m"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("乱"), 370, 0, 210)
    CALL(evt_npc_set_position, STRING("乱２"), 390, 0, 235)
    CALL(evt_msg_print, 0, STRING("stg3_tou_403"), 0, STRING("乱"))
#else
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FIND1_3")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(("m")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING(("\227\220")), 370, 0, 210)
    CALL(evt_npc_set_position, STRING(("\227\220\202Q")), 390, 0, 235)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_403")), 0, STRING(("\227\220")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 300, 134, 732, 300, 38, 190, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        CALL(evt_npc_move_position, STRING("乱２"), 290, 235, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING("乱２"), 200, 60, 192, 500, 30)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("乱"), 270, 210, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING("乱"), 180, 60, 167, 500, 30)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("乱２"), 75, 75, 0, FLOAT(80.0f), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("乱"), 55, 50, 0, FLOAT(80.0f), 1)
#else
        CALL(evt_npc_move_position, STRING(("\227\220\202Q")), 290, 235, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING(("\227\220\202Q")), 200, 60, 192, 500, 30)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING(("\227\220")), 270, 210, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING(("\227\220")), 180, 60, 167, 500, 30)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING(("M_I_Y")))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\227\220\202Q")), 75, 75, 0, FLOAT(80.0f), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\227\220")), 55, 50, 0, FLOAT(80.0f), 1)
#endif
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 180, 517, 0, 95, -145, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_cam3d_evt_set, 0, 130, 62, 0, 115, -22, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_403_01"), 0, STRING("ガンス"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_403_01")), 0, STRING(("\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 40, 155, 320, 40, 96, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_404"), 0, STRING("乱"))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_GONG1"), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_battle_info, STRING("乱"), 10)
    CALL(evt_npc_battle_start, STRING("乱"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_check, STRING("敵１"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("敵１"), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING("敵２"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("敵２"), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING("敵３"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("敵３"), 0, -1000, 0)
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_404")), 0, STRING(("\227\220")))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_GONG1")), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_battle_info, STRING(("\227\220")), 10)
    CALL(evt_npc_battle_start, STRING(("\227\220")))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_npc_check, STRING(("\223G\202P")), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING(("\223G\202P")), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING(("\223G\202Q")), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING(("\223G\202Q")), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING(("\223G\202R")), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING(("\223G\202R")), 0, -1000, 0)
#endif
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -50)
    ADD(LW(2), -20)
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_battle_result, LW(0))
    IF_INT_EQUAL(LW(0), 1)
#ifdef VERSION_G8ME01
        CALL(evt_npc_set_anim, STRING("乱"), STRING("SIN_K_1"))
        CALL(evt_npc_set_anim, STRING("乱２"), STRING("SIN_K_1"))
        CALL(evt_npc_flag_onoff, 0, STRING("乱"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_flag_onoff, 0, STRING("乱２"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_msg_print, 0, STRING("stg3_tou_406"), 0, STRING("乱"))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_403_02"), 0, STRING("ガンス"))
#else
        CALL(evt_npc_set_anim, STRING(("\227\220")), STRING(("SIN_K_1")))
        CALL(evt_npc_set_anim, STRING(("\227\220\202Q")), STRING(("SIN_K_1")))
        CALL(evt_npc_flag_onoff, 0, STRING(("\227\220")), 1536)
        CALL(evt_npc_flag_onoff, 0, STRING(("\227\220\202Q")), 1536)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_406")), 0, STRING(("\227\220")))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_403_02")), 0, STRING(("\203K\203\223\203X")))
#endif
        WAIT_MS(300)
        ELSE
        CALL_CHILD(PTR(evt_damage_pose))
#ifdef VERSION_G8ME01
        CALL(evt_msg_print, 0, STRING("stg3_tou_405"), 0, STRING("乱"))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING("stg3_tou_406_01"), 0, STRING("ガンス"))
#else
        CALL(evt_msg_print, 0, STRING(("stg3_tou_405")), 0, STRING(("\227\220")))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_406_01")), 0, STRING(("\203K\203\223\203X")))
#endif
        WAIT_MS(300)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_random_battle2)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING("m"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("乱"), 370, 0, 210)
    CALL(evt_msg_print, 0, STRING("stg3_tou_403_04"), 0, STRING("乱"))
    WAIT_MS(300)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_KUPPA2"))
    CALL(evt_cam3d_evt_set, 300, 134, 732, 300, 38, 190, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING("乱"), 270, 210, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING("乱"), 180, 60, 167, 500, 30)
#else
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FIND1_3")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(("m")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING(("\227\220")), 370, 0, 210)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_403_04")), 0, STRING(("\227\220")))
    WAIT_MS(300)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_EVT_KUPPA2")))
    CALL(evt_cam3d_evt_set, 300, 134, 732, 300, 38, 190, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING(("\227\220")), 270, 210, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING(("\227\220")), 180, 60, 167, 500, 30)
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.029296875f), 500)
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
#ifdef VERSION_G8ME01
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("乱"), 55, 50, 0, FLOAT(80.0f), 1)
#else
    CALL(evt_mario_set_pose, STRING(("M_I_Y")))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\227\220")), 55, 50, 0, FLOAT(80.0f), 1)
#endif
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 180, 517, 0, 95, -145, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_cam3d_evt_set, 0, 130, 62, 0, 115, -22, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_403_05"), 0, STRING("ガンス"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_403_05")), 0, STRING(("\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 15, 189, 396, 15, 116, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_403_06"), 0, STRING("乱"))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_GONG1"), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_battle_info, STRING("乱"), 24)
    CALL(evt_npc_battle_start, STRING("乱"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_npc_check, STRING("敵１"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("敵１"), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING("敵２"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("敵２"), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING("敵３"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING("敵３"), 0, -1000, 0)
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_403_06")), 0, STRING(("\227\220")))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_GONG1")), 0)
    WAIT_MS(500)
    CALL(evt_npc_set_battle_info, STRING(("\227\220")), 24)
    CALL(evt_npc_battle_start, STRING(("\227\220")))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_npc_check, STRING(("\223G\202P")), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING(("\223G\202P")), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING(("\223G\202Q")), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING(("\223G\202Q")), 0, -1000, 0)
    END_IF
    CALL(evt_npc_check, STRING(("\223G\202R")), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_position, STRING(("\223G\202R")), 0, -1000, 0)
#endif
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -50)
    ADD(LW(2), -20)
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_battle_result, LW(0))
    IF_INT_EQUAL(LW(0), 1)
#ifdef VERSION_G8ME01
        CALL(evt_npc_set_anim, STRING("乱"), STRING("KPA_Z_3"))
        CALL(evt_npc_set_autotalkpose, STRING("乱"), STRING("KPA_Z_3"), STRING("KPA_T_8"))
        CALL(evt_msg_print, 0, STRING("stg3_tou_403_08"), 0, STRING("乱"))
        WAIT_MS(300)
        CALL(evt_msg_togelr, 0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_403_10"), 0, STRING("ガンス"))
#else
        CALL(evt_npc_set_anim, STRING(("\227\220")), STRING(("KPA_Z_3")))
        CALL(evt_npc_set_autotalkpose, STRING(("\227\220")), STRING(("KPA_Z_3")), STRING(("KPA_T_8")))
        CALL(evt_msg_print, 0, STRING(("stg3_tou_403_08")), 0, STRING(("\227\220")))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_403_10")), 0, STRING(("\203K\203\223\203X")))
#endif
        WAIT_MS(300)
        ELSE
        CALL_CHILD(PTR(evt_damage_pose))
#ifdef VERSION_G8ME01
        CALL(evt_msg_print, 0, STRING("stg3_tou_403_07"), 0, STRING("乱"))
        WAIT_MS(300)
        CALL(evt_msg_togelr, 0)
        CALL(evt_msg_print, 0, STRING("stg3_tou_403_09"), 0, STRING("ガンス"))
#else
        CALL(evt_msg_print, 0, STRING(("stg3_tou_403_07")), 0, STRING(("\227\220")))
        WAIT_MS(300)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_403_09")), 0, STRING(("\203K\203\223\203X")))
#endif
        WAIT_MS(300)
    END_IF
    SET_VALUE(LW(0), GSW_STORY_PROGRESS)
    SET_VALUE(GSW_STORY_PROGRESS, STG4_00_WELCOME_TO_STAGE_4)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    SET_VALUE(GSW_STORY_PROGRESS, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(evt_teki_nyuujyou)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        SET_VALUE(LW(3), 70)
        SET_VALUE(LW(4), 0)
        CASE_EQUAL(1)
        SET_VALUE(LW(3), 40)
        SET_VALUE(LW(4), 25)
        CASE_EQUAL(2)
        SET_VALUE(LW(3), 20)
        SET_VALUE(LW(4), -25)
        CASE_EQUAL(3)
        SET_VALUE(LW(3), 60)
        SET_VALUE(LW(4), 0)
        CASE_EQUAL(4)
        SET_VALUE(LW(3), 80)
        SET_VALUE(LW(4), -25)
    END_SWITCH
    SET_VALUE(LW(5), 450)
    ADD(LW(5), LW(3))
    CALL(evt_npc_move_position, LW(0), LW(5), LW(4), 0, FLOAT(60.0f), 1)
    WAIT_MS(1000)
    CALL(evt_npc_move_position, LW(0), 280, LW(4), 0, FLOAT(60.0f), 1)
    CALL(evt_npc_jump_position_nohit, LW(0), 250, 20, LW(4), 500, 30)
    CALL(evt_npc_jump_position_nohit, LW(0), 215, 40, LW(4), 500, 30)
    CALL(evt_npc_jump_position_nohit, LW(0), 185, 60, LW(4), 500, 30)
    SET_VALUE(LW(5), 50)
    ADD(LW(5), LW(3))
    CALL(evt_npc_move_position, LW(0), LW(5), LW(4), 0, FLOAT(60.0f), 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_teki_nyuujyou_default)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        IF_INT_EQUAL(LW(2), 1)
            SET_VALUE(LW(3), 70)
            SET_VALUE(LW(4), 0)
        ELSE
            SET_VALUE(LW(3), 50)
            SET_VALUE(LW(4), 0)
        END_IF
        CASE_EQUAL(1)
        SET_VALUE(LW(3), 90)
        SET_VALUE(LW(4), 25)
        CASE_EQUAL(2)
        SET_VALUE(LW(3), 70)
        SET_VALUE(LW(4), -25)
        CASE_EQUAL(3)
        SET_VALUE(LW(3), 110)
        SET_VALUE(LW(4), 0)
        CASE_EQUAL(4)
        SET_VALUE(LW(3), 130)
        SET_VALUE(LW(4), -25)
    END_SWITCH
    CALL(tribe_chk, LW(0), STRING(/*"カメック緑空"*/"\203J\203\201\203b\203N\227\316\213\363"), LW(5))
    IF_INT_EQUAL(LW(5), 1)
        CALL(evt_npc_get_position, LW(0), LW(6), LW(7), LW(8))
        ADD(LW(7), 25)
        CALL(evt_npc_set_position, LW(0), LW(6), LW(7), LW(8))
        CALL(evt_npc_move_position, LW(0), 280, LW(4), 0, FLOAT(60.0f), 1)
        CALL(evt_npc_jump_position_nohit, LW(0), 250, 45, LW(4), 500, 30)
        CALL(evt_npc_jump_position_nohit, LW(0), 215, 65, LW(4), 500, 30)
        CALL(evt_npc_jump_position_nohit, LW(0), 185, 85, LW(4), 500, 30)
        CALL(evt_npc_move_position, LW(0), LW(3), LW(4), 0, FLOAT(60.0f), 1)
    ELSE
        CALL(evt_npc_move_position, LW(0), 280, LW(4), 0, FLOAT(60.0f), 1)
        CALL(evt_npc_jump_position_nohit, LW(0), 250, 20, LW(4), 500, 30)
        CALL(evt_npc_jump_position_nohit, LW(0), 215, 40, LW(4), 500, 30)
        CALL(evt_npc_jump_position_nohit, LW(0), 185, 60, LW(4), 500, 30)
        CALL(evt_npc_sound_data_set, LW(0), 0, 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        CALL(evt_npc_move_position, LW(0), LW(3), LW(4), 0, FLOAT(60.0f), 1)
        CALL(evt_npc_sound_data_reset, LW(0))
        CALL(tribe_chk, LW(0), STRING(/*"バーストワンワン"*/"\203o\201[\203X\203g\203\217\203\223\203\217\203\223"), LW(5))
        IF_INT_EQUAL(LW(5), 1)
            SPAWN_EVENT(burst_wangwang)
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(burst_chain_entry)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_npc_entry, STRING(("\215\2750_0")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_1")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_2")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_3")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_4")), STRING(("c_wanwan_a")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_0")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_1")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_2")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_3")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_4")), STRING(("WAN_S_1B")))
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_0")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_1")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_2")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_3")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_4")), 1074003968)
        CALL(burst_chain, STRING(("\223G\202P")), 0)
        CASE_EQUAL(1)
        CALL(evt_npc_entry, STRING(("\215\2751_0")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_1")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_2")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_3")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_4")), STRING(("c_wanwan_a")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_0")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_1")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_2")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_3")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_4")), STRING(("WAN_S_1B")))
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_0")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_1")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_2")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_3")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_4")), 1074003968)
        CALL(burst_chain, STRING(("\223G\202Q")), 1)
        CASE_EQUAL(6)
        CALL(evt_npc_entry, STRING(("\215\2750_0")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_1")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_2")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_3")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2750_4")), STRING(("c_wanwan_a")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_0")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_1")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_2")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_3")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2750_4")), STRING(("WAN_S_1B")))
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_0")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_1")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_2")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_3")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2750_4")), 1074003968)
        CALL(burst_chain, STRING(("\221I\216\3501")), 0)
        CASE_EQUAL(7)
        CALL(evt_npc_entry, STRING(("\215\2751_0")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_1")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_2")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_3")), STRING(("c_wanwan_a")))
        CALL(evt_npc_entry, STRING(("\215\2751_4")), STRING(("c_wanwan_a")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_0")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_1")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_2")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_3")), STRING(("WAN_S_1B")))
        CALL(evt_npc_set_anim, STRING(("\215\2751_4")), STRING(("WAN_S_1B")))
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_0")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_1")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_2")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_3")), 1074003968)
        CALL(evt_npc_flag_onoff, 1, STRING(("\215\2751_4")), 1074003968)
        CALL(burst_chain, STRING(("\221I\216\3502")), 1)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(burst_wangwang)
    SET_VALUE(LW(10), LW(0))
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(evt_sub_random, 360, LW(3))
        CALL(evt_sub_get_sincos, LW(3), LW(4), LW(5))
        MULTIPLY_FLOAT(LW(4), FLOAT(10.0f))
        MULTIPLY_FLOAT(LW(5), FLOAT(10.0f))
        ADD_FLOAT(LW(4), LW(0))
        ADD_FLOAT(LW(5), LW(2))
        CALL(evt_sub_random, 200, LW(6))
        ADD(LW(6), 300)
        CALL(evt_sub_random, 20, LW(7))
        ADD(LW(7), 20)
        CALL(evt_npc_flag_onoff, 0, LW(10), NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_jump_position_nohit, LW(10), LW(4), LW(1), LW(5), LW(6), LW(7))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_FLD_ENM_WAN_LANDING1")), LW(4), LW(1), LW(5), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_match)
#ifdef VERSION_G8ME01
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_mario_set_pos, -600, 0, 0)
        CALL(evt_party_set_pos, 0, -610, 0, -10)
    END_INLINE_EVENT
#endif
    SWITCH(GSW_GLITZ_PIT_OPPONENT)
        CASE_EQUAL(19)
        IF_INT_EQUAL(GSWF(2395), 0)
            SET_VALUE(GSWF(2395), 1)
            CALL_CHILD(PTR(evt_kurikuri_opening))
            ELSE
            CALL_CHILD(PTR(evt_default_opening))
        END_IF
        CASE_EQUAL(10)
        IF_INT_EQUAL(GSWF(2397), 0)
            SET_VALUE(GSWF(2397), 1)
            CALL_CHILD(PTR(evt_shinemon_opening))
            ELSE
            CALL_CHILD(PTR(evt_default_opening))
        END_IF
        CASE_EQUAL(0)
        IF_INT_EQUAL(GSWF(2399), 0)
            SET_VALUE(GSWF(2399), 1)
            IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
                CALL_CHILD(PTR(evt_champ_opening))
                ELSE
                CALL_CHILD(PTR(evt_champ_opening_re))
            END_IF
            ELSE
            CALL_CHILD(PTR(evt_default_opening))
        END_IF
        CASE_DEFAULT
        CALL_CHILD(PTR(evt_default_opening))
    END_SWITCH
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    IF_INT_EQUAL(GSWF(2509), 0)
#ifdef VERSION_G8ME01
        CALL(evt_npc_set_attack_mode, STRING("敵１"), 16)
        CALL(_firstattack_setting)
    END_IF
    CALL(evt_npc_set_battle_info, STRING("敵１"), LW(1))
    CALL(evt_npc_set_battle_rule, STRING("敵１"), GSW_GLITZ_PIT_CONDITION, GSW(503), GSW(504))
    CALL(evt_npc_battle_start, STRING("敵１"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_kill_party, 1)
#else
        CALL(evt_npc_set_attack_mode, STRING(("\223G\202P")), 16)
        CALL(_firstattack_setting)
    END_IF
    CALL(evt_npc_set_battle_info, STRING(("\223G\202P")), LW(1))
    CALL(evt_npc_set_battle_rule, STRING(("\223G\202P")), GSW_GLITZ_PIT_CONDITION, GSW(503), GSW(504))
    CALL(evt_npc_battle_start, STRING(("\223G\202P")))
    CALL(evt_npc_wait_battle_end)
#endif
    CALL(evt_npc_get_battle_rule_keep_result, LW(0))
    CALL(evt_tou_set_rule_result, LW(0))
    SET_VALUE(GSWF_GLITZ_PIT_MATCH_RESERVED, 0)
    SET_VALUE(GSWF(2389), 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), -50)
    ADD(LW(2), -20)
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_battle_result, LW(0))
    BEGIN_INLINE_EVENT
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
#ifdef VERSION_G8ME01
            CALL(evt_npc_set_stay_anim, STRING("ガンス"))
            CALL(evt_npc_set_autotalkpose, STRING("ガンス"), STRING("S_2"), STRING("T_2"))
            ELSE
            CALL(evt_npc_set_stay_anim, STRING("キノシコワ"))
            CALL(evt_npc_set_autotalkpose, STRING("キノシコワ"), STRING("S_2"), STRING("T_2"))
#else
            CALL(evt_npc_set_stay_anim, STRING(("\203K\203\223\203X")))
            CALL(evt_npc_set_autotalkpose, STRING(("\203K\203\223\203X")), STRING(("S_2")), STRING(("T_2")))
            ELSE
            CALL(evt_npc_set_stay_anim, STRING(("\203L\203m\203V\203R\203\217")))
            CALL(evt_npc_set_autotalkpose, STRING(("\203L\203m\203V\203R\203\217")), STRING(("S_2")), STRING(("T_2")))
#endif
        END_IF
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(0), 1)
        SWITCH(GSW_GLITZ_PIT_OPPONENT)
            CASE_EQUAL(19)
#ifdef VERSION_G8ME01
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_ENV_TOU2"))
#else
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_FF_ENV_TOU2")))
#endif
            IF_INT_EQUAL(GSWF(2396), 0)
                SET_VALUE(GSWF(2396), 1)
                CALL_CHILD(PTR(evt_kurikuri_win))
                ELSE
                CALL_CHILD(PTR(evt_default_win))
            END_IF
            CASE_EQUAL(10)
#ifdef VERSION_G8ME01
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_ENV_TOU2"))
#else
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_FF_ENV_TOU2")))
#endif
            IF_INT_EQUAL(GSWF(2398), 0)
                SET_VALUE(GSWF(2398), 1)
                CALL_CHILD(PTR(evt_shinemon_win))
                ELSE
                CALL_CHILD(PTR(evt_default_win))
            END_IF
            CASE_EQUAL(0)
#ifdef VERSION_G8ME01
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_ENV_TOU3"))
#else
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_FF_ENV_TOU3")))
#endif
            IF_INT_EQUAL(GSWF(2400), 0)
                SET_VALUE(GSWF(2400), 1)
                CALL_CHILD(PTR(evt_champ_win))
                ELSE
                CALL_CHILD(PTR(evt_default_win))
            END_IF
            CASE_DEFAULT
#ifdef VERSION_G8ME01
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_ENV_TOU2"))
#else
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_FF_ENV_TOU2")))
#endif
            CALL_CHILD(PTR(evt_default_win))
        END_SWITCH
        CALL(evt_tou_get_ranking, LW(10))
        CALL(evt_tou_set_ranking_win_lose, 0, GSW_GLITZ_PIT_OPPONENT)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
            IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_30_A_NEW_CHAMPION)
                CALL(evt_tou_get_ranking, LW(10))
                ELSE
                IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_20_MAJORS_10TH_PLACE)
                    IF_INT_GREATER(LW(10), 1)
                        CALL(evt_tou_get_ranking, LW(10))
                    END_IF
                END_IF
            END_IF
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(7)
                IF_INT_EQUAL(GSWF(2417), 0)
                    CALL_CHILD(PTR(evt_random_battle))
                END_IF
                CASE_EQUAL(5)
                IF_INT_EQUAL(GSWF(2419), 0)
                    CALL_CHILD(PTR(evt_random_battle2))
                END_IF
            END_SWITCH
            ELSE
            IF_INT_EQUAL(LW(10), 1)
                IF_INT_EQUAL(GSW_GLITZ_PIT_OPPONENT, 0)
                    SET_VALUE(LW(10), 0)
                END_IF
            END_IF
        END_IF
        ELSE
#ifdef VERSION_G8ME01
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_ENV_TOU2"))
#else
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_FF_ENV_TOU2")))
#endif
        SWITCH(GSW_GLITZ_PIT_OPPONENT)
            CASE_EQUAL(19)
            CALL_CHILD(PTR(evt_kurikuri_lose))
            CASE_EQUAL(10)
            CALL_CHILD(PTR(evt_shinemon_lose))
            CASE_EQUAL(0)
            CALL_CHILD(PTR(evt_champ_lose))
            CASE_DEFAULT
            CALL_CHILD(PTR(evt_default_lose))
        END_SWITCH
        IF_INT_EQUAL(GSW_GLITZ_PIT_OPPONENT, 10)
            IF_INT_LESS(GSW(505), 100)
                ADD(GSW(505), 1)
            END_IF
        END_IF
        CALL(evt_tou_set_ranking_win_lose, 1, GSW_GLITZ_PIT_OPPONENT)
        CALL(evt_tou_get_ranking, LW(10))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_06_1ST_MATCH)
        SET_VALUE(GSW_STORY_PROGRESS, STG3_06_1ST_MATCH)
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(GSWF(2383), 0)
    END_IF
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
    CALL(evt_snd_envoff, PSND_FADEOUT)
    CALL(snd_sfx_stop)
    SWITCH(LW(10))
        CASE_LESS(1)
#ifdef VERSION_G8ME01
        CALL(evt_bero_mapchange, STRING("tou_07"), 0)
        CASE_LESS(11)
        CALL(evt_bero_mapchange, STRING("tou_08"), 0)
        CASE_DEFAULT
        CALL(evt_bero_mapchange, STRING("tou_10"), 0)
#else
        CALL(evt_bero_mapchange, STRING(("tou_07")), 0)
        CASE_LESS(11)
        CALL(evt_bero_mapchange, STRING(("tou_08")), 0)
        CASE_DEFAULT
        CALL(evt_bero_mapchange, STRING(("tou_10")), 0)
#endif
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(evt_kurikuri_opening)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_status_onoff, 1, STRING(/*"ガンス"*/"\203K\203\223\203X"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING(/*"ガンス"*/"\203K\203\223\203X"), 0, 60, -130)
    CALL(evt_npc_set_position, STRING(/*"敵１"*/"\223G\202P"), 50, 60, 0)
    CALL(evt_npc_set_position, STRING(/*"敵２"*/"\223G\202Q"), 90, 60, 25)
    CALL(evt_npc_set_position, STRING(/*"敵３"*/"\223G\202R"), 70, 60, -25)
    CALL(evt_npc_set_position, STRING(/*"敵４"*/"\223G\202S"), 110, 60, 0)
    CALL(evt_npc_set_position, STRING(/*"敵５"*/"\223G\202T"), 130, 60, -25)
    CALL(evt_cam3d_evt_set, 1075, 547, 998, 0, 287, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_bgmon, PSND_FADEOUT, PTR("BGM_FF_ENV_TOU1"))
    CALL(evt_cam3d_evt_set, -1024, 547, 1050, 0, 287, -8, 6000, EASING_LINEAR)
    WAIT_MS(6000)
    CALL(evt_cam3d_evt_set, 0, 127, 105, 0, 95, -145, 0, EASING_LINEAR)
    CALL(evt_msg_print, 0, PTR("stg3_tou_77"), 0, STRING(/*"ガンス"*/"\203K\203\223\203X"))
    CALL(evt_cam3d_evt_set, 70, 137, 260, 70, 96, -70, 2000, EASING_LINEAR)
    CALL(evt_npc_move_position, STRING(/*"ガンス"*/"\203K\203\223\203X"), 10, -20, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, PTR("stg3_tou_78"), 0, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")))
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("M_5"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("M_6"))
    CALL(evt_msg_print, 0, PTR("stg3_tou_79"), 0, PTR(/*("\223G\202Q")*/("\223G\202Q")))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, PTR("eff"), PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("M_7"))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("S_2"))
    CALL(evt_set_dir_to_target, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")), PTR("mario"))
    CALL(evt_mario_set_pos, -550, 0, 0)
    CALL(evt_party_set_pos, 0, -560, 0, -10)
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    CALL_CHILD(w_door_open)
    CALL(evt_cam3d_evt_off, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, PTR("SFX_DOOR_SHUT_IRON2"), -565, 0, 0, 0)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -180, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL_CHILD(w_door_open)
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_msg_print, 0, PTR("stg3_tou_80"), 0, 0)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -450, 0, FLOAT(120.0f))
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_move_pos2, 0, -280, 25, FLOAT(120.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -250, 20, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -215, 40, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -185, 60, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_move_pos2, 0, -90, -25, FLOAT(120.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -280, 0, FLOAT(120.0f))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, PTR("SFX_VOICE_MARIO_FLD_JUMP3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, -250, 20, 0, 400, 1, -1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, PTR("SFX_VOICE_MARIO_FLD_JUMP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, -215, 40, 0, 400, 1, -1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, PTR("SFX_VOICE_MARIO_FLD_JUMP5"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_jump_pos, -185, 60, 0, 400, 1, -1000)
    CALL(evt_mario_mov_pos2, -55, 0, FLOAT(120.0f))
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_move_position, STRING(/*"ガンス"*/"\203K\203\223\203X"), -20, -20, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, PTR("stg3_tou_81"), 0, PTR(/*("\203K\203\223\203X")*/("\203K\203\223\203X")))
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, PTR("stg3_tou_82"), 0, PTR(/*("\223G\202P")*/("\223G\202P")))
    END_INLINE_EVENT
    CALL(evt_npc_move_position, PTR(/*("\223G\202P")*/("\223G\202P")), 0, 0, 0, FLOAT(100.0f), 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, PTR("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, PTR("M_N_7"))
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(10)
            ADD_FLOAT(LW(0), FLOAT(13.5f))
            CALL(evt_npc_set_rotate, STRING(/*"敵１"*/"\223G\202P"), 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_sound_data_set, PTR(/*("\223G\202P")*/("\223G\202P")), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, PTR("SFX_STG3_KURI_ATTACK1"), EVT_NULLPTR)
    CALL(evt_npc_jump_position_nohit, STRING(/*"敵１"*/"\223G\202P"), -30, 100, 0, 800, 100)
    RETURN
EVT_END()

EVT_BEGIN(evt_kurikuri_win)
    CALL(evt_cam3d_evt_set, 70, 138, 263, 70, 90, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 50, 60, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202Q")), 90, 80, 25)
    CALL(evt_npc_set_position, STRING(("\223G\202R")), 70, 60, -25)
    CALL(evt_npc_set_position, STRING(("\223G\202S")), 110, 60, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202T")), 130, 60, -25)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_rotate, STRING(("\223G\202P")), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING(("\223G\202Q")), 0, 0, 180)
    CALL(evt_npc_set_rotate, STRING(("\223G\202R")), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING(("\223G\202S")), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING(("\223G\202T")), 0, 0, 0)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING(("\223G\202P")), STRING(("KUR_K_1")))
    CALL(evt_npc_set_anim, STRING(("\223G\202Q")), STRING(("KUR_D_1")))
    CALL(evt_npc_set_anim, STRING(("\223G\202R")), STRING(("KUR_K_1")))
    CALL(evt_npc_set_anim, STRING(("\223G\202S")), STRING(("KUR_K_1")))
    CALL(evt_npc_set_anim, STRING(("\223G\202T")), STRING(("KUR_K_1")))
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL_CHILD(PTR(evt_kime_pose))
    CALL(evt_npc_get_position, STRING(("\223G\202R")), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_KURI_FURAFURA1")), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_flag_onoff, 0, STRING(("\223G\202P")), 512)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_86")), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING(("\203K\203\223\203X")), STRING(("mario")))
    CALL(evt_cam3d_evt_set, -70, 138, 263, -70, 90, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_87")), 0, STRING(("\203K\203\223\203X")))
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_5")))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_6")))
    CALL(evt_msg_select, 0, STRING(("stg3_tou_87_yn")))
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_7")))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("S_2")))
    CALL(evt_msg_print_add, 0, STRING(("stg3_tou_88")))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(("")), STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_WIN_AUDIENCE2")), LW(0), LW(1), LW(2), 0)
    CALL(evt_cam3d_evt_set, 0, 850, 2160, 0, 287, -8, 10000, EASING_LINEAR)
    WAIT_MS(7000)
    RETURN
EVT_END()

EVT_BEGIN(evt_kurikuri_lose)
    CALL_CHILD(PTR(evt_damage_pose))
    CALL(evt_npc_set_rotate, STRING(("\223G\202P")), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING(("\223G\202Q")), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING(("\223G\202R")), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING(("\223G\202S")), 0, 0, 0)
    CALL(evt_npc_set_rotate, STRING(("\223G\202T")), 0, 0, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 50, 60, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202Q")), 90, 60, 25)
    CALL(evt_npc_set_position, STRING(("\223G\202R")), 70, 60, -25)
    CALL(evt_npc_set_position, STRING(("\223G\202S")), 110, 60, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202T")), 130, 60, -25)
    CALL(evt_npc_get_position, STRING(("\223G\202Q")), LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(("")), STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_cam3d_evt_set, 70, 138, 263, 70, 90, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_WIN_AUDIENCE1")), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(2), STRING(("\203K\203\223\203X")))
        ELSE
        SET_VALUE(LW(2), STRING(("\203L\203m\203V\203R\203\217")))
    END_IF
    CALL(evt_npc_set_position, LW(2), 33, 60, -83)
    CALL(evt_set_dir_to_target, LW(2), STRING(("\223G\202P")))
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    CALL(evt_msg_print_insert, 0, STRING(("stg3_tou_118_61")), 0, LW(2), LW(0))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_85")), 0, STRING(("\223G\202P")))
    RETURN
EVT_END()

EVT_BEGIN(evt_shinemon_opening)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_status_onoff, 1, STRING(("\203K\203\223\203X")), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 0, 60, -130)
    CALL(evt_cam3d_evt_set, 1075, 547, 998, 0, 287, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_FF_ENV_TOU1")))
    CALL(evt_cam3d_evt_set, -1024, 547, 1050, 0, 287, -8, 5000, EASING_LINEAR)
    WAIT_MS(5000)
    CALL(evt_cam3d_evt_set, 0, 127, 105, 0, 95, -145, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_257")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_258_00")), 0, STRING(("\203K\203\223\203X")))
    CALL(evt_mario_set_pos, -550, 0, 0)
    CALL(evt_party_set_pos, 0, -560, 0, -10)
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    CALL_CHILD(PTR(w_door_open))
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING(("SFX_DOOR_SHUT_IRON2")), -565, 0, 0, 0)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -180, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL_CHILD(PTR(w_door_open))
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -450, 0, FLOAT(120.0f))
        WAIT_MS(1000)
        SET_VALUE(GF(1), 1)
        CALL(evt_mario_mov_pos2, -280, 0, FLOAT(120.0f))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP1")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -250, 20, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP2")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -215, 40, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP3")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -185, 60, 0, 400, 1, -1000)
        CALL(evt_mario_mov_pos2, -80, 0, FLOAT(120.0f))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            IF_INT_EQUAL(GF(1), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(300)
        CALL(evt_party_move_pos2, 0, -280, 25, FLOAT(120.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -250, 20, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -215, 40, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -185, 60, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_move_pos2, 0, -130, -25, FLOAT(120.0f))
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -426, 30, -195)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_258_01")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -379, 30, 133)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_258_02")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -293, 60, -180)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_258_03")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -272, 90, 200)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_258_04")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_move_position, STRING(("\203K\203\223\203X")), -20, -20, 0, FLOAT(70.0f), 1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_259")), 0, STRING(("\203K\203\223\203X")))
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_5")))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_6")))
    CALL(evt_msg_select, 0, STRING(("stg3_tou_259_yn")))
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_7")))
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("S_2")))
    CALL(evt_msg_print_add, 0, STRING(("stg3_tou_260")))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(("e")), STRING(("\203K\203\223\203X")), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_set_dir_to_target, STRING(("\203K\203\223\203X")), STRING(("\223G\202P")))
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    CALL_CHILD(PTR(e_door_open))
    CALL(evt_cam3d_evt_set, 475, 134, 533, 475, 38, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 550, 0, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202Q")), 550, 0, 0)
    SET_VALUE(LW(0), STRING(("\223G\202Q")))
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 2)
    SPAWN_EVENT(PTR(evt_teki_nyuujyou))
    WAIT_MS(500)
    SET_VALUE(LW(0), STRING(("\223G\202P")))
    SET_VALUE(LW(1), 1)
    SET_VALUE(LW(2), 2)
    SPAWN_EVENT(PTR(evt_teki_nyuujyou))
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING(("SFX_DOOR_SHUT_IRON2")), 565, 0, 0, 0)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -180, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL_CHILD(PTR(e_door_open))
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 426, 30, -195)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_261_01")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 379, 30, 133)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_261_02")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 293, 30, -180)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_261_03")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_set, 50, 146, 273, 50, 96, -8, 6000, EASING_LINEAR)
    WAIT_MS(6000)
    CALL(evt_npc_move_position, STRING(("\203K\203\223\203X")), 0, -30, 0, FLOAT(70.0f), 1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_262")), 0, STRING(("\203K\203\223\203X")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_263")), 0, STRING(("\223G\202P")))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 80, 146, 273, 80, 96, -8, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_264")), 0, STRING(("\223G\202Q")))
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 0, 60, -130)
    CALL(evt_cam3d_evt_set, 0, 150, 141, 0, 100, -154, 0, EASING_SINE_IN_OUT)
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_1")))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_2")))
    CALL(evt_npc_set_autotalkpose, STRING(("\203K\203\223\203X")), STRING(("M_2")), STRING(("M_3")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_116_20_01")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_GONG1")), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 0, 250, 730, 0, 100, -154, 2000, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(evt_shinemon_win)
    SET_VALUE(GSWF(2512), 1)
    CALL(evt_cam3d_evt_set, 60, 143, 263, 60, 95, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 55, 60, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202Q")), 95, 60, 0)
    CALL(evt_npc_set_anim, STRING(("\223G\202P")), STRING(("SIN_K_1")))
    CALL(evt_npc_set_anim, STRING(("\223G\202Q")), STRING(("SIN_K_1")))
    CALL(evt_npc_flag_onoff, 0, STRING(("\223G\202P")), 1536)
    CALL(evt_npc_flag_onoff, 0, STRING(("\223G\202Q")), 1536)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    ADD(LW(2), -30)
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), LW(0), LW(1), LW(2))
    CALL(evt_set_dir_to_target, STRING(("\203K\203\223\203X")), STRING(("mario")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_275")), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_276")), 0, STRING(("\223G\202Q")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -65, 143, 263, -65, 95, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_277")), 0, STRING(("\203K\203\223\203X")))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(("")), STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mario_set_pose, STRING(("M_I_U")))
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(400)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(400)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(400)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_PAUSE2")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_V_2")))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_WIN_AUDIENCE2")), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 850, 2160, 0, 287, -8, 10000, EASING_LINEAR)
    WAIT_MS(7000)
    RETURN
EVT_END()

EVT_BEGIN(evt_shinemon_lose)
    CALL_CHILD(PTR(evt_damage_pose))
    CALL(evt_npc_get_position, STRING(("\223G\202P")), LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(("")), STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_WIN_AUDIENCE1")), 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(2), STRING(("\203K\203\223\203X")))
        ELSE
        SET_VALUE(LW(2), STRING(("\203L\203m\203V\203R\203\217")))
    END_IF
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    CALL(evt_msg_print_insert, 0, STRING(("stg3_tou_118_61")), 0, LW(2), LW(0))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 60, 143, 263, 60, 95, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_274")), 0, STRING(("\223G\202P")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_274_01")), 0, STRING(("\223G\202Q")))
    WAIT_MS(300)
    RETURN
EVT_END()

EVT_BEGIN(evt_champ_opening)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_status_onoff, 1, STRING(("\203K\203\223\203X")), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 0, 60, 0)
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 80, 60, 0)
    CALL(evt_npc_flag_onoff, 1, STRING(("\203K\203\223\203X")), 32768)
    CALL(evt_set_dir_to_target, STRING(("\203K\203\223\203X")), STRING(("\223G\202P")))
    CALL(evt_set_dir_to_target, STRING(("\223G\202P")), STRING(("\203K\203\223\203X")))
    CALL(evt_cam3d_evt_set, 0, 192, 620, 0, 95, -145, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_tribe, STRING(("\223G\202P")), STRING(("\203u\203\214\203b\203h\203n\201[\203g\203x\203\213\203g")))
    BEGIN_INLINE_EVENT
        CALL(tou_champion_belt, STRING(("\223G\202P")))
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG3_TOU2")))
    CALL(evt_snd_bgm_scope, 0, 1)
    WAIT_MS(2000)
    CALL(evt_npc_flag_onoff, 0, STRING(("\203K\203\223\203X")), 32768)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING(("a")), STRING(("\203K\203\223\203X")), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_set_ry, STRING(("\203K\203\223\203X")), 270)
    WAIT_MS(500)
    CALL(evt_mario_set_pos, -550, 0, 0)
    CALL(evt_party_set_pos, 0, -560, 0, -10)
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    CALL_CHILD(PTR(w_door_open))
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING(("SFX_DOOR_SHUT_IRON2")), -565, 0, 0, 0)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -180, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL_CHILD(PTR(w_door_open))
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -450, 0, FLOAT(120.0f))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 155, 330, 0, 95, -145, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_424")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 328, 1219, 0, 130, 145, 3000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -200, 30, -200)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_425")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 30, 200)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_426")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -300, 200, -400)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_427")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 200, -300)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_428")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(5000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 30, 133)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_429")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(7000)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING(("stg3_tou_430")))
    WAIT_MS(300)
    CALL(evt_party_run, 0)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -280, 0, FLOAT(60.0f))
        CALL(evt_cam3d_evt_set, 0, 192, 620, 0, 95, -145, 4000, EASING_SINE_IN_OUT)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP5")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -250, 20, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP4")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -215, 40, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP3")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -185, 60, 0, 400, 1, -1000)
        CALL(evt_mario_mov_pos2, -80, 0, FLOAT(60.0f))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_stop, 0)
        WAIT_MS(300)
        CALL(evt_party_move_pos2, 0, -280, 25, FLOAT(60.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -250, 20, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -215, 40, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -185, 60, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_move_pos2, 0, -130, -25, FLOAT(60.0f))
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -426, 30, -195)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_430_01")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -379, 30, 233)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_430_02")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -293, 30, -180)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_430_03")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -272, 30, 100)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_430_04")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -150, 100, -200)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_430_04")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_1")))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_2")))
    CALL(evt_npc_set_autotalkpose, STRING(("\203K\203\223\203X")), STRING(("M_2")), STRING(("M_3")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_431")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_GONG1")), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 0, 250, 730, 0, 100, -154, 2000, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(evt_champ_opening_re)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_npc_status_onoff, 1, STRING(("\203L\203m\203V\203R\203\217")), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING(("\203L\203m\203V\203R\203\217")), 0, 60, -130)
    CALL(evt_cam3d_evt_set, 0, 481, 1310, 0, 295, -145, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG3_TOU2")))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam3d_evt_set, 0, 281, 1310, 0, 95, -145, 3000, EASING_LINEAR)
    WAIT_MS(3000)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 127, 105, 0, 95, -145, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_086")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_087")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 550, 0, 0)
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    CALL_CHILD(PTR(e_door_open))
    CALL(evt_cam3d_evt_set, 475, 134, 533, 475, 38, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING(("SFX_DOOR_SHUT_IRON2")), 565, 0, 0, 0)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -180, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL_CHILD(PTR(e_door_open))
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\223G\202P")), 450, 0, 0, FLOAT(60.0f), 1)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING(("\223G\202P")), 310, 0, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING(("\223G\202P")), 280, 20, 0, 500, 30)
        CALL(evt_npc_jump_position_nohit, STRING(("\223G\202P")), 245, 40, 0, 500, 30)
        CALL(evt_npc_jump_position_nohit, STRING(("\223G\202P")), 215, 60, 0, 500, 30)
        CALL(evt_npc_move_position, STRING(("\223G\202P")), 80, 0, 0, FLOAT(60.0f), 1)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 426, 30, -195)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_089")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 379, 30, 133)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_090")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 293, 30, -180)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_091")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(5000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 30, 133)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_092")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_set, 50, 146, 273, 50, 96, -8, 6000, EASING_LINEAR)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_npc_move_position, STRING(("\203L\203m\203V\203R\203\217")), 20, -20, 0, FLOAT(70.0f), 1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_093")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_094")), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_095")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_096")), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_097")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    CALL(evt_npc_set_ry, STRING(("\203L\203m\203V\203R\203\217")), 270)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING(("\203L\203m\203V\203R\203\217")), 0, -130, 0, FLOAT(70.0f), 1)
    END_INLINE_EVENT
    CALL(evt_mario_set_pos, -550, 0, 0)
    CALL(evt_party_set_pos, 0, -560, 0, -10)
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    CALL_CHILD(PTR(w_door_open))
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING(("SFX_DOOR_SHUT_IRON2")), -565, 0, 0, 0)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -180, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL_CHILD(PTR(w_door_open))
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -450, 0, FLOAT(120.0f))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 328, 1219, 0, 130, 145, 3000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        WAIT_MS(2000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -200, 30, -200)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_098")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 30, 200)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_099")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -300, 200, -400)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_100")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 200, -300)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_101")), 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(5000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 30, 133)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_102")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(7000)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_party_stop, 0)
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING(("stg3_tou_re_103")))
    WAIT_MS(300)
    CALL(evt_party_run, 0)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -280, 0, FLOAT(60.0f))
        CALL(evt_cam3d_evt_set, 0, 192, 620, 0, 95, -145, 5000, EASING_SINE_IN_OUT)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP5")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -250, 20, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP2")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -215, 40, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP4")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -185, 60, 0, 400, 1, -1000)
        CALL(evt_mario_mov_pos2, -80, 0, FLOAT(60.0f))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_party_move_pos2, 0, -280, 25, FLOAT(60.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -250, 20, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -215, 40, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -185, 60, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_move_pos2, 0, -130, -25, FLOAT(60.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -426, 30, -195)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_110")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -379, 30, 233)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_111")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -293, 60, -180)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_112")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -272, 90, 300)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_113")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -150, 100, -200)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_re_114")), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_anim, STRING(("\203L\203m\203V\203R\203\217")), STRING(("A_2")))
    WAIT_MS(200)
    CALL(evt_npc_set_autotalkpose, STRING(("\203L\203m\203V\203R\203\217")), STRING(("S_3")), STRING(("T_3")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_re_115")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_GONG1")), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 0, 250, 730, 0, 100, -154, 2000, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(evt_champ_win)
    CALL(tou_champion_belt_onoff, 1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(15), STRING(("\203K\203\223\203X")))
        SET_VALUE(LW(14), STRING(("stg3_tou_438")))
        SET_VALUE(LW(13), STRING(("stg3_tou_439")))
        ELSE
        SET_VALUE(LW(15), STRING(("\203L\203m\203V\203R\203\217")))
        SET_VALUE(LW(14), STRING(("stg3_tou_re_133")))
        SET_VALUE(LW(13), STRING(("stg3_tou_re_134")))
    END_IF
    CALL(evt_npc_set_position, LW(15), -20, 60, -50)
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 80, 60, 0)
    CALL(evt_npc_get_position, STRING(("\223G\202P")), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_BLED_DIE1")), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING(("\223G\202P")), STRING(("D_3")))
    CALL(evt_npc_set_autotalkpose, STRING(("\223G\202P")), STRING(("S_10")), STRING(("T_10")))
    CALL(evt_cam3d_evt_set, 80, 146, 273, 80, 96, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, LW(14), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -80, 146, 273, -80, 96, -8, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, LW(13), 0, LW(15))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(("")), STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_WIN_AUDIENCE1")), 0)
    CALL(evt_mario_set_pose, STRING(("M_I_U")))
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(400)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(400)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(400)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_PAUSE2")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_V_2")))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_WIN_AUDIENCE2")), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_cam3d_evt_set, 1075, 547, 998, 0, 287, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -1024, 547, 1050, 0, 287, -8, 5000, EASING_LINEAR)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(15), STRING(("stg3_tou_440")))
        SET_VALUE(LW(14), STRING(("stg3_tou_441")))
        SET_VALUE(LW(13), STRING(("stg3_tou_442")))
        SET_VALUE(LW(12), STRING(("stg3_tou_443")))
        SET_VALUE(LW(11), STRING(("stg3_tou_444")))
        ELSE
        SET_VALUE(LW(15), STRING(("stg3_tou_re_135")))
        SET_VALUE(LW(14), STRING(("stg3_tou_re_136")))
        SET_VALUE(LW(13), STRING(("stg3_tou_re_137")))
        SET_VALUE(LW(12), STRING(("stg3_tou_re_138")))
        SET_VALUE(LW(11), STRING(("stg3_tou_re_139")))
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -300, 300, -195)
        CALL(evt_msg_print, 0, LW(15), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, -150, 100, -133)
        CALL(evt_msg_print, 0, LW(14), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 0, 300, -180)
        CALL(evt_msg_print, 0, LW(13), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 150, 30, -200)
        CALL(evt_msg_print, 0, LW(12), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 300, 200, -200)
        CALL(evt_msg_print, 0, LW(11), 0, 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    RETURN
EVT_END()

EVT_BEGIN(evt_champ_lose)
    CALL_CHILD(PTR(evt_damage_pose))
    CALL(evt_npc_get_position, STRING(("\223G\202P")), LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(("")), STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_WIN_AUDIENCE1")), 0)
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(2), STRING(("\203K\203\223\203X")))
        ELSE
        SET_VALUE(LW(2), STRING(("\203L\203m\203V\203R\203\217")))
    END_IF
    CALL(evt_msg_print_insert, 0, STRING(("stg3_tou_118_61")), 0, LW(2), LW(0))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 40, 142, 250, 40, 96, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_tou_get_ranking, LW(1))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSWF(2383), 1)
            SET_VALUE(LW(0), STRING(("stg3_tou_437")))
            ELSE
            IF_INT_EQUAL(LW(1), 0)
                SET_VALUE(LW(0), STRING(("stg3_tou_400_03")))
                ELSE
                SET_VALUE(LW(0), STRING(("stg3_tou_400_01")))
            END_IF
        END_IF
        ELSE
        IF_INT_EQUAL(GSWF(2383), 1)
            SET_VALUE(LW(0), STRING(("stg3_tou_re_132")))
            ELSE
            IF_INT_EQUAL(LW(1), 0)
                SET_VALUE(LW(0), STRING(("stg3_tou_re_290")))
                ELSE
                SET_VALUE(LW(0), STRING(("stg3_tou_re_288")))
            END_IF
        END_IF
    END_IF
    CALL(evt_msg_print, 0, LW(0), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    RETURN
EVT_END()

EVT_BEGIN(evt_default_opening)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(0), STRING(("\203K\203\223\203X")))
        ELSE
        SET_VALUE(LW(0), STRING(("\203L\203m\203V\203R\203\217")))
    END_IF
    CALL(evt_npc_status_onoff, 1, LW(0), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, LW(0), 0, 60, -130)
    CALL(evt_mario_set_pos, -350, 0, 0)
    CALL(evt_party_set_pos, 0, -390, 0, 25)
    CALL(evt_tou_get_fighter_num, GSW_GLITZ_PIT_OPPONENT, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_npc_set_position, STRING(("\223G\202P")), 350, 0, 0)
        CASE_EQUAL(2)
        CALL(evt_npc_set_position, STRING(("\223G\202P")), 350, 0, 0)
        CALL(evt_npc_set_position, STRING(("\223G\202Q")), 390, 0, 25)
        CASE_EQUAL(3)
        CALL(evt_npc_set_position, STRING(("\223G\202P")), 350, 0, 0)
        CALL(evt_npc_set_position, STRING(("\223G\202Q")), 390, 0, 25)
        CALL(evt_npc_set_position, STRING(("\223G\202R")), 370, 0, -25)
        CASE_EQUAL(4)
        CALL(evt_npc_set_position, STRING(("\223G\202P")), 350, 0, 0)
        CALL(evt_npc_set_position, STRING(("\223G\202Q")), 390, 0, 25)
        CALL(evt_npc_set_position, STRING(("\223G\202R")), 370, 0, -25)
        CALL(evt_npc_set_position, STRING(("\223G\202S")), 410, 0, 0)
        CASE_EQUAL(5)
        CALL(evt_npc_set_position, STRING(("\223G\202P")), 350, 0, 0)
        CALL(evt_npc_set_position, STRING(("\223G\202Q")), 390, 0, 25)
        CALL(evt_npc_set_position, STRING(("\223G\202R")), 370, 0, -25)
        CALL(evt_npc_set_position, STRING(("\223G\202S")), 410, 0, 0)
        CALL(evt_npc_set_position, STRING(("\223G\202T")), 430, 0, -25)
    END_SWITCH
    CALL(evt_cam3d_evt_set, 0, 585, 1310, 0, 400, -145, 0, EASING_SINE_IN_OUT)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_30_A_NEW_CHAMPION)
        IF_INT_EQUAL(GSW_GLITZ_PIT_OPPONENT, 0)
            CALL(evt_npc_set_tribe, STRING(("\223G\202P")), STRING(("\203u\203\214\203b\203h\203n\201[\203g\203x\203\213\203g")))
            BEGIN_INLINE_EVENT
                CALL(tou_champion_belt, STRING(("\223G\202P")))
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_FF_ENV_TOU1")))
    CALL(evt_cam3d_evt_set, 0, 331, 1310, 0, 145, -145, 3000, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, -280, 0, FLOAT(120.0f))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP2")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -250, 20, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP4")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -215, 40, 0, 400, 1, -1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FLD_JUMP1")), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, -185, 60, 0, 400, 1, -1000)
        CALL(evt_mario_mov_pos2, -55, 0, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(evt_party_move_pos2, 0, -280, 25, FLOAT(120.0f))
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -250, 20, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -215, 40, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_jump_pos, 0, -185, 60, 25, 400, 1, -1000)
        WAIT_FRAMES(1)
        CALL(evt_party_move_pos2, 0, -90, -25, FLOAT(120.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_tou_get_fighter_num, GSW_GLITZ_PIT_OPPONENT, LW(0))
        SET_VALUE(LW(3), LW(0))
        SET_VALUE(LW(4), 0)
        LOOP(0)
            IF_INT_EQUAL(LW(3), LW(4))
                BREAK_LOOP
            END_IF
            SWITCH(LW(4))
                CASE_EQUAL(0)
                SET_VALUE(LW(0), STRING(("\223G\202P")))
                CASE_EQUAL(1)
                SET_VALUE(LW(0), STRING(("\223G\202Q")))
                CASE_EQUAL(2)
                SET_VALUE(LW(0), STRING(("\223G\202R")))
                CASE_EQUAL(3)
                SET_VALUE(LW(0), STRING(("\223G\202S")))
                CASE_EQUAL(4)
                SET_VALUE(LW(0), STRING(("\223G\202T")))
            END_SWITCH
            SET_VALUE(LW(1), LW(4))
            SET_VALUE(LW(2), LW(3))
            SPAWN_EVENT(PTR(evt_teki_nyuujyou_default))
            ADD(LW(4), 1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(3000)
    LOOP(0)
        IS_EVENT_RUNNING(LW(10), LW(11))
        IF_INT_EQUAL(LW(11), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_party_set_pos, 0, -95, 60, -25)
    CALL(evt_cam3d_evt_set, 0, 127, 105, 0, 95, -145, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    CALL(evt_tou_get_fighter_name2, GSW_GLITZ_PIT_OPPONENT, LW(2))
    IF_INT_EQUAL(GSW_GLITZ_PIT_OPPONENT, 0)
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
            SET_VALUE(LW(0), STRING(("\203K\203\223\203X")))
            ELSE
            SET_VALUE(LW(0), STRING(("\203L\203m\203V\203R\203\217")))
        END_IF
        CALL(evt_tou_get_ranking, LW(1))
        SWITCH(LW(1))
            CASE_EQUAL(0)
            IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
                SET_VALUE(LW(1), STRING(("stg3_tou_378")))
                ELSE
                SET_VALUE(LW(1), STRING(("stg3_tou_re_266")))
            END_IF
            CASE_EQUAL(1)
            IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
                SET_VALUE(LW(1), STRING(("stg3_tou_377")))
                ELSE
                SET_VALUE(LW(1), STRING(("stg3_tou_re_265")))
            END_IF
        END_SWITCH
        CALL(evt_msg_print, 0, LW(1), 0, LW(0))
        ELSE
        IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
            SET_VALUE(LW(3), STRING(("\203K\203\223\203X")))
            SET_VALUE(LW(4), STRING(("stg3_tou_115")))
            SET_VALUE(LW(5), STRING(("stg3_tou_116_20")))
            ELSE
            SET_VALUE(LW(3), STRING(("\203L\203m\203V\203R\203\217")))
            SET_VALUE(LW(4), STRING(("stg3_tou_re_147")))
            SET_VALUE(LW(5), STRING(("stg3_tou_re_149")))
        END_IF
        CALL(evt_msg_print, 0, LW(4), 0, LW(3))
        CALL(evt_msg_print_add_insert, 0, LW(5), LW(2), LW(0))
    END_IF
    CALL(evt_cam3d_evt_set, -65, 143, 263, -65, 95, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_party_stop, 0)
            WAIT_MS(300)
            CALL(evt_sub_random, 1000, LW(0))
            IF_INT_LESS(LW(0), 500)
                CALL(evt_msg_print_party, STRING(("stg3_tou_117_00")))
                ELSE
                CALL(evt_msg_print_party, STRING(("stg3_tou_117_01")))
            END_IF
            WAIT_MS(300)
            ELSE
            CALL(evt_msg_print, 0, STRING(("stg3_tou_305")), 0, STRING(("\203K\203\223\203X")))
        END_IF
        ELSE
        CALL(evt_party_stop, 0)
        WAIT_MS(300)
        CALL(evt_msg_print_party, STRING(("stg3_tou_re_151")))
        WAIT_MS(300)
    END_IF
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    SWITCH(LW(1))
        CASE_EQUAL(3)
        CALL(evt_cam3d_evt_set, 65, 183, 263, 65, 135, -8, 0, EASING_LINEAR)
        CASE_DEFAULT
        CALL(evt_cam3d_evt_set, 60, 143, 263, 60, 95, -8, 0, EASING_LINEAR)
    END_SWITCH
    WAIT_FRAMES(1)
    CALL(evt_tou_get_ranking, LW(2))
    CALL(evt_tou_get_fighter_battle_msg, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    SWITCH(GSW_GLITZ_PIT_OPPONENT)
        CASE_EQUAL(10)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG3_20_MAJORS_10TH_PLACE, STG3_34_RETURN_TO_ROGUEPORT)
            SET_VALUE(LW(0), STRING(("stg3_tou_118_37")))
            CASE_GREATER_OR_EQUAL(STG3_34_RETURN_TO_ROGUEPORT)
            IF_INT_LESS_OR_EQUAL(LW(2), 10)
                SET_VALUE(LW(0), STRING(("stg3_tou_118_37")))
            END_IF
        END_SWITCH
        CASE_EQUAL(0)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_LESS(STG3_34_RETURN_TO_ROGUEPORT)
            IF_INT_EQUAL(LW(2), 1)
                SET_VALUE(LW(0), STRING(("stg3_tou_400_00")))
            END_IF
            CASE_DEFAULT
            IF_INT_EQUAL(LW(2), 1)
                SET_VALUE(LW(0), STRING(("stg3_tou_re_287")))
            END_IF
        END_SWITCH
    END_SWITCH
    WAIT_MS(300)
    CALL(evt_msg_print, 0, LW(0), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 150, 141, 0, 100, -154, 0, EASING_SINE_IN_OUT)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(0), STRING(("\203K\203\223\203X")))
        SET_VALUE(LW(1), STRING(("stg3_tou_116_20_01")))
        CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_1")))
        WAIT_MS(200)
        CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("M_2")))
        CALL(evt_npc_set_autotalkpose, STRING(("\203K\203\223\203X")), STRING(("M_2")), STRING(("M_3")))
        ELSE
        SET_VALUE(LW(0), STRING(("\203L\203m\203V\203R\203\217")))
        SET_VALUE(LW(1), STRING(("stg3_tou_re_150")))
        CALL(evt_npc_set_anim, STRING(("\203L\203m\203V\203R\203\217")), STRING(("A_2")))
        WAIT_MS(200)
        CALL(evt_npc_set_autotalkpose, STRING(("\203L\203m\203V\203R\203\217")), STRING(("S_3")), STRING(("T_3")))
    END_IF
    CALL(evt_msg_print, 0, LW(1), 0, LW(0))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_GONG1")), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_set, 0, 250, 730, 0, 100, -154, 2000, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_default_win)
    CALL(evt_cam3d_evt_set, 0, 220, 533, 0, 125, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(tou_champion_belt_onoff, 1)
    CALL(evt_tou_get_fighter_num, GSW_GLITZ_PIT_OPPONENT, LW(0))
    SET_VALUE(LW(1), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(0), LW(1))
            BREAK_LOOP
        END_IF
        SWITCH(LW(1))
            CASE_EQUAL(0)
#ifdef VERSION_G8ME01
            SET_VALUE(LW(2), STRING("敵１"))
            CASE_EQUAL(1)
            SET_VALUE(LW(2), STRING("敵２"))
            CASE_EQUAL(2)
            SET_VALUE(LW(2), STRING("敵３"))
            CASE_EQUAL(3)
            SET_VALUE(LW(2), STRING("敵４"))
            CASE_EQUAL(4)
            SET_VALUE(LW(2), STRING("敵５"))
#else
            SET_VALUE(LW(2), STRING(("\223G\202P")))
            CASE_EQUAL(1)
            SET_VALUE(LW(2), STRING(("\223G\202Q")))
            CASE_EQUAL(2)
            SET_VALUE(LW(2), STRING(("\223G\202R")))
            CASE_EQUAL(3)
            SET_VALUE(LW(2), STRING(("\223G\202S")))
            CASE_EQUAL(4)
            SET_VALUE(LW(2), STRING(("\223G\202T")))
#endif
        END_SWITCH
        CALL(confupose, LW(2), LW(3))
        CALL(evt_npc_set_anim, LW(2), LW(3))
        ADD(LW(1), 1)
    END_LOOP
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
#ifdef VERSION_G8ME01
    CALL(evt_eff, 0, STRING("confetti"), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_PAUSE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_V_2"))
#else
    CALL(evt_eff, 0, STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_PAUSE2")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_V_2")))
#endif
    CALL_CHILD(PTR(evt_kime_pose))
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
#ifdef VERSION_G8ME01
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_WIN_AUDIENCE2"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_msg_print, 0, STRING("stg3_tou_118_39"), 0, STRING("ガンス"))
        ELSE
        CALL(evt_msg_print, 0, STRING("stg3_tou_re_118_39"), 0, STRING("キノシコワ"))
    END_IF
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#else
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_WIN_AUDIENCE2")), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(0), STRING(("\203K\203\223\203X")))
        ELSE
        SET_VALUE(LW(0), STRING(("\203L\203m\203V\203R\203\217")))
    END_IF
    CALL(evt_msg_print, 0, STRING(("stg3_tou_118_39")), 0, LW(0))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
#endif
    CALL(evt_cam3d_evt_set, -65, 143, 263, -65, 95, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_party_stop, 0)
            WAIT_MS(300)
            CALL(evt_sub_random, 1000, LW(0))
            IF_INT_LESS(LW(0), 500)
#ifdef VERSION_G8ME01
                CALL(evt_msg_print_party, STRING("stg3_tou_117_02"))
                ELSE
                CALL(evt_msg_print_party, STRING("stg3_tou_117_03"))
#else
                CALL(evt_msg_print_party, STRING(("stg3_tou_117_02")))
                ELSE
                CALL(evt_msg_print_party, STRING(("stg3_tou_117_03")))
#endif
            END_IF
            WAIT_MS(300)
        END_IF
        ELSE
        CALL(evt_party_stop, 0)
        WAIT_MS(300)
#ifdef VERSION_G8ME01
        CALL(evt_msg_print_party, STRING("stg3_tou_re_153"))
#else
        CALL(evt_msg_print_party, STRING(("stg3_tou_re_153")))
#endif
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_default_lose)
    CALL(evt_cam3d_evt_set, 0, 220, 533, 0, 125, -8, 0, EASING_LINEAR)
    CALL_CHILD(PTR(evt_damage_pose))
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_npc_get_position, STRING("敵１"), LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(""), STRING("confetti"), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_WIN_AUDIENCE1"), 0)
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_118_61"), 0, STRING("ガンス"), LW(0))
        ELSE
        CALL(evt_msg_print_insert, 0, STRING("stg3_tou_re_118_61"), 0, STRING("キノシコワ"), LW(0))
    END_IF
#else
    CALL(evt_npc_get_position, STRING(("\223G\202P")), LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, STRING(("")), STRING(("confetti")), 3, LW(0), LW(1), LW(2), 300, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_WIN_AUDIENCE1")), 0)
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        SET_VALUE(LW(1), STRING(("\203K\203\223\203X")))
        ELSE
        SET_VALUE(LW(1), STRING(("\203L\203m\203V\203R\203\217")))
    END_IF
    CALL(evt_msg_print_insert, 0, STRING(("stg3_tou_118_61")), 0, LW(1), LW(0))
#endif
    CALL(evt_tou_get_fighter_info, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    SWITCH(LW(1))
        CASE_EQUAL(3)
        CALL(evt_cam3d_evt_set, 65, 183, 263, 65, 135, -8, 0, EASING_LINEAR)
        CASE_DEFAULT
        CALL(evt_cam3d_evt_set, 60, 143, 263, 60, 95, -8, 0, EASING_LINEAR)
    END_SWITCH
    WAIT_FRAMES(1)
    CALL(evt_tou_get_fighter_battle_msg, GSW_GLITZ_PIT_OPPONENT, LW(0), LW(1))
    SWITCH(GSW_GLITZ_PIT_OPPONENT)
        CASE_EQUAL(10)
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG3_20_MAJORS_10TH_PLACE, STG3_34_RETURN_TO_ROGUEPORT)
#ifdef VERSION_G8ME01
            SET_VALUE(LW(0), STRING("stg3_tou_118_38"))
            CASE_GREATER_OR_EQUAL(STG3_34_RETURN_TO_ROGUEPORT)
            IF_INT_LESS_OR_EQUAL(LW(2), 10)
                SET_VALUE(LW(0), STRING("stg3_tou_118_38"))
#else
            SET_VALUE(LW(0), STRING(("stg3_tou_118_38")))
            CASE_GREATER_OR_EQUAL(STG3_34_RETURN_TO_ROGUEPORT)
            IF_INT_LESS_OR_EQUAL(LW(2), 10)
                SET_VALUE(LW(0), STRING(("stg3_tou_118_38")))
#endif
            END_IF
        END_SWITCH
    END_SWITCH
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, LW(1), 0, STRING("敵１"))
#else
    CALL(evt_msg_print, 0, LW(1), 0, STRING(("\223G\202P")))
#endif
    WAIT_MS(300)
    RETURN
EVT_END()

static EVT_BEGIN(evt_champion)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
#ifdef VERSION_G8ME01
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_2_a"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_2_b"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_status_onoff, 1, STRING("ガンス"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING("ガンス"), 0, 60, 0)
    CALL(evt_npc_set_ry, STRING("ガンス"), 270)
    CALL(evt_npc_flag_onoff, 1, STRING("ガンス"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 0, 281, 1310, 0, 95, -145, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_tribe, STRING("敵１"), STRING("ブレッドハートベルト"))
    BEGIN_INLINE_EVENT
        CALL(tou_champion_belt, STRING("敵１"))
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_set_pos, -600, 0, 0)
    CALL(evt_party_set_pos, 0, -610, 0, -10)
    WAIT_MS(2000)
    CALL(evt_npc_flag_onoff, 0, STRING("ガンス"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 0, 155, 327, 0, 96, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_411"), 0, STRING("ガンス"))
    CALL(evt_npc_set_ry, STRING("ガンス"), 90)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg3_tou_412"), 0, STRING("ガンス"))
    CALL(evt_npc_set_position, STRING("敵１"), 550, 0, 0)
#else
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG3_TOU2")))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_2_a")), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_2_b")), 1)
    CALL(evt_npc_status_onoff, 1, STRING(("\203K\203\223\203X")), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 0, 60, 0)
    CALL(evt_npc_set_ry, STRING(("\203K\203\223\203X")), 270)
    CALL(evt_npc_flag_onoff, 1, STRING(("\203K\203\223\203X")), 32768)
    CALL(evt_cam3d_evt_set, 0, 281, 1310, 0, 95, -145, 0, EASING_SINE_IN_OUT)
    CALL(evt_npc_set_tribe, STRING(("\223G\202P")), STRING(("\203u\203\214\203b\203h\203n\201[\203g\203x\203\213\203g")))
    BEGIN_INLINE_EVENT
        CALL(tou_champion_belt, STRING(("\223G\202P")))
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(2000)
    CALL(evt_npc_flag_onoff, 0, STRING(("\203K\203\223\203X")), 32768)
    CALL(evt_cam3d_evt_set, 0, 155, 327, 0, 96, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_411")), 0, STRING(("\203K\203\223\203X")))
    CALL(evt_npc_set_ry, STRING(("\203K\203\223\203X")), 90)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_412")), 0, STRING(("\203K\203\223\203X")))
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 550, 0, 0)
#endif
    SET_FLOAT_VALUE(LW(0), FLOAT(-180.0f))
    CALL_CHILD(PTR(e_door_open))
    CALL(evt_cam3d_evt_set, 475, 134, 533, 475, 38, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
#ifdef VERSION_G8ME01
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_IRON2"), 565, 0, 0, 0)
#else
        CALL(evt_snd_sfxon_3d, STRING(("SFX_DOOR_SHUT_IRON2")), 565, 0, 0, 0)
#endif
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -180, 0, 20)
        LOOP(21)
            CALL(evt_sub_intpl_get_value)
            CALL_CHILD(PTR(e_door_open))
        END_LOOP
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        CALL(evt_npc_move_position, STRING("敵１"), 450, 0, 0, FLOAT(60.0f), 1)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING("敵１"), 310, 0, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING("敵１"), 280, 20, 0, 500, 30)
        CALL(evt_npc_jump_position_nohit, STRING("敵１"), 245, 40, 0, 500, 30)
        CALL(evt_npc_jump_position_nohit, STRING("敵１"), 215, 60, 0, 500, 30)
        CALL(evt_npc_move_position, STRING("敵１"), 80, 0, 0, FLOAT(60.0f), 1)
#else
        CALL(evt_npc_move_position, STRING(("\223G\202P")), 450, 0, 0, FLOAT(60.0f), 1)
        WAIT_MS(1000)
        CALL(evt_npc_move_position, STRING(("\223G\202P")), 310, 0, 0, FLOAT(60.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING(("\223G\202P")), 280, 20, 0, 500, 30)
        CALL(evt_npc_jump_position_nohit, STRING(("\223G\202P")), 245, 40, 0, 500, 30)
        CALL(evt_npc_jump_position_nohit, STRING(("\223G\202P")), 215, 60, 0, 500, 30)
        CALL(evt_npc_move_position, STRING(("\223G\202P")), 80, 0, 0, FLOAT(60.0f), 1)
#endif
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 426, 30, -195)
#ifdef VERSION_G8ME01
        CALL(evt_msg_print, 0, STRING("stg3_tou_413_01"), 0, 0)
#else
        CALL(evt_msg_print, 0, STRING(("stg3_tou_413_01")), 0, 0)
#endif
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 379, 30, 133)
#ifdef VERSION_G8ME01
        CALL(evt_msg_print, 0, STRING("stg3_tou_413_02"), 0, 0)
#else
        CALL(evt_msg_print, 0, STRING(("stg3_tou_413_02")), 0, 0)
#endif
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 293, 30, -180)
#ifdef VERSION_G8ME01
        CALL(evt_msg_print, 0, STRING("stg3_tou_413_03"), 0, 0)
#else
        CALL(evt_msg_print, 0, STRING(("stg3_tou_413_03")), 0, 0)
#endif
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(5000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 200, 30, 133)
#ifdef VERSION_G8ME01
        CALL(evt_msg_print, 0, STRING("stg3_tou_413_04"), 0, 0)
#else
        CALL(evt_msg_print, 0, STRING(("stg3_tou_413_04")), 0, 0)
#endif
    END_INLINE_EVENT
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_set, 50, 155, 273, 50, 106, -8, 6000, EASING_LINEAR)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    CALL(evt_npc_move_position, STRING("ガンス"), 20, -20, 0, FLOAT(70.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_414"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_415"), 0, STRING("敵１"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_416"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_417"), 0, STRING("敵１"))
#else
    CALL(evt_npc_move_position, STRING(("\203K\203\223\203X")), 20, -20, 0, FLOAT(70.0f), 1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_414")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_415")), 0, STRING(("\223G\202P")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_416")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_417")), 0, STRING(("\223G\202P")))
#endif
    WAIT_MS(300)
    WAIT_MS(1000)
    SET_VALUE(GSWF(2455), 1)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
#ifdef VERSION_G8ME01
    CALL(evt_bero_mapchange, STRING("tou_11"), STRING("nidome"))
#else
    CALL(evt_bero_mapchange, STRING(("tou_11")), STRING(("nidome")))
#endif
    RETURN
EVT_END()

EVT_BEGIN(evt_champion2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
#ifdef VERSION_G8ME01
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam3d_evt_set, 50, 155, 273, 50, 106, -8, 0, EASING_LINEAR)
    CALL(evt_npc_set_position, STRING("敵１"), 80, 60, 0)
    CALL(evt_npc_set_ry, STRING("敵１"), 270)
    CALL(evt_npc_set_position, STRING("ガンス"), 20, 60, -20)
    CALL(evt_npc_set_ry, STRING("ガンス"), 90)
    CALL(evt_npc_set_tribe, STRING("敵１"), STRING("ブレッドハートベルト"))
    BEGIN_INLINE_EVENT
        CALL(tou_champion_belt, STRING("敵１"))
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_set_pos, -600, 0, 0)
    CALL(evt_party_set_pos, 0, -610, 0, -10)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING("ガンス"), 270)
    CALL(evt_msg_print, 0, STRING("stg3_tou_418"), 0, STRING("ガンス"))
#else
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG3_TOU2")))
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam3d_evt_set, 50, 155, 273, 50, 106, -8, 0, EASING_LINEAR)
    CALL(evt_npc_set_position, STRING(("\223G\202P")), 80, 60, 0)
    CALL(evt_npc_set_ry, STRING(("\223G\202P")), 270)
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 20, 60, -20)
    CALL(evt_npc_set_ry, STRING(("\203K\203\223\203X")), 90)
    CALL(evt_npc_set_tribe, STRING(("\223G\202P")), STRING(("\203u\203\214\203b\203h\203n\201[\203g\203x\203\213\203g")))
    BEGIN_INLINE_EVENT
        CALL(tou_champion_belt, STRING(("\223G\202P")))
    END_INLINE_EVENT
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(1000)
    CALL(evt_npc_set_ry, STRING(("\203K\203\223\203X")), 270)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_418")), 0, STRING(("\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -475, 134, 533, -475, 38, -8, 0, EASING_LINEAR)
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_set, 20, 155, 273, 20, 106, -8, 0, EASING_LINEAR)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_419"), 0, STRING("ガンス"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_419")), 0, STRING(("\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 80, 155, 273, 80, 106, -8, 1000, EASING_LINEAR)
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_420"), 0, STRING("敵１"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_420")), 0, STRING(("\223G\202P")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 20, 155, 273, 20, 106, -8, 1000, EASING_LINEAR)
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("敵１"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_421"), 0, STRING("ガンス"))
#else
    CALL(evt_set_dir_to_target, STRING(("\203K\203\223\203X")), STRING(("\223G\202P")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_421")), 0, STRING(("\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 850, 2160, 0, 287, -8, 10000, EASING_LINEAR)
    WAIT_MS(7000)
    CALL(evt_fade_set_mapchange_type, 0, 2, 300, 1, 300)
#ifdef VERSION_G8ME01
    CALL(evt_bero_mapchange, STRING("tou_11"), STRING("sandome"))
#else
    CALL(evt_bero_mapchange, STRING(("tou_11")), STRING(("sandome")))
#endif
    RETURN
EVT_END()

void kyaku_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

void ext_kyaku_main(void) {
    pose_main();
}

void ext_kyaku_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, kyaku_disp, NULL);
}

static void screen_capture(CameraId cameraId, void* param) {
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x130, 0xF0, GX_TF_RGB565, GX_TRUE);
    GXCopyTex(screen_wp->buf, GX_FALSE);
    GXPixModeSync();
    screen_wp->ready = 1;
}

s32 screen_init(EventEntry* event, BOOL isFirstCall) {
    u32 size = GXGetTexBufferSize(0x260, 0x1E0, GX_TF_I4, GX_FALSE, 0);

    screen_wp = _mapAlloc(sizeof(ScreenWork));
    screen_wp->buf = _mapAlloc(size);
    screen_wp->ready = 0;
    screen_wp->scale = screen_wp->scaleTarget = 1.0f;
    screen_wp->x = screen_wp->xTarget = 0.0f;
    screen_wp->y = screen_wp->yTarget = 0.0f;
    screen_wp->count = 0;
    screen_wp->field_0x28 = -1.0f;
#ifdef VERSION_G8ME01
    size = GXGetTexBufferSize(64, 64, GX_TF_I8, GX_FALSE, 0);
    screen_wp->noise = _mapAlloc(size);
#else
    screen_wp->noise = _mapAlloc(GXGetTexBufferSize(64, 64, GX_TF_I8, GX_FALSE, 0));
#endif
    if (evtGetValue(event, GSW_STORY_PROGRESS) == 163) {
        screen_wp->mode = 1;
    }
    return EVT_RETURN_DONE;
}

static s32 screen_evt(EventEntry* event, BOOL isFirstCall) {
    dispEntry(CAMERA_2D, 1000.0f, 0, screen_capture, NULL);
    return EVT_RETURN_BLOCK;
}

#pragma dont_inline on
void GXSetTexCoordGen(GXTexCoordID dst, GXTexGenType func, GXTexGenSrc src, u32 mtx) {
    GXSetTexCoordGen2(dst, func, src, mtx, GX_FALSE, GX_PTIDENTITY);
}
#pragma dont_inline reset

void screen_tev(MapTevCallbackInfo* info) {
    GXTexObj tex;
    Mtx mtx;
    Mtx scale;
    Mtx trans;
    s32 by;
    s32 ry;
    u8* buf;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 len;
    s32 off;
    s32 w;
    s32 start;
    u8 col;
    f32 lim;
    f32 x;
    f32 y;
    CameraEntry* cam;

    if (screen_wp->ready) {
        if (screen_wp->mode) {
            buf = screen_wp->noise;
            for (i = 0; i < 64; i++) {
                for (j = 0; j < 64; j++) {
                    by = i / 4;
                    ry = i % 4;
                    off = j / 8;
                    w = j % 8;
                    buf[((off << 5) + (by << 8)) + (w + ry * 8)] = (i & 1) ? 0xFF : 0;
                }
                n = rand() % 100 + 100;
                for (k = 0; k < n; k++) {
                    len = rand() % 4 + 2;
                    start = rand() % (65 - len);
                    col = rand() % 256;
                    for (j = start; j < start + len; j++) {
                        by = i / 4;
                        ry = i % 4;
                        off = j / 8;
                        w = j % 8;
                        buf[((off << 5) + (by << 8)) + (w + ry * 8)] = col;
                    }
                }
            }
            DCFlushRange(buf, 0x1000);
            GXInitTexObj(&tex, buf, 64, 64, GX_TF_I8, GX_REPEAT, GX_REPEAT, GX_FALSE);
            GXInitTexObjLOD(&tex, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
            GXLoadTexObj(&tex, GX_TEXMAP0);
        } else {
            GXInitTexObj(&tex, screen_wp->buf, 0x130, 0xF0, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
            GXInitTexObjLOD(&tex, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        }
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXInitTexObj(&tex, inta_tex, 4, 8, GX_TF_I4, GX_REPEAT, GX_REPEAT, GX_FALSE);
        GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&tex, GX_TEXMAP1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        if (screen_wp->mode == 0) {
            screen_wp->count++;
            if (screen_wp->count % 200 == 0) {
                switch (rand() % 3) {
                    case 0:
                        screen_wp->xTarget = randf(0.0f, (1.0f - screen_wp->scale) / screen_wp->scale);
                        screen_wp->yTarget = randf(0.0f, (1.0f - screen_wp->scale) / screen_wp->scale);
                        break;
                    case 1:
                        screen_wp->scaleTarget = randf(1.0f, 0.25f);
                        break;
                    case 2:
                        break;
                }
            }
            screen_wp->scale += (screen_wp->scaleTarget - screen_wp->scale) / 60.0f;
            screen_wp->x += (screen_wp->xTarget - screen_wp->x) / 60.0f;
            screen_wp->y += (screen_wp->yTarget - screen_wp->y) / 60.0f;
            PSMTXScale(scale, screen_wp->scale, screen_wp->scale, 1.0f);
            lim = (1.0f - screen_wp->scale) / screen_wp->scale;
            y = screen_wp->y > lim ? lim : screen_wp->y;
            if (screen_wp->x > lim) {
                x = lim;
            } else {
                x = screen_wp->x;
            }
            PSMTXTrans(trans, x, y, 0.0f);
            PSMTXConcat(scale, trans, mtx);
        } else {
            PSMTXScale(mtx, 4.75f, 3.75f, 0.0f);
        }
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
        cam = camGetPtr(CAMERA_3D);
        x = 100.0f - cam->cameraPos.z;
        PSMTXScale(mtx, 1.0f, 6.0f * (x / 1000.0f) + 16.0f, 1.0f);
        GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX1);
        GXLoadTexMtxImm(mtx, GX_TEXMTX1, GX_MTX2x4);
        info->tevStage = 2;
        info->texCoord = 2;
        info->texMap = 2;
    }
}

s32 tribe_chk(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    const char* tribe = (const char*)evtGetValue(event, args[1]);

    if (strcmp(npcNameToPtr(name)->tribe->nameJp, tribe) == 0) {
        evtSetValue(event, args[2], 1);
    } else {
        evtSetValue(event, args[2], 0);
    }
    return EVT_RETURN_DONE;
}

s32 burst_chain(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 id = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr(name);
    NpcEntry* a;
    NpcEntry* b;
    char buf[32];
    s32 i;
    f32 t;
    f32 dist;

    sprintf(buf, "\215\275%d_%d", id, 0);
    npcNameToPtr(buf)->position = npc->position;
    for (i = 0; i < 4; i++) {
        sprintf(buf, "\215\275%d_%d", id, i);
        a = npcNameToPtr(buf);
        sprintf(buf, "\215\275%d_%d", id, i + 1);
        b = npcNameToPtr(buf);
        b->position.y -= 3.0f;
        dist = PSVECDistance(&a->position, &b->position);
        t = 1.0f - 10.0f / dist;
        b->position.x += t * (a->position.x - b->position.x);
        b->position.y += t * (a->position.y - b->position.y);
        b->position.z += t * (a->position.z - b->position.z);
        if (b->position.x > 210.0f || b->position.x < -210.0f) {
            if (b->position.y < 0.0f) {
                b->position.y = 0.0f;
            }
        } else {
            if (b->position.y < 60.0f) {
                b->position.y = 60.0f;
            }
        }
    }
    return EVT_RETURN_BLOCK;
}

s32 snd_sfx_stop(EventEntry* event, BOOL isFirstCall) {
    psndMapChange();
    return EVT_RETURN_DONE;
}

s32 confupose(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, args[0]));

    evtSetValue(event, args[1], (EvtWord)npc->tribe->confuseAnimation);
    return EVT_RETURN_DONE;
}

#pragma dont_inline on
static f32 sinf(f32 x) {
    return sin(x);
}
#pragma dont_inline reset

#pragma dont_inline on
static f32 cosf(f32 x) {
    return cos(x);
}
#pragma dont_inline reset

static const Vec item_jump_zero = {0.0f, 0.0f, 0.0f};
static inline s32 item_jump_sub(EventEntry* event, BOOL isFirstCall, f32 speed, f32 g) {
    EvtWord* args = event->args;
    f32 z;
    f32 y;
    f32 x;
    ItemEntry* item;
    f32 angle;
    f32 dy;
    f32 t;
    f32 vy;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    item = itemNameToPtr("stone");
    if (isFirstCall) {
        dy = y - item->position.y;
        t = distABf(x, z, item->position.x, item->position.z) / speed;
        vy = dy / t + 0.5f * t * g;
        angle = angleABf(item->position.x, item->position.z, x, z);
        event->userdata[0] = *(s32*)&t;
        event->userdata[1] = *(s32*)&vy;
        event->userdata[2] = *(s32*)&angle;
    }
    t = event->userdataf[0];
    vy = event->userdataf[1];
    angle = event->userdataf[2];
    item->position.x += speed * sinf(6.2831855f * angle / 360.0f);
    item->position.z -= speed * cosf(6.2831855f * angle / 360.0f);
    item->position.y += vy;
    vy -= g;
    t -= speed;
    if (t < 0.0f) {
        item->position.x = x;
        item->position.y = y;
        item->position.z = z;
        {
            Vec tmp = {0.0f, 0.0f, 0.0f};
            Vec pos;

            tmp.x = item->position.x;
            tmp.y = item->position.y;
            tmp.z = item->position.z;
            pos = tmp;
            psndSFXOn_3D("SFX_EVT_STARSTONE_JUMP1", &pos);
        }
        return 1;
    }
    event->userdata[0] = *(s32*)&t;
    event->userdata[1] = *(s32*)&vy;
    return EVT_RETURN_BLOCK;
}

s32 item_jump(EventEntry* event, BOOL isFirstCall) {
    return item_jump_sub(event, isFirstCall, 1.0f, 0.5f);
}

const char* block_data[] = {
    "block_1", "block_2", "block_3", "block_4", "block_5", "block_6", "block_7", "block_8",
    "block_9",
};

static EVT_BEGIN(evt_shake)
    LOOP(0)
        CALL(evt_npc_get_position, STRING(("\203K\203\223\203X")), LW(0), LW(1), LW(2))
        ADD(LW(0), 2)
        CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), LW(0), LW(1), LW(2))
        WAIT_FRAMES(2)
        CALL(evt_npc_get_position, STRING(("\203K\203\223\203X")), LW(0), LW(1), LW(2))
        ADD(LW(0), -2)
        CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), LW(0), LW(1), LW(2))
        WAIT_FRAMES(2)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_boss_stone)
    SET_VALUE(LW(10), 0)
    CALL(evt_item_entry, STRING(("stone")), ITEM_GOLD_STAR, 0, 0, 0, ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    LOOP(0)
        CALL(evt_mapobj_get_position, STRING(("kaiten_souti")), LW(0), LW(1), LW(2))
        ADD_FLOAT(LW(1), FLOAT(20.0f))
        CALL(evt_sub_get_sincos, LW(10), LW(11), LW(12))
        MULTIPLY_FLOAT(LW(12), FLOAT(10.0f))
        ADD(LW(1), LW(12))
        ADD(LW(10), FLOAT(2.5f))
        MODULO(LW(10), 360)
        IF_INT_LESS(LW(1), 50)
            CALL(starstone_sound_onoff, 0)
            ELSE
            CALL(starstone_sound_onoff, 1)
        END_IF
        CALL(evt_item_set_position, STRING(("stone")), LW(0), LW(1), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

s32 getY(EventEntry* event, BOOL isFirstCall) {
    NpcEntry* npc = npcNameToPtr("\203K\203\223\203X");
    MapObject* obj = mapGetMapObj("block_5");
    Vec pos;
    f32 y;

    mapObjGetPos("group151", &pos);
    y = 100.0f + obj->modelWorldMtx[1][3];
    if (y > pos.y) {
        npc->position.y = y;
    } else {
        npc->position.x = npc->position.z = 0.0f;
        npc->position.y = pos.y;
    }
    return EVT_RETURN_DONE;
}

s32 starstone_sound_onoff(EventEntry* event, BOOL isFirstCall) {
    s32 flag = evtGetValue(event, event->args[0]);
    EffectEntry* eff = itemNameToPtr("stone")->effect;

    if (eff) {
        if (flag == 0) {
            ((EffStarStoneWork*)eff->userdata)->flags &= ~4;
        }
        if (flag == 1) {
            ((EffStarStoneWork*)eff->userdata)->flags |= 4;
        }
    }
    return EVT_RETURN_DONE;
}

s32 evt_boss_stone_r(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    EffectEntry* eff = itemNameToPtr("stone")->effect;

    ((EffStarStoneWork*)eff->userdata)->rot.y = evtGetFloat(event, args[0]);
    return EVT_RETURN_DONE;
}

s32 eff_pos(EventEntry* event, BOOL isFirstCall) {
    ItemEntry* item = itemNameToPtr("stone");
    EffectEntry* eff1 = effNameToPtr("eff_1");
    EffectEntry* eff2 = effNameToPtr("eff_2");

    if (item) {
        if (eff1) {
            ((EffPokopiPchargeN64Elem*)eff1->userdata)->position.x = item->position.x;
            ((EffPokopiPchargeN64Elem*)eff1->userdata)->position.y = 15.0f + item->position.y;
            ((EffPokopiPchargeN64Elem*)eff1->userdata)->position.z = item->position.z;
        }
        if (eff2) {
            ((EffPokopiPchargeN64Elem*)eff2->userdata)->position.x = item->position.x;
            ((EffPokopiPchargeN64Elem*)eff2->userdata)->position.y = 15.0f + item->position.y;
            ((EffPokopiPchargeN64Elem*)eff2->userdata)->position.z = item->position.z;
        }
    }
    return EVT_RETURN_BLOCK;
}

static inline void cb_set_color(u8 a) {
    GXColor color = {255, 255, 255, 0};

    color.a = a;
    GXSetTevColor(GX_TEVREG0, color);
}

static void callback(MapTevCallbackInfo* info) {
    s32 stage = info->tevStage;
    s32 texMap = info->texMap;
    s32 texCoord = info->texCoord;
    s32 indStage = info->indStage;

    cb_set_color(gans_alpha);
    GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(stage, GX_CC_CPREV, GX_CC_C0, GX_CC_A0, GX_CC_ZERO);
    GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
    GXSetTevSwapMode(stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
    info->tevStage = stage + 1;
    info->texCoord = texCoord;
    info->indStage = indStage;
    info->texMap = texMap;
}

s32 gans_white_on(EventEntry* event, BOOL isFirstCall) {
    NpcEntry* npc = npcNameToPtr("\203K\203\223\203X");

    if (isFirstCall) {
        gans_alpha = 0;
        mapSetTevCallback(12, callback);
        animPoseSetMaterialFlagOn(npc->poseId, 0xC000000);
    }
    gans_alpha += 5;
    if (gans_alpha > 255) {
        gans_alpha = 255;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

s32 gans_white_off(EventEntry* event, BOOL isFirstCall) {
    NpcEntry* npc = npcNameToPtr("\203K\203\223\203X");

    mapSetTevCallback(12, NULL);
    animPoseSetMaterialFlagOff(npc->poseId, 0xC000000);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(evt_boss)
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("group34"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_kai_right"), 0, 0)
    SET_VALUE(LW(0), STRING("e_bero"))
    CALL_CHILD(bero_case_switch_off)
    CALL(evt_npc_set_position, STRING("ガンス"), 0, 60, 0)
    CALL(evt_set_dir_to_target, STRING("ガンス"), STRING("mario"))
    CALL(evt_npc_set_tribe, STRING("ガンス"), STRING("ガンス"))
#else
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("group34")), 1)
    CALL(evt_hitobj_onoff, STRING(("a_kai_right")), 0, 0)
    SET_VALUE(LW(0), STRING(("e_bero")))
    CALL_CHILD(PTR(bero_case_switch_off))
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 0, 60, 0)
    CALL(evt_set_dir_to_target, STRING(("\203K\203\223\203X")), STRING(("mario")))
    CALL(evt_npc_set_tribe, STRING(("\203K\203\223\203X")), STRING(("\203K\203\223\203X")))
#endif
    SPAWN_EVENT_GET_ID(PTR(evt_boss_stone), GW(10))
    WAIT_FRAMES(2)
    SET_VALUE(GW(12), 1000)
    BEGIN_INLINE_EVENT_GET_ID(GW(11))
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 360, GW(12))
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_boss_stone_r, LW(0))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    LOOP(0)
        CALL(evt_mario_get_name_hitobj_ride, LW(9))
#ifdef VERSION_G8ME01
        IF_STR_EQUAL(LW(9), STRING("a_butai"))
#else
        IF_STR_EQUAL(LW(9), STRING(("a_butai")))
#endif
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_cam3d_evt_set, 0, 197, 484, 0, 110, -8, 2000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos2, -75, 0, FLOAT(60.0f))
    CALL(evt_mario_set_dir, 90, 200, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_party_set_dir, 0, 90, 200)
#ifdef VERSION_G8ME01
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG3_GANCE2"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_463"), 0, STRING("ガンス"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG3_GROUND_SHAKE1"), LW(10))
#else
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_EVT_STG3_GANCE2")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_463")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING(("SFX_STG3_GROUND_SHAKE1")), LW(10))
#endif
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0029296875f), FLOAT(0.0029296875f), 13000)
        CALL(evt_snd_sfxoff, LW(10))
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING("m"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(1), 50)
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING("p"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
#else
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_SURPRISED1")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(("m")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(1), 50)
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(("p")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING(("M_N_7")))
#endif
    CALL(evt_cam3d_evt_set, 0, 242, 821, 0, 96, -8, 3000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -180, 0, FLOAT(60.0f))
        CALL(evt_party_set_dir, 0, 90, 200)
        CALL(evt_party_stop, 0)
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_jump_pos, -150, 60, 0, 500, 0, -1000)
    CALL(evt_mario_set_dir, 90, 200, 0)
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("butai_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("butai_1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_playanim, STRING("tyuubu"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("guruguru"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("hakkou"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("souti"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_playanim, STRING("comp"), MAP_ANIM_PLAY_LOOP, 0)
#else
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("butai_2")), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("butai_1")), 1)
    CALL(evt_map_playanim, STRING(("tyuubu")), 1, 0)
    CALL(evt_map_playanim, STRING(("guruguru")), 1, 0)
    CALL(evt_map_playanim, STRING(("hakkou")), 1, 0)
    CALL(evt_map_playanim, STRING(("souti")), 0, 0)
    CALL(evt_map_playanim, STRING(("comp")), 1, 0)
#endif
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
#ifdef VERSION_G8ME01
        CALL(evt_map_pauseanim, 0, STRING("souti"))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_ABSORB_DEVICE_UP1"), 0, 60, 0, LW(10))
        WAIT_MS(2000)
        CALL(evt_map_replayanim, 0, STRING("souti"))
        CALL(evt_map_checkanim, STRING("souti"), LW(0), LW(1))
#else
        CALL(evt_map_pauseanim, 0, STRING(("souti")))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_ABSORB_DEVICE_UP1")), 0, 60, 0, LW(10))
        WAIT_MS(2000)
        CALL(evt_map_replayanim, 0, STRING(("souti")))
        CALL(evt_map_checkanim, STRING(("souti")), LW(0), LW(1))
#endif
        SUBTRACT(LW(1), 100)
        WAIT_MS(LW(1))
        CALL(evt_snd_sfxoff, LW(10))
#ifdef VERSION_G8ME01
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_ABSORB_DEVICE_UP_STOP1"), 0, 60, 0, 0)
#else
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_ABSORB_DEVICE_UP_STOP1")), 0, 60, 0, 0)
#endif
    END_INLINE_EVENT
    SET_INT_READ_ADDR(PTR(&block_data))
    LOOP(9)
        READ_INT(LW(5))
        BEGIN_INLINE_EVENT
            CALL(evt_mapobj_get_position, LW(5), LW(0), LW(1), LW(2))
            LOOP(0)
                CALL(evt_mapobj_get_position, LW(5), LW(10), LW(11), LW(12))
                IF_INT_NOT_EQUAL(LW(1), LW(11))
#ifdef VERSION_G8ME01
                    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_RING_DOWN1"), LW(10), LW(11), LW(12), 0)
#else
                    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_RING_DOWN1")), LW(10), LW(11), LW(12), 0)
#endif
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_LOOP
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 0, STRING("ガンス"), NPC_FLAG_AUTO_MOVE_ANIM)
    LOOP(0)
        CALL(evt_map_checkanim, STRING("souti"), LW(0), LW(1))
#else
    CALL(evt_npc_flag_onoff, 0, STRING(("\203K\203\223\203X")), 1024)
    LOOP(0)
        CALL(evt_map_checkanim, STRING(("souti")), LW(0), LW(1))
#endif
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        CALL(getY)
        WAIT_FRAMES(1)
    END_LOOP
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 1, STRING("ガンス"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_hitobj_onoff, STRING("a_souti"), 1, 1)
#else
    CALL(evt_npc_flag_onoff, 1, STRING(("\203K\203\223\203X")), 1024)
    CALL(evt_hitobj_onoff, STRING(("a_souti")), 1, 1)
#endif
    WAIT_MS(1000)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    IF_INT_GREATER(LW(1), 50)
#ifdef VERSION_G8ME01
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING("p"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
#else
        CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(("p")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
#endif
    END_IF
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, 0, 305, 384, 0, 325, -15, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_464"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_464")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 155, 288, 0, 105, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_465"), 0, STRING("ガンス"))
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_465")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING(("M_I_Y")))
#endif
    CALL(evt_party_set_pos, 0, -180, 60, 0)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -150, 155, 288, -150, 105, -8, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_465_01"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_465_01")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 155, 288, 0, 105, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_466"), 0, STRING("ガンス"))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("ガンス"), STRING("P_1"))
        WAIT_MS(333)
        CALL(evt_npc_set_anim, STRING("ガンス"), STRING("P_2"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_466")), 0, STRING(("\203K\203\223\203X")))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("P_1")))
        WAIT_MS(333)
        CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("P_2")))
#endif
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_VALUE(GF(10), 0)
#ifdef VERSION_G8ME01
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_ABSORB_DEVICE1"), 0, 60, 0, LW(9))
#else
        CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_ABSORB_DEVICE1")), 0, 60, 0, LW(9))
#endif
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 2000, 500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_snd_sfx_pit, LW(9), LW(0))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 2000, 0, 500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_snd_sfx_pit, LW(9), LW(0))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            IF_INT_EQUAL(GF(10), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 90, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_snd_sfx_vol, LW(9), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_snd_sfxoff, LW(9))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 100, 800, 5000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
#ifdef VERSION_G8ME01
        CALL(evt_map_set_playrate, STRING("tyuubu"), LW(0))
        CALL(evt_map_set_playrate, STRING("hakkou"), LW(0))
        CALL(evt_map_set_playrate, STRING("guruguru"), LW(0))
#else
        CALL(evt_map_set_playrate, STRING(("tyuubu")), LW(0))
        CALL(evt_map_set_playrate, STRING(("hakkou")), LW(0))
        CALL(evt_map_set_playrate, STRING(("guruguru")), LW(0))
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(evt_shake), LW(9))
    CALL(evt_cam3d_evt_set, 0, 142, 215, 0, 105, -8, 5000, EASING_SINE_IN_OUT)
    WAIT_MS(5000)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#ifdef VERSION_G8ME01
    CALL(evt_npc_get_position, STRING("ガンス"), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_GANSU_BIG1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_npc_set_anim, STRING("ガンス"), STRING("H_1"))
#else
    CALL(evt_npc_get_position, STRING(("\203K\203\223\203X")), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_GANSU_BIG1")), LW(10), LW(11), LW(12), 0)
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("H_1")))
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 100, 200, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
#ifdef VERSION_G8ME01
            CALL(evt_npc_set_scale, STRING("ガンス"), LW(0), LW(0), LW(0))
#else
            CALL(evt_npc_set_scale, STRING(("\203K\203\223\203X")), LW(0), LW(0), LW(0))
#endif
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 170, 385, 0, 135, -11, 300, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(gans_white_on)
#ifdef VERSION_G8ME01
    CALL(evt_paper_entry, STRING("OFF_d_bom"))
    CALL(evt_npc_get_position, STRING("ガンス"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("マッチョガンス"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("マッチョガンス"), 270)
    CALL(evt_npc_set_width, STRING("マッチョガンス"), 60)
    CALL(evt_npc_set_height, STRING("マッチョガンス"), 80)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_GANSU_BIG2"), 0)
    CALL(evt_npc_set_paper, STRING("ガンス"), STRING("OFF_d_bom"))
    CALL(evt_npc_set_paper_anim, STRING("ガンス"), STRING("A_1"))
    WAIT_MS(2500)
    CALL(evt_npc_set_position, STRING("ガンス"), 0, -1000, 0)
    CALL(evt_npc_clear_paper, STRING("ガンス"))
    CALL(gans_white_off)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING("stg3_tou_466_01"), 0, STRING("マッチョガンス"))
#else
    CALL(evt_paper_entry, STRING(("OFF_d_bom")))
    CALL(evt_npc_get_position, STRING(("\203K\203\223\203X")), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")), 270)
    CALL(evt_npc_set_width, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")), 60)
    CALL(evt_npc_set_height, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")), 80)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_GANSU_BIG2")), 0)
    CALL(evt_npc_set_paper, STRING(("\203K\203\223\203X")), STRING(("OFF_d_bom")))
    CALL(evt_npc_set_paper_anim, STRING(("\203K\203\223\203X")), STRING(("A_1")))
    WAIT_MS(2500)
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 0, -1000, 0)
    CALL(evt_npc_clear_paper, STRING(("\203K\203\223\203X")))
    CALL(gans_white_off)
    DELETE_EVENT(LW(9))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_466_01")), 0, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -150, 155, 288, -150, 105, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_mario_set_pose, STRING("M_I_N"))
#else
    CALL(evt_mario_set_pose, STRING(("M_I_N")))
#endif
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 300, 0, -1000)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_467"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_467")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 170, 385, 0, 135, -11, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_468"), 0, STRING("マッチョガンス"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_468")), 0, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 210, 852, 0, 135, -12, 1000, EASING_SINE_IN_OUT)
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_battle_info, STRING("マッチョガンス"), 23)
    CALL(evt_npc_battle_start, STRING("マッチョガンス"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_mario_kill_party, 1)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_ABSORB_DEVICE2"), LW(15))
    CALL(evt_snd_sfx_vol, LW(15), 38)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_party_stop, 0)
    CALL(evt_npc_get_position, STRING("マッチョガンス"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ガンス"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("マッチョガンス"), 0, -1000, 0)
    CALL(evt_npc_set_scale, STRING("ガンス"), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING("ガンス"), STRING("D_1"))
    CALL(evt_npc_set_autotalkpose, STRING("ガンス"), STRING("D_1"), STRING("D_2"))
    CALL(evt_cam3d_evt_set, 0, 140, 385, 0, 105, -11, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_473"), 0, STRING("ガンス"))
#else
    CALL(evt_npc_set_battle_info, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")), 23)
    CALL(evt_npc_battle_start, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_ABSORB_DEVICE2")), LW(15))
    CALL(evt_snd_sfx_vol, LW(15), 38)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_party_stop, 0)
    CALL(evt_npc_get_position, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING(("\203}\203b\203`\203\207\203K\203\223\203X")), 0, -1000, 0)
    CALL(evt_npc_set_scale, STRING(("\203K\203\223\203X")), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING(("\203K\203\223\203X")), STRING(("D_1")))
    CALL(evt_npc_set_autotalkpose, STRING(("\203K\203\223\203X")), STRING(("D_1")), STRING(("D_2")))
    CALL(evt_cam3d_evt_set, 0, 140, 385, 0, 105, -11, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473")), 0, STRING(("\203K\203\223\203X")))
#endif
    WAIT_MS(300)
    SET_VALUE(GF(10), 1)
    CALL(evt_cam3d_evt_set, -130, 140, 385, -130, 105, -11, 700, EASING_SINE_IN_OUT)
    WAIT_MS(700)
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
#ifdef VERSION_G8ME01
        CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING("BGM_EVT_ORGEL1"), 3000)
    END_INLINE_EVENT
    CALL(evt_npc_set_position, STRING("キノシコワ"), -325, 0, 0)
    CALL(evt_npc_status_onoff, 1, STRING("キノシコワ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_01"), 0, STRING("キノシコワ"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING("m"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING("p"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
#else
        CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_FADEOUT, STRING(("BGM_EVT_ORGEL1")), 3000)
    END_INLINE_EVENT
    CALL(evt_npc_set_position, STRING(("\203L\203m\203V\203R\203\217")), -325, 0, 0)
    CALL(evt_npc_status_onoff, 1, STRING(("\203L\203m\203V\203R\203\217")), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_01")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FIND1_2")), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(("m")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(("p")), 0, 0, 0, 0, 0, 0, 0, 0, 0)
#endif
    WAIT_MS(1500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 270, 200, 0)
    END_INLINE_EVENT
    CALL(evt_party_set_dir, 0, 270, 200)
    CALL(evt_cam3d_evt_set, -320, 95, 287, -320, 40, -8, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    WAIT_MS(333)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_02"), 0, STRING("キノシコワ"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_02")), 0, STRING(("\203L\203m\203V\203R\203\217")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -160, 150, 288, -160, 100, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_473_03"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_473_03")))
#endif
    CALL(evt_cam3d_evt_set, -200, 222, 682, -200, 100, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(333)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_04"), 0, STRING("キノシコワ"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_04")), 0, STRING(("\203L\203m\203V\203R\203\217")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -320, 95, 287, -320, 40, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(333)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_05"), 0, STRING("キノシコワ"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_05")), 0, STRING(("\203L\203m\203V\203R\203\217")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -160, 150, 288, -160, 100, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_473_06"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_473_06")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -320, 95, 287, -320, 40, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(333)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -25, 197, 574, -25, 105, -8, 3000, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -280, 20, 0, FLOAT(60.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING("キノシコワ"), -250, 20, 20, 500, 30)
    CALL(evt_npc_jump_position_nohit, STRING("キノシコワ"), -215, 40, 20, 500, 30)
    CALL(evt_npc_jump_position_nohit, STRING("キノシコワ"), -185, 60, 20, 500, 30)
    CALL(evt_npc_move_position, STRING("キノシコワ"), -135, 80, 0, FLOAT(60.0f), 1)
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -25, 197, 574, -25, 105, -8, 3000, EASING_SINE_IN_OUT)
    CALL(evt_npc_move_position, STRING(("\203L\203m\203V\203R\203\217")), -280, 20, 0, FLOAT(60.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING(("\203L\203m\203V\203R\203\217")), -250, 20, 20, 500, 30)
    CALL(evt_npc_jump_position_nohit, STRING(("\203L\203m\203V\203R\203\217")), -215, 40, 20, 500, 30)
    CALL(evt_npc_jump_position_nohit, STRING(("\203L\203m\203V\203R\203\217")), -185, 60, 20, 500, 30)
    CALL(evt_npc_move_position, STRING(("\203L\203m\203V\203R\203\217")), -135, 80, 0, FLOAT(60.0f), 1)
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_mario_set_dir, 90, 300, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_set_dir, 0, 90, 300)
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    CALL(evt_npc_move_position, STRING("キノシコワ"), -50, 75, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07_01"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07_02"), 0, STRING("ガンス"))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07_03"), 0, STRING("キノシコワ"))
#else
    CALL(evt_npc_move_position, STRING(("\203L\203m\203V\203R\203\217")), -50, 75, 0, FLOAT(60.0f), 1)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07_01")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    WAIT_MS(333)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07_02")), 0, STRING(("\203K\203\223\203X")))
    WAIT_MS(300)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07_03")), 0, STRING(("\203L\203m\203V\203R\203\217")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -160, 150, 288, -160, 100, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_473_07_04"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_473_07_04")))
#endif
    WAIT_MS(300)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(eff_pos)
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 30)
    ADD(LW(1), 30)
#ifdef VERSION_G8ME01
    CALL(evt_eff64, STRING("sl"), STRING("sunlight_n64"), 1, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
#else
    CALL(evt_eff64, STRING(("sl")), STRING(("sunlight_n64")), 1, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
#endif
    WAIT_MS(300)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_473_07_04_01"))
    CALL(evt_mapobj_get_position, STRING("kaiten_souti"), LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(1), FLOAT(40.0f))
    CALL(evt_eff64, STRING("eff_2"), STRING("pokopi_pcharge_n64"), 11, LW(0), LW(1), LW(2), FLOAT(1.0f), 270, 0, 0, 0, 0, 0, 0)
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG3_STARSTONE_ROUND1"), 0)
    CALL(evt_snd_sfx_vol, LW(15), 161)
    CALL(evt_cam3d_evt_set, 0, 400, 357, 0, 325, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_eff_delete, STRING("sl"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_473_07_04_01")))
    CALL(evt_mapobj_get_position, STRING(("kaiten_souti")), LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(1), FLOAT(40.0f))
    CALL(evt_eff64, STRING(("eff_2")), STRING(("pokopi_pcharge_n64")), 11, LW(0), LW(1), LW(2), FLOAT(1.0f), 270, 0, 0, 0, 0, 0, 0)
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_STARSTONE_ROUND1")), 0)
    CALL(evt_snd_sfx_vol, LW(15), 161)
    CALL(evt_cam3d_evt_set, 0, 400, 357, 0, 325, -8, 0, EASING_SINE_IN_OUT)
    CALL(evt_eff_delete, STRING(("sl")))
#endif
    WAIT_FRAMES(1)
    SET_VALUE(GW(12), 650)
    WAIT_MS(600)
    SET_VALUE(GW(12), 400)
    WAIT_MS(350)
    SET_VALUE(GW(12), 250)
    WAIT_MS(250)
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_get_position, STRING("kaiten_souti"), LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(1), FLOAT(40.0f))
    CALL(evt_eff64, STRING("eff_1"), STRING("pokopi_pcharge_n64"), 4, LW(0), LW(1), LW(2), FLOAT(1.0f), 180, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    CALL(evt_npc_set_scale, STRING("キノシチョフ"), FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
    CALL(evt_mapobj_get_position, STRING("kaiten_souti"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("キノシチョフ"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING("キノシチョフ"), NPC_FLAG_AUTO_MOVE_ANIM)
#else
    CALL(evt_mapobj_get_position, STRING(("kaiten_souti")), LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(1), FLOAT(40.0f))
    CALL(evt_eff64, STRING(("eff_1")), STRING(("pokopi_pcharge_n64")), 4, LW(0), LW(1), LW(2), FLOAT(1.0f), 180, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    CALL(evt_npc_set_scale, STRING(("\203L\203m\203V\203`\203\207\203t")), FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
    CALL(evt_mapobj_get_position, STRING(("kaiten_souti")), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING(("\203L\203m\203V\203`\203\207\203t")), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_flag_onoff, 0, STRING(("\203L\203m\203V\203`\203\207\203t")), 1024)
#endif
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 1800, 3000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
#ifdef VERSION_G8ME01
            CALL(evt_npc_set_rotate, STRING("キノシチョフ"), FLOAT(0.0f), LW(0), FLOAT(0.0f))
#else
            CALL(evt_npc_set_rotate, STRING(("\203L\203m\203V\203`\203\207\203t")), FLOAT(0.0f), LW(0), FLOAT(0.0f))
#endif
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
#ifdef VERSION_G8ME01
        CALL(evt_npc_flag_onoff, 1, STRING("キノシチョフ"), NPC_FLAG_AUTO_MOVE_ANIM)
        CALL(evt_npc_set_rotate, STRING("キノシチョフ"), FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
#else
        CALL(evt_npc_flag_onoff, 1, STRING(("\203L\203m\203V\203`\203\207\203t")), 1024)
        CALL(evt_npc_set_rotate, STRING(("\203L\203m\203V\203`\203\207\203t")), FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
#endif
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 100, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
#ifdef VERSION_G8ME01
            CALL(evt_npc_set_scale, STRING("キノシチョフ"), LW(0), LW(0), LW(0))
#else
            CALL(evt_npc_set_scale, STRING(("\203L\203m\203V\203`\203\207\203t")), LW(0), LW(0), LW(0))
#endif
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    CALL(evt_npc_jump_position_nohit, STRING("キノシチョフ"), 100, 60, 75, 800, 170)
#else
    CALL(evt_npc_jump_position_nohit, STRING(("\203L\203m\203V\203`\203\207\203t")), 100, 60, 75, 800, 170)
#endif
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 127)
        LOOP(10)
            ADD(LW(0), -8)
            CALL(evt_snd_sfx_vol, LW(15), LW(0))
            WAIT_MS(100)
        END_LOOP
        CALL(evt_snd_sfx_vol, LW(15), 38)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 100, 166, 536, 100, 90, -8, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    WAIT_MS(2000)
    WAIT_MS(400)
    CALL(evt_cam3d_evt_set, 100, 133, 339, 100, 85, -8, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07_05"), 0, STRING("キノシチョフ"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07_05")), 0, STRING(("\203L\203m\203V\203`\203\207\203t")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -75, 133, 339, -75, 85, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07_06"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("キノシコワ"), 75, 75, 0, FLOAT(100.0f), 1)
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07_06")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\203L\203m\203V\203R\203\217")), 75, 75, 0, FLOAT(100.0f), 1)
#endif
    END_INLINE_EVENT
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, 88, 133, 339, 88, 85, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_set_dir_to_target, STRING("キノシチョフ"), STRING("キノシコワ"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07_07"), 0, STRING("キノシチョフ"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_07_08"), 0, STRING("キノシコワ"))
#else
    CALL(evt_set_dir_to_target, STRING(("\203L\203m\203V\203`\203\207\203t")), STRING(("\203L\203m\203V\203R\203\217")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07_07")), 0, STRING(("\203L\203m\203V\203`\203\207\203t")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_07_08")), 0, STRING(("\203L\203m\203V\203R\203\217")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -160, 150, 288, -160, 100, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_473_07_09"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_473_07_09")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 356, 1016, 25, 180, 32, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_npc_move_position, STRING("キノシコワ"), 75, 75, 0, FLOAT(60.0f), 1)
    WAIT_MS(333)
    CALL(evt_set_dir_to_target, STRING("キノシコワ"), STRING("mario"))
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_08"), 0, STRING("キノシコワ"))
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_snd_sfxon, STRING("SFX_STG3_STARSTONE_ROUND2"), 0)
    CALL(evt_npc_set_anim, STRING("キノシコワ"), STRING("A_1"))
    WAIT_MS(500)
    CALL(evt_map_pauseanim, 0, STRING("comp"))
#else
    CALL(evt_npc_move_position, STRING(("\203L\203m\203V\203R\203\217")), 75, 75, 0, FLOAT(60.0f), 1)
    WAIT_MS(333)
    CALL(evt_set_dir_to_target, STRING(("\203L\203m\203V\203R\203\217")), STRING(("mario")))
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_08")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_snd_sfxon, STRING(("SFX_STG3_STARSTONE_ROUND2")), 0)
    CALL(evt_npc_set_anim, STRING(("\203L\203m\203V\203R\203\217")), STRING(("A_1")))
    WAIT_MS(500)
    CALL(evt_map_pauseanim, 0, STRING(("comp")))
#endif
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 800, 0, 5000)
    LABEL(10)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
#ifdef VERSION_G8ME01
    CALL(evt_map_set_playrate, STRING("tyuubu"), LW(0))
    CALL(evt_map_set_playrate, STRING("hakkou"), LW(0))
    CALL(evt_map_set_playrate, STRING("guruguru"), LW(0))
#else
    CALL(evt_map_set_playrate, STRING(("tyuubu")), LW(0))
    CALL(evt_map_set_playrate, STRING(("hakkou")), LW(0))
    CALL(evt_map_set_playrate, STRING(("guruguru")), LW(0))
#endif
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(10)
    END_IF
#ifdef VERSION_G8ME01
    CALL(evt_map_pauseanim, 0, STRING("tyuubu"))
    CALL(evt_map_pauseanim, 0, STRING("guruguru"))
    CALL(evt_map_pauseanim, 0, STRING("hakkou"))
#else
    CALL(evt_map_pauseanim, 0, STRING(("tyuubu")))
    CALL(evt_map_pauseanim, 0, STRING(("guruguru")))
    CALL(evt_map_pauseanim, 0, STRING(("hakkou")))
#endif
    DELETE_EVENT(GW(11))
    DELETE_EVENT(GW(10))
    CALL(evt_boss_stone_r, FLOAT(0.0f))
    CALL(item_jump, FLOAT(0.0f), FLOAT(200.0f), FLOAT(60.0f))
    CALL(item_jump, FLOAT(0.0f), FLOAT(60.0f), FLOAT(100.0f))
    CALL(item_jump, FLOAT(0.0f), FLOAT(60.0f), FLOAT(100.0f))
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -160, 150, 288, -160, 100, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_473_09"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_473_09")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 88, 133, 339, 88, 85, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg3_tou_473_10"), 0, STRING("キノシコワ"))
#else
    CALL(evt_msg_print, 0, STRING(("stg3_tou_473_10")), 0, STRING(("\203L\203m\203V\203R\203\217")))
#endif
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -160, 150, 288, -160, 100, -8, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print_party, STRING("stg3_tou_473_11"))
#else
    CALL(evt_msg_print_party, STRING(("stg3_tou_473_11")))
#endif
    WAIT_MS(300)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
#else
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_NOD1_4")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_N_2")))
#endif
    WAIT_MS(500)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_party_run, 0)
    CALL(evt_cam3d_evt_set, 0, 200, 584, 0, 100, -8, 2000, EASING_SINE_IN_OUT)
    CALL(evt_mario_mov_pos2, 0, 75, FLOAT(60.0f))
    SET_VALUE(GSW_STORY_PROGRESS, STG3_32_BOSS_BATTLE)
    SET_VALUE(LW(0), 3)
#ifdef VERSION_G8ME01
    SET_VALUE(LW(1), STRING("stone"))
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL_CHILD(evt_sub_starstone)
#else
    SET_VALUE(LW(1), STRING(("stone")))
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL_CHILD(PTR(evt_sub_starstone))
#endif
    RETURN
EVT_END()

EVT_BEGIN(evt_watch_match)
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        CALL(evt_npc_set_position, STRING(("\203K\203\223\203X")), 0, 60, -130)
        ELSE
        CALL(evt_npc_set_position, STRING(("\203L\203m\203V\203R\203\217")), 0, 60, -130)
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING(("\221I\216\3501")), 32)
    LOOP(0)
        CALL(evt_sub_random, 3, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_npc_move_position, STRING(("\221I\216\3501")), -30, 0, 0, FLOAT(200.0f), 0)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3501")), 20, 60, 0, 500, 30)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3501")), 80, 70, 0, 500, 60)
            CALL(evt_eff, 0, STRING(("hit")), 0, 80, 80, 0, FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
            CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_FIGHTING3")), 80, 80, 0, 0)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3501")), 60, 60, 0, 500, 30)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3501")), 30, 60, 0, 300, 20)
            CALL(evt_npc_move_position, STRING(("\221I\216\3501")), -80, 0, 0, FLOAT(200.0f), 0)
            CALL(evt_npc_move_position, STRING(("\221I\216\3502")), 30, 0, 0, FLOAT(200.0f), 0)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3502")), -20, 60, 0, 500, 30)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3502")), -80, 70, 0, 500, 60)
            CALL(evt_eff, 0, STRING(("hit")), 0, -80, 80, 0, FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
            CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_FIGHTING3")), -80, 80, 0, 0)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3502")), -60, 60, 0, 500, 30)
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3502")), -30, 60, 0, 300, 20)
            CALL(evt_npc_move_position, STRING(("\221I\216\3502")), 80, 0, 0, FLOAT(200.0f), 0)
            CASE_EQUAL(1)
            BEGIN_INLINE_EVENT_GET_ID(LW(12))
                CALL(evt_npc_move_position, STRING(("\221I\216\3501")), -10, 0, 300, 0, 0)
            END_INLINE_EVENT
            CALL(evt_npc_move_position, STRING(("\221I\216\3502")), 10, 0, 300, 0, 0)
            CALL(evt_eff, 0, STRING(("hit")), 1, 0, 80, 0, FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
            CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_FIGHTING3")), 0, 80, 0, 0)
            WAIT_MS(100)
            LOOP(0)
                IS_EVENT_RUNNING(LW(12), LW(13))
                IF_INT_EQUAL(LW(13), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_sub_random, 4, LW(10))
            ADD(LW(10), 1)
            LOOP(LW(10))
                CALL(evt_sub_random, 100, LW(0))
                IF_INT_LESS(LW(0), 50)
                    BEGIN_INLINE_EVENT_GET_ID(LW(12))
                        CALL(evt_npc_move_position, STRING(("\221I\216\3501")), -140, 0, 800, 0, 0)
                    END_INLINE_EVENT
                    CALL(evt_npc_move_position, STRING(("\221I\216\3502")), -120, 0, 800, 0, 0)
                    CALL(evt_eff, 0, STRING(("hit")), 0, -130, 80, 0, FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
                    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_FIGHTING3")), -130, 80, 0, 0)
                    ELSE
                    BEGIN_INLINE_EVENT_GET_ID(LW(12))
                        CALL(evt_npc_move_position, STRING(("\221I\216\3501")), 120, 0, 800, 0, 0)
                    END_INLINE_EVENT
                    CALL(evt_npc_move_position, STRING(("\221I\216\3502")), 140, 0, 800, 0, 0)
                    CALL(evt_eff, 0, STRING(("hit")), 0, 130, 80, 0, FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
                    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_FIGHTING3")), 130, 80, 0, 0)
                END_IF
                WAIT_MS(100)
                LOOP(0)
                    IS_EVENT_RUNNING(LW(12), LW(13))
                    IF_INT_EQUAL(LW(13), 0)
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
            BEGIN_INLINE_EVENT_GET_ID(LW(12))
                CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3501")), -80, 60, 0, 500, 30)
            END_INLINE_EVENT
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3502")), 80, 60, 0, 500, 30)
            WAIT_MS(100)
            LOOP(0)
                IS_EVENT_RUNNING(LW(12), LW(13))
                IF_INT_EQUAL(LW(13), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CASE_DEFAULT
            BEGIN_INLINE_EVENT_GET_ID(LW(12))
                CALL(evt_npc_move_position, STRING(("\221I\216\3501")), -40, 0, 500, 0, 0)
            END_INLINE_EVENT
            CALL(evt_npc_move_position, STRING(("\221I\216\3502")), 40, 0, 500, 0, 0)
            WAIT_MS(100)
            LOOP(0)
                IS_EVENT_RUNNING(LW(12), LW(13))
                IF_INT_EQUAL(LW(13), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            BEGIN_INLINE_EVENT_GET_ID(LW(12))
                CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3501")), -10, 100, 0, 400, 30)
            END_INLINE_EVENT
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3502")), 10, 100, 0, 400, 30)
            CALL(evt_eff, 0, STRING(("hit")), 2, 0, 120, 0, FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
            CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_FIGHTING3")), 0, 120, 0, 0)
            WAIT_MS(100)
            LOOP(0)
                IS_EVENT_RUNNING(LW(12), LW(13))
                IF_INT_EQUAL(LW(13), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            BEGIN_INLINE_EVENT_GET_ID(LW(12))
                CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3501")), -80, 60, 0, 500, 30)
            END_INLINE_EVENT
            CALL(evt_npc_jump_position_nohit, STRING(("\221I\216\3502")), 80, 60, 0, 500, 30)
            WAIT_MS(100)
            LOOP(0)
                IS_EVENT_RUNNING(LW(12), LW(13))
                IF_INT_EQUAL(LW(13), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SWITCH
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku1")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku1")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_119")), 0, STRING(("kyaku1")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_119")), 0, STRING(("kyaku1")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_120")), 0, STRING(("kyaku1")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_121")), 0, STRING(("kyaku1")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku2")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku2")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_122")), 0, STRING(("kyaku2")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_122")), 0, STRING(("kyaku2")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_123")), 0, STRING(("kyaku2")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_124")), 0, STRING(("kyaku2")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku3")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku3")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_125")), 0, STRING(("kyaku3")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_125")), 0, STRING(("kyaku3")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_126")), 0, STRING(("kyaku3")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_127")), 0, STRING(("kyaku3")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_4)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku4")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku4")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_128")), 0, STRING(("kyaku4")))
        CALL(evt_msg_select, 0, STRING(("stg3_tou_128_01")))
        CALL(evt_msg_print_add, 0, STRING(("stg3_tou_128_02")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_128")), 0, STRING(("kyaku4")))
            CALL(evt_msg_select, 0, STRING(("stg3_tou_128_01")))
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_128_02")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_129")), 0, STRING(("kyaku4")))
            CALL(evt_msg_select, 0, STRING(("stg3_tou_129_01")))
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_129_02")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_130")), 0, STRING(("kyaku4")))
            CALL(evt_msg_select, 0, STRING(("stg3_tou_130_01")))
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_130_02")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_5)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku1")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku1")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_131")), 0, STRING(("kyaku1")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_131")), 0, STRING(("kyaku1")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_132")), 0, STRING(("kyaku1")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_133")), 0, STRING(("kyaku1")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_6)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku2")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku2")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_134")), 0, STRING(("kyaku2")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_134")), 0, STRING(("kyaku2")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_135")), 0, STRING(("kyaku2")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_136")), 0, STRING(("kyaku2")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_7)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku3")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku3")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_137")), 0, STRING(("kyaku3")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_137")), 0, STRING(("kyaku3")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_138")), 0, STRING(("kyaku3")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_139")), 0, STRING(("kyaku3")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_kyaku_8)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_set_dir_to_target, STRING(("kyaku4")), STRING(("mario")))
    CALL(evt_mario_set_dir_npc, STRING(("kyaku4")))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_04_GONZALES_IS_BORN)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_140")), 0, STRING(("kyaku4")))
        CALL(evt_msg_select, 0, STRING(("stg3_tou_140_01")))
        CALL(evt_msg_print_add, 0, STRING(("stg3_tou_140_02")))
        CASE_DEFAULT
        CALL(evt_tou_get_ranking, LW(10))
        SWITCH(LW(10))
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_140")), 0, STRING(("kyaku4")))
            CALL(evt_msg_select, 0, STRING(("stg3_tou_140_01")))
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_140_02")))
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_141")), 0, STRING(("kyaku4")))
            CALL(evt_msg_select, 0, STRING(("stg3_tou_141_01")))
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_141_02")))
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_tou_142")), 0, STRING(("kyaku4")))
            CALL(evt_msg_select, 0, STRING(("stg3_tou_142_01")))
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_142_02")))
        END_SWITCH
    END_SWITCH
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
EVT_BEGIN(evt_kyaku_check)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        SET_VALUE(LW(1), 0)
        CALL(evt_mario_get_name_hitobj_front, LW(2))
        IF_STR_EQUAL(LW(2), STRING("kyaku1"))
            SET_VALUE(LW(1), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("kyaku2"))
            SET_VALUE(LW(1), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("kyaku3"))
            SET_VALUE(LW(1), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("kyaku4"))
            SET_VALUE(LW(1), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("kyaku5"))
            SET_VALUE(LW(1), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("kyaku6"))
            SET_VALUE(LW(1), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("kyaku7"))
            SET_VALUE(LW(1), 1)
        END_IF
        IF_STR_EQUAL(LW(2), STRING("kyaku8"))
            SET_VALUE(LW(1), 1)
        END_IF
        CALL(evt_mario_party_use_check, LW(3))
        IF_INT_NOT_EQUAL(LW(3), 0)
            SET_VALUE(LW(1), 0)
        END_IF
        IF_INT_EQUAL(LW(1), 1)
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_mario_balloon_tenten)
                SET_VALUE(LW(0), 1)
            END_IF
            ELSE
            IF_INT_EQUAL(LW(0), 1)
                CALL(evt_mario_balloon_off)
                SET_VALUE(LW(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif
EVT_BEGIN(tou_03_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(STG3_31_GRUBBAS_PLANS)
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CASE_DEFAULT
        BEGIN_INLINE_EVENT
            CALL(evt_snd_envoff, PSND_FADEOUT)
            WAIT_MS(1000)
#ifdef VERSION_G8ME01
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG3_TOU3"))
#else
            CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING(("ENV_STG3_TOU3")))
#endif
        END_INLINE_EVENT
    END_SWITCH
    CALL(evt_bero_get_entername, LW(0))
#ifdef VERSION_G8ME01
    IF_STR_NOT_EQUAL(LW(0), STRING("s_bero_2"))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("ue"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_map_playanim, STRING("light_coloer"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("light_l"), MAP_ANIM_PLAY_LOOP, 0)
    CALL(evt_map_playanim, STRING("light_r"), MAP_ANIM_PLAY_LOOP, 0)
#else
    IF_STR_NOT_EQUAL(LW(0), STRING(("s_bero_2")))
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("ue")), 1)
    END_IF
    CALL(evt_map_playanim, STRING(("light_coloer")), 1, 0)
    CALL(evt_map_playanim, STRING(("light_l")), 1, 0)
    CALL(evt_map_playanim, STRING(("light_r")), 1, 0)
#endif
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG3_31_GRUBBAS_PLANS)
        CALL_CHILD(PTR(evt_kyaku))
        WAIT_FRAMES(1)
    END_IF
    CALL(screen_init)
    BEGIN_INLINE_EVENT
        CALL(screen_evt)
    END_INLINE_EVENT
    WAIT_FRAMES(1)
    CALL(evt_map_set_tevcallback, 1, PTR(screen_tev))
#ifdef VERSION_G8ME01
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("monittor"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("monittor"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("butai_2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_souti"), 1, 0)
#else
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("monittor")), 16777216)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("monittor")), 256)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("butai_2")), 1)
    CALL(evt_hitobj_onoff, STRING(("a_souti")), 1, 0)
#endif
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        RETURN
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(npc_entry))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_01_ARENA_INTRO_1F)
        SET_VALUE(LW(0), PTR(bero_data))
        CALL(evt_bero_get_info)
#ifdef VERSION_G8ME01
        CALL_CHILD(bero_case_entry)
#else
        CALL_CHILD(PTR(bero_case_entry))
#endif
        CASE_DEFAULT
        SET_VALUE(LW(0), PTR(bero_data))
        CALL(evt_bero_get_info)
#ifdef VERSION_G8ME01
        CALL_CHILD(evt_bero_info_run)
#else
        CALL_CHILD(PTR(evt_bero_info_run))
#endif
    END_SWITCH
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG3_01_ARENA_INTRO_1F)
#ifdef VERSION_G8ME01
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
#else
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG3_TOU2")))
#endif
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
        SPAWN_EVENT(PTR(evt_tougi))
        CASE_EQUAL(STG3_31_GRUBBAS_PLANS)
#ifdef VERSION_G8ME01
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_2_a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_2_b"), MAP_OBJ_FLAG_HIDDEN)
        SET_VALUE(LW(0), STRING("s_bero_1"))
        CALL_CHILD(bero_case_switch_off)
#else
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_2_a")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_2_b")), 1)
        SET_VALUE(LW(0), STRING(("s_bero_1")))
        CALL_CHILD(PTR(bero_case_switch_off))
#endif
        SPAWN_EVENT(PTR(evt_boss))
    END_SWITCH
    IF_INT_EQUAL(GSWF(2450), 1)
        IF_INT_EQUAL(GSWF(2451), 0)
            CALL(evt_mario_kill_party, 0)
            CALL(evt_mario_kill_party, 1)
        END_IF
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, STG3_34_RETURN_TO_ROGUEPORT)
        IF_INT_EQUAL(GSW_GLITZ_PIT_OPPONENT, 0)
            IF_INT_EQUAL(GSWF(2383), 1)
                IF_INT_EQUAL(GSWF(2455), 0)
                    CALL(evt_snd_bgmoff, PSND_FADEOUT)
                    SPAWN_EVENT(PTR(evt_champion))
                    ELSE
                    SPAWN_EVENT(PTR(evt_champion2))
                END_IF
                RETURN
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(GSWF_GLITZ_PIT_MATCH_RESERVED, 1)
        CALL(evt_mario_set_pos, 0, 0, 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_party_set_pos, 1, LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_2_a"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("door_2_b"), MAP_OBJ_FLAG_HIDDEN)
#else
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_2_a")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_2_b")), 1)
#endif
        CALL(evt_tou_get_ranking, LW(0))
        SET_VALUE(GSW(506), LW(0))
        SPAWN_EVENT(PTR(evt_match))
        ELSE
        SWITCH(GSW_STORY_PROGRESS)
            CASE_BETWEEN(STG3_01_ARENA_INTRO_1F, STG3_30_04_AIR_DUCT_EXIT)
#ifdef VERSION_G8ME01
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
#else
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG3_TOU2")))
#endif
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            SPAWN_EVENT(PTR(evt_watch_match))
            CASE_LESS(STG3_32_BOSS_BATTLE)
            CASE_DEFAULT
#ifdef VERSION_G8ME01
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG3_TOU2"))
#else
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING(("BGM_STG3_TOU2")))
#endif
            CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_UNSILENT, 0)
            SPAWN_EVENT(PTR(evt_watch_match))
        END_SWITCH
    END_IF
    IF_INT_NOT_EQUAL(GSW_STORY_PROGRESS, STG3_31_GRUBBAS_PLANS)
#ifdef VERSION_G8ME01
        CALL(evt_hitobj_onoff, STRING("a_kyaku_A"), 1, 1)
        CALL(evt_hitobj_onoff, STRING("a_kyaku_B"), 1, 1)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku1"), 0, PTR(evt_kyaku_1), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku2"), 0, PTR(evt_kyaku_2), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku3"), 0, PTR(evt_kyaku_3), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku4"), 0, PTR(evt_kyaku_4), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku5"), 0, PTR(evt_kyaku_5), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku6"), 0, PTR(evt_kyaku_6), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku7"), 0, PTR(evt_kyaku_7), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 0, STRING("kyaku8"), 0, PTR(evt_kyaku_8), 0)
        SPAWN_EVENT(PTR(evt_kyaku_check))
        ELSE
        CALL(evt_hitobj_onoff, STRING("a_kyaku_A"), 1, 0)
        CALL(evt_hitobj_onoff, STRING("a_kyaku_B"), 1, 0)
#else
        CALL(evt_hitobj_onoff, STRING(("a_kyaku_A")), 1, 1)
        CALL(evt_hitobj_onoff, STRING(("a_kyaku_B")), 1, 1)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku1")), 0, PTR(evt_kyaku_1), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku2")), 0, PTR(evt_kyaku_2), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku3")), 0, PTR(evt_kyaku_3), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku4")), 0, PTR(evt_kyaku_4), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku5")), 0, PTR(evt_kyaku_5), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku6")), 0, PTR(evt_kyaku_6), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku7")), 0, PTR(evt_kyaku_7), 0)
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING(("kyaku8")), 0, PTR(evt_kyaku_8), 0)
        ELSE
        CALL(evt_hitobj_onoff, STRING(("a_kyaku_A")), 1, 0)
        CALL(evt_hitobj_onoff, STRING(("a_kyaku_B")), 1, 0)
#endif
    END_IF
    RETURN
EVT_END()

