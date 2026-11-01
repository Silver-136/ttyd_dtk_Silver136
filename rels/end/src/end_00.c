// end_00 (エンディング, ending): the staff roll with the parade of characters, the Smorg (moamoa) silhouettes and the
// closing pictures before the return to the title.

#include "version.h"
#include "driver/fadedrv.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "libc_decls.h"
#include "libc/stdlib.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_ext.h"
#include "evt/evt_fade.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "dolphin/exi.h"
#include <dolphin/pad.h>

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
#include "driver/seqdrv.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
typedef struct EndMoa {
    Vec pos;
    s32 chain;
    s32 idx;
} EndMoa;
typedef struct EndWork {
    FileEntry* tpl;
#if VERSION > VERSION_DEMO_US
    s32 mode;
#endif
    s32 alpha;
#if VERSION <= VERSION_DEMO_US
    s32 npcLoadNum;     // credit NPCs whose animation groups have been requested so far
    s32 npcLoadRequest;
#else
    s32 alpha2;
    s32 tevAlpha;
#endif
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
typedef struct CreditNpc {
    const char* agb;  // animation group of the NPC model
    const char* anim;
    EvtWord* evt;     // started instead of the normal walk (evt_end_credit_npc_main)
} CreditNpc;
typedef struct CreditActor {
    s32 state;
    s32 poseId;
    f32 angle;
    f32 limit;
    s32 pos;
    Mtx mtx;
} CreditActor;
EndWork end_work;
#if VERSION > VERSION_DEMO_US
s32 (*end_moa_chain_group)[2];
s32* end_moa_chain_count;
ExtTexInfo* end_moa_ext_data;
EndMoa* end_moa_tbl;
#endif
extern EndWork* end_wp;
#if VERSION <= VERSION_DEMO_US
extern CreditMsg end_staff_roll_msg_tbl[];
extern CreditNpc R_credits_npc_info[151];
#else
extern CreditMsg end_staff_roll_msg_tbl[50];
extern CreditNpc R_credits_npc_info[152];
#endif
static inline f32 tanf(f32 x) { return tan(x); }
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
USER_FUNC(end_bgm_wait_stop);
void end_staff_roll_exit(void);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

#if VERSION <= VERSION_DEMO_US
extern EvtWord end_credit_npc_parade[];
// US: "THE END" texture per language (indexed by gp->language)
extern EvtWord end_image_main_evt[];
EvtWord end_return_title_evt[];
extern EvtWord end_00_init_evt[];
EvtWord end_credit_gesso_evt[48];
extern EvtWord end_credit_gonbaba_evt[];
extern EvtWord end_credit_batten2_evt[];
extern EvtWord end_credit_atomic_evt[];
extern EvtWord end_credit_korutesu_evt[];
extern EvtWord end_credit_para_evt[];
extern EvtWord end_credit_batten_evt[];
EvtWord end_credit_queen_evt[];
#else
EvtWord end_credit_npc_parade[44];
EvtWord end_image_blend_loop[123];
// US: "THE END" texture per language (indexed by gp->language)
EvtWord end_image_main_evt[187];
EvtWord end_return_title_evt[];
EvtWord end_00_init_evt[63];
EvtWord end_credit_gesso_evt[48];
EvtWord end_credit_gonbaba_evt[89];
EvtWord end_credit_batten2_evt[54];
EvtWord end_credit_atomic_evt[60];
EvtWord end_credit_korutesu_evt[81];
EvtWord end_credit_para_evt[50];
EvtWord end_credit_batten_evt[83];
EvtWord end_credit_queen_evt[];
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



/**
 * REL prolog of the ending module (the releases keep it in end_00.c):
 * runs the static constructors, registers the init event of end_00 and, in the US and later versions,
 * switches off the 2D camera's letterbox bars so the credits fill the screen.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("end_00", end_00_init_evt);
#if VERSION >= VERSION_US
    camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_OFF;
    camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX;
    camGetPtr(CAMERA_2D)->flags |= CAMERA_FLAG_LETTER_BOX_INSTANT;
#endif
}

/**
 * REL epilog of the ending module: runs the static destructors, ends the staff roll (end_staff_roll_exit) and,
 * in the US and later versions, lets the 2D letterbox bars come back.
 */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    end_staff_roll_exit();
#if VERSION >= VERSION_US
    camGetPtr(CAMERA_2D)->flags &= ~CAMERA_FLAG_LETTER_BOX_OFF;
#endif
}

/** Target of unresolved relocations; the module has no code to run for them, so it is empty. */
void _unresolved(void) {
}
#endif

/**
 * Map TEV callback that makes a model draw as a flat black silhouette:
 * it replaces the colour of the stage before the callback's stage with black.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00002508, JP end .text+0x2444. See docs/NAME_PROVENANCE.md.
void end_silhouette_tev(MapTevCallbackInfo* info) {
    GXSetTevColorIn(info->tevStage - 1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
}

/**
 * Animation GX callback used with the silhouette:
 * sets opaque drawing with the alpha test at 0x80 and depth writes on, so the silhouettes have clean edges,
 * and records the display mode for the animation driver.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00002494, JP end .text+0x23D0. See docs/NAME_PROVENANCE.md.
void end_silhouette_rendermode(s32 wXluStage) {
    animGetPtr()->currDispMode = wXluStage;
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_OR, GX_NEVER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
}

/**
 * Draw callback for one parade character (CreditActor):
 * draws its pose three times as a black silhouette (three animation draw passes)
 * unless the roll has ended (GF(10) set).
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_000023ac, JP end .text+0x22E8. See docs/NAME_PROVENANCE.md.
void end_credit_npc_disp(CameraId cameraId, void* param) {
    CreditActor* actor = param;

    if (evtGetValue(NULL, GF(10)) == 0) {
        mapSetTevCallback(1, end_silhouette_tev);
        animPoseSetMaterialFlagOn(actor->poseId, MAP_OBJ_TEV_CALLBACK(1));
        animPoseSetGXFunc(actor->poseId, end_silhouette_rendermode, 0);
        animPoseDrawMtx(actor->poseId, actor->mtx, 1, 0.0f, 2.0f);
        animPoseDrawMtx(actor->poseId, actor->mtx, 2, 0.0f, 2.0f);
        animPoseDrawMtx(actor->poseId, actor->mtx, 3, 0.0f, 2.0f);
        animPoseSetMaterialFlagOff(actor->poseId, MAP_OBJ_TEV_CALLBACK(1));
    }
}

/**
 * Script helper: for credit NPC number args[0] writes into args[1] how long the parade script waits
 * before sending the next one: 0 for a character with its own script,
 * 1 for a normal silhouette (about 2 seconds), and in the Japanese release
 * and later 2 for the Gazigazi (a longer 6 second wait). Out-of-range numbers give 0.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_000022e8, JP end .text+0x2224. See docs/NAME_PROVENANCE.md.
USER_FUNC(evt_end_credit_npc_get_wait_type) {
    EvtWord* args = event->args;
    u32 id = evtGetValue(event, args[0]);

    if (id >= ARRAY_SIZE(R_credits_npc_info)) {
        evtSetValue(event, args[1], 0);
        return 2;
    }
    if (R_credits_npc_info[id].evt != NULL) {
        evtSetValue(event, args[1], 0);
#if VERSION > VERSION_DEMO_US
    } else if (strcmp(R_credits_npc_info[id].agb, "c_gazigazi") == 0) {
        evtSetValue(event, args[1], 2);
#endif
    } else {
        evtSetValue(event, args[1], 1);
    }
    return 2;
}

/** Pointer to the ending's work data (end_work), used by the ending callbacks. */
EndWork* end_wp = &end_work;

