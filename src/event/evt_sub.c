// Miscellaneous script commands and scripts: tree and bush shaking, mail, star stones, splines,
// countdowns, play time, area and language queries.

#include "version.h"
#include "eff/eff_stageclear.h"
#include "driver/fadedrv.h"
#include "evt/evt_window.h"
#include "eff/eff_itemget.h"
#include "icon_ids.h"
#include "evt/evt_cmd.h"
#include "evt/evt_save_flags.h"
#include "countdown.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_starstone.h"
#include "driver/itemdrv.h"
#include "driver/npcdrv.h"
#include "driver/swdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "driver/icondrv.h"
#include "memory.h"
#include "mario/mario_pouch.h"
#include "evt/evt_mario.h"
#include "evt/evt_memcard.h"
#include "evt/evt_sub.h"
#include <dolphin/pad.h>
#include "statuswindow.h"
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "mario/mario_motion.h"
#include "evt/evt_bero.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_cam.h"
#include "evt/evt_pouch.h"
#include "mario/mario_cam.h"
#include "poly_trig.h"
#include "driver/mapdrv.h"
#include "driver/casedrv.h"


#include "system.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "manager/winmgr.h"
#include <stdio.h>
#include <dolphin/os.h>


s32 stop;

typedef struct SplineWork {
    s32 n;         // 0x0
    f32* ts;       // 0x4
    Vec* pts;      // 0x8
    Vec* tbl;      // 0xC
    s32 cur;       // 0x10
    s32 steps;     // 0x14
    s32 mode;      // 0x18
    s32 msec;      // 0x1C
    OSTime start;  // 0x20
    s32 manual;    // 0x28
} SplineWork;

// FAKE: the pragma below keeps an orphan string that is not evt_sub's. "%f%f%f" belongs to evt_npc's dead-stripped
// evt_npc_set_position_xls (JP demo map: UNUSED, 0xb4), and in the early demos it ends evt_npc's .rodata (the
// npc_report_pos stand-in there). From JP on, the split puts the evt_npc/evt_sub boundary in front of it, so it is
// kept here. The function's body is not known (sscanf forms tried compile to 0xa0-0xb0).
#pragma force_active on

#if VERSION >= VERSION_JP
static const char coin_fmt[] = "%f%f%f";
#endif

#pragma force_active reset

/** Black text colour of the "You got N coins!" line in the coin reward window (coingetDisp). */
static const GXColor coin_black = {0x00, 0x00, 0x00, 0xFF};

USER_FUNC(bgm_start_wait);
USER_FUNC(stone_bg);
static void coingetDisp(WinMgrEntry* entry);

#if VERSION < VERSION_US
s32 evt_sub_mail_event_end[1];
#endif

#if VERSION <= VERSION_DEMO_JP
EVT_BEGIN(evt_sub_stopwatch)
    LOOP(0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Sway of a hammered tree, one row per frame for 25 frames: the trunk's tilt in degrees and the leaves' two
 * offsets, swinging out and settling back to rest. tree_evt reads it.
 */
s32 tree_data[] = {
    FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f),
    FLOAT(-0.5341796875f), FLOAT(1.4462890625f), FLOAT(1.623046875f),
    FLOAT(-1.1708984375f), FLOAT(3.1748046875f), FLOAT(3.56640625f),
    FLOAT(-1.6044921875f), FLOAT(4.3388671875f), FLOAT(4.87109375f),
    FLOAT(-1.533203125f), FLOAT(4.0908203125f), FLOAT(4.580078125f),
    FLOAT(-0.685546875f), FLOAT(1.74609375f), FLOAT(1.892578125f),
    FLOAT(0.7158203125f), FLOAT(-2.0673828125f), FLOAT(-2.482421875f),
    FLOAT(2.228515625f), FLOAT(-6.2314453125f), FLOAT(-7.2392578125f),
    FLOAT(3.4111328125f), FLOAT(-9.6240234375f), FLOAT(-11.072265625f),
    FLOAT(3.822265625f), FLOAT(-11.1240234375f), FLOAT(-12.6748046875f),
    FLOAT(3.1015625f), FLOAT(-9.98828125f), FLOAT(-11.146484375f),
    FLOAT(1.5439453125f), FLOAT(-6.962890625f), FLOAT(-7.357421875f),
    FLOAT(-0.3095703125f), FLOAT(-3.1650390625f), FLOAT(-2.658203125f),
    FLOAT(-1.9228515625f), FLOAT(0.2880859375f), FLOAT(1.59765625f),
    FLOAT(-2.75390625f), FLOAT(2.279296875f), FLOAT(4.060546875f),
    FLOAT(-2.5517578125f), FLOAT(2.4150390625f), FLOAT(4.2607421875f),
    FLOAT(-1.6767578125f), FLOAT(1.439453125f), FLOAT(3.0986328125f),
    FLOAT(-0.5048828125f), FLOAT(-0.0546875f), FLOAT(1.2783203125f),
    FLOAT(0.583984375f), FLOAT(-1.4775390625f), FLOAT(-0.4921875f),
    FLOAT(1.21484375f), FLOAT(-2.2353515625f), FLOAT(-1.509765625f),
    FLOAT(1.3037109375f), FLOAT(-2.2197265625f), FLOAT(-1.6611328125f),
    FLOAT(1.1015625f), FLOAT(-1.8271484375f), FLOAT(-1.416015625f),
    FLOAT(0.734375f), FLOAT(-1.2177734375f), FLOAT(-0.943359375f),
    FLOAT(0.3251953125f), FLOAT(-0.5546875f), FLOAT(-0.4150390625f),
    FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f),
};

/**
 * Stopwatch script that just waits a frame forever: scripts spawn it with SPAWN_EVENT_GET_ID
 * and read how long it has run with evt_sub_get_stopwatch (e.g. Peach's timed tasks in aji_06, aji_16 and aji_17).
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(evt_sub_stopwatch)
    LOOP(0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

#if VERSION <= VERSION_DEMO_JP
EVT_BEGIN(tree_evt)
    SET_VALUE(LF(0), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(4), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1_LEAVES1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_mapobj_get_position, LW(3), LW(10), LW(11), LW(12))
        SET_VALUE(LW(14), LW(11))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        SET_VALUE(LW(15), LW(12))
        SUBTRACT(LW(14), 25)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 0, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(1)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(2)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(3)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(4)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(5)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(6)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
        END_SWITCH
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 0)
        SPAWN_EVENT(LW(5))
    END_IF
    SET_INT_READ_ADDR(PTR(tree_data))
    LOOP(25)
        READ_INT3(LW(10), LW(11), LW(12))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_rotate, 1, LW(2), FLOAT(0.0f), FLOAT(0.0f), LW(10))
        END_IF
        IF_INT_NOT_EQUAL(LW(3), 0)
            CALL(evt_mapobj_trans, 1, LW(3), LW(11), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        IF_INT_NOT_EQUAL(LW(4), 0)
            CALL(evt_mapobj_trans, 1, LW(4), LW(12), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
EVT_BEGIN(evt_sub_tree_access_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        READ_INT3(LW(3), LW(4), LW(5))
        IF_INT_EQUAL(LW(0), 7)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_HAMMER, 1, LW(1), 0, PTR(tree_evt), 0)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Sideways sway of a checked bush, one offset per frame for 12 frames, out and back to rest;
 * kusa_evt reads it.
 */
static s32 kusa_data[] = {
    FLOAT(0.0f), FLOAT(-1.873046875f), FLOAT(-2.998046875f), FLOAT(-2.2197265625f), FLOAT(-0.775390625f), FLOAT(0.0f), FLOAT(-0.775390625f), FLOAT(-2.2197265625f),
    FLOAT(-2.998046875f), FLOAT(-2.4423828125f), FLOAT(-1.2197265625f), FLOAT(0.0f),
};

#if VERSION <= VERSION_DEMO_JP
EVT_BEGIN(kusa_evt)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_GRASS_SHAKE1"), LW(10), LW(11), LW(12), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SPAWN_EVENT(LW(3))
    END_IF
    SET_INT_READ_ADDR(PTR(kusa_data))
    LOOP(12)
        READ_INT(LW(10))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_trans, 1, LW(2), LW(10), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
EVT_BEGIN(evt_sub_kusa_access_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        READ_INT(LW(3))
        IF_INT_EQUAL(LW(0), 4)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(1), 0, PTR(kusa_evt), 0)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * The pale yellow "You got N coins!" window that evt_sub_get_coin opens for coin rewards;
 * coingetDisp draws it.
 */
WinMgrDesc coinget_window_desc[] = {
    {0, 0, 8, -175, 120, 350, 42, {0xE1, 0xD7, 0x9B, 0xFF}, NULL, coingetDisp},
};

