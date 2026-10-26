#include "evt/evt_cmd.h"
#include "evt/evt_npc.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "koopa.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mario/mario_party.h"
#include "mario/mario_pouch.h"
#include "mario/mario_sbr.h"
#include "mario/mariost.h"
#include "party.h"
#include "system.h"
#include <string.h>
#include "evt/evt_mario.h"
#include "party/party_vivian.h"
#include "mario/mario_hit.h"
#include "party/party_yoshi.h"
#include "mot/mot_slit.h"
#include "party/party_motion.h"
#include "mario/mario_balloon.h"
#include "party/party_move.h"
#include "peach.h"
#include "koopa_motion.h"
#include "mot/mot_roll.h"
#include "mot/mot_plane.h"
#include "eff/eff_kemuri.h"


void marioSetForceMove(s32 frames, f32 dir, f32 spd);

#define HITOBJ(n, field)                                   \
    USER_FUNC(evt_mario_get_name_hitobj_##n) {             \
        EvtWord id = event->args[0];                       \
        const char* name = NULL;                           \
        MarioWork* mp = marioGetPtr();                     \
        if (mp->field) {                                   \
            name = hitGetName(mp->field);                  \
        }                                                  \
        evtSetValue(event, id, (EvtWord)name);             \
        return EVT_RETURN_DONE;                            \
    }

#define FLAGONOFF(n, field)                                \
    USER_FUNC(evt_mario_##n##_onoff) {                     \
        EvtWord* args = event->args;                           \
        MarioWork* mp = marioGetPtr();                     \
        if (evtGetValue(event, args[0])) {                 \
            mp->field |= evtGetValue(event, args[1]);      \
        } else {                                           \
            mp->field &= ~evtGetValue(event, args[1]);     \
        }                                                  \
        return EVT_RETURN_DONE;                            \
    }

#define SETPOSE_BODY                                       \
    if (mp->flags & MARIO_FLAG_IS_8_BIT) {                          \
        name = toDotMarioPose(name);                       \
        if (!name) {                                       \
            return EVT_RETURN_DONE;                        \
        }                                                  \
        mp->animName = name;                               \
        mp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;                           \
    } else {                                               \
        notFound = TRUE;                                   \
        for (i = 0; a_mario_group[i]; i++) {               \
            if (!strcmp(name, a_mario_group[i])) {         \
                notFound = FALSE;                          \
                break;                                     \
            }                                              \
        }                                                  \
        if (!notFound) {                                   \
            mp->animName = name;                           \
            mp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;                       \
            mp->dispFlags &= ~MARIO_DISP_FLAG_EVT_POSE;                  \
        } else {                                           \
            mp->animName = name;                           \
            mp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;                       \
            mp->dispFlags |= MARIO_DISP_FLAG_EVT_POSE;                   \
        }                                                  \
    }                                                      \
    if (mp->dispFlags & MARIO_DISP_FLAG_SLIT) {                       \
        mp->trigFlags |= 0x8000;                           \
    }                                                      \
    return EVT_RETURN_DONE;

static inline BOOL marioChkLanding(void) {
    MarioWork* mp = marioGetPtr();
    s32 m = mp->currMotionId;
    if (m == 0 || m == 1 || m == 2) {
        return TRUE;
    }
    if (m == 0x16 && mp->airTimer == 0) {
        return TRUE;
    }
    if (m == 0x15 && mp->airTimer == 0) {
        return TRUE;
    }
    if (m == 0x19 && mp->airTimer == 0) {
        return TRUE;
    }
    if (m == 0x1a) {
        return TRUE;
    }
    return FALSE;
}

void vivianUnhold(PartyEntry* party);



static inline BOOL marioLandChk(f32 vy) {
    MarioWork* mp = marioGetPtr();
    f32 a;
    f32 y;
    f32 b;
    void* hit;
    if (mp->flags & MARIO_FLAG_TUBE_MODE) {
        hit = marioSearchGroundRoll(&y, &a, &b, 11.0f, mp->jumpVelocityY);
    } else {
        hit = marioSearchGround(&y, &a, &b, vy, vy);
    }
    if (hit && mp->position.y + vy <= y) {
        marioClearJumpPara();
        mp->hitobjStandOn = hit;
        mp->position.y = y;
        mp->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mp->jumpDistance = __fabsf(distABf(mp->position.x, mp->position.z, mp->lastGroundPos.x, mp->lastGroundPos.z));
        mp->airTimer = 0;
        mp->jumpPeakAirTime = 0;
        mp->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
        camFollowYOn();
        return TRUE;
    }
    return FALSE;
}

void partyMoveBehindMario3(PartyEntry* party, s32 a, f32 dir);

USER_FUNC(evt_mario_dokan_prepare) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    PartyEntry* party;
    s32 type;
    mp->flags |= MARIO_FLAG_FLOORS_DISABLED;
    type = evtGetValue(event, args[0]);
    if (type == 4) {
        mp->scale = (Vec){0.0f, 0.0f, 0.0f};
        mp->dispFlags |= MARIO_DISP_FLAG_SHADOW_OFF;
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            partyShadowOff(party);
            party->scale = (Vec){0.0f, 0.0f, 0.0f};
            party->flags2 |= 1;
            party->flags |= PARTY_FLAG_DISABLE_FLOORS;
            party->flags |= PARTY_FLAG_FORCE_FALL;
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            partyShadowOff(party);
            party->scale = (Vec){0.0f, 0.0f, 0.0f};
            party->flags2 |= 1;
            party->flags |= PARTY_FLAG_DISABLE_FLOORS;
            party->flags |= PARTY_FLAG_FORCE_FALL;
        }
    } else if (type == 5) {
        mp->dispFlags |= MARIO_DISP_FLAG_SHADOW_OFF;
        partyShadowOff(partyGetPtr(marioGetPartyId()));
        partyShadowOff(partyGetPtr(marioGetExtraPartyId()));
    }
    return EVT_RETURN_DONE;
}



USER_FUNC(evt_mario_paper_pose_dokan) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    s32 type = evtGetValue(event, args[0]);
    PartyEntry* party;
    s32 ret;
#ifdef VERSION_G8ME01
    f32 s, c;
