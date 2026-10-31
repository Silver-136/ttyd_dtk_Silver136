// dou.rel (洞窟, Pirate's Grotto): _prolog registers each map's init script (dou_00 to dou_13) and the area's battle tables.
// Also the shared bobbing-platform (puka) events.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"

#include "data/mapdata.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "system.h"
#include "dou.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 dou_normal_puka_data[16];
EvtWord dou_normal_puka[19];
EvtWord dou_puka_event_sub[54];
EvtWord dou_puka_event[40];
EvtWord dou_mario_pos_osreport[4];

/**
 * Up-and-down offsets, in 16 steps, that make a Pirate's Grotto platform bob on the water; one step is
 * applied every 130 ms.
 */
f32 dou_normal_puka_data[] = {
    -0.0f, -0.5f, -1.0f, -1.5f, -2.0f, -2.2f, -2.5f, -2.7f,
    -2.7f, -2.7f, -2.5f, -2.2f, -2.0f, -1.5f, -1.0f, -0.5f,
};

/**
 * Idle bobbing of a Pirate's Grotto floating platform (puka):
 * LW(6) holds a reference to the register that stores the platform's height offset.
 * Every 130 ms it writes the next of 16 offsets from dou_normal_puka_data into it,
 * a slow sinking and rising cycle, forever. The map script translates the platform by that register.
 */
EVT_BEGIN(dou_normal_puka)
    SET_VALUE(LW(0), 1)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(1))
        SET_FLOAT_INDEXED(LW(6), LW(1))
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

/**
 * One dip of a floating platform: sinks the offset in LW(6) to LW(2) over 300 ms, holds for LW(3) ms,
 * then rises back to 0 over 300 ms. The debug messages are "sink" and "float" in Japanese.
 */
EVT_BEGIN(dou_puka_event_sub)
    DEBUG_PUT_MSG(STRING("しずむ"))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 0, LW(2), 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_INDEXED(LW(6), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(LW(3))
    DEBUG_PUT_MSG(STRING("うく"))
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_IN, LW(2), 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_INDEXED(LW(6), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * REL module prolog, run when dou.rel is loaded:
 * runs the static constructors, registers the init scripts of dou_00 to dou_13
 * and the Pirate's Grotto battle setup tables.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("dou_00", dou_00_init_evt);
    relSetEvtAddr("dou_01", dou_01_init_evt);
    relSetEvtAddr("dou_02", dou_02_init_evt);
    relSetEvtAddr("dou_03", dou_03_init_evt);
    relSetEvtAddr("dou_04", dou_04_init_evt);
    relSetEvtAddr("dou_05", dou_05_init_evt);
    relSetEvtAddr("dou_06", dou_06_init_evt);
    relSetEvtAddr("dou_07", dou_07_init_evt);
    relSetEvtAddr("dou_08", dou_08_init_evt);
    relSetEvtAddr("dou_09", dou_09_init_evt);
    relSetEvtAddr("dou_10", dou_10_init_evt);
    relSetEvtAddr("dou_11", dou_11_init_evt);
    relSetEvtAddr("dou_12", dou_12_init_evt);
    relSetEvtAddr("dou_13", dou_13_init_evt);
    relSetBtlAddr("dou", btlsetup_dou_tbl, btl_dou_setup_no_tbl);
}

/**
 * Shared reaction of a floating platform when Mario steps on it:
 * four dips of shrinking depth and length (-10 for 200 ms, -8 for 150, -5 for 100, -2 for 50),
 * so the platform settles like a boat taking a weight.
 * LW(6) must hold the reference to the platform's offset register (set by the caller).
 */
EVT_BEGIN(dou_puka_event)
    SET_VALUE(LW(7), LW(0))
    SET_VALUE(LW(8), LW(1))
    SET_VALUE(LW(2), -10)
    SET_VALUE(LW(3), 200)
    CALL_CHILD(PTR(dou_puka_event_sub))
    SET_VALUE(LW(2), -8)
    SET_VALUE(LW(3), 150)
    CALL_CHILD(PTR(dou_puka_event_sub))
    SET_VALUE(LW(2), -5)
    SET_VALUE(LW(3), 100)
    CALL_CHILD(PTR(dou_puka_event_sub))
    SET_VALUE(LW(2), -2)
    SET_VALUE(LW(3), 50)
    CALL_CHILD(PTR(dou_puka_event_sub))
    RETURN
EVT_END()

EVT_BEGIN(dou_mario_pos_osreport)
    CALL(dou_mario_pos_osreport_func)
    RETURN
EVT_END()

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
/**
 * Module unresolved-symbol stub: if a relocation could not be resolved the REL linker points it here;
 * it does nothing.
 */
void _unresolved(void) {
}
/**
 * Script helper: stores the bobbing offset for step LW(0) (taken modulo 16) from dou_normal_puka_data into
 * args[0] as a float.
 */
USER_FUNC(evt_dou_normal_puka) {
    // LW(0) is the sample counter; its low four bits wrap the 16-entry bobbing curve.
    evtSetFloat(event, event->args[0], dou_normal_puka_data[event->lwData[0] & 0xF]);
    return EVT_RETURN_DONE;
}
/**
 * Debug script function named for printing Mario's position with OSReport: in this build it only looks up
 * Mario and reads the button trigger, prints nothing and blocks forever.
 * Only the unused dou_mario_pos_osreport calls it.
 */
USER_FUNC(dou_mario_pos_osreport_func) {
    // This diagnostic helper polls input but remains blocked; it does not finish the event.
    marioGetPtr();

    keyGetButtonTrg(0);
    return EVT_RETURN_BLOCK;
}
