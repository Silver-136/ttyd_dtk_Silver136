#include "event/evt_snd.h"

s32 evtGetValue(void* evt, s32 value);
f32 evtGetFloat(void* evt, s32 value);
s32 evtSetValue(void* evt, s32 var, s32 value);
void psndBGMOff(s32 name);
void psndENVOff(s32 name);
void psndSetReverb(s32 mode);
void psndBGMOn(s32 name, s32 flags);
void psndENVOn(s32 name, s32 flags);
void psndSFXOff(s32 id);
void psndSFXAllOff(void);
void psndBGMOff_f_d(s32 name, s32 frames, s32 flags);
void psndENVOff_f_d(s32 name, s32 frames, s32 flags);
void unk_800db778(s32 name, s32 flags, s32 frames, s32 extra);
void psndENVOn_f_d(s32 name, s32 flags, s32 frames, s32 extra);
void psndSFX_dist(s32 name, u16 distance);
s32 psndSFXOn(s32 name);
s32 psndSFXOn_(s32 name);
s32 psndSFXOn_3D(s32 name, void* pos);
s32 psndSFXOnEx_3D(s32 name, void* pos, u8 volume, u8 pitch, u16 span, u16 flags);
void psndSFX_pos(s32 name, void* pos);

s32 evt_snd_bgmoff(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    psndBGMOff(args[0]);
    return 2;
}

s32 evt_snd_envoff(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    psndENVOff(args[0]);
    return 2;
}

s32 evt_snd_set_rev_mode(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    psndSetReverb(args[0]);
    return 2;
}

s32 evt_snd_bgmon(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    psndBGMOn(args[0], args[1]);
    return 2;
}

s32 evt_snd_envon(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    psndENVOn(args[0], args[1]);
    return 2;
}

s32 evt_snd_sfxoff(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    psndSFXOff(evtGetValue(evt, args[0]));
    return 2;
}

s32 evt_snd_sfx_all_stop(void) {
    psndSFXAllOff();
    return 2;
}

s32 evt_snd_bgmoff_f(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = args[0];
    s32 frames = evtGetValue(evt, args[1]);
    psndBGMOff_f_d(name, frames, 0);
    return 2;
}

s32 evt_snd_envoff_f(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = args[0];
    s32 frames = evtGetValue(evt, args[1]);
    psndENVOff_f_d(name, frames, 0);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_snd_bgmon_f(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = args[0];
    s32 flags = args[1];
    s32 frames = evtGetValue(evt, args[2]);
    unk_800db778(name, flags, frames, 0);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_snd_envon_f(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = args[0];
    s32 flags = args[1];
    s32 frames = evtGetValue(evt, args[2]);
    psndENVOn_f_d(name, flags, frames, 0);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_snd_sfx_dist(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    psndSFX_dist(name, evtGetValue(evt, args[1]));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_snd_sfxon(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 out = args[1];
    s32 name = args[0];
    if (out == 0) {
        psndSFXOn(name);
    } else {
        evtSetValue(evt, out, psndSFXOn(name));
    }
    return 2;
}

s32 evt_snd_sfxon_(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 out = args[1];
    s32 name = args[0];
    if (out == 0) {
        psndSFXOn_(name);
    } else {
        evtSetValue(evt, out, psndSFXOn_(name));
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_snd_sfxon__(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    s32 out = args[1];
    if (out == 0) {
        psndSFXOn(name);
    } else {
        evtSetValue(evt, out, psndSFXOn(name));
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_snd_sfxon_3d_ex(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = args[0];
    f32 pos[3];
    u16 volume;
    u16 pitch;
    u16 span;
    u16 flags;
    s32 out;

    pos[0] = evtGetFloat(evt, args[1]);
    pos[1] = evtGetFloat(evt, args[2]);
    pos[2] = evtGetFloat(evt, args[3]);
    volume = evtGetValue(evt, args[4]);
    pitch = evtGetValue(evt, args[5]);
    span = evtGetValue(evt, args[6]);
    flags = evtGetValue(evt, args[7]);
    out = args[8];
    if (out == 0) {
        psndSFXOnEx_3D(name, pos, (u8)volume, (u8)pitch, span, flags);
    } else {
        evtSetValue(evt, out, psndSFXOnEx_3D(name, pos, (u8)volume, (u8)pitch, span, flags));
    }
    return 2;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_snd_sfxon_3d(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = args[0];
    f32 pos[3];
    s32 out;

    pos[0] = evtGetFloat(evt, args[1]);
    pos[1] = evtGetFloat(evt, args[2]);
    pos[2] = evtGetFloat(evt, args[3]);
    out = args[4];
    if (out == 0 || out == -0x0EE6B280) {
        psndSFXOn_3D(name, pos);
    } else {
        evtSetValue(evt, out, psndSFXOn_3D(name, pos));
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_snd_sfx_pos(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 name = evtGetValue(evt, args[0]);
    f32 pos[3];

    pos[0] = evtGetFloat(evt, args[1]);
    pos[1] = evtGetFloat(evt, args[2]);
    pos[2] = evtGetFloat(evt, args[3]);
    psndSFX_pos(name, pos);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 evt_snd_sfx_vol(int param_1) {
    return 0;
}


s32 evt_snd_sfx_pit(void* pEvt) {
    return 0;
}


s32 evt_snd_sfxchk(int param_1) {
    return 0;
}


s32 evt_snd_bgm_scope(int param_1) {
    return 0;
}


s32 evt_snd_bgm_freq(int param_1) {
    return 0;
}


s32 evt_snd_env_lpf(int param_1) {
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 unk_801524c8(void* evt) {
    extern void psndBGMOn_f_d(s32 name, s32 flags, s32 frames, s32 fade, s32 mode);
    s32* args;
    s32 name;
    s32 flags;
    s32 frames;

    args = *(s32**)((s32)evt + 0x18);
    name = args[0];
    flags = args[1];
    frames = evtGetValue(evt, args[2]);
    evtGetValue(evt, args[3]);
    psndBGMOn_f_d(name, flags, frames, frames, 0);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
