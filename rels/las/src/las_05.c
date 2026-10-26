#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "sfx_name_word.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_ext.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "evt/evt_pouch.h"
#include "mot/mot_hammer.h"
#include "driver/mapdrv.h"
#include "driver/itemdrv.h"

#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "mario/mario.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "pmario_sound.h"
#include "party/party_vivian.h"
#include "dolphin/mtx.h"
#include "memory.h"
#include "string.h"
#include "party.h"
#include "mario/mario_party.h"
#include "party/party_cloud.h"
#include "mario/mariost.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "stdio.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "las.h"
#include "libc_decls.h"
#include "data_tables.h"
#include "mario/mario_pouch.h"
#include "driver/extdrv.h"
#include <math.h>
#include "driver/seqdrv.h"

s32 _kurokaron_run_away(f32 x, f32 z, f32 tx, f32 tz);
s32 kurokaron_move2(NpcEntry* npc, MarioWork* mario);
void _kurokaron_next_move_index(void);
typedef struct Senkaron {
    s8 state;
    u16 area;
    s32 timer;
    f32 vx;
    f32 vy;
    f32 vz;
    u32 flags;
} Senkaron;
typedef struct SenkaronWork {
    s32 attackStart;
    s32 escape;
    s32 mesPushing;
    u32 frameCount;
    s32 marioPushed;
    s32 hammerFrame;
    Senkaron* sk;
} SenkaronWork;
SenkaronWork* skwp;
static f32 randf(f32 min, f32 max);
s32 nokonokoGetStatus(void);
double sqrt(double x);
s32 party_hit_check(f32 x, f32 z, f32* dir);
void calc_area(s32 i, f32 x, f32 z);
void move_check(ExtdrvPose* pose, s32 idx);
static void senkaron_ext_disp(CameraId camera, void* param);
static void senkaron_ext_shadow_disp(CameraId camera, void* param);

USER_FUNC(chk_mario_ctrloff);
USER_FUNC(evt_key_get_button);
USER_FUNC(evt_key_get_dir);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_win_item_select);
USER_FUNC(get_mario_y_pos);
USER_FUNC(get_party_y_pos);
USER_FUNC(kurokaron_flag_check);
USER_FUNC(kurokaron_get_move_pos);
USER_FUNC(kurokaron_move_position);
static USER_FUNC(make_name);
USER_FUNC(mario_land_on);
USER_FUNC(party_hit);
USER_FUNC(party_land_on);
static USER_FUNC(senkaronAttackStart);
static USER_FUNC(senkaronFlagOffMesPushing);
static USER_FUNC(senkaronFlagOnMesPushing);
static USER_FUNC(senkaronGetMarioPushedFlag);
static USER_FUNC(senkaronToEscape);
USER_FUNC(set_mario_y_pos);
static USER_FUNC(set_party_y_pos);
static void senkaron_ext_dispent(void);
static void senkaron_ext_main(void);
static void senkaron_ext_init(void);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static EvtWord e_door_close[22];
static EvtWord e_door_open[22];
static BeroEntry bero_entry_data[3];
EvtWord kurokaron_move[103];
static ExtTexInfo senkaron_ext_entry_data[26];
s32 move_check_list[20];
static EvtWord senkaron_event[917];
EvtWord kurokaron_init[29];
EvtWord kurokaron_talk[13];
static NpcSetupInfo npcEnt[3];
static EvtWord key_evt[5];
static s32 key_tbl[2];
static EvtWord key_check_evt[23];
static EvtWord unlock_evt[10];
static EvtWord shan_init[68];
EvtWord las_05_init_evt[63];

USER_FUNC(kurokaron_flag_check) {
    EvtWord* args = event->args;
    s32 mask = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], kurokaron_move_flag & mask);
    return EVT_RETURN_DONE;
}

USER_FUNC(kurokaron_get_move_pos) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], kurokaron_move_list[kurokaron_move_index][0]);
    evtSetFloat(event, args[1], kurokaron_move_list[kurokaron_move_index][1]);
    return EVT_RETURN_DONE;
}

s32 _kurokaron_run_away(f32 x, f32 z, f32 tx, f32 tz) {
    s32 cand[6];
    f32 base;
    f32 diff;
    f32 curDiff;
    f32 nextDiff;
    u8 cur;
    s32 cnt;
    s32 next;
    s32 i;
    s32 result;

    base = angleABf(x, z, tx, tz);
    cur = kurokaron_move_index;
    if (!(kurokaron_move_flag & 1)) {
        if (cur == 0) {
            next = 5;
        } else {
            next = cur - 1;
        }
    } else {
        next = (cur + 1) % 6;
    }
    cnt = 0;
    for (i = 0; i < 6; i++) {
        diff = __fabsf(compAngle(angleABf(x, z, kurokaron_move_list[i][0], kurokaron_move_list[i][1]), base));
        if (i == cur) {
            curDiff = diff;
        }
        if (i == next) {
            nextDiff = diff;
        }
        if (diff > 90.0f) {
            cand[cnt++] = i;
        }
    }
    if (cnt > 0) {
        result = cand[rand() % cnt];
        if (curDiff < 90.0f) {
            if (!(kurokaron_move_flag & 1)) {
                kurokaron_move_flag |= 1;
            } else {
                kurokaron_move_flag &= ~1;
            }
        }
    } else if (curDiff < nextDiff) {
        if (!(kurokaron_move_flag & 1)) {
            if (kurokaron_move_index == 0) {
                result = 5;
            } else {
                result = kurokaron_move_index - 1;
            }
            kurokaron_move_flag |= 1;
        } else {
            kurokaron_move_flag &= ~1;
            result = (kurokaron_move_index + 1) % 6;
        }
    }
    return result;
}

u8 kurokaron_move_index = 0;
u8 kurokaron_move_flag = 0;
u8 kurokaron_same_goal = 0;
s32 kurokaron_vivian_prev_sts = 0;
u8 kurokaron_stop_cnt = 0;
u16 kurokaron_run_cnt = 0;

