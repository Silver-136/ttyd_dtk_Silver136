// Script commands for the battle audience: audience flags, positions and reactions.

#include "evt/evt_audience.h"
#include "battle/battle_audience.h"
#include "system.h"
#include "manager/evtmgr_cmd.h"
double sqrt(double);

/** s32* mask */
//s32* mask
USER_FUNC(evt_audience_base_flag_on) {
    BattleAudience* audience;
    s32 mask;

    mask = evtGetValue(event, *event->args);
    audience = BattleAudienceBaseGetPtr();
    audience->flags |= mask;
    return EVT_RETURN_DONE;
}

/** s32* memberId, s32* flags, s32 retIndex */
//s32* memberId, s32* flags, s32 retIndex
USER_FUNC(evt_audience_flag_check) {
    EvtWord* args = event->args;
    s32 id, flags;
    u32 fl;

    id = evtGetValue(event, args[0]);
    flags = evtGetValue(event, args[1]);
    fl = BattleAudienceGetPtr(id)->flags;
    if (fl & flags) {
        evtSetValue(event, args[2], TRUE);
    } else {
        evtSetValue(event, args[2], FALSE);
    }
    return EVT_RETURN_DONE;
}

/** s32* memberId, s32* flags */
//s32* memberId, s32* flags
USER_FUNC(evt_audience_flag_on) {
    EvtWord* args = event->args;
    s32 id, flags;

    id = evtGetValue(event, args[0]);
    flags = evtGetValue(event, args[1]);
    BattleAudienceGetPtr(id)->flags |= flags;
    return EVT_RETURN_DONE;
}

/** s32* memberId, s32* flags */
//s32* memberId, s32* flags
USER_FUNC(evt_audience_flag_off) {
    EvtWord* args = event->args;
    s32 id, flags;

    id = evtGetValue(event, args[0]);
    flags = evtGetValue(event, args[1]);
    BattleAudienceGetPtr(id)->flags &= ~flags;
    return EVT_RETURN_DONE;
}

