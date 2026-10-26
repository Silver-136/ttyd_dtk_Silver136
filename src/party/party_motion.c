#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "pmario_sound.h"
#include "driver/hitdrv.h"
#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include "mario/mariost.h"
#include <string.h>
#include "version.h"
#include "libc_decls.h"
#include "sfx_name_word.h"
#include "mario/mario_party.h"
#include "mario/mario_sbr.h"
#include "system.h"
#include "party/party_move.h"
#include "party/party_hit.h"
#include "party/party_bomhei.h"
#include "party/party_nokonoko.h"
#include "party/party_motion.h"
#include "mot/mot_stay.h"
#include "party/party_chuchu.h"
#include "mot/mot_slit.h"
#include "mario/mario_hit.h"

#include "party.h"
#include "mario/mario.h"
#include <math.h>

static BOOL partyChkMarioFollowY(MarioWork* mario);
void partyChgMotSlitFloor(PartyEntry* party);

static s32 unk_804189f4;
static f32 touch_wall_ct;
static s32 upstairs_count;
static f32 upstairs_dy;

BOOL party_slit(PartyEntry* party);
BOOL party_roll(PartyEntry* party);
void partyFlyAdjustY(PartyEntry* party);
void partyWalkChkGnd(PartyEntry* party);
s32 partyGetFootmarkId();

// Note: the real evt_snd user functions live in this split (see evt_snd.c).
// Functions are defined in reverse order (-inline deferred).

void psndSFX_vol(s32 id, u8 vol);
void psndSFX_pit(s32 id, s16 pit);

USER_FUNC(evt_snd_bgmon) {
    EvtWord* args = event->args;
    psndBGMOn(args[0], (char*)args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgmon_f) {
    EvtWord* args = event->args;
    s32 a = args[0];
    const char* bgm_name = (const char*)args[1];
    s32 fadetime = evtGetValue(event, args[2]);
    psndBGMOn_f(a, bgm_name, fadetime, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgmon_f_d) {
    EvtWord* args = event->args;
    s32 a = args[0];
    const char* bgm_name = (const char*)args[1];
    s32 c = evtGetValue(event, args[2]);
    evtGetValue(event, args[3]);
    psndBGMOn_f_d(a, bgm_name, c, c, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgmoff) {
    psndBGMOff(event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgmoff_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 t = evtGetValue(event, args[1]);
    psndBGMOff_f_d(flags, t, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgm_freq) {
    EvtWord* args = event->args;
    s32 id = args[0];
    s32 t = evtGetValue(event, args[1]);
    psndBGM_rate(id, t);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgm_scope) {
    EvtWord* args = event->args;
    s32 id = args[0];
    s32 t = evtGetValue(event, args[1]);
    psndBGMScope(id, t);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    s32 out = args[1];
    if (!out) {
        psndSFXOn(name);
    } else {
        evtSetValue(event, out, psndSFXOn(name));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon__) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 out = args[1];
    if (!out) {
        psndSFXOn(name);
    } else {
        evtSetValue(event, out, psndSFXOn(name));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon_) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    s32 out = args[1];
    if (!out) {
        psndSFXOn_(name);
    } else {
        evtSetValue(event, out, psndSFXOn_(name));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon_3d) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    Vec pos;
    s32 out;
    pos.x = evtGetFloat(event, args[1]);
    pos.y = evtGetFloat(event, args[2]);
    pos.z = evtGetFloat(event, args[3]);
    out = args[4];
    if (!out || out == EVT_NULLPTR) {
        psndSFXOn_3D(name, &pos);
    } else {
        evtSetValue(event, out, psndSFXOn_3D(name, &pos));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon_3d_ex) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    Vec pos;
    u16 a, b, c, d;
    s32 out;
    pos.x = evtGetFloat(event, args[1]);
    pos.y = evtGetFloat(event, args[2]);
    pos.z = evtGetFloat(event, args[3]);
    a = evtGetValue(event, args[4]);
    b = evtGetValue(event, args[5]);
    c = evtGetValue(event, args[6]);
    d = evtGetValue(event, args[7]);
    out = args[8];
    if (!out) {
        psndSFXOnEx_3D(name, &pos, a, b, c, d);
    } else {
        evtSetValue(event, out, psndSFXOnEx_3D(name, &pos, a, b, c, d));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxoff) {
    psndSFXOff(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxchk) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], psndSFXChk(evtGetValue(event, args[0])));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_pit) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 t = evtGetValue(event, args[1]);
    psndSFX_pit(id, t);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_vol) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 t = evtGetValue(event, args[1]);
    psndSFX_vol(id, t);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_pos) {
    EvtWord* args = event->args;
    Vec pos;
    s32 id = evtGetValue(event, args[0]);
    pos.x = evtGetFloat(event, args[1]);
    pos.y = evtGetFloat(event, args[2]);
    pos.z = evtGetFloat(event, args[3]);
    psndSFX_pos(id, &pos);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_dist) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    psndSFX_dist(id, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envon) {
    EvtWord* args = event->args;
    psndENVOn(args[0], (const char*)args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envon_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 b = args[1];
    s32 fadetime = evtGetValue(event, args[2]);
    psndENVOn_f_d(flags, SFX_NAME_WORD(b), fadetime, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envoff) {
    psndENVOff(event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envoff_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 t = evtGetValue(event, args[1]);
    psndENVOff_f_d(flags, t, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_env_lpf) {
    EvtWord* args = event->args;
    s32 id = args[0];
    psndENV_LPF(id, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_set_rev_mode) {
    psndSetReverb(event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_all_stop) {
    psndSFXAllOff();
    return EVT_RETURN_DONE;
}

void partyResetTouchWallCt(void) {
    touch_wall_ct = 60.0f;
}

void N_partyFollowCloseOn(PartyEntry* party) {
    party->trigFlags |= PARTY_TRIG_FOLLOW_CLOSE;
}

void N_partyFollowCloseOff(PartyEntry* party) {
    party->trigFlags &= ~PARTY_TRIG_FOLLOW_CLOSE;
}

static inline HitEntry* partyBehindGround(PartyEntry* party) {
    HitEntry* hit = partySearchGround(party, 10.0f, 0.0f);
    if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
        hit = NULL;
        if (!party->splashTimer) {
            party->trigFlags |= PARTY_TRIG_SPLASH;
            party->flags |= PARTY_FLAG_FORCE_FALL;
        }
    }
    return hit;
}

static f32 partyStayDist(PartyEntry* party);
static u32 partyGetFrontAttr(PartyEntry* party, f32 dir);
static f32 partyHomingOff(PartyEntry* party);
static f32 partyGetGndY(f32 x, f32 y, f32 z);
static f32 partyGetDirGndY(Vec u, f32 md);
static u32 partyGetFrontAttrFar(PartyEntry* party);
static void partyGorFix(PartyEntry* party);
static void partyPoseA2(PartyEntry* party);
s32 getFrontFloor(PartyEntry* party, Vec* out, f32 dir);


void party_force_reset_outofscreen(PartyEntry* party);

void party_motion_stay(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    s32 id;
    PartyEntry* other;
    HitEntry* hit;
    f32 lim;
    f32 x, z;
    f32 dir, spd;
    Vec v;
    Vec p;

    id = partyGetFootmarkId(party);
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        partyChgPoseId(party, 1);
        if (!(party->trigFlags & PARTY_TRIG_LANDED)) {
            if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
                partyClearFootmark();
            } else {
                partyClearFootmark2();
            }
        }
        party->trigFlags &= ~PARTY_TRIG_LANDED;
        party->yoshiPsndSFXId = -1;
        partyForceSetSpd(party, 0.0f);
    }
    if (mario->currMotionId != MARIO_MOTION_TALK && (party->flags & PARTY_FLAG_IS_BEHIND_MARIO)) {
        party->flags &= ~PARTY_FLAG_IS_BEHIND_MARIO;
    }
    lim = party->homingDist;
    if (!mario->controlStickSensitivity && mario->baseSpeed < 1.0f) {
        lim = 100.0f;
    }
    if ((s8)mario->characterId == CHARACTER_BOWSER) {
        lim = 70.0f;
    }
    if (!strcmp(gp->currentMapName, "aji_02") || !strcmp(gp->currentMapName, "aji_05") ||
        !strcmp(gp->currentMapName, "aji_12")) {
        if (!marioKeyOffChk()) {
            {
                f32 sd;
                if (party->moveKind == 0) {
                    sd = 18.0f;
                } else if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
                    if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
                        sd = 8.0f;
                    } else {
                        sd = 30.0f;
                    }
                } else if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
                    if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
                        sd = 8.0f;
                    } else {
                        sd = 30.0f;
                    }
                }
                lim = sd;
            }
            party->trigFlags |= PARTY_TRIG_FOLLOW_CLOSE;
        }
    }
    if ((party->trigFlags >> 17) & 1) {
        {
            f32 sd;
            if (party->moveKind == 0) {
                sd = 18.0f;
            } else if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
                if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
                    sd = 8.0f;
                } else {
                    sd = 30.0f;
                }
            } else if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
                if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
                    sd = 8.0f;
                } else {
                    sd = 30.0f;
                }
            }
            lim = sd;
        }
    }
    party_slit(party);
    party_roll(party);
    partyChgStayPose(party);
    if (mario->currMotionId == MARIO_MOTION_TALK && !(party->flags & PARTY_FLAG_IS_BEHIND_MARIO)) {
        partyMoveBehindMario(party, 0);
        return;
    }
    if (party->runMode != 0) {
        if (!party->currentSlotId || !partyGetPtr(marioGetPartyId())) {
            if (partyChkMarioFollowY(party->playerPtr)) {
                f32 d;
                if (party->moveKind == 0) {
                    p = party->position;
                    d = partyGetTargetDist3D(id, &p);
                    partyGetTargetDist(id, party->position.x, party->position.z);
                } else {
                    x = party->position.x;
                    z = party->position.z;
                    movePos(&x, &z, 0.5f * party->width,
                            angleABf(party->position.x, party->position.z, mario->position.x, mario->position.z));
                    d = distABf(x, z, mario->position.x, mario->position.z);
                }
                if (marioChkSlitThrouh() || d >= lim) {
                    if (!marioChkDeepSleep()) {
                        partyChgMot(party, 1);
                    }
                }
            }
        } else {
            other = anotherPartyGetPtr(party->currentSlotId);
            if (other) {
                if (nokonokoGetStatus(other) == 3 || PSVECDistance(&mario->position, &other->position) >= 150.0f) {
                    if (partyGetTargetDist(id, party->position.x, party->position.z) >= lim) {
                        partyChgMot(party, 1);
                        partyClearFootmark2();
                    }
                } else if (other->currentMemberId == PARTY_MEMBER_BOBBERY && other->runMode == 3) {
                    if (mario->currMotionId != MARIO_MOTION_DAMAGE && mario->currMotionId != MARIO_MOTION_HIP && mario->currMotionId != MARIO_MOTION_HIP_2 &&
                        distABf(party->position.x, party->position.z, mario->position.x, mario->position.z) >= lim) {
                        partyChgMot(party, 1);
                    }
                } else if (!(other->flags & PARTY_FLAG_IS_BEING_USED) || other->currentMemberId == 4) {
                    if (distABf(party->position.x, party->position.z, other->position.x, other->position.z) >= lim) {
                        partyChgMot(party, 1);
                    }
                }
                if (marioChkSlitThrouh()) {
                    partyChgMot(party, 1);
                }
            }
        }
    }
    v = party->position;
    partyGetMoveDirSpd(party, &dir, &spd);
    hit = partySearchFrontWall(party, &v, spd, dir);
    if (hit) {
        party->hitobjFront = hit;
        if (party->moveSpeed) {
            party->hitobjPush = hit;
        }
    }
    party->position.x = v.x;
    party->position.z = v.z;
    partyGetMoveDirSpd(party, &dir, &spd);
#ifdef VERSION_G8ME01
    partyChkWallAround(party, &v, spd, dir, party->width, party->height);
#else
    marioChkWallAround(&v, 1, spd, dir, party->width, party->height);
#endif
    party->position.x = v.x;
    party->position.z = v.z;
    if (party->moveKind == 1) {
        partyFlyAdjustY(party);
        return;
    }
    partyWalkChkGnd(party);
    party_force_reset_outofscreen(party);
    if (party->currentMemberId == PARTY_MEMBER_MS_MOWZ && !(party->flags2 & PARTY_DISP_FLAG_FAMICOM) && !marioKeyOffChk() && marioChkDeepSleep() != 1 &&
        chuchu_searchObject()) {
        switch (party->useMotionId) {
        case 1:
            if (--party->moveFrames > 0) {
                break;
            }
            partyChgPoseId(party, 1);
            party->moveFrames = 30;
            party->useMotionId = 2;
            break;
        case 2:
            if (--party->moveFrames > 0) {
                break;
            }
            party->useMotionId = 0;
            break;
        case 0:
        default:
            partyPoseA2(party);
            break;
        }
    }
}

