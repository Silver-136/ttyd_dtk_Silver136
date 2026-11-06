// Battle audience control for every audience member (the "Kinopio" routine is shared by all kinds): jumping, entering,
// singing and status behaviors.

#include "version.h"
#include "battle/battle_audience.h"
#include "battle/battle.h"
#include "pmario_sound.h"
#include "system.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "battle/battle_unit.h"
#include "driver/camdrv.h"
#include "eff/eff_sleep.h"
#include "eff/eff_small_star.h"
#include <string.h>
#include "battle/battle_audience_kinopio.h"
#include "battle/battle_database_common.h"
#include "battle/battle_status_effect.h"
#include "eff/eff_magic1_n64.h"
#include "eff/n64/eff_onpu_n64.h"
#include "eff/eff_sandars.h"
#include "battle/battle_camera.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "libc_decls.h"
#include "battle/battle_break_slot.h"


static void BattleAudienceCtrlProcessKinopioWait(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioWaitHurimuki(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioWaitJump(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioJoyRaiseArm(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioJoyJump(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioJoyJump2(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioItemOn(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioItemThrow(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioBooing(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioDamage(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioEnter(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioEscape(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioSleep(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioTransEvt(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioIntrude(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioSing(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioShell(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioIgnite(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioFire(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioEat(s32 index, s32 front);

#if VERSION <= VERSION_DEMO_JP
static void BattleAudienceCtrlProcessKinopioNoGravity(s32 index, s32 front);
static void BattleAudienceCtrlProcessKinopioAudBall(s32 index, s32 front);
#endif

static void BattleAudienceCtrlProcessKinopioSingSub(s32 index);
static void BattleAudienceCtrlProcessKinopioFireSub(s32 index);

/**
 * Per-frame update of one audience member, whatever its kind (the "Kinopio" Toad routine is shared by all):
 * runs the handler for its status (0 idle, 1-3 idle fidgets, 4-6 cheering, 7-8 item throw, 9 booing, 10 hit,
 * 11 enter, 12 leave, 13 asleep, 14-20 the kind-specific phase events; 21-22 empty, early demos only),
 * then integrates velocity and hop height and works out the draw position.
 * Boos bob up and down; Piranha Plants never hop.
 */
void BattleAudienceCtrlProcessKinopio(s32 index) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BOOL front = BattleAudience_GetFront(index);
    switch (m->status) {
    case AUDIENCE_STATUS_WAIT:
        BattleAudienceCtrlProcessKinopioWait(index, front);
        break;
    case AUDIENCE_STATUS_WAIT_HURIMUKI:
        BattleAudienceCtrlProcessKinopioWaitHurimuki(index, front);
        break;
    case AUDIENCE_STATUS_WAIT_KUBI_KATAMUKE:
        BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(index, front);
        break;
    case AUDIENCE_STATUS_WAIT_JUMP:
        BattleAudienceCtrlProcessKinopioWaitJump(index, front);
        break;
    case AUDIENCE_STATUS_JOY_RAISE_ARM:
        BattleAudienceCtrlProcessKinopioJoyRaiseArm(index, front);
        break;
    case AUDIENCE_STATUS_JOY_JUMP:
        BattleAudienceCtrlProcessKinopioJoyJump(index, front);
        break;
    case AUDIENCE_STATUS_JOY_JUMP2:
        BattleAudienceCtrlProcessKinopioJoyJump2(index, front);
        break;
    case AUDIENCE_STATUS_ITEM_ON:
        BattleAudienceCtrlProcessKinopioItemOn(index, front);
        break;
    case AUDIENCE_STATUS_ITEM_THROW:
        BattleAudienceCtrlProcessKinopioItemThrow(index, front);
        break;
    case AUDIENCE_STATUS_BOOING:
        BattleAudienceCtrlProcessKinopioBooing(index, front);
        break;
    case AUDIENCE_STATUS_DAMAGE:
        BattleAudienceCtrlProcessKinopioDamage(index, front);
        break;
    case AUDIENCE_STATUS_ENTER:
        BattleAudienceCtrlProcessKinopioEnter(index, front);
        break;
    case AUDIENCE_STATUS_ESCAPE:
        BattleAudienceCtrlProcessKinopioEscape(index, front);
        break;
    case AUDIENCE_STATUS_SLEEP:
        BattleAudienceCtrlProcessKinopioSleep(index, front);
        break;
    case AUDIENCE_STATUS_TRANS_EVT:
        BattleAudienceCtrlProcessKinopioTransEvt(index, front);
        break;
    case AUDIENCE_STATUS_INTRUDE:
        BattleAudienceCtrlProcessKinopioIntrude(index, front);
        break;
    case AUDIENCE_STATUS_SING:
        BattleAudienceCtrlProcessKinopioSing(index, front);
        break;
    case AUDIENCE_STATUS_SHELL:
        BattleAudienceCtrlProcessKinopioShell(index, front);
        break;
    case AUDIENCE_STATUS_IGNITE:
        BattleAudienceCtrlProcessKinopioIgnite(index, front);
        break;
    case AUDIENCE_STATUS_FIRE:
        BattleAudienceCtrlProcessKinopioFire(index, front);
        break;
    case AUDIENCE_STATUS_EAT:
        BattleAudienceCtrlProcessKinopioEat(index, front);
        break;
#if VERSION <= VERSION_DEMO_JP
        case 21:
            BattleAudienceCtrlProcessKinopioNoGravity(index, front);
            break;
        case 22:
            BattleAudienceCtrlProcessKinopioAudBall(index, front);
            break;
#endif
    }
    m->position.x += m->velocity.x;
    m->velocity.x += m->accel.x;
    m->position.y += m->velocity.y;
    m->velocity.y += m->accel.y;
    m->position.z += m->velocity.z;
    m->velocity.z += m->accel.z;
    if (m->velocity.x > 0.0f) {
        m->angle = 90.0f;
    } else if (m->velocity.x < 0.0f) {
        m->angle = 270.0f;
    }
    if (m->hopY < 0.0f) {
        m->hopY = 0.0f;
        m->hopSpeed = 0.0f;
        m->hopAccel = 0.0f;
    }
    if (m->type == AUDIENCE_PIRANHA_PLANT) {
        m->hopSpeed = 0.0f;
        m->hopY = 0.0f;
        m->hopAccel = 0.0f;
    } else {
        m->hopY += m->hopSpeed;
        m->hopSpeed += m->hopAccel;
    }
    if (m->type == AUDIENCE_BOO) {
        m->bobAngle += 0.05f;
        if (m->bobAngle >= 6.283184f) {
            m->bobAngle -= 6.283184f;
        }
        m->bobOffset = 5.0f * (f32)sin(m->bobAngle);
        m->drawPosX = m->position.x + m->hopX;
        m->drawPosY = m->bobOffset + (m->position.y + m->hopY);
        m->drawPosZ = m->position.z + m->hopZ;
    } else {
        m->drawPosX = m->position.x + m->hopX;
        m->drawPosY = m->position.y + m->hopY;
        m->drawPosZ = m->position.z + m->hopZ;
    }
}

// propagation optimisation off for BattleAudienceCtrlProcessKinopioWait: the original register allocation and scheduling need it
#pragma opt_propagation off
// lifetimes optimisation off for BattleAudienceCtrlProcessKinopioWait: the original register allocation and scheduling need it
#pragma opt_lifetimes off
/**
 * Status 0, idle: faces the stage (or away, for back-facing seats) and now and
 * then starts a fidget (turn around, tilt the head, hop) or a cheer,
 * more often the fuller the crowd is relative to its expected size. A member hit by Mario switches to status 10.
 */
static void BattleAudienceCtrlProcessKinopioWait(s32 index, s32 front) {
    BattleAudience* base;
    BattleAudienceMember* m;
    f32 a;
    f32 b;
    f32 periodScale;
    f32 stepScale;
    f32 crowdRatio;
    s32 threshold;
    s32 rnd;
    s32 n;
    base = BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    pouchGetPtr();
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, FALSE);
    }
    crowdRatio = (base->targetAudienceCount + base->addedAudienceCount) / base->audienceDeltaMultiplier;
    if (crowdRatio >= 0.75f) {
        stepScale = 1.0f;
        periodScale = stepScale;
    } else if (crowdRatio >= 0.5f) {
        stepScale = 0.5f;
        periodScale = 2.0f;
    } else if (crowdRatio >= 0.25f) {
        stepScale = 0.25f;
        periodScale = 4.0f;
    } else {
        // BUG: periodScale is not set on this path, so the timer limits below compare against an uninitialized
        // value (the size steps are 0 because stepScale is 0, but when the timer resets depends on it)
        stepScale = 0.0f;
    }
    if (crowdRatio >= 1.75f) {
        a = 10.0f;
        b = 4.0f;
    } else if (crowdRatio >= 1.5f) {
        a = 5.0f;
        b = 2.0f;
    } else if (crowdRatio >= 1.25f) {
        a = 2.0f;
        b = 1.0f;
    } else if (crowdRatio >= 1.0f) {
        a = 1.0f;
        b = 0.0f;
    } else {
        b = a = 0.0f;
    }
    switch (m->type) {
    case AUDIENCE_DULL_BONES:
        if (m->timer > 180) {
            if (m->timer <= 188) {
                m->baseScale.y += 0.005f * stepScale;
            } else if ((f32)m->timer <= 8.0f * periodScale + 188.0f) {
                m->baseScale.y -= 0.005f * stepScale;
            } else if ((f32)m->timer <= 16.0f * periodScale + 188.0f) {
                m->baseScale.y += 0.005f * stepScale;
            } else if ((f32)m->timer <= 24.0f * periodScale + 188.0f) {
                m->baseScale.y -= 0.005f * stepScale;
            } else if ((f32)m->timer <= 32.0f * periodScale + 188.0f) {
                m->baseScale.y += 0.005f * stepScale;
            } else if ((f32)m->timer <= 40.0f * periodScale + 188.0f) {
                m->baseScale.y -= 0.005f * stepScale;
            } else {
                m->baseScale.y = 1.0f;
                m->timer = 0;
            }
        }
        m->timer++;
        break;
    case AUDIENCE_CRAZY_DAYZEE:
        if ((f32)m->timer >= 36.0f * periodScale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 17.0f * periodScale) {
            m->baseScale.y += 0.00175f * stepScale;
        } else {
            m->baseScale.y -= 0.00175f * stepScale;
        }
        m->timer++;
        break;
    case AUDIENCE_PUNI:
        if ((f32)m->timer >= 22.0f * periodScale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 10.0f * periodScale) {
            m->baseScale.y += 0.00375f * stepScale;
        } else {
            m->baseScale.y -= 0.00375f * stepScale;
        }
        m->timer++;
        break;
    case AUDIENCE_BULKY_BOBOMB:
        if ((f32)m->timer >= 64.0f * periodScale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 31.0f * periodScale) {
            m->baseScale.y += 0.00125f * stepScale;
        } else {
            m->baseScale.y -= 0.00125f * stepScale;
        }
        m->timer++;
        break;
    case AUDIENCE_BOO:
        break;
    default:
        if ((f32)m->timer >= 32.0f * periodScale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 15.0f * periodScale) {
            m->baseScale.y += 0.0025f * stepScale;
        } else {
            m->baseScale.y -= 0.0025f * stepScale;
        }
        m->timer++;
        break;
    }
    if (m->flags & AUDIENCE_MEMBER_FLAG_ATTACKED) {
        m->flags &= ~(AUDIENCE_MEMBER_FLAG_TARGETED | AUDIENCE_MEMBER_FLAG_ATTACKED);
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_DAMAGE);

        // BUG: tests the type of item slot 0, which belongs to whichever member took it, not to this member.
        if (!(BattleAudienceItemGetPtr(0)->itemType >= ITEM_AUDIENCE_CAN && BattleAudienceItemGetPtr(0)->itemType < BADGE_MIN)) {
            BattleAudienceAddTargetNum(-0.14999999f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
        }
    } else if (!(m->flags & AUDIENCE_MEMBER_FLAG_TARGETED)) {
        switch (m->itemId) {
        case -1:
            rnd = irand(360000);
            if (rnd < 10) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT_HURIMUKI);
            } else {
                threshold = 20;
                if (rnd < threshold) {
                    BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT_KUBI_KATAMUKE);
                } else {
                    threshold = (s32)(threshold + (f32)(10.0f * a));
                    if (rnd < threshold) {
                        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_JOY_RAISE_ARM);
                    } else if (BattleBreakSlot_GetBreakTurn() >= 1 || base->excited == 1) {
                        if (rnd < threshold + 300) {
                            if (m->type != AUDIENCE_PIRANHA_PLANT) {
                                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT_JUMP);
                            }
                        }
                    } else {
                        threshold += (s32)(10.0f * b);
                        if (rnd < threshold) {
                            n = BattleAudience_GetAudienceNoFromOffset(index, 0, 1);
                            if (n != -1) {
                                BattleAudienceGetPtr(n);
                                if (m->type != AUDIENCE_PIRANHA_PLANT) {
                                    if ((u8)BattleAudience_GetExist(n) == 1) {
                                        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT_JUMP);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        }
    }
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset
/** Status 1, "furimuki" (looking back): turns 180 degrees over 15 frames, holds, turns back, then idles again. */
static void BattleAudienceCtrlProcessKinopioWaitHurimuki(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, FALSE);
    }
    if (m->timer <= 15) {
        m->rotation.y = 12.0f * (f32)m->timer;
    }
    if (m->timer >= 30) {
        m->rotation.y = 12.0f * (f32)(45 - m->timer);
    }
    if (m->timer > 45) {
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
    } else {
        m->timer++;
    }
}

/** Status 2, "kubi katamuke" (head tilt): sways the sprite sideways and back over 60 frames, then idles again. */
static void BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, FALSE);
    }
    if (m->timer >= 0 && m->timer <= 10) {
        m->dispOffset.x = 1.5f * (f32)m->timer;
    }
    if (m->timer >= 20 && m->timer <= 40) {
        m->dispOffset.x -= 1.5f;
    }
    if (m->timer >= 50 && m->timer <= 60) {
        m->dispOffset.x += 1.5f;
    }
    if (m->timer > 60) {
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
    } else {
        m->timer++;
    }
}

/** Status 3, idle hop: one small hop (three higher ones for Punies), then back to idle. */
static void BattleAudienceCtrlProcessKinopioWaitJump(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_JUMP_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_JUMP_BACK, FALSE);
    }
    switch (m->step) {
    case 0:
        if (m->type == AUDIENCE_PUNI) {
            m->counter = 3;
        } else {
            m->counter = 1;
        }
        m->step++;
    case 1:
        if (m->type == AUDIENCE_PUNI) {
            m->hopSpeed = 5.0f;
            m->hopAccel = -1.0f;
        } else {
            m->hopSpeed = 3.4f;
            m->hopAccel = -0.2f;
        }
        m->step++;
    case 2:
        if (0.0f == m->hopAccel) {
            m->counter--;
            if (m->counter <= 0) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            } else {
                m->step = 1;
            }
        }
    }
}

/** Status 4, cheering with raised arms: bobs up and down for counterF cycles of 30 frames. */
static void BattleAudienceCtrlProcessKinopioJoyRaiseArm(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 0) {
        BattleAudience_SetAnim(index, AUD_ANIM_RAISE_BACK, FALSE);
    }
    switch (m->step) {
    case 0:
        if (irand(2) == 0) {
            m->angle = 270.0f;
        } else {
            m->angle = 90.0f;
        }
        m->step++;
    case 1:
        m->dispOffset.y = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 10.0f, m->counter, 15);
        m->counter++;
        if (m->counter > 30) {
            m->counterF -= 1.0f;
            if (m->counterF <= 0.0f) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            } else {
                m->counter = 0;
            }
        }
    }
}

/** Status 5, cheering: one excited hop of slightly random height. */
static void BattleAudienceCtrlProcessKinopioJoyJump(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_JUMP_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_JUMP_BACK, FALSE);
    }
    switch (m->step) {
    case 0:
        m->counter = 1;
        m->step++;
    case 1:
        m->hopSpeed = 3.4f + (f32)rand() / 32767.0f;
        m->hopAccel = -0.2f;
        m->step++;
    case 2:
        if (0.0f == m->hopAccel) {
            m->counter--;
            if (m->counter <= 0) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            } else {
                m->step = 1;
            }
        }
    }
}

/** Status 6, big cheer: two higher hops while spinning. */
static void BattleAudienceCtrlProcessKinopioJoyJump2(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_JUMP_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_JUMP_BACK, FALSE);
    }
    switch (m->step) {
    case 0:
        m->counter = 2;
        m->step++;
    case 1:
        m->hopSpeed = 4.0f + (f32)rand() / 32767.0f;
        m->hopAccel = -0.2f;
        m->step++;
    case 2:
        m->rotation.y += 20.0f;
        if (0.0f == m->hopAccel) {
            m->counter--;
            if (m->counter <= 0) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            } else {
                m->step = 1;
            }
        }
    }
}

/**
 * Status 7, holding an item to throw: waits during command input until the item's timer runs out,
 * then throws (status 8).
 * If Mario or a partner hits the member first, the item is dropped, the member is knocked back (status 10) and,
 * unless item slot 0 holds junk (ITEM_AUDIENCE_CAN up to the badges), the crowd's target size shrinks a little.
 */
static void BattleAudienceCtrlProcessKinopioItemOn(s32 index, s32 front) {
    BattleWork* bw = _battleWorkPointer;
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BattleAudienceItem* item;
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_ITEM_ON_BACK, FALSE);
    }
    if (m->flags & AUDIENCE_MEMBER_FLAG_ATTACKED) {
        item = BattleAudienceItemGetPtr(m->itemId);
        m->flags &= ~(AUDIENCE_MEMBER_FLAG_TARGETED | AUDIENCE_MEMBER_FLAG_ATTACKED);
        m->itemId = -1;
        item->flags = 0;
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_DAMAGE);

        // BUG: tests the type of item slot 0, not this member's item (item), so the shrink depends on another slot.
        if (!(BattleAudienceItemGetPtr(0)->itemType >= ITEM_AUDIENCE_CAN && BattleAudienceItemGetPtr(0)->itemType < BADGE_MIN)) {
            BattleAudienceAddTargetNum(-0.14999999f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
        }
    } else if (!(m->flags & AUDIENCE_MEMBER_FLAG_TARGETED) && (bw->flags & BATTLE_FLAG_COMMAND_INPUT)) {
        item = BattleAudienceItemGetPtr(m->itemId);
        if (!(base->flags & AUDIENCE_FLAG_EVENT_CTRL)) {
            item->timer--;
            if ((item->flags & AUDIENCE_ITEM_FLAG_ACTIVE) && item->timer <= 0 && item->state == 1) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_ITEM_THROW);
                BattleAudienceSoundItemThrow(0, index);
            }
        }
    }
}