/**
 * Shakes a tree hit with the hammer: plays the tree and leaves sounds,
 * drops falling-leaf effects (LW(0) picks the leaf variant), spawns the optional drop script in LW(5),
 * then sways map objects LW(2) (trunk, rotated)
 * and LW(3)/LW(4) (leaves, shifted) through tree_data for 25 frames. Run per tree by evt_sub_tree_access_entry.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(tree_evt)
    SET_TYPE(0)
    SET_VALUE(LF(0), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(4), 0)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_HIT_TREE1_LEAVES1"), LW(13), LW(14), LW(15), 0)
        CALL(evt_mapobj_get_position, LW(2), LW(13), LW(14), LW(15))
        CALL(evt_mapobj_get_position, LW(3), LW(10), LW(11), LW(12))
        SET_VALUE(LW(14), LW(11))
        CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
        SET_VALUE(LW(15), LW(12))
        SUBTRACT(LW(14), 25)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 0, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(1)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(2)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(3)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 4, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(4)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 3, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(5)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(6)
            CALL(evt_eff64, 0, STRING("fall_leaf_n64"), 2, LW(13), LW(14), LW(15), 60, 0, 0, 0, 0, 0, 0, 0)
        END_SWITCH
    END_IF
    IF_INT_NOT_EQUAL(LW(5), 0)
        SPAWN_EVENT(LW(5))
    END_IF
    SET_INT_READ_ADDR(PTR(tree_data))
    LOOP(25)
        READ_INT3(LW(10), LW(11), LW(12))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_rotate, 1, LW(2), FLOAT(0.0f), FLOAT(0.0f), LW(10))
        END_IF
        IF_INT_NOT_EQUAL(LW(3), 0)
            CALL(evt_mapobj_trans, 1, LW(3), LW(11), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        IF_INT_NOT_EQUAL(LW(4), 0)
            CALL(evt_mapobj_trans, 1, LW(4), LW(12), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Map setup helper: walks the tree table at LW(0) (rows of six words: leaf variant 0-6, hit object, trunk,
 * leaves, leaves, drop script; a first word of 7 ends it)
 * and registers a hammer case on each hit object that runs tree_evt.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(evt_sub_tree_access_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        READ_INT3(LW(3), LW(4), LW(5))
        IF_INT_EQUAL(LW(0), 7)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_HAMMER, 1, LW(1), 0, PTR(tree_evt), 0)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Shakes a bush Mario checks: plays the grass sound at Mario, spawns the optional drop script in LW(3),
 * and sways map object LW(2) sideways through kusa_data for 12 frames.
 * Run per bush by evt_sub_kusa_access_entry.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(kusa_evt)
    SET_TYPE(0)
    CALL(evt_mario_get_pos, 0, LW(10), LW(11), LW(12))
    CALL(evt_snd_sfxon_3d, STRING("SFX_GRASS_SHAKE1"), LW(10), LW(11), LW(12), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        SPAWN_EVENT(LW(3))
    END_IF
    SET_INT_READ_ADDR(PTR(kusa_data))
    LOOP(12)
        READ_INT(LW(10))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(evt_mapobj_trans, 1, LW(2), LW(10), FLOAT(0.0f), FLOAT(0.0f))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * Map setup helper: walks the bush table at LW(0) (rows of four words: kind, hit object, map object,
 * drop script; a first word of 4 ends it) and registers an A-button case in front of each one that runs
 * kusa_evt.
 */
#if VERSION >= VERSION_JP
EVT_BEGIN(evt_sub_kusa_access_entry)
    SET_INT_READ_ADDR(LW(0))
    LOOP(0)
        READ_INT3(LW(0), LW(1), LW(2))
        READ_INT(LW(3))
        IF_INT_EQUAL(LW(0), 4)
            BREAK_LOOP
        END_IF
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, LW(1), 0, PTR(kusa_evt), 0)
    END_LOOP
    RETURN
EVT_END()
#endif

/**
 * The Crystal Star pickup cutscene.
 * LW(0) is the star (1 Diamond ... 7 Crystal, -1 the chapter 4 scene where the star is shown with Doopliss as
 * Mario, NPC "nise Mario"), LW(1) the item entry, LW(2) the pickup flag.
 * Adds the star to the pouch, spins it up around Mario with the fanfare, prints the chapter's lines,
 * runs the stage-clear curtain and save prompt, then changes map:
 * Peach's interlude at aji_10 after chapters 1-3, usu_00, muj_12 and so on after the others.
 * US and later time the spin in milliseconds and scale its steps by the frame rate.
 */
EVT_BEGIN(evt_sub_starstone)
    // Mario stops and the star goes into his pouch; the Doopliss scene (-1) adds nothing.
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_pouch_get_starstone, 1)
        CALL(evt_pouch_add_item, ITEM_DIAMOND_STAR, LW(2))
        CASE_EQUAL(2)
        CALL(evt_pouch_get_starstone, 2)
        CALL(evt_pouch_add_item, ITEM_EMERALD_STAR, LW(2))
        CASE_EQUAL(3)
        CALL(evt_pouch_get_starstone, 3)
        CALL(evt_pouch_add_item, ITEM_GOLD_STAR, LW(2))
        CASE_EQUAL(4)
        CALL(evt_pouch_get_starstone, 4)
        CALL(evt_pouch_add_item, ITEM_RUBY_STAR, LW(2))
        CASE_EQUAL(5)
        CALL(evt_pouch_get_starstone, 5)
        CALL(evt_pouch_add_item, ITEM_SAPPHIRE_STAR, LW(2))
        CASE_EQUAL(6)
        CALL(evt_pouch_get_starstone, 6)
        CALL(evt_pouch_add_item, ITEM_GARNET_STAR, LW(2))
        CASE_EQUAL(7)
        CALL(evt_pouch_get_starstone, 7)
        CALL(evt_pouch_add_item, ITEM_CRYSTAL_STAR, LW(2))
        CASE_EQUAL(-1)
    END_SWITCH

    // Remember which star this is for the rest of the scene.
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LF(1), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        SET_VALUE(LF(2), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        SET_VALUE(LF(3), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 4)
        SET_VALUE(LF(4), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 5)
        SET_VALUE(LF(5), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 6)
        SET_VALUE(LF(6), 1)
    END_IF
    IF_INT_EQUAL(LW(0), 7)
        SET_VALUE(LF(7), 1)
    END_IF
    IF_INT_EQUAL(LW(0), -1)
        SET_VALUE(LF(8), 1)
    END_IF

    // The scene lasts at least 9.7 seconds from here; the screen flashes white with a chime.
    CALL(set_stop, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(9700)
        CALL(set_stop, 1)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_EVT_STARSTONE_WHITEOUT1"), 0)
    CALL(evt_fade_entry, FADE_OUT_BLACK, 300, 255, 255, 255)
    CALL(evt_fade_end_wait)

    // In the flash the star appears beside Mario (or Doopliss posing as him), he strikes a pose and the
    // Crystal Star fanfare starts.
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
    ELSE
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    END_IF
    CALL(evt_sub_get_sincos, 0, LW(5), LW(6))
    MULTIPLY_FLOAT(LW(5), FLOAT(40.0f))
    MULTIPLY_FLOAT(LW(6), FLOAT(40.0f))
    ADD_FLOAT(LW(5), LW(2))
    ADD_FLOAT(LW(6), LW(4))
    CALL(evt_item_set_position, LW(1), LW(5), LW(3), LW(6))
    CALL(evt_mario_adjust_dir)
    WAIT_MS(500)
    IF_INT_EQUAL(LF(8), 0)
        CALL(evt_mario_set_pose, STRING("M_N_3"))
    END_IF
#if VERSION < VERSION_US
    CALL(evt_fade_entry, FADE_IN_BLACK, 300, 255, 255, 255)
#endif
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GET_STARSTONE1"))
    CALL(bgm_start_wait)

    // Fade back in; the camera closes in while the star spirals up around him, spinning,
    // until it hovers overhead.
#if VERSION >= VERSION_US
    CALL(evt_fade_entry, FADE_IN_BLACK, 300, 255, 255, 255)
#endif
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_cam3d_evt_set_npc_rel, STRING("にせマリオ"), 0, 50, 397, 0, 70, 25, 2000, EASING_SINE_IN_OUT)
    ELSE
        CALL(evt_cam3d_evt_set_rel, 0, 50, 397, 0, 70, 25, 2000, EASING_SINE_IN_OUT)
    END_IF
    CALL(evt_snd_sfxon_3d, STRING("SFX_SE2_EVT_STARSTONE_GET1"), LW(5), LW(3), LW(6), 0)
    SET_VALUE(LW(7), LW(1))
    SET_FLOAT_VALUE(LW(8), FLOAT(35.0f))
    SET_FLOAT_VALUE(LW(9), LW(3))
#if VERSION >= VERSION_US
    CALL(evt_sub_intpl_msec_init, EASING_QUADRATIC_OUT, 0, 3600, 2833)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
#else
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 0, 3600, 170)
    LOOP(171)
        CALL(evt_sub_intpl_get_value)
#endif
        IF_INT_EQUAL(LF(8), 1)
            CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
        ELSE
            CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
        END_IF
        CALL(evt_sub_get_sincos, LW(0), LW(5), LW(6))
        MULTIPLY_FLOAT(LW(5), LW(8))
        MULTIPLY_FLOAT(LW(6), LW(8))
        ADD_FLOAT(LW(5), LW(2))
        ADD_FLOAT(LW(6), LW(4))
        CALL(evt_item_set_position, LW(7), LW(5), LW(9), LW(6))
        CALL(stone_ry, LW(7), LW(0))
