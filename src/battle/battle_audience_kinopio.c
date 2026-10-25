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



double sqrt(double);
double fabs(double);
double __fabs(double);
s32 rand();
BattleUnit* BattleAudienceDetectTargetAll(void);
s32 BattleAudienceDetectPakkunEatTarget(s32 index);
void BattleAudiencePuniAllEscape(void);
void BattleAudienceSoundSing(s32 index);
void BattleAudienceSoundShell(s32 index);
void BattleAudienceSoundBombIgnite(s32 index, s32 duration, s32 fade);
void BattleAudienceSoundBombFire(s32 index);
void BattleAudienceSoundPakkunEat(s32 index);
void BattleAudienceSoundSleep(s32 index);
void BattleAudienceSoundZZZ(s32 index);
void btl_camera_set_moveto(s32 a, s32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h, s32 i);
void btl_camera_shake_h(s32 a, f32 b, f32 c, s32 d, s32 e);
void BattleStage_NozzleWorkCheck(s32 a);
s32 BattleBreakSlot_GetBreakTurn();
u8 BattleAudience_GetEscapeChangeOK();
BOOL BattleAudience_GetFront(s32 index);
s32 BattleAudience_GetAudienceNoFromOffset(s32 index, s32 column, s32 row);
void BattleAudienceSoundItemThrow(s32 kind, s32 index);

