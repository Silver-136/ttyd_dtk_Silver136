// NOTE: in JP and later the event/evt_snd.c split (JP 0x80077EC4..0x80077F88; the early demos have none) holds
// evt_snd_bgm_unpause and the tail of seq_gameover.c (seq_gameOverExit/Init); the other evt_snd
// functions live inside the party/party_motion.c split (JP from 0x8014C488).
#include "evt/evt_snd.h"
#include "driver/camdrv.h"
#include "driver/seqdrv.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "pmario_sound.h"
#include "sound.h"
#include "win/win_main.h"

void seq_gameOverInit(SequenceWork* work) {
    winReInit();
    camDispOn(CAMERA_3D);
    psndStopAllFadeOut();
    psndSetReverb(0);
    L_psndBGM_stop();
    psndClearFlag(PSND_FLAG_MUTE_SFX);
    psndClearFlag(PSND_FLAG_LETTER_BOX);
    psndClearFlag(PSND_FLAG_LETTER_BOX_OFF);
    pouchUnEquipBadgeID(ITEM_SLOW_GO);
    marioStSystemLevel(4);
}

void seq_gameOverExit(SequenceWork* work) {
    psndStopAllFadeOut();
    marioStSystemLevel(0);
}

// Descriptive name (not in the symbol maps).
USER_FUNC(evt_snd_bgm_unpause) {
    if ((u32)psbgm[0].streamId != 0xFFFFFFFFU) {
        sound.streams[psbgm[0].streamId].flags &= ~SOUND_STREAM_FLAG_PAUSE;
    }
    return EVT_RETURN_DONE;
}
