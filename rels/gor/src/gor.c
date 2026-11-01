// gor.rel (ゴロツキタウン, Rogueport): shared code and the REL prolog/epilog.
// Registers the init events of gor_00..gor_04 and gor_10..gor_12.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_urouro.h"
#include "evt/evt_window.h"
#include "evt/evt_cmd.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_johoya.h"
#include "rels/gor/include/gor.h"

#include "data/mapdata.h"
#include "mario/mariost.h"
#include "string.h"
#include "memory.h"
#include "driver/msgdrv.h"
#include "stdio.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "system.h"
#include "pmario_sound.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
#if VERSION > VERSION_DEMO_US
static s32 gor_inemuri_snd_id;
static char* gor_msg_cat_buf;
#endif

void _unresolved(void);
void _epilog(void);
void _prolog(void);

EvtWord gor_villager_urouro[19];
#if VERSION <= VERSION_DEMO_US
extern EvtWord inemuri_snd[];
extern EvtWord mario_and_party_sleep[];
extern EvtWord luigi_gor_first_talk[];
#else
EvtWord inemuri_snd[27];
EvtWord mario_and_party_sleep[259];
EvtWord luigi_gor_first_talk[205];
#endif

/**
 * REL prolog for the Rogueport module: runs the static constructors
 * and registers the init event of each Rogueport map (gor_00 to gor_04, gor_10 to gor_12)
 * and the area's battle setup tables.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("gor_00", gor_00_init_evt);
    relSetEvtAddr("gor_01", gor_01_init_evt);
    relSetEvtAddr("gor_02", gor_02_init_evt);
    relSetEvtAddr("gor_03", gor_03_init_evt);
    relSetEvtAddr("gor_04", gor_04_init_evt);
    relSetEvtAddr("gor_10", gor_10_init_evt);
    relSetEvtAddr("gor_11", gor_11_init_evt);
    relSetEvtAddr("gor_12", gor_12_init_evt);
    relSetBtlAddr("gor", btlsetup_gor_tbl, btl_gor_setup_no_tbl);
}

/**
 * REL epilog: when the module is unloaded from gor_10 it deletes the letter object (tegami_delete),
 * then runs the static destructors.
 */
