#pragma once

#include <dolphin/mtx.h>

/* typedef union SoundData {
    const char* name;
    s32 value;
} SoundData; */

typedef struct SoundEnvList {
    u16 id;          // 0x0, psenvlistname index; 0x76 ends the list
    u16 sfxId;       // 0x2, ORed with 0x4/0x5/0x6000000 and passed to __psndSFXOn as the name
    u8 volume;       // 0x4
    u8 reserved_05;  // 0x5, copied into PaperSoundEnv+0x7 by psndENVOn_f_d, never read (0 in every entry)
    u8 type;         // 0x6, 0: random position + interval, 1: loop, 2: interval, 3: one-shot
    u8 intervalRand; // 0x7, seconds, irand(intervalRand) added to interval
    u8 interval;     // 0x8, seconds between plays (types 0 and 2)
    f32 posX;        // 0xC, copied (as raw words) to the PaperSoundEnv position
    f32 posY;        // 0x10
    f32 posZ;        // 0x14
    u8 radiusMin;    // 0x18, type 0 spawn radius = (radiusMin + irand(radiusRange)) * 10
    u8 radiusRange;  // 0x19
    u8 dist;         // 0x1A, __psndSFXOn dist
    // pad 1 byte
} SoundEnvList;

typedef enum SoundEffectFlags {
    SFX_FLAG_2 = (1 << 29),
    SFX_FLAG_4 = (1 << 30),
    SFX_FLAG_8 = (1 << 31)
} SoundEffectFlags;

// most are translated in __psndSFXOn, TODO
typedef struct SoundEffectEntry {
    const char* name;       // 0x0, sfx name
    u32 fxId;               // 0x4, lower 2 bytes are SND_FXID, top bits SoundEffectFlags
    const char* streamName; // 0x8, .stm name
    u8 volume;              // 0xC
    u8 pan;                 // 0xD
    u8 specialFlag;         // 0xE, 1 sets PaperSoundEffect flag 0x200
    u8 reverb;              // 0xF, aux/reverb send
    u8 priority;            // 0x10, an existing SFX with a higher priority is not stolen
    u8 delay;               // 0x11, start delay frames
    s16 pitch;              // 0x12, signed (0xFAAB etc. are downward shifts)
} SoundEffectEntry;

typedef struct PaperSoundFade {
    u8 state;       // 0x0
    u8 pad_01;      // 0x1, never accessed
    s16 time;       // 0x2
    s16 timeMax;    // 0x4
    u8 dipState;    // 0x6, partial volume dip: 8 = going down (psndBGMOff_f_d), 0x10 = coming back (psndBGMOn_f_d), 0 = idle
    u8 dipPercent;  // 0x7, dip target volume in percent
    s16 dipTimeMax; // 0x8, dip length in frames (fadetime / 16.67)
    s16 dipTime;    // 0xA, current dip level, steps toward dipTimeMax * dipPercent / 100
} PaperSoundFade;

typedef struct PaperSoundBGM {
    u32 listId;             // 0x0, index into psbgmlist, 0xFFFFFFFF = idle
    s32 streamId;           // 0x4, sound stream/song channel
    u32 flags;              // 0x8, psndBGMOn flags (0x40 = paused/muted)
    u8 pad_0C;              // 0xC, never accessed
    u8 group;               // 0xD, SoundBGMEntry params byte 2; a new BGM in the same nonzero group keeps playing
    u8 volume;              // 0xE, base volume from SoundBGMEntry
    u8 pan;                 // 0xF
    u8 scopeTimer;          // 0x10, psndBGMScope: 0xFF = full scope, 0 = none, else counts down
    u8 pad_11[0x20 - 0x11]; // 0x11
    PaperSoundFade fade;    // 0x20
    u8 pad_2C[0x30 - 0x2C]; // 0x2C
    s64 startTime;          // 0x30, gp->renderTime when started (psndBGMPlayTime)
} PaperSoundBGM;

typedef struct PaperSoundEffect {
    u32 listId;            // 0x0, searchPSSFXList id, 0xFFFFFFFF = free slot
    u16 serial;            // 0x4, psnd.sfxSerial at start (checked against the id handed out)
    u16 flags;             // 0x6
    u8 volume;             // 0x8
    u8 entryVolume;        // 0x9, SoundEffectEntry volume, scales volume
    u8 pan;                // 0xA
    u8 reverb;             // 0xB
    u16 priority;          // 0xC, SoundEffectEntry priority
    u16 delay;             // 0xE, start delay frames
    u16 attenuation;       // 0x10, subtracted from the scaled volume
    u16 dist;              // 0x12, 3D attenuation distance (psndSFX_dist)
    s16 pitch;             // 0x14 (psndSFX_pit)
    u16 lpf;               // 0x16, low-pass value set from PaperSoundWork.envLpf
    u32 channel;           // 0x18, sound effect/stream channel, -1 = none
    Vec position;          // 0x1C, 3D source position (psndSFX_pos)
} PaperSoundEffect;