// PAL credits: the European staff entries
CreditMsg end_staff_roll_msg_tbl[] = {
#if VERSION <= VERSION_DEMO_US
    0, 0, 0, 0, NULL, 0, "a00", NULL,
    0, 0, 0, 1, 0, "a01", 0, "b00",
    NULL, 0, 0, 0, 2, 0, "a02", 0,
    "b01", NULL, 0, 0, 0, 3, 0, NULL,
    0, "b02", "b03", 0, 0, 0, 4, 0,
    "a03", 0, "b04", NULL, 0, 0, 0, 5,
    0, "a04", 0, "b05", NULL, 0, 0, 0,
    6, 0, "a05", 0, "b06", NULL, 0, 0,
    0, 7, 0, "a06", 0, "b07", NULL, 0,
    0, 0, 8, 0, "a07", 0, "b08", NULL,
    0, 0, 0, 9, 0, "a08", 0, "b09",
    NULL, 0, 0, 0, 10, 0, NULL, 0,
    "b10", "b11", 0, 0, 1, 11, 0, "a09",
    0, "b12", NULL, 0, 0, 1, 12, 0,
    NULL, 0, "b13", NULL, 0, 0, 1, 13,
    0, NULL, 0, "b14", NULL, 0, 0, 1,
    14, 0, NULL, 0, "b15", "b16", 0, 0,
    1, 15, 0, "a10", 0, "b17", NULL, 0,
    0, 1, 16, 0, "a11", 0, "b18", NULL,
    0, 0, 1, 17, 0, "a12", 0, "b19",
    NULL, 0, 0, 1, 18, 0, "a13", 0,
    "b20", NULL, 0, 0, 1, 19, 0, NULL,
    0, "b21", NULL, 0, 0, 2, 20, 0,
    "a14", 0, "b22", NULL, 0, 0, 2, 21,
    0, NULL, 0, "b23", NULL, 0, 0, 2,
    22, 0, NULL, 0, "b24", NULL, 0, 0,
    2, 23, 0, NULL, 0, "b25", NULL, 0,
    0, 2, 24, 0, "a15", 0, "b26", NULL,
    0, 0, 2, 25, 0, NULL, 0, "b27",
    NULL, 0, 0, 2, 26, 0, "a16", 0,
    "b28", NULL, 0, 0, 2, 27, 0, "a17",
    0, "b29", NULL, 0, 0, 2, 28, 0,
    "a18", 0, "b30", NULL, 0, 0, 2, 29,
    0, NULL, 0, "b31", "b32", 0, 0, 3,
    30, 0, "a19", 0, "b33", NULL, 0, 0,
    3, 31, 0, "a20", 0, "b34", "b35", "b36",
    0, 3, 32, 0, "a21", 0, "b37", "b38",
    0, 0, 3, 33, 0, NULL, 0, "b39",
    "b40", 0, 0, 3, 34, 0, NULL, 0,
    "b41", "b42", 0, 0, 3, 35, 0, NULL,
    0, "b43", "b44", 0, 0, 3, 36, 0,
    NULL, 0, "b45", NULL, 0, 0, 3, 37,
    0, NULL, 0, "b46", "b47", 0, 0, 3,
    38, 0, "a22", 0, "b48", "b49", 0, 0,
    3, 39, 0, NULL, 0, "b50", "b51", 0,
    0, 0, 40, 0, "a23", 0, "b52", "b53",
    0, 0, 0, 41, 0, "a24", 0, "b54",
    NULL, 0, 0, 0, 42, 0, "a25", 0,
    "b55", NULL, 0, 0, 0, 43, 0, "a26",
    0, "b56", NULL, 0, 0, 0, 44, 0,
    "a27", 0, "b57", NULL, 0, 0, 0, 45,
    0, "a28", 0, "b58", "b59", 0, 0, 0,
    46, 0, NULL, 0, "b60", "b61", 0, 0,
    0, 47, 0, "a29", 0, "b62", "b63", 0,
    0, 0, 48, 0, "a30", 0, "b64", NULL,
    0, 0, 4, 49, "c00", NULL, 0, "c01",
    NULL, 0, 0, 4, 50, "c02", 0, 0,
    "c02", 0, 0,
#else
    {0, 0, 0, {NULL, NULL, NULL}, {"a00", NULL, NULL}},
    {0, 0, 1, {NULL, "a01", NULL}, {"b00", NULL, NULL}},
    {0, 0, 2, {NULL, "a02", NULL}, {"b01", NULL, NULL}},
    {0, 0, 3, {NULL, NULL, NULL}, {"b02", "b03", NULL}},
    {0, 0, 4, {NULL, "a03", NULL}, {"b04", NULL, NULL}},
    {0, 0, 5, {NULL, "a04", NULL}, {"b05", NULL, NULL}},
    {0, 0, 6, {NULL, "a05", NULL}, {"b06", NULL, NULL}},
    {0, 0, 7, {NULL, "a06", NULL}, {"b07", NULL, NULL}},
    {0, 0, 8, {NULL, "a07", NULL}, {"b08", NULL, NULL}},
#if VERSION == VERSION_EU
    {0, 0, 9, {NULL, "a08", NULL}, {"b09", "b10", "b11"}},
    {0, 0, 10, {NULL, "a09", NULL}, {"b12", NULL, NULL}},
    {0, 0, 11, {NULL, NULL, NULL}, {"b13", NULL, NULL}},
    {0, 0, 12, {NULL, NULL, NULL}, {"b14", NULL, NULL}},
    {0, 0, 13, {NULL, NULL, NULL}, {"b15", "b16", NULL}},
    {0, 1, 14, {NULL, "a10", NULL}, {"b17", NULL, NULL}},
    {0, 1, 15, {NULL, "a11", NULL}, {"b18", NULL, NULL}},
    {0, 1, 16, {NULL, "a12", NULL}, {"b19", NULL, NULL}},
    {0, 1, 17, {NULL, "a13", NULL}, {"b20", "b21", NULL}},
    {0, 1, 18, {NULL, "a14", NULL}, {"b22", "b23", NULL}},
    {0, 1, 19, {NULL, NULL, NULL}, {"b24", "b25", NULL}},
    {0, 1, 20, {NULL, "a15", NULL}, {"b26", "b27", NULL}},
    {0, 1, 21, {NULL, "a17", NULL}, {"b28", NULL, NULL}},
    {0, 1, 22, {NULL, NULL, NULL}, {"b29", NULL, NULL}},
    {0, 1, 23, {NULL, "a18", NULL}, {"b30", NULL, NULL}},
    {0, 1, 24, {NULL, NULL, NULL}, {"b31", "b32", NULL}},
    {0, 2, 25, {NULL, "a19", NULL}, {"b33", NULL, NULL}},
    {0, 2, 26, {NULL, "a20", NULL}, {"b34", "b35", "b36"}},
    {0, 2, 27, {NULL, "a32", NULL}, {"b65", "b66", NULL}},
    {0, 2, 28, {NULL, "a33", NULL}, {"b67", "b68", NULL}},
    {0, 2, 29, {NULL, "a34", NULL}, {"b69", "b70", NULL}},
    {0, 2, 30, {NULL, "a35", NULL}, {"b71", "b72", NULL}},
    {0, 3, 31, {NULL, "a31", NULL}, {"b73", "b74", "b75"}},
    {0, 3, 32, {NULL, NULL, NULL}, {"b76", "b77", "b81"}},
    {0, 3, 33, {NULL, "a36", NULL}, {"b79", NULL, NULL}},
    {0, 3, 34, {NULL, "a21", NULL}, {"b37", "b38", "b92"}},
    {0, 3, 35, {NULL, NULL, NULL}, {"b39", "b40", "b82"}},
    {0, 3, 36, {NULL, NULL, NULL}, {"b46", "b47", "b83"}},
    {0, 3, 37, {NULL, "a22", NULL}, {"b48", "b49", NULL}},
    {0, 3, 38, {NULL, NULL, NULL}, {"b50", "b51", NULL}},
    {0, 3, 39, {NULL, "a23", NULL}, {"b52", "b53", "b84"}},
    {0, 3, 40, {NULL, "a24", NULL}, {"b85", NULL, NULL}},
    {0, 3, 41, {NULL, "a25", NULL}, {"b55", "b86", "b87"}},
    {0, 0, 42, {NULL, "a38", NULL}, {"b90", "b91", NULL}},
    {0, 0, 43, {NULL, "a39", NULL}, {"b56", NULL, NULL}},
#else
    {0, 0, 9, {NULL, "a08", NULL}, {"b09", NULL, NULL}},
    {0, 0, 10, {NULL, NULL, NULL}, {"b10", "b11", NULL}},
    {0, 1, 11, {NULL, "a09", NULL}, {"b12", NULL, NULL}},
    {0, 1, 12, {NULL, NULL, NULL}, {"b13", NULL, NULL}},
    {0, 1, 13, {NULL, NULL, NULL}, {"b14", NULL, NULL}},
    {0, 1, 14, {NULL, NULL, NULL}, {"b15", "b16", NULL}},
    {0, 1, 15, {NULL, "a10", NULL}, {"b17", NULL, NULL}},
    {0, 1, 16, {NULL, "a11", NULL}, {"b18", NULL, NULL}},
    {0, 1, 17, {NULL, "a12", NULL}, {"b19", NULL, NULL}},
    {0, 1, 18, {NULL, "a13", NULL}, {"b20", NULL, NULL}},
    {0, 1, 19, {NULL, NULL, NULL}, {"b21", NULL, NULL}},
    {0, 2, 20, {NULL, "a14", NULL}, {"b22", NULL, NULL}},
    {0, 2, 21, {NULL, NULL, NULL}, {"b23", NULL, NULL}},
    {0, 2, 22, {NULL, NULL, NULL}, {"b24", NULL, NULL}},
    {0, 2, 23, {NULL, NULL, NULL}, {"b25", NULL, NULL}},
    {0, 2, 24, {NULL, "a15", NULL}, {"b26", NULL, NULL}},
    {0, 2, 25, {NULL, NULL, NULL}, {"b27", NULL, NULL}},
    {0, 2, 26, {NULL, "a17", NULL}, {"b28", NULL, NULL}},
    {0, 2, 27, {NULL, NULL, NULL}, {"b29", NULL, NULL}},
    {0, 2, 28, {NULL, "a18", NULL}, {"b30", NULL, NULL}},
    {0, 2, 29, {NULL, NULL, NULL}, {"b31", "b32", NULL}},
    {0, 3, 30, {NULL, "a19", NULL}, {"b33", NULL, NULL}},
    {0, 3, 31, {NULL, "a20", NULL}, {"b34", "b35", "b36"}},
#if VERSION >= VERSION_US
    {0, 3, 32, {NULL, "a31", NULL}, {"b73", "b74", "b75"}},
    {0, 3, 33, {NULL, NULL, NULL}, {"b76", "b77", "b92"}},
    {0, 3, 34, {NULL, "a36", NULL}, {"b78", "b79", NULL}},
    {0, 3, 35, {NULL, "a37", NULL}, {"b80", "b81", NULL}},
    {0, 3, 36, {NULL, "a21", NULL}, {"b37", "b38", NULL}},
    {0, 3, 37, {NULL, NULL, NULL}, {"b39", "b40", "b41"}},
    {0, 3, 38, {NULL, NULL, NULL}, {"b46", "b47", NULL}},
    {0, 3, 39, {NULL, "a22", NULL}, {"b48", "b49", NULL}},
    {0, 3, 40, {NULL, NULL, NULL}, {"b50", "b51", NULL}},
    {0, 0, 41, {NULL, "a23", NULL}, {"b52", "b53", "b84"}},
    {0, 0, 42, {NULL, "a25", NULL}, {"b55", "b86", NULL}},
    {0, 0, 43, {NULL, "a26", NULL}, {"b88", "b89", "b56"}},
#else
    {0, 3, 32, {NULL, "a21", NULL}, {"b37", "b38", NULL}},
    {0, 3, 33, {NULL, NULL, NULL}, {"b39", "b40", NULL}},
    {0, 3, 34, {NULL, NULL, NULL}, {"b41", "b42", NULL}},
    {0, 3, 35, {NULL, NULL, NULL}, {"b43", "b44", NULL}},
    {0, 3, 36, {NULL, NULL, NULL}, {"b45", NULL, NULL}},
    {0, 3, 37, {NULL, NULL, NULL}, {"b46", "b47", NULL}},
    {0, 3, 38, {NULL, "a22", NULL}, {"b48", "b49", NULL}},
    {0, 3, 39, {NULL, NULL, NULL}, {"b50", "b51", NULL}},
    {0, 0, 40, {NULL, "a23", NULL}, {"b52", "b53", NULL}},
    {0, 0, 41, {NULL, "a24", NULL}, {"b54", NULL, NULL}},
    {0, 0, 42, {NULL, "a25", NULL}, {"b55", NULL, NULL}},
    {0, 0, 43, {NULL, "a26", NULL}, {"b56", NULL, NULL}},
#endif
#endif
    {0, 0, 44, {NULL, "a27", NULL}, {"b57", NULL, NULL}},
    {0, 0, 45, {NULL, "a28", NULL}, {"b58", "b59", NULL}},
    {0, 0, 46, {NULL, NULL, NULL}, {"b60", "b61", NULL}},
    {0, 0, 47, {NULL, "a29", NULL}, {"b62", "b63", NULL}},
    {0, 0, 48, {NULL, "a30", NULL}, {"b64", NULL, NULL}},
    {0, 4, 49, {"c00", NULL, NULL}, {"c01", NULL, NULL}},
#endif
};

