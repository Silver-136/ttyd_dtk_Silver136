#pragma once

/* SLIB File Structure:
 * chunks of data, first word is size (including these bytes), 0x20 "header"
 * "arrfile" pointer is size+0x20, to end of chunk
 */

#include "dolphin/types.h"
#include "manager/dvdmgr.h"
#include <dolphin/ai.h>
#include <dolphin/ar.h>
// #include <dolphin/arq.h>
#include <dolphin/dvd.h>

#include <musyx/musyx.h>

#define TWO_PI 6.28318530717958647692528676655900576839433879875021f

typedef void (*ReverbCallback)(u8 reason, SND_AUX_INFO* info, void* user);

typedef struct StreamHeader {
    u8 type;        // 0x0
    u16 freq;       // 0x2
    u32 count;      // 0x4
    u32 loopEnd;    // 0x8
    u32 loopStart;  // 0xC
    u32 size0;      // 0x10, channel 0 data size
    u32 size1;      // 0x14, channel 1
    u32 loopStart0; // 0x18
    u32 loopStart1; // 0x1C
    u16 initHist0;  // 0x20, ADPCM initial history
    u16 initHist1;  // 0x22
    u16 loopHist0;  // 0x24, ADPCM loop history
    u16 loopHist1;  // 0x26
    u8 pad_28[0x5C - 0x28];       // 0x28
    SND_ADPCMSTREAM_INFO coef0;   // 0x5C, type 2: channel 0 DSP header (at 0x40) coefficients
    u8 pad_7C[0xBC - 0x7C];       // 0x7C
    SND_ADPCMSTREAM_INFO coef1;   // 0xBC, type 2: channel 1 DSP header (at 0xA0) coefficients
} StreamHeader;

typedef struct SoundGroup {  // TODO: rename
    u16 flags;               // 0x0
    u16 index;               // 0x2, position in the group list (SoundSongGetGroup lookup key)
    SND_GROUPID groupId;     // 0x4
    SND_SONGID songId;       // 0x6
    void* arrfile;           // 0x8
    struct SoundGroup* next; // 0xC
} SoundGroup;

typedef struct SoundSong {
    SND_SEQID seqId;   // 0x0
    u32 vol;           // 0x4, casts to u8
    SoundGroup* group; // 0x8
    u32 mask;          // 0xC
} SoundSong;

typedef struct SoundEffect {
    u16 flags;              // 0x0
    u16 priority;           // 0x2, SoundEfxPlayEx priority; a full table steals the first slot with priority <= the new one
    u16 vol;                // 0x4
    u16 pan;                // 0x6
    u16 span;               // 0x8
    u16 checkDelay;         // 0xA, 3D voices: frames before SoundEfxCheckCh runs again (decremented, never set nonzero in C)
    SND_VOICEID voiceId;    // 0xC
    u16 fid;                // 0x10
    u8 pad_12[0x14 - 0x12]; // 0x12
    SND_EMITTER emitter;    // 0x14
    SND_FVECTOR pos;        // 0x64
    SND_FVECTOR prevPos;    // 0x70
    SND_FVECTOR dir;        // 0x7C
} SoundEffect;

typedef struct SoundDVD {
    u16 flags;               // 0x0
    u16 pan;                 // 0x2, SoundDVDSetPanCh
    f32 volTarget;           // 0x4, fade-in stops here
    f32 vol;                 // 0x8, current volume, split into left/right by pan
    f32 volStep;             // 0xC, added/subtracted per frame while fading
    u8 pad_10[0x114 - 0x10]; // 0x10
    u32 samples;             // 0x114
    DVDFileInfo info;        // 0x118
} SoundDVD;

typedef struct SoundStreamEntry {
    DVDEntry* dvd;             // 0x0
    ARQRequest request;        // 0x4
    SND_STREAMID streamId;     // 0x24
    void* buffer;              // 0x28, sndStream sample buffer, also header read buffer
    u8* loadBuffer[2];         // 0x2C
    u32 offset;                // 0x34, file read position
    u32 pos;                   // 0x38, sample position / aram write position
    u32 loadedLen[2];          // 0x3C
    u32 srcPos;                // 0x44, read position in current load buffer
    u32 requestLength;         // 0x48
    u16 curBuf;                // 0x4C
    s16 initHist;              // 0x4E
    s16 loopHist;              // 0x50
    s16 hist;                  // 0x52
    s16 loaded;                // 0x54
    s16 ready;                 // 0x56
    u16 scale;                 // 0x58
    u16 nibble;                // 0x5A
    u16 runCount;              // 0x5C
    u8 pad5E[2];               // 0x5E
    u32 size;                  // 0x60
    u32 loopStart;             // 0x64
    SND_ADPCMSTREAM_INFO info; // 0x68
} SoundStreamEntry;

typedef struct SoundStream {
    u16 flags;                   // 0x0
    u16 slot;                    // 0x2, aram slot
    u16 dataOffset;              // 0x4
    u16 pan;                     // 0x6
    u16 span;                    // 0x8
    u8 padA[0xC - 0xA];          // 0xA
    f32 vol;                     // 0xC
    f32 curVol;                  // 0x10
    f32 fadeStep;                // 0x14
    u16 freq;                    // 0x18
    u16 count;                   // 0x1A
    u32 loopEnd;                 // 0x1C
    u32 loopStart;               // 0x20
    u32 samplePos;               // 0x24
    SoundStreamEntry entries[2]; // 0x28
} SoundStream;

