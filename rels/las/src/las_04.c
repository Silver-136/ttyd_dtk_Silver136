#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "mario/mario_pouch.h"
#include "sfx_name_word.h"
#include "mot/mot_damage.h"
#include "pmario_sound.h"
#include "driver/shadowdrv.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "mario/mario_motion.h"

#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "driver/itemdrv.h"
#include "driver/camdrv.h"
#include "driver/animdrv.h"
#include "dolphin/mtx.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "string.h"
#include "eff/eff_kemu_test.h"
#include "system.h"
#include "party/party_vivian.h"
#include "evt/evt_bero.h"
#include "evt/evt_damage.h"
#include "las.h"
#include "libc_decls.h"
#include "driver/hitdrv.h"
#include <math.h>

typedef struct FirebarBall { s32 pose; Vec pos; } FirebarBall;
typedef struct FirebarSetup {
    u8 type;
    u8 count;
    u8 field_0x2[2];
    Vec pos;
    f32 spacing;
    f32 field_0x14;
    f32 speed;
    void* callback;
    f32 param;
    f32 radius;
} FirebarSetup; // param: type 0 start angle, type 1 end x
typedef struct FirebarWork {
    u8 type;
    u8 count;
    u16 flags;
    Vec pos;
    f32 prevAngle;
    f32 angle;
    f32 speed;
    f32 spacing;
    f32 field_0x20;
    f32 scale;
    f32 endX;
    f32 radius;
    void (*callback)(struct FirebarWork* wp, s32 type);
    s32 hitCount;
    s32 sfxId;
    s32 state;
    f32 shadowAngle;
    FirebarBall* balls;
    FirebarSetup* setup;
} FirebarWork;
ItemEntry* itemEntry(const char* name, s32 itemId, s32 mode, s32 flag, void* script, f32 x, f32 y, f32 z);
void shadowSetVolumeDecay(f32 a, f32 b, f32 c);
void firebar_disp(CameraId camera, void* param);
void _firebar_rot(FirebarWork* wp);
void _firebar_trans(FirebarWork* wp);
s32 _firebar_hitCheck2(FirebarWork* wp);

USER_FUNC(coin_entry);
USER_FUNC(firebar_disp_evt);
USER_FUNC(firebar_entry);
void firebar_cb(FirebarWork* wp, s32 type);
s32 _firebar_hitCheck(FirebarWork* wp, FirebarBall* ball);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
EvtWord firebar_coin[68];
FirebarSetup firebardesc[7];
EvtWord firebar_evt[102];
static HitDamageReturn dmgRetPoint[4];
EvtWord las_04_init_evt[55];

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_l_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_01"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("S_doa_r_02"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_03", // nextMapName
        "e_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_05", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(firebar_coin)
    SWITCH(LW(2))
        CASE_EQUAL(0)
        SET_VALUE(LW(3), 0)
        SET_VALUE(LW(4), 360)
        CASE_LESS(0)
        SET_VALUE(LW(3), 90)
        SET_VALUE(LW(4), 270)
        CASE_GREATER(0)
        SET_VALUE(LW(3), 270)
        SET_VALUE(LW(4), 450)
    END_SWITCH
    SET_VALUE(LW(5), LW(4))
    SUBTRACT(LW(5), LW(3))
    DIVIDE(LW(5), 6)
    SET_VALUE(LW(6), LW(3))
    SET_VALUE(LW(7), LW(5))
    DIVIDE(LW(7), 2)
    ADD(LW(6), LW(7))
    LOOP(5)
        CALL(evt_sub_random, LW(5), LW(7))
        ADD(LW(6), LW(7))
        CALL(coin_entry, LW(0), LW(1), LW(2), LW(6))
        WAIT_MS(200)
    END_LOOP
    RETURN
EVT_END()

