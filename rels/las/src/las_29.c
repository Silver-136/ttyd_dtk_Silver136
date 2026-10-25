#include "eff/eff_starstone.h"
#include "eff/n64/eff_pokopi_pcharge_n64.h"
#include "eff/n64/eff_radiation_n64.h"
#include "eff/n64/eff_pokopi_thunder_n64.h"
#include "eff/eff_mist2.h"
#include "eff/eff_fire.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "mario/mario_pouch.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_env.h"
#include "evt/evt_img.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "eff/eff_mist.h"
#include "driver/mobjdrv.h"

#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "stdio.h"
#include "driver/npcdrv.h"
#include "driver/camdrv.h"
#include "eff/eff_ripple.h"
#include "party.h"
#include "driver/mapdrv.h"
#include "dolphin/gx.h"
#include "pmario_sound.h"
#include "evt/evt_bero.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_sub.h"
#include "las.h"
#include "mario/mario_party.h"

void _hikariEntry(const char* name, s32 type, f32 scale);
typedef struct HikariData { s32 key; s32 id; } HikariData;
typedef struct HikariList40 { HikariData entries[40]; s32 endKey; } HikariList40;
typedef struct HikariList27 { HikariData entries[27]; s32 endKey; } HikariList27;
#define HIKARI_DATA ((HikariData*)&hikari_data)

USER_FUNC(add_vy);
USER_FUNC(all_hikari_change_kuro);
USER_FUNC(all_hikari_onoff);
USER_FUNC(b_peach_thunder);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_shuryolight_delete);
USER_FUNC(evt_shuryolight_init);
USER_FUNC(evt_shuryolight_run_evt);
USER_FUNC(hikari_change);
USER_FUNC(hikari_onoff);
USER_FUNC(hikari_se);
USER_FUNC(hikari_set_scale);
USER_FUNC(make_koe_label);
USER_FUNC(make_koe_pos);
USER_FUNC(pcharge_entry);
USER_FUNC(pcharge_set_pos);
USER_FUNC(radiation_entry);
USER_FUNC(rou_hikari_front_onoff);
static USER_FUNC(rousoku_init);
USER_FUNC(slowturnoff_check);
static USER_FUNC(starstone_entry);
static USER_FUNC(starstone_set_pos);
static USER_FUNC(starstone_set_ry);
USER_FUNC(mist_add_vy);
USER_FUNC(make_mist_name);
USER_FUNC(eff_b_peach_mist);
USER_FUNC(eff_b_peach_ripple);
USER_FUNC(radiation_set_pos);
void b_peach_msgcb(s32 type, void* param);
void map_color_tev(MapTevCallbackInfo* info);

static EvtWord w_door_close[22];
static EvtWord w_door_open[22];
static BeroEntry bero_entry_data[2];
static EvtWord syuryo_init[16];
EvtWord q_kage_init[10];
static NpcSetupInfo npcEnt[8];
HikariList40 hikari_data;
HikariList27 hikari_data_2;
EvtWord hikari_off_evt[42];
EvtWord hikari_on_evt[63];
EvtWord flush_on[106];
EvtWord flush_off[106];
EvtWord last_evt_1[1078];
#ifdef VERSION_G8ME01
EvtWord last_evt_2[358];
#else
EvtWord last_evt_2[383];
#endif
#ifndef VERSION_G8ME01
EvtWord nise_mario_init[29];
#endif
#ifdef VERSION_G8ME01
EvtWord last_evt_3[1349];
#else
EvtWord last_evt_3[1379];
#endif
EvtWord b_peach_fuwafuwa[39];
#ifdef VERSION_G8ME01
EvtWord last_evt_3_1[524];
#else
EvtWord last_evt_3_1[540];
#endif
#ifdef VERSION_G8ME01
EvtWord koe_evt[78];
#else
EvtWord koe_evt[65];
#endif
EvtWord majo_kyorokyoro[84];
#ifdef VERSION_G8ME01
EvtWord last_evt_3_2[1837];
#else
EvtWord last_evt_3_2[1821];
#endif
EvtWord last_evt_4[1836];
static EvtWord shuryolight_init[14];
EvtWord shuryolight_delete[20];
#ifdef VERSION_G8ME01
EvtWord las_29_init_evt[180];
#else
EvtWord las_29_init_evt[182];
#endif

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1_2"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1_1"), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 0, STRING("door_1_2"), 0, LW(0), 0)
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
        "las_30", // nextMapName
        "e_bero", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(syuryo_init)
    CALL(evt_npc_set_autotalkpose, STRING("me"), STRING("S_3"), STRING("T_4"))
    BEGIN_INLINE_EVENT
        CALL(evt_seq_wait, 2)
        CALL(evt_npc_set_anim, STRING("me"), STRING("S_3"))
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(q_kage_init)
    CALL(evt_npc_pera_onoff, STRING("me"), 0)
    CALL(evt_npc_set_ry, STRING("me"), 90)
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"シュリョー"*/"\x83\x56\x83\x85\x83\x8A\x83\x87\x81\x5B", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM, // flags
        0, // reactionFlags
        syuryo_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50", // name
        NPC_FLAG_TALK_POSE, // flags
        0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, // reactionFlags..returnEvent
        NULL, 0, // blowEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B", // name
        0, 0, // flags..reactionFlags
        npc_init_evt, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4", // name
        NPC_FLAG_TALK_POSE, // flags
        0, // reactionFlags
        q_kage_init, // initEvent
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, // regularEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

HikariList40 hikari_data = {
    {
        {324, 38},
        {294, 20},
        {272, 19},
        {271, 37},
        {246, 36},
        {245, 64},
        {215, 18},
        {210, 35},
        {208, 34},
        {195, 17},
        {188, 33},
        {174, 32},
        {161, 16},
        {148, 66},
        {146, 15},
        {143, 31},
        {110, 14},
        {94, 30},
        {53, 29},
        {48, 13},
        {44, 28},
        {18, 12},
        {-9, 27},
        {-26, 11},
        {-40, 26},
        {-51, 10},
        {-51, 9},
        {-85, 8},
        {-87, 7},
        {-100, 25},
        {-122, 24},
        {-143, 6},
        {-154, 5},
        {-157, 4},
        {-197, 23},
        {-204, 3},
        {-239, 22},
        {-248, 2},
        {-281, 21},
        {-294, 1},
    },
    -999,
};

HikariList27 hikari_data_2 = {
    {
        {325, 49},
        {295, 41},
        {273, 40},
        {247, 48},
        {245, 63},
        {215, 39},
        {195, 60},
        {188, 47},
        {162, 59},
        {150, 65},
        {150, 66},
        {111, 58},
        {95, 46},
        {48, 57},
        {45, 45},
        {18, 56},
        {-26, 55},
        {-40, 44},
        {-51, 54},
        {-87, 53},
        {-100, 43},
        {-143, 52},
        {-155, 51},
        {-205, 50},
        {-239, 42},
        {-248, 62},
        {-295, 61},
    },
    -999,
};

