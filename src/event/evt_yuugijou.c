// Script commands for the Pianta Parlor (yuugijou): minigame windows, Piantas and records.

#include "version.h"
#include "icon_ids.h"
#include "evt/evt_yuugijou.h"
#include "evt/evt_cmd.h"
#include "driver/icondrv.h"
#include "driver/windowdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "pmario_sound.h"
#include "sfx_name_word.h"
#include "evt/evt_bero.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"



//.bss
PiantaParlorWork yuugijouwork;

//.sbss
PiantaParlorWork* yuwp;

#if VERSION >= VERSION_JP
EvtWord* yuu_window_evt;
#endif

EVT_BEGIN(minigame_end)
    SET_VALUE(GW(0), 2)
    SET_VALUE(GSWF_GOR_RETURNING_FROM_MINIGAME, 1)
    CALL(evt_mario_set_prev_party_pos, 0, 0, -1000, 0)
    CALL(evt_mario_set_prev_party_pos, 1, 0, -1000, 0)
#if VERSION >= VERSION_JP
    CALL(evt_mario_set_enable_key, 0)
#endif
    CALL(evt_bero_mapchange, STRING("gor_03"), STRING("A_dokan_02"))
    RETURN
EVT_END()

/**
 * The pause window during a minigame.
 * JP and later first freeze Mario in place and run the minigame's own window script (evtYuuSetWindowEvt);
 * then it asks whether to quit and runs minigame_end on the first choice, then flags it closed.
 */
EVT_BEGIN(window_open)
#if VERSION >= VERSION_JP
    CALL(evt_mario_cont_onoff, 0)
    BEGIN_SIBLING_EVENT
        CALL(evt_mario_get_pos, 0, LW(1), LW(2), LW(3))
        LOOP(0)
            WAIT_FRAMES(1)
            CALL(evt_mario_set_pos, LW(1), LW(2), LW(3))
        END_LOOP
    END_SIBLING_EVENT
    CALL(yuuminigame_get_window_evt, LW(0))
    CALL_CHILD(LW(0))
