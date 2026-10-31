/* "swdrv" - Saved Work Driver (for evt)
 * Helper driver for accessing the "saved work" data for evt scripts.
 */
#include "driver/swdrv.h"
#include "mario/mariost.h"
#include <string.h>

void swInit(void) {
    memset(gp->gSWF, 0, sizeof(gp->gSWF));
    memset(gp->gSW, 0, sizeof(gp->gSW));
    memset(gp->lSWF, 0, sizeof(gp->lSWF));
    memset(gp->lSW, 0, sizeof(gp->lSW));
    gp->gSW0 = 0;
}

/**
 * Clears only the area-local flags (LSWF) and words (LSW);
 * the map-change sequence calls it when Mario enters a different area.
 */
void swReInit(void) {
    memset(gp->lSWF, 0, sizeof(gp->lSWF));
    memset(gp->lSW, 0, sizeof(gp->lSW));
}

/** Global Saved Work Flags -------------------------------------- */
//Global Saved Work Flags --------------------------------------
void swSet(s32 index) { //set specific bit
    gp->gSWF[index / 32] |= (1 << (index % 32));
}

/** Returns whether global saved flag GSWF(index) is set. */
BOOL swGet(s32 index) { //get specific bit
    return (gp->gSWF[index / 32] & (1 << (index % 32))) != 0;
}

/** Clears global saved flag GSWF(index). */
void swClear(s32 index) { //clear specific bit
    gp->gSWF[index / 32] &= ~(1 << (index % 32));
}

/** Global Saved Work -------------------------------------------- */
//Global Saved Work --------------------------------------------
void swByteSet(s32 index, s32 value) {
    if (index == 0) {
        gp->gSW0 = value;
    } else {
        gp->gSW[index] = (u8)value;
    }
}

/** Returns global saved word GSW(index). GSW(0), the story progress, is a full 32-bit value; the others are bytes. */
s32 swByteGet(s32 index) {
    if (index == 0) {
        return gp->gSW0;
    } else {
        return gp->gSW[index];
    }
}

/** Local Saved Work Flags --------------------------------------- */
//Local Saved Work Flags ---------------------------------------
void _swSet(s32 index) { //set specific bit
    gp->lSWF[index / 32] |= (1 << (index % 32));
}

/** Returns whether area-local saved flag LSWF(index) is set. */
BOOL _swGet(s32 index) { //get specific bit
    return (gp->lSWF[index / 32] & (1 << (index % 32))) != 0;
}

/** Clears area-local saved flag LSWF(index). */
void _swClear(s32 index) { //clear specific bit
    gp->lSWF[index / 32] &= ~(1 << (index % 32));
}

/** Local Saved Work --------------------------------------------- */
//Local Saved Work ---------------------------------------------
void _swByteSet(s32 index, u8 value) {
    gp->lSW[index] = value;
}

/** Returns area-local saved byte LSW(index). */
s32 _swByteGet(s32 index) {
    return gp->lSW[index];
}
