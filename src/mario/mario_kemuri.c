#include "mario/mario.h"
#include "mario/mariost.h"
#include "driver/effdrv.h"
#include "driver/hitdrv.h"
#include "eff/eff_kemuri.h"
#include "eff/eff_ripple.h"
#include "eff/n64/eff_kemuri7_n64.h"
#include "party.h"
#include "system.h"
#include <string.h>
#include "mario/mario_sbr.h"
#include "eff/eff_mugi.h"
#include "eff/n64/eff_water_damage_n64.h"
#include "koopa.h"
#include "libc_decls.h"

extern EffectEntry* effKemuri2Entry(s32 type, f32 x, f32 y, f32 z, f32 s, f32 c);

void kemuri(void);
void kemuri_mugi(void);
void mario_kemuri_landon(void);


void mario_kemuri(void) {
    static s32 ct = 8;
    static s32 left_or_right = 0;
    MarioWork* mario = marioGetPtr();
    u32 attr;
    f32 c, s;

    attr = 0;

    if (marioStGetSystemLevel() == 0) {
      if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
        if (mario->currMotionId == MARIO_MOTION_DOKAN) {
            // FAKE: no-op self-assignment as the then-branch; without it the branch is laid out differently
            s = s;
        } else {
        if (mario->currMotionId != MARIO_MOTION_JUMP) {
            if (mario->hitobjStandOn != NULL) {
                attr = hitGetAttr(mario->hitobjStandOn);
            }
        } else if (mario->hitobjJumpFrom != NULL) {
            attr = hitGetAttr(mario->hitobjJumpFrom);
        }
        {
            if (attr & 0x1000) {
                kemuri_mugi();
            } else if (attr & 0x100) {
                mario_kemuri_landon();
            } else if (attr & 0x100000) {
                mario = marioGetPtr();
                if (mario->currMotionId != MARIO_MOTION_DASH && mario->currMotionId != MARIO_MOTION_WALK) {
                    ct = 8;
                } else if (ct++ > 8) {
                    ct = 0;
                    sincosf(&s, &c, 6.2831855f * reviseAngle(-mario->directionView) / 360.0f);
                    effKemuri7N64Entry(mario->position.x + 0.2f * (mario->wPlayerCollisionBox.x * s),
                                       0.5f + mario->position.y,
                                       mario->position.z + 0.2f * (mario->wPlayerCollisionBox.x * c),
                                       -mario->directionView, (f32)left_or_right);
                    left_or_right = left_or_right == 0;
                }
            } else if (!(mario->flags & MARIO_FLAG_TUBE_MODE)) {
                if (kpaGetBodyStatus() <= 0 && mario->characterId != CHARACTER_PEACH) {
                    kemuri();
                }
            }
        }
        }
      }
    }
    out:;
}

void mario_simple_kemuri(void) {
    MarioWork* mario = marioGetPtr();
    EffectEntry* eff = effKemuriEntry(0, mario->position.x, mario->position.y, mario->position.z, 0.0f);

    if (mario->flags & MARIO_FLAG_BG_MODE) {
        ((effKemuriData*)eff->userdata)->scale = 0.6f;
    }
}

void mario_land_kemuri(void) {
    MarioWork* mario = marioGetPtr();
    HitEntry* hit = mario->hitobjStandOn;
    u32 attr;
    EffectEntry* eff;

    if (hit != NULL) {
        if (mario->trigFlags & MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI) {
            mario->trigFlags &= ~MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI;
        } else {
            attr = hitGetAttr(hit);
            if (attr & 0x1000) {
                kemuri_mugi();
            } else if (attr & 0x100) {
                mario_kemuri_landon();
            } else {
                eff = effKemuriEntry(0, mario->position.x, mario->position.y, mario->position.z, 0.0f);
                if (mario->flags & MARIO_FLAG_BG_MODE) {
                    ((effKemuriData*)eff->userdata)->scale = 0.6f;
                }
            }
        }
    }
}