#if VERSION >= VERSION_US
        SET_FLOAT_VALUE(LW(2), FLOAT(0.20507813f))
        SET_FLOAT_VALUE(LW(3), FLOAT(0.28222656f))
        CALL(evt_sub_scale_by_fps, LW(2), LW(3))
        SUBTRACT_FLOAT(LW(8), LW(2))
        ADD_FLOAT(LW(9), LW(3))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_get_position, STRING("にせマリオ"), LW(2), LW(3), LW(4))
    ELSE
        CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    END_IF
    ADD(LW(3), 48)
    CALL(evt_item_set_position, LW(7), LW(2), LW(3), LW(4))
#else
        SUBTRACT_FLOAT(LW(8), FLOAT(0.20507813f))
        ADD_FLOAT(LW(9), FLOAT(0.28222656f))
        WAIT_FRAMES(1)
    END_LOOP
#endif

    // Mario cheers and holds the star high in a burst of light, and the chapter's "You got a Crystal Star!"
    // message shows.
    IF_INT_NOT_EQUAL(LF(8), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAPPY1"), LW(0), LW(1), LW(2), 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_npc_set_anim, STRING("にせマリオ"), STRING("EM_I_1"))
    ELSE
        CALL(evt_mario_set_pose, STRING("M_P_2"))
    END_IF
    CALL(evt_item_get_position, LW(7), LW(0), LW(1), LW(2))
    ADD(LW(1), 16)
    CALL(evt_eff64, STRING("sub_hikari"), STRING("radiation_n64"), 12, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    ADD(LW(2), -10)
    CALL(evt_eff, STRING("sub_bg"), STRING("itemget"), 0, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(stone_bg, LW(7))
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_msg_print, 0, STRING("stg1_gon_33_18"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_03"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_475"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_39"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_msg_print, 0, STRING("stg5_dou_29_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_20"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_42_02"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_10_02"), 0, 0)
    END_IF

    // Wait out the fanfare, then the stage-clear jingle plays and the light fades.
    LOOP(0)
        CALL(get_stop, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_STAGE_CLEAR1"))
#if VERSION >= VERSION_JP
    CALL(evt_snd_envoff, PSND_FADEOUT)
#endif
    CALL(evt_eff_softdelete, STRING("sub_hikari"))
    CALL(stone_bg_open, LW(7))

    // The curtain comes down (the map's own star object is hidden behind it),
    // STAGE CLEAR appears and Mario cheers.
    BEGIN_INLINE_EVENT
        IF_INT_EQUAL(LF(1), 1)
            SET_VALUE(LW(0), STRING("gon_11_S"))
        END_IF
        IF_INT_EQUAL(LF(2), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(3), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(4), 1)
            SET_VALUE(LW(0), STRING("S_jin04"))
        END_IF
        IF_INT_EQUAL(LF(5), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(6), 1)
            SET_VALUE(LW(0), STRING("S"))
        END_IF
        IF_INT_EQUAL(LF(7), 1)
            SET_VALUE(LW(0), STRING("S_aji_14"))
        END_IF
        IF_INT_EQUAL(LF(8), 1)
            SET_VALUE(LW(0), STRING("S_jin04"))
        END_IF
        WAIT_FRAMES(10)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, LW(0), MAP_OBJ_FLAG_HIDDEN)
    END_INLINE_EVENT
    CALL(evt_fade_entry, FADE_CURTAIN_ENTER_3D, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(1000)
    CALL(evt_eff, STRING("sub_clear"), STRING("stageclear"), 10, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(stageclear_wait)
    IF_INT_NOT_EQUAL(LF(8), 1)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_HAPPY2"), LW(0), LW(1), LW(2), 0)
    END_IF
    WAIT_MS(2000)

    // The curtain closes and the chapter's diary page sums up what happened.
    CALL(evt_fade_entry, FADE_CURTAIN_CENTER_A2_3D, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_item_delete, LW(7))
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_msg_print, 0, STRING("stg1_gon_34"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_msg_print, 0, STRING("stg2_mri_e25_04"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_msg_print, 0, STRING("stg3_tou_475_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_39_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_msg_print, 0, STRING("stg5_dou_29_02"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_msg_print, 0, STRING("stg6_pik_20_01"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_msg_print, 0, STRING("stg7_aji_42_03"), 0, 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        CALL(evt_msg_print, 0, STRING("stg4_jin_10_03"), 0, 0)
    END_IF

    // Offer to save; the save kind tells which chapter was just finished.
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(1), 1)
        SET_VALUE(LW(0), 1)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        SET_VALUE(LW(0), 4)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        SET_VALUE(LW(0), 7)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        SET_VALUE(LW(0), 10)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        SET_VALUE(LW(0), 13)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        SET_VALUE(LW(0), 16)
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        SET_VALUE(LW(0), 19)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
        SET_VALUE(LW(0), 22)
    END_IF
    SET_VALUE(LF(10), 0)
#if VERSION >= VERSION_US
    CALL_CHILD(evt_memcard_save_wrapper)
#else
    CALL_CHILD(evt_memcard_save)
#endif

    // Move on: Peach's interlude (aji_10) after chapters 1-3, then usu_00, muj_12, pik_03 and pik_01 after
    // chapters 4-7. The Doopliss scene instead fades back in where it is.
    CALL(evt_mario_init_camid)
    CALL(evt_party_init_camid, 0)
    CALL(evt_party_init_camid, 1)
    IF_INT_EQUAL(LF(1), 1)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, FADE_OUT_WHITE, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, FADE_STORY_MARIO_A1, 0, FADE_STORY_MARIO_A3, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(2), 1)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, FADE_OUT_WHITE, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, FADE_STORY_MARIO_A1, 0, FADE_STORY_MARIO_A3, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(3), 1)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, FADE_OUT_WHITE, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, FADE_STORY_MARIO_A1, 0, FADE_STORY_MARIO_A3, 0)
        CALL(evt_bero_mapchange, STRING("aji_10"), 0)
    END_IF
    IF_INT_EQUAL(LF(4), 1)
        CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_WHITE, 800, FADE_IN_WHITE, 800)
        CALL(evt_bero_mapchange, STRING("usu_00"), 0)
    END_IF
    IF_INT_EQUAL(LF(5), 1)
        CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_WHITE, 800, FADE_IN_WHITE, 800)
        CALL(evt_bero_mapchange, STRING("muj_12"), 0)
    END_IF
    IF_INT_EQUAL(LF(6), 1)
        CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_WHITE, 800, FADE_IN_WHITE, 800)
        CALL(evt_bero_mapchange, STRING("pik_03"), STRING("next"))
    END_IF
    IF_INT_EQUAL(LF(7), 1)
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_sfx_all_stop)
        CALL(evt_fade_entry, FADE_OUT_WHITE, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_fade_set_mapchange_type, 1, FADE_STORY_MARIO_A1, 0, FADE_STORY_MARIO_A3, 0)
        CALL(evt_bero_mapchange, STRING("pik_01"), 0)
    END_IF
    IF_INT_EQUAL(LF(8), 1)
#if VERSION >= VERSION_JP
        CALL(evt_snd_bgmoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_envoff_f, PSND_FADEOUT, 2000)
        CALL(evt_snd_sfx_all_stop)
#endif
        CALL(evt_fade_entry, FADE_OUT_WHITE, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(2000)
        CALL(evt_eff_delete, STRING("sub_bg"))
        CALL(evt_eff_delete, STRING("sub_clear"))
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("S_jin04"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_fade_reset, 1)
        CALL(evt_npc_set_camid, STRING("にせマリオ"), 4)
        CALL(evt_mario_set_pose, STRING("M_D_2"))
        CALL(evt_fade_entry, FADE_IN_WHITE, 1000, 0, 0, 0)
        CALL(evt_fade_end_wait)
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Rumbles controller 0 in ten bursts of ten 50 ms pulses with two seconds between,
 * the buzz of an incoming email. Spawned by mail_evt while the mail message is up.
 */
EVT_BEGIN(evt_sub_mail_vibration)
    LOOP(10)
        LOOP(10)
            CALL(evt_sub_rumble_onoff, 0, 0)
            WAIT_MS(50)
            CALL(evt_sub_rumble_onoff, 1, 0)
            WAIT_MS(50)
        END_LOOP
        CALL(evt_sub_rumble_onoff, 2, 0)
        WAIT_MS(2000)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(mail_evt)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_mario_wait_rideon)
    CALL(evt_mario_balloon_bikkuri)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_MAIL_RECEPTION3"))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    CALL(evt_win_one_message, 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    IF_INT_EQUAL(GSWF_MAIL_TUTORIAL_SEEN, 0)
        SET_VALUE(GSWF_MAIL_TUTORIAL_SEEN, 1)
        CALL(evt_msg_print, 0, STRING("mail_first_hit"), 0, 0)
    ELSE
        IF_INT_EQUAL(GSWF_MAIL_TUTORIAL_REPEAT_OFF, 0)
            CALL(evt_msg_print, 0, STRING("mail_first_hit"), 0, 0)
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_cam_letter_box_disable, 0)
    RETURN
EVT_END()

/**
 * Petalburg (nok_01): once Kroop's talk is done (STG1_02),
 * sends Frankly's mail (16, plus 26) when Mario walks past x = 190.
 */