#endif
    {
        static const char names[] = "DOKAN_TATE_DOWNIN\0\0\0DOKAN_TATE_UPIN\0DOKAN_TATE_DOWNOUT\0\0DOKAN_TATE_UPOUT\0\0\0\0DOKAN_YOKO_IN\0\0\0DOKAN_YOKO_OUT\0";
    }

    if (isFirstCall) {
        event->userdata[1] = 0;
        if (mp->currMotionId == MARIO_MOTION_DOKAN) {
            marioPaperOff();
            mp->scale = (Vec){1.0f, 1.0f, 1.0f};
            mp->rot = (Vec){0.0f, 0.0f, 0.0f};
            mp->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
            mp->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
#ifdef VERSION_G8ME01
            mp->dispOfs2 = (Vec){0.0f, 0.0f, 0.0f};
#endif
        } else {
            switch (type) {
                case 4:
                case 5:
                    event->userdata[1] = 20;
                    ret = 2;
                    break;
                case 1:
                    if (!(mp->flags & MARIO_FLAG_BG_MODE)) {
                        marioUpdateCamPos();
                        camFollowYOn();
#ifdef VERSION_G8ME01
                        sincosf(&s, &c, mp->targetYaw);
                        mp->dispOfs2.x = -0.4f * s;
                        mp->dispOfs2.z = -0.4f * c;
#endif
                    }
                    event->userdata[1] = 0;
                    break;
                case 0:
                    event->userdata[1] = 2;
                    break;
                case 2:
                    event->userdata[1] = 10;
                    break;
                case 3:
                    event->userdata[1] = 10;
                    break;
            }
        }
        if (type == 2 || type == 3) {
            marioSlitForceCancel();
            marioRollForceCancel();
            partyShadowOff(partyGetPtr(marioGetPartyId()));
            partyShadowOff(partyGetPtr(marioGetExtraPartyId()));
            marioPaperOn("p_dokan_y");
            marioPaperLightOff();
            marioChgPaper("PM_D_1A");
            marioChgPose("M_Z_1");
            mp->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
            mp->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            mp->displayDirection = mp->displayDirectionTarget;
            mp->mot.dokan.rotSpeed = 0.0f;
            mp->dispOfs = (Vec){0.0f, 0.0f, (mp->flags & MARIO_FLAG_BG_MODE) ? -16.0f : 0.0f};
            event->userdata[0] = 0;
            marioNoUpdateCamPos();
            if (!(mp->flags & MARIO_FLAG_BG_MODE)) {
                camFollowYOff();
            }
        } else if (type == 5) {
            partyGoodbye(marioGetPartyId());
            partyGoodbye(marioGetExtraPartyId());
        }
    }

    switch (event->userdata[1]) {
        case 2:
            marioSlitForceCancel();
            marioRollForceCancel();
            marioPaperOn("p_dokan_y");
            marioPaperLightOff();
            marioChgPaper("PM_D_1A");
            marioChgPose("M_Z_1");
            mp->dispFlags |= MARIO_DISP_FLAG_NO_DIR_FLIP;
            mp->dispFlags |= MARIO_DISP_FLAG_NO_MAKE_DISP_DIR;
            ret = 2;
            mp->scale = (Vec){1.0f, 1.0f, 1.0f};
            mp->mot.dokan.rotSpeed = 30.0f;
            event->userdata[0] = 8;
            event->userdata[1] = 3;
            break;
        case 3:
            ret = 0;
            if (--event->userdata[0] > 0) {
                break;
            }
            event->userdata[0] = 10;
            event->userdata[1] = 4;
            break;
        case 4:
            ret = 0;
            if (--event->userdata[0] > 0) {
                break;
            }
            ret = 2;
            break;
        case 0:
            mp->scale = (Vec){0.0f, 0.0f, 0.0f};
            mp->dispOfs = (Vec){0.0f, 0.0f, (mp->flags & MARIO_FLAG_BG_MODE) ? -16.0f : 0.0f};
            if (!(mp->flags & MARIO_FLAG_BG_MODE)) {
                mp->position.z += -8.0f;
            }
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                party->position = mp->position;
                party->scale = mp->scale;
                party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
                party->rot = (Vec){0.0f, 0.0f, 0.0f};
                party->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
                party->rotOfsY = mp->rotOfs.y;
                party->dispOfs.y = mp->dispOfs.y;
                party->dispOfs.z = mp->dispOfs.z;
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party) {
                party->position = mp->position;
                party->scale = mp->scale;
                party->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
                party->rot = (Vec){0.0f, 0.0f, 0.0f};
                party->rotOfs = (Vec){0.0f, 0.0f, 0.0f};
                party->rotOfsY = mp->rotOfs.y;
                party->dispOfs.y = mp->dispOfs.y;
                party->dispOfs.z = mp->dispOfs.z;
            }
            ret = 2;
            break;
        case 10:
#ifdef VERSION_G8ME01
            mp->mot.dokan.rotSpeed = 30.0f;
#else
            mp->mot.dokan.rotSpeed += 1.0f;
            if (mp->mot.dokan.rotSpeed > 30.0f) {
                mp->mot.dokan.rotSpeed = 30.0f;
            }
#endif
            event->userdata[0]++;
            if (event->userdata[0] > 10) {
                mp->displayDirection += mp->mot.dokan.rotSpeed;
                mp->mot.dokan.rotSpeed += 0.1f;
                if (mp->mot.dokan.rotSpeed > 30.0f) {
                    mp->mot.dokan.rotSpeed = 30.0f;
                }
            }
            if (event->userdata[0] < sysMsec2Frame(300)) {
                ret = 0;
            } else {
                partyGoodbye(marioGetPartyId());
                partyGoodbye(marioGetExtraPartyId());
                ret = 2;
            }
            break;
    }
    if (ret == 2) {
        mp->dokanType = type;
        marioChgMot(MARIO_MOTION_DOKAN);
    }
    return ret;
}

const char* a_mario_group[] = {
    "M_Z_1", "M_Z_9", "M_S_1", "M_S_3", "M_W_1", "M_R_1", "M_J_1A", "M_J_1B", "M_J_1C", "M_H_1",
    "M_H_2", "M_H_3", "M_H_3A", "M_H_3B", "M_H_4", "M_H_5", "M_H_6", "M_H_6A", "M_H_6B", "M_H_7",
    "M_H_8", "M_H_9", "M_H_9A", "M_H_9B", "M_O_1", "M_O_2", "M_B_1", "M_B_2", "M_D_1", "M_D_2",
    "M_U_3", "M_D_6", "M_P_1", "M_P_2", "M_I_1", "M_I_2", "M_I_S", "M_X_1", "M_I_U", "M_A_1A",
    "M_A_1B", "M_U_2", "M_W_7", "M_C_1", "M_C_2", "M_Y_1", "M_Y_2", "M_Y_3", "M_A_2A", "M_A_2B",
    "M_A_2C", "M_B_3", "M_B_4", "M_J_5A", "M_J_5B", "M_S_4", "M_W_6", "M_V_2", "M_I_Y", NULL,
};