CreditNpc R_credits_npc_info[] = {
#if VERSION <= VERSION_DEMO_US
    "c_luigi", "W_1", 0, "c_pkr", "PKR_W_1", 0, "c_kuri_h", "W_1",
    0, "c_kino_ji", "W_1", 0, "c_cook", "W_1", 0, "c_deale",
    "W_1", 0, "c_levela", "W_1", 0, "c_uranai", "W_1", 0,
    "c_majinai", "W_1", 0, "c_botta", "W_1", 0, "MOBJ_BlackTreasureBox", "A_3",
    0, "c_mania", "MNA_W_1", 0, "c_joho_a", "W_1", 0, "c_joho_b",
    "W_1", 0, "c_gshop_m", "W_2", 0, "c_gshop_m", "W_1", 0,
    "c_pub_m", "W_1", 0, "c_roten", "W_1", 0, "c_tyusan", "CHU_W_1",
    0, "c_borodo_n", "W_1", 0, "c_bom_deb", "W_1", 0, "c_kuribo_n",
    "W_1", 0, "c_hannya", "W_1", 0, "c_hannya_n", "W_1", 0,
    "c_gesso", "GSO_S2_1", end_credit_gesso_evt, "c_pnk", "PNK_W_1", 0, "c_nokorin", "W_1",
    0, "c_nokopapa", "NKP_W_1", 0, "c_nokoson", "NKS_W_1", 0, "c_nokonoko",
    "NOK_W_1", 0, "c_kinopio", "KNP_W_1", 0, "c_kinopiko", "KNF_W_1", 0,
    "c_kokinopio", "CKP_W_1", 0, "c_flower", "W_1", 0, "c_kiza", "W_1",
    0, "c_kuribo", "KUR_W_1", 0, "c_kuribo", "TGK_W_1", 0, "c_kuribo",
    "PTK_W_1", 0, "c_nokoteki", "NKT_W_1", 0, "c_nokoteki", "PTP_W_1", 0,
    "c_sinnosuke", "SIS_W_1", 0, "c_togedaruma", "TGD_S_1", 0, "c_chorobon", "CBN_W_1",
    0, "c_karon_h", "KRN_W_1", 0, "c_gonbaba", "GNB_S_1", end_credit_gonbaba_evt, "c_windy",
    "PWD_W_1", 0, "c_pguide", "W_1", 0, "c_psister", "W_1", 0,
    "c_psoncho", "W_1", 0, "c_pshop", "W_1", 0, "c_puni", "W_1",
    0, "c_puni", "W_1", 0, "c_puni", "W_1", 0, "c_puni",
    "W_1", 0, "c_puni", "W_1", 0, "c_puni", "W_1", 0,
    "c_prival", "W_1", 0, "c_toge", "W_1", 0, "c_tchibi", "W_1",
    0, "c_sinemon", "SIN_W_1", 0, "c_paid", "PAI_W_1", 0, "c_mb_robo_1b",
    "MGN_W_1", end_credit_batten_evt, "c_m_boss", "W_1", 0, "c_m_buka", "W_1", 0,
    "c_m_waka", "W_1", 0, "c_m_ojo", "W_1", 0, "c_binbo", "W_1",
    0, "c_monban", "W_1", 0, "c_bteresa", "BTS_W_1", 0, "c_babyyoshi",
    "PYS_W_1", 0, "c_ksikowa", "W_1", 0, "c_k_chof", "W_1", 0,
    "c_gans", "W_1", 0, "c_bred_h", "W_1", 0, "c_report", "W_1",
    0, "c_hotdog", "W_1", 0, "c_shasho_g", "W_1", 0, "c_jugem_n",
    "W_1", 0, "c_togezo", "TGZ_W_1", 0, "c_heavy", "W_1", 0,
    "c_kamek", "KMK_W_1", 0, "c_burosu_h", "BRO_W_1", 0, "c_togenoko_b", "W_1",
    0, "c_koopa", "KPA_W_1", 0, "c_kamek_be", "W_1", 0, "c_majyorin",
    "MJR_W_1", 0, "c_maririn", "MRR_W_1", 0, "c_vivian", "PTR_W_1", 0,
    "c_f_son", "W_1", 0, "c_f_mura", "W_1", 0, "c_f_mama", "W_1",
    0, "c_f_child_a", "W_1", 0, "c_f_oji", "W_1", 0, "c_f_oba",
    "W_1", 0, "c_f_karas", "A_1", 0, "c_teresa", "W_1", 0,
    "c_atmic_trs", "W_1", 0, "c_ranpel", "W_1", 0, "c_f_owm", "A_1",
    0, "c_met", "MET_W_1", 0, "c_met", "TMT_W_1", 0, "c_pansy",
    "PAN_W_1", 0, "c_dance_r", "W_1", 0, "c_dance_p", "W_1", 0,
    "c_dance_k", "W_1", 0, "c_bomt_n", "W_1", 0, "c_marco", "W_1",
    0, "c_gara", "W_1", 0, "c_megane", "W_1", 0, "c_odo_k",
    "W_1", 0, "c_suifu_k", "W_1", 0, "c_suifu_b", "W_1", 0,
    "c_elmos", "W_1", 0, "c_kilr", "KIL_W_1", 0, "c_pakflwr", "PKF_S_1",
    0, "c_korutesu1", "KRT_S_1", end_credit_korutesu_evt, "c_tyutyu", "PCH_W_1", 0, "c_powan",
    "W_1", 0, "c_papab", "W_1", 0, "c_mamab", "W_1", 0,
    "c_konari", "W_1", 0, "c_unten", "W_1", 0, "c_shasho_r", "W_1",
    0, "c_smoko", "W_1", 0, "c_rose", "W_1", 0, "c_kinodebu",
    "W_1", 0, "c_ghost_k", "W_1", 0, "c_waitress", "W_1", 0,
    "c_puku_c", "W_1", 0, "c_kinopiko", "KNF_W_1", 0, "c_n_moa_a", "W_1",
    0, "c_met", "PMT_W_1", 0, "c_sanbo", "SNB_W_1", 0, "c_bomson",
    "W_1", 0, "c_bwhite", "W_1", 0, "c_bom_sen", "S_2", 0,
    "c_hito_b", "W_1", 0, "c_bom_n", "W_1", 0, "c_kmoon", "W_1",
    0, "c_baria", "W_1", 0, "c_mb_robo_2b", "MGN_W_1", end_credit_batten2_evt, "c_basabasa",
    "BSA_W_1", 0, "c_wanwan", "WAN_S_1A", 0, "c_maho", "MAH_W_1", 0,
    "c_zako_n", "W_1", 0, "c_zako_m", "W_1", 0, "c_shuryo", "W_1",
    0, "c_q_kage", "KRT_S_1", end_credit_queen_evt, "c_gazigazi", "GAZ_A_1", 0, "a_mario",
    "M_W_1", 0, "c_peach", "P_W_1", 0,
#else
    {"c_luigi", "W_1", NULL},
    {"c_pkr", "PKR_W_1", NULL},
    {"c_kuri_h", "W_1", NULL},
    {"c_kino_ji", "W_1", NULL},
    {"c_cook", "W_1", NULL},
    {"c_deale", "W_1", NULL},
    {"c_levela", "W_1", NULL},
    {"c_uranai", "W_1", NULL},
    {"c_majinai", "W_1", NULL},
    {"c_botta", "W_1", NULL},
    {"c_mania", "MNA_W_1", NULL},
    {"c_joho_a", "W_1", NULL},
    {"c_gesso", "GSO_S2_1", end_credit_gesso_evt},
    {"c_joho_b", "W_1", NULL},
    {"c_gshop_m", "W_2", NULL},
    {"c_gshop_m", "W_1", NULL},
    {"c_pub_m", "W_1", NULL},
    {"c_roten", "W_1", NULL},
    {"c_tyusan", "CHU_W_1", NULL},
    {"c_borodo_n", "W_1", NULL},
    {"c_bom_deb", "W_1", NULL},
    {"c_kuribo_n", "W_1", NULL},
    {"c_hannya", "W_1", NULL},
    {"c_hannya_n", "W_1", NULL},
    {"c_pnk", "PNK_W_1", NULL},
    {"c_nokorin", "W_1", NULL},
    {"c_nokopapa", "NKP_W_1", NULL},
    {"c_nokoson", "NKS_W_1", NULL},
    {"c_nokonoko", "NOK_W_1", NULL},
    {"c_kinopio", "KNP_W_1", NULL},
    {"c_kinopiko", "KNF_W_1", NULL},
    {"c_kokinopio", "CKP_W_1", NULL},
    {"c_gonbaba", "GNB_S_3", end_credit_gonbaba_evt},
    {"c_flower", "W_1", NULL},
    {"c_kiza", "W_1", NULL},
    {"c_kuribo", "KUR_W_1", NULL},
    {"c_kuribo", "TGK_W_1", NULL},
    {"c_kuribo", "PTK_W_1", NULL},
    {"c_nokoteki", "NKT_W_1", NULL},
    {"c_nokoteki", "PTP_W_1", NULL},
    {"c_sinnosuke", "SIS_W_1", NULL},
    {"c_togedaruma", "TGD_S_1", NULL},
    {"c_chorobon", "CBN_W_1", NULL},
    {"c_karon_h", "KRN_W_1", NULL},
    {"c_windy", "PWD_W_1", NULL},
    {"c_pguide", "W_1", NULL},
    {"c_psister", "W_1", NULL},
    {"c_psoncho", "W_1", NULL},
    {"c_pshop", "W_1", NULL},
    {"c_puni", "W_1", NULL},
    {"c_puni", "W_1", NULL},
    {"c_puni", "W_1", NULL},
    {"c_puni", "W_1", NULL},
    {"c_puni", "W_1", NULL},
    {"c_mb_robo_1a", "W_1", end_credit_batten_evt},
    {"c_puni", "W_1", NULL},
    {"c_prival", "W_1", NULL},
    {"c_toge", "W_1", NULL},
    {"c_tchibi", "W_1", NULL},
    {"c_sinemon", "SIN_W_1", NULL},
    {"c_paid", "PAI_W_1", NULL},
    {"c_m_boss", "W_1", NULL},
    {"c_m_buka", "W_1", NULL},
    {"c_m_waka", "W_1", NULL},
    {"c_m_ojo", "W_1", NULL},
    {"c_binbo", "W_1", NULL},
    {"c_monban", "W_1", NULL},
    {VERSION_PICK("c_bteresa", "c_bteresa2"), VERSION_PICK("BTS_W_1", "W_1"), NULL},
    {"c_babyyoshi", "PYS_W_1", NULL},
    {"c_ksikowa", "W_1", NULL},
    {"c_k_chof", "W_1", NULL},
    {"c_gans", "W_1", NULL},
    {"c_bred_h", "W_1", NULL},
    {"c_report", "W_1", NULL},
    {"c_hotdog", "W_1", NULL},
    {"c_shasho_g", "W_1", NULL},
    {"c_jugem_n", "W_1", NULL},
    {"c_togezo", "TGZ_W_1", NULL},
    {"c_heavy", "W_1", NULL},
    {"c_kamek", "KMK_W_1", NULL},
    {"c_togenoko_b", "W_1", NULL},
    {"c_burosu_h", "BRO_W_1", NULL},
    {"c_koopa", "KPA_W_1", NULL},
    {"c_atmic_trs", "W_1", end_credit_atomic_evt},
    {"c_kamek_be", "W_1", NULL},
    {"c_f_son", "W_1", NULL},
    {"c_bubusan", "W_1", NULL},
    {"c_f_mura", "W_1", NULL},
    {"c_f_mama", "W_1", NULL},
    {"c_f_child_a", "W_1", NULL},
    {"c_f_oji", "W_1", NULL},
    {"c_f_oba", "W_1", NULL},
    {"c_f_karas", "A_1", NULL},
    {"c_teresa", "W_1", NULL},
    {"c_ranpel", "W_1", NULL},
    {"c_f_owm", "A_1", NULL},
    {"c_met", "MET_W_1", NULL},
    {"c_met", "TMT_W_1", NULL},
    {"c_pansy", "PAN_W_1", NULL},
    {"c_majyorin", "MJR_W_1", NULL},
    {"c_maririn", "MRR_W_1", NULL},
    {"c_vivian", "PTR_W_1", NULL},
    {"c_korutesu1", "KRT_S_1", end_credit_korutesu_evt},
    {"c_dance_r", "W_1", NULL},
    {"c_dance_p", "W_1", NULL},
    {"c_dance_k", "W_1", NULL},
    {"c_bomt_n", "W_1", NULL},
    {"c_marco", "W_1", NULL},
    {"c_gara", "W_1", NULL},
    {"c_megane", "W_1", NULL},
    {"c_odo_k", "W_1", NULL},
    {"c_suifu_k", "W_1", NULL},
    {"c_suifu_b", "W_1", NULL},
    {"c_elmos", "W_1", NULL},
    {"c_kilr", "KIL_W_1", NULL},
    {"c_pakflwr", "PKF_S_1", NULL},
    {"c_tyutyu", "PCH_W_1", NULL},
    {"c_powan", "W_1", NULL},
    {"c_g_moamoa", "S_3", end_credit_moamoa_evt},
    {"c_papab", "W_1", NULL},
    {"c_mamab", "W_1", NULL},
    {"c_konari", "W_1", NULL},
    {"c_unten", "W_1", NULL},
    {"c_shasho_r", "W_1", NULL},
    {"c_smoko", "W_1", NULL},
    {"c_rose", "W_1", NULL},
    {"c_kinodebu", "W_1", NULL},
    {"c_ghost_k", "W_1", NULL},
    {"c_bom_sen", "S_2", end_credit_para_evt},
    {"c_waitress", "W_1", NULL},
    {"c_puku_c", "W_1", NULL},
    {"c_kinopiko", "KNF_W_1", NULL},
    {"c_n_moa_a", "W_1", NULL},
    {"c_met", "PMT_W_1", NULL},
    {"c_sanbo", "SNB_W_1", NULL},
    {"c_bomson", "W_1", NULL},
    {"c_mb_robo_2a", "J_2", end_credit_batten2_evt},
    {"c_bwhite", "W_1", NULL},
    {"c_hito_b", "W_1", NULL},
    {"c_bom_n", "W_1", NULL},
    {"c_kmoon", "W_1", NULL},
    {"c_baria", "W_1", NULL},
    {"c_basabasa", "BSA_W_1", NULL},
    {"c_wanwan", "WAN_A_1", NULL},
    {"c_maho", "MAH_W_1", NULL},
    {"c_zako_n", "W_1", NULL},
    {"c_zako_m", "W_1", NULL},
    {"c_shuryo", "W_1", NULL},
    {"c_q_kage_m", "KRT_Z_1", end_credit_queen_evt},
    {"c_gazigazi", "GAZ_A_1", NULL},
    {"a_mario", "M_W_1", NULL},
    {"c_peach", "P_W_1", NULL},
#endif
};

