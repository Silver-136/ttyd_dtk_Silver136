#include "eff/eff_magic1_n64.h"
#include "eff/eff_fire.h"
#include "evt/evt_cmd.h"
#include "evt/evt_npc.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include <dolphin/gx.h>
#include <string.h>
#include "evt/evt_eff.h"
#include "evt/evt_mario.h"
#include "libc_decls.h"
#include "koopa.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "dolphin/mtx.h"
#include "evt/evt_item.h"
#include "driver/itemdrv.h"
static void piders_draw_yarn_sub(CameraId cameraId, void* param);

double sqrt(double);
double __fabs(double);
#define fabs(x) __fabs(x)


// functions are defined in reverse of target order (deferred inlining)

USER_FUNC(mahoon_get_groupname) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    evtSetValue(event, args[0], (EvtWord)npc->tribe->modelName);
    return EVT_RETURN_DONE;
}

USER_FUNC(npc_check_wall_stop) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    if (npc->wallHit) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_wait_blow_end) {
    const char* nm = (const char*)evtGetValue(event, event->args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, nm);
    if (npcCheckBlow(npc)) {
        npc->rotation.y += 18.0f;
        return EVT_RETURN_BLOCK;
    }
    npc->rotation.y = 0.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(pressCheck) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    f32 hx, hy, hz, dist, nx, ny, nz;
    if (kpaGetLevel() == 0) {
        evtSetValue(event, args[0], 0);
        return EVT_RETURN_DONE;
    }
    dist = 5.0f;
    if (hitCheckFilter(0, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, 1.0f + npc->position.y, npc->position.z, 0.0f, -1.0f, 0.0f)) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_2d_dead_jump) {
    EvtWord* args = event->args;
    const char* id = (const char*)evtGetValue(event, args[0]);
    f32 tx = evtGetFloat(event, args[1]);
    f32 ty = evtGetFloat(event, args[2]);
    f32 tz = evtGetFloat(event, args[3]);
    f32 t = (f32)evtGetValue(event, args[4]) / 1000.0f;
    f32 spd = evtGetFloat(event, args[5]);
    f32 vy = evtGetFloat(event, args[6]);
    f32 gy = (f32)evtGetValue(event, args[7]);
    f32 h;
    NpcEntry* npc;
    Vec d;
    npc = evtNpcNameToPtr(event, id);
    h = vy;
    npc->jumpFlags |= 0x10;
    npc->jumpFlags |= 0x1000;
    if (isFirstCall) {
        npc->flags |= NPC_FLAG_GRAVITY;
        npc->flags &= ~(NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_WALL_COLLISION);
        npc->moveTicks = 0;
        npc->jumpTicks = 0;
        npc->moveAnimState = 0;
        npc->jumpTargetPos.x = tx;
        npc->jumpTargetPos.y = ty;
        npc->jumpTargetPos.z = tz;
        npc->jumpStartPos = npc->position;
        PSVECSubtract(&npc->jumpTargetPos, &npc->jumpStartPos, &d);
        // FAKE: an if/else that only self-assigns d.y; the then-branch statement and the else block are both needed
        // (an empty else gives the same code, the plain if or no statement do not)
        if (d.x * d.x + d.z * d.z > 0.0f) {
            d.y = d.y;
        } else {
            d.y = d.y;
        }
        npc->moveDir = angleABf(0.0f, 0.0f, d.x, d.z);
        if (t > 0.0f) {
            npc->moveSpeed = fabs(tx - npc->position.x) / t;
        } else {
            npc->moveSpeed = spd;
        }
        npc->moveTime = 0.0f;
        npc->moveOffset.y = 0.0f;
        npc->jumpVelY = 0.0f;
        npc->jumpYScale = 1.0f;
        {
            f32 dy = npc->jumpTargetPos.y - npc->jumpStartPos.y;
            if (vy) {
                f32 g, half, apex;
                if (dy > 0.0f) {
                    h += dy;
                }
                npc->jumpVelY = 0.0f;
                if (t == 0.0f) {
                    npc->jumpVelY = vy;
                } else {
                    g = -980.0f * npc->jumpGravity;
                    half = 0.5f * t;
                    npc->jumpVelY = -(t * (g * t)) / t;
                    apex = npc->jumpVelY * half + half * (-980.0f * npc->jumpGravity * half);
                    if (apex != 0.0f) {
                        npc->jumpYScale = h / apex;
                    }
                }
            } else {
                if (dy >= 0.0f) {
                    return 1;
                }
            }
        }
        return 0;
    }
    {
        f32 s = (f32)sin(6.2831855f * npc->moveDir / 360.0f);
        npc->moveOffset.x += npc->moveDt * (npc->moveSpeed * s);
    }
    {
        f32 c = (f32)cos(6.2831855f * npc->moveDir / 360.0f);
        npc->moveOffset.z += npc->moveDt * (npc->moveSpeed * -c);
    }
    if (npc->position.y >= gy) {
        return 0;
    }
    npc->moveSpeed = 0.0f;
    npc->jumpVelY = 0.0f;
    npc->jumpYScale = 1.0f;
    npc->jumpGravity = 1.0f;
    return 1;
}

USER_FUNC(_2d_get_dead_jump_offset) {
    EvtWord* args = event->args;
    f32 d;
    f32 k;
    const char* nm = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, nm);
    s32 out = args[1];
    MarioWork* mario = marioGetPtr();
    if (mario->position.x == npc->position.x) {
        d = 0.0f;
    } else if (mario->position.x > npc->position.x) {
        d = -1.0f;
    } else {
        d = 1.0f;
    }
    k = 20.0f;
    k *= d;
    evtSetValue(event, out, (s32)k);
    return EVT_RETURN_DONE;
}

USER_FUNC(killer_make_name) {
    static char name[32];
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    s32 n = evtGetValue(event, args[0]);
    s32 len;
    strcpy(name, npc->description);
    len = strlen(name);
    name[len] = n + 0x31;
    name[len + 1] = 0;
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}

USER_FUNC(piders_get_height) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    evtSetFloat(event, args[0], npc->territoryLoiter.z);
    return EVT_RETURN_DONE;
}




















USER_FUNC(evt_npc_set_confuse_anim);
USER_FUNC(evt_npc_set_damage_anim);
USER_FUNC(evt_npc_set_run_anim);
USER_FUNC(evt_npc_set_stay_anim);
USER_FUNC(evt_npc_set_talk_anim);
USER_FUNC(evt_npc_set_walk_anim);
USER_FUNC(piders_draw_yawn);
USER_FUNC(urouro_init_func);
USER_FUNC(urouro_main_func);
USER_FUNC(gesso_ground_check);
USER_FUNC(set_bottle_flag_init);
USER_FUNC(npc_fire_eff_fadeout);
USER_FUNC(zakoEntryFall);
USER_FUNC(dokanCheck);
USER_FUNC(zakoEntryDokan);
USER_FUNC(limitSpd);
USER_FUNC(getSpdRun);
USER_FUNC(getSpdWalk);
USER_FUNC(upperCheck);
USER_FUNC(cliffCheck);
USER_FUNC(camGetPos);
USER_FUNC(chain_main);
USER_FUNC(kamec_fire_magic_delete);
USER_FUNC(kamec_fire_magic);
#ifdef VERSION_G8ME01
#endif