EVT_BEGIN(hikari_off_evt)
    SET_VALUE(LW(10), LW(0))
    SET_VALUE(LW(9), LW(1))
    CALL(hikari_se, LW(10), 1, LW(9))
    CALL(evt_sub_intpl_msec_init, 0, 500, 0, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(hikari_set_scale, LW(10), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(hikari_change, LW(10), LW(9))
    RETURN
EVT_END()

EVT_BEGIN(hikari_on_evt)
    SET_VALUE(LW(10), LW(0))
    SET_VALUE(LW(9), LW(1))
    CALL(hikari_se, LW(10), 1, LW(9))
    CALL(evt_sub_intpl_msec_init, 0, 0, 650, 150)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(hikari_set_scale, LW(10), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_sub_intpl_msec_init, 4, 650, 500, 100)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(hikari_set_scale, LW(10), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

void _hikariEntry(const char* name, s32 type, f32 scale) {
    Vec pos;
    EffectEntry* eff;

    mapObjGetPos(name, &pos);
    eff = effFireEntry(type, 1000, pos.x, pos.y - 7.0f, pos.z, 1.0f);
    ((EffFireWork*)eff->userdata)->scaleX = scale;
    ((EffFireWork*)eff->userdata)->scaleY = scale;
    effSetName(eff, name);
}

static USER_FUNC(rousoku_init) {
    char name[8];
    s32 val = evtGetValue(event, event->args[0]);
    s32 type;
    s32 i;

    type = 3;
    if (val) {
        type = 8;
    }
    for (i = 1; i <= 66; i++) {
        sprintf(name, "hi%d", i);
        mapGrpFlagOn(name, 1);
        _hikariEntry(name, type, 0.5f);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(rou_hikari_front_onoff) {
    char name[8];
    EvtWord* args = event->args;
    s32 on = evtGetValue(event, args[0]);
    void (*func)(const char*, u32);
    f32 scale;
    s32 i;
    EffectEntry* eff;

    evtGetValue(event, args[1]);
    func = on ? mapGrpFlagOn : mapGrpFlagOff;
    scale = on ? 0.0f : 0.5f;
    for (i = 39; i <= 62; i++) {
        sprintf(name, "S_rou%02d", i);
        func(name, 1);
        sprintf(name, "hi%d", i);
        eff = effNameToPtr(name);
        ((EffFireWork*)eff->userdata)->scaleX = scale;
        ((EffFireWork*)eff->userdata)->scaleY = scale;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(hikari_set_scale) {
    char name[8];
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    f32 scale = evtGetFloat(event, args[1]);
    EffectEntry* eff;

    sprintf(name, "hi%d", n);
    eff = effNameToPtr(name);
    ((EffFireWork*)eff->userdata)->scaleX = scale;
    ((EffFireWork*)eff->userdata)->scaleY = scale;
    return EVT_RETURN_DONE;
}

USER_FUNC(hikari_change) {
    char name[8];
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    s32 type = evtGetValue(event, args[1]) ? 3 : 8;

    sprintf(name, "hi%d", n);
    effDelete(effNameToPtr(name));
    _hikariEntry(name, type, 0.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(hikari_se) {
    char name[8];
    Vec pos;
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    s32 off = evtGetValue(event, args[1]);
    s32 dark = evtGetValue(event, args[2]);

    sprintf(name, "hi%d", n);
    mapObjGetPos(name, &pos);
    if (!off) {
        if (!dark) {
            psndSFXOn_3D("SFX_STG8_CANDLE_ON1", &pos);
        } else {
            psndSFXOn_3D("SFX_STG8_CANDLE_DARK_ON1", &pos);
        }
    } else {
        if (!dark) {
            psndSFXOn_3D("SFX_STG8_CANDLE_OFF1", &pos);
        } else {
            psndSFXOn_3D("SFX_STG8_CANDLE_DARK_OFF1", &pos);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(hikari_onoff) {
    EvtWord* args = event->args;
    HikariData* tbl = (HikariData*)args[0];
    s32 val = evtGetValue(event, args[1]);
    s32 off = evtGetValue(event, args[2]);
    s32 start = evtGetValue(event, args[3]);
    s32 limit = evtGetValue(event, args[4]);
    s32 key = tbl[start].key;
    EvtWord* evt = off ? hikari_off_evt : hikari_on_evt;
    EventEntry* e;
    s32 i;

    if (key == -999) {
        evtSetValue(event, args[5], -1);
        return EVT_RETURN_DONE;
    }
    i = start;
    while (key > limit && key != -999) {
        e = evtEntry(evt, 0, 0);
        e->lwData[0] = tbl[i].id;
        e->lwData[1] = val;
        i++;
        key = tbl[i].key;
    }
    evtSetValue(event, args[5], i);
    return EVT_RETURN_DONE;
}

USER_FUNC(all_hikari_change_kuro) {
    char name[8];
    s32 idx = evtGetValue(event, event->args[0]);
    s32 i;
    s32 key;

    if (idx != -1) {
        key = HIKARI_DATA[idx].key;
        while (key != -999) {
            sprintf(name, "hi%d", HIKARI_DATA[idx].id);
            effDelete(effNameToPtr(name));
            _hikariEntry(name, 8, 0.5f);
            idx++;
            key = HIKARI_DATA[idx].key;
        }
    }
    for (i = 39; i <= 66; i++) {
        if (i == 64 || i == 66) {
            continue;
        }
        sprintf(name, "hi%d", i);
        effDelete(effNameToPtr(name));
        _hikariEntry(name, 8, 0.5f);
    }
    return EVT_RETURN_DONE;
}

void map_color_tev(MapTevCallbackInfo* info) {
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    if (evtGetValue(NULL, LSWF(10))) {
        GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};

        GXSetTevColor(GX_TEVREG0, color);
    } else {
        GXColor color = {0x00, 0x00, 0x00, 0xFF};

        GXSetTevColor(GX_TEVREG0, color);
    }
    info->tevStage = 1;
    info->texCoord = 0;
    info->texMap = 0;
}

USER_FUNC(all_hikari_onoff) {
    char name[16];
    f32 scale = evtGetValue(event, event->args[0]) ? 0.0f : 0.5f;
    s32 i;
    EffectEntry* eff;

    for (i = 1; i <= 66; i++) {
        sprintf(name, "hi%d", i);
        eff = effNameToPtr(name);
        ((EffFireWork*)eff->userdata)->scaleX = scale;
        ((EffFireWork*)eff->userdata)->scaleY = scale;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(b_peach_thunder) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    EffectEntry* eff = effPokopiThunderN64Entry(0, 150, 35.0f + x, 160.0f + y, z, x, y, z, 2.0f);

    ((EffPokopiThunderN64Work*)eff->userdata)->r = 255;
    ((EffPokopiThunderN64Work*)eff->userdata)->g = 160;
    ((EffPokopiThunderN64Work*)eff->userdata)->b = 192;
    ((EffPokopiThunderN64Work*)eff->userdata)->r2 = 20;
    ((EffPokopiThunderN64Work*)eff->userdata)->g2 = 20;
    ((EffPokopiThunderN64Work*)eff->userdata)->b2 = 20;
    return EVT_RETURN_DONE;
}

void b_peach_msgcb(s32 type, void* param) {
    switch (type) {
        case 0:
        case 3:
            npcSetTalkPose("\211e\202\314\217\227\211\244");
            break;
        case 1:
        case 2:
            npcSetStayPose("\211e\202\314\217\227\211\244");
            break;
    }
}

USER_FUNC(add_vy) {
    EvtWord* args = event->args;
    s32 dir = evtGetValue(event, args[0]);
    s32 n = evtGetValue(event, args[1]);
    f32 base;
    f32 step;
    NpcEntry* npc;

    base = dir ? -0.3f : 0.3f;
    step = dir ? 0.01f : -0.01f;
    npc = npcNameToPtr("\203u\203\211\203b\203N\203s\201[\203`");
    base += step * n;
    npc->moveOffset.y += base;
    return EVT_RETURN_DONE;
}

USER_FUNC(radiation_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    f32 scale = evtGetFloat(event, args[4]);

    effSetName(effRadiationN64Entry(2, x, y, z, scale), name);
    return EVT_RETURN_DONE;
}

USER_FUNC(radiation_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffRadiationN64Work*)eff->userdata)->position.x = x;
    ((EffRadiationN64Work*)eff->userdata)->position.y = y;
    ((EffRadiationN64Work*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(pcharge_entry) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effSetName(effPokopiPchargeN64Entry(8, 0, x, y, z, 1.0f), name);
    return EVT_RETURN_DONE;
}

USER_FUNC(pcharge_set_pos) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    EffectEntry* eff = effNameToPtr(name);

    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.x = x;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.y = y;
    ((EffPokopiPchargeN64Elem*)eff->userdata)->position.z = z;
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_entry) {
    char name[16];
    EvtWord* args = event->args;
    f32 x;
    f32 z;
    f32 r;
    f32 c;
    f32 angle;
    f32 y;
    f32 s;
    EffectEntry* eff;
    s32 i;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    r = evtGetFloat(event, args[3]);
    for (i = 0; i < 7; i++) {
        angle = 6.2831855f * i / 7.0f;
        c = cos(angle);
        s = sin(angle);
        sprintf(name, "starstone%d", i);
        eff = effStarStoneEntry(i, r * c + x, y, r * s + z, 1.0f);
        effSetName(eff, name);
        ((EffStarStoneWork*)eff->userdata)->flags &= ~4;
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_pos) {
    char name[16];
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);
    f32 phase = evtGetFloat(event, args[3]);
    f32 r = evtGetFloat(event, args[4]);
    f32 c;
    f32 angle;
    f32 s;
    EffectEntry* eff;
    s32 i;

    for (i = 0; i < 7; i++) {
        angle = 6.2831855f * i / 7.0f + phase;
        if (angle >= 6.2831855f) {
            angle -= 6.2831855f;
        }
        c = cos(angle);
        s = sin(angle);
        sprintf(name, "starstone%d", i);
        eff = effNameToPtr(name);
        ((EffStarStoneWork*)eff->userdata)->position.x = r * c + x;
        ((EffStarStoneWork*)eff->userdata)->position.y = y;
        ((EffStarStoneWork*)eff->userdata)->position.z = r * s + z;
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(starstone_set_ry) {
    char name[16];
    f32 ry = evtGetFloat(event, event->args[0]);
    s32 i;

    for (i = 0; i < 7; i++) {
        sprintf(name, "starstone%d", i);
        ((EffStarStoneWork*)effNameToPtr(name)->userdata)->rot.y = ry;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(make_koe_label) {
    static char name[32];
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);

    if (n == 18 && !partyChkJoin(PARTNER_MSMOWZ)) {
        n = 1;
        evtSetValue(event, args[0], 1);
    }
    sprintf(name, "stg8_las_148_74_%02d", n + 1);
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}

USER_FUNC(make_koe_pos) {
    EvtWord* args = event->args;
    s32 a = ((rand() >> 1) % 41) * 10;
    s32 b = ((rand() >> 1) % 33) * 10;
    s32 x = a - 200;
    s32 y = b - 190;

    if (x > 0) {
        x += rand() % 10;
    } else {
        x -= rand() % 10;
    }
    if (y > 0) {
        y += rand() % 10;
    } else {
        y += rand() % 10;
    }
    evtSetValue(event, args[0], x);
    evtSetValue(event, args[1], y);
    evtSetValue(event, args[2], 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(slowturnoff_check) {
    return (npcNameToPtr((const char*)evtGetValue(event, event->args[0]))->flags & 0x8000) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(eff_b_peach_ripple) {
    NpcEntry* npc = npcNameToPtr("\203u\203\211\203b\203N\203s\201[\203`");

    effRippleSetRxRz(effRippleEntry(3, npc->position.x, 30.0f + npc->position.y, npc->position.z), 0.0f, 176.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_b_peach_mist) {
    NpcEntry* npc = npcNameToPtr("\203s\201[\203`\225P");
    f32 s;
    f32 angle;
    f32 c;
    f32 dx;
    f32 dz;

    s = 180.0f + camGetPtr(CAMERA_3D)->viewYaw;
    angle = 6.2831855f * s / 360.0f;
    s = sin(angle);
    c = cos(angle);
    dx = 20.0f * s;
    dz = 20.0f * -c;

    effMistEntry(6, 300, npc->position.x + dx, 30.0f + npc->position.y, npc->position.z + dz, 0.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(make_mist_name) {
    static char name[8];
    EvtWord* args = event->args;

    sprintf(name, "mist%d", evtGetValue(event, args[0]));
    evtSetValue(event, args[1], (EvtWord)name);
    return EVT_RETURN_DONE;
}

USER_FUNC(mist_add_vy) {
    char name[16];
    f32 vy = evtGetFloat(event, event->args[0]);
    s32 i;

    for (i = 0; i < 10; i++) {
        sprintf(name, "mist%d", i);
        ((EffMist2Work*)effNameToPtr(name)->userdata)->position.y += vy;
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(flush_on)
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), NPC_FLAG_NO_DRAW)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_hakahuta"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabe"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kuro"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ten"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S11_archi1"), MAP_OBJ_TEV_CALLBACK(1))
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hakahuta"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kabe"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kuro"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ten"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S11_archi1"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_hakahuta"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabe"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kuro"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ten"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S11_archi1"), MAP_OBJ_FLAG_NO_FOG)
    CALL(all_hikari_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(flush_off)
    CALL(evt_npc_flag_onoff, 0, STRING("シュリョー"), NPC_FLAG_NO_DRAW)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_rou"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hakahuta"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kabe"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kuro"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ten"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S11_archi1"), MAP_OBJ_TEV_CALLBACK_MASK)
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_hakahuta"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabe"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kuro"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_ten"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S11_archi1"), MAP_OBJ_FLAG_DISPLAY_LIST)
    CALL(evt_mapobj_flag_onoff, 0, 0, STRING("S_hakahuta"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kabe"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kuro"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_ten"), MAP_OBJ_FLAG_NO_FOG)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S11_archi1"), MAP_OBJ_FLAG_NO_FOG)
    CALL(all_hikari_onoff, 0)
    RETURN
EVT_END()

EVT_BEGIN(last_evt_1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), 135, 15, 0)
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 270)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_6F"))
    CALL(evt_npc_set_position, STRING("シュリョー"), 35, 0, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_mario_set_pos, -420, 0, 0)
    CALL(evt_party_set_pos, 0, -460, 0, 20)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_seq_wait, 2)
    WAIT_MS(300)
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_open))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -350, 20, FLOAT(150.0f))
    END_INLINE_EVENT
    CALL(evt_mario_mov_pos2, -310, 0, FLOAT(150.0f))
    SET_VALUE(LW(1), 0)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL_CHILD(PTR(w_door_close))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_snd_bgmoff_f, 512, 3000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_6F"), STRING("P_T_6F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_108"), 0, STRING("ピーチ姫"))
    CALL(evt_cam3d_evt_set, -205, 145, 350, 82, 65, 14, 1000, 11)
    WAIT_MS(1000)
    CALL(rou_hikari_front_onoff, 0, 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_109"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -330, 100, 332, -330, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_110"))
    CALL(evt_npc_pera_onoff, STRING("シュリョー"), 0)
    CALL(evt_npc_pera_onoff, STRING("ピーチ姫"), 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 90)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -372, 200, 0, 15, 100, 0, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_111"), 0, STRING("シュリョー"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -330, 80, 332, -330, 37, -13, 0, 11)
    CALL(evt_npc_pera_onoff, STRING("シュリョー"), 1)
    CALL(evt_npc_pera_onoff, STRING("ピーチ姫"), 1)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 270)
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 270)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_112"))
    CALL(evt_cam3d_evt_set, 20, 100, 300, 115, 60, 20, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_113"), 0, STRING("ピーチ姫"))
    CALL(evt_cam3d_evt_set, -63, 100, 273, 32, 60, -7, 800, 11)
    WAIT_MS(800)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_114"), 0, STRING("シュリョー"))
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    WAIT_MS(300)
    CALL(evt_snd_bgmon, 512, STRING("BGM_FF_ENV_WITCH_APPEAR1"))
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3A"))
    WAIT_MS(170)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3B"))
    WAIT_MS(250)
    CALL(evt_npc_get_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_SHURYO_STUFF_PISECE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("A_3C"))
    WAIT_MS(400)
    BEGIN_INLINE_EVENT
        WAIT_MS(3000)
        CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("S_3"))
    END_INLINE_EVENT
    LOOP(2)
        CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
        CALL(evt_fade_entry, 17, 1, 255, 255, 255)
        WAIT_MS(100)
        CALL(evt_fade_entry, 16, 1, 255, 255, 255)
        WAIT_MS(100)
    END_LOOP
    SET_VALUE(LW(0), 0)
    SET_FLOAT_VALUE(LW(1), FLOAT(350.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(255.0f))
    LOOP(0)
        IF_INT_EQUAL(LW(0), -1)
            BREAK_LOOP
        END_IF
        CALL(hikari_onoff, PTR(&hikari_data), 0, 1, LW(0), LW(1), LW(0))
        SUBTRACT_FLOAT(LW(1), FLOAT(3.2998047f))
        CALL(evt_mapobj_color, 1, STRING("S_yukahikari02"), 255, 255, 255, LW(2))
        SUBTRACT_FLOAT(LW(2), FLOAT(2.4033203f))
        IF_INT_LESS_OR_EQUAL(LW(2), 0)
            SET_FLOAT_VALUE(LW(2), 0)
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(all_hikari_change_kuro, LW(0))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hikari"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(2000)
    SET_VALUE(LW(0), 0)
    SET_FLOAT_VALUE(LW(1), FLOAT(350.0f))
    SET_VALUE(LW(2), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(0), -1)
            BREAK_LOOP
        END_IF
        CALL(hikari_onoff, PTR(&hikari_data), 1, 0, LW(0), LW(1), LW(0))
        SUBTRACT(LW(1), FLOAT(3.2998047f))
        ADD(LW(2), 1)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_cam3d_evt_set, 52, 143, 432, 183, 87, 44, 800, 11)
    WAIT_MS(1600)
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_WITCH"))
    CALL(evt_mapobj_get_position, STRING("S_o_hikari1"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GRAVE_LIGHT1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 0, 1000, 10000, 250)
    LABEL(3)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    CALL(evt_mapobj_scale, 0, STRING("S_o_hikari1"), LW(0), 1, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    CALL(evt_fade_entry, 17, 1, 255, 255, 255)
    WAIT_MS(100)
    CALL(evt_fade_entry, 16, 1, 255, 255, 255)
    CALL(evt_sub_intpl_msec_init, 0, 255, 0, 200)
    LABEL(4)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_color, 1, STRING("S_o_hikari1"), 255, 255, 255, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    CALL(evt_mapobj_flag_onoff, 0, 1, STRING("S_o_hikari1"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_get_position, STRING("S_hakahuta"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GRAVE_SHAKE1"), LW(0), LW(1), LW(2), LW(3))
    LOOP(90)
        CALL(evt_sub_random, 2, LW(0))
        CALL(evt_sub_random, 2, LW(1))
        SUBTRACT(LW(0), 1)
        SUBTRACT(LW(1), 1)
        CALL(evt_mapobj_trans, 0, STRING("S_hakahuta"), LW(0), 0, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(3))
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    SET_VALUE(GF(0), 0)
    SET_VALUE(GF(1), 0)
    SET_VALUE(GF(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 1, 0, 30, 1650)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_sub_intpl_msec_init, 0, 30, 55, 550)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_sub_intpl_msec_init, 0, 55, 60, 100)
        LABEL(3)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(0), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(3)
        END_IF
        SET_VALUE(GF(0), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1550)
        CALL(evt_sub_intpl_msec_init, 1, 0, -20, 600)
        LABEL(9)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(9)
        END_IF
        CALL(evt_sub_intpl_msec_init, 0, -20, -40, 150)
        LABEL(10)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(1), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(10)
        END_IF
        SET_VALUE(GF(1), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1450)
        CALL(evt_sub_intpl_msec_init, 1, 0, -45, 600)
        LABEL(20)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(2), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(20)
        END_IF
        WAIT_MS(150)
        CALL(evt_sub_intpl_msec_init, 0, -45, 0, 150)
        LABEL(21)
        CALL(evt_sub_intpl_msec_get_value)
        SET_VALUE(GW(2), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(21)
        END_IF
        SET_VALUE(GF(2), 1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("S_hakahuta"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GRAVE_MOVE1"), LW(0), LW(1), LW(2), LW(3))
        WAIT_MS(1500)
        CALL(evt_snd_sfxoff, LW(3))
        WAIT_MS(800)
        CALL(evt_mapobj_get_position, STRING("S_hakahuta"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GRAVE_DOWN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    LOOP(0)
        CALL(evt_mapobj_trans, 0, STRING("S_hakahuta"), GW(0), GW(1), 0)
        CALL(evt_mapobj_rotate, 0, STRING("S_hakahuta"), 0, 0, GW(2))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(GF(0), 1)
            IF_INT_EQUAL(GF(1), 1)
                IF_INT_EQUAL(GF(2), 1)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 300)
    END_INLINE_EVENT
    CALL(evt_mapobj_get_position, STRING("S_hakahuta"), LW(0), LW(1), LW(2))
    ADD(LW(2), 50)
    CALL(evt_eff64, 0, STRING("kemuri1_n64"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_mapobj_get_position, STRING("S_haka"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GRAVE_LIGHT2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_map_playanim, STRING("hihi"), 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_map_checkanim, STRING("hihi"), LW(0), LW(1))
        SUBTRACT(LW(1), 1000)
        WAIT_MS(LW(1))
        CALL(evt_sub_intpl_msec_init, 11, 255, 0, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_mapobj_color, 0, STRING("S_o_hika"), 255, 255, 255, LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(800)
    CALL(evt_mapobj_get_position, STRING("S_haka"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("uzu"), STRING("queen"), 0, LW(0), 0, LW(2), 1, 1000, 0, 0, 0, 0, 0, 0)
    WAIT_MS(500)
    CALL(evt_npc_set_position, STRING("影の女王"), 200, 15, 0)
    CALL(evt_npc_set_ry, STRING("影の女王"), 90)
    CALL(evt_npc_set_anim, STRING("影の女王"), STRING("KRT_O_1"))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_wait_anim, STRING("影の女王"), FLOAT(0.265625f))
        CALL(evt_cam3d_evt_set, -39, 225, 701, 183, 130, 44, 8000, 11)
    END_INLINE_EVENT
    CALL(evt_npc_wait_anim, STRING("影の女王"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING("影の女王"), STRING("KRT_S_1"))
    CALL(evt_eff_softdelete, STRING("uzu"))
    WAIT_MS(2000)
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("gor_01"), STRING("sekai_yami1"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(last_evt_2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -310, 0, 0)
    CALL(evt_party_set_pos, 0, -350, 0, 20)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), 135, 15, 0)
    CALL(evt_npc_set_ry, STRING("ピーチ姫"), 270)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_6F"))
    CALL(evt_npc_set_position, STRING("シュリョー"), 35, 0, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_position, STRING("影の女王"), 200, 15, 0)
    CALL(evt_npc_set_ry, STRING("影の女王"), 90)
    CALL(evt_mapobj_trans, 0, STRING("S_hakahuta"), 60, -40, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_o_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -283, 0, 472, 200, 150, -12, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_seq_wait, 2)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg8_las_115"), 0, STRING("影の女王"))
    CALL(evt_cam3d_evt_set, -220, 20, 269, 130, 90, -82, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("stg8_las_116"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -80, 125, 268, 200, 180, -12, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_117"), 0, STRING("影の女王"))
    CALL(evt_npc_flag_onoff, 1, STRING("影の女王"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 410, 375, 366, 125, 100, -15, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_118"), 0, STRING("シュリョー"))
    CALL(evt_npc_flag_onoff, 0, STRING("影の女王"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 30, 90, 305, 125, 50, 24, 2000, 11)
    WAIT_MS(3500)
    CALL(evt_npc_flag_onoff, 1, STRING("影の女王"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -80, 125, 268, 200, 180, -12, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_119"), 0, STRING("影の女王"))
    CALL(evt_cam3d_evt_set, -140, 100, 284, 178, 75, -34, 4000, 11)
    CALL(evt_npc_get_position, STRING("影の女王"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_MOVE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_set_anim, STRING("影の女王"), STRING("KRT_A_1"))
    CALL(evt_npc_wait_anim, STRING("影の女王"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_D_1F"))
    CALL(evt_npc_get_position, STRING("影の女王"), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("uzu"), STRING("queen"), 0, LW(0), 0, LW(2), 1, 1000, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_MOVE2"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(2000)
    CALL(evt_eff_delete, STRING("uzu"))
    CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
    SET_VALUE(LSWF(10), 1)
    CALL(evt_map_set_blend, 0, 0, 0, 0, 255)
    CALL(evt_map_set_tevcallback, 1, PTR(map_color_tev))
    CALL_CHILD(PTR(flush_on))
    WAIT_MS(500)
#ifndef VERSION_G8ME01
    LOOP(2)
        CALL(evt_map_blend_off, 0)
        CALL_CHILD(PTR(flush_off))
        WAIT_MS(100)
        CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
        CALL(evt_map_set_blend, 0, 0, 0, 0, 255)
        CALL_CHILD(PTR(flush_on))
        WAIT_MS(100)
    END_LOOP
#endif
    CALL(evt_fade_set_mapchange_type, 1, 17, 300, 9, 300)
    CALL(evt_bero_mapchange, STRING("usu_00"), STRING("sekai_yami2"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

#ifndef VERSION_G8ME01  // JP-only script (US dropped it)
EVT_BEGIN(nise_mario_init)
    CALL(evt_npc_entry, STRING("にせマリオ"), STRING("c_mario"))
    CALL(evt_npc_set_tribe, STRING("にせマリオ"), STRING("にせマリオ"))
    CALL(evt_npc_set_position, STRING("にせマリオ"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("にせマリオ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_ry, STRING("にせマリオ"), 90)
    CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_S_1"))
    RETURN
EVT_END()
#endif

EVT_BEGIN(last_evt_3)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, -310, 0, 0)
    CALL(evt_party_set_pos, 0, -350, 0, 20)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_npc_set_position, STRING("シュリョー"), 35, 0, 0)
    CALL(evt_npc_set_ry, STRING("シュリョー"), 90)
    CALL(evt_npc_set_position, STRING("ブラックピーチ"), 135, 15, 0)
    CALL(evt_npc_set_ry, STRING("ブラックピーチ"), 270)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_NO_CULL)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_NO_CULL)
    CALL(evt_mapobj_trans, 0, STRING("S_hakahuta"), 60, -40, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_o_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, 33, 47, 226, 133, 57, -2, 0, 11)
    SET_VALUE(LSWF(10), 1)
    CALL(evt_map_set_tevcallback, 1, PTR(map_color_tev))
    CALL(evt_map_set_blend, 0, 0, 0, 0, 255)
    CALL_CHILD(PTR(flush_on))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_seq_wait, 2)
    WAIT_MS(1000)
    SET_VALUE(LSWF(10), 0)
#ifdef VERSION_G8ME01
    WAIT_MS(300)
#else
    WAIT_MS(100)
    CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
    SET_VALUE(LSWF(10), 1)
    WAIT_MS(100)
    SET_VALUE(LSWF(10), 0)
    WAIT_MS(100)
#endif
    CALL(evt_snd_sfxon, STRING("SFX_STG8_LIGHT1"), 0)
    CALL(evt_fade_entry, 17, 1, 255, 255, 255)
    CALL(evt_map_blend_off, 0)
    CALL_CHILD(PTR(flush_off))
#ifdef VERSION_G8ME01
    WAIT_MS(200)
#else
    WAIT_MS(100)
#endif
    CALL(evt_fade_entry, 16, 2000, 255, 255, 255)
    CALL(evt_fade_end_wait)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_L_2"))
    WAIT_MS(200)
    CALL(evt_npc_set_autotalkpose, STRING("ブラックピーチ"), STRING("KRT_L_3"), STRING("KRT_T_3"))
    CALL(evt_msg_print, 0, STRING("stg8_las_120"), 0, STRING("ブラックピーチ"))
    CALL(evt_cam3d_evt_set, -330, 100, 332, -330, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_120"))
    CALL(evt_cam3d_evt_set, -16, 60, 400, 70, 60, 9, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_121"), 0, STRING("シュリョー"))
    CALL(evt_cam3d_evt_set, -73, 60, 284, 160, 60, -107, 800, 11)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_S_1"))
    WAIT_MS(1300)
    CALL(evt_npc_set_autotalkpose, STRING("ブラックピーチ"), STRING("KRT_S_1"), STRING("KRT_T_1"))
    CALL(evt_msg_print, 0, STRING("stg8_las_122"), 0, STRING("ブラックピーチ"))
    CALL(rou_hikari_front_onoff, 1, 1)
    CALL(evt_npc_flag_onoff, 1, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 80, 155, 480, 80, 65, -11, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("シュリョー"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg8_las_123"), 0, STRING("シュリョー"))
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_A_1"))
    WAIT_MS(550)
    CALL(evt_npc_get_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
    CALL(b_peach_thunder, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_THUNDER1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_zoom_in, 15, 100, 0, 11)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, 80, 155, 480, 80, 65, -11, 100, 11)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        ADD(LW(2), 5)
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 30)
        SET_VALUE(LW(4), LW(1))
        ADD(LW(4), 50)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(3), LW(4), LW(2), 1, 5, 0, 0, 0, 0, 0, 0)
        WAIT_MS(100)
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 10)
        SET_VALUE(LW(4), LW(1))
        ADD(LW(4), 25)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(3), LW(4), LW(2), 1, 5, 0, 0, 0, 0, 0, 0)
        WAIT_MS(100)
        SET_VALUE(LW(3), LW(0))
        SUBTRACT(LW(3), 20)
        SET_VALUE(LW(4), LW(1))
        ADD(LW(4), 10)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(3), LW(4), LW(2), 1, 5, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(130)
    CALL(evt_npc_set_anim, STRING("シュリョー"), STRING("Q_2"))
    CALL(evt_npc_get_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
    ADD_FLOAT(LW(0), FLOAT(0.0f))
    ADD_FLOAT(LW(1), FLOAT(50.0f))
    CALL(evt_npc_set_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
    SET_FLOAT_VALUE(LW(3), FLOAT(-0.39941406f))
    SET_FLOAT_VALUE(LW(4), FLOAT(6.5f))
    SET_FLOAT_VALUE(LW(5), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(6), FLOAT(0.0f))
    SET_FLOAT_VALUE(LW(7), FLOAT(-0.2998047f))
    SET_FLOAT_VALUE(LW(8), FLOAT(0.0f))
    LOOP(0)
        CALL(evt_npc_set_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
        CALL(evt_npc_add_rotate, STRING("シュリョー"), 0, 0, FLOAT(2.0f))
        ADD_FLOAT(LW(0), LW(3))
        ADD_FLOAT(LW(1), LW(4))
        ADD_FLOAT(LW(2), LW(5))
        ADD_FLOAT(LW(3), LW(6))
        ADD_FLOAT(LW(4), LW(7))
        ADD_FLOAT(LW(5), LW(8))
        IF_INT_LESS_OR_EQUAL(LW(1), 16)
            SET_FLOAT_VALUE(LW(1), 16)
            MULTIPLY_FLOAT(LW(4), FLOAT(-0.5f))
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_SHURYO_HEAD_LEAPED1"), LW(0), LW(1), LW(2), 0)
            IF_INT_LESS(LW(4), 2)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_set_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
        ADD(LW(1), 10)
        ADD(LW(2), 10)
        LOOP(3)
            CALL(evt_sub_random, 20, LW(3))
            SUBTRACT(LW(3), 10)
            ADD(LW(3), LW(0))
            CALL(evt_eff64, 0, STRING("kemuri12_n64"), 0, LW(3), LW(1), LW(2), FLOAT(1.0f), 30, 0, 0, 0, 0, 0, 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_S_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_125"), 0, STRING("ブラックピーチ"))
    WAIT_MS(500)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_A_1"))
    WAIT_MS(550)
    CALL(evt_npc_get_position, STRING("シュリョー"), LW(0), LW(1), LW(2))
    CALL(b_peach_thunder, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_THUNDER1"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_cam3d_evt_zoom_in, 15, 100, 0, 11)
        WAIT_MS(100)
        CALL(evt_cam3d_evt_set, 80, 155, 480, 80, 65, -11, 100, 11)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        ADD(LW(2), 5)
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 30)
        SET_VALUE(LW(4), LW(1))
        ADD(LW(4), 15)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(3), LW(4), LW(2), 1, 5, 0, 0, 0, 0, 0, 0)
        WAIT_MS(100)
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), 5)
        SET_VALUE(LW(4), LW(1))
        ADD(LW(4), 30)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(3), LW(4), LW(2), 1, 5, 0, 0, 0, 0, 0, 0)
        WAIT_MS(100)
        SET_VALUE(LW(3), LW(0))
        SUBTRACT(LW(3), 25)
        SET_VALUE(LW(4), LW(1))
        ADD(LW(4), 10)
        CALL(evt_eff, 0, STRING("hit"), 2, LW(3), LW(4), LW(2), 1, 5, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    WAIT_MS(150)
    CALL(evt_npc_set_position, STRING("シュリョー"), 0, -1000, 0)
    WAIT_MS(500)
    CALL_CHILD(PTR(shuryolight_delete))
    CALL(evt_cam3d_evt_set, 130, 100, 235, 130, 57, -11, 800, 11)
    WAIT_MS(800)
    CALL(rou_hikari_front_onoff, 0, 1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_126"), 0, STRING("ブラックピーチ"))
    CALL(evt_cam3d_evt_set, -330, 84, 258, -330, 37, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_127"))
    WAIT_MS(1000)
    CALL(evt_npc_set_position, STRING("マジョリン"), -50, 0, 150)
    CALL(evt_msg_print, 0, STRING("stg8_las_128"), 0, STRING("マジョリン"))
    CALL(evt_npc_set_position, STRING("マジョリン"), 0, -1000, 0)
    CALL(rou_hikari_front_onoff, 1, 1)
    CALL(evt_cam3d_evt_set, -70, 96, 130, -70, 45, -165, 800, 11)
    WAIT_MS(1800)
    CALL(rou_hikari_front_onoff, 0, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_majo_disp_on, STRING("マジョリン"), -50, 0, -150, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_majo_disp_on, STRING("マリリン"), -110, 0, -170, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(evt_npc_set_position, STRING("ランペル"), -250, 0, -200)
        CALL(evt_npc_jump_position_nohit, STRING("ランペル"), -75, 0, -200, 1300, 100)
    END_INLINE_EVENT
    WAIT_MS(2800)
    CALL(evt_msg_print, 0, STRING("stg8_las_129"), 0, STRING("マジョリン"))
    CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_NO_CULL)
    CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_NO_CULL)
    CALL(evt_cam3d_evt_set, -70, 169, 499, 44, 60, -80, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_130"), 0, STRING("ブラックピーチ"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_131"), 0, STRING("マジョリン"))
    CALL(evt_npc_flag_onoff, 1, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 125, 100, 235, 125, 57, -11, 0, 11)
    WAIT_MS(50)
    CALL(evt_npc_flag_onoff, 0, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_L_2"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("ブラックピーチ"), STRING("KRT_L_3"), STRING("KRT_T_3"))
    CALL(evt_msg_print, 0, STRING("stg8_las_132"), 0, STRING("ブラックピーチ"))
    CALL(evt_cam3d_evt_set, -70, 96, 130, -70, 45, -165, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_133"), 0, STRING("マジョリン"))
    CALL(evt_cam3d_evt_set, 125, 100, 235, 125, 57, -11, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("ブラックピーチ"), STRING("KRT_S_1"), STRING("KRT_T_1"))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 10)
    CALL(evt_npc_move_position, STRING("ブラックピーチ"), LW(0), LW(2), 0, FLOAT(60.0f), 0)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_134"), 0, STRING("ブラックピーチ"))
    CALL(evt_cam3d_evt_set, -330, 78, 235, -330, 35, -11, 800, 11)
    WAIT_MS(800)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 10)
    CALL(evt_party_stop, 0)
    CALL(evt_party_dispflg_onoff, 0, 16, 1)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(60.0f))
    WAIT_FRAMES(1)
    CALL(evt_party_dispflg_onoff, 0, 16, 0)
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 1, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -50, 115, 260, 135, 55, -30, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_L_2"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("ブラックピーチ"), STRING("KRT_L_3"), STRING("KRT_T_3"))
    CALL(evt_msg_print, 0, STRING("stg8_las_135"), 0, STRING("ブラックピーチ"))
    CALL(evt_npc_flag_onoff, 0, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -223, 84, 250, -375, 15, -113, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_136"))
    CALL(evt_msg_select, 0, STRING("stg8_las_136_yesno1"))
    CALL(evt_msg_continue)
    CALL(evt_cam3d_evt_set, -50, 115, 260, 135, 55, -13, 0, 11)
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg8_las_137"), 0, STRING("ブラックピーチ"))
        CALL(evt_cam3d_evt_set, -330, 100, 332, -330, 40, -12, 800, 11)
        WAIT_MS(800)
        CALL(evt_fade_entry, 10, 0, 255, 255, 255)
        WAIT_MS(100)
#ifndef VERSION_G8ME01
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mario_set_pos, 0, -1000, 0)
        CALL(evt_npc_set_position, STRING("にせマリオ"), LW(0), LW(1), LW(2))
#endif
        CALL(evt_fade_entry, 10, 0, 0, 0, 0)
        WAIT_MS(100)
        CALL(evt_fade_entry, 9, 0, 0, 0, 0)
        WAIT_MS(100)
        WAIT_MS(500)
        CALL(evt_msg_print, 0, STRING("stg8_las_138"), 0, STRING("ブラックピーチ"))
#ifdef VERSION_G8ME01
        BEGIN_INLINE_EVENT
            WAIT_MS(500)
            CALL(all_hikari_onoff, 1)
        END_INLINE_EVENT
#endif
        CALL(evt_sub_intpl_msec_init, 4, 255, 0, 1000)
        LABEL(1)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(0), 255)
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(1)
        END_IF
        CALL(evt_msg_print, 0, STRING("stg8_las_139"), 0, 0)
        CALL(evt_seq_set_seq, 5, 0, 0)
    END_IF
    WAIT_MS(500)
#ifndef VERSION_G8ME01
    CALL(evt_npc_delete, STRING("にせマリオ"))
#endif
    CALL(evt_msg_print, 0, STRING("stg8_las_140"), 0, STRING("ブラックピーチ"))
    CALL(rou_hikari_front_onoff, 1, 1)
    CALL(evt_cam3d_evt_set, 125, 77, 237, 125, 57, -13, 800, 11)
    WAIT_MS(800)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_S_1"))
    CALL(evt_npc_set_autotalkpose, STRING("ブラックピーチ"), STRING("KRT_S_1"), STRING("KRT_T_1"))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_140_01"), 0, STRING("ブラックピーチ"))
    CALL(evt_mario_set_pos, -180, 0, 0)
    CALL(evt_party_set_pos, 0, -220, 0, 20)
    CALL(evt_cam3d_evt_set, 60, 150, 684, 60, 75, -11, 1500, 11)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 1500)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_move_pos2, 0, -40, 20, FLOAT(120.0f))
    END_INLINE_EVENT
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_mario_mov_pos2, 0, 0, FLOAT(120.0f))
    CALL(evt_npc_release_fieldnpc, 1)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_npc_set_battle_info, STRING("ブラックピーチ"), 16)
    CALL(evt_npc_battle_start, STRING("ブラックピーチ"))
    CALL(evt_npc_wait_battle_end)
    CALL(rou_hikari_front_onoff, 0, 1)
    CALL(evt_npc_get_battle_result, LW(0))
    IF_INT_EQUAL(LW(0), 5)
        SPAWN_EVENT(PTR(last_evt_3_1))
        ELSE
        SET_VALUE(GSW_STORY_PROGRESS, 401)
        SPAWN_EVENT(PTR(last_evt_4))
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(b_peach_fuwafuwa)
    LOOP(0)
        SET_VALUE(LW(0), 0)
        LOOP(60)
            WAIT_WHILE(GF(2))
            CALL(add_vy, 0, LW(0))
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        LOOP(60)
            WAIT_WHILE(GF(2))
            CALL(add_vy, 1, LW(0))
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(last_evt_3_1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, 0, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_set_pose, STRING("M_I_Y"))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_party_set_pos, 0, -45, 0, 25)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_position, STRING("影の女王"), 230, 15, 0)
    CALL(evt_npc_set_ry, STRING("影の女王"), 90)
    CALL(evt_npc_set_position, STRING("ブラックピーチ"), 200, 85, 0)
    CALL(evt_npc_set_ry, STRING("ブラックピーチ"), 270)
    CALL(evt_npc_set_position, STRING("マジョリン"), -50, 0, -150)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    CALL(evt_npc_set_position, STRING("マリリン"), -110, 0, -170)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_position, STRING("ランペル"), -75, 0, -200)
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -381, 10, 450, 140, 130, -1, 0, 11)
    CALL(evt_mapobj_trans, 1, STRING("S_hakahuta"), 60, -40, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_o_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(rou_hikari_front_onoff, 1, 1)
    CALL(evt_seq_wait, 2)
    SET_VALUE(GF(2), 0)
    SPAWN_EVENT(PTR(b_peach_fuwafuwa))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_146"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -22, 89, 282, -22, 35, -13, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print_party, STRING("stg8_las_147"))
    CALL(evt_cam3d_evt_set, -80, -40, 210, 204, 140, -7, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 25)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_STARSTONE_SHINE1"), LW(0), LW(1), LW(2), LW(9))
    CALL(radiation_entry, STRING("rad"), LW(0), LW(1), LW(2), FLOAT(1.0f))
    CALL(evt_snd_bgmoff_f, 512, 2000)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_01"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    CALL(evt_cam3d_evt_set, 0, 74, 236, 0, 30, -10, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_eff_delete, STRING("rad"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_STARSTONE_ROUND1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_mario_get_pos, 0, GW(0), GW(1), GW(2))
    ADD(GW(1), 25)
    SET_VALUE(GW(5), 25)
    CALL(starstone_entry, GW(0), GW(1), GW(2), GW(5))
    CALL(pcharge_entry, STRING("pcharge"), GW(0), GW(1), GW(2))
    SET_FLOAT_VALUE(GW(3), FLOAT(0.0f))
    SET_FLOAT_VALUE(GW(4), FLOAT(0.016601563f))
    BEGIN_INLINE_EVENT
        LOOP(0)
            ADD_FLOAT(GW(3), GW(4))
            WAIT_FRAMES(1)
            IF_INT_GREATER_OR_EQUAL(GW(3), FLOAT(6.2822266f))
                SUBTRACT_FLOAT(GW(3), FLOAT(6.2822266f))
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(starstone_set_pos, GW(0), GW(1), GW(2), GW(3), GW(5))
            CALL(pcharge_set_pos, STRING("pcharge"), GW(0), GW(1), GW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        LOOP(0)
            CALL(starstone_set_ry, LW(0))
            SUBTRACT_FLOAT(LW(0), FLOAT(3.0f))
            IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 0, 25, 40, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            SET_VALUE(GW(5), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 100, 414, 0, 75, -10, 2000, 0)
    CALL(evt_sub_intpl_msec_init, 4, GW(1), 70, 2000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(GW(1), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_msg_print_party, STRING("stg8_las_148_02"))
    CALL(evt_snd_sfxoff, LW(9))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_STARSTONE_ROUND2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_sub_intpl_msec_init, 0, 34, 102, 1500)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    SET_FLOAT_VALUE(GW(4), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_cam3d_evt_set, 0, 150, 460, 0, 430, 0, 800, 0)
    END_INLINE_EVENT
    CALL(evt_sub_intpl_msec_init, 0, 70, 380, 1000)
    LABEL(2)
    CALL(evt_sub_intpl_msec_get_value)
    SET_VALUE(GW(1), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(evt_fade_entry, 17, 1, 255, 255, 255)
#ifdef VERSION_G8ME01
    WAIT_MS(200)
#else
    WAIT_MS(50)
    CALL(evt_fade_entry, 16, 1, 255, 255, 255)
    WAIT_MS(50)
    CALL(evt_fade_entry, 17, 1, 255, 255, 255)
#endif
    CALL(evt_fade_set_mapchange_type, 1, 17, 1, 9, 300)
    CALL(evt_bero_mapchange, STRING("gor_01"), STRING("minnnanokoe"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(koe_evt)
    SET_VALUE(LW(0), 1)
    LOOP(0)
        CALL(make_koe_label, LW(0), LW(1))
        CALL(make_koe_pos, LW(2), LW(3), LW(4))
        BEGIN_INLINE_EVENT
            ADD(GW(1), 1)
            CALL(evt_msg_toge, 16777219, LW(2), LW(3), LW(4))
#ifdef VERSION_G8ME01
            CALL(evt_msg_pri, GW(6))
            ADD(GW(6), 1)
            IF_INT_EQUAL(GW(6), 10)
                SET_VALUE(GW(6), 0)
            END_IF
#endif
            CALL(evt_msg_print, 0, LW(1), 0, 0)
            SUBTRACT(GW(1), 1)
        END_INLINE_EVENT
        WAIT_MS(300)
        LOOP(0)
            SET_VALUE(LF(0), 0)
            IF_INT_GREATER_OR_EQUAL(GW(1), 6)
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        ADD(LW(0), 3)
        MODULO(LW(0), 20)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(majo_kyorokyoro)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_sub_random, 5, LW(0))
            MULTIPLY(LW(0), 200)
            ADD(LW(0), 2000)
            WAIT_MS(LW(0))
            CALL(slowturnoff_check, STRING("マジョリン"))
            CALL(evt_npc_reverse_ry, STRING("マジョリン"))
            WAIT_FRAMES(1)
            CALL(evt_npc_wait_pera, STRING("マジョリン"))
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_sub_random, 5, LW(0))
            MULTIPLY(LW(0), 200)
            ADD(LW(0), 2000)
            WAIT_MS(LW(0))
            CALL(slowturnoff_check, STRING("マリリン"))
            CALL(evt_npc_reverse_ry, STRING("マリリン"))
            WAIT_FRAMES(1)
            CALL(evt_npc_wait_pera, STRING("マリリン"))
        END_LOOP
    END_SIBLING_EVENT
    LOOP(0)
        CALL(evt_sub_random, 5, LW(0))
        MULTIPLY(LW(0), 200)
        ADD(LW(0), 2000)
        WAIT_MS(LW(0))
        CALL(slowturnoff_check, STRING("ランペル"))
        CALL(evt_npc_reverse_ry, STRING("ランペル"))
        WAIT_FRAMES(1)
        CALL(evt_npc_wait_pera, STRING("ランペル"))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(last_evt_3_2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_pos, 0, 0, 0)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_pos, 0, -45, 0, 25)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_position, STRING("影の女王"), 230, 15, 0)
    CALL(evt_npc_set_ry, STRING("影の女王"), 90)
    CALL(evt_npc_set_position, STRING("ブラックピーチ"), 200, 85, 0)
    CALL(evt_npc_set_ry, STRING("ブラックピーチ"), 270)
    CALL(evt_npc_set_position, STRING("マジョリン"), -50, 0, -150)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    CALL(evt_npc_set_position, STRING("マリリン"), -110, 0, -170)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_position, STRING("ランペル"), -75, 0, -200)
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_cam3d_evt_set, -384, 20, 345, 100, 100, 0, 0, 11)
    CALL(evt_mapobj_trans, 1, STRING("S_hakahuta"), 60, -40, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_o_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(rou_hikari_front_onoff, 1, 1)
    CALL(evt_seq_wait, 2)
    SET_VALUE(GF(2), 0)
    SPAWN_EVENT(PTR(b_peach_fuwafuwa))
    SET_VALUE(LW(0), 0)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GF(1), 0)
#ifdef VERSION_G8ME01
    SET_VALUE(GW(6), 0)
#endif
    LOOP(10)
        CALL(make_koe_label, LW(0), LW(1))
        CALL(make_koe_pos, LW(2), LW(3), LW(4))
        BEGIN_INLINE_EVENT
            ADD(GW(1), 1)
            CALL(evt_msg_toge, 16777219, LW(2), LW(3), LW(4))
#ifdef VERSION_G8ME01
            CALL(evt_msg_pri, GW(6))
            ADD(GW(6), 1)
            IF_INT_EQUAL(GW(6), 10)
                SET_VALUE(GW(6), 0)
            END_IF
#endif
            CALL(evt_msg_print, 0, LW(1), 0, 0)
            SUBTRACT(GW(1), 1)
        END_INLINE_EVENT
        WAIT_MS(200)
        ADD(LW(0), 3)
        MODULO(LW(0), 20)
    END_LOOP
    SPAWN_EVENT_GET_ID(PTR(koe_evt), GW(0))
    SPAWN_EVENT_GET_ID(PTR(majo_kyorokyoro), GW(2))
    WAIT_MS(1000)
    CALL(evt_party_set_dir, 0, 270, 400)
    WAIT_MS(500)
    CALL(evt_party_set_dir, 0, 90, 400)
    WAIT_MS(500)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print_party, STRING("stg8_las_148_72"))
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    CALL(evt_mario_dispflag_onoff, 1, 2)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -25, 90, 284, -25, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 270, 300, 0)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print_party, STRING("stg8_las_148_73"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_4"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_dispflag_onoff, 0, 2)
    CALL(evt_cam3d_evt_set, -650, 87, 643, 100, 156, -13, 0, 11)
    WAIT_FRAMES(1)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_dispflag_onoff, 1, 4)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_mario_set_dir, 90, 300, 0)
            WAIT_MS(1000)
            CALL(evt_mario_set_dir, 270, 300, 0)
            WAIT_MS(1000)
        END_LOOP
        CALL(evt_mario_dispflag_onoff, 0, 4)
    END_INLINE_EVENT
    WAIT_MS(5000)
    SET_VALUE(GF(0), 1)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_mario_adjust_dir)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -25, 90, 284, -25, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
        CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
        CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print_party, STRING("stg8_las_148_75"))
    CALL(evt_cam3d_evt_set, -100, 150, 520, 200, 150, 0, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_76"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    CALL(evt_eff64, 0, STRING("stardust_n64"), 1, LW(0), LW(1), LW(2), 30, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_SHINE1"), LW(0), LW(1), LW(2), LW(9))
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -22, 90, 284, -22, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
        CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
        CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    END_INLINE_EVENT
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print_party, STRING("stg8_las_148_77"))
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(500)
    CALL(evt_cam3d_evt_set, -100, 150, 520, 200, 150, 0, 0, 11)
    WAIT_FRAMES(1)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            LOOP(8)
                CALL(evt_sub_random, 160, LW(3))
                SUBTRACT(LW(3), 80)
                ADD(LW(3), LW(0))
                CALL(evt_sub_random, 100, LW(4))
                ADD(LW(4), LW(1))
                CALL(evt_eff64, 0, STRING("stardust_n64"), 2, LW(3), LW(4), LW(2), 42, 0, 0, 0, 0, 0, 0, 0)
                WAIT_MS(100)
            END_LOOP
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(500)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_78"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_SHINE2"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
        CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -150, 1, 433, 200, 150, 0, 800, 11)
    CALL(evt_env_blur_on, 0, 800)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_79"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_SHINE2_2"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
        CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -200, 1, 356, 200, 150, 0, 800, 11)
    CALL(evt_env_blur_on, 0, 800)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_80"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_SHINE2_3"), LW(0), LW(1), LW(2), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
        ADD(LW(1), 20)
        CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
        WAIT_MS(200)
        CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 15)
        ADD(LW(1), 20)
        ADD(LW(2), 10)
        CALL(evt_eff64, 0, STRING("snow_dust_n64"), 4, LW(0), LW(1), LW(2), 15, 15, FLOAT(1.0f), 6, 60, 0, 0, 0)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, -150, 1, 260, 200, 150, 0, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_148_81"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    SET_VALUE(GF(0), 1)
    CALL(evt_snd_sfxoff, LW(9))
    WAIT_MS(500)
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_SHINE3"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), 20)
    CALL(evt_eff64, 0, STRING("release_n64"), 3, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1200)
    CALL(evt_fade_entry, 17, 1, 255, 255, 255)
    WAIT_MS(100)
    CALL(evt_fade_entry, 16, 1, 255, 255, 255)
    WAIT_FRAMES(1)
    CALL(evt_paper_entry, STRING("OFF_d_bom"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom"))
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_D_1"))
    CALL(evt_snd_sfxon, STRING("SFX_STG8_QUEEN_SHINE4"), 0)
    CALL(evt_img_onoff, STRING("img"), 1)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_bom"))
    WAIT_MS(1000)
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -22, 90, 284, -22, 40, -13, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_S_1"))
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(1)
        CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
        CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
        CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    END_INLINE_EVENT
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_149"), 0, STRING("ピーチ姫"))
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1200)
    SET_VALUE(GF(2), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, 100, 200, 692, 100, 100, 0, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 0, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print_party, STRING("stg8_las_150"))
    CALL(evt_npc_flag_onoff, 1, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_set, -150, 178, 288, 195, 150, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_2F"))
    CALL(evt_paper_entry, STRING("p_b_st"))
    CALL(evt_npc_set_paper, STRING("ピーチ姫"), STRING("p_b_st"))
    CALL(evt_npc_set_paper_anim, STRING("ピーチ姫"), STRING("PM_Y_1A"))
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 10)
    ADD(LW(2), 10)
    CALL(evt_npc_set_color, STRING("ピーチ姫"), 255, 255, 255, 0)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    CALL(evt_sub_intpl_msec_init, 11, 0, 153, 1000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_npc_set_color, STRING("ピーチ姫"), 255, 255, 255, LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    WAIT_MS(500)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_2F"), STRING("P_T_2F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_151"), 0, STRING("ピーチ姫"))
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_152"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_cam3d_evt_set, 3, 146, 160, 195, 130, 0, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_7F"), STRING("P_T_7F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_153"), 0, STRING("ピーチ姫"))
    CALL(evt_sub_intpl_msec_init, 11, 153, 51, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_color, STRING("ピーチ姫"), 255, 255, 255, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_eff_fukidashi, 1, 0, STRING("ブラックピーチ"), 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("ブラックピーチ"), STRING("KRT_A_3"))
    WAIT_MS(200)
    CALL(evt_npc_get_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_PEACH_SHINE1"), LW(0), LW(1), LW(2), 0)
    ADD(LW(1), 30)
    CALL(evt_eff64, 0, STRING("lensflare_n64"), 0, LW(0), LW(1), LW(2), 0, 96, 0, 0, 0, 0, 0, 0)
    WAIT_MS(400)
    CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 48, 0, 0, 0, 0, 0, 0)
    WAIT_MS(200)
    CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 48, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_eff, 0, STRING("stardust"), 6, LW(0), LW(1), LW(2), 20, 20, 8, 78, 0, 0, 0, 0)
    CALL(evt_eff64, 0, STRING("broken_barrier_n64"), 1, LW(0), LW(1), LW(2), 1, 78, 0, 0, 0, 0, 0, 0)
    WAIT_MS(400)
    CALL(evt_eff64, 0, STRING("crystal_n64"), 4, LW(0), LW(1), LW(2), 1, 72, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(evt_cam3d_evt_set, -22, 82, 258, -22, 35, -13, 800, 11)
    WAIT_MS(1000)
    CALL(evt_sub_status_gauge_force_open)
    WAIT_MS(300)
    CALL(evt_snd_bgmoff_f, 512, 3000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_MARIO_SHINE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_party_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_eff64, 0, STRING("broken_barrier_n64"), 2, LW(0), LW(1), LW(2), 1, 60, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff64, 0, STRING("broken_barrier_n64"), 2, LW(3), LW(4), LW(5), 1, 60, 0, 0, 0, 0, 0, 0)
    WAIT_MS(900)
    CALL(evt_eff, 0, STRING("stardust"), 4, LW(0), LW(1), LW(2), 30, 40, 12, 120, 0, 0, 0, 0)
    CALL(evt_eff, 0, STRING("stardust"), 4, LW(3), LW(4), LW(5), 30, 40, 12, 120, 0, 0, 0, 0)
    WAIT_MS(200)
    CALL(evt_eff, 0, STRING("stardust"), 4, LW(0), LW(1), LW(2), 30, 40, 12, 120, 0, 0, 0, 0)
    CALL(evt_eff, 0, STRING("stardust"), 4, LW(3), LW(4), LW(5), 30, 40, 12, 120, 0, 0, 0, 0)
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    LOOP(0)
        CALL(evt_sub_status_gauge_check_update, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(500)
    CALL(evt_sub_status_gauge_force_close)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print_party, STRING("stg8_las_154"))
    CALL(evt_cam3d_evt_set, -125, 155, 240, 195, 130, 0, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_155"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_156"), 0, STRING("ピーチ姫"))
    CALL(evt_npc_flag_onoff, 0, STRING("ブラックピーチ"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_sub_intpl_msec_init, 11, 51, 0, 1000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_npc_set_color, STRING("ピーチ姫"), 255, 255, 255, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_npc_clear_paper, STRING("ピーチ姫"))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_2F"))
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_2F"), STRING("P_T_2F"))
    CALL(evt_cam3d_evt_set, 100, 200, 692, 100, 100, 0, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print_party, STRING("stg8_las_157"))
    CALL(evt_cam3d_evt_set, 0, 150, 347, 200, 150, 0, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_158"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_cam3d_evt_set, 195, 146, 274, 195, 130, 0, 800, 11)
    WAIT_MS(800)
    CALL(evt_msg_pri, 20)
    CALL(evt_msg_print, 0, STRING("stg8_las_159"), PTR(b_peach_msgcb), STRING("ブラックピーチ"))
    CALL(evt_cam3d_evt_set, 60, 150, 684, 60, 75, -11, 1000, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, 1000)
    END_INLINE_EVENT
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_adjust_dir)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 1000, 1, -1000)
    END_INLINE_EVENT
    WAIT_MS(300)
    DELETE_EVENT(GW(0))
    DELETE_EVENT(GW(2))
    CALL(evt_npc_release_fieldnpc, 1)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_npc_set_battle_info, STRING("ブラックピーチ"), 17)
    CALL(evt_npc_battle_start, STRING("ブラックピーチ"))
    CALL(evt_npc_wait_battle_end)
    SET_VALUE(GSW_STORY_PROGRESS, 401)
    WAIT_FRAMES(1)
    CALL(rou_hikari_front_onoff, 0, 1)
    SPAWN_EVENT(PTR(last_evt_4))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(last_evt_4)
    CALL(evt_snd_bgmoff, 512)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_cam3d_evt_set, -560, 100, 283, 19, 150, 0, 0, 11)
    CALL(evt_mario_set_pos, -150, 0, 0)
    CALL(evt_party_set_pos, 0, -190, 0, 15)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_position, STRING("影の女王"), 55, 0, 0)
    CALL(evt_npc_set_position, STRING("ブラックピーチ"), 25, 70, 0)
    CALL(evt_npc_set_position, STRING("マジョリン"), -50, 0, -150)
    CALL(evt_npc_set_position, STRING("マリリン"), -90, 0, -170)
    CALL(evt_npc_set_position, STRING("ランペル"), -70, 0, -200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_ry, STRING("影の女王"), 90)
    CALL(evt_npc_set_ry, STRING("ブラックピーチ"), 270)
    CALL(evt_npc_set_ry, STRING("マジョリン"), 90)
    CALL(evt_npc_set_ry, STRING("マリリン"), 90)
    CALL(evt_npc_set_ry, STRING("ランペル"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING("影の女王"), NPC_FLAG_NO_HIT)
#else
    CALL(evt_npc_set_position, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), 55, 0, 0)
    CALL(evt_npc_set_position, STRING(/*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60"), 25, 70, 0)
    CALL(evt_npc_set_position, STRING(/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93"), -50, 0, -150)
    CALL(evt_npc_set_position, STRING(/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93"), -90, 0, -170)
    CALL(evt_npc_set_position, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), -70, 0, -200)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 90, 0)
    CALL(evt_npc_set_ry, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), 90)
    CALL(evt_npc_set_ry, STRING(/*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60"), 270)
    CALL(evt_npc_set_ry, STRING(/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93"), 90)
    CALL(evt_npc_set_ry, STRING(/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93"), 90)
    CALL(evt_npc_set_ry, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), 90)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), NPC_FLAG_NO_HIT)
#endif
    CALL(evt_mapobj_trans, 0, STRING("S_hakahuta"), 60, -40, 0)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_o_hikari"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kuro"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
#ifdef VERSION_G8ME01
    CALL(evt_npc_entry, STRING("クリハカセ"), STRING("c_kuri_h"))
    CALL(evt_npc_set_tribe, STRING("クリハカセ"), STRING("クリハカセ"))
    CALL(evt_npc_flag_onoff, 1, STRING("クリハカセ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_position, STRING("クリハカセ"), 0, -1000, 0)
#else
    CALL(evt_npc_entry, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), STRING("c_kuri_h"))
    CALL(evt_npc_set_tribe, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_position, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), 0, -1000, 0)
#endif
    CALL(evt_seq_wait, 2)
    SET_VALUE(GF(0), 0)
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_STG8_QUAKE1"), LW(9))
        SPAWN_EVENT_GET_ID(PTR(evt_sub_stopwatch), LW(15))
        LOOP(0)
            IF_INT_NOT_EQUAL(GF(0), 0)
                BREAK_LOOP
            END_IF
            CALL(evt_sub_get_stopwatch, LW(15), LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 30000)
                BREAK_LOOP
            END_IF
            CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 1000)
        END_LOOP
        CALL(evt_snd_sfxoff, LW(9))
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_anim, STRING("影の女王"), STRING("KRT_H_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING("影の女王"), STRING("KRT_S_3"), STRING("KRT_T_2"))
    CALL(evt_msg_print, 0, STRING("stg8_las_169"), 0, STRING("影の女王"))
#else
    CALL(evt_npc_set_anim, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), STRING("KRT_H_1"))
    WAIT_MS(500)
    CALL(evt_npc_set_autotalkpose, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), STRING("KRT_S_3"), STRING("KRT_T_2"))
    CALL(evt_msg_print, 0, STRING("stg8_las_169"), 0, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"))
#endif
    CALL(evt_cam3d_evt_set, -530, 31, 530, 19, 120, 0, 800, 11)
    WAIT_MS(800)
    BEGIN_INLINE_EVENT
        WAIT_MS(150)
        CALL(evt_snd_sfxon, STRING("SFX_STG8_QUEEN_DIE2"), 0)
        CALL(evt_fade_entry, 17, 1, 255, 255, 255)
        WAIT_MS(300)
        CALL(evt_fade_entry, 16, 1, 255, 255, 255)
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
#else
    CALL(evt_npc_get_position, STRING(/*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60"), LW(0), LW(1), LW(2))
#endif
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_DIE1"), LW(0), LW(1), LW(2), LW(15))
    SET_VALUE(LW(3), 0)
    SET_FLOAT_VALUE(LW(5), FLOAT(30.0f))
    LOOP(10)
        CALL(make_mist_name, LW(3), LW(4))
        ADD(LW(1), 5)
        CALL(evt_eff, LW(4), STRING("mist2"), 0, LW(0), LW(1), LW(2), LW(5), 0, 1000, 0, 0, 0, 0, 0)
        WAIT_MS(350)
        ADD(LW(3), 1)
        IF_INT_GREATER_OR_EQUAL(LW(3), 5)
            SUBTRACT_FLOAT(LW(5), FLOAT(5.6992188f))
#ifdef VERSION_G8ME01
            ELSE
#else
        ELSE
#endif
            ADD_FLOAT(LW(5), FLOAT(5.6992188f))
        END_IF
    END_LOOP
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_color, STRING("ブラックピーチ"), 96, 96, 96, 255)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("影の女王"), STRING("KRT_C_1"))
    WAIT_MS(350)
    CALL(evt_npc_flag_onoff, 0, STRING("影の女王"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_set_anim, STRING("影の女王"), STRING("KRT_C_2"))
    CALL(evt_msg_print, 0, STRING("stg8_las_170"), 0, STRING("影の女王"))
    CALL(evt_msg_print, 0, STRING("stg8_las_171"), 0, STRING("マジョリン"))
    CALL(evt_msg_print, 0, STRING("stg8_las_172"), 0, STRING("マリリン"))
#else
    CALL(evt_npc_set_color, STRING(/*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60"), 96, 96, 96, 255)
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), STRING("KRT_C_1"))
    WAIT_MS(350)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_set_anim, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), STRING("KRT_C_2"))
    CALL(evt_msg_print, 0, STRING("stg8_las_170"), 0, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"))
    CALL(evt_msg_print, 0, STRING("stg8_las_171"), 0, STRING(/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93"))
    CALL(evt_msg_print, 0, STRING("stg8_las_172"), 0, STRING(/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93"))
#endif
    SET_VALUE(GF(0), 1)
    CALL(evt_snd_sfxoff, LW(15))
    LOOP(2)
#ifdef VERSION_G8ME01
        WAIT_MS(450)
#else
        WAIT_MS(150)
#endif
        CALL(evt_snd_sfxon, STRING("SFX_STG8_QUEEN_DIE2"), 0)
        CALL(evt_fade_entry, 17, 1, 255, 255, 255)
        WAIT_MS(150)
        CALL(evt_fade_entry, 16, 1, 255, 255, 255)
    END_LOOP
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_position, STRING("影の女王"), 0, -1000, 0)
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
#else
    CALL(evt_npc_set_position, STRING(/*"影の女王"*/"\x89\x65\x82\xCC\x8F\x97\x89\xA4"), 0, -1000, 0)
    CALL(evt_npc_get_position, STRING(/*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60"), LW(0), LW(1), LW(2))
#endif
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_QUEEN_DIE3"), LW(0), LW(1), LW(2), 0)
    CALL(eff_b_peach_ripple)
    WAIT_MS(800)
    CALL(eff_b_peach_ripple)
    WAIT_MS(1500)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_1F"))
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_get_position, STRING("ブラックピーチ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING("ブラックピーチ"), 0, -1000, 0)
    ADD(LW(1), 20)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_get_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
#else
    CALL(evt_npc_set_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), STRING("P_S_1F"))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_NO_DRAW)
    CALL(evt_npc_get_position, STRING(/*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_position, STRING(/*"ブラックピーチ"*/"\x83\x75\x83\x89\x83\x62\x83\x4E\x83\x73\x81\x5B\x83\x60"), 0, -1000, 0)
    ADD(LW(1), 20)
    CALL(evt_npc_set_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_get_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(1), LW(2))
#endif
    SET_FLOAT_VALUE(LW(3), FLOAT(-0.5f))
    LOOP(0)
        ADD_FLOAT(LW(3), FLOAT(0.0009765625f))
        IF_FLOAT_GREATER_OR_EQUAL(LW(3), FLOAT(-0.099609375f))
            SET_FLOAT_VALUE(LW(3), FLOAT(-0.099609375f))
        END_IF
        ADD_FLOAT(LW(1), LW(3))
#ifdef VERSION_G8ME01
        CALL(evt_npc_set_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
#else
        CALL(evt_npc_set_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(1), LW(2))
#endif
        CALL(mist_add_vy, LW(3))
        WAIT_FRAMES(1)
        IF_INT_LESS_OR_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 0, STRING("ピーチ姫"), NPC_FLAG_NO_DRAW)
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_npc_set_color, STRING("ピーチ姫"), 96, 96, 96, 255)
#else
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_NO_DRAW)
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_npc_set_color, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), 96, 96, 96, 255)
#endif
        CALL(evt_sub_intpl_msec_init, 11, 96, 255, 1000)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
#ifdef VERSION_G8ME01
            CALL(evt_npc_set_color, STRING("ピーチ姫"), LW(0), LW(0), LW(0), 255)
#else
            CALL(evt_npc_set_color, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(0), LW(0), 255)
#endif
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(eff_b_peach_mist)
    WAIT_MS(300)
    SET_VALUE(LW(0), 1)
    LOOP(5)
        CALL(make_mist_name, LW(0), LW(1))
        CALL(evt_eff_softdelete, LW(1))
        ADD(LW(0), 2)
    END_LOOP
    WAIT_MS(200)
    SET_VALUE(LW(0), 0)
    LOOP(5)
        CALL(make_mist_name, LW(0), LW(1))
        CALL(evt_eff_softdelete, LW(1))
        ADD(LW(0), 2)
    END_LOOP
    WAIT_MS(4400)
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 0, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN)
#else
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_INSTANT_TURN)
#endif
    WAIT_MS(500)
    SET_VALUE(LW(0), 0)
    SET_FLOAT_VALUE(LW(1), FLOAT(350.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(255.0f))
    SET_VALUE(LW(3), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(0), -1)
            IF_INT_EQUAL(LW(3), -1)
                BREAK_LOOP
            END_IF
        END_IF
        IF_INT_NOT_EQUAL(LW(0), -1)
            CALL(hikari_onoff, PTR(&hikari_data), 1, 1, LW(0), LW(1), LW(0))
        END_IF
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(hikari_onoff, PTR(&hikari_data_2), 1, 1, LW(3), LW(1), LW(3))
        END_IF
        SUBTRACT_FLOAT(LW(1), FLOAT(3.2998047f))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_yukahikari01"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("S_yukahikari02"), MAP_OBJ_FLAG_HIDDEN)
    SET_VALUE(LW(0), 0)
    SET_FLOAT_VALUE(LW(1), FLOAT(350.0f))
    SET_FLOAT_VALUE(LW(2), FLOAT(0.0f))
    SET_VALUE(LW(3), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(0), -1)
            IF_INT_EQUAL(LW(3), -1)
                BREAK_LOOP
            END_IF
        END_IF
        IF_INT_NOT_EQUAL(LW(0), -1)
            CALL(hikari_onoff, PTR(&hikari_data), 0, 0, LW(0), LW(1), LW(0))
        END_IF
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(hikari_onoff, PTR(&hikari_data_2), 0, 0, LW(3), LW(1), LW(3))
        END_IF
        SUBTRACT(LW(1), FLOAT(3.2998047f))
        CALL(evt_mapobj_color, 1, STRING("S_yukahikari01"), 255, 255, 255, LW(2))
        CALL(evt_mapobj_color, 1, STRING("S_yukahikari02"), 255, 255, 255, LW(2))
        ADD_FLOAT(LW(2), FLOAT(1.5292969f))
        IF_INT_GREATER(LW(2), FLOAT(255.0f))
            SET_FLOAT_VALUE(LW(2), FLOAT(255.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("hikari"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_MS(1000)
    CALL(evt_map_get_fog, LW(2), LW(3), LW(4), LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), LW(4))
    MULTIPLY(LW(8), 2)
    CALL(evt_sub_intpl_msec_init, 1, LW(4), LW(8), 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_fog, LW(2), LW(3), LW(0), LW(5), LW(6), LW(7))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    SET_VALUE(LW(9), LW(3))
    MULTIPLY(LW(9), 2)
    CALL(evt_sub_intpl_msec_init, 0, LW(3), LW(9), 2000)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_map_set_fog, LW(2), LW(0), LW(8), LW(5), LW(6), LW(7))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_map_fog_onoff, 0)
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 1, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
#else
    CALL(evt_npc_flag_onoff, 1, STRING(/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), NPC_FLAG_INSTANT_TURN)
#endif
    CALL(evt_cam3d_evt_set, -70, 105, 180, -70, 45, -165, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 0, STRING("マジョリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("マリリン"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING("ランペル"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg8_las_173"), 0, STRING("マジョリン"))
    CALL(evt_npc_majo_disp_off, STRING("マジョリン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_174"), 0, STRING("マリリン"))
    CALL(evt_npc_majo_disp_off, STRING("マリリン"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_175"), 0, STRING("ランペル"))
    CALL(evt_npc_reverse_ry, STRING("ランペル"))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING("ランペル"))
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING("ランペル"), 270)
    WAIT_MS(300)
    CALL(evt_npc_sound_data_set, STRING("ランペル"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, 0)
    CALL(evt_npc_get_position, STRING("ランペル"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 230)
    CALL(evt_npc_jump_position_nohit, STRING("ランペル"), LW(0), LW(1), LW(2), 800, 105)
    CALL(evt_npc_flag_onoff, 1, STRING("ランペル"), NPC_FLAG_NO_DRAW)
#else
    CALL(evt_npc_flag_onoff, 0, STRING(/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_msg_print, 0, STRING("stg8_las_173"), 0, STRING(/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93"))
    CALL(evt_npc_majo_disp_off, STRING(/*"マジョリン"*/"\x83\x7D\x83\x57\x83\x87\x83\x8A\x83\x93"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_174"), 0, STRING(/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93"))
    CALL(evt_npc_majo_disp_off, STRING(/*"マリリン"*/"\x83\x7D\x83\x8A\x83\x8A\x83\x93"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_175"), 0, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"))
    CALL(evt_npc_reverse_ry, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"))
    WAIT_MS(300)
    CALL(evt_npc_reverse_ry, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"))
    WAIT_MS(300)
    CALL(evt_npc_set_ry, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), 270)
    WAIT_MS(300)
    CALL(evt_npc_sound_data_set, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, 0)
    CALL(evt_npc_get_position, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 230)
    CALL(evt_npc_jump_position_nohit, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), LW(0), LW(1), LW(2), 800, 105)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ランペル"*/"\x83\x89\x83\x93\x83\x79\x83\x8B"), NPC_FLAG_NO_DRAW)
#endif
    CALL(rou_hikari_front_onoff, 1, 0)
    CALL(evt_cam3d_evt_set, -170, 100, 332, -170, 40, -13, 0, 11)
    CALL(evt_mario_set_pose, STRING("M_I_N"))
    WAIT_FRAMES(1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_176"))
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN | NPC_FLAG_NO_VIEW_CORRECTION)
#else
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_INSTANT_TURN | NPC_FLAG_NO_VIEW_CORRECTION)
#endif
    CALL(evt_cam3d_evt_set, 15, 85, 258, 15, 38, -13, 1500, 11)
    CALL(evt_party_move_pos, 0, 0, 10, 1500)
    WAIT_MS(1700)
    CALL(evt_snd_bgmon, 512, STRING("BGM_ENDING1"))
    WAIT_MS(300)
    CALL(rou_hikari_front_onoff, 0, 0)
    CALL(evt_eff_fukidashi, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_party_set_dir, 0, 270, -1)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_177"))
    CALL(evt_cam3d_evt_set, 130, 20, 114, -145, 40, -5, 0, 11)
    WAIT_FRAMES(1)
    WAIT_MS(300)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos, -35, 0, 2800)
    END_INLINE_EVENT
    WAIT_MS(250)
    CALL(evt_cam3d_evt_set, 0, 87, 258, 0, 40, -13, 750, 11)
    CALL(evt_party_move_pos, 0, -10, -40, 800)
    WAIT_MS(700)
    CALL(evt_msg_print_party, STRING("stg8_las_178"))
    CALL(evt_mario_wait_move_end)
    WAIT_MS(100)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 0, STRING("ピーチ姫"), NPC_FLAG_INSTANT_TURN | NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_get_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 1)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    WAIT_MS(50)
    ADD(LW(2), 2)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    WAIT_MS(50)
    SUBTRACT(LW(2), 1)
    CALL(evt_npc_set_position, STRING("ピーチ姫"), LW(0), LW(1), LW(2))
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 0, STRING("ピーチ姫"), NPC_FLAG_TALK_POSE)
    CALL(evt_msg_print, 0, STRING("stg8_las_179"), 0, STRING("ピーチ姫"))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_A_1F"))
    CALL(evt_npc_wait_anim, STRING("ピーチ姫"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_A_3F"))
    CALL(evt_npc_wait_anim, STRING("ピーチ姫"), FLOAT(1.0f))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_8F"), STRING("P_T_6F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_180"), 0, STRING("ピーチ姫"))
#else
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_INSTANT_TURN | NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_get_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 1)
    CALL(evt_npc_set_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(1), LW(2))
    WAIT_MS(50)
    ADD(LW(2), 2)
    CALL(evt_npc_set_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(1), LW(2))
    WAIT_MS(50)
    SUBTRACT(LW(2), 1)
    CALL(evt_npc_set_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), LW(0), LW(1), LW(2))
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_TALK_POSE)
    CALL(evt_msg_print, 0, STRING("stg8_las_179"), 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"))
    CALL(evt_npc_set_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), STRING("P_A_1F"))
    CALL(evt_npc_wait_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), STRING("P_A_3F"))
    CALL(evt_npc_wait_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), FLOAT(1.0f))
    WAIT_MS(500)
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_TALK_POSE)
    CALL(evt_npc_set_autotalkpose, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), STRING("P_S_8F"), STRING("P_T_6F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_180"), 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"))
