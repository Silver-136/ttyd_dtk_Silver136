// bom_01 (ボム村, Fahr Outpost): the village square with the mayor and the Bob-omb soldiers, and the cannon launch to
// the Moon with the crowd of Bob-ombs.

#include "version.h"
#include "evt/evt_story.h"
#include "eff/eff_starstone.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "evt/evt_bg.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_ext.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_telop.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "poly_trig.h"
#include "eff/eff_kemutest.h"
#include "driver/mapdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"
#include "mario/mario.h"
#include "driver/envdrv.h"

#include "memory.h"
#include "string.h"
#include "stdlib.h"
#include "system.h"
#include "manager/filemgr.h"
#include "manager/evtmgr_cmd.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "sdk/texPalette.h"
#include "dolphin/gx.h"
#include "dolphin/mtx.h"
#include "win/win_main.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "sound.h"
#include "dolphin/os.h"
#include "driver/effdrv.h"
#include "eff/n64/eff_ripple_n64.h"
#include "stdio.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "driver/extdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_door.h"
#include "evt/evt_sub.h"
#include "bom.h"
#include "data_tables.h"
#include "evt/evt_npc.h"
#include "driver/seqdrv.h"

typedef struct SightWork {
    FileEntry* tpl;
    s32 level[5];
    f32 rot;
    s32 dir;
    s32 count1;
    s32 count2;
} SightWork;
static SightWork* sight_wp;

// make_name's sprintf buffer.
static char name[64];
typedef struct BomAnim {
    char name[8];
    f32 time;
    s32 start;
    s32 count;
} BomAnim;
typedef struct Bom {
    u8 state;
    u8 next;
    Vec target;
    f32 speed;
    f32 gravity;
    s16 timer;
    f32 vy;
    s32 anim;
    u8 pad_24[4];
    OSTime time;
} Bom;
typedef struct BomWork {
    u16 flags;
    s32 count;
    BomAnim anims[8];
    s32 animCount;
    u8 pad_ac[4];
    Bom bom[1000];
} BomWork;
static smartEntry* lp;
static void pose_change(s32 i, const char* name);
static ExtTexInfo ext_data[31];

/**
 * Approximates sin(x) with a short polynomial for x in [0, 2*pi) radians,
 * folding the argument into the first quadrant;
 * used for the crowd Bob-ombs' circle positions and hop directions.
 */
static inline f32 chb_sin(f32 x) {
    f32 y;
    f32 x2;
    f32 r;
    f32 a = 0.00761f;
    f32 b = 0.16605f;
    f32 c = 1.0f;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * x;
            r = a;
        } else {
            y = 1.5707964f - (x - 1.5707964f);
            x2 = y * y;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * y;
            r = a;
        }
    } else if (x < 4.712389f) {
        y = x - 3.1415927f;
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * y;
        a = -a;
        r = a;
    } else {
        y = 1.5707964f - (x - 4.712389f);
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * y;
        a = -a;
        r = a;
    }
    return r;
}

/**
 * Approximates cos(x) with a short polynomial for x in [0, 2*pi) radians,
 * folding the argument into the first quadrant; the cosine partner of chb_sin.
 */
static inline f32 chb_cos(f32 x) {
    f32 y;
    f32 x2;
    f32 r;
    f32 a = 0.03705f;
    f32 b = 0.4967f;
    f32 c = 1.0f;
    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            r = a;
        } else {
            y = 1.5707964f - (x - 1.5707964f);
            x2 = y * y;
            a = a * x2 - b;
            a = a * x2 + c;
            a = -a;
            r = a;
        }
    } else if (x < 4.712389f) {
        y = x - 3.1415927f;
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
        a = -a;
        r = a;
    } else {
        y = 1.5707964f - (x - 4.712389f);
        x2 = y * y;
        a = a * x2 - b;
        a = a * x2 + c;
        r = a;
    }
    return r;
}

static void pose_init(void);
static void pose_main(void);
static void ext_disp(CameraId camera, void* param);
static void depthDisp(CameraId camera, void* param);
void envSetYami(MapTevCallbackInfo* info);