FirebarSetup firebardesc[] = {
    {
        0, // type
        5, // count
        {0}, // field_0x2
        {-700.0f, 80.0f, -55.0f}, // pos
        25.0f, // spacing
        25.0f, // field_0x14
        0.04f, // speed
        firebar_cb, // callback
        0.0f, 0.0f, // param, radius
    },
    {
        0, // type
        5, // count
        {0}, // field_0x2
        {-500.0f, 80.0f, 55.0f}, // pos
        25.0f, // spacing
        25.0f, // field_0x14
        -0.04f, // speed
        firebar_cb, // callback
        0.0f, 0.0f, // param, radius
    },
    {
        0, // type
        5, // count
        {0}, // field_0x2
        {-300.0f, 80.0f, -55.0f}, // pos
        25.0f, // spacing
        25.0f, // field_0x14
        0.04f, // speed
        firebar_cb, // callback
        0.0f, 0.0f, // param, radius
    },
    {
        0, // type
        4, // count
        {0}, // field_0x2
        {0.0f, 80.0f, 0.0f}, // pos
        25.0f, // spacing
        25.0f, // field_0x14
        0.03f, // speed
        firebar_cb, // callback
        0.0f, // param
        30.0f, // radius
    },
    {
        0, // type
        4, // count
        {0}, // field_0x2
        {0.0f, 80.0f, 0.0f}, // pos
        25.0f, // spacing
        25.0f, // field_0x14
        0.03f, // speed
        firebar_cb, // callback
        3.1415927f, // param
        30.0f, // radius
    },
    {
        1, // type
        5, // count
        {0}, // field_0x2
        {800.0f, -10.0f, -50.0f}, // pos
        25.0f, // spacing
        25.0f, // field_0x14
        1.5f, // speed
        firebar_cb, // callback
        225.0f, // param
        0.0f, // radius
    },
    {
        1, // type
        5, // count
        {0}, // field_0x2
        {800.0f, 15.0f, -50.0f}, // pos
        25.0f, // spacing
        25.0f, // field_0x14
        1.5f, // speed
        firebar_cb, // callback
        225.0f, // param
        0.0f, // radius
    },
};

EVT_BEGIN(firebar_evt)
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(firebardesc))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(&firebardesc[1]))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(&firebardesc[2]))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(&firebardesc[3]))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(&firebardesc[4]))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(&firebardesc[5]))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(&firebardesc[6]))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT_GET_ID(LW(0))
        CALL(firebar_entry, PTR(&firebardesc[5]))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_TYPE(0)
        CALL(firebar_disp_evt)
    END_INLINE_EVENT
    RETURN
EVT_END()

s32 _firebar_hitCheck(FirebarWork* wp, FirebarBall* ball) {
    MarioWork* mario = marioGetPtr();
    f32 dx;
    f32 dz;
    f32 dy;
    BOOL near = FALSE;

    if (wp->flags & 6) {
        return 0;
    }
    dx = mario->position.x - ball->pos.x;
    dy = mario->position.y - ball->pos.y;
    dz = mario->position.z - ball->pos.z;
    if (dy > 0.0f) {
        if (dy < 0.8f * (15.0f * wp->scale)) {
            near = TRUE;
        }
    } else if (__fabsf(dy) < mario->wPlayerCollisionBox.y) {
        near = TRUE;
    }
    if (near && sqrtf(dx * dx + dz * dz) <= mario->wPlayerCollisionBox.x / 2.0f / 2.0f + wp->spacing / 2.0f * wp->scale &&
        vivianGetStatus() != 1 && vivianGetStatus() != 2) {
        return 1;
    }
    return 0;
}

s32 _firebar_hitCheck2(FirebarWork* wp) {
    FirebarBall* balls = wp->balls;
    MarioWork* mario = marioGetPtr();
    Vec rel;
    Vec dir;
    Vec side;
    Vec up = {0.0f, 1.0f, 0.0f};
    f32 dy;
    f32 len;
    f32 w;
    f32 dp;
    f32 ds;
    f32 r;
    BOOL near;

    if (wp->flags & 6) {
        return 0;
    }
    dy = mario->position.y - wp->pos.y;
    near = FALSE;
    if (dy > 0.0f) {
        if (dy < 0.8f * (15.0f * wp->scale)) {
            near = TRUE;
        }
    } else if (__fabsf(dy) < mario->wPlayerCollisionBox.y) {
        near = TRUE;
    }
    if (!near) {
        return 0;
    }
    PSVECSubtract(&balls[wp->count - 1].pos, &balls[0].pos, &dir);
    PSVECCrossProduct(&dir, &up, &side);
    len = PSVECMag(&dir);
    w = 5.0f * wp->scale;
    PSVECNormalize(&dir, &dir);
    PSVECNormalize(&side, &side);
    PSVECSubtract(&mario->position, &balls[0].pos, &rel);
    dp = PSVECDotProduct(&dir, &rel);
    ds = PSVECDotProduct(&side, &rel);
    r = 10.0f + mario->wPlayerCollisionBox.x;
    if (dp > -r / 2.0f && dp <= r / 2.0f + len && __fabsf(ds) <= mario->wPlayerCollisionBox.x / 2.0f + w &&
        vivianGetStatus() != 1 && vivianGetStatus() != 2) {
        return 1;
    }
    return 0;
}