/**
 * Runs one silhouette of the character parade (credit NPC number args[0] in R_credits_npc_info).
 * Characters with their own script (evt) just start it.
 * Others get a CreditActor that is drawn as a silhouette spinning around a screen corner chosen by the current
 * staff roll position, rotating until it passes its limit angle and is then freed.
 * The next character's animation group is requested early so it is loaded in time.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00001d90, JP end .text+0x1CCC. See docs/NAME_PROVENANCE.md.
/**
 * Runs one silhouette of the character parade (credit NPC number args[0] in R_credits_npc_info).
 * Characters with their own script (evt) just start it.
 * Others get a CreditActor that is drawn as a silhouette spinning around a screen corner chosen by the current
 * staff roll position, rotating until it passes its limit angle and is then freed.
 * The next character's animation group is requested early so it is loaded in time.
 */
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
#if VERSION > VERSION_DEMO_US
        if (id + 5 < ARRAY_SIZE(R_credits_npc_info)) {
            animGroupBaseAsync(R_credits_npc_info[id + 5].agb, inBattle, NULL);
        }
#endif
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
#if VERSION > VERSION_DEMO_US
            case 4:
#endif
                actor->angle = 160.0f;
                actor->limit = 90.0f + actor->angle + 30.0f;
                break;
            case 2:
                actor->angle = 250.0f;
                actor->limit = 90.0f + actor->angle + 30.0f;
                break;
#if VERSION <= VERSION_DEMO_US
            case 3:
#endif
            default:
                actor->angle = 70.0f;
                actor->limit = 90.0f + actor->angle + 30.0f;
                break;
#if VERSION > VERSION_DEMO_US
            }
            if (id + 5 < ARRAY_SIZE(R_credits_npc_info)) {
                animGroupBaseAsync(R_credits_npc_info[id + 5].agb, inBattle, NULL);
#endif
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
#if VERSION > VERSION_DEMO_US
        case 4:
#endif
            x = -304.0f;
            y = 240.0f;
            r = 100.0f;
            break;
        case 2:
            x = -304.0f;
            y = -240.0f;
            r = 100.0f;
            break;
#if VERSION <= VERSION_DEMO_US
        case 3:
#endif
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
#if VERSION >= VERSION_US
            actor->poseId = animPoseEntry(R_credits_npc_info[id].agb, inBattle);
            animPoseSetAnim(actor->poseId, R_credits_npc_info[id].anim, 1);
#else
            EXIGetType(0, 2, &type);
            if (type != EXI_IS_VIEWER) {
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
#if VERSION >= VERSION_US
            actor->angle += 9.0f / gp->framerate;
#else
            actor->angle += 0.15f;
#endif
            if (actor->angle > actor->limit) {
                animPoseRelease(actor->poseId);
                _mapFree(actor);
#if VERSION <= VERSION_DEMO_US
                end_wp->npcLoadRequest++;
#endif
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
#if VERSION > VERSION_DEMO_US
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
#endif
    animPoseMain(actor->poseId);
    dispEntry(CAMERA_3D, 0.0f, 1, end_credit_npc_disp, actor);
    return 0;
}

/**
 * Runs the parade of characters during the staff roll:
 * goes through R_credits_npc_info in order, starts evt_end_credit_npc_main for each
 * and waits between characters as evt_end_credit_npc_get_wait_type says,
 * until the table ends or the roll finishes.
 */
EVT_BEGIN(end_credit_npc_parade)
    SET_VALUE(LW(0), 0)
    LOOP(0)
        CALL(evt_end_credit_npc_get_wait_type, LW(0), LW(1))
        BEGIN_INLINE_EVENT
            CALL(evt_end_credit_npc_main, LW(0))
        END_INLINE_EVENT
        IF_INT_EQUAL(LW(1), 1)
#if VERSION <= VERSION_DEMO_US
            WAIT_MS(2000)
#else
            WAIT_MS(1920)
        END_IF
        IF_INT_EQUAL(LW(1), 2)
            WAIT_MS(6000)
#endif
        END_IF
        ADD(LW(0), 1)
#if VERSION <= VERSION_DEMO_US
        IF_INT_GREATER_OR_EQUAL(LW(0), ARRAY_SIZE(R_credits_npc_info))
#else
        IF_INT_GREATER_OR_EQUAL(LW(0), 152)
#endif
            DEBUG_PUT_MSG(STRING("あにめおわり"))
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(GF(10), 1)
            RETURN
        END_IF
    END_LOOP
    RETURN
EVT_END()

#if VERSION >= VERSION_US
/** Texture numbers of the overlay pictures shown during the ending, in the order they appear. */
s32 end_overlay_tex_tbl[6] = {52, 52, 54, 55, 55, 56};
#endif

/**
 * Japanese release and later: loops the two ending pictures during the last credit image:
 * shows one, cross-fades to the next and back, with long holds (about 18 s) and 4 s fades,
 * by driving evt_end_image_set_mode and the TEV blend value.
 */
#if VERSION > VERSION_DEMO_US
EVT_BEGIN(end_image_blend_loop)
    LOOP(0)
        CALL(evt_end_image_set_mode, 1)
        CALL(evt_end_image_set_blend, 0)
        WAIT_MS(18000)
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 4000)
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
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 4000)
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
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 4000)
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
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 4000)
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
#endif

