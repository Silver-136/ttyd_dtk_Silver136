#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mobj.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "driver/mapdrv.h"
#include "system.h"

#include "data/mapdata.h"
#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "manager/evtmgr_cmd.h"
#include "las.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

EvtWord box_syutugen[177];
EvtWord move_dai[130];
EvtWord hikari_evt[54];

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("las_00", las_00_init_evt);
    relSetEvtAddr("las_01", las_01_init_evt);
    relSetEvtAddr("las_02", las_02_init_evt);
    relSetEvtAddr("las_03", las_03_init_evt);
    relSetEvtAddr("las_04", las_04_init_evt);
    relSetEvtAddr("las_05", las_05_init_evt);
    relSetEvtAddr("las_06", las_06_init_evt);
    relSetEvtAddr("las_07", las_07_init_evt);
    relSetEvtAddr("las_08", las_08_init_evt);
    relSetEvtAddr("las_09", las_09_init_evt);
    relSetEvtAddr("las_10", las_10_init_evt);
    relSetEvtAddr("las_11", las_11_init_evt);
    relSetEvtAddr("las_12", las_12_init_evt);
    relSetEvtAddr("las_13", las_13_init_evt);
    relSetEvtAddr("las_14", las_14_init_evt);
    relSetEvtAddr("las_15", las_15_init_evt);
    relSetEvtAddr("las_16", las_16_init_evt);
    relSetEvtAddr("las_17", las_17_init_evt);
    relSetEvtAddr("las_18", las_18_init_evt);
    relSetEvtAddr("las_19", las_19_init_evt);
    relSetEvtAddr("las_20", las_20_init_evt);
    relSetEvtAddr("las_21", las_21_init_evt);
    relSetEvtAddr("las_22", las_22_init_evt);
    relSetEvtAddr("las_23", las_23_init_evt);
    relSetEvtAddr("las_24", las_24_init_evt);
    relSetEvtAddr("las_25", las_25_init_evt);
    relSetEvtAddr("las_26", las_26_init_evt);
    relSetEvtAddr("las_27", las_27_init_evt);
    relSetEvtAddr("las_28", las_28_init_evt);
    relSetEvtAddr("las_29", las_29_init_evt);
    relSetEvtAddr("las_30", las_30_init_evt);
    relSetBtlAddr("las", btlsetup_las_tbl, btl_las_setup_no_tbl);
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _unresolved(void) {
}

USER_FUNC(mobj_set_color) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 r = evtGetValue(event, args[1]);
    s32 g = evtGetValue(event, args[2]);
    s32 b = evtGetValue(event, args[3]);
    MapObjectEntry* mobj = mobjNameToPtr(name);

    animPoseSetMaterialEvtColor(mobj->poseId, (GXColor){r, g, b, 255});
    animPoseSetMaterialFlagOn(mobj->poseId, 0x40);
    return EVT_RETURN_DONE;
}

USER_FUNC(mobj_color_off) {
    MapObjectEntry* mobj = mobjNameToPtr((const char*)evtGetValue(event, event->args[0]));

    animPoseSetMaterialFlagOff(mobj->poseId, 0x40);
    return EVT_RETURN_DONE;
}

USER_FUNC(mobj_set_ry) {
    EvtWord* args = event->args;
    MapObjectEntry* mobj = mobjNameToPtr((const char*)evtGetValue(event, args[0]));

    mobj->rotation.y = evtGetFloat(event, args[1]);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(box_syutugen)
    SET_VALUE(LW(2), LW(0))
    CALL(evt_mobj_get_position, LW(2), LW(3), LW(4), LW(5))
    CALL(evt_eff, STRING("mist"), STRING("mist2"), 0, LW(3), 3, LW(5), 25, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_TBOX_APPEAR1"), LW(3), LW(4), LW(5), LW(8))
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        LOOP(0)
            CALL(evt_mobj_get_y_position, LW(2), LW(1))
            SUBTRACT(LW(1), LW(4))
            SUBTRACT(LW(1), 3)
            CALL(evt_eff, 0, STRING("mist2"), 1, LW(3), 0, LW(5), 15, LW(1), 100, 0, 0, 0, 0, 0)
            WAIT_MS(200)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(1750)
        CALL(evt_mobj_get_position, LW(2), LW(3), LW(4), LW(5))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_TBOX_APPEAR2"), LW(3), LW(4), LW(5), 0)
    END_INLINE_EVENT
    CALL(mobj_set_color, LW(2), 0, 0, 0)
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 1000, 3000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
    SET_FLOAT_VALUE(LW(3), LW(0))
    MULTIPLY_FLOAT(LW(3), FLOAT(30.0f))
    ADD_FLOAT(LW(3), FLOAT(-30.0f))
    CALL(evt_mobj_set_y_position, LW(2), LW(3))
    MULTIPLY_FLOAT(LW(0), FLOAT(360.0f))
    CALL(mobj_set_ry, LW(2), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    DELETE_EVENT(LW(9))
    CALL(evt_eff_softdelete, STRING("mist"))
    WAIT_MS(1000)
    CALL(evt_snd_sfxoff, LW(8))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 255, 1000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(mobj_set_color, LW(2), LW(0), LW(0), LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(mobj_color_off, LW(2))
    CALL(mobj_set_ry, LW(2), 0)
    CALL(evt_mobj_set_y_position, LW(2), 0)
    RETURN
EVT_END()

EVT_BEGIN(move_dai)
    SET_VALUE(LW(7), LW(0))
    SET_VALUE(LW(8), LW(1))
    SET_VALUE(LW(9), LW(2))
    CALL(evt_mapobj_trans, 1, LW(7), 0, 0, -100)
    CALL(evt_mapobj_flag_onoff, 1, 0, LW(7), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, LW(8), 0, 1)
    CALL(evt_mapobj_get_position, LW(7), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_FLOOR_MOVE1"), LW(0), LW(1), LW(2), LW(6))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -100, 0, 1000)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, LW(7), 0, 0, LW(0))
    CALL(evt_hit_bind_update, LW(8))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_snd_sfxoff, LW(6))
    WAIT_MS(LW(9))
    CALL(evt_mapobj_get_position, LW(7), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_FLOOR_MOVE1"), LW(0), LW(1), LW(2), LW(6))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, -100, 1000)
    LABEL(1)
    CALL(evt_sub_intpl_msec_get_value)
    CALL(evt_mapobj_trans, 1, LW(7), 0, 0, LW(0))
    CALL(evt_hit_bind_update, LW(8))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(evt_snd_sfxoff, LW(6))
    CALL(evt_mapobj_flag_onoff, 1, 1, LW(7), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, LW(8), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(hikari_evt)
    LOOP(0)
        CALL(evt_sub_random, 5, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(1), STRING("hikari_01"))
            CASE_EQUAL(1)
            SET_VALUE(LW(1), STRING("hikari_02"))
            CASE_EQUAL(2)
            SET_VALUE(LW(1), STRING("hikari_03"))
            CASE_EQUAL(3)
            SET_VALUE(LW(1), STRING("hikari_04"))
            CASE_EQUAL(4)
            SET_VALUE(LW(1), STRING("hikari_05"))
            CASE_EQUAL(5)
            SET_VALUE(LW(1), STRING("hikari_06"))
        END_SWITCH
        CALL(evt_map_playanim, LW(1), 0, 0)
        CALL(evt_map_checkanim, LW(1), LW(2), LW(3))
        WAIT_MS(LW(3))
    END_LOOP
    RETURN
EVT_END()

