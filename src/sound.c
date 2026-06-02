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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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


u8 SoundSSPlayChEx_main(void) {
    return 0;
}


int _sscallback(void* param_1, u32 param_2, void* param_3, u32 param_4, u32 param_5) {
    return 0;
}


u8 SoundSSMain(void) {
    return 0;
}


u8 SoundInit(void) {
    return 0;
}


void SoundDropData(void) {
}


void SoundSSMainInt(void) {
}


void _ssDVDReadAsync_activeChk(s32 zero, void* ptr) {
}


u8 SoundDVDMain(void) {
    return 0;
}


s32 SoundSLibLoadDVD(s32 param_1) {
    return 0;
}


u8 _ssDVDReadAsync_cache_aram(int param_1) {
    return 0;
}


u8 SoundEfxMain(void) {
    return 0;
}


s32 SoundLoadDVD2(s32 param_1) {
    return 0;
}


u32 SoundEfxPlayEx(s32 param_1, u16 param_2, u32 param_3, u32 param_4) {
    return 0;
}


void SoundSSPlayChEx(s32 a, s32 b, s32 c) {
}


void* ssDecodeDPCM(void* param_1, int param_2, void* param_3, int param_4, void* param_5) {
    return 0;
}


u8 SoundSSSetVolCh(void) {
    return 0;
}


u8 SoundSSSetSrndPanCh(void) {
    return 0;
}


u8 SoundSSSetPanCh(void) {
    return 0;
}


u8 SoundCloseCover(void) {
    return 0;
}


s32 SoundLoadDVD2PushGroup(void* param_1) {
    return 0;
}


u8 SoundAIDMACallback(void) {
    return 0;
}


u8 SoundSSStopCh(int param_1) {
    return 0;
}


u8 _ssDVDReadAsync_cache_next(int param_1, int param_2) {
    return 0;
}


u8 SoundSongPlayCh(int param_1, u32 param_2) {
    return 0;
}


u8 SoundEfxSetSrndPan(int param_1, u32 param_2) {
    return 0;
}


u8 SoundSSSetPlayFreqCh(int param_1, u16 param_2) {
    return 0;
}


u8 SoundEfxSetVolume(int param_1, u32 param_2) {
    return 0;
}


u8 SoundEfxSetLPF(int param_1, short param_2) {
    return 0;
}


u8 SoundMain(void) {
    return 0;
}


u8 SoundSSFadeoutCh(int param_1) {
    return 0;
}


s32 SoundEfxCheck(int param_1) {
    return 0;
}


s32 loadDVD_callback(u32 param_1, int param_2) {
    return 0;
}


u8 SoundEfxSetPan(int param_1, u32 param_2) {
    return 0;
}


u8 SoundSongSetVolCh(int param_1, u8 param_2) {
    return 0;
}


u8 SoundEfxStop(int param_1) {
    return 0;
}


u8 SoundLoadDVD2Free(void) {
    return 0;
}
