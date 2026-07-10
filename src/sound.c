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
    ;
}


void SoundSSMainInt(void) {
    ;
}


void _ssDVDReadAsync_activeChk(s32 zero, void* ptr) {
    ;
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


u32 SoundEfxPlayEx(s32 id, u16 priority, u32 volume, u32 pan) {
    extern char sound[];
    extern void sndFXKeyOff(u32 id);
    extern void sndRemoveEmitter(void* emitter);
    extern s32 OSDisableInterrupts(void);
    extern void OSRestoreInterrupts(s32 level);
    extern u32 sndFXStartEx(s32 id, u32 volume, u32 pan, u8 zero);
    extern void sndFXCtrl(u32 id, s32 ctrl, u32 value);
    extern u32 sndFXCheck(u32 id);
    extern u32 sndEmitterVoiceID(void* emitter);
    extern u32 sndCheckEmitter(void* emitter);
    s32 i;
    void* entry;
    u16 flags;
    u32 voice;

    i = 0;
    entry = *(void**)(sound + 0xF4);
    while (i < 0x28) {
        if (*(u16*)entry == 0) {
            break;
        }
        i++;
        entry = (void*)((s32)entry + 0x88);
    }
    if (i > 0x27) {
        i = 0;
        entry = *(void**)(sound + 0xF4);
        while (i < 0x28) {
            if (*(u16*)((s32)entry + 2) <= (priority & 0xFF)) {
                if (*(u16*)entry != 0) {
                    sndFXKeyOff(*(u32*)((s32)entry + 0xC));
                    if ((*(u16*)entry & 0x10) != 0) {
                        sndRemoveEmitter((void*)((s32)entry + 0x14));
                    }
                    *(u16*)entry = 0;
                }
                break;
            }
            i++;
            entry = (void*)((s32)entry + 0x88);
        }
        if (i > 0x27) {
            return -1;
        }
    }

    *(u16*)entry = 0;
    if (*(u16*)(sound + 0x20C) == 0) {
        *(s32*)(sound + 0x210) = OSDisableInterrupts();
    }
    *(u16*)(sound + 0x20C) = *(u16*)(sound + 0x20C) + 1;
    *(u32*)((s32)entry + 0xC) = sndFXStartEx(id, 0x7F, pan, 0);
    sndFXCtrl(*(u32*)((s32)entry + 0xC), 7, volume);
    if (*(u16*)(sound + 0x20C) != 0) {
        *(u16*)(sound + 0x20C) = *(u16*)(sound + 0x20C) - 1;
        if (*(u16*)(sound + 0x20C) == 0) {
            OSRestoreInterrupts(*(s32*)(sound + 0x210));
        }
    }
    if (*(s32*)((s32)entry + 0xC) == -1) {
        return -1;
    }
    *(u16*)entry = 1;
    *(u16*)((s32)entry + 2) = priority & 0xFF;
    *(u16*)((s32)entry + 4) = volume & 0xFF;
    *(u16*)((s32)entry + 6) = pan & 0xFF;
    *(u16*)((s32)entry + 8) = 0;
    *(u16*)((s32)entry + 0x10) = id;

    if (*(u16*)(sound + 0x214) != 0) {
        sndFXCtrl(*(u32*)((s32)entry + 0xC), 7, 0);
    }

    flags = *(u16*)entry;
    if (flags == 0) {
        return -1;
    }
    if ((flags & 0x10) != 0) {
        if (sndCheckEmitter((void*)((s32)entry + 0x14)) == 0) {
            return -1;
        }
    } else {
        if ((flags & 0x10) == 0) {
            voice = sndFXCheck(*(u32*)((s32)entry + 0xC));
        } else {
            voice = sndFXCheck(sndEmitterVoiceID((void*)((s32)entry + 0x14)));
        }
        voice = sndFXCheck(voice);
        if ((u32)(voice + 0x10000) == 0xFFFF) {
            return -1;
        }
    }
    return i;
}

void SoundSSPlayChEx(s32 index, char* name, s32 useSlot) {
    extern char sound[];
    extern f32 float_0_80421908;
    extern f32 float_127_80421918;
    extern char str_436[];
    extern char str_PCTs_PCTsPCTs_802e3cd0[];
    extern char str_stm_8042191c[];
    extern s32 OSDisableInterrupts(void);
    extern void OSRestoreInterrupts(s32 level);
    extern void DVDMgrClose(void* handle);
    extern void sndStreamFree(void* stream);
    extern char* getMarioStDvdRoot(void);
    extern int sprintf(char* s, char* format, ...);
    extern void* DVDMgrOpen(char* path, s32 zero, s32 zero2);
    extern void DVDMgrReadAsync(void* handle, void* dst, s32 size, s32 offset, void* callback);
    void* entry;
    s32 slot;
    char* base;
    u16 zero;

    entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);
    if (*(u16*)entry != 0) {
        if (*(u16*)(sound + 0x20C) == 0) {
            *(s32*)(sound + 0x210) = OSDisableInterrupts();
        }
        *(u16*)(sound + 0x20C) = *(u16*)(sound + 0x20C) + 1;
        if ((*(u16*)entry & 0x400) != 0) {
            slot = *(u16*)((s32)entry + 2);
            base = sound;
            zero = 0;
            slot <<= 1;
            *(u16*)((s32)base + slot + 0x226) = zero;
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

    if (useSlot != 0) {
        slot = 0;
        base = sound;
        while (slot < *(u16*)(sound + 0x224)) {
            if (*(u16*)((s32)base + 0x226) == 0) {
                *(u16*)(sound + 0x226 + (slot * 2)) = 1;
                *(u16*)((s32)entry + 2) = slot;
                break;
            }
            base += 2;
            slot++;
        }
        if (slot >= *(u16*)(sound + 0x224)) {
            return;
        }
    }

    *(s32*)(sound + 0x104) = index;
    *(u16*)((s32)entry + 0x74) = 0;
    *(s32*)((s32)entry + 0x6C) = 0;
    *(s32*)((s32)entry + 0x5C) = 0;
    *(s32*)((s32)entry + 0x60) = 0;
    *(u16*)((s32)entry + 0x7C) = 0;
    *(u16*)((s32)entry + 0x7E) = 0;
    *(u16*)((s32)entry + 0x80) = 0;
    *(u16*)((s32)entry + 0x82) = 0;
    *(u16*)((s32)entry + 0x84) = 0;
    *(u16*)((s32)entry + 0xFC) = 0;
    *(s32*)((s32)entry + 0xF4) = 0;
    *(s32*)((s32)entry + 0xE4) = 0;
    *(s32*)((s32)entry + 0xE8) = 0;
    *(u16*)((s32)entry + 0x104) = 0;
    *(u16*)((s32)entry + 0x106) = 0;
    *(u16*)((s32)entry + 0x108) = 0;
    *(u16*)((s32)entry + 0x10A) = 0;
    *(u16*)((s32)entry + 0x10C) = 0;
    *(u16*)((s32)entry + 6) = 0x40;
    *(u16*)((s32)entry + 8) = 0;
    *(f32*)((s32)entry + 0x10) = float_0_80421908;
    *(f32*)((s32)entry + 0xC) = float_127_80421918;
    *(u16*)entry = 1;
    if (useSlot != 0) {
        *(u16*)entry = *(u16*)entry | 0x400;
    }
    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_stm_8042191c);
    *(void**)((s32)entry + 0x28) = DVDMgrOpen(str_436, 0, 0);
    if (*(void**)((s32)entry + 0x28) == 0) {
        *(u16*)entry = 0;
        return;
    }
    *(void**)((s32)entry + 0xB0) = DVDMgrOpen(str_436, 0, 0);
    if (*(void**)((s32)entry + 0xB0) == 0) {
        *(u16*)entry = 0;
        return;
    }
    DVDMgrReadAsync(*(void**)((s32)entry + 0x28), *(void**)((s32)entry + 0x50), 0x100, 0, SoundSSPlayChEx_main);
}

u8* ssDecodeDPCM(u16* ctrl, s32 state, s16* out, u32 count, u8* src) {
    extern s16 dtbl[];
    u8 byte;
    u32 header;
    s32 nibble;
    s32 sample;

    goto check_count;

loop:
    byte = *src;
    if (*(u16*)(state + 0x5C) != 0) {
        nibble = (s8)(byte >> 4);
        if ((*(u16*)(state + 0x5A) & 1) != 0) {
            nibble = byte & 0xF;
        }
        header = *(u16*)(state + 0x58);
        *(u16*)(state + 0x5A) = *(u16*)(state + 0x5A) + 1;
        nibble = (s8)((s8)(nibble << 4) >> 4);
        if (*(u16*)(state + 0x5A) >= *(u16*)(state + 0x5C)) {
            src++;
            *(u16*)(state + 0x5C) = 0;
            *(s32*)(state + 0x44) = *(s32*)(state + 0x44) + 1;
        } else if ((*(u16*)(state + 0x5A) & 1) == 0) {
            src++;
            *(s32*)(state + 0x44) = *(s32*)(state + 0x44) + 1;
        }
    } else {
        header = byte & 0xF;
        src++;
        *(s32*)(state + 0x44) = *(s32*)(state + 0x44) + 1;
        if (header == 8) {
            *(u16*)(state + 0x5C) = (byte >> 4) + 4;
            *(u16*)(state + 0x5A) = 0;
            goto check_count;
        }
        nibble = (s8)((s8)(header << 4) >> 4);
        header = byte >> 4;
        *(u16*)(state + 0x58) = header;
    }

    sample = *(s16*)(state + 0x52) + ((s8)nibble * dtbl[header]);
    if (sample > 0x7FFF) {
        sample = 0x7FFF;
    }
    if (sample < -0x8000) {
        sample = -0x8000;
    }
    *(u16*)(state + 0x52) = sample;
    count--;
    *out = sample;
    out++;
    *(s32*)((s32)ctrl + 0x24) = *(s32*)((s32)ctrl + 0x24) + 1;

    if (*(u32*)(state + (*(u16*)(state + 0x4C) * 4) + 0x3C) <= *(u32*)(state + 0x44)) {
        if (*(s16*)(state + 0x54) == 0 && ((*ctrl & 0x4000) == 0)) {
            *(u32*)(state + 0x38) = *(u32*)(state + 0x38) - *(u32*)(state + 0x44);
            *(u32*)(state + 0x44) = 0;
            src = *(u8**)(state + (*(u16*)(state + 0x4C) * 4) + 0x2C);
            goto check_count;
        }
        *(u16*)(state + 0x54) = 0;
        *(u32*)(state + 0x44) = 0;
        *(u16*)(state + 0x4C) = 1 - *(u16*)(state + 0x4C);
        src = *(u8**)(state + (*(u16*)(state + 0x4C) * 4) + 0x2C);
    }
    *(u32*)(state + 0x38) = *(u32*)(state + 0x38) + 2;
    if (*(u32*)(state + 0x38) >= *(u32*)((s32)ctrl + 0x1C)) {
        *(u32*)(state + 0x38) = *(u32*)((s32)ctrl + 0x20);
        *(u16*)(state + 0x52) = *(u16*)(state + 0x50);
        if ((*ctrl & 0x200) == 0) {
            *ctrl |= 4;
            return src;
        }
    }

check_count:
    if (count != 0) {
        goto loop;
    }
    return src;
}

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
void SoundSSSetVolCh(s32 index, u8 volume) {
    extern char sound[];
    extern f32 float_63_8042190c;
    extern void sndStreamMixParameter(void* stream, s32 volume, s32 pan, s32 aux, s32 zero);
    void* entry;
    s32 offset;
    void* table;
    u32 pan8;
    s32 rightPan;
    f32 left;
    f32 right;

    offset = index * 0x138;
    entry = (void*)((s32)*(void**)(sound + 0x100) + offset);
    if (*(u16*)entry != 0) {
        right = (f32)(u8)volume;
        *(f32*)((s32)entry + 0x10) = right;
        *(f32*)((s32)entry + 0xC) = right;
        if ((*(u16*)entry & 8) != 0) {
            table = *(void**)(sound + 0x100);
            pan8 = *(u16*)((s32)entry + 6) & 0xFF;
            entry = (void*)((s32)table + offset);
            if (*(u16*)entry != 0) {
                *(u16*)((s32)entry + 6) = pan8;
                if ((*(u16*)entry & 8) != 0) {
                    if (*(u16*)((s32)entry + 0x1A) == 1) {
                        sndStreamMixParameter(*(void**)((s32)entry + 0x4C), (s32)*(f32*)((s32)entry + 0x10), pan8, *(u16*)((s32)entry + 8) & 0xFF, 0);
                    } else {
                        right = *(f32*)((s32)entry + 0x10);
                        left = right;
                        if (pan8 < 0x40) {
                            left = ((f32)pan8 * right) / float_63_8042190c;
                        }
                        if (((u32)(pan8 - 0x40) >> 31) == 0) {
                            rightPan = 0x7F - pan8;
                            right = ((f32)rightPan * right) / float_63_8042190c;
                        }
                        sndStreamMixParameter(*(void**)((s32)entry + 0x4C), (s32)right, 0, *(u16*)((s32)entry + 8) & 0xFF, 0);
                        sndStreamMixParameter(*(void**)((s32)entry + 0xD4), (s32)left, 0x7F, *(u16*)((s32)entry + 8) & 0xFF, 0);
                    }
                }
            }
        }
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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSSSetSrndPanCh(s32 index, u8 pan) {
    extern char sound[];
    extern f32 float_63_8042190c;
    extern void sndStreamMixParameter(void* stream, s32 volume, s32 pan, s32 aux, s32 zero);
    s32 offset;
    void* entry;
    void* table;
    u32 pan8;
    s32 rightPan;
    f32 left;
    f32 right;

    offset = index * 0x138;
    entry = (void*)((s32)*(void**)(sound + 0x100) + offset);
    if (*(u16*)entry != 0) {
        *(u16*)((s32)entry + 8) = (u8)pan;
        if ((*(u16*)entry & 8) != 0) {
            table = *(void**)(sound + 0x100);
            pan8 = *(u16*)((s32)entry + 6) & 0xFF;
            entry = (void*)((s32)table + offset);
            if (*(u16*)entry != 0) {
                *(u16*)((s32)entry + 6) = pan8;
                if ((*(u16*)entry & 8) != 0) {
                    if (*(u16*)((s32)entry + 0x1A) == 1) {
                        sndStreamMixParameter(*(void**)((s32)entry + 0x4C), (s32)*(f32*)((s32)entry + 0x10), pan8, *(u16*)((s32)entry + 8) & 0xFF, 0);
                    } else {
                        right = *(f32*)((s32)entry + 0x10);
                        left = right;
                        if (pan8 < 0x40) {
                            left = ((f32)pan8 * right) / float_63_8042190c;
                        }
                        if (((u32)(pan8 - 0x40) >> 31) == 0) {
                            rightPan = 0x7F - pan8;
                            right = ((f32)rightPan * right) / float_63_8042190c;
                        }
                        sndStreamMixParameter(*(void**)((s32)entry + 0x4C), (s32)right, 0, *(u16*)((s32)entry + 8) & 0xFF, 0);
                        sndStreamMixParameter(*(void**)((s32)entry + 0xD4), (s32)left, 0x7F, *(u16*)((s32)entry + 8) & 0xFF, 0);
                    }
                }
            }
        }
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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void SoundSSSetPanCh(s32 index, u8 pan) {
    extern char sound[];
    extern f32 float_63_8042190c;
    extern void sndStreamMixParameter(void* stream, s32 volume, s32 pan, s32 aux, s32 zero);
    void* entry;
    u32 pan8;
    s32 panArg;
    f32 left;
    f32 right;

    entry = (void*)((s32)*(void**)(sound + 0x100) + index * 0x138);
    if (*(u16*)entry != 0) {
        pan8 = (u8)pan;
        *(u16*)((s32)entry + 6) = pan8;
        if ((*(u16*)entry & 8) != 0) {
            if (*(u16*)((s32)entry + 0x1A) == 1) {
                panArg = pan;
                sndStreamMixParameter(*(void**)((s32)entry + 0x4C), (s32)*(f32*)((s32)entry + 0x10), panArg, *(u16*)((s32)entry + 8) & 0xFF, 0);
            } else {
                right = *(f32*)((s32)entry + 0x10);
                left = right;
                if (pan8 < 0x40) {
                    left = ((f32)pan8 * right) / float_63_8042190c;
                }
                if ((s32)((u8)pan - 0x40) >= 0) {
                    right = ((f32)(0x7F - (u8)pan) * right) / float_63_8042190c;
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

void SoundEfxStop(s32 index) {
    extern char sound[];
    extern void sndFXKeyOff(s32 id);
    extern void sndRemoveEmitter(void* emitter);
    void* entry;

    entry = (void*)((s32)*(void**)(sound + 0xF4) + index * 0x88);
    if (*(u16*)entry != 0) {
        sndFXKeyOff(*(s32*)((s32)entry + 0xC));
        if ((*(u16*)entry & 0x10) != 0) {
            sndRemoveEmitter((void*)((s32)entry + 0x14));
        }
        *(u16*)entry = 0;
    }
}

void SoundLoadDVD2Free(void) {
    extern char sound[];
    extern void __memFree(s32 heap, void* ptr);
    void* ptr;

    ptr = *(void**)(sound + 0x32C);
    if (ptr != 0) {
        __memFree(0, ptr);
        *(void**)(sound + 0x32C) = 0;
    }
}