EvtWord enemy_common_dead_event[156];
EvtWord enemy_event_dead_event_sub[36];
EvtWord enemy_common_blow_event[13];
EvtWord enemy_2d_common_dead_event[415];
EvtWord _2d_dead_rotate_event[16];
EvtWord testenemynpc_move_event[13];
EvtWord npc_urouro_move_event[21];
EvtWord npc_urouro_return_event[37];
EvtWord kuriboo_move_event[39];
EvtWord kuriboo_find_event[54];
EvtWord kuriboo_lost_event[31];
EvtWord kuriboo_return_event[37];
EvtWord patakuri_move_event[148];
EvtWord patakuri_find_event[90];
EvtWord patakuri_lost_event[31];
EvtWord patakuri_return_event[170];
EvtWord nokonoko_shell_return_event[50];
EvtWord nokonoko_init_event[28];
EvtWord nokonoko_move_event[58];
EvtWord nokonoko_find_event[172];
EvtWord nokonoko_lost_event[46];
EvtWord nokonoko_return_event[55];
EvtWord togenoko_shell_return_event[50];
EvtWord togenoko_init_event[28];
EvtWord togenoko_move_event[58];
EvtWord togenoko_find_event[172];
EvtWord togenoko_lost_event[46];
EvtWord togenoko_return_event[55];
EvtWord patapata_move_event[123];
EvtWord patapata_find_event[131];
EvtWord patapata_lost_event[44];
EvtWord patapata_return_event[135];
EvtWord met_init_event_common[21];
EvtWord met_set_anim_wait_event[48];
EvtWord met_set_anim_walk_event[48];
EvtWord met_init_event[19];
EvtWord met_init_event_ceil[14];
EvtWord togemet_init_event[19];
EvtWord togemet_init_event_ceil[14];
EvtWord met_move_event[54];
EvtWord met_find_event[269];
EvtWord met_lost_event[43];
EvtWord met_return_event[51];
EvtWord patamet_move_event[151];
EvtWord patamet_find_event[108];
EvtWord patamet_lost_event[34];
EvtWord patamet_return_event[143];
#ifdef VERSION_G8ME01
EvtWord chorobon_move_event[71];
#else
EvtWord chorobon_move_event[69];
#endif
EvtWord chorobon_find_event[63];
EvtWord chorobon_lost_event[14];
EvtWord chorobon_return_event[43];
EvtWord pansy_move_event[42];
EvtWord pansy_find_event[46];
EvtWord pansy_lost_event[35];
EvtWord pansy_return_event[45];
EvtWord twinkling_pansy_move_event[42];
EvtWord twinkling_pansy_find_event[54];
EvtWord twinkling_pansy_lost_event[35];
EvtWord twinkling_pansy_return_event[45];
EvtWord karon_init_event[36];
EvtWord karon_move_event[57];
EvtWord karon_find_event[150];
EvtWord karon_lost_event[18];
EvtWord karon_return_event[37];
EvtWord karon_blow_event[19];
EvtWord honenoko2_init_event[18];
EvtWord honenoko2_move_event[49];
EvtWord honenoko2_find_event[58];
EvtWord honenoko2_lost_event[36];
EvtWord honenoko2_return_event[46];
EvtWord killer_init_event[19];
EvtWord killer_move_event[79];
EvtWord killer_dead_event[156];
EvtWord killer_cannon_init_event[13];
EvtWord killer_cannon_move_event[157];
EvtWord sambo_move_event[22];
EvtWord sambo_find_event[33];
EvtWord sambo_lost_event[22];
EvtWord sambo_return_event[30];
EvtWord sinnosuke_init_event[26];
EvtWord sinnosuke_move_event[21];
EvtWord sinnosuke_find_event[146];
EvtWord sinnosuke_lost_event[36];
EvtWord sinnosuke_return_event[94];
EvtWord sinemon_init_event[26];
EvtWord sinemon_move_event[26];
EvtWord sinemon_find_event[146];
EvtWord sinemon_lost_event[36];
EvtWord sinemon_return_event[94];
EvtWord togedaruma_move_event[49];
EvtWord togedaruma_find_event[61];
EvtWord togedaruma_lost_event[38];
EvtWord togedaruma_return_event[44];
EvtWord barriern_init_event[37];
#ifdef VERSION_G8ME01
EvtWord barriern_move_event[90];
#else
EvtWord barriern_move_event[89];
#endif
#ifdef VERSION_G8ME01
EvtWord barriern2_move_event[90];
#else
EvtWord barriern2_move_event[70];
#endif
EvtWord barriern_dead_event[17];
EvtWord barriern_find_event[189];
EvtWord barriern_lost_event[17];
#ifdef VERSION_G8ME01
EvtWord barriern_return_event[43];
#else
EvtWord barriern_return_event[42];
#endif
EvtWord barriern_blow_event[27];
EvtWord piders_move_event[111];
EvtWord piders_find_event[206];
EvtWord piders_blow_event[13];
EvtWord pakkun_attack_event[74];
EvtWord pakkun_init_event[34];
EvtWord pakkun_move_event[17];
EvtWord pakkun_find_event[106];
EvtWord pakkun_return_event[81];
EvtWord dokugassun_init_event[14];
EvtWord dokugassun_move_event[153];
EvtWord dokugassun_find_event[69];
EvtWord dokugassun_lost_event[27];
EvtWord dokugassun_return_event[158];
EvtWord basabasa_init_event[17];
#ifdef VERSION_G8ME01
EvtWord basabasa_move_event[72];
#else
EvtWord basabasa_move_event[67];
#endif
EvtWord basabasa_find_event[69];
EvtWord basabasa_lost_event[31];
EvtWord basabasa_return_event[44];
EvtWord basabasa2_move_event[148];
EvtWord basabasa2_find_event[60];
EvtWord basabasa2_lost_event[31];
EvtWord basabasa2_return_event[140];
EvtWord teresa_move_event[165];
EvtWord teresa_find_event[60];
EvtWord teresa_lost_event[31];
EvtWord teresa_return_event[104];
EvtWord bubble_move_event[159];
EvtWord bubble_find_event[63];
EvtWord bubble_lost_event[34];
EvtWord bubble_return_event[151];
EvtWord hbom_move_event[41];
EvtWord hbom_find_event[54];
EvtWord hbom_lost_event[31];
EvtWord hbom_return_event[37];
EvtWord zakowiz_init_event[38];
EvtWord zakowiz_move_event[70];
EvtWord zakowiz_find_event[159];
EvtWord zakowiz_lost_event[18];
EvtWord zakowiz_return_event[37];
EvtWord zakowiz_blow_event[19];
EvtWord hannya_move_event[37];
EvtWord hannya_find_event[86];
EvtWord hannya_lost_event[23];
EvtWord hannya_return_event[29];
EvtWord mahoon_init_event[120];
EvtWord mahoon_move_event[259];
EvtWord mahoon_dead_event[24];
EvtWord mahoon_find_event[177];
EvtWord mahoon_lost_event[142];
EvtWord mahoon_return_event[251];
EvtWord kamec_init_event[54];
EvtWord kamec_move_event[177];
EvtWord kamec_find_event[150];
EvtWord kamec_lost_event[31];
EvtWord kamec_return_event[150];
EvtWord kamec_blow_event[19];
EvtWord kamec2_init_event[54];
EvtWord kamec2_move_event[58];
EvtWord kamec2_find_event[153];
EvtWord kamec2_lost_event[31];
EvtWord kamec2_return_event[37];
EvtWord kamec2_blow_event[19];
EvtWord hbross_init_event[36];
EvtWord hbross_move_event[57];
EvtWord hbross_find_event[141];
EvtWord hbross_lost_event[18];
EvtWord hbross_return_event[37];
EvtWord hbross_blow_event[19];
EvtWord enemy_event_dead_event[10];
EvtWord enemy_slave_common_dead_event[8];
EvtWord enemy_common_flag_init_event[12];
EvtWord testenemynpc_init_event[4];
EvtWord npc_urouro_init_event[12];
EvtWord kuriboo_init_event[4];
EvtWord patakuri_init_event[12];
EvtWord patapata_init_event[12];
EvtWord patamet_init_event[12];
EvtWord chorobon_init_event[12];
EvtWord pansy_init_event[4];
EvtWord twinkling_pansy_init_event[4];
EvtWord karon_dead_event[9];
EvtWord sambo_init_event[12];
EvtWord togedaruma_init_event[4];
EvtWord piders_init_event[9];
EvtWord basabasa_dead_event[4];
EvtWord basabasa2_init_event[12];
EvtWord basabasa2_dead_event[4];
EvtWord teresa_init_event[12];
EvtWord teresa_dead_event[4];
EvtWord bubble_init_event[12];
EvtWord hbom_init_event[4];
EvtWord zakowiz_dead_event[9];
EvtWord hannya_init_event[9];
EvtWord kamec_dead_event[9];
EvtWord kamec2_dead_event[9];
EvtWord hbross_dead_event[9];
EvtWord zakoM2D_dead_event[9];
EvtWord wanwan_init_event[64];
EvtWord wanwan_init_event_sub[64];
EvtWord wanwan_move_event[151];
EvtWord wanwan_find_event[90];
EvtWord kuriboo2D_init_event[22];
EvtWord kuriboo2D_move_event[72];
EvtWord kuriboo2D_find_event[70];
EvtWord zako2D_init_event[27];
EvtWord zako2D_move_event[244];
EvtWord zako2D_find_event[74];
EvtWord dokan2D_init_event[19];
EvtWord dokan2D_regl_event[48];
EvtWord fall2D_init_event[19];
EvtWord fall2D_regl_event[34];
EvtWord zakoM2D_init_event[37];
EvtWord zakoM2D_move_event[95];
EvtWord zakoM2D_find_event[179];
EvtWord zakoM2D_find_start_event[88];
EvtWord gesso2D_init_event[34];
EvtWord gesso2D_move_event[262];

