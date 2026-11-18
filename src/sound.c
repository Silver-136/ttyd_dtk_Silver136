// sound.c: the sound driver over MusyX (Sound*): sound effects with 3D emitters, songs, ADPCM streams read from the
// DVD, reverb, output mode, and loading the sound project data into ARAM.

#include "version.h"
#include "sound.h"
#include <dolphin/os.h>
#include "manager/dvdmgr.h"
#include "memory.h"
#include "musyx/musyx.h"
#include "system.h"
#include "libc_decls.h"
#include <string.h>
#include "eff/eff_magic1_n64.h"

#if VERSION >= VERSION_US
#include "mario/mariost.h"
#endif

//.bss
#if VERSION <= VERSION_DEMO_JP
static u32 aramMemArray[8];
#endif

static SND_AUX_REVERBHI revH;
static SND_AUX_REVERBSTD revS;
static SND_AUX_CHORUS cho;
static SND_AUX_DELAY dly;
#define SS_LOAD_BUFFER_SIZE 0x8000 // bytes in each stream load buffer, the size of one DVD/ARAM read

SoundWork sound;

//.sbss
static u32 aramMemArray[8];

/** Returns the shared 256-byte buffer the loaders build file paths in. */
static inline char* _ssGetStr(void) {
    static char str[0x100];
    return str;
}

// local prototypes
static void SoundEfxMain(void);
inline void SoundEfxMuteCh(u32 chan);
static void SoundDVDMain(void);
static void SoundDVDStopCh(u32 chan);
static void SoundDVDSetPanCh(u32 chan, u8 pan);
static void SoundDVDFadeoutCh(u32 chan);
static void SoundDVDFademuteCh(u32 chan);
static void SoundDVDFadeinCh(u32 chan);

static void SoundSSMainInt(void);
static void SoundSSMain(void);

static void SoundAIDMACallback(void);

/** Undoes one sndDisableIRQ, restoring interrupts at the outermost level. */
// Original name and size (0x48) from the JP demo map (UNUSED there: every call is inlined).
void sndEnableIRQ(void) {
    if (sound.interruptDepth) {
        if (!--sound.interruptDepth) {
            OSRestoreInterrupts(sound.interrupts);
        }
    }
}

/** Disables interrupts for stream bookkeeping; nests. */
// Original name and size (0x54) from the JP demo map (UNUSED there: every call is inlined).
void sndDisableIRQ(void) {
    if (!sound.interruptDepth) {
        sound.interrupts = OSDisableInterrupts();
    }
    sound.interruptDepth++;
}

/** MusyX allocation hook: allocates from the default heap. */
void* sndMalloc(size_t size) {
    return __memAlloc(HEAP_DEFAULT, size);
}

/** MusyX free hook: frees to the default heap. */
void sndFree(void* ptr) {
    __memFree(HEAP_DEFAULT, ptr);
}

/** Builds "<DVD root>/<path><ext>" in the shared string buffer and returns it. */
// Original name and size (0x64) from the JP demo map (UNUSED there: every call is inlined).
char* makeFilename(const char* path, const char* ext) {
    sprintf(_ssGetStr(), "%s/%s%s", getMarioStDvdRoot(), path, ext);
    return _ssGetStr();
}

/**
 * Reads the whole file name into a new default-heap buffer (size rounded up to 32 bytes) and returns it, or NULL
 * when the file is missing or empty, the allocation fails or the read fails.
 */
// Original name and size (0xc4) from the JP demo map (UNUSED there: every call is inlined).
void* ReadMusyXData(const char* name) {
    DVDEntry* entry;
    u32 size;
    u32 length;
    void* pointer;

    entry = DVDMgrOpen(name, 2, 0);
    if (entry == NULL) {
        return NULL;
    }
    size = DVDMgrGetLength(entry);
    if (size == 0) {
        return NULL;
    }
    length = OSRoundUp32B(size);
    pointer = __memAlloc(HEAP_DEFAULT, length);

    // BUG: DVDMgrClose runs only after a good read: an empty file, a failed allocation or a failed read leaves the
    // entry open.
    if (pointer == NULL) {
        return NULL;
    }
    if (DVDMgrRead(entry, pointer, length, 0) <= 0) {
        __memFree(HEAP_DEFAULT, pointer);
        return NULL;
    }
    DVDMgrClose(entry);
    return pointer;
}

/** Returns the song library group with index, or NULL. */
// Original name and size (0x38) from the JP demo map (UNUSED there: every call is inlined).
SoundGroup* searchSongPtr(u8 groupIndex) {
    u16 index = groupIndex;
    SoundGroup* group;

    for (group = sound.groups;; group = group->next) {
        if (group == NULL) {
            return NULL;
        }
        if (group->index == index) {
            break;
        }
    }
    return group;
}

/**
 * Boots the sound system: sets up ARAM and its queue, starts MusyX (64 voices, 12 MB of ARAM),
 * prepares the reverb, chorus and delay effects, sets stereo output,
 * hooks the audio DMA callback and allocates the song (4), effect (40),
 * DVD audio (2) and stream (3) channel tables with their buffers. Fade in/out default to 10 and 500 ms.
 */
void SoundInit(void) {
    SND_HOOKS hooks = (SND_HOOKS){ sndMalloc, sndFree };
    void* data;
    SoundStream* streams;
    SoundSong* songs;
    SoundDVD* dvds;
    SoundEffect* effects;
    int i;
    SoundStream* temp;
    SoundSong* tempsong;
    SoundDVD* tempdvd;
    void* ptr;
    u32 size;
    SoundTemp* slot;

    ARInit(aramMemArray, 8);
    ARQInit();
    AIInit(0);
    ARAlloc(0xBFC000); // 12MB-16KB (reserved)
    sndSetHooks(&hooks);
    sndInit(0x40, 0xC, 0x2C, 1, 1, 0xBFC000);
    revH.tempDisableFX = 0;
    revH.time = 3.0f;
    revH.preDelay = 0.1f;
    revH.damping = 0.3f;
    revH.coloration = 0.0f;
    revH.crosstalk = 0.0f;
    revH.mix = 0.8f;
    sndAuxCallbackPrepareReverbHI(&revH);
    revS.tempDisableFX = 0;
    revS.time = 2.0f;
    revS.preDelay = 0.1f;
    revS.damping = 0.5f;
    revS.coloration = 0.5f;
    revS.mix = 0.5f;
    sndAuxCallbackPrepareReverbSTD(&revS);
    cho.baseDelay = 10;
    cho.variation = 0;
    cho.period = 500;
    sndAuxCallbackPrepareChorus(&cho);
    dly.delay[0] = 60;
    dly.delay[1] = 90;
    dly.delay[2] = 70;
    dly.feedback[0] = 20;
    dly.feedback[1] = 20;
    dly.feedback[2] = 20;
    dly.output[0] = 50;
    dly.output[1] = 50;
    dly.output[2] = 50;
    sndAuxCallbackPrepareDelay(&dly);
    sndSetAuxProcessingCallbacks(0, sndAuxCallbackReverbHI, &revH, 0xFF, 0, sndAuxCallbackChorus, &cho, 0xFF, 0);
    sndVolume(0x7F, 0, 0xFF);
    SoundSetOutputMode(SND_OUTPUTMODE_STEREO);
    SoundInitAI();
    sound.fadeIn = 10;
    sound.fadeOut = 500;
    sound.dataId = 0;
    sound.slibId = 0;
    sound.coverState = SOUND_COVER_CLOSED;
    sound.slotCount = 0;
    sound.nextGroupId = 0;

    data = sndMalloc(0x20000);
    sound.filedata = data;
    if (data == NULL) {
        while (1)
            ;
    }

    sound.groups = NULL;

    songs = sndMalloc(sizeof(SoundSong) * SOUND_SONG_MAX);
    sound.songs = songs;
    if (songs == NULL) {
        while (1)
            ;
    }

    tempsong = songs;
    for (i = 0; i < SOUND_SONG_MAX; i++, tempsong++) {
        tempsong->seqId = -1;
        tempsong->group = NULL;
    }

    effects = sndMalloc(sizeof(SoundEffect) * SOUND_EFFECT_MAX);
    sound.effects = effects;
    if (effects == NULL) {
        while (1)
            ;
    }

    for (i = 0; i < SOUND_EFFECT_MAX; i++, effects++) {
        effects->flags = 0;
        effects->voiceId = -1;
        effects->priority = 0;
        effects->vol = 0x7F;
        effects->pan = 0x40;
    }

    dvds = sndMalloc(sizeof(SoundDVD) * SOUND_DVD_MAX);
    sound.dvds = dvds;
    if (dvds == NULL) {
        while (1)
            ;
    }

    tempdvd = dvds;
    for (i = 0; i < SOUND_DVD_MAX; i++, tempdvd++) {
        tempdvd->flags = 0;
    }

    streams = sndMalloc(sizeof(SoundStream) * SOUND_STREAM_MAX);
    sound.streams = streams;
    if (streams == NULL) {
        while (1)
            ;
    }

    temp = streams;
    for (i = 0; i < SOUND_STREAM_MAX; i++, temp++) {
        temp->flags = 0;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == NULL) {
            while (1)
                ;
        }
        temp->entries[0].loadBuffer[0] = ptr;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == NULL) {
            while (1)
                ;
        }
        temp->entries[0].loadBuffer[1] = ptr;

        size = sndStreamAllocLength(0xE00, 0);
        ptr = sndMalloc(size);
        if (ptr == NULL) {
            while (1)
                ;
        }
        temp->entries[0].buffer = ptr;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == NULL) {
            while (1)
                ;
        }
        temp->entries[1].loadBuffer[0] = ptr;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == NULL) {
            while (1)
                ;
        }
        temp->entries[1].loadBuffer[1] = ptr;

        size = sndStreamAllocLength(0xE00, 0);
        ptr = sndMalloc(size);
        if (ptr == NULL) {
            while (1)
                ;
        }
        temp->entries[1].buffer = ptr;
    }

    slot = sound.aux;
    slot->callback = sndAuxCallbackReverbHI;
    slot->data = &revH;
    slot->midi = 0xFF;
    slot->seqId = 0;

    slot++;
    slot->callback = sndAuxCallbackChorus;
    slot->data = &cho;
    slot->midi = 0xFF;
    slot->seqId = 0;
}

/** Per-frame sound update: runs the song, effect, DVD-audio and stream updates. */
void SoundMain(void) {
    SoundSongMain();
    SoundEfxMain();
    SoundDVDMain();
    SoundSSMain();
}

/** Audio-interrupt update: services the streams' DVD reads (SoundSSMainInt). */
void SoundMainInt(void) {
    SoundSSMainInt();
}

/** Sets the song and stream fade-in and fade-out times in milliseconds. */
void SoundSetFadeTime(u16 in, u16 out) {
    sound.fadeIn = in;
    sound.fadeOut = out;
}

/**
 * MusyX sample upload callback: reads bytes at offset of the current sample file (.samp) from the disc into the
 * 128 KB staging buffer and returns it, or NULL on a read error.
 */
static void* loadDVD_callback(u32 offset, u32 bytes) {
    DVDFileInfo info;

    if (!DVDOpen(sound.filepath, &info)) {
        return NULL;
    }
    if (DVDRead(&info, sound.filedata, (s32)bytes, (s32)offset) <= 0) {
        return NULL;
    }
    DVDClose(&info);
    return sound.filedata;
}

/**
 * Loads a MusyX project from the disc: path + ".pool", ".proj" and ".sdir" into memory,
 * and registers the ".samp" file for sample upload through loadDVD_callback.
 * Stores them in project slot dataId, which nothing increments, so every call uses slot 0;
 * returns FALSE when a file fails.
 */
BOOL SoundLoadDVD2(const char* path) {
#if VERSION <= VERSION_DEMO_JP
    const char* root;
#endif
    void* pointer;

    if (sound.dataId >= 8) {
        return FALSE;
    }
#if VERSION <= VERSION_DEMO_JP
    root = getMarioStDvdRoot();
    sprintf(_ssGetStr(), "%s/%s%s", root, path, ".pool");
    pointer = ReadMusyXData(_ssGetStr());
#else
    pointer = ReadMusyXData(makeFilename(path, ".pool"));
#endif
    if (pointer == NULL) {
        return FALSE;
    }
    sound.poolData[sound.dataId] = pointer;

    pointer = ReadMusyXData(makeFilename(path, ".proj"));
    if (pointer == NULL) {
        return FALSE;
    }
    sound.projData[sound.dataId] = pointer;

    pointer = ReadMusyXData(makeFilename(path, ".sdir"));
    if (pointer == NULL) {
        return FALSE;
    }
    sound.sdirData[sound.dataId] = pointer;

    strcpy(sound.filepath, makeFilename(path, ".samp"));
    sndSetSampleDataUploadCallback(loadDVD_callback, 0x20000);
    return TRUE;
}

/**
 * Pushes the sound groups listed in groupIds (0xFF-terminated; NULL = every group from nextGroupId on until a push
 * fails) of the project in slot dataId (slot 0) into MusyX,
 * uploading their samples to ARAM, and counts them for SoundDropData.
 */