void _epilog(void) {
    voidfunctionptr* dtor;

    if (strcmp(gp->currentMapName, "gor_10") == 0) {
        tegami_delete();
    }
    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

/** Target of unresolved relocations; the module has no code to run for them, so it is empty. */
void _unresolved(void) {
}

/** Script helper (Japanese release and later): frees the string built by evt_gor_msg_cat_alloc, if there is one. */
// Descriptive name (not in the symbol maps); placeholder unk_gor_0000050c, JP gor .text+0x440. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/** Script helper (Japanese release and later): frees the string built by evt_gor_msg_cat_alloc, if there is one. */
USER_FUNC(evt_gor_msg_cat_free) {
    if (gor_msg_cat_buf == NULL) {
        return EVT_RETURN_DONE;
    }
    _mapFree(gor_msg_cat_buf);
    gor_msg_cat_buf = NULL;
    return EVT_RETURN_DONE;
}
#endif

/**
 * Script helper (Japanese release and later): builds one string out of args[0],
 * the message named args[1] and args[2], in a map-allocated buffer,
 * and writes the buffer's address into args[3].
 * Used to put fixed text around a looked-up message in Rogueport scripts.
 */
// Descriptive name (not in the symbol maps); placeholder unk_gor_0000041c, JP gor .text+0x350. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Script helper (Japanese release and later): builds one string out of args[0],
 * the message named args[1] and args[2], in a map-allocated buffer,
 * and writes the buffer's address into args[3].
 * Used to put fixed text around a looked-up message in Rogueport scripts.
 */
USER_FUNC(evt_gor_msg_cat_alloc) {
    EvtWord* args = event->args;
    const char* head = (const char*)evtGetValue(event, args[0]);
    const char* msg = msgSearch((const char*)evtGetValue(event, args[1]));
    const char* tail = (const char*)evtGetValue(event, args[2]);
    s32 size = strlen(head) + strlen(msg) + strlen(tail) + 1;

    gor_msg_cat_buf = _mapAlloc(size);
    memset(gor_msg_cat_buf, 0, size);
    sprintf(gor_msg_cat_buf, "%s%s%s", head, msg, tail);
    evtSetValue(event, args[3], (EvtWord)gor_msg_cat_buf);
    return EVT_RETURN_DONE;
}
#endif

/**
 * Script helper for the wandering Rogueport villagers:
 * args[0] and args[1] are the script variables holding the villager's target x and z, args[2],
 * args[3] the centre of its wander area and args[4] the radius.
 * If the target is outside the circle it is moved back to the centre,
 * so a villager never wanders out of its zone.
 */
USER_FUNC(evt_gor_villager_urouro_check_func) {
    EvtWord* args = event->args;
    s32 xVar = args[0];
    s32 zVar = args[1];
    f32 cx = evtGetFloat(event, args[2]);
    f32 cz = evtGetFloat(event, args[3]);
    f32 radius = evtGetFloat(event, args[4]);
    f32 x = evtGetFloat(event, xVar);
    f32 z = evtGetFloat(event, zVar);
    Vec pos;
    Vec center;

    pos = (Vec){x, 0.0f, z};
    center = (Vec){cx, 0.0f, cz};
    if (PSVECDistance(&center, &pos) <= radius) {
        return EVT_RETURN_DONE;
    }
    evtSetFloat(event, xVar, cx);
    evtSetFloat(event, zVar, cz);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: starts a two-value interpolation in the script's LW(9) to LW(15):
 * args[0] is the curve mode, args[1] and args[2] the start and end of the first value,
 * args[3] and args[4] those of the second, args[5] the number of steps. Used with evt_goki_sub_intpl_get_value.
 */
USER_FUNC(evt_goki_sub_intpl_init) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 x0 = evtGetValue(event, args[1]);
    s32 x1 = evtGetValue(event, args[2]);
    s32 y0 = evtGetValue(event, args[3]);
    s32 y1 = evtGetValue(event, args[4]);
    s32 steps = evtGetValue(event, args[5]);

    event->lwData[9] = mode;
    event->lwData[10] = x0;
    event->lwData[11] = x1;
    event->lwData[12] = y0;
    event->lwData[13] = y1;
    event->lwData[14] = 0;
    event->lwData[15] = steps;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: advances the interpolation started by evt_goki_sub_intpl_init by one step
 * and returns the two interpolated values in LW(0) and LW(1).
 */
USER_FUNC(evt_goki_sub_intpl_get_value) {
    event->lwData[0] = intplGetValue(event->lwData[9], event->lwData[10], event->lwData[11], event->lwData[14], event->lwData[15]);
    event->lwData[1] = intplGetValue(event->lwData[9], event->lwData[12], event->lwData[13], event->lwData[14], event->lwData[15]);
    event->lwData[14]++;
    return EVT_RETURN_DONE;
}

/**
 * Script helper (Japanese release and later): remembers the sound effect id args[0] of Mario's snoring
 * so it can be stopped later (-1 for none).
 */
// Descriptive name (not in the symbol maps); placeholder unk_gor_00000104, JP gor .text+0x38. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Script helper (Japanese release and later): remembers the sound effect id args[0] of Mario's snoring
 * so it can be stopped later (-1 for none).
 */
USER_FUNC(evt_gor_inemuri_snd_set) {
    gor_inemuri_snd_id = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}
#endif

/**
 * Script helper (Japanese release and later): stops the snoring sound effect saved by evt_gor_inemuri_snd_set,
 * if any.
 */
// Descriptive name (not in the symbol maps); placeholder unk_gor_000000cc, JP gor .text+0x0. See docs/NAME_PROVENANCE.md.
#if VERSION > VERSION_DEMO_US
/**
 * Script helper (Japanese release and later): stops the snoring sound effect saved by evt_gor_inemuri_snd_set,
 * if any.
 */
USER_FUNC(evt_gor_inemuri_snd_off) {
    if (gor_inemuri_snd_id != -1) {
        psndSFXOff(gor_inemuri_snd_id);
    }
    return EVT_RETURN_DONE;
}
#endif

/**
 * NPC regular event shared by Rogueport villagers:
 * lets the NPC wander (urouro) within 100 units of where it starts, with a 30 unit step.
 */
EVT_BEGIN(gor_villager_urouro)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(urouro_init_func, STRING("me"), LW(0), LW(2), FLOAT(100.0f), FLOAT(30.0f), 0)
    CALL(urouro_main_func, STRING("me"))
    RETURN
EVT_END()

/**
 * Looping helper: plays Mario's sleeping voice at his position every 140 frames.
 * The Japanese release and later also remember the sound's id so it can be cut off when he wakes.
 */
EVT_BEGIN(inemuri_snd)
#if VERSION > VERSION_DEMO_US
    CALL(evt_gor_inemuri_snd_set, -1)
#endif
    LABEL(0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SLEEP1"), LW(0), LW(1), LW(2), 0)
#else
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SLEEP1"), LW(0), LW(1), LW(2), LW(3))
    CALL(evt_gor_inemuri_snd_set, LW(3))
#endif
    WAIT_FRAMES(140)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Plays while Mario listens to Luigi's long talk (started by luigi_gor_first_talk):
 * after 1200 frames (20 s) Mario dozes off and snores, after 900 frames the partner falls asleep too,
 * and after 600 frames Mario yawns and stretches.
 * It runs until GF(0) is set, then wakes Mario (and the partner) up, with a wake-up voice and a short stretch.
 */
EVT_BEGIN(mario_and_party_sleep)
    // Set the timers: Mario dozes off after 20 seconds, the partner after 15, and a first yawn comes at 10.
    SET_VALUE(LW(1), 1200)
    SET_VALUE(LW(2), 900)
    SET_VALUE(LW(3), 600)
    SET_VALUE(LF(0), 0)
    SET_VALUE(LW(15), -1)
    // Every frame while Luigi talks on:
    LOOP(0)
        // Mario nods off and starts to snore.
        IF_INT_EQUAL(LW(1), 0)
            CALL(evt_mario_set_pose, STRING("M_N_5A"))
            SPAWN_EVENT_GET_ID(PTR(inemuri_snd), LW(15))
        END_IF
        // The partner falls asleep as well, in its own sleeping pose.
        IF_INT_EQUAL(LW(2), 0)
            CALL(evt_mario_get_party, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                SWITCH(LW(0))
                    CASE_EQUAL(PARTY_MEMBER_GOOMBELLA)
                    CALL(evt_party_set_pose, 0, STRING("PKR_N_1"))
                    CASE_EQUAL(PARTY_MEMBER_KOOPS)
                    CALL(evt_party_set_pose, 0, STRING("PNK_N_1"))
                    CASE_EQUAL(PARTY_MEMBER_BOBBERY)
                    CALL(evt_party_set_pose, 0, STRING("PBM_N_1"))
                    CASE_EQUAL(PARTY_MEMBER_YOSHI)
                    CALL(evt_party_set_pose, 0, STRING("PYS_N_1"))
                    CASE_EQUAL(PARTY_MEMBER_FLURRIE)
                    CALL(evt_party_set_pose, 0, STRING("PWD_N_1"))
                    CASE_EQUAL(PARTY_MEMBER_VIVIAN)
                    CALL(evt_party_set_pose, 0, STRING("PTR_N_1"))
                    CASE_EQUAL(PARTY_MEMBER_MS_MOWZ)
                    CALL(evt_party_set_pose, 0, STRING("PCH_N_1"))
                END_SWITCH
            END_IF
        END_IF
        // Mario yawns and stretches, then goes back to standing; this repeats.
        IF_INT_EQUAL(LW(3), 0)
            IF_INT_EQUAL(LF(0), 0)
                SET_VALUE(LF(0), 1)
                SET_VALUE(LW(3), 51)
                CALL(evt_mario_get_pos, 0, LW(4), LW(5), LW(6))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_YAWN1"), LW(4), LW(5), LW(6), 0)
                CALL(evt_mario_set_pose, STRING("M_I_5"))
            ELSE
                SET_VALUE(LF(0), 0)
                CALL(evt_mario_set_pose, STRING("M_S_1"))
            END_IF
        END_IF
        WAIT_FRAMES(1)
        IF_INT_NOT_EQUAL(GF(0), 0)
            BREAK_LOOP
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(1), 0)
            SUBTRACT(LW(1), 1)
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(2), 0)
            SUBTRACT(LW(2), 1)
        END_IF
        IF_INT_GREATER_OR_EQUAL(LW(3), 0)
            SUBTRACT(LW(3), 1)
        END_IF
    END_LOOP
    // Luigi has finished: the snoring stops.
    IF_INT_NOT_EQUAL(LW(15), -1)
        DELETE_EVENT(LW(15))
#if VERSION > VERSION_DEMO_US
        CALL(evt_gor_inemuri_snd_off)
#endif
    END_IF
    // The partner wakes up.
    IF_INT_LESS_OR_EQUAL(LW(2), 0)
        CALL(evt_mario_get_party, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SWITCH(LW(0))
                CASE_EQUAL(PARTY_MEMBER_GOOMBELLA)
                CALL(evt_party_set_pose, 0, STRING("PKR_S_1"))
                CASE_EQUAL(PARTY_MEMBER_KOOPS)
                CALL(evt_party_set_pose, 0, STRING("PNK_S_1"))
                CASE_EQUAL(PARTY_MEMBER_BOBBERY)
                CALL(evt_party_set_pose, 0, STRING("PBM_S_1"))
                CASE_EQUAL(PARTY_MEMBER_YOSHI)
                CALL(evt_party_set_pose, 0, STRING("PYS_S_1"))
                CASE_EQUAL(PARTY_MEMBER_FLURRIE)
                CALL(evt_party_set_pose, 0, STRING("PWD_S_1"))
                CASE_EQUAL(PARTY_MEMBER_VIVIAN)
                CALL(evt_party_set_pose, 0, STRING("PTR_S_1"))
                CASE_EQUAL(PARTY_MEMBER_MS_MOWZ)
                CALL(evt_party_set_pose, 0, STRING("PCH_S_1"))
            END_SWITCH
        END_IF
    END_IF
    // If Mario was mid-yawn, let him finish.
    IF_INT_EQUAL(LF(0), 1)
        IF_INT_NOT_EQUAL(LW(3), 0)
            SUBTRACT(LW(3), 1)
        END_IF
        WAIT_FRAMES(LW(3))
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_IF
    // If Mario was asleep he wakes with a voice and a short stretch.
    IF_INT_LESS_OR_EQUAL(LW(1), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_AWAKE1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(550)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
    END_IF
    SET_VALUE(GF(0), 0)
    RETURN
EVT_END()

/**
 * Talk event for Luigi in Rogueport.
 * The first time he greets Mario (gor_luigi_00 and 01);
 * then he says a line that depends on the story chapter (gor_luigi_02 to 09)
 * and asks whether Mario wants to hear his stories (gor_luigi_yesno).
 * If so, Mario picks stories from a list (the johoya data gor_luigi_talk);
 * each plays with the long-talk music while Mario and the partner fall asleep (mario_and_party_sleep),
 * until he picks nothing or says no.
 */
EVT_BEGIN(luigi_gor_first_talk)
    CALL(evt_party_stop, 0)
    IF_INT_EQUAL(GSWF_GOR_LUIGI_FIRST_TALK_DONE, 0)
        CALL(evt_msg_print, 0, STRING("gor_luigi_00"), 0, STRING("me"))
        CALL(evt_mario_set_pose, STRING("M_I_2"))
        WAIT_MS(500)
#if VERSION <= VERSION_DEMO_US
        IF_INT_LESS(GSW_STORY_PROGRESS, STG8_16_THE_RETURN_OF_THE_QUEEN)
            CALL(evt_msg_print, 0, STRING("gor_luigi_01"), 0, STRING("me"))
#else
        IF_INT_LESS(GSW_STORY_PROGRESS, STG8_16_THE_RETURN_OF_THE_QUEEN)
            SET_VALUE(LW(2), STRING("gor_luigi_01"))
#endif
            ELSE
#if VERSION <= VERSION_DEMO_US
            CALL(evt_msg_print, 0, STRING("gor_luigi_01_1"), 0, STRING("me"))
#else
            SET_VALUE(LW(2), STRING("gor_luigi_01_1"))
#endif
        END_IF
#if VERSION > VERSION_DEMO_US
        CALL(evt_msg_print, 0, LW(2), 0, STRING("me"))
#endif
        CALL(evt_mario_set_pose, STRING("M_N_5B"))
        WAIT_MS(500)
        SET_VALUE(GSWF_GOR_LUIGI_FIRST_TALK_DONE, 1)
    END_IF
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(STG1_17_FATHER_SON_REUNITED, STG2_24_BOSS_BATTLE)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print, 0, STRING("gor_luigi_02"), 0, STRING("me"))
#else
        SET_VALUE(LW(2), STRING("gor_luigi_02"))
#endif
        CASE_BETWEEN(STG2_25_AFTER_THE_BOSS_BATTLE, STG3_31_GRUBBAS_PLANS)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print, 0, STRING("gor_luigi_03"), 0, STRING("me"))
#else
        SET_VALUE(LW(2), STRING("gor_luigi_03"))
#endif
        CASE_BETWEEN(STG3_32_BOSS_BATTLE, STG4_15_04_SHADOW_SIRENS_DOOPLISS)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print, 0, STRING("gor_luigi_04"), 0, STRING("me"))
#else
        SET_VALUE(LW(2), STRING("gor_luigi_04"))
#endif
        CASE_BETWEEN(STG4_15_05_PEACH_X_NAUT_PHD_REPORT, STG5_42_PIRATES_VS_X_NAUTS)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print, 0, STRING("gor_luigi_05"), 0, STRING("me"))
