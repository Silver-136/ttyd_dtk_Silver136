// Declares the audience animation bytecode and its opcodes.

#pragma once

/* Audience animation bytecode (interpreted by BattleAudienceAnimMain); every command is a run of s32 words. */
#define AUD_END -1                 /* stop */
#define AUD_FRAME(anim, wait) -2, anim, wait /* show animation frame, then wait */
#define AUD_RESTART -3             /* jump back to the start of the script */
#define AUD_LABEL(n) -4, n         /* jump target for AUD_GOTO */
#define AUD_GOTO(n) -5, n          /* jump back to AUD_LABEL(n) */
#define AUD_REPEAT(n) -6, n        /* start of a counted loop */
#define AUD_END_REPEAT -7          /* end of the counted loop */
#define AUD_WAIT(n) -8, n          /* wait n frames */