BOOL SoundLoadDVD2PushGroup(u8* groupIds) {
    SND_GROUPID id;
    s32 temp;

    if (sound.dataId >= 8) {
        return FALSE;
    }
    sound.groupCount[sound.dataId] = 0;
    if (groupIds) {
        while (*groupIds != 0xFF) {
            id = *groupIds;
            sndPushGroup(sound.projData[sound.dataId], id, 0, sound.sdirData[sound.dataId],
                         sound.poolData[sound.dataId]);
            sound.groupCount[sound.dataId]++;
            groupIds++;
        }
    } else {
        temp = sound.nextGroupId;
        while (temp < 0xFF) {
            if (sndPushGroup(sound.projData[sound.dataId], (SND_GROUPID)temp, 0, sound.sdirData[sound.dataId],
                             sound.poolData[sound.dataId]) != TRUE) {
                break;
            }
            sound.groupCount[sound.dataId]++;
            sound.nextGroupId++;
            temp++;
        }
    }
    return TRUE;
}

/** Frees the sample upload staging buffer once loading is done. */
void SoundLoadDVD2Free(void) {
    if (sound.filedata) {
        __memFree(HEAP_DEFAULT, sound.filedata);
        sound.filedata = NULL;
    }
}

/**
 * Loads the song library: path + ".slib" split into one SoundGroup per song (song ids are their order),
 * plus the ".stbl" and ".etbl" tables. The game loads it once at boot for sound/proj/pmario.
 */
BOOL SoundSLibLoadDVD(const char* path) {
    SoundGroup* groupsHead;
    SoundGroup* iter;
    SoundGroup* group;
    u32* cursor;
    u32 blockSize;
    u16 id;
    void* pointer;
    void* pointer2;
    void* pointer3;

    if (sound.slibId >= 8) {
        return FALSE;
    }

    pointer = ReadMusyXData(makeFilename(path, ".slib"));
    if ((cursor = pointer) == NULL) {
        return FALSE;
    }
    sound.slibData[sound.slibId] = pointer;

    while (1) {
        blockSize = *cursor;
        if (*cursor == 0xFFFFFFFF) {
            break;
        }
        group = __memAlloc(HEAP_DEFAULT, sizeof(SoundGroup));
        if (group == NULL) {
            while (1)
                ;
        }
        groupsHead = sound.groups;
        id = 0;
        if (groupsHead == NULL) {
            sound.groups = group;
        } else {
            for (iter = sound.groups;; iter = iter->next) {
                id++;
                if (!iter->next) {
                    break;
                }
            }
            iter->next = group;
        }
        group->flags = SOUND_GROUP_IN_SLIB;
        group->index = id;
        group->groupId = 0;
        group->songId = 0;
        group->arrfile = cursor + 8;
        group->next = NULL;
        cursor = (u32*)((u8*)cursor + blockSize);
    }

    pointer2 = ReadMusyXData(makeFilename(path, ".stbl"));
    if (pointer2 == NULL) {
        return FALSE;
    }
    sound.stblData[sound.slibId] = pointer2;

    pointer3 = ReadMusyXData(makeFilename(path, ".etbl"));
    if (pointer3 == NULL) {
        return FALSE;
    }
    sound.etblData[sound.slibId] = pointer3;
    sound.slibId++;
    return TRUE;
}

/**
 * Unloads all sound data: stops everything, frees the song library and group memory. The loops that pop the
 * pushed groups and free the project files run up to dataId, which is never incremented (0), so they do nothing.
 * Returns TRUE.
 */
BOOL SoundDropData(void) {
    SoundGroup* group;
    SoundGroup* temp;
    u32 i;

    group = sound.groups;
    SoundStopAll();
    sndSilence();

    // a second stop pass: the song loop written out, effects, DVD audio and streams through their channel stops; a
    // second SoundStopAll() call, or SoundSongStopCh for the song loop, changes the loops' register order
    for (i = 0; i < SOUND_SONG_MAX; i++) {
        SoundSong* song = &sound.songs[i];
        if (sndSeqGetValid(song->seqId)) {
            sndSeqStop(song->seqId);
            song->seqId = (SND_SEQID)(-1);
        }
    }
    for (i = 0; i < SOUND_EFFECT_MAX; i++) {
        SoundEfxMuteCh(i);
    }

    for (i = 0; i < SOUND_DVD_MAX; i++) {
        SoundDVDStopCh(i);
    }

    for (i = 0; i < SOUND_STREAM_MAX; i++) {
        SoundSSStopCh(i);
    }

    // BUG: SoundLoadDVD2 stores into slot dataId but never increments it, so dataId is 0 here and none of these
    // loops runs: the groups stay pushed and the slot 0 project buffers are not freed.
    for (i = 0; i < sound.dataId; i++) {
        while (sound.groupCount[i]) {
            sndPopGroup();
            sound.groupCount[i]--;
        }
    }

    for (i = 0; i < sound.dataId; i++) {
        __memFree(HEAP_DEFAULT, sound.poolData[i]);
    }

    for (i = 0; i < sound.dataId; i++) {
        __memFree(HEAP_DEFAULT, sound.projData[i]);
    }

    for (i = 0; i < sound.dataId; i++) {
        __memFree(HEAP_DEFAULT, sound.sdirData[i]);
    }

    for (i = 0; i < sound.slibId; i++) {
        __memFree(HEAP_DEFAULT, sound.slibData[i]);
        __memFree(HEAP_DEFAULT, sound.stblData[i]);
        __memFree(HEAP_DEFAULT, sound.etblData[i]);
    }

    sound.dataId = 0;
    sound.slibId = 0;
    sound.nextGroupId = 0;

    while (1) {
        if (!group) {
            break;
        }
        temp = group->next;
        if (!(group->flags & SOUND_GROUP_IN_SLIB)) {
            __memFree(HEAP_DEFAULT, group->arrfile);
        }
        __memFree(HEAP_DEFAULT, group);
        group = temp;
    }
    sound.groups = NULL;

    return TRUE;
}

/**
 * Stops every song, effect, DVD-audio track and stream. Only SoundDropData calls it (inlined); the JP demo map
 * lists its out-of-line copy as stripped (SoundStopAll, 0x208 bytes, which this body compiles to there).
 */
// The effect, DVD and stream loops call their channel stops (inlined: the same code as writing them out); with
// them the body has 23 statements, so as a plain function it is auto-inlined into SoundDropData and its own copy
// is emitted at the map size.
void SoundStopAll(void) {
    u32 i;

    for (i = 0; i < SOUND_SONG_MAX; i++) {
        SoundSong* song = &sound.songs[i];
        if (sndSeqGetValid(song->seqId)) {
            sndSeqStop(song->seqId);
            song->seqId = (SND_SEQID)-1;
        }
    }
    for (i = 0; i < SOUND_EFFECT_MAX; i++) {
        SoundEfxMuteCh(i);
    }

    for (i = 0; i < SOUND_DVD_MAX; i++) {
        SoundDVDStopCh(i);
    }

    for (i = 0; i < SOUND_STREAM_MAX; i++) {
        SoundSSStopCh(i);
    }
}

/** Switches MusyX between mono, stereo and surround output (JP and later only when it changes). */
void SoundSetOutputMode(SND_OUTPUTMODE mode) {
#if VERSION >= VERSION_JP
    if (sound.outputMode != mode) {
        sound.outputMode = mode;
        sndOutputMode(mode);
    }
#else
    sound.outputMode = mode;
    sndOutputMode(mode);
#endif
}

/** Returns the output mode last set. */
// Original name and size (0x10) from the JP demo map (UNUSED there: nothing calls it); the getter of
// SoundSetOutputMode's field.
SND_OUTPUTMODE SoundGetOutputMode(void) {
    return sound.outputMode;
}

/** Called when the disc cover opens: mutes sound until it is closed. */
void SoundOpenCover(void) {
    if (!sound.coverState) {
        sound.coverState = SOUND_COVER_OPEN;
    }
}

/**
 * Called when the disc cover closes: unmutes, or waits (coverState 2)
 * while a stream still has a disc read in flight.
 */
void SoundCloseCover(void) {
    SoundStream* stream;
    BOOL close;
    int i;

    if (sndIsInstalled() && sound.coverState == SOUND_COVER_OPEN) {
        close = FALSE;
        for (i = 0; i < SOUND_STREAM_MAX; i++) {
            stream = &sound.streams[i];
            if (stream->flags & SOUND_STREAM_FLAG_STARTED && !(stream->flags & SOUND_STREAM_FLAG_ARAM) && !(stream->flags & SOUND_STREAM_FLAG_MUTE)) {
                if (stream->flags & SOUND_STREAM_FLAG_READ_L) {
                    close = TRUE;
                }
                if (stream->count == 2 && stream->flags & SOUND_STREAM_FLAG_READ_R) {
                    close = TRUE;
                }
            }
        }
        if (close) {
            sound.coverState = SOUND_COVER_RELOADING;
        } else {
            sound.coverState = SOUND_COVER_CLOSED;
        }
    }
}

/** SoundSongPlayCh for song channel 0. */
// The channel-0 forms (SoundSongPlay, SoundSongContinue, SoundSongStop, SoundSongFadeout, SoundSongFadein,
// SoundSongSetVol, SoundDVDStop, SoundDVDSetPan, SoundSSPlay, SoundSSStop, SoundSSFadeout, SoundSSFademute,
// SoundSSContinue, SoundSSSetPan, SoundSSSetVol, SoundSSSetSrndPan, SoundSSSetPlayFreq):
// original names and sizes from the JP demo map (UNUSED there: nothing calls them). The map lists each right before
// its ...Ch function, and each size is the ...Ch body inlined for channel 0 (0x100 here).
void SoundSongPlay(u32 groupInfo) {
    SoundSongPlayCh(0, groupInfo);
}

/**
 * Starts the song in groupInfo (low byte: library index, bits 8-15: song id, bits 16-23: MusyX group id) on song
 * channel chan at full volume,
 * stopping what played there.
 */
void SoundSongPlayCh(s32 chan, u32 groupInfo) {
    SoundSong* song;
    SoundGroup* group;

    song = &sound.songs[chan];
    if (sndSeqGetValid(song->seqId) == TRUE) {
        sndSeqStop(song->seqId);
    }
    song->vol = 127;
    group = searchSongPtr(groupInfo);
    if (group) {
        song->group = group;
        group->groupId = (SND_GROUPID)((groupInfo >> 16) & 0xFF);
        group->songId = (SND_SONGID)((groupInfo >> 8) & 0xFF);
        song->mask = 0xFFFFFFFF;
        song->seqId = sndSeqPlayEx(group->groupId, group->songId, group->arrfile, 0, 0);
        sndSeqVolume(song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
        sndSeqContinue(song->seqId);
        sndSeqMute(song->seqId, song->mask, 0);
    }
}

#if VERSION <= VERSION_DEMO_JP
// FAKE: stand-in for a dead-stripped song loader; nothing calls it. It only places ".song" (JP demo map @3563) in the
// pool. Its number lies between SoundEfxMain's constants and SoundSLibLoadDVD's strings, and the map lists
// SoundSongLoadPlay (0x2c) and SoundSongLoadPlayCh (0x258) UNUSED here, after SoundSongPlayCh: loading a song file
// fits SoundSongLoadPlayCh. Body not known.
const char* SoundSongStrippedName(void) {
    return ".song";
}
#endif

/** SoundSongContinueCh for song channel 0. */
// Original name and size (0x60) from the JP demo map (see SoundSongPlay).
void SoundSongContinue(void) {
    SoundSongContinueCh(0);
}

/** Resumes a paused song on chan at its volume. */
void SoundSongContinueCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqContinue(song->seqId);
        sndSeqVolume(song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
    }
}

/** SoundSongStopCh for song channel 0. */
// Original name and size (0x50) from the JP demo map (see SoundSongPlay).
void SoundSongStop(void) {
    SoundSongStopCh(0);
}

/** Stops the song on chan at once. */
void SoundSongStopCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqStop(song->seqId);
        song->seqId = (SND_SEQID)-1;
    }
}

/** SoundSongFadeoutCh for song channel 0. */
// Original name and size (0x64) from the JP demo map (see SoundSongPlay).
void SoundSongFadeout(void) {
    SoundSongFadeoutCh(0);
}

/** Fades the song on chan out over the fade-out time and releases the channel. */
void SoundSongFadeoutCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqVolume(0, sound.fadeOut, song->seqId, SND_SEQVOL_STOP);
        song->seqId = (SND_SEQID)-1;
    }
}

/** SoundSongFademuteCh for song channel 0. */
// Original name and size (0x5c) from the JP demo map (see SoundSongPlay).
void SoundSongFademute(void) {
    SoundSongFademuteCh(0);
}