typedef struct PaperSoundEnv {
    u32 sfxId;          // 0x0, SoundEnvList sfxId, ORed with 0x4/0x5/0x6000000 for __psndSFXOn
    u16 id;             // 0x4, SoundEnvList id, 0 = free slot
    u8 volume;          // 0x6
    u8 reserved_07;     // 0x7, copied from SoundEnvList reserved_05, never read
    u8 type;            // 0x8, 0: random position + interval, 1: loop, 2: interval, 3: one-shot
    u8 intervalRand;    // 0x9, seconds
    u8 interval;        // 0xA, seconds
    u8 radiusMin;       // 0xB, type 0 spawn radius = (radiusMin + irand(radiusRange)) * 10
    u8 radiusRange;     // 0xC
    u8 state;           // 0xD
    u16 timer;          // 0xE, frames until the next play
    u16 dist;           // 0x10, __psndSFXOn dist
    u16 pad_12;         // 0x12, never accessed
    u32 sfx;            // 0x14, playing SFX handle, 0xFFFFFFFF = none
    Vec position;       // 0x18
} PaperSoundEnv;

typedef struct PaperSoundWork {
    PaperSoundFade fade[2];         // 0x0
    u16 envId[2];                   // 0x18, active ENV id per channel (psndENVOn/Off)
    u16 envLpf[2];                  // 0x1C, ENV low-pass per channel, set to 0x7D00 and ramped toward envFreq
    u16 envFreq[2];                 // 0x20, ENV play frequency per channel (default 0x7D00)
    u8 sfxFadeState;                // 0x24, global SFX fade (2 = fading out, 0 = idle)
    u8 pad_25;                      // 0x25
    s16 sfxFadeMax;                 // 0x26, fade length in frames
    s16 sfxFadeTime;                // 0x28, frames left
    u8 pad_2A[0x30 - 0x2A];         // 0x2A
    Vec position;                   // 0x30
    f32 direction;                  // 0x3C
    char mapName[0x10];             // 0x40, map the sound state was set up for (psndMapChange)
    s16 sfxSerial;                  // 0x50, incremented per SFX start, packed into SFX ids
    s16 reverb;                     // 0x52, psndSetReverb level (0/20/40/60)
    s16 sfxDelay;                   // 0x54, +8 for each repeat of a SFX already playing this frame, reset each frame
    u16 flags;                      // 0x56, psndSetFlag/psndClearFlag/psndGetFlag
    s8 bgmVolume;                   // 0x58, BGM master volume, ramps toward 100 (65 when flag 0x20)
    s8 sfxVolume;                   // 0x59, SFX master volume, ramps toward 100 (120 when flag 0x20)
    s8 envVolume;                   // 0x5A, ENV master volume, ramps toward 100 (80 when flag 0x20)
    u8 sfxActiveCount[0x5C - 0x5B]; // 0x5B, active PaperSoundEffect slots counted by psndSFXMain
    s8 sfxCooldown;                 // 0x5C, frames before certain repeating SFX may restart (set to 6)
    u8 pad_5D[0x60 - 0x5D];         // 0x5D
} PaperSoundWork;

s32 searchPSSFXList(const char* name);
void psndInit(void);
void psndMain(void);
void psndMainInt(void);
void psndExit(void);

BOOL psndPushGroup(void);

BOOL psndBGMOn(u32 r3, char* bgm_name);
void psndStopAllFadeOut(void);
BOOL psndENVOff(u32 r3);
void psndSFXAllOff(void);

#include "sfx_name_word.h"

s32 psndSFXOn(const char* name);
s32 psndSFXOn_3D(const char* name, Vec* position);
s32 psndSFXOnEx_3D(const char* name, Vec* position, u8 vol, u8 pan, u16 pitch, u16 dist);

BOOL psndBGMOff_f_d(s32 flags, u32 fadetime, u32 someswitch);
BOOL psndBGMStartCheck(s32 id);

/* psndSetFlag/psndClearFlag: sound effects play at volume 0 unless their id has PSND_SFX_BYPASS_MUTE */
#define PSND_FLAG_MUTE_SFX 0x80
void psndSetFlag(s32 flag);
void psndSFXOff(s32 id);
void L_psndBGM_stop(void);
void psndClearFlag(s32 flag);

void psndSetPosDirListener(Vec* position, f32 direction);

extern PaperSoundBGM psbgm[2];

BOOL psndBGMOff(u32 flags);
s32 psndGetFlag(void);
void psndSFX_pos(s32 id, Vec* position);
s32 psndBGMPlayTime(s32 id);
void psndBGMScope(s32 id, s32 scope);
void psndBGM_rate(s32 id, u16 rate);
void psndMapChange(void);
s32 psndSFXChk(s32 id);
s32 psndSFXOnVol(const char* name, u8 vol);
s32 psndSFXOn_(const char* name);
void psndSFX_dist(s32 id, u16 dist);
u8 psndSFX_get_vol(s32 id);
void psndSetReverb(s32 mode);
s32 psndBGMChk(s32 id);
BOOL psndENVOff_f_d(u32 flags, u32 fadetime, s32 r5);
BOOL psndENV_LPF(s32 id, u16 freq);