void BattleAudienceCtrlProcessKinopioWait(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioWaitHurimuki(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioWaitJump(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioJoyRaiseArm(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioJoyJump(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioJoyJump2(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioItemOn(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioItemThrow();
void BattleAudienceCtrlProcessKinopioBooing();
void BattleAudienceCtrlProcessKinopioDamage();
void BattleAudienceCtrlProcessKinopioEnter();
void BattleAudienceCtrlProcessKinopioEscape();
void BattleAudienceCtrlProcessKinopioSleep(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioTransEvt();
void BattleAudienceCtrlProcessKinopioIntrude();
void BattleAudienceCtrlProcessKinopioSing();
void BattleAudienceCtrlProcessKinopioShell();
void BattleAudienceCtrlProcessKinopioIgnite(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioFire(s32 index, s32 front);
void BattleAudienceCtrlProcessKinopioEat();
void BattleAudienceCtrlProcessKinopioSingSub(s32 index);
void BattleAudienceCtrlProcessKinopioFireSub(s32 index);

void BattleAudienceCtrlProcessKinopio(s32 index) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BOOL front = BattleAudience_GetFront(index);
    switch (m->status) {
    case 0:
        BattleAudienceCtrlProcessKinopioWait(index, front);
        break;
    case 1:
        BattleAudienceCtrlProcessKinopioWaitHurimuki(index, front);
        break;
    case 2:
        BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(index, front);
        break;
    case 3:
        BattleAudienceCtrlProcessKinopioWaitJump(index, front);
        break;
    case 4:
        BattleAudienceCtrlProcessKinopioJoyRaiseArm(index, front);
        break;
    case 5:
        BattleAudienceCtrlProcessKinopioJoyJump(index, front);
        break;
    case 6:
        BattleAudienceCtrlProcessKinopioJoyJump2(index, front);
        break;
    case 7:
        BattleAudienceCtrlProcessKinopioItemOn(index, front);
        break;
    case 8:
        BattleAudienceCtrlProcessKinopioItemThrow(index, front);
        break;
    case 9:
        BattleAudienceCtrlProcessKinopioBooing(index, front);
        break;
    case 10:
        BattleAudienceCtrlProcessKinopioDamage(index, front);
        break;
    case 11:
        BattleAudienceCtrlProcessKinopioEnter(index, front);
        break;
    case 12:
        BattleAudienceCtrlProcessKinopioEscape(index, front);
        break;
    case 13:
        BattleAudienceCtrlProcessKinopioSleep(index, front);
        break;
    case 14:
        BattleAudienceCtrlProcessKinopioTransEvt(index, front);
        break;
    case 15:
        BattleAudienceCtrlProcessKinopioIntrude(index, front);
        break;
    case 16:
        BattleAudienceCtrlProcessKinopioSing(index, front);
        break;
    case 17:
        BattleAudienceCtrlProcessKinopioShell(index, front);
        break;
    case 18:
        BattleAudienceCtrlProcessKinopioIgnite(index, front);
        break;
    case 19:
        BattleAudienceCtrlProcessKinopioFire(index, front);
        break;
    case 20:
        BattleAudienceCtrlProcessKinopioEat(index, front);
        break;
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
    if (m->type == 11) {
        m->hopSpeed = 0.0f;
        m->hopY = 0.0f;
        m->hopAccel = 0.0f;
    } else {
        m->hopY += m->hopSpeed;
        m->hopSpeed += m->hopAccel;
    }
    if (m->type == 2) {
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

#pragma opt_propagation off
#pragma opt_lifetimes off
void BattleAudienceCtrlProcessKinopioWait(s32 index, s32 front) {
    BattleAudience* base;
    BattleAudienceMember* m;
    f32 a;
    f32 b;
    f32 scale;
    f32 speed;
    f32 r;
    s32 th;
    s32 rnd;
    s32 n;
    base = BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    pouchGetPtr();
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 0, FALSE);
    } else {
        BattleAudience_SetAnim(index, 1, FALSE);
    }
    r = (base->mTargetAudienceCount + base->mAddedAudienceCount) / base->mAudienceDeltaMultiplier;
    if (r >= 0.75f) {
        speed = 1.0f;
        scale = speed;
    } else if (r >= 0.5f) {
        speed = 0.5f;
        scale = 2.0f;
    } else if (r >= 0.25f) {
        speed = 0.25f;
        scale = 4.0f;
    } else {
        speed = 0.0f;
    }
    if (r >= 1.75f) {
        a = 10.0f;
        b = 4.0f;
    } else if (r >= 1.5f) {
        a = 5.0f;
        b = 2.0f;
    } else if (r >= 1.25f) {
        a = 2.0f;
        b = 1.0f;
    } else if (r >= 1.0f) {
        a = 1.0f;
        b = 0.0f;
    } else {
        b = a = 0.0f;
    }
    switch (m->type) {
    case 4:
        if (m->timer > 180) {
            if (m->timer <= 188) {
                m->baseScale.y += 0.005f * speed;
            } else if ((f32)m->timer <= 8.0f * scale + 188.0f) {
                m->baseScale.y -= 0.005f * speed;
            } else if ((f32)m->timer <= 16.0f * scale + 188.0f) {
                m->baseScale.y += 0.005f * speed;
            } else if ((f32)m->timer <= 24.0f * scale + 188.0f) {
                m->baseScale.y -= 0.005f * speed;
            } else if ((f32)m->timer <= 32.0f * scale + 188.0f) {
                m->baseScale.y += 0.005f * speed;
            } else if ((f32)m->timer <= 40.0f * scale + 188.0f) {
                m->baseScale.y -= 0.005f * speed;
            } else {
                m->baseScale.y = 1.0f;
                m->timer = 0;
            }
        }
        m->timer++;
        break;
    case 6:
        if ((f32)m->timer >= 36.0f * scale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 17.0f * scale) {
            m->baseScale.y += 0.00175f * speed;
        } else {
            m->baseScale.y -= 0.00175f * speed;
        }
        m->timer++;
        break;
    case 7:
        if ((f32)m->timer >= 22.0f * scale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 10.0f * scale) {
            m->baseScale.y += 0.00375f * speed;
        } else {
            m->baseScale.y -= 0.00375f * speed;
        }
        m->timer++;
        break;
    case 9:
        if ((f32)m->timer >= 64.0f * scale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 31.0f * scale) {
            m->baseScale.y += 0.00125f * speed;
        } else {
            m->baseScale.y -= 0.00125f * speed;
        }
        m->timer++;
        break;
    case 2:
        break;
    default:
        if ((f32)m->timer >= 32.0f * scale) {
            m->baseScale.y = 1.0f;
            m->timer = 0;
        }
        if ((f32)m->timer <= 15.0f * scale) {
            m->baseScale.y += 0.0025f * speed;
        } else {
            m->baseScale.y -= 0.0025f * speed;
        }
        m->timer++;
        break;
    }
    if (m->flags & 0x40) {
        m->flags &= ~0x60;
        BattleAudience_ChangeStatus(index, 10);
        if (!(BattleAudienceItemGetPtr(0)->mItemType >= 0xEC && BattleAudienceItemGetPtr(0)->mItemType < 0xF0)) {
            BattleAudienceAddTargetNum(-0.14999999f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
        }
    } else if (!(m->flags & 0x20)) {
        switch (m->itemId) {
        case -1: {
            rnd = irand(360000);
            if (rnd < 10) {
                BattleAudience_ChangeStatus(index, 1);
            } else {
                th = 20;
                if (rnd < th) {
                    BattleAudience_ChangeStatus(index, 2);
                } else {
                    th = (s32)(th + (f32)(10.0f * a));
                    if (rnd < th) {
                        BattleAudience_ChangeStatus(index, 4);
                    } else if (BattleBreakSlot_GetBreakTurn() >= 1 || base->excited == 1) {
                        if (rnd < th + 300) {
                            if (m->type != 11) {
                                BattleAudience_ChangeStatus(index, 3);
                            }
                        }
                    } else {
                        th += (s32)(10.0f * b);
                        if (rnd < th) {
                            n = BattleAudience_GetAudienceNoFromOffset(index, 0, 1);
                            if (n != -1) {
                                BattleAudienceGetPtr(n);
                                if (m->type != 11) {
                                    if ((u8)BattleAudience_GetExist(n) == 1) {
                                        BattleAudience_ChangeStatus(index, 3);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break; }
        }
    }
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset
void BattleAudienceCtrlProcessKinopioWaitHurimuki(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 0, FALSE);
    } else {
        BattleAudience_SetAnim(index, 1, FALSE);
    }
    if (m->timer <= 15) {
        m->mRotation.y = 12.0f * (f32)m->timer;
    }
    if (m->timer >= 30) {
        m->mRotation.y = 12.0f * (f32)(45 - m->timer);
    }
    if (m->timer > 45) {
        BattleAudience_ChangeStatus(index, 0);
    } else {
        m->timer++;
    }
}

void BattleAudienceCtrlProcessKinopioWaitKubiKatamuke(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 0, FALSE);
    } else {
        BattleAudience_SetAnim(index, 1, FALSE);
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
        BattleAudience_ChangeStatus(index, 0);
    } else {
        m->timer++;
    }
}

void BattleAudienceCtrlProcessKinopioWaitJump(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 2, FALSE);
    } else {
        BattleAudience_SetAnim(index, 3, FALSE);
    }
    switch (m->step) {
    case 0:
        if (m->type == 7) {
            m->counter = 3;
        } else {
            m->counter = 1;
        }
        m->step++;
    case 1:
        if (m->type == 7) {
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
                BattleAudience_ChangeStatus(index, 0);
            } else {
                m->step = 1;
            }
        }
    }
}

void BattleAudienceCtrlProcessKinopioJoyRaiseArm(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 0) {
        BattleAudience_SetAnim(index, 4, FALSE);
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
        m->dispOffset.y = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, 10.0f, 4, m->counter, 15);
        m->counter++;
        if (m->counter > 30) {
            m->counterF -= 1.0f;
            if (m->counterF <= 0.0f) {
                BattleAudience_ChangeStatus(index, 0);
            } else {
                m->counter = 0;
            }
        }
    }
}

void BattleAudienceCtrlProcessKinopioJoyJump(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 2, FALSE);
    } else {
        BattleAudience_SetAnim(index, 3, FALSE);
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
                BattleAudience_ChangeStatus(index, 0);
            } else {
                m->step = 1;
            }
        }
    }
}

void BattleAudienceCtrlProcessKinopioJoyJump2(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 2, FALSE);
    } else {
        BattleAudience_SetAnim(index, 3, FALSE);
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
        m->mRotation.y += 20.0f;
        if (0.0f == m->hopAccel) {
            m->counter--;
            if (m->counter <= 0) {
                BattleAudience_ChangeStatus(index, 0);
            } else {
                m->step = 1;
            }
        }
    }
}

void BattleAudienceCtrlProcessKinopioItemOn(s32 index, s32 front) {
    BattleWork* bw = _battleWorkPointer;
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BattleAudienceItem* item;
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 0, FALSE);
    } else {
        BattleAudience_SetAnim(index, 5, FALSE);
    }
    if (m->flags & 0x40) {
        item = BattleAudienceItemGetPtr(m->itemId);
        m->flags &= ~0x60;
        m->itemId = -1;
        item->flags = 0;
        BattleAudience_ChangeStatus(index, 10);
        if (!(BattleAudienceItemGetPtr(0)->mItemType >= 0xEC && BattleAudienceItemGetPtr(0)->mItemType < 0xF0)) {
            BattleAudienceAddTargetNum(-0.14999999f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
        }
    } else if (!(m->flags & 0x20) && (bw->flags & 0x80)) {
        item = BattleAudienceItemGetPtr(m->itemId);
        if (!(base->flags & 0x40000)) {
            item->timer--;
            if ((item->flags & 1) && item->timer <= 0 && item->state == 1) {
                BattleAudience_ChangeStatus(index, 8);
                BattleAudienceSoundItemThrow(0, index);
            }
        }
    }
}

void BattleAudienceCtrlProcessKinopioItemThrow(s32 index) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BattleAudience_SetAnim(index, 6, FALSE);
    if (BattleAudience_GetAnimEnd(index) == 1) {
        BattleAudience_ChangeStatus(index, 0);
    }
    if (m->flags & 0x40) {
        m->flags &= ~0x60;
        BattleAudience_ChangeStatus(index, 10);
        if (!(BattleAudienceItemGetPtr(0)->mItemType >= 0xEC && BattleAudienceItemGetPtr(0)->mItemType < 0xF0)) {
            BattleAudienceAddTargetNum(-0.14999999f * BattleAudienceBaseGetPtr()->mAudienceDeltaMultiplier, 0.0f);
        }
    } else {
        if (m->flags & 0x20) {
            return;
        }
    }
}

void BattleAudienceCtrlProcessKinopioBooing(s32 index) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    BattleAudience_SetAnim(index, 4, FALSE);
    switch (m->step) {
    case 0:
        if (irand(2) == 0) {
            m->angle = 270.0f;
        } else {
            m->angle = 90.0f;
        }
        m->step++;
    case 1:
        m->dispOffset.y = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, 10.0f, 4, m->counter, 15);
        m->counter++;
        if (m->counter > 30) {
            m->counterF -= 1.0f;
            if (m->counterF <= 0.0f) {
                BattleAudience_ChangeStatus(index, 0);
            } else {
                m->counter = 0;
            }
        }
    }
}