void _firebar_rot(FirebarWork* wp) {
    FirebarBall* ball = wp->balls;
    MarioWork* mario = marioGetPtr();
    s32 hit;
    f32 dist;
    f32 limit;
    f32 angle1;
    f32 angle2;
    f32 diff;
    f32 r;
    f32 s;
    f32 c;
    f32 dx;
    f32 dz;
    s32 i;

    sin(6.2831855f * wp->shadowAngle / 360.0f);
    for (i = 0; i < wp->count; i++, ball++) {
        animPoseMain(ball->pose);
        if (!(wp->flags & 1)) {
            r = wp->scale * (i * wp->spacing) + wp->radius;
            s = sin(wp->angle);
            c = cos(wp->angle);
            dx = r * s;
            dz = -r * c;
            ball->pos.x = wp->pos.x + dx;
            ball->pos.y = wp->pos.y;
            ball->pos.z = wp->pos.z + dz;
        }
    }
    wp->angle = wp->speed * (1.0f + wp->hitCount / 10.0f) + wp->angle;
    hit = _firebar_hitCheck2(wp);
    dist = distABf(wp->pos.x, wp->pos.z, mario->position.x, mario->position.z);
    limit = mario->wPlayerCollisionBox.x / 2.0f / 2.0f + wp->spacing / 2.0f * wp->scale / 2.0f +
            distABf(wp->pos.x, wp->pos.z, wp->balls[wp->count - 1].pos.x, wp->balls[wp->count - 1].pos.z);
    angle1 = angleABf(wp->pos.x, wp->pos.z, mario->position.x, mario->position.z);
    angle2 = angleABf(wp->pos.x, wp->pos.z, wp->balls[wp->count - 1].pos.x, wp->balls[wp->count - 1].pos.z);
    diff = compAngle(angle1, angle2);
    if (hit > 0) {
        wp->hitCount = 0;
        if (wp->callback != NULL) {
            wp->callback(wp, 2);
        }
    } else if (dist > wp->radius && dist < limit && !(wp->flags & 6) && hit == 0 && vivianGetStatus() != 1 && vivianGetStatus() != 2) {
        if (wp->speed > 0.0f) {
            if (wp->prevAngle < 0.0f && diff > 0.0f && wp->pos.y < mario->position.y) {
                wp->hitCount++;
                if (wp->callback != NULL) {
                    wp->callback(wp, 1);
                }
            }
        } else if (wp->speed < 0.0f) {
            if (wp->prevAngle > 0.0f && diff < 0.0f && wp->pos.y < mario->position.y) {
                wp->hitCount++;
                if (wp->callback != NULL) {
                    wp->callback(wp, 1);
                }
            }
        }
    }
    wp->prevAngle = diff;
}