#endif
    WAIT_MS(500)
    CALL(evt_mario_mov_pos2, -25, 0, FLOAT(30.0f))
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg8_las_181"), 0, STRING("ピーチ姫"))
#else
    CALL(evt_msg_print, 0, STRING("stg8_las_181"), 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"))
#endif
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg8_las_182"), 0, STRING("ピーチ姫"))
#else
    CALL(evt_msg_print, 0, STRING("stg8_las_182"), 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"))
#endif
    CALL(evt_mario_set_pose, STRING("M_N_B"))
    CALL(evt_party_move_pos2, 0, 0, -30, FLOAT(30.0f))
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    WAIT_MS(300)
    CALL(evt_msg_print_party, STRING("stg8_las_183"))
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_A_4F"))
    CALL(evt_npc_wait_anim, STRING("ピーチ姫"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING("ピーチ姫"), STRING("P_S_9F"))
#else
    CALL(evt_npc_set_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), STRING("P_A_4F"))
    CALL(evt_npc_wait_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), FLOAT(1.0f))
    CALL(evt_npc_set_anim, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), STRING("P_S_9F"))
#endif
    WAIT_MS(500)
    CALL(evt_mario_mov_pos2, -15, 0, FLOAT(30.0f))
    WAIT_MS(500)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_autotalkpose, STRING("ピーチ姫"), STRING("P_S_9F"), STRING("P_T_9F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_184"), 0, STRING("ピーチ姫"))
    CALL(evt_npc_set_position, STRING("クリハカセ"), -150, 0, 50)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
