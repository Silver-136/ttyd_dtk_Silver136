// sound.h: the game's sound layer over MusyX: music (SoundSong*), sound effects (SoundEfx*), streamed audio and sample banks.

#pragma once

/* SLIB File Structure:
 * chunks of data, first word is size (including these bytes), 0x20 "header"
 * "arrfile" pointer is size+0x20, to end of chunk
 */

#include "dolphin/types.h"
#include "manager/dvdmgr.h"
#include <dolphin/ai.h>
#include <dolphin/ar.h>
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

// SoundGroup.flags
#define SOUND_GROUP_IN_SLIB (1 << 0)          // the .arr data lies in the slib block (not freed on its own)
#define SOUND_GROUP_SONG_ENDED_UNUSED (1 << 1) // a song of the group ended; never read

typedef struct SoundGroup {
    u16 flags;               // 0x0, SOUND_GROUP_*
    u16 index;               // 0x2, position in the group list (searchSongPtr lookup key)
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

// SoundEffect.flags
#define SOUND_EFX_FLAG_PLAY (1 << 0)            // slot in use
#define SOUND_EFX_FLAG_EMITTER_UNUSED (1 << 4)  // a 3D emitter voice; nothing creates one

typedef struct SoundEffect {
    u16 flags;              // 0x0, SOUND_EFX_FLAG_*
    u16 priority;           // 0x2, SoundEfxPlayEx priority; a full table steals the first slot with priority <= the new one
    u16 vol;                // 0x4
    u16 pan;                // 0x6
    u16 span;               // 0x8
    u16 checkDelay;         // 0xA, 3D voices: frames before SoundEfxCheckCh runs again (decremented, never set nonzero in C)
    SND_VOICEID voiceId;    // 0xC
    u16 fid;                // 0x10
    SND_EMITTER emitter;    // 0x14
    SND_FVECTOR pos;        // 0x64
    SND_FVECTOR prevPos;    // 0x70
    SND_FVECTOR dir;        // 0x7C
} SoundEffect;

// SoundDVD.flags (their setters are stripped from every build except MUTE in SoundDVDMain)
typedef enum SoundDVDFlags {
    SOUND_DVD_FLAG_PLAY = (1 << 0),     // 0x1
    SOUND_DVD_FLAG_MUTE = (1 << 1),     // 0x2
    SOUND_DVD_FLAG_FADEIN = (1 << 4),   // 0x10
    SOUND_DVD_FLAG_FADEOUT = (1 << 5),  // 0x20
    SOUND_DVD_FLAG_FADEMUTE = (1 << 6), // 0x40
} SoundDVDFlags;

typedef struct SoundDVD {
    u16 flags;               // 0x0, SoundDVDFlags
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
    u32 size;                  // 0x60
    u32 loopStart;             // 0x64
    SND_ADPCMSTREAM_INFO info; // 0x68
} SoundStreamEntry;

// Stream file header type: selects the decoder flag SoundSSPlayChEx_main sets (none for raw samples).
typedef enum SoundStreamType {
    SOUND_STREAM_TYPE_RAW = 0,
    SOUND_STREAM_TYPE_DPCM = 1,  // SOUND_STREAM_FLAG_DPCM
    SOUND_STREAM_TYPE_ADPCM = 2, // SOUND_STREAM_FLAG_ADPCM
} SoundStreamType;

// SoundStream.flags (SoundSS* functions; JP demo map for the stripped setters). 0x800 is not used.
typedef enum SoundStreamFlags {
    SOUND_STREAM_FLAG_PLAY = (1 << 0),      // 0x1, channel in use
    SOUND_STREAM_FLAG_MUTE = (1 << 1),      // 0x2, SoundSSMuteOn
    SOUND_STREAM_FLAG_END = (1 << 2),       // 0x4, a non-looping stream reached its end
    SOUND_STREAM_FLAG_STARTED = (1 << 3),   // 0x8, psndBGMStartCheck
    SOUND_STREAM_FLAG_FADEIN = (1 << 4),    // 0x10
    SOUND_STREAM_FLAG_FADEOUT = (1 << 5),   // 0x20, SoundSSFadeoutCh
    SOUND_STREAM_FLAG_FADEMUTE = (1 << 6),  // 0x40
    SOUND_STREAM_FLAG_DPCM = (1 << 7),      // 0x80, ssDecodeDPCM
    SOUND_STREAM_FLAG_ADPCM = (1 << 8),     // 0x100
    SOUND_STREAM_FLAG_LOOP = (1 << 9),      // 0x200
    SOUND_STREAM_FLAG_ARAM = (1 << 10),     // 0x400
    SOUND_STREAM_FLAG_READ_L = (1 << 12),   // 0x1000, a read for channel 0 in flight
    SOUND_STREAM_FLAG_READ_R = (1 << 13),   // 0x2000, a read for channel 1 in flight
    SOUND_STREAM_FLAG_READ_END = (1 << 14), // 0x4000, no more data will come
    SOUND_STREAM_FLAG_PAUSE = (1 << 15),    // 0x8000, cleared by evt_snd_bgm_unpause
} SoundStreamFlags;

typedef struct SoundStream {
    u16 flags;                   // 0x0, SoundStreamFlags
    u16 slot;                    // 0x2, aram slot
    u16 dataOffset;              // 0x4
    u16 pan;                     // 0x6
    u16 span;                    // 0x8
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
    s32 seqId;      // 0xC, sndSetAuxProcessingCallbacks seqID
} SoundTemp;

// SoundWork.coverState: disc cover.
enum {
    SOUND_COVER_CLOSED = 0,
    SOUND_COVER_OPEN = 1,
    SOUND_COVER_RELOADING = 2,  // closed again; waiting for the streams to reload
};

// Sizes of the tables SoundInit allocates
#define SOUND_SONG_MAX 4    // SoundSong
#define SOUND_EFFECT_MAX 40 // SoundEffect channels
#define SOUND_DVD_MAX 2     // SoundDVD
#define SOUND_STREAM_MAX 3  // SoundStream

typedef struct SoundWork {
    void* poolData[8];         // 0x0, .pool file of each loaded project (SoundLoadDVD2), by dataId
    void* projData[8];         // 0x20, .proj file, by dataId
    void* sdirData[8];         // 0x40, .sdir file, by dataId
    void* slibData[8];         // 0x60, .slib song library file, by slibId (SoundSLibLoadDVD)
    void* stblData[8];         // 0x80, .stbl file, by slibId
    void* etblData[8];         // 0xA0, .etbl file, by slibId
    u32 groupCount[8];         // 0xC0, groups pushed from each project (SoundLoadDVD2PushGroup)
    u32 dataId;                // 0xE0, not incremented, memory leak if SoundLoadDVD2 called multiple times
    u32 slibId;                // 0xE4, next slibData slot (up to 8)
    s32 nextGroupId;           // 0xE8, next SND_GROUPID pushed by SoundLoadDVD2
    SoundGroup* groups;        // 0xEC, SoundGroup list, one per .slib song (next links)
    SoundSong* songs;          // 0xF0, SOUND_SONG_MAX entries (SoundInit)
    SoundEffect* effects;      // 0xF4, SOUND_EFFECT_MAX
    SoundDVD* dvds;            // 0xF8, SOUND_DVD_MAX
    s32 dvdChannel;            // 0xFC, set to -1 whenever a DVD stream stops; never read
    SoundStream* streams;      // 0x100, SOUND_STREAM_MAX
    s32 streamChannel;         // 0x104, channel of the last started SoundSS stream, -1 after stop
    SoundTemp aux[2];          // 0x108, aux A/B (reverb HI, chorus) callback setup
    SND_LISTENER listener;     // 0x128, MusyX listener (sndUpdateListener in SoundEfxMain)
    SND_FVECTOR pos;           // 0x1B8, listener position
    SND_FVECTOR prevPos;       // 0x1C4, pos last frame; the difference is the listener velocity
    SND_FVECTOR dir;           // 0x1D0, listener velocity (pos - prevPos)
    SND_FVECTOR heading;       // 0x1DC, listener heading, from listenerDir
    SND_FVECTOR up;            // 0x1E8, listener up vector, (0, 1, 0)
    f32 listenerDir;           // 0x1F4, degrees, turned into the listener heading in SoundMain; never written in C
    u8 pad_1F8[0x204 - 0x1F8]; // 0x1F8
    u32 listenerVol;           // 0x204, volume passed to sndUpdateListener; never written in C
    u16 fadeIn;                // 0x208, song/stream fade-in ms (SoundSetFadeTime; 10 at SoundInit)
    u16 fadeOut;               // 0x20A, song/stream fade-out ms (500 at SoundInit)
    u16 interruptDepth;        // 0x20C, OSDisableInterrupts nesting count
    BOOL interrupts;           // 0x210, OSDisableInterrupts state restored by sndEnableIRQ
    u16 coverState;            // 0x214, 0: closed, 1: cover opened, 2: waiting for streams to reload after close
    SND_OUTPUTMODE outputMode; // 0x218, SoundSetOutputMode (stereo at SoundInit)
    u32 aramAddr[2];           // 0x21C, ARAM base of each stream slot; never written in C
    u16 slotCount;             // 0x224, ARAM stream slots; only set to 0, so SoundSSPlayChEx's aram path returns
    u16 slotUsed[2];           // 0x226, 1 while a stream holds that ARAM slot
    char filepath[0x100];      // 0x22A, .samp file the sample upload callback reads
    void* filedata;            // 0x32C, 0x20000-byte sample upload buffer (SoundInit)
    u16 sampleBank;            // 0x330, samples bank being filled (0/1)
    u8* dmaAddress;            // 0x334, audio DMA buffer (AIGetDMAStartAddr)
    u32 dmaLength;             // 0x338, its length in bytes
    u16 dmaBuf[0x140];         // 0x33C, byte-swapped copy of the DMA buffer
    AIDCallback callback;      // 0x5BC, previous AI DMA callback, chained by SoundAIDMACallback
    s16 samples[2][0x400];     // 0x5C0, record of the output, two banks of 0x400 samples (one channel)
    u32 sampleIndex;           // 0x15C0, next sample in the current bank
} SoundWork;

extern SoundWork sound;

char* makeFilename(const char* path, const char* ext);
void* ReadMusyXData(const char* name);
void sndEnableIRQ(void);
void sndDisableIRQ(void);
void SoundInit(void);
void SoundMain(void);
void SoundMainInt(void);
void SoundSetFadeTime(u16 in, u16 out);
BOOL SoundLoadDVD2(const char* path);
BOOL SoundLoadDVD2PushGroup(u8* groupIds);
void SoundLoadDVD2Free(void);
BOOL SoundSLibLoadDVD(const char* path);
BOOL SoundDropData(void);
void SoundSetOutputMode(SND_OUTPUTMODE mode);
SND_OUTPUTMODE SoundGetOutputMode(void);
void SoundOpenCover(void);
void SoundCloseCover(void);

void SoundSongPlay(u32 groupInfo);

void SoundSongPlayCh(s32 chan, u32 groupInfo);
void SoundSongContinue(void);
void SoundSongContinueCh(s32 chan);
void SoundSongStop(void);
void SoundSongStopCh(s32 chan);
void SoundSongFadeout(void);
void SoundSongFadeoutCh(s32 chan);
void SoundSongFademute(void);
void SoundSongFademuteCh(s32 chan);
void SoundSongFadein(void);
void SoundSongFadeinCh(s32 chan);
void SoundSongSetVol(u8 vol);
void SoundSongSetVolCh(s32 chan, u8 vol);
u8 SoundSongGetVolCh(s32 chan);
void SoundSongMuteTrackOn(u8 track);
void SoundSongMuteTrackOnCh(s32 chan, u8 track);
void SoundSongMuteTrackOff(u8 track);
void SoundSongMuteTrackOffCh(s32 chan, u8 track);
void SoundSongSetMuteTrack(u32 mask);
void SoundSongSetMuteTrackCh(s32 chan, u32 mask);
u32 SoundSongCheck(s32 chan);
u8 SoundSongGetMIDICtrl(u8 channel, u8 ctrl);
u8 SoundSongGetMIDICtrlCh(s32 chan, u8 channel, u8 ctrl);
BOOL SoundSongSetMIDICtrl(u8 channel, u8 ctrl, u8 value);
BOOL SoundSongSetMIDICtrlCh(s32 chan, u8 channel, u8 ctrl, u8 value);
void SoundSongMain(void);

void sndMovePos(f32* x, f32* z, f32 dist, f32 dir);
void calcVectEmitter(SoundEffect* effect);
void calcVectListener(void);
s32 SoundEfxPlayEx(SND_FXID fid, u8 priority, u8 vol, u8 pan);
void SoundEfxSetPosDirListener(SND_FVECTOR* pos, f32 dir);
void SoundEfxSetPosEmitter(s32 chan, SND_FVECTOR* pos);
void SoundEfxStop(s32 chan);
SND_VOICEID SoundEfxGetVoiceID(s32 chan);
SND_FXID SoundEfxGetFXID(s32 chan);
void SoundEfxSetPitch(s32 chan, s16 pitch);
void SoundEfxSetPitchDirect(s32 chan, u16 pitch);
void SoundEfxSetVolume(s32 chan, u8 vol);
u8 SoundEfxGetVolume(s32 chan);
void SoundEfxSetAux1(s32 chan, u8 reverb);
void SoundEfxSetAux2(s32 chan, u8 chorus);
void SoundEfxSetPan(s32 chan, u8 pan);
void SoundEfxSetSrndPan(s32 chan, u8 span);
void SoundEfxSetLPF(s32 chan, s16 freq);
u32 SoundEfxCheck(s32 chan);

void SoundDVDStop(void);
void SoundDVDSetPan(u8 pan);
void SoundDVDSetVol(u8 vol);
void SoundDVDSetVolCh(u32 chan, u8 vol);
void SoundDVDFadeout(void);
void SoundDVDFademute(void);
void SoundDVDFadein(void);

void SoundSSMuteOff(s32 chan);
u32 SoundSSCheck(s32 chan);
void SoundSSContinueCh(s32 chan);
void SoundSSContinue(void);
void SoundSSFadeoutCh(s32 chan);
void SoundSSFadeout(void);
void SoundSSFademute(void);
void SoundSSFademuteCh(s32 chan);
void SoundSSFadein(void);
void SoundSSFadeinCh(s32 chan);
void SoundSSFadeinChEx(s32 chan, s32 time);
void SoundSSMuteOn(s32 chan);
void SoundSSPlay(const char* name);
void SoundSSPlayCh(s32 chan, const char* name);
void SoundSSSetPanCh(u32 chan, u8 pan);
void SoundSSSetPan(u8 pan);
void SoundSSSetPlayFreqCh(s32 chan, u16 freq);
void SoundSSSetPlayFreq(u16 freq);
void SoundSSSetSrndPanCh(u32 chan, u8 span);
void SoundSSSetSrndPan(u8 span);
void SoundSSSetVolCh(u32 chan, u8 vol);
void SoundSSSetVol(u8 vol);
void SoundSSStopCh(u32 chan);
void SoundSSStop(void);
u8 SoundSSGetVolCh(s32 chan);
void SoundStopAll(void);
void SoundInitAI(void);