/**
 * Status 8, throwing: plays the throw animation, then idles.
 * Being hit during the throw knocks it back as in status 7.
 */
static void BattleAudienceCtrlProcessKinopioItemThrow(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BattleAudience_SetAnim(index, AUD_ANIM_ITEM_THROW_BACK, FALSE);
    if (BattleAudience_GetAnimEnd(index) == 1) {
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
    }
    if (m->flags & AUDIENCE_MEMBER_FLAG_ATTACKED) {
        m->flags &= ~(AUDIENCE_MEMBER_FLAG_TARGETED | AUDIENCE_MEMBER_FLAG_ATTACKED);
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_DAMAGE);

        // BUG: tests the type of item slot 0, not this member's item (m->itemId), so the shrink depends on another slot.
        if (!(BattleAudienceItemGetPtr(0)->itemType >= ITEM_AUDIENCE_CAN && BattleAudienceItemGetPtr(0)->itemType < BADGE_MIN)) {
            BattleAudienceAddTargetNum(-0.14999999f * BattleAudienceBaseGetPtr()->audienceDeltaMultiplier, 0.0f);
        }
    } else {
        if (m->flags & AUDIENCE_MEMBER_FLAG_TARGETED) {
            return;
        }
    }
}

/** Status 9, booing: bobs with the arms-up animation for counterF cycles of 30 frames, then idles. */
static void BattleAudienceCtrlProcessKinopioBooing(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BattleAudience_SetAnim(index, AUD_ANIM_RAISE_BACK, FALSE);
    switch (m->step) {
    case 0:
        if (irand(2) == 0) {
            m->angle = 270.0f;
        } else {
            m->angle = 90.0f;
        }
        m->step++;
    case 1:
        m->dispOffset.y = intplGetValue(EASING_QUADRATIC_OUT, 0.0f, 10.0f, m->counter, 15);
        m->counter++;
        if (m->counter > 30) {
            m->counterF -= 1.0f;
            if (m->counterF <= 0.0f) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            } else {
                m->counter = 0;
            }
        }
    }
}