void party_motion_wait(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        partyChgPoseId(party, 1);
        if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
            partyClearFootmark();
        } else {
            partyClearFootmark2();
        }
        party->yoshiPsndSFXId = -1;
    }
    party_slit(party);
    party_roll(party);
    partyChgStayPose(party);
    if (!(party->flags & PARTY_FLAG_DISABLE_FRONT_WALL_CHK)) {
        partyChkWall(party);
    }
    partyChkGnd(party);
    if (distABf(party->position.x, party->position.z, mario->position.x, mario->position.z) > 31.0f) {
        partyChgRunMode(party, 2);
    }
    if (mario->currMotionId == MARIO_MOTION_TALK && !(party->flags & PARTY_FLAG_IS_BEHIND_MARIO)) {
        partyMoveBehindMario(party, 0);
    } else if (party->moveKind == 1) {
        partyFlyAdjustY(party);
    } else {
        partyWalkChkGnd(party);
        if (party->currentMemberId == PARTY_MEMBER_MS_MOWZ && !(party->flags2 & PARTY_DISP_FLAG_FAMICOM) && !marioKeyOffChk() &&
            marioChkDeepSleep() != 1 && chuchu_searchObject()) {
            switch (party->useMotionId) {
                case 1:
                    if (--party->moveFrames <= 0) {
                        partyChgPoseId(party, 1);
                        party->moveFrames = 30;
                        party->useMotionId = 2;
                    }
                    break;
                case 2:
                    if (--party->moveFrames <= 0) {
                        party->useMotionId = 0;
                    }
                    break;
                case 0:
                default:
                    partyPoseA2(party);
                    break;
            }
        }
    }
}

void partyWalkChkGnd(PartyEntry* party) {
    Vec pos;
    f32 height;
    void* head;
    HitEntry* hit;
    s32 attr;

    pos = party->position;
    height = party->height;
    head = partySearchHead(party, &pos, &height, party->displayDirection);
    if (head) {
        party->hitobjHead = head;
    }
    if (party->flags & PARTY_FLAG_DISABLE_FLOORS) {
        return;
    }
    party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
    hit = partySearchGround(party, 15.0f, -15.0f);
    if (hit) {
        attr = hitGetAttr(hit);
        if (attr & HIT_ATTR_WATER_HAZARD) {
            hit = NULL;
            if (!party->splashTimer) {
                party->trigFlags |= PARTY_TRIG_SPLASH;
                party->flags |= PARTY_FLAG_FORCE_FALL;
            }
        }
    }
    party->hitobjStandOn = hit;
    if (!hit) {
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        partyChgMot(party, 3);
        party->gravity = -1.5f;
        return;
    }
    party->position.y = party->groundY;
    party->flags |= PARTY_FLAG_IS_GROUNDED;
    if (attr & HIT_ATTR_SHALLOW_WATER) {
        party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
    } else {
        party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
    }
    if (attr & HIT_ATTR_DAMAGE_RETURN) {
        party->damageHit = hit;
    }
    if (partyChkNoStandMobj(hit)) {
        partyGetAppearPos5(party, hit, &party->moveTarget);
        party->trigFlags |= PARTY_TRIG_JUMP_TARGET_SET;
        party->moveTarget.y = party->position.y;
        partyChgMot(party, 2);
    }
}

static inline BOOL partyChkMarioFollowY(MarioWork* mario) {
    if (mario->airTimer != 0 && mario->baseSpeed == 0.0f) {
        return FALSE;
    }
    if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE || mario->currMotionId == MARIO_MOTION_HIP ||
        mario->currMotionId == MARIO_MOTION_HIP_2) {
        return FALSE;
    }
    return TRUE;
}

#ifdef VERSION_G8ME01
static inline void partyFlyMoveY(PartyEntry* party, f32 target, f32 rate) {
    f32 add;
    f32 diff;

    add = 5.0f;
    if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
        add = 1.0f;
    }
    diff = target + add - party->position.y;
    if (-5.0f > diff || diff >= 0.0f) {
        party->position.y += diff * rate;
    }
}

// US: on las_10 the flying partner is pushed down out of low ceilings
static inline void partyFlyHeadCheck(PartyEntry* party) {
    Vec pos;
    f32 height;
    void* head;
    f32 diff;
    f32 rate;

    if (!strcmp(gp->currentMapName, "las_10")) {
        pos = party->position;
        height = party->height;
        head = partySearchHead(party, &pos, &height, party->displayDirection);
        if (head) {
            party->hitobjHead = head;
            diff = pos.y - party->height - party->position.y;
            if (-5.0f > diff || diff >= 0.0f) {
                rate = 0.1f;
                party->position.y += diff * rate;
            }
        }
    }
}

#endif
void partyFlyAdjustY(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    HitEntry* hit;
    s32 attr;
    f32 target;
    f32 rate;
#ifndef VERSION_G8ME01
    f32 diff;
    f32 add;
#endif

    if (party->flags & PARTY_FLAG_DISABLE_FLOORS) {
        return;
    }
#ifdef VERSION_G8ME01
    partyFlyHeadCheck(party);
#endif
    hit = partySearchGround(party, 10.0f, 0.0f);
    if (hit) {
        attr = hitGetAttr(hit);
        if (attr & HIT_ATTR_TOGE) {
            partyChgRunMode(party, 0xD);
            return;
        }
        if (attr & HIT_ATTR_SHALLOW_WATER) {
            party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
        } else {
            party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
        }
    }
    if (partyChkMarioFollowY(party->playerPtr)) {
#ifndef VERSION_G8ME01
        rate = 0.05f;
#endif
        if (hit) {
            if (mario->position.y > party->groundY) {
                target = mario->position.y;
                rate = 0.1f;
            } else {
                target = party->groundY;
                rate = 0.1f;
            }
            if (party->position.y < party->groundY) {
#ifdef VERSION_G8ME01
                target = party->groundY;
                rate = 0.2f;
            }
            partyFlyMoveY(party, target, rate);
        } else if (party->dropDepth >= 0.0f) {
            if (mario->position.y > party->groundY) {
                target = mario->position.y;
                rate = 0.1f;
            } else {
                target = party->groundY;
                rate = 0.1f;
            }
            if (party->position.y < party->groundY) {
                target = party->groundY;
                rate = 0.2f;
            }
            partyFlyMoveY(party, target, rate);
        }
    } else {
        f32 t;
        rate = 0.05f;
        if (hit) {
            t = mario->position.y;
            if (t > party->groundY) {
                t = mario->position.y;
            } else {
                t = party->groundY;
            }
            if (party->position.y < party->groundY) {
                t = 18.5f + party->groundY;
                rate = 0.2f;
            }
            partyFlyMoveY(party, t, rate);
#else
                target = 18.5f + party->groundY;
                rate = 0.1f;
            }
        } else {
            target = mario->position.y;
        }
        add = 5.0f;
        if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
            add = 1.0f;
        }
        target += add;
        diff = target - party->position.y;
        if (diff > 5.0f || 0.0f > diff) {
            party->position.y += diff * rate;
        }
    } else {
        if (hit) {
            if (mario->position.y > party->groundY) {
                target = mario->position.y;
            } else {
                target = party->groundY;
            }
            if (party->position.y < party->groundY) {
                target = 18.5f + party->groundY;
            }
        } else {
            target = party->position.y;
        }
        add = 5.0f;
        if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
            add = 1.0f;
        }
        target += add;
        diff = target - party->position.y;
        if (diff > 5.0f || 0.0f > diff) {
            rate = 0.05f;
            party->position.y += diff * rate;
#endif
        }
    }
}


s32 marioGetSlitFloorPos(Vec* pos);

void partyGetReadId2(s32 id, Vec* pos);
void L_getTargetPos(s32 id, Vec* pos);

void party_motion_homing_walk(PartyEntry* party) {
    HitEntry* hit = NULL;
    s32 id;
    MarioWork* mario;
    PartyEntry* other;
    s32 i;
    Footmark* fm;
    u32 attr;
    f32 lim;
    f32 lim2;
    f32 spd;
    f32 px, py, pz;
    f32 d3;
    f32 a;
    f32 gy;
    f32 md;
    Vec v;
    Vec t;
    Vec tp;
    Vec hp;
    Vec p2;
    Vec p3;
    f32 sp;
    f32 dir;
    f32 s4;
    f32 c4;
    f32 rx, ry, rz;
    f32 fsy, fsa, fsb;
    f32 hh;

    id = partyGetFootmarkId(party);
    mario = party->playerPtr;
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->flags &= ~PARTY_FLAG_IS_BEHIND_MARIO;
    }
    if (party->forceMoveFrames != 0) {
        lim = 28.0f;
    } else {
        lim = VERSION_PICK(37.5f, 45.0f);
    }
    if (!strcmp(gp->currentMapName, "aji_02") || !strcmp(gp->currentMapName, "aji_05") ||
        !strcmp(gp->currentMapName, "aji_12")) {
        if (!marioKeyOffChk()) {
            lim = partyStayDist(party);
            party->trigFlags |= PARTY_TRIG_FOLLOW_CLOSE;
        }
    }
    if (partyGetTargetMovespd(id)) {
        lim2 = 50.0f;
    } else {
        lim2 = VERSION_PICK(35.0f, 45.0f);
    }
    if ((s32)mario->characterId == CHARACTER_BOWSER) {
        lim2 = 70.0f;
    }
    if ((party->trigFlags >> 17) & 1) {
        lim2 = partyStayDist(party);
    }
    if (!party_slit(party)) {
        return;
    }
    if (!party_roll(party)) {
        return;
    }
    if (mario->currMotionId == MARIO_MOTION_TALK && !(party->flags & PARTY_FLAG_IS_BEHIND_MARIO)) {
        partyMoveBehindMario(party, 0);
        return;
    }
    if (!partyChkMarioFollowY(party->playerPtr)) {
        partyChgMot(party, 0);
        return;
    }
    spd = partyGetTargetMovespd2(party);
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        d3 = PSVECDistance(&mario->position, &party->position);
        if (d3 < VERSION_PICK(35.0f, 45.0f)) {
            spd = 1.0f;
        } else if (d3 >= 50.0f) {
            spd = 3.0f;
        }
        if (party->playerPtr->flags & MARIO_FLAG_IS_PUSHING) {
            spd = 1.0f;
            lim = 100.0f;
        }
        partySetTargetSpd(party, spd);
    } else if ((other = partyGetPtr(marioGetPartyId()))) {
        v = other->position;
        if (nokonokoGetStatus(other) == 3 || PSVECDistance(&mario->position, &other->position) >= 150.0f) {
            d3 = PSVECDistance(&mario->position, &party->position);
            if (d3 < VERSION_PICK(35.0f, 45.0f)) {
                spd = 1.0f;
            } else if (d3 >= 50.0f) {
                spd = 3.0f;
            }
            if (party->playerPtr->flags & MARIO_FLAG_IS_PUSHING) {
                spd = 1.0f;
            }
            partySetTargetSpd(party, spd);
        } else {
            d3 = PSVECDistance(&other->position, &party->position);
            if (d3 < VERSION_PICK(35.0f, 45.0f)) {
                spd = 1.0f;
            } else if (d3 >= 50.0f) {
                spd = 3.0f;
            }
            if (party->playerPtr->flags & MARIO_FLAG_IS_PUSHING) {
                spd = 1.0f;
            }
            partySetTargetSpd(party, spd);
        }
    }
    px = party->position.x;
    i = 0;
    py = party->position.y;
    pz = party->position.z;
    do {
        fm = partyGetFootmarkPtr(id);
        t = fm->pos;
        dir = angleABf(px, pz, t.x, t.z);
        attr = partyGetFrontAttr(party, dir);
        if (!(attr & 0x800)) {
            goto found;
        }
        partyNextReadId(id);
        partyGetReadId2(id, &t);
    } while (++i <= 10);
    t = marioGetPtr()->position;