/** Fades the song on chan out over the fade-out time and pauses it there (SoundSongContinueCh resumes it). */
// Original name and size (0x68) from the JP demo map (UNUSED there: nothing calls it); SoundSongFadeoutCh with
// MusyX's SND_SEQVOL_PAUSE and the channel kept (SoundSSFademuteCh is the stream form).
void SoundSongFademuteCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqVolume(0, sound.fadeOut, song->seqId, SND_SEQVOL_PAUSE);
    }
}

/** SoundSongFadeinCh for song channel 0. */
// Original name and size (0x60) from the JP demo map (see SoundSongPlay).
void SoundSongFadein(void) {
    SoundSongFadeinCh(0);
}

/** Fades the song on chan in to its volume over the fade-in time. */
void SoundSongFadeinCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqVolume(song->vol, sound.fadeIn, song->seqId, SND_SEQVOL_CONTINUE);
    }
}

/** SoundSongSetVolCh for song channel 0. */
// Original name and size (0x7c) from the JP demo map (see SoundSongPlay).
void SoundSongSetVol(u8 vol) {
    SoundSongSetVolCh(0, vol);
}

/** Sets the volume (0-127) of the song on chan. */
void SoundSongSetVolCh(s32 chan, u8 vol) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        if (vol > 127) {
            vol = 127;
        }
        song->vol = vol;
        sndSeqVolume(song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
    }
}

/** Returns the volume of the song on chan, 0 when none plays. */
u8 SoundSongGetVolCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId) == 0) {
        return 0;
    }
    return (u8)song->vol;
}

// The track-mute and MIDI-controller functions below: original names and sizes from the JP demo map (UNUSED there:
// nothing calls them; the map lists them between SoundSongGetVolCh and SoundSongMain). song->mask is the
// sndSeqMute track mask SoundSongPlayCh starts at 0xFFFFFFFF; MusyX plays a track while its bit is set (seq.c), so
// muting a track clears its bit. The track parameter is u8 (MusyX's track id type); s8, or s32 with `track & 31`,
// give the same sizes.

/** SoundSongMuteTrackOnCh for song channel 0. */
// Size 0x74.
void SoundSongMuteTrackOn(u8 track) {
    SoundSongMuteTrackOnCh(0, track);
}

/** Mutes one track of the song on chan. */
// Size 0x7c.
void SoundSongMuteTrackOnCh(s32 chan, u8 track) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        song->mask &= ~(1 << track);
        sndSeqMute(song->seqId, song->mask, 0);
    }
}

/** SoundSongMuteTrackOffCh for song channel 0. */
// Size 0x74.
void SoundSongMuteTrackOff(u8 track) {
    SoundSongMuteTrackOffCh(0, track);
}

/** Unmutes one track of the song on chan. */
// Size 0x7c.
void SoundSongMuteTrackOffCh(s32 chan, u8 track) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        song->mask |= (1 << track);
        sndSeqMute(song->seqId, song->mask, 0);
    }
}

/** SoundSongSetMuteTrackCh for song channel 0. */
// Size 0x60.
void SoundSongSetMuteTrack(u32 mask) {
    SoundSongSetMuteTrackCh(0, mask);
}

/** Sets the whole track mask (bit set: track plays) of the song on chan. */
// Size 0x68.
void SoundSongSetMuteTrackCh(s32 chan, u32 mask) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        song->mask = mask;
        sndSeqMute(song->seqId, song->mask, 0);
    }
}

/** Returns whether a song is playing on chan. */
u32 SoundSongCheck(s32 chan) {
    return sndSeqGetValid(sound.songs[chan].seqId) != 0;
}

/** SoundSongGetMIDICtrlCh for song channel 0. */
// Original name and size (0x70) from the JP demo map, like the MIDI-controller functions below.
u8 SoundSongGetMIDICtrl(u8 channel, u8 ctrl) {
    return SoundSongGetMIDICtrlCh(0, channel, ctrl);
}

/** Returns MIDI controller ctrl of MIDI channel channel in the song on chan, 0 when none plays. */
// Size 0x7c.
u8 SoundSongGetMIDICtrlCh(s32 chan, u8 channel, u8 ctrl) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        return sndSeqGetMidiCtrl(song->seqId, channel, ctrl);
    }
    return 0;
}

/** SoundSongSetMIDICtrlCh for song channel 0. */
// Size 0x80: it passes the result on (a void form is 0x78).
BOOL SoundSongSetMIDICtrl(u8 channel, u8 ctrl, u8 value) {
    return SoundSongSetMIDICtrlCh(0, channel, ctrl, value);
}

/** Sets MIDI controller ctrl of MIDI channel channel in the song on chan; returns sndSeqSetMidiCtrl's result. */
// Size 0x6c: it returns sndSeqSetMidiCtrl's result, FALSE when no song plays (a void form is 0x64). BOOL is a
// choice: the bytes do not fix the return type's name.
BOOL SoundSongSetMIDICtrlCh(s32 chan, u8 channel, u8 ctrl, u8 value) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        return sndSeqSetMidiCtrl(song->seqId, channel, ctrl, value);
    }
    return FALSE;
}

/** Per-frame song update: notices songs that ended on their own (marks their group and frees the channel). */
// Original name and size (0x9c) from the JP demo map (UNUSED there: its one call, in SoundMain, is inlined).
void SoundSongMain(void) {
    SoundSong* song;
    int i;

    for (i = 0, song = sound.songs; i < SOUND_SONG_MAX; i++, song++) {
        if (song->group && song->seqId != -1 && sndSeqGetValid(song->seqId) != TRUE) {
            song->group->flags |= SOUND_GROUP_SONG_ENDED_UNUSED;
            sndSeqStop(song->seqId);
            song->seqId = -1;
        }
    }
}

/** Stops the sound effect on effect channel chan and frees the channel (and its 3D emitter). */
inline void SoundEfxMuteCh(u32 chan) {
    SoundEffect* sfx = &sound.effects[chan];
    if (sfx->flags) {
        sndFXKeyOff(sfx->voiceId);
        if (sfx->flags & SOUND_EFX_FLAG_EMITTER_UNUSED) {
            sndRemoveEmitter(&sfx->emitter);
        }
        sfx->flags = 0;
    }
}

/**
 * Plays MusyX effect fid on a free effect channel (or takes one from an effect of lower or equal priority) with volume
 * and pan; returns the channel, or -1 when none is available.
 */
s32 SoundEfxPlayEx(SND_FXID fid, u8 priority, u8 vol, u8 pan) {
    SoundEffect* sfx;
    s32 chan;

    sfx = sound.effects;
    for (chan = 0; chan < SOUND_EFFECT_MAX; chan++, sfx++) {
        if (!sfx->flags) {
            break;
        }
    }
    if (chan >= 40u) {
        sfx = sound.effects;
        for (chan = 0; chan < SOUND_EFFECT_MAX; chan++, sfx++) {
            if (sfx->priority <= priority) {
                SoundEfxMuteCh(chan);
                break;
            }
        }
        if (chan >= 40u) {
            return -1;
        }
    }
    sfx->flags = 0;
    sndDisableIRQ();
    sfx->voiceId = sndFXStart(fid, 127, pan);
    sndFXVolume(sfx->voiceId, vol);
    sndEnableIRQ();
    if (sfx->voiceId == -1) {
        return -1;
    }
    sfx->flags = SOUND_EFX_FLAG_PLAY;
    sfx->priority = priority;
    sfx->vol = vol;
    sfx->pan = pan;
    sfx->span = 0;
    sfx->fid = fid;
    if (sound.coverState) {
        sndFXVolume(sfx->voiceId, 0);
    }
    if (!SoundEfxCheck(chan)) {
        chan = -1;
    }
    return chan;
}

/** Sets the listener position and facing (degrees) that SoundEfxMain hands to MusyX each frame. */
// Original name and size (0x28) from the JP demo map (UNUSED there: nothing calls it; listed between the stripped
// SoundEfxPlayEmitter and SoundEfxSetPosEmitter). pmario_sound's psndSetPosDirListener form on the sound.pos and
// sound.listenerDir fields SoundEfxMain reads (nothing else writes them). The SND_FVECTOR* parameter type is a
// choice (a Vec* with field copies is the same size).
void SoundEfxSetPosDirListener(SND_FVECTOR* pos, f32 dir) {
    sound.pos = *pos;
    sound.listenerDir = dir;
}

/** Sets the 3D position of the emitter effect on chan. */
// Original name and size (0x34) from the JP demo map (UNUSED there: nothing calls it; listed right before
// SoundEfxStop). The listener setter's form for the SoundEffect.pos field (a Vec* parameter is the same size).
void SoundEfxSetPosEmitter(s32 chan, SND_FVECTOR* pos) {
    SoundEffect* effect = &sound.effects[chan];

    effect->pos = *pos;
}

/** Stops the effect on chan and frees the channel. */
void SoundEfxStop(s32 chan) {
    SoundEffect* sfx;

    sfx = &sound.effects[chan];
    if (sfx->flags) {
        sndFXKeyOff(sfx->voiceId);
        if (sfx->flags & SOUND_EFX_FLAG_EMITTER_UNUSED) {
            sndRemoveEmitter(&sfx->emitter);
        }
        sfx->flags = 0;
    }
}

/**
 * Stops the effects on all effect channels and frees the channels (unused; the JP demo map lists it stripped,
 * 0x7c bytes, which this loop compiles to).
 */
static void SoundEfxStopAll(void) {
    s32 chan;

    for (chan = 0; chan < SOUND_EFFECT_MAX; chan++) {
        SoundEfxMuteCh(chan);
    }
}

/** Returns the MusyX voice of the effect on chan (through its emitter for emitter effects). */
// Original name and size (0x54) from the JP demo map (UNUSED there: every call is inlined).
SND_VOICEID SoundEfxGetVoiceID(s32 chan) {
    SoundEffect* sfx = &sound.effects[chan];

    if (sfx->flags & SOUND_EFX_FLAG_EMITTER_UNUSED) {
        return sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
    }
    return sndFXCheck(sfx->voiceId);
}

/** Returns the MusyX effect id playing on chan. */
// Original name and size (0x1c) from the JP demo map (UNUSED there: nothing calls it); returns the id
// SoundEfxPlayEx stores, like SoundEfxGetVolume.
SND_FXID SoundEfxGetFXID(s32 chan) {
    SoundEffect* sfx = &sound.effects[chan];

    return sfx->fid;
}

/** Bends the pitch of the effect on chan (0 = unchanged; 0x2000 is MusyX's centre). */
void SoundEfxSetPitch(s32 chan, s16 pitch) {
    SND_VOICEID vid;

    vid = SoundEfxGetVoiceID(chan);
    if (vid != -1) {
        sndFXPitchBend(vid, (u16)(pitch + 0x2000));
    }
}

/** Sets the MusyX pitch bend value of the effect on chan as it is (0x2000 = unchanged). */
// Original name and size (0x78) from the JP demo map (UNUSED there: nothing calls it); SoundEfxSetPitch without
// the 0x2000 centre.
void SoundEfxSetPitchDirect(s32 chan, u16 pitch) {
    SND_VOICEID vid;

    vid = SoundEfxGetVoiceID(chan);
    if (vid != -1) {
        sndFXPitchBend(vid, pitch);
    }
}

/** Sets the volume of the effect on chan (kept silent while the disc cover is open). */
void SoundEfxSetVolume(s32 chan, u8 vol) {
    SoundEffect* sfx;
    SND_VOICEID vid;

    sfx = &sound.effects[chan];
    vid = SoundEfxGetVoiceID(chan);
    if (vid != -1) {
        sfx->vol = vol;
        sndFXVolume(vid, vol);
        if (sound.coverState) {
            sndFXVolume(vid, 0);
        }
    }
}

/** Returns the volume last set for the effect on chan. */
u8 SoundEfxGetVolume(s32 chan) {
    SoundEffect* sfx = &sound.effects[chan];

    return sfx->vol;
}

/** Sets the reverb send of the effect on chan. */
void SoundEfxSetAux1(s32 chan, u8 reverb) {
    SND_VOICEID vid;

    vid = SoundEfxGetVoiceID(chan);
    if (vid != -1) {
        sndFXReverb(vid, reverb);
    }
}

/** Sets the chorus send of the effect on chan. */
// Original name and size (0x78) from the JP demo map (UNUSED there: nothing calls it); SoundEfxSetAux1 for the
// second of the two MusyX effect sends (SoundInit puts the reverb on aux A and the chorus on aux B).
void SoundEfxSetAux2(s32 chan, u8 chorus) {
    SND_VOICEID vid;

    vid = SoundEfxGetVoiceID(chan);
    if (vid != -1) {
        sndFXChorus(vid, chorus);
    }
}

/** Sets the left-right pan of the effect on chan (0-127, 64 centre). */
void SoundEfxSetPan(s32 chan, u8 pan) {
    SoundEffect* sfx;
    SND_VOICEID vid;

    sfx = &sound.effects[chan];
    sfx->pan = pan;
    vid = SoundEfxGetVoiceID(chan);
    if (vid != -1) {
        sndFXPanning(vid, pan);
    }
}