EVT_BEGIN(mail_evt_nok_01)
    IF_INT_EQUAL(GSWF_MAIL_FRANKLY_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 190, -2000, 190, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG1_02_TALKING_WITH_KROOP)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_FRANKLY_SENT, 1)
    CALL(evt_pouch_receive_mail, 16)
    CALL(evt_pouch_receive_mail, 26)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Rogueport (gor_03): after the Glitzville ticket (STG2_47) sends Koopley's mail (17)
 * when Mario crosses x = -100; also delivers any Trouble Center follow-up mail that is due (irai_mail_check).
 * Loops for as long as the map runs.
 */
EVT_BEGIN(mail_evt_gor_03)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -100, -2000, -100, 2000, LW(5))
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_47_AFTER_GETTING_THE_TICKET)
                IF_INT_EQUAL(GSWF_MAIL_KOOPLEY_SENT, 0)
                    SET_VALUE(GSWF_MAIL_KOOPLEY_SENT, 1)
                    CALL(evt_pouch_receive_mail, 17)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(mail_evt)
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Rogueport (gor_02): at STG3_51 sends Koopie Koo's mail (18)
 * when Mario crosses the line from (-280, -50) to (-230, 220); also delivers due Trouble Center follow-up mail.
 * Loops for as long as the map runs.
 */
EVT_BEGIN(mail_evt_gor_02)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -280, -50, -230, 220, LW(5))
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_51_FRANKLYS_HOUSE_STG_3)
                IF_INT_EQUAL(GSWF_MAIL_KOOPIE_KOO_SENT, 0)
                    SET_VALUE(GSWF_MAIL_KOOPIE_KOO_SENT, 1)
                    CALL(evt_pouch_receive_mail, 18)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(mail_evt)
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Rogueport (gor_01): at STG4_37 sends Petuni's mail (19) when Mario crosses x = 350. */
EVT_BEGIN(mail_evt_gor_01)
    IF_INT_EQUAL(GSWF_MAIL_PETUNI_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 350, -2000, 350, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_37_TO_THE_PORT)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_PETUNI_SENT, 1)
    CALL(evt_pouch_receive_mail, 19)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Rogueport (gor_04): at STG5_66 sends the Boo mail (20) when Mario crosses x = 30,
 * but only while Mario has control (mail_evt_gor_04_keyoff_check);
 * also delivers due Trouble Center follow-up mail. Loops for as long as the map runs.
 */
EVT_BEGIN(mail_evt_gor_04)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 30, -2000, 30, 2000, LW(5))
        CALL(mail_evt_gor_04_keyoff_check, LW(3))
        IF_INT_GREATER(LW(3), 0)
            SET_VALUE(LW(5), 0)
        END_IF
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_66_BELDAM_FINDS_OUT)
                IF_INT_EQUAL(GSWF_MAIL_BOO_SENT, 0)
                    SET_VALUE(GSWF_MAIL_BOO_SENT, 1)
                    CALL(evt_pouch_receive_mail, 20)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(mail_evt)
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Excess Express (rsh_02_a in US and later, rsh_03_a in JP and the early demos):
 * from day 3 (STG6_45) sends Pa-Patch's mail (21)
 * when Mario crosses the line at x = -100 (JP and the early demos: 400).
 */
EVT_BEGIN(mail_evt_rsh_03_a)
    IF_INT_EQUAL(GSWF_MAIL_PA_PATCH_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#if VERSION >= VERSION_US
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -100, -2000, -100, 2000, LW(5))
#else
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 400, -2000, 400, 2000, LW(5))
#endif
        IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_45_DAY_3_STARTS)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_PA_PATCH_SENT, 1)
    CALL(evt_pouch_receive_mail, 21)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Riverside Station (hom_00): once the drawbridge is down (STG6_42) sends King K's mail (22)
 * when Mario crosses x = -600.
 */
EVT_BEGIN(mail_evt_hom_00)
    IF_INT_EQUAL(GSWF_MAIL_KING_K_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -600, -2000, -600, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_42_DRAWBRIDGE_DOWN)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_KING_K_SENT, 1)
    CALL(evt_pouch_receive_mail, 22)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Rogueport (gor_02): at STG6_71 sends Zip Toad's mail (23) when Mario crosses the line from
 * (-1000, -4226) to (1000, 7374). The PAL demo and PAL also wait until Mario is out of motion 25.
 */
EVT_BEGIN(mail_evt_gor_02_2)
    IF_INT_EQUAL(GSWF_MAIL_ZIP_TOAD_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -1000, -4226, 1000, 7374, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_71_FRANKLYS_HOUSE_STG_6)
            IF_INT_EQUAL(LW(5), 1)
#if VERSION >= VERSION_DEMO_EU
                CALL(evt_mario_get_motion, LW(5))
                IF_INT_NOT_EQUAL(LW(5), 25)
#endif
                BREAK_LOOP
#if VERSION >= VERSION_DEMO_EU
                END_IF
#endif
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_ZIP_TOAD_SENT, 1)
    CALL(evt_pouch_receive_mail, 23)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Fahr Outpost (bom_01): at STG7_02 after the cannon is approved sends Rawk Hawk's mail (24)
 * when Mario crosses x = -230.
 */
EVT_BEGIN(mail_evt_bom_01)
    IF_INT_EQUAL(GSWF_MAIL_RAWK_HAWK_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -230, -2000, -230, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_02_MAYOR_BOB)
            IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 1)
                IF_INT_EQUAL(LW(5), 1)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_RAWK_HAWK_SENT, 1)
    CALL(evt_pouch_receive_mail, 24)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_tik_00)
    IF_INT_EQUAL(GSWF_MAIL_GHOST_T_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 470, -2000, 470, 2000, LW(5))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -500, -2000, -500, 2000, LW(6))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_50_A_WAITING_PROF_FRANKLY)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(6), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_GHOST_T_SENT, 1)
    CALL(evt_pouch_receive_mail, 25)
    CALL(evt_pouch_receive_mail, 33)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Boggly Woods (win_01): after Flurrie joins (STG2_09) sends mail 27,
 * issue 1 of the RDM series (GSWF_MAIL_RDM_ISSUE_1_SENT), when Mario crosses x = 390.
 */
EVT_BEGIN(mail_evt_win_01)
    IF_INT_EQUAL(GSWF_MAIL_RDM_ISSUE_1_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 390, -2000, 390, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG2_09_FLURRIE_JOINS)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_RDM_ISSUE_1_SENT, 1)
    CALL(evt_pouch_receive_mail, 27)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_tou_01)
    IF_INT_EQUAL(GSWF_MAIL_RDM_ISSUE_2_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -50, -2000, -50, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG3_12_THE_MINI_YOSHI_EGG)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_RDM_ISSUE_2_SENT, 1)
    CALL(evt_pouch_receive_mail, 28)
    SET_VALUE(GSWF_SHOP_MRI_07_DOUBLE_POINTS_SALE, 1)
    CALL(evt_sub_save_playtime, GSW_SHOP_MRI_07_SALE_START_TIME)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Twilight Town (usu_01): at STG4_04_01 sends mail 29 (RDM issue 3)
 * when Mario crosses x = 110 (the early demos: 300).
 */
EVT_BEGIN(mail_evt_usu_01)
    IF_INT_EQUAL(GSWF_MAIL_RDM_ISSUE_3_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 300, -2000, 300, 2000, LW(5))
#else
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 110, -2000, 110, 2000, LW(5))
#endif
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG4_04_01_PAPER_MAGE_TUBE_MOD)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_RDM_ISSUE_3_SENT, 1)
    CALL(evt_pouch_receive_mail, 29)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

EVT_BEGIN(mail_evt_gor_01_2)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -2000, 200, 2000, 200, LW(5))
        IF_INT_EQUAL(LW(5), 1)
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(GSW_STORY_PROGRESS, STG5_59_01_A_MAIL_FROM_PEACH)
                IF_INT_EQUAL(GSWF_MAIL_RDM_ISSUE_4_SENT, 0)
                    SET_VALUE(GSWF_MAIL_RDM_ISSUE_4_SENT, 1)
                    CALL(evt_pouch_receive_mail, 30)
                    SET_VALUE(GSWF_SHOP_USU_01_DOUBLE_POINTS_SALE, 1)
                    CALL(evt_sub_save_playtime, GSW_SHOP_USU_01_SALE_START_TIME)
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            CALL(irai_mail_check, 0, LW(5))
            IF_INT_EQUAL(LW(5), 1)
                CALL(irai_mail_check, 1, LW(5))
                SET_VALUE(LF(0), 1)
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL_CHILD(mail_evt)
                SET_VALUE(LF(0), 0)
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Excess Express (rsh_03_a): on day 2 (STG6_21) sends mail 31 (RDM issue 5) when Mario crosses x = 400. */
EVT_BEGIN(mail_evt_rsh_03_a_2)
    IF_INT_EQUAL(GSWF_MAIL_RDM_ISSUE_5_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), 400, -2000, 400, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG6_21_DAY_2)
            IF_INT_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_RDM_ISSUE_5_SENT, 1)
    CALL(evt_pouch_receive_mail, 31)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

/**
 * Poshley Heights (pik_00): at STG7_02 after the cannon is approved sends mail 32 (RDM issue 6)
 * when Mario crosses x = -60 and starts the rsh_03 double-points sale.
 */