found:
    p2 = party->position;
    if (partyGetTargetDist3D(id, &p2) >= lim) {
        partySetTargetSpd(party, spd);
    }
    if (lim2 < 20.0f) {
        partySetTargetSpd(party, spd);
    }
    while (TRUE) {
        d3 = distABf(px, pz, t.x, t.z);
        dir = angleABf(px, pz, t.x, t.z);
        if (mario->flags & MARIO_FLAG_IS_PUSHING) {
            a = party->width + mario->wPlayerCollisionBox.x;
        } else {
            a = 0.5f * party->width + party->moveSpeed;
        }
        if (d3 > a) {
            p3 = (Vec){px, py, pz};
            d3 = partyGetTargetDist3D(id, &p3);
            if (d3 >= lim) {
                if (party->trigFlags & PARTY_TRIG_FOLLOW_CLOSE) {
                    a = 0.1f * (d3 - lim2);
                    if (a > 4.0f) {
                        a = 4.0f;
                    }
                    if (a < 0.5f) {
                        a = 0.0f;
                    }
                    partyForceSetSpd(party, mario->baseSpeed + a);
                }
#ifdef VERSION_G8ME01
                else {
                    // US: catch up with the speed of Mario when far behind
                    a = 0.1f * (d3 - lim);
                    if (a > 4.0f) {
                        a = 4.0f;
                    }
                    if (a < 0.5f) {
                        a = 0.0f;
                    }
                    partyForceSetSpd(party, mario->baseSpeed + a);
                }
#endif
                break;
            }
            if (marioGetSlitFloorPos(&v)) {
                partySetTargetSpd(party, 1.5f);
                partyNextReadId(id);
                if (marioGetSlitFloorPos(&v)) {
                    t = v;
                    py = distABf(px, pz, t.x, t.z);
                    dir = angleABf(px, pz, t.x, t.z);
                    if (py <= 5.0f) {
                        ((void (*)(PartyEntry*))partyChgMotSlitFloor)(party);
                    }
                }
                break;
            }
            if (d3 > lim2) {
                if (party->trigFlags & PARTY_TRIG_FOLLOW_CLOSE) {
                    a = 0.1f * (d3 - lim2);
                    if (a > 4.0f) {
                        a = 4.0f;
                    }
                    if (a < 0.5f) {
                        a = 0.0f;
                    }
                    partyForceSetSpd(party, mario->baseSpeed + a);
                } else {
                    a = d3 - lim2;
                    if (a >= 1.0f) {
                        a = 1.0f;
                        if (lim2 < 20.0f) {
                            a = spd;
                        }
                    }
                    partySetTargetSpd(party, a);
                }
                break;
            } else if (party->pushVelX == 0.0f && party->pushVelZ == 0.0f) {
                partySetTargetSpd(party, party->playerPtr->baseSpeed);
            } else {
                partyForceSetSpd(party, 0.0f);
                party->targetDir = angleABf(party->position.x, party->position.z, mario->position.x, mario->position.z);
                partyChgPoseId(party, 1);
            }
        }
        if (partyGetFootmarkReadId(id) == (i = partyGetFootmarkWriteId(id))) {
            partySetTargetSpd(party, party->playerPtr->baseSpeed);
            if (party->playerPtr->baseSpeed == 0.0f || party->pushVelX != 0.0f || party->pushVelZ != 0.0f) {
                dir = party->targetDir;
                partySetTargetSpd(party, 0.0f);
                party->gravity = 0.0f;
                if (!(party->flags & PARTY_FLAG_KEEP_POSE_UNUSED)) {
                    partyChgPoseId(party, 1);
                }
                partyChgMot(party, 0);
            }
            break;
        }
        partyNextReadId(id);
        partyGetReadId2(id, &t);
        if (!(party->flags & PARTY_FLAG_DISABLE_FLOORS)) {
            party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
            hit = partySearchGround(party, 10.0f, 0.0f);
            if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
                hit = NULL;
                if (!party->splashTimer) {
                    party->trigFlags |= PARTY_TRIG_SPLASH;
                    party->flags |= PARTY_FLAG_FORCE_FALL;
                }
            }
            party->hitobjStandOn = hit;
            if (!hit) {
                partyChgMot(party, 3);
                party->gravity = -1.0f;
            } else {
                attr = hitGetAttr(hit);
                if (attr & 0x100) {
                    party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
                } else {
                    party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
                }
                if (attr & 0x800) {
                    partyChgMot(party, 0);
                    break;
                }
            }
        }
        if ((party->flags & PARTY_FLAG_IS_GROUNDED) && (fm->flag & FOOTMARK_FLAG_JUMP)) {
            md = toMovedir(party->displayDirectionTarget);
            partyGetReadId(id, &rx, &ry, &rz);
            L_getTargetPos(id, &tp);
            if (tp.y < ry) {
                ry = tp.y;
            }
            party->moveTarget.x = rx;
            party->moveTarget.z = rz;
            party->moveTarget.y = ry;
            gy = partyGetDirGndY(party->moveTarget, md);
            if (gy > party->position.y) {
                dir = partyToMovedir(party, party->displayDirectionTarget);
                sincosf(&s4, &c4, dir);
                { f32 k = 40.0f;
                party->moveTarget.x = s4 * k;
                party->moveTarget.z = c4 * k; }
                party->moveTarget.y = gy;
            }
            partyChgMot(party, 2);
            break;
        }
        if (partyChkNoStandMobj(hit)) {
            partyGetAppearPos5(party, hit, &party->moveTarget);
            party->trigFlags |= PARTY_TRIG_JUMP_TARGET_SET;
            partyChgMot(party, 2);
            break;
        }
        if ((party->flags & PARTY_FLAG_IS_GROUNDED) && (fm->flag & FOOTMARK_FLAG_SLIT)) {
            if (distABf(px, pz, t.x, t.z) <= 5.0f) {
                ((void (*)(PartyEntry*))partyChgMotSlitFloor)(party);
            }
            break;
        }
    }
    if (party->moveSpeed) {
        partyChgWalkPose(party, party->moveSpeed);
    } else if (!(fm->flag & FOOTMARK_FLAG_SLIT)) {
        partyChgMot(party, 0);
    }
    party->targetDir = dir;
    v = party->position;
    partyGetMoveDirSpd(party, &dir, &sp);
    hit = partySearchFrontWall(party, &v, sp, dir);
    if (party->flags & PARTY_FLAG_DISABLE_FRONT_WALL_CHK) {
        hit = NULL;
    }
    if (!hit) {
        party->wallStuckFrames = 0;
        if (!(party->trigFlags & PARTY_TRIG_DOOR_MOVE)) {
            getFrontFloor(party, &t, party->targetDir);
        } else {
            party->homeDir = revise360(270.0f + mario->targetYaw);
            partyGetAppearPos4(party, &t);
        }
        if (t.y > party->position.y) {
            a = t.y - party->position.y;
            if (a > 12.0 && a < 60.0f) {
                party->moveTarget = t;
                partyChgMot(party, 2);
                return;
            }
            if (t.y > party->position.y) {
                party->position.y = t.y;
            }
        }
        if (party->moveSpeed) {
            attr = partyGetFrontAttrFar(party);
            if (!(attr & 0x200)) {
                v.x = party->position.x;
                v.z = party->position.z;
                movePos(&v.x, &v.z, party->moveSpeed, party->targetDir);
            }
        }
    } else {
        party->hitobjFront = hit;
        if (party->moveSpeed) {
            party->hitobjPush = hit;
            if (hitGetAttr(hit) & HIT_ATTR_SLIT) {
                if (party->slitOn != 1) {
                    if (!(party->playerPtr->trigFlags & MARIO_TRIG_FLAG_NORMALIZING)) {
                        party->trigFlags |= PARTY_TRIG_SLIT_INIT;
                        party->slitOn = 1;
                    }
                } else if (party->moveSpeed) {
                    v.x = party->position.x;
                    v.z = party->position.z;
                    movePos(&v.x, &v.z, party->moveSpeed, party->targetDir);
                }
            } else {
                party->wallStuckFrames++;
                if (party->wallStuckFrames > 8) {
                    party->wallStuckFrames = 0;
                    if (partyGetTargetDistY(id, party->position.y) >= 1.0f) {
                        getFrontFloor(party, &t, party->targetDir);
                        if (t.y - party->position.y > 5.0f) {
                            party->moveTarget = t;
                            partyChgMot(party, 2);
                            return;
                        }
                        if (!(party->flags & (PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING))) {
                            partyChkFrontStep(party, 11.0f, &fsy, &fsa, &fsb);
                            if (fsy - party->position.y >= 5.0f && party->groundSlope <= 30.0f) {
                                party->stepY = fsy;
                                if (party->moveSpeed) {
                                    partyChgMot(party, 6);
                                    return;
                                }
                            }
                        }
                    } else {
                        dir = partyGetTargetDir(id, party->position.x, party->position.z);
                        getFrontFloor(party, &t, dir);
                        a = t.y - party->position.y;
                        if (a > 100.0f) {
                            party->position = t;
                            partyChgMot(party, 1);
                        } else if (a >= 0.0f) {
                            party->moveTarget = t;
                            partyChgMot(party, 2);
                        } else {
                            partyChgMot(party, 0);
                        }
                    }
                }
            }
        }
    }
    party->position.x = v.x;
    party->position.z = v.z;
    hh = party->height;
    hp = party->position;
    hit = partySearchHead(party, &hp, &hh, party->displayDirection);
    if (hit) {
        party->hitobjHead = hit;
    }
    party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
    hit = partySearchGround(party, 10.0f, 10.0f);
    v.y = party->groundY;
    if (hit) {
        attr = hitGetAttr(hit);
        if (attr & 0x200) {
            hit = NULL;
            if (!party->splashTimer) {
                party->trigFlags |= PARTY_TRIG_SPLASH;
                party->flags |= PARTY_FLAG_FORCE_FALL;
            }
        } else if (attr & 0x800) {
            partyChgRunMode(party, 13);
            return;
        }
    }
    party->hitobjStandOn = hit;
    if (!hit) {
        partyChgMot(party, 3);
        party->gravity = -1.0f;
    } else {
        if (hitGetAttr(hit) & HIT_ATTR_SHALLOW_WATER) {
            party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
        } else {
            party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
        }
        if (hitGetAttr(hit) & HIT_ATTR_DAMAGE_RETURN) {
            party->damageHit = hit;
        }
    }
    party->position.y = v.y;
    party_force_reset_outofscreen(party);
    partyGorFix(party);
}