static s32 anim_tevcallback(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(anim_tevcallback);
static s32 depth(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(depth);
s32 evt_mario_dispflag_onoff(EventEntry* event, BOOL isFirstCall);
s32 evt_snd_env_lpf(EventEntry* event, BOOL isFirstCall);
static s32 make_name(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(make_name);
static s32 starstone_entry(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(starstone_entry);
static s32 starstone_set_pos(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(starstone_set_pos);
static s32 starstone_set_ry(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(starstone_set_ry);
static s32 taihou_r(EventEntry* event, BOOL isFirstCall);
static s32 yami_view(EventEntry* event, BOOL isFirstCall);
static USER_FUNC(yami_view);
static void ext_dispent(void);
static void ext_main(void);
static void ext_init(void);

static EvtWord evt_door_1[12];
static EvtWord evt_door_2[12];
static EvtWord evt_wall[12];
static const char* door_npc[4];
static EvtWord door_open[9];
static EvtWord door_close[10];
static DoorParam door;
static BeroEntry bero_entry_data[3];

#if VERSION <= VERSION_DEMO_JP
static EvtWord first_evt[146];
extern EvtWord evt_bom_01_karakara[];
#else
static EvtWord first_evt[148];
EvtWord evt_bom_01_karakara[123];
#endif

static ExtTexInfo ext_data[31];
static EvtWord evt_bom1000[15];
static EvtWord gamen_shake[8];
static EvtWord gamen_shake2[26];
static EvtWord mario_tobideru[54];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord mario_shot[];
#else
EvtWord mario_shot[160];
#endif

static EvtWord effect_delete[11];
static EvtWord mario_kemuri[29];
static EvtWord evt_depth[4];
static EvtWord cam_outside[48];
static EvtWord cam_inside[52];
static EvtWord cam_inside2[52];
static EvtWord party_damage_pose[52];
static EvtWord effect_1[84];
static EvtWord open_hatch[83];
static f32 spline_dt[9];
static const char* oh_data[8];

#if VERSION <= VERSION_DEMO_JP
static EvtWord evt_taihou[2440];
#else
static EvtWord evt_taihou[2446];
#endif

static EvtWord init_soncho[13];
static EvtWord talk_soncho[24];
static EvtWord init_nitoro[8];
static EvtWord talk_nitoro[44];
static EvtWord init_pentrit[8];
static EvtWord talk_pentrit[44];
static EvtWord init_surary[8];
static EvtWord talk_surary[44];
static EvtWord init_murabito_d[8];
static EvtWord talk_murabito_d[44];
static EvtWord init_murabito_e[8];
static EvtWord talk_murabito_e[28];
static EvtWord init_murabito_f[8];
static EvtWord talk_murabito_f[28];
static EvtWord init_murabito_g[8];
static EvtWord talk_murabito_g[28];
static EvtWord init_murabito_h[8];
static EvtWord talk_murabito_h[28];
static EvtWord init_murabito_i[8];
static EvtWord talk_murabito_i[28];
static EvtWord init_white[22];
static EvtWord talk_white[39];
static EvtWord musume_talk[64];
static EvtWord musume_a_entry[26];
static EvtWord musume_b_entry[26];
static EvtWord musume_c_entry[26];
static NpcSetupInfo npcEnt[7];
static EvtWord bom1000_minnnanokoe[21];

#if (VERSION == VERSION_US || VERSION == VERSION_DEMO_US_2)
static EvtWord minnnanokoe_evt[466];
#else
static EvtWord minnnanokoe_evt[450];
#endif

static EvtWord evt_door[14];
EvtWord bom_01_init_evt[189];

/**
 * Draws one textured 2D quad from the cannon-sight texture file (taihou.tpl) at screen position (x, y),
 * scaled by (sx, sy) and rotated by rot degrees; a negative scale mirrors it.
 * Texture 0 is drawn in black with inverse texture alpha.
 */
static void drawBG(s32 id, f32 x, f32 y, f32 sx, f32 sy, f32 rot) {
    CameraEntry* cam = camGetPtr(CAMERA_2D);
    Mtx trans;
    Mtx rotm;
    Mtx scale;
    GXTexObj obj;
    s16 w;
    u16 h;

    TEXGetGXTexObjFromPalette(*sight_wp->tpl->data, &obj, id);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    if (id == 0) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_TEXA, GX_CA_ZERO);
        GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    }
    PSMTXTrans(trans, x, y, 0.0f);
    PSMTXScale(scale, sx, sy, 1.0f);
    PSMTXRotRad(rotm, 'z', MTXDegToRad(rot));
    PSMTXConcat(trans, scale, scale);
    PSMTXConcat(scale, rotm, trans);
    PSMTXConcat(cam->view, trans, trans);
    GXLoadPosMtxImm(trans, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    w = GXGetTexObjWidth(&obj);
    h = GXGetTexObjHeight(&obj);
    GXPosition3s16(-w, h, 0);
    GXTexCoord2s16(0, 0);
    GXPosition3s16(0, h, 0);
    GXTexCoord2s16(1, 0);
    GXPosition3s16(0, 0, 0);
    GXTexCoord2s16(1, 1);
    GXPosition3s16(-w, 0, 0);
    GXTexCoord2s16(0, 1);
    GXEnd();
}

/**
 * Draws a number from the sight texture file one digit at a time, right-aligned:
 * digits glyphs starting at (x, y), at the given scale; the sign is ignored.
 */
static void drawNum(s32 num, s32 digits, f32 x, f32 y, f32 scale) {
    GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
    GXTexObj obj;
    s32 i;
    s32 id;
    f32 w;
    s32 n = num < 0 ? -num : num;

    winTexInit(*sight_wp->tpl->data);
    for (i = 0; i < digits; i++) {
        id = n % 10 + 13;
        TEXGetGXTexObjFromPalette(*sight_wp->tpl->data, &obj, id);
        w = scale * GXGetTexObjWidth(&obj);
        w += 2.0f;
        winTexSet(id, (Vec){x + w * (digits - i - 1), y, 0.0f},
                  (Vec){scale, scale, scale}, color);
        n /= 10;
    }
}

// taihou.tpl textures of the direction arrows (drawBG ids).
typedef enum SightArrowTex {
    SIGHT_TEX_ARROW_LIT = 6,
    SIGHT_TEX_ARROW = 7
} SightArrowTex;

/**
 * Draws the cannon's targeting display (taihou.tpl) over the aiming part of evt_taihou ("Sighting! Target is the
 * moon!"): corner frames that spin with the sight direction, blinking inner frames, the four direction arrows
 * with the one set by sight_dir lit (arrows 2 and 3 tick a counter up while lit), a five-bar level meter fed by the game's sound
 * output (sound.samples), and running number readouts made from the render time and retrace count.
 * sight queues it only while the direction is not 5 (idle).
 */
static void disp_sight(CameraId camera, void* param) {
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor black = {0x00, 0x00, 0x00, 0xFF};
    s32 i;
    s32 j;
    s32 n;
    s32 id;
    int on;
    f32 x;
    f32 y;

    drawBG(0, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
    drawBG(0, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
    drawBG(0, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f);
    drawBG(0, 0.0f, 0.0f, -1.0f, -1.0f, 0.0f);
    drawBG(1, 0.0f, 0.0f, 1.0f, 1.0f, sight_wp->rot);
    drawBG(1, 0.0f, 0.0f, -1.0f, 1.0f, -sight_wp->rot);
    drawBG(1, 0.0f, 0.0f, 1.0f, -1.0f, -sight_wp->rot);
    drawBG(1, 0.0f, 0.0f, -1.0f, -1.0f, sight_wp->rot);
    if (sight_wp->dir == 2) {
        sight_wp->rot += 2.0f;
    } else if (sight_wp->dir == 3) {
        sight_wp->rot -= 2.0f;
    } else if (sight_wp->dir == 0) {
        sight_wp->rot -= 2.0f;
    } else {
        sight_wp->rot += 1.0f;
    }

    winTexInit(*sight_wp->tpl->data);
    winTexSet(5, (Vec){0.0f, 0.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, black);
    for (j = 0; j < 3; j++) {
        if ((s32)(gp->retraceCount % 30) / 10 != j) {
            i = 4 - j;
            drawBG(i, 100.0f, 60.0f, 1.0f, 1.0f, 0.0f);
            drawBG(i, -100.0f, 60.0f, -1.0f, 1.0f, 0.0f);
            drawBG(i, 100.0f, -60.0f, 1.0f, -1.0f, 0.0f);
            drawBG(i, -100.0f, -60.0f, -1.0f, -1.0f, 0.0f);
        }
    }

    winTexInit(*sight_wp->tpl->data);
    winTexSet(23, (Vec){-200.0f, 150.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    drawNum((gp->renderTime / 100) / 1000, 10, 90.0f, 180.0f, 0.6f);
    drawNum((gp->renderTime / 200) / 1000, 8, 126.0f, 150.0f, 0.6f);
    drawNum((gp->renderTime / 300) / 1000, 6, 162.0f, 120.0f, 0.6f);
    drawNum((gp->renderTime / 400) / 1000, 6, 162.0f, 90.0f, 0.6f);
    drawNum(gp->retraceCount / 4, 4, 110.0f, -160.0f, 1.0f);
    drawNum(gp->retraceCount / 8, 2, 230.0f, -170.0f, 0.8f);

    winTexSet(8, (Vec){-210.0f, -140.0f, 0.0f}, (Vec){1.0f, 1.0f, 1.0f}, white);
    if ((gp->retraceCount & 3) == 0) {
        for (i = 0; i < 4; i++) {
            sight_wp->level[i] = sight_wp->level[i + 1];
        }
        sight_wp->level[4] = abs(sound.samples[sound.sampleBank][sound.sampleIndex]);
    }
    for (i = 0; i < 5; i++) {
        n = sight_wp->level[i];
        n /= 300;
        if (n > 7) {
            n = 7;
        }
        x = (-240.0f) + 20.0f * i;
        for (j = 0; j < n; j++) {
            y = (-190.0f) + 16.0f * j;
            if (j == 6) {
                drawBG(10, x, y, 1.0f, 1.0f, 0.0f);
            } else {
                drawBG(9, x, y, 1.0f, 1.0f, 0.0f);
            }
        }
        for (j = n; j < 7; j++) {
            y = (-190.0f) + 16.0f * j;
            if (j == 6) {
                drawBG(12, x, y, 1.0f, 1.0f, 0.0f);
            } else {
                drawBG(11, x, y, 1.0f, 1.0f, 0.0f);
            }
        }
    }

    winTexInit(*sight_wp->tpl->data);
    for (i = 0; i < 4; i++) {
        on = (gp->retraceCount % 20 > 6 && i == sight_wp->dir) ? 1 : 0;

        id = on ? SIGHT_TEX_ARROW_LIT : SIGHT_TEX_ARROW;
        if (sight_wp->dir == 4) {
            on = 0;
            id = SIGHT_TEX_ARROW;
        }
        switch (i) {
            case 0:
                drawBG(id, 16.0f, 180.0f, 1.0f, 1.0f, 90.0f);
                break;
            case 1:
                drawBG(id, -16.0f, -180.0f, 1.0f, 1.0f, 270.0f);
                break;
            case 2:
                drawBG(id, -180.0f, 16.0f, 1.0f, 1.0f, 180.0f);
                if (on && (gp->retraceCount & 3) == 0) {
                    sight_wp->count1++;
                }
                break;
            case 3:
                drawBG(id, 180.0f, -16.0f, 1.0f, 1.0f, 0.0f);
                if (on && (gp->retraceCount & 3) == 0) {
                    sight_wp->count2++;
                }
                break;
        }
    }
    drawNum(sight_wp->count1, 2, -220.0f, 0.0f, 0.8f);
    drawNum(sight_wp->count2, 2, 200.0f, 0.0f, 0.8f);
}

/**
 * Script helper started from the map init: loads taihou.tpl, allocates the SightWork state and,
 * once the sight direction is not idle (5), queues disp_sight every frame until the direction is set to 6,
 * which frees everything and ends it.
 */
static s32 sight(EventEntry* event, BOOL isFirstCall) {
    s32 i;

    if (isFirstCall) {
        sight_wp = _mapAlloc(sizeof(SightWork));
        memset(sight_wp, 0, sizeof(SightWork));
        for (i = 0; i < 5; i++) {
            sight_wp->level[i] = 0;
        }
        sight_wp->count1 = rand();
        sight_wp->count2 = rand();
        sight_wp->dir = 5;
    }
    if (sight_wp->tpl == NULL) {
        if (fileAsyncf(FILE_TYPE_TPL, NULL, "%s/%s", getMarioStDvdRoot(), "taihou.tpl") == NULL) {
            return EVT_RETURN_BLOCK;
        }
        sight_wp->tpl = fileAllocf(FILE_TYPE_TPL, "%s/%s", getMarioStDvdRoot(), "taihou.tpl");
    }
    if (sight_wp->dir == 6) {
        fileFree(sight_wp->tpl);
        _mapFree(sight_wp);
        return EVT_RETURN_DONE;
    }
    if (sight_wp->dir != 5) {
        dispEntry(CAMERA_2D, 800.0f, DISP_MODE_OPAQUE, disp_sight, NULL);
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: sets the sight overlay's direction state to args[0] (0 to 3 are the arrows, 4 none lit,
 * 5 idle and hidden, 6 shut the overlay down).
 */
static s32 sight_dir(EventEntry* event, BOOL isFirstCall) {
    sight_wp->dir = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] switches the bits in args[1] on
 * or off in the crowd work flags (bit 0 hides the crowd and mutes the Bob-omb hop sounds).
 */
static s32 bom_flag(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    s32 mask = evtGetValue(event, args[1]);
    BomWork* wp = lp->address;

    if (on) {
        wp->flags |= mask;
    } else {
        wp->flags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: puts every crowd Bob-omb (Bom state 127) at a random spot high above the village
 * and switches them to the falling pose S_2, so they drift down as a slow rain of bombs.
 */
static s32 bom_fall(EventEntry* event, BOOL isFirstCall) {
    BomWork* wp = lp->address;
    ExtdrvPose* pose = extGetPosePtr();
    s32 i;

    for (i = 0; i < wp->count; i++, pose++) {
        wp->bom[i].state = 127;
        (*pose->mtx)[0][3] = 600.0f * rand() / 32767.0f;
        (*pose->mtx)[1][3] = 300.0f + 3700.0f * rand() / 32767.0f;
        (*pose->mtx)[2][3] = -650.0f + -450.0f * rand() / 32767.0f;
        pose_change(i, "S_2");
    }
    return EVT_RETURN_DONE;
}

/**
 * Builds the crowd's animation table (name, start frame, frame count, duration) from the c_bom_sen entries in
 * ext_data, one table row per run of identical animation names.
 */
static void pose_init(void) {
    BomWork* wp;
    ExtTexInfo* def;
    char buf[32];
    s32 i;
    s32 start;
    i = 0;
    wp = lp->address;
    def = ext_data;
    strcpy(buf, "");
    wp->animCount = -1;
    do {
        if (strcmp(def->anim, buf) != 0) {
            strcpy(buf, def->anim);
            wp->animCount++;
            start = i;
        }
        strcpy(wp->anims[wp->animCount].name, buf);
        wp->anims[wp->animCount].start = start;
        wp->anims[wp->animCount].count = i - start + 1;
        i++;
        wp->anims[wp->animCount].time = def->frame;
    } while ((++def)->group != NULL);
    wp->animCount++;
}

/**
 * Per-frame update of the crowd: turns each Bob-omb's elapsed time into a frame of its current animation
 * and stores it in the extdrv pose texId.
 */
static void pose_main(void) {
    OSTime now;
    BomWork* wp = lp->address;
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();
    f32 t;

    for (i = 0; i < wp->count; i++, pose++) {
        BomAnim* a = &wp->anims[wp->bom[i].anim];
        now = gp->renderTime;

        t = 60.0f * OSTicksToMilliseconds((u64)(now - wp->bom[i].time)) / 1000.0f / a->time;
        if (t >= 1.0f) {
            wp->bom[i].time = now;
            t = 0.0f;
        }
        pose->texId = a->start + (s32)((a->count - 1) * t);
    }
}

/**
 * Switches crowd Bob-omb i to the animation called name (looked up in the table from pose_init),
 * restarting its timer at a random offset so the crowd does not animate in lockstep.
 */
static void pose_change(s32 i, const char* name) {
    BomWork* wp = lp->address;
    s32 j = 0;
    BomAnim* a;
    for (a = wp->anims; j < wp->animCount; j++, a++) {
        if (strcmp(a->name, name) == 0) {
            break;
        }
    }
    if (j < wp->animCount && wp->bom[i].anim != j) {
        wp->bom[i].anim = (s16)j;
        wp->bom[i].time = gp->renderTime + rand();
    }
}

/**
 * Script helper: fills ext_data with the c_bom_sen (Bob-omb crowd) sprite frames for the cannon scene:
 * ten frames of "R_1" followed by twenty of "S_2".
 */
static s32 make_ext_table(EventEntry* event, BOOL isFirstCall) {
    s32 i;

    memset(ext_data, 0, sizeof(ext_data));
    for (i = 0; i < 10; i++) {
        ext_data[i].group = "c_bom_sen";
        ext_data[i].anim = "R_1";
        ext_data[i].frame = 2.0f * i;
    }
    for (i = 0; i < 20; i++) {
        ext_data[10 + i].group = "c_bom_sen";
        ext_data[10 + i].anim = "S_2";
        ext_data[10 + i].frame = 6.0f * i;
    }
    ext_data[30].group = NULL;
    return EVT_RETURN_DONE;
}

/**
 * Door callback for the house door: LW(0) is the opening angle;
 * scales it by 0.5546875 and turns the door object s_ie_door around the Y axis.
 */
static EVT_BEGIN(evt_door_1)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5546875f))
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_door"), FLOAT(0.0f), LW(0), FLOAT(0.0f))
    RETURN
EVT_END()

/** Door-swing callback: scales LW(0) by 0.5 and applies it as the s_ie_door map object's X rotation. */
static EVT_BEGIN(evt_door_2)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_door"), LW(0), FLOAT(0.0f), FLOAT(0.0f))
    RETURN
EVT_END()

/** Wall-swing callback: scales LW(0) by -0.5 and applies it as the s_ie_kai map object's Y rotation. */
static EVT_BEGIN(evt_wall)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.5f))
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_kai"), FLOAT(0.0f), LW(0), FLOAT(0.0f))
    RETURN
EVT_END()

/** Actors that pass through the village house door during its scenes: General White and his crew. */
static const char* door_npc[] = {
    /*"ホワイト"*/"\203z\203\217\203C\203g",
    /*"ペントリット"*/"\203y\203\223\203g\203\212\203b\203g",
    /*"スラリー"*/"\203X\203\211\203\212\201[",
    NULL,
};

/** House door open callback: dips the ambience to 50 and low-pass filters it (the muffled indoor sound). */
static EVT_BEGIN(door_open)
    CALL(evt_snd_envoff, PSND_SILENT | PSND_SILENT_LEVEL(4))
    CALL(evt_snd_env_lpf, 0, 800)
    RETURN
EVT_END()

/** House door close callback: resumes the ambience from the dip and removes the low-pass filter. */
static EVT_BEGIN(door_close)
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_UNSILENT, 0)
    CALL(evt_snd_env_lpf, 0, 0)
    RETURN
EVT_END()

/** The village house door on the square, with the map objects it swaps and its open and close handlers. */
static DoorParam door = {
    0, // flags
    3, // type
    1, // se
    "s_ie_door", // mapobj
    "A_ie_door_u", // hitIn
    "A_ie_door", // hitOut
    "s_ie_kai", // outside
    NULL, // outsideHit
    "S_ie_naka", // inside
    NULL, // insideHit
    evt_door_1, // evtIn
    evt_door_2, // evtOut
    evt_wall, // evtWall
    door_npc, // npcs
    NULL, // mobjs
    NULL, // items
    door_open, // evtOpen
    door_close, // evtClose
};

/** Exits of this room: the west door to bom_04 and the east door to bom_02. */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "bom_04", // nextMapName
        "e_bero4", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero", // hitName
        0, 0, // type..sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "bom_02", // nextMapName
        "w_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(first_evt)
    CALL(evt_snd_bgm_scope, 0, 1)
    CALL(evt_cam_letter_box_onoff, 1, 1)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_mario_normalize)
#endif
    CALL(evt_cam3d_evt_set, 340, 100, -53, 340, 80, -302, 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pos, -580, 0, 0)
    CALL(evt_party_set_pos, 0, -580, 0, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    BEGIN_INLINE_EVENT
        WAIT_MS(6000)
        CALL(evt_telop_entry, 13)
    END_INLINE_EVENT
    WAIT_MS(4000)
    CALL(evt_cam3d_evt_set, 200, 150, 1001, -40, 130, 348, 6000, EASING_SINE_IN_OUT)
    WAIT_MS(6000)
#if VERSION >= VERSION_JP
    CALL(evt_mario_normalize)
#endif
    CALL(evt_mario_mov_pos2, -351, 0, FLOAT(120.0f))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -380, 97, 267, -380, 38, -9, 0, EASING_SINE_IN_OUT)
    WAIT_MS(300)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_party_stop, 0)
    WAIT_MS(200)
    IF_INT_EQUAL(GSWF_BOM_01_EARLY_VISIT, 1)
        CALL(evt_msg_print_party, STRING("stg7_bom_01__2_00"))
    ELSE
        CALL(evt_msg_print_party, STRING("stg7_bom_01__2"))
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    SET_VALUE(GSW_STORY_PROGRESS, STG7_01_FAHR_OUTPOST_1ST_VISIT)
#if VERSION <= VERSION_DEMO_JP
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(bero_case_switch_on)
#else
    CALL(evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    SET_VALUE(LW(0), STRING("w_bero"))
    CALL_CHILD(bero_case_switch_on)
#endif
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_snd_bgm_scope, 0, 0)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_bom_01_karakara)
    SET_VALUE(LW(9), LW(0))
    SET_VALUE(LW(8), 0)
    LOOP(0)
        CALL(evt_mapobj_get_position, LW(9), LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_AMB_WINDVANE1"), LW(0), LW(1), LW(2), LW(10))
#endif
        CALL(evt_sub_intpl_msec_init, EASING_SINE_OUT, 1, 10, 1000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        SUBTRACT(LW(8), LW(0))
        IF_INT_LESS(LW(8), 0)
            ADD(LW(8), 360)
        END_IF
        CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(8), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_sub_random, 10, LW(5))
        ADD(LW(5), 1)
        MULTIPLY(LW(5), 30)
        LOOP(LW(5))
            SUBTRACT(LW(8), LW(0))
            IF_INT_LESS(LW(8), 0)
                ADD(LW(8), 360)
            END_IF
            CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(8), 0)
            WAIT_FRAMES(1)
        END_LOOP
        BEGIN_INLINE_EVENT
            WAIT_MS(900)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_snd_sfxoff, LW(10))
#endif
        END_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_SINE_OUT, 10, 1, 1000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        SUBTRACT(LW(8), LW(0))
        IF_INT_LESS(LW(8), 0)
            ADD(LW(8), 360)
        END_IF
        CALL(evt_mapobj_rotate, 1, LW(9), 0, LW(8), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
    END_LOOP
    RETURN
EVT_END()

// Kept (pattern): propagation and lifetimes off for the extdrv, npc_jump and depthDisp functions of bom_01, eki_00,
// gor_10, hei_10, rsh_05, rsh_06, tik_05 and tou_02 (one half is a -opt file flag where configure.py sets it); the
// target needs it and no natural form was found (cleanup_notes/mweppc_pilot.md, pragma review rels).
#pragma opt_propagation off
#pragma opt_lifetimes off

/**
 * Extension-draw init for the 1000-Bob-omb crowd of the cannon scene:
 * allocates BomWork and scatters every sprite on a ring 500 to 900 units from the origin in the "R_1" pose.
 */
static void ext_init(void) {
    s32 i;
    BomWork* wp;
    ExtdrvPose* pose;
    f32 angle;
    f32 dist;

    lp = smartAlloc(sizeof(BomWork), SMART_GROUP_MAP);
    wp = lp->address;
    memset(wp, 0, sizeof(BomWork));
    wp->count = extGetPoseNum();
    pose_init();

    pose = extGetPosePtr();
    for (i = 0; i < wp->count; i++, pose++) {
        wp->bom[i].vy = 0.0f;
        wp->bom[i].gravity = 0.0f;
        wp->bom[i].state = 0;
        pose->flags |= (EXT_POSE_FLAG_VISIBLE | EXT_POSE_FLAG_BILLBOARD | EXT_POSE_FLAG_NO_SHADOW);
        PSMTXIdentity(*pose->mtx);
        angle = 6.2831855f * rand() / 32767.0f;
        dist = 500.0f + 400.0f * rand() / 32767.0f;
        (*pose->mtx)[0][3] = dist * chb_sin(angle);
        (*pose->mtx)[1][3] = 0.0f;
        (*pose->mtx)[2][3] = dist * chb_cos(angle);
        pose_change(i, "R_1");
    }
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

/** Draw callback for the crowd sprites: loads render mode, vertices, texture and TEV, then draws all poses. */
static void ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

/**
 * Starts a hop for crowd Bob-omb i towards (x, y, z) at speed per frame under gravity;
 * computes the flight time and vertical speed, faces the target
 * and plays the "bombs together" sound for every 20th one. next is the state it takes on landing.
 */
static void jump(s32 i, s32 next, f32 x, f32 y, f32 z, f32 speed, f32 gravity) {
    BomWork* wp = lp->address;
    ExtdrvPose* pose = extGetPosePtr() + i;
    f32 dy;
    f32 dist;

    wp->bom[i].target = (Vec){x, y, z};
    wp->bom[i].speed = speed;
    wp->bom[i].gravity = gravity;
    dy = y - (*pose->mtx)[1][3];
    dist = distABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], x, z);
    angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], x, z);
    wp->bom[i].timer = dist / wp->bom[i].speed;
    wp->bom[i].vy = dy / wp->bom[i].timer + wp->bom[i].gravity * wp->bom[i].timer / 2;
    wp->bom[i].next = next;
    wp->bom[i].state = 100;
    pose->dir = angleABf(x, z, (*pose->mtx)[0][3], (*pose->mtx)[2][3]);
    if (i % 20 == 0 && !(wp->flags & 1)) {
        Vec pos = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};

        // sound name passed as an integer word, as in the original (only loops show it; see sfx_name_word.h)
        psndSFXOn_3D(SFX_NAME_WORD("SFX_STG7_BOMB_TOGETHER2"), &pos);
    }
}

// Kept (pattern): propagation and lifetimes off for the extdrv, npc_jump and depthDisp functions of bom_01, eki_00,
// gor_10, hei_10, rsh_05, rsh_06, tik_05 and tou_02 (one half is a -opt file flag where configure.py sets it); the
// target needs it and no natural form was found (cleanup_notes/mweppc_pilot.md, pragma review rels).
#pragma opt_propagation off
#pragma opt_lifetimes off

/**
 * Per-frame logic of the crowd: state 0 walks each Bob-omb towards the middle,
 * states 1 to 3 hop it onto the pile in three stages, 100 runs the hop arc,
 * and 127 makes it sink and sway while falling (with an occasional falling sound).
 */
static void ext_main(void) {
    s32 i;
    BomWork* wp = lp->address;
    ExtdrvPose* pose = extGetPosePtr();
    f32 z;
    f32 speed;
    f32 gravity;
    f32 val;
    f32 dist;
    f32 angle;
    f32 rad;
    Mtx m1;
    Mtx m2;
    Mtx m3;

    for (i = 0; i < wp->count; i++, pose++) {
        Vec center = {0.0f, 0.0f, 0.0f};
        Vec pos = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};
        Vec move = {0.0f, 0.0f, 0.0f};
        Vec dir;

        switch ((s8)wp->bom[i].state) {
            case 0:
                val = PSVECDistance(&center, &pos) ; if (val > 280.0f) {
                    PSVECSubtract(&center, &pos, &dir);
                    PSVECNormalize(&dir, &dir);
                    PSVECScale(&dir, &move, 3.0f);
                    PSVECAdd(&pos, &move, &pos);
                    (*pose->mtx)[0][3] = pos.x;
                    (*pose->mtx)[1][3] = pos.y;
                    (*pose->mtx)[2][3] = pos.z;
                    pose->dir = angleABf(0.0f, 0.0f, pos.x, pos.z);
                } else {
                    wp->bom[i].state++;
                }
                break;
            case 1:
                angle = ( 6.2831855f * rand() ) / 32767.0f;
                dist = 150.0f + (( 30.0f * rand() ) / 32767.0f) ;
                gravity = 0.1f + (( 0.1f * rand() ) / 32767.0f) ;
                speed = 3.0f + ( rand() / 32767.0f) ;
                z = dist * ( -chb_cos(angle) );
                jump(i, 2, dist * chb_sin(angle), 150.0f, z, speed, gravity);
                break;
            case 2:
                angle = ( 6.2831855f * rand() ) / 32767.0f;
                dist = 50.0f + (( 30.0f * rand() ) / 32767.0f) ;
                z = 0.1f + (( 0.1f * rand() ) / 32767.0f);
                speed = 3.0f + ( rand() / 32767.0f) ;
                gravity = dist * ( -chb_cos(angle) );
                dist = dist * chb_sin(angle);
                jump(i, 3, dist, 400.0f, gravity, speed, z);
                break;
            case 3:
                angle = ( 6.2831855f * rand() ) / 32767.0f;
                dist = ( 70.0f * rand() ) / 32767.0f;

                // z stays 0.1f (+196) and speed 3.0f: both rand() results are unused, as in the game
                rand();
                z = 0.1f;
                rand();
                speed = 3.0f;
                gravity = dist * ( -chb_cos(angle)) ;
                dist = ( dist * chb_sin(angle) ) + (100.0f + ( i % 100) );
                jump(i, 4, dist, 350.0f + ( i % 150) , gravity, speed, z);
                break;
            case 4:
                wp->bom[i].state++;
                break;
            case 5:
                break;
            case 100:
                (*pose->mtx)[1][3] += wp->bom[i].vy;
                wp->bom[i].vy -= wp->bom[i].gravity;
                rad = ( 3.1415927f * angleABf((*pose->mtx)[0][3], (*pose->mtx)[2][3], wp->bom[i].target.x, wp->bom[i].target.z) ) / 180.0f;
                (*pose->mtx)[0][3] += wp->bom[i].speed * chb_sin(rad);
                (*pose->mtx)[2][3] += wp->bom[i].speed * ( -chb_cos(rad)) ;
                if ( (--wp->bom[i].timer ) < 0) {
                    (*pose->mtx)[0][3] = wp->bom[i].target.x;
                    (*pose->mtx)[1][3] = wp->bom[i].target.y;
                    (*pose->mtx)[2][3] = wp->bom[i].target.z;
                    wp->bom[i].state = wp->bom[i].next;
                }
                break;
            case 127:
                if ((*pose->mtx)[1][3] > ( -1000.0f) ) { // tests y, not z, as in the game
                    (*pose->mtx)[1][3] = (*pose->mtx)[1][3] - 2.0f;
                    pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                    PSMTXTrans(m1, 0.0f, -20.0f, 0.0f);
                    val = (f32)sin((i + gp->retraceCount) / 100.0f);
                    PSMTXRotRad(m2, 'z', 0.017453292f * (10.0f * val));
                    PSMTXTrans(m3, 0.0f, 20.0f, 0.0f);
                    PSMTXConcat(m1, m2, m2);
                    PSMTXConcat(m2, m3, m3);
                    PSMTXTrans(m1, (*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]);
                    PSMTXConcat(m1, m3, *pose->mtx);
                    if (irand(100) == 0) {
                        Vec p = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};

                        // sound name passed as an integer word, as in the original (only loops show it; see sfx_name_word.h)
                        psndSFXOn_3D(SFX_NAME_WORD("SFX_STG7_BOMB_FALL1"), &p);
                    }
                }
                break;
        }
    }
    pose_main();
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

