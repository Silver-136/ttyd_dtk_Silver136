/* "swdrv" - Saved Work Driver (for evt)
 * Helper driver for accessing the "saved work" data for evt scripts.
 */
#include "driver/swdrv.h"
#include "mario/mariost.h"
#include <string.h>

void swInit(void) {
    memset(gp->mGSWF, 0, sizeof(gp->mGSWF));
    memset(gp->mGSW, 0, sizeof(gp->mGSW));
    memset(gp->mLSWF, 0, sizeof(gp->mLSWF));
    memset(gp->mLSW, 0, sizeof(gp->mLSW));
    gp->mGSW0 = 0;
}

void swReInit(void) {
    memset(gp->mLSWF, 0, sizeof(gp->mLSWF));
    memset(gp->mLSW, 0, sizeof(gp->mLSW));
}

//Global Saved Work Flags --------------------------------------
void swSet(s32 index) { //set specific bit
    gp->mGSWF[index / 32] |= (1 << (index % 32));
}

BOOL swGet(s32 index) { //get specific bit
    return (gp->mGSWF[index / 32] & (1 << (index % 32))) != 0;
}

void swClear(s32 index) { //clear specific bit
    gp->mGSWF[index / 32] &= ~(1 << (index % 32));
}

//Global Saved Work --------------------------------------------
void swByteSet(s32 index, s32 value) {
    if (index == 0) {
        gp->mGSW0 = value;
    }
    else {
        gp->mGSW[index] = (u8)value;
    }
}

s32 swByteGet(s32 index) {
    if (index == 0) {
        return gp->mGSW0;
    }
    else {
        return gp->mGSW[index];
    }
}

//Local Saved Work Flags ---------------------------------------
void _swSet(s32 index) { //set specific bit
    gp->mLSWF[index / 32] |= (1 << (index % 32));
}

BOOL _swGet(s32 index) { //get specific bit
    return (gp->mLSWF[index / 32] & (1 << (index % 32))) != 0;
}

void _swClear(s32 index) { //clear specific bit
    gp->mLSWF[index / 32] &= ~(1 << (index % 32));
}

//Local Saved Work ---------------------------------------------
void _swByteSet(s32 index, u8 value) {
    gp->mLSW[index] = value;
}

s32 _swByteGet(s32 index) {
    return gp->mLSW[index];
}