s32 kurokaron_move2(NpcEntry* npc, MarioWork* mario) {
    static s32 flag = 0;
    static s32 count = 0;
    f32 angle;

    if (!(kurokaron_move_flag & 2)) {
        flag = 0;
        return 0;
    }
    kurokaron_move_flag &= ~4;
    if (npc->position.x < -560.0f || npc->position.x > 560.0f || npc->position.z < -260.0f || npc->position.z > 260.0f) {
        if (rand() % 100 == 0) {
            if (kurokaron_move_flag & 8) {
                kurokaron_move_flag &= ~8;
            } else {
                kurokaron_move_flag |= 8;
            }
        }
        angle = angleABf(mario->position.x, mario->position.z, npc->position.x, npc->position.z);
        if (kurokaron_move_flag & 8) {
            npc->moveDir = reviseAngle(angle - 90.0f);
        } else {
            npc->moveDir = reviseAngle(angle + 90.0f);
        }
        kurokaron_move_flag |= 4;
        flag = 0;
        return 0;
    }
    if (distABf(npc->position.x, npc->position.z, mario->position.x, mario->position.z) < 100.0f) {
        if (count != 0) {
            count--;
        } else {
            if (npc->position.x < -490.0f || npc->position.x > 490.0f || npc->position.z < -190.0f || npc->position.z > 190.0f) {
                angle = angleABf(mario->position.x, mario->position.z, npc->position.x, npc->position.z);
                if (kurokaron_move_flag & 8) {
                    npc->moveDir = angle - 90.0f;
                } else {
                    npc->moveDir = angle + 90.0f;
                }
                count = 10;
            } else {
                npc->moveDir = angleABf(mario->position.x, mario->position.z, npc->position.x, npc->position.z);
                count = 20;
            }
            flag = 0;
        }
        return 0;
    }
    if (flag == 0) {
        kurokaron_move_index = _kurokaron_run_away(npc->position.x, npc->position.z, mario->position.x, mario->position.z);
        flag = 1;
        return 1;
    }
    return 0;
}

void _kurokaron_next_move_index(void) {
    if (kurokaron_move_flag & 1) {
        if (kurokaron_move_index == 0) {
            kurokaron_move_index = 5;
        } else {
            kurokaron_move_index--;
        }
    } else {
        kurokaron_move_index++;
        kurokaron_move_index %= 6;
    }
}

USER_FUNC(kurokaron_move_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    f32 speed = evtGetFloat(event, args[3]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    MarioWork* mario;
    Vec diff;
    f32 step;
    f32 marioDist;
    f32 dist;

    npc->jumpFlags |= 0x10;
    if (isFirstCall) {
        npc->moveTicks = 0;
        npc->moveAnimState = 0;
        npc->jumpTargetPos.x = x;
        npc->jumpTargetPos.y = npc->position.y;
        npc->jumpTargetPos.z = z;
        npc->jumpStartPos = npc->position;
        PSVECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &diff);
        dist = sqrtf(diff.x * diff.x + diff.z * diff.z);
        npc->moveDir = angleABf(0.0f, 0.0f, diff.x, diff.z);
        npc->moveSpeed = speed;
        npc->moveOffset.y = 0.0f;
        if (npc->moveSpeed <= 0.1f && npc->moveTime == 0.0f) {
            npc->moveSpeed = 1.0f;
        }
        npc->footstepAccum = 0.0f;
        npc->footstepToggle = TRUE;
    }
    mario = marioGetPtr();
    PSVECSubtract(&npc->jumpTargetPos, &npc->position, &diff);
    dist = sqrtf(diff.x * diff.x + diff.z * diff.z);
    step = (20.0f * npc->moveSpeed) / 1000.0f;
    PSVECSubtract(&mario->position, &npc->position, &diff);
    marioDist = sqrtf(diff.x * diff.x + diff.z * diff.z);
    if (marioDist < 80.0f && kurokaron_vivian_prev_sts == 3 && vivianGetStatus() == 0) {
        kurokaron_stop_cnt = 60;
    }
    kurokaron_vivian_prev_sts = vivianGetStatus();
    if (kurokaron_stop_cnt != 0) {
        npc->jumpFlags &= ~0x10;
        kurokaron_stop_cnt--;
        return EVT_RETURN_BLOCK;
    }
    if (!(kurokaron_move_flag & 2) && vivianGetStatus() != 1 && vivianGetStatus() != 2 && vivianGetStatus() != 3 &&
        marioDist < 80.0f) {
        kurokaron_move_index = _kurokaron_run_away(npc->position.x, npc->position.z, mario->position.x, mario->position.z);
        kurokaron_run_cnt = 350;
        kurokaron_move_flag |= 2;
        return EVT_RETURN_DONE;
    }
    if (kurokaron_move2(npc, mario)) {
        return EVT_RETURN_DONE;
    }
    if (kurokaron_run_cnt != 0) {
        kurokaron_run_cnt--;
    }
    if (npc->wallHit) {
        f32 angle = angleABf(mario->position.x, mario->position.z, npc->position.x, npc->position.z);

        if (kurokaron_move_flag & 4) {
            if (kurokaron_move_flag & 8) {
                kurokaron_move_flag &= ~8;
                npc->moveDir = reviseAngle(angle + 90.0f);
            } else {
                kurokaron_move_flag |= 8;
                npc->moveDir = reviseAngle(angle - 90.0f);
            }
        }
        kurokaron_run_cnt = 350;
        kurokaron_move_flag |= 2;
    }
    if (dist <= step && marioDist > 100.0f) {
        npc->moveSpeed = 0.0f;
        kurokaron_move_flag &= ~2;
        if (kurokaron_prev_goal == kurokaron_move_index) {
            if (++kurokaron_same_goal > 3) {
                kurokaron_move_flag |= 2;
            }
        } else {
            kurokaron_same_goal = 0;
        }
        kurokaron_prev_goal = kurokaron_move_index;
        _kurokaron_next_move_index();
        return EVT_RETURN_DONE;
    }
    if ((npc->jumpFlags & 2) && npc->moveSpeed > 0.1f) {
        npcTuningRy(npc, npc->moveDir);
    }
    if ((kurokaron_move_flag & 2) && kurokaron_run_cnt != 0) {
        speed = 1.8f * npc->moveSpeed;
    } else {
        speed = npc->moveSpeed;
    }
    {
        f32 temp = (f32)sin(npc->moveDir * 6.2831855f / 360.0f);
        npc->moveOffset.x += npc->moveDt * (speed * temp);
        temp = (f32)cos(npc->moveDir * 6.2831855f / 360.0f);
        npc->moveOffset.z += npc->moveDt * (speed * -temp);
    }
    if (npc->footstepAccum > (f32)npc->footstepStride / npc->moveSpeed) {
        if (npc->footstepToggle != 0) {
            if (npc->footstepSfxA != NULL) {
                psndSFXOn_3D(npc->footstepSfxA, &npc->position);
            }
        } else {
            if (npc->footstepSfxB != NULL) {
                psndSFXOn_3D(npc->footstepSfxB, &npc->position);
            }
        }
        npc->footstepAccum = 0.0f;
        npc->footstepToggle = (s8)(1 - npc->footstepToggle);
    }
    if (npc->footstepSfxB != NULL || npc->footstepToggle != 0) {
        npc->footstepAccum += 1000.0f * npc->moveDt;
    }
    return EVT_RETURN_BLOCK;
}


static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