/** Queues ext_disp every frame for the crowd unless bit 0 of the BomWork flags is set (it also mutes the hops). */
static void ext_dispent(void) {
    BomWork* wp = lp->address;

    if (!(wp->flags & 1)) {
        dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, ext_disp, NULL);
    }
}

/**
 * Sprite animation table for the Bob-omb crowd (c_bom_sen); it is filled in at run time for the cannon
 * scene.
 */
static ExtTexInfo ext_data[] = {
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
};

/**
 * Registers the 1000-sprite Bob-omb crowd of the cannon scene (make_ext_table frames,
 * ext_init, ext_main, ext_dispent).
 */
static EVT_BEGIN(evt_bom1000)
    CALL(make_ext_table)
    CALL(evt_npc_entry, STRING("dummy"), STRING("c_bom_sen")) // hidden Bob-omb NPC behind the crowd sprites
    CALL(evt_ext_entry, 1000, PTR(ext_data), PTR(ext_init), PTR(ext_main), PTR(ext_dispent))
    RETURN
EVT_END()

/** Small camera shake (amplitude 0.002) used while the floor and the cannon move. */
static EVT_BEGIN(gamen_shake)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.001953125f), FLOAT(0.0f), 0)
    RETURN
EVT_END()

/** Heavy camera shake that dies down in four steps (0.02, 0.01, 0.005, 0) of 100 each; the cannon's recoil. */
static EVT_BEGIN(gamen_shake2)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.01953125f), FLOAT(0.0f), 100)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 100)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 100)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0f), 100)
    RETURN
EVT_END()

/**
 * Moves Mario up and to the right along a 45 degree line from (400, 780) over 0.3 s,
 * with the partner trailing 30 units behind; Mario flying out of the cannon.
 */