EVT_BEGIN(mail_evt_pik_00)
    IF_INT_EQUAL(GSWF_MAIL_RDM_ISSUE_6_SENT, 1)
        RETURN
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    LOOP(0)
        SET_VALUE(LW(10), LW(0))
        SET_VALUE(LW(11), LW(1))
        SET_VALUE(LW(12), LW(2))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_check_intersect, LW(0), LW(2), LW(10), LW(12), -60, -2000, -60, 2000, LW(5))
        IF_INT_EQUAL(GSW_STORY_PROGRESS, STG7_02_MAYOR_BOB)
            IF_INT_EQUAL(GSWF_PIK_GOLDBOB_PERMISSION_GIVEN, 1)
                IF_INT_EQUAL(LW(5), 1)
                    BREAK_LOOP
                END_IF
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    SET_VALUE(GSWF_MAIL_RDM_ISSUE_6_SENT, 1)
    CALL(evt_pouch_receive_mail, 32)
    SET_VALUE(GSWF_SHOP_RSH_03_DOUBLE_POINTS_SALE, 1)
    CALL(evt_sub_save_playtime, GSW_SHOP_RSH_03_SALE_START_TIME)
    CALL_CHILD(mail_evt)
    RETURN
EVT_END()

#if VERSION >= VERSION_US
EVT_BEGIN(evt_sub_mail_event)
#else
EvtWord evt_sub_mail_event[] = {
#endif

    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_E3)
        RETURN
    END_IF
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("nok_01"))
        CALL_CHILD(mail_evt_nok_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_03"))
        CALL_CHILD(mail_evt_gor_03)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_02"))
        SPAWN_EVENT(mail_evt_gor_02)
        CALL_CHILD(mail_evt_gor_02_2)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_01"))
        SPAWN_EVENT(mail_evt_gor_01_2)
        CALL_CHILD(mail_evt_gor_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_04"))
#if VERSION >= VERSION_US
        CALL_CHILD(mail_evt_gor_04)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("rsh_02_a"))
        CALL_CHILD(mail_evt_rsh_03_a)
#else
        CALL_CHILD(mail_evt_gor_04)
#endif
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("rsh_03_a"))
#if VERSION >= VERSION_US
        CALL_CHILD(mail_evt_rsh_03_a_2)
#else
        CALL_CHILD(mail_evt_rsh_03_a_2)
        CALL_CHILD(mail_evt_rsh_03_a)
#endif
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("hom_00"))
        CALL_CHILD(mail_evt_hom_00)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("bom_01"))
        CALL_CHILD(mail_evt_bom_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tik_00"))
        CALL_CHILD(mail_evt_tik_00)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("win_01"))
        CALL_CHILD(mail_evt_win_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("tou_01"))
        CALL_CHILD(mail_evt_tou_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("usu_01"))
        CALL_CHILD(mail_evt_usu_01)
        RETURN
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_00"))
        CALL_CHILD(mail_evt_pik_00)
        RETURN
    END_IF
    RETURN
#if VERSION >= VERSION_US
EVT_END()
#else
};
#endif

#pragma force_active on

#if VERSION < VERSION_US
// FAKE: placed in .data; without it the object changes
__declspec(section ".data") s32 evt_sub_mail_event_end[] = {
    1,
};
#endif

#pragma force_active reset

/**
 * Starts a frame-counted interpolation in the calling script:
 * args are the easing mode, start and end values and the step count, kept in LW(11)..LW(15).
 * Read it back with evt_sub_intpl_get_value.
 */
USER_FUNC(evt_sub_intpl_init) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 start = evtGetValue(event, args[1]);
    s32 end = evtGetValue(event, args[2]);
    s32 steps = evtGetValue(event, args[3]);
    event->lwData[11] = mode;
    event->lwData[12] = start;
    event->lwData[13] = end;
    event->lwData[14] = 0;
    event->lwData[15] = steps;
    return EVT_RETURN_DONE;
}

/**
 * Advances the interpolation set by evt_sub_intpl_init one step:
 * LW(0) gets the current value, LW(1) is 1 while steps remain and 0 on the last one.
 */
USER_FUNC(evt_sub_intpl_get_value) {
    event->lwData[0] = intplGetValue(event->lwData[11], event->lwData[12], event->lwData[13], event->lwData[14], event->lwData[15]);
    if (event->lwData[14] >= event->lwData[15]) {
        event->lwData[1] = 0;
    } else {
        event->lwData[1] = 1;
    }
    event->lwData[14]++;
    return EVT_RETURN_DONE;
}

/** Like evt_sub_intpl_get_value, but writes the value as a float to args[0] and the still-running flag to args[1]. */
USER_FUNC(evt_sub_intpl_get_value_para) {
    EvtWord* args = event->args;
    evtSetFloat(event, args[0], intplGetValue(event->lwData[11], event->lwData[12], event->lwData[13], event->lwData[14], event->lwData[15]));
    if (event->lwData[14] >= event->lwData[15]) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], 1);
    }
    event->lwData[14]++;
    return EVT_RETURN_DONE;
}

/** Float version of evt_sub_intpl_init: args are the easing mode, float start and end, and the step count. */
USER_FUNC(evt_sub_intpl_init_float) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    f32 start = evtGetFloat(event, args[1]);
    f32 end = evtGetFloat(event, args[2]);
    s32 steps = evtGetValue(event, args[3]);
    event->lwData[11] = mode;
    event->lwData[12] = FLOAT(start);
    event->lwData[13] = FLOAT(end);
    event->lwData[14] = 0;
    event->lwData[15] = steps;
    return EVT_RETURN_DONE;
}

/**
 * Advances the float interpolation from evt_sub_intpl_init_float:
 * float value to args[0], still-running flag to args[1]; either may be null.
 */
USER_FUNC(evt_sub_intpl_get_float) {
    EvtWord* args = event->args;
    s32 out = args[0];
    s32 done = args[1];
    f32 start = evtGetFloat(event, LW(12));
    f32 end = evtGetFloat(event, LW(13));
    f32 v = intplGetValue(event->lwData[11], start, end, event->lwData[14], event->lwData[15]);
    if (out != EVT_NULLPTR) {
        evtSetFloat(event, out, v);
    }
    if (done != EVT_NULLPTR) {
        if (event->lwData[14] >= event->lwData[15]) {
            evtSetValue(event, done, 0);
        } else {
            evtSetValue(event, done, 1);
        }
    }
    event->lwData[14]++;
    return EVT_RETURN_DONE;
}

/**
 * Starts a time-based interpolation: easing mode, start, end and duration in milliseconds,
 * timed from the script's own clock, so animations run at the same speed at 50 Hz.
 */
USER_FUNC(evt_sub_intpl_msec_init) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 start = evtGetValue(event, args[1]);
    s32 end = evtGetValue(event, args[2]);
    s32 msec = evtGetValue(event, args[3]);
    event->lwData[11] = mode;
    event->lwData[12] = start;
    event->lwData[13] = end;
    event->lwData[15] = msec;
    event->savedTime = event->lifetime;
    return EVT_RETURN_DONE;
}

/**
 * Reads the time-based interpolation: LW(0) gets the value for the elapsed milliseconds,
 * LW(1) is 1 until the duration has passed, then the end value and 0.
 */
USER_FUNC(evt_sub_intpl_msec_get_value) {
    s32 ms = OSTicksToMilliseconds((u64)OSDiffTick(event->lifetime, event->savedTime));
    if (ms < event->lwData[15]) {
        event->lwData[0] = intplGetValue(event->lwData[11], event->lwData[12], event->lwData[13], ms, event->lwData[15]);
        event->lwData[1] = 1;
    } else {
        event->lwData[0] = event->lwData[13];
        event->lwData[1] = 0;
    }
    return EVT_RETURN_DONE;
}

