// Dust and splash effects under Mario's feet (walking, landing, running on water).

#include "version.h"
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
#include "mario/mario_kemuri.h"

static void kemuri(void);
static void kemuri_mugi(void);
void kemuri_water(void);
static void partyKemuri(PartyEntry* party);

/**
 * Per-frame footstep effects for the player character,
 * run from marioMain and from Peach's and Bowser's main loops.
 * Picks by the floor's hit attribute: 0x1000 floors use kemuri_mugi,
 * water (0x100) splashes via kemuri_water,
 * 0x100000 floors leave the N64-style kemuri7 puffs while walking,
 * and other floors get the normal dust from kemuri (not for Peach).
 * Nothing in tube mode, while entering a pipe or at a raised system level.
 */
void mario_kemuri(void) {
#if VERSION >= VERSION_JP
    void kemuri_water(void);
#endif
    static s32 ct = 8;
    static s32 left_or_right = 0;
    MarioWork* mario = marioGetPtr();
    u32 attr;
    f32 c, s;

    attr = 0;

    // no dust at a raised system level, in tube mode or while going through a pipe
    if (marioStGetSystemLevel() != 0 || (mario->flags & MARIO_FLAG_TUBE_MODE) ||
        mario->currMotionId == MARIO_MOTION_DOKAN) {
        return;
    }
    if (mario->currMotionId != MARIO_MOTION_JUMP) {
        if (mario->hitobjStandOn != NULL) {
            attr = hitGetAttr(mario->hitobjStandOn);
        }
    } else if (mario->hitobjJumpFrom != NULL) {
        attr = hitGetAttr(mario->hitobjJumpFrom);
    }
    if (attr & HIT_ATTR_GRASS) {
        kemuri_mugi();
        return;
    }
    if (attr & HIT_ATTR_SHALLOW_WATER) {
        kemuri_water();
        return;
    }
    if (attr & HIT_ATTR_SNOW) {
        mario = marioGetPtr();
        if (mario->currMotionId != MARIO_MOTION_DASH && mario->currMotionId != MARIO_MOTION_WALK) {
            ct = 8;
            return;
        }
        if (ct++ > 8) {
            ct = 0;
            sincosf(&s, &c, 6.2831855f * reviseAngle(-mario->directionView) / 360.0f);
            effKemuri7N64Entry(mario->position.x + 0.2f * (mario->wPlayerCollisionBox.x * s),
                               0.5f + mario->position.y,
                               mario->position.z + 0.2f * (mario->wPlayerCollisionBox.x * c),
                               -mario->directionView, (f32)left_or_right);
            left_or_right = left_or_right == 0;
        }
        return;
    }
    if (!(mario->flags & MARIO_FLAG_TUBE_MODE) && kpaGetBodyStatus() <= KPA_BODY_DRY &&
        mario->characterId != CHARACTER_PEACH) {
        kemuri();
    }
}

/**
 * Spawns a single dust puff at Mario's feet (smaller in background mode).
 * Used by the damage motions in mot_damage.c when he hits the ground.
 */
void mario_simple_kemuri(void) {
    MarioWork* mario = marioGetPtr();
    EffectEntry* eff = effKemuriEntry(0, mario->position.x, mario->position.y, mario->position.z, 0.0f);

    if (mario->flags & MARIO_FLAG_BG_MODE) {
        ((effKemuriData*)eff->userdata)->scale = 0.6f;
    }
}

/**
 * Landing effect after a Spring Jump (mot_jabara.c):
 * a 0x1000-floor effect, a water splash, or a dust puff, depending on the floor.
 * Skipped once if MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI is set, which it clears.
 */
// Original name from the JP demo map, where mot_jabara calls it; there the water splash below is the stripped
// kemuri_water (0x1a4, inlined everywhere).
void mario_kemuri_landon(void) {
#if VERSION >= VERSION_JP
    void kemuri_water(void);
#endif
    MarioWork* mario = marioGetPtr();
    HitEntry* hit = mario->hitobjStandOn;
    u32 attr;
    EffectEntry* eff;

    if (hit != NULL) {
        if (mario->trigFlags & MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI) {
            mario->trigFlags &= ~MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI;
        } else {
            attr = hitGetAttr(hit);
            if (attr & HIT_ATTR_GRASS) {
                kemuri_mugi();
            } else if (attr & HIT_ATTR_SHALLOW_WATER) {
                kemuri_water();
            } else {
                eff = effKemuriEntry(0, mario->position.x, mario->position.y, mario->position.z, 0.0f);
                if (mario->flags & MARIO_FLAG_BG_MODE) {
                    ((effKemuriData*)eff->userdata)->scale = 0.6f;
                }
            }
        }
    }
}

/**
 * Water effects at Mario's feet: a splash and a ripple on the first frame of a jump (at most every 10 frames), a
 * ripple every 30 to 49 frames while he stands, walks or dashes (and at once on landing), and a splash every 20 frames
 * while he walks or dashes and on landing. JP and later use other splash types in mri_05.
 */
