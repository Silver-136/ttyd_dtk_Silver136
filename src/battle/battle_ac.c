#include "battle/battle_ac.h"
#include "battle/battle.h"
#include "battle/ac/ac_table.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "driver/icondrv.h"
#include "battle/battle_disp.h"
#include "battle/battle_unit.h"
#include "icon_ids.h"
#include "pmario_sound.h"
#include "battle/battle_audience.h"
#include "battle/battle_pad.h"

void psndSFX_pit(s32 id, s32 pitch);

// local prototypes
s32 BattleACPadCheckRecordTrigger(s32 frame, s32 mask);

/** Clears the action-command manager at battle start: resets its pad history and drops every command callback. */
void BattleActionCommandManagerInit(BattleWork* wp) {
    BtlPad_WorkInit(&wp->actionCommands.pad);
    wp->actionCommands.maincb = NULL;
    wp->actionCommands.resultcb = NULL;
    wp->actionCommands.dispcb = NULL;
    wp->actionCommands.deletecb = NULL;
    wp->actionCommands.seId = -1;
}

/**
 * Per-frame action-command driver: records the pad, runs the active command's main callback until it returns 0,
 * and queues its draw callback on the 2D camera.
 */
void BattleActionCommandManager(BattleWork* wp) {
    BattleACMainCallback main;
    DispCallback disp;

    BtlPad_WorkUpdate(&wp->actionCommands.pad, 0);
    main = wp->actionCommands.maincb;
    if (main) {
        if (!main(wp)) {
            wp->actionCommands.maincb = NULL;
        }

        disp = wp->actionCommands.dispcb;
        if (disp) {
            dispEntry(CAMERA_2D, 900.0f, 1, disp, wp);
        }
    }
}

/**
 * Returns the active command's result through its result callback,
 * or bit 1 of the raw result (success) when it has none.
 */
s32 BattleActionCommandResult(BattleWork* wp) {
    BattleACResultCallback result;

    result = wp->actionCommands.resultcb;
    if (result) {
        return result(wp);
    } else {
        return wp->actionCommands.result & AC_RESULT_SUCCESS;
    }
}

/**
 * Reports an action command's outcome to the crowd: -1 counts as a flop (audience reacts badly);
 * otherwise the weapon, its Bingo-slot chance and a Star Power multiplier of 0.5 + 0.25 * result are stored for
 * the bonus that follows.
 */
void BattleActionCommandDeclareACResult(BattleWork* work, BattleWeapon* weapon, s32 result) {
    StarPowerInfo* info = &work->impendingBonuses;
    switch (result) {
    case -1:
        BattleAudience_Case_ActionCommandBad(weapon);
        break;
    default:
        info->mWeapon = weapon;
        info->mSpAcSuccessMultiplier = 0.25f * (f32)result + 0.5f;
        info->reserved_09 = weapon->reserved_19;
        info->mBingoSlotChance = weapon->mBingoSlotChance;
        BattleAudience_Case_ActionCommandGood(weapon);
        break;
    }
}

/**
 * Prepares action command id (from ActionCommandList) for unit:
 * stores the setup flags and parameter, resets the state and result, and installs the command's main,
 * result and delete callbacks. A confused unit gets no on-screen prompt (no draw callback).
 */
void BattleActionCommandSetup(BattleWork* work, s32 id, BattleUnit* unit, s32 setupFlags, s32 setupParam) {
    ActionCommandEntry* entry;

    entry = ActionCommandList;
    work->actionCommands.mAcUnit = unit;
    work->actionCommands.setupFlags = setupFlags;
    work->actionCommands.mAcState = 0;
    work->actionCommands.mResultCount = 0;
    work->actionCommands.result = AC_RESULT_RUNNING;
    work->actionCommands.maincb = NULL;
    work->actionCommands.dispcb = NULL;
    work->actionCommands.resultcb = NULL;
    work->actionCommands.deletecb = NULL;
    work->actionCommands.setupParam = setupParam;
    work->actionCommands.doneFlags = 0;
    for (; entry->id; entry++) {
        if (entry->id == id) {
            work->actionCommands.maincb = entry->main;
            if (!(work->actionCommands.mAcUnit->mTokenFlags & kConfusedAction)) {
                work->actionCommands.dispcb = entry->disp;
            }
            work->actionCommands.resultcb = entry->result;
            work->actionCommands.deletecb = entry->delete;
            return;
        }
    }
}

