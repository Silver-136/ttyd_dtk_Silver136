// Declares the audience animation bytecode and its opcodes.

#pragma once

/* Audience animation bytecode (interpreted by BattleAudienceAnimMain); every command is a run of s32 words. */
enum {
    AUD_OP_END = -1,
    AUD_OP_FRAME = -2,
    AUD_OP_RESTART = -3,
    AUD_OP_LABEL = -4,
    AUD_OP_GOTO = -5,
    AUD_OP_REPEAT = -6,
    AUD_OP_END_REPEAT = -7,
    AUD_OP_WAIT = -8,
};

#define AUD_END -1                 /* stop */
#define AUD_FRAME(anim, wait) -2, anim, wait /* show animation frame, then wait */
#define AUD_RESTART -3             /* jump back to the start of the script */
#define AUD_LABEL(n) -4, n         /* jump target for AUD_GOTO */
#define AUD_GOTO(n) -5, n          /* jump to AUD_LABEL(n), searched from the script start */
#define AUD_REPEAT(n) -6, n        /* start of a counted loop */
#define AUD_END_REPEAT -7          /* end of the counted loop */
#define AUD_WAIT(n) -8, n          /* wait n frames */