USER_FUNC(evt_mario_dokan_end) {
    MarioWork* mp = marioGetPtr();
    if (isFirstCall && mp->currMotionId == MARIO_MOTION_DOKAN) {
        mp->currSubMotionId = 100;
        return EVT_RETURN_BLOCK;
    }
    if (mp->currMotionId == MARIO_MOTION_DOKAN) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_paper_pose_off) {
    marioPaperOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_balloon_bikkuri) {
    MarioWork* mp = marioGetPtr();
    if (!(mp->dispFlags & MARIO_DISP_FLAG_EVT_BALLOON)) {
        marioBalloonOn(0);
        mp->dispFlags |= MARIO_DISP_FLAG_EVT_BALLOON;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_balloon_tenten) {
    MarioWork* mp = marioGetPtr();
    if (!(mp->dispFlags & MARIO_DISP_FLAG_EVT_BALLOON)) {
        marioBalloonOn(1);
        mp->dispFlags |= MARIO_DISP_FLAG_EVT_BALLOON;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_balloon_off) {
    MarioWork* mp = marioGetPtr();
    if (mp->dispFlags & MARIO_DISP_FLAG_EVT_BALLOON) {
        marioBalloonOff();
        mp->dispFlags &= ~MARIO_DISP_FLAG_EVT_BALLOON;
    }
    return EVT_RETURN_DONE;
}

FLAGONOFF(flag, flags)

FLAGONOFF(dispflag, dispFlags)

FLAGONOFF(trigflag, trigFlags)

USER_FUNC(evt_mario_cont_onoff) {
    if (!evtGetValue(event, event->args[0])) {
        marioCtrlOff();
    } else {
        marioCtrlOn();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_key_onoff) {
    EvtWord* args = event->args;
    marioGetPtr();
    if (!evtGetValue(event, args[0])) {
        marioKeyOff();
        partyKeyOff();
    } else {
        marioKeyOn();
        partyKeyOn();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_enable_key) {
    s32 key = evtGetValue(event, event->args[0]);
    marioGetPtr()->enableKeyMask = key;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_bgmode_on) {
    marioBgmodeOn();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_bgmode_off) {
    marioBgmodeOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_cam_on) {
    marioUpdateCamPos();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_cam_off) {
    marioNoUpdateCamPos();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_cam_keep_on) {
    camFollowYOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_cam_keep_off) {
    camFollowYOn();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_cam_y_on) {
    camFollowYOn();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_cam_y_off) {
    camFollowYOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_init_camid) {
    marioInitCamId();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_camid) {
    marioSetCamId(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_pos) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    if (!evtGetValue(event, args[0])) {
        evtSetFloat(event, args[1], mp->position.x);
        evtSetFloat(event, args[2], mp->position.y);
        evtSetFloat(event, args[3], mp->position.z);
    } else {
        evtSetFloat(event, args[1], mp->position.x);
        evtSetFloat(event, args[2], mp->position.y - 1.0f);
        evtSetFloat(event, args[3], mp->position.z);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_pos) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    mp->position.x = evtGetFloat(event, args[0]);
    mp->position.y = evtGetFloat(event, args[1]);
    mp->position.z = evtGetFloat(event, args[2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_hosei_xyz) {
    MarioWork* mp = marioGetPtr();
    EvtWord* args = event->args;
    mp->floorMove.x = evtGetFloat(event, args[0]);
    mp->floorMove.y = evtGetFloat(event, args[1]);
    mp->floorMove.z = evtGetFloat(event, args[2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_mov_spd) {
    f32 spd = evtGetFloat(event, event->args[0]) / (f32)gp->framerate;
    marioGetPtr()->baseSpeed = spd;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_mov_spd) {
    s32 id = event->args[0];
    evtSetFloat(event, id, (f32)gp->framerate * marioGetPtr()->baseSpeed);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_dir) {
    s32 id = event->args[0];
    evtSetFloat(event, id, marioGetPtr()->directionView);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_dir) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 dir = evtGetFloat(event, args[0]);
    s32 frames = sysMsec2Frame(evtGetValue(event, args[1]));
    s32 wait = evtGetValue(event, args[2]);
    f32 target;
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[2] = 0;
        event->userdataf[1] = mp->displayDirection;
        if (dir > 270.0f) {
            dir -= 360.0f;
        }
    }
    if (event->userdata[2] == 1) {
        mp->directionView = 0.0f;
        mp->moveDirection = 0.0f;
        return EVT_RETURN_DONE;
    }
    if (frames > 0) {
        target = revise360(270.0f - dir);
        if (target > 270.0f) {
            target -= 360.0f;
        }
        target = (target - event->userdataf[1]) * ((f32)event->userdata[0] / (f32)frames);
        mp->displayDirection = revise360(event->userdataf[1] + target);
        mp->displayDirectionTarget = mp->displayDirection;
        mp->directionView = toMovedirSimple(mp->displayDirection);
        mp->moveDirection = mp->directionView;
        if (++event->userdata[0] > frames) {
            if (!wait) {
                return EVT_RETURN_YIELD;
            }
            event->userdata[2] = 1;
        }
        return EVT_RETURN_BLOCK;
    }
    mp->displayDirection = revise360(270.0f - dir);
    mp->displayDirectionTarget = mp->displayDirection;
    mp->directionView = toMovedirSimple(mp->displayDirection);
    mp->moveDirection = mp->directionView;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_reardir) {
    MarioWork* mp = marioGetPtr();
    marioAdjustMoveDir();
    mp->directionView = 0.0f;
    mp->moveDirection = 0.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_dispdir) {
    s32 id = event->args[0];
    evtSetFloat(event, id, marioGetPtr()->displayDirection);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_dispdir) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    mp->displayDirectionTarget = reviseAngle(evtGetFloat(event, args[0]));
    mp->displayDirection = mp->displayDirectionTarget;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_dir_npc) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);
    f32 camAngle;
    f32 angle;
    if (!npc) {
        return EVT_RETURN_DONE;
    }
    camAngle = camGetPtr(CAMERA_3D)->viewYaw;
    angle = revise360(angleABf(mp->position.x, mp->position.z, npc->position.x, npc->position.z) - camAngle);
    mp->displayDirectionTarget = angle < 180.0f ? 180.0f : 0.0f;
    mp->moveDirection = toMovedirSimple(mp->displayDirectionTarget);
    mp->directionView = mp->moveDirection;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_dir_pos) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 x = evtGetValue(event, args[0]);
    f32 z = evtGetValue(event, args[1]);
    f32 camAngle = camGetPtr(CAMERA_3D)->viewYaw;
    f32 angle = revise360(angleABf(mp->position.x, mp->position.z, x, z) - camAngle);
    mp->displayDirectionTarget = angle < 180.0f ? 180.0f : 0.0f;
    mp->directionView = angle;
    mp->moveDirection = angle;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_adjust_dir) {
    marioAdjustMoveDir();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_normal_pose) {
    s32 i;
    BOOL notFound;
    MarioWork* mp = marioGetPtr();
    const char* name;
    if (mp->currMotionId == MARIO_MOTION_PLANE || mp->currMotionId == MARIO_MOTION_SHIP || mp->currMotionId == MARIO_MOTION_ROLL ||
        mp->currMotionId == MARIO_MOTION_SLIT) {
        return EVT_RETURN_DONE;
    }
    if (mp->currMotionId == MARIO_MOTION_WALK) {
        name = "M_W_1";
    } else if (mp->currMotionId == MARIO_MOTION_DASH) {
        name = "M_R_1";
    } else {
        name = "M_S_1";
    }
    SETPOSE_BODY
}

USER_FUNC(evt_mario_set_pose) {
    s32 i;
    BOOL notFound;
    const char* name;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    if (mp->currMotionId == MARIO_MOTION_PLANE || mp->currMotionId == MARIO_MOTION_SHIP || mp->currMotionId == MARIO_MOTION_ROLL ||
        mp->currMotionId == MARIO_MOTION_SLIT) {
        return EVT_RETURN_DONE;
    }
    name = (const char*)evtGetValue(event, args[0]);
    SETPOSE_BODY
}

#ifdef VERSION_G8ME01
#endif

USER_FUNC(evt_mario_normalize) {
    s32 ret = 0;
    MarioWork* mp = marioGetPtr();
    PartyEntry* party;
    BOOL off;

    if (isFirstCall) {
        party = partyGetPtr(marioGetPartyId());
        if (party) {
#ifdef VERSION_G8ME01
            partySlitReset(party);
#else
            party->slitOn = 0;
#endif
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
#ifdef VERSION_G8ME01
            partySlitReset(party);
#else
            party->slitOn = 0;
#endif
        }
        mp->trigFlags |= 0x80000000;
        event->userdata[1] = 0;
        off = !marioChkCtrl();
        if (off) {
            event->userdata[1] = 1;
            marioCtrlOn();
            partyCtrlOn();
        }
        event->userdata[0] = 0;
        if (mp->currMotionId != MARIO_MOTION_YOSHI && mp->currMotionId != MARIO_MOTION_ROLL && mp->currMotionId != MARIO_MOTION_VIVIAN &&
            !(mp->dispFlags & MARIO_DISP_FLAG_SLIT)) {
            marioChgMot(MARIO_MOTION_STAY);
            if (partyRideChk(partyGetPtr(marioGetPartyId())) || partyRideChk(partyGetPtr(marioGetExtraPartyId()))) {
                event->userdata[0] = 2;
            } else {
                if (event->userdata[1]) {
                    marioCtrlOff();
                    partyCtrlOff();
                }
                mp->trigFlags &= ~0x80000000;
                return EVT_RETURN_DONE;
            }
        }
    }

    switch (event->userdata[0]) {
        case 0:
            ret = 0;
            if (mp->currMotionId == MARIO_MOTION_ROLL) {
                marioRollCancel();
                event->userdata[0] = 1;
            } else if (mp->currMotionId == MARIO_MOTION_SLIT) {
                if (mp->flags & MARIO_FLAG_PAPER_MODE) {
                    motSlitCancel2();
                    event->userdata[0] = 1;
                }
            } else if (mp->currMotionId == MARIO_MOTION_YOSHI) {
                if (yoshiGetStatus() != 2 && marioGetoffYoshi()) {
                    event->userdata[0] = 3;
                }
            } else if (mp->currMotionId == MARIO_MOTION_VIVIAN) {
                if (vivianGetStatus() != 0 && vivianGetStatus() == 2) {
                    vivianUnhold(partyGetPtr(marioGetPartyId()));
                    event->userdata[0] = 5;
                }
            } else if (mp->currMotionId == MARIO_MOTION_STAY) {
                if (!(mp->dispFlags & MARIO_DISP_FLAG_SLIT)) {
                    ret = 2;
                }
            }
            break;
        case 1:
            ret = 0;
            if (mp->currMotionId == MARIO_MOTION_ROLL) {
                if (mp->rollFlags == 0) {
                    event->userdata[0] = 2;
                }
            } else if (mp->currMotionId == MARIO_MOTION_SLIT) {
                if (marioGetMotSlitCancel3()) {
                    ret = 0;
                } else {
                    ret = 2;
                }
            } else {
                ret = 2;
            }
            break;
    }

    switch (event->userdata[0]) {
        case 2:
            ret = 0;
            if (!partyRideChk(partyGetPtr(marioGetPartyId())) && !partyRideChk(partyGetPtr(marioGetExtraPartyId()))) {
                ret = 2;
            }
            break;
        case 3:
            if (mp->currMotionId != MARIO_MOTION_YOSHI) {
                event->userdata[0] = 4;
            } else if (yoshiGetStatus() == 0) {
                event->userdata[0] = 4;
            }
            break;
        case 4:
            if (mp->hitobjStandOn) {
                event->userdata[0] = 10;
                event->userdata[2] = 3;
            }
            break;
        case 5:
            if (mp->currMotionId != MARIO_MOTION_VIVIAN) {
                event->userdata[0] = 6;
            } else if (vivianGetStatus() == 0) {
                event->userdata[0] = 6;
            }
            break;
        case 6:
            if (mp->hitobjStandOn) {
                event->userdata[0] = 10;
                event->userdata[2] = 3;
            }
            break;
        case 10:
            if (--event->userdata[2] > 0) {
                ret = 2;
            }
            break;
    }
    if (ret == 2) {
        mp->trigFlags &= ~0x80000000;
        if (event->userdata[1]) {
            marioCtrlOff();
            partyCtrlOff();
        }
    }
    return ret;
}

USER_FUNC(evt_mario_get_motion) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    s32 id = args[0];
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        evtSetValue(event, id, marioGetPtr()->prevMotionId);
    } else {
        evtSetValue(event, id, marioGetPtr()->currMotionId);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_motion) {
    ((void (*)(s32))marioChgMot)(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_chk_hipbump) {
    EvtWord* args = event->args;
    s32 val;
    s32 id;
    MarioWork* mp = marioGetPtr();
    val = 0;
    id = args[0];
    if (!(mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) && marioGetPtr()->currMotionId == MARIO_MOTION_HIP_2) {
        val = 1;
    }
    evtSetValue(event, id, val);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_talk_motion) {
    marioChgTalkMotion();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_cancel_roll_motion) {
    if (isFirstCall) {
        marioRollTryCancel();
    }
    if (marioGetPtr()->flags & MARIO_FLAG_TUBE_MODE) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_pose) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], (EvtWord)marioGetPtr()->animName);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_force_dir) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    if (mp->airTimer > 0) {
        mp->flags |= MARIO_FLAG_USE_FORCE_DIRECTION;
        mp->forceDir = evtGetValue(event, args[0]);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_mov_pos) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    if (isFirstCall) {
        f32 x = evtGetFloat(event, args[0]);
        f32 z = evtGetFloat(event, args[1]);
        f32 spd;
        event->userdata[0] = sysMsec2Frame(evtGetValue(event, args[2]));
        mp->directionView = angleABf(mp->position.x, mp->position.z, x, z);
        if (event->userdata[0] == 0) {
            event->userdata[0] = distABf(mp->position.x, mp->position.z, x, z) / mp->baseSpeed;
            spd = mp->baseSpeed;
        } else {
            spd = distABf(mp->position.x, mp->position.z, x, z) / event->userdata[0];
        }
        marioSetForceMove(event->userdata[0], mp->directionView, spd);
    }
    if (--event->userdata[0] < 0) {
        return EVT_RETURN_YIELD;
    } else {
        return EVT_RETURN_BLOCK;
    }
}