/** Starts the prepared action command (state 100 tells its main callback to begin). */
void BattleActionCommandStart(BattleWork* work) {
    work->actionCommands.mAcState = 100;
}

/** Ends the active action command early through its delete callback. */
void BattleActionCommandStop(BattleWork* work) {
    if (work->actionCommands.deletecb) {
        work->actionCommands.deletecb(work);
    }
}

s32 BattleActionCommandCheckDefence(BattleUnit* unit, BattleWeapon* weapon) {
    BattleWork* wp;
    BattleWorkPad* pad;
    s32 i;
    s32 count;
    s32 done;
    s32 superguard;
    s32 normal;
    s32 diff;
    const u8 normal_frames[7] = {10, 9, 8, 8, 7, 6, 5};
    const u8 super_frames[7] = {5, 4, 3, 3, 2, 1, 1};

    done = 0;
    wp = _battleWorkPointer;
    pad = &wp->actionCommands.pad;
    wp->actionCommands.mDefenseResult = 0;
    diff = unit->badgesEquipped.Unsimplifier + 3;
    diff -= unit->badgesEquipped.Simplifier;
    if (diff < 0) {
        diff = 0;
    }
    if (diff > 6) {
        diff = 6;
    }
    superguard = super_frames[diff];
    normal = normal_frames[diff];

    if (weapon->superguardsAllowed && unit->currentHp > 0) {
        if (wp->actionCommands.countdown > 0) {
            wp->actionCommands.countdown--;
            done = 1;
            wp->actionCommands.mDefenseResult = 5;
        } else if (unit->badgesEquipped.AutoCommand) {
            done = 1;
            wp->actionCommands.mDefenseResult = 5;
        } else {
            for (i = 0; i < superguard; i++) {
                if (BattleACPadCheckRecordTrigger(i, PAD_BUTTON_B)) {
                    break;
                }
            }
            if (i < superguard) {
                count = 0;
                for (i = 0; i < 15; i++) {
                    if (BattleACPadCheckRecordTrigger(i, PAD_BUTTON_A)) {
                        count++;
                    }
                    if (BattleACPadCheckRecordTrigger(i, PAD_BUTTON_B)) {
                        count++;
                    }
                }
                if (count < 2) {
                    done = 1;
                    wp->actionCommands.mDefenseResult = 5;
                }
            }
        }
    }

    if (!done) {
        if (wp->actionCommands.countdown > 0) {
            wp->actionCommands.countdown--;
            wp->actionCommands.mDefenseResult = 4;
        } else {
            count = 0;
            for (i = 0; i < 15; i++) {
                if (BattleACPadCheckRecordTrigger(i, PAD_BUTTON_A)) {
                    count++;
                }
                if (BattleACPadCheckRecordTrigger(i, PAD_BUTTON_B)) {
                    count++;
                }
            }
            if (count >= 2) {
                wp->actionCommands.mDefenseResult = 1;
            } else {
                for (i = 0; i < normal; i++) {
                    if (BattleACPadCheckRecordTrigger(i, PAD_BUTTON_A)) {
                        break;
                    }
                }
                if (i >= normal) {
                    if (count > 0) {
                        wp->actionCommands.mDefenseResult = 1;
                    } else {
                        wp->actionCommands.mDefenseResult = 2;
                    }
                } else {
                    wp->actionCommands.mDefenseResult = 4;
                }
            }
        }
    }
    BtlPad_WorkInit(pad);
    return wp->actionCommands.mDefenseResult;
}

/** Returns the buttons in mask that were newly pressed chan frames ago in the action-command pad history. */
s32 BattleACPadCheckRecordTrigger(s32 frame, s32 mask) {
    return _battleWorkPointer->actionCommands.pad.mButtonsPressedHistory[frame] & mask;
}

/** Returns the last guard result from BattleActionCommandCheckDefence (5 Superguard, 4 Guard, 2 none, 1 mashed). */
s32 BattleActionCommandGetDefenceResult(void) {
    return _battleWorkPointer->actionCommands.mDefenseResult;
}

/**
 * Clears the guard result and the pad history, so damage that is not guardable (audience items) counts as
 * unguarded.
 */