#if VERSION <= VERSION_DEMO_JP
static inline
#endif

void kemuri_water(void) {
    static s32 cnt = 0;
    static s32 cnt2 = 10;
    MarioWork* mario = marioGetPtr();
    s32 type = 0;
#if VERSION >= VERSION_JP
    if (strcmp(gp->currentMapName, "mri_05") == 0) {
        type = 10;
    }
#endif
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

/**
 * Effects for floors with HIT_ATTR_GRASS. On the Great Tree maps (mri) water-drop effects: every 30 frames while
 * Mario stands, every 20 frames (and on landing) while he walks or dashes. Elsewhere mugi (wheat) puffs: on the first
 * frame of a jump (at most every 10 frames) and every few frames while he walks or dashes.
 */
static void kemuri_mugi(void) {
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

/**
 * Normal dust: a puff when Mario lands on a floor after being airborne,
 * and trailing dust every eight calls while Mario dashes (Bowser's interval depends on his size level).
 * For Bowser (characterId 2) the landing puff and the dash dust grow with his size level (kpaGetLevel).
 */
static void kemuri(void) {
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
        x = mario->position.x + 0.2f * (mario->wPlayerCollisionBox.x * s);
        y = 1.5f + mario->position.y;
        z = mario->position.z + 0.2f * (mario->wPlayerCollisionBox.x * c);
        eff = effKemuri2Entry(0, x, y, z, s, c);
            if (mario->flags & MARIO_FLAG_BG_MODE) {
                ((effKemuri2Data*)eff->userdata)->scale2 = 0.6f;
            }
        }
    } else if (--ct <= 0) {
        f32 scale, dist;
        sincosf(&s, &c, 6.2831855f * reviseAngle(-mario->directionView) / 360.0f);
        sincosf(&s, &c, toMovedirSimple(180.0f + (kpaGetStageViewType() == KPA_VIEW_SIDE ? mario->displayDirectionTarget : mario->displayDirection)));
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

/**
 * Dust trail behind the partner, run each frame from party.c:
 * every eight qualifying updates while it walks at speed 2 or more, or while Yoshi carries Mario.
 * JP and the early demos also puff over water, in pipe mode and at any system level.
 */
void party_kemuri(PartyEntry* party) {
#if VERSION >= VERSION_US
    if (marioStGetSystemLevel() == 0) {
        if (party->runMode != PARTY_RUN_PIPE) {
            if (party->moveKind != 1) {
                if (party->runMode == PARTY_RUN_USE && party->currentMemberId == PARTY_YOSHI) {
                    if (party->hitobjStandOn != NULL) {
                        if (!(hitGetAttr(party->hitobjStandOn) & HIT_ATTR_SHALLOW_WATER)) {
                            partyKemuri(party);
                        }
                    } else {
                        party->kemuriTimer = 4;
                    }
                } else if (party->motionId == PARTY_MOTION_WALK) {
                    if (party->hitobjStandOn != NULL) {
                        if (!(hitGetAttr(party->hitobjStandOn) & HIT_ATTR_SHALLOW_WATER)) {
                            partyKemuri(party);
                        }
                    } else {
                        party->kemuriTimer = 4;
                    }
                }
#else
    if (party->moveKind != 1) {
        if (party->runMode == PARTY_RUN_USE && party->currentMemberId == PARTY_YOSHI) {
            if (party->hitobjStandOn != NULL) {
                partyKemuri(party);
            } else {
                party->kemuriTimer = 4;
            }
        } else if (party->motionId == PARTY_MOTION_WALK) {
            if (party->hitobjStandOn != NULL) {
                partyKemuri(party);
            } else {
                party->kemuriTimer = 4;
#endif
            }
        }
    }
}

/**
 * One step of the partner's dust trail (party_kemuri): a puff every eight calls at speed 2 or more, offset
 * behind the partner by a fifth of its width. Below speed 2 there is no puff; JP and the early demos then set
 * the timer so the next faster step puffs at once.
 */
// Original name and size (0xc8) from the JP demo map (UNUSED there: every call is inlined). Where the US water
// test sits (here or in the callers) is not fixed by the bytes.
static void partyKemuri(PartyEntry* party) {
    f32 s, c;

    if (party->moveSpeed < 2.0f) {
#if VERSION >= VERSION_US
        return;
#else
        party->kemuriTimer = 7;
#endif
    } else if (party->kemuriTimer++ > 6) {
        party->kemuriTimer = 0;
        sincosf(&s, &c, 6.2831855f * reviseAngle(-party->targetDir) / 360.0f);
        effKemuri2Entry(0, party->position.x + 0.2f * (party->width * s), 1.5f + party->position.y, party->position.z + 0.2f * (party->width * c), s, c);
    }
}
