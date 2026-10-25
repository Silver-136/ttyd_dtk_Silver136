#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_ext.h"
#include "evt/evt_fade.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "dolphin/exi.h"

#include "driver/animdrv.h"
#include "driver/npcdrv.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "driver/mapdrv.h"
#include "driver/msgdrv.h"
#include "driver/windowdrv.h"
#include "driver/swdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "manager/filemgr.h"
#include "manager/fontmgr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "string.h"
#include "stdio.h"
#include "pmario_sound.h"
#include "system.h"
#include "sdk/texPalette.h"
#include "data/mapdata.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "evt/evt_memcard.h"
#include "end.h"
#include "driver/extdrv.h"
#ifdef VERSION_G8ME01
#endif

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
typedef struct EndMoa { Vec pos; s32 chain; s32 idx; } EndMoa;
typedef struct EndExtData { const char* anim; const char* pose; f32 speed; } EndExtData;
typedef struct EndWork {
    FileEntry* tpl;
    s32 mode;
    s32 alpha;
    s32 alpha2;
    s32 tevAlpha;
    s32 win1[3];
    s32 win2[3];
    s32 field_0x2c;
    s32 pos;
    EventEntry* evt;
} EndWork;
typedef struct CreditMsg {
    s32 field_0x0;
    s32 pos;
    s32 type;
    const char* msg1[3];
    const char* msg2[3];
} CreditMsg;
typedef struct CreditNpc { const char* agb; const char* anim; EvtWord* evt; } CreditNpc;
typedef struct CreditActor {
    s32 state;
    s32 poseId;
    f32 angle;
    f32 limit;
    s32 pos;
    Mtx mtx;
} CreditActor;
EndWork end_work;
s32 (*end_moa_chain_group)[2];
s32* end_moa_chain_count;
EndExtData* end_moa_ext_data;
EndMoa* end_moa_tbl;
extern EndWork* end_wp;
extern CreditMsg end_staff_roll_msg_tbl[50];
extern CreditNpc R_credits_npc_info[152];
extern const char* g_moamoa_chain_tbl[][2];
static inline f32 tanf(f32 x) { return tan(x); }
void animPoseVivianMain(s32 poseId, Vec* pos);
s32 msgWindow_Entry(const char* msg, s32 a, s32 b);
typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

USER_FUNC(evt_end_moamoa_exit);
USER_FUNC(evt_end_moamoa_init);
USER_FUNC(evt_end_npc_silhouette_on);
USER_FUNC(evt_end_font_restore);
USER_FUNC(evt_end_staff_roll_init);
USER_FUNC(evt_end_staff_roll_msg_main);
USER_FUNC(evt_end_image_set_blend);
USER_FUNC(evt_end_image_set_mode);
USER_FUNC(evt_end_image_set_overlay_alpha);
USER_FUNC(evt_end_image_set_alpha);
USER_FUNC(evt_end_image_disp_entry);
USER_FUNC(evt_end_credit_npc_main);
USER_FUNC(evt_end_credit_npc_get_wait_type);
void end_ext_moamoa_dispent(void);
void end_ext_disp(CameraId cameraId, void* param);
void end_ext_moamoa_main(void);
void end_ext_moamoa_init(void);
f32 end_randf(f32 min, f32 max);
USER_FUNC(end_bgm_wait_stop);
void end_staff_roll_exit(void);
void end_image_disp(CameraId cameraId, void* param);
void end_credit_npc_disp(CameraId cameraId, void* param);
void end_silhouette_rendermode(s32 wXluStage);
void end_silhouette_tev(MapTevCallbackInfo* info);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

EvtWord end_credit_npc_parade[44];
EvtWord end_image_blend_loop[123];
#ifdef VERSION_G8ME01
// US: "THE END" texture per language (indexed by gp->language)
#endif
EvtWord end_image_main_evt[187];
EvtWord end_return_title_evt[49];
EvtWord end_00_init_evt[63];
EvtWord end_credit_gesso_evt[48];
EvtWord end_credit_gonbaba_evt[89];
EvtWord end_credit_batten2_evt[54];
EvtWord end_credit_atomic_evt[60];
EvtWord end_credit_korutesu_evt[81];
EvtWord end_credit_para_evt[50];
EvtWord end_credit_batten_evt[83];
EvtWord end_credit_queen_evt[89];
EvtWord end_credit_moamoa_evt[108];
f32 fd_end_00_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_end_00_neg_one_382;
f32 fd_end_00___local_PI_383;
f32 fd_end_00___local_3PIO2_385;
f32 fd_end_00_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_end_00___local_PIO2_384;
f32 fd_end_00_one_381;
f32 fd_end_00_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_end_00_neg_one_365;
f32 fd_end_00___local_PI_366;
f32 fd_end_00___local_3PIO2_368;
f32 fd_end_00_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_end_00___local_PIO2_367;
f32 fd_end_00_one_364;



void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("end_00", end_00_init_evt);
#ifdef VERSION_G8ME01
    camGetPtr(CAMERA_2D)->flags |= 0x200;
    camGetPtr(CAMERA_2D)->flags &= ~0x100;
    camGetPtr(CAMERA_2D)->flags |= 0x400;
#endif
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    end_staff_roll_exit();
#ifdef VERSION_G8ME01
    camGetPtr(CAMERA_2D)->flags &= ~0x200;
#endif
}

void _unresolved(void) {
}