static void senkaron_ext_init(void) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();
    Senkaron* sk;

    skwp = _mapAlloc(0x18);
    memset(skwp, 0, 0x18);
    skwp->sk = _mapAlloc(sizeof(Senkaron) * 1000);
    sk = skwp->sk;
    for (i = extGetPoseNum() - 1; i >= 0; i--, pose++) {
        pose->flags = 3;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = randf(-570.0f, 570.0f);
        (*pose->mtx)[1][3] = 250.0f;
        (*pose->mtx)[2][3] = randf(-270.0f, 270.0f);
        pose->dir = 90.0f;
        sk[i].state = 0;
        sk[i].timer = randf(1.0f, 200.0f);
        sk[i].vy = -3.0f;
    }
}

void move_check(ExtdrvPose* pose, s32 idx) {
    MarioWork* mario;
    Senkaron* sk;
    ExtdrvPose* pose2;
    s32 cnt;
    s32 j;

    extGetPosePtr();
    mario = marioGetPtr();
    sk = skwp->sk;
    cnt = extGetPoseNum() - (idx + 1);
    if (sk[idx].state == 1) {
        pose2 = pose + 1;
        for (j = idx + 1; cnt != 0; j++, pose2++, cnt--) {
            if (sk[j].state == 1 && sk[j].area == sk[idx].area) {
                f32 dx1 = __fabsf((*pose->mtx)[0][3] - mario->position.x);
                f32 dz1 = __fabsf((*pose->mtx)[2][3] - mario->position.z);
                f32 dx2 = __fabsf((*pose2->mtx)[0][3] - mario->position.x);
                f32 dz2 = __fabsf((*pose2->mtx)[2][3] - mario->position.z);

                if (dx1 * dx1 + dz1 * dz1 > dx2 * dx2 + dz2 * dz2) {
                    if (__fabsf(__fabsf((*pose->mtx)[0][3] - (*pose2->mtx)[0][3]) - __fabsf((*pose->mtx)[2][3] - (*pose2->mtx)[2][3])) < 20.0f) {
                        sk[idx].flags |= 2;
                    }
                } else {
                    sk[j].flags |= 2;
                }
            }
        }
    }
}

void calc_area(s32 i, f32 x, f32 z) {
    f32 fx = x - -570.0f;
    f32 fz = z - -270.0f;
    u16 iz;
    u16 ix;

    fx /= 40.0f;
    fz /= 40.0f;
    iz = fz;
    ix = fx;
    skwp->sk[i].area = ix + iz * 29.5f;
}

s32 party_hit_check(f32 x, f32 z, f32* dir) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    s32 hit = 0;
    s32 active = 0;
    f32 dx;
    f32 dz;

    if (party == NULL) {
        return 0;
    }
    dx = __fabsf(x - party->position.x);
    dz = __fabsf(z - party->position.z);
    switch (party->currentMemberId) {
        case 2:
            switch (nokonokoGetStatus()) {
                case 2:
                case 3:
                case 4:
                case 5:
                    active = 1;
                    break;
            }
            if (active && dx < party->width && dz < party->width) {
                hit = 1;
                if (x < party->position.x) {
                    *dir = 1.0f;
                } else {
                    *dir = -1.0f;
                }
            }
            break;
        case 3:
            if (partyGetPtr(0)->useMotionId == 0x46 && dx < 80.0f && dz < 80.0f) {
                hit = 1;
                if (x < party->position.x) {
                    *dir = 1.0f;
                } else {
                    *dir = -1.0f;
                }
            }
            break;
        case 5:
            if ((party->flags & PARTY_FLAG_IS_BEING_USED) && dx < cloudGetBreathDist() && dz < 30.0f) {
                if (cloudGetBreathDir() < 180.0f) {
                    if (x > party->position.x) {
                        hit = 1;
                        *dir = -1.0f;
                    }
                } else {
                    if (x < party->position.x) {
                        hit = 1;
                        *dir = 1.0f;
                    }
                }
            }
            break;
    }
    return hit;
}

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
        "las_04", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {
        "e_bero_1", // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_06", // nextMapName
        "w_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_door_close, // enterAnimeArg
        e_door_open, // exitAnimeArg
    },
    {0},
};

u8 kurokaron_prev_goal = 5;

f32 kurokaron_move_list[6][2] = {
    {500.0f, -130.0f}, {0.0f, -200.0f}, {-500.0f, -130.0f}, {-500.0f, 130.0f}, {0.0f, 200.0f}, {500.0f, 130.0f},
};

EVT_BEGIN(kurokaron_move)
    SET_VALUE(LW(3), 0)
    SET_FLOAT_VALUE(GW(0), FLOAT(70.0f))
    SET_VALUE(GW(1), 200)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(13), 0)
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(15))
        LOOP(0)
            CALL(evt_sub_get_stopwatch, LW(15), LW(14))
            IF_INT_GREATER_OR_EQUAL(LW(14), 10000)
                ADD(LW(13), 1)
                IF_INT_EQUAL(LW(13), 6)
                    BREAK_LOOP
                END_IF
                SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(15))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        SET_FLOAT_VALUE(GW(0), FLOAT(50.0f))
        SET_VALUE(GW(1), 300)
        DEBUG_PUT_MSG(STRING("kurokaron 1 minute late"))
    END_SIBLING_EVENT
    SET_VALUE(LF(0), 0)
    LOOP(0)
        CALL(kurokaron_get_move_pos, LW(0), LW(1))
        CALL(kurokaron_move_position, STRING("黒カロン"), LW(0), LW(1), GW(0))
        CALL(kurokaron_flag_check, 2, LW(2))
        IF_INT_NOT_EQUAL(LW(2), 0)
            IF_INT_EQUAL(LW(3), 0)
                IF_INT_EQUAL(LF(0), 0)
                    WAIT_MS(GW(1))
                    SET_VALUE(LF(0), 1)
                    ELSE
                    CALL(evt_sub_random, 300, LW(5))
                    ADD(LW(5), GW(1))
                    WAIT_MS(LW(5))
                END_IF
            END_IF
        END_IF
        SET_VALUE(LW(3), LW(2))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static ExtTexInfo senkaron_ext_entry_data[] = {
    {"c_karon_h", "KRN_R_1", 0.0f},
    {"c_karon_h", "KRN_R_1", 1.0f},
    {"c_karon_h", "KRN_R_1", 2.0f},
    {"c_karon_h", "KRN_R_1", 3.0f},
    {"c_karon_h", "KRN_R_1", 4.0f},
    {"c_karon_h", "KRN_R_1", 5.0f},
    {"c_karon_h", "KRN_R_1", 6.0f},
    {"c_karon_h", "KRN_R_1", 7.0f},
    {"c_karon_h", "KRN_R_1", 8.0f},
    {"c_karon_h", "KRN_R_1", 9.0f},
    {"c_karon_h", "KRN_R_1", 10.0f},
    {"c_karon_h", "KRN_R_1", 11.0f},
    {"c_karon_h", "KRN_R_1", 12.0f},
    {"c_karon_h", "KRN_R_1", 13.0f},
    {"c_karon_h", "KRN_R_1", 14.0f},
    {"c_karon_h", "KRN_R_1", 15.0f},
    {"c_karon_h", "KRN_R_1", 16.0f},
    {"c_karon_h", "KRN_R_1", 17.0f},
    {"c_karon_h", "KRN_R_1", 18.0f},
    {"c_karon_h", "KRN_R_1", 19.0f},
    {"c_karon_h", "KRN_R_1", 20.0f},
    {"c_karon_h", "KRN_R_1", 21.0f},
    {"c_karon_h", "KRN_R_1", 22.0f},
    {"c_karon_h", "KRN_R_1", 23.0f},
    {"c_karon_h", "KRN_D_1", 0.0f},
    {0},
};