USER_FUNC(evt_mario_mov_pos2) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 x = evtGetFloat(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    f32 spd = evtGetFloat(event, args[2]);
    if (isFirstCall) {
        f32 dist = distABf(mp->position.x, mp->position.z, x, z);
        mp->directionView = angleABf(mp->position.x, mp->position.z, x, z);
        if (mp->flags & MARIO_FLAG_BG_MODE) {
            spd *= 0.4f;
        }
        spd /= (f32)gp->framerate;
        marioSetForceMove(1.0f / spd + dist / spd, mp->directionView, spd);
    }
    if (mp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        return EVT_RETURN_BLOCK;
    }
    mp->position.x = x;
    mp->position.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(N_evt_mario_party_door_halve_hitbox) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party) {
        party->trigFlags |= 0x40000;
    }
    party = partyGetPtr(marioGetExtraPartyId());
    if (party) {
        party->trigFlags |= 0x40000;
    }
    return EVT_RETURN_DONE;
}

static inline f32 doorAngle(f32 cam) {
    if (cam > 60.0f && cam < 120.0f) return 180.0f;
    if (cam > 240.0f && cam < 300.0f) return 180.0f;
    return 270.0f;
}
USER_FUNC(evt_mario_party_door_move) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 x = evtGetFloat(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    f32 spd = evtGetFloat(event, args[2]);
    PartyEntry* party;
    s32 ret;
    f32 sx, sy, sz;
    f32 dist;
    f32 cam;

    if (isFirstCall) {
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            marioGetScreenPos(party->position, &sx, &sy, &sz);
            if (!(0.0f <= sx && sx <= 600.0f && 0.0f <= sy && sy <= 535.0f)) {
                party->position = mp->position;
            }
            if (mp->position.y > party->position.y) {
                party->position = mp->position;
            }
            party->trigFlags |= 0x40000;
            N_partyFollowCloseOn(party);
            party->flags |= PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
            party->trigFlags |= 0x200000;
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            marioGetScreenPos(party->position, &sx, &sy, &sz);
            if (!(0.0f <= sx && sx <= 600.0f && 0.0f <= sy && sy <= 535.0f)) {
                party->position = mp->position;
            }
            if (mp->position.y > party->position.y) {
                party->position = mp->position;
            }
            party->trigFlags |= 0x40000;
            N_partyFollowCloseOn(party);
            party->flags |= PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
            party->trigFlags |= 0x200000;
        }
        event->userdata[0] = 0;
        event->userdata[2] = 0;
    }

    ret = 0;
    switch (event->userdata[0]) {
        case 0:
            dist = distABf(mp->position.x, mp->position.z, x, z);
            mp->directionView = angleABf(mp->position.x, mp->position.z, x, z);
            if (mp->flags & MARIO_FLAG_BG_MODE) {
                spd *= 0.4f;
            }
            {
                s32 n = (f32)gp->framerate * (dist / spd);
                spd = spd / (f32)gp->framerate;
                marioSetForceMove(n, mp->directionView, spd);
            }
            event->userdata[0] = 2;
            break;
        case 2:
            if (mp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
                break;
            }
            event->userdata[1] = sysMsec2Frame(10);
            event->userdata[0] = 4;
        case 4:
            if (--event->userdata[1] > 0) {
                break;
            }
            event->userdata[1] = sysMsec2Frame(400);
            event->userdata[0] = 6;
            cam = camGetPtr(CAMERA_3D)->viewYaw;
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                partyMoveBehindMario3(party, 0,
                    !strcmp(gp->currentMapName, "pik_00") ? (party->position.x > 0.0f ? 270.0f : 90.0f)
                    : !strcmp(gp->currentMapName, "muj_01") ? 330.0f
                    : doorAngle(cam));
                if (party->moveKind == 1) {
                    party->flags2 |= 0x10;
                }
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party) {
                partyMoveBehindMario3(party, 0,
                    !strcmp(gp->currentMapName, "pik_00") ? (party->position.x > 0.0f ? 270.0f : 90.0f)
                    : !strcmp(gp->currentMapName, "muj_01") ? 30.0f : 270.0f);
                if (party->moveKind == 1) {
                    party->flags2 |= 0x10;
                }
            }
            break;
        case 6:
            if (--event->userdata[1] > 0) {
                break;
            }
            cam = camGetPtr(CAMERA_3D)->viewYaw;
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                partyMoveBehindMario3(party, 0,
                    !strcmp(gp->currentMapName, "pik_00") ? (party->position.x > 0.0f ? 270.0f : 90.0f)
                    : !strcmp(gp->currentMapName, "muj_01") ? 330.0f
                    : doorAngle(cam));
                if (party->moveKind == 1) {
                    party->flags2 |= 0x10;
                }
                N_partyFollowCloseOff(party);
                party->flags &= ~PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
                party->trigFlags &= ~0x40000;
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party) {
                partyMoveBehindMario3(party, 0,
                    !strcmp(gp->currentMapName, "pik_00") ? (party->position.x > 0.0f ? 270.0f : 90.0f)
                    : !strcmp(gp->currentMapName, "muj_01") ? 30.0f : 270.0f);
                if (party->moveKind == 1) {
                    party->flags2 |= 0x10;
                }
                N_partyFollowCloseOff(party);
                party->flags &= ~PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
                party->trigFlags &= ~0x40000;
            }