EVT_BEGIN(enemy_common_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_damage_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_ENEMY_DIE1"), 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_drop_coin, STRING("me"), LW(3))
    IF_INT_GREATER(LW(3), 0)
        LOOP(LW(3))
            CALL(evt_item_entry, 0, ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_DROP_REPLACEABLE, -1, 0)
        END_LOOP
    END_IF
    CALL(evt_npc_get_drop_heart, STRING("me"), LW(3))
    IF_INT_GREATER(LW(3), 0)
        LOOP(LW(3))
            CALL(evt_item_entry, 0, ITEM_HEART, LW(0), LW(1), LW(2), ITEM_TYPE_DROP_REPLACEABLE, -1, 0)
        END_LOOP
    END_IF
    CALL(evt_npc_get_drop_flower, STRING("me"), LW(3))
    IF_INT_GREATER(LW(3), 0)
        LOOP(LW(3))
            CALL(evt_item_entry, 0, ITEM_FLOWER, LW(0), LW(1), LW(2), ITEM_TYPE_DROP_REPLACEABLE, -1, 0)
        END_LOOP
    END_IF
    CALL(evt_npc_get_drop_item, STRING("me"), LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(evt_item_entry, 0, LW(3), LW(0), LW(1), LW(2), ITEM_TYPE_DROP_LOST_BADGE, -1, 0)
    END_IF
    CALL(evt_npc_getback_item_entry, STRING("me"))
    CALL(evt_npc_get_drop_fixitem, STRING("me"), LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(evt_item_entry, 0, LW(3), LW(0), LW(1), LW(2), ITEM_TYPE_DROP_LOST_BADGE, -1, 0)
    END_IF
    CALL(evt_npc_delete, STRING("me"))
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(enemy_event_dead_event_sub)
    CALL(evt_npc_set_damage_anim, LW(10))
    WAIT_MS(500)
    CALL(evt_npc_get_position, LW(10), LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_ENEMY_DIE1"), 0)
    CALL(evt_npc_delete, LW(10))
    RETURN
EVT_END()

EVT_BEGIN(enemy_common_blow_event)
    CALL(evt_npc_set_confuse_anim, STRING("me"))
    CALL(_wait_blow_end, STRING("me"))
    WAIT_MS(2000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(enemy_2d_common_dead_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 268435456)
    CALL(pressCheck, LW(15))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("me"), LW(0), LW(1), 200)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_ENEMY_DIE1"), 0)
    CALL(evt_npc_set_damage_anim, STRING("me"))
    CALL(evt_npc_get_kpencount_type, STRING("me"), LW(0))
    SWITCH(LW(0))
        CASE_FLAG(1)
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_calc_score, STRING("me"))
            SPAWN_EVENT(PTR(_2d_dead_rotate_event))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(_2d_get_dead_jump_offset, STRING("me"), LW(3))
            ADD(LW(0), LW(3))
            CALL(_2d_dead_jump, STRING("me"), LW(0), LW(1), LW(2), 600, FLOAT(40.0f), FLOAT(0.0f), -300)
            ELSE
            CALL(evt_npc_calc_score, STRING("me"))
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
            CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
            CALL(evt_npc_status_onoff, 0, STRING("me"), 2)
            CALL(evt_npc_set_talk_anim, STRING("me"))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 5)
            CALL(evt_npc_set_position, STRING("me"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(30)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            BEGIN_SIBLING_EVENT
                LOOP(30)
                    CALL(evt_npc_add_rotate, STRING("me"), 0, 0, 1)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            SUBTRACT(LW(0), 40)
            SUBTRACT(LW(1), 20)
            CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, -10, 4, 0)
            LOOP(0)
                BEGIN_SIBLING_EVENT
                    LOOP(60)
                        CALL(evt_npc_add_rotate, STRING("me"), 0, 0, -1)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_SIBLING_EVENT
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                ADD(LW(0), 80)
                SUBTRACT(LW(1), 20)
                CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 1000, 0, -10, 4, 0)
                BEGIN_SIBLING_EVENT
                    LOOP(60)
                        CALL(evt_npc_add_rotate, STRING("me"), 0, 0, 1)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_SIBLING_EVENT
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 80)
                SUBTRACT(LW(1), 20)
                CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 1000, 0, -10, 4, 0)
                IF_INT_LESS_OR_EQUAL(LW(1), -200)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_IF
        CASE_FLAG(2)
        CALL(evt_npc_calc_score, STRING("me"))
        SPAWN_EVENT(PTR(_2d_dead_rotate_event))
        CALL(evt_npc_status_onoff, 1, STRING("me"), 4194304)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(_2d_get_dead_jump_offset, STRING("me"), LW(3))
        ADD(LW(0), LW(3))
        CALL(_2d_dead_jump, STRING("me"), LW(0), LW(1), LW(2), 600, FLOAT(40.0f), FLOAT(65.0f), -300)
        CASE_FLAG(4)
        CALL(evt_npc_calc_score, STRING("me"))
        SPAWN_EVENT(PTR(_2d_dead_rotate_event))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(_2d_get_dead_jump_offset, STRING("me"), LW(3))
        ADD(LW(0), LW(3))
        CALL(_2d_dead_jump, STRING("me"), LW(0), LW(1), LW(2), 600, FLOAT(40.0f), FLOAT(65.0f), -300)
        CASE_FLAG(32)
        SPAWN_EVENT(PTR(_2d_dead_rotate_event))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(_2d_get_dead_jump_offset, STRING("me"), LW(3))
        ADD(LW(0), LW(3))
        CALL(_2d_dead_jump, STRING("me"), LW(0), LW(1), LW(2), 600, FLOAT(40.0f), FLOAT(65.0f), -300)
        CASE_FLAG(8)
        CALL(evt_npc_calc_score, STRING("me"))
        SPAWN_EVENT(PTR(_2d_dead_rotate_event))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(_2d_get_dead_jump_offset, STRING("me"), LW(3))
        ADD(LW(0), LW(3))
        CALL(_2d_dead_jump, STRING("me"), LW(0), LW(1), LW(2), 600, FLOAT(40.0f), FLOAT(65.0f), -300)
        CASE_FLAG(16)
        CALL(evt_npc_calc_score, STRING("me"))
        SPAWN_EVENT(PTR(_2d_dead_rotate_event))
        CALL(evt_npc_status_onoff, 1, STRING("me"), 4194304)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(_2d_get_dead_jump_offset, STRING("me"), LW(3))
        ADD(LW(0), LW(3))
        CALL(_2d_dead_jump, STRING("me"), LW(0), LW(1), LW(2), 600, FLOAT(40.0f), FLOAT(65.0f), -300)
        CASE_DEFAULT
        WAIT_MS(500)
    END_SWITCH
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(_2d_dead_rotate_event)
    SET_VALUE(LW(0), -12)
    LOOP(15)
        CALL(evt_npc_add_rotate, STRING("me"), 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(testenemynpc_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_set_stay_anim, STRING("me"))
    LABEL(0)
    WAIT_MS(500)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(npc_urouro_move_event)
    CALL_CHILD(PTR(npc_urouro_init_event))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(npc_urouro_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kuriboo_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(kuriboo_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
    CALL(evt_npc_homing_target, STRING("me"), STRING("mario"), 0, FLOAT(120.0f), FLOAT(10.0f), FLOAT(60.0f), 4)
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kuriboo_lost_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kuriboo_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patakuri_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(patakuri_find_event)
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 0, 75, FLOAT(-50.0f), 0, 3)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 1000, 0, 0, 0, 1)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patakuri_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patakuri_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 1000, 0, 0, 0, 1)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_shell_return_event)
    CALL(evt_npc_get_unitwork, STRING("me"), 1, LW(15))
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
        CALL(evt_npc_set_unitwork, STRING("me"), 1, 0)
        CALL(evt_npc_set_attack_mode, STRING("me"), 0)
        CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
        CALL(evt_npc_set_stay_anim, STRING("me"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_get_unitwork, STRING("me"), 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_move_event)
    CALL_CHILD(PTR(nokonoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_find_event)
    CALL_CHILD(PTR(nokonoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
    CALL(evt_npc_sound_data_set, STRING("me"), STRING("SFX_FLD_ENM_NOKO_MOVE1"), 0, 0, 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 1, 1)
    CALL(evt_npc_set_attack_mode, STRING("me"), 11)
    CALL(evt_npc_set_anim, STRING("me"), STRING("NKT_A_1"))
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
    CALL(evt_npc_set_ry, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_NOKO_SHELL1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(256.0f))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
            IF_INT_EQUAL(LW(15), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_add_rotate, STRING("me"), 0, 40, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(100)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            ADD(LW(1), 10)
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.2998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 800, FLOAT(400.0f), 4)
    DELETE_EVENT(LW(14))
    CALL_CHILD(PTR(nokonoko_shell_return_event))
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL_CHILD(PTR(nokonoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(nokonoko_return_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL_CHILD(PTR(nokonoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togenoko_shell_return_event)
    CALL(evt_npc_get_unitwork, STRING("me"), 1, LW(15))
    IF_INT_EQUAL(LW(15), 1)
        CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
        CALL(evt_npc_set_unitwork, STRING("me"), 1, 0)
        CALL(evt_npc_set_attack_mode, STRING("me"), 0)
        CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
        CALL(evt_npc_set_stay_anim, STRING("me"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(togenoko_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_get_unitwork, STRING("me"), 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 1, 0)
    RETURN
EVT_END()

EVT_BEGIN(togenoko_move_event)
    CALL_CHILD(PTR(togenoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(togenoko_find_event)
    CALL_CHILD(PTR(togenoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
    CALL(evt_npc_sound_data_set, STRING("me"), STRING("SFX_FLD_ENM_NOKO_MOVE1"), 0, 0, 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 1, 1)
    CALL(evt_npc_set_attack_mode, STRING("me"), 11)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1"))
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
    CALL(evt_npc_set_ry, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_NOKO_SHELL1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(256.0f))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
            IF_INT_EQUAL(LW(15), 1)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_add_rotate, STRING("me"), 0, 40, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_MS(100)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            ADD(LW(1), 10)
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.2998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 800, FLOAT(400.0f), 4)
    DELETE_EVENT(LW(14))
    CALL_CHILD(PTR(togenoko_shell_return_event))
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togenoko_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL_CHILD(PTR(togenoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togenoko_return_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL_CHILD(PTR(togenoko_shell_return_event))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patapata_move_event)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    SET_VALUE(LW(10), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(10), 0)
            CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(-20.0f), FLOAT(20.0f))
        END_IF
        IF_INT_EQUAL(LW(10), 2)
            CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(-20.0f), FLOAT(20.0f))
        END_IF
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(30.0f))
        CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
        CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-7.0f), 0, 2)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(30.0f))
        CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
        CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(7.0f), 0, 2)
        IF_INT_GREATER_OR_EQUAL(LW(10), 4)
            BREAK_LOOP
        END_IF
        ADD(LW(10), 1)
    END_LOOP
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(patapata_find_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_pera_onoff, STRING("me"), 1)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    LOOP(18)
        CALL(evt_npc_add_rotate, STRING("me"), 0, 15, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_anim, STRING("me"), STRING("PTP_A_5"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PATA_MOVE2"), LW(0), LW(1), LW(2), 0)
    LOOP(6)
        CALL(evt_npc_add_rotate, STRING("me"), 0, 15, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 10)
    CALL(evt_npc_sound_data_set, STRING("me"), STRING("SFX_FLD_ENM_PATA_MOVE3"), 0, 0, 0, 0)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(200.0f))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 1000, 0, FLOAT(-40.0f), 4, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patapata_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patapata_return_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(30.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(7.0f), 0, 2)
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(30.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-7.0f), 0, 2)
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(30.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(7.0f), 0, 2)
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(30.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-7.0f), 0, 2)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EvtWord met_init_event_common[] = {
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
};

EVT_BEGIN(met_set_anim_wait_event)
    CALL(evt_npc_get_btlsetup_work, STRING("me"), 0, LW(15))
    IF_INT_EQUAL(LW(15), 0)
        CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("MET_S_1"))
            ELSE
            CALL(evt_npc_set_anim, STRING("me"), STRING("TMT_S_1"))
        END_IF
        ELSE
        CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("SMT_S_1"))
            ELSE
            CALL(evt_npc_set_anim, STRING("me"), STRING("STM_S_1"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(met_set_anim_walk_event)
    CALL(evt_npc_get_btlsetup_work, STRING("me"), 0, LW(15))
    IF_INT_EQUAL(LW(15), 0)
        CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("MET_W_1"))
            ELSE
            CALL(evt_npc_set_anim, STRING("me"), STRING("TMT_W_1"))
        END_IF
        ELSE
        CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("SMT_W_1"))
            ELSE
            CALL(evt_npc_set_anim, STRING("me"), STRING("STM_W_1"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(met_init_event)
    CALL_CHILD(PTR(met_init_event_common))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_set_btlsetup_work, STRING("me"), 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(met_init_event_ceil)
    CALL_CHILD(PTR(met_init_event_common))
    CALL(evt_npc_set_btlsetup_work, STRING("me"), 0, 1)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(togemet_init_event)
    CALL_CHILD(PTR(met_init_event_common))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_set_btlsetup_work, STRING("me"), 0, 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
    RETURN
EVT_END()

EVT_BEGIN(togemet_init_event_ceil)
    CALL_CHILD(PTR(met_init_event_common))
    CALL(evt_npc_set_btlsetup_work, STRING("me"), 0, 1)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
    RETURN
EVT_END()

EVT_BEGIN(met_move_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL_CHILD(PTR(met_set_anim_wait_event))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    LABEL(0)
    WAIT_MS(500)
    CALL_CHILD(PTR(met_set_anim_walk_event))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(met_find_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_get_btlsetup_work, STRING("me"), 0, LW(15))
    IF_INT_EQUAL(LW(15), 0)
        CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
        CALL(evt_npc_set_attack_mode, STRING("me"), 11)
        CALL(evt_npc_sound_data_set, STRING("me"), STRING("SFX_FLD_ENM_MET_ATTACK1"), 0, 0, 0, 0)
        CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("MET_A_1"))
            ELSE
            CALL(evt_npc_set_anim, STRING("me"), STRING("TMT_A_1"))
        END_IF
        CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
        CALL(evt_npc_set_ry, STRING("me"), LW(3))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(256.0f))
        BEGIN_SIBLING_EVENT
            SET_VALUE(LW(4), 0)
            SET_VALUE(LW(15), 0)
            LOOP(0)
                WAIT_FRAMES(1)
                ADD(LW(15), 1)
                IF_INT_GREATER_OR_EQUAL(LW(15), 55)
                    BREAK_LOOP
                END_IF
                ADD(LW(4), 40)
                CALL(evt_npc_set_rotate, STRING("me"), 0, LW(4), 0)
            END_LOOP
        END_SIBLING_EVENT
        WAIT_MS(100)
        BEGIN_SIBLING_EVENT_GET_ID(LW(14))
            LOOP(0)
                CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
                ADD(LW(1), 10)
                CALL(evt_eff, STRING(""), STRING("kemuri_test"), 0, LW(0), LW(1), LW(2), FLOAT(0.2998047f), 0, 0, 0, 0, 0, 0, 0)
                WAIT_FRAMES(3)
            END_LOOP
        END_SIBLING_EVENT
        CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 800, FLOAT(400.0f), 4)
        DELETE_EVENT(LW(14))
        CALL(evt_npc_set_attack_mode, STRING("me"), 0)
        CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
        CALL_CHILD(PTR(met_set_anim_wait_event))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
        ELSE
        CALL(evt_npc_set_attack_mode, STRING("me"), 11)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_GRAVITY)
        CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("SMT_A_1"))
            ELSE
            CALL(evt_npc_set_anim, STRING("me"), STRING("STM_A_1"))
        END_IF
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position, STRING("me"), LW(0), 0, LW(2), 0, 0, 0, 0)
        CALL(evt_npc_set_attack_mode, STRING("me"), 0)
        CALL(evt_npc_set_btlsetup_work, STRING("me"), 0, 0)
        CALL_CHILD(PTR(met_set_anim_wait_event))
        WAIT_MS(1000)
    END_IF
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(met_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL_CHILD(PTR(met_set_anim_wait_event))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(met_return_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL_CHILD(PTR(met_set_anim_wait_event))
    WAIT_MS(500)
    CALL_CHILD(PTR(met_set_anim_walk_event))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patamet_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(patamet_find_event)
    CALL(evt_npc_sound_data_set, STRING("me"), STRING("SFX_FLD_ENM_PATAMET_MOVE2"), 0, 0, 0, 0)
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 0, 75, FLOAT(-50.0f), 0, 3)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 1000, 0, 0, 0, 1)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patamet_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(patamet_return_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(chorobon_move_event)
    CALL(evt_npc_set_scale, STRING("me"), 1, 1, 1)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    LABEL(0)
    CALL(evt_sub_random, 3, LW(4))
    ADD(LW(4), 2)
    LOOP(LW(4))
#ifdef VERSION_G8ME01
        WAIT_FRAMES(1)
#endif
        CALL(evt_sub_random, 360, LW(3))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), 50)
#ifdef VERSION_G8ME01
        CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(30.0f), 0)
#else
        CALL(evt_npc_jump_position, STRING("me"), LW(0), -10000, LW(2), 300, 0, FLOAT(30.0f), 0)
#endif
    END_LOOP
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
    CALL(evt_npc_set_ry, STRING("me"), LW(3))
    CALL(evt_sub_random, 500, LW(4))
    ADD(LW(4), 10)
    WAIT_MS(LW(4))
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(chorobon_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    LOOP(3)
        CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
        CALL(evt_npc_set_ry, STRING("me"), LW(3))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), 50)
#ifdef VERSION_G8ME01
        CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(30.0f), 0)
#else
        CALL(evt_npc_jump_position, STRING("me"), LW(0), -10000, LW(2), 300, 0, FLOAT(30.0f), 0)
#endif
        WAIT_MS(50)
    END_LOOP
    CALL(evt_npc_set_scale, STRING("me"), 1, 1, 1)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(chorobon_lost_event)
    CALL(evt_npc_set_scale, STRING("me"), 1, 1, 1)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(chorobon_return_event)
    CALL(evt_npc_set_scale, STRING("me"), 1, 1, 1)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), 50)
#ifdef VERSION_G8ME01
    CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(30.0f), 0)
#else
    CALL(evt_npc_jump_position, STRING("me"), LW(0), -10000, LW(2), 300, 0, FLOAT(30.0f), 0)