/**
 * Status 10, hit by Mario or a partner: knocked up and squashed,
 * then flees screaming (status 12) when allowed to leave, otherwise goes back to idle.
 */
static void BattleAudienceCtrlProcessKinopioDamage(s32 index, s32 front) {
    BattleAudienceMember* m;
    BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    switch (m->step) {
    case 0:
        BattleAudience_SetAnim(index, AUD_ANIM_DAMAGE_FRONT, FALSE);
        m->hopSpeed = 8.0f;
        m->hopAccel = -1.0f;
        m->timer = 0;
        m->angle = 270.0f;
        m->step++;
    case 1:
        if (m->timer >= 8) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if (m->timer <= 4) {
            m->baseScale.y -= 0.1f;
        } else {
            m->baseScale.y += 0.1f;
        }
        m->timer++;
        if (BattleAudience_GetAnimEnd(index) == 1) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
            if ((u8)BattleAudience_GetEscapeChangeOK(index) == 1) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_ESCAPE);
                psndSFXOn_3D("SFX_AUDIENCE_PANIC_SCREAM2", &m->position);
            } else {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            }
        }
    }
}

/**
 * Status 11, arriving: walks in to the member's seat (Piranha Plants rise from below after a random delay),
 * then idles. Also used to send phase-event members back to their seats.
 */