#else
        SET_VALUE(LW(2), STRING("gor_luigi_05"))
#endif
        CASE_BETWEEN(STG5_43_X_NAUTS_RETREAT, STG6_53_THE_REAL_GARNET_STAR)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print, 0, STRING("gor_luigi_06"), 0, STRING("me"))
#else
        SET_VALUE(LW(2), STRING("gor_luigi_06"))
#endif
        CASE_BETWEEN(STG6_54_WELCOME_BACK, STG7_19_THE_DOOR_TO_LORD_CRUMP)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print, 0, STRING("gor_luigi_07"), 0, STRING("me"))
        CASE_BETWEEN(STG7_20_LORD_CRUMP_DEFEATED, STORY_STG8_15_GRODUS)
        CALL(evt_msg_print, 0, STRING("gor_luigi_08"), 0, STRING("me"))
#else
        SET_VALUE(LW(2), STRING("gor_luigi_07"))
        CASE_BETWEEN(STG7_20_LORD_CRUMP_DEFEATED, STG8_15_GRODUS)
        SET_VALUE(LW(2), STRING("gor_luigi_08"))
#endif
        CASE_GREATER_OR_EQUAL(STG8_16_THE_RETURN_OF_THE_QUEEN)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print, 0, STRING("gor_luigi_09"), 0, STRING("me"))