void end_silhouette_tev(MapTevCallbackInfo* info) {
    GXSetTevColorIn(info->tevStage - 1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
}

void end_silhouette_rendermode(s32 wXluStage) {
    animGetPtr()->currDispMode = wXluStage;
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}

void end_credit_npc_disp(CameraId cameraId, void* param) {
    CreditActor* actor = param;

    if (evtGetValue(NULL, GF(10)) == 0) {
        mapSetTevCallback(1, end_silhouette_tev);
        animPoseSetMaterialFlagOn(actor->poseId, 0x1000000);
        animPoseSetGXFunc(actor->poseId, end_silhouette_rendermode, 0);
        animPoseDrawMtx(actor->poseId, actor->mtx, 1, 0.0f, 2.0f);
        animPoseDrawMtx(actor->poseId, actor->mtx, 2, 0.0f, 2.0f);
        animPoseDrawMtx(actor->poseId, actor->mtx, 3, 0.0f, 2.0f);
        animPoseSetMaterialFlagOff(actor->poseId, 0x1000000);
    }
}

USER_FUNC(evt_end_credit_npc_get_wait_type) {
    EvtWord* args = event->args;
    u32 id = evtGetValue(event, args[0]);

    if (id >= ARRAY_SIZE(R_credits_npc_info)) {
        evtSetValue(event, args[1], 0);
        return 2;
    }
    if (R_credits_npc_info[id].evt != NULL) {
        evtSetValue(event, args[1], 0);
    } else if (strcmp(R_credits_npc_info[id].agb, "c_gazigazi") == 0) {
        evtSetValue(event, args[1], 2);
    } else {
        evtSetValue(event, args[1], 1);
    }
    return 2;
}

EndWork* end_wp = &end_work;

CreditMsg end_staff_roll_msg_tbl[] = {
    0, 0, 0, 0, 0, 0, "a00", 0,
    0, 0, 0, 1, 0, "a01", 0, "b00",
    0, 0, 0, 0, 2, 0, "a02", 0,
    "b01", 0, 0, 0, 0, 3, 0, 0,
    0, "b02", "b03", 0, 0, 0, 4, 0,
    "a03", 0, "b04", 0, 0, 0, 0, 5,
    0, "a04", 0, "b05", 0, 0, 0, 0,
    6, 0, "a05", 0, "b06", 0, 0, 0,
    0, 7, 0, "a06", 0, "b07", 0, 0,
    0, 0, 8, 0, "a07", 0, "b08", 0,
    0, 0, 0, 9, 0, "a08", 0, "b09",
    0, 0, 0, 0, 10, 0, 0, 0,
    "b10", "b11", 0, 0, 1, 11, 0, "a09",
    0, "b12", 0, 0, 0, 1, 12, 0,
    0, 0, "b13", 0, 0, 0, 1, 13,
    0, 0, 0, "b14", 0, 0, 0, 1,
    14, 0, 0, 0, "b15", "b16", 0, 0,
    1, 15, 0, "a10", 0, "b17", 0, 0,
    0, 1, 16, 0, "a11", 0, "b18", 0,
    0, 0, 1, 17, 0, "a12", 0, "b19",
    0, 0, 0, 1, 18, 0, "a13", 0,
    "b20", 0, 0, 0, 1, 19, 0, 0,
    0, "b21", 0, 0, 0, 2, 20, 0,
    "a14", 0, "b22", 0, 0, 0, 2, 21,
    0, 0, 0, "b23", 0, 0, 0, 2,
    22, 0, 0, 0, "b24", 0, 0, 0,
    2, 23, 0, 0, 0, "b25", 0, 0,
    0, 2, 24, 0, "a15", 0, "b26", 0,
    0, 0, 2, 25, 0, 0, 0, "b27",
    0, 0, 0, 2, 26, 0, "a17", 0,
    "b28", 0, 0, 0, 2, 27, 0, 0,
    0, "b29", 0, 0, 0, 2, 28, 0,
    "a18", 0, "b30", 0, 0, 0, 2, 29,
    0, 0, 0, "b31", "b32", 0, 0, 3,
    30, 0, "a19", 0, "b33", 0, 0, 0,
    3, 31, 0, "a20", 0, "b34", "b35", "b36",
#ifdef VERSION_G8ME01
    0, 3, 32, 0, "a31", 0, "b73", "b74",
    "b75", 0, 3, 33, 0, 0, 0, "b76",
    "b77", "b92", 0, 3, 34, 0, "a36", 0,
    "b78", "b79", 0, 0, 3, 35, 0, "a37",
    0, "b80", "b81", 0, 0, 3, 36, 0,
    "a21", 0, "b37", "b38", 0, 0, 3, 37,
    0, 0, 0, "b39", "b40", "b41", 0, 3,
    38, 0, 0, 0, "b46", "b47", 0, 0,
    3, 39, 0, "a22", 0, "b48", "b49", 0,
    0, 3, 40, 0, 0, 0, "b50", "b51",
    0, 0, 0, 41, 0, "a23", 0, "b52",
    "b53", "b84", 0, 0, 42, 0, "a25", 0,
    "b55", "b86", 0, 0, 0, 43, 0, "a26",
    0, "b88", "b89", "b56", 0, 0, 44, 0,
#else
    0, 3, 32, 0, "a21", 0, "b37", "b38",
    0, 0, 3, 33, 0, 0, 0, "b39",
    "b40", 0, 0, 3, 34, 0, 0, 0,
    "b41", "b42", 0, 0, 3, 35, 0, 0,
    0, "b43", "b44", 0, 0, 3, 36, 0,
    0, 0, "b45", 0, 0, 0, 3, 37,
    0, 0, 0, "b46", "b47", 0, 0, 3,
    38, 0, "a22", 0, "b48", "b49", 0, 0,
    3, 39, 0, 0, 0, "b50", "b51", 0,
    0, 0, 40, 0, "a23", 0, "b52", "b53",
    0, 0, 0, 41, 0, "a24", 0, "b54",
    0, 0, 0, 0, 42, 0, "a25", 0,
    "b55", 0, 0, 0, 0, 43, 0, "a26",
    0, "b56", 0, 0, 0, 0, 44, 0,
#endif
    "a27", 0, "b57", 0, 0, 0, 0, 45,
    0, "a28", 0, "b58", "b59", 0, 0, 0,
    46, 0, 0, 0, "b60", "b61", 0, 0,
    0, 47, 0, "a29", 0, "b62", "b63", 0,
    0, 0, 48, 0, "a30", 0, "b64", 0,
    0, 0, 4, 49, "c00", 0, 0, "c01",
    0, 0,
};

CreditNpc R_credits_npc_info[] = {
    "c_luigi", "W_1", 0, "c_pkr", "PKR_W_1", 0, "c_kuri_h", "W_1",
    0, "c_kino_ji", "W_1", 0, "c_cook", "W_1", 0, "c_deale",
    "W_1", 0, "c_levela", "W_1", 0, "c_uranai", "W_1", 0,
    "c_majinai", "W_1", 0, "c_botta", "W_1", 0, "c_mania", "MNA_W_1",
    0, "c_joho_a", "W_1", 0, "c_gesso", "GSO_S2_1", end_credit_gesso_evt, "c_joho_b",
    "W_1", 0, "c_gshop_m", "W_2", 0, "c_gshop_m", "W_1", 0,
    "c_pub_m", "W_1", 0, "c_roten", "W_1", 0, "c_tyusan", "CHU_W_1",
    0, "c_borodo_n", "W_1", 0, "c_bom_deb", "W_1", 0, "c_kuribo_n",
    "W_1", 0, "c_hannya", "W_1", 0, "c_hannya_n", "W_1", 0,
    "c_pnk", "PNK_W_1", 0, "c_nokorin", "W_1", 0, "c_nokopapa", "NKP_W_1",
    0, "c_nokoson", "NKS_W_1", 0, "c_nokonoko", "NOK_W_1", 0, "c_kinopio",
    "KNP_W_1", 0, "c_kinopiko", "KNF_W_1", 0, "c_kokinopio", "CKP_W_1", 0,
    "c_gonbaba", "GNB_S_3", end_credit_gonbaba_evt, "c_flower", "W_1", 0, "c_kiza", "W_1",
    0, "c_kuribo", "KUR_W_1", 0, "c_kuribo", "TGK_W_1", 0, "c_kuribo",
    "PTK_W_1", 0, "c_nokoteki", "NKT_W_1", 0, "c_nokoteki", "PTP_W_1", 0,
    "c_sinnosuke", "SIS_W_1", 0, "c_togedaruma", "TGD_S_1", 0, "c_chorobon", "CBN_W_1",
    0, "c_karon_h", "KRN_W_1", 0, "c_windy", "PWD_W_1", 0, "c_pguide",
    "W_1", 0, "c_psister", "W_1", 0, "c_psoncho", "W_1", 0,
    "c_pshop", "W_1", 0, "c_puni", "W_1", 0, "c_puni", "W_1",
    0, "c_puni", "W_1", 0, "c_puni", "W_1", 0, "c_puni",
    "W_1", 0, "c_mb_robo_1a", "W_1", end_credit_batten_evt, "c_puni", "W_1", 0,
    "c_prival", "W_1", 0, "c_toge", "W_1", 0, "c_tchibi", "W_1",
    0, "c_sinemon", "SIN_W_1", 0, "c_paid", "PAI_W_1", 0, "c_m_boss",
    "W_1", 0, "c_m_buka", "W_1", 0, "c_m_waka", "W_1", 0,
    "c_m_ojo", "W_1", 0, "c_binbo", "W_1", 0, "c_monban", "W_1",
    0, VERSION_PICK("c_bteresa", "c_bteresa2"), VERSION_PICK("BTS_W_1", "W_1"), 0, "c_babyyoshi", "PYS_W_1", 0, "c_ksikowa",
    "W_1", 0, "c_k_chof", "W_1", 0, "c_gans", "W_1", 0,
    "c_bred_h", "W_1", 0, "c_report", "W_1", 0, "c_hotdog", "W_1",
    0, "c_shasho_g", "W_1", 0, "c_jugem_n", "W_1", 0, "c_togezo",
    "TGZ_W_1", 0, "c_heavy", "W_1", 0, "c_kamek", "KMK_W_1", 0,
    "c_togenoko_b", "W_1", 0, "c_burosu_h", "BRO_W_1", 0, "c_koopa", "KPA_W_1",
    0, "c_atmic_trs", "W_1", end_credit_atomic_evt, "c_kamek_be", "W_1", 0, "c_f_son",
    "W_1", 0, "c_bubusan", "W_1", 0, "c_f_mura", "W_1", 0,
    "c_f_mama", "W_1", 0, "c_f_child_a", "W_1", 0, "c_f_oji", "W_1",
    0, "c_f_oba", "W_1", 0, "c_f_karas", "A_1", 0, "c_teresa",
    "W_1", 0, "c_ranpel", "W_1", 0, "c_f_owm", "A_1", 0,
    "c_met", "MET_W_1", 0, "c_met", "TMT_W_1", 0, "c_pansy", "PAN_W_1",
    0, "c_majyorin", "MJR_W_1", 0, "c_maririn", "MRR_W_1", 0, "c_vivian",
    "PTR_W_1", 0, "c_korutesu1", "KRT_S_1", end_credit_korutesu_evt, "c_dance_r", "W_1", 0,
    "c_dance_p", "W_1", 0, "c_dance_k", "W_1", 0, "c_bomt_n", "W_1",
    0, "c_marco", "W_1", 0, "c_gara", "W_1", 0, "c_megane",
    "W_1", 0, "c_odo_k", "W_1", 0, "c_suifu_k", "W_1", 0,
    "c_suifu_b", "W_1", 0, "c_elmos", "W_1", 0, "c_kilr", "KIL_W_1",
    0, "c_pakflwr", "PKF_S_1", 0, "c_tyutyu", "PCH_W_1", 0, "c_powan",
    "W_1", 0, "c_g_moamoa", "S_3", end_credit_moamoa_evt, "c_papab", "W_1", 0,
    "c_mamab", "W_1", 0, "c_konari", "W_1", 0, "c_unten", "W_1",
    0, "c_shasho_r", "W_1", 0, "c_smoko", "W_1", 0, "c_rose",
    "W_1", 0, "c_kinodebu", "W_1", 0, "c_ghost_k", "W_1", 0,
    "c_bom_sen", "S_2", end_credit_para_evt, "c_waitress", "W_1", 0, "c_puku_c", "W_1",
    0, "c_kinopiko", "KNF_W_1", 0, "c_n_moa_a", "W_1", 0, "c_met",
    "PMT_W_1", 0, "c_sanbo", "SNB_W_1", 0, "c_bomson", "W_1", 0,
    "c_mb_robo_2a", "J_2", end_credit_batten2_evt, "c_bwhite", "W_1", 0, "c_hito_b", "W_1",
    0, "c_bom_n", "W_1", 0, "c_kmoon", "W_1", 0, "c_baria",
    "W_1", 0, "c_basabasa", "BSA_W_1", 0, "c_wanwan", "WAN_A_1", 0,
    "c_maho", "MAH_W_1", 0, "c_zako_n", "W_1", 0, "c_zako_m", "W_1",
    0, "c_shuryo", "W_1", 0, "c_q_kage_m", "KRT_Z_1", end_credit_queen_evt, "c_gazigazi",
    "GAZ_A_1", 0, "a_mario", "M_W_1", 0, "c_peach", "P_W_1", 0,
};

USER_FUNC(evt_end_credit_npc_main) {
    s32 id = evtGetValue(event, event->args[0]);
    BOOL inBattle;
    CreditActor* actor;
    f32 x;
    f32 y;
    f32 r;
    Mtx m1;
    Mtx m2;
    Mtx m3;
    Vec v;
    u32 type;

    inBattle = gp->inBattle != 0;
    if (R_credits_npc_info[id].evt != NULL) {
        evtEntry(R_credits_npc_info[id].evt, 0, 0);
        if (id + 5 < ARRAY_SIZE(R_credits_npc_info)) {
            animGroupBaseAsync(R_credits_npc_info[id + 5].agb, inBattle, NULL);
        }
        return 2;
    }
    if (isFirstCall) {
        actor = _mapAlloc(sizeof(CreditActor));
        memset(actor, 0, sizeof(CreditActor));
        actor->state = 0;
        actor->pos = end_wp->pos;
        switch (actor->pos) {
            case 0:
                actor->angle = -20.0f;
                actor->limit = 90.0f + actor->angle + 30.0f;
                break;
            case 1:
            case 4:
                actor->angle = 160.0f;
                actor->limit = 90.0f + actor->angle + 30.0f;
                break;
            case 2:
                actor->angle = 250.0f;
                actor->limit = 90.0f + actor->angle + 30.0f;
                break;
            default:
                actor->angle = 70.0f;
                actor->limit = 90.0f + actor->angle + 30.0f;
                break;
        }
        if (id + 5 < ARRAY_SIZE(R_credits_npc_info)) {
            animGroupBaseAsync(R_credits_npc_info[id + 5].agb, inBattle, NULL);
        }
        event->userdatap[0] = actor;
    }
    actor = event->userdatap[0];
    switch (actor->pos) {
        case 0:
            x = 304.0f;
            y = -240.0f;
            r = 100.0f;
            break;
        case 1:
        case 4:
            x = -304.0f;
            y = 240.0f;
            r = 100.0f;
            break;
        case 2:
            x = -304.0f;
            y = -240.0f;
            r = 100.0f;
            break;
        default:
            x = 304.0f;
            y = 240.0f;
            r = 100.0f;
            break;
    }
    if (!animGroupBaseAsync(R_credits_npc_info[id].agb, inBattle, NULL)) {
        return 0;
    }
    switch (actor->state) {
        case 0:
#ifdef VERSION_G8ME01
            actor->poseId = animPoseEntry(R_credits_npc_info[id].agb, inBattle);
            animPoseSetAnim(actor->poseId, R_credits_npc_info[id].anim, 1);
#else
            EXIGetType(0, 2, &type);
            if (type != 0x05070000) {
                actor->poseId = animPoseEntry(R_credits_npc_info[id].agb, inBattle);
                animPoseSetAnim(actor->poseId, R_credits_npc_info[id].anim, 1);
            } else {
                actor->poseId = animPoseEntry("a_mario", inBattle);
                animPoseSetAnim(actor->poseId, "M_W_1", 1);
            }
#endif
            actor->state++;
            break;
        case 1:
#ifdef VERSION_G8ME01
            actor->angle += 9.0f / gp->framerate;
#else
            actor->angle += 0.15f;
#endif
            if (actor->angle > actor->limit) {
                animPoseRelease(actor->poseId);
                _mapFree(actor);
                return 2;
            }
            break;
    }
    PSMTXTrans(m1, 0.0f, r, 0.0f);
    PSMTXRotRad(m2, 'z', 0.017453292f * actor->angle);
    PSMTXConcat(m2, m1, m1);
    PSMTXTrans(m2, x / 2.0f, y / 2.0f, 0.0f);
    PSMTXConcat(m2, m1, actor->mtx);
    PSMTXScale(m3, 1.0f, 1.0f, 0.01f);
    PSMTXConcat(actor->mtx, m3, actor->mtx);
    if (animPoseGetVivianType(actor->poseId)) {
        v = (Vec){0.0f, 0.0f, 0.0f};
        v.x = actor->mtx[0][3];
        v.y = actor->mtx[1][3];
        v.z = actor->mtx[2][3];
        animPoseVivianMain(actor->poseId, &v);
    }
    if (strcmp((const char*)animPoseGetCurrentAnim(actor->poseId), "GAZ_A_1") == 0 && animPoseGetLoopTimes(actor->poseId) > 0.9f) {
        animPoseSetAnim(actor->poseId, "GAZ_A_1", 1);
    }
    animPoseMain(actor->poseId);
    dispEntry(CAMERA_3D, 0.0f, 1, end_credit_npc_disp, actor);
    return 0;
}

EVT_BEGIN(end_credit_npc_parade)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_end_credit_npc_get_wait_type, LW(0), LW(1))
        BEGIN_INLINE_EVENT
            CALL(evt_end_credit_npc_main, LW(0))
        END_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 1)
            WAIT_MS(1920)
        END_IF
        IF_INT_EQUAL(LW(1), 2)
            WAIT_MS(6000)
        END_IF
        ADD(LW(0), 1)
        IF_INT_GREATER_OR_EQUAL(LW(0), 152)
            DEBUG_PUT_MSG(STRING("あにめおわり"))
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(GF(10), 1)
            RETURN
        END_IF
    END_LOOP
    RETURN