#ifdef VERSION_G8ME01
            event->userdata[0] = 8;
            event->userdata[1] = 1;
            break;
        case 8:
            if (--event->userdata[1] > 0) {
                break;
            }
#endif
            ret = 2;
            break;
    }
    return ret;
}

USER_FUNC(evt_mario_party_bero_move) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 x = evtGetFloat(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    f32 spd = evtGetFloat(event, args[2]);
    PartyEntry* party;
    s32 ret;
    f32 sx, sy, sz;
    f32 dist;

    if (isFirstCall) {
        party = partyGetPtr(marioGetPartyId());
        if (party) {
            marioGetScreenPos(party->position, &sx, &sy, &sz);
            if (!(0.0f <= sx && sx <= 600.0f && 0.0f <= sy && sy <= 535.0f)) {
                party->position = mp->position;
            }
            if (mp->position.y > party->position.y) {
                party->position = mp->position;
            }
            N_partyFollowCloseOn(party);
            party->flags |= PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
            party->trigFlags |= 0x200000;
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party) {
            marioGetScreenPos(party->position, &sx, &sy, &sz);
            if (!(0.0f <= sx && sx <= 600.0f && 0.0f <= sy && sy <= 535.0f)) {
                party->position = mp->position;
            }
            if (mp->position.y > party->position.y) {
                party->position = mp->position;
            }
            N_partyFollowCloseOn(party);
            party->flags |= PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
            party->trigFlags |= 0x200000;
        }
        event->userdata[0] = 0;
        event->userdata[2] = 0;
    }

    ret = 0;
    switch (event->userdata[0]) {
        case 0:
            dist = distABf(mp->position.x, mp->position.z, x, z);
            mp->directionView = angleABf(mp->position.x, mp->position.z, x, z);
            if (mp->flags & MARIO_FLAG_BG_MODE) {
                spd *= 0.4f;
            }
            {
                s32 n = (f32)gp->framerate * (dist / spd);
                spd = spd / (f32)gp->framerate;
                marioSetForceMove(n, mp->directionView, spd);
            }
            event->userdata[0] = 2;
            break;
        case 2:
            if (mp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
                break;
            }
            event->userdata[1] = sysMsec2Frame(10);
            event->userdata[0] = 4;
        case 4:
            if (--event->userdata[1] > 0) {
                break;
            }
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                partyMoveBehindMario3(party, 0,
                    party->moveKind == 0
                        ? revise360(mp->targetYaw + ((mp->displayDirectionTarget >= 90.0f && mp->displayDirectionTarget <= 270.0f) ? 220.0f : 140.0f))
                        : revise360(mp->targetYaw + ((mp->displayDirectionTarget >= 90.0f && mp->displayDirectionTarget <= 270.0f) ? 225.0f : 135.0f)));
                if (party->moveKind == 1) {
                    party->flags2 |= 0x10;
                }
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party) {
                partyMoveBehindMario3(party, 0,
                    party->moveKind == 0
                        ? revise360(mp->targetYaw + ((mp->displayDirectionTarget >= 90.0f && mp->displayDirectionTarget <= 270.0f) ? 220.0f : 140.0f))
                        : revise360(mp->targetYaw + ((mp->displayDirectionTarget >= 90.0f && mp->displayDirectionTarget <= 270.0f) ? 225.0f : 135.0f)));
                if (party->moveKind == 1) {
                    party->flags2 |= 0x10;
                }
            }
            event->userdata[1] = sysMsec2Frame(400);
            event->userdata[0] = 6;
            break;
        case 6:
            party = partyGetPtr(marioGetPartyId());
            if (party && (party->flags & PARTY_FLAG_IS_BEHIND_MARIO)) {
                event->userdata[1] = 0;
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party && (party->flags & PARTY_FLAG_IS_BEHIND_MARIO)) {
                event->userdata[1] = 0;
            }
            if (--event->userdata[1] > 0) {
                break;
            }
            party = partyGetPtr(marioGetPartyId());
            if (party) {
                N_partyFollowCloseOff(party);
                party->flags &= ~PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
                party->trigFlags &= ~0x200000;
                if (party->moveKind == 1) {
                    partySetDispDirToMario(party);
                    party->flags2 &= ~0x10;
                }
            }
            party = partyGetPtr(marioGetExtraPartyId());
            if (party) {
                N_partyFollowCloseOff(party);
                party->flags &= ~PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
                party->trigFlags &= ~0x200000;
                if (party->moveKind == 1) {
                    partySetDispDirToMario(party);
                    party->flags2 &= ~0x10;
                }
            }
            ret = 2;
            break;
    }
    return ret;
}