#endif
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(pansy_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(pansy_find_event)
    CALL(evt_npc_sound_data_set, STRING("me"), STRING("SFX_FLD_ENM_PANZI_MOVE2"), STRING("SFX_FLD_ENM_PANZI_MOVE2"), 30000, 0, 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_homing_target, STRING("me"), STRING("mario"), 0, FLOAT(100.0f), FLOAT(10.0f), FLOAT(60.0f), 4)
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(pansy_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_anim, STRING("me"), STRING("PAN_S_1"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(pansy_return_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_anim, STRING("me"), STRING("PAN_S_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PAN_W_1"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(twinkling_pansy_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(twinkling_pansy_find_event)
    CALL(evt_npc_sound_data_set, STRING("me"), STRING("SFX_FLD_ENM_PANZI_MOVE2"), STRING("SFX_FLD_ENM_PANZI_MOVE2"), 30000, 0, 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
    ADD(LW(3), 180)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(300.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 1000, FLOAT(300.0f), 6)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(twinkling_pansy_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_anim, STRING("me"), STRING("PAN_S_1_K"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(twinkling_pansy_return_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_anim, STRING("me"), STRING("PAN_S_1_K"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PAN_W_1_K"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(karon_init_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(mahoon_get_groupname, LW(10))
    CALL(evt_npc_slave_entry, STRING("me"), 0, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_PLAYER_ATTACK)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("KRN_H_1"))
    CALL(evt_npc_set_width, STRING("slave_0"), 24)
    CALL(evt_npc_set_height, STRING("slave_0"), 8)
    RETURN
EVT_END()

EVT_BEGIN(karon_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(karon_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(evt_npc_add_rotate, STRING("slave_0"), 0, 0, 30)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_A_1"))
    WAIT_MS(250)
    CALL(evt_npc_get_position, STRING("me"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_KARON_BONE_THROW1"), LW(2), LW(3), LW(4), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_A_2"))
    WAIT_MS(150)
    BEGIN_SIBLING_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_S_1"))
    END_SIBLING_EVENT
    CALL(evt_npc_set_rotate, STRING("slave_0"), 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    ADD(LW(1), 25)
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 12)
    CALL(evt_npc_jump_position, STRING("slave_0"), LW(0), LW(1), LW(2), 1000, 0, FLOAT(75.0f), 0)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(karon_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(karon_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(karon_blow_event)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL_CHILD(PTR(enemy_common_blow_event))
    RETURN
EVT_END()

EVT_BEGIN(honenoko2_init_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_Z_2"))
    RETURN
EVT_END()

EVT_BEGIN(honenoko2_move_event)
    DEBUG_PUT_MSG(STRING("[EVENT] HONENOKO MOVE\n"))
    LABEL(0)
    CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_npc_wait_msec, STRING("me"), 1000)
        GOTO(0)
    END_IF
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_D_2"))
    CALL(evt_npc_wait_msec, STRING("me"), 170)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_Z_2"))
    CALL(evt_npc_wait_msec, STRING("me"), 10)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(honenoko2_find_event)
    DEBUG_PUT_MSG(STRING("[EVENT] HONENOKO FIND\n"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_D_3"))
    CALL(evt_npc_wait_msec, STRING("me"), 170)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_D_4"))
    CALL(evt_npc_wait_msec, STRING("me"), 170)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_homing_target, STRING("me"), STRING("mario"), 0, FLOAT(120.0f), FLOAT(10.0f), FLOAT(60.0f), 4)
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(honenoko2_lost_event)
    DEBUG_PUT_MSG(STRING("[EVENT] HONENOKO LOST\n"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_wait_msec, STRING("me"), 1000)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_D_2"))
    WAIT_FRAMES(10)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_Z_2"))
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(honenoko2_return_event)
    DEBUG_PUT_MSG(STRING("[EVENT] HONENOKO RETURN\n"))
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_S_1"))
    CALL(evt_npc_wait_msec, STRING("me"), 1000)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KRN_W_1"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(killer_init_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_CULL)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_RECORD_DEFEAT)
    RETURN
EVT_END()

EVT_BEGIN(killer_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 11)
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), 2000)
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, 200, 4)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 17, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(killer_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_damage_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon, STRING("SFX_ENEMY_DIE1"), 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_drop_coin, STRING("me"), LW(3))
    IF_INT_GREATER(LW(3), 0)
        LOOP(LW(3))
            CALL(evt_item_entry, 0, ITEM_COIN, LW(0), LW(1), LW(2), ITEM_TYPE_DROP, -1, 0)
        END_LOOP
    END_IF
    CALL(evt_npc_get_drop_heart, STRING("me"), LW(3))
    IF_INT_GREATER(LW(3), 0)
        LOOP(LW(3))
            CALL(evt_item_entry, 0, ITEM_HEART, LW(0), LW(1), LW(2), ITEM_TYPE_DROP, -1, 0)
        END_LOOP
    END_IF
    CALL(evt_npc_get_drop_flower, STRING("me"), LW(3))
    IF_INT_GREATER(LW(3), 0)
        LOOP(LW(3))
            CALL(evt_item_entry, 0, ITEM_FLOWER, LW(0), LW(1), LW(2), ITEM_TYPE_DROP, -1, 0)
        END_LOOP
    END_IF
    CALL(evt_npc_get_drop_item, STRING("me"), LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(evt_item_entry, 0, LW(3), LW(0), LW(1), LW(2), ITEM_TYPE_DROP_LOST_BADGE, -1, 0)
    END_IF
    CALL(evt_npc_getback_item_entry, STRING("me"))
    CALL(evt_npc_get_drop_fixitem, STRING("me"), LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(evt_item_entry, 0, LW(3), LW(0), LW(1), LW(2), ITEM_TYPE_DROP_LOST_BADGE, -1, 0)
    END_IF
    CALL(evt_npc_set_position, STRING("me"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(killer_cannon_init_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_get_dir, STRING("me"), LW(0))
    CALL(evt_npc_set_unitwork, STRING("me"), 0, LW(0))
    RETURN
EVT_END()

EVT_BEGIN(killer_cannon_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_set_anim, STRING("me"), STRING("KTA_S_1"))
    SET_VALUE(LW(10), 0)
    LABEL(0)
    WAIT_MS(3000)
    LABEL(1)
    WAIT_MS(100)
    ADD(LW(10), 1)
    IF_INT_EQUAL(LW(10), 2)
        SET_VALUE(LW(10), 0)
    END_IF
    CALL(evt_npc_set_anim, STRING("me"), STRING("KTA_A_1"))
    WAIT_MS(200)
    CALL(evt_npc_get_position, STRING("me"), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_KILLER_CANNON1"), LW(2), LW(3), LW(4), 0)
    WAIT_MS(200)
    CALL(evt_npc_get_dir, STRING("me"), LW(1))
    CALL(evt_npc_get_position, STRING("me"), LW(2), LW(3), LW(4))
    CALL(evt_npc_add_dirdist, LW(2), LW(4), LW(1), 30)
    ADD(LW(3), 30)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 18, LW(2), LW(3), LW(4), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_get_dir, STRING("me"), LW(1))
    CALL(killer_make_name, LW(10), LW(0))
    CALL(evt_npc_set_ry, LW(0), LW(1))
    CALL(evt_npc_get_position, STRING("me"), LW(2), LW(3), LW(4))
    CALL(evt_npc_add_dirdist, LW(2), LW(4), LW(1), 30)
    ADD(LW(3), 11)
    CALL(killer_make_name, LW(10), LW(0))
    CALL(evt_npc_set_position, LW(0), LW(2), LW(3), LW(4))
    CALL(killer_make_name, LW(10), LW(0))
    CALL(evt_npc_set_anim, LW(0), STRING("KIL_S_1"))
    CALL(killer_make_name, LW(10), LW(0))
    CALL(evt_npc_change_interrupt, LW(0), 1, PTR(killer_move_event))
    CALL(killer_make_name, LW(10), LW(0))
    CALL(evt_npc_restart_regular_event, LW(0))
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(sambo_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SNB_S_1"))
    LABEL(0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(sambo_find_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SNB_S_1"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_homing_target, STRING("me"), STRING("mario"), 0, FLOAT(100.0f), FLOAT(7.0f), FLOAT(60.0f), 4)
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sambo_lost_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SNB_S_1"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sambo_return_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SNB_S_1"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(100.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sinnosuke_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_STOP_FOR_EVENT)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    RETURN
EVT_END()

EVT_BEGIN(sinnosuke_move_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_Z_3"))
    LABEL(0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(sinnosuke_find_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_Z_3"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    SET_VALUE(LW(15), 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 270)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_S_1"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 270, 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 360)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    LOOP(0)
        CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_A_1a"))
        WAIT_MS(300)
        CALL(evt_npc_set_attack_mode, STRING("me"), 13)
        CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
        CALL(evt_npc_set_ry, STRING("me"), LW(3))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
        CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(200.0f), 6)
        CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_S_1"))
        CALL(evt_npc_set_attack_mode, STRING("me"), 0)
        WAIT_MS(200)
    END_LOOP
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sinnosuke_lost_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_S_1"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sinnosuke_return_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_W_1"))
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(30.0f), 4)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_S_1"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    SET_VALUE(LW(15), 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 270)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIS_Z_3"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 270, 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 360)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sinemon_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    RETURN
EVT_END()

EVT_BEGIN(sinemon_move_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_Z_3"))
    LABEL(0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(sinemon_find_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_Z_3"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    SET_VALUE(LW(15), 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 270)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_S_1"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 270, 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 360)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    LOOP(0)
        CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_A_1a"))
        WAIT_MS(300)
        CALL(evt_npc_set_attack_mode, STRING("me"), 14)
        CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
        CALL(evt_npc_set_ry, STRING("me"), LW(3))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
        CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(200.0f), 6)
        CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_S_1"))
        CALL(evt_npc_set_attack_mode, STRING("me"), 0)
        WAIT_MS(200)
    END_LOOP
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sinemon_lost_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_S_1"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(sinemon_return_event)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_W_1"))
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(30.0f), 4)
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_S_1"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    SET_VALUE(LW(15), 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 270)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_anim, STRING("me"), STRING("SIN_Z_3"))
    CALL(evt_npc_set_rotate, STRING("me"), 0, 270, 0)
    LOOP(0)
        WAIT_FRAMES(1)
        ADD(LW(15), 30)
        CALL(evt_npc_set_rotate, STRING("me"), 0, LW(15), 0)
        IF_INT_GREATER_OR_EQUAL(LW(15), 360)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togedaruma_move_event)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(togedaruma_find_event)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_set_attack_mode, STRING("me"), 15)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_npc_add_rotate, STRING("me"), 0, 45, 0)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_homing_target, STRING("me"), STRING("mario"), 0, FLOAT(120.0f), FLOAT(10.0f), FLOAT(60.0f), 4)
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togedaruma_lost_event)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(togedaruma_return_event)
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(barriern_init_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_slave_entry, STRING("me"), 0, STRING("c_baria"), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_PLAYER_ATTACK)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    RETURN
EVT_END()

EVT_BEGIN(barriern_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_GRAVITY)
    LABEL(0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_3"))
    WAIT_FRAMES(60)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    WAIT_FRAMES(60)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
#ifdef VERSION_G8ME01
    CALL(evt_fbat_trans_floor_position_wide, LW(0), LW(1), LW(2), 30, 50, 60)
#else
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 30, 50)
#endif
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 2000, FLOAT(40.0f), 0, 0, 1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(barriern2_move_event)
    CALL_CHILD(PTR(enemy_common_flag_init_event))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
#endif
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_GRAVITY)
    LABEL(0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_3"))
    WAIT_FRAMES(60)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    WAIT_FRAMES(60)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
#ifdef VERSION_G8ME01
    CALL(evt_fbat_trans_floor_position_wide, LW(0), LW(1), LW(2), 30, 50, 60)
#else
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 30, 50)
#endif
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 1000, FLOAT(120.0f), 0, 0, 1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(barriern_dead_event)
    CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(10))
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(PTR(enemy_common_dead_event))
    RETURN
EVT_END()

EVT_BEGIN(barriern_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    LABEL(0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_2"))
    WAIT_FRAMES(30)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_3"))
    WAIT_FRAMES(10)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_4"))
    BEGIN_SIBLING_EVENT
        SET_FLOAT_VALUE(LW(15), 0)
        LOOP(60)
            ADD_FLOAT(LW(15), FLOAT(0.029296875f))
            CALL(evt_npc_set_scale, STRING("slave_0"), 1, LW(15), 1)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("Z_3"))
    CALL(evt_npc_blur_onoff, 1, STRING("slave_0"))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_BARRI_ATTACK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_BARRI_ATTACK2"), LW(0), LW(1), LW(2), LW(10))
    CALL(evt_npc_set_unitwork, STRING("me"), 0, LW(10))
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(500.0f))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 17)
    CALL(evt_npc_glide_position, STRING("slave_0"), LW(0), LW(1), LW(2), 1250, 10, 0, 0, 1)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(10))
    CALL(evt_snd_sfxoff, LW(10))
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_3"))
    WAIT_FRAMES(30)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    WAIT_FRAMES(30)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

static void piders_draw_yarn_sub(CameraId cameraId, void* param) {
    NpcEntry* npc = param;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec p;
    Vec q;
    Vec a = {0.0f, 0.0f, 0.0f};
    Vec b = {0.0f, 0.0f, 0.0f};
    a.x = npc->territoryBase.x;
    a.y = 30.0f + npc->territoryBase.y;
    a.z = npc->territoryBase.z;
    b.x = npc->position.x;
    b.y = 30.0f + npc->position.y;
    b.z = npc->position.z;
    p = a;
    q = b;
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(p.x - 1.0f, p.y, p.z - 5.0f);
    GXPosition3f32(1.0f + p.x, p.y, p.z - 5.0f);
    GXPosition3f32(1.0f + q.x, q.y, q.z - 5.0f);
    GXPosition3f32(q.x - 1.0f, q.y, q.z - 5.0f);
    GXEnd();
}


USER_FUNC(piders_draw_yawn) {
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    dispEntry(CAMERA_3D, 0.0f, 1, piders_draw_yarn_sub, npc);
    return EVT_RETURN_DONE;
}


USER_FUNC(kamec_fire_magic) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 z = evtGetValue(event, args[2]);
    s32 tx = evtGetValue(event, args[3]);
    s32 ty = evtGetValue(event, args[4]);
    s32 tz = evtGetValue(event, args[5]);
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    EffectEntry* eff = effMagic1N64Entry(0, 60, (f32)x, (f32)y, (f32)z, (f32)tx, (f32)ty, (f32)tz);
    effSetName(eff, npc->description);
    evtSetValue(event, args[6], (EvtWord)eff);
    return EVT_RETURN_DONE;
}


USER_FUNC(kamec_fire_magic_delete) {
    EffectEntry* eff = effNameToPtr(evtNpcNameToPtr(event, "me")->description);
    if (eff) {
        ((EffMagic1N64Work*)eff->userdata)->life = 1;
    }
    return EVT_RETURN_DONE;
}


EVT_BEGIN(barriern_lost_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_3"))
    WAIT_FRAMES(60)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    WAIT_FRAMES(60)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(barriern_return_event)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
#ifdef VERSION_G8ME01
    CALL(evt_fbat_trans_floor_position_wide, LW(0), LW(1), LW(2), 30, 50, 60)
#else
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 30, 50)
#endif
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 2000, FLOAT(40.0f), 0, 0, 1)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(barriern_blow_event)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(10))
    CALL(evt_snd_sfxoff, LW(10))
    CALL_CHILD(PTR(enemy_common_blow_event))
    RETURN
EVT_END()

EVT_BEGIN(piders_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PAI_S_1"))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(piders_draw_yawn)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
            SET_VALUE(LW(3), LW(1))
            SUBTRACT(LW(1), 10)
            CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 150, 0, 0, 4, 0)
            CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(3), LW(2), 150, 0, 0, 4, 0)
            WAIT_FRAMES(100)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
        LOOP(20)
            CALL(evt_npc_add_rotate, STRING("me"), 0, 2, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(0)
            LOOP(40)
                CALL(evt_npc_add_rotate, STRING("me"), 0, -2, 0)
                WAIT_FRAMES(1)
            END_LOOP
            LOOP(40)
                CALL(evt_npc_add_rotate, STRING("me"), 0, 2, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_LOOP
    END_SIBLING_EVENT
    LABEL(0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(piders_find_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(piders_draw_yawn)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    BEGIN_INLINE_EVENT
        LOOP(10)
            CALL(evt_npc_add_rotate, STRING("me"), 0, 6, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(22)
            CALL(evt_npc_add_rotate, STRING("me"), 0, -5, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(20)
            CALL(evt_npc_add_rotate, STRING("me"), 0, 4, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(16)
            CALL(evt_npc_add_rotate, STRING("me"), 0, -3, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(14)
            CALL(evt_npc_add_rotate, STRING("me"), 0, 2, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(10)
            CALL(evt_npc_add_rotate, STRING("me"), 0, -1, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_set_rotate, STRING("me"), 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAIDA_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(piders_get_height, LW(15))
    SET_VALUE(LW(14), LW(15))
    DIVIDE(LW(14), 3)
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(1))
    SUBTRACT(LW(1), LW(15))
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, 0, 4, 0)
    WAIT_FRAMES(60)
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAIDA_MOVE2"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), LW(14))
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 700, 0, 20, 4, 0)
    ADD(LW(1), LW(14))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAIDA_MOVE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 700, 0, 20, 4, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAIDA_MOVE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(3), LW(2), 600, 0, 0, 4, 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(piders_blow_event)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(piders_draw_yawn)
        END_LOOP
    END_SIBLING_EVENT
    CALL_CHILD(PTR(enemy_common_blow_event))
    RETURN
EVT_END()

EVT_BEGIN(pakkun_attack_event)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAKKUN_ATTACK1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_A_1A"))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_A_1B"))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 18)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 30)
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    WAIT_MS(200)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_A_1C"))
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    WAIT_MS(200)
    RETURN
EVT_END()

EVT_BEGIN(pakkun_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_slave_entry, STRING("me"), 0, STRING("c_pakflwr"), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_tribe, STRING("slave_0"), STRING("パックンフラワー"))
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    RETURN
EVT_END()

EVT_BEGIN(pakkun_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_S_1"))
    LABEL(0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(pakkun_find_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAKKUN_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_E_2"))
    WAIT_FRAMES(10)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_sub_random, 10, LW(3))
    CALL(evt_mario_get_mov_spd, LW(4))
    MULTIPLY_FLOAT(LW(4), FLOAT(0.5f))
    ADD_FLOAT(LW(3), LW(4))
    ADD_FLOAT(LW(3), 30)
    CALL(evt_npc_facedirection_add, STRING("mario"), LW(0), LW(3))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 200, 0, 4)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAKKUN_MOVE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_E_1"))
    WAIT_FRAMES(10)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    WAIT_FRAMES(20)
    CALL_CHILD(pakkun_attack_event)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(pakkun_return_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_PAKKUN_MOVE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_E_1"))
    WAIT_FRAMES(10)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    WAIT_FRAMES(20)
    CALL_CHILD(pakkun_attack_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_E_2"))
    WAIT_FRAMES(10)
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 200, 0, 4)
    CALL(evt_npc_set_anim, STRING("me"), STRING("PKF_E_1"))
    WAIT_FRAMES(10)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    WAIT_FRAMES(25)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(dokugassun_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_STOP_FOR_EVENT)
    RETURN
EVT_END()

EVT_BEGIN(dokugassun_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(6.0f), 0, 2)
    CALL(evt_npc_sound_data_set, STRING("me"), 0, 0, 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(-6.0f), 0, 2)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(6.0f), 0, 2)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(6.0f), 0, 2)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(dokugassun_find_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    WAIT_MS(200)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(200.0f))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 10, 500)
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 1000, 1, 0, 4, 1)
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(dokugassun_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(dokugassun_return_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("me"), STRING("W_1"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(-6.0f), 0, 2)
    CALL(evt_npc_sound_data_set, STRING("me"), 0, 0, 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(6.0f), 0, 2)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(-6.0f), 0, 2)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 10, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 1, FLOAT(6.0f), 0, 2)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(basabasa_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    RETURN
EVT_END()

EVT_BEGIN(basabasa_move_event)
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("me"), LW(3), LW(4), LW(5))
    IF_INT_LESS(LW(4), LW(1))
        CALL(evt_npc_set_walk_anim, STRING("me"))
        CALL(evt_set_dir_to_home, STRING("me"))
        CALL(evt_npc_get_dir, STRING("me"), LW(3))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
        CALL(evt_npc_get_home_position, STRING("me"), LW(4), LW(1), LW(5))
        CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-50.0f), 0, 2)
    END_IF
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
#endif
    LABEL(0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("BSA_S_2"))
    WAIT_FRAMES(60)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(basabasa_find_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_get_home_position, STRING("me"), LW(4), LW(5), LW(6))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(5), LW(3), 0, 75, FLOAT(-140.0f), 0, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(basabasa_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(basabasa_return_event)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_npc_get_home_position, STRING("me"), LW(4), LW(1), LW(5))
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-50.0f), 0, 2)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(basabasa2_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(basabasa2_find_event)
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 80, 500)
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 0, 75, FLOAT(-80.0f), 0, 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(basabasa2_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(basabasa2_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 80, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(teresa_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_color, STRING("me"), 255, 255, 255, 255)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-10.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(10.0f), 0, 3)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(teresa_find_event)
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 0, 75, FLOAT(-40.0f), 0, 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(teresa_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(teresa_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT)
    SET_VALUE(LW(0), 255)
    LOOP(16)
        CALL(evt_npc_set_color, STRING("me"), 255, 255, 255, LW(0))
        SUBTRACT(LW(0), 16)
        WAIT_MS(30)
    END_LOOP
    CALL(evt_npc_set_color, STRING("me"), 255, 255, 255, 0)
    WAIT_MS(500)
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("me"), LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 0)
    LOOP(16)
        ADD(LW(0), 16)
        IF_INT_GREATER(LW(0), 255)
            SET_VALUE(LW(0), 255)
        END_IF
        CALL(evt_npc_set_color, STRING("me"), 255, 255, 255, LW(0))
        WAIT_MS(30)
    END_LOOP
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(bubble_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_sound_data_set, STRING("me"), 0, 0, 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(5.0f), 0, 3)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(bubble_find_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 0, 90, FLOAT(0.0f), 0, 2)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(bubble_lost_event)
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(bubble_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_sound_data_reset, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_sound_data_set, STRING("me"), 0, 0, 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hbom_move_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(20.0f), 4)
    WAIT_MS(1000)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(hbom_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 300, 0, FLOAT(20.0f), 0)
    CALL(evt_npc_homing_target, STRING("me"), STRING("mario"), 0, FLOAT(60.0f), FLOAT(10.0f), FLOAT(60.0f), 4)
    WAIT_MS(500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hbom_lost_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hbom_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(30.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(zakowiz_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_slave_entry, STRING("me"), 0, STRING("c_zako_m"), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_PLAYER_ATTACK)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("B_1A"))
    CALL(evt_npc_set_width, STRING("slave_0"), 10)
    CALL(evt_npc_set_height, STRING("slave_0"), 10)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
    RETURN
EVT_END()

EVT_BEGIN(zakowiz_move_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    CALL(npc_check_wall_stop, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(500)
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_MS(1000)
    END_IF
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(zakowiz_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1B"))
    WAIT_MS(500)
    BEGIN_SIBLING_EVENT
        WAIT_MS(700)
        CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    END_SIBLING_EVENT
    CALL(evt_npc_set_rotate, STRING("slave_0"), 0, 0, 0)
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    ADD(LW(1), 25)
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("B_1A"))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 12)
    CALL(evt_npc_jump_position, STRING("slave_0"), LW(0), LW(1), LW(2), 1000, 0, FLOAT(75.0f), 0)
    CALL(evt_npc_get_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 16, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("B_2"))
    WAIT_MS(300)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(zakowiz_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(zakowiz_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(zakowiz_blow_event)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL_CHILD(enemy_common_blow_event)
    RETURN
EVT_END()

EVT_BEGIN(hannya_move_event)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(hannya_find_event)
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_HANNYA_MOVE1"), LW(1), LW(2), LW(3), 0)
    CALL(evt_npc_get_home_position, STRING("me"), LW(4), LW(5), LW(6))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_npc_set_attack_mode, STRING("me"), 11)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_3"))
    CALL(evt_npc_move_position, STRING("me"), LW(1), LW(3), 0, FLOAT(180.0f), 6)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hannya_lost_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hannya_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 2)
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 3000, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(mahoon_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(mahoon_get_groupname, LW(10))
    CALL(evt_npc_slave_entry, STRING("me"), 0, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("MAH_S_2A"))
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_slave_entry, STRING("me"), 1, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, STRING("slave_1"), STRING("MAH_S_2A"))
    CALL(evt_npc_set_position, STRING("slave_1"), 0, -1000, 0)
    CALL(evt_npc_slave_entry, STRING("me"), 2, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, STRING("slave_2"), STRING("MAH_S_2A"))
    CALL(evt_npc_set_position, STRING("slave_2"), 0, -1000, 0)
    CALL(evt_npc_slave_entry, STRING("me"), 3, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, STRING("slave_3"), STRING("MAH_S_2A"))
    CALL(evt_npc_set_position, STRING("slave_3"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_1"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_2"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_3"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_1"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_2"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_3"), NPC_FLAG_NO_HIT)
    RETURN
EVT_END()

EVT_BEGIN(mahoon_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_npc_get_dir, STRING("me"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_0"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_1"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_2"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_3"), LW(3))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_1"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_2"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_3"), LW(0), LW(1), LW(2))
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(5.0f), 0, 3)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(mahoon_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_1"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_2"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_3"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(mahoon_find_event)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_npc_get_dir, STRING("me"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_0"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_1"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_2"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_3"), LW(3))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_1"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_2"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_3"), LW(0), LW(1), LW(2))
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_set_run_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_get_home_position, STRING("me"), LW(4), LW(5), LW(6))
    CALL(evt_npc_add_dirdist, LW(1), LW(3), LW(0), FLOAT(100.0f))
    CALL(evt_fbat_trans_floor_position, LW(1), LW(2), LW(3), 20, 500)
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_glide_position, STRING("me"), LW(1), LW(2), LW(3), 0, 135, FLOAT(0.0f), 0, 0)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(mahoon_lost_event)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_npc_get_dir, STRING("me"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_0"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_1"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_2"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_3"), LW(3))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_1"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_2"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_3"), LW(0), LW(1), LW(2))
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(mahoon_return_event)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_npc_get_dir, STRING("me"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_0"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_1"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_2"), LW(3))
            CALL(evt_npc_set_ry, STRING("slave_3"), LW(3))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 5)
            CALL(evt_npc_set_position, STRING("slave_1"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_2"), LW(0), LW(1), LW(2))
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(-25.0f))
            ADD(LW(1), 30)
            CALL(evt_npc_set_position, STRING("slave_3"), LW(0), LW(1), LW(2))
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 20, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kamec_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(mahoon_get_groupname, LW(10))
    CALL(evt_npc_slave_entry, STRING("me"), 0, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_PLAYER_ATTACK)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_GRAVITY)
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("KMK_Z_1"))
    CALL(evt_npc_set_width, STRING("slave_0"), 10)
    CALL(evt_npc_set_height, STRING("slave_0"), 10)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    RETURN