/**
 * Guess (details): draw callback of the ending pictures (Japanese release and later):
 * draws the picture selected by GW(11) from ending.tpl in the position given by GW(12), with the fade alpha,
 * the overlay picture and the cross-blend of end_image_blend_loop;
 * for picture 9 it shows the version where Ms. Mowz was met with Koops if that flag is set.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00001324, JP end .text+0x1260. See docs/NAME_PROVENANCE.md.
/**
 * Guess (details): draw callback of the ending pictures (Japanese release and later):
 * draws the picture selected by GW(11) from ending.tpl in the position given by GW(12), with the fade alpha,
 * the overlay picture and the cross-blend of end_image_blend_loop;
 * for picture 9 it shows the version where Ms. Mowz was met with Koops if that flag is set.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Requests the animation groups of the credit NPCs after the ones already loaded,
 * one for each NPC that has left the screen, so the parade's next characters are ready in time;
 * runs until the roll ends (GF(10)).
 */
USER_FUNC(evt_end_credit_npc_preload) {
    BOOL inBattle = gp->inBattle != 0;
    s32 i;

    for (i = 0; i < end_wp->npcLoadRequest; i++) {
        if (end_wp->npcLoadNum + i >= ARRAY_SIZE(R_credits_npc_info)) {
            return 2;
        }
        animGroupBaseAsync(R_credits_npc_info[end_wp->npcLoadNum + i].agb, inBattle, NULL);
    }
    end_wp->npcLoadNum += end_wp->npcLoadRequest;
    end_wp->npcLoadRequest = 0;
    return evtGetValue(event, GF(10)) ? 2 : 0;
}
#endif
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
#if VERSION <= VERSION_DEMO_US
    if (end_wp->alpha == 0) {
        return;
    }
    id %= ((TEXPalette*)*end_wp->tpl->data)->numDescriptors;
#else
    if (id >= ((TEXPalette*)*end_wp->tpl->data)->numDescriptors) {
        return;
    }
    if (id == 9 && swGet(0x5E7)) { // GSWF_GON_12_MOWZ_MET_WITH_KOOPS
        id = 53;
    }
#endif
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
#if VERSION > VERSION_DEMO_US
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
#endif
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, end_wp->alpha});
#if VERSION <= VERSION_DEMO_US
    TEXGetGXTexObjFromPalette(*end_wp->tpl->data, &obj, id);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    switch (pos) {
        case 0:
            PSMTXTrans(mtx, 55.0f, 30.0f, -30.0f);
            break;
        case 1:
            PSMTXTrans(mtx, -55.0f, -30.0f, -30.0f);
            break;
        case 2:
            PSMTXTrans(mtx, -55.0f, 30.0f, -30.0f);
            break;
        case 3:
            PSMTXTrans(mtx, 55.0f, -30.0f, -30.0f);
#else
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
#endif
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
#if VERSION > VERSION_DEMO_US
    if (end_wp->alpha2 > 0) {
        f32 w2;
        f32 h2;

        camPos = (Vec){0.0f, 0.0f, 240.0f / tanf(0.21816616f)};
        up = (Vec){0.0f, 1.0f, 0.0f};
        target = (Vec){0.0f, 0.0f, 0.0f};
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
#if VERSION >= VERSION_US
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
#endif
}

/**
 * Runs the pictures shown beside the credits: waits until the roll asks for a picture (GW(10) = 1),
 * fades it in over 500 ms, holds it until GW(10) = 2 and fades it out.
 * For picture 49, the last one, the Japanese release and later also fade in an overlay, change the music,
 * loop the blended pictures until the player presses a button and then start end_return_title_evt.
 */
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
        // The picture appears and fades in.
        BEGIN_INLINE_EVENT_GET_ID(LW(9))
            CALL(evt_end_image_disp_entry)
        END_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 500)
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
#if VERSION > VERSION_DEMO_US
        IF_INT_EQUAL(GW(11), 49)
            // The very last picture: a second overlay picture fades in over it.
            WAIT_MS(500)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value)
                CALL(evt_end_image_set_overlay_alpha, LW(0))
                WAIT_FRAMES(1)
                IF_INT_EQUAL(LW(1), 0)
                    BREAK_LOOP
                END_IF
            END_LOOP
            WAIT_MS(3000)
            // The credits music fades out, and after a pause the closing music begins.
            SET_VALUE(GF(0), 0)
            BEGIN_INLINE_EVENT
                CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 5000)
                WAIT_MS(5000)
                IF_INT_EQUAL(GF(0), 0)
                    CALL(evt_snd_bgmon, PSND_FADEOUT | PSND_SLOT(1), STRING("BGM_EVT_PEACH1"))
                END_IF
            END_INLINE_EVENT
            WAIT_MS(500)
            // The final pictures blend back and forth until the player presses a button.
            SPAWN_EVENT_GET_ID(PTR(end_image_blend_loop), LW(10))
            LOOP(0)
                CALL(evt_key_get_buttontrg, 0, LW(0))
                IF_BITS_SET(LW(0), PAD_BUTTON_A | PAD_BUTTON_START)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            SET_VALUE(GF(0), 1)
            // The player pressed a button: the screen fades to black, the pictures are cleared and the title
            // return starts.
            DELETE_EVENT(LW(10))
            CALL(evt_fade_entry, FADE_OUT_BLACK, 500, 0, 0, 0)
            CALL(evt_fade_end_wait)
            CALL(evt_end_image_set_mode, 0)
            CALL(evt_end_image_set_alpha, LW(0))
            CALL(evt_end_image_set_overlay_alpha, LW(0))
            CALL(evt_fade_entry, FADE_IN_BLACK, 0, 0, 0, 0)
            CALL(evt_fade_end_wait)
            SET_VALUE(GW(10), 0)
            SPAWN_EVENT(PTR(end_return_title_evt))
            RETURN
        END_IF
#endif
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 255, 0, 500)
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

/**
 * End of the credits: fades the screen to black, restores the font, sets the story to STG9_01_END_CREDITS,
 * saves the game to the memory card (the European release first heals Mario and the party),
 * fades the music out and changes map to the title screen.
 */
EVT_BEGIN(end_return_title_evt)
    CALL(evt_fade_entry, FADE_OUT_BLACK_2D, 2000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1500)
    SET_VALUE(GF(10), 1)
    CALL(evt_end_font_restore)
    SET_VALUE(GSW_STORY_PROGRESS, STG9_01_END_CREDITS)
#if VERSION <= VERSION_DEMO_US
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 21)
    CALL_CHILD(evt_memcard_save)
    CALL(end_bgm_wait_stop)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 3000)
#else
#if VERSION == VERSION_EU
    CALL(evt_pouch_mario_recovery) // PAL: the party is healed before the save
    CALL(evt_pouch_all_party_recovery)
#endif
    SET_VALUE(LF(10), 0)
    SET_VALUE(LW(0), 21)
#if VERSION >= VERSION_US
    CALL_CHILD(PTR(evt_memcard_save_wrapper))
#else
    CALL_CHILD(PTR(evt_memcard_save))
#endif
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT | PSND_SLOT(1), 2000)
#endif
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_WHITE, 2000, -1, -1)
    CALL(evt_seq_set_seq, SEQ_MAP_CHANGE, STRING("title"), 0)
    RETURN
EVT_END()

/**
 * Map init for end_00 (the only room): hides Mario and the partner, sets the fixed camera and black background,
 * loads the ending data, starts the character parade and the staff roll message script,
 * and starts the staff roll music (BGM_STAFF_ROLL1).
 * The demo versions also start the NPC preload script and queue the return to the title.
 */
EVT_BEGIN(end_00_init_evt)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_cam3d_evt_set, 0, 0, 545, 0, 0, 0, 0, EASING_SINE_IN_OUT)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("pPlane1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_bg_set_color, 0, 0, 0, 255)
    SET_VALUE(GF(10), 0)
    CALL(evt_end_staff_roll_init)
#if VERSION <= VERSION_DEMO_US
    BEGIN_INLINE_EVENT
            CALL(evt_end_credit_npc_preload)
        END_INLINE_EVENT
    SPAWN_EVENT(end_credit_npc_parade)
#else
    SPAWN_EVENT(PTR(end_credit_npc_parade))
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_end_staff_roll_msg_main)
#if VERSION <= VERSION_DEMO_US
    SPAWN_EVENT(end_return_title_evt)
