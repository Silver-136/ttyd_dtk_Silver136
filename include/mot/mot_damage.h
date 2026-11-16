// mot_damage.c: Mario's damage motion (mot_damage, mot_damageToge), splash and quake helpers.

#pragma once

#include "dolphin/types.h"
#include "dolphin/mtx.h"

// MarioWork.currSubMotionId in mot_damage (water) and mot_damageToge (spikes); names from what each step does
enum {
    DAMAGE_SUB_START = 0,             // water: sinks 160 units below the surface; spikes: 5 frames, then the hurt pose
    DAMAGE_SUB_UNDERWATER = 1,        // water: 40 frames out of sight
    DAMAGE_SUB_LEAP_START = 10,       // water: two frames later the leap sound and the biting fish script
    DAMAGE_SUB_RISE = 11,             // thrown up until 500 units above the start height
    DAMAGE_SUB_RETURN = 12,           // carried hidden to the return position, 8 units a frame
    DAMAGE_SUB_FALL_WAIT = 13,
    DAMAGE_SUB_RETURN_IN_PLACE = 14,  // water, JP and later: already at the return position, 10 frames of camera move
    DAMAGE_SUB_FALL = 20,             // drops until he lands
    DAMAGE_SUB_LANDED = 30,           // reached from FALL on landing: starts a 50-frame wait
    DAMAGE_SUB_LANDED_WAIT = 31,      // then takes 1 HP
    DAMAGE_SUB_STAND_UP = 32,         // 12 frames, then the stay motion
    DAMAGE_SUB_HP_ZERO = 33,          // HP ran out: the motion stops here (no case)
};

void marioSetSplash(s32 kind, Vec* pos);
void set_damage_root_ypos(f32 y);
void marioDmgRetQuakeOn(void);
BOOL marioWaitDamageReturnFall(void);
BOOL marioWaitDamageTogeReturnFall(void);
void mot_damage(void);
void mot_damageToge(void);
BOOL marioChkDamageSink(void);