EVT_END()

#ifdef VERSION_G8ME01
s32 end_overlay_tex_tbl[6] = {52, 52, 54, 55, 55, 56};
#endif

EVT_BEGIN(end_image_blend_loop)
    LOOP(0)
        CALL(evt_end_image_set_mode, 1)
        CALL(evt_end_image_set_blend, 0)
        WAIT_MS(18000)
        CALL(evt_sub_intpl_msec_init, 11, 0, 255, 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_end_image_set_blend, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_end_image_set_blend, 255)
        WAIT_MS(4000)
        CALL(evt_end_image_set_mode, 2)
        CALL(evt_end_image_set_blend, 0)
        WAIT_MS(4000)
        CALL(evt_sub_intpl_msec_init, 11, 0, 255, 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_end_image_set_blend, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_end_image_set_blend, 255)
        WAIT_MS(18000)
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_end_image_set_blend, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_end_image_set_blend, 0)
        WAIT_MS(4000)
        CALL(evt_end_image_set_mode, 1)
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 4000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_end_image_set_blend, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

void end_image_disp(CameraId cameraId, void* param) {
    s32 i;
    f32 w;
    f32 h;
    CameraEntry* cam = camGetPtr(cameraId);
    s32 id = evtGetValue(NULL, GW(11));
    s32 pos = evtGetValue(NULL, GW(12));
    Vec camPos;
    Vec up;
    Vec target;
    GXTexObj obj;
    Mtx mtx;
    Mtx scale;
    Mtx lookAt;

    if (end_wp->tpl == NULL) {
        return;
    }
    if (id >= ((TEXPalette*)*end_wp->tpl->data)->numDescriptors) {
        return;
    }
    if (id == 9 && swGet(0x5E7)) {
        id = 53;
    }
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, end_wp->alpha});
    switch (end_wp->mode) {
        case 0:
            TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, id);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            break;
        case 1:
            GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, end_wp->tevAlpha});
            TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, 49);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, 50);
            GXLoadTexObj(&obj, GX_TEXMAP1);
            break;
        case 2:
            GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, end_wp->tevAlpha});
            TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, 50);
            GXLoadTexObj(&obj, GX_TEXMAP0);
            TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, 51);
            GXLoadTexObj(&obj, GX_TEXMAP1);
            break;
    }
    switch (end_wp->mode) {
        case 0:
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            break;
        default:
            GXSetNumTevStages(2);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_A0, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
            break;
    }
    switch (pos) {
        case 0:
            PSMTXTrans(mtx, 51.0f, 30.0f, -30.0f);
            break;
        case 1:
            PSMTXTrans(mtx, -51.0f, -30.0f, -30.0f);
            break;
        case 2:
            PSMTXTrans(mtx, -51.0f, 30.0f, -30.0f);
            break;
        case 3:
            PSMTXTrans(mtx, 51.0f, -30.0f, -30.0f);
            break;
        case 4:
            PSMTXTrans(mtx, 0.0f, 30.0f, -30.0f);
            break;
    }
    PSMTXScale(scale, 0.6f, 0.6f, 1.0f);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, end_wp->alpha});
            GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        } else {
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, end_wp->alpha});
            GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        w = GXGetTexObjWidth(&obj);
        h = GXGetTexObjHeight(&obj);
        GXPosition3f32(-w / 2.0f, h / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w / 2.0f, h / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(w / 2.0f, -h / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w / 2.0f, -h / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
    if (end_wp->alpha2 > 0) {
        f32 w2;
        f32 h2;

        camPos = (Vec){0.0f, 0.0f, 240.0f / tanf(0.21816616f)};
        up = (Vec){0.0f, 1.0f, 0.0f};
        target = (Vec){0.0f, 0.0f, 0.0f};
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
#ifdef VERSION_G8ME01
        TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, end_overlay_tex_tbl[gp->language]);
#else
        TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, 52);
#endif
        GXLoadTexObj(&obj, GX_TEXMAP0);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, end_wp->alpha2});
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        C_MTXLookAt(lookAt, &camPos, &up, &target);
        PSMTXTrans(mtx, 0.0f, -138.0f, 0.0f);
        PSMTXScale(scale, 1.0f, 1.0f, 1.0f);
        PSMTXConcat(lookAt, mtx, mtx);
        PSMTXConcat(mtx, scale, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        w2 = GXGetTexObjWidth(&obj);
        h2 = GXGetTexObjHeight(&obj);
        GXPosition3f32(-w2 / 2.0f, h2 / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(w2 / 2.0f, h2 / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(w2 / 2.0f, -h2 / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w2 / 2.0f, -h2 / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}

EVT_BEGIN(end_image_main_evt)
    LOOP(0)
        IF_INT_EQUAL(GF(10), 1)
            RETURN
        END_IF
        LOOP(0)
            IF_INT_EQUAL(GW(10), 1)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        BEGIN_INLINE_EVENT_GET_ID(LW(9))
            CALL(evt_end_image_disp_entry)
        END_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 11, 0, 255, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_end_image_set_alpha, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        LOOP(0)
            IF_INT_EQUAL(GW(10), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        IF_INT_EQUAL(GW(11), 49)
            WAIT_MS(500)
            CALL(evt_sub_intpl_msec_init, 11, 0, 255, 500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_end_image_set_overlay_alpha, LW(0))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            WAIT_MS(3000)
            SET_VALUE(GF(0), 0)
            BEGIN_INLINE_EVENT
                CALL(evt_snd_bgmoff_f, 512, 5000)
                WAIT_MS(5000)
                IF_INT_EQUAL(GF(0), 0)
                    CALL(evt_snd_bgmon, 513, STRING("BGM_EVT_PEACH1"))
                END_IF
            END_INLINE_EVENT
            WAIT_MS(500)
            SPAWN_EVENT_GET_ID(PTR(end_image_blend_loop), LW(10))
            LOOP(0)
                CALL(evt_key_get_buttontrg, 0, LW(0))
                IF_BITS_SET(LW(0), 4352)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            SET_VALUE(GF(0), 1)
            DELETE_EVENT(LW(10))
            CALL(evt_fade_entry, 10, 500, 0, 0, 0)
            CALL(evt_fade_end_wait)
            CALL(evt_end_image_set_mode, 0)
            CALL(evt_end_image_set_alpha, LW(0))
            CALL(evt_end_image_set_overlay_alpha, LW(0))
            CALL(evt_fade_entry, 9, 0, 0, 0, 0)
            CALL(evt_fade_end_wait)
            SET_VALUE(GW(10), 0)
            SPAWN_EVENT(PTR(end_return_title_evt))
            RETURN
        END_IF
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_end_image_set_alpha, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        DELETE_EVENT(LW(9))
        SET_VALUE(GW(10), 0)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(end_return_title_evt)
    CALL(evt_fade_entry, 12, 2000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1500)
    SET_VALUE(GF(10), 1)
    CALL(evt_end_font_restore)
    SET_VALUE(GSW_STORY_PROGRESS, 404)
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 21)
#ifdef VERSION_G8ME01
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_snd_bgmoff_f, 512, 2000)
    CALL(evt_snd_bgmoff_f, 513, 2000)
    CALL(evt_fade_set_mapchange_type, 1, 17, 2000, -1, -1)
    CALL(evt_seq_set_seq, 3, STRING("title"), 0)
    RETURN
EVT_END()

EVT_BEGIN(end_00_init_evt)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_cam3d_evt_set, 0, 0, 545, 0, 0, 0, 0, 11)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("pPlane1"), 1)
    CALL(evt_bg_set_color, 0, 0, 0, 255)
    SET_VALUE(GF(10), 0)
    CALL(evt_end_staff_roll_init)
    SPAWN_EVENT(PTR(end_credit_npc_parade))
    BEGIN_INLINE_EVENT
        CALL(evt_end_staff_roll_msg_main)
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, 512, STRING("BGM_STAFF_ROLL1"))
    SET_VALUE(GSW_STORY_PROGRESS, 404)
    RETURN
