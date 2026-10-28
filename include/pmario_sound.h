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
    SFX_FLAG_EFX = (1 << 29),    // 0x20000000, a MusyX sound effect (SoundEfx*)
    SFX_FLAG_SONG = (1 << 30),   // 0x40000000, played as a song (SoundSong*)
    SFX_FLAG_STREAM = (1 << 31)  // 0x80000000, a streamed .stm sound (SoundSS*, streamName)
} SoundEffectFlags;

// sound effect table entry; __psndSFXOn looks names up here
typedef struct SoundEffectEntry {
    const char* name;       // 0x0, sfx name
    u32 fxId;               // 0x4, lower 2 bytes are SND_FXID, top bits SoundEffectFlags
    const char* streamName; // 0x8, .stm name
    u8 volume;              // 0xC
    u8 pan;                 // 0xD
    u8 specialFlag;         // 0xE, 1 sets PSFX_FLAG_NO_MUTE
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
    u32 flags;              // 0x8, copy of the psndBGMOn word (only PSND_QUICK is read back)
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

// PaperSoundEffect.flags (__psndSFXOn sets them from the sound id and its pssfxlist entry)
typedef enum PaperSfxFlags {
    PSFX_FLAG_START = (1 << 0),             // 0x1, waiting to start (delay countdown in psndSFXMain)
    PSFX_FLAG_3D = (1 << 1),                // 0x2, positional: volume and pan from calc3D every frame
    PSFX_FLAG_POS_UNUSED = (1 << 2),        // 0x4, position given with the r8 argument set; never read
    PSFX_FLAG_BATTLE_START = (1 << 4),      // 0x10, started while PSND_FLAG_BATTLE_START was set
    PSFX_FLAG_ENV = (1 << 5),               // 0x20, environment sound (PSND_SFX_ENV)
    PSFX_FLAG_FADE_OUT = (1 << 6),          // 0x40
    PSFX_FLAG_KEEP = (1 << 7),              // 0x80, survives psndSFXAllOff (PSND_SFX_KEEP)
    PSFX_FLAG_NEW_UNUSED = (1 << 8),        // 0x100, set at the start, cleared once started; never read
    PSFX_FLAG_NO_MUTE = (1 << 9),           // 0x200, not muted by PSND_FLAG_MUTE_SFX (entry specialFlag 1)
    PSFX_FLAG_DOOR_MUTE = (1 << 10),        // 0x400, silent while PSND_FLAG_DOOR_IN (PSND_SFX_DOOR_MUTE)
    PSFX_FLAG_BYPASS_MUTE = (1 << 11),      // 0x800, PSND_SFX_BYPASS_MUTE
} PaperSfxFlags;

typedef struct PaperSoundEffect {
    u32 listId;            // 0x0, searchPSSFXList id, 0xFFFFFFFF = free slot
    u16 serial;            // 0x4, psnd.sfxSerial at start (checked against the id handed out)
    u16 flags;             // 0x6, PaperSfxFlags
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

// psndSetFlag/psndClearFlag/psndGetFlag (PaperSoundWork.flags)
typedef enum PsndFlags {
    PSND_FLAG_BGM_OFF = (1 << 0),         // 0x1, setting it fades both BGM players out
    PSND_FLAG_ENV_OFF = (1 << 1),         // 0x2, setting it stops both environment sounds
    PSND_FLAG_NO_SFX = (1 << 2),          // 0x4, sound requests that do not force playback are dropped
    PSND_FLAG_BATTLE_START = (1 << 4),    // 0x10, a battle is starting: sounds started now stop with the field BGM
    PSND_FLAG_LETTER_BOX = (1 << 5),      // 0x20, the camera letter box is shown
    PSND_FLAG_LETTER_BOX_OFF = (1 << 6),  // 0x40, the letter box is disabled
    PSND_FLAG_MUTE_SFX = (1 << 7),        // 0x80, sound effects play at volume 0 unless their id has PSND_SFX_BYPASS_MUTE
    PSND_FLAG_DOOR_IN = (1 << 8),         // 0x100, snd_door_in .. snd_door_out (cleared on a map change)
    PSND_FLAG_BGM_WAIT_0 = (1 << 12),     // 0x1000, the map change waits until BGM player 0 has played 3 s
    PSND_FLAG_BGM_WAIT_1 = (1 << 13),     // 0x2000, the same for player 1
} PsndFlags;

// First argument of psndBGMOn*/psndBGMOff* and psndENVOn*/psndENVOff* (and evt_snd_bgmon/bgmoff/envon/envoff): one
// layout for BGM and environment sound. Low nibble: the BGM player / environment slot (0 field music, 1 fanfares and
// battle music). PSND_FADEOUT/PSND_PAUSE/PSND_SILENT act in the Off functions, the others in the On functions.
#define PSND_SLOT(n) (n)
#define PSND_BGM_STREAM 0x80000000  // SoundBGMEntry.flags: a streamed .stm (path); otherwise ...
#define PSND_BGM_SONG_MASK 0x0FFFFFFF // ... the low bits are the song id
#define PSND_RESTART 0x10          // restart even if the same song or group is already playing
#define PSND_CONTINUE 0x20         // resume the faded-out (PSND_PAUSE) song: SoundSSContinueCh/SoundSongContinueCh
#define PSND_QUICK 0x40            // BGM: dip and undo move 5 steps per frame; ENV (US): dip applied at once
#define PSND_UNSILENT 0x80         // with PSND_CONTINUE: undo PSND_SILENT
#define PSND_FADEIN 0x100          // fade in over the fade time
#define PSND_FADEOUT 0x200         // Off: fade out, then free the slot (On calls ignore it; many evt_snd_bgmon scripts pass it anyway)
#define PSND_PAUSE 0x400           // Off: fade to 0 but keep the slot (PSND_CONTINUE brings it back)
#define PSND_SILENT 0x800          // Off: dip to a lower volume (psndBGMChkSilent) and stay there
#define PSND_SILENT_LEVEL(n) ((n) << 12) // dip target silent_tbl[n] = (n + 1) * 10 % of the volume, n = 0..9
void psndSetFlag(s32 flag);
void psndSFXOff(s32 id);
void psndBGM_stop(void);
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
u32 psndBGMChkSilent(s32 id);
BOOL psndENVOn_f_d(u32 flags, const char* name, u32 fadetime, s32 r6);
BOOL psndBGMOn_f(u32 r3, const char* bgm_name, u32 fadetime, s32 r6);
BOOL psndENVOn(u32 flags, const char* name);
BOOL psndBGMOn_f_d(u32 flags, const char* bgm_name, u32 fadetime, u32 fadeout, s32 r7);
