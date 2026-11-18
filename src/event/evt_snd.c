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

/**
 * Starts BGM args[1] (a literal name) with a 1 second fade (psndBGMOn); args[0] is the literal PSND flag word
 * (BGM channel in the low nibble).
 */
USER_FUNC(evt_snd_bgmon) {
    EvtWord* args = event->args;
    psndBGMOn(args[0], (char*)args[1]);
    return EVT_RETURN_DONE;
}

/** Starts BGM args[1] (a literal name) with the literal PSND flags args[0] and a fade time of args[2] ms. */
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
/**
 * Starts BGM args[1] (a literal name) with the literal PSND flags args[0], using the time args[2] ms both for its
 * fade-in and for the fade-out of the BGM it replaces; args[3] is read but not used.
 */
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

/** Stops or fades the BGM channel of the literal PSND flags args[0] with a 1 second fade (psndBGMOff). */
USER_FUNC(evt_snd_bgmoff) {
    psndBGMOff(event->args[0]);
    return EVT_RETURN_DONE;
}

/** Stops or fades the BGM channel of the literal PSND flags args[0] over args[1] ms. */
USER_FUNC(evt_snd_bgmoff_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 fadetime = evtGetValue(event, args[1]);
    psndBGMOff_f_d(flags, fadetime, 0);
    return EVT_RETURN_DONE;
}

/** Sets the playback rate of BGM channel args[0] (a literal) to args[1]; only a streamed BGM changes (psndBGM_rate). */
USER_FUNC(evt_snd_bgm_freq) {
    EvtWord* args = event->args;
    s32 id = args[0];
    s32 rate = evtGetValue(event, args[1]);
    psndBGM_rate(id, rate);
    return EVT_RETURN_DONE;
}

/** Passes args[1] to psndBGMScope for BGM channel args[0] (a literal): how long it ignores the master BGM volume. */
USER_FUNC(evt_snd_bgm_scope) {
    EvtWord* args = event->args;
    s32 id = args[0];
    s32 scope = evtGetValue(event, args[1]);
    psndBGMScope(id, scope);
    return EVT_RETURN_DONE;
}

/**
 * Plays sound effect args[0] (a literal name, not read through evtGetValue) unpositioned;
 * a nonzero args[1] names the variable that gets the sound id (for evt_snd_sfxoff).
 */
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

/** evt_snd_sfxon with the effect name read through evtGetValue, so args[0] can be a variable holding it. */
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

/** evt_snd_sfxon through psndSFXOn_: the effect plays even while sound effects are switched off. */
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

/**
 * Plays sound effect args[0] (a literal name) unpositioned with volume args[1], pan args[2] and a start delay of
 * args[3] ms; a nonzero args[4] names the variable that gets the sound id.
 */
// Original name and size (0xa0) from the JP demo map (UNUSED there: no script uses it), between evt_snd_sfxon_ and
// evt_snd_sfxon_3d: evt_snd_sfxon_3d_ex's form without the position (name, volume, pan and delay; result to
// args[4]) for psndSFXOnEx; the size fits exactly (s32 locals 0xa8, with the EVT_NULLPTR test 0xac).
USER_FUNC(evt_snd_sfxon_ex) {
    EvtWord* args = event->args;
    const char* name = (const char*)args[0];
    u16 vol, pan, delayMs;
    s32 out;
    vol = evtGetValue(event, args[1]);
    pan = evtGetValue(event, args[2]);
    delayMs = evtGetValue(event, args[3]);
    out = args[4];
    if (!out) {
        psndSFXOnEx(name, vol, pan, delayMs);
    } else {
        evtSetValue(event, out, psndSFXOnEx(name, vol, pan, delayMs));
    }
    return EVT_RETURN_DONE;
}

/**
 * Plays sound effect args[0] (a literal name) as a 3D sound at (args[1], args[2], args[3]); args[4], unless 0 or
 * EVT_NULLPTR, names the variable that gets the sound id.
 */
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

/**
 * Plays sound effect args[0] (a literal name) as a 3D sound at (args[1], args[2], args[3]) with volume args[4], pan
 * args[5], a start delay of args[6] ms and audible distance args[7]; a nonzero args[8] names the variable that gets the
 * sound id.
 */
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