static EVT_BEGIN(mario_tobideru)
    CALL(evt_mario_set_pos, 400, 780, 0)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, 400, 300)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(LW(2), 400)
    SET_VALUE(LW(3), 780)
    ADD(LW(2), LW(0))
    ADD(LW(3), LW(0))
    CALL(evt_mario_set_pos, LW(2), LW(3), 0)
    SUBTRACT(LW(2), 30)
    SUBTRACT(LW(3), 30)
    CALL(evt_party_set_pos, 0, LW(2), LW(3), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    RETURN
EVT_END()

/**
 * The cannon firing: starts Mario's flight, plays the discharge sound,
 * kicks the barrel (dekatai) back and forth, shakes the screen and spawns the smoke,
 * ripple and particle effects at the muzzle.
 */
EVT_BEGIN(mario_shot)
    SPAWN_EVENT(mario_tobideru)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_DISCHARGE1"), 0)
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_IN, 0, -50, 150)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 1, STRING("dekatai"), 0, 0, -40)
    CALL(evt_mapobj_trans, 1, STRING("dekatai"), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SPAWN_EVENT(gamen_shake2)
    CALL(evt_eff, STRING("kemuri"), STRING("kemuri_test"), 11, 570, 950, 0, FLOAT(3.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, STRING("ripple"), STRING("ripple_n64"), 2, 570, 950, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff, STRING("particle"), STRING("particle"), 0, 520, 900, 0, 1, 1, 0, 100, 0, 0, 0, 0)
    CALL(taihou_r)
#if VERSION >= VERSION_JP
    BEGIN_INLINE_EVENT
        CALL(evt_eff, 0, STRING("hit"), 1, 570, 950, 0, FLOAT(3.0f), 4, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
        CALL(evt_eff, 0, STRING("hit"), 1, 570, 950, 0, FLOAT(3.0f), 4, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
#endif
    CALL(evt_sub_intpl_msec_init, EASING_CUBIC_OUT, -50, 0, 150)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_rotate, 1, STRING("dekatai"), 0, 0, -40)
    CALL(evt_mapobj_trans, 1, STRING("dekatai"), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    RETURN
EVT_END()

/** Removes the smoke, ripple and particle effects spawned by mario_shot. */
static EVT_BEGIN(effect_delete)
    CALL(evt_eff_delete, STRING("kemuri"))
    CALL(evt_eff_delete, STRING("ripple"))
    CALL(evt_eff_delete, STRING("particle"))
    RETURN
EVT_END()

/**
 * Runs forever, puffing a smoke effect at Mario's position every 2 frames (used while Mario is fired through the
 * air in the cannon scene).
 */
static EVT_BEGIN(mario_kemuri)
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_eff, 0, STRING("kemuri_test"), 12, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(2)
    END_LOOP
    RETURN
EVT_END()

/** Starts the depth (screen blur) display; it runs until its script event is deleted. */
static EVT_BEGIN(evt_depth)
    CALL(depth)
    RETURN
EVT_END()

/**
 * Camera/map setup for being outside the house:
 * resets the blend colours, hides the interior (S_ie_naka) and shows the exterior wall.
 */
static EVT_BEGIN(cam_outside)
    CALL(evt_map_set_blend, 0, 255, 255, 255, 255)
    CALL(evt_map_set_blend, 1, 255, 255, 255, 255)
    CALL(evt_map_blend_off, 0)
    CALL(evt_map_blend_off, 1)
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_door"), 0, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_kai"), 0, 0, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_ie_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("s_ie_kai"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/**
 * Camera/map setup for the first interior view of the house:
 * sets the camera, darkens the blend colours, opens the door and hides the exterior wall (s_ie_kai)
 * so the inside can be seen.
 */
static EVT_BEGIN(cam_inside)
    CALL(evt_cam3d_evt_set, -160, 125, -100, 10, 55, -427, 0, EASING_SINE_IN_OUT)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 0)
    CALL(evt_map_set_blend, 1, 0, 0, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_door"), 90, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_kai"), 0, -90, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_ie_kai"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/** Same as cam_inside with a different, closer camera position. */
static EVT_BEGIN(cam_inside2)
    CALL(evt_cam3d_evt_set, -93, 104, -250, 9, 62, -447, 0, EASING_SINE_IN_OUT)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 0)
    CALL(evt_map_set_blend, 1, 0, 0, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_door"), 90, 0, 0)
    CALL(evt_mapobj_rotate, 1, STRING("s_ie_kai"), 0, -90, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_ie_naka"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_ie_kai"), MAP_OBJ_FLAG_HIDDEN)
    RETURN
EVT_END()

/**
 * Puts the current partner into its damage pose (the D_1 pose of Goombella, Koops, Flurrie, Yoshi, Vivian,
 * Bobbery or Ms. Mowz, chosen by evt_mario_get_party).
 */
static EVT_BEGIN(party_damage_pose)
    CALL(evt_mario_get_party, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
            CALL(evt_party_set_pose, 0, STRING("PKR_D_1"))
        CASE_EQUAL(2)
            CALL(evt_party_set_pose, 0, STRING("PNK_D_1"))
        CASE_EQUAL(3)
            CALL(evt_party_set_pose, 0, STRING("D_1"))
        CASE_EQUAL(4)
            CALL(evt_party_set_pose, 0, STRING("PYS_D_1"))
        CASE_EQUAL(5)
            CALL(evt_party_set_pose, 0, STRING("PWD_D_1"))
        CASE_EQUAL(6)
            CALL(evt_party_set_pose, 0, STRING("PTR_D_1"))
        CASE_EQUAL(7)
            CALL(evt_party_set_pose, 0, STRING("PCH_D_1"))
    END_SWITCH
    RETURN
EVT_END()

/** Rings of dust puffs (kemuri_test) along the edges of a 440 by 440 square on the ground, one every 20 units. */
static EVT_BEGIN(effect_1)
    SET_VALUE(LW(0), -220)
    LOOP(22)
        CALL(evt_eff, 0, STRING("kemuri_test"), 2, LW(0), 5, -220, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_eff, 0, STRING("kemuri_test"), 2, LW(0), 5, 220, FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        ADD(LW(0), 20)
    END_LOOP
    SET_VALUE(LW(0), -220)
    LOOP(22)
        CALL(evt_eff, 0, STRING("kemuri_test"), 2, -220, 5, LW(0), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_eff, 0, STRING("kemuri_test"), 2, 220, 5, LW(0), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        ADD(LW(0), 20)
    END_LOOP
    RETURN
EVT_END()

/**
 * Slides two hatch or shutter objects apart over half a second with a shutter sound and a screen shake.
 * LW(0) and LW(1) name the objects, LW(2) is 0 to slide along Z and anything else along X.
 */
static EVT_BEGIN(open_hatch)
    CALL(evt_mapobj_get_position, LW(1), LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_SHUTTER_OPEN1"), LW(10), LW(11), LW(12), 0)
    SET_VALUE(LW(8), LW(0))
    SET_VALUE(LW(9), LW(1))
    SET_VALUE(LW(7), LW(2))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 180, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    IF_INT_EQUAL(LW(7), 0)
        CALL(evt_mapobj_trans, 1, LW(9), 0, 0, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, LW(8), 0, 0, LW(0))
    ELSE
        CALL(evt_mapobj_trans, 1, LW(9), LW(0), 0, 0)
        MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
        CALL(evt_mapobj_trans, 1, LW(8), LW(0), 0, 0)
    END_IF
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    SPAWN_EVENT(gamen_shake2)
    RETURN
EVT_END()

/** Control points of Mario's flight path (a spline) used in the cannon launch scene. */
static f32 spline_dt[] = {
    -200.0f, 0.0f, 650.0f, 590.0f, 200.0f, -2150.0f, 1380.0f, 100.0f,
    -4950.0f,
};

/** The cannon crew: each name with the message key of his drill call (stg7_bom_65_a to d). */
static const char* oh_data[] = {
    /*"\203j\203g\203\215"*/"\203j\203g\203\215", "stg7_bom_65_a",
    /*"\203y\203\223\203g\203\212\203b\203g"*/"\203y\203\223\203g\203\212\203b\203g", "stg7_bom_65_b",
    /*"\203X\203\211\203\212\201["*/"\203X\203\211\203\212\201[", "stg7_bom_65_c",
    /*"\203{\203\200\203w\203CD"*/"\203{\203\200\203w\203CD", "stg7_bom_65_d",
};

/**
 * The cannon scene (taihou = cannon) started by talking to the mayor before STG7_05_THE_CANNON:
 * the mayor places Mario, the floor opens and Mario and the partner (in its damage pose) drop below;
 * the mayor asks General White whether the big bomb cannon is ready (stg7_bom_63 to 64_01),
 * and White's crew (ニトロ, ペントリット, スラリー and ボムヘイD, oh_data) load and aim it on his orders
 * (stg7_bom_65 to 86: guidance system online, hatches and shutters open, sighting the moon with the disp_sight
 * display) while the camera switches between the room and the outside and the screen shakes;
 * the mayor orders everyone in and fires it (stg7_bom_81, 87), Mario is fired from the cannon,
 * the Goldbob guide (ITEM_GOLDBOB_GUIDE) is taken from the pouch and the map changes to moo_03.
 */
static EVT_BEGIN(evt_taihou)
    CALL(evt_mario_key_onoff, 0)

    // The scene starts in the cannon room; the music stops.
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_msg_print, 0, STRING("stg7_bom_63"), 0, STRING("me"))
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos, 0, 10, 0, 500)
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos, 0, 10, 500)
    CALL(evt_cam3d_evt_set, 0, 152, 555, 0, 57, 13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("chika_off"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("jime_ugo"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yuka1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_yuka2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
    WAIT_FRAMES(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    CALL_CHILD(party_damage_pose)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_FLOOR_OPEN1"), LW(10), LW(11), LW(12), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_OUT, 0, 20, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("hiraku_ss_1_b"), LW(0), -5, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("hiraku_ss_1_a"), LW(0), -5, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_snd_sfxoff, LW(9))
    WAIT_MS(800)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_MARIO_FALL1"), LW(3), LW(4), LW(5), LW(6))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, -100, 400)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_mario_set_pos, LW(3), LW(0), LW(5))
    CALL(evt_snd_sfx_pos, LW(6), LW(3), LW(0), LW(5))
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_party_set_pos, 0, LW(3), LW(0), LW(5))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_mario_set_pos, 0, -500, 0)
    CALL(evt_party_set_pos, 0, 0, -500, 0)
    CALL(evt_snd_sfxoff, LW(6))
    CALL(evt_mario_cont_onoff, 1)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pose, STRING("M_N_7"))
    WAIT_FRAMES(1)
    CALL(evt_mario_cont_onoff, 0)
#if VERSION >= VERSION_JP
    DEBUG_PUT_MSG(STRING("\203r\203r\203A\203\223\202\265\202\301\202\333\226\263\214\370"))
    CALL(evt_party_vivian_tail, 0, 0)
#endif
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_move_position, STRING("me"), -50, -40, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_move_position, STRING("me"), -55, -30, 0, FLOAT(80.0f), 1)
    CALL(evt_npc_move_position, STRING("me"), -45, 80, 0, FLOAT(80.0f), 1)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("me"), 0, 275, 0, FLOAT(80.0f), 1)
        CALL(evt_set_dir_to_target, STRING("me"), STRING("mario"))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, 0, 230, 994, 0, 80, 259, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("me"), 90)

    // The mayor asks General White whether the big bomb cannon is ready, and White calls his crew to the drill.
    CALL(evt_msg_print, 0, STRING("stg7_bom_64"), 0, STRING("me"))
    CALL(evt_msg_print, 0, STRING("stg7_bom_64_01"), 0, STRING("\203z\203\217\203C\203g"))
    SET_INT_READ_ADDR(PTR(&oh_data))

    // The four crew members shout their drill calls in turn (Ho, Hup, Hut, Boom).
    LOOP(4)
        READ_INT2(LW(0), LW(1))
        BEGIN_INLINE_EVENT
            CALL(evt_set_dir_to_target, LW(0), STRING("mario"))
            CALL(evt_msg_print, 0, LW(1), 0, LW(0))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(evt_npc_get_position, LW(0), LW(10), LW(11), LW(12))
            CALL(evt_npc_jump_position_nohit, LW(0), LW(10), LW(11), LW(12), 300, FLOAT(20.0f))
        END_INLINE_EVENT
        WAIT_MS(300)
    END_LOOP
    WAIT_MS(500)

    // The mayor orders positions, and the crew answer (stg7_bom_66 and 67).
    CALL(evt_msg_print, 0, STRING("stg7_bom_66"), 0, STRING("me"))
    SET_VALUE(GF(0), 0)
    CALL(evt_npc_flag_onoff, 0, STRING("\203z\203\217\203C\203g"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 0, STRING("\203y\203\223\203g\203\212\203b\203g"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 0, STRING("\203X\203\211\203\212\201["), NPC_FLAG_TALK_POSE)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg7_bom_67"), 0, STRING("\203z\203\217\203C\203g"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("\203z\203\217\203C\203g"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("\203z\203\217\203C\203g"), LW(0), LW(1), LW(2), 300, FLOAT(20.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg7_bom_67"), 0, STRING("\203y\203\223\203g\203\212\203b\203g"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("\203y\203\223\203g\203\212\203b\203g"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("\203y\203\223\203g\203\212\203b\203g"), LW(0), LW(1), LW(2), 300, FLOAT(20.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("\203X\203\211\203\212\201["), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position_nohit, STRING("\203X\203\211\203\212\201["), LW(0), LW(1), LW(2), 300, FLOAT(20.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_msg_print, 0, STRING("stg7_bom_67"), 0, STRING("\203X\203\211\203\212\201["))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(1000)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    SET_VALUE(GF(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("\203z\203\217\203C\203g"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_move_position, STRING("\203z\203\217\203C\203g"), -50, -80, 0, FLOAT(100.0f), 1)
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("\203y\203\223\203g\203\212\203b\203g"), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), 0, -80, 0, FLOAT(100.0f), 1)
        SET_VALUE(GF(1), 1)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 1, STRING("\203X\203\211\203\212\201["), NPC_STATUS_FACE_MOVE_DIR)
        CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), 50, -80, 0, FLOAT(100.0f), 1)
        SET_VALUE(GF(2), 1)
    END_INLINE_EVENT
    LABEL(2)
    IF_INT_EQUAL(GF(0), 0)
        WAIT_FRAMES(1)
        GOTO(2)
    END_IF
    IF_INT_EQUAL(GF(1), 0)
        WAIT_FRAMES(1)
        GOTO(2)
    END_IF
    IF_INT_EQUAL(GF(2), 0)
        WAIT_FRAMES(1)
        GOTO(2)
    END_IF
    CALL(evt_npc_flag_onoff, 1, STRING("\203z\203\217\203C\203g"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 1, STRING("\203y\203\223\203g\203\212\203b\203g"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 1, STRING("\203X\203\211\203\212\201["), NPC_FLAG_TALK_POSE)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("\203z\203\217\203C\203g"), -50, -200, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_move_position, STRING("\203z\203\217\203C\203g"), -35, -260, 0, FLOAT(80.0f), 1)
    END_INLINE_EVENT
    WAIT_MS(400)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), -50, -80, 0, FLOAT(80.0f), 1)
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), -55, -225, 0, FLOAT(80.0f), 1)
    END_INLINE_EVENT
    WAIT_MS(400)
    CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), -50, -80, 0, FLOAT(80.0f), 1)
    CALL(evt_cam3d_evt_set, 0, 152, 108, 0, 57, -432, 0, EASING_SINE_IN_OUT)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), -70, -180, 0, FLOAT(80.0f), 1)
    END_INLINE_EVENT
    SET_VALUE(GF(0), 0)
    SET_VALUE(LW(0), PTR(&door))

    // The cannon-room doors slide open.
    SPAWN_EVENT(evt_door_open_bom_01)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("\203z\203\217\203C\203g"), 0, -320, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_move_position, STRING("\203z\203\217\203C\203g"), 35, -330, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_move_position, STRING("\203z\203\217\203C\203g"), 80, -380, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING("\203z\203\217\203C\203g"), 70, 30, -435, 300, FLOAT(20.0f))
        CALL(evt_npc_move_position, STRING("\203z\203\217\203C\203g"), 40, -445, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING("\203z\203\217\203C\203g"), 0, 40, -430, 300, FLOAT(20.0f))
        CALL(evt_npc_set_ry, STRING("\203z\203\217\203C\203g"), 90)
    END_INLINE_EVENT
    WAIT_MS(400)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), 0, -320, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), 35, -330, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), 80, -380, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), 80, -415, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_move_position, STRING("\203y\203\223\203g\203\212\203b\203g"), 45, -415, 0, FLOAT(70.0f), 1)
        CALL(evt_npc_jump_position_nohit, STRING("\203y\203\223\203g\203\212\203b\203g"), -20, 15, -390, 300, FLOAT(20.0f))
        CALL(evt_npc_set_ry, STRING("\203y\203\223\203g\203\212\203b\203g"), 315)
    END_INLINE_EVENT
    WAIT_MS(400)
    CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), 0, -320, 0, FLOAT(70.0f), 1)
    SET_VALUE(GF(0), 0)
    CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), 35, -330, 0, FLOAT(70.0f), 1)
    CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), 80, -380, 0, FLOAT(70.0f), 1)
    CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), 80, -415, 0, FLOAT(70.0f), 1)
    CALL(evt_npc_move_position, STRING("\203X\203\211\203\212\201["), 45, -415, 0, FLOAT(70.0f), 1)
    CALL(evt_npc_jump_position_nohit, STRING("\203X\203\211\203\212\201["), 20, 15, -390, 300, FLOAT(20.0f))
    CALL(evt_npc_set_ry, STRING("\203X\203\211\203\212\201["), 90)
    CALL(evt_cam3d_evt_set, -160, 125, -100, 10, 55, -427, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_set_dir_to_target, STRING("\203z\203\217\203C\203g"), STRING("\203y\203\223\203g\203\212\203b\203g"))

    // System check: the guidance system is brought online (stg7_bom_68 to 72).
    CALL(evt_msg_print, 0, STRING("stg7_bom_68"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING("\203y\203\223\203g\203\212\203b\203g"), STRING("\203z\203\217\203C\203g"))
    CALL(evt_msg_print, 0, STRING("stg7_bom_69"), 0, STRING("\203y\203\223\203g\203\212\203b\203g"))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("\203y\203\223\203g\203\212\203b\203g"))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE1"), 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(10))
    CALL(evt_map_playanim, STRING("kikai"), 1, 0)
    WAIT_MS(1000)
    CALL(evt_set_dir_to_target, STRING("\203y\203\223\203g\203\212\203b\203g"), STRING("\203z\203\217\203C\203g"))
    CALL(evt_msg_print, 0, STRING("stg7_bom_70"), 0, STRING("\203y\203\223\203g\203\212\203b\203g"))
    CALL(evt_npc_set_ry, STRING("\203y\203\223\203g\203\212\203b\203g"), 315)
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING("\203z\203\217\203C\203g"), STRING("\203X\203\211\203\212\201["))
    CALL(evt_msg_print, 0, STRING("stg7_bom_71"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_set_dir_to_target, STRING("\203X\203\211\203\212\201["), STRING("\203z\203\217\203C\203g"))
    CALL(evt_msg_print, 0, STRING("stg7_bom_72"), 0, STRING("\203X\203\211\203\212\201["))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("\203X\203\211\203\212\201["))
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_cam3d_evt_set, 200, 350, 1224, 200, 125, 250, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SIREN1"), LW(10))
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)

        // The detonation sequence is announced and the outer hatch and inner shutter open, with the camera
        // cutting between inside and outside (stg7_bom_73 to 76).
        CALL(evt_msg_print, 0, STRING("stg7_bom_73"), 0, STRING("\203z\203\217\203C\203g"))
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 350, 1224, 0, 125, 250, 6000, EASING_SINE_IN_OUT)
    WAIT_MS(6000)
    LOOP(0)
        IF_INT_EQUAL(GF(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(10))
    CALL_CHILD(cam_inside)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg7_bom_74"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(10))

    // The cannon's theme starts and the screen shakes.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG7_HUGE_GUN1"))
    CALL(evt_cam3d_evt_set, 0, 350, 1224, 0, 125, 250, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 283, 932, 0, 125, 250, 3000, EASING_SINE_IN_OUT)
    WAIT_MS(1500)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_FLOOR_QUAKE1"), LW(7))
    SPAWN_EVENT_GET_ID(PTR(gamen_shake), LW(9))
    WAIT_MS(1500)
    SPAWN_EVENT(effect_1)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_FLOOR_QUAKE2"), 0)
    SPAWN_EVENT(gamen_shake2)
    WAIT_MS(300)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yuka2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_yuka3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("chika_off"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("chika_off"), 0, FLOAT(0.0f), 0)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_FLOOR_DOWN1"), LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, -20, 1000)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("hiraku_l_1_a"), 0, LW(0), 0)
    CALL(evt_mapobj_trans, 1, STRING("hiraku_l_1_b"), 0, LW(0), 0)
    ADD(LW(0), -5)
    CALL(evt_mapobj_trans, 1, STRING("hiraku_ss_1_a"), -20, LW(0), 0)
    CALL(evt_mapobj_trans, 1, STRING("hiraku_ss_1_b"), 20, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_FLOOR_OPEN2"), LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 0, 170, 3000)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("hiraku_l_1_b"), LW(0), -20, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("hiraku_l_1_a"), LW(0), -20, 0)
    ADD(LW(0), -20)
    CALL(evt_mapobj_trans, 1, STRING("hiraku_ss_1_a"), LW(0), -25, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING("hiraku_ss_1_b"), LW(0), -25, 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    CALL(evt_snd_sfxoff, LW(8))
    SPAWN_EVENT(gamen_shake2)
    WAIT_MS(600)
    DELETE_EVENT(LW(9))
    CALL(evt_snd_sfxoff, LW(7))
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0f), 100)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(8))
    CALL_CHILD(cam_inside)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg7_bom_75"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_cam3d_evt_set, 0, 283, 932, 0, 125, 250, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -200, 734, 684, -10, 50, 63, 2800, EASING_SINE_IN_OUT)
    WAIT_MS(1800)
    SET_VALUE(LW(0), STRING("hiraku_s_1_a"))
    SET_VALUE(LW(1), STRING("hiraku_s_1_b"))
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(open_hatch)
    WAIT_MS(400)
    SET_VALUE(LW(0), STRING("hiraku_s_2_a"))
    SET_VALUE(LW(1), STRING("hiraku_s_2_b"))
    SET_VALUE(LW(2), 1)
    SPAWN_EVENT(open_hatch)
    WAIT_MS(400)
    SET_VALUE(LW(0), STRING("hiraku_s_3_a"))
    SET_VALUE(LW(1), STRING("hiraku_s_3_b"))
    SET_VALUE(LW(2), 0)
    SPAWN_EVENT(open_hatch)
    WAIT_MS(400)
    SET_VALUE(LW(0), STRING("hiraku_s_4_a"))
    SET_VALUE(LW(1), STRING("hiraku_s_4_b"))
    SET_VALUE(LW(2), 1)
    SPAWN_EVENT(open_hatch)
    WAIT_MS(400)
    WAIT_MS(1000)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(8))
    CALL_CHILD(cam_inside2)
    WAIT_MS(300)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg7_bom_76"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_cam3d_evt_set, -230, 734, 684, -10, 50, 63, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    WAIT_MS(300)
    SET_VALUE(LW(0), 0)
    LOOP(36)
        ADD(LW(0), 1)
        CALL(make_name, LW(0), LW(1))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, LW(1), MAP_OBJ_FLAG_HIDDEN)
        WAIT_MS(100)
    END_LOOP
    BEGIN_INLINE_EVENT
        WAIT_MS(4000)
        CALL(evt_cam3d_evt_set, -543, 200, 1194, -16, 175, 60, 3000, EASING_SINE_IN_OUT)
        WAIT_MS(5500)
        CALL(evt_cam3d_evt_set, 0, 70, 650, 0, 60, 250, 0, EASING_SINE_IN_OUT)
    END_INLINE_EVENT
    SPAWN_EVENT_GET_ID(PTR(gamen_shake), LW(9))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_UP1"), LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, 0, 1600, 11250)
    LABEL(5)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, STRING("S_dekatai"), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    SPAWN_EVENT(effect_1)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_STOP1"), 0)
    DELETE_EVENT(LW(9))
    SPAWN_EVENT(gamen_shake2)
    WAIT_MS(2000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, 1500, 100, 2539, 0, 650, 0, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, -1500, 100, 2539, 0, 650, 0, 5000, EASING_SINE_IN_OUT)
    WAIT_MS(5000)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(10))
    CALL_CHILD(cam_inside)
    WAIT_MS(300)

    // White sights the moon and reads out the aim and wind figures (stg7_bom_77 to 80).
    CALL(evt_msg_print, 0, STRING("stg7_bom_77"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    SET_VALUE(GW(5), 70)
    SPAWN_EVENT_GET_ID(PTR(evt_depth), LW(9))
    CALL(sight_dir, 3)
    CALL_CHILD(cam_outside)
    CALL(evt_cam3d_evt_set, 400, 151, -171, 400, 62, -676, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_cam3d_evt_set, 420, 151, -171, 420, 62, -676, 2000, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(sight_dir, 2)
    CALL(evt_cam3d_evt_set, 400, 151, -171, 400, 62, -676, 2000, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(sight_dir, 5)
    DELETE_EVENT(LW(9))
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_cam3d_evt_set, -1500, 100, 2539, 0, 650, 0, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_MOVE1"), LW(8))
    SPAWN_EVENT_GET_ID(PTR(gamen_shake), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -500, 3000)
    LABEL(6)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_rotate, 1, STRING("dekatai"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(6)
    END_IF
    DELETE_EVENT(LW(9))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_STOP2"), 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0f), 500)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(8))
    SET_VALUE(GW(10), 70)
    SPAWN_EVENT_GET_ID(PTR(evt_depth), LW(9))
    CALL(sight_dir, 4)
    CALL_CHILD(cam_outside)
    CALL(evt_cam3d_evt_set, 400, 151, -171, 400, 62, -676, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg7_bom_78"), 0, 0)
    WAIT_MS(300)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_moon"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("S_moon"), 400, -738, -676)
    CALL(evt_mapobj_scale, 1, STRING("S_moon"), FLOAT(0.7998047f), FLOAT(0.7998047f), FLOAT(0.7998047f))
    CALL(sight_dir, 0)
    CALL(evt_cam3d_evt_set, 400, 151, -171, 400, 262, -676, 2000, EASING_SINE_IN_OUT)
    WAIT_MS(2000)
    CALL(sight_dir, 4)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 70, 0, 1000)
    LABEL(100)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(GW(5), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(100)
    END_IF
    DELETE_EVENT(LW(9))
    WAIT_MS(2000)
    CALL(sight_dir, 6)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_moon"), MAP_OBJ_FLAG_HIDDEN)
    CALL_CHILD(cam_inside)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg7_bom_79"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_cam3d_evt_set, -1500, 100, 2539, 0, 650, 0, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_MOVE1"), LW(8))
    SPAWN_EVENT_GET_ID(PTR(gamen_shake), LW(9))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -500, -400, 1000)
    LABEL(7)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_rotate, 1, STRING("dekatai"), 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(7)
    END_IF
    DELETE_EVENT(LW(9))
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_STOP2"), 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.0f), 500)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg7_bom_80"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_HATCH_OPEN1"), LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -1200, 1000)
    LABEL(8)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_rotate, 1, STRING("futa"), 0, LW(0), 0)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(8)
    END_IF
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_cam3d_evt_set, 21, 175, 781, 9, 42, 265, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)

    // The mayor orders everyone in; the crowd squeezes into the chamber, grumbling (stg7_bom_81 to 85_6).
    CALL(evt_msg_print, 0, STRING("stg7_bom_81"), 0, STRING("\221\272\222\267"))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_BOMB_TOGETHER1"), LW(8))
    CALL_CHILD(evt_bom1000)
    WAIT_MS(5000)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_cam3d_evt_set, -120, 697, 788, 39, 556, 265, 0, EASING_SINE_IN_OUT)
    WAIT_MS(5000)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(10))
    CALL(bom_flag, 1, 1)
    CALL_CHILD(cam_inside2)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg7_bom_85"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(bom_flag, 0, 1)
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_cam3d_evt_set, -120, 697, 788, 39, 556, 265, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_BOMB_TOGETHER1_2"), LW(7))
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 180, 520, 100)
    CALL(evt_msg_print, 0, STRING("stg7_bom_85_1"), 0, 0)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 100, 400, 95)
    CALL(evt_msg_print, 0, STRING("stg7_bom_85_2"), 0, 0)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 10, 475, 75)
    CALL(evt_msg_print, 0, STRING("stg7_bom_85_3"), 0, 0)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG7_HATCH_SHUT1"), LW(8))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -1200, 0, 3000)
        LABEL(9)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
        CALL(evt_mapobj_rotate, 1, STRING("futa"), 0, LW(0), 0)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(9)
        END_IF
        CALL(evt_snd_sfxoff, LW(8))
        CALL(evt_snd_sfxoff, LW(7))
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 180, 520, 100)
    CALL(evt_msg_print, 0, STRING("stg7_bom_85_4"), 0, 0)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 100, 400, 95)
    CALL(evt_msg_print, 0, STRING("stg7_bom_85_5"), 0, 0)
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 10, 475, 75)
    CALL(evt_msg_print, 0, STRING("stg7_bom_85_6"), 0, 0)
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("\221\272\222\267"))
    CALL(bom_flag, 1, 1)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_CANNON_SYS_ACTIVE2"), LW(10))
    CALL_CHILD(cam_inside)
    WAIT_MS(300)

    // White reports all green and the mayor gives the order to fire (stg7_bom_86 and 87).
    CALL(evt_msg_print, 0, STRING("stg7_bom_86"), 0, STRING("\203z\203\217\203C\203g"))
    WAIT_MS(300)
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_cam3d_evt_set, 21, 175, 781, 9, 42, 265, 0, EASING_SINE_IN_OUT)
    CALL_CHILD(cam_outside)
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg7_bom_87"), 0, STRING("\221\272\222\267"))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 200, 0, 2483, 200, 550, 0, 0, EASING_SINE_IN_OUT)
    SPAWN_EVENT(mario_shot)
    WAIT_MS(800)
    CALL_CHILD(effect_delete)
    CALL(evt_cam3d_evt_set, 806, 1189, 451, 600, 982, 69, 0, EASING_SINE_IN_OUT)
    SPAWN_EVENT(mario_shot)
    WAIT_MS(800)
    CALL_CHILD(effect_delete)
    CALL(evt_cam3d_evt_set, -605, 170, 785, -25, 334, 227, 0, EASING_SINE_IN_OUT)
    SPAWN_EVENT(mario_shot)
    WAIT_MS(800)
    CALL_CHILD(effect_delete)
    SPAWN_EVENT_GET_ID(PTR(mario_kemuri), LW(9))
    CALL(evt_mario_set_pos, 200, 0, -650)
    CALL(evt_party_set_pos, 0, 200, 0, -650)
    CALL(evt_cam3d_evt_set, 370, 245, -173, 370, 62, -1205, 0, EASING_SINE_IN_OUT)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("HAIKEI"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_DAMAGED2"), 200, 0, -650, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_AC3_4"), 200, 0, -650, LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG7_MARIO_FLY1"), 200, 0, -650, LW(5))
    SET_FLOAT_VALUE(LW(6), FLOAT(0.0f))

    // He flies toward the Moon along the spline, then vanishes in a flash of light.
    CALL(evt_sub_spline_init, 1, 11, PTR(spline_dt), 3, 0, 2000)
    LABEL(20)
    CALL(evt_sub_spline_get_value, LW(0), LW(1), LW(2), LW(3))
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfx_pos, LW(4), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfx_pos, LW(5), LW(0), LW(1), LW(2))
    ADD(LW(0), -20)
    ADD(LW(1), -20)
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(3), 1)
        GOTO(20)
    END_IF
    DELETE_EVENT(LW(9))
    CALL(evt_snd_sfxoff, LW(5))
    WAIT_FRAMES(1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
    WAIT_MS(1000)
    WAIT_MS(500)
    CALL(evt_snd_sfxon, STRING("SFX_STG7_MARIO_LIGHT1"), 0)
    CALL(evt_eff64, 0, STRING("wangflush_n64"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 340, 200, -590)

        // The crowd cheers (stg7_bom_88 to 90).
        CALL(evt_msg_print, 0, STRING("stg7_bom_88"), 0, 0)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 400, 160, -590)
        CALL(evt_msg_print, 0, STRING("stg7_bom_89"), 0, 0)
        CALL(evt_msg_toge, MSG_TOGE_AT_POS, 373, 135, -590)
        CALL(evt_msg_print, 0, STRING("stg7_bom_90"), 0, 0)
    END_INLINE_EVENT
    CALL(bom_flag, 0, 1)
    CALL(bom_fall)
    WAIT_MS(6000)
    CALL(evt_ext_reset)
    CALL(evt_npc_set_ry, STRING("\221\272\222\267"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("\221\272\222\267"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -60, 26, 493, 22, 33, 257, 0, EASING_SINE_IN_OUT)
    WAIT_MS(2000)

    // The mayor wishes Mario luck, the Goldbob guide is taken from the pouch and the map changes to moo_03.
    CALL(evt_msg_print, 0, STRING("stg7_bom_91"), 0, STRING("\221\272\222\267"))
    CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
    WAIT_MS(300)
    WAIT_MS(2000)
    CALL(evt_pouch_remove_item, ITEM_GOLDBOB_GUIDE, LW(0))
    CALL(evt_bero_mapchange, STRING("moo_03"), 0)
    RETURN
EVT_END()

/** Init of the mayor (村長): from STG7_04_BACK_ONLINE on he stands at (-15, 0, -35). */
static EVT_BEGIN(init_soncho)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_GREATER_OR_EQUAL(STG7_04_BACK_ONLINE)
            CALL(evt_npc_set_position, STRING("me"), -15, 0, -35)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of the mayor: before STG7_05_THE_CANNON it runs the cannon scene (evt_taihou);
 * until the end of chapter 8's battle he says stg7_bom_59_04_1, and afterwards stg7_bom_59_04_2.
 */
static EVT_BEGIN(talk_soncho)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG7_05_THE_CANNON)
            CALL_CHILD(evt_taihou)
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_59_04_1"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_59_04_2"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init for the Bob-omb Nitoro: places him at (-120, 0, -220). */
static EVT_BEGIN(init_nitoro)
    CALL(evt_npc_set_position, STRING("me"), -120, 0, -220)
    RETURN
EVT_END()

/**
 * Talk script for Nitoro: his line depends on story progress (an early visit before STG6_71_01,
 * before STG7_04_BACK_ONLINE, before the cannon, until the end of Chapter 8, and afterwards).
 */
static EVT_BEGIN(talk_nitoro)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_02_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_02"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_02_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_03"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_03_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init for the Bob-omb Pentrit: places him at (-150, 0, 130). */
static EVT_BEGIN(init_pentrit)
    CALL(evt_npc_set_position, STRING("me"), -150, 0, 130)
    RETURN
EVT_END()

/** Talk script for Pentrit: five story-progress dependent lines, in the same stages as Nitoro. */
static EVT_BEGIN(talk_pentrit)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_04_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_04"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_04_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_05"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_05_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init for the Bob-omb Surary: places him at a fixed spot in the village. */
static EVT_BEGIN(init_surary)
    CALL(evt_npc_set_position, STRING("me"), 152, 0, -188)
    RETURN
EVT_END()

/** Talk script for Surary: five story-progress dependent lines, in the same stages as Nitoro. */
static EVT_BEGIN(talk_surary)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_06_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_06"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_06_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_07"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_07_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init for the villager Bomb-hei D: places him at a fixed spot. */
static EVT_BEGIN(init_murabito_d)
    CALL(evt_npc_set_position, STRING("me"), 175, 0, 210)
    RETURN
EVT_END()

/** Talk script for villager D: five story-progress dependent lines, in the same stages as Nitoro. */
static EVT_BEGIN(talk_murabito_d)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_08_00"), 0, STRING("me"))
        CASE_LESS(STG7_04_BACK_ONLINE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_08"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_08_1"), 0, STRING("me"))
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_09"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_1"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init of villager E: places him at (-200, 0, -80). */
static EVT_BEGIN(init_murabito_e)
    CALL(evt_npc_set_position, STRING("me"), -200, 0, -80)
    RETURN
EVT_END()

/**
 * Talk script of villager E: stg7_bom_10_00 before STG6_71_01, stg7_bom_10 until the cannon,
 * then stg7_bom_11.
 */
static EVT_BEGIN(talk_murabito_e)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_10_00"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_10"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_11"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init of villager F: places him at (-150, 0, -80). */
static EVT_BEGIN(init_murabito_f)
    CALL(evt_npc_set_position, STRING("me"), -150, 0, -80)
    RETURN
EVT_END()

/**
 * Talk script of villager F: stg7_bom_12_00 before STG6_71_01, stg7_bom_12 until the cannon,
 * then stg7_bom_13.
 */
static EVT_BEGIN(talk_murabito_f)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_12_00"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_12"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_13"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init of villager G: places him at (-100, 0, -80). */
static EVT_BEGIN(init_murabito_g)
    CALL(evt_npc_set_position, STRING("me"), -100, 0, -80)
    RETURN
EVT_END()

/**
 * Talk script of villager G: stg7_bom_14_00 before STG6_71_01, stg7_bom_14 until the cannon,
 * then stg7_bom_15.
 */
static EVT_BEGIN(talk_murabito_g)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_14_00"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_14"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_15"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init of villager H: places him at (-50, 0, -80). */
static EVT_BEGIN(init_murabito_h)
    CALL(evt_npc_set_position, STRING("me"), -50, 0, -80)
    RETURN
EVT_END()

/**
 * Talk script of villager H: stg7_bom_16_00 before STG6_71_01, stg7_bom_16 until the cannon,
 * then stg7_bom_17.
 */
static EVT_BEGIN(talk_murabito_h)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_16_00"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_16"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_17"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/** Init of villager I: places him at (0, 0, -80). */
static EVT_BEGIN(init_murabito_i)
    CALL(evt_npc_set_position, STRING("me"), 0, 0, -80)
    RETURN
EVT_END()

/**
 * Talk script of villager I: stg7_bom_18_00 before STG6_71_01, stg7_bom_18 until the cannon,
 * then stg7_bom_19.
 */
static EVT_BEGIN(talk_murabito_i)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL(evt_msg_print, 0, STRING("stg7_bom_18_00"), 0, STRING("me"))
        CASE_LESS(STG7_05_THE_CANNON)
            CALL(evt_msg_print, 0, STRING("stg7_bom_18"), 0, STRING("me"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_19"), 0, STRING("me"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Init of General White: leaves him where he is
 * while Trouble Center job 25 is active with its work flag 6 unset;
 * otherwise, once GSWF_BOM_GENERAL_WHITE_AGREED is set, he stands at (50, 0, -100).
 */
static EVT_BEGIN(init_white)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_DELIVERY_PLEASE)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(6), 0)
            RETURN
        END_IF
    END_IF

    // A stray END_IF in the original script (no IF left to close); executing it does nothing.
    END_IF
    IF_INT_EQUAL(GSWF_BOM_GENERAL_WHITE_AGREED, 1)
    CALL(evt_npc_set_position, STRING("me"), 50, 0, -100)
    END_IF
    RETURN
EVT_END()

/**
 * Talk script of General White: before the cannon he says stg7_bom_59_02
 * or 59_03 depending on whether Goldbob's cannon approval (GSWF_PIK_GOLDBOB_PERMISSION_GIVEN) has been given,
 * until the end of the chapter 8 battle stg7_bom_59_04, and afterwards stg7_bom_59_05.
 */
static EVT_BEGIN(talk_white)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG7_05_THE_CANNON)
            IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 0)
                CALL(evt_msg_print, 0, STRING("stg7_bom_59_02"), 0, 0)
            ELSE
                CALL(evt_msg_print, 0, STRING("stg7_bom_59_03"), 0, 0)
            END_IF
        CASE_LESS_OR_EQUAL(STG8_END_AFTER_THE_BATTLE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_59_04"), 0, 0)
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_59_05"), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Talk script of the three dancing girls (３人娘Ａ, Ｂ, Ｃ):
 * three lines each that change at STG7_02_MAYOR_BOB and STG7_04_BACK_ONLINE.
 */
static EVT_BEGIN(musume_talk)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG7_02_MAYOR_BOB)
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_01"), 0, STRING("\202R\220l\226\272\202`"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_02"), 0, STRING("\202R\220l\226\272\202a"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_03"), 0, STRING("\202R\220l\226\272\202b"))
        CASE_LESS(STG7_04_BACK_ONLINE)
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_04"), 0, STRING("\202R\220l\226\272\202`"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_05"), 0, STRING("\202R\220l\226\272\202a"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_06"), 0, STRING("\202R\220l\226\272\202b"))
        CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_07"), 0, STRING("\202R\220l\226\272\202`"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_08"), 0, STRING("\202R\220l\226\272\202a"))
            CALL(evt_msg_print, 0, STRING("stg7_bom_09_01_09"), 0, STRING("\202R\220l\226\272\202b"))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Spawns dancing girl A (踊り子, model c_dance_r) at (370, 5, -120) with movement and talk animations
 * and her talk script musume_talk.
 */
static EVT_BEGIN(musume_a_entry)
    CALL(evt_npc_entry, STRING("\202R\220l\226\272\202`"), STRING("c_dance_r")) // one of the Traveling Sisters 3 (red)
    CALL(evt_npc_set_tribe, STRING("\202R\220l\226\272\202`"), STRING("\227x\202\350\216q"))
    CALL(evt_npc_set_position, STRING("\202R\220l\226\272\202`"), 370, 5, -120)
    CALL(evt_npc_flag_onoff, 1, STRING("\202R\220l\226\272\202`"), NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_TALK_POSE)
    CALL(evt_npc_change_interrupt, STRING("\202R\220l\226\272\202`"), 6, PTR(musume_talk))
    RETURN
EVT_END()

/**
 * Spawns dancing girl B (踊り子ピンク, model c_dance_p) at (400, 5, -150) with movement and talk animations
 * and her talk script musume_talk.
 */
static EVT_BEGIN(musume_b_entry)
    CALL(evt_npc_entry, STRING("\202R\220l\226\272\202a"), STRING("c_dance_p")) // one of the Traveling Sisters 3 (pink)
    CALL(evt_npc_set_tribe, STRING("\202R\220l\226\272\202a"), STRING("\227x\202\350\216q\203s\203\223\203N"))
    CALL(evt_npc_set_position, STRING("\202R\220l\226\272\202a"), 400, 5, -150)
    CALL(evt_npc_flag_onoff, 1, STRING("\202R\220l\226\272\202a"), NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_TALK_POSE)
    CALL(evt_npc_change_interrupt, STRING("\202R\220l\226\272\202a"), 6, PTR(musume_talk))
    RETURN
EVT_END()

/**
 * Spawns dancing girl C (踊り子黄緑, model c_dance_k) at (430, 5, -100) with movement and talk animations
 * and her talk script musume_talk.
 */
static EVT_BEGIN(musume_c_entry)
    CALL(evt_npc_entry, STRING("\202R\220l\226\272\202b"), STRING("c_dance_k")) // one of the Traveling Sisters 3 (yellow-green)
    CALL(evt_npc_set_tribe, STRING("\202R\220l\226\272\202b"), STRING("\227x\202\350\216q\211\251\227\316"))
    CALL(evt_npc_set_position, STRING("\202R\220l\226\272\202b"), 430, 5, -100)
    CALL(evt_npc_flag_onoff, 1, STRING("\202R\220l\226\272\202b"), NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_TALK_POSE)
    CALL(evt_npc_change_interrupt, STRING("\202R\220l\226\272\202b"), 6, PTR(musume_talk))
    RETURN
EVT_END()

/** The cast of the village square: the mayor, Nitro, Pentrit, Surary, the Bob-omb soldier D and General White. */
static NpcSetupInfo npcEnt[] = {
    {
        /*"村長"*/"\221\272\222\267", // name (the mayor of Fahr Outpost)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_soncho, // initEvent
        NULL, // regularEvent
        talk_soncho, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ニトロ"*/"\203j\203g\203\215", // name (Nob, a Fahr Outpost Bob-omb)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_nitoro, // initEvent
        NULL, // regularEvent
        talk_nitoro, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ペントリット"*/("\203y\203\223\203g\203\212\203b\203g"), // name (Rob, a Fahr Outpost Bob-omb)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_pentrit, // initEvent
        NULL, // regularEvent
        talk_pentrit, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"スラリー"*/("\203X\203\211\203\212\201["), // name (Fred, a Fahr Outpost Bob-omb)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_surary, // initEvent
        NULL, // regularEvent
        talk_surary, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボムヘイD"*/"\203{\203\200\203w\203CD", // name (a Fahr Outpost Bob-omb)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_murabito_d, // initEvent
        NULL, // regularEvent
        talk_murabito_d, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ホワイト"*/("\203z\203\217\203C\203g"), // name (General White)
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_white, // initEvent
        NULL, // regularEvent
        talk_white, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

/**
 * Script helper: tilts the cannon smoke (kemuri) and ripple effects 45 degrees so they leave along the barrel
 * when the cannon fires.
 */
static s32 taihou_r(EventEntry* event, BOOL isFirstCall) {
    f32 c = cos(0.7853981852531433);
    f32 s = sin(0.7853981852531433);
    f32 x;
    f32 y;
    f32 rx;
    f32 rz;

    x = 100.0f * c;
    rx = -angleABf(0.0f, 0.0f, 0.0f, y = 100.0f * s);
    rz = -angleABf(0.0f, 0.0f, x, y);

    effKemuTestSetRxRz(effNameToPtr("kemuri"), rx, rz);
    effRippleN64SetRxRz(effNameToPtr("ripple"), rx, rz);
    return EVT_RETURN_DONE;
}

// Kept (pattern): propagation and lifetimes off for the extdrv, npc_jump and depthDisp functions of bom_01, eki_00,
// gor_10, hei_10, rsh_05, rsh_06, tik_05 and tou_02 (one half is a -opt file flag where configure.py sets it); the
// target needs it and no natural form was found (cleanup_notes/mweppc_pilot.md, pragma review rels).
#pragma opt_propagation off
#pragma opt_lifetimes off

/**
 * Full-screen blur: copies the frame buffer into a texture and redraws it as eight overlapping copies,
 * each shifted by GW(5)/10000 of the screen in a ring (45 degrees apart) and blended at 1/8 strength.
 * evt_taihou sets GW(5) to 70 while the cannon sights the moon and eases it to 0 to bring the view into focus.
 */
static void depthDisp(CameraId camera, void* param) {
    f32 amp = evtGetValue(NULL, GW(5)) / 100.0f;
    CameraEntry* cam = camGetPtr(camera);
    smartEntry* buf = smartAlloc(GXGetTexBufferSize(608, 480, GX_TF_RGB565, GX_FALSE, 0), SMART_GROUP_TEMP);
    GXTexObj obj;
    Mtx mtx;
    f32 a;
    f32 s;
    f32 c;
    s32 i;
    BOOL first;
    GXTevColorArg carg;

    GXSetTexCopySrc(0, 0, 608, 480);
    GXSetTexCopyDst(608, 480, GX_TF_RGB565, GX_FALSE);
    GXCopyTex(buf->address, GX_FALSE);
    GXPixModeSync();
    GXInitTexObj(&obj, buf->address, 608, 480, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(8);
    GXSetNumTevStages(8);
    for (i = 0; i < 8; i++) {
        a = 6.2831855f * i / 8.0f;
        s = polySinf(a);
        c = polyCosf(a);
        MTXTrans(mtx, (amp * s) / 100.0f, (amp * c) / 100.0f, 0.0f);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0 + i * 3, GX_MTX2x4);
        GXSetTexCoordGen2((GXTexCoordID)i, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0 + i * 3, GX_FALSE, GX_PTIDENTITY);
        GXSetTevKColorSel((GXTevStageID)i, GX_TEV_KCSEL_1_8);
        GXSetTevKAlphaSel((GXTevStageID)i, GX_TEV_KASEL_1_8);
        GXSetTevOrder((GXTevStageID)i, (GXTexCoordID)i, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp((GXTevStageID)i, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        first = !i;
        carg = first ? GX_CC_ZERO : GX_CC_CPREV;
        GXSetTevColorIn((GXTevStageID)i, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, carg);
        GXSetTevAlphaIn((GXTevStageID)i, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, first ? GX_CA_ZERO : GX_CA_APREV);
        GXSetTevSwapMode((GXTevStageID)i, GX_TEV_SWAP0, GX_TEV_SWAP0);
    }
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(-304, 240, 0);
    GXTexCoord2s16(0, 0);
    GXPosition3s16(304, 240, 0);
    GXTexCoord2s16(1, 0);
    GXPosition3s16(304, -240, 0);
    GXTexCoord2s16(1, 1);
    GXPosition3s16(-304, -240, 0);
    GXTexCoord2s16(0, 1);
    GXEnd();
    sysWaitDrawSync();
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

/** Script helper: queues depthDisp on the 2D camera and keeps blocking, so the blur runs every frame. */
static s32 depth(EventEntry* event, BOOL isFirstCall) {
    dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, depthDisp, NULL);
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: formats the name "S_hikari<args[0]>" into a static buffer and returns its address in args[1],
 * to address the numbered light objects.
 */
static s32 make_name(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;

    sprintf(name, "S_hikari%d", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: fills ext_data with the c_bom_sen sprite frames for the cheering crowd:
 * ten "S_1" frames followed by twenty "R_1" frames.
 */
static s32 make_ext_table_minnnanokoe(EventEntry* event, BOOL isFirstCall) {
    s32 i;

    memset(ext_data, 0, sizeof(ext_data));
    for (i = 0; i < 10; i++) {
        ext_data[i].group = "c_bom_sen";
        ext_data[i].anim = "S_1";
        ext_data[i].frame = 4.0f * i;
    }
    for (i = 0; i < 20; i++) {
        ext_data[10 + i].group = "c_bom_sen";
        ext_data[10 + i].anim = "R_1";
        ext_data[10 + i].frame = 2.0f * i;
    }
    ext_data[30].group = NULL;
    return EVT_RETURN_DONE;
}

/**
 * Extension-draw init for the cheering crowd of the final-battle scene:
 * places Bob-ombs in a grid of rows of 14 (30 units wide, 20 deep), all facing the same way in the "S_1" pose.
 */
static void ext_init_minnnanokoe(void) {
    s32 i;
    BomWork* wp;
    ExtdrvPose* pose;
    s32 x;
    s32 z;

    lp = smartAlloc(sizeof(BomWork), SMART_GROUP_MAP);
    wp = lp->address;
    memset(wp, 0, sizeof(BomWork));
    wp->count = extGetPoseNum();
    pose_init();

    pose = extGetPosePtr();
    for (i = 0; i < wp->count; i++, pose++) {
        wp->bom[i].vy = 0.0f;
        wp->bom[i].gravity = 0.0f;
        wp->bom[i].state = 5;
        pose->flags |= (EXT_POSE_FLAG_VISIBLE | EXT_POSE_FLAG_BILLBOARD);
        PSMTXIdentity(*pose->mtx);
        x = (i % 14) * 30 - 205 + rand() % 10;
        z = 190 - (i / 14) * 20 + rand() % 20;
        (*pose->mtx)[0][3] = x;
        (*pose->mtx)[1][3] = 0.0f;
        (*pose->mtx)[2][3] = z;
        pose->dir = 270.0f;
        pose_change(i, "S_1");
    }
}

/**
 * Makes crowd Bob-omb i hop on the spot to height h over 20 frames,
 * and plays the "bombs together" sound for every fifth one (not during the Shadow Queen story step).
 */
static void jump_minnnanokoe(s32 i, f32 h) {
    BomWork* wp = lp->address;
    ExtdrvPose* pose = extGetPosePtr() + i;

    wp->bom[i].target = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};
    wp->bom[i].speed = 0.0f;
    wp->bom[i].gravity = 0.1f;
    wp->bom[i].timer = 20;
    wp->bom[i].vy = wp->bom[i].gravity * wp->bom[i].timer / 2 + h / wp->bom[i].timer;
    wp->bom[i].next = 5;
    wp->bom[i].state = 100;
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) != STG8_16_THE_RETURN_OF_THE_QUEEN && i % 5 == 0) {
        Vec pos = (Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]};
        psndSFXOn_3D("SFX_STG7_BOMB_TOGETHER2", &pos);
    }
}

/**
 * Draw callback for the cheering crowd: draws each Bob-omb with the darkness TEV callback (envSetYami)
 * so they are lit like the stage.
 */
static void ext_disp_minnnanokoe(CameraId camera, void* param) {
    MapTevCallbackInfo yp;
    MapTevCallbackInfo* p = &yp;
    ExtdrvPose* pose = extGetPosePtr();
    s32 i;

    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        yp.tevStage = 1;
        yp.texMap = 1;
        yp.texCoord = 1;
        yp.indStage = 0;
        yp.mtx = *pose->mtx;
        envSetYami(p);
        GXSetNumTexGens(yp.texCoord);
        GXSetNumTevStages(yp.tevStage);
        GXSetNumIndStages(yp.indStage);
        extPoseDraw(i);
    }
    extLoadTextureExit();
}

/** Queues ext_disp_minnnanokoe for the 3D camera each frame. */
static void ext_dispent_minnnanokoe(void) {
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, ext_disp_minnnanokoe, NULL);
}

/**
 * Script helper: picks the position for the next speech bubble above the cheering crowd.
 * args[0] and args[1] are updated (row group and a column that moves 4 or 8 places);
 * the chosen Bob-omb's X and Z come back through the next two arguments.
 */
static s32 bom1000_msg(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    s32* fa = args;
    s32 flag;
    s32 type = evtGetValue(event, *fa++);
    s32 idx = evtGetValue(event, *fa++);
    ExtdrvPose* pose = extGetPosePtr();
    ExtdrvPose* p;
    s32 row;
    s32 n;

    if (type == 0) {
        row = 3;
        flag = 1;
    } else {
        row = 10;
        flag = 0;
    }
    n = idx + row * 14;
    idx += (rand() % 2 + 1) * 4;
    idx %= 14;
    p = &pose[n];
    evtSetValue(event, args[0], flag);
    evtSetValue(event, args[1], idx);
    evtSetFloat(event, fa[0], (*p->mtx)[0][3]);
    evtSetFloat(event, fa[1], (*p->mtx)[2][3]);
    return EVT_RETURN_DONE;
}

/** Script helper: makes a random crowd Bob-omb hop (height 10), unless it is already mid-hop. */
static s32 bom1000_jump(EventEntry* event, BOOL isFirstCall) {
    BomWork* wp = lp->address;
    s32 i = rand() % wp->count;

    s32 state = wp->bom[i].state;

    if (state != 100) {
        jump_minnnanokoe(i, 10.0f);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] is a float Z offset; builds a look-at matrix from (0, 100, 100+z)
 * and hands it to the darkness effect (envSetYamiView) as its view.
 */
static s32 yami_view(EventEntry* event, BOOL isFirstCall) {
    f32 z = evtGetFloat(event, event->args[0]);
    Mtx mtx;

    C_MTXLookAt(mtx, &(Vec){0.0f, 100.0f, 100.0f + z}, &(Vec){0.0f, 1.0f, 0.0f}, &(Vec){0.0f, 0.0f, z});
    envSetYamiView(mtx);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] is an NPC name and args[1] picks a TEV callback slot (low four bits);
 * turns that callback on in the NPC's animation materials. Does nothing if the NPC does not exist.
 */
static s32 anim_tevcallback(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    NpcEntry* npc = npcNameToPtr_NoAssert(name);

    if (npc == NULL) {
        return EVT_RETURN_DONE;
    }
    animPoseSetMaterialFlagOn(npc->poseId, MAP_OBJ_TEV_CALLBACK(flag & 0xF));
    return EVT_RETURN_DONE;
}

/** Script helper: creates a Star Stone effect named args[0] at (args[1], args[2], args[3]), size 1. */
static s32 starstone_entry(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effStarStoneEntry(6, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

/** Script helper: moves the named Star Stone effect (args[0]) to the float position in args[1..3]. */
static s32 starstone_set_pos(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffStarStoneWork*)eff->userdata)->position.x = x;
    ((EffStarStoneWork*)eff->userdata)->position.y = y;
    ((EffStarStoneWork*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

/** Script helper: sets the Y rotation of the named Star Stone effect (args[0]) to the float in args[1]. */
static s32 starstone_set_ry(EventEntry* event, BOOL isFirstCall) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 ry = evtGetFloat(event, args[1]);

    ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    return EVT_RETURN_DONE;
}

/**
 * Spawns the cheering crowd: builds its frame table, adds a dummy c_bom_sen NPC below the map,
 * and registers 250 sprites with the crowd callbacks.
 */
static EVT_BEGIN(bom1000_minnnanokoe)
    CALL(make_ext_table_minnnanokoe)
    CALL(evt_npc_entry, STRING("dummy"), STRING("c_bom_sen"))
    CALL(evt_npc_set_position, STRING("dummy"), 0, -1000, 0)
    CALL(evt_ext_entry, 250, PTR(ext_data), PTR(ext_init_minnnanokoe), PTR(ext_main), PTR(ext_dispent_minnnanokoe))
    RETURN
EVT_END()

/**
 * Chapter 8 scene "everyone's voices" (minnnanokoe):
 * a crowd of Bob-ombs is created, the partner is parked off-stage,
 * the screen darkens and the stg8_las_148_65 to 71 messages play
 * before the map changes to las_29 at the minnnanokoe entrance.
 */
static EVT_BEGIN(minnnanokoe_evt)
    SPAWN_EVENT(bom1000_minnnanokoe)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_yuka1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_set_pos, 0, -1000, 0)
    CALL(evt_party_set_pos, 0, 0, -1000, 0)
#if VERSION >= VERSION_US
    CALL(evt_npc_set_position, STRING("村長"), 520, 0, 0)
    CALL(evt_npc_set_ry, STRING("村長"), 270)
    CALL(evt_npc_status_onoff, 1, STRING("村長"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING("ニトロ"), -30, 0, -180)
    CALL(evt_npc_set_ry, STRING("ニトロ"), 270)
    CALL(evt_npc_set_position, STRING("ペントリット"), 0, 0, -180)
    CALL(evt_npc_set_ry, STRING("ペントリット"), 270)
    CALL(evt_npc_set_position, STRING("スラリー"), 30, 0, -180)
    CALL(evt_npc_set_ry, STRING("スラリー"), 90)
    CALL(evt_npc_set_position, STRING("ボムヘイD"), 570, 0, 25)
    CALL(evt_npc_set_ry, STRING("ボムヘイD"), 270)
    CALL(evt_npc_status_onoff, 1, STRING("ボムヘイD"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING("ホワイト"), 0, 0, -230)
    CALL(evt_npc_set_ry, STRING("ホワイト"), 270)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_bom01"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_bom01"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("村長"), 1)
    CALL(anim_tevcallback, STRING("ニトロ"), 1)
    CALL(anim_tevcallback, STRING("ペントリット"), 1)
    CALL(anim_tevcallback, STRING("スラリー"), 1)
    CALL(anim_tevcallback, STRING("ボムヘイD"), 1)
    CALL(anim_tevcallback, STRING("ホワイト"), 1)
#else
    CALL(evt_npc_set_position, STRING("\221\272\222\267"), 520, 0, 0)
    CALL(evt_npc_set_ry, STRING("\221\272\222\267"), 270)
    CALL(evt_npc_status_onoff, 1, STRING("\221\272\222\267"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING("\203j\203g\203\215"), -30, 0, -180)
    CALL(evt_npc_set_ry, STRING("\203j\203g\203\215"), 270)
    CALL(evt_npc_set_position, STRING("\203y\203\223\203g\203\212\203b\203g"), 0, 0, -180)
    CALL(evt_npc_set_ry, STRING("\203y\203\223\203g\203\212\203b\203g"), 270)
    CALL(evt_npc_set_position, STRING("\203X\203\211\203\212\201["), 30, 0, -180)
    CALL(evt_npc_set_ry, STRING("\203X\203\211\203\212\201["), 90)
    CALL(evt_npc_set_position, STRING("\203{\203\200\203w\203CD"), 570, 0, 25)
    CALL(evt_npc_set_ry, STRING("\203{\203\200\203w\203CD"), 270)
    CALL(evt_npc_status_onoff, 1, STRING("\203{\203\200\203w\203CD"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_npc_set_position, STRING("\203z\203\217\203C\203g"), 0, 0, -230)
    CALL(evt_npc_set_ry, STRING("\203z\203\217\203C\203g"), 270)
    CALL(evt_map_set_tevcallback, 1, PTR(envSetYami))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_bom01"), 16777216)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_bom01"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(anim_tevcallback, STRING("\221\272\222\267"), 1)
    CALL(anim_tevcallback, STRING("\203j\203g\203\215"), 1)
    CALL(anim_tevcallback, STRING("\203y\203\223\203g\203\212\203b\203g"), 1)
    CALL(anim_tevcallback, STRING("\203X\203\211\203\212\201["), 1)
    CALL(anim_tevcallback, STRING("\203{\203\200\203w\203CD"), 1)
    CALL(anim_tevcallback, STRING("\203z\203\217\203C\203g"), 1)
#endif
    CALL(yami_view, -1500)
    CALL(evt_bg_set_color, 64, 64, 64, 255)
    CALL(starstone_entry, STRING("starstone"), 0, 50, 10)
    SET_VALUE(GW(0), 50)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, STRING("starstone"), 0, GW(0), 10)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, STRING("starstone"), LW(0))
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 5000, 5500, 500)
            LABEL(1)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(1)
            END_IF
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 5500, 5000, 500)
            LABEL(2)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(100.0f))
            SET_FLOAT_VALUE(GW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 1)
                GOTO(2)
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 400, 106, 331, 400, 45, -13, 0, EASING_SINE_IN_OUT)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(500)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
#if VERSION >= VERSION_US
        CALL(evt_npc_move_position, STRING("ボムヘイD"), 450, 25, 0, 90, 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("村長"), 400, 0, 0, 90, 0)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_65"), 0, STRING("村長"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_66"), 0, STRING("村長"))