void party_motion_homing_fly(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    s32 id;
    s32 near;
    s32 w;
    HitEntry* hit;
    MarioWork* m2;
    Footmark* fm;
    u32 attr;
    f32 lim;
    f32 lim2;
    f32 spd;
    f32 px, py, pz;
    f32 d3;
    f32 dd;
    f32 a;
    f32 off;
    f32 gy;
    Vec v;
    Vec s2;
    Vec ap;
    Vec ff;
#ifdef VERSION_G8ME01
    Vec hp;
#endif
    Vec p2;
    Vec p3;
    f32 dir, sp;
    f32 tx, ty, tz;
    f32 x, z;
#ifdef VERSION_G8ME01
    f32 hh;
    f32 hd;
    void* head;
#endif

    id = partyGetFootmarkId(party);
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->flags &= ~PARTY_FLAG_IS_BEHIND_MARIO;
        party->noFloorFrames = 0;
    }
    if (party->forceMoveFrames != 0) {
        lim = 28.0f;
    } else {
        lim = VERSION_PICK(32.0f, 45.0f);
    }
    if (!strcmp(gp->currentMapName, "aji_02") || !strcmp(gp->currentMapName, "aji_05") ||
        !strcmp(gp->currentMapName, "aji_12")) {
        if (!marioKeyOffChk()) {
            lim = partyStayDist(party);
            party->trigFlags |= PARTY_TRIG_FOLLOW_CLOSE;
        }
    }
    if (partyGetTargetMovespd(id)) {
        lim2 = 50.0f;
    } else {
        lim2 = VERSION_PICK(35.0f, 45.0f);
    }
    if ((party->trigFlags >> 17) & 1) {
        lim2 = partyStayDist(party);
    }
    if (!party_slit(party)) {
        return;
    }
    if (!party_roll(party)) {
        return;
    }
    if (mario->currMotionId == MARIO_MOTION_TALK && !(party->flags & PARTY_FLAG_IS_BEHIND_MARIO)) {
        partyMoveBehindMario(party, 0);
        return;
    }
    if (!partyChkMarioFollowY(party->playerPtr)) {
        partyChgMot(party, 0);
        return;
    }
    spd = partyGetTargetMovespd2(party);
    d3 = PSVECDistance(&mario->position, &party->position);
    if (d3 < VERSION_PICK(35.0f, 45.0f)) {
        spd = 1.0f;
    } else if (d3 >= 50.0f) {
        spd = 3.0f;
    }
    if (party->playerPtr->flags & MARIO_FLAG_IS_PUSHING) {
        spd = 1.0f;
        lim = 100.0f;
    }
    partySetTargetSpd(party, spd);
    px = party->position.x;
    py = party->position.y;
    pz = party->position.z;
    fm = partyGetFootmarkPtr(id);
    tx = fm->pos.x;
    off = partyHomingOff(party);
    ty = fm->pos.y + off;
    tz = fm->pos.z;
    p2 = party->position;
    if (partyGetTargetDist3D(id, &p2) >= lim) {
        partySetTargetSpd(party, spd);
    }
    if (lim2 < 20.0f) {
        partySetTargetSpd(party, spd);
    }
    near = 0;
    while (TRUE) {
        dir = angleABf(px, pz, tx, tz);
        dd = distABf(px, pz, tx, tz);
        if (mario->flags & MARIO_FLAG_IS_PUSHING) {
            a = party->width + mario->wPlayerCollisionBox.x;
        } else {
            a = 0.5f * party->width + party->moveSpeed;
        }
        if (dd > a) {
            p3 = (Vec){px, py, pz};
            d3 = partyGetTargetDist3D(id, &p3);
            if (d3 >= lim) {
                if (party->trigFlags & PARTY_TRIG_FOLLOW_CLOSE) {
                    a = 0.1f * (d3 - lim2);
                    if (a > 4.0f) {
                        a = 4.0f;
                    }
                    if (a < 0.5f) {
                        a = 0.0f;
                    }
                    partyForceSetSpd(party, mario->baseSpeed + a);
                }
#ifdef VERSION_G8ME01
                else {
                    // US: catch up with the speed of Mario when far behind
                    a = 0.1f * (d3 - lim);
                    if (a > 4.0f) {
                        a = 4.0f;
                    }
                    if (a < 0.5f) {
                        a = 0.0f;
                    }
                    partyForceSetSpd(party, mario->baseSpeed + a);
                }
#endif
                near = 1;
                break;
            }
            if (marioGetSlitFloorPos(&v)) {
                partySetTargetSpd(party, 1.5f);
                partyNextReadId(id);
                if (marioGetSlitFloorPos(&v)) {
                    s2 = v;
                    py = distABf(px, pz, s2.x, s2.z);
                    dir = angleABf(px, pz, s2.x, s2.z);
                    if (py <= 5.0f) {
                        ((void (*)(PartyEntry*))partyChgMotSlitFloor)(party);
                    }
                }
                break;
            }
            if (d3 > lim2) {
                if (party->trigFlags & PARTY_TRIG_FOLLOW_CLOSE) {
                    a = 0.1f * (d3 - lim2);
                    if (a > 4.0f) {
                        a = 4.0f;
                    }
                    if (a < 0.5f) {
                        a = 0.0f;
                    }
                    partyForceSetSpd(party, mario->baseSpeed + a);
                } else {
                    a = d3 - lim2;
                    if (a >= 1.0f) {
                        a = 1.0f;
                        if (lim2 < 20.0f) {
                            a = spd;
                        }
                    }
                    partySetTargetSpd(party, a);
                    break;
                }
            } else if (party->pushVelX == 0.0f && party->pushVelZ == 0.0f) {
                partySetTargetSpd(party, party->playerPtr->baseSpeed);
            } else {
                partyForceSetSpd(party, 0.0f);
                party->targetDir = angleABf(party->position.x, party->position.z, mario->position.x, mario->position.z);
                partyChgPoseId(party, 1);
            }
        }
        if (partyGetFootmarkReadId(id) == (w = partyGetFootmarkWriteId(id))) {
            partySetTargetSpd(party, party->playerPtr->baseSpeed);
            if (party->playerPtr->baseSpeed == 0.0f || party->pushVelX != 0.0f || party->pushVelZ != 0.0f) {
                dir = party->targetDir;
                partySetTargetSpd(party, 0.0f);
                party->gravity = 0.0f;
                if (!(party->flags & PARTY_FLAG_KEEP_POSE_UNUSED)) {
                    partyChgPoseId(party, 1);
                }
                partyChgMot(party, 0);
            }
            break;
        }
        partyNextReadId(id);
        partyGetReadId(id, &tx, &ty, &tz);
        if (party->flags & PARTY_FLAG_DISABLE_FLOORS) {
            continue;
        }
        party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
        hit = partySearchGround(party, 100.0f, 100.0f);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
            hit = NULL;
        }
        party->hitobjStandOn = hit;
        if (!hit) {
#ifndef VERSION_G8ME01
            partyChgMot(party, 3);
            party->gravity = -1.0f;
#endif
            continue;
        }
        attr = hitGetAttr(hit);
        if (attr & HIT_ATTR_SHALLOW_WATER) {
            party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
        } else {
            party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
        }
        if (attr & HIT_ATTR_TOGE) {
            partyChgMot(party, 0);
            break;
        }
    }
    if (party->moveSpeed) {
        partyChgWalkPose(party, party->moveSpeed);
    } else {
        partyChgMot(party, 0);
    }
    if (party->noFloorFrames > 10) {
        dir = angleABf(party->position.x, party->position.z, mario->position.x, mario->position.z);
    }
    if (near) {
        m2 = party->playerPtr;
        partyGetAppearPos3(party, &ap);
        dir = angleABf(party->position.x, party->position.z, ap.x, ap.z);
        hit = partySearchGround(party, 10.0f, 0.0f);
        ap.y = party->groundY;
        if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
            hit = NULL;
        }
        if (hit) {
            party->position.y = ap.y;
        } else {
            off = partyHomingOff(party);
            party->position.y += 0.05f * ((m2->position.y + off) - party->position.y);
        }
    }
    party->targetDir = dir;
    v = party->position;
    partyGetMoveDirSpd(party, &dir, &sp);
    hit = partySearchFrontWall(party, &v, sp, dir);
    if (party->flags & PARTY_FLAG_DISABLE_FRONT_WALL_CHK) {
        hit = NULL;
    }
    if (!hit) {
        if (!(party->trigFlags & PARTY_TRIG_DOOR_MOVE)) {
            getFrontFloor(party, &ff, party->targetDir);
        } else {
            party->homeDir = revise360(270.0f + mario->targetYaw);
            partyGetAppearPos4(party, &ff);
        }
        if (ff.y >= mario->position.y) {
            off = partyHomingOff(party);
            party->position.y += 0.05f * ((ff.y + off) - party->position.y);
        }
        party->noFloorFrames = 0;
        if (party->moveSpeed) {
            v.x = party->position.x;
            v.z = party->position.z;
            movePos(&v.x, &v.z, party->moveSpeed, party->targetDir);
        }
    } else {
        party->hitobjFront = hit;
        if (party->moveSpeed) {
            party->hitobjPush = hit;
            if (hitGetAttr(hit) & HIT_ATTR_SLIT) {
                if (party->slitOn != 1) {
                    if (!(party->playerPtr->trigFlags & MARIO_TRIG_FLAG_NORMALIZING)) {
                        party->trigFlags |= PARTY_TRIG_SLIT_INIT;
                        party->slitOn = 1;
                    }
                } else if (party->moveSpeed) {
                    v.x = party->position.x;
                    v.z = party->position.z;
                    movePos(&v.x, &v.z, party->moveSpeed, party->targetDir);
                }
            } else {
                x = party->position.x;
                gy = party->position.y;
                z = party->position.z;
                movePos(&x, &z, party->width, dir);
                gy = partyGetGndY(x, gy, z);
                if (gy > -2000.0f) {
                    party->position.y += 0.05f * ((gy + party->height) - party->position.y);
                } else {
                    party->noFloorFrames++;
                    if (partyGetTargetDistY(id, party->position.y) >= 1.0f &&
                        __fabsf(mario->position.y - party->position.y) >= 10.0f) {
                        party->position.y = party->position.y + 0.1f * ((10.0f + mario->position.y) - party->position.y);
                        party->position.x = v.x;
                        party->position.z = v.z;
                        return;
                    }
                }
            }
        }
    }
    party->position.x = v.x;
    party->position.z = v.z;
#ifdef VERSION_G8ME01
    // US: on las_10 the partner is pushed down out of low ceilings
    if (!strcmp(gp->currentMapName, "las_10")) {
        hp = party->position;
        hh = party->height;
        head = partySearchHead(party, &hp, &hh, party->displayDirection);
        if (head) {
            party->hitobjHead = head;
            hd = hp.y - party->height - party->position.y;
            if (-5.0f > hd || hd >= 0.0f) {
                a = 0.1f;
                party->position.y += hd * a;
            }
        }
    }
#endif
    party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
    hit = partySearchGround(party, 10.0f, 10.0f);
    if (hit) {
        attr = hitGetAttr(hit);
        if (attr & HIT_ATTR_TOGE) {
            partyChgRunMode(party, 13);
            return;
        }
        if (attr & HIT_ATTR_SHALLOW_WATER) {
            party->flags |= PARTY_FLAG_IN_SHALLOW_WATER;
        } else {
            party->flags &= ~PARTY_FLAG_IN_SHALLOW_WATER;
        }
    }
    party_force_reset_outofscreen(party);
}

void party_set_behind_dir(PartyEntry* party, f32 dir) {
    party->homeDir = revise360(dir + camGetPtr(CAMERA_3D)->viewYaw);
    party->trigFlags |= PARTY_TRIG_BEHIND_DIR_SET;
}

static inline f32 partyGetFollowDir(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    PartyEntry* other;
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        return toMovedir(mario->displayDirection);
    }
    other = partyGetPtr(marioGetPartyId());
    if (other) {
        return partyToMovedir(party, other->displayDirection);
    }
    return toMovedir(mario->displayDirection);
}


void party_motion_behind_mario(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    HitEntry* hit;
    f32 ang;
    f32 gy;
    f32 dist;
    f32 thr;
    f32 ty;
    f32 x, z;
    Vec target;

    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->trigFlags |= PARTY_TRIG_HALF_PUSH;
        if (!(party->trigFlags & PARTY_TRIG_BEHIND_DIR_SET)) {
            {
            f32 t;
            if (!(party->trigFlags & PARTY_TRIG_HOME_DIR_FROM_MOVE_UNUSED)) {
                t = partyGetFollowDir(party);
                t += 180.0f;
                t = revise360(t);
            } else {
                party->trigFlags &= ~PARTY_TRIG_HOME_DIR_FROM_MOVE_UNUSED;
                t = revise360(180.0f + angleABf(mario->position.x, mario->position.z, party->moveTarget.x, party->moveTarget.z));
            }
            party->homeDir = t;
            }
        }
        partyGetAppearPos4(party, &target);
        party->moveFrames = distABf(party->position.x, party->position.z, target.x, target.z);
        if (party->moveFrames <= 0) {
            party->moveFrames = 1;
        }
    }
    if (!(party->trigFlags & PARTY_TRIG_BEHIND_DIR_SET) && marioKeyOffChk()) {
        {
            f32 t2 = partyGetFollowDir(party);
            t2 += 180.0f;
            party->homeDir = revise360(t2);
        }
    }
    partyGetAppearPos4(party, &target);
    x = target.x;
    z = target.z;
    ty = target.y;
    ang = angleABf(party->position.x, party->position.z, x, z);
    {
        f32 py = ty;
        f32 nz, ny, nx, dd, hz, hy, hx;
        py += touch_wall_ct;
        dd = 1000.0f;
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dd, &nx, &ny, &nz, x, py, z, 0.0f, -1.0f, 0.0f);
        if (!hit) {
            gy = -2000.0f;
        } else {
            gy = (s32)(100.0f * hy + 0.5f) / 100.0f;
            hitGetAttr(hit);
        }
    }
    if (gy <= -2000.0f) {
        gy = 1.5f * mario->wPlayerCollisionBox.x;
        movePos(&x, &z, gy, toMovedir(mario->displayDirection));
    }
    thr = 2.0f;
    if (mario->baseSpeed > 1.0f) {
        thr = 10.0f;
    }
    dist = distABf(party->position.x, party->position.z, x, z);
    party->moveFrames--;
    if (dist >= thr && party->moveFrames > 0) {
        party->targetDir = angleABf(party->position.x, party->position.z, x, z);
        distABf(party->position.x, party->position.z, x, z);
        partyForceSetSpd(party, (party->trigFlags & PARTY_TRIG_BEHIND_DIR_SET) ? 3.6f : 2.0f);
        partyMoveCheckWall(party);
        partyChgWalkPose(party, party->moveSpeed);
        if (party->hitobjFront) {
            x = party->position.x;
            z = party->position.z;
            movePos(&x, &z, dist, ang);
            angleABf(party->position.x, party->position.z, x, z);
            {
                f32 py = ty;
                f32 GG;
                f32 nz, ny, nx, dd, hz, hy, hx;
                py += touch_wall_ct;
                dd = 1000.0f;
                hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dd, &nx, &ny, &nz, x, py, z, 0.0f, -1.0f, 0.0f);
                if (!hit) {
                    GG = -2000.0f;
                } else {
                    GG = (s32)(100.0f * hy + 0.5f) / 100.0f;
                    hitGetAttr(hit);
                }
                thr = GG;
            }
            if (__fabsf(party->position.y - thr) > 1.0f) {
                party->moveTarget.x = x;
                party->moveTarget.y = thr;
                party->moveTarget.z = z;
                partyChgMot(party, 2);
                return;
            }
        } else if (ty > party->position.y) {
            x = party->position.x;
            z = party->position.z;
            movePos(&x, &z, dist, ang);
            angleABf(party->position.x, party->position.z, x, z);
            {
                f32 py = ty;
                f32 GG;
                f32 nz, ny, nx, dd, hz, hy, hx;
                py += touch_wall_ct;
                dd = 1000.0f;
                hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dd, &nx, &ny, &nz, x, py, z, 0.0f, -1.0f, 0.0f);
                if (!hit) {
                    GG = -2000.0f;
                } else {
                    GG = (s32)(100.0f * hy + 0.5f) / 100.0f;
                    hitGetAttr(hit);
                }
                thr = GG;
            }
            if (__fabsf(party->position.y - thr) > 1.0f) {
                party->moveTarget.x = x;
                party->moveTarget.y = thr;
                party->moveTarget.z = z;
                partyChgMot(party, 2);
                return;
            }
        }
        hit = partyBehindGround(party);
        party->hitobjStandOn = hit;
        if (!hit) {
            partyChgMot(party, 3);
            party->gravity = -1.0f;
        }
        party->position.y = party->groundY;
    } else {
        party->moveFrames = 0;
        if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
            party->trigFlags &= ~PARTY_TRIG_FOLLOW_CLOSE;
        }
        partyChgMoveMode(party, 1);
        partyChgMot(party, 0);
        partySetDir(party, mario->position.x, mario->position.z);
        party->homingDist = 50.0f;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags |= PARTY_FLAG_IS_BEHIND_MARIO;
        party->trigFlags &= ~PARTY_TRIG_BEHIND_DIR_SET;
        party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