/** Like evt_sub_intpl_msec_get_value, but writes the value as a float to args[0] and the running flag to args[1]. */
USER_FUNC(evt_sub_intpl_msec_get_value_para) {
    EvtWord* args = event->args;
    s32 ms = OSTicksToMilliseconds((u64)OSDiffTick(event->lifetime, event->savedTime));
    if (ms < event->lwData[15]) {
        evtSetFloat(event, args[0], intplGetValue(event->lwData[11], event->lwData[12], event->lwData[13], ms, event->lwData[15]));
        evtSetValue(event, args[1], 1);
    } else {
        evtSetFloat(event, args[0], event->lwData[13]);
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Builds a spline through the args[3] points at args[2] and starts walking it:
 * args[0] picks millisecond timing (1, duration args[5]) or step timing (0, args[4] steps), args[1] the easing.
 * The work lives in map memory, pointed to by LW(15).
 */
USER_FUNC(evt_sub_spline_init) {
    EvtWord* args = event->args;
    s32 manual = evtGetValue(event, args[0]);
    s32 mode = evtGetValue(event, args[1]);
    Vec* pts = (Vec*)evtGetValue(event, args[2]);
    s32 n = evtGetValue(event, args[3]);
    s32 steps = evtGetValue(event, args[4]);
    s32 msec = evtGetValue(event, args[5]);
    SplineWork* w = _mapAlloc(sizeof(SplineWork));
    event->lwData[15] = (EvtWord)w;
    w->n = n;
    w->ts = _mapAlloc(n * sizeof(f32));
    w->pts = pts;
    w->tbl = _mapAlloc(n * sizeof(Vec));
    spline_maketable(n, w->pts, w->ts, w->tbl);
    w->manual = manual;
    w->mode = mode;
    w->cur = 0;
    w->steps = steps;
    w->msec = msec;
    w->start = event->lifetime;
    return EVT_RETURN_DONE;
}

/**
 * Samples the next point of the spline from evt_sub_spline_init into args[0..2] (x, y, z) and sets args[3] to 1
 * while the path continues; on the last point it frees the spline work and sets 0.
 */
USER_FUNC(evt_sub_spline_get_value) {
    EvtWord* args = event->args;
    SplineWork* w = (SplineWork*)event->lwData[15];
    f32 el;
    Vec v;
    f32 t;
    if (!w->manual) {
        t = intplGetValue(w->mode, 0.0f, 1.0f, w->cur, w->steps);
    } else {
        el = OSTicksToMilliseconds((u64)(event->lifetime - w->start));
        t = intplGetValue(w->mode, 0.0f, 1.0f, (s32)el, w->msec);
    }
    spline_getvalue(&v, t, w->n, w->pts, w->ts, w->tbl);
    evtSetValue(event, args[0], FLOAT(v.x));
    evtSetValue(event, args[1], FLOAT(v.y));
    evtSetValue(event, args[2], FLOAT(v.z));
    w->cur++;
    if (!w->manual) {
        if (w->cur <= w->steps) {
            evtSetValue(event, args[3], 1);
            return EVT_RETURN_DONE;
        }
    } else if (el <= (f32)w->msec) {
        evtSetValue(event, args[3], 1);
        return EVT_RETURN_DONE;
    }
    _mapFree(w->ts);
    _mapFree(w->tbl);
    _mapFree(w);
    evtSetValue(event, args[3], 0);
    return EVT_RETURN_DONE;
}

/**
 * Samples the spline at a script-chosen position:
 * args[0] is the step (or millisecond) count along it, the point goes to args[1..3].
 * Does not advance or free the spline.
 */
USER_FUNC(evt_sub_spline_get_value_manual) {
    EvtWord* args = event->args;
    SplineWork* w = (SplineWork*)event->lwData[15];
    f32 t = evtGetValue(event, args[0]);
    Vec v;

    // FAKE: no-op self-assignment; without it t's conversion is scheduled differently
    t = t;
    t = t / (w->manual ? (f32)w->msec : (f32)w->steps);
    spline_getvalue(&v, t, w->n, w->pts, w->ts, w->tbl);
    evtSetValue(event, args[1], FLOAT(v.x));
    evtSetValue(event, args[2], FLOAT(v.y));
    evtSetValue(event, args[3], FLOAT(v.z));
    return EVT_RETURN_DONE;
}

/** Frees the spline work from evt_sub_spline_init, for scripts that stop before the end of the path. */
USER_FUNC(evt_sub_spline_free) {
    SplineWork* w = (SplineWork*)event->lwData[15];
    _mapFree(w->ts);
    _mapFree(w->tbl);
    _mapFree(w);
    return EVT_RETURN_DONE;
}

/** Returns the sine and cosine of the angle in args[0] (degrees) to args[1] and args[2]. */
USER_FUNC(evt_sub_get_sincos) {
    EvtWord* args = event->args;
    f32 rad = (3.141592f * evtGetFloat(event, args[0])) / 180.0f;
    f32 s = (f32)sin(rad);
    evtSetValue(event, args[1], FLOAT(s));
    s = (f32)cos(rad);
    evtSetValue(event, args[2], FLOAT(s));
    return EVT_RETURN_DONE;
}

/** Controller rumble: args[0] 0 starts it, 1 stops it, 2 stops it hard; args[1] is the controller channel. */
USER_FUNC(evt_sub_rumble_onoff) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 chan = evtGetValue(event, args[1]);
    switch (mode) {
    case 0:
        padRumbleOn(chan);
        break;
    case 1:
        padRumbleOff(chan);
        break;
    case 2:
        padRumbleHardOff(chan);
        break;
    }
    return EVT_RETURN_DONE;
}

/** Returns a random integer from 0 to args[0] inclusive in args[1]. */
USER_FUNC(evt_sub_random) {
    EvtWord* args = event->args;
    s32 n = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], rand() % (n + 1));
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[1] how many milliseconds event args[0] has run (its EventEntry.lifetime),
 * capped at ten minutes; used with an evt_sub_stopwatch event.
 */
USER_FUNC(evt_sub_get_stopwatch) {
    EvtWord* args = event->args;
    s64* sw = (s64*)evtGetPtrID(evtGetValue(event, args[0]));
    s32 ms = OSTicksToMilliseconds(*sw);
    if (ms > 600000) {
        ms = 600000;
    }
    evtSetValue(event, args[1], ms);
    return EVT_RETURN_DONE;
}

/**
 * Tests whether the point (args[4], args[5]) lies inside a shape at (args[1], args[2]) of size args[3]:
 * mode args[0] 0 is a circle of that radius, 1 a square of that half-width. Result 1 or 0 in args[6].
 */
USER_FUNC(evt_sub_area_check) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    f32 cx = evtGetValue(event, args[1]);
    f32 cy = evtGetValue(event, args[2]);
    f32 range = evtGetValue(event, args[3]);
    f32 px = evtGetValue(event, args[4]);
    f32 py = evtGetValue(event, args[5]);
    s32 result;
    f32 dist;
    switch (mode) {
    case 0:
        dist = distABf(cx, cy, px, py);
        if (range < dist) {
            result = 0;
        } else {
            result = 1;
        }
        break;
    case 1:
        result = 1;
        if (cx - range > px) {
            result = 0;
        }
        if (cx + range < px) {
            result = 0;
        }
        if (cy - range > py) {
            result = 0;
        }
        if (cy + range < py) {
            result = 0;
        }
        break;
    default:
        result = 0;
        break;
    }
    evtSetValue(event, args[6], result);
    return EVT_RETURN_DONE;
}

/** Returns the distance between (args[0], args[1]) and (args[2], args[3]) as a float in args[4]. */
USER_FUNC(evt_sub_get_dist) {
    EvtWord* args = event->args;
    f32 x1 = evtGetValue(event, args[0]);
    f32 y1 = evtGetValue(event, args[1]);
    f32 x2 = evtGetValue(event, args[2]);
    f32 y2 = evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], distABf(x1, y1, x2, y2));
    return EVT_RETURN_DONE;
}

/** Returns the direction in degrees from (args[0], args[1]) to (args[2], args[3]) as a float in args[4]. */
USER_FUNC(evt_sub_get_dir) {
    EvtWord* args = event->args;
    f32 x1 = evtGetValue(event, args[0]);
    f32 y1 = evtGetValue(event, args[1]);
    f32 x2 = evtGetValue(event, args[2]);
    f32 y2 = evtGetValue(event, args[3]);
    evtSetFloat(event, args[4], angleABf(x1, y1, x2, y2));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_system_flag) {
    evtSetValue(event, event->args[0], gp->flags);
    return EVT_RETURN_DONE;
}

/** Sets (args[0] nonzero) or clears the system flag bits given literally in args[1]. */
USER_FUNC(evt_sub_system_flag_onoff) {
    EvtWord* args = event->args;
    s32 onoff = evtGetValue(event, args[0]);
    s32 flag = args[1];
    if (onoff) {
        gp->flags |= flag;
    } else {
        gp->flags &= ~flag;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_key_get_dir) {
    EvtWord* args = event->args;
    s32 chan = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], keyGetDir(chan));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_key_get_button) {
    EvtWord* args = event->args;
    s32 chan = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], keyGetButton(chan));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_key_get_buttontrg) {
    EvtWord* args = event->args;
    s32 chan = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], keyGetButtonTrg(chan));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_sub_get_mapname) {
    evtSetValue(event, event->args[0], (EvtWord)gp->currentMapName);
    return EVT_RETURN_DONE;
}

/** Returns the current area name (e.g. "gor") in args[0]. */
USER_FUNC(evt_sub_get_areaname) {
    evtSetValue(event, event->args[0], (EvtWord)gp->currentAreaName);
    return EVT_RETURN_DONE;
}

/** Forces the HP/FP status bar at the top of the screen open. */
USER_FUNC(evt_sub_status_gauge_force_open) {
    statusWinForceOpen();
    return EVT_RETURN_DONE;
}

/** Forces the HP/FP status bar closed again. */
USER_FUNC(evt_sub_status_gauge_force_close) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