/**
 * Sets the front-back surround pan of the effect on chan,
 * lowering the pitch slightly (Doppler) the further it is from centre.
 */
void SoundEfxSetSrndPan(s32 chan, u8 span) {
    SoundEffect* sfx;
    SND_VOICEID vid;
    s32 doppler;

    sfx = &sound.effects[chan];
    sfx->span = span;
    vid = SoundEfxGetVoiceID(chan);
    if (vid != -1) {
        sndFXSurroundPanning(vid, span);
        if (span == 0) {
            doppler = 0x2000;
        } else if (span < 64) {
            doppler = 0x2000 - 3 * (64 - span);
        } else {
            doppler = 0x2000 - 3 * (span - 64);
        }
        sndFXDoppler(vid, doppler);
    }
}

/** Applies a low-pass filter at freq to the effect on chan; 0 removes it. */
void SoundEfxSetLPF(s32 chan, s16 freq) {
    u32 voiceId;

    voiceId = SoundEfxGetVoiceID(chan);
    if (voiceId != -1) {
        if (!freq) {
            sndFXSetFilter(voiceId, SND_FILTER_NONE, 0);
        } else if (freq >= 16000) {
            sndFXSetFilter(voiceId, SND_FILTER_NONE, 0);
        } else {
            sndFXSetFilter(voiceId, SND_FILTER_LOWPASS, freq);
        }
    }
}

/**
 * Returns whether the effect on chan is still sounding (3D emitters count as sounding while they exist);
 * it does not free the channel.
 */
u32 SoundEfxCheck(s32 chan) {
    SoundEffect* sfx;

    sfx = &sound.effects[chan];
    if (!sfx->flags) {
        return FALSE;
    }
    if (!(sfx->flags & SOUND_EFX_FLAG_EMITTER_UNUSED)) {
        SND_VOICEID vid = SoundEfxGetVoiceID(chan);

        if (sndFXCheck(vid) == -1) {
            return FALSE;
        }
    } else if (!sndCheckEmitter(&sfx->emitter)) {
        return FALSE;
    }
    return TRUE;
}

/** Frees the effect channel chan if its voice has ended. */
inline void SoundEfxCheckCh(u32 chan) {
    SND_VOICEID vid;

    vid = SoundEfxGetVoiceID(chan);
    if (sndFXCheck(vid) == -1) {
        SoundEfxMuteCh(chan);
    }
}

/**
 * Per-frame effect update: moves the MusyX listener to sound.pos
 * and heading (velocity from the last frame's move), updates every 3D emitter's position and volume,
 * and frees channels whose sound has ended.
 */
static void SoundEfxMain(void) {
    SND_FVECTOR* dir;
    SoundEffect* effect;
    int i;
    f32 headingZ;
    f32 headingX;
    f32 r = 1.0f;
    f32 angle;
    f32 x, y, z;

    headingZ = headingX = 0.0f;
    x = sound.pos.x - sound.prevPos.x;
    y = sound.pos.y - sound.prevPos.y;
    z = sound.pos.z - sound.prevPos.z;
    angle = (TWO_PI * sound.listenerDir / 360.0f);
    sound.prevPos.x = sound.pos.x;
    sound.prevPos.y = sound.pos.y;
    sound.prevPos.z = sound.pos.z;
    dir = &sound.dir;
    dir->x = x;
    dir->y = y;
    dir->z = z;
    headingX += r * (f32)sin(angle);
    headingZ -= r * (f32)cos(angle);
    sound.heading.x = headingX;
    sound.heading.y = 0.0f;
    sound.heading.z = headingZ;
    sound.up.x = 0.0f;
    sound.up.y = 1.0f;
    sound.up.z = 0.0f;
    sndUpdateListener(&sound.listener, &sound.pos, dir, &sound.heading, &sound.up, sound.listenerVol, NULL);

    effect = sound.effects;
    for (i = 0; i < SOUND_EFFECT_MAX; i++, effect++) {
        if (effect->flags) {
            if (effect->flags & SOUND_EFX_FLAG_EMITTER_UNUSED) {
                effect->dir.x = effect->pos.x - effect->prevPos.x;
                effect->dir.y = effect->pos.y - effect->prevPos.y;
                effect->dir.z = effect->pos.z - effect->prevPos.z;
                effect->prevPos.x = effect->pos.x;
                effect->prevPos.y = effect->pos.y;
                effect->prevPos.z = effect->pos.z;
                sndUpdateEmitter(&effect->emitter, &effect->pos, &effect->dir, effect->vol, NULL);
            }
        }
    }

    for (effect = sound.effects, i = 0; i < SOUND_EFFECT_MAX; i++, effect++) {
        if (effect->flags) {
            if (!(effect->flags & SOUND_EFX_FLAG_EMITTER_UNUSED)) {
                SoundEfxCheckCh(i);
            } else {
                if (!sndCheckEmitter(&effect->emitter)) {
                    SoundEfxMuteCh(i);
                }
                if (effect->checkDelay) {
                    effect->checkDelay--;
                } else {
                    SoundEfxCheckCh(i);
                }
            }
        }
    }
}

#if VERSION <= VERSION_DEMO_JP
// FAKE: stand-in for the dead-stripped DVD stream starters; nothing calls it. It only places their ".adp" (JP demo
// map @3067) in the pool. The map lists _dvdcallback, SoundDVDPlay (0x204) and SoundDVDPlayCh (0x1f8) UNUSED at this
// place (after SoundEfxMain); the extension of the disc's ADP stream files fits them. Bodies not known.
const char* SoundDVDStrippedName(void) {
    return ".adp";
}
#endif

/** SoundDVDStopCh for DVD audio channel 0. */
// Original name and size (0x6c) from the JP demo map (see SoundSongPlay).
void SoundDVDStop(void) {
    SoundDVDStopCh(0);
}

/** Stops the DVD audio stream (the disc's streamed music) on chan and closes its file. */
static void SoundDVDStopCh(u32 chan) {
    SoundDVD* dvd = &sound.dvds[chan];
    if (dvd->flags & (SOUND_DVD_FLAG_PLAY | SOUND_DVD_FLAG_MUTE)) {
        AISetStreamPlayState(0);
        dvd->flags = 0;
        sound.dvdChannel = -1;
        DVDCancelStream(&dvd->info.cb);
        DVDClose(&dvd->info);
    }
}

#if VERSION <= VERSION_DEMO_JP
// FAKE: stand-in for a dead-stripped DVD audio function; nothing calls it. It only places the doubles 3.5 and 4.0
// (JP demo map @3005/@3006, after the stripped .sdata2 float @3004) in the pool. The unit's JP demo @N numbering
// puts their owner here, where the map lists the UNUSED SoundDVDContinue (0x1f8) and SoundDVDContinueCh (0x208)
// between SoundDVDStopCh and SoundDVDSetPan: at this slot the numbers before the doubles (from SoundSSPlayChEx's
// ".stm") and after them (to ".adp") both fall short of the map only by absent stripped functions. Which of the
// two, and their bodies, are not known. Removing it changes the object.
f64 SoundDemoStrippedRate(f64 t) {
    f64 a = t * 3.5;
    f64 b = t * 4.0;
    return a + b;
}
#endif

/** SoundDVDSetPanCh for DVD audio channel 0. */
// Original name and size (0xd8) from the JP demo map (see SoundSongPlay).
void SoundDVDSetPan(u8 pan) {
    SoundDVDSetPanCh(0, pan);
}

/** Applies pan and the current volume of DVD audio channel chan to the hardware stream volume. */
static void SoundDVDSetPanCh(u32 chan, u8 pan) {
    f32 left, right;
    SoundDVD* dvd;

    dvd = &sound.dvds[chan];
    dvd->pan = pan;
    left = dvd->vol;
    right = left;
    if (pan < 64) {
        right = (((f32)pan * dvd->vol) / 63.0f);
    }

    // sign-bit test for pan >= 64 (the subtract and the shift are in the retail code)
    if (!((u32)(pan - 64) >> 31)) {
        left = (((f32)(127 - pan) * dvd->vol) / 63.0f);
    }
    AISetStreamVolLeft(left);
    AISetStreamVolRight(right);
}

// The DVD audio volume and fade setters below: original names and sizes from the JP demo map (UNUSED there:
// nothing calls them; the map lists them in this order between SoundDVDSetPanCh and SoundDVDMain). With
// SoundDVDSetVolCh they are the stream forms (SoundSSSetVolCh, SoundSSFadeoutCh, SoundSSFademuteCh) on the SoundDVD
// fields SoundDVDMain fades, without the stream forms' in-use test, like SoundDVDSetPanCh (with it each is 0xc
// larger). The fades use SoundSSFadeoutCh's frame time in each version (only the JP demo sizes are known).

/** SoundDVDSetVolCh for DVD audio channel 0. */
// Size 0xf8.
void SoundDVDSetVol(u8 vol) {
    SoundDVDSetVolCh(0, vol);
}

/** Sets the volume of DVD audio channel chan (also the fade-in target) and applies it with the current pan. */
// Original name and size (0x104) from the JP demo map (UNUSED there: nothing calls it), after SoundDVDSetPanCh.
// Body: SoundSSSetVolCh's form for the DVD channel (target and current volume set together, then the pan update);
// the size fits exactly. The order of the two stores in the chained assignment is not fixed by the size.
void SoundDVDSetVolCh(u32 chan, u8 vol) {
    SoundDVD* dvd = &sound.dvds[chan];
    dvd->volTarget = dvd->vol = vol;
    SoundDVDSetPanCh(chan, dvd->pan);
}

/** SoundDVDFadeoutCh for DVD audio channel 0. */
// Size 0x68.
void SoundDVDFadeout(void) {
    SoundDVDFadeoutCh(0);
}

/** Fades DVD audio channel chan out over the fade-out time (SoundDVDMain stops it at silence). */
// Size 0x70.
static void SoundDVDFadeoutCh(u32 chan) {
    SoundDVD* dvd = &sound.dvds[chan];
    f32 frames;

#if VERSION >= VERSION_US
    frames = sound.fadeOut / (1000.0f / gp->framerate);
#else
    frames = sound.fadeOut / 16.666666f;
#endif
    if (frames < 1.0f) {
        frames = 1.0f;
    }
    dvd->volStep = dvd->vol / frames;
    dvd->flags |= SOUND_DVD_FLAG_FADEOUT;
}

/** SoundDVDFademuteCh for DVD audio channel 0. */
// Size 0x68.
void SoundDVDFademute(void) {
    SoundDVDFademuteCh(0);
}

/** Fades DVD audio channel chan out over the fade-out time and pauses it there (SoundDVDMain). */
// Size 0x70.
static void SoundDVDFademuteCh(u32 chan) {
    SoundDVD* dvd = &sound.dvds[chan];
    f32 frames;

#if VERSION >= VERSION_US
    frames = sound.fadeOut / (1000.0f / gp->framerate);
#else
    frames = sound.fadeOut / 16.666666f;
#endif
    if (frames < 1.0f) {
        frames = 1.0f;
    }
    dvd->volStep = dvd->vol / frames;
    dvd->flags |= SOUND_DVD_FLAG_FADEMUTE;
}

/** SoundDVDFadeinCh for DVD audio channel 0. */
// Size 0x70.
void SoundDVDFadein(void) {
    SoundDVDFadeinCh(0);
}

/** Fades DVD audio channel chan in from silence to its volume over the fade-in time. */
// Size 0x78: the fade-out form plus one store; starting from silence (vol = 0) is the store that fits
// SoundDVDMain's fade-in, where it is placed is a choice.
static void SoundDVDFadeinCh(u32 chan) {
    SoundDVD* dvd = &sound.dvds[chan];
    f32 frames;

#if VERSION >= VERSION_US
    frames = sound.fadeIn / (1000.0f / gp->framerate);
#else
    frames = sound.fadeIn / 16.666666f;
#endif
    if (frames < 1.0f) {
        frames = 1.0f;
    }
    dvd->vol = 0.0f;
    dvd->volStep = dvd->volTarget / frames;
    dvd->flags |= SOUND_DVD_FLAG_FADEIN;
}

/**
 * Per-frame DVD audio update: runs fade-outs (stopping or pausing at silence, remembering the sample position)
 * and fade-ins.
 */