#ifdef VERSION_G8ME01
        party->trigFlags &= ~PARTY_TRIG_DOOR_MOVE;
#endif
    }
    if (party->hitobjPush) {
        party->flags |= PARTY_FLAG_IS_BEHIND_MARIO;
        partyChgMoveMode(party, 1);
        partyChgMot(party, 0);
        partySetDir(party, x, z);
        party->homingDist = 50.0f;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
#ifdef VERSION_G8ME01
        party->trigFlags &= ~PARTY_TRIG_DOOR_MOVE;
#endif
    }
}

void party_motion_behind_mario_continue(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    PartyEntry* other;
    HitEntry* hit;
    f32 y;
    f32 gy;
    f32 sp;
    f32 thr;
    f32 x, z;

    {
        f32 t = partyGetFollowDir(party);
        t += 180.0f;
        party->homeDir = revise360(t);
    }
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        sp = 1.5f * mario->wPlayerCollisionBox.x;
        x = mario->position.x;
        z = mario->position.z;
        movePos(&x, &z, sp, party->homeDir);
        party->moveFrames = distABf(party->position.x, party->position.z, x, z);
        if (party->moveFrames <= 0) {
            party->moveFrames = 1;
        }
    }
    if (marioKeyOffChk()) {
        {
            f32 t = partyGetFollowDir(party);
            t += 180.0f;
            party->homeDir = revise360(t);
        }
    }
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        sp = 1.5f * mario->wPlayerCollisionBox.x;
        x = mario->position.x;
        z = mario->position.z;
        y = mario->position.y;
    } else {
        sp = 1.5f * mario->wPlayerCollisionBox.x;
        other = anotherPartyGetPtr(party->currentSlotId);
        if (other) {
            x = other->position.x;
            z = other->position.z;
            y = other->position.y;
        } else {
            x = mario->position.x;
            z = mario->position.z;
            y = mario->position.y;
        }
    }
    movePos(&x, &z, sp, party->homeDir);
    angleABf(party->position.x, party->position.z, x, z);
    {
        f32 py = y;
        f32 nz, ny, nx, dist, hz, hy, hx;
        py += touch_wall_ct;
        dist = 1000.0f;
        hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, py, z, 0.0f, -1.0f, 0.0f);
        if (!hit) {
            gy = -2000.0f;
        } else {
            gy = (s32)(100.0f * hy + 0.5f) / 100.0f;
            hitGetAttr(hit);
        }
    }
    if (gy <= -2000.0f) {
        sp = 1.5f * mario->wPlayerCollisionBox.x;
        movePos(&x, &z, sp, toMovedir(mario->displayDirection));
    }
    thr = 2.0f;
    if (mario->baseSpeed > 1.0f) {
        thr = 10.0f;
    }
    sp = distABf(party->position.x, party->position.z, x, z);
    party->moveFrames--;
    if (sp >= thr) {
        party->targetDir = angleABf(party->position.x, party->position.z, x, z);
        sp = distABf(party->position.x, party->position.z, x, z) / 10.0f;
        if (sp > 2.0f) {
            sp = 2.0f;
        }
        if (sp < 1.0f) {
            sp = 1.0f;
        }
        partyForceSetSpd(party, sp);
        partyMoveCheckWall(party);
        partyChgWalkPose(party, party->moveSpeed);
        hit = partyBehindGround(party);
        party->hitobjStandOn = hit;
        if (!hit) {
            partyChgMot(party, 3);
            party->gravity = -1.0f;
        }
        party->position.y = party->groundY;
    } else {
        party->moveFrames = 0;
        if (mario->baseSpeed == 0.0f) {
            party->flags |= PARTY_FLAG_IS_BEHIND_MARIO;
            partyChgMoveMode(party, 1);
            partyChgMot(party, 0);
            partySetDir(party, mario->position.x, mario->position.z);
            party->homingDist = 50.0f;
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        }
    }
    if (party->hitobjPush) {
        partyChgMoveMode(party, 1);
        partyChgMot(party, 0);
        partySetDir(party, x, z);
        party->homingDist = 50.0f;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    }
}

void party_motion_behind_mario_fly(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    HitEntry* hit;
    f32 thr;
    f32 spd;
    f32 dist;
    Vec pos;
    f32 sp, dir;

    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->trigFlags |= PARTY_TRIG_HALF_PUSH;
        if (!(party->trigFlags & PARTY_TRIG_BEHIND_DIR_SET)) {
            {
            f32 t;
            if (!(party->trigFlags & PARTY_TRIG_HOME_DIR_FROM_MOVE_UNUSED)) {
                t = partyGetFollowDir(party);
                t += 180.0f;
                t = revise360(t);
            } else {
                party->trigFlags &= ~PARTY_TRIG_HOME_DIR_FROM_MOVE_UNUSED;
                t = revise360(angleABf(mario->position.x, mario->position.z, party->moveTarget.x, party->moveTarget.z) + 180.0f);
            }
            party->homeDir = t;
            }
        }
        partyGetAppearPos4(party, &party->moveTarget);
        party->moveFrames = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        if (party->moveFrames <= 0) {
            party->moveFrames = 1;
        }
    }
    if (!(party->trigFlags & PARTY_TRIG_BEHIND_DIR_SET) && marioKeyOffChk()) {
        f32 t2 = partyGetFollowDir(party);
        t2 += 180.0f;
        party->homeDir = revise360(t2);
    }
    angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
    if (mario->baseSpeed > 1.0f) {
        thr = 6.0f;
    } else {
        thr = 2.0f;
    }
    dist = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
    party->moveFrames--;
    if ((dist >= thr || __fabsf(party->moveTarget.y - party->position.y) > 2.0f) && party->moveFrames > 0) {
        party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        if (party->trigFlags & PARTY_TRIG_BEHIND_DIR_SET) {
            spd = 3.6f;
        } else {
            spd = 2.0f;
        }
        if (dist < thr) {
            spd = 0.0f;
            partyForceSetSpd(party, spd);
        }
        partySetTargetSpd(party, spd);
        pos = party->position;
        partyGetMoveDirSpd(party, &dir, &sp);
        hit = partySearchFrontWall(party, &pos, sp, dir);
        if (hit) {
            party->hitobjFront = hit;
            if (party->moveSpeed) {
                party->hitobjPush = hit;
            }
        }
        party->position.x = pos.x;
        party->position.z = pos.z;
        party->position.y += 0.05f * (party->moveTarget.y - party->position.y);
        partyChgWalkPose(party, party->moveSpeed);
        return;
    }
    party->moveFrames = 0;
    if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
        party->trigFlags &= ~PARTY_TRIG_FOLLOW_CLOSE;
        if (party->moveKind == 1) {
            party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        }
    }
    partyChgMoveMode(party, 1);
    partyChgMot(party, 0);
    partySetDir(party, mario->position.x, mario->position.z);
    party->homingDist = 50.0f;
    party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    partyForceSetSpd(party, 0.0f);
    party->flags |= PARTY_FLAG_IS_BEHIND_MARIO;
    party->trigFlags &= ~PARTY_TRIG_BEHIND_DIR_SET;
    party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
#ifdef VERSION_G8ME01
    party->trigFlags &= ~PARTY_TRIG_DOOR_MOVE;
#endif
    partySetDispDirToMario(party);
    if (party->hitobjPush) {
        party->flags |= PARTY_FLAG_IS_BEHIND_MARIO;
        partyChgMoveMode(party, 1);
        partyChgMot(party, 0);
        partySetDir(party, mario->position.x, mario->position.z);
        party->homingDist = 50.0f;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->trigFlags &= ~PARTY_TRIG_HALF_PUSH;
    }
    hit = partySearchGround(party, 100.0f, 100.0f);
    if (hit) {
        if (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD) {
            hit = NULL;
            if (!party->splashTimer) {
                party->trigFlags |= PARTY_TRIG_SPLASH;
                party->flags |= PARTY_FLAG_FORCE_FALL;
            }
        }
        if (dist > party->height) {
            hit = NULL;
        }
        party->position.y += 0.05f * (party->groundY - party->position.y);
    }
    party->hitobjStandOn = hit;
}

void party_motion_beside_mario(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    HitEntry* hit;
    f32 x, z;
    f32 y;
    f32 thr;
    f32 sp;
    f32 t;

    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        t = partyGetFollowDir(party);
        t += 90.0f;
        party->homeDir = revise360(t);
        sp = 1.5f * mario->wPlayerCollisionBox.x;
        x = mario->position.x;
        z = mario->position.z;
        movePos(&x, &z, sp, party->homeDir);
        party->moveFrames = distABf(party->position.x, party->position.z, x, z);
        if (party->moveFrames <= 0) {
            party->moveFrames = 1;
        }
    }
    if (marioKeyOffChk()) {
        t = partyGetFollowDir(party);
        t += 90.0f;
        party->homeDir = revise360(t);
    }
    sp = 1.5f * mario->wPlayerCollisionBox.x;
    x = mario->position.x;
    z = mario->position.z;
    y = mario->position.y;
    movePos(&x, &z, sp, party->homeDir);
    angleABf(party->position.x, party->position.z, x, z);
    y = partyGetGndY(x, y, z);
    if (y <= -2000.0f) {
        sp = 1.5f * mario->wPlayerCollisionBox.x;
        x = mario->position.x;
        z = mario->position.z;
        movePos(&x, &z, sp, toMovedir(mario->displayDirection));
    }
    thr = 2.0f;
    if (mario->baseSpeed > 1.0f) {
        thr = 10.0f;
    }
    sp = distABf(party->position.x, party->position.z, x, z);
    party->moveFrames--;
    if (sp >= thr && party->moveFrames > 0) {
        party->targetDir = angleABf(party->position.x, party->position.z, x, z);
        sp = distABf(party->position.x, party->position.z, x, z) / 10.0f;
        if (sp > 2.0f) {
            sp = 2.0f;
        }
        if (sp < 1.0f) {
            sp = 1.0f;
        }
        partyForceSetSpd(party, sp);
        partyMoveCheckWall(party);
        partyChgWalkPose(party, party->moveSpeed);
        hit = partySearchGround(party, 10.0f, 0.0f);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
            hit = NULL;
            if (!party->splashTimer) {
                party->trigFlags |= PARTY_TRIG_SPLASH;
                party->flags |= PARTY_FLAG_FORCE_FALL;
            }
        }
        party->hitobjStandOn = hit;
        if (!hit) {
            partyChgMot(party, 3);
            party->gravity = -1.0f;
        }
        party->position.y = party->groundY;
    } else {
        party->moveFrames = 0;
        partyChgMoveMode(party, 1);
        partyChgMot(party, 0);
        partySetDir(party, mario->position.x, mario->position.z);
        party->homingDist = 50.0f;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
        party->flags |= PARTY_FLAG_IS_BEHIND_MARIO;
    }
    if (party->hitobjPush) {
        partyChgMoveMode(party, 1);
        partyChgMot(party, 0);
        partySetDir(party, mario->position.x, mario->position.z);
        party->homingDist = 50.0f;
        party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
    }
}

static inline void partyJumpWallMove(PartyEntry* party) {
    Vec v;
    HitEntry* hit;

    if (party->flags & PARTY_FLAG_DISABLE_FRONT_WALL_CHK) {
        movePos(&v.x, &v.z, party->moveSpeed, party->targetDir);
        return;
    }
    v = party->position;
    hit = partySearchFrontWall(party, &v, party->moveSpeed, party->targetDir);
    if (hit) {
        party->hitobjFront = hit;
        if (party->moveSpeed) {
            party->hitobjPush = hit;
        }
    }
    party->position.x = v.x;
    party->position.z = v.z;
}

