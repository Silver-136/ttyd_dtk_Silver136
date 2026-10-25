#define _MATH_H_ // skip math.h (its sqrtf statics are not in this TU)
#include "sound.h"
#include <dolphin/os.h>
#include "manager/dvdmgr.h"
#include "memory.h"
#include "musyx/musyx.h"
#include "system.h"
#include "libc_decls.h"
#include <string.h>
#include "eff/eff_magic1_n64.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#endif

// TODO: use SoundInit to fill in SoundStream/Entry

//.bss
SND_AUX_REVERBHI revH;
SND_AUX_REVERBSTD revS;
SND_AUX_CHORUS cho;
SND_AUX_DELAY dly;
#define SS_LOAD_BUFFER_SIZE 0x8000 // bytes in each stream load buffer, the size of one DVD/ARAM read

SoundWork sound;

//.sbss
u32 aramMemArray[8];

static inline char* _ssGetStr(void) {
    static char str[0x100];
    return str;
}
#define str _ssGetStr()

// local prototypes
void* sndMalloc(size_t size);
void sndFree(void* ptr);
void* loadDVD_callback(u32 offset, u32 length);
void SoundEfxMain(void);
void SoundDVDMain(void);

void SoundSSMainInt(void);
void SoundSSMain(void);

void SoundAIDMACallback(void);

void* sndMalloc(size_t size) {
    return __memAlloc(HEAP_DEFAULT, size);
}

void sndFree(void* ptr) {
    __memFree(HEAP_DEFAULT, ptr);
}

void SoundInit(void) { // 1:1, assuming sndMalloc inlines
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
    if (sound.outputMode != SND_OUTPUTMODE_STEREO) {
        sound.outputMode = SND_OUTPUTMODE_STEREO;
        sndOutputMode(SND_OUTPUTMODE_STEREO);
    }
    sound.callback = AIRegisterDMACallback(SoundAIDMACallback);
#ifdef VERSION_G8ME01
    sound.dmaAddress = (u8*)(AIGetDMAStartAddr() | 0x80000000);
    sound.dmaLength = AIGetDMALength();
    if (sound.dmaLength > 0x280) {
        while (1)
            ;
    }
#endif
    sound.sampleBank = 0;
    sound.sampleIndex = 0;
    sound.fadeIn = 10;
    sound.fadeOut = 500;
    sound.dataId = 0;
    sound.slibId = 0;
    sound.coverState = 0;
    sound.slotCount = 0;
    sound.nextGroupId = 0;

    data = sndMalloc(0x20000);
    sound.filedata = data;
    if (data == 0) {
        while (1)
            ;
    } // infinite loop

    sound.groups = 0;

    // force register
    tempsong = (sound.songs = sndMalloc(sizeof(SoundSong) * 4));
    songs = tempsong;
    if (songs == 0) {
        while (1)
            ;
    } // infinite loop

    for (i = 0; i < 4; i++, songs++) {
        songs->seqId = -1;
        songs->group = NULL;
    }

    effects = sndMalloc(sizeof(SoundEffect) * 40);
    sound.effects = effects;
    if (effects == 0) {
        while (1)
            ;
    } // infinite loop

    for (i = 0; i < 40; i++, effects++) {
        effects->flags = 0;
        effects->voiceId = -1;
        effects->priority = 0;
        effects->vol = 0x7F;
        effects->pan = 0x40;
    }

    // force register
    tempdvd = (sound.dvds = sndMalloc(sizeof(SoundDVD) * 2));
    dvds = tempdvd;
    if (dvds == 0) {
        while (1)
            ;
    } // infinite loop

    for (i = 0; i < 2; i++, dvds++) {
        dvds->flags = 0;
    }

    streams = sndMalloc(sizeof(SoundStream) * 3);
    sound.streams = streams;
    if (streams == 0) {
        while (1)
            ;
    } // infinite loop

    temp = streams;
    for (i = 0; i < 3; i++, temp++) {
        temp->flags = 0;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == 0) {
            while (1)
                ;
        } // infinite loop
        temp->entries[0].loadBuffer[0] = ptr;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == 0) {
            while (1)
                ;
        } // infinite loop
        temp->entries[0].loadBuffer[1] = ptr;

        size = sndStreamAllocLength(0xE00, 0);
        ptr = sndMalloc(size);
        if (ptr == 0) {
            while (1)
                ;
        } // infinite loop
        temp->entries[0].buffer = ptr;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == 0) {
            while (1)
                ;
        } // infinite loop
        temp->entries[1].loadBuffer[0] = ptr;

        ptr = sndMalloc(SS_LOAD_BUFFER_SIZE);
        if (ptr == 0) {
            while (1)
                ;
        } // infinite loop
        temp->entries[1].loadBuffer[1] = ptr;

        size = sndStreamAllocLength(0xE00, 0);
        ptr = sndMalloc(size);
        if (ptr == 0) {
            while (1)
                ;
        } // infinite loop
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

void SoundMain(void) {
    int i;
    SoundSong* song;

    for (i = 0, song = sound.songs; i < 4; i++, song++) {
        if (song->group && song->seqId != -1 && sndSeqGetValid(song->seqId) != TRUE) {
            song->group->flags |= 2;
            sndSeqStop(song->seqId);
            song->seqId = -1;
        }
    }
    SoundEfxMain();
    SoundDVDMain();
    SoundSSMain();
}

void SoundMainInt(void) {
    SoundSSMainInt();
}

void SoundSetFadeTime(u16 in, u16 out) { // 1:1
    sound.fadeIn = in;
    sound.fadeOut = out;
}