#else
    CALL(evt_npc_set_autotalkpose, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), STRING("P_S_9F"), STRING("P_T_9F"))
    CALL(evt_msg_print, 0, STRING("stg8_las_184"), 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"))
    CALL(evt_npc_set_position, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), -150, 0, 50)
    CALL(evt_npc_set_ry, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), 90)
#endif
    WAIT_MS(2000)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_mario_set_dir, 270, 300, 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        CALL(evt_party_set_dir, 0, 270, 300)
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg8_las_185"), 0, STRING("クリハカセ"))
#else
    CALL(evt_msg_print, 0, STRING("stg8_las_185"), 0, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
#endif
    CALL(rou_hikari_front_onoff, 1, 0)
    CALL(evt_cam3d_evt_set, 0, 104, 383, 0, 40, -12, 1000, 11)
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    CALL(evt_npc_move_position, STRING("クリハカセ"), -50, 50, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_186"), 0, STRING("クリハカセ"))
    CALL(evt_npc_reverse_ry, STRING("クリハカセ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("クリハカセ"))
    WAIT_MS(100)
    CALL(evt_npc_reverse_ry, STRING("クリハカセ"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("クリハカセ"))
    WAIT_MS(100)
    CALL(evt_msg_print, 0, STRING("stg8_las_187"), 0, STRING("クリハカセ"))
#else
    CALL(evt_npc_move_position, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), -50, 50, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_186"), 0, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    CALL(evt_npc_reverse_ry, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    WAIT_MS(100)
    CALL(evt_npc_reverse_ry, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    WAIT_MS(100)
    CALL(evt_msg_print, 0, STRING("stg8_las_187"), 0, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
#endif
    WAIT_MS(250)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAND_UP1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_I_2"))
    WAIT_MS(1200)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
    CALL(evt_msg_print, 0, STRING("stg8_las_188"), 0, STRING("クリハカセ"))
#else
    CALL(evt_msg_print, 0, STRING("stg8_las_188"), 0, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
#endif
    CALL(evt_cam3d_evt_set, -50, 87, 307, -50, 35, 12, 1000, 11)
    WAIT_MS(1000)
    CALL(rou_hikari_front_onoff, 0, 0)
#ifdef VERSION_G8ME01
    CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(1), LW(2))
#else
    CALL(evt_npc_get_position, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), LW(0), LW(1), LW(2))
#endif
    ADD(LW(0), 5)
    ADD(LW(1), 20)
    CALL(evt_mobj_itembox, STRING("box"), LW(0), LW(1), LW(2), 4, 0, 0, 0)
    CALL(evt_mobj_set_scale, STRING("box"), FLOAT(0.2998047f), FLOAT(0.2998047f), FLOAT(0.2998047f))
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY)
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, 1, 300, 500, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_mobj_set_scale, STRING("box"), LW(0), LW(0), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
#ifdef VERSION_G8ME01
        CALL(evt_npc_get_position, STRING("クリハカセ"), LW(0), LW(2), LW(1))
#else
        CALL(evt_npc_get_position, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), LW(0), LW(2), LW(1))
#endif
        CALL(evt_sub_intpl_msec_init, 0, 20, 30, 300)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            ADD_FLOAT(LW(0), LW(2))
            CALL(evt_mobj_set_y_position, STRING("box"), LW(0))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_anim, STRING("クリハカセ"), STRING("A_2"))
    WAIT_MS(800)
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 270)
#else
    CALL(evt_npc_set_anim, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), STRING("A_2"))
    WAIT_MS(800)
    CALL(evt_npc_set_ry, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), 270)
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_mobj_get_y_position, STRING("box"), LW(2))
        CALL(evt_sub_intpl_msec_init, 0, 0, 180, 400)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value)
            CALL(evt_sub_get_sincos, LW(0), LW(3), LW(4))
            MULTIPLY_FLOAT(LW(3), FLOAT(10.0f))
            ADD_FLOAT(LW(3), LW(2))
            CALL(evt_mobj_set_y_position, STRING("box"), LW(3))
            WAIT_FRAMES(1)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mobj_get_x_position, STRING("box"), LW(0))
    SET_VALUE(LW(1), LW(0))
    SUBTRACT(LW(1), 30)
    CALL(evt_sub_intpl_msec_init, 0, LW(0), LW(1), 400)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        CALL(evt_mobj_set_x_position, STRING("box"), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mobj_get_y_position, STRING("box"), LW(0))
    CALL(evt_sub_intpl_msec_init, 1, LW(0), 0, 200)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_mobj_set_y_position, STRING("box"), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_KURI_TBOX_LANDING1"), LW(0), LW(1), LW(2), 0)
    ADD(LW(2), 25)
    SET_VALUE(LW(3), LW(0))
    ADD(LW(3), 15)
    SET_VALUE(LW(4), LW(1))
    ADD(LW(4), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(3), LW(4), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(3), LW(0))
    ADD(LW(3), 5)
    SET_VALUE(LW(4), LW(1))
    ADD(LW(4), 5)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(3), LW(4), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(3), LW(0))
    SUBTRACT(LW(3), 20)
    SET_VALUE(LW(4), LW(1))
    ADD(LW(4), 7)
    CALL(evt_eff, 0, STRING("kemuri_test"), 0, LW(3), LW(4), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
#ifdef VERSION_G8ME01
    CALL(evt_npc_set_ry, STRING("クリハカセ"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("クリハカセ"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_189"), 0, STRING("クリハカセ"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_190"))
    CALL(evt_msg_print, 0, STRING("stg8_las_191"), 0, STRING("クリハカセ"))
#else
    CALL(evt_npc_set_ry, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING("stg8_las_189"), 0, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg8_las_190"))
    CALL(evt_msg_print, 0, STRING("stg8_las_191"), 0, STRING(/*"クリハカセ"*/"\x83\x4E\x83\x8A\x83\x6E\x83\x4A\x83\x5A"))
#endif
    CALL(rou_hikari_front_onoff, 1, 0)
    CALL(evt_cam3d_evt_set, 0, 100, 333, 0, 40, -13, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_NOD1_6"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING("M_N_2"))
    WAIT_MS(500)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
#ifdef VERSION_G8ME01
    CALL(evt_npc_flag_onoff, 1, STRING("ピーチ姫"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING("ピーチ姫"), 15, 0, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_192"), 0, STRING("ピーチ姫"))
#else
    CALL(evt_npc_flag_onoff, 1, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_move_position, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"), 15, 0, 0, FLOAT(60.0f), 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_192"), 0, STRING(/*"ピーチ姫"*/"\x83\x73\x81\x5B\x83\x60\x95\x50"))
#endif
    CALL(rou_hikari_front_onoff, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_PAUSE3"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), 500, 1, 30)
    END_INLINE_EVENT
    CALL(evt_cam3d_evt_set, 0, 300, 840, 0, -20, 0, 4000, 11)
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff_f, 512, 3000)
    CALL(evt_fade_out, 1800)
    SET_VALUE(GSW_STORY_PROGRESS, 402)
    CALL(evt_fade_set_mapchange_type, 1, 10, 1, 9, 300)
    CALL(evt_bero_mapchange, STRING("gor_11"), 0)
    RETURN
EVT_END()

static EVT_BEGIN(shuryolight_init)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 400)
        CALL(evt_shuryolight_init, STRING("シュリョー"))
        CALL(evt_shuryolight_run_evt, PTR(shuryolight_pow_tenmetu), PTR(shuryolight_head_off), PTR(shuryolight_tue_normal))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(shuryolight_delete)
    CALL(evt_npc_check, STRING("シュリョー"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_seq_wait, 2)
        WAIT_MS(500)
        CALL(evt_npc_delete, STRING("シュリョー"))
        CALL(evt_shuryolight_delete)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(las_29_init_evt)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(shuryolight_init))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(400)
        CALL(evt_bero_get_entername, LW(15))
        IF_STR_EQUAL(LW(15), STRING("minnnanokoe"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_VOICE1"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS8"))
            CALL(evt_snd_bgmoff, 26624)
            CALL(evt_snd_set_rev_mode, 2)
            CALL(rousoku_init, 1)
            SPAWN_EVENT(PTR(shuryolight_delete))
            SPAWN_EVENT(PTR(last_evt_3_2))
        END_IF
        IF_STR_EQUAL(LW(15), STRING("starstone"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_WITCH"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS8"))
            CALL(evt_snd_set_rev_mode, 2)
            CALL(rousoku_init, 1)
            SPAWN_EVENT(PTR(shuryolight_delete))
            SPAWN_EVENT(PTR(last_evt_3_1))
        END_IF
        IF_STR_EQUAL(LW(15), STRING("sekai_yami2"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_WITCH"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS8"))
            CALL(evt_snd_set_rev_mode, 2)
            CALL(rousoku_init, 1)
#ifndef VERSION_G8ME01
            CALL_CHILD(PTR(nise_mario_init))
#endif
            SPAWN_EVENT(PTR(last_evt_3))
        END_IF
        IF_STR_EQUAL(LW(15), STRING("sekai_yami1"))
            CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_STG8_WITCH"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS8"))
            CALL(evt_snd_set_rev_mode, 2)
            CALL(rousoku_init, 1)
            SPAWN_EVENT(PTR(last_evt_2))
        END_IF
        IF_STR_EQUAL(LW(15), STRING("w_bero"))
            CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS8"))
            CALL(evt_snd_set_rev_mode, 2)
            CALL(rousoku_init, 0)
            SPAWN_EVENT(PTR(last_evt_1))
        END_IF
        CASE_EQUAL(401)
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS8"))
        CALL(evt_snd_set_rev_mode, 2)
        CALL(evt_party_stop, 0)
        CALL(rousoku_init, 1)
        SPAWN_EVENT(PTR(last_evt_4))
        CASE_DEFAULT
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG8_LAS8"))
        CALL(evt_snd_set_rev_mode, 2)
        CALL_CHILD(PTR(evt_bero_info_run))
        CALL(rousoku_init, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_kabekesu"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_o_hikari"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("S_hakahuta"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_map_fog_onoff, 0)
    END_SWITCH
    RETURN
EVT_END()
