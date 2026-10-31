// Program entry: initialises the game and runs the main loop (game update, draw, frame timing).

#include "main.h"

#include "mario/mariost.h"
#include "sdk/DEMOInit.h"

int main(void) {
    OSTick base_tick;

    marioStInit();
    marioStMain();
    while (1) {
        base_tick = OSGetTick();
        DEMOBeforeRender();
        marioStDisp();
        marioStMain();
        gp->mDeltaGame = OSGetTick() - base_tick;
        DEMODoneRender();
        gp->mDeltaRender = OSGetTick() - base_tick;
    }

    return 0;
}