static void BattleAudienceCtrlProcessKinopioEnter(s32 index, s32 front) {
    BattleAudienceMember* m;
    f32 dx;
    f32 dz;
    f32 d;
    BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    camGetPtr(CAMERA_2D);
    BattleAudience_SetAnim(index, AUD_ANIM_RUN_FRONT, FALSE);
    if (m->type == AUDIENCE_PIRANHA_PLANT) {
        switch (m->step) {
        case 0:
            m->position.x = m->homePosition.x;
            m->position.y = -100.0f;
            m->position.z = m->homePosition.z;
            m->velocity.x = 0.0f;
            m->velocity.y = 0.0f;
            m->velocity.z = 0.0f;
            m->counter += irand(60) + 1;
            m->step++;
            break;
        case 1:
            m->counter--;
            if (m->counter <= 0) {
                m->velocity.x = 0.0f;
                m->velocity.y = 1.0f;
                m->velocity.z = 0.0f;
                m->step++;
            }
            break;
        case 2:
            if (m->position.y >= m->homePosition.y) {
                m->position.y = m->homePosition.y;
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
                m->step++;
            }
            break;
        }
    } else {
        switch (m->step) {
        case 0:
            m->position.x = (f32)(irand(400) - 200);
            m->position.y = m->homePosition.y;
            m->position.z = 1000.0f;
            m->velocity.x = 0.0f;
            m->velocity.y = 0.0f;
            m->velocity.z = 0.0f;
            m->counter += irand(60) + 1;
            m->step++;
            break;
        case 1:
            m->counter--;
            if (m->counter <= 0) {
                dx = m->homePosition.x - m->position.x;
                dz = m->homePosition.z - m->position.z;
                d = (f32)sqrt(dx * dx + dz * dz);
                m->velocity.x = 10.0f * (dx / d);
                m->velocity.y = 0.0f;
                m->velocity.z = 10.0f * (dz / d);
                m->counterF = 10.0f;
                m->workF1 = 10.0f;
                m->workF2 = d;
                m->step++;
            }
            break;
        case 2:
            m->counterF += m->workF1;
            if (m->counterF >= m->workF2) {
                m->position.x = m->homePosition.x;
                m->position.y = m->homePosition.y;
                m->position.z = m->homePosition.z;
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            }
            break;
        }
    }
}