static inline void partyJumpFrontMove(PartyEntry* party) {
    Vec v;
    HitEntry* hit;
    if (party->flags & PARTY_FLAG_DISABLE_FRONT_WALL_CHK) {
        movePos(&v.x, &v.z, party->moveSpeed, party->targetDir);
        return;
    }
    v = party->position;
    hit = partySearchFrontWall(party, &v, party->moveSpeed, party->targetDir);
    if (hit) {
        party->hitobjFront = hit;
        if (party->moveSpeed) {
            party->hitobjPush = hit;
        }
    }
    party->position.x = v.x;
    party->position.z = v.z;
}

static inline f32 partyJumpHeight(f32 dy) {
    if (__fabsf(dy) < 1.0f) {
        return 25.0f;
    } else if (dy >= 0.0f) {
        return dy;
    } else {
        return 10.0f;
    }
}

static inline void partyJumpCalc(PartyEntry* party, MarioWork* mario, f32 d) {
    f32 t;
    f32 dy;
    f32 g;
    s32 gz;
    party->moveFrames = (10.0f * (0.9f * (d / party->moveSpeed)) + 0.5f) / 10.0f;
    if (party->moveFrames < 12) {
        party->moveFrames = 12;
    }
    if (party->moveFrames > 40) {
        party->moveFrames = 40;
    }
    t = 0.5f * party->moveFrames;
    dy = partyJumpHeight(party->moveTarget.y - party->position.y);
    // FAKE: dead int-guarded re-definition (folded only by the post-RA peephole) keeps the moveDistance value d live
    // across the jump-height block so it takes f5 and dy f6 as in the target; behaviour-identical (gz is 0).
    gz = 0;
    if (gz != 0) {
        t = d;
    }
    g = (dy + dy) / (t * t - t);
    party->velY = mario->playerGravity * (g * t);
    if (party->velY > 13.0f) {
        party->velY = 13.0f;
    }
    if (party->velY < 3.0f) {
        party->velY = 3.0f;
    }
    party->gravity = -g * mario->playerGravity;
}

void party_motion_jump(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    s32 id;
    HitEntry* hit;
    f32 dir;
    f32 dist;
    f32 lim;
#ifdef VERSION_G8ME01
    f32 dist2;
#endif
    Vec fl;
    f32 s, c;
    f32 x, y, z;

    id = partyGetFootmarkId(party);
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->motionStep = 0;
    }
    switch (party->motionStep) {
    case 0:
        x = party->moveTarget.x;
        y = party->moveTarget.y;
        z = party->moveTarget.z;
        if (party->forceMoveFrames != 0 || party->runMode == 9) {
            x = party->position.x;
            y = party->position.y;
            z = party->position.z;
            movePos(&x, &z, party->moveSpeed, party->targetDir);
            party->moveTarget = (Vec){x, y, z};
        } else if (!(party->trigFlags & PARTY_TRIG_JUMP_TARGET_SET)) {
            if (partyGetFootmarkReadId(id) == partyGetFootmarkWriteId(id)) {
                if (partyGetTargetDist(id, party->position.x, party->position.z) > 2.0f * mario->wPlayerCollisionBox.x) {
                    dir = partyGetTargetDir(id, party->position.x, party->position.z);
                    getFrontFloor(party, &fl, dir);
                    fl.y += 10.0f;
                    y = fl.y;
                    x = party->position.x;
                    z = party->position.z;
                    movePos(&x, &z, 0.6f * party->width, dir);
                }
                party->moveTarget = (Vec){x, y, z};
            } else {
                partyGetReadId(id, &x, &y, &z);
                lim = 50.0f;
                dist = distABf(party->position.x, party->position.z, x, z);
#ifdef VERSION_G8ME01
                dist2 = __fabsf(y - party->position.y);
                if (50.0f > 1.4f * dist2) {
                    lim = 1.4f * dist2;
                }
#else
                if (50.0f > 1.4f * (y - party->position.y)) {
                    lim = 1.4f * (y - party->position.y);
                }
#endif
                sincosf(&s, &c, angleABf(party->position.x, party->position.z, x, z));
                if (dist > lim) {
                    dist = lim;
                    if (partyGetTargetMovespd(id) == 0.0f) {
                        dist = 0.0f;
                    }
                    x = party->position.x;
                    z = party->position.z;
                    x = x + dist * s;
                    z = z + dist * c;
                    y += 10.0f;
                    party->moveTarget = (Vec){x, y, z};
                } else {
                    if (partyGetTargetMovespd(id) == 0.0f) {
                        dist = 0.0f;
                    }
                    x = x + dist * s;
                    z = z + dist * c;
                    party->moveTarget = (Vec){x, y, z};
                }
                if (dist < lim) {
                    partyNextReadId(id);
                }
            }
        }
        party->moveDistance = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        if (party->forceMoveFrames == 0) {
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->moveSpeed = 2.25f * party->playerPtr->playerGravity;
        }
        partyJumpCalc(party, mario, party->moveDistance);
        party->flags |= PARTY_FLAG_IS_JUMPING;
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        partyChgPoseId(party, 4);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_JUMP1), &party->position);
        party->motionStep = 10;
        if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
            partyNextReadId(partyGetFootmarkId(party));
        }
        break;
    case 10:
        if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
            partyNextReadId(partyGetFootmarkId(party));
        }
        party->velY += party->gravity;
        party->position.y += party->velY;
        if (party->position.y < -2000.0f) {
            party->flags &= ~PARTY_FLAG_IS_JUMPING;
            party->velY = 0.0f;
            party->targetDir = angleABf(party->position.x, party->position.z, mario->position.x, mario->position.z);
            party->position = mario->position;
            partyChgMot(party, 0);
            break;
        }
        if (party->velY <= 0.0f) {
            party->flags &= ~PARTY_FLAG_IS_JUMPING;
            party->flags |= PARTY_FLAG_IS_FALLING;
            hit = partySearchGround(party, __fabsf(party->velY), 0.0f);
            if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
                if (!party->splashTimer) {
                    party->trigFlags |= PARTY_TRIG_SPLASH;
                    party->flags |= PARTY_FLAG_FORCE_FALL;
                }
                party->hitobjStandOn = NULL;
                partyChgMot(party, 3);
                party->gravity = -1.0f;
                break;
            }
            party->hitobjStandOn = hit;
            party->position.y = party->groundY;
            if (hit) {
                party->flags |= PARTY_FLAG_IS_GROUNDED;
#ifdef VERSION_G8ME01
                party->trigFlags &= ~PARTY_TRIG_JUMP_TARGET_SET;
#endif
                party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
                N_partyPlaySfxPartyLanding1(party);
                party->velY = 0.0f;
                partyChgMot(party, 0);
                party->trigFlags |= PARTY_TRIG_LANDED;
                break;
            }
        }
        partyJumpFrontMove(party);
        break;
    }
}
void party_motion_jump2(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    HitEntry* hit;
    Vec pos;
    f32 dist;
    f32 dy;
    f32 fr;
    s32 frames;

    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->moveFrames = 0;
        party->gravity = -1.0f * mario->playerGravity;
        party->moveSpeed = 2.25f * mario->playerGravity;
        party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        dist = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        if (dist > 50.0f) {
            dist = 50.0f;
        }
        party->moveDistance = dist;
        dy = party->moveTarget.y - party->position.y;
        frames = (10.0f * (dist / party->moveSpeed) + 0.5f) / 10.0f;
        if (frames < 10) {
            frames = 10;
        }
        fr = frames;
        party->moveFrames = frames;
        party->moveSpeed = mario->playerGravity * (party->moveDistance / fr);
        party->velY = (0.5f * (fr * (fr * __fabsf(party->gravity))) + dy) / fr;
        party->velY *= mario->playerGravity;
        if (party->velY > 20.0f) {
            party->velY = 20.0f;
        }
        if (party->velY < 0.0f) {
            party->velY = 0.0f;
        }
        partyChgPoseId(party, 4);
        party->flags |= PARTY_FLAG_IS_JUMPING;
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_JUMP1), &party->position);
        party->motionStep = 0;
    }
    switch (party->motionStep) {
        case 0:
            party->motionStep = 1;
            break;
        case 1:
            party->velY += party->gravity;
            party->position.y += party->velY;
            pos = party->position;
            partySearchFrontWall(party, &pos, party->moveSpeed, party->targetDir);
            party->position.x = pos.x;
            party->position.z = pos.z;
            if (party->velY <= 0.0f) {
                hit = partySearchGround(party, __fabsf(party->velY), 0.0f);
                pos.y = party->groundY;
                if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
                    if (!party->splashTimer) {
                        party->trigFlags |= PARTY_TRIG_SPLASH;
                        party->flags |= PARTY_FLAG_FORCE_FALL;
                    }
                    party->hitobjStandOn = NULL;
                    partyChgMot(party, 3);
                    party->gravity = -1.0f;
                    return;
                }
                party->position.y = pos.y;
                party->hitobjStandOn = hit;
                if (hit) {
                    N_partyPlaySfxPartyLanding1(party);
                    party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
                    party->flags |= PARTY_FLAG_IS_GROUNDED;
#ifdef VERSION_G8ME01
                    party->trigFlags &= ~PARTY_TRIG_JUMP_TARGET_SET;
#endif
                    party->velY = 0.0f;
                    partyChgMot(party, distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z) < 5.0f ? 0 : 1);
                }
            }
            break;
    }
    if (party->position.y < -2000.0f) {
        party->flags &= ~PARTY_FLAG_IS_JUMPING;
        party->velY = 0.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, mario->position.x, mario->position.z);
        party->position = mario->position;
        partyChgMot(party, 0);
    }
}

s32 party_motion_fall(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    HitEntry* hit;
    Vec pos;

    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        partyNextReadId(partyGetFootmarkId());
    }
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        partyChgPoseId(party, 4);
        party->flags &= ~PARTY_FLAG_IS_BEHIND_MARIO;
    }
    hit = NULL;
    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        hit = partySearchGround(party, 10.0f, __fabsf(party->velY));
    }
    if (hit && (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD)) {
        hit = NULL;
        if (party->moveKind == 0 && !party->splashTimer) {
            party->trigFlags |= PARTY_TRIG_SPLASH;
            party->flags |= PARTY_FLAG_FORCE_FALL;
        }
    }
    party->hitobjStandOn = hit;
    if (!hit) {
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        if (!(party->flags & PARTY_FLAG_IS_FALLING)) {
            party->flags |= PARTY_FLAG_IS_FALLING;
            if (party->gravity == 0.0f) {
                party->gravity = -1.4f;
            }
        }
        party->velY += party->gravity;
        if (party->velY < -16.0f) {
            party->velY = -16.0f;
        }
        party->position.y += party->velY;
#ifdef VERSION_G8ME01
        // US: with the stick released the drift of the partner slows down while falling
        if (!marioKeyOffChk()) {
            party->moveSpeed += -0.2f;
            if (party->moveSpeed < 0.0f) {
                party->moveSpeed = 0.0f;
            }
        }
#endif
        if (party->position.y < -2000.0f) {
            party->position.y = -2001.0f;
            if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
                return 2;
            }
            party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
            party->velY = 0.0f;
            party->gravity = 0.0f;
            party->targetDir = angleABf(party->position.x, party->position.z, mario->position.x, mario->position.z);
            party->homeDir = revise360(180.0f + toMovedir(mario->displayDirectionTarget));
            partyGetAppearPos4(party, &party->position);
            party->position.y += 37.0f;
            partyChgMot(party, 0);
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
        }
        if (party->moveSpeed) {
            pos = party->position;
            partySearchFrontWall(party, &pos, 1.3f * party->moveSpeed, party->targetDir);
            party->position.x = pos.x;
            party->position.z = pos.z;
        }
        return 2;
    }
    if (party->flags & PARTY_FLAG_IS_FALLING) {
        party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
        party->flags |= PARTY_FLAG_IS_GROUNDED;
#ifdef VERSION_G8ME01
        party->trigFlags &= ~PARTY_TRIG_JUMP_TARGET_SET;
#endif
        party->velY = 0.0f;
        party->position.y = party->groundY;
        if (party->moveSpeed == 0.0f) {
            partyChgMot(party, 0);
        } else {
            partyChgMot(party, 1);
        }
        N_partyPlaySfxPartyLanding1(party);
        return 3;
    }
    party->flags &= ~PARTY_FLAG_MOT_SLIT_FLOOR;
    party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
    party->flags |= PARTY_FLAG_IS_GROUNDED;
#ifdef VERSION_G8ME01
    party->trigFlags &= ~PARTY_TRIG_JUMP_TARGET_SET;
#endif
    party->velY = 0.0f;
    party->position.y = party->groundY;
    if (party->moveSpeed == 0.0f) {
        partyChgMot(party, 0);
    } else {
        partyChgMot(party, 1);
    }
    N_partyPlaySfxPartyLanding1(party);
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        partyClearFootmark();
    } else {
        partyClearFootmark2();
    }
    return 3;
}