void mario_kemuri_landon(void) {
    static s32 cnt = 0;
    static s32 cnt2 = 10;
    MarioWork* mario = marioGetPtr();
    s32 type = 0;

    if (strcmp(gp->currentMapName, "mri_05") == 0) {
        type = 10;
    }
    if (mario->currMotionId == MARIO_MOTION_JUMP && mario->airTimer == 1 && cnt2 == 10) {
        effWaterDamageN64Entry(type + 5, 30, mario->position.x, mario->position.y, mario->position.z, 1.0f);
        cnt2 = 0;
        effRippleEntry(0, mario->position.x, 11.0f + mario->position.y, mario->position.z);
    }
    cnt2++;
    if (cnt2 > 10) {
        cnt2 = 10;
    }
    if (mario->currMotionId == MARIO_MOTION_DASH || mario->currMotionId == MARIO_MOTION_WALK || !mario->currMotionId || mario->currMotionId == MARIO_MOTION_LAND) {
        if (mario->currMotionId == MARIO_MOTION_LAND) {
            cnt = 0;
        }
        if (--cnt < 0) {
            cnt = rand() % 20 + 30;
            effRippleEntry(0, mario->position.x, 11.0f + mario->position.y, mario->position.z);
        }
        if (mario->currMotionId == MARIO_MOTION_WALK || mario->currMotionId == MARIO_MOTION_DASH || mario->currMotionId == MARIO_MOTION_LAND) {
            if (gp->retraceCount % 20 == 0 || mario->currMotionId == MARIO_MOTION_LAND) {
                effWaterDamageN64Entry(type + 7, 30, mario->position.x, 11.0f + mario->position.y, mario->position.z, 1.0f);
            }
        }
    }
}

void kemuri_mugi(void) {
    static f32 dir = 0;
    static s32 ct = 4;
    static s32 ct2 = 10;
    static s32 left_or_right = 0;
    MarioWork* mario = marioGetPtr();

    if (strncmp(gp->currentMapName, "mri", 3) == 0) {
        if (mario->currMotionId == MARIO_MOTION_STAY && gp->retraceCount % 30 == 0) {
            effWaterDamageN64Entry(16, 30, mario->position.x, 11.0f + mario->position.y, mario->position.z, 0.8f);
        }
        if (mario->currMotionId == MARIO_MOTION_WALK || mario->currMotionId == MARIO_MOTION_DASH || mario->currMotionId == MARIO_MOTION_LAND) {
            if (gp->retraceCount % 20 == 0 || mario->currMotionId == MARIO_MOTION_LAND) {
                effWaterDamageN64Entry(17, 30, mario->position.x, 11.0f + mario->position.y, mario->position.z, 0.8f);
            }
        }
    } else if (mario->currMotionId == MARIO_MOTION_JUMP && mario->airTimer == 1 && ct2 == 10) {
        effMugiKemuriEntry(mario->position.x, 14.0f + mario->position.y, mario->position.z, dir);
        dir = reviseAngle(35.0f + dir);
        ct2 = 0;
    } else {
        ct2++;
        if (ct2 > 10) {
            ct2 = 10;
        }
        if (mario->currMotionId != MARIO_MOTION_DASH && mario->currMotionId != MARIO_MOTION_WALK) {
            ct = 2;
        } else if (ct++ > 2) {
            f32 s, c;
            f32 x, y, z;
            ct = 0;
            sincosf(&s, &c, 6.2831855f * reviseAngle(-mario->directionView) / 360.0f);
            x = -0.4f * (mario->wPlayerCollisionBox.x * s) + mario->position.x;
            y = 3.0f + mario->position.y;
            z = -0.4f * (mario->wPlayerCollisionBox.x * c) + mario->position.z;
            effMugiKemuri2Entry(0, x, y, z, -mario->directionView + (f32)(rand() % 10) - 5.0f, (f32)left_or_right);
            left_or_right = left_or_right == 0;
        }
    }
}

void kemuri(void) {
    static f32 dir = 0;
    static s32 ct = 4;
    MarioWork* mario = marioGetPtr();
    EffectEntry* eff;
    f32 s, c;
    f32 x, y, z;

    if (mario->prevAirTimer != 0 && mario->prevHitobjStandOn == NULL && mario->hitobjStandOn != NULL) {
        if (mario->trigFlags & MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI) {
            mario->trigFlags &= ~MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI;
        } else {
            x = mario->position.x;
            y = mario->position.y;
            z = mario->position.z;
            if (mario->characterId == CHARACTER_BOWSER) {
                eff = effKemuriEntry(0, x, y, 10.0f + z, 0.0f);
                ((effKemuriData*)eff->userdata)->scale = kpaGetBaseScale(kpaGetLevel());
            } else {
                effKemuriEntry(0, x, y, z, dir);
            }
        }
    } else if (mario->currMotionId != MARIO_MOTION_DASH) {
        ct = 7;
    } else if (mario->characterId != CHARACTER_BOWSER) {
        if (ct++ > 6) {
            ct = 0;
            sincosf(&s, &c, 6.2831855f * reviseAngle(-mario->directionView) / 360.0f);
            {
            x = mario->position.x + 0.2f * (mario->wPlayerCollisionBox.x * s);
            y = 1.5f + mario->position.y;
            z = mario->position.z + 0.2f * (mario->wPlayerCollisionBox.x * c);
            eff = effKemuri2Entry(0, x, y, z, s, c);
            }
            if (mario->flags & MARIO_FLAG_BG_MODE) {
                ((effKemuri2Data*)eff->userdata)->scale2 = 0.6f;
            }
        }
    } else if (--ct <= 0) {
        f32 scale, dist;
        sincosf(&s, &c, 6.2831855f * reviseAngle(-mario->directionView) / 360.0f);
        sincosf(&s, &c, toMovedirSimple(180.0f + (kpaGetStageViewType() == 0 ? mario->displayDirectionTarget : mario->displayDirection)));
        switch (kpaGetLevel()) {
        case 0:
            ct = 4;
            dist = 10.0f;
            scale = 0.5f;
            break;
        case 1:
            ct = 7;
            dist = 15.0f;
            scale = 1.0f;
            break;
        case 2:
            ct = 10;
            dist = 20.0f;
            scale = 2.0f;
            break;
        case 3:
            ct = 10;
            dist = 50.0f;
            scale = 4.0f;
            break;
        }
        x = mario->position.x + dist * s;
        y = 1.5f + mario->position.y;
        z = mario->position.z + dist * c;
        eff = effKemuri2Entry(0, x, y, z, s, c);
        ((effKemuri2Data*)eff->userdata)->scale2 = scale;
    }
}