/** Returns 1 in args[0] while the status bar is still animating a change to its numbers, 0 once it has settled. */
USER_FUNC(evt_sub_status_gauge_check_update) {
    EvtWord* args = event->args;
    if (statusWinCheckUpdate()) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

#if VERSION >= VERSION_US
/** Makes the status bar refresh its numbers at once. Not in JP or the early demos. */
// Descriptive name (not in the symbol maps).
USER_FUNC(evt_sub_status_gauge_force_update) {
    statusWinForceUpdate();
    return EVT_RETURN_DONE;
}
#endif

/**
 * Stamps the current play-time clock (8 bytes) into the saved GSW bytes at args[0],
 * so a later evt_sub_load_progresstime can measure how long ago it was. Used to time the double-points sales.
 */
USER_FUNC(evt_sub_save_playtime) {
    s32 base = event->args[0]; // a GSW(n) script value: base + EVTDAT_GSW_BASE is n
    u8 buf[8];
    *(s64*)buf = gp->playTime;
    swByteSet(base + EVTDAT_GSW_BASE + 0, buf[0]);
    swByteSet(base + EVTDAT_GSW_BASE + 1, buf[1]);
    swByteSet(base + EVTDAT_GSW_BASE + 2, buf[2]);
    swByteSet(base + EVTDAT_GSW_BASE + 3, buf[3]);
    swByteSet(base + EVTDAT_GSW_BASE + 4, buf[4]);
    swByteSet(base + EVTDAT_GSW_BASE + 5, buf[5]);
    swByteSet(base + EVTDAT_GSW_BASE + 6, buf[6]);
    swByteSet(base + EVTDAT_GSW_BASE + 7, buf[7]);
    return EVT_RETURN_DONE;
}

/**
 * Reads the time stamp saved by evt_sub_save_playtime at args[0] and returns the milliseconds since
 * then in args[1], capped at 24 hours.
 */
USER_FUNC(evt_sub_load_progresstime) {
    EvtWord* args = event->args;
    s32 base = args[0];
    u8 buf[8];
    s64 ms;
    buf[0] = swByteGet(base + EVTDAT_GSW_BASE + 0);
    buf[1] = swByteGet(base + EVTDAT_GSW_BASE + 1);
    buf[2] = swByteGet(base + EVTDAT_GSW_BASE + 2);
    buf[3] = swByteGet(base + EVTDAT_GSW_BASE + 3);
    buf[4] = swByteGet(base + EVTDAT_GSW_BASE + 4);
    buf[5] = swByteGet(base + EVTDAT_GSW_BASE + 5);
    buf[6] = swByteGet(base + EVTDAT_GSW_BASE + 6);
    buf[7] = swByteGet(base + EVTDAT_GSW_BASE + 7);
    ms = OSTicksToMilliseconds(gp->playTime - *(s64*)buf);
    if (ms > 86400000) {
        ms = 86400000;
    }
    evtSetValue(event, args[1], ms);
    return EVT_RETURN_DONE;
}

/** Returns the text language setting (gp->language) in args[0]; 0 is Japanese (LANGUAGE_JP). */
USER_FUNC(evt_sub_get_language) {
    evtSetValue(event, event->args[0], gp->language);
    return EVT_RETURN_DONE;
}

/** Loads the animation group named in args[0] (as a battle group when in a battle), blocking until it is resident. */
USER_FUNC(evt_sub_animgroup_async) {
    const char* group = (const char*)evtGetValue(event, event->args[0]);
    s32 r = animGroupBaseAsync(group, gp->inBattle != 0, NULL);
    if (r == 0) {
        return EVT_RETURN_BLOCK;
    }

    // FAKE: redundant test (r is nonzero here); returning EVT_RETURN_DONE directly changes the object
    if (r != 0) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

#if VERSION >= VERSION_DEMO_EU
/** Returns the display frame rate (50 or 60) in args[0]. Only in the PAL demo and PAL. */
USER_FUNC(evt_sub_get_framerate) {
    evtSetValue(event, event->args[0], gp->framerate);
    return EVT_RETURN_DONE;
}
#endif

/** Starts the on-screen countdown timer: args[0] is the time limit, args[1] the restart time. */
USER_FUNC(evt_sub_countdown_start) {
    EvtWord* args = event->args;
    s32 limit = evtGetValue(event, args[0]);
    s32 restart = evtGetValue(event, args[1]);
    countDownStart(limit, restart);
    return EVT_RETURN_DONE;
}

/** Returns the countdown timer's state in args[0]. */
USER_FUNC(evt_sub_countdown_get_status) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], countDownGetStatus());
    return EVT_RETURN_DONE;
}

/** Ends and removes the countdown timer. */
USER_FUNC(evt_sub_countdown_end) {
    countDownEnd();
    return EVT_RETURN_DONE;
}

/** Sets (args[0] nonzero) or clears the countdown flags given in args[1]. */
USER_FUNC(evt_sub_countdown_flag_onoff) {
    EvtWord* args = event->args;
    s32 onoff = evtGetValue(event, args[0]);
    s32 flag = evtGetValue(event, args[1]);
    if (onoff) {
        countDownFlagOn(flag);
    } else {
        countDownFlagOff(flag);
    }
    return EVT_RETURN_DONE;
}

/** Sets the time the countdown restarts from to args[0]. */
USER_FUNC(evt_sub_countdown_set_restart_time) {
    countDownSetRestartTime(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/**
 * Draws the "You got N coins!" message inside the coin window of evt_sub_get_coin;
 * work[0] is the coin count, work[1] turns the text on.
 * US and later widen the window to fit the message and format it per language.
 */
static void coingetDisp(WinMgrEntry* entry) {
    s32* work = entry->param;
    char buf[0x400];
    if (work[1]) {
#if VERSION >= VERSION_US
        u16 lines;
        s32 w;

        if (gp->language == LANGUAGE_JP) {
            sprintf(buf, msgSearch("msg_window_coin_get"), work[0]);
        } else {
            strcpy(buf, msgSearch("msg_window_coin_get"));
            evtMsgFmtInt(buf, work[0]);
        }
        w = FontGetMessageWidthLine(buf, &lines) + 20;
        if (w > entry->width) {
            entry->width = w;
            entry->x = -entry->width / 2;
        }
        entry->height = entry->description->height + lines * 24;
#else
        sprintf(buf, msgSearch("msg_window_coin_get"), work[0]);
#endif
        FontDrawStart();
        FontDrawColor(coin_black);
        FontDrawMessage(-(s32)FontGetMessageWidth(buf) / 2, entry->y - 8, buf);
    }
}

#pragma opt_lifetimes off
/**
 * The "got coins" scene: shows a coin icon in front of Mario with the item-get pose and jingle
 * and the coin window, waits for A, then adds args[0] coins to the pouch and restores the music.
 * Used for coin rewards from requests (the "iraiCoin" icon), e.g. evt_kinopio.c.
 */
USER_FUNC(evt_sub_get_coin) {
    s32 coin = evtGetValue(event, event->args[0]);
    MarioWork* mario = marioGetPtr();
    Vec a = mario->position;
    Vec b = a;
#if VERSION <= VERSION_DEMO_JP
    Vec scr;
#endif
    f32 rad = reviseAngle((100.0f * camGetPtr(CAMERA_3D)->viewYaw + 5.0f) / 100.0f);
    f32 s;
    f32 c;
    s32 state;
    EffectEntry* eff;
    s32* work;
    rad = 6.2831855f * rad / 360.0f;
    s = 10.0f * polySinf(rad);
    c = 10.0f * polyCosf(rad);
    a.x += s;
    a.z -= c;
    a.y += 62.0f;
    b.x += s;
    b.y += 52.0f;
    b.z -= c;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdatap[1] = effItemGetEntry(7, a.x, a.y, a.z);
        event->userdata[2] = 0;
        event->userdata3 = 0;
        event->wInterpWork = _mapAlloc(8);
    }
    state = event->userdata[0];
    eff = event->userdatap[1];
    work = event->wInterpWork;
    switch (state) {
    case 0:
#if VERSION <= VERSION_DEMO_JP
        iconEntry2D("iraiCoin", ICON_ITEM_COIN);
        getScreenPoint(&b, &scr);
        iconSetPos("iraiCoin", scr.x, scr.y, scr.z);
        iconFlagOn("iraiCoin", ICON_FLAG_HIDDEN);
#else
        iconEntry("iraiCoin", ICON_ITEM_COIN);
        iconSetPos("iraiCoin", b.x, b.y, b.z);
        iconFlagOn("iraiCoin", ICON_FLAG_HIDDEN | ICON_FLAG_CAM_EFFECT_A);
        ((ItemGetWork*)eff->userdata)->camera = CAMERA_3D_EFFECTA;
#endif
        marioChgGetItemMotion();
        event->userdata3 = winMgrEntry(coinget_window_desc);
        winMgrOpen(event->userdata3);
        winMgrSetParam(event->userdata3, work);
        work[0] = coin;
        work[1] = 0;
        psndBGMOff(PSND_SILENT | PSND_SILENT_LEVEL(3));
        psndBGMOn(PSND_FADEOUT | PSND_SLOT(1), "BGM_FF_GET_ITEM1");
        event->userdata[2] = 200;
        event->savedTime = gp->renderTime;
        event->userdata[0] = 1;
        return EVT_RETURN_BLOCK;
    case 1:
        if ((s32)OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) > event->userdata[2]) {
            iconFlagOff("iraiCoin", ICON_FLAG_HIDDEN);
            work[1] = 1;
            event->userdata[2] = 500;
            event->savedTime = gp->renderTime;
            event->userdata[0] = 2;
        }
        return EVT_RETURN_BLOCK;
    case 2:
        if ((s32)OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) > event->userdata[2]) {
            event->userdata[0] = 10;
        }
        return EVT_RETURN_BLOCK;
    case 10:
        if (keyGetButtonTrg(0) & PAD_BUTTON_A) {
            event->userdata[0] = 100;
        }
        return EVT_RETURN_BLOCK;
    case 100:
        pouchAddCoin(coin);
        effSoftDelete(eff);
        iconDelete("iraiCoin");
        marioChgStayMotion();
        psndBGMOn(PSND_CONTINUE | PSND_UNSILENT, NULL);
        psndBGMOff(PSND_FADEOUT | PSND_SLOT(1));
        winMgrCloseAutoDelete(event->userdata3);
        work[1] = 0;
        event->userdata[0] = 101;

        // BUG: returns DONE instead of BLOCK, so case 101 never runs and the 8-byte work block is never _mapFree'd.
        return EVT_RETURN_DONE;
    case 101:
        if (!winMgrAction(event->userdata3)) {
            _mapFree(work);
            return EVT_RETURN_DONE;
        }
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}
#pragma opt_lifetimes reset