void BattleAudienceCtrlProcessKinopioDamage(s32 index) {
    BattleAudienceMember* m;
    BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    switch (m->step) {
    case 0:
        BattleAudience_SetAnim(index, 9, FALSE);
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
            if (BattleAudience_GetEscapeChangeOK(index) == 1) {
                BattleAudience_ChangeStatus(index, 12);
                psndSFXOn_3D("SFX_AUDIENCE_PANIC_SCREAM2", &m->position);
            } else {
                BattleAudience_ChangeStatus(index, 0);
            }
        }
    }
}

void BattleAudienceCtrlProcessKinopioEnter(s32 index) {
    BattleAudienceMember* m;
    f32 dx;
    f32 dz;
    BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    camGetPtr(CAMERA_2D);
    BattleAudience_SetAnim(index, 7, FALSE);
    if (m->type == 11) {
        switch (m->step) {
        case 0:
            m->position.x = m->mHomePosition.x;
            m->position.y = -100.0f;
            m->position.z = m->mHomePosition.z;
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
            if (m->position.y >= m->mHomePosition.y) {
                m->position.y = m->mHomePosition.y;
                BattleAudience_ChangeStatus(index, 0);
                m->step++;
            }
            break;
        }
    } else {
        switch (m->step) {
        case 0:
            m->position.x = (f32)(irand(400) - 200);
            m->position.y = m->mHomePosition.y;
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
                dx = m->mHomePosition.x - m->position.x;
                dz = m->mHomePosition.z - m->position.z;

                {
                    f32 d = (f32)sqrt(dx * dx + dz * dz);
                    m->velocity.x = 10.0f * (dx / d);
                    m->velocity.y = 0.0f;
                    m->velocity.z = 10.0f * (dz / d);
                    m->counterF = 10.0f;
                    m->workF1 = 10.0f;
                    m->workF2 = d;
                }
                m->step++;
            }
            break;
        case 2:
            m->counterF += m->workF1;
            if (m->counterF >= m->workF2) {
                m->position.x = m->mHomePosition.x;
                m->position.y = m->mHomePosition.y;
                m->position.z = m->mHomePosition.z;
                BattleAudience_ChangeStatus(index, 0);
            }
            break;
        }
    }
}