void party_kemuri(PartyEntry* party) {

#ifdef VERSION_G8ME01
    if (marioStGetSystemLevel() == 0) {
        if (party->runMode != 10) {
            if (party->moveKind != 1) {
                if (party->runMode == 3 && party->currentMemberId == PARTY_MEMBER_YOSHI) {
                    if ((void*)party->hitobjStandOn != NULL) {
                        if (!(hitGetAttr((void*)party->hitobjStandOn) & HIT_ATTR_SHALLOW_WATER)) {
                            if (party->moveSpeed < 2.0f) {
                            } else if (party->kemuriTimer++ > 6) {
                                {
                                    f32 c, s;
                                    party->kemuriTimer = 0;
                                    sincosf(&s, &c, 6.2831855f * reviseAngle(-party->targetDir) / 360.0f);
                                    effKemuri2Entry(0, party->position.x + 0.2f * (party->width * s), 1.5f + party->position.y, party->position.z + 0.2f * (party->width * c), s, c);
                                }
                            }
                        }
                    } else {
                        party->kemuriTimer = 4;
                    }
                } else if (party->motionId == 1) {
                    if ((void*)party->hitobjStandOn != NULL) {
                        if (!(hitGetAttr((void*)party->hitobjStandOn) & HIT_ATTR_SHALLOW_WATER)) {
                            if (party->moveSpeed < 2.0f) {
                            } else if (party->kemuriTimer++ > 6) {
                                {
                                    f32 c, s;
                                    party->kemuriTimer = 0;
                                    sincosf(&s, &c, 6.2831855f * reviseAngle(-party->targetDir) / 360.0f);
                                    effKemuri2Entry(0, party->position.x + 0.2f * (party->width * s), 1.5f + party->position.y, party->position.z + 0.2f * (party->width * c), s, c);
                                }
                            }
                        }
                    } else {
                        party->kemuriTimer = 4;
                    }
                }
#else
    if (party->moveKind != 1) {
        if (party->runMode == 3 && party->currentMemberId == PARTY_MEMBER_YOSHI) {
            if ((void*)party->hitobjStandOn != NULL) {
                if (party->moveSpeed < 2.0f) {
                    party->kemuriTimer = 7;
                } else if (party->kemuriTimer++ > 6) {
                    f32 c, s;
                    f32 x, y, z;
                    party->kemuriTimer = 0;
                    sincosf(&s, &c, 6.2831855f * reviseAngle(-party->targetDir) / 360.0f);
                    effKemuri2Entry(0, party->position.x + 0.2f * (party->width * s), 1.5f + party->position.y, party->position.z + 0.2f * (party->width * c), s, c);
                }
            } else {
                party->kemuriTimer = 4;
            }
        } else if (party->motionId == 1) {
            if ((void*)party->hitobjStandOn != NULL) {
                if (party->moveSpeed < 2.0f) {
                    party->kemuriTimer = 7;
                } else if (party->kemuriTimer++ > 6) {
                    f32 c, s;
                    f32 x, y, z;
                    party->kemuriTimer = 0;
                    sincosf(&s, &c, 6.2831855f * reviseAngle(-party->targetDir) / 360.0f);
                    effKemuri2Entry(0, party->position.x + 0.2f * (party->width * s), 1.5f + party->position.y, party->position.z + 0.2f * (party->width * c), s, c);
                }
            } else {
                party->kemuriTimer = 4;
#endif
            }
        }
    }
}