void _firebar_trans(FirebarWork* wp) {
    FirebarBall* ball = wp->balls;
    MarioWork* mario = marioGetPtr();
    s32 hit;
    f32 angle1;
    f32 angle2;
    f32 diff;
    f32 k;
    f32 dy;
    f32 dz;
    s32 i;

    sin(6.2831855f * wp->shadowAngle / 360.0f);
    switch (wp->state) {
        case 0:
            wp->scale += 0.05f;
            if (wp->scale > 1.0f) {
                wp->scale = 1.0f;
                wp->state++;
            }
            break;
        case 1:
            if (__fabsf(wp->pos.x - wp->endX) < 20.0f * __fabsf(wp->speed)) {
                wp->state++;
            }
            break;
        case 2:
            wp->scale -= 0.05f;
            if (wp->scale < 0.0f) {
                wp->pos.x = wp->setup->pos.x;
                wp->state = 0;
            }
            break;
    }
    if (wp->state == 1) {
        k = wp->scale;
    } else {
        k = 1.0f;
    }
    wp->pos.x += wp->speed;
    for (i = 0; i < wp->count; i++, ball++) {
        animPoseMain(ball->pose);
        if (!(wp->flags & 1)) {
            dz = k * (i * wp->spacing);
            dy = 0.5f * (20.0f * (1.0f - wp->scale));
            ball->pos.x = wp->pos.x;
            ball->pos.y = wp->pos.y + dy;
            ball->pos.z = wp->pos.z + dz;
        }
    }
    if (wp->state == 1) {
        hit = _firebar_hitCheck2(wp);
        angle1 = angleABf(wp->pos.x, wp->pos.z, mario->position.x, mario->position.z);
        angle2 = angleABf(wp->pos.x, wp->pos.z, wp->balls[wp->count - 1].pos.x, wp->balls[wp->count - 1].pos.z);
    diff = compAngle(angle1, angle2);
        if (hit > 0) {
            wp->hitCount = 0;
            if (wp->callback != NULL) {
                wp->callback(wp, 2);
            }
        } else if (!(wp->flags & 6) && hit == 0 && vivianGetStatus() != 1 && vivianGetStatus() != 2) {
            if (wp->prevAngle < 0.0f && diff > 0.0f && wp->pos.y < mario->position.y) {
                wp->hitCount++;
                if (wp->callback != NULL) {
                    wp->callback(wp, 1);
                }
            }
        }
        wp->prevAngle = diff;
    }
}