USER_FUNC(evt_mario_jump_pos) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 t, y, frames, x, z, h;
    s32 msec;
    s32 noDir;
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    msec = evtGetValue(event, args[3]);
    noDir = evtGetValue(event, args[4]);
    h = evtGetFloat(event, args[5]);
    if (h <= -1000.0f) {
        h = 20.0f;
    }
    frames = sysMsec2Frame(msec);
    switch (event->userdata[0]) {
        case 0: {
            f32 dy;
            f32 g;
            f32 tmp;
            mp->baseSpeed = distABf(mp->position.x, mp->position.z, x, z) / frames;
            mp->directionView = angleABf(mp->position.x, mp->position.z, x, z);
            if (!noDir) {
                mp->moveDirection = mp->directionView;
            }
            dy = y - mp->position.y;
            t = 0.5f * frames;
            if (__fabsf(dy) < 1.0f) {
                g = (h + h) / (t * t - t);
                mp->jumpVelocityY = g * t;
                mp->jumpAccelerationY = -g;
                mp->jumpJerkY = 0.0f;
                mp->jumpSnapY = 0.0f;
            } else {
                if (dy >= 0.0f) {
                    if (mp->characterId == CHARACTER_BOWSER) {
                        dy += 18.5f;
                    } else {
                        dy = 0.5f * mp->wPlayerCollisionBox.y + dy;
                    }
                } else if (searchUnder2(&tmp, x, y, z)) {
                    dy = h;
                } else {
                    dy = h;
                }
                g = (dy + dy) / (t * t - t);
                mp->jumpVelocityY = g * t;
                mp->jumpAccelerationY = -g;
                mp->jumpJerkY = 0.0f;
                mp->jumpSnapY = 0.0f;
            }
            if (!(mp->flags & MARIO_FLAG_TUBE_MODE)) {
                marioChgMot(MARIO_MOTION_JUMP_3);
            } else {
                rollEvtJumpSetup();
                mp->displayDirection = mp->directionView;
                mp->displayDirectionTarget = mp->directionView;
            }
            event->userdata[2] = 0.5f + t;
            event->userdata[0] = 1;
        }
        case 1:
            if (mp->flags & MARIO_FLAG_TUBE_MODE) {
                setRollEvtFlag();
                movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            } else {
                movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            }
            mp->position.y += mp->jumpVelocityY;
            marioMakeJumpPara();
            if (--event->userdata[2] > 0) {
                break;
            }
            {
                f32 t2 = 0.5f * frames;
                f32 dy = y - mp->position.y;
                mp->jumpVelocityY = 0.0f;
                mp->jumpAccelerationY = (dy + dy) / (t2 * t2 - t2);
                mp->jumpJerkY = 0.0f;
                mp->jumpSnapY = 0.0f;
                event->userdata[2] = 0.5f + t2;
            }
            event->userdata[0] = 2;
            break;
        case 2:
            movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            marioMakeJumpPara();
            if (marioLandChk(mp->jumpVelocityY)) {
                event->userdata[2] = 0;
            } else {
                mp->position.y += mp->jumpVelocityY;
            }
            if (--event->userdata[2] > 0) {
                break;
            }
            mp->jumpVelocityY = 0.0f;
            mp->jumpAccelerationY = 0.0f;
            mp->jumpJerkY = 0.0f;
            mp->jumpSnapY = 0.0f;
            mp->baseSpeed = 0.0f;
            mp->baseSpeed = 0.0f;
            camFollowYOn();
            if (!(mp->flags & MARIO_FLAG_TUBE_MODE)) {
                marioChgMot(MARIO_MOTION_STAY);
            } else {
                marioResetRollSpd();
                clrRollEvtFlag();
            }
            return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_mario_jump_jumpstand) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 tx, ty, tz, x2, y2, z2, h, x1, y1, z1;
    s32 mode;
    s32 msec;
    f32 t, dy, g;

    evtGetFloat(event, args[0]);
    h = evtGetFloat(event, args[1]);
    evtGetFloat(event, args[2]);
    x2 = evtGetFloat(event, args[3]);
    y2 = evtGetFloat(event, args[4]);
    z2 = evtGetFloat(event, args[5]);
    x1 = evtGetFloat(event, args[6]);
    y1 = evtGetFloat(event, args[7]);
    z1 = evtGetFloat(event, args[8]);
    msec = evtGetValue(event, args[9]);
    mode = evtGetValue(event, args[10]);
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[3] = 0;
        if (mode == 3) {
            event->userdata[3] = 2;
        } else if (mp->fallStartPos.y < mp->position.y) {
            event->userdata[3] = 2;
        } else if (mp->fallStartPos.y <= 1.5f * mp->wPlayerCollisionBox.y + h) {
            event->userdata[3] = 2;
        } else {
            event->userdata[3] = 1;
        }
        gp->systemLevelFlags |= 0x10;
    }
    if (event->userdata[3] & 1) {
        tx = x1;
        ty = y1;
        tz = z1;
    } else if (event->userdata[3] & 2) {
        tx = x2;
        ty = y2;
        tz = z2;
    }

    switch (event->userdata[0]) {
        case 0:
            mp->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
            if (marioChkSts(0x10)) {
                effKemuriEntry(0, mp->position.x, mp->position.y, mp->position.z, 0.0f);
            }
            marioChgMot(MARIO_MOTION_JUMP_3);
            if (event->userdata[3] & 1) {
                msec = 550;
            }
            event->userdata[1] = sysMsec2Frame(msec);
            mp->baseSpeed = distABf(mp->position.x, mp->position.z, tx, tz) / (f32)event->userdata[1];
            mp->directionView = angleABf(mp->position.x, mp->position.z, tx, tz);
            mp->moveDirection = mp->directionView;
            dy = ty - mp->position.y;
            t = 0.5f * (f32)event->userdata[1];
            if (__fabsf(dy) < 1.0f) {
                dy += 20.0f;
            } else if (dy >= 0.0f) {
                dy = 0.5f * mp->wPlayerCollisionBox.y + dy;
            } else {
                dy = 20.0f;
            }
            g = (dy + dy) / (t * t - t);
            mp->jumpVelocityY = g * t;
            mp->jumpAccelerationY = -g;
            mp->jumpJerkY = 0.0f;
            mp->jumpSnapY = 0.0f;
            if (event->userdata[3] & 2) {
                event->userdata[0] = 10;
            }
            event->userdata[2] = 0.5f * (f32)event->userdata[1] + 0.5f;
            if (event->userdata[0] == 10) {
                break;
            }
            event->userdata[0] = 1;
        case 1:
            movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            mp->position.y += mp->jumpVelocityY;
            marioMakeJumpPara();
            if (--event->userdata[2] > 0) {
                break;
            }
            {
                f32 t2 = 0.5f * (f32)event->userdata[1];
                f32 dy2 = ty - mp->position.y;
                mp->jumpVelocityY = 0.0f;
                mp->jumpAccelerationY = (dy2 + dy2) / (t2 * t2 - t2);
                mp->jumpJerkY = 0.0f;
                mp->jumpSnapY = 0.0f;
                event->userdata[2] = 0.5f * (f32)event->userdata[1] + 0.5f;
            }
            event->userdata[0] = 2;
            if (!(mp->flags & MARIO_FLAG_TUBE_MODE)) {
                marioChgPose("M_J_1C");
            }
            break;
        case 2:
            movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            marioMakeJumpPara();
            if (marioLandChk(mp->jumpVelocityY)) {
                event->userdata[2] = 0;
            } else {
                mp->position.y += mp->jumpVelocityY;
            }
            if (--event->userdata[2] > 0) {
                break;
            }
            event->userdata[0] = 20;
            break;
    }

    switch (event->userdata[0]) {
        case 10:
            if (mp->position.y < y2) {
                if (!(mode & 1)) {
                    movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
                }
                mp->position.y += mp->jumpVelocityY;
                marioMakeJumpPara();
                event->userdata[2]--;
                break;
            }
            event->userdata[2] = 54;
            event->userdata[1] = event->userdata[2] / 2;
            camFollowYOff();
            ty = y2;
            mp->baseSpeed = distABf(mp->position.x, mp->position.z, x2, z2) / (f32)event->userdata[1];
            mp->directionView = angleABf(mp->position.x, mp->position.z, x2, z2);
            mp->moveDirection = mp->directionView;
            t = 0.5f * (f32)event->userdata[1];
            g = 20.0f / (t * t - t);
            mp->jumpVelocityY = g * t;
            mp->jumpAccelerationY = -g;
            mp->jumpJerkY = 0.0f;
            mp->jumpSnapY = 0.0f;
            event->userdata[2] = 0.5f * (f32)event->userdata[1] + 0.5f;
            event->userdata[0] = 11;
        case 11:
            movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            mp->position.y += mp->jumpVelocityY;
            marioMakeJumpPara();
            if (--event->userdata[2] > 0) {
                break;
            }
            {
                f32 t2 = 0.5f * (f32)event->userdata[1];
                f32 dy2 = ty - mp->position.y;
                mp->jumpVelocityY = 0.0f;
                mp->jumpAccelerationY = (dy2 + dy2) / (t2 * t2 - t2);
                mp->jumpJerkY = 0.0f;
                mp->jumpSnapY = 0.0f;
                event->userdata[2] = 0.5f * (f32)event->userdata[1] + 0.5f;
            }
            event->userdata[0] = 12;
            marioChgPose("M_J_1C");
            break;
        case 12:
            movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            marioMakeJumpPara();
            if (marioLandChk(mp->jumpVelocityY)) {
                event->userdata[2] = 0;
            } else {
                mp->position.y += mp->jumpVelocityY;
            }
            if (--event->userdata[2] > 0) {
                break;
            }
            event->userdata[0] = 20;
            break;
    }

    if (event->userdata[0] == 20) {
        mp->jumpVelocityY = 0.0f;
        mp->jumpAccelerationY = 0.0f;
        mp->jumpJerkY = 0.0f;
        mp->jumpSnapY = 0.0f;
        mp->baseSpeed = 0.0f;
        mp->baseSpeed = 0.0f;
        marioChgMot(MARIO_MOTION_STAY);
        mp->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
        marioAdjustMoveDir();
        camFollowYOn();
        gp->systemLevelFlags &= ~0x10;
        return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_mario_wait_landon) {
    if (marioGetPtr()->hitobjStandOn && marioChkLanding()) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_mario_wait_rideon) {
    if (marioGetPtr()->hitobjStandOn && marioChkLanding()) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_mario_check_landon) {
    EvtWord* args = event->args;
    if (marioGetPtr()->hitobjStandOn && marioChkLanding()) {
        evtSetValue(event, args[0], 1);
        return EVT_RETURN_YIELD;
    }
    evtSetValue(event, args[0], 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_koopa_hip_attack) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    f32 x, y, z, h, frames;
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    h = evtGetFloat(event, args[3]);
    frames = sysMsec2Frame(evtGetValue(event, args[4]));
    switch (event->userdata[0]) {
        case 0: {
            f32 dy;
            f32 g;
            camFollowYOff();
            mp->baseSpeed = distABf(mp->position.x, mp->position.z, x, z) / frames;
            mp->directionView = angleABf(mp->position.x, mp->position.z, x, z);
            mp->moveDirection = mp->directionView;
            dy = y - mp->position.y;
            if (__fabsf(dy) < 1.0f) {
                dy += h;
            } else if (dy >= 0.0f) {
                dy += 18.5f;
            } else {
                dy = h;
            }
            g = (dy + dy) / (frames * frames - frames);
            mp->jumpVelocityY = g * frames;
            mp->jumpAccelerationY = -g;
            mp->jumpJerkY = 0.0f;
            mp->jumpSnapY = 0.0f;
            marioChgMot(MARIO_MOTION_JUMP_3);
            event->userdata[2] = frames;
            event->userdata[0] = 1;
        }
        case 1:
            movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            mp->position.y += mp->jumpVelocityY;
            marioMakeJumpPara();
            if (--event->userdata[2] > 0) {
                break;
            }
            mp->baseSpeed = 0.0f;
            mp->jumpAccelerationY = -0.4744f;
            mp->jumpJerkY = 0.4704f;
            mp->jumpSnapY = -0.1904f;
            mp->jumpVelocityY = mp->jumpAccelerationY;
            kpaChgPose("KPA_A_2D", "KPA2_J_1C");
            event->userdata[0] = 2;
            break;
        case 2:
            movePos(&mp->position.x, &mp->position.z, mp->baseSpeed, mp->directionView);
            marioMakeJumpPara();
            if (!marioLandChk(mp->jumpVelocityY)) {
                mp->position.y += mp->jumpVelocityY;
                break;
            }
            quake_kpaHipAttack();
            event->userdata[0] = 3;
            event->userdata[2] = 10;
            break;
        case 3:
            if (--event->userdata[2] > 0) {
                break;
            }
            camFollowYOn();
            marioChgMot(MARIO_MOTION_STAY);
            return EVT_RETURN_YIELD;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_koopa_fire) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    if (isFirstCall) {
        event->userdata[0] = 0;
        event->userdata[2] = (f32)sysMsec2Frame(evtGetValue(event, args[0]));
        kpaAttackStart();
        marioChgMot(MARIO_MOTION_STAY);
    }
    switch (event->userdata[0]) {
        case 0:
            if (--event->userdata[2] != 0) {
                mp->kpaWork->fireHold = 1;
                return EVT_RETURN_BLOCK;
            }
    }
    mp->kpaWork->fireHold = 0;
    return EVT_RETURN_YIELD;
}

