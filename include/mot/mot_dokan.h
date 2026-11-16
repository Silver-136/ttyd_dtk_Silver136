// mot_dokan.c: Mario's pipe (dokan) motion, mot_dokan.

#pragma once

#include <dolphin/types.h>

// MarioWork.currSubMotionId in mot_dokan (names from what each step does; TATE = vertical pipe, YOKO = sideways pipe)
enum {
    DOKAN_SUB_YOKO_SHRINK = 0,       // DOKAN_YOKO_OUT: after 20 frames tumbles and shrinks
    DOKAN_SUB_TATE_SHRINK = 10,      // DOKAN_TATE_DOWNOUT / UPOUT: spins and shrinks
    DOKAN_SUB_TATE_GROW_START = 20,  // DOKAN_TATE_UPIN: scale 0, then
    DOKAN_SUB_TATE_GROW = 21,        //   spins and grows to full size
    DOKAN_SUB_TATE_SPIN = 30,        // DOKAN_TATE_DOWNIN: spins in place
    DOKAN_SUB_YOKO_GROW = 40,        // DOKAN_YOKO_IN: tumbles and grows, the partners follow
    DOKAN_SUB_END = 100,             // set by evt_mario_dokan_end: picks the ending by dokanType
    DOKAN_SUB_END_TURN = 101,        // keeps spinning until 540 degrees
    DOKAN_SUB_END_WAIT = 102,        // 20 frames in the PM_D_1B paper pose
    DOKAN_SUB_END_FINISH = 103,      // back to the stay motion
    DOKAN_SUB_UPIN_END = 110,        // DOKAN_TATE_UPIN ending: the same four steps with the camera's y follow off
    DOKAN_SUB_UPIN_END_TURN = 111,
    DOKAN_SUB_UPIN_END_WAIT = 112,
    DOKAN_SUB_UPIN_END_FINISH = 113,
};

void mot_dokan(void);