#else
        CALL(evt_npc_move_position, STRING("\203{\203\200\203w\203CD"), 450, 25, 0, 90, 0)
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING("\221\272\222\267"), 400, 0, 0, 90, 0)
    WAIT_FRAMES(1)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_65"), 0, STRING("\221\272\222\267"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_66"), 0, STRING("\221\272\222\267"))
#endif
    CALL(evt_cam3d_evt_set, 0, 283, 1182, 0, 75, 0, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_67"), 0, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(bom1000_jump)
            WAIT_MS(100)
        END_LOOP
    END_SIBLING_EVENT
#if (VERSION == VERSION_US || VERSION == VERSION_DEMO_US_2)
    SET_VALUE(GW(6), 0)
#endif
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(14), 0)
        SET_VALUE(LW(15), 3)

        // BUG: textbox limit. Every message here opens a window (msgWindow_Entry), and there are only
        // 7 window slots; windowEntry returns -1 when all are in use and msgWindow_Entry does not check it.
        // A new one opens every 300 ms with no other check, which is not always enough for old ones to close.
        LOOP(0)
            CALL(bom1000_msg, LW(14), LW(15), LW(0), LW(2))
            CALL(evt_sub_random, 2, LW(1))
            ADD(LW(1), 1)
            MULTIPLY(LW(1), 30)
            CALL(evt_sub_random, 20, LW(3))
            SUBTRACT(LW(3), 10)
            ADD(LW(1), LW(3))
            CALL(evt_msg_toge, MSG_TOGE_AT_POS_FAR, LW(0), LW(1), LW(2))
            CALL(evt_sub_random, 2, LW(3))
            SWITCH(LW(3))
                CASE_EQUAL(0)
                    SET_VALUE(LW(3), STRING("stg8_las_148_68"))
                CASE_EQUAL(1)
                    SET_VALUE(LW(3), STRING("stg8_las_148_69"))
                CASE_EQUAL(2)
                    SET_VALUE(LW(3), STRING("stg8_las_148_70"))
            END_SWITCH
            BEGIN_INLINE_EVENT