void party_motion_upstairs(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->flags |= PARTY_FLAG_IS_STEPPING;
        upstairs_count = 3;
        upstairs_dy = (party->stepY - party->position.y) / 3.0f;
    }
    party->position.y += upstairs_dy;
    if (--upstairs_count <= 0) {
        party->flags &= ~PARTY_FLAG_IS_STEPPING;
        partyChgMot(party, 1);
    }
}

static inline BOOL partyChkHitAttr200(HitEntry* hit) {
    if (!hit) {
        return FALSE;
    }
    if (hitGetAttr(hit) & HIT_ATTR_WATER_HAZARD) {
        return TRUE;
    }
    return FALSE;
}

void party_motion_damage(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 x, y, z;
    Vec ret;

    if (party->trigFlags & PARTY_TRIG_CHG_MOT) {
        party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
        party->motionStep = 0;
    }
    switch (party->motionStep) {
        case 0:
            partyChgPoseId(party, 8);
            party->velY = 15.0f;
            party->accelY = 0.0f;
            party->moveFrames = 60;
            party->motionStep = 1;
        case 1:
            party->position.y += party->velY;
            marioGetScreenPos(party->position, &x, &y, &z);
            if (y <= -300.0f) {
                party->moveFrames = 20;
                party->motionStep = 10;
            }
            break;
        case 10:
            if (mario->currMotionId == MARIO_MOTION_DAMAGE || mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
                break;
            }
            if (--party->moveFrames <= 0) {
                partyGetReturnPos(&ret);
                party->position.x = ret.x;
                party->position.y = 200.0f + ret.y;
                party->position.z = ret.z;
                party->velY = 0.0f;
                party->accelY = -1.0f;
                partyForceSetSpd(party, 0.0f);
                party->flags |= PARTY_FLAG_IS_FALLING;
                party->flags |= PARTY_FLAG_FORCE_FALL;
                party->motionStep = 11;
            }
            break;
        case 11:
            marioGetScreenPos(party->position, &x, &y, &z);
            if (y > 240.0f) {
                party->flags &= ~PARTY_FLAG_FORCE_FALL;
            }
            partyMove(party);
            if (party->velY <= -6.0f) {
                party->accelY = 0.0f;
                party->velY = -6.0f;
            }
            if (party->position.y < -2000.0f) {
                party->position = mario->position;
                party->position.y += 200.0f;
                party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
            }
            if (party->hitobjStandOn && !partyChkHitAttr200(party->hitobjStandOn)) {
                party->motionStep = 12;
                party->moveFrames = 30;
            }
            break;
        case 12:
            if (--party->moveFrames <= 0) {
                partyChgRunMode(party, 2);
                party->moveMode = 0;
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
            }
            break;
    }
}

void allPartySlitOn(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        if (!(party->playerPtr->trigFlags & MARIO_TRIG_FLAG_NORMALIZING)) {
            party->trigFlags |= PARTY_TRIG_SLIT_INIT;
            party->slitOn = 1;
        }
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        if (!(party->playerPtr->trigFlags & MARIO_TRIG_FLAG_NORMALIZING)) {
            party->trigFlags |= PARTY_TRIG_SLIT_INIT;
            party->slitOn = 1;
        }
    }
}

void allPartySlitOff(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party && (party->flags & PARTY_FLAG_PAPER_MODE) && party->slitStep < 0x28) {
        party->slitStep = 0x28;
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party && (party->flags & PARTY_FLAG_PAPER_MODE) && party->slitStep < 0x28) {
        party->slitStep = 0x28;
    }
}

void allPartyForceSlitOff(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        L_partyForceSlitOff(party);
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        L_partyForceSlitOff(party);
    }
}

#ifdef VERSION_G8ME01
void U_allPartySlitOffReq(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party && ((party->flags & PARTY_FLAG_PAPER_MODE) || (party->flags2 & PARTY_DISP_FLAG_SLIT))) {
        party->flags2 &= ~(PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED | PARTY_DISP_FLAG_SLIT | PARTY_DISP_FLAG_SLIT_DIR_280);
        partyPaperOff(party);
        party->flags &= ~(PARTY_FLAG_PAPER_MODE | PARTY_FLAG_MOT_SLIT_FLOOR);
        party->slitOn = 0;
        partyChgPoseId(party, 1);
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party && ((party->flags & PARTY_FLAG_PAPER_MODE) || (party->flags2 & PARTY_DISP_FLAG_SLIT))) {
        party->flags2 &= ~(PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED | PARTY_DISP_FLAG_SLIT | PARTY_DISP_FLAG_SLIT_DIR_280);
        partyPaperOff(party);
        party->flags &= ~(PARTY_FLAG_PAPER_MODE | PARTY_FLAG_MOT_SLIT_FLOOR);
        party->slitOn = 0;
        partyChgPoseId(party, 1);
    }
}

#endif
void L_partyForceSlitOff(PartyEntry* party) {
    if ((party->flags & PARTY_FLAG_PAPER_MODE) || (party->flags2 & PARTY_DISP_FLAG_SLIT)) {
        party->displayDirection = revise360(party->targetDir - party->camDir) >= 180.0f ? 0.0f : 180.0f;
        party->displayDirectionTarget = party->displayDirection;
        party->moveDir = revise360(270.0f - party->displayDirection);
        party->targetDir = party->moveDir;
        party->flags2 &= ~(PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED | PARTY_DISP_FLAG_SLIT | PARTY_DISP_FLAG_SLIT_DIR_280);
        partyPaperOff(party);
        party->flags &= ~(PARTY_FLAG_PAPER_MODE | PARTY_FLAG_MOT_SLIT_FLOOR);
        party->slitOn = 0;
        partyChgPoseId(party, 1);
    }
}

void partyChgMotSlitFloor(PartyEntry* party) {
    if (party->flags & PARTY_FLAG_MOT_SLIT_FLOOR) {
        return;
    }
    if (!(party->playerPtr->trigFlags & MARIO_TRIG_FLAG_NORMALIZING)) {
        party->trigFlags |= PARTY_TRIG_SLIT_INIT;
        party->slitOn = 1;
    }
    party->flags |= PARTY_FLAG_MOT_SLIT_FLOOR;
}

#ifdef VERSION_G8ME01
void partySlitReset(PartyEntry* party) {
    if (party) {
        if (party->slitOn == 1) {
            L_partyForceSlitOff(party);
        }
        party->slitOn = 0;
    }
}

#endif
BOOL party_slit(PartyEntry* party) {
    BOOL ret = TRUE;
    MarioWork* mario;
    f32 a;

    mario = party->playerPtr;
#ifdef VERSION_G8ME01
    // US: a partner not yet in slit mode joins in while Mario is on a slit floor
    if (party->slitOn != 1) {
        if (mario->flags & MARIO_FLAG_PAPER_MODE) {
            if (mario->trigFlags & MARIO_TRIG_FLAG_NORMALIZING) {
                return TRUE;
            }
            party->trigFlags |= PARTY_TRIG_SLIT_INIT;
            party->slitOn = 1;
        } else {
            return TRUE;
        }
    }
#else
    if (party->slitOn != 1) {
        return TRUE;
    }
#endif
    if (party->trigFlags & PARTY_TRIG_SLIT_INIT) {
        party->trigFlags &= ~PARTY_TRIG_SLIT_INIT;
        party->flags2 |= PARTY_DISP_FLAG_SLIT;
        partyPaperOn(party, "p_slit");
        partyChgPaper(party, "PM_S_1A");
        party->moveFrames = 30;
        partyForceSetSpd(party, 0.0f);
        if (revise360(party->targetDir - mario->targetYaw) <= 180.0f) {
            party->flags2 &= ~PARTY_DISP_FLAG_SLIT_DIR_280;
            party->slitStep = 0;
        } else {
            party->flags2 |= PARTY_DISP_FLAG_SLIT_DIR_280;
            party->slitStep = 2;
        }
    }
    switch (party->slitStep) {
        case 0:
            if (--party->moveFrames < 0) {
                party->flags |= PARTY_FLAG_PAPER_MODE;
                party->displayDirection = 270.0f;
                party->displayDirectionTarget = party->displayDirection;
                partyChgPaper(party, "PM_S_1B");
                if (!(party->flags & PARTY_FLAG_MOT_SLIT_FLOOR)) {
                    party->slitStep = 10;
                    ret = FALSE;
                } else {
                    party->slitStep = 20;
                }
            }
            break;
        case 2:
            if (--party->moveFrames < 0) {
                party->flags |= PARTY_FLAG_PAPER_MODE;
                party->displayDirection = 270.0f;
                party->displayDirectionTarget = party->displayDirection;
                partyChgPaper(party, "PM_S_1B");
                if (!(party->flags & PARTY_FLAG_MOT_SLIT_FLOOR)) {
                    party->slitStep = 10;
                    ret = FALSE;
                } else {
                    party->slitStep = 20;
                }
            }
            break;
        case 10:
            if (!party->moveSpeed) {
                partyChgPaper(party, "PM_S_1B");
                party->displayDirection = 270.0f;
            } else {
                if (revise360(party->targetDir - mario->targetYaw) >= 180.0) {
                    party->flags2 |= PARTY_DISP_FLAG_SLIT_DIR_280;
                } else {
                    party->flags2 &= ~PARTY_DISP_FLAG_SLIT_DIR_280;
                }
                party->displayDirection = (party->flags2 & PARTY_DISP_FLAG_SLIT_DIR_280) ? 280.0f : 260.0f;
                a = revise360(party->targetDir - mario->targetYaw);
                if (a >= 315.0f || a <= 45.0f || (a >= 135.0f && a <= 225.0f)) {
                    partyChgPaper(party, "PM_S_1E");
                } else {
                    partyChgPaper(party, "PM_S_1D");
                }
            }
#ifdef VERSION_G8ME01
            if (!partyChkBetweenWalls(party, party->position) && !(mario->flags & MARIO_FLAG_PAPER_MODE) &&
                !(hitGetAttr(party->hitobjFront) & HIT_ATTR_SLIT) && !(hitGetAttr(party->hitobjPush) & HIT_ATTR_SLIT))
#else
            if (!(mario->flags & MARIO_FLAG_PAPER_MODE) && !(hitGetAttr(party->hitobjFront) & HIT_ATTR_SLIT) &&
                !(hitGetAttr(party->hitobjPush) & HIT_ATTR_SLIT))
#endif
            {
                party->slitStep = 40;
            }
            break;
    }
    switch (party->slitStep) {
        case 20:
            partyChgPaper(party, "PM_S_1H");
            party->flags |= PARTY_FLAG_FORCE_FALL;
            party->flags |= PARTY_FLAG_IS_FALLING;
            party->moveFrames = 20;
            partyForceSetSpd(party, 0.0f);
            party->slitStep = 21;
            break;
        case 21:
            party->displayDirection = revise360(party->targetDir - party->camDir) <= 180.0f ? 260.0f : 280.0f;
            party->position.y += -3.0f;
            if (--party->moveFrames <= 0) {
                party->slitStep = 22;
            }
            break;
        case 22:
            party->slitStep = 10;
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
            marioClearSlitFloor();
            break;
    }
    switch (party->slitStep) {
        case 40:
            partyChgPaper(party, "PM_S_1C");
            party->moveFrames = 30;
            party->slitStep = 41;
            party->displayDirection = revise360(party->targetDir - mario->targetYaw) >= 180.0f ? 0.0f : 180.0f;
            party->displayDirectionTarget = party->displayDirection;
        case 41:
            if (--party->moveFrames <= 0) {
                party->slitStep = 42;
            }
            break;
        case 42:
            party->flags2 &= ~(PARTY_DISP_FLAG_TURN_LEFT_UNUSED | PARTY_DISP_FLAG_TURN_RIGHT_UNUSED | PARTY_DISP_FLAG_SLIT | PARTY_DISP_FLAG_SLIT_DIR_280);
            partyPaperOff(party);
            party->flags &= ~(PARTY_FLAG_PAPER_MODE | PARTY_FLAG_MOT_SLIT_FLOOR);
            party->slitOn = 0;
            break;
    }
    return ret;
}