s32 move_check_list[] = {
    25, 52, 79, 107, 136, 166, 197, 229,
    263, 298, 335, 374, 416, 461, 510, 565,
    628, 703, 806, 1000,
};

static void senkaron_ext_main(void) {
    MarioWork* mario;
    ExtdrvPose* pose;
    s32 hit;
    f32 range;
    f32 near;
    ExtdrvPose* p;
    Senkaron* s;
    Senkaron* sk;
    ExtdrvPose* ep;
    const char* sfx[2];
    s32 hammerDir;
    f32 dist;
    s32 group;
    s32 start;
    s32 end;
    s32 i;
    f32 scale;
    f32 push;
    f32 dx;
    f32 adx;
    f32 adz;
    f32 angle;
    f32 speed;
    f32 d2;
    f32 mx;
    f32 mz;

    pose = extGetPosePtr();
    mario = marioGetPtr();
    hammerDir = 0;
    sk = skwp->sk;
    scale = mario->wPlayerCollisionBox.x / 2.0f;
    skwp->marioPushed = 0;
    if (mario->currMotionId == MARIO_MOTION_HAMMER) {
        skwp->hammerFrame++;
        skwp->hammerFrame %= 15;
    } else {
        skwp->hammerFrame = 0;
    }
    if (mario->currMotionId == MARIO_MOTION_HAMMER && skwp->hammerFrame == 10) {
        if (0.0f == mario->displayDirectionTarget) {
            hammerDir = 1;
        } else if (180.0f == mario->displayDirectionTarget) {
            hammerDir = 2;
        } else if (mario->displayDirection < 90.0f || 270.0f < mario->displayDirection) {
            hammerDir = 1;
        } else {
            hammerDir = 2;
        }
    }
    group = skwp->frameCount % 20;
    if (group != 0) {
        start = move_check_list[group - 1] + 1;
    } else {
        start = 0;
    }
    end = move_check_list[group];
    p = &pose[start];
    for (i = start; i < end; i++, p++) {
        sk[i].flags &= ~2;
        calc_area(i, (*p->mtx)[0][3], (*p->mtx)[2][3]);
    }
    p = &extGetPosePtr()[start];
    for (i = start; i < end; i++, p++) {
        move_check(p, i);
    }
    skwp->frameCount++;
    ep = extGetPosePtr();
    sfx[0] = "SFX_STG8_KARON_LANDING1";
    range = 1.5f * (6.0f * scale);
    sfx[1] = "SFX_STG8_KARON_FLY1";
    near = 3.0f * scale;
    for (i = 0; i < 1000; i++, ep++) {
        s = &sk[i];
        switch (s->state) {
            case 0:
                ep->texId = (gp->retraceCount + i * i) % 24;
                if (s->timer > 0) {
                    if (skwp->escape) {
                        ep->flags &= ~EXT_POSE_FLAG_VISIBLE;
                        s->state = 0x63;
                    } else if (skwp->attackStart) {
                        s->timer--;
                    }
                    break;
                }
                (*ep->mtx)[1][3] += s->vy;
                s->vy -= 0.3f;
                if ((*ep->mtx)[1][3] < 0.0f) {
                    (*ep->mtx)[1][3] = 0.0f;
                    s->state = 1;
                    s->timer = 0;
                    s->flags &= 0xFFFE;
                    if (i % 8 == 0) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*ep->mtx)[0][3];
                        pos.y = (*ep->mtx)[1][3];
                        pos.z = (*ep->mtx)[2][3];
                        psndSFXOn_3D(sfx[0], &pos);
                    }
                }
                break;
            case 1:
                ep->texId = (gp->retraceCount + i * i) % 24;
                if (skwp->escape) {
                    if (0.0f == (*ep->mtx)[0][3] && 0.0f == (*ep->mtx)[2][3]) {
                        s->vx = 1.0f;
                        s->vz = 1.0f;
                    } else {
                        f32 d1 = sqrt((*ep->mtx)[0][3] * (*ep->mtx)[0][3] + (*ep->mtx)[2][3] * (*ep->mtx)[2][3]);
                        s->vx = 4.0f * (*ep->mtx)[0][3] / d1;
                        s->vz = 4.0f * (*ep->mtx)[2][3] / d1;
                    }
                    if (s->vx < 0.0f) {
                        ep->dir = 90.0f;
                    } else {
                        ep->dir = 270.0f;
                    }
                    s->state = 0x62;
                    s->timer = 300;
                    break;
                }
                hit = 0;
                dx = (*ep->mtx)[0][3] - mario->position.x;
                push = 0.0f;
                adx = __fabsf(dx);
                adz = __fabsf((*ep->mtx)[2][3] - mario->position.z);
                if ((*ep->mtx)[0][3] >= mario->position.x) {
                    if (s->flags & 1) {
                        ep->dir = 270.0f;
                    } else {
                        ep->dir = 90.0f;
                    }
                } else {
                    if (s->flags & 1) {
                        ep->dir = 90.0f;
                    } else {
                        ep->dir = 270.0f;
                    }
                }
                if (skwp->mesPushing) {
                    break;
                }
                if (hammerDir != 0 && adz < 80.0f) {
                    if (hammerDir == 2) {
                        if ((*ep->mtx)[0][3] > mario->position.x && adx < range) {
                            hit = 1;
                            push = 1.0f;
                        }
                    } else {
                        if ((*ep->mtx)[0][3] < mario->position.x && adx < range) {
                            hit = 1;
                            push = -1.0f;
                        }
                    }
                }
                angle = marioGetAngleKaitenHammer();
                if (angle >= 0.0f && adx < 80.0f && adz < 80.0f) {
                    if (angle >= 0.0f && angle < 90.0f) {
                        if ((*ep->mtx)[2][3] > mario->position.z && (*ep->mtx)[0][3] > mario->position.x) {
                            hit = 1;
                            push = 1.0f;
                        }
                    } else if (angle >= 90.0f && angle < 180.0f) {
                        if ((*ep->mtx)[2][3] > mario->position.z && (*ep->mtx)[0][3] < mario->position.x) {
                            hit = 1;
                            push = -1.0f;
                        }
                    } else if (angle >= 180.0f && angle < 270.0f) {
                        if ((*ep->mtx)[2][3] < mario->position.z && (*ep->mtx)[0][3] < mario->position.x) {
                            hit = 1;
                            push = -1.0f;
                        }
                    } else {
                        if ((*ep->mtx)[2][3] < mario->position.z && (*ep->mtx)[0][3] > mario->position.x) {
                            hit = 1;
                            push = -1.0f;
                        }
                    }
                }
                {
                    f32 dir = 0.0f;

                    if (party_hit_check((*ep->mtx)[0][3], (*ep->mtx)[2][3], &dir)) {
                        hit = 2;
                        push = dir;
                    }
                }
                if (hit != 0) {
                    d2 = adx * adx + adz * adz;
                    if (d2 > 0.0f) {
                        f64 guess = __frsqrte(d2);
                        guess = .5 * guess * (3.0 - guess * guess * d2);
                        guess = .5 * guess * (3.0 - guess * guess * d2);
                        guess = .5 * guess * (3.0 - guess * guess * d2);
                        d2 = d2 * guess;
                    } else if (d2 < 0.0) {
                        d2 = NAN;
                    } else if (isnan(d2)) {
                        d2 = NAN;
                    }

                    s->vx = push + 3.0f * dx / d2;
                    s->vy = randf(5.0f, 10.0f);
                    s->vz = randf(-5.0f, 10.0f);
                    s->state = 0x61;
                    if (hit == 1) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*ep->mtx)[0][3];
                        pos.y = (*ep->mtx)[1][3];
                        pos.z = (*ep->mtx)[2][3];
                        psndSFXOn_3D(sfx[1], &pos);
                    }
                    break;
                }
                if (adz < 40.0f && vivianGetStatus() != 1 && vivianGetStatus() != 2 && vivianGetStatus() != 3) {
                    if ((*ep->mtx)[0][3] > 0.0f) {
                        if ((*ep->mtx)[0][3] > mario->position.x + scale - 10.0f && (*ep->mtx)[0][3] < mario->position.x + near) {
                            skwp->marioPushed |= 1;
                        }
                    } else {
                        if ((*ep->mtx)[0][3] > mario->position.x - near && (*ep->mtx)[0][3] < 10.0f + (mario->position.x - scale)) {
                            skwp->marioPushed |= 2;
                        }
                    }
                }
                if (s->timer == 0) {
                    f32 tx = (*ep->mtx)[0][3] - mario->position.x;
                    f32 tz = (*ep->mtx)[2][3] - mario->position.z;
                    f32 td = 10.0f * (f32)sqrt(tx * tx + tz * tz);
                    s32 r = rand() % 50;

                    s->vx = randf(-0.5f, 0.5f);
                    s->vz = randf(-0.5f, 0.5f);
                    s->vx -= tx / td;
                    s->vz -= tz / td;
                    if (r == 1) {
                        s->flags |= 1;
                    } else {
                        s->flags &= 0xFFFE;
                    }
                    s->timer = 60;
                }
                s->timer--;
                if (s->flags & 2) {
                    (*ep->mtx)[0][3] -= s->vx;
                    (*ep->mtx)[2][3] -= s->vz;
                } else {
                    (*ep->mtx)[0][3] += s->vx;
                    (*ep->mtx)[2][3] += s->vz;
                }
                if ((*ep->mtx)[0][3] > 570.0f) {
                    (*ep->mtx)[0][3] = 570.0f;
                }
                if ((*ep->mtx)[0][3] < -570.0f) {
                    (*ep->mtx)[0][3] = -570.0f;
                }
                if ((*ep->mtx)[2][3] > 270.0f) {
                    (*ep->mtx)[2][3] = 270.0f;
                }
                if ((*ep->mtx)[2][3] < -270.0f) {
                    (*ep->mtx)[2][3] = -270.0f;
                }
                break;
            case 0x61:
                ep->flags |= EXT_POSE_FLAG_NO_SHADOW;
                if ((*ep->mtx)[1][3] > -100.0f) {
                    (*ep->mtx)[0][3] += s->vx;
                    (*ep->mtx)[1][3] += s->vy;
                    (*ep->mtx)[2][3] += s->vz;
                    if ((*ep->mtx)[2][3] <= -270.0f) {
                        (*ep->mtx)[2][3] = -270.0f;
                    }
                    s->vy -= 0.3f;
                    ep->texId = 24;
                } else {
                    marioGetPtr();
                    (*ep->mtx)[0][3] = randf(-570.0f, 570.0f);
                    (*ep->mtx)[1][3] = 250.0f;
                    (*ep->mtx)[2][3] = randf(-270.0f, 270.0f);
                    ep->dir = randf(0.0f, 360.0f);
                    ep->flags &= ~EXT_POSE_FLAG_NO_SHADOW;
                    s->state = 0;
                    s->timer = rand() % 1000 + 1;
                    s->vy = 0.0f;
                }
                break;
            case 0x62:
                if (s->timer > 0) {
                    (*ep->mtx)[0][3] += s->vx;
                    (*ep->mtx)[2][3] += s->vz;
                    s->timer--;
                    if (s->timer % 10 == 0) {
                        if (!((s->timer / 5) & 1)) {
                            Vec pos = {0.0f, 0.0f, 0.0f};
                            pos.x = (*ep->mtx)[0][3];
                            pos.y = (*ep->mtx)[1][3];
                            pos.z = (*ep->mtx)[2][3];
                            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_FLD_ENM_HONENOKO_MOVE1L), &pos);
                        } else {
                            Vec pos = {0.0f, 0.0f, 0.0f};
                            pos.x = (*ep->mtx)[0][3];
                            pos.y = (*ep->mtx)[1][3];
                            pos.z = (*ep->mtx)[2][3];
                            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_FLD_ENM_HONENOKO_MOVE1R), &pos);
                        }
                    }
                } else {
                    ep->flags &= ~EXT_POSE_FLAG_VISIBLE;
                    s->state = 0x63;
                }
                ep->texId = (gp->retraceCount + i * i) % 24;
                break;
            case 0x63:
                break;
        }
    }
    if (((skwp->marioPushed & 1) && mario->position.x > 0.0f) || ((skwp->marioPushed & 2) && mario->position.x < 0.0f)) {
        mx = -mario->position.x;
        mz = -mario->position.z;
        dist = mx * mx + mz * mz;
        if (dist > 0.0f) {
            f64 guess = __frsqrte(dist);
            guess = .5 * guess * (3.0 - guess * guess * dist);
            guess = .5 * guess * (3.0 - guess * guess * dist);
            guess = .5 * guess * (3.0 - guess * guess * dist);
            dist = dist * guess;
        } else if (dist < 0.0) {
            dist = NAN;
        } else if (isnan(dist)) {
            dist = NAN;
        }

        switch (mario->currMotionId) {
            case 0x12:
            case 0x13:
                speed = 0.0f;
                break;
            case 1:
                speed = randf(3.0f, 3.5f);
                break;
            case 2:
                speed = randf(3.8f, 4.3f);
                break;
            case 3:
                speed = randf(3.3f, 3.8f);
                break;
            default:
                speed = randf(1.7f, 2.2f);
                break;
        }
        mario->position.x += speed * (mx / dist);
        mario->position.z += speed * (mz / dist);
    }
}