EVT_END()

EVT_BEGIN(kamec_move_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_S_2"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    LABEL(0)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 50, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(kamec_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_A_3A"))
    WAIT_MS(700)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_A_3B"))
    WAIT_MS(150)
    BEGIN_SIBLING_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_S_2"))
    END_SIBLING_EVENT
    CALL(evt_npc_set_rotate, STRING("slave_0"), 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(20.0f))
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    ADD(LW(4), 20)
    CALL(kamec_fire_magic, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(10))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 12)
    CALL(evt_npc_glide_position, STRING("slave_0"), LW(0), LW(1), LW(2), 1000, 0, FLOAT(0.0f), 0, 3)
    CALL(kamec_fire_magic_delete, LW(10))
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kamec_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kamec_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_set_walk_anim, STRING("me"))
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(-5.0f), 0, 3)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(25.0f))
    CALL(evt_fbat_trans_floor_position, LW(0), LW(1), LW(2), 50, 500)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 500, 0, FLOAT(5.0f), 0, 3)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kamec_blow_event)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL_CHILD(enemy_common_blow_event)
    RETURN
EVT_END()

EVT_BEGIN(kamec2_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(mahoon_get_groupname, LW(10))
    CALL(evt_npc_slave_entry, STRING("me"), 0, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_PLAYER_ATTACK)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_GRAVITY)
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("KMK_Z_1"))
    CALL(evt_npc_set_width, STRING("slave_0"), 10)
    CALL(evt_npc_set_height, STRING("slave_0"), 10)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    RETURN