void BattleActionCommandResetDefenceResult(void) {
    BattleWork* wp = _battleWorkPointer;
    wp->actionCommands.mDefenseResult = 0;
    BtlPad_WorkInit(&wp->actionCommands.pad);
}

/** Returns the action-command difficulty level 0-6 after Simplifier/Unsimplifier adjustments. */
s32 BattleActionCommandGetDifficulty(BattleWork* work) {
    return work->actionCommands.mAcDifficulty;
}

/**
 * Sets the base action-command difficulty and the adjusted one:
 * minus one per Simplifier, plus one per Unsimplifier the unit wears, clamped to 0-6.
 */
void BattleActionCommandSetDifficulty(BattleWork* work, BattleUnit* unit, s32 difficulty) {
    s32 adjusted;

    work->actionCommands.mBaseAcDifficulty = difficulty;
    adjusted = difficulty - unit->badgesEquipped.Simplifier;
    adjusted += unit->badgesEquipped.Unsimplifier;
    if (adjusted < 0) {
        adjusted = 0;
    }
    if (adjusted > 6) {
        adjusted = 6;
    }
    work->actionCommands.mAcDifficulty = adjusted;
}

/**
 * Returns the prize level (0-6) for a successful command, lv plus the difficulty minus one,
 * so harder commands earn the better rating; -1 when the unit's damage code is 0x1E.
 */
s32 BattleActionCommandGetPrizeLv(BattleWork* work, BattleUnit* unit, s32 lv) {
    if (unit != NULL && (u8)unit->damageCode == DAMAGE_CODE_NO_DAMAGE) {
        return -1;
    }
    lv += work->actionCommands.mAcDifficulty;
    lv--;
    if (lv < 0) {
        lv = 0;
    }
    if (lv >= 7) {
        lv = 6;
    }
    return lv;
}

/**
 * Returns the icon to show for a button in an action-command prompt (A, B, X, Y, Z, L, R and the stick
 * directions), in its pressed or released form.
 */
