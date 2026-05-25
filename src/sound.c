#include "sound.h"

void SoundSetFadeTime(s16 a, s16 b) {
    extern char sound[];

    *(s16*)(sound + 0x208) = a;
    *(s16*)(sound + 0x20A) = b;
}

u16 SoundSSCheck(s32 index) {
    extern char sound[];
    void* table = *(void**)(sound + 0x100);

    return *(u16*)((s32)table + index * 0x138);
}

s32 SoundEfxGetVolume(s32 index) {
    extern char sound[];
    void* table = *(void**)(sound + 0xF4);

    return *(u16*)((s32)table + index * 0x88 + 4) & 0xFF;
}

void SoundMainInt(void) {
    extern void SoundSSMainInt(void);

    SoundSSMainInt();
}

void SoundOpenCover(void) {
    extern char sound[];

    if (*(u16*)(sound + 0x214) != 0) {
        return;
    }
    *(u16*)(sound + 0x214) = 1;
}

void SoundSSMuteOff(s32 index) {
    extern char sound[];
    void* table = *(void**)(sound + 0x100);
    u16* flags = (u16*)((s32)table + index * 0x138);

    if (*flags == 0) {
        return;
    }
    *flags &= ~2;
}

void SoundSSMuteOn(s32 index) {
    extern char sound[];
    void* table = *(void**)(sound + 0x100);
    u16* flags = (u16*)((s32)table + index * 0x138);

    if (*flags == 0) {
        return;
    }
    *flags |= 2;
}

void SoundSetOutputMode(s32 mode) {
    extern char sound[];
    extern void sndOutputMode(s32 mode);

    if (*(s32*)(sound + 0x218) != mode) {
        *(s32*)(sound + 0x218) = mode;
        sndOutputMode(mode);
    }
}

void* sndMalloc(s32 size) {
    extern void* __memAlloc(s32 heap, s32 size);

    return __memAlloc(0, size);
}

void sndFree(void* ptr) {
    extern void __memFree(s32 heap, void* ptr);

    __memFree(0, ptr);
}

void cache_flush(void* ptr) {
    extern void _ssDVDReadAsync_activeChk(s32 zero, void* ptr);

    _ssDVDReadAsync_activeChk(0, (void*)(**(s32**)((s32)ptr + 4) + 0x40));
}

void SoundSSPlayCh(s32 a, s32 b) {
    extern void SoundSSPlayChEx(s32 a, s32 b, s32 c);

    SoundSSPlayChEx(a, b, 0);
}

void SoundSSContinueCh(s32 index) {
    extern char sound[];
    void* entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);
    u16 flags = *(u16*)entry;

    if (flags == 0) {
        return;
    }
    *(u16*)entry = flags & ~2;
    *(f32*)((s32)entry + 0x14) = *(f32*)((s32)entry + 0xC);
    *(u16*)entry |= 0x10;
}

s32 SoundSongCheck(s32 index) {
    extern char sound[];
    extern s32 sndSeqGetValid(void* seq);
    void* seq = *(void**)((s32)*(void**)(sound + 0xF0) + index * 0x10);
    s32 valid = sndSeqGetValid(seq);

    return ((u32)(-valid) | (u32)valid) >> 31;
}

s32 SoundSSGetVolCh(s32 index) {
    extern char sound[];
    void* entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);

    if (*(u16*)entry != 0) {
        return *(f32*)((s32)entry + 0xC);
    }
    return 0;
}

s32 SoundSongGetVolCh(s32 index) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    void* entry = (void*)((s32)*(void**)(sound + 0xF0) + index * 0x10);

    if (sndSeqGetValid(*(void**)entry) == 0) {
        return 0;
    }
    return *(u32*)((s32)entry + 4) & 0xFF;
}

void SoundSongStopCh(s32 index) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    extern void sndSeqStop(void* seq);
    void** table = *(void***)(sound + 0xF0);
    s32 offset = index * 0x10;

    if (sndSeqGetValid(*(void**)((s32)table + offset)) != 0) {
        sndSeqStop(*(void**)((s32)table + offset));
        *(s32*)((s32)table + offset) = -1;
    }
}

void SoundSongContinueCh(s32 index) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    extern void sndSeqContinue(void* seq);
    extern void sndSeqVolume(s32 volume, s32 frames, void* seq, s32 flag);
    void* entry = (void*)((s32)*(void**)(sound + 0xF0) + index * 0x10);

    if (sndSeqGetValid(*(void**)entry) != 0) {
        sndSeqContinue(*(void**)entry);
        sndSeqVolume(*(u32*)((s32)entry + 4) & 0xFF, 0, *(void**)entry, 0);
    }
}

void SoundSongFadeinCh(s32 index) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    extern void sndSeqVolume(s32 volume, s32 frames, void* seq, s32 flag);
    void* entry = (void*)((s32)*(void**)(sound + 0xF0) + index * 0x10);

    if (sndSeqGetValid(*(void**)entry) != 0) {
        sndSeqVolume(*(u32*)((s32)entry + 4) & 0xFF, *(u16*)(sound + 0x208), *(void**)entry, 0);
    }
}

void SoundSongFadeoutCh(s32 index) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    extern void sndSeqVolume(s32 volume, s32 frames, void* seq, s32 flag);
    void** table = *(void***)(sound + 0xF0);
    s32 offset = index * 0x10;

    if (sndSeqGetValid(*(void**)((s32)table + offset)) != 0) {
        sndSeqVolume(0, *(u16*)(sound + 0x20A), *(void**)((s32)table + offset), 1);
        *(s32*)((s32)table + offset) = -1;
    }
}

void SoundEfxSetAux1(s32 index, s32 value) {
    extern char sound[];
    extern s32 sndEmitterVoiceID(void* emitter);
    extern s32 sndFXCheck(s32 id);
    extern void sndFXCtrl(s32 id, s32 ctrl, s32 value);
    void* entry = (void*)((s32)*(void**)(sound + 0xF4) + index * 0x88);
    s32 id;

    if ((*(u16*)entry & 0x10) != 0) {
        id = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
    } else {
        id = sndFXCheck(*(s32*)((s32)entry + 0xC));
    }
    if ((u32)(id + 0x10000) != 0xFFFF) {
        sndFXCtrl(id, 0x5B, value);
    }
}

void SoundEfxSetPitch(s32 index, u32 value) {
    extern char sound[];
    extern s32 sndEmitterVoiceID(void* emitter);
    extern s32 sndFXCheck(s32 id);
    extern void sndFXCtrl14(s32 id, s32 ctrl, s32 value);
    void* entry = (void*)((s32)*(void**)(sound + 0xF4) + index * 0x88);
    s32 id;

    if ((*(u16*)entry & 0x10) != 0) {
        id = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
    } else {
        id = sndFXCheck(*(s32*)((s32)entry + 0xC));
    }
    if ((u32)(id + 0x10000) != 0xFFFF) {
        sndFXCtrl14(id, 0x80, (u16)(value + 0x2000));
    }
}