/** Turns the star stone item args[0] to the yaw args[1] (degrees) while it spins up in evt_sub_starstone. */
USER_FUNC(stone_ry) {
    EvtWord* args = event->args;
    char* name = (char*)evtGetValue(event, args[0]);
    s32 ang = evtGetValue(event, args[1]);
    EffectEntry* item = itemNameToPtr(name)->effect;
    ((EffStarStoneWork*)item->userdata)->flags &= ~STAR_STONE_FLAG_SHINE_SFX;
    ((EffStarStoneWork*)item->userdata)->rot.y = ang % 360;
    return EVT_RETURN_DONE;
}

/**
 * Puts the star stone, its glow and Mario (or the Doopliss Mario NPC before STG4_15) on camera 5,
 * so they draw in front of the stage-clear background in evt_sub_starstone.
 */
USER_FUNC(stone_bg) {
    char* name;
    CameraEntry* cam;
    EffectEntry* eff;
    ItemEntry* item;
    name = (char*)evtGetValue(event, event->args[0]);
    cam = camGetPtr(CAMERA_2D);
    cam->letterBoxCamId = CAMERA_3D_EFFECTA;
    eff = effNameToPtr("sub_bg");
    ((ItemGetWork*)eff->userdata)->camera = CAMERA_3D_EFFECTA;
    item = itemNameToPtr(name);
    ((EffStarStoneWork*)item->effect->userdata)->cam = CAMERA_3D_EFFECTA;

    // The name is Shift-JIS "にせマリオ", the NPC evt_sub_starstone uses when LW(0) is -1
    if (npcNameToPtr_NoAssert("にせマリオ") != NULL && evtGetValue(event, GSW_STORY_PROGRESS) < STG4_15_DOOPLISS_BATTLE_2) {
        npcNameToPtr_NoAssert("にせマリオ")->cameraId = CAMERA_3D_EFFECTA;
    } else {
        marioSetCamId(CAMERA_3D_EFFECTA);
    }
    return EVT_RETURN_DONE;
}

/** Starts the opening of the "sub_bg" item-get background effect in evt_sub_starstone. */
USER_FUNC(stone_bg_open) {
    EffectEntry* e = effNameToPtr("sub_bg");
    ((ItemGetWork*)e->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/** Blocks until the "sub_clear" stage-clear effect has finished. */
USER_FUNC(stageclear_wait) {
    return effStageClearEndChk(effNameToPtr("sub_clear")) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** Sets the module-level stop flag to args[0]; evt_sub_starstone raises it 9.7 seconds after the white flash starts. */
USER_FUNC(set_stop) {
    stop = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/** Returns the stop flag set by set_stop in args[0]. */
USER_FUNC(get_stop) {
    evtSetValue(event, event->args[0], stop);
    return EVT_RETURN_DONE;
}

/** Blocks until the music started on channel 1 has begun playing. */
USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

#if VERSION >= VERSION_US
/**
 * Scales the two float variables args[0] and args[1] by 60 / frame rate,
 * so per-frame steps cover the same distance at 50 Hz. Not in JP or the early demos.
 */
USER_FUNC(evt_sub_scale_by_fps) {
    EvtWord* args = event->args;
    s32 a0 = args[0];
    f32 x = evtGetFloat(event, a0);
    s32 a1 = args[1];
    f32 y = evtGetFloat(event, a1);
    x *= 60.0f / gp->framerate;
    y *= 60.0f / gp->framerate;
    evtSetFloat(event, a0, x);
    evtSetFloat(event, a1, y);
    return EVT_RETURN_DONE;
}
#endif

/**
 * Tests whether the segment from (args[0], args[1]) to (args[2], args[3]) crosses the segment from (args[4],
 * args[5]) to (args[6], args[7]); 1 or 0 in args[8].
 * The mail scripts use it to detect Mario walking across a line.
 */
USER_FUNC(evt_sub_check_intersect) {
    EvtWord* args = event->args;

    // Segment P1-P2 (args 0-3) against segment Q1-Q2 (args 4-7); args[8] = 1 when they intersect
    f32 p1x, p1y, p2x, p2y, q1x, q1y, q2x, q2y;
    f32 t1, t2;
    p1x = evtGetFloat(event, args[0]);
    p1y = evtGetFloat(event, args[1]);
    p2x = evtGetFloat(event, args[2]);
    p2y = evtGetFloat(event, args[3]);
    q1x = evtGetFloat(event, args[4]);
    q1y = evtGetFloat(event, args[5]);
    q2x = evtGetFloat(event, args[6]);
    q2y = evtGetFloat(event, args[7]);
    if (p1x >= p2x) {
        if ((p1x < q1x && p1x < q2x) || (p2x > q1x && p2x > q2x)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    } else {
        if ((p2x < q1x && p2x < q2x) || (p1x > q1x && p1x > q2x)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    }
    if (p1y >= p2y) {
        if ((p1y < q1y && p1y < q2y) || (p2y > q1y && p2y > q2y)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    } else {
        if ((p2y < q1y && p2y < q2y) || (p1y > q1y && p1y > q2y)) {
            evtSetValue(event, args[8], 0);
            return EVT_RETURN_DONE;
        }
    }
    t1 = (p1x - p2x) * (q1y - p1y) + (p1y - p2y) * (p1x - q1x);
    t2 = (p1x - p2x) * (q2y - p1y) + (p1y - p2y) * (p1x - q2x);
    if (t1 * t2 > 0.0f) {
        evtSetValue(event, args[8], 0);
        return EVT_RETURN_DONE;
    }
    t1 = (q1x - q2x) * (p1y - q1y) + (q1y - q2y) * (q1x - p1x);
    t2 = (q1x - q2x) * (p2y - q1y) + (q1y - q2y) * (q1x - p2x);
    if (t1 * t2 > 0.0f) {
        evtSetValue(event, args[8], 0);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[8], 1);
    return EVT_RETURN_DONE;
}

// Statement macro on irai_mail_check's locals (i, u, ret, arg): for an active, unfinished request more than 7200 s old it sets ret and, if arg, marks it done and sends the mail.
#define IRAI_CHECK(flag, done, base, mail)                         \
    if (swGet(flag) && !swGet(done)) {                             \
        for (i = 0; i < 8; i++) {                                  \
            u.b[i] = swByteGet(base + i);                          \
        }                                                          \
        if (OSTicksToSeconds(gp->playTime - u.t) > 7200) {         \
            ret = 1;                                               \
            if (arg) {                                             \
                swSet(done);                                       \
                pouchReceiveMail(mail);                            \
            }                                                      \
        }                                                          \
    }
USER_FUNC(irai_mail_check) {
    EvtWord* args = event->args;
    s32 arg = evtGetValue(event, args[0]);
    s32 ret = 0;
    s32 i;
    union {
        u8 b[8];
        OSTime t;
    } u;
    IRAI_CHECK(0x1505, 0x17, GSW_ID(GSW_GON_TRY_TO_FIND_ME_DONE_TIME), 0x25)
    IRAI_CHECK(0x150a, 0x18, GSW_ID(GSW_TIK_HELP_MY_DADDY_DONE_TIME), 0x26)
    IRAI_CHECK(0x150b, 0x19, GSW_ID(GSW_TOU_HELP_WANTED_DONE_TIME), 0x27)
    IRAI_CHECK(0x150d, 0x1a, GSW_ID(GSW_NOK_FOOD_I_WANT_DONE_TIME), 0x28)
    IRAI_CHECK(0x1512, 0x1b, GSW_ID(GSW_USU_TROUBLE_20_DONE_TIME), 0x29)
    IRAI_CHECK(0x1518, 0x1c, GSW_ID(GSW_BOM_I_CANT_SPEAK_DONE_TIME), 0x2a)
    evtSetValue(event, args[1], ret);
    return EVT_RETURN_DONE;
}

/**
 * Returns in args[0] how many of Mario's control and key locks are on;
 * mail_evt_gor_04 holds the mail while it is nonzero.
 */
USER_FUNC(mail_evt_gor_04_keyoff_check) {
    EvtWord* args = event->args;
    s32 a = marioCtrlOffChk();
    s32 b = marioKeyOffChk();
    evtSetValue(event, args[0], b + a);
    return EVT_RETURN_DONE;
}