USER_FUNC(firebar_entry) {
    FirebarSetup* setup = (FirebarSetup*)evtGetValue(event, event->args[0]);
    FirebarWork* wp;
    FirebarBall* ball;
    MarioWork* mario;
    FirebarBall* balls;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 j;
    s32 n;

    if (isFirstCall) {
        wp = _mapAlloc(sizeof(FirebarWork));
        memset(wp, 0, sizeof(FirebarWork));
        wp->type = setup->type;
        wp->pos = setup->pos;
        wp->count = setup->count;
        wp->callback = setup->callback;
        wp->field_0x20 = setup->field_0x14;
        wp->spacing = setup->spacing;
        wp->speed = setup->speed;
        wp->scale = 1.0f;
        wp->prevAngle = 0.0f;
        wp->angle = 0.0f;
        wp->hitCount = 0;
        wp->setup = setup;
        wp->endX = setup->param;
        wp->radius = setup->radius;
        wp->sfxId = 0;
        wp->state = 0;
        wp->shadowAngle = 0.0f;
        switch (wp->type) {
            case 0:
                wp->angle = setup->param;
                break;
            case 1:
                if (setup->pos.x < setup->param) {
                    wp->speed = __fabsf(setup->speed);
                } else {
                    wp->speed = -1.0f * __fabsf(setup->speed);
                }
                wp->scale = 0.0f;
                break;
        }
        wp->balls = _mapAlloc(wp->count * sizeof(FirebarBall));
        memset(wp->balls, 0, wp->count * sizeof(FirebarBall));
        ball = wp->balls;
        for (i = 0; i < wp->count; i++, ball++) {
            ball->pose = animPoseEntry("c_fireber", 0);
            animPosePeraOn(ball->pose);
            ball->pos = (Vec){0.0f, 0.0f, 0.0f};
            animPoseSetAnim(ball->pose, "FRB_S_1", TRUE);
        }
        event->userdata[0] = (EvtWord)wp;
    }
    *(s32*)&wp = event->userdata[0];
    if (wp->flags & 8) {
        if (psndSFXChk(wp->sfxId) == 0) {
            psndSFXOff(wp->sfxId);
            wp->sfxId = -1;
        }
        balls = wp->balls;
        for (j = 0; j < 3; j++) {
            n = (wp->count + 1) * j / 3;
            x = balls[n].pos.x + (rand() % 16 - 8);
            y = 10.0f + balls[n].pos.y;
            z = balls[n].pos.z + (rand() % 5 + 3);
            effKemuTestEntry(0, x, y, z, 1.0f);
        }
        ball = wp->balls;
        for (i = 0; i < wp->count; i++, ball++) {
            animPoseRelease(ball->pose);
        }
        _mapFree(wp->balls);
        _mapFree(wp);
        return EVT_RETURN_DONE;
    }
    mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE || mario->currMotionId == MARIO_MOTION_DAMAGE || mario->mutekiTimer > 0) {
        wp->flags |= 4;
    } else {
        wp->flags &= ~4;
    }
    if (__fabsf(mario->position.x - wp->pos.x) < 120.0f) {
        if (psndSFXChk(wp->sfxId)) {
            wp->sfxId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MOBJ_FIREBAR_ROUND1), &wp->pos);
        }
    } else if (psndSFXChk(wp->sfxId) == 0) {
        psndSFXOff(wp->sfxId);
        wp->sfxId = -1;
    }
    wp->shadowAngle += 6.0f;
    if (wp->shadowAngle > 360.0f) {
        wp->shadowAngle -= 360.0f;
    }
    switch (wp->type) {
        case 0:
            _firebar_rot(wp);
            break;
        case 1:
            _firebar_trans(wp);
            break;
    }
    if (wp->callback != NULL) {
        wp->callback(wp, 0);
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(firebar_disp_evt) {
    FirebarWork* wp;
    FirebarBall* ball;
    f32 a;
    s32 i;

    if (evtCheckID(event->lwData[0])) {
        wp = (FirebarWork*)evtGetPtrID(event->lwData[0])->userdata[0];
        a = sin(6.2831855f * wp->shadowAngle / 360.0f);
        a = 0.375f * a + 0.625f;
        ball = wp->balls;
        for (i = 0; i < wp->count; i++, ball++) {
            shadowEntryMode(2, ball->pos.x, ball->pos.y, ball->pos.z, 17.0f * wp->scale);
            shadowSetVolumeDecay(a, 0.2f * a, 0.4f * a);
        }
        dispEntry(CAMERA_3D, dispCalcZ(&wp->pos), 1, firebar_disp, wp);
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

void firebar_disp(CameraId camera, void* param) {
    FirebarWork* wp = param;
    FirebarBall* ball = wp->balls;
    Mtx mtx;
    s32 i;

    for (i = 0; i < wp->count; i++, ball++) {
        PSMTXTrans(mtx, ball->pos.x, ball->pos.y, ball->pos.z);
        animPoseDrawMtx(ball->pose, mtx, 1, 0.0f, 2.0f * wp->scale);
        animPoseDrawMtx(ball->pose, mtx, 2, 0.0f, 2.0f * wp->scale);
        animPoseDrawMtx(ball->pose, mtx, 3, 0.0f, 2.0f * wp->scale);
    }
}

USER_FUNC(coin_entry) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 z = evtGetValue(event, args[2]);
    s32 w = evtGetValue(event, args[3]);
    ItemEntry* item = itemEntry(NULL, ITEM_COIN, 3, -1, NULL, x, y, z);

    itemStatusOn(item, 0x10);
    item->bounceFactor = 0.8f;
    item->gravityScale = 1.0f;
    item->maxBounces = 5;
    item->velocityY = 550.0f;
    item->speed = 70.0f;
    item->direction = w;
    return EVT_RETURN_DONE;
}

void firebar_cb(FirebarWork* wp, s32 type) {
    MarioWork* mario = marioGetPtr();
    EventEntry* evt;

    switch (type) {
        case 1:
            if (wp->hitCount >= 10 && wp->type == 0) {
                wp->flags |= 8;
                evt = evtEntry(firebar_coin, 0, 0);
                evt->lwData[0] = wp->pos.x;
                evt->lwData[1] = wp->pos.y;
                evt->lwData[2] = wp->pos.z;
            }
            break;
        case 2:
            set_damage_root_ypos(mario->position.y);
            marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
            break;
    }
}

static HitDamageReturn dmgRetPoint[] = {
    {"A_oti_01", {0.0f, -1000.0f, 0.0f}},
    {"A_oti_02", {0.0f, -1000.0f, 0.0f}},
    {"A_oti_03", {0.0f, -1000.0f, 0.0f}},
    {0},
};

EVT_BEGIN(las_04_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS2"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_hit_damage_return_set, PTR(dmgRetPoint))
    CALL_CHILD(PTR(evt_gazigazi_entry))
    SPAWN_EVENT(PTR(firebar_evt))
    CALL(evt_map_playanim, STRING("mizu"), 1, 0)
    CALL(evt_map_playanim, STRING("hamon"), 1, 0)
    CALL(evt_mobj_brick, STRING("mobj_01"), 0, 150, 0, 134, 11, 0, GSWF(4384))
    CALL(evt_mobj_save_blk, STRING("mobj_save"), -950, 140, -100, 0, 0)
    RETURN
EVT_END()