BOOL party_roll(PartyEntry* party) {
    BOOL ret = TRUE;
    f32 x;

    if (party->slitOn != 2) {
        return TRUE;
    }
    if (party->trigFlags & PARTY_TRIG_SLIT_INIT) {
        party->trigFlags &= ~PARTY_TRIG_SLIT_INIT;
        partyPaperOn(party, "p_roll");
        partyChgPaper(party, "PM_R_1A");
        partyChgPoseId(party, 0);
        party->rotOfs.x = -6.0f;
        party->rotOfs.y = 6.0f;
        party->dispOfs.x = 6.0f;
        party->dispOfs.y = 2.0f;
        party->moveFrames = 0x41;
        party->slitStep = 0;
        partyForceSetSpd(party, 0.0f);
    }
    switch (party->slitStep) {
        case 0:
            party->moveFrames--;
            if (party->moveFrames <= 0) {
                partyChgPaper(party, "PM_R_1B");
                party->slitStep = 10;
            }
            break;
        case 10:
            party->displayDirection = revise360(270.0f - party->targetDir);
            if (revise360(party->moveDir + party->displayDirection) > 180.0f) {
                party->rotation = revise360(7.0f * party->moveSpeed + party->rotation);
            } else {
                party->rotation = revise360(party->rotation - 7.0f * party->moveSpeed);
            }
            break;
    }
    switch (party->slitStep) {
        case 100:
            party->moveSpeed = 0.0f;
            party->rot.x = 0.0f;
            party->rotation = 0.0f;
            party->rotOfs.x = 0.0f;
            party->rotOfs.y = 0.0f;
            party->dispOfs.x = party->targetDir < 180.0f ? 3.0f : -3.0f;
            party->dispOfs.y = 0.0f;
            party->moveFrames = 0x41;
            party->slitStep = 101;
            partyChgPaper(party, "PM_R_1C");
            partyPaperLightOff(party);
        case 101:
            ret = FALSE;
            party->moveFrames--;
            if (party->moveFrames > 0) {
                if (party->targetDir < 180.0f) {
                    x = 0.1f * (90.0f - party->displayDirection);
                    x += party->displayDirection;
                    if (__fabsf(90.0f - x < 1.0f)) {
                        x = 90.0f;
                    }
                    party->displayDirection = x;
                    party->displayDirectionTarget = x;
                } else {
                    x = 0.1f * (270.0f - party->displayDirection);
                    x += party->displayDirection;
                    if (__fabsf(270.0f - x < 1.0f)) {
                        x = 270.0f;
                    }
                    party->displayDirection = x;
                    party->displayDirectionTarget = x;
                }
            } else {
                ret = TRUE;
                if (party->targetDir >= 180.0f) {
                    party->displayDirection = 0.0f;
                    party->displayDirectionTarget = 0.0f;
                    party->moveDir = 270.0f;
                    party->targetDir = 270.0f;
                } else {
                    party->displayDirection = 180.0f;
                    party->displayDirectionTarget = 180.0f;
                    party->moveDir = 90.0f;
                    party->targetDir = 90.0f;
                }
                party->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
                party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
                party->slitOn = 0;
                party->flags &= ~PARTY_FLAG_PAPER_MODE;
                party->flags2 &= ~PARTY_DISP_FLAG_SLIT;
                party->flags &= ~PARTY_FLAG_KEEP_POSE_UNUSED;
                party->flags2 &= ~PARTY_DISP_FLAG_NO_MAKE_DISP_DIR;
                party->flags2 &= ~PARTY_DISP_FLAG_NO_FLIP;
                party->slitStep = 100;
                partyPaperOff(party);
            }
            break;
    }
    return ret;
}

static f32 partyStayDist(PartyEntry* party) {
    f32 d;
    if (party->moveKind == 0) {
        d = 18.0f;
    } else if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
        if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
            d = 8.0f;
        } else {
            d = 30.0f;
        }
    } else if (party->currentMemberId == PARTY_MEMBER_VIVIAN) {
        if (party->trigFlags & PARTY_TRIG_DOOR_MOVE) {
            d = 8.0f;
        } else {
            d = 30.0f;
        }
    }
    return d;
}

static f32 partyHomingOff(PartyEntry* party) {
    f32 off = 5.0f;
    if (party->flags2 & PARTY_DISP_FLAG_FAMICOM) {
        off = 1.0f;
    }
    return off;
}

s32 getFrontFloor(PartyEntry* party, Vec* out, f32 dir) {
    f32 s2, c2;
    HitCheckQuery q2;
    HitCheckQuery q;
    f32 c, s;
    f32 r;
    f32 wallct;
    f32 y;
    f32 rf;
    s32 i;
    HitEntry* hit;

    r = 2.0f * party->width / 3.0f;
    q.targetDir = (Vec){0.0f, 1.0f, 0.0f};
    wallct = touch_wall_ct;
    sincosf(&s, &c, dir);
    for (i = 0; i < 3; i++) {
        q.targetPos.x = party->position.x + (f32)i * (s * r);
        q.targetPos.z = party->position.z + (f32)i * (c * r);
        q.targetPos.y = party->position.y + party->height;
        q.targetDistance = 100.0f;
        if (hitCheckVecFilter(&q, NULL)) {
            if (sqrtf(q.hitNormal.x * q.hitNormal.x + q.hitNormal.z * q.hitNormal.z) < 45.0f) {
                y = q.hitPos.y;
                goto found;
            }
        }
    }
    y = -2000.0f;
found:
    if (y > -2000.0f) {
        f32 t = party->position.y + wallct;
        f32 u = y - 2.0f;
        if (t < u) {
            q2.targetPos.y = t;
        } else {
            q2.targetPos.y = u;
        }
    } else {
        q2.targetPos.y = party->position.y + wallct;
    }
    r = 0.75f * party->width;
    sincosf(&s2, &c2, dir);
    q2.targetPos.x = s2 * r + party->position.x;
    q2.targetPos.z = c2 * r + party->position.z;
    q2.targetDistance = 300.0f;
    q2.targetDir = (Vec){0.0f, -1.0f, 0.0f};
    hit = hitCheckVecFilter(&q2, NULL);
    if (!hit) {
        out->x = party->position.x;
        out->y = party->position.y;
        out->z = party->position.z;
        return 0;
    }
    rf = q2.hitPos.x >= 0.0f ? 0.5f : -0.5f;
    out->x = (s32)((1000.0f * q2.hitPos.x) + rf) / 1000.0f;
    rf = q2.hitPos.y >= 0.0f ? 0.5f : -0.5f;
    out->y = (s32)((1000.0f * q2.hitPos.y) + rf) / 1000.0f;
    rf = q2.hitPos.z >= 0.0f ? 0.5f : -0.5f;
    out->z = (s32)((1000.0f * q2.hitPos.z) + rf) / 1000.0f;
    return hitGetAttr(hit);
}

static inline void partyDownDirX(HitCheckQuery* q) {
    q->targetDir = (Vec){0.0f, -1.0f, 0.0f};
}

static f32 partyGetDirGndY(Vec u, f32 md) {
    f32 k = 40.0f;
    HitCheckQuery q2;
    HitEntry* hit;
    f32 s2, c2;
    f32 gy;
    f32 a;
    gy = touch_wall_ct;
    sincosf(&s2, &c2, md);
    q2.targetPos = u;
    q2.targetPos.x = s2 * k + q2.targetPos.x;
    q2.targetPos.z = c2 * k + q2.targetPos.z;
    q2.targetPos.y = q2.targetPos.y + gy;
    q2.targetDistance = 1000.0f;
    q2.targetDir = (Vec){0.0f, -1.0f, 0.0f};
    hit = hitCheckVecFilter(&q2, NULL);
    if (hit) {
        a = q2.hitPos.y >= 0.0f ? 0.5f : -0.5f;
        gy = (s32)((1000.0f * q2.hitPos.y) + a) / 1000.0f;
        hitGetAttr(hit);
    } else {
        gy = -2000.0f;
    }
    return gy;
}

static f32 partyGetGndY(f32 x, f32 y, f32 z) {
    f32 r;
    f32 hx, hy, hz, dist, nx, ny, nz;
    HitEntry* hit;
    y += touch_wall_ct;
    dist = 1000.0f;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, 0.0f, -1.0f, 0.0f);
    if (!hit) {
        r = -2000.0f;
    } else {
        r = (s32)(100.0f * hy + 0.5f) / 100.0f;
        hitGetAttr(hit);
    }
    return r;
}

static u32 partyGetFrontAttr(PartyEntry* party, f32 dir) {
    HitCheckQuery q;
    HitEntry* hit;
    f32 s, c;
    sincosf(&s, &c, dir);
    q.targetDistance = 40.0f;
    q.targetPos.x = 20.0f * s + party->position.x;
    q.targetPos.z = 20.0f * c + party->position.z;
    q.targetPos.y = 20.0f + party->position.y;
    q.targetDir = (Vec){0.0f, -1.0f, 0.0f};
    hit = hitCheckVecFilter(&q, NULL);
    return hit ? hitGetAttr(hit) : 0;
}

static inline u32 partyGetFrontAttrFar(PartyEntry* party) {
    HitCheckQuery q;
    HitEntry* hit;
    f32 s, c;
    f32 k = 40.0f;
    sincosf(&s, &c, party->targetDir);
    q.targetDistance = 100.0f;
    q.targetPos.x = s * k + party->position.x;
    q.targetPos.z = c * k + party->position.z;
    q.targetPos.y = party->position.y + k;
    q.targetDir = (Vec){0.0f, -1.0f, 0.0f};
    hit = hitCheckVecFilter(&q, NULL);
    return hit ? hitGetAttr(hit) : 0;
}

static inline void partyDownDirY(HitCheckQuery* q) {
    q->targetDir = (Vec){0.0f, -1.0f, 0.0f};
}

void party_force_reset_outofscreen(PartyEntry* party) {
    MarioWork* m2;
    s32 i;
    s32 margin;
    s32 j;
    MarioWork* mario = party->playerPtr;
    f32 speed;
    f32 y;
    f32 up;
    HitEntry* hit;
    f32 dist;
    f32 sx, sy, sz;
    Vec pos;
    Vec start;
    Vec dir;
    Vec hitPos;
    Vec nrm;

    if (marioCtrlOffChk()) {
        return;
    }
    if (marioKeyOffChk()) {
        return;
    }
    marioGetScreenPos(party->position, &sx, &sy, &sz);
    if (distABf(mario->position.x, mario->position.z, party->position.x, party->position.z) < 20.0f &&
        sy > 800.0f) {
        partyGetAppearPos3(party, &pos);
        party->position = pos;
        return;
    }
    margin = 100;
    if (-100.0f <= sx && sx <= 700.0f && sy <= 650.0f) {
        return;
    }
    if (!(party->flags & PARTY_FLAG_IS_PARTNER)) {
        partyClearFootmark();
    } else {
        partyClearFootmark();
    }
    m2 = party->playerPtr;
    i = 0;
    pos = party->position;
    if (party->moveKind == 0) {
        speed = party->width;
    } else if (party->currentMemberId == PARTY_MEMBER_FLURRIE) {
        speed = 10.0f;
        margin = 70;
    } else {
        speed = 10.0f;
        margin = 50;
    }
    {
        Vec d = {0.0f, -1.0f, 0.0f};
        dir = d;
    }
    while (TRUE) {
        marioGetScreenPos(pos, &sx, &sy, &sz);
        if ((f32)-margin <= sx && sx <= (f32)(margin + 600)) {
            party->position = pos;
            return;
        }
        if (i > 100) {
            return;
        }
        {
            f32 degree = angleABf(pos.x, pos.z, m2->position.x, m2->position.z);
            movePos(&pos.x, &pos.z, speed, degree);
        }
        up = 200.0f;
        j = 0;
        y = pos.y;
        while (TRUE) {
            dist = 300.0f;
            start = pos;
            start.y += up;
            hit = partyHitCheck(party, &start, &dir, &hitPos, &nrm, &dist);
            if (hit) {
                if (!partyChkHitAttr200(hit)) {
                    y = hitPos.y;
                }
            } else {
                y = m2->position.y;
            }
            start.y = y;
            marioGetScreenPos(start, &sx, &sy, &sz);
            if (sy > 100.0f) {
                dist = 100.0f;
                start = pos;
                if (pos.y + 74.0f > y) {
                    start.y += 74.0f;
                    hit = partyHitCheck(party, &start, &dir, &hitPos, &nrm, &dist);
                    if (hit && !partyChkHitAttr200(hit)) {
                        y = hitPos.y;
                    }
                }
                break;
            }
            if (++j > 4) {
                break;
            }
            up -= 50.0f;
            if (up < 0.0f) {
                up = 0.0f;
            }
        }
        pos.y = y;
        i++;
    }
}

static void partyGorFix(PartyEntry* party) {
    MarioWork* m3 = party->playerPtr;
    if (!strcmp(gp->currentMapName, "gor_01") && m3->hitobjStandOn && party->hitobjStandOn &&
        !strcmp(hitGetName(m3->hitobjStandOn), "A_ie_03_yuka_f1_C") && !strcmp(hitGetName(party->hitobjStandOn), "A_ie_03_yuka_f2_C")) {
        Vec fix = {122.0f, 130.0f, -313.0f};
        party->position = fix;
    }
}

static void partyPoseA2(PartyEntry* party) {
    partyChgPose(party, "PCH_A2_1");
    party->useMotionId = 1;
    party->moveFrames = 30;
}