void BattleAudienceCtrlProcessKinopioEscape(s32 index) {
    BattleAudienceMember* m;
    BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    BattleAudience_SetAnim(index, 7, FALSE);
    if (m->type == 11) {
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

void BattleAudienceCtrlProcessKinopioSleep(s32 index, s32 front) {
    BattleAudience* base;
    BattleAudienceMember* m;
    base = BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    if (m->step <= 1) {
        if ((u8)front == 1) {
            BattleAudience_SetAnim(index, 10, FALSE);
        } else {
            BattleAudience_SetAnim(index, 11, FALSE);
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
            BattleAudience_SetAnim(index, 0, FALSE);
        } else {
            BattleAudience_SetAnim(index, 1, FALSE);
        }
        m->baseScale.y = 1.0f;
        m->colorB = 0;
        m->step++;
    case 3:
        if (0.0f == m->hopAccel) {
            BattleAudience_ChangeStatus(index, 0);
        }
        break;
    }
}

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
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20008, // special
    0x7FF, // counterResistFlags
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

void BattleAudienceCtrlProcessKinopioTransEvt(s32 index) {
    BattleWork* bw = _battleWorkPointer;
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    static f32 x;
    static f32 y;
    static f32 z;
    BattleUnit* unit;
    BattleUnitPart* parts;
    s32 partsId;
    s8 st1;
    s8 st0;
    switch (m->step) {
    case 0:
        BattleAudience_SetAnim(index, 1, FALSE);
        m->targetUnit = BattleAudienceDetectTargetAll();
        m->counter = 0;
        psndSFXOn_3D("SFX_AUDIENCE_TELESA_MOVE1", &m->position);
        m->step = 5;
    case 5:
        unit = m->targetUnit;
        parts = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(unit), BtlUnit_GetBodyPartsId(unit));

        m->counter++;
        m->position.x = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(m->mHomePosition.x, 0.0f, 5, m->counter, 60);
        m->position.y = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(m->mHomePosition.y, 80.0f, 5, m->counter, 60);
        m->position.z = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(m->mHomePosition.z, 100.0f, 5, m->counter, 60);
        if (m->counter >= 60) {
            BattleAudience_SetAnim(index, 7, FALSE);
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
        m->position.x = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(0.0f, x, 0, m->counter, 30);
        m->position.y = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(80.0f, y - 15.0f, 4, m->counter, 30);
        m->position.z = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(100.0f, z, 0, m->counter, 30);
        if (m->counter == 30) {
            unit = m->targetUnit;
            parts = BattleGetUnitPartsPtr(BtlUnit_GetUnitId(unit), BtlUnit_GetBodyPartsId(unit));
            BtlUnit_GetStatus(unit, STATUS_INVISIBLE, &st1, &st0);
            if (st1 < 1 && !BtlUnit_CheckShadowGuard(unit)) {
                BattleCheckDamage(BattleGetSystemPtr(bw), unit, (struct BattleWorkUnitPart*)parts,
                                  &weapon_teresa_trans, 0x100);
                BtlUnit_snd_se(unit, "SFX_AUDIENCE_TELESA_INVISIBLE1", 0xF1194D80, 0);
            }
        }
        if (m->counter >= 120) {
            BattleAudience_ChangeStatus(index, 11);
        }
        break;
    }
}

s32 intrudeSound;

__declspec(section ".rodata") static Vec v0 = {0.0f, 0.0f, 0.0f};
void BattleAudienceCtrlProcessKinopioIntrude(s32 index) {
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
    BattleAudience_SetAnim(index, 7, FALSE);
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
            btl_camera_set_mode(0, 3);
            btl_camera_set_moveto(0, 30, 698.0f * m->counterF, 100.0f, 0.0f, 0.0f, 60.0f, 0.0f, 4);
            intrudeSound = psndSFXOn("SFX_AUDIENCE_HEIHO_MOVE1");
        }
        break;
    case 15:
        m->mRotation.y = camGetPtr(CAMERA_3D)->viewYaw;
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
            btl_camera_set_mode(0, 0);
            btl_camera_set_moveSpeedLv(0, 2);
            psndSFXOff(intrudeSound);
        }
        break;
    case 18:
        m->counter--;
        if (m->counter <= 0) {
            m->step = 19;
            m->counter = 60;
            btl_camera_set_moveSpeedLv(0, 1);
            effSmallStarEntry(0, 4, 185.0f * m->counterF, 20.0f, 40.0f, 0.0f, -1.0f, 0.0f);
        }
        break;
    case 19:
        m->counter--;
        if (m->counter <= 0) {
            m->step = 20;
            btl_camera_set_moveSpeedLv(0, 1);
        }
    case 20:
        m->statusTimer = BattleStage_ObjectFallCheck();
        m->step = 25;
        break;
    case 25:
        if (!evtCheckID(m->statusTimer)) {
            m->step = 30;
            m->mRotation.y = 0.0f;
        }
        break;
    case 30:
        BattleAudience_ChangeStatus(index, 11);
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
        BattleAudience_ChangeStatus(index, 11);
        break;
    }
}