static void SoundDVDMain(void) {
    s32 i;

    for (i = 0; i < SOUND_DVD_MAX; i++) {
        SoundDVD* dvd = &sound.dvds[i];
        if (!(dvd->flags & SOUND_DVD_FLAG_MUTE)) {
            if (dvd->flags & SOUND_DVD_FLAG_FADEMUTE) {
                if (dvd->vol == 0.0f) {
                    dvd->samples = AIGetStreamSampleCount();
                    SoundDVDStopCh(i);
                    dvd->flags |= SOUND_DVD_FLAG_MUTE;
                    dvd->flags &= ~SOUND_DVD_FLAG_FADEMUTE;
                    continue;
                }
                dvd->vol -= dvd->volStep;
                if (dvd->vol <= 0.0f) {
                    dvd->vol = 0.0f;
                }
                SoundDVDSetPanCh(i, dvd->pan);
            }
            if (dvd->flags & SOUND_DVD_FLAG_FADEOUT) {
                if (dvd->vol == 0.0f) {
                    dvd->flags &= ~SOUND_DVD_FLAG_FADEOUT;
                    SoundDVDStopCh(i);
                    continue;
                }
                dvd->vol -= dvd->volStep;
                if (dvd->vol <= 0.0f) {
                    dvd->vol = 0.0f;
                }
                SoundDVDSetPanCh(i, dvd->pan);
            }
            if (dvd->flags & SOUND_DVD_FLAG_FADEIN) {
                dvd->vol += dvd->volStep;
                if (dvd->vol >= dvd->volTarget) {
                    dvd->vol = dvd->volTarget;
                    dvd->volStep = 0.0f;
                    dvd->flags &= ~SOUND_DVD_FLAG_FADEIN;
                }
                SoundDVDSetPanCh(i, dvd->pan);
            }
        }
    }
}

static void _ssDVDReadAsync_activeChk(s32 result, DVDFileInfo* fileInfo);
static void cache_flush(u32 pointerToARQRequest);
static void _ssDVDReadAsync_cache_next(s32 result, DVDFileInfo* fileInfo);
static void SoundSSPlayChEx_main(s32 result, DVDFileInfo* fileInfo);

/** Rounds x up to a multiple of 32 bytes (DVD and DMA alignment). */
// FAKE: invented one-use inline helper (no map name); writing it out changes the code (FAKES.md)
static inline u32 _ssAlign32(u32 x) {
    return (x + 0x1F) & ~0x1F;
}

/**
 * Returns how many bytes the next stream read loads:
 * a full 32 KB buffer, or what is left before the end of the stream data.
 */