u16 BattleACGetButtonIcon(s32 button, s32 pressed) {
    u16 icon = 0;

    switch (button) {
    case PAD_BUTTON_A:
        if (pressed) {
            icon = ICON_BUTTON_A_PRESSED;
        } else {
            icon = ICON_BUTTON_A;
        }
        break;
    case PAD_BUTTON_B:
        if (pressed) {
            icon = ICON_BUTTON_B_PRESSED;
        } else {
            icon = ICON_BUTTON_B;
        }
        break;
    case PAD_BUTTON_X:
        if (pressed) {
            icon = ICON_BUTTON_X_PRESSED;
        } else {
            icon = ICON_BUTTON_X;
        }
        break;
    case PAD_BUTTON_Y:
        if (pressed) {
            icon = ICON_BUTTON_Y_PRESSED;
        } else {
            icon = ICON_BUTTON_Y;
        }
        break;
    case PAD_TRIGGER_R:
        if (pressed) {
            icon = ICON_TRIGGER_R_PRESSED;
        } else {
            icon = ICON_TRIGGER_R;
        }
        break;
    case PAD_TRIGGER_L:
        if (pressed) {
            icon = ICON_TRIGGER_L_PRESSED;
        } else {
            icon = ICON_TRIGGER_L;
        }
        break;
    case PAD_TRIGGER_Z:
        if (pressed) {
            icon = ICON_TRIGGER_Z_PRESSED;
        } else {
            icon = ICON_TRIGGER_Z;
        }
        break;
    case BATTLE_PAD_STICK_UP:
        if (pressed) {
            icon = ICON_STICK_UP;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    case BATTLE_PAD_STICK_DOWN:
        if (pressed) {
            icon = ICON_STICK_DOWN;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    case BATTLE_PAD_STICK_LEFT:
        if (pressed) {
            icon = ICON_STICK_LEFT;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    case BATTLE_PAD_STICK_RIGHT:
        if (pressed) {
            icon = ICON_STICK_RIGHT;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    case BATTLE_PAD_STICK_UP | BATTLE_PAD_STICK_RIGHT:
        if (pressed) {
            icon = ICON_STICK_UP_RIGHT;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    case BATTLE_PAD_STICK_DOWN | BATTLE_PAD_STICK_RIGHT:
        if (pressed) {
            icon = ICON_STICK_DOWN_RIGHT;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    case BATTLE_PAD_STICK_UP | BATTLE_PAD_STICK_LEFT:
        if (pressed) {
            icon = ICON_STICK_UP_LEFT;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    case BATTLE_PAD_STICK_DOWN | BATTLE_PAD_STICK_LEFT:
        if (pressed) {
            icon = ICON_STICK_DOWN_LEFT;
        } else {
            icon = ICON_STICK_NEUTRAL;
        }
        break;
    }
    return icon;
}

/** Draws one flat-coloured rectangle of the action-command gauge in screen space; skips it when it has no width. */
static inline void gaugeQuad(CameraEntry* cam, f32 x0, f32 y0, f32 x1, f32 y1, u8 r, u8 g, u8 b, u8 a) {
    f32 ty1, tx1, ty0, tx0;
    tx0 = x0;
    ty0 = y0;
    tx1 = x1;
    ty1 = y1;
    if (tx1 >= tx0) {
        btlDispGXInit2DRasta();
        GXLoadPosMtxImm(cam->view, GX_PNMTX0);
        btlDispGXQuads2DRasta(tx0, ty0, tx1, ty1, r, g, b, a);
    }
}

/**
 * Draws the action-command power gauge at (x, y), width pixels wide,
 * filled to ratio (0-1; below 0 draws only the frame).
 * From level 2 the background is split into coloured bands at thresholds a, b and c (percent).
 * flags 1 tints the fill with the command's display colour, flags 2 draws it red.
 * Used by the air gauge and timing gauge commands.
 */
void BattleAcDrawGauge(s32 x, s32 y, f32 ratio, s32 width, s32 level, s32 a, s32 b, s32 c, s32 flags) {
    BattleWork* wp = _battleWorkPointer;
    s32 max = 100;
    u8* mod = (u8*)&wp->actionCommands.disp.color;
    Vec pos;
    GXColor color = {0x00, 0xE4, 0x86, 0xFF};
    CameraEntry* cam;
    f32 r;

    cam = camGetPtr(CAMERA_2D);
    pos = (Vec){0.0f, 0.0f, 0.0f};
    pos.x = x - 200;
    pos.y = y + 25;
    iconDispGx(&pos, ICON_FLAG_2D, ICON_AC_GAUGE_FRAME, 1.0f);

    if (level >= 2) {
        gaugeQuad(cam, x - 289, y + 50, x - 289 + width * a / max, y + 32, 0x21, 0x21, 0x75, 0xFF);
        if (b - a > 0) {
            gaugeQuad(cam, x - 289 + width * a / max, y + 50, x - 289 + width * b / max, y + 32, 0x1D, 0x23, 0xA3, 0xFF);
        }
        if (c - b > 0) {
            gaugeQuad(cam, x - 289 + width * b / max, y + 50, x - 289 + width * c / max, y + 32, 0x46, 0x0C, 0xB4, 0xFF);
        }
        if (100 - c > 0) {
            gaugeQuad(cam, x - 289 + width * c / max, y + 50, x - 289 + width * 100 / max, y + 32, 0x73, 0x0D, 0x13, 0xFF);
        }
    }

    if (level >= 2) {
        btlDispGXInit2DRasta();
        GXLoadPosMtxImm(cam->view, GX_PNMTX0);
        btlDispGXQuads2DRasta(x - 289, y + 50, x - 289, y + 32, 0x2D, 0x38, 0xD2, 0xFF);
        if (a > 0) {
            f32 tx0, ty0, tx1, ty1;
            tx1 = x - 289 + width * a / max;
            ty1 = y + 32;
            ty0 = y + 50;
            tx0 = tx1 - 2.0f;
            if (tx1 >= tx0) {
                btlDispGXInit2DRasta();
                GXLoadPosMtxImm(cam->view, GX_PNMTX0);
                btlDispGXQuads2DRasta(tx0, ty0, tx1, ty1, 0x54, 0x28, 0xD1, 0xFF);
            }
        }
        if (b - a > 0) {
            f32 tx0, ty0, tx1, ty1;
            tx1 = x - 289 + width * b / max;
            ty1 = y + 32;
            ty0 = y + 50;
            tx0 = tx1 - 2.0f;
            if (tx1 >= tx0) {
                btlDispGXInit2DRasta();
                GXLoadPosMtxImm(cam->view, GX_PNMTX0);
                btlDispGXQuads2DRasta(tx0, ty0, tx1, ty1, 0x7D, 0x2C, 0xB5, 0xFF);
            }
        }
        if (c - b > 0) {
            f32 tx0, ty0, tx1, ty1;
            tx1 = x - 289 + width * c / max;
            ty1 = y + 32;
            ty0 = y + 50;
            tx0 = tx1 - 2.0f;
            if (tx1 >= tx0) {
                btlDispGXInit2DRasta();
                GXLoadPosMtxImm(cam->view, GX_PNMTX0);
                btlDispGXQuads2DRasta(tx0, ty0, tx1, ty1, 0xA1, 0x1B, 0x55, 0xFF);
            }
        }
    }

    if (ratio < 0.0f) {
        return;
    }

    if (flags & 3) {
        if (flags & 1) {
            color.r = color.r * mod[0] / 255;
            color.g = color.g * mod[1] / 255;
            color.b = color.b * mod[2] / 255;
            color.a = color.a * mod[3] / 255;
        } else {
            color = (GXColor){0xFF, 0x00, 0x00, 0xFF};
        }
        if (ratio > 0.0f) {
            f32 tx0 = x - 289, ty0 = y + 50, tx1 = (f32)(x - 289) + (f32)width * ratio, ty1 = y + 32;
            if (tx1 >= tx0) {
                btlDispGXInit2DRasta();
                GXLoadPosMtxImm(cam->view, GX_PNMTX0);
                btlDispGXQuads2DRasta(tx0, ty0, tx1, ty1, color.r, color.g, color.b, color.a);
            }
        }
        return;
    }

    if (ratio <= 0.0f) {
        return;
    }
    ratio = 100.0f * ratio;

    r = ratio;
    if (ratio > a) {
        r = a;
    }
    if (r > 0.0f) {
        gaugeQuad(cam, x - 289, y + 50, (f32)(x - 289) + width * r / 100.0f, y + 32, 0x00, 0xE4, 0x86, 0xFF);
    }

    r = ratio;
    if (ratio > b) {
        r = b;
    }
    if (r - a > 0.0f) {
        gaugeQuad(cam, x - 289 + width * a / max, y + 50, (f32)(x - 289) + width * r / 100.0f, y + 32, 0x2E, 0xB4, 0xF2, 0xFF);
    }

    r = ratio;
    if (ratio > c) {
        r = c;
    }
    if (r - b > 0.0f) {
        gaugeQuad(cam, x - 289 + width * b / max, y + 50, (f32)(x - 289) + width * r / 100.0f, y + 32, 0x75, 0x70, 0xFF, 0xFF);
    }

    if (ratio > 100.0f) {
        ratio = 100.0f;
    }
    if (ratio - c > 0.0f) {
        gaugeQuad(cam, x - 289 + width * c / max, y + 50, (f32)(x - 289) + width * ratio / 100.0f, y + 32, 0xF3, 0x04, 0xBC, 0xFF);
    }
}

/** Starts the rising "power gauge" sound that accompanies a filling action-command gauge. */
void BattleAcGaugeSeInit(void) {
    BattleWork* wp = _battleWorkPointer;

    wp->actionCommands.seId = psndSFXOn("SFX_AC_POWER_GAUGE1");
    wp->actionCommands.seValue = 0.0f;
}

/** Sets the gauge sound's pitch from value (0-100, clamped). */
void BattleAcGaugeSeUpdate(f32 value) {
    BattleWork* wp = _battleWorkPointer;

    if (value < 0.0f) {
        value = 0.0f;
    }
    if (value > 100.0f) {
        value = 100.0f;
    }
    wp->actionCommands.seValue = value;
    if (wp->actionCommands.seId != -1) {
        psndSFX_pit(wp->actionCommands.seId, 0.01f * (4092.0f * wp->actionCommands.seValue));
    }
}

/** Stops the gauge sound, if playing. */
void BattleAcGaugeSeDelete(void) {
    BattleWork* wp = _battleWorkPointer;

    if (wp->actionCommands.seId != -1) {
        psndSFXOff(wp->actionCommands.seId);
        wp->actionCommands.seId = -1;
    }
}