void* loadDVD_callback(u32 offset, u32 bytes) { // 1:1
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

BOOL SoundLoadDVD2(const char* path) { // 1:1
    DVDEntry* entry;
    u32 length;
    void* pointer;

    if (sound.dataId >= 8) {
        return 0;
    }

    sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".pool");
    entry = DVDMgrOpen(str, 2, 0);
    length = entry == NULL; // fixes regalloc
    if (entry == NULL) {
        pointer = NULL;
    } else {
        length = DVDMgrGetLength(entry);
        if (length == 0) {
            pointer = NULL;
        } else {
            length = OSRoundUp32B(length);
            pointer = __memAlloc(HEAP_DEFAULT, length);
            if (pointer == NULL) {
                pointer = NULL;
            } else if (DVDMgrRead(entry, pointer, length, 0) <= 0) {
                __memFree(HEAP_DEFAULT, pointer);
                pointer = NULL;
            } else {
                DVDMgrClose(entry);
            }
        }
    }
    if (pointer == NULL) {
        return FALSE;
    }
    sound.poolData[sound.dataId] = pointer;

    sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".proj");
    entry = DVDMgrOpen(str, 2, 0);
    if (entry == NULL) {
        pointer = NULL;
    } else {
        length = DVDMgrGetLength(entry);
        if (length == 0) {
            pointer = NULL;
        } else {
            length = OSRoundUp32B(length);
            pointer = __memAlloc(HEAP_DEFAULT, length);
            if (pointer == NULL) {
                pointer = NULL;
            } else if (DVDMgrRead(entry, pointer, length, 0) <= 0) {
                __memFree(HEAP_DEFAULT, pointer);
                pointer = NULL;
            } else {
                DVDMgrClose(entry);
            }
        }
    }
    if (pointer == NULL) {
        return FALSE;
    }
    sound.projData[sound.dataId] = pointer;

    sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".sdir");
    entry = DVDMgrOpen(str, 2, 0);
    if (entry == NULL) {
        pointer = NULL;
    } else {
        length = DVDMgrGetLength(entry);
        if (length == 0) {
            pointer = NULL;
        } else {
            length = OSRoundUp32B(length);
            pointer = __memAlloc(HEAP_DEFAULT, length);
            if (pointer == NULL) {
                pointer = NULL;
            } else if (DVDMgrRead(entry, pointer, length, 0) <= 0) {
                __memFree(HEAP_DEFAULT, pointer);
                pointer = NULL;
            } else {
                DVDMgrClose(entry);
            }
        }
    }
    if (pointer == NULL) {
        return FALSE;
    }
    sound.sdirData[sound.dataId] = pointer;

    sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".samp");
    strcpy(sound.filepath, str);
    sndSetSampleDataUploadCallback(loadDVD_callback, 0x20000);
    return TRUE;
}

BOOL SoundLoadDVD2PushGroup(u8* groupIds) { // 1:1
    SND_GROUPID id;
    s32 temp;

    if (sound.dataId >= 8) {
        return 0;
    }
    sound.groupCount[sound.dataId] = 0;
    if (groupIds) {
        while (*groupIds != 0xFF) {
            id = *groupIds;
            sndPushGroup(sound.projData[sound.dataId], id, 0, sound.sdirData[sound.dataId],
                         sound.poolData[sound.dataId]);
            sound.groupCount[sound.dataId]++;
            groupIds++;
        };
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

void SoundLoadDVD2Free(void) {
    if (sound.filedata) {
        __memFree(HEAP_DEFAULT, sound.filedata);
        sound.filedata = NULL;
    }
}

BOOL SoundSLibLoadDVD(const char* path) { // regalloc, stack, otherwise 1:1
    SoundGroup* groupsHead;
    u32 length2;
    u32 length3;
    SoundGroup* iter;
    SoundGroup* group;
    SoundGroup* tmp;
    u32 blockSize;
    u32* cursor;
    u16 id;
    void* val;
    DVDEntry* entry;
    u32 length;
    void* pointer;
    void* pointer2;
    void* pointer3;

    if (sound.slibId >= 8) {
        return FALSE;
    }

    sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".slib");
    entry = DVDMgrOpen(str, 2, 0);
    if (entry == NULL) {
        pointer = NULL;
    } else {
        length = DVDMgrGetLength(entry);
        if (length == 0) {
            pointer = NULL;
        } else {
            length = OSRoundUp32B(length);
            pointer = __memAlloc(HEAP_DEFAULT, length);
            if (pointer == NULL) {
                pointer = NULL;
            } else if (DVDMgrRead(entry, pointer, length, 0) <= 0) {
                __memFree(HEAP_DEFAULT, pointer);
                pointer = NULL;
            } else {
                DVDMgrClose(entry);
            }
        }
    }
    if ((cursor = pointer) == NULL) {
        return FALSE;
    }
    sound.slibData[sound.slibId] = pointer;

    while (1) {
        blockSize = *cursor;
        if (*cursor == 0xFFFFFFFF) {
            break;
        }
        group = __memAlloc(HEAP_DEFAULT, 0x10);
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
        group->flags = 1;
        group->index = id;
        group->groupId = 0;
        group->songId = 0;
        group->arrfile = cursor + 8;
        group->next = 0;
        cursor = (u32*)((u8*)cursor + blockSize);
    }

    sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".stbl");
    entry = DVDMgrOpen(str, 2, 0);
    if (entry == NULL) {
        pointer2 = NULL;
    } else {
        length2 = DVDMgrGetLength(entry);
        if (length2 == 0) {
            pointer2 = NULL;
        } else {
            length2 = OSRoundUp32B(length2);
            pointer2 = __memAlloc(HEAP_DEFAULT, length2);
            if (pointer2 == NULL) {
                pointer2 = NULL;
            } else if (DVDMgrRead(entry, pointer2, length2, 0) <= 0) {
                __memFree(HEAP_DEFAULT, pointer2);
                pointer2 = NULL;
            } else {
                DVDMgrClose(entry);
            }
        }
    }
    if (pointer2 == NULL) {
        return FALSE;
    }
    sound.stblData[sound.slibId] = pointer2;

    sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".etbl");
    entry = DVDMgrOpen(str, 2, 0);
    if (entry == NULL) {
        pointer3 = NULL;
    } else {
        length3 = DVDMgrGetLength(entry);
        if (length3 == 0) {
            pointer3 = NULL;
        } else {
            length3 = OSRoundUp32B(length3);
            pointer3 = __memAlloc(HEAP_DEFAULT, length3);
            if (pointer3 == NULL) {
                pointer3 = NULL;
            } else if (DVDMgrRead(entry, pointer3, length3, 0) <= 0) {
                __memFree(HEAP_DEFAULT, pointer3);
                pointer3 = NULL;
            } else {
                DVDMgrClose(entry);
            }
        }
    }
    if (pointer3 == NULL) {
        return FALSE;
    }
    sound.etblData[sound.slibId] = pointer3;
    sound.slibId++;
    return TRUE;
}

