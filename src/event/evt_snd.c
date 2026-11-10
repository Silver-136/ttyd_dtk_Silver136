// Sound script commands: BGM (evt_snd_bgmon), SFX (evt_snd_sfxon, evt_snd_sfxon_3d), environment sounds
// (evt_snd_envon) and reverb. Its own object in the JP demo map, right after party_motion.o; the functions are
// emitted in reverse order (-inline deferred), so evt_snd_sfx_all_stop comes first in the object.
#include "version.h"
#include "evt/evt_snd.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "pmario_sound.h"
#include "sfx_name_word.h"
#include <dolphin/mtx.h>

USER_FUNC(evt_snd_bgmon) {
    EvtWord* args = event->args;
    psndBGMOn(args[0], (char*)args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgmon_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    const char* bgm_name = (const char*)args[1];
    s32 fadetime = evtGetValue(event, args[2]);
#if VERSION <= VERSION_DEMO_JP
    psndBGMOn_f_d(flags, (char*)bgm_name, fadetime, 0);
#else
    psndBGMOn_f(flags, bgm_name, fadetime, 0);
#endif
    return EVT_RETURN_DONE;
}

#if VERSION >= VERSION_JP
// Descriptive name (not in the symbol maps); placeholder unk_8014cc24, JP 0x8014CC24. See docs/NAME_PROVENANCE.md.
USER_FUNC(evt_snd_bgmon_f_d) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    const char* bgm_name = (const char*)args[1];
    s32 fadetime = evtGetValue(event, args[2]);

    // The fourth argument is evaluated but not used; the third is passed for both fade times.
    evtGetValue(event, args[3]);
    psndBGMOn_f_d(flags, bgm_name, fadetime, fadetime, 0);
    return EVT_RETURN_DONE;
}
#endif

USER_FUNC(evt_snd_bgmoff) {
    psndBGMOff(event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgmoff_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 fadetime = evtGetValue(event, args[1]);
    psndBGMOff_f_d(flags, fadetime, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgm_freq) {
    EvtWord* args = event->args;
    s32 id = args[0];
    s32 rate = evtGetValue(event, args[1]);
    psndBGM_rate(id, rate);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_bgm_scope) {
    EvtWord* args = event->args;
    s32 id = args[0];
    s32 scope = evtGetValue(event, args[1]);
    psndBGMScope(id, scope);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    s32 out = args[1];
    if (!out) {
        psndSFXOn(name);
    } else {
        evtSetValue(event, out, psndSFXOn(name));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon__) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 out = args[1];
    if (!out) {
        psndSFXOn(name);
    } else {
        evtSetValue(event, out, psndSFXOn(name));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon_) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    s32 out = args[1];
    if (!out) {
        psndSFXOn_(name);
    } else {
        evtSetValue(event, out, psndSFXOn_(name));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon_3d) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    Vec pos;
    s32 out;
    pos.x = evtGetFloat(event, args[1]);
    pos.y = evtGetFloat(event, args[2]);
    pos.z = evtGetFloat(event, args[3]);
    out = args[4];
    if (!out || out == EVT_NULLPTR) {
        psndSFXOn_3D(name, &pos);
    } else {
        evtSetValue(event, out, psndSFXOn_3D(name, &pos));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxon_3d_ex) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    Vec pos;
    u16 vol, pan, delayMs, dist;
    s32 out;
    pos.x = evtGetFloat(event, args[1]);
    pos.y = evtGetFloat(event, args[2]);
    pos.z = evtGetFloat(event, args[3]);
    vol = evtGetValue(event, args[4]);
    pan = evtGetValue(event, args[5]);
    delayMs = evtGetValue(event, args[6]);
    dist = evtGetValue(event, args[7]);
    out = args[8];
    if (!out) {
        psndSFXOnEx_3D(name, &pos, vol, pan, delayMs, dist);
    } else {
        evtSetValue(event, out, psndSFXOnEx_3D(name, &pos, vol, pan, delayMs, dist));
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxoff) {
    psndSFXOff(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfxchk) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], psndSFXChk(evtGetValue(event, args[0])));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_pit) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 pitch = evtGetValue(event, args[1]);
    psndSFX_pit(id, pitch);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_vol) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 vol = evtGetValue(event, args[1]);
    psndSFX_vol(id, vol);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_pos) {
    EvtWord* args = event->args;
    Vec pos;
    s32 id = evtGetValue(event, args[0]);
    pos.x = evtGetFloat(event, args[1]);
    pos.y = evtGetFloat(event, args[2]);
    pos.z = evtGetFloat(event, args[3]);
    psndSFX_pos(id, &pos);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_dist) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    psndSFX_dist(id, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envon) {
    EvtWord* args = event->args;
    psndENVOn(args[0], (const char*)args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envon_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 name = args[1];
    s32 fadetime = evtGetValue(event, args[2]);
    psndENVOn_f_d(flags, SFX_NAME_WORD(name), fadetime, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envoff) {
    psndENVOff(event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_envoff_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 fadetime = evtGetValue(event, args[1]);
    psndENVOff_f_d(flags, fadetime, 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_env_lpf) {
    EvtWord* args = event->args;
    s32 id = args[0];
    psndENV_LPF(id, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_set_rev_mode) {
    psndSetReverb(event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_snd_sfx_all_stop) {
    psndSFXAllOff();
    return EVT_RETURN_DONE;
}