EVT_END()

EVT_BEGIN(kamec2_move_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_S_1"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(kamec2_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_A_2A"))
    WAIT_MS(700)
    CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_A_2B"))
    WAIT_MS(150)
    BEGIN_SIBLING_EVENT
        WAIT_MS(1000)
        CALL(evt_npc_set_anim, STRING("me"), STRING("KMK_S_1"))
    END_SIBLING_EVENT
    CALL(evt_npc_set_rotate, STRING("slave_0"), 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(20.0f))
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    ADD(LW(4), 20)
    CALL(kamec_fire_magic, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(10))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 12)
    CALL(evt_npc_glide_position, STRING("slave_0"), LW(0), LW(1), LW(2), 1000, 0, FLOAT(0.0f), 0, 3)
    CALL(kamec_fire_magic_delete, LW(10))
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kamec2_lost_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 1, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kamec2_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(kamec2_blow_event)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL_CHILD(enemy_common_blow_event)
    RETURN
EVT_END()

EVT_BEGIN(hbross_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(mahoon_get_groupname, LW(10))
    CALL(evt_npc_slave_entry, STRING("me"), 0, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_PLAYER_ATTACK)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("BRO_A_1B"))
    CALL(evt_npc_set_width, STRING("slave_0"), 10)
    CALL(evt_npc_set_height, STRING("slave_0"), 10)
    RETURN
EVT_END()

EVT_BEGIN(hbross_move_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_set_anim, STRING("me"), STRING("BRO_S_1"))
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(hbross_find_event)
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(evt_npc_add_rotate, STRING("slave_0"), 0, 0, 30)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(evt_npc_set_anim, STRING("me"), STRING("BRO_A_1A"))
    WAIT_MS(400)
    BEGIN_SIBLING_EVENT
        WAIT_MS(500)
        CALL(evt_npc_set_anim, STRING("me"), STRING("BRO_S_1"))
    END_SIBLING_EVENT
    CALL(evt_npc_set_rotate, STRING("slave_0"), 0, 0, 0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    ADD(LW(1), 25)
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(20.0f))
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 12)
    CALL(evt_npc_jump_position, STRING("slave_0"), LW(0), LW(1), LW(2), 1000, 0, FLOAT(75.0f), 0)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

USER_FUNC(chain_main) {
    NpcEntry* me = evtNpcNameToPtr(event, "me");
    NpcEntry* end = me->slaves[3];
    NpcEntry* cur;
    NpcEntry* prev;
    f32 len, sag, cx, cy, cz, dx, dy, dz, t;
    s32 i;
    dx = me->position.x - end->position.x;
    dy = me->position.y - end->position.y;
    dz = me->position.z - end->position.z;
    len = (f32)sqrt(dx * dx + dy * dy + dz * dz) / 4.0f;
    if (len < 10.0f) {
        len = 10.0f;
    }
    sag = 300.0f / (len * len);
    cur = me->slaves[0];
    cy = cur->position.y;
    cy -= sag;
    cx = cur->position.x;
    cz = cur->position.z;
    dx = me->position.x - cx;
    dy = me->position.y - cy;
    dz = me->position.z - cz;
    {
    f32 dist = (f32)sqrt(dx * dx + dy * dy + dz * dz);
    t = 1.0f - len / dist;
    }
    cy = dy * t + cy;
    cx = dx * t + cx;
    cz = dz * t + cz;
    if (cy < me->territoryBase.y) {
        cy = me->territoryBase.y;
    }
    {
    Vec a = {0.0f, 0.0f, 0.0f};
    a.x = cx;
    a.y = cy;
    a.z = cz;
    cur->position = a;
    }
    for (i = 0; i < 2; i++) {
        cur = me->slaves[i + 1];
        prev = me->slaves[i];
        cy = cur->position.y;
        cy -= sag;
        cx = cur->position.x;
        cz = cur->position.z;
        dx = prev->position.x - cx;
        dy = prev->position.y - cy;
        dz = prev->position.z - cz;
        t = 1.0f - len / (f32)sqrt(dx * dx + dy * dy + dz * dz);
        cy = dy * t + cy;
        cx = dx * t + cx;
        cz = dz * t + cz;
        if (cy < me->territoryBase.y) {
            cy = me->territoryBase.y;
        }
        {
        Vec b = {0.0f, 0.0f, 0.0f};
        b.x = cx;
        b.y = cy;
        b.z = cz;
        cur->position = b;
        }
    }
    for (i = 3; i >= 1; i--) {
        cur = me->slaves[i - 1];
        prev = me->slaves[i];
        cy = cur->position.y;
        cy -= sag;
        cx = cur->position.x;
        cz = cur->position.z;
        dx = prev->position.x - cx;
        dy = prev->position.y - cy;
        dz = prev->position.z - cz;
        t = 1.0f - len / (f32)sqrt(dx * dx + dy * dy + dz * dz);
        cy = dy * t + cy;
        cx = dx * t + cx;
        cz = dz * t + cz;
        if (cy < me->territoryBase.y) {
            cy = me->territoryBase.y;
        }
        {
        Vec c = {0.0f, 0.0f, 0.0f};
        c.x = cx;
        c.y = cy;
        c.z = cz;
        cur->position = c;
        }
    }
    return EVT_RETURN_DONE;
}


USER_FUNC(camGetPos) {
    EvtWord* args = event->args;
    evtSetFloat(event, args[0], camGetPtr(CAMERA_3D)->cameraPos.x);
    return EVT_RETURN_DONE;
}


USER_FUNC(cliffCheck) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    f32 dirv;
    f32 hx, hy, hz, dist, nx, ny, nz;
    HitEntry* h1;
    HitEntry* h2;
    if (90.0f == npcTransRytoFaceDir(npc)) {
        dirv = 1.0f;
    } else {
        dirv = -1.0f;
    }
    dist = 5.0f;
    h1 = hitCheckFilter(0, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, 1.0f + npc->position.y, npc->position.z, 0.0f, -1.0f, 0.0f);
    dist = 5.0f;
    h2 = hitCheckFilter(0, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, 1.0f + npc->position.y, npc->position.z, dirv, -1.0f, 0.0f);
    if (h1 && h2) {
        evtSetValue(event, args[0], 0);
    } else if (h1 && !h2) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 2);
    }
    return EVT_RETURN_DONE;
}


USER_FUNC(upperCheck) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    f32 hx, hy, hz, dist, nx, ny, nz;
    HitEntry* h1;
    HitEntry* h2;
    dist = 5.0f;
    h1 = hitCheckFilter(0, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x - 15.0f, 30.0f + npc->position.y, npc->position.z, 0.0f, 2.0f, 0.0f);
    h2 = hitCheckFilter(0, &hx, &hy, &hz, &dist, &nx, &ny, &nz, 15.0f + npc->position.x, 30.0f + npc->position.y, npc->position.z, 0.0f, 2.0f, 0.0f);
    if (h1 || h2) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}


USER_FUNC(getSpdWalk) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    evtSetFloat(event, args[0], npc->territoryLoiter.z);
    return EVT_RETURN_DONE;
}


USER_FUNC(getSpdRun) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    evtSetFloat(event, args[0], npc->territoryHoming.z);
    return EVT_RETURN_DONE;
}


USER_FUNC(limitSpd) {
    f32 v = evtGetFloat(event, event->args[0]);
    evtNpcNameToPtr(event, "me")->maxFallSpeed = v;
    return EVT_RETURN_DONE;
}