#endif
    CALL(evt_msg_print, 0, STRING("sysyuu_all_01"), 0, 0)
    CALL(evt_msg_select, 0, STRING("sysyuu_all_02"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        CALL(yuuminigame_end)
    END_IF
    CALL(yuuminigame_window_end)
#if VERSION >= VERSION_JP
    CALL(evt_mario_cont_onoff, 1)
#endif
    RETURN
EVT_END()

/** Clamps a value to zero from below; used for the per-rank values of the default records. */
static inline s32 MAX0(s32 x) {
    return x >= 0 ? x : 0;
}

/**
 * Resets the Pianta Parlor work: token counts and flags cleared,
 * and the default record tables (five ranks per minigame, score plus a derived per-rank value) filled in.
 * Called at boot (mariost.c) and on a new game (stg0_00_init in event.c).
 * The early demos have their own records; US and later keep eight record rows, JP four.
 */
void yuugijou_init(void) {
    s32 t;
    yuwp = &yuugijouwork;
    memset(yuwp, 0, sizeof(PiantaParlorWork));

    yuwp->recordOn[0][0] = 1;
    yuwp->recordOn[0][1] = 1;
    yuwp->recordOn[0][2] = 1;
    yuwp->recordOn[0][3] = 1;
    yuwp->recordOn[0][4] = 1;

#if VERSION <= VERSION_DEMO_JP
    yuwp->recordScore[0][0] = 0x18EE8;
    yuwp->recordScore[0][1] = 0x1708E;
    yuwp->recordScore[0][2] = 0x13F42;
    yuwp->recordScore[0][3] = 0x1318C;
    yuwp->recordScore[0][4] = 0x125FC;

    yuwp->recordParam[0][0] = 0x1F;
    yuwp->recordParam[0][1] = 0x18;
    yuwp->recordParam[0][2] = 0x16;
    yuwp->recordParam[0][3] = 0x11;
    yuwp->recordParam[0][4] = 0xB;
#else
    yuwp->recordScore[0][0] = 0x5B964;
    yuwp->recordScore[0][1] = 0x5825A;
    yuwp->recordScore[0][2] = 0x4F826;
    yuwp->recordScore[0][3] = 0x4C3D8;
    yuwp->recordScore[0][4] = 0x4237E;

    yuwp->recordParam[0][0] = 0xE;
    yuwp->recordParam[0][1] = 0xC;
    yuwp->recordParam[0][2] = 0xF;
    yuwp->recordParam[0][3] = 0xB;
    yuwp->recordParam[0][4] = 0xA;
#endif
    yuwp->recordOn[1][0] = 1;
    yuwp->recordOn[1][1] = 1;
    yuwp->recordOn[1][2] = 1;
    yuwp->recordOn[1][3] = 1;
    yuwp->recordOn[1][4] = 1;

#if VERSION <= VERSION_DEMO_JP
    yuwp->recordScore[1][0] = 0x1D2;
    yuwp->recordScore[1][1] = 0x1EA;
    yuwp->recordScore[1][2] = 0x200;
    yuwp->recordScore[1][3] = 0x21B;
    yuwp->recordScore[1][4] = 0x22A;

    yuwp->recordParam[1][0] = 0x18;
    yuwp->recordParam[1][1] = 0x15;
    yuwp->recordParam[1][2] = 0x12;
    yuwp->recordParam[1][3] = 0xE;
    yuwp->recordParam[1][4] = 0x8;
#else
    yuwp->recordScore[1][0] = 2;
    yuwp->recordScore[1][1] = 5;
    yuwp->recordScore[1][2] = 0x12;
    yuwp->recordScore[1][3] = 0x1B;
    yuwp->recordScore[1][4] = 0x8A;

    yuwp->recordParam[1][0] = MAX0(((700 - (s32)yuwp->recordScore[1][0]) * 15) / 700) + 1;
    t = (700 - (s32)yuwp->recordScore[1][1]) * 15;
    yuwp->recordParam[1][1] = MAX0(t / 700);
    t = (700 - (s32)yuwp->recordScore[1][2]) * 15;
    yuwp->recordParam[1][2] = MAX0(t / 700);
    t = (700 - (s32)yuwp->recordScore[1][3]) * 15;
    yuwp->recordParam[1][3] = MAX0(t / 700);
    t = (700 - (s32)yuwp->recordScore[1][4]) * 15;
    yuwp->recordParam[1][4] = MAX0(t / 700);
#endif
    yuwp->recordOn[2][0] = 1;
    yuwp->recordOn[2][1] = 1;
    yuwp->recordOn[2][2] = 1;
    yuwp->recordOn[2][3] = 1;
    yuwp->recordOn[2][4] = 1;

#if VERSION <= VERSION_DEMO_JP
    yuwp->recordScore[2][0] = 0x62E8;
    yuwp->recordScore[2][1] = 0x5410;
    yuwp->recordScore[2][2] = 0x4B96;
    yuwp->recordScore[2][3] = 0x2D82;
    yuwp->recordScore[2][4] = 0x23DC;

    yuwp->recordParam[2][0] = 0x25;
    yuwp->recordParam[2][1] = 0x20;
    yuwp->recordParam[2][2] = 0x1B;
    yuwp->recordParam[2][3] = 0xC;
    yuwp->recordParam[2][4] = 0x9;
#else
    yuwp->recordScore[2][0] = 0x20850;
    yuwp->recordScore[2][1] = 0x20436;
    yuwp->recordScore[2][2] = 0x20184;
    yuwp->recordScore[2][3] = 0x1F91E;
    yuwp->recordScore[2][4] = 0x1E3AC;

    yuwp->recordParam[2][0] = MAX0(((s32)yuwp->recordScore[2][0] - 109000) / 2000) + 7;
    t = (s32)yuwp->recordScore[2][1] - 109000;
    yuwp->recordParam[2][1] = MAX0(t / 2000) + 6;
    yuwp->recordParam[2][2] = MAX0(((s32)yuwp->recordScore[2][2] - 109000) / 2000) + 4;
    t = (s32)yuwp->recordScore[2][3] - 109000;
    yuwp->recordParam[2][3] = MAX0(t / 2000) + 4;
#if VERSION >= VERSION_US
    yuwp->recordParam[2][4] = ((t = ((s32)yuwp->recordScore[2][4] - 109000) / 2000) >= 0 ? t : 0) + 1;
#else
    yuwp->recordParam[2][4] = MAX0(((s32)yuwp->recordScore[2][4] - 109000) / 2000) + 1;
#endif
#endif
    yuwp->recordOn[3][0] = 1;
    yuwp->recordOn[3][1] = 1;
    yuwp->recordOn[3][2] = 1;
    yuwp->recordOn[3][3] = 1;
    yuwp->recordOn[3][4] = 1;

#if VERSION <= VERSION_DEMO_JP
    yuwp->recordScore[3][0] = 0x4EFC;
    yuwp->recordScore[3][1] = 0x4416;
    yuwp->recordScore[3][2] = 0x302A;
    yuwp->recordScore[3][3] = 0x2044;
    yuwp->recordScore[3][4] = 0x1428;

    yuwp->recordParam[3][0] = 0x21;
    yuwp->recordParam[3][1] = 0x1C;
    yuwp->recordParam[3][2] = 0x16;
    yuwp->recordParam[3][3] = 0x10;
    yuwp->recordParam[3][4] = 0xC;
#else
    yuwp->recordScore[3][0] = 0x16BDE;
    yuwp->recordScore[3][1] = 0x16968;
    yuwp->recordScore[3][2] = 0x16382;
    yuwp->recordScore[3][3] = 0x1627E;
    yuwp->recordScore[3][4] = 0x15950;

#if VERSION >= VERSION_US
    yuwp->recordParam[3][0] = ((t = ((s32)yuwp->recordScore[3][0] - 74000) / 2000) >= 0 ? t : 0) + 7;
    yuwp->recordParam[3][1] = ((t = ((s32)yuwp->recordScore[3][1] - 74000) / 2000) >= 0 ? t : 0) + 6;
    yuwp->recordParam[3][2] = ((t = ((s32)yuwp->recordScore[3][2] - 74000) / 2000) >= 0 ? t : 0) + 4;
    yuwp->recordParam[3][3] = ((t = ((s32)yuwp->recordScore[3][3] - 74000) / 2000) >= 0 ? t : 0) + 3;
    yuwp->recordParam[3][4] = ((t = ((s32)yuwp->recordScore[3][4] - 74000) / 2000) >= 0 ? t : 0) + 4;

    yuwp->recordOn[4][0] = 1;
    yuwp->recordOn[4][1] = 1;
    yuwp->recordOn[4][2] = 1;
    yuwp->recordOn[4][3] = 1;
    yuwp->recordOn[4][4] = 1;

    yuwp->recordScore[4][0] = 0x5B964;
    yuwp->recordScore[4][1] = 0x5825A;
    yuwp->recordScore[4][2] = 0x4F826;
    yuwp->recordScore[4][3] = 0x4C3D8;
    yuwp->recordScore[4][4] = 0x4237E;

    yuwp->recordParam[4][0] = 0xE;
    yuwp->recordParam[4][1] = 0xC;
    yuwp->recordParam[4][2] = 0xF;
    yuwp->recordParam[4][3] = 0xB;
    yuwp->recordParam[4][4] = 0xA;

    yuwp->recordOn[5][0] = 1;
    yuwp->recordOn[5][1] = 1;
    yuwp->recordOn[5][2] = 1;
    yuwp->recordOn[5][3] = 1;
    yuwp->recordOn[5][4] = 1;

    yuwp->recordScore[5][0] = 2;
    yuwp->recordScore[5][1] = 5;
    yuwp->recordScore[5][2] = 0x12;
    yuwp->recordScore[5][3] = 0x1B;
    yuwp->recordScore[5][4] = 0x8A;

    yuwp->recordParam[5][0] = MAX0(((700 - (s32)yuwp->recordScore[5][0]) * 15) / 700) + 1;
    t = (700 - (s32)yuwp->recordScore[5][1]) * 15;
    yuwp->recordParam[5][1] = MAX0(t / 700);
    t = (700 - (s32)yuwp->recordScore[5][2]) * 15;
    yuwp->recordParam[5][2] = MAX0(t / 700);
    t = (700 - (s32)yuwp->recordScore[5][3]) * 15;
    yuwp->recordParam[5][3] = MAX0(t / 700);
    yuwp->recordParam[5][4] = MAX0(((700 - (s32)yuwp->recordScore[5][4]) * 15) / 700);

    yuwp->recordOn[6][0] = 1;
    yuwp->recordOn[6][1] = 1;
    yuwp->recordOn[6][2] = 1;
    yuwp->recordOn[6][3] = 1;
    yuwp->recordOn[6][4] = 1;

    yuwp->recordScore[6][0] = 0x1E3DE;
    yuwp->recordScore[6][1] = 0x1E00A;
    yuwp->recordScore[6][2] = 0x1DD8A;
    yuwp->recordScore[6][3] = 0x1D5BA;
    yuwp->recordScore[6][4] = 0x1C1CE;

    yuwp->recordParam[6][0] = MAX0(((s32)yuwp->recordScore[6][0] - 109000) / 2000) + 7;
    t = (s32)yuwp->recordScore[6][1] - 109000;
    yuwp->recordParam[6][1] = MAX0(t / 2000) + 6;
    yuwp->recordParam[6][2] = MAX0(((s32)yuwp->recordScore[6][2] - 109000) / 2000) + 4;
    t = (s32)yuwp->recordScore[6][3] - 109000;
    yuwp->recordParam[6][3] = MAX0(t / 2000) + 4;
    t = (s32)yuwp->recordScore[6][4] - 109000;
    yuwp->recordParam[6][4] = MAX0(t / 2000) + 1;

    yuwp->recordOn[7][0] = 1;
    yuwp->recordOn[7][1] = 1;
    yuwp->recordOn[7][2] = 1;
    yuwp->recordOn[7][3] = 1;
    yuwp->recordOn[7][4] = 1;

    yuwp->recordScore[7][0] = 0x1525C;
    yuwp->recordScore[7][1] = 0x15018;
    yuwp->recordScore[7][2] = 0x14A96;
    yuwp->recordScore[7][3] = 0x149A6;
    yuwp->recordScore[7][4] = 0x14122;

    t = (s32)yuwp->recordScore[7][0] - 74000;
    yuwp->recordParam[7][0] = MAX0(t / 2000) + 7;
    t = (s32)yuwp->recordScore[7][1] - 74000;
    yuwp->recordParam[7][1] = MAX0(t / 2000) + 6;
    yuwp->recordParam[7][2] = MAX0(((s32)yuwp->recordScore[7][2] - 74000) / 2000) + 4;
    t = (s32)yuwp->recordScore[7][3] - 74000;
    yuwp->recordParam[7][3] = MAX0(t / 2000) + 3;
    yuwp->recordParam[7][4] = MAX0(((s32)yuwp->recordScore[7][4] - 74000) / 2000) + 4;
#else
    t = (s32)yuwp->recordScore[3][0] - 74000;
    yuwp->recordParam[3][0] = MAX0(t / 2000) + 7;
    t = (s32)yuwp->recordScore[3][1] - 74000;
    yuwp->recordParam[3][1] = MAX0(t / 2000) + 6;
    yuwp->recordParam[3][2] = MAX0(((s32)yuwp->recordScore[3][2] - 74000) / 2000) + 4;
    t = (s32)yuwp->recordScore[3][3] - 74000;
    yuwp->recordParam[3][3] = MAX0(t / 2000) + 3;
    yuwp->recordParam[3][4] = MAX0(((s32)yuwp->recordScore[3][4] - 74000) / 2000) + 4;
#endif
#endif
}
static inline s32 cmpSign(s32 a, s32 b) {
    if (a > b) {
        return 1;
    }
    return -1;
}

/**
 * Draws the Pianta token counter (window, token icons and number) while flags 1 and 2 are set,
 * higher on screen with flag 4.
 * Every 4th frame the shown count steps a tenth of the way (at least 1) toward the real count,
 * with a tick sound; JP and later space the sound out to every 8 steps.
 * Called from the draw callbacks of gor_03 (the Pianta Changer)
 * and of the Pianta Parlor games (yuu_00 to yuu_03).
 */
void monteCountDisp(void) {
    static s32 monte_snd_flag;
    s32 y = -0xB4;
    s32 delta;
    u32 flags = yuwp->flags;
    Mtx mtx;
    GXColor c1, c2, c3, c4;
#if VERSION <= VERSION_DEMO_JP
    if (flags & PARLOR_FLAG_COUNTER_HIGH)
#else
    if (flags & PARLOR_FLAG_COUNTER_HIGH) {
#endif
        y = -0x78;
#if VERSION <= VERSION_DEMO_JP
        if (!(flags & PARLOR_FLAG_COUNTER_SHOWN) || !(flags & PARLOR_FLAG_TOKENS_OUT))
#else
    }
    if (!(flags & PARLOR_FLAG_COUNTER_SHOWN) || !(flags & PARLOR_FLAG_TOKENS_OUT)) {
#endif
        return;
#if VERSION <= VERSION_DEMO_JP
        {
            {
#else
    }
#endif
    c1 = (GXColor){255, 255, 255, 153};
    windowDispGX_Waku_col(0, c1, -288.0f, (f32)(y + 0x2C), 200.0f, 44.0f, 16.0f);
    PSMTXTrans(mtx, -260.0f, (f32)(y + 2), 0.0f);
    c2 = (GXColor){255, 255, 255, 255};
    iconDispGxCol(mtx, ICON_FLAG_2D, ICON_ITEM_PIANTA_TOKEN, c2);
    PSMTXTrans(mtx, -230.0f, (f32)(y + 2), 0.0f);
    c3 = (GXColor){255, 255, 255, 255};
    iconDispGxCol(mtx, ICON_FLAG_2D, ICON_TIMES, c3);
    PSMTXTrans(mtx, -128.0f, (f32)(y + 2), 0.0f);
    c4 = (GXColor){255, 255, 255, 255};
    iconNumberDispGx(mtx, yuwp->dispTokens, 0, c4);
    yuwp->dispTick++;
    if ((yuwp->dispTick & 3) == 0) {
        delta = yuwp->tokens - yuwp->dispTokens;
        yuwp->dispTick = 0;
        if (monte_snd_flag == 0 && delta != 0) {
            psndSFXOn(SFX_NAME_WORD(PSND_SFX_EVT_GAME_MONTE_COUNT1));
            monte_snd_flag = 8;
        }
#if VERSION <= VERSION_DEMO_JP
        monte_snd_flag = 0;
#endif
        if (delta != 0) {
            delta = delta / 10;
            if (delta == 0) {
                delta = (s32)yuwp->tokens > (s32)yuwp->dispTokens ? 1 : -1;
            }
            yuwp->dispTokens += delta;
        }
    }
#if VERSION <= VERSION_DEMO_JP
    monte_snd_flag = 0;
#else
    if (monte_snd_flag != 0) {
        monte_snd_flag--;
#endif
    }
}
#if VERSION <= VERSION_DEMO_JP
}
#endif

/**
 * Adds args[1] Pianta tokens (negative to spend). JP and later cap the total at 99999; the early demos do not.
 * A nonzero args[0] snaps the counter display to the new total instead of letting it count up.
 */
USER_FUNC(yuugijou_add_montemedal) {
    EvtWord* args = event->args;
    s32 immediate = evtGetValue(event, args[0]);
#if VERSION <= VERSION_DEMO_JP
    yuwp->tokens += evtGetValue(event, args[1]);
    if (immediate != 0)
        yuwp->dispTokens = yuwp->tokens;
#else
    s32 sum = yuwp->tokens + evtGetValue(event, args[1]);
    if (sum < 99999) {
        yuwp->tokens = sum;
    } else {
        yuwp->tokens = 99999;
    }
    if (immediate != 0) {
        yuwp->dispTokens = yuwp->tokens;
    }
#endif
    return EVT_RETURN_DONE;
}

/** Writes the Pianta tokens Mario is holding to args[0]. */
USER_FUNC(yuugijou_get_montemedal) {
    evtSetValue(event, event->args[0], yuwp->tokens);
    return EVT_RETURN_DONE;
}

/** Writes to args[0] the tokens left in deposit (yuugijou_montemedal_keep), or 0 while the tokens are taken out. */
USER_FUNC(yuugijou_get_montemedal_keep) {
    EvtWord* args = event->args;
    if (yuwp->flags & PARLOR_FLAG_TOKENS_OUT) {
        evtSetValue(event, args[0], 0);
    } else {
        evtSetValue(event, args[0], yuwp->keptTokens);
    }
    return EVT_RETURN_DONE;
}

/**
 * Adds args[1] to keptTokens while the tokens are taken out (flag 2); args[0] is read but ignored.
 * keptTokens is overwritten by the next yuugijou_montemedal_keep, so nothing comes of it.
 * Its one caller, gor_03's Pianta Changer (exchange_set), passes only the tokens bought,
 * so args[1] reads the next script word.
 */
USER_FUNC(yuugijou_add_montemedal_moto) {
    EvtWord* args = event->args;
    evtGetValue(event, args[0]);
    if (yuwp->flags & PARLOR_FLAG_TOKENS_OUT) {
        yuwp->keptTokens += evtGetValue(event, args[1]);
    }
    return EVT_RETURN_DONE;
}

/** Shows (args[0] nonzero, display snapped to the real count) or hides the Pianta token counter. */
USER_FUNC(yuugijou_montemedal_disp_onoff) {
    if (evtGetValue(event, event->args[0]) != 0) {
        yuwp->flags |= PARLOR_FLAG_COUNTER_SHOWN;
        yuwp->dispTokens = yuwp->tokens;
    } else {
        yuwp->flags &= ~PARLOR_FLAG_COUNTER_SHOWN;
    }
    return EVT_RETURN_DONE;
}

/** Blocks the script until the token counter has finished counting up or down to the real total. */
USER_FUNC(yuugijou_montemedal_wait) {
    if ((s32)yuwp->dispTokens != yuwp->tokens) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Deposits all held tokens: moves them to keptTokens, zeroes the held count and hides the counter.
 * Does nothing unless the tokens are currently taken out.
 */
USER_FUNC(yuugijou_montemedal_keep) {
    if (yuwp->flags & PARLOR_FLAG_TOKENS_OUT) {
        yuwp->keptTokens = yuwp->tokens;
        yuwp->tokens = 0;
        yuwp->dispTokens = 0;
        yuwp->flags &= ~PARLOR_FLAG_TOKENS_OUT;
        yuwp->flags &= ~PARLOR_FLAG_COUNTER_SHOWN;
    }
    return EVT_RETURN_DONE;
}

/** Takes the deposited tokens back out and shows the counter again; does nothing if they are already out. */
USER_FUNC(yuugijou_montemedal_return) {
    if (!(yuwp->flags & PARLOR_FLAG_TOKENS_OUT)) {
        yuwp->dispTokens = yuwp->tokens = yuwp->keptTokens;
        yuwp->flags |= PARLOR_FLAG_TOKENS_OUT;
        yuwp->flags |= PARLOR_FLAG_COUNTER_SHOWN;
    }
    return EVT_RETURN_DONE;
}

/** Writes 1 to args[0] while Mario has his tokens taken out (flag 2), else 0. */
USER_FUNC(yuugijou_get_takeoutflag) {
    EvtWord* args = event->args;
    if (yuwp->flags & PARLOR_FLAG_TOKENS_OUT) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

/** Sets (args[0] nonzero) or clears flag 4, which draws the token counter higher on screen. */
USER_FUNC(yuugijou_set_yupflag) {
    if (evtGetValue(event, event->args[0]) != 0) {
        yuwp->flags |= PARLOR_FLAG_COUNTER_HIGH;
    } else {
        yuwp->flags &= ~PARLOR_FLAG_COUNTER_HIGH;
    }
    return EVT_RETURN_DONE;
}

/** Flags the pause window as finished (flag 8), which evtYuuWindowEndChk reports to the window code. */
USER_FUNC(yuuminigame_window_end) {
    yuwp->flags |= PARLOR_FLAG_WINDOW_DONE;
    return EVT_RETURN_DONE;
}

/** Starts minigame_end as a separate event so the quit warp runs after the pause window. */
USER_FUNC(yuuminigame_end) {
    evtEntryType(minigame_end, 0, 0, 0xEF);
    return EVT_RETURN_DONE;
}

// Descriptive name (not in the symbol maps); placeholder unk_802243a4, JP 0x802243A4. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
USER_FUNC(yuuminigame_get_window_evt) {
    evtSetValue(event, event->args[0], (EvtWord)yuu_window_evt);
    return EVT_RETURN_DONE;
}
#endif

// Descriptive name (not in the symbol maps); placeholder unk_8022439c, JP 0x8022439C. See docs/NAME_PROVENANCE.md.
#if VERSION >= VERSION_JP
/** JP and later: sets the script window_open runs as a child before the quit prompt; each minigame sets its own. */
void evtYuuSetWindowEvt(EvtWord* evt) {
    yuu_window_evt = evt;
}
#endif

/** Opens the minigame pause window (window_open); called by the menu code (win_main.c, state 400). */
void evtYuuWindow(void) {
    yuwp->flags &= ~PARLOR_FLAG_WINDOW_DONE;
    evtEntryType(window_open, 0, 0, 0);
}

/** Returns 1 once the minigame pause window has closed; win_main.c polls it to leave state 401. */
s32 evtYuuWindowEndChk(void) {
    return (yuwp->flags >> 3) & 1;
}