/** Status 12, leaving: walks out of the stands (Piranha Plants sink into the floor) and removes the member. */
static void BattleAudienceCtrlProcessKinopioEscape(s32 index, s32 front) {
    BattleAudienceMember* m;
    BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    BattleAudience_SetAnim(index, AUD_ANIM_RUN_FRONT, FALSE);
    if (m->type == AUDIENCE_PIRANHA_PLANT) {
        switch (m->step) {
        case 0:
            m->counter--;
            if (m->counter <= 0) {
                m->velocity.x = 0.0f;
                m->velocity.y = -1.0f;
                m->velocity.z = 0.0f;
                m->step++;
            }
            break;
        case 1:
            if (m->position.y <= -100.0f) {
                BattleAudience_Delete(index);
                m->step++;
            }
            break;
        }
    } else {
        switch (m->step) {
        case 0:
            m->counter--;
            if (m->counter <= 0) {
                m->velocity.x = 2.0f * (f32)rand() / 32767.0f - 1.0f - m->position.x / 120.0f;
                m->velocity.y = 0.0f;
                m->velocity.z = 8.0f;
                m->hopSpeed = 6.0f;
                m->hopAccel = -0.5f;
                m->step++;
                psndSFXOn_3D("SFX_AUDIENCE_RUN3", &m->position);
            }
            break;
        case 1:
            if (0.0f == m->hopAccel) {
                m->step++;
            }
            break;
        case 2:
            if (m->position.z >= 600.0f) {
                BattleAudience_Delete(index);
            }
            break;
        }
    }
}

/** Status 13, asleep after a Crazee Dayzee's song: breathes slowly with a pulsing blue tint until woken. */
static void BattleAudienceCtrlProcessKinopioSleep(s32 index, s32 front) {
    BattleAudience* base;
    BattleAudienceMember* m;
    base = BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    if (m->step <= 1) {
        if ((u8)front == 1) {
            BattleAudience_SetAnim(index, AUD_ANIM_SLEEP_FRONT, FALSE);
        } else {
            BattleAudience_SetAnim(index, AUD_ANIM_SLEEP_BACK, FALSE);
        }
        if (m->timer >= 320) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if (m->timer <= 160) {
            m->baseScale.y -= 0.0025f;
        } else {
            m->baseScale.y += 0.0025f;
        }
        m->timer++;
        m->colorB += m->colorStep;
        if (m->colorB >= 200) {
            m->colorStep = -4;
        } else if (m->colorB <= 5) {
            m->colorStep = 4;
        }
    }
    switch (m->step) {
    case 0:
        BattleAudienceSoundSleep(index);
        m->step++;
    case 1:
        if (irand(36000) == 0) {
            effSleepEntry(0, 180, m->position.x, 25.0f + m->position.y, m->position.z, 1.0f, 45.0f);
            BattleAudienceSoundZZZ(index);
        }
        break;
    case 2:
        m->hopSpeed = 6.0f + (f32)rand() / 32767.0f;
        m->hopAccel = -0.5f;
        if ((u8)front == 1) {
            BattleAudience_SetAnim(index, AUD_ANIM_WAIT_FRONT, FALSE);
        } else {
            BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, FALSE);
        }
        m->baseScale.y = 1.0f;
        m->colorB = 0;
        m->step++;
    case 3:
        if (0.0f == m->hopAccel) {
            BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
        }
        break;
    }
}

/** Weapon a Boo in the audience uses to turn a unit invisible for two turns (always hits, can't be guarded). */
static BattleWeapon weapon_teresa_trans = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, // holdFastTime
    100, // invisibleChance
    2, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Status 14, Boo phase event: the Boo flies out of the stands to a random unit and,
 * unless it is already invisible or shadow-guarded, turns it invisible (weapon_teresa_trans),
 * then returns to its seat.
 */
static void BattleAudienceCtrlProcessKinopioTransEvt(s32 index, s32 front) {
    BattleWork* bw = _battleWorkPointer;
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    static f32 x;
    static f32 y;
    static f32 z;
    BattleUnit* unit;
    BattleUnitPart* parts;
    s8 st1;
    s8 st0;
    switch (m->step) {
    case 0:
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, FALSE);
        m->targetUnit = BattleAudienceDetectTargetAll();
        m->counter = 0;
        psndSFXOn_3D("SFX_AUDIENCE_TELESA_MOVE1", &m->position);
        m->step = 5;
    case 5:
        unit = m->targetUnit;
        parts = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(unit), BtlUnit_GetBodyPartsId(unit));
        m->counter++;
        m->position.x = intplGetValue(EASING_CUBIC_OUT, m->homePosition.x, 0.0f, m->counter, 60);
        m->position.y = intplGetValue(EASING_CUBIC_OUT, m->homePosition.y, 80.0f, m->counter, 60);
        m->position.z = intplGetValue(EASING_CUBIC_OUT, m->homePosition.z, 100.0f, m->counter, 60);
        if (m->counter >= 60) {
            BattleAudience_SetAnim(index, AUD_ANIM_RUN_FRONT, FALSE);
            BtlUnit_GetHitPos(unit, parts, &x, &y, &z);
            if (m->position.x >= x) {
                m->angle = 270.0f;
            } else {
                m->angle = 90.0f;
            }
            m->step = 10;
            m->counter = 0;
            unit = m->targetUnit;
            parts = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(unit), BtlUnit_GetBodyPartsId(unit));
            BtlUnit_GetHitPos(unit, parts, &x, &y, &z);
            psndSFXOn_3D("SFX_AUDIENCE_TELESA_MOVE1", &m->position);
        }
        break;
    case 10:
        m->counter++;
        m->position.x = intplGetValue(EASING_LINEAR, 0.0f, x, m->counter, 30);
        m->position.y = intplGetValue(EASING_QUADRATIC_OUT, 80.0f, y - 15.0f, m->counter, 30);
        m->position.z = intplGetValue(EASING_LINEAR, 100.0f, z, m->counter, 30);
        if (m->counter == 30) {
            unit = m->targetUnit;
            parts = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(unit), BtlUnit_GetBodyPartsId(unit));
            BtlUnit_GetStatus(unit, STATUS_INVISIBLE, &st1, &st0);
            if (st1 < 1 && !BtlUnit_CheckShadowGuard(unit)) {
                BattleCheckDamage(BattleGetSystemPtr(bw), unit, parts,
                                  &weapon_teresa_trans, DAMAGE_RESULT_FINAL_HIT);
                BtlUnit_snd_se(unit, "SFX_AUDIENCE_TELESA_INVISIBLE1", EVT_NULLPTR, 0);
            }
        }
        if (m->counter >= 120) {
            BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_ENTER);
        }
        break;
    }
}

s32 intrudeSound;

static const Vec v0 = {0.0f, 0.0f, 0.0f};
/**
 * Status 15, Shy Guy phase event: the Shy Guy runs onto the stage
 * and either makes a stage prop fall (BattleStage_ObjectFallCheck) or wiggles three times
 * and sets off the stage nozzle (BattleStage_NozzleWorkCheck) while running off the side;
 * then it returns to its seat. At rank 0 it always makes a prop fall.
 */