EVT_BEGIN(hbross_lost_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hbross_return_event)
    CALL(evt_npc_set_stay_anim, STRING("me"))
    WAIT_MS(500)
    CALL(evt_set_dir_to_home, STRING("me"))
    CALL(evt_npc_get_dir, STRING("me"), LW(3))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(40.0f), 4)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(hbross_blow_event)
    CALL(evt_npc_set_attack_mode, STRING("slave_0"), 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL_CHILD(enemy_common_blow_event)
    RETURN
EVT_END()

EVT_BEGIN(enemy_event_dead_event)
    SET_VALUE(LW(10), STRING("me"))
    CALL_CHILD(enemy_event_dead_event_sub)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(enemy_slave_common_dead_event)
    CALL(evt_npc_delete, STRING("me"))
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY | NPC_FLAG_UPDATE_BY_EVENT | NPC_FLAG_STOP_FOR_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(testenemynpc_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    RETURN
EVT_END()

EVT_BEGIN(npc_urouro_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(kuriboo_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    RETURN
EVT_END()

EVT_BEGIN(patakuri_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(patapata_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_WALL_BOUNCE | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(patamet_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(chorobon_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(pansy_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    RETURN
EVT_END()

EVT_BEGIN(twinkling_pansy_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    RETURN
EVT_END()

EVT_BEGIN(karon_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(sambo_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_UPDATE_BY_EVENT)
    RETURN
EVT_END()

EVT_BEGIN(togedaruma_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    RETURN
EVT_END()

EVT_BEGIN(piders_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    RETURN
EVT_END()

EVT_BEGIN(basabasa_dead_event)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(basabasa2_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(basabasa2_dead_event)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(teresa_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(teresa_dead_event)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(bubble_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_UPDATE_BY_EVENT)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(hbom_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    RETURN
EVT_END()

EVT_BEGIN(zakowiz_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(hannya_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 2)
    RETURN
EVT_END()

EVT_BEGIN(kamec_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(kamec2_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(hbross_dead_event)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    CALL_CHILD(enemy_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(zakoM2D_dead_event)
    CALL(evt_npc_status_onoff, 0, STRING("slave_0"), 4)
    CALL_CHILD(enemy_2d_common_dead_event)
    RETURN
EVT_END()

EVT_BEGIN(wanwan_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(mahoon_get_groupname, LW(10))
    CALL(evt_npc_slave_entry, STRING("me"), 0, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, LW(0), STRING("WAN_S_1B"))
    CALL_CHILD(wanwan_init_event_sub)
    CALL(evt_npc_slave_entry, STRING("me"), 1, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, LW(0), STRING("WAN_S_1B"))
    CALL_CHILD(wanwan_init_event_sub)
    CALL(evt_npc_slave_entry, STRING("me"), 2, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, LW(0), STRING("WAN_S_1B"))
    CALL_CHILD(wanwan_init_event_sub)
    CALL(evt_npc_slave_entry, STRING("me"), 3, LW(10), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_anim, LW(0), STRING("WAN_Z_2"))
    CALL_CHILD(wanwan_init_event_sub)
    RETURN
EVT_END()

EVT_BEGIN(wanwan_init_event_sub)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_FLOOR_COLLISION)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_HIDE)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_TALK_TURN)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_FIND)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_CULL)
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_SHADOW)
    CALL(evt_npc_get_home_position, STRING("me"), LW(1), LW(2), LW(3))
    CALL(evt_npc_set_position, LW(0), LW(1), LW(2), LW(3))
    RETURN
EVT_END()

EVT_BEGIN(wanwan_move_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_attack_mode, STRING("me"), 0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("WAN_S_1A"))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
            IF_INT_GREATER_OR_EQUAL(LW(0), LW(10))
                CALL(evt_npc_set_ry, STRING("me"), 270)
            ELSE
                CALL(evt_npc_set_ry, STRING("me"), 90)
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
        WAIT_MS(1500)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_NO_FIND)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(chain_main)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_npc_set_anim, STRING("me"), STRING("WAN_A_1"))
            WAIT_MS(1000)
            CALL(evt_npc_set_anim, STRING("me"), STRING("WAN_S_1A"))
            WAIT_MS(500)
        END_LOOP
    END_SIBLING_EVENT
    LABEL(0)
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_sub_random, 360, LW(3))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), 50)
    CALL(evt_sub_random, 15, LW(4))
    ADD(LW(4), 15)
    SET_VALUE(LW(5), LW(4))
    SET_VALUE(LW(6), LW(4))
    MULTIPLY(LW(6), 17)
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_WAN_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), LW(6), 80, LW(5), 1)
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_WAN_LANDING1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(wanwan_find_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_FIND)
    CALL(evt_npc_set_anim, STRING("me"), STRING("WAN_A_1"))
    CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    CALL(evt_npc_set_attack_mode, STRING("me"), 12)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(chain_main)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_WAN_MOVE2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_FLD_ENM_WAN_ATTACK2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_get_home_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(3))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), 100)
    CALL(evt_npc_glide_position, STRING("me"), LW(0), LW(1), LW(2), 0, 600, FLOAT(0.0f), 0, 6)
    WAIT_MS(1500)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

USER_FUNC(zakoEntryDokan) {
    char names[3][16];
    char nm3[32];
    NpcEntry* me = evtNpcNameToPtr(event, "me");
    NpcEntry* zako;
    s32 i;
    if (!strcmp(me->description, "\214R\222c\203U\203R\202\252\202\305\202\351\223y\212\307\202P")) {
        strcpy(names[0], "\214R\222c\203U\203R\223y\212\307\202P");
        strcpy(names[1], "\214R\222c\203U\203R\223y\212\307\202Q");
        strcpy(names[2], "\214R\222c\203U\203R\223y\212\307\202R");
        strcpy(nm3, "MOBJ_mobj9");
    }
    if (!strcmp(me->description, "\214R\222c\203U\203R\202\252\202\305\202\351\223y\212\307\202Q")) {
        strcpy(names[0], "\214R\222c\203U\203R\223y\212\307\202S");
        strcpy(names[1], "\214R\222c\203U\203R\223y\212\307\202T");
        strcpy(names[2], "\214R\222c\203U\203R\223y\212\307\202U");
        strcpy(nm3, "MOBJ_mobj14");
    }
    {
        const char* hn = hitGetName(marioGetPtr()->hitobjStandOn);
        if (hn && !strcmp(hn, nm3)) return EVT_RETURN_DONE;
        hn = hitGetName(marioGetPtr()->hitobjJumpFrom);
        if (hn && !strcmp(hn, nm3)) return EVT_RETURN_DONE;
        hn = hitGetName(marioGetPtr()->hitobjFront);
        if (hn && !strcmp(hn, nm3)) return EVT_RETURN_DONE;
    }
    for (i = 0; i < 3; i++) {
        zako = npcNameToPtr(names[i]);
        if (zako->position.y <= -75.0f) {
            zako->flags |= (NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY);
            zako->flags &= ~NPC_FLAG_NO_HIT;
            zako->jumpFlags |= 0x1002;
            zako->jumpFlags &= 0xEFAFFFFB;
            {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = me->position.x;
            pos.y = me->position.y - 35.0f;
            pos.z = me->position.z;
            zako->position = pos;
            }
            zako->rotation = (Vec){0.0f, 0.0f, 0.0f};
            zako->unitWork[0] = (void*)1;
            if (zako->moveEvent) {
                EventEntry* e;
                if (zako->regularEvtId) {
                    evtDeleteID(zako->regularEvtId);
                }
                e = evtEntry(zako->moveEvent, 0, EVENT_START_IMMEDIATE);
                e->wNpcEventType = 1;
                e->ownerNPC = zako;
                zako->regularEvtId = e->eventId;
            }
            return EVT_RETURN_DONE;
        }
    }
    return EVT_RETURN_DONE;
}


USER_FUNC(dokanCheck) {
    EvtWord* args = event->args;
    char name[32];
    NpcEntry* me = evtNpcNameToPtr(event, "me");
    if (!strcmp(me->description, "\214R\222c\203U\203R\202\252\202\305\202\351\223y\212\307\202P")) {
        strcpy(name, "mobj9");
    }
    if (!strcmp(me->description, "\214R\222c\203U\203R\202\252\202\305\202\351\223y\212\307\202Q")) {
        strcpy(name, "mobj14");
    }
    if (mobjNameToPtrNoAssert(name)) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}


USER_FUNC(zakoEntryFall) {
    char names[3][16];
    NpcEntry* me = evtNpcNameToPtr(event, "me");
    NpcEntry* zako;
    s32 i;
    if (!strcmp(me->description, "\214R\222c\203U\203R\215~\211\272\222n\223_\202P")) {
        strcpy(names[0], "\214R\222c\203U\203R\215~\211\272\202P");
        strcpy(names[1], "\202\310\202\265");
        strcpy(names[2], "\202\310\202\265");
    }
    if (!strcmp(me->description, "\214R\222c\203U\203R\215~\211\272\222n\223_\202Q\201Q\202P")) {
        strcpy(names[0], "\214R\222c\203U\203R\215~\211\272\202P");
        strcpy(names[1], "\214R\222c\203U\203R\215~\211\272\202Q");
        strcpy(names[2], "\214R\222c\203U\203R\215~\211\272\202R");
    }
    if (!strcmp(me->description, "\214R\222c\203U\203R\215~\211\272\222n\223_\202Q\201Q\202Q")) {
        strcpy(names[0], "\214R\222c\203U\203R\215~\211\272\202S");
        strcpy(names[1], "\214R\222c\203U\203R\215~\211\272\202T");
        strcpy(names[2], "\214R\222c\203U\203R\215~\211\272\202U");
    }
    for (i = 0; i < 3; i++) {
        zako = npcNameToPtr_NoAssert(names[i]);
        if (zako && zako->position.y <= -75.0f) {
            zako->flags |= (NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY);
            zako->flags &= ~NPC_FLAG_NO_HIT;
            zako->jumpFlags |= 0x1002;
            zako->jumpFlags &= 0xEF8FFFFB;
            {
            Vec pos = {0.0f, 0.0f, 0.0f};
            pos.x = me->position.x;
            pos.y = me->position.y;
            pos.z = me->position.z;
            zako->position = pos;
            }
            zako->rotation = (Vec){0.0f, 0.0f, 0.0f};
            zako->unitWork[0] = (void*)2;
            if (zako->moveEvent) {
                EventEntry* e;
                if (zako->regularEvtId) {
                    evtDeleteID(zako->regularEvtId);
                }
                e = evtEntry(zako->moveEvent, 0, EVENT_START_IMMEDIATE);
                e->wNpcEventType = 1;
                e->ownerNPC = zako;
                zako->regularEvtId = e->eventId;
            }
            return EVT_RETURN_DONE;
        }
    }
    return EVT_RETURN_DONE;
}


USER_FUNC(npc_fire_eff_fadeout) {
    EvtWord* args;
    NpcEntry* npc;
    f32 x, y, z;
    EffectEntry* eff;
    npc = evtNpcNameToPtr(event, "me");
    args = event->args;
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    eff = (EffectEntry*)evtGetValue(event, args[3]);
    if (isFirstCall) {
        npc->unitWork[0] = effFireEntry(5, 1000, x, y, z, 1.0f);
        npc->unitWork[1] = (void*)eff;
    }
    eff = npc->unitWork[0];
    npc->unitWork[1] = (char*)npc->unitWork[1] - 1;
    if ((EvtWord)npc->unitWork[1] <= 30) {
        f32 sc = intplGetValue(EASING_QUADRATIC_IN, 1.0f, 0.0f, 30 - (EvtWord)npc->unitWork[1], 30);
        ((EffFireWork*)eff->userdata)->scaleX = sc;
        ((EffFireWork*)eff->userdata)->scaleY = sc;
        ((EffFireWork*)eff->userdata)->alpha = (u8)intplGetValue(EASING_LINEAR, 255.0f, 0.0f, 30 - (EvtWord)npc->unitWork[1], 30);
    }
    if ((EvtWord)npc->unitWork[1] <= 0) {
        effDelete(eff);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}


USER_FUNC(set_bottle_flag_init) {
    NpcEntry* npc = evtNpcNameToPtr(event, "slave_0");
    npc->flags |= (NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY | NPC_FLAG_2D | NPC_FLAG_NO_PLAYER_ATTACK);
    npc->hazardFlags |= 0x1f;
    return EVT_RETURN_DONE;
}


USER_FUNC(gesso_ground_check) {
    EvtWord* args = event->args;
    NpcEntry* npc = evtNpcNameToPtr(event, "me");
    f32 hx, hy, hz, dist, nx, ny, nz;
    dist = 5.0f;
    if (hitCheckFilter(0, &hx, &hy, &hz, &dist, &nx, &ny, &nz, npc->position.x, 1.0f + npc->position.y, npc->position.z, 0.0f, -1.0f, 0.0f)) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}


EVT_BEGIN(kuriboo2D_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_2D)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4)
    CALL(limitSpd, FLOAT(400.0f))
    RETURN
EVT_END()

EVT_BEGIN(kuriboo2D_move_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    LABEL(0)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 4096)
    CALL(evt_npc_get_position, STRING("me"), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(camGetPos, LW(1))
    SUBTRACT(LW(0), LW(1))
    IF_INT_LESS_OR_EQUAL(LW(0), 250)
        GOTO(1)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(1)
    CALL(evt_npc_set_anim, STRING("me"), STRING("W_1"))
    LABEL(2)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 1000)
    CALL(getSpdWalk, LW(3))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, LW(3), 4)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_FRAMES(1)
    GOTO(2)
    RETURN
EVT_END()

EVT_BEGIN(kuriboo2D_find_event)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_GREATER_OR_EQUAL(LW(1), -100)
        CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    END_IF
    CALL(evt_npc_set_anim, STRING("me"), STRING("R_1"))
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_reverse_ry, STRING("me"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 1000)
    CALL(getSpdRun, LW(3))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 4000, LW(3), 2)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(zako2D_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_2D)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2097152)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4)
    CALL(limitSpd, FLOAT(400.0f))
    RETURN
EVT_END()

EVT_BEGIN(zako2D_move_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    LABEL(0)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 4096)
    CALL(evt_npc_get_position, STRING("me"), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(camGetPos, LW(1))
    SUBTRACT(LW(0), LW(1))
    IF_INT_LESS_OR_EQUAL(LW(0), 250)
        GOTO(1)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(1)
    CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
        CALL(evt_npc_set_anim, STRING("me"), STRING("Z_1"))
        CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
        CALL(evt_npc_set_ry, STRING("me"), LW(0))
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_FLOOR_COLLISION)
        CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
        LOOP(35)
            CALL(evt_npc_status_onoff, 1, STRING("me"), 4096)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            ADD(LW(1), 1)
            CALL(evt_npc_set_position, STRING("me"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION)
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_GRAVITY)
        CALL(evt_npc_set_anim, STRING("me"), STRING("W_1"))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 50)
        CALL(evt_npc_jump_position, STRING("me"), LW(0), LW(1), LW(2), 500, FLOAT(100.0f), FLOAT(30.0f), 1)
    END_IF
    CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
        CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
        CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_GRAVITY)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_jump_position, STRING("me"), LW(0), -100, LW(2), 0, FLOAT(100.0f), FLOAT(0.0f), 1)
    END_IF
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4096)
    CALL(evt_npc_set_anim, STRING("me"), STRING("W_1"))
    LABEL(10)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 1000)
            CALL(getSpdWalk, LW(3))
            CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, LW(3), 4)
            CALL(evt_npc_reverse_ry, STRING("me"))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    LABEL(11)
    CALL(cliffCheck, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        DELETE_EVENT(LW(15))
        CALL(evt_npc_reverse_ry, STRING("me"))
        WAIT_FRAMES(1)
        GOTO(10)
    END_IF
    WAIT_FRAMES(1)
    GOTO(11)
    RETURN
EVT_END()

EVT_BEGIN(zako2D_find_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    IF_INT_GREATER_OR_EQUAL(LW(1), -100)
        CALL(evt_eff_fukidashi, 1, 0, STRING("me"), 0, 0, 0, 0, 0, 0, 0, 30)
    END_IF
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1"))
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 1000)
    CALL(getSpdRun, LW(3))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 3000, LW(3), 2)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    LABEL(99)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(dokan2D_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4)
    RETURN
