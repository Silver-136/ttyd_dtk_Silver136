// NOTE: in G8MJ01 the event/evt_snd.c split (0x80077EC4..0x80077F88) actually holds
// the tail of seq_gameover.c (seq_gameOverExit/Init); the real evt_snd functions
// live at 0x8014C488 inside the party/party_motion.c split.
#include "evt/evt_snd.h"
#include "driver/camdrv.h"
#include "driver/seqdrv.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "sound.h"
#include "win/win_main.h"


void psndSetReverb(s32 mode);
BOOL N_pouchUnEquipBadgeID(s32 badgeId);

void seq_gameOverInit(SequenceWork* work) {
    winReInit();
    camDispOn(CAMERA_3D);
    psndStopAllFadeOut();
    psndSetReverb(0);
    L_psndBGM_stop();
    psndClearFlag(0x80);
    psndClearFlag(0x20);
    psndClearFlag(0x40);
    N_pouchUnEquipBadgeID(0x143);
    marioStSystemLevel(4);
}

void seq_gameOverExit(SequenceWork* work) {
    psndStopAllFadeOut();
    marioStSystemLevel(0);
}

USER_FUNC(N_evt_snd_bgm_unpause) {
    if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
        sound.streams[psbgm[0].streamId].flags &= ~0x8000;
    }
    return EVT_RETURN_DONE;
}