static inline void _ssDropStop(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        SoundSong* song = &sound.songs[i];
        if (sndSeqGetValid(song->seqId)) {
            sndSeqStop(song->seqId);
            song->seqId = (SND_SEQID)-1;
        }
    }
    for (i = 0; i < 40; i++) {
        SoundEffect* effect = &sound.effects[i];
        if (effect->flags) {
            sndFXKeyOff(effect->voiceId);
            if (effect->flags & 0x10) {
                sndRemoveEmitter(&effect->emitter);
            }
            effect->flags = 0;
        }
    }

    for (i = 0; i < 2; i++) {
        SoundDVD* dvd = &sound.dvds[i];
        if (dvd->flags & 3) {
            AISetStreamPlayState(0);
            dvd->flags = 0;
            sound.dvdChannel = -1;
            DVDCancelStream(&dvd->info.cb);
            DVDClose(&dvd->info);
        }
    }

    for (i = 0; i < 3; i++) {
        SoundStream* stream;
        if (sound.streams[i].flags) {
            stream = &sound.streams[i];
            if (!sound.interruptDepth) {
                sound.interrupts = OSDisableInterrupts();
            }
            sound.interruptDepth++;
            if (stream->flags & 0x400) {
                sound.slotUsed[stream->slot] = 0;
            }
            stream->flags = 0;
            DVDMgrClose(stream->entries[0].dvd);
            DVDMgrClose(stream->entries[1].dvd);
            sndStreamFree(stream->entries[0].streamId);
            if (stream->count == 2) {
                sndStreamFree(stream->entries[1].streamId);
            }
            if (sound.interruptDepth) {
                if (!--sound.interruptDepth) {
                    OSRestoreInterrupts(sound.interrupts);
                }
            }
            sound.streamChannel = -1;
        }
    }
}

BOOL SoundDropData(void) {
    SoundGroup *group; SoundGroup *temp;
    u32 i;

    group = sound.groups;
    _ssDropStop();
    sndSilence();
    for (i = 0; i < 4; i++) {
        SoundSong* song = &sound.songs[i];
        if (sndSeqGetValid(song->seqId)) {
            sndSeqStop(song->seqId);
            song->seqId = (SND_SEQID)(-1);
        }
    }
    for (i = 0; i < 40; i++) {
        SoundEffect* effect = (0, &sound.effects[i]);
        if (effect->flags) {
            sndFXKeyOff(effect->voiceId);
            if (effect->flags & 0x10) {
                sndRemoveEmitter(&effect->emitter);
            }
            effect->flags = 0;
        }
    }

    for (i = 0; i < 2; i++) {
        SoundDVD* dvd = (0, &sound.dvds[i]);
        if (dvd->flags & 3) {
            AISetStreamPlayState(0);
            dvd->flags = 0;
            sound.dvdChannel = -1;
            DVDCancelStream(&dvd->info.cb);
            DVDClose(&dvd->info);
        }
    }

    for (i = 0; i < 3; i++) {
        SoundStream* stream;
        if (sound.streams[i].flags) {
            stream = &sound.streams[i];
            if (!sound.interruptDepth) {
                sound.interrupts = OSDisableInterrupts();
            }
            sound.interruptDepth++;
            if (stream->flags & 0x400) {
                sound.slotUsed[stream->slot] = 0;
            }
            stream->flags = 0;
            DVDMgrClose(stream->entries[0].dvd);
            DVDMgrClose(stream->entries[1].dvd);
            sndStreamFree(stream->entries[0].streamId);
            if (stream->count == 2) {
                sndStreamFree(stream->entries[1].streamId);
            }
            if (sound.interruptDepth) {
                if (!(--sound.interruptDepth)) {
                    OSRestoreInterrupts(sound.interrupts);
                }
            }
            sound.streamChannel = -1;
        }
    }

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
        if (!(group->flags & 1)) {
            __memFree(HEAP_DEFAULT, group->arrfile);
        }
        __memFree(HEAP_DEFAULT, group);
        group = temp;
    }
    sound.groups = 0;

    return TRUE;
}

void SoundSetOutputMode(SND_OUTPUTMODE mode) {
    if (sound.outputMode != mode) {
        sound.outputMode = mode;
        sndOutputMode(mode);
    }
}

void SoundOpenCover(void) {
    if (!sound.coverState) {
        sound.coverState = 1;
    }
}

void SoundCloseCover(void) {
    SoundStream* stream;
    BOOL close;
    int i;

    if (sndIsInstalled() && sound.coverState == 1) {
        close = FALSE;
        for (i = 0; i < 3; i++) {
            stream = &sound.streams[i];
            if (stream->flags & 8 && !(stream->flags & 0x400) && !(stream->flags & 2)) {
                if (stream->flags & 0x1000) {
                    close = TRUE;
                }
                if (stream->count == 2 && stream->flags & 0x2000) {
                    close = TRUE;
                }
            }
        }
        if (close) {
            sound.coverState = 2;
        } else {
            sound.coverState = 0;
        }
    }
}

inline SoundGroup* SoundSongGetGroup(u8 a1) {
    u16 temp = a1;
    SoundGroup* group;

    for (group = sound.groups;; group = group->next) {
        if (group == 0) {
            return 0;
        }
        if (group->index == temp) {
            break;
        }
    }
    return group;
}

void SoundSongPlayCh(s32 chan, u32 a2) {
    SoundSong* song;
    SoundGroup* group;

    song = &sound.songs[chan];
    if (sndSeqGetValid(song->seqId) == 1) {
        sndSeqStop(song->seqId);
    }
    song->vol = 127;
    group = SoundSongGetGroup((u8)a2);
    if (group) {
        song->group = group;
        group->groupId = (SND_GROUPID)((a2 >> 16) & 0xFF);
        group->songId = (SND_SONGID)((a2 >> 8) & 0xFF);
        song->mask = 0xFFFFFFFF;
        song->seqId = sndSeqPlayEx(group->groupId, group->songId, group->arrfile, 0, 0);
        sndSeqVolume((u8)song->vol, 0, song->seqId, 0);
        sndSeqContinue(song->seqId);
        sndSeqMute(song->seqId, song->mask, 0);
    }
}

void SoundSongContinueCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqContinue(song->seqId);
        sndSeqVolume((u8)song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
    }
}

void SoundSongStopCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqStop(song->seqId);
        song->seqId = (SND_SEQID)-1;
    }
}

void SoundSongFadeoutCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqVolume(0, sound.fadeOut, song->seqId, SND_SEQVOL_STOP);
        song->seqId = (SND_SEQID)-1;
    }
}

void SoundSongFadeinCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        sndSeqVolume((u8)song->vol, sound.fadeIn, song->seqId, SND_SEQVOL_CONTINUE);
    }
}

void SoundSongSetVolCh(s32 chan, u8 vol) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId)) {
        if (vol > 127) {
            vol = 127;
        }
        song->vol = vol;
        sndSeqVolume((u8)song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
    }
}

u8 SoundSongGetVolCh(s32 chan) {
    SoundSong* song = &sound.songs[chan];

    if (sndSeqGetValid(song->seqId) == 0) {
        return 0;
    }
    return (u8)song->vol;
}