EVT_END()

EVT_BEGIN(dokan2D_regl_event)
    LABEL(0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(camGetPos, LW(1))
    SUBTRACT(LW(0), LW(1))
    IF_INT_LESS_OR_EQUAL(LW(0), 250)
        GOTO(1)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(1)
    CALL(dokanCheck, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(zakoEntryDokan)
    WAIT_FRAMES(120)
    GOTO(1)
    LABEL(99)
    CALL(evt_npc_delete, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(fall2D_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_DRAW)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4)
    RETURN
EVT_END()

EVT_BEGIN(fall2D_regl_event)
    LABEL(0)
    CALL(evt_npc_get_position, STRING("me"), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(camGetPos, LW(1))
    SUBTRACT(LW(0), LW(1))
    IF_INT_LESS_OR_EQUAL(LW(0), 250)
        GOTO(1)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(1)
    CALL(zakoEntryFall)
    WAIT_FRAMES(120)
    GOTO(1)
    RETURN
EVT_END()

EVT_BEGIN(zakoM2D_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_2D)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4)
    CALL(limitSpd, FLOAT(400.0f))
    CALL(evt_npc_slave_entry, STRING("me"), 0, STRING("c_zako_km"), LW(0), PTR(enemy_slave_common_dead_event))
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(set_bottle_flag_init)
    RETURN
EVT_END()

EVT_BEGIN(zakoM2D_move_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    LABEL(2)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 4096)
    CALL(evt_npc_status_check, STRING("me"), 4, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(3)
    END_IF
    WAIT_FRAMES(1)
    GOTO(2)
    LABEL(3)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 100, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    LABEL(0)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 4096)
    CALL(evt_npc_get_position, STRING("me"), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(camGetPos, LW(1))
    SUBTRACT(LW(0), LW(1))
    IF_INT_LESS_OR_EQUAL(LW(0), 250)
        GOTO(1)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    LABEL(1)
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    WAIT_FRAMES(1)
    GOTO(1)
    RETURN
EVT_END()

EVT_BEGIN(zakoM2D_find_event)
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("B_1"))
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 10)
    ADD(LW(1), 20)
    CALL(evt_npc_set_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    IF_INT_GREATER_OR_EQUAL(LW(10), 200)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_facedirection_add, STRING("me"), LW(0), 100)
        CALL(evt_npc_jump_position, STRING("slave_0"), LW(0), LW(1), LW(2), 0, FLOAT(300.0f), FLOAT(300.0f), 0)
    ELSE
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_facedirection_add, STRING("me"), LW(0), LW(10))
        MULTIPLY_FLOAT(LW(10), FLOAT(1.5f))
        CALL(evt_npc_jump_position, STRING("slave_0"), LW(0), LW(1), LW(2), 0, LW(10), FLOAT(300.0f), 0)
    END_IF
    CALL(evt_npc_get_position, STRING("slave_0"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_CONDITION_FIRE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ENM_3RD2_CRASH1"), LW(0), LW(1), LW(2), 0)
    SUBTRACT(LW(1), 5)
    ADD(LW(2), 200)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 16, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    LOOP(12)
        CALL(evt_eff64, STRING(""), STRING("glass_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 30, 0, 0, 0, 0, 0, 0)
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(npc_fire_eff_fadeout, LW(0), LW(1), LW(2), 60)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("slave_0"), STRING("B_2"))
    WAIT_FRAMES(10)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_DRAW)
    WAIT_FRAMES(60)
    CALL(evt_npc_set_position, STRING("slave_0"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("slave_0"), NPC_FLAG_NO_HIT)
    RETURN
EVT_END()

EVT_BEGIN(zakoM2D_find_start_event)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 100, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
    SUBTRACT_FLOAT(LW(10), LW(0))
    IF_INT_LESS(LW(10), 0)
        MULTIPLY_FLOAT(LW(10), FLOAT(-1.0f))
    END_IF
    IF_INT_LESS(LW(10), 30)
        SET_FLOAT_VALUE(LW(10), FLOAT(30.0f))
    END_IF
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1"))
    WAIT_FRAMES(64)
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_2"))
    WAIT_FRAMES(24)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(24)
        CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    END_SIBLING_EVENT
    SPAWN_EVENT(zakoM2D_find_event)
    WAIT_FRAMES(100)
    CALL(evt_npc_return_interrupt, STRING("me"))
    RETURN
EVT_END()

EVT_BEGIN(gesso2D_init_event)
    CALL_CHILD(enemy_common_flag_init_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_2D)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 0, STRING("me"), NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 268435456)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2097152)
    CALL(evt_npc_status_onoff, 0, STRING("me"), 4)
    RETURN
EVT_END()

EVT_BEGIN(gesso2D_move_event)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    LABEL(0)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 4096)
    CALL(evt_npc_get_position, STRING("me"), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(camGetPos, LW(1))
    SUBTRACT(LW(0), LW(1))
    IF_INT_LESS_OR_EQUAL(LW(0), 250)
        GOTO(1)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    LABEL(1)
    SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(13), FLOAT(0.0f))
    LOOP(0)
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        WAIT_FRAMES(1)
        SUBTRACT(LW(0), LW(10))
        IF_INT_GREATER_OR_EQUAL(LW(0), 300)
            GOTO(5)
        END_IF
        IF_INT_LESS_OR_EQUAL(LW(0), -300)
            GOTO(5)
        END_IF
        IF_INT_LESS(LW(1), LW(11))
            BREAK_LOOP
        END_IF
        IF_INT_LESS(LW(1), 90)
            BREAK_LOOP
        END_IF
        LABEL(5)
        CALL(gesso_ground_check, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            SET_FLOAT_VALUE(LW(14), LW(1))
            SUBTRACT_FLOAT(LW(14), LW(13))
            ADD_FLOAT(LW(13), FLOAT(0.099609375f))
            IF_INT_GREATER_OR_EQUAL(LW(13), FLOAT(1.0f))
                SET_FLOAT_VALUE(LW(13), FLOAT(1.0f))
            END_IF
            CALL(evt_npc_set_position, STRING("me"), LW(0), LW(14), LW(2))
        END_IF
    END_LOOP
    CALL(evt_get_target_dir, STRING("me"), STRING("mario"), LW(0))
    CALL(evt_npc_set_ry, STRING("me"), LW(0))
    CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
    CALL(evt_npc_facedirection_add, STRING("me"), LW(10), 100)
    ADD(LW(11), 50)
    IF_INT_GREATER(LW(11), 350)
        SET_VALUE(LW(11), 350)
    END_IF
    LOOP(60)
        CALL(gesso_ground_check, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
            SET_FLOAT_VALUE(LW(14), LW(1))
            SUBTRACT_FLOAT(LW(14), LW(13))
            ADD_FLOAT(LW(13), FLOAT(0.099609375f))
            IF_INT_GREATER_OR_EQUAL(LW(13), FLOAT(1.0f))
                SET_FLOAT_VALUE(LW(13), FLOAT(1.0f))
            END_IF
            CALL(evt_npc_set_position, STRING("me"), LW(0), LW(14), LW(2))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_anim, STRING("me"), STRING("A_1"))
    CALL(evt_snd_sfxon_3d, STRING("SFX_KUPPA_GESSO_MOVE1"), LW(10), LW(11), LW(12), 0)
    CALL(evt_npc_set_unitwork, STRING("me"), 0, 0)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_npc_glide_position, STRING("me"), LW(10), LW(11), LW(12), 2000, FLOAT(50.0f), 10, 4, 0)
        CALL(evt_npc_set_unitwork, STRING("me"), 0, 1)
    END_SIBLING_EVENT
    LOOP(0)
        CALL(evt_npc_get_unitwork, STRING("me"), 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        CALL(upperCheck, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            DELETE_EVENT(LW(15))
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_anim, STRING("me"), STRING("S_1"))
    GOTO(1)
    RETURN
EVT_END()