#endif
    END_INLINE_EVENT
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STAFF_ROLL1"))
    SET_VALUE(GSW_STORY_PROGRESS, STG9_01_END_CREDITS)
    RETURN
EVT_END()

/**
 * the demo's silhouette scripts: other models and paths than the releases' (most rise straight up through the
 * middle of the screen)
 */
EVT_BEGIN(end_credit_gesso_evt)
    CALL(evt_npc_entry, STRING("gesso"), STRING("c_gesso"))
    CALL(evt_npc_set_anim, STRING("gesso"), STRING("GSO_S2_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("gesso"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("gesso"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -4000, 4000, 5000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_set_position, STRING("gesso"), 0, LW(0), 0)
#else
        CALL(evt_npc_set_position, STRING("gesso"), 130, LW(0), 0)
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("gesso"))
    RETURN
EVT_END()

/** Demo parade script for Hooktail (ゴンババ): the silhouette slides across the screen from the right. */
EVT_BEGIN(end_credit_gonbaba_evt)
    CALL(evt_npc_entry, STRING("gonbaba"), STRING("c_gonbaba"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_npc_set_anim, STRING("gonbaba"), STRING("GNB_S_1"))
#else
    CALL(evt_npc_set_anim, STRING("gonbaba"), STRING("GNB_S_3"))
#endif
    CALL(evt_npc_flag_onoff, 1, STRING("gonbaba"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("gonbaba"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 10000, 2000, 2000)
#else
    CALL(evt_npc_set_scale, STRING("gonbaba"), FLOAT(0.5f), FLOAT(0.5f), FLOAT(0.5f))
    CALL(evt_npc_set_rotate, STRING("gonbaba"), 0, 180, 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10000, -1300, 2000)
#endif
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_set_position, STRING("gonbaba"), LW(0), -100, 0)
#else
        CALL(evt_npc_set_position, STRING("gonbaba"), LW(0), -20, 200)
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(3000)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 2000, 10000, 2000)
#else
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1300, -10000, 2000)
#endif
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_set_position, STRING("gonbaba"), LW(0), -100, 0)
#else
        CALL(evt_npc_set_position, STRING("gonbaba"), LW(0), -20, 200)
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_delete, STRING("gonbaba"))
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(end_credit_batten_evt)
    CALL(evt_npc_entry, STRING("batten"), STRING("c_mb_robo_1b"))
    CALL(evt_npc_set_anim, STRING("batten"), STRING("MGN_W_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("batten"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("batten"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -4000, 4000, 5000)
#else
/**
 * Demo parade script for the second robot model (c_mb_robo_2b):
 * rises up through the middle of the screen as a silhouette.
 */
EVT_BEGIN(end_credit_batten2_evt)
    CALL(evt_npc_entry, STRING("batten2"), STRING("c_mb_robo_2a"))
    CALL(evt_npc_set_anim, STRING("batten2"), STRING("J_2"))
    CALL(evt_npc_flag_onoff, 1, STRING("batten2"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("batten2"))
    CALL(evt_npc_set_rotate, STRING("batten2"), 180, 0, 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 4000, -4000, 4000)
#endif
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_set_position, STRING("batten"), 0, LW(0), 0)
#else
        CALL(evt_npc_set_position, STRING("batten2"), 100, LW(0), 0)
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
#if VERSION <= VERSION_DEMO_US
    CALL(evt_npc_delete, STRING("batten"))
#else
    CALL(evt_npc_delete, STRING("batten2"))
#endif
    RETURN
EVT_END()

/** Parade script (releases) for Atomic Boo (c_atmic_trs): drifts across the screen at 0.8 scale. */
#if VERSION <= VERSION_DEMO_US
/** Demo parade script for Cortez (korutesu): rises up through the middle of the screen. */
EVT_BEGIN(end_credit_korutesu_evt)
    CALL(evt_npc_entry, STRING("kortesu"), STRING("c_korutesu1"))
    CALL(evt_npc_set_anim, STRING("kortesu"), STRING("KRT_S_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("kortesu"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("kortesu"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -4000, 4000, 5000)
#else
EVT_BEGIN(end_credit_atomic_evt)
    CALL(evt_npc_entry, STRING("atomic"), STRING("c_atmic_trs"))
    CALL(evt_npc_set_anim, STRING("atomic"), STRING("W_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("atomic"), NPC_FLAG_NO_CULL | NPC_FLAG_INSTANT_TURN)
    CALL(evt_end_npc_silhouette_on, STRING("atomic"))
    CALL(evt_npc_set_rotate, STRING("atomic"), 0, 180, 0)
    CALL(evt_npc_set_scale, STRING("atomic"), FLOAT(0.7998047f), FLOAT(0.7998047f), FLOAT(0.7998047f))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -5000, 5000, 6000)
#endif
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_set_position, STRING("kortesu"), 0, LW(0), 0)
#else
        CALL(evt_npc_set_position, STRING("atomic"), LW(0), 20, 0)
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
#if VERSION <= VERSION_DEMO_US
    CALL(evt_npc_delete, STRING("kortesu"))
#else
    CALL(evt_npc_delete, STRING("atomic"))
#endif
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
/**
 * Demo parade script for the second robot model (c_mb_robo_2b):
 * rises up through the middle of the screen as a silhouette.
 */
EVT_BEGIN(end_credit_batten2_evt)
    CALL(evt_npc_entry, STRING("batten2"), STRING("c_mb_robo_2b"))
    CALL(evt_npc_set_anim, STRING("batten2"), STRING("MGN_W_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("batten2"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("batten2"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -4000, 4000, 5000)
#else
/** Demo parade script for Cortez (korutesu): rises up through the middle of the screen. */
EVT_BEGIN(end_credit_korutesu_evt)
    CALL(evt_npc_entry, STRING("korutesu"), STRING("c_korutesu1"))
    CALL(evt_npc_set_anim, STRING("korutesu"), STRING("KRT_S_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("korutesu"), NPC_FLAG_NO_CULL | NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("korutesu"), 90)
    CALL(evt_end_npc_silhouette_on, STRING("korutesu"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10000, -2000, 2000)
#endif
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_set_position, STRING("batten2"), 0, LW(0), 0)
#else
        CALL(evt_npc_set_position, STRING("korutesu"), LW(0), -150, 0)
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
#if VERSION <= VERSION_DEMO_US
    CALL(evt_npc_delete, STRING("batten2"))
#else
    WAIT_MS(3000)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -2000, -10000, 2000)
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
#endif
    RETURN
EVT_END()

/**
 * Parade script (releases) for the c_bom_sen model, the Bob-omb crowd sprite of Fahr Outpost's cannon scene
 * (bom_01's evt_bom1000), in pose S_2: falls down the screen as a silhouette.
 */
#if VERSION <= VERSION_DEMO_US
/** Demo parade script for the Shadow Queen model (c_q_kage): rises up through the middle of the screen. */
EVT_BEGIN(end_credit_queen_evt)
    CALL(evt_npc_entry, STRING("queen"), STRING("c_q_kage"))
    CALL(evt_npc_set_anim, STRING("queen"), STRING("KRT_S_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("queen"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("queen"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -4000, 4000, 5000)
#else
EVT_BEGIN(end_credit_para_evt)
    WAIT_MS(1000)
    CALL(evt_npc_entry, STRING("para"), STRING("c_bom_sen"))
    CALL(evt_npc_set_anim, STRING("para"), STRING("S_2"))
    CALL(evt_npc_flag_onoff, 1, STRING("para"), NPC_FLAG_NO_CULL | NPC_FLAG_INSTANT_TURN)
    CALL(evt_end_npc_silhouette_on, STRING("para"))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 3000, -3000, 6000)
#endif
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_npc_set_position, STRING("queen"), 0, LW(0), 0)
#else
        CALL(evt_npc_set_position, STRING("para"), 50, LW(0), 0)
#endif
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
#if VERSION <= VERSION_DEMO_US
    CALL(evt_npc_delete, STRING("queen"))
    RETURN
EVT_END()
#else
    CALL(evt_npc_delete, STRING("para"))
    RETURN
EVT_END()

EVT_BEGIN(end_credit_batten_evt)
    CALL(evt_npc_entry, STRING("batten"), STRING("c_mb_robo_1a"))
    CALL(evt_npc_set_anim, STRING("batten"), STRING("W_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("batten"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("batten"))
    CALL(evt_npc_set_scale, STRING("batten"), FLOAT(0.5996094f), FLOAT(0.5996094f), FLOAT(0.5996094f))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 10000, 500, 2000)
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
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 500, 10000, 2000)
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

/** Demo parade script for the Shadow Queen model (c_q_kage): rises up through the middle of the screen. */
EVT_BEGIN(end_credit_queen_evt)
    WAIT_MS(7000)
    CALL(evt_npc_entry, STRING("queen"), STRING("c_q_kage_m"))
    CALL(evt_npc_set_anim, STRING("queen"), STRING("KRT_Z_1"))
    CALL(evt_npc_flag_onoff, 1, STRING("queen"), NPC_FLAG_NO_CULL)
    CALL(evt_end_npc_silhouette_on, STRING("queen"))
    CALL(evt_npc_set_scale, STRING("queen"), FLOAT(0.5f), FLOAT(0.5f), FLOAT(0.5f))
    CALL(evt_npc_set_ry, STRING("queen"), 90)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10000, -1000, 2000)
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
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1000, -10000, 2000)
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

/** Model name of the Smorg silhouette used in the ending's closing pictures. */
const char* end_moa_ext_anim = "c_n_moa_a";

/** Animation name of the Smorg silhouette used in the ending's closing pictures. */
const char* end_moa_ext_pose = "S_1";
#endif

/** Script helper: queues the ending picture draw callback (end_image_disp) for this frame. */
// Descriptive name (not in the symbol maps); placeholder unk_end_000012e0, JP end .text+0x121C. See docs/NAME_PROVENANCE.md.
USER_FUNC(evt_end_image_disp_entry) {
    dispEntry(CAMERA_3D, 0.0f, 7, end_image_disp, NULL);
    return 0;
}

/** Script helper: sets the opacity (0 to 255) of the ending picture to args[0]. */
// Descriptive name (not in the symbol maps); placeholder unk_end_000012a4, JP end .text+0x11E0. See docs/NAME_PROVENANCE.md.
USER_FUNC(evt_end_image_set_alpha) {
    end_wp->alpha = evtGetValue(event, event->args[0]);
    return 2;
}

/**
 * Script helper: sets args[0] as alpha2, the opacity of the overlay picture end_image_disp draws over the
 * picture while it is above 0 (image 52 in JP, end_overlay_tex_tbl by language later).
 * The staff roll fades it in
 * over the last picture (49) before end_image_blend_loop starts.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00001268, JP end .text+0x11A4. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Script helper: sets args[0] as alpha2, the opacity of the overlay picture end_image_disp draws over the
 * picture while it is above 0 (image 52 in JP, end_overlay_tex_tbl by language later).
 * The staff roll fades it in
 * over the last picture (49) before end_image_blend_loop starts.
 */
USER_FUNC(evt_end_image_set_overlay_alpha) {
    end_wp->alpha2 = evtGetValue(event, event->args[0]);
    return 2;
}
#endif

/**
 * Script helper: selects what end_image_disp draws (args[0]): 0 the current picture, 1 images 49 and 50 blended,
 * 2 images 50 and 51 blended; end_image_blend_loop switches between 1 and 2.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_0000122c, JP end .text+0x1168. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Script helper: selects what end_image_disp draws (args[0]): 0 the current picture, 1 images 49 and 50 blended,
 * 2 images 50 and 51 blended; end_image_blend_loop switches between 1 and 2.
 */
USER_FUNC(evt_end_image_set_mode) {
    end_wp->mode = evtGetValue(event, event->args[0]);
    return 2;
}
#endif

/**
 * Script helper: sets args[0] (0 to 255) as the TEV blend alpha of the ending pictures:
 * in modes 1 and 2 it is the
 * cross-fade amount between the two blended images, which end_image_blend_loop ramps over 4 seconds.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_000011f0, JP end .text+0x112C. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Script helper: sets args[0] (0 to 255) as the TEV blend alpha of the ending pictures:
 * in modes 1 and 2 it is the
 * cross-fade amount between the two blended images, which end_image_blend_loop ramps over 4 seconds.
 */
USER_FUNC(evt_end_image_set_blend) {
    end_wp->tevAlpha = evtGetValue(event, event->args[0]);
    return 2;
}
#endif

/**
 * Script helper that runs the whole staff roll:
 * steps through end_staff_roll_msg_tbl, and for each entry opens two message windows (job title and names) at
 * one of five screen positions, waits the entry's time, closes them and starts the next,
 * while telling end_image_main_evt when to show each picture (GW(10) and GW(11)).
 * Starts end_image_main_evt on its first call.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00000bc8, JP end .text+0xB04. See docs/NAME_PROVENANCE.md.
/**
 * Script helper that runs the whole staff roll:
 * steps through end_staff_roll_msg_tbl, and for each entry opens two message windows (job title and names) at
 * one of five screen positions, waits the entry's time, closes them and starts the next,
 * while telling end_image_main_evt when to show each picture (GW(10) and GW(11)).
 * Starts end_image_main_evt on its first call.
 */
USER_FUNC(evt_end_staff_roll_msg_main) {
    s32 state = event->userdata[0];
    s32 idx = event->userdata[1];
    s32 i;
    const char* msg;
    const char* msg2;
    s32 x;
    s32 y;
    s32 wx;
    s32 wx2;
    s32 wy;
    s32 wy2;
    s32 n;
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
#if VERSION <= VERSION_DEMO_US
            y = -20;
#else
            y = -24;
#endif
            break;
        case 1:
            x = 274;
            y = 250;
            break;
        case 2:
            x = 274;
#if VERSION <= VERSION_DEMO_US
            y = -20;
#else
            y = -24;
#endif
            break;
        case 3:
            x = -274;
            y = 250;
            break;
        case 4:
            x = 0;
#if VERSION <= VERSION_DEMO_US
            y = -80;
#else
            y = -60;
#endif
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
#if VERSION <= VERSION_DEMO_US
                    }
                    sprintf(buf1, "<clear><wait %d><dkey><wpos %d %d 400 200><col f0f0f0ff><speed 0><scale 0.8>%s</scale></speed></col><wait 500></dkey><once_stop>", 0, wx - 62, wy, msg);
#else
#if VERSION == VERSION_EU
                    }
                    if (strcmp(end_staff_roll_msg_tbl[idx].msg1[i], "c00") == 0) {
                        wx += 10;
#endif
                    }
                    sprintf(buf1, "<clear><dkey><wpos %d %d 400 200><col f0f0f0ff><speed 0><scale 0.8>%s</scale></speed></col><wait 500></dkey><once_stop>", wx - 62, wy, msg);
#endif
                    end_wp->win1[i] = msgWindow_Entry(buf1, 0, 0);
                }
            }
            for (i = 0; i < 3; i++) {
                if (end_staff_roll_msg_tbl[idx].msg2[i] != NULL) {
                    msg2 = msgSearch(end_staff_roll_msg_tbl[idx].msg2[i]);
#if VERSION <= VERSION_DEMO_US
                    wx2 = x + 8;
                    wy2 = y - i * 24 - 50;
                    if (x == 0) {
                        wx2 -= FontGetMessageWidth(msg2) / 2;
                    } else if (x > 0) {
                        wx2 -= FontGetMessageWidth(msg2);
                    }
                    sprintf(buf2, "<clear><wait %d><dkey><wpos %d %d 400 200><col f0f0f0ff><speed 0>%s</speed></col><wait 5200></dkey><once_stop>", 0, wx2 - 62, wy2, msg2);
#else
                    wx2 = x;
                    wy2 = y - i * 24 - 50;
                    if (x == 0) {
                        wx2 = x - FontGetMessageWidth(msg2) / 2.0f;
                    } else if (x > 0) {
                        wx2 = x - FontGetMessageWidth(msg2);
                    }
                    sprintf(buf2, "<clear><dkey><wpos %d %d 400 200><col f0f0f0ff><speed 0>%s</speed></col><wait 5200></dkey><once_stop>", wx2 - 62, wy2, msg2);
#endif
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
#if VERSION <= VERSION_DEMO_US
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
                state++;
            } else {
                // the last entry stays up, with BGM_EVT_PEACH1 (BGM id 0xCD), until A is pressed
                psndBGMOn(PSND_FADE_OUT, (char*)0xCD);
#else
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
#endif
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
#if VERSION <= VERSION_DEMO_US
            if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
                evtSetValue(event, GW(10), 2);
                state++;
            }
            break;
        case 5:
            if (evtGetValue(event, GW(10)) == 0) {
                return 2;
            }
            break;
#else
            return 2;
#endif
    }
    event->userdata[0] = state;
    event->userdata[1] = idx;
    return 0;
}

/**
 * Script helper: prepares the staff roll.
 * On the first call it resets the roll state, mutes the sound effects,
 * and in the Japanese versions switches the language to English for the credits text;
 * then it waits until ending.tpl (the picture file) and the first characters' animation groups have loaded.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00000a6c, JP end .text+0x9A8. See docs/NAME_PROVENANCE.md.
/**
 * Script helper: prepares the staff roll.
 * On the first call it resets the roll state, mutes the sound effects,
 * and in the Japanese versions switches the language to English for the credits text;
 * then it waits until ending.tpl (the picture file) and the first characters' animation groups have loaded.
 */
USER_FUNC(evt_end_staff_roll_init) {
    BOOL inBattle = gp->inBattle ? TRUE : FALSE;
    s32 i;

    if (isFirstCall) {
#if VERSION <= VERSION_DEMO_US
        end_wp->npcLoadNum = 0;
        end_wp->npcLoadRequest = 0;
        end_wp->tpl = NULL;
        end_wp->alpha = 0;
        end_wp->pos = 0;
        psndSetFlag(PSND_FLAG_NO_SFX);
        gp->language = 1;
        JUTFont_Free();
        JUTFontSetup(1);
        msgLoad("global", 1);
#else
        end_wp->tpl = NULL;
        end_wp->alpha = 0;
        end_wp->mode = 0;
        end_wp->pos = 0;
        psndSetFlag(PSND_FLAG_NO_SFX);
#if VERSION < VERSION_US
        gp->language = 1;
        JUTFont_Free();
        JUTFontSetup(1);
#endif
#endif
    }
    if ((u32)fileAsyncf(FILE_TYPE_TPL, NULL, "%s/ending.tpl", getMarioStDvdRoot()) == 0) {
        return 0;
    }
    if (end_wp->tpl == NULL) {
        end_wp->tpl = fileAllocf(FILE_TYPE_TPL, "%s/ending.tpl", getMarioStDvdRoot());
    }
#if VERSION <= VERSION_DEMO_US
    end_wp->npcLoadNum = 10;
    for (i = 0; i < end_wp->npcLoadNum; i++) {
#else
    for (i = 0; i < 5; i++) {
#endif
        if (!animGroupBaseAsync(R_credits_npc_info[i].agb, inBattle, NULL)) {
            return 0;
        }
    }
#if VERSION <= VERSION_DEMO_US
    for (i = 0; i < end_wp->npcLoadNum; i++) {
#else
    // FAKE: empty five-iteration loop retained; removing it changes the object.
    for (i = 0; i < 5; i++) {
#endif
    }
    return 2;
}

#pragma dont_inline on
// Descriptive name (not in the symbol maps); placeholder unk_end_00000a14, JP end .text+0x950. See docs/NAME_PROVENANCE.md.
/**
 * Cleans up after the staff roll: frees the ending picture file, lets sound effects play again and,
 * in the Japanese versions, switches back to Japanese text (and in the demos reloads the global messages).
 */
void end_staff_roll_exit(void) {
    fileFree(end_wp->tpl);
    psndClearFlag(PSND_FLAG_NO_SFX);
#if VERSION <= VERSION_DEMO_US
    gp->language = 0;
    JUTFont_Free();
    JUTFontSetup(0);
    msgLoad("global", 1);
#else
#if VERSION < VERSION_US
    gp->language = 0;
    JUTFont_Free();
    JUTFontSetup(0);
#endif
#endif
}
#pragma dont_inline reset

/**
 * Script helper: puts the font and messages back to normal before the game continues:
 * restores Japanese text where the credits switched it, or turns the sound effects back on.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_000009cc, JP end .text+0x908. See docs/NAME_PROVENANCE.md.
/**
 * Script helper: puts the font and messages back to normal before the game continues:
 * restores Japanese text where the credits switched it, or turns the sound effects back on.
 */
USER_FUNC(evt_end_font_restore) {
#if VERSION <= VERSION_DEMO_US
    gp->language = 0;
    JUTFont_Free();
    JUTFontSetup(0);
    msgLoad("global", 1);
#else
#if VERSION < VERSION_US
    gp->language = 0;
    JUTFont_Free();
    JUTFontSetup(0);
#endif
    psndClearFlag(PSND_FLAG_NO_SFX);
#endif
    return 2;
}

// Descriptive name (not in the symbol maps); placeholder unk_end_000009a4, JP end .text+0x8E0. See docs/NAME_PROVENANCE.md.
USER_FUNC(end_bgm_wait_stop) {
    psndSetFlag(PSND_FLAG_BGM_WAIT_0);
    return 2;
}

/**
 * Script helper: turns the NPC named by args[0] into a black silhouette by hooking the silhouette TEV callback
 * and GX callback into its animation pose.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00000938, JP end .text+0x874. See docs/NAME_PROVENANCE.md.
USER_FUNC(evt_end_npc_silhouette_on) {
    NpcEntry* npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));

    mapSetTevCallback(1, end_silhouette_tev);
    animPoseSetMaterialFlagOn(npc->poseId, MAP_OBJ_TEV_CALLBACK(1));
    animPoseSetGXFunc(npc->poseId, end_silhouette_rendermode, 0);
    return 2;
}

/** Random float in [min, max] from rand(). */
// Descriptive name (not in the symbol maps); placeholder unk_end_000008bc, JP end .text+0x7F8. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/** Random float in [min, max] from rand(). */
f32 end_randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}
#endif

/**
 * Ext-pose init for the Smorg silhouette in the credits:
 * gives each of the 300 small Moamoa a random offset of up to 10 units
 * and assigns it to a chain (between two points of the big Smorg's skeleton) and a place along that chain.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_000006f8, JP end .text+0x634. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Ext-pose init for the Smorg silhouette in the credits:
 * gives each of the 300 small Moamoa a random offset of up to 10 units
 * and assigns it to a chain (between two points of the big Smorg's skeleton) and a place along that chain.
 */
void end_ext_moamoa_init(void) {
    ExtdrvPose* pose = extGetPosePtr();
    EndMoa* elem = end_moa_tbl;
    s32 i;
    Vec v;

    for (i = 0; i < extGetPoseNum(); i++, pose++, elem++) {
        pose->flags |= (EXT_POSE_FLAG_VISIBLE | EXT_POSE_FLAG_NO_SHADOW);
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
#endif

/**
 * Per-frame update of the Smorg silhouette: places each small Moamoa on its chain between two joints of the big
 * Smorg model (c_g_moamoa), hides it when a joint is off screen, scales it with the model,
 * and sets its animation frame from the retrace counter.
 * Then clears the model's joint positions for the next frame.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_00000440, JP end .text+0x37C. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Per-frame update of the Smorg silhouette: places each small Moamoa on its chain between two joints of the big
 * Smorg model (c_g_moamoa), hides it when a joint is off screen, scales it with the model,
 * and sets its animation frame from the retrace counter.
 * Then clears the model's joint positions for the next frame.
 */
void end_ext_moamoa_main(void) {
    NpcEntry* npc = npcNameToPtr("\203\202\203A\203\202\203A");
    Vec* wpos = (Vec*)animPoseGetAnimPosePtr(npc->poseId)->worldPositions->address;
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
            pose->flags &= ~EXT_POSE_FLAG_VISIBLE;
        } else {
            PSVECScale(a, &v, t);
            PSVECScale(b, &w, s);
            PSVECAdd(&v, &w, &v);
            pose->flags |= EXT_POSE_FLAG_VISIBLE;
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
    p = (Vec*)ap->worldPositions->address;
    for (i = 0; i < n; i++) {
        p[i] = (Vec){0.0f, -1000.0f, 0.0f};
    }
}
#endif

/**
 * Draw callback for the Smorg silhouette: draws all the small Moamoa in plain black using the silhouette render
 * mode.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_0000039c, JP end .text+0x2D8. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Draw callback for the Smorg silhouette: draws all the small Moamoa in plain black using the silhouette render
 * mode.
 */
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
#endif

/** Queues end_ext_disp into the 3D camera every frame. */
// Descriptive name (not in the symbol maps); placeholder unk_end_0000035c, JP end .text+0x298. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/** Queues end_ext_disp into the 3D camera every frame. */
void end_ext_moamoa_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, end_ext_disp, NULL);
}
#endif

/**
 * Script helper: allocates the Smorg silhouette's tables (300 Moamoa, 40 animation frames,
 * the chains between joints of the big Smorg model),
 * turns on world-position tracking for its pose and writes the animation table to LW(10) for evt_ext_entry.
 */
// Descriptive name (not in the symbol maps); placeholder unk_end_0000012c, JP end .text+0x68. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Script helper: allocates the Smorg silhouette's tables (300 Moamoa, 40 animation frames,
 * the chains between joints of the big Smorg model),
 * turns on world-position tracking for its pose and writes the animation table to LW(10) for evt_ext_entry.
 */
USER_FUNC(evt_end_moamoa_init) {
    NpcEntry* npc = npcNameToPtr("\203\202\203A\203\202\203A");
    AnimationPose* ap;
    Vec* p;
    s32 n;
    s32 i;

    end_moa_tbl = _mapAlloc(sizeof(EndMoa) * 300);
    memset(end_moa_tbl, 0, sizeof(EndMoa) * 300);
    animPoseWorldPositionEvalOn(npc->poseId);
    end_moa_ext_data = _mapAlloc(sizeof(ExtTexInfo) * 41);
    memset(end_moa_ext_data, 0, sizeof(ExtTexInfo) * 41);
    for (i = 0; i < 40; i++) {
        end_moa_ext_data[i].group = end_moa_ext_anim;
        end_moa_ext_data[i].anim = end_moa_ext_pose;
        end_moa_ext_data[i].frame = i;
    }
    evtSetValue(event, LW(10), (EvtWord)end_moa_ext_data);
    ap = animPoseGetAnimPosePtr(npc->poseId);
    n = ap->numWorldPositions;
    p = (Vec*)ap->worldPositions->address;
    for (i = 0; i < n; i++) {
        p[i] = (Vec){0.0f, -1000.0f, 0.0f};
    }
    end_moa_chain_count = _mapAlloc(g_moamoa_chain_num * sizeof(s32));
    memset(end_moa_chain_count, 0, g_moamoa_chain_num * sizeof(s32));
    end_moa_chain_group = _mapAlloc(g_moamoa_chain_num * sizeof(s32[2]));
    memset(end_moa_chain_group, 0, g_moamoa_chain_num * sizeof(s32[2]));
    for (i = 0; i < g_moamoa_chain_num; i++) {
        end_moa_chain_group[i][0] = animPoseGetGroupIdx(npc->poseId, g_moamoa_chain_tbl[i].parent);
        end_moa_chain_group[i][1] = animPoseGetGroupIdx(npc->poseId, g_moamoa_chain_tbl[i].child);
    }
    return 2;
}
#endif

/** Script helper: turns off the joint tracking, frees the Smorg silhouette's tables and resets the ext-pose driver. */
// Descriptive name (not in the symbol maps); placeholder unk_end_000000c4, JP end .text+0x0. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/** Script helper: turns off the joint tracking, frees the Smorg silhouette's tables and resets the ext-pose driver. */
USER_FUNC(evt_end_moamoa_exit) {
    NpcEntry* npc = npcNameToPtr("\203\202\203A\203\202\203A");

    animPoseWorldPositionEvalOff(npc->poseId);
    _mapFree(end_moa_tbl);
    extReset();
    _mapFree(end_moa_chain_group);
    _mapFree(end_moa_chain_count);
    return 2;
}
#endif


/**
 * Parade script for the giant Smorg: builds the big Smorg out of 300 small Moamoa as a silhouette,
 * raises it up the screen over 2 seconds, holds it for 3 seconds, lowers it away, and cleans it up.
 */
#if VERSION > VERSION_DEMO_US
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
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -10000, -1000, 2000)
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
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1000, -10000, 2000)
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

/** Float constant -1 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00_neg_one_382 = -1.0f;

/** Float constant pi used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00___local_PI_383 = 3.1415927f;

/** Float constant 3 pi / 2 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00___local_3PIO2_385 = 4.712389f;

f32 fd_end_00_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

/** Float constant pi / 2 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00___local_PIO2_384 = 1.5707964f;

/** Float constant 1 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00_one_381 = 1.0f;

f32 fd_end_00_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

/** Float constant -1 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00_neg_one_365 = -1.0f;

/** Float constant pi used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00___local_PI_366 = 3.1415927f;

/** Float constant 3 pi / 2 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00___local_3PIO2_368 = 4.712389f;

f32 fd_end_00_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

/** Float constant pi / 2 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00___local_PIO2_367 = 1.5707964f;

/** Float constant 1 used by the ending's trigonometric code, placed here by the compiler. */
f32 fd_end_00_one_364 = 1.0f;
#endif