#else
        SET_VALUE(LW(2), STRING("gor_luigi_09"))
#endif
    END_SWITCH
#if VERSION > VERSION_DEMO_US
    CALL(evt_msg_print, 0, LW(2), 0, STRING("me"))
#endif
    CALL(evt_msg_select, 0, STRING("gor_luigi_yesno"))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("gor_luigi_12"))
        GOTO(1)
    END_IF
    CALL(evt_msg_print_add, 0, STRING("gor_luigi_11"))
    CALL(johoya_data_alloc, STRING("gor_luigi_talk"))
    SET_VALUE(LW(10), 0)
    LABEL(0)
    CALL(evt_win_other_select, 18)
    IF_INT_EQUAL(LW(0), 0)
        CALL(johoya_data_free)
        CALL(evt_msg_print, 0, STRING("gor_luigi_13"), 0, STRING("me"))
        GOTO(1)
    END_IF
    IF_INT_EQUAL(LW(10), 0)
        CALL(evt_snd_bgmoff, PSND_PAUSE)
        CALL(evt_snd_bgmon_f, PSND_FADEIN | PSND_SLOT(1), STRING("BGM_EVT_RUIJI1"), 500)
        CALL(evt_snd_envoff, PSND_PAUSE)
        SET_VALUE(LW(10), 1)
    END_IF
    SET_VALUE(GF(0), 0)
    SPAWN_EVENT(PTR(mario_and_party_sleep))
    CALL(evt_msg_print, 0, LW(1), 0, STRING("me"))
    SET_VALUE(GF(0), 1)
    WAIT_WHILE(GF(0))
    CALL(johoya_luigi_newjoho_setreadflag)
    GOTO(0)
    LABEL(1)
    IF_INT_EQUAL(LW(10), 1)
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
        CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_IF
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