typedef struct SoundTemp {
    void* callback; // 0x0
    void* data;     // 0x4
    u8 midi;        // 0x8, sndSetAuxProcessingCallbacks midi (0xFF = none)
    u8 pad9[3];     // 0x9
    s32 seqId;      // 0xC, sndSetAuxProcessingCallbacks seqID
} SoundTemp;

typedef struct SoundWork {
    void* poolData[8];         // 0x0
    void* projData[8];         // 0x20
    void* sdirData[8];         // 0x40
    void* slibData[8];         // 0x60
    void* stblData[8];         // 0x80
    void* etblData[8];         // 0xA0
    u32 groupCount[8];         // 0xC0
    u32 dataId;                // 0xE0, not incremented, memory leak if SoundLoadDVD2 called multiple times
    u32 slibId;                // 0xE4
    s32 nextGroupId;           // 0xE8, next SND_GROUPID pushed by SoundLoadDVD2
    SoundGroup* groups;        // 0xEC
    SoundSong* songs;          // 0xF0
    SoundEffect* effects;      // 0xF4
    SoundDVD* dvds;            // 0xF8
    s32 dvdChannel;            // 0xFC, set to -1 whenever a DVD stream stops; never read
    SoundStream* streams;      // 0x100
    s32 streamChannel;         // 0x104, channel of the last started SoundSS stream, -1 after stop
    SoundTemp aux[2];          // 0x108, aux A/B (reverb HI, chorus) callback setup
    SND_LISTENER listener;     // 0x128
    SND_FVECTOR pos;           // 0x1B8
    SND_FVECTOR prevPos;       // 0x1C4
    SND_FVECTOR dir;           // 0x1D0
    SND_FVECTOR heading;       // 0x1DC
    SND_FVECTOR up;            // 0x1E8
    f32 listenerDir;           // 0x1F4, degrees, turned into the listener heading in SoundMain; never written in C
    u8 pad_1F8[0x204 - 0x1F8]; // 0x1F8
    u32 listenerVol;           // 0x204
    u16 fadeIn;                // 0x208
    u16 fadeOut;               // 0x20A
    u16 interruptDepth;        // 0x20C, OSDisableInterrupts nesting count
    u8 pad_20E[0x210 - 0x20E]; // 0x20E
    BOOL interrupts;           // 0x210
    u16 coverState;            // 0x214, 0: closed, 1: cover opened, 2: waiting for streams to reload after close
    u8 pad216[2];              // 0x216
    SND_OUTPUTMODE outputMode; // 0x218
    u32 aramAddr[2];           // 0x21C
    u16 slotCount;             // 0x224
    u16 slotUsed[2];           // 0x226
    char filepath[0x100];      // 0x22A
    u8 pad32A[2];              // 0x32A
    void* filedata;            // 0x32C
    u16 sampleBank;            // 0x330
    u8 pad332[2];              // 0x332
    u8* dmaAddress;            // 0x334
    u32 dmaLength;             // 0x338
    u16 dmaBuf[0x140];         // 0x33C
    AIDCallback callback;      // 0x5BC
    s16 samples[2][0x400];     // 0x5C0
    u32 sampleIndex;           // 0x15C0
} SoundWork;

extern SoundWork sound;

void SoundInit(void);
void SoundMain(void);
void SoundMainInt(void);
void SoundSetFadeTime(u16 in, u16 out);
BOOL SoundLoadDVD2(const char* path);
BOOL SoundLoadDVD2PushGroup(u8* group_ids);
void SoundLoadDVD2Free(void);
BOOL SoundSLibLoadDVD(const char* path);
BOOL SoundDropData(void);
void SoundSetOutputMode(SND_OUTPUTMODE mode);
void SoundOpenCover(void);
void SoundCloseCover(void);

void SoundSongPlayCh(s32 id, u32 mask);
void SoundSongContinueCh(s32 chan);
void SoundSongStopCh(s32 chan);
void SoundSongFadeoutCh(s32 chan);
void SoundSongFadeinCh(s32 chan);
void SoundSongSetVolCh(s32 chan, u8 vol);
u8 SoundSongGetVolCh(s32 chan);
u32 SoundSongCheck(s32 chan);

s32 SoundEfxPlayEx(SND_FXID fid, u8 a2, u8 vol, u8 pan);
void SoundEfxStop(s32 chan);
void SoundEfxSetPitch(s32 chan, u16 pitch);
void SoundEfxSetVolume(s32 chan, u8 vol);
u8 SoundEfxGetVolume(s32 chan);
void SoundEfxSetAux1(s32 chan, u8 reverb);
void SoundEfxSetPan(s32 chan, u8 pan);
void SoundEfxSetSrndPan(s32 chan, u8 span);
void SoundEfxSetLPF(s32 chan, s16 freq);
BOOL SoundEfxCheck(s32 chan);

void SoundSSMuteOff(s32 chan);
u32 SoundSSCheck(s32 chan);
void SoundSSContinueCh(s32 chan);
void SoundSSFadeoutCh(s32 chan);
void SoundSSMuteOn(s32 chan);
void SoundSSPlayCh(s32 chan, const char* name);
void SoundSSSetPanCh(s32 chan, u8 pan);
void SoundSSSetPlayFreqCh(s32 chan, u16 freq);
void SoundSSSetSrndPanCh(s32 chan, u8 span);
void SoundSSSetVolCh(s32 chan, u8 vol);
void SoundSSStopCh(s32 chan);