static void senkaron_ext_dispent(void) {
    dispEntry(CAMERA_3D, 0.0f, 1, senkaron_ext_disp, NULL);
    dispEntry(CAMERA_3D, 0.0f, 2, senkaron_ext_shadow_disp, NULL);
}

static void senkaron_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

static void senkaron_ext_shadow_disp(CameraId camera, void* param) {
    static Mtx mt = {
        {10.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 10.0f, 0.0f, 0.1f},
        {0.0f, 0.0f, 10.0f, 0.0f},
    };

    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    extDrawShadow();
}

static EVT_BEGIN(senkaron_event)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 382)
        CALL(evt_mario_key_onoff, 0)
        BEGIN_INLINE_EVENT
            LOOP(10)
                CALL(evt_npc_get_position, STRING("黒カロン"), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KARON_APPEAR1"), LW(0), LW(1), LW(2), 0)
                WAIT_MS(200)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("黒カロン"), STRING("KRN_D_3"))
        WAIT_MS(300)
        CALL(evt_cam3d_evt_set, 550, 80, 310, 550, 30, -90, 300, 0)
        WAIT_MS(300)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg8_las_02"), 0, STRING("黒カロン"))
        CALL(evt_ext_entry, 1000, PTR(senkaron_ext_entry_data), PTR(senkaron_ext_init), PTR(senkaron_ext_main), PTR(senkaron_ext_dispent))
        CALL(senkaronFlagOffMesPushing)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_adjust_dir)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1_3"), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, STRING("fukidashi"), 0, 0, 0, 0, 0, 0, 0, 0, 0)
        CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_DANGER4"))
        WAIT_MS(1000)
        BEGIN_INLINE_EVENT
            CALL(evt_mario_set_pose, STRING("M_N_5B"))
            WAIT_MS(500)
            CALL(evt_mario_set_pose, STRING("M_I_Y"))
        END_INLINE_EVENT
        CALL(evt_npc_set_anim, STRING("黒カロン"), STRING("KRN_D_4"))
        SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(11), FLOAT(10.0f))
        SET_FLOAT_VALUE(LW(12), FLOAT(-1.0f))
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_npc_get_position, STRING("黒カロン"), LW(13), LW(10), LW(14))
            ADD_FLOAT(LW(10), LW(11))
            ADD_FLOAT(LW(11), LW(12))
            IF_INT_LESS_OR_EQUAL(LW(10), 0)
                SET_FLOAT_VALUE(LW(10), FLOAT(0.0f))
                CALL(evt_npc_set_position, STRING("黒カロン"), LW(13), LW(10), LW(14))
                BREAK_LOOP
            END_IF
            CALL(evt_npc_set_position, STRING("黒カロン"), LW(13), LW(10), LW(14))
        END_LOOP
        CALL(evt_npc_set_anim, STRING("黒カロン"), STRING("KRN_R_1"))
        CALL(evt_cam3d_evt_off, 300, 0)
        CALL(senkaronAttackStart)
        CALL(senkaronFlagOnMesPushing)
        BEGIN_INLINE_EVENT
            WAIT_MS(600)
            CALL(evt_snd_sfxon, STRING("SFX_STG8_KARON_DOWN1"), 0)
            CALL(evt_snd_sfxon, STRING("SFX_STG8_KARON_MOVE1"), GW(13))
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(400)
            CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.0048828125f), 3500)
        END_INLINE_EVENT
        SET_VALUE(GF(0), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_npc_flag_onoff, 1, STRING("黒カロン"), NPC_FLAG_NO_HIT)
            CALL(kurokaron_get_move_pos, LW(0), LW(1))
            CALL(evt_npc_move_position, STRING("黒カロン"), LW(0), LW(1), 0, FLOAT(80.0f), 0)
            WAIT_FRAMES(1)
            CALL(evt_npc_flag_onoff, 0, STRING("黒カロン"), NPC_FLAG_NO_HIT)
            SET_VALUE(GF(0), 1)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(evt_mario_set_dir, 270, 300, 0)
        END_INLINE_EVENT
        WAIT_MS(1000)
        CALL(evt_msg_print_party, STRING("stg8_las_03"))
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_msg_print, 0, STRING("stg8_las_04"), 0, STRING("黒カロン"))
        SET_FLOAT_VALUE(LW(3), FLOAT(-1.5f))
        SET_FLOAT_VALUE(LW(4), FLOAT(0.0009765625f))
        SET_VALUE(LW(5), 270)
        CALL(evt_mario_set_pose, STRING("M_I_O"))
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(0), 370)
                BREAK_LOOP
            END_IF
            ADD_FLOAT(LW(0), LW(3))
            ADD_FLOAT(LW(3), LW(4))
            IF_INT_GREATER_OR_EQUAL(LW(3), 0)
                SET_FLOAT_VALUE(LW(3), FLOAT(-0.099609375f))
            END_IF
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            ADD(LW(5), 10)
            IF_INT_GREATER_OR_EQUAL(LW(5), 360)
                SUBTRACT(LW(5), 360)
            END_IF
            CALL(evt_mario_set_dispdir, LW(5))
        END_LOOP
        CALL(senkaronFlagOffMesPushing)
        CALL(evt_npc_flag_onoff, 1, STRING("黒カロン"), NPC_FLAG_NO_HIT)
        CALL(evt_npc_change_interrupt, STRING("黒カロン"), 1, PTR(kurokaron_move))
        CALL(evt_npc_restart_regular_event, STRING("黒カロン"))
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_POSE_LOCK)
        CALL(evt_mario_key_onoff, 1)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(GF(0), 0)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(7), 0)
            LOOP(0)
                IF_INT_NOT_EQUAL(GF(0), 0)
                    BREAK_LOOP
                END_IF
                CALL(senkaronGetMarioPushedFlag, LW(6))
                IF_INT_NOT_EQUAL(LW(6), 0)
                    SET_VALUE(LW(7), 18)
                END_IF
                CALL(evt_mario_party_use_check, LW(6))
                IF_INT_NOT_EQUAL(LW(6), 0)
                    SET_VALUE(LW(7), 0)
                END_IF
                IF_INT_GREATER(LW(7), 0)
                    CALL(evt_mario_get_motion, LW(8))
                    IF_INT_EQUAL(LW(8), 0)
                        CALL(evt_mario_get_pose, LW(9))
                        IF_STR_NOT_EQUAL(LW(9), STRING("M_I_O"))
                            CALL(evt_mario_set_pose, STRING("M_I_O"))
                        END_IF
                        CALL(evt_mario_get_dispdir, LW(5))
                        ADD(LW(5), 10)
                        IF_INT_GREATER_OR_EQUAL(LW(5), 360)
                            SUBTRACT(LW(5), 360)
                        END_IF
                        CALL(evt_mario_set_dispdir, LW(5))
                        CALL(evt_party_set_dispdir, 0, LW(5))
                        SUBTRACT(LW(7), 1)
                        IF_INT_EQUAL(LW(7), 0)
                            CALL(evt_mario_set_pose, STRING("M_S_1"))
                        END_IF
                        ELSE
                        SET_VALUE(LW(7), 0)
                    END_IF
                    ELSE
                    CALL(evt_mario_get_motion, LW(8))
                    IF_INT_EQUAL(LW(8), 0)
                        CALL(evt_mario_get_dispdir, LW(5))
                        SET_VALUE(LF(1), 0)
                        IF_INT_GREATER_OR_EQUAL(LW(5), 90)
                            IF_INT_LESS(LW(5), 270)
                                SET_VALUE(LF(1), 1)
                            END_IF
                        END_IF
                        IF_INT_EQUAL(LF(1), 0)
                            CALL(evt_mario_set_dispdir, 0)
                            ELSE
                            CALL(evt_mario_set_dispdir, 180)
                        END_IF
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            SET_VALUE(LF(0), 0)
            SET_VALUE(LW(15), 0)
            LOOP(0)
                IF_INT_NOT_EQUAL(GF(0), 0)
                    CALL(evt_cam_letter_box_disable, 0)
                    BREAK_LOOP
                END_IF
                SET_VALUE(LW(5), 0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_sub_area_check, 0, 0, 0, 5, LW(0), LW(2), LW(3))
                ADD(LW(5), LW(3))
                CALL(evt_key_get_dir, 0, LW(0))
                CALL(evt_key_get_button, 0, LW(1))
                ADD(LW(0), LW(1))
                IF_INT_EQUAL(LW(0), 0)
                    ADD(LW(5), 1)
                END_IF
                SET_VALUE(LW(2), 0)
                CALL(evt_mario_party_use_check, LW(2))
                SWITCH(LW(2))
                    CASE_EQUAL(0)
                    ADD(LW(5), 1)
                    CASE_EQUAL(4)
                    ADD(LW(5), 1)
                END_SWITCH
                CALL(evt_mario_get_motion, LW(4))
                SWITCH(LW(4))
                    CASE_EQUAL(0)
                    ADD(LW(5), 1)
                    CASE_EQUAL(22)
                    ADD(LW(5), 1)
                    CASE_EQUAL(26)
                    ADD(LW(5), 1)
                END_SWITCH
                IF_INT_EQUAL(LW(5), 4)
                    IF_INT_EQUAL(LF(0), 0)
                        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(14))
                        SET_VALUE(LF(0), 1)
                        ELSE
                        CALL(evt_sub_get_stopwatch, LW(14), LW(0))
                        SWITCH(LW(15))
                            CASE_EQUAL(0)
                            IF_INT_GREATER_OR_EQUAL(LW(0), 1000)
                                CALL(evt_cam_letter_box_onoff, 1, 0)
                                CALL(evt_cam_letter_box_disable, 1)
                                CALL(evt_mario_cont_onoff, 0)
                                CALL(evt_party_cont_onoff, 0, 0)
                                CALL(evt_mario_cam_off)
                                ADD(LW(15), 1)
                            END_IF
                            CASE_EQUAL(1)
                            IF_INT_GREATER_OR_EQUAL(LW(0), 2000)
                                CALL(evt_msg_print_party, STRING("stg8_las_05"))
                                SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(14))
                                ADD(LW(15), 1)
                            END_IF
                            CASE_EQUAL(2)
                            IF_INT_GREATER_OR_EQUAL(LW(0), 3000)
                                CALL(evt_msg_print_party, STRING("stg8_las_06"))
                                CALL(evt_seq_set_seq, SEQ_GAME_OVER, 0, 0)
                            END_IF
                        END_SWITCH
                        IF_INT_GREATER(LW(15), 0)
                            CALL(get_mario_y_pos, LW(0))
                            CALL(get_party_y_pos, LW(1))
                            ADD_FLOAT(LW(0), FLOAT(-0.16601563f))
                            ADD_FLOAT(LW(1), FLOAT(-0.16601563f))
                            CALL(set_mario_y_pos, LW(0))
                            CALL(set_party_y_pos, LW(1))
                        END_IF
                    END_IF
                    ELSE
                    CALL(mario_land_on)
                    CALL(party_land_on)
                    CALL(chk_mario_ctrloff, LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL(evt_mario_cont_onoff, 1)
                        CALL(evt_mario_cam_on)
                        CALL(evt_party_cont_onoff, 0, 1)
                        CALL(evt_cam_letter_box_disable, 0)
                    END_IF
                    SET_VALUE(LF(0), 0)
                    SET_VALUE(LW(15), 0)
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(party_hit, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                BREAK_LOOP
            END_IF
            SET_VALUE(LF(0), 0)
            CALL(evt_party_get_status, 6, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LW(0), 2)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LW(0), 3)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_npc_get_position, STRING("黒カロン"), LW(3), LW(4), LW(5))
                CALL(evt_sub_get_dist, LW(0), LW(2), LW(3), LW(5), LW(6))
                IF_INT_LESS_OR_EQUAL(LW(6), 30)
                    BREAK_LOOP
                END_IF
            END_IF
        END_LOOP
        SET_VALUE(GF(0), 1)
        WAIT_FRAMES(1)
        CALL(mario_land_on)
        CALL(party_land_on)
        CALL(evt_mario_adjust_dir)
        CALL(evt_mario_set_motion, 0)
        CALL(chk_mario_ctrloff, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_mario_cont_onoff, 1)
            CALL(evt_mario_cam_on)
            CALL(evt_party_cont_onoff, 0, 1)
        END_IF
        CALL(evt_npc_get_reglid, STRING("黒カロン"), LW(0))
        DELETE_EVENT(LW(0))
        CALL(evt_npc_change_interrupt, STRING("黒カロン"), 1, 0)
        CALL(evt_npc_set_stay_anim, STRING("黒カロン"))
        CALL(senkaronFlagOnMesPushing)
        CALL(evt_snd_sfxoff, GW(13))
        WAIT_FRAMES(1)
        CALL(evt_npc_set_battle_info, STRING("黒カロン"), 7)
        CALL(evt_npc_battle_start, STRING("黒カロン"))
        CALL(evt_npc_wait_battle_end)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST1"))
        CALL(senkaronToEscape)
        SET_VALUE(GSW_STORY_PROGRESS, 383)
        SET_VALUE(LW(0), STRING("w_bero"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CALL(evt_npc_get_position, STRING("黒カロン"), LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
        CALL(evt_sub_get_dir, LW(3), LW(5), LW(0), LW(2), LW(6))
        CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x2E, LW(0), LW(1), LW(2), ITEM_TYPE_SCATTER_NO_PICKUP, -1, PTR(key_evt))
        CALL(evt_item_set_move_dir_speed, STRING("key"), LW(6), FLOAT(45.0f))
        CALL(evt_item_set_jump_power, STRING("key"), FLOAT(500.0f))
        CALL(evt_item_set_gravity, STRING("key"), FLOAT(1.0f))
        CALL(evt_item_set_bound_rate, STRING("key"), FLOAT(1.0f))
        CALL(evt_item_set_bound_limit, STRING("key"), 0)
        CALL(evt_item_status_on, STRING("key"), 4)
        CALL(evt_item_status_on, STRING("key"), 16)
        CALL(evt_npc_set_position, STRING("黒カロン"), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(kurokaron_init)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 382)
        CALL(evt_npc_set_position, STRING("me"), 570, 0, 0)
        CALL(evt_npc_set_ry, STRING("me"), 270)
        CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
        CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_Z_2"))
        CALL(evt_npc_set_balloontype, STRING("me"), 0)
    END_IF
    RETURN
EVT_END()

static USER_FUNC(senkaronToEscape) {
    skwp->escape = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronFlagOnMesPushing) {
    skwp->mesPushing = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronFlagOffMesPushing) {
    skwp->mesPushing = 0;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronAttackStart) {
    skwp->attackStart = 1;
    return EVT_RETURN_DONE;
}

static USER_FUNC(senkaronGetMarioPushedFlag) {
    evtSetValue(event, event->args[0], skwp->marioPushed);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_mario_y_pos) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], marioGetPtr()->position.y);
    return EVT_RETURN_DONE;
}