u32 SoundSongCheck(s32 chan) {
    return sndSeqGetValid(sound.songs[chan].seqId) != 0;
}

inline void SoundEfxMuteCh(s32 chan) {
    SoundEffect* sfx = &sound.effects[chan];
    if (sfx->flags) {
        sndFXKeyOff(sfx->voiceId);
        if (sfx->flags & 0x10) {
            sndRemoveEmitter(&sfx->emitter);
        }
        sfx->flags = 0;
    }
}

inline s32 SoundEfxStopAll(u8 a1) {
    s32 chan;
    SoundEffect* sfx = sound.effects;

    for (chan = 0; chan < 40; chan++, sfx++) {
        if (sfx->priority <= a1) {
            SoundEfxMuteCh(chan);
            return chan;
        }
    }
    return chan;
}

inline u32 SoundEfxCheckPlay(s32 chan) {
    SoundEffect* sfx = &sound.effects[chan];
    SND_VOICEID vid;
    if (!sfx->flags) {
        return FALSE;
    }
    if (!(sfx->flags & 0x10)) {
        if (sfx->flags & 0x10) {
            vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
        } else {
            vid = sndFXCheck(sfx->voiceId);
        }
        if (sndFXCheck(vid) == -1) {
            return FALSE;
        }
    } else if (!sndCheckEmitter(&sfx->emitter)) {
        return FALSE;
    }
    return TRUE;
}

s32 SoundEfxPlayEx(SND_FXID fid, u8 a2, u8 vol, u8 pan) {
    SoundEffect* sfx;
    s32 chan;

    sfx = sound.effects;
    for (chan = 0; chan < 40; chan++, sfx++) {
        if (!sfx->flags) {
            break;
        }
    }
    if (chan >= 40u) {
        sfx = sound.effects;
        for (chan = 0; chan < 40; chan++, sfx++) {
            if (sfx->priority <= a2) {
                SoundEfxMuteCh(chan);
                break;
            }
        }
        if (chan >= 40u) {
            return -1;
        }
    }
    sfx->flags = 0;
    if (!sound.interruptDepth) {
        sound.interrupts = OSDisableInterrupts();
    }
    ++sound.interruptDepth;
    sfx->voiceId = sndFXStart(fid, 127, pan);
    sndFXVolume(sfx->voiceId, vol);
    if (sound.interruptDepth && !--sound.interruptDepth) {
        OSRestoreInterrupts(sound.interrupts);
    }
    if (sfx->voiceId == -1) {
        return -1;
    }
    sfx->flags = 1;
    sfx->priority = a2;
    sfx->vol = vol;
    sfx->pan = pan;
    sfx->span = 0;
    sfx->fid = fid;
    if (sound.coverState) {
        sndFXVolume(sfx->voiceId, 0);
    }
    if (!SoundEfxCheckPlay(chan)) {
        chan = -1;
    }
    return chan;
}

void SoundEfxStop(s32 chan) {
    SoundEffect* sfx;

    sfx = &sound.effects[chan];
    if (sfx->flags) {
        sndFXKeyOff(sfx->voiceId);
        if (sfx->flags & 0x10) {
            sndRemoveEmitter(&sfx->emitter);
        }
        sfx->flags = 0;
    }
}

void SoundEfxSetPitch(s32 chan, u16 pitch) {
    SoundEffect* sfx;
    SND_VOICEID vid;

    sfx = &sound.effects[chan];
    if (sfx->flags & 0x10) {
        vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
    } else {
        vid = sndFXCheck(sfx->voiceId);
    }
    if (vid != -1) {
        sndFXPitchBend(vid, (u16)(pitch + 0x2000));
    }
}

void SoundEfxSetVolume(s32 chan, u8 vol) {
    SoundEffect* sfx;
    SND_VOICEID vid;

    sfx = &sound.effects[chan];
    if (sfx->flags & 0x10) {
        vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
    } else {
        vid = sndFXCheck(sfx->voiceId);
    }
    if (vid != -1) {
        sfx->vol = vol;
        sndFXVolume(vid, vol);
        if (sound.coverState) {
            sndFXVolume(vid, 0);
        }
    }
}

u8 SoundEfxGetVolume(s32 chan) {
    s32 off = chan * sizeof(SoundEffect);
    return (u8)*(u16*)((u8*)sound.effects + off + offsetof(SoundEffect, vol)); // FAKE: byte-offset load from the chan * sizeof offset (sound.effects[chan].vol schedules differently)
}

void SoundEfxSetAux1(s32 chan, u8 reverb) {
    SoundEffect* sfx;
    SND_VOICEID vid;

    sfx = &sound.effects[chan];
    if (sfx->flags & 0x10) {
        vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
    } else {
        vid = sndFXCheck(sfx->voiceId);
    }
    if (vid != -1) {
        sndFXReverb(vid, reverb);
    }
}

void SoundEfxSetPan(s32 chan, u8 pan) {
    SoundEffect* sfx;
    SND_VOICEID vid;
    s32 off = chan * sizeof(SoundEffect);

    *(u16*)((u8*)sound.effects + off + offsetof(SoundEffect, pan)) = pan; // FAKE: byte-offset store, then sfx from the same offset (sfx = &sound.effects[chan]; sfx->pan = pan does not match)
    sfx = (SoundEffect*)((u8*)sound.effects + off);
    if (sfx->flags & 0x10) {
        vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
    } else {
        vid = sndFXCheck(sfx->voiceId);
    }
    if (vid != -1) {
        sndFXPanning(vid, pan);
    }
}