USER_FUNC(evt_koopa_chk_dead) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    evtSetValue(event, args[0], (mp->kpaWork->flags >> 28) & 1);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_koopa_weary) {
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    if (args[0]) {
        mp->kpaWork->flags |= 0x1000000;
    } else {
        mp->kpaWork->flags &= ~0x1000000;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_wait_move_end) {
    return marioGetPtr()->forceMoveTimer ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_wait_movable) {
    return (marioGetPtr()->flags & MARIO_FLAG_KEY_OFF) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_state) {
    EvtWord id;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    s32 v;
    id = *args;
    if (marioChkSlitThrouhEnd()) {
        evtSetValue(event, id, 2);
        return EVT_RETURN_DONE;
    }
    v = 1;
    if (mp->currMotionId == MARIO_MOTION_STAY) {
        v = 0;
    }
    evtSetValue(event, id, v);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_mode) {
    s32 id = event->args[0];
    evtSetValue(event, id, marioGetPtr()->characterId);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_mode) {
    marioSetCharMode(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_koopa_get_level) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], kpaGetLevel());
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_koopa_set_level) {
    kpaSetLevel(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_peach_transform_gundan_on) {
    peachTransformOn();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_peach_transform_gundan_off) {
    peachTransformOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_peach_set_condition) {
    switch (evtGetValue(event, event->args[0])) {
        case 0: peachSetStsNormal(); break;
        case 1: peachSetStsAftershower(); break;
        case 2: peachSetStsInvisible(); break;
        case 3: peachSetStsHappy(); break;
        case 4: peachSetStsUneasy(); break;
        case 5: peachSetStsAngry(); break;
        case 6: peachSetStsOrdinary(); break;
        case 7: peachSetStsCarry(); break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_party) {
    s32 id = event->args[0];
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    evtSetValue(event, id, party ? party->currentMemberId : 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_get_exparty) {
    s32 id = event->args[0];
    PartyEntry* party = partyGetPtr(marioGetExtraPartyId());
    evtSetValue(event, id, party ? party->currentMemberId : 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_chk_join_party) {
    EvtWord* args = event->args;
    s32 id = args[0];
    evtSetValue(event, id, partyChkJoin(evtGetValue(event, args[1])));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_party_use_check) {
    s32 id = event->args[0];
    s32 val = 0;
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        val = party->currentMemberId;
    }
    evtSetValue(event, id, val);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_prev_party_dokan) {
    s32 slot;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    slot = args[0];
    if (isFirstCall) {
        s32 id;
        PartyEntry* party;
        s32 prev;
        if (mp->currPartySlotId[slot] >= 0) {
            return EVT_RETURN_DONE;
        }
        prev = mp->prevPartyMemberId[slot];
        if (!prev) {
            return EVT_RETURN_DONE;
        }
        if (slot == 0) {
            id = partyEntry(prev);
        } else {
            id = partyEntry2(prev);
        }
        if (id >= 0) {
            mp->currPartySlotId[slot] = id;
            party = partyGetPtr(id);
            partyChgRunMode(party, 0);
            party->flags |= PARTY_FLAG_DISABLE_FLOORS;
            party->flags |= PARTY_FLAG_FORCE_FALL;
            party->scale = (Vec){0.0f, 0.0f, 0.0f};
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_hello_party) {
    s32 slot;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    PartyEntry* party;
    slot = args[0];
    if (isFirstCall) {
        s32 id;
        s32 member;
        if (mp->currPartySlotId[slot] >= 0) {
            return EVT_RETURN_DONE;
        }
        member = evtGetValue(event, args[1]);
        if (slot == 0) {
            id = partyEntryHello(member);
        } else {
            id = partyEntry2Hello(member);
        }
        if (id < 0) {
            return EVT_RETURN_DONE;
        }
        mp->currPartySlotId[slot] = id;
        return EVT_RETURN_BLOCK;
    }
    party = partyGetPtr(slot);
    if (!party) {
        return EVT_RETURN_DONE;
    }
    if (!(party->flags & PARTY_FLAG_IS_ACTIVE) || party->runMode != 1) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_hello_exparty_pos) {
    s32 member;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    PartyEntry* party;
    if (isFirstCall) {
        s32 id;
        if (mp->currPartySlotId[1] >= 0) {
            return EVT_RETURN_DONE;
        }
        member = evtGetValue(event, args[0]);
        evtGetValue(event, args[1]);
        evtGetValue(event, args[2]);
        evtGetValue(event, args[3]);
        id = partyEntry2Hello(member);
        if (id < 0) {
            return EVT_RETURN_DONE;
        }
        mp->currPartySlotId[1] = id;
        return EVT_RETURN_BLOCK;
    }
    party = partyGetPtr(1);
    if (!party) {
        return EVT_RETURN_DONE;
    }
    if (!(party->flags & PARTY_FLAG_IS_ACTIVE) || party->runMode != 1) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_party) {
    s32 slot;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    slot = args[0];
    if (isFirstCall) {
        s32 id;
        s32 member;
        if (mp->currPartySlotId[slot] >= 0) {
            return EVT_RETURN_DONE;
        }
        member = evtGetValue(event, args[1]);
        if (slot == 0) {
            id = partyEntry(member);
        } else {
            id = partyEntry2(member);
        }
        if (id < 0) {
            return EVT_RETURN_DONE;
        }
        mp->currPartySlotId[slot] = id;
    }
    if (!partyGetPtr(mp->currPartySlotId[slot])) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_party_pos) {
    s32 slot;
    s32 member;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    slot = args[0];
    if (isFirstCall) {
        f32 x, y, z;
        s32 id;
        if (mp->currPartySlotId[slot] >= 0) {
            return EVT_RETURN_DONE;
        }
        member = evtGetValue(event, args[1]);
        x = evtGetValue(event, args[2]);
        y = evtGetValue(event, args[3]);
        z = evtGetValue(event, args[4]);
        if (slot == 0) {
            id = partyEntryPos(member, x, y, z);
        } else {
            id = partyEntry2Pos(member, x, y, z);
        }
        if (id < 0) {
            return EVT_RETURN_DONE;
        }
        mp->currPartySlotId[slot] = id;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_prev_party) {
    s32 slot;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    slot = args[0];
    if (isFirstCall) {
        s32 id;
        s32 prev;
        if (mp->currPartySlotId[slot] >= 0) {
            return EVT_RETURN_DONE;
        }
        prev = mp->prevPartyMemberId[slot];
        if (!prev) {
            return EVT_RETURN_DONE;
        }
        if (slot == 0) {
            id = partyEntry(prev);
        } else {
            id = partyEntry2(prev);
        }
        if (id < 0) {
            return EVT_RETURN_DONE;
        }
        mp->currPartySlotId[slot] = id;
    }
    if (!partyGetPtr(mp->currPartySlotId[slot])) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_set_prev_party_pos) {
    s32 slot;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    slot = args[0];
    if (isFirstCall) {
        f32 x, y, z;
        s32 id;
        s32 prev;
        if (mp->currPartySlotId[slot] >= 0) {
            return EVT_RETURN_DONE;
        }
        x = evtGetValue(event, args[1]);
        y = evtGetValue(event, args[2]);
        z = evtGetValue(event, args[3]);
        prev = mp->prevPartyMemberId[slot];
        if (!prev) {
            return EVT_RETURN_DONE;
        }
        if (slot == 0) {
            id = partyEntryPos(prev, x, y, z);
        } else {
            id = partyEntry2Pos(prev, x, y, z);
        }
        if (id < 0) {
            return EVT_RETURN_DONE;
        }
        mp->currPartySlotId[slot] = id;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_goodbye_party) {
    MarioWork* mp = marioGetPtr();
    s32 slot = event->args[0];
    if (isFirstCall) {
        if (mp->currPartySlotId[slot] < 0) {
            return EVT_RETURN_DONE;
        }
        partyGoodbye(mp->currPartySlotId[slot]);
    }
    if (mp->currPartySlotId[slot] >= 0) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_kill_party) {
    MarioWork* mp = marioGetPtr();
    s32 slot = event->args[0];
    s8 id = mp->currPartySlotId[slot];
    if (id < 0) {
        return EVT_RETURN_DONE;
    }
    if (slot == 0) {
        partyKill(id);
    } else {
        partyKill(1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_clear_party) {
    MarioWork* mp = marioGetPtr();
    s32 slot = event->args[0];
    s8 id = mp->currPartySlotId[slot];
    if (id < 0) {
        return EVT_RETURN_DONE;
    }
    if (slot == 0) {
        partyKill(id);
    } else {
        partyKill2(1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_push) {
    marioGetPtr()->flags |= MARIO_FLAG_IS_PUSHING;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_sleep_on) {
    marioChgEvtPose("M_N_5A");
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_sleep_off) {
    if (isFirstCall) {
        marioChgEvtPose("M_N_5B");
        event->userdata[0] = 32;
    }
    if (--event->userdata[0] > 0) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_check_key_item) {
    EvtWord* args = event->args;
    s32 item = evtGetValue(event, args[0]);
    s32 id = args[1];
    if (pouchCheckItem(item)) {
        evtSetValue(event, id, 1);
    } else {
        evtSetValue(event, id, 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_dump_item) {
    pouchRemoveItem(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

HITOBJ(head, hitobjHead)

HITOBJ(ride, hitobjStandOn)

HITOBJ(under, hitobjUnder)

HITOBJ(front, hitobjFront)

HITOBJ(push, hitobjPush)

USER_FUNC(L_evt_mario_keyon_wait) {
    marioGetPtr();
    return marioKeyOffChk() ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(evt_mario_paper_plane) {
    s32 mode;
    EvtWord* args = event->args;
    MarioWork* mp = marioGetPtr();
    mode = args[0];

    if (isFirstCall) {
        if (!mode) {
            mario_plane_cancel();
        } else {
            marioChgMot(MARIO_MOTION_PLANE);
            mp->trigFlags |= MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON;
        }
    }
    if (!mode) {
        if (mp->currMotionId == MARIO_MOTION_STAY && mp->hitobjStandOn) {
            mp->trigFlags &= ~MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON;
            return EVT_RETURN_DONE;
        }
    } else if (marioGetPlaneStatus() == 2) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}