static void BattleAudienceCtrlProcessKinopioIntrude(s32 index, s32 front) {
    BattleWork* bw = _battleWorkPointer;
    BattleAudienceMember* m;
    PouchData* pouch;
    f32 dx;
    f32 dz;
    f32 d;
    f32 tx;
    f32 tz;
    BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    pouch = pouchGetPtr();
    BattleAudience_SetAnim(index, AUD_ANIM_RUN_FRONT, FALSE);
    switch (m->step) {
    case 0:
        if (irand(2) == 0) {
            m->counterF = 1.0f;
        } else {
            m->counterF = -1.0f;
        }
        if (pouch->rank == 0) {
            m->step = 3;
        } else if (irand(2) == 0) {
            m->step = 3;
        } else {
            m->step = 50;
        }
        break;
    case 3:
        m->step = 5;
        tx = 25.0f * m->counterF;
        tz = 110.0f;
        dx = tx - m->position.x;
        dz = tz - m->position.z;
        d = (f32)sqrt(dx * dx + dz * dz);
        m->velocity.x = 5.0f * (dx / d);
        m->velocity.y = 0.0f;
        m->accel.y = 0.0f;
        m->velocity.z = 5.0f * (dz / d);
        intrudeSound = psndSFXOn("SFX_AUDIENCE_HEIHO_MOVE1");
    case 5:
        if (m->position.z <= 110.0f) {
            m->step = 10;
            m->position.z = 110.0f;
            m->velocity.x = 0.0f;
            m->velocity.y = 10.0f;
            m->accel.y = -0.7f;
            m->velocity.z = -1.5f;
            psndSFXOff(intrudeSound);
        }
        break;
    case 10:
        if (m->velocity.y <= 0.0f && m->position.y <= 0.0f) {
            m->step = 15;
            m->position.y = 0.0f;
            m->velocity.y = 0.0f;
            m->accel.y = 0.0f;
            tx = 300.0f * m->counterF;
            tz = 0.0f;
            dx = tx - m->position.x;
            dz = tz - m->position.z;
            d = (f32)sqrt(dx * dx + dz * dz);
            m->velocity.x = 5.0f * (dx / d);
            m->velocity.y = 0.0f;
            m->velocity.z = 5.0f * (dz / d);
            btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE);
            btl_camera_set_moveto(BTL_CAM_PRI_NORMAL, 698.0f * m->counterF, 100.0f, 0.0f, 0.0f, 60.0f, 0.0f, 30, 4);
            intrudeSound = psndSFXOn("SFX_AUDIENCE_HEIHO_MOVE1");
        }
        break;
    case 15:
        m->rotation.y = camGetPtr(CAMERA_3D)->viewYaw;
        if (__fabs(m->position.x) >= 450.0) {
            m->step = 16;
            m->counter = 35;
            m->velocity = (Vec){0.0f, 0.0f, 0.0f};
        }
        break;
    case 16:
        m->counter--;
        if (m->counter <= 0) {
            m->step = 18;
            m->counter = 25;
            btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT);
            btl_camera_set_moveSpeedLv(BTL_CAM_PRI_NORMAL, 2);
            psndSFXOff(intrudeSound);
        }
        break;
    case 18:
        m->counter--;
        if (m->counter <= 0) {
            m->step = 19;
            m->counter = 60;
            btl_camera_set_moveSpeedLv(BTL_CAM_PRI_NORMAL, 1);
            effSmallStarEntry(0, 4, 185.0f * m->counterF, 20.0f, 40.0f, 0.0f, -1.0f, 0.0f);
        }
        break;
    case 19:
        m->counter--;
        if (m->counter <= 0) {
            m->step = 20;
            btl_camera_set_moveSpeedLv(BTL_CAM_PRI_NORMAL, 1);
        }
    case 20:
        m->statusTimer = BattleStage_ObjectFallCheck();
        m->step = 25;
        break;
    case 25:
        if (!evtCheckID(m->statusTimer)) {
            m->step = 30;
            m->rotation.y = 0.0f;
        }
        break;
    case 30:
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_ENTER);
        break;
    case 50:
        m->step = 55;
        tx = 25.0f * m->counterF;
        tz = 110.0f;
        dx = tx - m->position.x;
        dz = tz - m->position.z;
        d = (f32)sqrt(dx * dx + dz * dz);
        m->velocity.x = 5.0f * (dx / d);
        m->velocity.y = 0.0f;
        m->accel.y = 0.0f;
        m->velocity.z = 5.0f * (dz / d);
        intrudeSound = psndSFXOn("SFX_AUDIENCE_HEIHO_MOVE1");
    case 55:
        if (m->position.z <= 110.0f) {
            m->step = 60;
            m->position.z = 110.0f;
            m->velocity.x = 0.0f;
            m->velocity.y = 10.0f;
            m->accel.y = -0.7f;
            m->velocity.z = -1.5f;
            psndSFXOff(intrudeSound);
        }
        break;
    case 60:
        if (m->velocity.y <= 0.0f && m->position.y <= 0.0f) {
            m->step = 65;
            m->counter = 0;
            m->position.y = 0.0f;
            m->velocity.x = 0.0f;
            m->velocity.y = 0.0f;
            m->accel.y = 0.0f;
            m->velocity.z = 0.0f;
            intrudeSound = psndSFXOn("SFX_AUDIENCE_HEIHO_MOVE1");
        }
        break;
    case 65:
    case 66:
    case 67:
        m->counter++;
        if (1.0f == m->counterF) {
            m->angle = 270.0f;
        }
        if (-1.0f == m->counterF) {
            m->angle = 90.0f;
        }
        if (m->counter <= 5) {
            m->position.x -= 5.0f * m->counterF;
        } else if (m->counter <= 10) {
            m->position.x += 5.0f * m->counterF;
        }
        if (m->counter == 10) {
            if (m->step == 67) {
                m->step = 70;
            } else {
                m->step++;
            }
            m->counter = 0;
        }
        break;
    case 70:
        BattleStage_NozzleWorkCheck(1);
        m->statusTimer = bw->stageObjectWork.nozzleWorkEvtId;
        m->step = 75;
        tx = 300.0f * m->counterF;
        tz = 0.0f;
        dx = tx - m->position.x;
        dz = tz - m->position.z;
        d = (f32)sqrt(dx * dx + dz * dz);
        m->velocity.x = 5.0f * (dx / d);
        m->velocity.y = 0.0f;
        m->velocity.z = 5.0f * (dz / d);
        break;
    case 75:
        if (__fabs(m->position.x) >= 300.0) {
            m->step = 80;
            m->velocity = (Vec){0.0f, 0.0f, 0.0f};
            psndSFXOff(intrudeSound);
        }
        break;
    case 80:
        if (!evtCheckID(m->statusTimer)) {
            m->step = 85;
        }
        break;
    case 85:
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_ENTER);
        break;
    }
}