void BattleAudienceCtrlProcessKinopioSing(s32 index) {
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
        m->mRotation.y += 6.0f;
        if (m->counter == 15) {
            BattleAudience_SetAnim(index, 14, FALSE);
            BattleAudienceSoundSing(index);
        }
        if (m->counter <= 0) {
            m->counter = 120;
            m->step++;
        }
        break;
    case 2:
        m->counter--;
        if ((u32)(s32)gp->retraceCount % 10 == 0) {
            z = m->position.z + (f32)irand(10) - 5.0f;
            y = m->position.y + (f32)irand(10);
            effOnpuN64Entry(0, m->position.x + (f32)irand(200) - 100.0f, y, z);
        }
        if (m->counter <= 0) {
            for (i = 0; i < base->mMaxAudience; i++) {
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
        m->mRotation.y += 6.0f;
        if (m->counter <= 15) {
            BattleAudience_SetAnim(index, 1, FALSE);
        }
        if (m->counter <= 0) {
            BattleAudience_ChangeStatus(index, 0);
        }
        break;
    }
}

void BattleAudienceCtrlProcessKinopioSingSub(s32 index) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if (m->flags & 0x100) {
        BattleAudienceCtrlProcessKinopioSingSub(index - 1);
    } else if (m->status != 17) {
        BattleAudience_ChangeStatus(index, 13);
        m->statusDelay = 3;
    }
}