static inline u32 _ssCalcLen(SoundStreamEntry* entry) {
    u32 len;
    int i;
    len = SS_LOAD_BUFFER_SIZE;
    if (entry->offset + SS_LOAD_BUFFER_SIZE > entry->size) {
        len = entry->size - entry->offset;
    } else {
#if VERSION <= VERSION_DEMO_JP
        if (entry->offset + 2 * SS_LOAD_BUFFER_SIZE > entry->size) {
            len -= (SS_LOAD_BUFFER_SIZE - (entry->size - (entry->offset + SS_LOAD_BUFFER_SIZE))) / 2;
            len += 0x20 - (len % 0x20);
#else
        for (i = 2; i < 6; i++) {
            if (entry->offset + i * SS_LOAD_BUFFER_SIZE > entry->size) {
                len -= (SS_LOAD_BUFFER_SIZE - (entry->size - (entry->offset + (i - 1) * SS_LOAD_BUFFER_SIZE))) / i;
                len += 0x20 - (len % 0x20);
                break;
            }
#endif
        }
    }
    return len;
}

// _ssDVDReadAsync_sub, _ssDVDReadAsync_l and _ssDVDReadAsync_r: original names, sizes (0x174, 0x16c, 0x180) and order
// from the JP demo map (all UNUSED there: every call is inlined), between cache_flush and _ssDVDReadAsync_cache_aram.
// _l and _r call _sub for the first and second channel (as the READ_L / READ_R flags name them); this form compiles to
// exactly the three map sizes and leaves every caller unchanged (plain functions: every call is auto-inlined and the
// copies are emitted, which also fits the unit's JP demo @N numbering). Not fixed by the bytes: the order of _sub's
// parameters (all four orders tried build the same) and the order of its locals.

/**
 * Starts the asynchronous disc read of the next chunk of one stream channel (entry), whose data starts extra bytes
 * into the stream's data.
 */
void _ssDVDReadAsync_sub(SoundStream* stream, SoundStreamEntry* entry, u32 extra) {
    u32 len;
    u32 aligned;
    u32 addr;

#if VERSION <= VERSION_DEMO_JP
    aligned = len = _ssCalcLen(entry);
    if (len < SS_LOAD_BUFFER_SIZE) {
        aligned = SS_LOAD_BUFFER_SIZE;
    }
    aligned = (aligned + 0x1F) & ~0x1F;
#else
    aligned = _ssAlign32(len = _ssCalcLen(entry));
#endif
    if (!(stream->flags & SOUND_STREAM_FLAG_ARAM)) {
        DVDMgrReadAsync(entry->dvd, entry->loadBuffer[1 - entry->curBuf], aligned,
                        stream->dataOffset + entry->offset + extra, _ssDVDReadAsync_activeChk);
    } else {
        addr = stream->dataOffset + entry->offset + extra + sound.aramAddr[stream->slot];
        DCInvalidateRange(entry->loadBuffer[1 - entry->curBuf], SS_LOAD_BUFFER_SIZE);
        ARQPostRequest(&entry->request, (u32)entry, 1, 0, addr, (u32)entry->loadBuffer[1 - entry->curBuf], aligned,
                       cache_flush);
    }
    entry->requestLength = len;
}

/** Starts the asynchronous disc read of the next chunk of the stream's first (left) channel into its load buffer. */
void _ssDVDReadAsync_l(SoundStream* stream) {
    SoundStreamEntry* entry = &stream->entries[0];

    _ssDVDReadAsync_sub(stream, entry, 0);
}

/** The same for the second (right) channel, whose data follows the first's in the stream. */
void _ssDVDReadAsync_r(SoundStream* stream) {
    SoundStreamEntry* entry = &stream->entries[1];

    _ssDVDReadAsync_sub(stream, entry, entry->size + (0x20 - (entry->size % 0x20)));
}

/** Rewinds both channels of a stream to the start of their data. */
inline void _ssResetEntries(SoundStream* stream) {
    stream->entries[0].curBuf = 0;
    stream->entries[0].srcPos = 0;
    stream->entries[0].offset = 0;
    stream->entries[0].pos = 0;
    stream->entries[0].loaded = 0;
    stream->entries[0].ready = 0;
    stream->entries[0].scale = 0;
    stream->entries[0].nibble = 0;
    stream->entries[0].runCount = 0;
    stream->entries[1].curBuf = 0;
    stream->entries[1].srcPos = 0;
    stream->entries[1].offset = 0;
    stream->entries[1].pos = 0;
    stream->entries[1].loaded = 0;
    stream->entries[1].ready = 0;
    stream->entries[1].scale = 0;
    stream->entries[1].nibble = 0;
    stream->entries[1].runCount = 0;
}

/**
 * Marks the stream entry whose DVD read finished as loaded,
 * then starts the first stream whose entries are all ready.
 * The early demos mask interrupts only around the updates, JP and later around the whole search.
 */
static void _ssDVDReadAsync_activeChk(s32 result, DVDFileInfo* fileInfo) {
#if VERSION <= VERSION_DEMO_JP
#else
    int i;
#endif
    SoundStream* stream;
    SoundStreamEntry* entry;
#if VERSION <= VERSION_DEMO_JP
    int i;
#endif
    int ch;
#if VERSION <= VERSION_DEMO_JP
    for (i = 0; i < SOUND_STREAM_MAX; i++) {
        stream = &sound.streams[i];
#else
    sndDisableIRQ();
    stream = sound.streams;
    for (i = 0; i < SOUND_STREAM_MAX; i++, stream++) {
#endif
        if (stream->flags) {
            entry = NULL;
            ch = 0;
            if (&stream->entries[0].dvd->info == fileInfo) {
                entry = &stream->entries[0];
                ch = 0;
            }
            if (&stream->entries[1].dvd->info == fileInfo) {
                entry = &stream->entries[1];
                ch = 1;
            }
            if (entry) {
#if VERSION <= VERSION_DEMO_JP
                if (result >= 0) {
                    sndDisableIRQ();
#else
                if (result < 0) {
                    goto end;
                }
#endif
                entry->offset += entry->requestLength;
                entry->loadedLen[1 - entry->curBuf] = entry->requestLength;
                entry->loaded = 1;
                if (entry->offset >= entry->size) {
                    entry->offset = entry->loopStart;
                    if (stream->flags & SOUND_STREAM_FLAG_LOOP) {
                        if (ch == 0) {
                            stream->flags &= ~SOUND_STREAM_FLAG_READ_L;
                        } else {
                            stream->flags &= ~SOUND_STREAM_FLAG_READ_R;
                        }
                    } else {
                        stream->flags |= SOUND_STREAM_FLAG_READ_END;
                    }
                } else if (ch == 0) {
                    stream->flags &= ~SOUND_STREAM_FLAG_READ_L;
                } else {
                    stream->flags &= ~SOUND_STREAM_FLAG_READ_R;
                }
#if VERSION <= VERSION_DEMO_JP
                sndEnableIRQ();
#endif
                if (stream->count == 2) {
                    if (sound.coverState == SOUND_COVER_RELOADING && stream->entries[0].loaded == 1 && stream->entries[1].loaded == 1) {
                        sound.coverState = SOUND_COVER_CLOSED;
                    }
                } else if (sound.coverState == SOUND_COVER_RELOADING && stream->entries[0].loaded == 1) {
                    sound.coverState = SOUND_COVER_CLOSED;
                }
#if VERSION <= VERSION_DEMO_JP
                continue;
            }
            return;
#else
                break;
#endif
            }
        }
    }
    for (i = 0; i < SOUND_STREAM_MAX; i++) {
#if VERSION <= VERSION_DEMO_JP
        stream = &sound.streams[i];
#else
        SoundStream* stream = &sound.streams[i];
#endif
        if (stream->flags && !(stream->flags & SOUND_STREAM_FLAG_STARTED)) {
            if (&stream->entries[0].dvd->info == fileInfo) {
                stream->entries[0].ready = 1;
            }
            if (&stream->entries[1].dvd->info == fileInfo) {
                stream->entries[1].ready = 1;
            }
            if (stream->count == 2) {
                if (!stream->entries[0].ready || !stream->entries[1].ready) {
                    continue;
                }
            } else if (!stream->entries[0].ready) {
                continue;
            }
            stream->entries[0].curBuf = 0;
            stream->entries[1].curBuf = 0;
            stream->entries[0].loaded = 0;
            stream->entries[1].loaded = 0;
#if VERSION <= VERSION_DEMO_JP
            if (stream->flags & SOUND_STREAM_FLAG_ADPCM) {
                stream->entries[0].srcPos = 0;
                stream->entries[0].pos = 0;
                stream->entries[1].srcPos = 0;
                stream->entries[1].pos = 0;
            }
            sndDisableIRQ();
#endif
            sndStreamActivate(stream->entries[0].streamId);
            if (stream->count == 2) {
                sndStreamActivate(stream->entries[1].streamId);
            }
            SoundSSSetPanCh(i, stream->pan);
#if VERSION <= VERSION_DEMO_JP
            sndEnableIRQ();
#endif
            stream->flags |= SOUND_STREAM_FLAG_STARTED;
            break;
        }
    }
#if VERSION >= VERSION_JP
    end:
    sndEnableIRQ();
#endif
}

/**
 * ARQ callback after a chunk of an ARAM-cached stream was copied from ARAM into its load buffer:
 * finishes it like a disc read (_ssDVDReadAsync_activeChk).
 */
static void cache_flush(u32 pointerToARQRequest) {
    _ssDVDReadAsync_activeChk(0, &((SoundStreamEntry*)((ARQRequest*)pointerToARQRequest)->owner)->dvd->info);
}

/**
 * ARQ callback for ARAM-cached streams: when the whole stream is in ARAM,
 * rewinds both channels and starts playing from the beginning; otherwise reads the next chunk.
 */
static void _ssDVDReadAsync_cache_aram(u32 pointerToARQRequest) {
    u32 len;
    SoundStream* stream;

    stream = &sound.streams[(u8)((ARQRequest*)pointerToARQRequest)->owner];
    if (stream->entries[0].pos >= DVDMgrGetLength(stream->entries[0].dvd)) {
        stream->entries[0].offset = 0;
        stream->entries[0].pos = 0;
        stream->entries[1].offset = 0;
        stream->entries[1].pos = 0;
        stream->entries[0].curBuf = 1;
        _ssDVDReadAsync_l(stream);
        if (stream->count == 2) {
            stream->entries[1].curBuf = 1;
            _ssDVDReadAsync_r(stream);
        }
    } else {
        len = DVDMgrGetLength(stream->entries[0].dvd);
        if (stream->entries[0].offset + SS_LOAD_BUFFER_SIZE >= len) {
            len = len - stream->entries[0].offset;
        } else {
            len = SS_LOAD_BUFFER_SIZE;
        }
        DVDMgrReadAsync(stream->entries[0].dvd, stream->entries[0].loadBuffer[0], (len + 0x1F) & ~0x1F,
                        stream->entries[0].offset, _ssDVDReadAsync_cache_next);
        stream->entries[0].requestLength = len;
    }
}

/**
 * Disc read callback while caching a stream into ARAM:
 * copies the chunk just read into the stream's ARAM slot with an ARQ request and advances the offsets.
 */
static void _ssDVDReadAsync_cache_next(s32 result, DVDFileInfo* fileInfo) {
    static u32 owner_no;
    SoundStream* stream;
    u32 dest;
    int i;

    stream = sound.streams;
    for (i = 0; i < SOUND_STREAM_MAX; stream++, i++) {
        if (stream->flags && &stream->entries[0].dvd->info == fileInfo) {
            if (result >= 0) {
                stream->entries[0].offset += stream->entries[0].requestLength;
                dest = stream->entries[0].pos + sound.aramAddr[stream->slot];
                DCFlushRange(stream->entries[0].loadBuffer[0], SS_LOAD_BUFFER_SIZE);
                ARQPostRequest(&stream->entries[0].request, (owner_no++ << 16) | i, 0, 0,
                               (u32)stream->entries[0].loadBuffer[0], dest, SS_LOAD_BUFFER_SIZE, _ssDVDReadAsync_cache_aram);
                stream->entries[0].pos += stream->entries[0].requestLength;
            }
            break;
        }
    }
}

/** Step sizes of the streamed-music DPCM decoder (ssDecodeDPCM): each 4-bit delta is scaled by one of them. */
static s16 dtbl[16] = {0x0001, 0x0007, 0x0018, 0x003B, 0x0053, 0x006F, 0x00EC, 0x022B,
                0x02E6, 0x0375, 0x041C, 0x08AD, 0x0CD5, 0x1150, 0x1B32, 0x2454};

/**
 * Decodes count samples of the game's 4-bit DPCM stream format from src into out, with run-length scale changes,
 * keeping the decoder state in entry; returns the new source pointer.
 */
static u8* ssDecodeDPCM(SoundStream* stream, SoundStreamEntry* entry, s16* out, u32 count, u8* src) {
    s8 delta;
    int b;
    u32 scale;
    s32 sample;

    while (count != 0) {
        b = *src;
        if (entry->runCount != 0) {
            if (entry->nibble & 1) {
                delta = b & 0xF;
            } else {
                delta = (b >> 4) & 0xF;
            }
            scale = entry->scale;
            delta <<= 4;
            entry->nibble++;
            delta >>= 4;
            if (entry->nibble >= entry->runCount) {
                entry->runCount = 0;
                src++;
                entry->srcPos++;
            } else if (!(entry->nibble & 1)) {
                src++;
                entry->srcPos++;
            }
        } else {
            src++;
            entry->srcPos++;

            if ((b & 0xF) == 8) {
                entry->runCount = ((b >> 4) & 0xF) + 4;
                entry->nibble = 0;
                continue;
            }
            scale = (b >> 4) & 0xF;
            entry->scale = scale;
            delta = b & 0xF;
            delta <<= 4;
            delta >>= 4;
        }
        sample = entry->hist + delta * dtbl[scale];
        if (sample > 0x7FFF) {
            sample = 0x7FFF;
        }
        if (sample < -0x8000) {
            sample = -0x8000;
        }
        entry->hist = sample;
        count--;
        *out++ = sample;
        stream->samplePos++;
        if (entry->srcPos >= entry->loadedLen[entry->curBuf]) {
            if (entry->loaded == 0 && !(stream->flags & SOUND_STREAM_FLAG_READ_END)) {
                entry->pos -= entry->srcPos;
                entry->srcPos = 0;
                src = entry->loadBuffer[entry->curBuf];
                continue;
            }
            entry->loaded = 0;
            entry->srcPos = 0;
            entry->curBuf = 1 - entry->curBuf;
            src = entry->loadBuffer[entry->curBuf];
        }
        entry->pos += 2;
        if (entry->pos >= stream->loopEnd) {
            entry->pos = stream->loopStart;
            entry->hist = entry->loopHist;
            if (!(stream->flags & SOUND_STREAM_FLAG_LOOP)) {
                stream->flags |= SOUND_STREAM_FLAG_END;
                break;
            }
        }
    }
    return src;
}

/** Early-demo and JP signature of _sscallback: the first buffer's length arrives as len1. */
#if VERSION < VERSION_US
static u32 _sscallback(void* p_b1, u32 len1, void* p_b2, u32 p_l2, u32 user) {
#else
static u32 _sscallback(void* p_b1, u32 p_l1, void* p_b2, u32 p_l2, u32 user) {
    u32 len1;
#endif
    s16* dst;
    SoundStream* stream;
    SoundStreamEntry* entry;
    unsigned int len2;
    s16* buffer1;
    s16* buffer2;
    s16* src16;
    u32 i;
    u32 n;
    s32 j;
    u8* src8;
    u8* src8b;
#if VERSION <= VERSION_DEMO_JP
    len2 = p_l2;
    buffer1 = p_b1;
    buffer2 = p_b2;
    stream = &sound.streams[(u8)user];
#else
    // FAKE: len2/buffer1/buffer2 are copied from the params into locals of a different type (unsigned int, s16*) so
    // IRO cannot propagate the copies; the params then coalesce into r3/r5/r6 and these statement copies give the
    // target order len1 (param web, after the stream add) < len2 < buffer1 < buffer2 (buffer1 lands in r6).
#if VERSION >= VERSION_US
    // FAKE (US): len1 is also a local copy of its param, copied first, so it leaves r4 before the sound base is formed.
    len1 = p_l1;
    len2 = p_l2;
    stream = &sound.streams[(u8)user];
    if (p_b1 == NULL && p_b2 == NULL && len1 == 0 && len2 == 0) {
        return 0;
    }
    buffer1 = p_b1;
    buffer2 = p_b2;
#else
    len2 = p_l2;
    buffer1 = p_b1;
    buffer2 = p_b2;
    stream = &sound.streams[(u8)user];
#endif
#endif
    stream->samplePos = 0;
    if ((stream->flags & (SOUND_STREAM_FLAG_MUTE | SOUND_STREAM_FLAG_END | SOUND_STREAM_FLAG_PAUSE)) || sound.coverState) {
        if (!(stream->flags & SOUND_STREAM_FLAG_ADPCM)) {
            for (; len1 != 0; len1--) {
                *(buffer1++) = 0;
                stream->samplePos++;
            }
            for (; len2 != 0; len2--) {
                *(buffer2++) = 0;
                stream->samplePos++;
            }
            return stream->samplePos;
        } else {
            j = (s32)(len1 / 14 * 8);
            j /= 2;
            for (; j != 0; j--) {
                *(buffer1++) = 0;
            }
            j = (s32)(len2 / 14 * 8);
            j /= 2;
            for (; j != 0; j--) {
                *(buffer2++) = 0;
            }
            return len1 + len2;
        }
    }
    if (user & 0x100) {
        entry = &stream->entries[1];
    } else {
        entry = &stream->entries[0];
    }
    if (stream->flags & SOUND_STREAM_FLAG_DPCM) {
        ssDecodeDPCM(stream, entry, buffer2, len2,
                     ssDecodeDPCM(stream, entry, buffer1, len1, entry->loadBuffer[entry->curBuf] + entry->srcPos));
    } else if (stream->flags & SOUND_STREAM_FLAG_ADPCM) {
        dst = buffer1; // FAKE: dst (not a u8* dst8) for the byte copies, so dst's split webs number above i/n
        src8 = entry->loadBuffer[entry->curBuf];
        src8 = src8 + entry->srcPos;
        n = ((len1 + 13) / 14) * 8;
        while (n != 0) {
#if VERSION <= VERSION_DEMO_JP
            if (stream->flags & SOUND_STREAM_FLAG_END) {
                *(u8*)dst = 0;
                dst = (s16*)((u8*)dst + 1);
                n--;
                continue;
            }
            *(u8*)dst = *(src8++);
            dst = (s16*)((u8*)dst + 1);
            n--;
            entry->srcPos++;
#else
            *(((u8*)dst)++) = *(src8++);
            n--;
            entry->srcPos++;
#if VERSION >= VERSION_US
            entry->pos++;
#endif
#endif
            if (entry->srcPos >= entry->loadedLen[entry->curBuf]) {
                if (entry->loaded == 0 && !(stream->flags & SOUND_STREAM_FLAG_READ_END)) {
                    entry->pos -= entry->srcPos;
                    entry->srcPos = 0;
                    src8 = entry->loadBuffer[entry->curBuf];
                    continue;
                }
                entry->loaded = 0;
                entry->srcPos = 0;
                entry->curBuf = 1 - entry->curBuf;
                src8 = entry->loadBuffer[entry->curBuf];
            }
#if VERSION < VERSION_US
            entry->pos++;
#endif
            if (entry->pos >= stream->loopEnd) {
                entry->pos = stream->loopStart;
                if (!(stream->flags & SOUND_STREAM_FLAG_LOOP)) {
                    stream->flags |= SOUND_STREAM_FLAG_END;
                    break;
                }
            }
        }

        // FAKE: loop 2 reads through its own copy of src8; the copy takes the pre-RA scheduler's cycle-1 slot so `mr dst,buffer2`
        // issues after mulhwu (magic constant and dst then share r3, as in the target); the copy itself colours to a self-move (FAKES.md)
        src8b = src8;
        dst = buffer2;
        n = ((len2 + 13) / 14) * 8;
        while (n != 0) {
#if VERSION <= VERSION_DEMO_JP
            if (stream->flags & SOUND_STREAM_FLAG_END) {
                *(u8*)dst = 0;
                dst = (s16*)((u8*)dst + 1);
                n--;
                continue;
            }
            *(u8*)dst = *(src8b++);
            dst = (s16*)((u8*)dst + 1);
            n--;
            entry->srcPos++;
#else
            *(((u8*)dst)++) = *(src8b++);
            n--;
            entry->srcPos++;
#if VERSION >= VERSION_US
            entry->pos++;
#endif
#endif
            if (entry->srcPos >= entry->loadedLen[entry->curBuf]) {
                if (entry->loaded == 0 && !(stream->flags & SOUND_STREAM_FLAG_READ_END)) {
                    entry->pos -= entry->srcPos;
                    entry->srcPos = 0;
                    src8b = entry->loadBuffer[entry->curBuf];
                    continue;
                }
                entry->loaded = 0;
                entry->srcPos = 0;
                entry->curBuf = 1 - entry->curBuf;
                src8b = entry->loadBuffer[entry->curBuf];
            }
#if VERSION < VERSION_US
            entry->pos++;
#endif
            if (entry->pos >= stream->loopEnd) {
                entry->pos = stream->loopStart;
                if (!(stream->flags & SOUND_STREAM_FLAG_LOOP)) {
                    stream->flags |= SOUND_STREAM_FLAG_END;
                    break;
                }
            }
        }
        stream->samplePos = len1 + len2;
    } else {
        i = len1;
        dst = buffer1;
        src16 = (s16*)(entry->loadBuffer[entry->curBuf] + (entry->srcPos & (~1)));
        while (i != 0) {
            *(dst++) = *(src16++);
            i--;
            stream->samplePos++;
            entry->srcPos += 2;
            if (entry->srcPos >= entry->loadedLen[entry->curBuf]) {
                if (entry->loaded == 0 && !(stream->flags & SOUND_STREAM_FLAG_READ_END)) {
                    entry->pos -= entry->srcPos;
                    entry->srcPos = 0;
                    src16 = (s16*)entry->loadBuffer[entry->curBuf];
                    continue;
                }
                entry->loaded = 0;
                entry->srcPos = 0;
                entry->curBuf = 1 - entry->curBuf;
                src16 = (s16*)entry->loadBuffer[entry->curBuf];
            }
            entry->pos += 2;
            if (entry->pos >= stream->loopEnd) {
                entry->pos = stream->loopStart;
                if (!(stream->flags & SOUND_STREAM_FLAG_LOOP)) {
                    stream->flags |= SOUND_STREAM_FLAG_END;
                    break;
                }
            }
        }
        i = len2;
        dst = buffer2;
        while (i != 0) {
            *(dst++) = *(src16++);
            i--;
            stream->samplePos++;
            entry->srcPos += 2;
            if (entry->srcPos >= entry->loadedLen[entry->curBuf]) {
                if (entry->loaded == 0 && !(stream->flags & SOUND_STREAM_FLAG_READ_END)) {
                    entry->pos -= entry->srcPos;
                    entry->srcPos = 0;
                    src16 = (s16*)entry->loadBuffer[entry->curBuf];
                    continue;
                }
                entry->loaded = 0;
                entry->srcPos = 0;
                entry->curBuf = 1 - entry->curBuf;
                src16 = (s16*)entry->loadBuffer[entry->curBuf];
            }
            entry->pos += 2;
            if (entry->pos >= stream->loopEnd) {
                entry->pos = stream->loopStart;
                if (!(stream->flags & SOUND_STREAM_FLAG_LOOP)) {
                    stream->flags |= SOUND_STREAM_FLAG_END;
                    break;
                }
            }
        }
    }
    return stream->samplePos;
}

void SoundSSPlayChEx(s32 chan, const char* name, s32 aram);

/** SoundSSPlayCh for stream channel 0. */
// Original name and size (0x2c) from the JP demo map (see SoundSongPlay).
void SoundSSPlay(const char* name) {
    SoundSSPlayCh(0, name);
}

/** Plays stream name on stream channel chan from the disc (see SoundSSPlayChEx). */
void SoundSSPlayCh(s32 chan, const char* name) {
    SoundSSPlayChEx(chan, name, 0);
}

/**
 * Starts stream file name + ".stm" on channel chan, stopping what played there;
 * aram != 0 caches the whole stream in ARAM first.
 * The header is read asynchronously and SoundSSPlayChEx_main finishes the setup.
 */
void SoundSSPlayChEx(s32 chan, const char* name, s32 aram) {
    SoundStream* stream;
    int i;
    const char* path;
    stream = &sound.streams[chan];
    if (stream->flags) {
        SoundSSStopCh(chan);
    }
    if (aram) {
        for (i = 0; i < sound.slotCount; i++) {
            if (!sound.slotUsed[i]) {
                sound.slotUsed[i] = 1;
                stream->slot = i;
                break;
            }
        }
        if (i >= sound.slotCount) {
            return;
        }
    }
    sound.streamChannel = chan;
    _ssResetEntries(stream);
    stream->pan = 0x40;
    stream->span = 0;
    stream->curVol = 0.0f;
    stream->vol = 127.0f;
    stream->flags = SOUND_STREAM_FLAG_PLAY;
    if (aram) {
        stream->flags |= SOUND_STREAM_FLAG_ARAM;
    }
#if VERSION <= VERSION_DEMO_JP
    path = makeFilename(name, ".stm");
    if (!(stream->entries[0].dvd = DVDMgrOpen(path, 0, 0))) {
#else
    path = getMarioStDvdRoot();
    sprintf(_ssGetStr(), "%s/%s%s", path, name, ".stm");
    if (!(stream->entries[0].dvd = DVDMgrOpen(path = _ssGetStr(), 0, 0))) {
#endif
        stream->flags = 0;
        return;
    }
    if (!(stream->entries[1].dvd = DVDMgrOpen(path, 0, 0))) {
        stream->flags = 0;
        return;
    }
    DVDMgrReadAsync(stream->entries[0].dvd, stream->entries[0].buffer, 0x100, 0, SoundSSPlayChEx_main);
}

/**
 * Disc read callback for a stream header: reads format (PCM16, DPCM or ADPCM with its coefficients),
 * sample rate, channel count, loop points and sizes,
 * allocates the MusyX stream(s) (one, or a left/right pair panned hard) and starts loading the data.
 */
static void SoundSSPlayChEx_main(s32 result, DVDFileInfo* fileInfo) {
    SoundStream* stream;
    u8* dst;
    u8* src;
    StreamHeader* header;
    u8 type;
    u8 pan;
    u32 size;
    u32 len;
    int i;
    int j;

    if (result >= 0) {
        for (i = 0; i < SOUND_STREAM_MAX; i++) {
            stream = &sound.streams[i];
            if (stream->flags && &stream->entries[0].dvd->info == fileInfo) {
                break;
            }
        }
        header = stream->entries[0].buffer;
        type = header->type;
        stream->freq = header->freq;
        stream->count = header->count;
        stream->loopEnd = header->loopEnd;
        stream->loopStart = header->loopStart;
        stream->entries[0].size = header->size0;
        stream->entries[1].size = header->size1;
        stream->entries[0].loopStart = header->loopStart0;
        stream->entries[1].loopStart = header->loopStart1;
        stream->entries[0].initHist = header->initHist0;
        stream->entries[1].initHist = header->initHist1;
        stream->entries[0].loopHist = header->loopHist0;
        stream->entries[1].loopHist = header->loopHist1;
        if (stream->loopStart != -1) {
            stream->flags |= SOUND_STREAM_FLAG_LOOP;
        } else {
            stream->loopStart = 0;
        }
        switch (type) {
            case SOUND_STREAM_TYPE_RAW:
                stream->entries[0].size = stream->loopEnd;
                stream->entries[1].size = stream->loopEnd;
                stream->entries[0].loopStart = stream->loopStart;
                stream->entries[1].loopStart = stream->loopStart;
                stream->dataOffset = 0x40;
                break;
            case SOUND_STREAM_TYPE_DPCM:
                stream->flags |= SOUND_STREAM_FLAG_DPCM;
                stream->entries[0].hist = stream->entries[0].initHist;
                stream->entries[1].hist = stream->entries[1].initHist;
                stream->dataOffset = 0x40;
                break;
            case SOUND_STREAM_TYPE_ADPCM:
                stream->flags |= SOUND_STREAM_FLAG_ADPCM;
                src = (u8*)&header->coef0;
                dst = (u8*)&stream->entries[0].info;
                for (j = 32; j != 0; j--) {
                    *dst++ = *src++;
                }
                if (stream->count == 2) {
                    u8* d2;
                    u8* s2;
                    for (s2 = (u8*)&header->coef1, d2 = (u8*)&stream->entries[1].info, j = 32; j != 0; j--) {
                        *d2++ = *s2++;
                    }
                }
                stream->dataOffset = 0x100;
                break;
        }
        _ssResetEntries(stream);
        if (!(stream->flags & SOUND_STREAM_FLAG_ARAM)) {
            stream->entries[0].curBuf = 1;
            _ssDVDReadAsync_l(stream);
#if VERSION >= VERSION_JP
            stream->flags |= SOUND_STREAM_FLAG_READ_L;
#endif
            if (stream->count == 2) {
                stream->entries[1].curBuf = 1;
                _ssDVDReadAsync_r(stream);
#if VERSION >= VERSION_JP
                stream->flags |= SOUND_STREAM_FLAG_READ_R;
#endif
            }
        }
        size = sndStreamAllocLength(0xE00, 0);
        {
            u8* p = (u8*)stream->entries[0].buffer;
            u32 n;
            for (n = size; n != 0; n--) {
                *p++ = 0;
            }
        }
        if (stream->count == 2) {
            u8* p = (u8*)stream->entries[1].buffer;
            u32 n;
            for (n = size; n != 0; n--) {
                *p++ = 0;
            }
        }
        pan = 0;
        if (stream->count == 1) {
            pan = 0x40;
        }
        if (!(stream->flags & SOUND_STREAM_FLAG_ADPCM)) {
            stream->entries[0].streamId = sndStreamAllocEx(0xFF, stream->entries[0].buffer, 0xE00, stream->freq, 0, pan,
                                                           0, 0, 0, 0, 0x10000, _sscallback, i, NULL);
            if (stream->count == 2) {
                stream->entries[1].streamId = sndStreamAllocEx(0xFF, stream->entries[1].buffer, 0xE00, stream->freq, 0,
                                                               0x7F, 0, 0, 0, 0, 0x10000, _sscallback, i | 0x100, NULL);
            }
            stream->flags |= SOUND_STREAM_FLAG_STARTED;
        } else {
            stream->entries[0].streamId =
                sndStreamAllocEx(0xFF, stream->entries[0].buffer, 0xE00, stream->freq, 0, pan, 0, 0, 0, 0, 0x10001,
                                 _sscallback, i, &stream->entries[0].info);
            if (stream->count == 2) {
                stream->entries[1].streamId =
                    sndStreamAllocEx(0xFF, stream->entries[1].buffer, 0xE00, stream->freq, 0, 0x7F, 0, 0, 0, 0, 0x10001,
                                     _sscallback, i | 0x100, &stream->entries[1].info);
            }
        }
#if VERSION <= VERSION_DEMO_JP
        SoundSSSetPanCh(i, stream->pan);
#endif
        stream->fadeStep = stream->vol;
        stream->flags |= SOUND_STREAM_FLAG_FADEIN;
#if VERSION >= VERSION_JP
        SoundSSSetPanCh(i, stream->pan);
#endif
        if (stream->flags & SOUND_STREAM_FLAG_ARAM) {
            len = DVDMgrGetLength(stream->entries[0].dvd);
            if (stream->entries[0].offset + SS_LOAD_BUFFER_SIZE >= len) {
                size = len - stream->entries[0].offset;
            } else {
                size = SS_LOAD_BUFFER_SIZE;
            }
            DVDMgrReadAsync(stream->entries[0].dvd, stream->entries[0].loadBuffer[0], (size + 0x1F) & ~0x1F,
                            stream->entries[0].offset, _ssDVDReadAsync_cache_next);
            stream->entries[0].requestLength = size;
        }
    }
}

/** SoundSSStopCh for stream channel 0. */
// Original name and size (0x114) from the JP demo map (see SoundSongPlay).
void SoundSSStop(void) {
    SoundSSStopCh(0);
}

/**
 * Stops stream chan and frees its MusyX streams and files;
 * the early demos defer the stop when the stream has not started.
 */
void SoundSSStopCh(u32 chan) {
    SoundStream* stream;

    if (sound.streams[chan].flags) {
        stream = &sound.streams[chan];
#if VERSION <= VERSION_DEMO_JP
        if (!(stream->flags & SOUND_STREAM_FLAG_STARTED)) {
            stream->flags |= SOUND_STREAM_FLAG_END;
            return;
        }
#else
        sndDisableIRQ();
#endif
        if (stream->flags & SOUND_STREAM_FLAG_ARAM) {
            sound.slotUsed[stream->slot] = 0;
        }
        stream->flags = 0;
#if VERSION <= VERSION_DEMO_JP
        sndDisableIRQ();
#endif
        DVDMgrClose(stream->entries[0].dvd);
        DVDMgrClose(stream->entries[1].dvd);
        sndStreamFree(stream->entries[0].streamId);
        if (stream->count == 2) {
            sndStreamFree(stream->entries[1].streamId);
        }
        sndEnableIRQ();
        sound.streamChannel = -1;
    }
}

/** SoundSSFadeoutCh for stream channel 0. */
// Original name and size (0x74) from the JP demo map (see SoundSongPlay).
void SoundSSFadeout(void) {
    SoundSSFadeoutCh(0);
}

/** Fades stream chan out over the fade-out time (converted to frames at the current frame rate in US and later). */
void SoundSSFadeoutCh(s32 chan) {
    SoundStream* stream = &sound.streams[chan];
    f32 frames;

    if (stream->flags) {
#if VERSION >= VERSION_US
        frames = sound.fadeOut / (1000.0f / gp->framerate);
#else
        frames = sound.fadeOut / 16.666666f;
#endif
        if (frames < 1.0f) {
            frames = 1.0f;
        }
        stream->fadeStep = stream->curVol / frames;
        stream->flags |= SOUND_STREAM_FLAG_FADEOUT;
    }
}

/** SoundSSFademuteCh for stream channel 0. */
// Original name and size (0x74) from the JP demo map (see SoundSongPlay).
void SoundSSFademute(void) {
    SoundSSFademuteCh(0);
}

/** Fades stream chan out over the fade-out time and pauses it at silence (SoundSSMain sets the mute flag). */
// Original name and size (0x7c) from the JP demo map (UNUSED there: nothing calls it); SoundSSFadeoutCh for
// SOUND_STREAM_FLAG_FADEMUTE, the fade SoundSSMain runs on the same fadeStep.
void SoundSSFademuteCh(s32 chan) {
    SoundStream* stream = &sound.streams[chan];
    f32 frames;

    if (stream->flags) {
#if VERSION >= VERSION_US
        frames = sound.fadeOut / (1000.0f / gp->framerate);
#else
        frames = sound.fadeOut / 16.666666f;
#endif
        if (frames < 1.0f) {
            frames = 1.0f;
        }
        stream->fadeStep = stream->curVol / frames;
        stream->flags |= SOUND_STREAM_FLAG_FADEMUTE;
    }
}

/** SoundSSFadeinCh for stream channel 0. */
// Original name and size (0x80) from the JP demo map, like SoundSSFadeinCh (0x88) and SoundSSFadeinChEx (0x84)
// (UNUSED there: nothing calls them; the map lists them in this order between SoundSSFademuteCh and
// SoundSSContinue).
void SoundSSFadein(void) {
    SoundSSFadeinCh(0);
}

/** SoundSSFadeinChEx with the fade-in time. */
void SoundSSFadeinCh(s32 chan) {
    SoundSSFadeinChEx(chan, sound.fadeIn);
}

/**
 * Fades stream chan in from silence to its volume over time milliseconds (SoundSSMain adds fadeStep per frame).
 * SoundSSFadeoutCh's form for SOUND_STREAM_FLAG_FADEIN with the time as a parameter. The sizes fix an s32 time
 * (u16 makes SoundSSFadeinCh 0x84) and one more store: the silent start (curVol = 0) that SoundSSMain's fade-in
 * needs; where it is placed is a choice.
 */
void SoundSSFadeinChEx(s32 chan, s32 time) {
    SoundStream* stream = &sound.streams[chan];
    f32 frames;

    if (stream->flags) {
#if VERSION >= VERSION_US
        frames = time / (1000.0f / gp->framerate);
#else
        frames = time / 16.666666f;
#endif
        if (frames < 1.0f) {
            frames = 1.0f;
        }
        stream->curVol = 0.0f;
        stream->fadeStep = stream->vol / frames;
        stream->flags |= SOUND_STREAM_FLAG_FADEIN;
    }
}

/** SoundSSContinueCh for stream channel 0. */
// Original name and size (0x38) from the JP demo map (see SoundSongPlay).
void SoundSSContinue(void) {
    SoundSSContinueCh(0);
}

/** Resumes a paused stream on chan and fades it back in to its volume. */
void SoundSSContinueCh(s32 chan) {
    SoundStream* stream = &sound.streams[chan];

    if (stream->flags) {
        stream->flags &= ~SOUND_STREAM_FLAG_MUTE;
        stream->fadeStep = stream->vol;
        stream->flags |= SOUND_STREAM_FLAG_FADEIN;
    }
}

/** SoundSSSetPanCh for stream channel 0. */
// Original name and size (0x158) from the JP demo map (see SoundSongPlay).
void SoundSSSetPan(u8 pan) {
    SoundSSSetPanCh(0, pan);
}

/**
 * Sets the pan of stream chan; a stereo stream keeps its two channels panned left
 * and right with the balance applied.
 */
void SoundSSSetPanCh(u32 chan, u8 pan) {
    SoundStream* stream;
    f32 left, right;

    if (sound.streams[chan].flags) {
        stream = &sound.streams[chan];
        stream->pan = pan;
        if (stream->flags & SOUND_STREAM_FLAG_STARTED) {
            if (stream->count == 1) {
                sndStreamMixParameter(stream->entries[0].streamId, stream->curVol, pan, stream->span, 0);
            } else {
                left = stream->curVol;
                right = left;
                if (pan < 64) {
                    right = (((f32)pan * stream->curVol) / 63.0f);
                } else {
                    left = (((f32)(127 - pan) * stream->curVol) / 63.0f);
                }
                sndStreamMixParameter(stream->entries[0].streamId, left, 0, stream->span, 0);
                sndStreamMixParameter(stream->entries[1].streamId, right, 0x7F, stream->span, 0);
            }
        }
    }
}

/** SoundSSSetVolCh for stream channel 0. */
// Original name and size (0x18c) from the JP demo map (see SoundSongPlay).
void SoundSSSetVol(u8 vol) {
    SoundSSSetVolCh(0, vol);
}

/** Sets the volume of stream chan. */
void SoundSSSetVolCh(u32 chan, u8 vol) {
    SoundStream* stream;

    if (sound.streams[chan].flags) {
        stream = &sound.streams[chan];
        stream->vol = stream->curVol = vol;
        if (stream->flags & SOUND_STREAM_FLAG_STARTED) {
            SoundSSSetPanCh(chan, stream->pan);
        }
    }
}

/** Returns the volume of stream chan, 0 when none plays. */
u8 SoundSSGetVolCh(s32 chan) {
    SoundStream* stream;

    stream = &sound.streams[chan];
    if (stream->flags) {
        return stream->vol;
    } else {
        return 0;
    }
}

/** SoundSSSetSrndPanCh for stream channel 0. */
// Original name and size (0x178) from the JP demo map (see SoundSongPlay).
void SoundSSSetSrndPan(u8 span) {
    SoundSSSetSrndPanCh(0, span);
}

/** Sets the surround (front-back) pan of stream chan. */
void SoundSSSetSrndPanCh(u32 chan, u8 span) {
    SoundStream* stream = &sound.streams[chan];

    if (stream->flags) {
        stream->span = span;
        if (stream->flags & SOUND_STREAM_FLAG_STARTED) {
            SoundSSSetPanCh(chan, stream->pan);
        }
    }
}

/** SoundSSSetPlayFreqCh for stream channel 0. */
// Original name and size (0xcc) from the JP demo map (see SoundSongPlay).
void SoundSSSetPlayFreq(u16 freq) {
    SoundSSSetPlayFreqCh(0, freq);
}

/** Sets the playback sample rate of stream chan (both channels of a stereo stream). */
void SoundSSSetPlayFreqCh(s32 chan, u16 freq) {
    SoundStream* stream = &sound.streams[chan];

    if (stream->flags) {
        stream->freq = freq;
        if (stream->flags & SOUND_STREAM_FLAG_STARTED) {
            sndDisableIRQ();
            sndStreamFrq(stream->entries[0].streamId, stream->freq);
            if (stream->count == 2) {
                sndStreamFrq(stream->entries[1].streamId, stream->freq);
            }
            sndEnableIRQ();
        }
    }
}

/** Returns the state flags of stream chan, non-zero while it is in use. */
u32 SoundSSCheck(s32 chan) {
    return sound.streams[chan].flags;
}

/** Pauses stream chan: while flagged it gets no fades or disc reads. */
void SoundSSMuteOn(s32 chan) {
    if (sound.streams[chan].flags) {
        sound.streams[chan].flags |= SOUND_STREAM_FLAG_MUTE;
    }
}

/** Lifts the pause flag of stream chan without the fade-in of SoundSSContinueCh. */
void SoundSSMuteOff(s32 chan) {
    if (sound.streams[chan].flags) {
        sound.streams[chan].flags &= ~SOUND_STREAM_FLAG_MUTE;
    }
}

/**
 * Per-frame stream update: carries out deferred stops and runs volume fades:
 * fade-to-pause, fade-out (stopping at silence) and fade-in.
 */
static void SoundSSMain(void) {
    s32 i;
    SoundStream* stream;

    for (i = 0; i < SOUND_STREAM_MAX; i++) {
        stream = &sound.streams[i];
        if ((stream->flags & SOUND_STREAM_FLAG_STARTED) && !(stream->flags & SOUND_STREAM_FLAG_MUTE)) {
            if (stream->flags & SOUND_STREAM_FLAG_END) {
                stream->flags &= ~SOUND_STREAM_FLAG_END;
                SoundSSStopCh(i);
                continue;
            }
            if (stream->flags & SOUND_STREAM_FLAG_FADEMUTE) {
                if (stream->curVol == 0.0f) {
                    stream->flags |= SOUND_STREAM_FLAG_MUTE;
                    stream->flags &= ~SOUND_STREAM_FLAG_FADEMUTE;
                    continue;
                }
                stream->curVol -= stream->fadeStep;
                if (stream->curVol <= 0.0f) {
                    stream->curVol = 0.0f;
                }
                SoundSSSetPanCh(i, stream->pan);
            }
            if (stream->flags & SOUND_STREAM_FLAG_FADEOUT) {
                if (stream->curVol == 0.0f) {
                    stream->flags &= ~SOUND_STREAM_FLAG_FADEOUT;
                    SoundSSStopCh(i);
                    continue;
                }
                stream->curVol -= stream->fadeStep;
                if (stream->curVol <= 0.0f) {
                    stream->curVol = 0.0f;
                }
                SoundSSSetPanCh(i, stream->pan);
            }
            if (stream->flags & SOUND_STREAM_FLAG_FADEIN) {
                stream->curVol += stream->fadeStep;
                if (stream->curVol >= stream->vol) {
                    stream->curVol = stream->vol;
                    stream->fadeStep = 0.0f;
                    stream->flags &= ~SOUND_STREAM_FLAG_FADEIN;
                }
                SoundSSSetPanCh(i, stream->pan);
            }
        }
    }
}

/**
 * Audio-interrupt stream service: starts the disc read for whichever load buffer has been consumed (both
 * channels of a stereo stream), so playback never runs dry.
 */
static void SoundSSMainInt(void) {
    int i;
    SoundStream* stream;

    for (i = 0; i < SOUND_STREAM_MAX; i++) {
        stream = &sound.streams[i];
#if VERSION >= VERSION_US
        if ((stream->flags & SOUND_STREAM_FLAG_STARTED) && !(stream->flags & SOUND_STREAM_FLAG_MUTE)) {
            if (stream->flags & SOUND_STREAM_FLAG_PLAY) {
                if (stream->entries[0].srcPos != 0 && stream->entries[0].loaded == 0 && !(stream->flags & SOUND_STREAM_FLAG_READ_L)) {
                    _ssDVDReadAsync_l(stream);
                    stream->flags |= SOUND_STREAM_FLAG_READ_L;
                    if (stream->flags & SOUND_STREAM_FLAG_ARAM) {
                        stream->entries[0].loaded = 1;
                    }
                }
                if (stream->count == 2 && stream->entries[1].srcPos != 0 && stream->entries[1].loaded == 0 &&
                    !(stream->flags & SOUND_STREAM_FLAG_READ_R)) {
                    _ssDVDReadAsync_r(stream);
                    stream->flags |= SOUND_STREAM_FLAG_READ_R;
                    if (stream->flags & SOUND_STREAM_FLAG_ARAM) {
                        stream->entries[1].loaded = 1;
                    }
                }
            }
            if (stream->count == 2 && stream->entries[0].loaded == 0 && stream->entries[1].loaded == 0 &&
                stream->entries[0].curBuf != stream->entries[1].curBuf) {
                stream->entries[1].curBuf = stream->entries[0].curBuf;
                stream->entries[1].srcPos = stream->entries[0].srcPos;
                stream->entries[1].pos = stream->entries[0].pos;
                stream->entries[1].offset = stream->entries[0].offset;
            }
            if (stream->count == 2) {
                if (sound.coverState == SOUND_COVER_RELOADING && stream->entries[0].loaded == 1 && stream->entries[1].loaded == 1) {
                    sound.coverState = SOUND_COVER_CLOSED;
                }
            } else if (sound.coverState == SOUND_COVER_RELOADING && stream->entries[0].loaded == 1) {
                sound.coverState = SOUND_COVER_CLOSED;
#else
        if ((stream->flags & SOUND_STREAM_FLAG_STARTED) && !(stream->flags & SOUND_STREAM_FLAG_MUTE) && (stream->flags & SOUND_STREAM_FLAG_PLAY)) {
            if (stream->entries[0].srcPos != 0 && stream->entries[0].loaded == 0 && !(stream->flags & SOUND_STREAM_FLAG_READ_L)) {
                _ssDVDReadAsync_l(stream);
                stream->flags |= SOUND_STREAM_FLAG_READ_L;
                if (stream->flags & SOUND_STREAM_FLAG_ARAM) {
                    stream->entries[0].loaded = 1;
                }
            }
            if (stream->count == 2 && stream->entries[1].srcPos != 0 && stream->entries[1].loaded == 0 &&
                !(stream->flags & SOUND_STREAM_FLAG_READ_R)) {
                _ssDVDReadAsync_r(stream);
                stream->flags |= SOUND_STREAM_FLAG_READ_R;
                if (stream->flags & SOUND_STREAM_FLAG_ARAM) {
                    stream->entries[1].loaded = 1;
                }
#endif
            }
        }
    }
}

/**
 * Hooks the audio DMA interrupt (keeping the previous callback for SoundAIDMACallback to chain to) and restarts
 * the sample record; US and later also note the DMA buffer and hang if it is longer than 0x280 bytes.
 */
// Original name and size (0x40) from the JP demo map (UNUSED there: its one call, in SoundInit, is inlined).
void SoundInitAI(void) {
    sound.callback = AIRegisterDMACallback(SoundAIDMACallback);
#if VERSION >= VERSION_US
    sound.dmaAddress = (u8*)(AIGetDMAStartAddr() | 0x80000000);
    sound.dmaLength = AIGetDMALength();
    if (sound.dmaLength > 0x280) {
        while (1)
            ;
    }
#endif
    sound.sampleBank = 0;
    sound.sampleIndex = 0;
}

/**
 * Audio DMA interrupt: chains to the previous callback,
 * then copies the frame's output (byte-swapped, one channel) into the two 1024-sample banks in sound.samples,
 * a running record of what is being played.
 */
static void SoundAIDMACallback(void) {
    s32 length;
    u8* address;
    u16* dst;
#if VERSION >= VERSION_US
    s32 n;
    s16* out;
    s16* src;

    if (sound.callback != NULL) {
        sound.callback();
    }
    DCFlushRange(sound.dmaAddress, sound.dmaLength);
    address = sound.dmaAddress;
    length = sound.dmaLength;
#else
    s16* src;
    s32 n;
    s16* out;

    sound.callback();
    sound.dmaAddress = (u8*)(AIGetDMAStartAddr() | 0x80000000);
    sound.dmaLength = length = AIGetDMALength();
    address = sound.dmaAddress;
#endif
    src = (s16*)sound.dmaBuf;
    dst = (u16*)src;
    while (length != 0) {
        *dst++ = (address[0] << 8) | address[1];
        address += 2;
        length -= 2;
    }
    n = sound.dmaLength >> 1;
    out = sound.samples[sound.sampleBank];
    out += sound.sampleIndex;
    while (n != 0) {
        *out++ = *src;
        src += 2;
        if (++sound.sampleIndex >= 0x400) {
            sound.sampleIndex = 0;
            sound.sampleBank = 1 - sound.sampleBank;
            out = sound.samples[sound.sampleBank];
        }
        n -= 2;
    }
}