/**
 * Status 16, Crazee Dayzee phase event: spins, sings (with floating notes) for 120 frames,
 * and puts the rest of the crowd to sleep.
 */
static void BattleAudienceCtrlProcessKinopioSing(s32 index, s32 front) {
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    f32 y;
    f32 z;
    s32 i;
    switch (m->step) {
    case 0:
        m->counter = 30;
        m->step++;
    case 1:
        m->counter--;
        m->rotation.y += 6.0f;
        if (m->counter == 15) {
            BattleAudience_SetAnim(index, AUD_ANIM_SING_FRONT, FALSE);
            BattleAudienceSoundSing(index);
        }
        if (m->counter <= 0) {
            m->counter = 120;
            m->step++;
        }
        break;
    case 2:
        m->counter--;
        if (gp->retraceCount % 10 == 0) {
            z = m->position.z + (f32)irand(10) - 5.0f;
            y = m->position.y + (f32)irand(10);
            effOnpuN64Entry(0, m->position.x + (f32)irand(200) - 100.0f, y, z);
        }
        if (m->counter <= 0) {
            for (i = 0; i < base->maxAudience; i++) {
                if (index != i) {
                    BattleAudienceCtrlProcessKinopioSingSub(i);
                }
            }
            m->counter = 30;
            m->step++;
        }
        break;
    case 3:
        m->counter--;
        m->rotation.y += 6.0f;
        if (m->counter <= 15) {
            BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, FALSE);
        }
        if (m->counter <= 0) {
            BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
        }
        break;
    }
}

/**
 * Puts one member to sleep (status 13; statusDelay 3 wakes it after three enemy phases) for the Dayzee's song,
 * redirecting a wide member's tail seat to its owner and sparing Koopas hiding in their shells.
 */
static void BattleAudienceCtrlProcessKinopioSingSub(s32 index) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if (m->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) {
        BattleAudienceCtrlProcessKinopioSingSub(index - 1);
    } else if (m->status != AUDIENCE_STATUS_SHELL) {
        BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_SLEEP);
        m->statusDelay = 3;
    }
}

/**
 * Status 17, Koopa phase event: after a random delay the Koopa hops into its shell (with the shell sound),
 * stays inside until released, then hops out and idles.
 */
static void BattleAudienceCtrlProcessKinopioShell(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if (m->step >= 2 && m->step <= 5) {
        BattleAudience_SetAnim(index, AUD_ANIM_SHELL_BACK, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_WAIT_BACK, FALSE);
    }
    switch (m->step) {
    case 0:
        m->counter = irand(30);
        m->step++;
    case 1:
        m->counter--;
        if (m->counter <= 0) {
            m->step++;
        }
        break;
    case 2:
        m->hopSpeed = 5.0f;
        m->hopAccel = -0.3f;
        BattleAudienceSoundShell(index);
        m->step++;
        break;
    case 3:
        break;
    case 4:
        m->counter = irand(30);
        m->step++;
    case 5:
        m->counter--;
        if (m->counter <= 0) {
            m->step++;
        }
        break;
    case 6:
        m->hopSpeed = 5.0f;
        m->hopAccel = -0.3f;
        m->counter = 10;
        BattleAudienceSoundShell(index);
        m->step++;
        break;
    case 7:
        m->counter--;
        if (m->counter <= 0) {
            BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
        }
        m->statusTimer = 0;
        break;
    }
}

/** Status 18, Bulky Bob-omb phase event: lights its fuse (sound for 180 frames) and counts down to the blast. */
static void BattleAudienceCtrlProcessKinopioIgnite(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_IGNITE_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_IGNITE_BACK, FALSE);
    }
    switch (m->step) {
    case 0:
        m->counter = 30;
        BattleAudienceSoundBombIgnite(index, 180, 120);
        m->step++;
    case 1:
        m->counter--;
        if (m->counter <= 0) {
            m->counter = 10;
            m->step++;
        }
        break;
    case 2:
        m->counter--;
        if (m->counter <= 0) {
            m->step++;
        }
        break;
    case 3:
        break;
    }
}

/**
 * Status 19, Bulky Bob-omb explosion: swells, explodes with a blast effect (front rows only)
 * and clears the members in the surrounding seats via BattleAudienceCtrlProcessKinopioFireSub.
 */
static void BattleAudienceCtrlProcessKinopioFire(s32 index, s32 front) {
    BattleAudience* base;
    BattleAudienceMember* m;
    s32 i;
    base = BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, AUD_ANIM_IGNITE_FRONT, FALSE);
    } else {
        BattleAudience_SetAnim(index, AUD_ANIM_IGNITE_BACK, FALSE);
    }
    switch (m->step) {
    case 0:
        m->counter = 10;
        m->step++;
    case 1:
        m->baseScale.x += 0.05f;
        m->baseScale.y += 0.05f;
        m->counter--;
        if (m->counter <= 0) {
            s32 offs[24][2] = {{0, 3},  {-1, 2},  {0, 2},  {1, 2},  {-2, 1},  {-1, 1},  {0, 1},  {1, 1},
                               {2, 1},  {-3, 0},  {-2, 0}, {-1, 0}, {1, 0},   {2, 0},   {3, 0},  {-2, -1},
                               {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {-1, -2}, {0, -2},  {1, -2}, {0, -3}};
            BattleAudienceSoundBombFire(index);
            m->flags &= ~AUDIENCE_MEMBER_FLAG_DISP;
            if (index < 60) {
                effSandarsEntry(4, m->position.x, 35.0f + m->position.y, m->position.z, 0.0f);
            }
            btl_camera_shake_h(BTL_CAM_PRI_NORMAL, 5.0f, 0.0f, 30, 0);
            for (i = 0; i < 24; i++) {
                s32 n = BattleAudience_GetAudienceNoFromOffset(index, offs[i][0], offs[i][1]);
                if (n != -1) {
                    BattleAudienceCtrlProcessKinopioFireSub(n);
                }
            }
            BattleAudiencePuniAllEscape();
            m->counter = 60;
            m->step++;
        }
        break;
    case 2:
        m->counter--;
        if (m->counter <= 0) {
            m->step++;
        }
        break;
    case 3:
        BattleAudience_Delete(index);
        base->targetAudienceCount -= 1.0f;
        break;
    }
}

