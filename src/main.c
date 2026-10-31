// Program entry: initialises the game and runs the main loop (game update, draw, frame timing).

#include "main.h"

#include "mario/mariost.h"
#include "sdk/DEMOInit.h"

/**
 * Program entry: initialises the game (marioStInit),
 * then loops forever running one game update (marioStMain) and one draw (marioStDisp) per frame,
 * recording the update and render times in OS ticks in gp->deltaGame and gp->deltaRender.
 */
int main(void) {
    OSTick base_tick;

    marioStInit();
    marioStMain();
    while (1) {
        base_tick = OSGetTick();
        DEMOBeforeRender();
        marioStDisp();
        marioStMain();
        gp->deltaGame = OSGetTick() - base_tick;
        DEMODoneRender();
        gp->deltaRender = OSGetTick() - base_tick;
    }

    return 0;
}
