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


#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSSSetPanCh(s32 index, u8 pan) {
    extern char sound[];
    extern void sndStreamMixParameter(void* stream, s32 volume, s32 pan, s32 aux, s32 zero);
    void* entry;
    u32 pan8;
    f32 left;
    f32 right;

    entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);
    if (*(u16*)entry != 0) {
        pan8 = (u8)pan;
        *(u16*)((s32)entry + 6) = pan8;
        if ((*(u16*)entry & 8) != 0) {
            if (*(u16*)((s32)entry + 0x1A) == 1) {
                sndStreamMixParameter(*(void**)((s32)entry + 0x4C), (s32)*(f32*)((s32)entry + 0x10), pan, *(u16*)((s32)entry + 8) & 0xFF, 0);
            } else {
                right = *(f32*)((s32)entry + 0x10);
                left = right;
                if (pan8 < 0x40) {
                    left = ((f32)pan8 * right) / 63.0f;
                }
                if ((u8)pan >= 0x40) {
                    right = ((f32)(0x7F - (u8)pan) * right) / 63.0f;
                }
                sndStreamMixParameter(*(void**)((s32)entry + 0x4C), (s32)right, 0, *(u16*)((s32)entry + 8) & 0xFF, 0);
                sndStreamMixParameter(*(void**)((s32)entry + 0xD4), (s32)left, 0x7F, *(u16*)((s32)entry + 8) & 0xFF, 0);
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void SoundCloseCover(void) {
    extern char sound[];
    extern s32 sndIsInstalled(void);
    void* table;
    u16 flags;
    s32 active;

    if (sndIsInstalled() == 0) {
        return;
    }
    if (*(u16*)(sound + 0x214) != 1) {
        return;
    }

    table = *(void**)(sound + 0x100);
    active = 0;

    flags = *(u16*)table;
    if ((flags & 8) != 0 && (flags & 0x400) == 0 && (flags & 2) == 0) {
        if ((flags & 0x1000) != 0) {
            active = 1;
        }
        if (*(u16*)((s32)table + 0x1A) == 2 && (flags & 0x2000) != 0) {
            active = 1;
        }
    }

    flags = *(u16*)((s32)table + 0x138);
    if ((flags & 8) != 0 && (flags & 0x400) == 0 && (flags & 2) == 0) {
        if ((flags & 0x1000) != 0) {
            active = 1;
        }
        if (*(u16*)((s32)table + 0x152) == 2 && (flags & 0x2000) != 0) {
            active = 1;
        }
    }

    flags = *(u16*)((s32)table + 0x270);
    if ((flags & 8) != 0 && (flags & 0x400) == 0 && (flags & 2) == 0) {
        if ((flags & 0x1000) != 0) {
            active = 1;
        }
        if (*(u16*)((s32)table + 0x28A) == 2 && (flags & 0x2000) != 0) {
            active = 1;
        }
    }

    if (active != 0) {
        *(u16*)(sound + 0x214) = 2;
    } else {
        *(u16*)(sound + 0x214) = 0;
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 SoundLoadDVD2PushGroup(u8* data) {
    extern char sound[];
    extern s32 sndPushGroup(void* group, u16 id, s32 zero, void* aux, void* base);
    u32 group;
    u32 count;
    char* groupBase;
    u32 result;

    group = *(u32*)(sound + 0xE0);
    if (group >= 8) {
        return 0;
    }
    *(s32*)(sound + 0xC0 + (group << 2)) = 0;
    if (data != 0) {
        while (*data != 0xFF) {
            group = *(u32*)(sound + 0xE0);
            sndPushGroup(*(void**)(sound + 0x20 + (group << 2)), *data, 0, *(void**)(sound + 0x40 + (group << 2)), *(void**)(sound + (group << 2)));
            data++;
            group = *(u32*)(sound + 0xE0);
            groupBase = sound + (group << 2);
            *(s32*)(groupBase + 0xC0) = *(s32*)(groupBase + 0xC0) + 1;
        }
    } else {
        count = *(u32*)(sound + 0xE8);
        while ((s32)count < 0xFF) {
            group = *(u32*)(sound + 0xE0);
            result = sndPushGroup(*(void**)(sound + 0x20 + (group << 2)), (u16)count, 0, *(void**)(sound + 0x40 + (group << 2)), *(void**)(sound + (group << 2)));
            if (result != 1) {
                break;
            }
            count++;
            group = *(u32*)(sound + 0xE0);
            groupBase = sound + (group << 2);
            *(s32*)(groupBase + 0xC0) = *(s32*)(groupBase + 0xC0) + 1;
            *(u32*)(sound + 0xE8) = *(u32*)(sound + 0xE8) + 1;
        }
    }
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundAIDMACallback(void) {
    extern char sound[];
    extern void DCFlushRange(void* addr, u32 len);
    void (*callback)(void);
    u8* src;
    s16* temp;
    s16* out;
    s32 remaining;
    u32 write;
    u16 bank;

    callback = *(void (**)(void))(sound + 0x5BC);
    if (callback != 0) {
        callback();
    }
    DCFlushRange(*(void**)(sound + 0x334), *(u32*)(sound + 0x338));
    src = *(u8**)(sound + 0x334);
    remaining = *(s32*)(sound + 0x338);
    temp = (s16*)(sound + 0x33C);
    while (remaining != 0) {
        *temp = (src[0] << 8) | src[1];
        src += 2;
        temp++;
        remaining -= 2;
    }
    temp = (s16*)(sound + 0x33C);
    remaining = *(u32*)(sound + 0x338) >> 1;
    out = (s16*)(sound + 0x5C0 + (*(u16*)(sound + 0x330) << 11) + (*(u32*)(sound + 0x15C0) << 1));
    while (remaining != 0) {
        *out = *temp;
        temp += 2;
        out++;
        write = *(u32*)(sound + 0x15C0) + 1;
        *(u32*)(sound + 0x15C0) = write;
        if (write >= 0x400) {
            *(u32*)(sound + 0x15C0) = 0;
            bank = 1 - *(u16*)(sound + 0x330);
            *(u16*)(sound + 0x330) = bank;
            out = (s16*)(sound + 0x5C0 + (bank << 11));
        }
        remaining -= 2;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSSStopCh(s32 index) {
    extern char sound[];
    extern s32 OSDisableInterrupts(void);
    extern void OSRestoreInterrupts(s32 level);
    extern void DVDMgrClose(void* handle);
    extern void sndStreamFree(void* stream);
    void* entry;
    u32 streamIndex;
    char* base;
    u16 zero;

    entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);
    if (*(u16*)entry != 0) {
        if (*(u16*)(sound + 0x20C) == 0) {
            *(s32*)(sound + 0x210) = OSDisableInterrupts();
        }
        *(u16*)(sound + 0x20C) = *(u16*)(sound + 0x20C) + 1;
        if ((*(u16*)entry & 0x400) != 0) {
            streamIndex = *(u16*)((s32)entry + 2);
            base = sound;
            zero = 0;
            streamIndex <<= 1;
            *(u16*)((s32)base + streamIndex + 0x226) = zero;
        }
        *(u16*)entry = 0;
        DVDMgrClose(*(void**)((s32)entry + 0x28));
        DVDMgrClose(*(void**)((s32)entry + 0xB0));
        sndStreamFree(*(void**)((s32)entry + 0x4C));
        if (*(u16*)((s32)entry + 0x1A) == 2) {
            sndStreamFree(*(void**)((s32)entry + 0xD4));
        }
        if (*(u16*)(sound + 0x20C) != 0) {
            *(u16*)(sound + 0x20C) = *(u16*)(sound + 0x20C) - 1;
            if (*(u16*)(sound + 0x20C) == 0) {
                OSRestoreInterrupts(*(s32*)(sound + 0x210));
            }
        }
        *(s32*)(sound + 0x104) = -1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void _ssDVDReadAsync_cache_next(s32 result, void* fileInfo) {
    extern char sound[];
    extern void DCFlushRange(void* addr, u32 len);
    extern void ARQPostRequest(void* request, u32 owner, u32 type, u32 priority, u32 source, u32 dest, u32 length, void* callback);
    extern void _ssDVDReadAsync_cache_aram(s32 value);
    static u32 owner_no;
    u8* entry;
    char* soundBase;
    s32 i;
    u32 owner;
    u32 dest;
    u32 size;
    u32 sum;
    u32 streamIndex;

    entry = *(u8**)(sound + 0x100);
    i = 0;
    while (i < 3) {
        if (*(u16*)entry != 0 && (void*)(*(s32*)(entry + 0x28) + 0x40) == fileInfo) {
            if (result >= 0) {
                size = *(u32*)(entry + 0x70);
                soundBase = sound;
                sum = *(u32*)(entry + 0x5C) + size;
                *(u32*)(entry + 0x5C) = sum;
                streamIndex = *(u16*)(entry + 2);
                dest = *(u32*)(entry + 0x60) + *(u32*)(soundBase + 0x21C + (streamIndex << 2));
                DCFlushRange(*(void**)(entry + 0x54), 0x8000);
                owner = owner_no;
                owner_no = owner + 1;
                ARQPostRequest(entry + 0x2C, (owner << 16) | i, 0, 0, *(u32*)(entry + 0x54), dest, 0x8000, _ssDVDReadAsync_cache_aram);
                *(u32*)(entry + 0x60) = *(u32*)(entry + 0x60) + *(u32*)(entry + 0x70);
            }
            break;
        }
        entry += 0x138;
        i++;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSongPlayCh(s32 index, u32 value) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    extern void sndSeqStop(void* seq);
    extern s32 sndSeqPlayEx(s32 a, s32 b, void* seq, s32 c, s32 d);
    extern void sndSeqVolume(s32 volume, s32 frames, void* seq, s32 flag);
    extern void sndSeqContinue(void* seq);
    extern void sndSeqMute(void* seq, s32 mute, s32 flag);
    void* entry;
    void* node;
    u32 id;

    entry = (void*)((s32)*(void**)(sound + 0xF0) + (index << 4));
    if (sndSeqGetValid(*(void**)entry) == 1) {
        sndSeqStop(*(void**)entry);
    }
    *(s32*)((s32)entry + 4) = 0x7F;

    id = (u8)value;
    node = *(void**)(sound + 0xEC);
    if (node != NULL) {
        do {
            if (*(u16*)((s32)node + 2) == id) {
                break;
            }
            node = *(void**)((s32)node + 0xC);
        } while (node != NULL);
    } else {
        node = NULL;
    }

    if (node != NULL) {
        *(void**)((s32)entry + 8) = node;
        *(u16*)((s32)node + 4) = (value >> 16) & 0xFF;
        *(u16*)((s32)node + 6) = (value >> 8) & 0xFF;
        *(s32*)((s32)entry + 0xC) = -1;
        *(s32*)entry = sndSeqPlayEx(*(u16*)((s32)node + 4), *(u16*)((s32)node + 6), *(void**)((s32)node + 8), 0, 0);
        sndSeqVolume(*(u32*)((s32)entry + 4) & 0xFF, 0, *(void**)entry, 0);
        sndSeqContinue(*(void**)entry);
        sndSeqMute(*(void**)entry, *(s32*)((s32)entry + 0xC), 0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundEfxSetSrndPan(s32 index, u32 value) {
    extern char sound[];
    extern s32 sndEmitterVoiceID(void* emitter);
    extern s32 sndFXCheck(s32 id);
    extern void sndFXCtrl(s32 id, s32 ctrl, s32 value);
    extern void sndFXCtrl14(s32 id, s32 ctrl, u16 value);
    s32 offset;
    void* entry;
    s32 id;
    u32 pan;
    s32 span;

    offset = index * 0x88;
    *(u16*)((s32)*(void**)(sound + 0xF4) + offset + 8) = (u8)value;
    entry = (void*)((s32)*(void**)(sound + 0xF4) + offset);
    if ((*(u16*)entry & 0x10) != 0) {
        id = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
    } else {
        id = sndFXCheck(*(s32*)((s32)entry + 0xC));
    }
    if ((u32)(id + 0x10000) != 0xFFFF) {
        sndFXCtrl(id, 0x83, value);
        pan = (u8)value;
        if (pan == 0) {
            span = 0x2000;
        } else if (pan < 0x40) {
            span = 0x2000 - ((0x40 - pan) * 3);
        } else {
            span = 0x2000 - ((pan - 0x40) * 3);
        }
        sndFXCtrl14(id, 0x84, (u16)span);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSSSetPlayFreqCh(s32 index, u16 value) {
    extern char sound[];
    extern s32 OSDisableInterrupts(void);
    extern void OSRestoreInterrupts(s32 level);
    extern void sndStreamFrq(void* stream, u16 freq);
    void* entry;

    entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);
    if (*(u16*)entry != 0) {
        *(u16*)((s32)entry + 0x18) = value;
        if ((*(u16*)entry & 8) != 0) {
            if (*(u16*)(sound + 0x20C) == 0) {
                *(s32*)(sound + 0x210) = OSDisableInterrupts();
            }
            *(u16*)(sound + 0x20C) = *(u16*)(sound + 0x20C) + 1;
            sndStreamFrq(*(void**)((s32)entry + 0x4C), *(u16*)((s32)entry + 0x18));
            if (*(u16*)((s32)entry + 0x1A) == 2) {
                sndStreamFrq(*(void**)((s32)entry + 0xD4), *(u16*)((s32)entry + 0x18));
            }
            if (*(u16*)(sound + 0x20C) != 0) {
                *(u16*)(sound + 0x20C) = *(u16*)(sound + 0x20C) - 1;
                if (*(u16*)(sound + 0x20C) == 0) {
                    OSRestoreInterrupts(*(s32*)(sound + 0x210));
                }
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundEfxSetVolume(s32 index, u32 value) {
    extern char sound[];
    extern s32 sndEmitterVoiceID(void* emitter);
    extern s32 sndFXCheck(s32 id);
    extern void sndFXCtrl(s32 id, s32 ctrl, s32 value);
    void* entry;
    s32 id;

    entry = (void*)((s32)*(void**)(sound + 0xF4) + index * 0x88);
    if ((*(u16*)entry & 0x10) != 0) {
        id = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
    } else {
        id = sndFXCheck(*(s32*)((s32)entry + 0xC));
    }
    if ((u32)(id + 0x10000) != 0xFFFF) {
        *(u16*)((s32)entry + 4) = (u8)value;
        sndFXCtrl(id, 7, value);
        if (*(u16*)(sound + 0x214) != 0) {
            sndFXCtrl(id, 7, 0);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundEfxSetLPF(s32 index, s16 value) {
    extern char sound[];
    extern s32 sndEmitterVoiceID(void* emitter);
    extern s32 sndFXCheck(s32 id);
    extern void sndFXSetFilter(s32 id, s32 on, s32 value);
    void* entry;
    s32 id;

    entry = (void*)((s32)*(void**)(sound + 0xF4) + index * 0x88);
    if ((*(u16*)entry & 0x10) != 0) {
        id = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
    } else {
        id = sndFXCheck(*(s32*)((s32)entry + 0xC));
    }
    if ((u32)(id + 0x10000) != 0xFFFF) {
        if (value == 0) {
            sndFXSetFilter(id, 0, 0);
        } else if (value >= 0x3E80) {
            sndFXSetFilter(id, 0, 0);
        } else {
            sndFXSetFilter(id, 1, value);
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundMain(void) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    extern void sndSeqStop(void* seq);
    extern void SoundEfxMain(void);
    extern void SoundDVDMain(void);
    extern void SoundSSMain(void);
    s32 i;
    void* entry;

    i = 0;
    entry = *(void**)(sound + 0xF0);
    while (i < 4) {
        if (*(void**)((s32)entry + 8) != NULL) {
            if ((u32)(*(s32*)entry + 0x10000) != 0xFFFF) {
                if (sndSeqGetValid(*(void**)entry) != 1) {
                    *(u16*)*(void**)((s32)entry + 8) |= 2;
                    sndSeqStop(*(void**)entry);
                    *(s32*)entry = -1;
                }
            }
        }
        i++;
        entry = (void*)((s32)entry + 0x10);
    }
    SoundEfxMain();
    SoundDVDMain();
    SoundSSMain();
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSSFadeoutCh(s32 index) {
    extern char sound[];
    extern void* gp;
    void* entry;
    f32 rate;

    entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);
    if (*(u16*)entry != 0) {
        rate = (f32)*(u16*)(sound + 0x20A) / (1000.0f / (f32)*(s32*)((s32)gp + 4));
        if (rate < 1.0f) {
            rate = 1.0f;
        }
        *(f32*)((s32)entry + 0x14) = *(f32*)((s32)entry + 0x10) / rate;
        *(u16*)entry |= 0x20;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 SoundEfxCheck(s32 index) {
    extern char sound[];
    extern s32 sndEmitterVoiceID(void* emitter);
    extern s32 sndFXCheck(s32 id);
    extern s32 sndCheckEmitter(void* emitter);
    void* entry;
    u16 flags;
    s32 id;

    entry = (void*)((s32)*(void**)(sound + 0xF4) + index * 0x88);
    flags = *(u16*)entry;
    if (flags == 0) {
        return 0;
    }
    if ((flags & 0x10) != 0) {
        goto check_emitter;
    }
    if ((flags & 0x10) == 0) {
        goto normal_id;
    }
    id = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
    goto check_id;
normal_id:
    id = sndFXCheck(*(s32*)((s32)entry + 0xC));
check_id:
    id = sndFXCheck(id);
    if ((u32)(id + 0x10000) != 0xFFFF) {
        goto ok;
    }
    return 0;
check_emitter:
    if (sndCheckEmitter((void*)((s32)entry + 0x14)) != 0) {
        goto ok;
    }
    return 0;
ok:
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 loadDVD_callback(u32 offset, s32 length) {
    extern char sound[];
    extern s32 DVDOpen(char* path, void* fileInfo);
    extern s32 DVDReadPrio(void* fileInfo, void* dst, s32 length, u32 offset, s32 prio);
    extern void DVDClose(void* fileInfo);
    u8 fileInfo[0x40];

    if (DVDOpen(sound + 0x22A, fileInfo) == 0) {
        return 0;
    }
    if (DVDReadPrio(fileInfo, *(void**)(sound + 0x32C), length, offset, 2) <= 0) {
        return 0;
    }
    DVDClose(fileInfo);
    return *(s32*)(sound + 0x32C);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundEfxSetPan(s32 index, u32 value) {
    extern char sound[];
    extern s32 sndEmitterVoiceID(void* emitter);
    extern s32 sndFXCheck(s32 id);
    extern void sndFXCtrl(s32 id, s32 ctrl, s32 value);
    s32 offset;
    void* entry;
    s32 id;

    offset = index * 0x88;
    *(u16*)((s32)*(void**)(sound + 0xF4) + offset + 6) = (u8)value;
    entry = (void*)((s32)*(void**)(sound + 0xF4) + offset);
    if ((*(u16*)entry & 0x10) != 0) {
        id = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
    } else {
        id = sndFXCheck(*(s32*)((s32)entry + 0xC));
    }
    if ((u32)(id + 0x10000) != 0xFFFF) {
        sndFXCtrl(id, 0xA, value);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSongSetVolCh(s32 index, u8 volume) {
    extern char sound[];
    extern u32 sndSeqGetValid(void* seq);
    extern void sndSeqVolume(s32 volume, s32 frames, void* seq, s32 flag);
    void* entry;

    entry = (void*)((s32)*(void**)(sound + 0xF0) + (index << 4));
    if (sndSeqGetValid(*(void**)entry) != 0) {
        if ((u8)volume > 0x7F) {
            volume = 0x7F;
        }
        *(s32*)((s32)entry + 4) = (u8)volume;
        sndSeqVolume(*(u32*)((s32)entry + 4) & 0xFF, 0, *(void**)entry, 0);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

u8 SoundEfxStop(int param_1) {
    return 0;
}


u8 SoundLoadDVD2Free(void) {
    return 0;
}