/** Stops the sound effect whose id is args[0]. */
USER_FUNC(evt_snd_sfxoff) {
    psndSFXOff(evtGetValue(event, event->args[0]));
    return EVT_RETURN_DONE;
}

/** Returns in args[1] 0 while the sound effect with id args[0] still plays, else -1 (psndSFXChk). */
USER_FUNC(evt_snd_sfxchk) {
    EvtWord* args = event->args;
    evtSetValue(event, args[1], psndSFXChk(evtGetValue(event, args[0])));
    return EVT_RETURN_DONE;
}

/** Sets the surround pan of the sound effect with id args[0] to args[1]. */
// Original name and size (0x60) from the JP demo map (UNUSED there: no script uses it), between evt_snd_sfxchk and
// evt_snd_sfx_pit (the stripped evt_snd_sfx_pan, whose psndSFX_pan body is not known, sits before it):
// evt_snd_sfx_pit's form (the same size) for psndSFX_span.
USER_FUNC(evt_snd_sfx_span) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 span = evtGetValue(event, args[1]);
    psndSFX_span(id, span);
    return EVT_RETURN_DONE;
}

/** Sets the pitch bend of the sound effect with id args[0] to args[1]. */
USER_FUNC(evt_snd_sfx_pit) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 pitch = evtGetValue(event, args[1]);
    psndSFX_pit(id, pitch);
    return EVT_RETURN_DONE;
}

/** Sets the volume of the sound effect with id args[0] to args[1]. */
USER_FUNC(evt_snd_sfx_vol) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 vol = evtGetValue(event, args[1]);
    psndSFX_vol(id, vol);
    return EVT_RETURN_DONE;
}

/** Moves the 3D sound effect with id args[0] to (args[1], args[2], args[3]). */
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

/** Sets the audible distance of the sound effect with id args[0] to args[1]. */
USER_FUNC(evt_snd_sfx_dist) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    psndSFX_dist(id, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

/**
 * Starts environment sound args[1] (a literal name) on the channel of the literal PSND flags args[0] with a
 * 1 second fade (psndENVOn).
 */
USER_FUNC(evt_snd_envon) {
    EvtWord* args = event->args;
    psndENVOn(args[0], (const char*)args[1]);
    return EVT_RETURN_DONE;
}

/** Starts environment sound args[1] (a literal name) with the literal PSND flags args[0] and a fade of args[2] ms. */
USER_FUNC(evt_snd_envon_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 name = args[1];
    s32 fadetime = evtGetValue(event, args[2]);
    psndENVOn_f_d(flags, SFX_NAME_WORD(name), fadetime, 0);
    return EVT_RETURN_DONE;
}

/** Stops or fades the environment channel of the literal PSND flags args[0] with a 1 second fade (psndENVOff). */
USER_FUNC(evt_snd_envoff) {
    psndENVOff(event->args[0]);
    return EVT_RETURN_DONE;
}

/** Stops or fades the environment channel of the literal PSND flags args[0] over args[1] ms. */
USER_FUNC(evt_snd_envoff_f) {
    EvtWord* args = event->args;
    s32 flags = args[0];
    s32 fadetime = evtGetValue(event, args[1]);
    psndENVOff_f_d(flags, fadetime, 0);
    return EVT_RETURN_DONE;
}

/** Sets the low-pass filter frequency of environment channel args[0] (a literal) to args[1] (0 = none). */
USER_FUNC(evt_snd_env_lpf) {
    EvtWord* args = event->args;
    s32 id = args[0];
    psndENV_LPF(id, evtGetValue(event, args[1]));
    return EVT_RETURN_DONE;
}

/** Sets the sound effect reverb mode to the literal args[0] (0-3: reverb 0, 20, 40 or 60). */
USER_FUNC(evt_snd_set_rev_mode) {
    psndSetReverb(event->args[0]);
    return EVT_RETURN_DONE;
}

/** Fades out every playing sound effect except environment sounds and those marked to survive a map change. */
USER_FUNC(evt_snd_sfx_all_stop) {
    psndSFXAllOff();
    return EVT_RETURN_DONE;
}