EVT_END()

EVT_BEGIN(end_credit_gesso_evt)
    CALL(evt_npc_entry, STRING("gesso"), STRING("c_gesso"))
    CALL(evt_npc_set_anim, STRING("gesso"), STRING("GSO_S2_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("gesso"), 64)
    CALL(evt_end_npc_silhouette_on, STRING("gesso"))
    CALL(evt_sub_intpl_msec_init, 11, -4000, 4000, 5000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("gesso"), 130, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("gesso"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_gonbaba_evt)
    CALL(evt_npc_entry, STRING("gonbaba"), STRING("c_gonbaba"))
    CALL(evt_npc_set_anim, STRING("gonbaba"), STRING("GNB_S_3"))
    CALL(evt_npc_flag_onoff, 1, STRING("gonbaba"), 64)
    CALL(evt_end_npc_silhouette_on, STRING("gonbaba"))
    CALL(evt_npc_set_scale, STRING("gonbaba"), FLOAT(0.5f), FLOAT(0.5f), FLOAT(0.5f))
    CALL(evt_npc_set_rotate, STRING("gonbaba"), 0, 180, 0)
    CALL(evt_sub_intpl_msec_init, 11, -10000, -1300, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("gonbaba"), LW(0), -20, 200)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_sub_intpl_msec_init, 11, -1300, -10000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("gonbaba"), LW(0), -20, 200)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("gonbaba"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_batten2_evt)
    CALL(evt_npc_entry, STRING("batten2"), STRING("c_mb_robo_2a"))
    CALL(evt_npc_set_anim, STRING("batten2"), STRING("J_2"))
    CALL(evt_npc_flag_onoff, 1, STRING("batten2"), 64)
    CALL(evt_end_npc_silhouette_on, STRING("batten2"))
    CALL(evt_npc_set_rotate, STRING("batten2"), 180, 0, 0)
    CALL(evt_sub_intpl_msec_init, 11, 4000, -4000, 4000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("batten2"), 100, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("batten2"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_atomic_evt)
    CALL(evt_npc_entry, STRING("atomic"), STRING("c_atmic_trs"))
    CALL(evt_npc_set_anim, STRING("atomic"), STRING("W_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("atomic"), 32832)
    CALL(evt_end_npc_silhouette_on, STRING("atomic"))
    CALL(evt_npc_set_rotate, STRING("atomic"), 0, 180, 0)
    CALL(evt_npc_set_scale, STRING("atomic"), FLOAT(0.7998047f), FLOAT(0.7998047f), FLOAT(0.7998047f))
    CALL(evt_sub_intpl_msec_init, 11, -5000, 5000, 6000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("atomic"), LW(0), 20, 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("atomic"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_korutesu_evt)
    CALL(evt_npc_entry, STRING("korutesu"), STRING("c_korutesu1"))
    CALL(evt_npc_set_anim, STRING("korutesu"), STRING("KRT_S_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("korutesu"), 32832)
    CALL(evt_npc_set_ry, STRING("korutesu"), 90)
    CALL(evt_end_npc_silhouette_on, STRING("korutesu"))
    CALL(evt_sub_intpl_msec_init, 11, -10000, -2000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("korutesu"), LW(0), -150, 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_sub_intpl_msec_init, 11, -2000, -10000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("korutesu"), LW(0), -150, 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("korutesu"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_para_evt)
    WAIT_MS(1000)
    CALL(evt_npc_entry, STRING("para"), STRING("c_bom_sen"))
    CALL(evt_npc_set_anim, STRING("para"), STRING("S_2"))
    CALL(evt_npc_flag_onoff, 1, STRING("para"), 32832)
    CALL(evt_end_npc_silhouette_on, STRING("para"))
    CALL(evt_sub_intpl_msec_init, 11, 3000, -3000, 6000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("para"), 50, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("para"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_batten_evt)
    CALL(evt_npc_entry, STRING("batten"), STRING("c_mb_robo_1a"))
    CALL(evt_npc_set_anim, STRING("batten"), STRING("W_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("batten"), 64)
    CALL(evt_end_npc_silhouette_on, STRING("batten"))
    CALL(evt_npc_set_scale, STRING("batten"), FLOAT(0.5996094f), FLOAT(0.5996094f), FLOAT(0.5996094f))
    CALL(evt_sub_intpl_msec_init, 11, 10000, 500, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("batten"), LW(0), -70, 200)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_sub_intpl_msec_init, 11, 500, 10000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("batten"), LW(0), -70, 200)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("batten"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_queen_evt)
    WAIT_MS(7000)
    CALL(evt_npc_entry, STRING("queen"), STRING("c_q_kage_m"))
    CALL(evt_npc_set_anim, STRING("queen"), STRING("KRT_Z_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("queen"), 64)
    CALL(evt_end_npc_silhouette_on, STRING("queen"))
    CALL(evt_npc_set_scale, STRING("queen"), FLOAT(0.5f), FLOAT(0.5f), FLOAT(0.5f))
    CALL(evt_npc_set_ry, STRING("queen"), 90)
    CALL(evt_sub_intpl_msec_init, 11, -10000, -1000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("queen"), LW(0), -20, 200)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_sub_intpl_msec_init, 11, -1000, -10000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("queen"), LW(0), -20, 200)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("queen"))
    RETURN
EVT_END()

const char* end_moa_ext_anim = "c_n_moa_a";

const char* end_moa_ext_pose = "S_1";

USER_FUNC(evt_end_image_disp_entry) {
    dispEntry(CAMERA_3D, 0.0f, 7, end_image_disp, NULL);
    return 0;
}

USER_FUNC(evt_end_image_set_alpha) {
    end_wp->alpha = evtGetValue(event, event->args[0]);
    return 2;
}

USER_FUNC(evt_end_image_set_overlay_alpha) {
    end_wp->alpha2 = evtGetValue(event, event->args[0]);
    return 2;
}

USER_FUNC(evt_end_image_set_mode) {
    end_wp->mode = evtGetValue(event, event->args[0]);
    return 2;
}

USER_FUNC(evt_end_image_set_blend) {
    end_wp->tevAlpha = evtGetValue(event, event->args[0]);
    return 2;
}

USER_FUNC(evt_end_staff_roll_msg_main) {
    s32 state = event->userdata[0];
    s32 idx = event->userdata[1];
    s32 i;
    s32 x;
    s32 y;
    s32 wx;
    s32 wy;
    s32 n;
    const char* msg;
    char buf1[0x400];
    char buf2[0x400];

    if (isFirstCall) {
        state = 0;
        idx = 0;
        for (i = 0; i < 3; i++) {
            end_wp->win1[i] = -1;
        }
        for (i = 0; i < 3; i++) {
            end_wp->win2[i] = -1;
        }
        evtSetValue(event, GW(10), 0);
        end_wp->evt = evtEntry(end_image_main_evt, 0, 0);
    }
    end_wp->pos = end_staff_roll_msg_tbl[idx].pos;
    switch (end_wp->pos) {
        case 0:
            x = -274;
            y = -24;
            break;
        case 1:
            x = 274;
            y = 250;
            break;
        case 2:
            x = 274;
            y = -24;
            break;
        case 3:
            x = -274;
            y = 250;
            break;
        case 4:
            x = 0;
            y = -60;
            break;
    }
    switch (state) {
        case 0:
            for (i = 0; i < 3; i++) {
                if (end_staff_roll_msg_tbl[idx].msg1[i] != NULL) {
                    msg = msgSearch(end_staff_roll_msg_tbl[idx].msg1[i]);
                    wx = x;
                    wy = y - i * 22 - 4;
                    if (x == 0) {
                        wx = x - 0.8f * FontGetMessageWidth(msg) / 2.0f;
                    } else if (x > 0) {
                        wx = x - 0.8f * FontGetMessageWidth(msg);
                    }
                    sprintf(buf1, "<clear><dkey><wpos %d %d 400 200><col f0f0f0ff><speed 0><scale 0.8>%s</scale></speed></col><wait 500></dkey><once_stop>", wx - 62, wy, msg);
                    end_wp->win1[i] = msgWindow_Entry(buf1, 0, 0);
                }
            }
            for (i = 0; i < 3; i++) {
                if (end_staff_roll_msg_tbl[idx].msg2[i] != NULL) {
                    msg = msgSearch(end_staff_roll_msg_tbl[idx].msg2[i]);
                    wx = x;
                    wy = y - i * 24 - 50;
                    if (x == 0) {
                        wx = x - FontGetMessageWidth(msg) / 2.0f;
                    } else if (x > 0) {
                        wx = x - FontGetMessageWidth(msg);
                    }
                    sprintf(buf2, "<clear><dkey><wpos %d %d 400 200><col f0f0f0ff><speed 0>%s</speed></col><wait 5200></dkey><once_stop>", wx - 62, wy, msg);
                    end_wp->win2[i] = msgWindow_Entry(buf2, 0, 0);
                } else {
                    end_wp->win2[i] = -1;
                }
            }
            evtSetValue(event, GW(10), 1);
            evtSetValue(event, GW(11), end_staff_roll_msg_tbl[idx].type);
            evtSetValue(event, GW(12), end_staff_roll_msg_tbl[idx].pos);
            state++;
            break;
        case 1:
            for (i = 0; i < 3; i++) {
                if (end_wp->win1[i] != -1 && windowCheckID(end_wp->win1[i])) {
                    break;
                }
            }
            if (i < 3) {
                break;
            }
            for (i = 0; i < 3; i++) {
                if (end_wp->win2[i] != -1 && windowCheckID(end_wp->win2[i])) {
                    break;
                }
            }
            if (i < 3) {
                break;
            }
            state++;
            break;
        case 2:
            n = idx + 1;
            if (n < ARRAY_SIZE(end_staff_roll_msg_tbl)) {
                for (i = 0; i < 3; i++) {
                    if (end_staff_roll_msg_tbl[n].msg1[i] != NULL) {
                        break;
                    }
                }
            } else {
                i = 0;
            }
            if (i < 3) {
                for (i = 0; i < 3; i++) {
                    if (end_wp->win1[i] != -1) {
                        msgWindow_Continue(end_wp->win1[i]);
                    }
                }
            }
            for (i = 0; i < 3; i++) {
                if (end_wp->win2[i] != -1) {
                    msgWindow_Continue(end_wp->win2[i]);
                }
            }
            evtSetValue(event, GW(10), 2);
            if (n < ARRAY_SIZE(end_staff_roll_msg_tbl)) {
                state++;
            } else {
                state += 2;
            }
            break;
        case 3:
            if (evtGetValue(event, GW(10)) == 0) {
                state = 0;
                idx++;
            }
            break;
        case 4:
            return 2;
    }
    event->userdata[0] = state;
    event->userdata[1] = idx;
    return 0;
}

USER_FUNC(evt_end_staff_roll_init) {
    BOOL inBattle = gp->inBattle ? TRUE : FALSE;
    s32 i;

    if (isFirstCall) {
        end_wp->tpl = NULL;
        end_wp->alpha = 0;
        end_wp->mode = 0;
        end_wp->pos = 0;
        psndSetFlag(4);
#ifndef VERSION_G8ME01
        gp->language = 1;
        JUTFont_Free();
        JUTFontSetup(1);
#endif
    }
    if ((u32)fileAsyncf(4, NULL, "%s/ending.tpl", getMarioStDvdRoot()) == 0) {
        return 0;
    }
    if (end_wp->tpl == NULL) {
        end_wp->tpl = fileAllocf(4, "%s/ending.tpl", getMarioStDvdRoot());
    }
    for (i = 0; i < 5; i++) {
        if (!animGroupBaseAsync(R_credits_npc_info[i].agb, inBattle, NULL)) {
            return 0;
        }
    }
    for (i = 0; i < 5; i++) {
    }
    return 2;
}

#pragma dont_inline on
void end_staff_roll_exit(void) {
    fileFree(end_wp->tpl);
    psndClearFlag(4);
#ifndef VERSION_G8ME01
    gp->language = 0;
    JUTFont_Free();
    JUTFontSetup(0);
#endif
}
#pragma dont_inline reset

USER_FUNC(evt_end_font_restore) {
#ifndef VERSION_G8ME01
    gp->language = 0;
    JUTFont_Free();
    JUTFontSetup(0);
#endif
    psndClearFlag(4);
    return 2;
}

USER_FUNC(end_bgm_wait_stop) {
    psndSetFlag(0x1000);
    return 2;
}

USER_FUNC(evt_end_npc_silhouette_on) {
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));

    mapSetTevCallback(1, end_silhouette_tev);
    animPoseSetMaterialFlagOn(npc->poseId, 0x1000000);
    animPoseSetGXFunc(npc->poseId, end_silhouette_rendermode, 0);
    return 2;
}

f32 end_randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

void end_ext_moamoa_init(void) {
    ExtdrvPose* pose = extGetPosePtr();
    EndMoa* elem = end_moa_tbl;
    s32 i;
    Vec v;

    for (i = 0; i < extGetPoseNum(); i++, pose++, elem++) {
        pose->flags |= 5;
        PSMTXIdentity(*pose->mtx);
        v = (Vec){0.0f, 0.0f, 0.0f};
        v.x = end_randf(-10.0f, 10.0f);
        v.y = end_randf(-10.0f, 10.0f);
        v.z = end_randf(-10.0f, 10.0f);
        elem->pos = v;
        elem->chain = i % g_moamoa_chain_num;
        elem->idx = end_moa_chain_count[elem->chain]++;
    }
}

void end_ext_moamoa_main(void) {
    NpcEntry* npc = npcNameToPtr("\203\202\203A\203\202\203A");
    Vec* wpos = *animPoseGetAnimPosePtr(npc->poseId)->worldPositions;
    s32 i;
    ExtdrvPose* pose;
    EndMoa* elem;
    AnimationPose* ap;
    Vec* p;
    s32 n;
    Vec* a;
    Vec* b;
    f32 t;
    f32 s;
    Mtx m1;
    Mtx m3;
    Mtx m2;
    Vec v;
    Vec w;

    if (wpos == NULL) {
        return;
    }
    pose = extGetPosePtr();
    elem = end_moa_tbl;
    for (i = 0; i < extGetPoseNum(); i++, elem++, pose++) {
        a = &wpos[end_moa_chain_group[elem->chain][0]];
        b = &wpos[end_moa_chain_group[elem->chain][1]];
        t = (f32)elem->idx / (f32)end_moa_chain_count[elem->chain];
        s = 1.0f - t;
        if (a->y == -1000.0f || b->y == -1000.0f) {
            v = (Vec){0.0f, -1000.0f, 0.0f};
            pose->flags &= ~1;
        } else {
            PSVECScale(a, &v, t);
            PSVECScale(b, &w, s);
            PSVECAdd(&v, &w, &v);
            pose->flags |= 1;
        }
        PSMTXTrans(m1, v.x, v.y, v.z);
        PSMTXScale(m2, npc->scale.x, npc->scale.y, npc->scale.z);
        PSMTXConcat(m1, m2, m1);
        PSMTXTrans(m3, elem->pos.x, elem->pos.y, elem->pos.z);
        PSMTXConcat(m1, m3, *pose->mtx);
        pose->texId = (gp->retraceCount + i) % 40;
    }
    ap = animPoseGetAnimPosePtr(npc->poseId);
    n = ap->numWorldPositions;
    p = *ap->worldPositions;
    for (i = 0; i < n; i++) {
        p[i] = (Vec){0.0f, -1000.0f, 0.0f};
    }
}

void end_ext_disp(CameraId cameraId, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
    end_silhouette_rendermode(0);
    extDraw();
    extLoadTextureExit();
}

void end_ext_moamoa_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, end_ext_disp, NULL);
}

USER_FUNC(evt_end_moamoa_init) {
    NpcEntry* npc = npcNameToPtr("\203\202\203A\203\202\203A");
    AnimationPose* ap;
    Vec* p;
    s32 n;
    s32 i;

    end_moa_tbl = _mapAlloc(sizeof(EndMoa) * 300);
    memset(end_moa_tbl, 0, sizeof(EndMoa) * 300);
    animPoseWorldPositionEvalOn(npc->poseId);
    end_moa_ext_data = _mapAlloc(sizeof(EndExtData) * 41);
    memset(end_moa_ext_data, 0, sizeof(EndExtData) * 41);
    for (i = 0; i < 40; i++) {
        end_moa_ext_data[i].anim = end_moa_ext_anim;
        end_moa_ext_data[i].pose = end_moa_ext_pose;
        end_moa_ext_data[i].speed = i;
    }
    evtSetValue(event, LW(10), (EvtWord)end_moa_ext_data);
    ap = animPoseGetAnimPosePtr(npc->poseId);
    n = ap->numWorldPositions;
    p = *ap->worldPositions;
    for (i = 0; i < n; i++) {
        p[i] = (Vec){0.0f, -1000.0f, 0.0f};
    }
    end_moa_chain_count = _mapAlloc(g_moamoa_chain_num * sizeof(s32));
    memset(end_moa_chain_count, 0, g_moamoa_chain_num * sizeof(s32));
    end_moa_chain_group = _mapAlloc(g_moamoa_chain_num * sizeof(s32[2]));
    memset(end_moa_chain_group, 0, g_moamoa_chain_num * sizeof(s32[2]));
    for (i = 0; i < g_moamoa_chain_num; i++) {
        end_moa_chain_group[i][0] = animPoseGetGroupIdx(npc->poseId, g_moamoa_chain_tbl[i][0]);
        end_moa_chain_group[i][1] = animPoseGetGroupIdx(npc->poseId, g_moamoa_chain_tbl[i][1]);
    }
    return 2;
}

USER_FUNC(evt_end_moamoa_exit) {
    NpcEntry* npc = npcNameToPtr("\203\202\203A\203\202\203A");

    L_animPoseWorldPositionEvalOff(npc->poseId);
    _mapFree(end_moa_tbl);
    extReset();
    _mapFree(end_moa_chain_group);
    _mapFree(end_moa_chain_count);
    return 2;
}


EVT_BEGIN(end_credit_moamoa_evt)
    CALL(evt_npc_entry, STRING("モアモアダミー"), STRING("c_n_moa_a"))
    CALL(evt_npc_set_position, STRING("モアモアダミー"), 0, -1000, 0)
    CALL(evt_end_npc_silhouette_on, STRING("モアモアダミー"))
    CALL(evt_npc_entry, STRING("モアモア"), STRING("c_g_moamoa"))
    CALL(evt_npc_set_anim, STRING("モアモア"), STRING("S_3"))
    CALL(evt_npc_set_position, STRING("モアモア"), 0, -1000, 0)
    CALL(evt_npc_set_scale, STRING("モアモア"), FLOAT(0.5f), FLOAT(0.5f), FLOAT(0.5f))
    CALL(evt_end_moamoa_init)
    CALL(evt_ext_entry, 300, LW(10), PTR(end_ext_moamoa_init), PTR(end_ext_moamoa_main), PTR(end_ext_moamoa_dispent))
    CALL(evt_sub_intpl_msec_init, 11, -10000, -1000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("モアモア"), 0, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_sub_intpl_msec_init, 11, -1000, -10000, 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_npc_set_position, STRING("モアモア"), 0, LW(0), 100)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_end_moamoa_exit)
    CALL(evt_npc_delete, STRING("モアモアダミー"))
    CALL(evt_npc_delete, STRING("モアモア"))
    RETURN
EVT_END()

f32 fd_end_00_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_end_00_neg_one_382 = -1.0f;

f32 fd_end_00___local_PI_383 = 3.1415927f;

f32 fd_end_00___local_3PIO2_385 = 4.712389f;

f32 fd_end_00_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_end_00___local_PIO2_384 = 1.5707964f;

f32 fd_end_00_one_381 = 1.0f;

f32 fd_end_00_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_end_00_neg_one_365 = -1.0f;

f32 fd_end_00___local_PI_366 = 3.1415927f;

f32 fd_end_00___local_3PIO2_368 = 4.712389f;

f32 fd_end_00_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_end_00___local_PIO2_367 = 1.5707964f;

f32 fd_end_00_one_364 = 1.0f;