void SoundEfxSetSrndPan(s32 chan, u8 span) {
    SoundEffect* sfx;
    SND_VOICEID vid;
    s32 doppler;
    s32 off = chan * sizeof(SoundEffect);

    *(u16*)((u8*)sound.effects + off + offsetof(SoundEffect, span)) = span; // FAKE: byte-offset store, then sfx from the same offset (sfx = &sound.effects[chan]; sfx->span = span does not match)
    sfx = (SoundEffect*)((u8*)sound.effects + off);
    vid = (sfx->flags & 0x10) ? sndFXCheck(sndEmitterVoiceID(&sfx->emitter))
                              : sndFXCheck(sfx->voiceId);
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

void SoundEfxSetLPF(s32 chan, s16 freq) {
    SoundEffect* sfx;
    u32 voiceId;

    sfx = &sound.effects[chan];
    if (sfx->flags & 0x10) {
        voiceId = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
    } else {
        voiceId = sndFXCheck(sfx->voiceId);
    }
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

BOOL SoundEfxCheck(s32 chan) {
    SoundEffect* sfx;

    sfx = &sound.effects[chan];
    if (!sfx->flags) {
        return FALSE;
    }
    if (!(sfx->flags & 0x10)) {
        SND_VOICEID vid;
        if (sfx->flags & 0x10) {
            vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
        } else {
            vid = sndFXCheck(sfx->voiceId);
        }
        if (sndFXCheck(vid) == -1) {
            return FALSE;
        }
    } else {
        if (!sndCheckEmitter(&sfx->emitter)) {
            return FALSE;
        }
    }
    return TRUE;
}

inline void SoundEfxCheckCh(s32 chan) {
    SoundEffect* effect;
    SND_VOICEID vid;

    effect = &sound.effects[chan];
    if (effect->flags & 0x10) {
        vid = sndFXCheck(sndEmitterVoiceID(&effect->emitter));
    } else {
        vid = sndFXCheck(effect->voiceId);
    }
    if (sndFXCheck(vid) == -1) {
        SoundEfxMuteCh(chan);
    }
}

// FAKE: opt_strength_reduction off for this function only. With it on, the second loop's chan*sizeof offset becomes an
// induction temp created before the loop's split temps and gets the highest register; the target has the effect
// pointer (second web of the reused `effect`) r30, i r29, the offset r28 (mwregdump, phase 5).
#pragma opt_strength_reduction off
void SoundEfxMain(void) {
    SND_FVECTOR* dir;
    SoundEffect* effect;
    int i;
    f32 v10;
    f32 v9;
    f32 r = 1.0f;
    f32 v6;
    f32 x, y, z;

    v10 = v9 = 0.0f;
    x = sound.pos.x - sound.prevPos.x;
    y = sound.pos.y - sound.prevPos.y;
    z = sound.pos.z - sound.prevPos.z;
    v6 = (TWO_PI * sound.listenerDir / 360.0f);
    sound.prevPos.x = sound.pos.x;
    sound.prevPos.y = sound.pos.y;
    sound.prevPos.z = sound.pos.z;
    dir = &sound.dir;
    dir->x = x;
    dir->y = y;
    dir->z = z;
    v9 += r * (f32)sin(v6);
    v10 -= r * (f32)cos(v6);
    sound.heading.x = v9;
    sound.heading.y = 0.0f;
    sound.heading.z = v10;
    sound.up.x = 0.0f;
    sound.up.y = 1.0f;
    sound.up.z = 0.0f;
    sndUpdateListener(&sound.listener, &sound.pos, dir, &sound.heading, &sound.up, (u8)sound.listenerVol, NULL);

    effect = sound.effects;
    for (i = 0; i < 40; i++, effect++) {
        if (effect->flags) {
            if (effect->flags & 0x10) {
                effect->dir.x = effect->pos.x - effect->prevPos.x;
                effect->dir.y = effect->pos.y - effect->prevPos.y;
                effect->dir.z = effect->pos.z - effect->prevPos.z;
                effect->prevPos.x = effect->pos.x;
                effect->prevPos.y = effect->pos.y;
                effect->prevPos.z = effect->pos.z;
                sndUpdateEmitter(&effect->emitter, &effect->pos, &effect->dir, (u8)effect->vol, NULL);
            }
        }
    }

    for (effect = sound.effects, i = 0; i < 40; i++, effect++) {
        if (effect->flags) {
            if (!(effect->flags & 0x10)) {
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
#pragma opt_strength_reduction reset

inline void SoundDVDStopCh(s32 chan) {
    SoundDVD* dvd = &sound.dvds[chan];
    if (dvd->flags & 3) {
        AISetStreamPlayState(0);
        dvd->flags = 0;
        sound.dvdChannel = -1;
        DVDCancelStream(&dvd->info.cb);
        DVDClose(&dvd->info);
    }
}

inline void SoundDVDSetPanCh(s32 chan, u8 pan) { // I think this is the correct func name
    f32 left, right;
    SoundDVD* dvd;

    dvd = &sound.dvds[chan];
    dvd->pan = pan;
    left = dvd->vol;
    right = left;
    if (pan < 64) {
        right = (((f32)pan * dvd->vol) / 63.0f);
    }
    if (!((u32)(pan - 64) >> 31)) {
        left = (((f32)(127 - pan) * dvd->vol) / 63.0f);
    }
    AISetStreamVolLeft((u8)left);
    AISetStreamVolRight((u8)right);
}

void SoundDVDMain(void) {
    u32 i;
    SoundDVD* dvd;

    for (i = 0; (int)i < 2; i++) {
        SoundDVD* dvd = &sound.dvds[i];
        if (!(dvd->flags & 2)) {
            if (dvd->flags & 0x40) {
                if (dvd->vol == 0.0f) {
                    dvd->samples = AIGetStreamSampleCount();
                    SoundDVDStopCh(i);
                    dvd->flags |= 0x2;
                    dvd->flags &= ~0x40;
                    continue;
                }
                dvd->vol -= dvd->volStep;
                if (dvd->vol <= 0.0f) {
                    dvd->vol = 0.0f;
                }
                SoundDVDSetPanCh(i, (u8)dvd->pan);
            }
            if (dvd->flags & 0x20) {
                if (dvd->vol == 0.0f) {
                    dvd->flags &= ~0x20;
                    SoundDVDStopCh(i);
                    continue;
                }
                dvd->vol -= dvd->volStep;
                if (dvd->vol <= 0.0f) {
                    dvd->vol = 0.0f;
                }
                SoundDVDSetPanCh(i, (u8)dvd->pan);
            }
            if (dvd->flags & 0x10) {
                dvd->vol += dvd->volStep;
                if (dvd->vol >= dvd->volTarget) {
                    dvd->vol = dvd->volTarget;
                    dvd->volStep = 0.0f;
                    dvd->flags &= ~0x10;
                }
                SoundDVDSetPanCh(i, (u8)dvd->pan);
            }
        }
    }
}

void _ssDVDReadAsync_activeChk(s32 result, DVDFileInfo* fileInfo);
void cache_flush(u32 pointerToARQRequest);
void _ssDVDReadAsync_cache_aram(u32 pointerToARQRequest);
void _ssDVDReadAsync_cache_next(s32 result, DVDFileInfo* fileInfo);
void SoundSSPlayChEx_main(s32 result, DVDFileInfo* fileInfo);

inline void SoundIntOff(void) {
    if (!sound.interruptDepth) {
        sound.interrupts = OSDisableInterrupts();
    }
    sound.interruptDepth++;
}

inline void SoundIntOn(void) {
    if (sound.interruptDepth) {
        if (!--sound.interruptDepth) {
            OSRestoreInterrupts(sound.interrupts);
        }
    }
}

static inline u32 _ssAlign32(u32 x) {
    return (x + 0x1F) & ~0x1F;
}

static inline u32 _ssCalcLen(SoundStreamEntry* entry) {
    u32 len;
    int i;
    len = SS_LOAD_BUFFER_SIZE;
    if (entry->offset + SS_LOAD_BUFFER_SIZE > entry->size) {
        len = entry->size - entry->offset;
    } else {
        for (i = 2; i < 6; i++) {
            if (entry->offset + i * SS_LOAD_BUFFER_SIZE > entry->size) {
                len -= (SS_LOAD_BUFFER_SIZE - (entry->size - (entry->offset + (i - 1) * SS_LOAD_BUFFER_SIZE))) / i;
                len += 0x20 - (len % 0x20);
                break;
            }
        }
    }
    return len;
}

inline void _ssDVDReadAsync(SoundStream* stream, int ch) {
    SoundStreamEntry* entry;
    u32 extra;
    u32 len;
    u32 aligned;
    u32 addr;

    entry = &stream->entries[ch];
    extra = 0;
    if (ch) {
        extra = entry->size + (0x20 - (entry->size % 0x20));
    }
    aligned = _ssAlign32(len = _ssCalcLen(entry));
    if (!(stream->flags & 0x400)) {
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

void _ssDVDReadAsync_activeChk(s32 result, DVDFileInfo* fileInfo) {
    int i;
    SoundStream* stream;
    SoundStreamEntry* entry;
    int ch;

    SoundIntOff();
    stream = sound.streams;
    for (i = 0; i < 3; i++, stream++) {
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
                if (result < 0) {
                    goto end;
                }
                entry->offset += entry->requestLength;
                entry->loadedLen[1 - entry->curBuf] = entry->requestLength;
                entry->loaded = 1;
                if (entry->offset >= entry->size) {
                    entry->offset = entry->loopStart;
                    if (stream->flags & 0x200) {
                        if (ch == 0) {
                            stream->flags &= ~0x1000;
                        } else {
                            stream->flags &= ~0x2000;
                        }
                    } else {
                        stream->flags |= 0x4000;
                    }
                } else if (ch == 0) {
                    stream->flags &= ~0x1000;
                } else {
                    stream->flags &= ~0x2000;
                }
                if (stream->count == 2) {
                    if (sound.coverState == 2 && stream->entries[0].loaded == 1 && stream->entries[1].loaded == 1) {
                        sound.coverState = 0;
                    }
                } else if (sound.coverState == 2 && stream->entries[0].loaded == 1) {
                    sound.coverState = 0;
                }
                break;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        SoundStream* stream = &sound.streams[i];
        if (stream->flags && !(stream->flags & 8)) {
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
            sndStreamActivate(stream->entries[0].streamId);
            if (stream->count == 2) {
                sndStreamActivate(stream->entries[1].streamId);
            }
            SoundSSSetPanCh(i, stream->pan);
            stream->flags |= 8;
            break;
        }
    }
end:
    SoundIntOn();
}

void cache_flush(u32 pointerToARQRequest) {
    _ssDVDReadAsync_activeChk(0, &((SoundStreamEntry*)((ARQRequest*)pointerToARQRequest)->owner)->dvd->info);
}

void _ssDVDReadAsync_cache_aram(u32 pointerToARQRequest) {
    u32 len;
    SoundStream* stream;

    stream = &sound.streams[(u8)((ARQRequest*)pointerToARQRequest)->owner];
    if (stream->entries[0].pos >= DVDMgrGetLength(stream->entries[0].dvd)) {
        stream->entries[0].offset = 0;
        stream->entries[0].pos = 0;
        stream->entries[1].offset = 0;
        stream->entries[1].pos = 0;
        stream->entries[0].curBuf = 1;
        _ssDVDReadAsync(stream, 0);
        if (stream->count == 2) {
            stream->entries[1].curBuf = 1;
            _ssDVDReadAsync(stream, 1);
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

void _ssDVDReadAsync_cache_next(s32 result, DVDFileInfo* fileInfo) {
    static u32 owner_no;
    SoundStream* stream;
    u32 dest;
    int i;

    stream = sound.streams;
    for (i = 0; i < 3; stream++, i++) {
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

s16 dtbl[16] = {0x0001, 0x0007, 0x0018, 0x003B, 0x0053, 0x006F, 0x00EC, 0x022B,
                0x02E6, 0x0375, 0x041C, 0x08AD, 0x0CD5, 0x1150, 0x1B32, 0x2454};

u8* ssDecodeDPCM(SoundStream* stream, SoundStreamEntry* entry, s16* out, u32 count, u8* src) {
    s8 delta;
    u32 b;
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
            if ((s32)(b & 0xF) == 8) {
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
            if (entry->loaded == 0 && !(stream->flags & 0x4000)) {
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
            if (!(stream->flags & 0x200)) {
                stream->flags |= 4;
                break;
            }
        }
    }
    return src;
}
#ifdef VERSION_G8ME01
u32 _sscallback(void* p_b1, u32 p_l1, void* p_b2, u32 p_l2, u32 user) {
    u32 len1;
#else
u32 _sscallback(void* p_b1, u32 len1, void* p_b2, u32 p_l2, u32 user) {
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

    // FAKE: len2/buffer1/buffer2 are copied from the params into locals of a different type (unsigned int, s16*) so
    // IRO cannot propagate the copies; the params then coalesce into r3/r5/r6 and these statement copies give the
    // target order len1 (param web, after the stream add) < len2 < buffer1 < buffer2 (buffer1 lands in r6).
#ifdef VERSION_G8ME01
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
    stream->samplePos = 0;
    if ((stream->flags & 0x8006) || sound.coverState) {
        if (!(stream->flags & 0x100)) {
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
    if (stream->flags & 0x80) {
        ssDecodeDPCM(stream, entry, buffer2, len2,
                     ssDecodeDPCM(stream, entry, buffer1, len1, entry->loadBuffer[entry->curBuf] + entry->srcPos));
    } else if (stream->flags & 0x100) {
        dst = buffer1; // FAKE: dst (not a u8* dst8) for the byte copies, so dst's split webs number above i/n
        src8 = entry->loadBuffer[entry->curBuf]; src8 = src8 + entry->srcPos;
        n = ((len1 + 13) / 14) * 8;
        while (n != 0) {
            *(((u8*)dst)++) = *(src8++);
            n--;
            entry->srcPos++;
#ifdef VERSION_G8ME01
            entry->pos++;
#endif
            if (entry->srcPos >= entry->loadedLen[entry->curBuf]) {
                if (entry->loaded == 0 && !(stream->flags & 0x4000)) {
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
#ifndef VERSION_G8ME01
            entry->pos++;
#endif
            if (entry->pos >= stream->loopEnd) {
                entry->pos = stream->loopStart;
                if (!(stream->flags & 0x200)) {
                    stream->flags |= 4;
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
            *(((u8*)dst)++) = *(src8b++);
            n--;
            entry->srcPos++;
#ifdef VERSION_G8ME01
            entry->pos++;
#endif
            if (entry->srcPos >= entry->loadedLen[entry->curBuf]) {
                if (entry->loaded == 0 && !(stream->flags & 0x4000)) {
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
#ifndef VERSION_G8ME01
            entry->pos++;
#endif
            if (entry->pos >= stream->loopEnd) {
                entry->pos = stream->loopStart;
                if (!(stream->flags & 0x200)) {
                    stream->flags |= 4;
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
                if (entry->loaded == 0 && !(stream->flags & 0x4000)) {
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
                if (!(stream->flags & 0x200)) {
                    stream->flags |= 4;
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
                if (entry->loaded == 0 && !(stream->flags & 0x4000)) {
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
                if (!(stream->flags & 0x200)) {
                    stream->flags |= 4;
                    break;
                }
            }
        }
    }
    return stream->samplePos;
}

void SoundSSPlayChEx(s32 chan, const char* name, s32 aram);

void SoundSSPlayCh(s32 chan, const char* name) {
    SoundSSPlayChEx(chan, name, 0);
}

void SoundSSPlayChEx(s32 chan, const char* name, s32 aram) {
    SoundStream* stream;
    int i;
    const char* paths [2]; // FAKE: alias array needed for 100% (a plain str/root form is 5 rows off)

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
    stream->flags = 1;
    if (aram) {
        stream->flags |= 0x400;
    }
    paths [1] = getMarioStDvdRoot ();
    sprintf(str, "%s/%s%s", paths [1], name, ".stm");
    if (!(stream->entries[0].dvd = DVDMgrOpen(paths [0] = str, 0, 0))) {
        stream->flags = 0;
        return;
    }
    if (!(stream->entries[1].dvd = DVDMgrOpen(paths [0], 0, 0))) {
        stream->flags = 0;
        return;
    }
    DVDMgrReadAsync(stream->entries[0].dvd, stream->entries[0].buffer, 0x100, 0, SoundSSPlayChEx_main);
}

void SoundSSPlayChEx_main(s32 result, DVDFileInfo* fileInfo) {
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
        for (i = 0; i < 3; i++) {
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
            stream->flags |= 0x200;
        } else {
            stream->loopStart = 0;
        }
        switch (type) {
            case 0:
                stream->entries[0].size = stream->loopEnd;
                stream->entries[1].size = stream->loopEnd;
                stream->entries[0].loopStart = stream->loopStart;
                stream->entries[1].loopStart = stream->loopStart;
                stream->dataOffset = 0x40;
                break;
            case 1:
                stream->flags |= 0x80;
                stream->entries[0].hist = stream->entries[0].initHist;
                stream->entries[1].hist = stream->entries[1].initHist;
                stream->dataOffset = 0x40;
                break;
            case 2:
                stream->flags |= 0x100;
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
        if (!(stream->flags & 0x400)) {
            stream->entries[0].curBuf = 1;
            _ssDVDReadAsync(stream, 0);
            stream->flags |= 0x1000;
            if (stream->count == 2) {
                stream->entries[1].curBuf = 1;
                _ssDVDReadAsync(stream, 1);
                stream->flags |= 0x2000;
            }
        }
        size = sndStreamAllocLength(0xE00, 0);
        { u8* p = (u8*)stream->entries[0].buffer; u32 n; for (n = size; n != 0; n--) { *p++ = 0; } }
        if (stream->count == 2) {
            { u8* p = (u8*)stream->entries[1].buffer; u32 n; for (n = size; n != 0; n--) { *p++ = 0; } }
        }
        pan = 0;
        if (stream->count == 1) {
            pan = 0x40;
        }
        if (!(stream->flags & 0x100)) {
            stream->entries[0].streamId = sndStreamAllocEx(0xFF, stream->entries[0].buffer, 0xE00, stream->freq, 0, pan,
                                                           0, 0, 0, 0, 0x10000, _sscallback, i, NULL);
            if (stream->count == 2) {
                stream->entries[1].streamId = sndStreamAllocEx(0xFF, stream->entries[1].buffer, 0xE00, stream->freq, 0,
                                                               0x7F, 0, 0, 0, 0, 0x10000, _sscallback, i | 0x100, NULL);
            }
            stream->flags |= 8;
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
        stream->fadeStep = stream->vol;
        stream->flags |= 0x10;
        SoundSSSetPanCh(i, stream->pan);
        if (stream->flags & 0x400) {
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

void SoundSSStopCh(s32 chan) {
    SoundStream* stream;

    if (sound.streams[chan].flags) {
        stream = &sound.streams[chan];
        SoundIntOff();
        if (stream->flags & 0x400) {
            sound.slotUsed[stream->slot] = 0;
        }
        stream->flags = 0;
        DVDMgrClose(stream->entries[0].dvd);
        DVDMgrClose(stream->entries[1].dvd);
        sndStreamFree(stream->entries[0].streamId);
        if (stream->count == 2) {
            sndStreamFree(stream->entries[1].streamId);
        }
        SoundIntOn();
        sound.streamChannel = -1;
    }
}

void SoundSSFadeoutCh(s32 chan) {
    SoundStream* stream = &sound.streams[chan];
    f32 frames;

    if (stream->flags) {
#ifdef VERSION_G8ME01
        frames = sound.fadeOut / (1000.0f / gp->framerate);
#else
        frames = sound.fadeOut / 16.666666f;
#endif
        if (frames < 1.0f) {
            frames = 1.0f;
        }
        stream->fadeStep = stream->curVol / frames;
        stream->flags |= 0x20;
    }
}

void SoundSSContinueCh(s32 chan) {
    SoundStream* stream = &sound.streams[chan];

    if (stream->flags) {
        stream->flags &= ~2;
        stream->fadeStep = stream->vol;
        stream->flags |= 0x10;
    }
}

void SoundSSSetPanCh(s32 chan, u8 pan) {
    SoundStream* stream;
    f32 left, right;

    if (sound.streams[chan].flags) {
        stream = &sound.streams[chan];
        stream->pan = pan;
        if (stream->flags & 8) {
            if (stream->count == 1) {
                sndStreamMixParameter(stream->entries[0].streamId, stream->curVol, pan, stream->span, 0);
            } else {
                left = stream->curVol;
                right = left;
                if (pan < 64) {
                    right = (((f32)pan * stream->curVol) / 63.0f);
                }
                if (!((u32)(pan - 64) >> 31)) {
                    left = (((f32)(127 - pan) * stream->curVol) / 63.0f);
                }
                sndStreamMixParameter(stream->entries[0].streamId, left, 0, stream->span, 0);
                sndStreamMixParameter(stream->entries[1].streamId, right, 0x7F, stream->span, 0);
            }
        }
    }
}

void SoundSSSetVolCh(s32 chan, u8 vol) {
    SoundStream* stream;

    if (sound.streams[chan].flags) {
        stream = &sound.streams[chan];
        stream->vol = stream->curVol = vol;
        if (stream->flags & 8) {
            SoundSSSetPanCh(chan, stream->pan);
        }
    }
}

s32 SoundSSGetVolCh(s32 chan) {
    SoundStream* stream;

    stream = &sound.streams[chan];
    if (stream->flags) {
        return (s32)stream->vol;
    } else {
        return 0;
    }
}

void SoundSSSetSrndPanCh(s32 chan, u8 span) {
    SoundStream* stream = &sound.streams[chan];

    if (stream->flags) {
        stream->span = span;
        if (stream->flags & 8) {
            SoundSSSetPanCh(chan, stream->pan);
        }
    }
}

void SoundSSSetPlayFreqCh(s32 chan, u16 freq) {
    SoundStream* stream = &sound.streams[chan];

    if (stream->flags) {
        stream->freq = freq;
        if (stream->flags & 8) {
            SoundIntOff();
            sndStreamFrq(stream->entries[0].streamId, stream->freq);
            if (stream->count == 2) {
                sndStreamFrq(stream->entries[1].streamId, stream->freq);
            }
            SoundIntOn();
        }
    }
}

u32 SoundSSCheck(s32 chan) {
    return sound.streams[chan].flags;
}

void SoundSSMuteOn(s32 chan) {
    if (sound.streams[chan].flags) {
        sound.streams[chan].flags |= 2;
    }
}

void SoundSSMuteOff(s32 chan) {
    if (sound.streams[chan].flags) {
        sound.streams[chan].flags &= ~2;
    }
}

void SoundSSMain(void) {
    u32 i;
    SoundStream* stream;

    for (i = 0; (int)i < 3; i++) {
        stream = &sound.streams[i];
        if ((stream->flags & 8) && !(stream->flags & 2)) {
            if (stream->flags & 4) {
                stream->flags &= ~4;
                SoundSSStopCh(i);
                continue;
            }
            if (stream->flags & 0x40) {
                if (stream->curVol == 0.0f) {
                    stream->flags |= 2;
                    stream->flags &= ~0x40;
                    continue;
                }
                stream->curVol -= stream->fadeStep;
                if (stream->curVol <= 0.0f) {
                    stream->curVol = 0.0f;
                }
                SoundSSSetPanCh(i, stream->pan);
            }
            if (stream->flags & 0x20) {
                if (stream->curVol == 0.0f) {
                    stream->flags &= ~0x20;
                    SoundSSStopCh(i);
                    continue;
                }
                stream->curVol -= stream->fadeStep;
                if (stream->curVol <= 0.0f) {
                    stream->curVol = 0.0f;
                }
                SoundSSSetPanCh(i, stream->pan);
            }
            if (stream->flags & 0x10) {
                stream->curVol += stream->fadeStep;
                if (stream->curVol >= stream->vol) {
                    stream->curVol = stream->vol;
                    stream->fadeStep = 0.0f;
                    stream->flags &= ~0x10;
                }
                SoundSSSetPanCh(i, stream->pan);
            }
        }
    }
}

void SoundSSMainInt(void) {
    int i;
    SoundStream* stream;

    for (i = 0; i < 3; i++) {
        stream = &sound.streams[i];
#ifdef VERSION_G8ME01
        if ((stream->flags & 8) && !(stream->flags & 2)) {
            if (stream->flags & 1) {
                if (stream->entries[0].srcPos != 0 && stream->entries[0].loaded == 0 && !(stream->flags & 0x1000)) {
                    _ssDVDReadAsync(stream, 0);
                    stream->flags |= 0x1000;
                    if (stream->flags & 0x400) {
                        stream->entries[0].loaded = 1;
                    }
                }
                if (stream->count == 2 && stream->entries[1].srcPos != 0 && stream->entries[1].loaded == 0 &&
                    !(stream->flags & 0x2000)) {
                    _ssDVDReadAsync(stream, 1);
                    stream->flags |= 0x2000;
                    if (stream->flags & 0x400) {
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
                if (sound.coverState == 2 && stream->entries[0].loaded == 1 && stream->entries[1].loaded == 1) {
                    sound.coverState = 0;
                }
            } else if (sound.coverState == 2 && stream->entries[0].loaded == 1) {
                sound.coverState = 0;
            }
        }
#else
        if ((stream->flags & 8) && !(stream->flags & 2) && (stream->flags & 1)) {
            if (stream->entries[0].srcPos != 0 && stream->entries[0].loaded == 0 && !(stream->flags & 0x1000)) {
                _ssDVDReadAsync(stream, 0);
                stream->flags |= 0x1000;
                if (stream->flags & 0x400) {
                    stream->entries[0].loaded = 1;
                }
            }
            if (stream->count == 2 && stream->entries[1].srcPos != 0 && stream->entries[1].loaded == 0 &&
                !(stream->flags & 0x2000)) {
                _ssDVDReadAsync(stream, 1);
                stream->flags |= 0x2000;
                if (stream->flags & 0x400) {
                    stream->entries[1].loaded = 1;
                }
            }
        }
#endif
    }
}

void SoundAIDMACallback(void) {
    s32 length;
    u8* address;
    u16* dst;
#ifdef VERSION_G8ME01
    s32 n;
    s16* out;
    s16* src;
#else
    s16* src;
    s32 n;
    s16* out;
#endif

#ifdef VERSION_G8ME01
    if (sound.callback != NULL) {
        sound.callback();
    }
    DCFlushRange(sound.dmaAddress, sound.dmaLength);
    address = sound.dmaAddress;
    length = sound.dmaLength;
#else
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