/** s32* memberId, f32* x, f32* y, f32* z */
//s32* memberId, f32* x, f32* y, f32* z
USER_FUNC(evt_audience_set_position) {
    EvtWord* args = event->args;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    BattleAudience_SetPosition(id, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* memberId, s32 retX, s32 retY, s32 retZ */
//s32* memberId, s32 retX, s32 retY, s32 retZ
USER_FUNC(evt_audience_get_position) {
    BattleAudienceMember* member;
    EvtWord* args = event->args;
    s32 id;

    id = evtGetValue(event, args[0]);
    member = BattleAudienceGetPtr(id);
    evtSetFloat(event, args[1], member->position.x);
    evtSetFloat(event, args[2], member->position.y);
    evtSetFloat(event, args[3], member->position.z);
    return EVT_RETURN_DONE;
}

/** s32* memberId, f32* x, f32* y, f32* z */
//s32* memberId, f32* x, f32* y, f32* z
USER_FUNC(evt_audience_set_rotate) {
    EvtWord* args = event->args;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    BattleAudience_SetRotate(id, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* memberId, f32* x, f32* y, f32* z */
//s32* memberId, f32* x, f32* y, f32* z
USER_FUNC(evt_audience_set_rotate_offset) {
    EvtWord* args = event->args;
    f32 x, y, z;
    s32 id;

    id = evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    BattleAudience_SetRotateOffset(id, x, y, z);
    return EVT_RETURN_DONE;
}

/** s32* memberId, f32* targetX, f32* targetY, f32* targetZ, s32* interpMode, s32* numFrames */
//s32* memberId, f32* targetX, f32* targetY, f32* targetZ, s32* interpMode, s32* numFrames
USER_FUNC(evt_audience_move_position_frame) {
    EvtWord* args = event->args;
    f32 targetX, targetY, targetZ;
    BattleAudienceMember* member;
    s32 id, mode, frames;
    f32 compare, x, y, z;

    id = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetY = evtGetFloat(event, args[2]);
    targetZ = evtGetFloat(event, args[3]);
    mode = evtGetValue(event, args[4]);
    frames = evtGetValue(event, args[5]);
    member = BattleAudienceGetPtr(id);
    if (isFirstCall) {
        member->startPosX = member->position.x;
        member->startPosY = member->position.y;
        member->startPosZ = member->position.z;
        member->targetPosX = targetX;
        member->targetPosY = targetY;
        member->targetPosZ = targetZ;
        member->interpType = mode;
        member->interpFrameCounter = frames;
        member->interpFramesTotal = frames;
    }
    compare = member->targetPosX - member->startPosX;
    if (compare > 0.0f) {
        member->angle = 180.0f;
    } else if (compare < 0.0f) {
        member->angle = 0.0f;
    }
    x = intplGetValue(member->interpType, member->startPosX, member->targetPosX, member->interpFramesTotal - member->interpFrameCounter, member->interpFramesTotal);
    y = intplGetValue(member->interpType, member->startPosY, member->targetPosY, member->interpFramesTotal - member->interpFrameCounter, member->interpFramesTotal);
    z = intplGetValue(member->interpType, member->startPosZ, member->targetPosZ, member->interpFramesTotal - member->interpFrameCounter, member->interpFramesTotal);
    BattleAudience_SetPosition(id, x, y, z);
    member->interpFrameCounter--;
    if (member->interpFrameCounter <= 0) {
        BattleAudience_SetPosition(id, member->targetPosX, member->targetPosY, member->targetPosZ);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** s32* memberId, f32* targetX, f32* targetY, f32* targetZ, f32* interpSpeed */
//s32* memberId, f32* targetX, f32* targetY, f32* targetZ, f32* interpSpeed
USER_FUNC(evt_audience_move_position_speed) {
    EvtWord* args = event->args;
    BattleAudienceMember* member;
    f32 targetX, targetY, targetZ;
    f32 amountX, amountY, amountZ;
    f32 speed, delta, compare;
    s32 id, frames;

    id = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetY = evtGetFloat(event, args[2]);
    targetZ = evtGetFloat(event, args[3]);
    speed = evtGetFloat(event, args[4]);
    member = BattleAudienceGetPtr(id);
    if (isFirstCall) {
        member->targetPosX = targetX;
        member->targetPosY = targetY;
        member->targetPosZ = targetZ;
        amountX = targetX - member->position.x;
        amountY = targetY - member->position.y;
        amountZ = targetZ - member->position.z;
        delta = (f32)sqrt((amountX * amountX) + (amountY * amountY) + (amountZ * amountZ));

        frames = (s32)(delta / speed);
        if (amountX != 0.0f) {
            member->interpDelta.x = speed * (amountX / delta);
        } else {
            member->interpDelta.x = 0.0f;
        }

        if (amountY != 0.0f) {
            member->interpDelta.y = speed * (amountY / delta);
        } else {
            member->interpDelta.y = 0.0f;
        }

        if (amountZ != 0.0f) {
            member->interpDelta.z = speed * (amountZ / delta);
        } else {
            member->interpDelta.z = 0.0f;
        }
        member->interpFrameCounter = frames;
        member->interpFramesTotal = frames;
    }

    compare = member->interpDelta.x;
    if (compare > 0.0f) {
        member->angle = 180.0f;
    } else if (compare < 0.0f) {
        member->angle = 0.0f;
    }

    BattleAudience_SetPosition(
        id,
        member->position.x + member->interpDelta.x,
        member->position.y + member->interpDelta.y,
        member->position.z + member->interpDelta.z);

    if (member->interpFrameCounter > 0) {
        member->interpFrameCounter--;
    }
    if (member->interpFrameCounter < 0) {
        member->interpFrameCounter++;
    }

    if (member->interpFrameCounter == 0) {
        BattleAudience_SetPosition(id, member->targetPosX, member->targetPosY, member->targetPosZ);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** s32* memberId, f32* targetX, f32* targetY, f32* targetZ, s32* numFrames, f32* deltaY */
//s32* memberId, f32* targetX, f32* targetY, f32* targetZ, s32* numFrames, f32* deltaY
USER_FUNC(evt_audience_jump_position_firstsp) {
    EvtWord* args = event->args;
    BattleAudienceMember* member;
    f32 targetX, targetY, targetZ;
    f32 amountX, amountY, amountZ;
    f32 deltaY;
    s32 id, frames;

    id = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetY = evtGetFloat(event, args[2]);
    targetZ = evtGetFloat(event, args[3]);
    frames = evtGetValue(event, args[4]);
    deltaY = evtGetFloat(event, args[5]);
    member = BattleAudienceGetPtr(id);
    if (isFirstCall) {
        amountX = targetX - member->position.x;
        amountY = targetY - member->position.y;
        amountZ = targetZ - member->position.z;
        member->targetPosX = targetX;
        member->targetPosY = targetY;
        member->targetPosZ = targetZ;
        member->interpDelta.x = amountX / (f32)frames;
        member->interpDelta.y = deltaY;
        member->interpDelta.z = amountZ / (f32)frames;
        member->interpGravity = (2.0f * -((deltaY * (f32)frames) - amountY)) / (f32)(frames * frames);
        member->interpFrameCounter = frames;
        member->interpFramesTotal = frames;
    }

    BattleAudience_SetPosition(
        id,
        member->position.x + member->interpDelta.x,
        member->position.y + member->interpDelta.y,
        member->position.z + member->interpDelta.z);

    member->interpDelta.y += member->interpGravity;
    member->interpFrameCounter--;
    if (member->interpFrameCounter <= 0) {
        BattleAudience_SetPosition(id, member->targetPosX, member->targetPosY, member->targetPosZ);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** s32* memberId, f32* targetX, f32* targetY, f32* targetZ, s32* numFrames, f32* gravity */
//s32* memberId, f32* targetX, f32* targetY, f32* targetZ, s32* numFrames, f32* gravity
USER_FUNC(evt_audience_jump_position_gravity) {
    EvtWord* args = event->args;
    BattleAudienceMember* member;
    f32 targetX, targetY, targetZ;
    f32 amountX, amountY, amountZ;
    s32 id, frames;
    f32 gravity, deltaY;

    id = evtGetValue(event, args[0]);
    targetX = evtGetFloat(event, args[1]);
    targetY = evtGetFloat(event, args[2]);
    targetZ = evtGetFloat(event, args[3]);
    frames = evtGetValue(event, args[4]);
    gravity = evtGetFloat(event, args[5]);
    member = BattleAudienceGetPtr(id);
    if (isFirstCall) {
        amountX = targetX - member->position.x;
        amountY = targetY - member->position.y;
        amountZ = targetZ - member->position.z;
        deltaY = 0.5f * gravity;
        member->targetPosX = targetX;
        member->targetPosY = targetY;
        member->targetPosZ = targetZ;
        member->interpDelta.x = amountX / (f32)frames;
        member->interpDelta.y = amountY / (f32)frames - (deltaY * (f32)frames);
        member->interpDelta.z = amountZ / (f32)frames;
        member->interpGravity = gravity;
        member->interpFrameCounter = frames;
        member->interpFramesTotal = frames;
    }

    BattleAudience_SetPosition(
        id,
        member->position.x + member->interpDelta.x,
        member->position.y + member->interpDelta.y,
        member->position.z + member->interpDelta.z);

    member->interpDelta.y += member->interpGravity;
    member->interpFrameCounter--;
    if (member->interpFrameCounter <= 0) {
        BattleAudience_SetPosition(id, member->targetPosX, member->targetPosY, member->targetPosZ);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** s32* memberId, s32* animId, BOOL* force */
//s32* memberId, s32* animId, BOOL* force
USER_FUNC(evt_audience_set_animpose) {
    EvtWord* args = event->args;
    s32 id, animId;
    BOOL force;

    id = evtGetValue(event, args[0]);
    animId = evtGetValue(event, args[1]);
    force = evtGetValue(event, args[2]);
    BattleAudience_SetAnim(id, animId, force);
    return EVT_RETURN_DONE;
}

/**
 * Adds audience member args[0] as AudienceMemberType args[1];
 * when the entry succeeds it is put straight into animation 1.
 */
USER_FUNC(evt_audience_entry) {
    EvtWord* args = event->args;
    AudienceMemberType type;
    s32 id;

    id = evtGetValue(event, args[0]);
    type = (AudienceMemberType)evtGetValue(event, args[1]);
    if (BattleAudience_Entry(id, type, AUDIENCE_STATUS_WAIT) == TRUE) {
        BattleAudience_SetAnim(id, AUD_ANIM_WAIT_BACK, TRUE);
    }
    return EVT_RETURN_DONE;
}

/** Removes audience member args[0] from the crowd. */
USER_FUNC(evt_audience_delete) {
    s32 id = evtGetValue(event, *event->args);
    BattleAudience_Delete(id);
    return EVT_RETURN_DONE;
}

/**
 * Does nothing in every version, though Mario's battle scripts still call it after his attacks.
 * Guess: a leftover hook for crowd-driven Star Power refills.
 */
USER_FUNC(evt_audience_ap_recovery) {
    return EVT_RETURN_DONE;
}

/**
 * Wakes sleeping members: gives every member in status 13 (asleep) at sub-step 1 an args[0]% chance to move on
 * to sub-step 2, where BattleAudienceCtrlProcessKinopioSleep hops them awake and back to idle.
 * args[1] and args[2] are read but unused. Called by the quake hammers and the POW Block.
 */
USER_FUNC(evt_audience_reflesh_natural_all) {
    EvtWord* args = event->args;
    BattleAudienceMember* member;
    s32 chance, i;

    chance = evtGetValue(event, args[0]);
    evtGetValue(event, args[1]); //unused
    evtGetValue(event, args[2]); //unused
    i = 0;
    member = BattleAudienceGetPtr(0);
    for (; i < AUDIENCE_MEMBER_MAX; i++, member++) {
        if (member->flags & AUDIENCE_MEMBER_FLAG_USED && member->status == AUDIENCE_STATUS_SLEEP && member->step == 1 && irand(100) < chance) {
            member->step = 2;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Puts every active member in status 0-7 to sleep (status 13, BattleAudienceCtrlProcessKinopioSleep)
 * with an args[0]% chance; args[1] and args[2] are unused.
 * Called by the Clock Out special move (sac_bakugame).
 */
USER_FUNC(evt_audience_abnormal_natural_all) {
    EvtWord* args = event->args;
    BattleAudienceMember* member;
    s32 chance, i;
    u8 status;

    chance = evtGetValue(event, args[0]);
    evtGetValue(event, args[1]); //unused
    evtGetValue(event, args[2]); //unused
    i = 0;
    member = BattleAudienceGetPtr(0);
    for (; i < AUDIENCE_MEMBER_MAX; i++, member++) {
        if ((u8)BattleAudience_GetSysCtrl(i)) {
            status = member->status;
            if ((status <= AUDIENCE_STATUS_WAIT_JUMP || (u8)(status - AUDIENCE_STATUS_JOY_RAISE_ARM) <= 2 || status == AUDIENCE_STATUS_ITEM_ON) && irand(100) < chance) {
                BattleAudience_ChangeStatus(i, AUDIENCE_STATUS_SLEEP);
            }
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Plays the crowd cheer sized to the current audience (BattleAudienceSoundCheer):
 * args[0] duration, args[1] fade time. The default battle events call it with 45, 25.
 */
USER_FUNC(evt_audience_sound_cheer) {
    EvtWord* args = event->args;
    s32 a, b;

    a = evtGetValue(event, args[0]);
    b = evtGetValue(event, args[1]);
    BattleAudienceSoundCheer(a, b);
    return EVT_RETURN_DONE;
}

/**
 * Adds the float args[0] to the audience's added-member count (BattleAudienceAddTargetNum with no direct
 * change), so events can raise or lower how many spectators turn up.
 */
USER_FUNC(evt_audience_num_updown_in_event) {
    f32 v = evtGetFloat(event, *event->args);
    BattleAudienceAddTargetNum(0.0f, v);
    return EVT_RETURN_DONE;
}

/**
 * Run when Mario skips the Stylish command.
 * Up to the US demo it clears AUDIENCE_FLAG_STYLISH_MISSED; the later releases do nothing here.
 */
USER_FUNC(evt_audience_acrobat_notry) {
    BattleAudience_Case_AcrobatNoTry();
    return EVT_RETURN_DONE;
}

/** Sets the whole crowd cheering with the ending reaction (BattleAudienceJoy type 3). */
USER_FUNC(evt_audience_joy_ending) {
    BattleAudienceJoyEnding();
    return EVT_RETURN_DONE;
}