#if VERSION == VERSION_US || VERSION == VERSION_DEMO_US_2
                CALL(evt_msg_pri, GW(6))
                ADD(GW(6), 1)
                IF_INT_EQUAL(GW(6), 10)
                    SET_VALUE(GW(6), 0)
                END_IF
#endif
                CALL(evt_msg_print, 0, LW(3), 0, 0)
            END_INLINE_EVENT
            WAIT_MS(300)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(3000)
    CALL(evt_cam3d_evt_set, 400, 106, 331, 400, 45, -13, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
#if VERSION >= VERSION_US
    CALL(evt_npc_set_ry, STRING("村長"), 90)
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("ボムヘイD"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("ボムヘイD"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_71"), 0, STRING("村長"))
#else
    CALL(evt_npc_set_ry, STRING("\221\272\222\267"), 90)
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("\203{\203\200\203w\203CD"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("\203{\203\200\203w\203CD"))
    CALL(evt_msg_print, 0, STRING("stg8_las_148_71"), 0, STRING("\221\272\222\267"))
#endif
    WAIT_MS(2000)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_bero_mapchange, STRING("las_29"), STRING("minnnanokoe"))
    RETURN
EVT_END()

/** Case script on the locked house door (A_ie_door): shows stg7_bom_09_01 with controls frozen. */
static EVT_BEGIN(evt_door)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg7_bom_09_01"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Init script of bom_01, the village of Fahr Outpost:
 * sets up the doors, wind vanes, snow, Star Piece, NPCs and the three dancers (during Chapter 7),
 * runs the arrival cutscene on the first visit and, in the Chapter 8 finale (when entered as "minnnanokoe"),
 * starts the cheering scene instead of the normal music.
 */
EVT_BEGIN(bom_01_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            CALL_CHILD(evt_bero_info_run)
        CASE_LESS(STG7_01_FAHR_OUTPOST_1ST_VISIT)
            CALL_CHILD(bero_case_entry)
        CASE_DEFAULT
            CALL_CHILD(evt_bero_info_run)
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        SET_VALUE(LF(0), 0)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envoff, PSND_FADEOUT)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG7_BOM1"))
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG7_BOM2"))
    END_IF
    SET_VALUE(LW(0), STRING("group10"))
    SPAWN_EVENT(evt_bom_01_karakara)
    SET_VALUE(LW(0), STRING("group11"))
    SPAWN_EVENT(evt_bom_01_karakara)
    SET_VALUE(LW(0), STRING("group12"))
    SPAWN_EVENT(evt_bom_01_karakara)
    SET_VALUE(LW(0), STRING("group13"))
    SPAWN_EVENT(evt_bom_01_karakara)
    SET_VALUE(LW(0), STRING("group14"))
    SPAWN_EVENT(evt_bom_01_karakara)
    SET_VALUE(LW(0), STRING("group15"))
    SPAWN_EVENT(evt_bom_01_karakara)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("chika_off"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("A_chika"), 1, 0)
    CALL_CHILD(evt_bom_snowfall)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_moon"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yuka2"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("S_yuka3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_trans, 1, STRING("chika_off"), 0, FLOAT(-1.0f), 0)
    CALL(evt_npc_setup, PTR(npcEnt))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG7_00_START, STG7_05_THE_CANNON)
            CALL_CHILD(musume_a_entry)
            CALL_CHILD(musume_b_entry)
            CALL_CHILD(musume_c_entry)
    END_SWITCH
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_ie_door"), 0, PTR(evt_door), 0)
    CALL(evt_item_entry, STRING("item_00"), ITEM_STAR_PIECE, -180, 5, 450, ITEM_TYPE_STILL, GSWF_STARPIECE_92, 0)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_71_01)
            SET_VALUE(GSWF_BOM_01_EARLY_VISIT, 1)
        CASE_LESS(STG7_01_FAHR_OUTPOST_1ST_VISIT)
            SPAWN_EVENT(first_evt)
    END_SWITCH
    IF_INT_EQUAL(GSW_STORY_PROGRESS, STG8_15_GRODUS)
        CALL(evt_bero_get_entername, LW(0))
        IF_STR_EQUAL(LW(0), STRING("minnnanokoe"))
            SPAWN_EVENT(minnnanokoe_evt)
        END_IF
    END_IF
    SPAWN_EVENT(evt_sub_mail_event)
    BEGIN_INLINE_EVENT
        CALL(sight)
    END_INLINE_EVENT
    RETURN
EVT_END()