/**
 * Applies a Bulky Bob-omb blast to one seat: another Bulky Bob-omb catches and explodes too (status 19),
 * anyone else except a shelled Koopa is removed from the crowd and the target count drops by one.
 */
static void BattleAudienceCtrlProcessKinopioFireSub(s32 index) {
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if (m->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) {
        BattleAudienceCtrlProcessKinopioFireSub(index - 1);
    } else if ((u8)BattleAudience_GetSysCtrl(index)) {
        if (m->type == AUDIENCE_BULKY_BOBOMB) {
            if (m->status != AUDIENCE_STATUS_FIRE) {
                BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_FIRE);
            } else {
                return;
            }
        } else if (m->status != AUDIENCE_STATUS_SHELL) {
            BattleAudience_Delete(index);
            base->targetAudienceCount -= 1.0f;
        }
    }
}

/**
 * Status 20, Piranha Plant phase event: picks a nearby member,
 * turns to it and eats it (removing it from the crowd), then idles.
 * Goes straight back to idle when no one is in reach.
 */
static void BattleAudienceCtrlProcessKinopioEat(s32 index, s32 front) {
    BattleAudience* base;
    BattleAudienceMember* m;
    BattleStage* stage;
    BattleAudienceMember* t;
    s32 list[60];
    f32 pz;
    s32 i;
    s32 cnt;
    s32 k;
    base = BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    stage = BattleStageGetPtr();
    switch (m->step) {
    case 0:
        k = BattleAudienceDetectPakkunEatTarget(index);
        if (k != -1) {
            m->targetMemberId = k;
            t = BattleAudienceGetPtr(m->targetMemberId);
            if (m->position.x >= t->position.x) {
                m->angle = 270.0f;
            } else {
                m->angle = 90.0f;
            }
            BattleAudience_SetAnim(index, AUD_ANIM_WAIT_FRONT, FALSE);
        } else {
            BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
            break;
        }
        m->counter = 30;
        m->step++;
    case 1:
        t = BattleAudienceGetPtr(m->targetMemberId);
        if (m->counter > 0) {
            m->counter--;
        }
        if (m->counter <= 0 && 0.0f == t->hopY) {
            BattleAudience_SetAnim(index, AUD_ANIM_EAT_FRONT, FALSE);
            t->flags |= AUDIENCE_MEMBER_FLAG_NOT_EXIST;
            m->step = 3;
        }
        break;
    case 3:
        t = BattleAudienceGetPtr(m->targetMemberId);
        if (m->position.x >= t->position.x) {
            m->dispOffset.x -= 6.0f;
        } else {
            m->dispOffset.x += 6.0f;
        }
        if (m->dispOffset.x <= -60.0f || m->dispOffset.x >= 60.0f) {
            m->step++;
        }
        break;
    case 4:
        t = BattleAudienceGetPtr(m->targetMemberId);
        t->flags = 0;
        base->targetAudienceCount -= 1.0f;
        BattleAudienceSoundPakkunEat(index);
        m->step++;
        break;
    case 5:
        if (m->dispOffset.x > 0.0f) {
            m->dispOffset.x -= 6.0f;
            if (m->dispOffset.x <= 0.0f) {
                m->step = 7;
            }
        } else {
            m->dispOffset.x += 6.0f;
            if (m->dispOffset.x >= 0.0f) {
                m->step = 7;
            }
        }
        break;
    case 7:
        if (BattleAudienceDetectPakkunEatTarget(index) != -1) {
            m->step = 99;
            m->counter = 10;
        } else {
            cnt = 0;
            for (i = 0; i <= 59; i++) {
                if (!(u8)BattleAudience_GetExist(i) && !(BattleAudienceGetPtr(i)->flags & AUDIENCE_MEMBER_FLAG_WIDE_SUB) &&
                    BattleAudienceDetectPakkunEatTarget(i) != -1) {
                    list[cnt] = i;
                    cnt++;
                }
            }
            if (cnt >= 1) {
                m->step = 8;
                m->statusTimer = list[irand(cnt)];
            } else if (i == 60) {
                m->step = 99;
                m->counter = 10;
            }
        }
        break;
    case 8:
        m->velocity.x = 0.0f;
        m->velocity.y = -1.0f;
        m->velocity.z = 0.0f;
        m->step++;
        break;
    case 9:
        if (m->position.y <= -100.0f) {
            k = m->statusTimer;
            pz = 0.1f * (f32)(k % AUDIENCE_SEAT_COLUMNS) + (f32)(k / AUDIENCE_SEAT_COLUMNS * 30 + 115);
            m->homePosition.x = m->position.x = (f32)((rand() * 10 / 32767) + (k % AUDIENCE_SEAT_COLUMNS * 25 - 235));
            m->homePosition.y = stage->audienceSizeY;
            m->homePosition.z = m->position.z = pz;
            m->step++;
        }
        break;
    case 10:
        m->velocity.x = 0.0f;
        m->velocity.y = 1.0f;
        m->velocity.z = 0.0f;
        m->step++;
        break;
    case 11:
        if (m->position.y >= stage->audienceSizeY) {
            m->position.y = m->homePosition.y = stage->audienceSizeY;
            m->velocity.y = 0.0f;
            m->step = 99;
            m->counter = 10;
            memcpy(BattleAudienceGetPtr(m->statusTimer), m, sizeof(BattleAudienceMember));
            BattleAudience_Delete(index);
        }
        break;
    case 99:
        m->counter--;
        if (m->counter <= 0) {
            BattleAudience_ChangeStatus(index, AUDIENCE_STATUS_WAIT);
        }
        break;
    }
}

#if VERSION <= VERSION_DEMO_JP
/** Status 21, early demos only: empty (Guess: a no-gravity state, from the name); JP and later drop the case. */
static void BattleAudienceCtrlProcessKinopioNoGravity(s32 index, s32 front) {
}

/** Status 22, early demos only: empty; whatever the "audience ball" event was, it was cut. */
static void BattleAudienceCtrlProcessKinopioAudBall(s32 index, s32 front) {
}
#endif