USER_FUNC(set_mario_y_pos) {
    EvtWord* args = event->args;
    f32 y = evtGetFloat(event, args[0]);

    marioGetPtr()->position.y = y;
    return EVT_RETURN_DONE;
}

USER_FUNC(get_party_y_pos) {
    EvtWord* args = event->args;
    PartyEntry* party = partyGetPtr(partyCtrlNo);

    if (party != NULL) {
        evtSetFloat(event, args[0], party->position.y);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(set_party_y_pos) {
    EvtWord* args = event->args;
    f32 y = evtGetFloat(event, args[0]);
    PartyEntry* party = partyGetPtr(partyCtrlNo);

    if (party != NULL) {
        party->position.y = y;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(mario_land_on) {
    MarioWork* mario = marioGetPtr();

    if (mario->position.y < 0.0f) {
        mario->position.y = 0.0f;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(party_land_on) {
    PartyEntry* party = partyGetPtr(partyCtrlNo);

    if (party != NULL && party->position.y < 0.0f) {
        party->position.y = 0.0f;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(chk_mario_ctrloff) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], marioCtrlOffChk());
    return EVT_RETURN_DONE;
}

USER_FUNC(party_hit) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    NpcEntry* npc = npcNameToPtr("\x8D\x95\x83\x4A\x83\x8D\x83\x93");
    EvtWord* args = event->args;
    f32 dir;

    if (party_hit_check(npc->position.x, npc->position.z, &dir) &&
        (party->currentMemberId == PARTY_MEMBER_KOOPS || party->currentMemberId == PARTY_MEMBER_BOBBERY)) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(kurokaron_talk)
    SPAWN_EVENT(PTR(senkaron_event))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 382)
        CALL(evt_npc_change_interrupt, STRING("me"), 6, 0)
    END_IF
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"黒カロン"*/"\x8D\x95\x83\x4A\x83\x8D\x83\x93", // name
        NPC_FLAG_NO_TALK_TURN, // flags
        0, // reactionFlags
        kurokaron_init, // initEvent
        NULL, // regularEvent
        kurokaron_talk, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カロン"*/"\x83\x4A\x83\x8D\x83\x93", // name
        0, 0, NULL, NULL, NULL, NULL, NULL, NULL, // flags..lostEvent
        NULL, NULL, 0, // returnEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

static EVT_BEGIN(key_evt)
    SET_VALUE(GSW_STORY_PROGRESS, 384)
    RETURN
EVT_END()

static s32 key_tbl[] = {
    ITEM_PALACE_KEY_0x2E, -1,
};

static EVT_BEGIN(key_check_evt)
    CALL(evt_pouch_check_item, ITEM_PALACE_KEY_0x2E, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(evt_win_item_select, 0, PTR(key_tbl), LW(0), 0)
        IF_INT_EQUAL(LW(0), -1)
            CALL(evt_mobj_exec_cancel, STRING("me"))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(unlock_evt)
    SET_VALUE(GSW_STORY_PROGRESS, 385)
    SET_VALUE(LW(0), STRING("e_bero_1"))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

static EVT_BEGIN(shan_init)
    SET_VALUE(LW(0), 1)
    LOOP(3)
        SWITCH(LW(0))
            CASE_EQUAL(1)
            SET_VALUE(LW(2), 8)
            CASE_DEFAULT
            SET_VALUE(LW(2), 4)
        END_SWITCH
        SET_VALUE(LW(1), 1)
        LOOP(LW(2))
            CALL(make_name, LW(0), LW(1), LW(3))
            CALL(evt_mapobj_flag_onoff, 1, 1, LW(3), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_get_position, LW(3), LW(4), LW(5), LW(6))
            SUBTRACT_FLOAT(LW(5), FLOAT(7.0f))
            CALL(evt_eff, 0, STRING("fire"), 3, LW(4), LW(5), LW(6), FLOAT(0.7998047f), 1000, 0, 0, 0, 0, 0, 0)
            ADD(LW(1), 1)
        END_LOOP
        ADD(LW(0), 1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(las_05_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 382)
        CALL(evt_npc_setup, PTR(npcEnt))
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 383)
        CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x2E, 0, 0, 0, ITEM_TYPE_GROUND, -1, PTR(key_evt))
    END_IF
    IF_INT_LESS(GSW_STORY_PROGRESS, 385)
        CALL(evt_mobj_lock, STRING("lock"), 46, 590, 20, 0, -90, PTR(key_check_evt), PTR(unlock_evt), 0)
        SET_VALUE(LW(0), STRING("e_bero_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
    END_IF
    CALL_CHILD(PTR(shan_init))
    RETURN
EVT_END()

static USER_FUNC(make_name) {
    static char name[20];
    EvtWord* args = event->args;

    s32 a = evtGetValue(event, args[0]);
    s32 b = evtGetValue(event, args[1]);

    sprintf(name, "S_shan_%02d_rou_%02d", a, b);
    evtSetValue(event, args[2], (EvtWord)name);
    return EVT_RETURN_DONE;
}

