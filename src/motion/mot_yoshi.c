// Mario's riding-Yoshi motion: clears his motion state when he gets on Yoshi (party_yoshi.c moves them both).

#include "motion/mot_yoshi.h"
#include "evt/evt_bero.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "party.h"
#include "pmario_sound.h"

/**
 * Motion while Mario rides Yoshi: on entry clears his jump flags, speed and every collision hit,
 * and sets the front pose without a shadow; party_yoshi.c then moves and animates both of them.
 */
void mot_yoshi(void) {
    MarioWork* mario = marioGetPtr();

    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->dispFlags |= MARIO_DISP_FLAG_NO_REAR_POSE;
        mario->dispFlags |= MARIO_DISP_FLAG_SHADOW_OFF;
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        mario->hitobjFrontA = NULL;
        mario->hitobjFront = NULL;
        mario->hitobjStandOn = NULL;
        mario->hitobjPush = NULL;
        mario->hitobjWall = NULL;
        mario->hitobjHead = NULL;
        mario->hitobjUnder = NULL;
    }
}

/**
 * Cleanup when Mario gets off Yoshi: stops Yoshi's running sound,
 * clears the door (bero) offsets and restores Mario's pose, shadow and draw offsets.
 */
void mot_yoshi_post(void) {
    PartyEntry* party;
    MarioWork* mario;

    party = partyGetPtr(marioGetPartyId());
    mario = marioGetPtr();
    if (party->yoshiPsndSFXId != -1) {
        psndSFXOff(party->yoshiPsndSFXId);
        party->yoshiPsndSFXId = -1;
    }
    bero_clear_Offset();
    mario->dispFlags &= ~MARIO_DISP_FLAG_NO_REAR_POSE;
    mario->dispFlags &= ~MARIO_DISP_FLAG_SHADOW_OFF;
    mario->dispOfs = (Vec){ 0.0f, 0.0f, 0.0f };
    mario->dispOfs2 = (Vec){ 0.0f, 0.0f, 0.0f };
}