void BattleAudienceCtrlProcessKinopioShell(s32 index) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if (m->step >= 2 && m->step <= 5) {
        BattleAudience_SetAnim(index, 15, FALSE);
    } else {
        BattleAudience_SetAnim(index, 1, FALSE);
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
            BattleAudience_ChangeStatus(index, 0);
        }
        m->statusTimer = 0;
        break;
    }
}

void BattleAudienceCtrlProcessKinopioIgnite(s32 index, s32 front) {
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 16, FALSE);
    } else {
        BattleAudience_SetAnim(index, 17, FALSE);
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

void BattleAudienceCtrlProcessKinopioFire(s32 index, s32 front) {
    BattleAudience* base;
    BattleAudienceMember* m;
    s32 i;
    base = BattleAudienceBaseGetPtr();
    m = BattleAudienceGetPtr(index);
    if ((u8)front == 1) {
        BattleAudience_SetAnim(index, 16, FALSE);
    } else {
        BattleAudience_SetAnim(index, 17, FALSE);
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
            m->flags &= ~4;
            if (index < 60) {
                effSandarsEntry(4, m->position.x, 35.0f + m->position.y, m->position.z, 0.0f);
            }
            btl_camera_shake_h(0, 5.0f, 0.0f, 30, 0);
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
        base->mTargetAudienceCount -= 1.0f;
        break;
    }
}

void BattleAudienceCtrlProcessKinopioFireSub(s32 index) {
    BattleAudience* base = BattleAudienceBaseGetPtr();
    BattleAudienceMember* m = BattleAudienceGetPtr(index);
    if (m->flags & 0x100) {
        BattleAudienceCtrlProcessKinopioFireSub(index - 1);
    } else if ((u8)BattleAudience_GetSysCtrl(index)) {
        if (m->type == 9) {
            if (m->status != 19) {
                BattleAudience_ChangeStatus(index, 19);
            } else {
                return;
            }
        } else if (m->status != 17) {
            BattleAudience_Delete(index);
            base->mTargetAudienceCount -= 1.0f;
        }
    }
}

void BattleAudienceCtrlProcessKinopioEat(s32 index) {
    BattleAudience* base;
    BattleAudienceMember* m;
    BattleStage* stage;
    BattleAudienceMember* t;
    s32 list[60];
    f32 px;
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
            BattleAudience_SetAnim(index, 0, FALSE);
        } else {
            BattleAudience_ChangeStatus(index, 0);
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
            BattleAudience_SetAnim(index, 18, FALSE);
            t->flags |= 2;
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
        base->mTargetAudienceCount -= 1.0f;
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
                if (!(u8)BattleAudience_GetExist(i) && !(BattleAudienceGetPtr(i)->flags & 0x100) &&
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
            pz = 0.1f * (f32)(k % 20) + (f32)(k / 20 * 30 + 115);
            m->mHomePosition.x = m->position.x = (f32)((rand() * 10 / 32767) + (k % 20 * 25 - 235));
            m->mHomePosition.y = stage->audienceSizeY;
            m->mHomePosition.z = m->position.z = pz;

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
            m->position.y = m->mHomePosition.y = stage->audienceSizeY;
            m->velocity.y = 0.0f;
            m->step = 99;
            m->counter = 10;
            memcpy(BattleAudienceGetPtr(m->statusTimer), m, 0x134);
            BattleAudience_Delete(index);
        }
        break;
    case 99:
        m->counter--;
        if (m->counter <= 0) {
            BattleAudience_ChangeStatus(index, 0);
        }
        break;
    }
}
