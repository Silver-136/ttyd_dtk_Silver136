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


void SoundSSPlayChEx_main(s32 result, s32 userData) {
    extern void DVDMgrReadAsync(void*, void*, u32, u32, void*);
    extern void DCInvalidateRange(void*, u32);
    extern void ARQPostRequest(void*, void*, u32, u32, u32, void*, u32, void*);
    extern u32 sndStreamAllocLength(u32, u32);
    extern void sndStreamAllocEx(void*, u32, u32, u32);
    extern void sndStreamMixParameter(void*, u32, u32, u32, u32);
    u16* sound = *(u16**)0x803E0238;
    u16* entry;
    u8* header;
    u32 slot;
    u32 length;
    s32 i;
    if (result < 0) return;
    entry = sound;
    for (slot = 0; slot < 3; slot++, entry += 0x4E) {
        if (*entry != 0 && *(s32*)(entry + 0xA) + 0x40 == userData) break;
    }
    if (slot == 3) return;
    header = *(u8**)(entry + 0x28);
    *(u16*)(entry + 0xC) = *(u16*)(header + 2);
    *(u16*)(entry + 0xD) = *(u32*)(header + 4);
    *(u32*)(entry + 0xE) = *(u32*)(header + 8);
    *(u32*)(entry + 0x10) = *(u32*)(header + 0xC);
    *(u32*)(entry + 0x44) = *(u32*)(header + 0x10);
    *(u32*)(entry + 0x88) = *(u32*)(header + 0x14);
    if (*header == 1) { *entry |= 0x80; entry[2] = 0x40; }
    else if (*header == 0) { *(u32*)(entry + 0x44) = *(u32*)(entry + 0xE); entry[2] = 0x40; }
    else { *entry |= 0x100; entry[2] = 0x100; }
    for (i = 0x2E; i <= 0x42; i++) entry[i] = 0;
    length = sndStreamAllocLength(0xE00, 0);
    if (length != 0) {
        u8* buffer = *(u8**)(entry + 0x28);
        for (i = 0; i < (s32)length; i++) buffer[i] = 0;
    }
    if ((*entry & 0x400) == 0) {
        u32 amount = (*(u32*)(entry + 0x44) - *(u32*)(entry + 0x2E) + 0x1F) & ~0x1F;
        DVDMgrReadAsync(*(void**)(entry + 0x14), *(void**)(entry + 0x2C), amount,
                        entry[2] + *(u32*)(entry + 0x2E), (void*)0x800DFC64);
        *entry |= 0x1000;
    } else {
        DCInvalidateRange(*(void**)(entry + 0x2C), 0x8000);
        ARQPostRequest(entry + 0x16, entry + 0x14, 1, 0, entry[2], *(void**)(entry + 0x2C), 0x8000, (void*)0x800DFC64);
    }
    sndStreamAllocEx(entry, 0xE00, 0, 0);
    sndStreamMixParameter(entry, 0x7F, 0x40, 0, 0);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _sscallback(void* outA, u32 countA, void* outB, u32 countB, u32 streamId) {
    extern char sound[];
    extern u8* ssDecodeDPCM(void* entry, s32 work, void* out, u32 count, u8* input);
    u8* entry = (u8*)(*(s32*)(sound + 0x100) + (streamId & 0xFF) * 0x138);
    u8* work;
    u8* src8;
    u16* src16;
    u16* dst16;
    u32 i;

    if (outA == 0 && outB == 0 && countA == 0 && countB == 0) {
        return 0;
    }
    *(u32*)(entry + 0x24) = 0;
    if ((*(u16*)entry & 0x8006) != 0 || *(u16*)(sound + 0x214) != 0) {
        if ((*(u16*)entry & 0x100) != 0) {
            dst16 = outA;
            for (i = 0; i < countA; i++) {
                dst16[i] = 0;
                *(u32*)(entry + 0x24) += 1;
            }
            dst16 = outB;
            for (i = 0; i < countB; i++) {
                dst16[i] = 0;
                *(u32*)(entry + 0x24) += 1;
            }
        } else {
            src8 = outA;
            for (i = 0; i < countA; i++) {
                src8[i] = 0;
                *(u32*)(entry + 0x24) += 1;
            }
            src8 = outB;
            for (i = 0; i < countB; i++) {
                src8[i] = 0;
                *(u32*)(entry + 0x24) += 1;
            }
        }
        return 0;
    }
    work = entry + ((streamId & 0x100) ? 0xB0 : 0x28);
    if ((*(u16*)entry & 0x80) != 0) {
        src8 = *(u8**)(work + *(u16*)(work + 0x4C) * 4 + 0x2C) + *(u32*)(work + 0x44);
        src8 = ssDecodeDPCM(entry, (s32)work, outA, countA, src8);
        ssDecodeDPCM(entry, (s32)work, outB, countB, src8);
        return *(s32*)(entry + 0x24);
    }
    if ((*(u16*)entry & 0x100) == 0) {
        src16 = (u16*)(*(s32*)(work + *(u16*)(work + 0x4C) * 4 + 0x2C) +
                           (*(u32*)(work + 0x44) & ~1));
        dst16 = outA;
        for (i = 0; i < countA; i++) {
            dst16[i] = *src16++;
            *(u32*)(entry + 0x24) += 1;
            *(u32*)(work + 0x44) += 2;
            *(u32*)(work + 0x38) += 2;
            if (*(u32*)(work + 0x44) >= *(u32*)(work + *(u16*)(work + 0x4C) * 4 + 0x3C)) {
                *(u32*)(work + 0x44) = 0;
                *(u16*)(work + 0x4C) = 1 - *(u16*)(work + 0x4C);
                src16 = *(u16**)(work + *(u16*)(work + 0x4C) * 4 + 0x2C);
            }
            if (*(u32*)(work + 0x38) >= *(u32*)(entry + 0x1C)) {
                *(u32*)(work + 0x38) = *(u32*)(entry + 0x20);
                if ((*(u16*)entry & 0x200) == 0) {
                    *(u16*)entry |= 4;
                    break;
                }
            }
        }
        dst16 = outB;
        for (i = 0; i < countB; i++) {
            dst16[i] = *src16++;
            *(u32*)(entry + 0x24) += 1;
            *(u32*)(work + 0x44) += 2;
            *(u32*)(work + 0x38) += 2;
        }
    } else {
        src8 = *(u8**)(work + *(u16*)(work + 0x4C) * 4 + 0x2C) + *(u32*)(work + 0x44);
        for (i = 0; i < countA; i++) {
            ((u8*)outA)[i] = *src8++;
            *(u32*)(work + 0x44) += 1;
            *(u32*)(work + 0x38) += 1;
        }
        for (i = 0; i < countB; i++) {
            ((u8*)outB)[i] = *src8++;
            *(u32*)(work + 0x44) += 1;
            *(u32*)(work + 0x38) += 1;
        }
        *(u32*)(entry + 0x24) = countA + countB;
    }
    return *(s32*)(entry + 0x24);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void SoundSSMain(void) {
    extern char sound[];
    extern void sndStreamMixParameter(void* stream, s32 volume, s32 pan, s32 span, s32 flags);
    extern void DVDMgrClose(void* entry);
    extern void sndStreamFree(void* stream);
    extern s32 OSDisableInterrupts(void);
    extern void OSRestoreInterrupts(s32 state);
    extern const f32 float_0_80421908;
    extern const f32 float_63_8042190c;
    u8* entry;
    s32 i;

    for (i = 0; i < 3; i++) {
        f32 left;
        f32 right;
        u32 pan;
        entry = (u8*)(*(s32*)(sound + 0x100) + i * 0x138);
        if ((*(u16*)entry & 8) == 0 || (*(u16*)entry & 2) != 0) {
            continue;
        }
        if ((*(u16*)entry & 4) != 0) {
            *(u16*)entry &= ~4;
            goto close_stream;
        }
        if ((*(u16*)entry & 0x40) != 0) {
            if (*(f32*)(entry + 0x10) == float_0_80421908) {
                *(u16*)entry |= 2;
                *(u16*)entry &= ~0x40;
                continue;
            }
            *(f32*)(entry + 0x10) -= *(f32*)(entry + 0x14);
            if (*(f32*)(entry + 0x10) <= float_0_80421908) {
                *(f32*)(entry + 0x10) = float_0_80421908;
            }
        }
        if ((*(u16*)entry & 0x20) != 0) {
            if (*(f32*)(entry + 0x10) == float_0_80421908) {
                *(u16*)entry &= ~0x20;
                goto close_stream;
            }
            *(f32*)(entry + 0x10) -= *(f32*)(entry + 0x14);
            if (*(f32*)(entry + 0x10) <= float_0_80421908) {
                *(f32*)(entry + 0x10) = float_0_80421908;
            }
        }
        if ((*(u16*)entry & 0x10) != 0) {
            *(f32*)(entry + 0x10) += *(f32*)(entry + 0x14);
            if (*(f32*)(entry + 0x10) >= *(f32*)(entry + 0xC)) {
                *(f32*)(entry + 0x10) = *(f32*)(entry + 0xC);
                *(f32*)(entry + 0x14) = float_0_80421908;
                *(u16*)entry &= ~0x10;
            }
        }
        pan = *(u16*)(entry + 6) & 0xFF;
        if (*(u16*)(entry + 0x1A) == 1) {
            sndStreamMixParameter(*(void**)(entry + 0x4C), (s32)*(f32*)(entry + 0x10),
                                  pan, *(u16*)(entry + 8) & 0xFF, 0);
        } else {
            left = right = *(f32*)(entry + 0x10);
            if (pan < 0x40) {
                right = ((f32)pan * right) / float_63_8042190c;
            } else {
                left = ((f32)(0x7F - pan) * left) / float_63_8042190c;
            }
            sndStreamMixParameter(*(void**)(entry + 0x4C), (s32)left, 0,
                                  *(u16*)(entry + 8) & 0xFF, 0);
            sndStreamMixParameter(*(void**)(entry + 0xD4), (s32)right, 0x7F,
                                  *(u16*)(entry + 8) & 0xFF, 0);
        }
        continue;

close_stream:
        if (*(u16*)entry != 0) {
            if (*(u16*)(sound + 0x20C) == 0) {
                *(s32*)(sound + 0x210) = OSDisableInterrupts();
            }
            *(u16*)(sound + 0x20C) += 1;
            if ((*(u16*)entry & 0x400) != 0) {
                *(u8*)(sound + 0x226 + *(u16*)(entry + 2)) = 0;
            }
            *(u16*)entry = 0;
            DVDMgrClose(*(void**)(entry + 0x28));
            DVDMgrClose(*(void**)(entry + 0xB0));
            sndStreamFree(*(void**)(entry + 0x4C));
            if (*(u16*)(entry + 0x1A) == 2) {
                sndStreamFree(*(void**)(entry + 0xD4));
            }
            *(u16*)(sound + 0x20C) -= 1;
            if (*(u16*)(sound + 0x20C) == 0) {
                OSRestoreInterrupts(*(s32*)(sound + 0x210));
            }
            *(s32*)(sound + 0x104) = -1;
        }
    }
}

void SoundInit(void) {
    extern char sound[];
    extern char aramMemArray[];
    extern char revH[];
    extern char revS[];
    extern char cho[];
    extern char dly[];
    extern void sndMalloc(void);
    extern void sndFree(void);
    extern void SoundAIDMACallback(void);
    extern void sndAuxCallbackReverbHI(void);
    extern void sndAuxCallbackReverbSTD(void);
    extern void sndAuxCallbackChorus(void);
    extern void ARInit(void* array, s32 count);
    extern void ARQInit(void);
    extern void AIInit(s32);
    extern s32 ARAlloc(s32 size);
    extern void sndSetHooks(void* hooks);
    extern void sndInit(s32, s32, s32, s32, s32, s32);
    extern void sndAuxCallbackPrepareReverbHI(void* rev);
    extern void sndAuxCallbackPrepareReverbSTD(void* rev);
    extern void sndAuxCallbackPrepareChorus(void* chorus);
    extern void sndAuxCallbackPrepareDelay(void* delay);
    extern void sndSetAuxProcessingCallbacks(s32, void*, void*, s32, s32, void*, void*, s32, s32);
    extern void sndVolume(s32, s32, s32);
    extern void sndOutputMode(s32);
    extern void* AIRegisterDMACallback(void* cb);
    extern u32 AIGetDMAStartAddr(void);
    extern u32 AIGetDMALength(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern u32 sndStreamAllocLength(s32 length, u32 flags);
    extern const f32 float_0_80421908;
    extern const f32 float_0p1_80421968;
    extern const f32 float_0p3_8042196c;
    extern const f32 float_0p5_80421978;
    extern const f32 float_0p8_80421970;
    extern const f32 float_2_80421974;
    extern const f32 float_3_80421964;

    void* hooks[2];
    u8* entry;
    s32 i;

    hooks[0] = sndMalloc;
    hooks[1] = sndFree;

    ARInit(aramMemArray, 8);
    ARQInit();
    AIInit(0);
    ARAlloc(0xBFC000);
    sndSetHooks(hooks);
    sndInit(0x40, 0xC, 0x2C, 1, 1, 0xBFC000);

    *(u8*)(revH + 0x1C4) = 0;
    *(f32*)(revH + 0x1D0) = float_3_80421964;
    *(f32*)(revH + 0x1D8) = float_0p1_80421968;
    *(f32*)(revH + 0x1D4) = float_0p3_8042196c;
    *(f32*)(revH + 0x1C8) = float_0_80421908;
    *(f32*)(revH + 0x1DC) = float_0_80421908;
    *(f32*)(revH + 0x1CC) = float_0p8_80421970;
    sndAuxCallbackPrepareReverbHI(revH);

    *(u8*)(revS + 0x13C) = 0;
    *(f32*)(revS + 0x148) = float_2_80421974;
    *(f32*)(revS + 0x150) = float_0p1_80421968;
    *(f32*)(revS + 0x14C) = float_0p5_80421978;
    *(f32*)(revS + 0x140) = float_0p5_80421978;
    *(f32*)(revS + 0x144) = float_0p5_80421978;
    sndAuxCallbackPrepareReverbSTD(revS);

    *(s32*)(cho + 0x90) = 10;
    *(s32*)(cho + 0x94) = 0;
    *(s32*)(cho + 0x98) = 500;
    sndAuxCallbackPrepareChorus(cho);

    *(s32*)(dly + 0x3C) = 0x3C;
    *(s32*)(dly + 0x40) = 0x5A;
    *(s32*)(dly + 0x44) = 0x46;
    *(s32*)(dly + 0x48) = 0x14;
    *(s32*)(dly + 0x4C) = 0x14;
    *(s32*)(dly + 0x50) = 0x14;
    *(s32*)(dly + 0x54) = 0x32;
    *(s32*)(dly + 0x58) = 0x32;
    *(s32*)(dly + 0x5C) = 0x32;
    sndAuxCallbackPrepareDelay(dly);

    sndSetAuxProcessingCallbacks(0, sndAuxCallbackReverbHI, revH, 0xFF, 0,
                                 sndAuxCallbackReverbSTD, cho, 0xFF, 0);
    sndVolume(0x7F, 0, 0xFF);
    if (*(s32*)(sound + 0x218) != 1) {
        *(s32*)(sound + 0x218) = 1;
        sndOutputMode(1);
    }

    *(void**)(sound + 0x5BC) = AIRegisterDMACallback(SoundAIDMACallback);
    *(u32*)(sound + 0x334) = AIGetDMAStartAddr() | 0x80000000;
    *(u32*)(sound + 0x338) = AIGetDMALength();
    if (*(u32*)(sound + 0x338) > 0x280) {
        for (;;) {
        }
    }

    *(u16*)(sound + 0x330) = 0;
    *(s32*)(sound + 0x15C0) = 0;
    *(u16*)(sound + 0x208) = 10;
    *(u16*)(sound + 0x20A) = 500;
    *(s32*)(sound + 0xE0) = 0;
    *(s32*)(sound + 0xE4) = 0;
    *(u16*)(sound + 0x214) = 0;
    *(u16*)(sound + 0x224) = 0;
    *(s32*)(sound + 0xE8) = 0;

    *(void**)(sound + 0x32C) = __memAlloc(0, 0x20000);
    if (*(void**)(sound + 0x32C) == NULL) {
        for (;;) {
        }
    }
    *(void**)(sound + 0xEC) = NULL;

    *(void**)(sound + 0xF0) = __memAlloc(0, 0x40);
    if (*(void**)(sound + 0xF0) == NULL) {
        for (;;) {
        }
    }
    entry = *(u8**)(sound + 0xF0);
    for (i = 0; i < 4; i++, entry += 0x10) {
        *(s32*)entry = -1;
        *(s32*)(entry + 8) = 0;
    }

    *(void**)(sound + 0xF4) = __memAlloc(0, 0x1540);
    if (*(void**)(sound + 0xF4) == NULL) {
        for (;;) {
        }
    }
    entry = *(u8**)(sound + 0xF4);
    for (i = 0; i < 0x28; i++, entry += 0x88) {
        *(u16*)entry = 0;
        *(s32*)(entry + 0xC) = -1;
        *(u16*)(entry + 2) = 0;
        *(u16*)(entry + 4) = 0x7F;
        *(u16*)(entry + 6) = 0x40;
    }

    *(void**)(sound + 0xF8) = __memAlloc(0, 0x2A8);
    if (*(void**)(sound + 0xF8) == NULL) {
        for (;;) {
        }
    }
    *(u16*)(*(u8**)(sound + 0xF8)) = 0;
    *(u16*)(*(u8**)(sound + 0xF8) + 0x154) = 0;

    *(void**)(sound + 0x100) = __memAlloc(0, 0x3A8);
    if (*(void**)(sound + 0x100) == NULL) {
        for (;;) {
        }
    }
    entry = *(u8**)(sound + 0x100);
    for (i = 0; i < 3; i++, entry += 0x138) {
        *(u16*)entry = 0;
        *(void**)(entry + 0x54) = __memAlloc(0, 0x8000);
        if (*(void**)(entry + 0x54) == NULL) {
            for (;;) {
            }
        }
        *(void**)(entry + 0x58) = __memAlloc(0, 0x8000);
        if (*(void**)(entry + 0x58) == NULL) {
            for (;;) {
            }
        }
        *(void**)(entry + 0x50) = __memAlloc(0, sndStreamAllocLength(0xE00, 0));
        if (*(void**)(entry + 0x50) == NULL) {
            for (;;) {
            }
        }
        *(void**)(entry + 0xDC) = __memAlloc(0, 0x8000);
        if (*(void**)(entry + 0xDC) == NULL) {
            for (;;) {
            }
        }
        *(void**)(entry + 0xE0) = __memAlloc(0, 0x8000);
        if (*(void**)(entry + 0xE0) == NULL) {
            for (;;) {
            }
        }
        *(void**)(entry + 0xD8) = __memAlloc(0, sndStreamAllocLength(0xE00, 0));
        if (*(void**)(entry + 0xD8) == NULL) {
            for (;;) {
            }
        }
    }

    *(void**)(sound + 0x108) = sndAuxCallbackReverbSTD;
    *(void**)(sound + 0x10C) = revH;
    *(u8*)(sound + 0x110) = 0xFF;
    *(s32*)(sound + 0x114) = 0;
    *(void**)(sound + 0x118) = sndAuxCallbackReverbHI;
    *(void**)(sound + 0x11C) = cho;
    *(u8*)(sound + 0x120) = 0xFF;
    *(s32*)(sound + 0x124) = 0;
}

s32 SoundDropData(void) {
    extern char sound[];
    extern u32 sndSeqGetValid(u32 id);
    extern void sndSeqStop(void);
    extern void sndFXKeyOff(u32 id);
    extern void sndRemoveEmitter(void* emitter);
    extern void AISetStreamPlayState(u32 state);
    extern void DVDCancelStream(void* fileInfo);
    extern void DVDClose(void* fileInfo);
    extern s32 OSDisableInterrupts(void);
    extern void OSRestoreInterrupts(s32 level);
    extern void DVDMgrClose(void* handle);
    extern void sndStreamFree(void* stream);
    extern void sndSilence(void);
    extern void sndPopGroup(void);
    extern void __memFree(s32 heap, void* ptr);

    u8* node;
    u8* entry;
    s32 i;

    node = *(u8**)(sound + 0xEC);

    for (i = 0; i < 4; i++) {
        if (sndSeqGetValid(*(u32*)(*(s32*)(sound + 0xF0) + i * 0x10)) != 0) {
            sndSeqStop();
            *(s32*)(*(s32*)(sound + 0xF0) + i * 0x10) = -1;
        }
    }

    entry = *(u8**)(sound + 0xF4);
    for (i = 0; i < 0x28; i++, entry += 0x88) {
        if (*(u16*)entry != 0) {
            sndFXKeyOff(*(u32*)(entry + 0xC));
            if ((*(u16*)entry & 0x10) != 0) {
                sndRemoveEmitter(entry + 0x14);
            }
            *(u16*)entry = 0;
        }
    }

    entry = *(u8**)(sound + 0xF8);
    for (i = 0; i < 2; i++, entry += 0x154) {
        if ((*(u16*)entry & 3) != 0) {
            AISetStreamPlayState(0);
            *(u16*)entry = 0;
            *(s32*)(sound + 0xFC) = -1;
            DVDCancelStream(entry + 0x118);
            DVDClose(entry + 0x118);
        }
    }

    entry = *(u8**)(sound + 0x100);
    for (i = 0; i < 3; i++, entry += 0x138) {
        if (*(u16*)entry != 0) {
            if (*(u16*)(sound + 0x20C) == 0) {
                *(s32*)(sound + 0x210) = OSDisableInterrupts();
            }
            *(u16*)(sound + 0x20C) += 1;
            if ((*(u16*)entry & 0x400) != 0) {
                *(u16*)(sound + 0x226 + *(u16*)(entry + 2) * 2) = 0;
            }
            *(u16*)entry = 0;
            DVDMgrClose(*(void**)(entry + 0x28));
            DVDMgrClose(*(void**)(entry + 0xB0));
            sndStreamFree(*(void**)(entry + 0x4C));
            if (*(u16*)(entry + 0x1A) == 2) {
                sndStreamFree(*(void**)(entry + 0xD4));
            }
            if (*(u16*)(sound + 0x20C) != 0) {
                *(u16*)(sound + 0x20C) -= 1;
                if (*(u16*)(sound + 0x20C) == 0) {
                    OSRestoreInterrupts(*(s32*)(sound + 0x210));
                }
            }
            *(s32*)(sound + 0x104) = -1;
        }
    }

    sndSilence();

    for (i = 0; i < 4; i++) {
        if (sndSeqGetValid(*(u32*)(*(s32*)(sound + 0xF0) + i * 0x10)) != 0) {
            sndSeqStop();
            *(s32*)(*(s32*)(sound + 0xF0) + i * 0x10) = -1;
        }
    }

    entry = *(u8**)(sound + 0xF4);
    for (i = 0; i < 0x28; i++, entry += 0x88) {
        if (*(u16*)entry != 0) {
            sndFXKeyOff(*(u32*)(entry + 0xC));
            if ((*(u16*)entry & 0x10) != 0) {
                sndRemoveEmitter(entry + 0x14);
            }
            *(u16*)entry = 0;
        }
    }

    entry = *(u8**)(sound + 0xF8);
    for (i = 0; i < 2; i++, entry += 0x154) {
        if ((*(u16*)entry & 3) != 0) {
            AISetStreamPlayState(0);
            *(u16*)entry = 0;
            *(s32*)(sound + 0xFC) = -1;
            DVDCancelStream(entry + 0x118);
            DVDClose(entry + 0x118);
        }
    }

    entry = *(u8**)(sound + 0x100);
    for (i = 0; i < 3; i++, entry += 0x138) {
        if (*(u16*)entry != 0) {
            if (*(u16*)(sound + 0x20C) == 0) {
                *(s32*)(sound + 0x210) = OSDisableInterrupts();
            }
            *(u16*)(sound + 0x20C) += 1;
            if ((*(u16*)entry & 0x400) != 0) {
                *(u16*)(sound + 0x226 + *(u16*)(entry + 2) * 2) = 0;
            }
            *(u16*)entry = 0;
            DVDMgrClose(*(void**)(entry + 0x28));
            DVDMgrClose(*(void**)(entry + 0xB0));
            sndStreamFree(*(void**)(entry + 0x4C));
            if (*(u16*)(entry + 0x1A) == 2) {
                sndStreamFree(*(void**)(entry + 0xD4));
            }
            if (*(u16*)(sound + 0x20C) != 0) {
                *(u16*)(sound + 0x20C) -= 1;
                if (*(u16*)(sound + 0x20C) == 0) {
                    OSRestoreInterrupts(*(s32*)(sound + 0x210));
                }
            }
            *(s32*)(sound + 0x104) = -1;
        }
    }

    for (i = 0; i < *(s32*)(sound + 0xE0); i++) {
        while (*(s32*)(sound + i * 4 + 0xC0) != 0) {
            sndPopGroup();
            *(s32*)(sound + i * 4 + 0xC0) -= 1;
        }
    }
    for (i = 0; i < *(s32*)(sound + 0xE0); i++) {
        __memFree(0, *(void**)(sound + i * 4));
    }
    for (i = 0; i < *(s32*)(sound + 0xE0); i++) {
        __memFree(0, *(void**)(sound + i * 4 + 0x20));
    }
    for (i = 0; i < *(s32*)(sound + 0xE0); i++) {
        __memFree(0, *(void**)(sound + i * 4 + 0x40));
    }
    for (i = 0; i < *(s32*)(sound + 0xE4); i++) {
        __memFree(0, *(void**)(sound + i * 4 + 0x60));
        __memFree(0, *(void**)(sound + i * 4 + 0x80));
        __memFree(0, *(void**)(sound + i * 4 + 0xA0));
    }

    *(s32*)(sound + 0xE0) = 0;
    *(s32*)(sound + 0xE4) = 0;
    *(s32*)(sound + 0xE8) = 0;

    while (node != NULL) {
        u8* next = *(u8**)(node + 0xC);
        if ((*(u16*)node & 1) == 0) {
            __memFree(0, *(void**)(node + 8));
        }
        __memFree(0, node);
        node = next;
    }
    *(void**)(sound + 0xEC) = NULL;
    return 1;
}

void SoundSSMainInt(void) {
    extern char sound[];
    extern void DVDMgrReadAsync(void*, void*, u32, u32, void*);
    extern void DCInvalidateRange(void*, u32);
    extern void ARQPostRequest(void*, void*, s32, s32, u32, void*, u32, void*);
    extern void _ssDVDReadAsync_activeChk(s32, void*);
    extern void cache_flush(void*);

    s32 i;
    s32 offset;
    u8* entry;

    offset = 0;
    for (i = 0; i < 3; i++, offset += 0x138) {
        entry = (u8*)(*(s32*)(sound + 0x100) + offset);
        if ((*(u16*)entry & 8) != 0 && (*(u16*)entry & 2) == 0) {
            if ((*(u16*)entry & 1) != 0) {
                if (*(s32*)(entry + 0x6C) != 0) {
                    if (*(u16*)(entry + 0x7C) == 0 && (*(u16*)entry & 0x1000) == 0) {
                        u8* work = entry + 0x28;
                        s32 pos = *(s32*)(entry + 0x5C);
                        u32 end = *(u32*)(entry + 0x88);
                        s32 chunk = 0x8000;
                        u32 readSize;

                        if (end < (u32)(pos + 0x8000)) {
                            chunk = end - pos;
                        } else {
                            s32 limit = 0x10000;
                            s32 div = 2;
                            s32 count = 4;
                            while (count != 0) {
                                if (end < (u32)(pos + limit)) {
                                    u32 remain = end - (pos + (div - 1) * 0x8000);
                                    chunk = 0x8000 - (0x8000 - remain) / div;
                                    chunk += 0x20 - (chunk & 0x1F);
                                    break;
                                }
                                div++;
                                limit += 0x8000;
                                count--;
                            }
                        }
                        readSize = (chunk + 0x1F) & ~0x1F;
                        if ((*(u16*)entry & 0x400) == 0) {
                            DVDMgrReadAsync(*(void**)work,
                                            *(void**)(work + (1 - *(u16*)(entry + 0x74)) * 4 + 0x2C),
                                            readSize, *(u16*)(entry + 4) + pos,
                                            _ssDVDReadAsync_activeChk);
                        } else {
                            s32 aramBase = *(s32*)(sound + 0x21C + *(u16*)(entry + 2) * 4);
                            void* dst = *(void**)(work + (1 - *(u16*)(entry + 0x74)) * 4 + 0x2C);
                            DCInvalidateRange(dst, 0x8000);
                            ARQPostRequest(entry + 0x2C, work, 1, 0,
                                           *(u16*)(entry + 4) + pos + aramBase, dst, readSize,
                                           cache_flush);
                        }
                        *(s32*)(entry + 0x70) = chunk;
                        *(u16*)entry |= 0x1000;
                        if ((*(u16*)entry & 0x400) != 0) {
                            *(u16*)(entry + 0x7C) = 1;
                        }
                    }
                }

                if (*(u16*)(entry + 0x1A) == 2 && *(s32*)(entry + 0xF4) != 0) {
                    if (*(u16*)(entry + 0x104) == 0 && (*(u16*)entry & 0x2000) == 0) {
                        u8* work = entry + 0xB0;
                        s32 pos = *(s32*)(entry + 0xE4);
                        u32 end = *(u32*)(entry + 0x110);
                        s32 chunk = 0x8000;
                        u32 readSize;
                        s32 add = end + (0x20 - (end & 0x1F));

                        if (end < (u32)(pos + 0x8000)) {
                            chunk = end - pos;
                        } else {
                            s32 limit = 0x10000;
                            s32 div = 2;
                            s32 count = 4;
                            while (count != 0) {
                                if (end < (u32)(pos + limit)) {
                                    u32 remain = end - (pos + (div - 1) * 0x8000);
                                    chunk = 0x8000 - (0x8000 - remain) / div;
                                    chunk += 0x20 - (chunk & 0x1F);
                                    break;
                                }
                                div++;
                                limit += 0x8000;
                                count--;
                            }
                        }
                        readSize = (chunk + 0x1F) & ~0x1F;
                        if ((*(u16*)entry & 0x400) == 0) {
                            DVDMgrReadAsync(*(void**)work,
                                            *(void**)(work + (1 - *(u16*)(entry + 0xFC)) * 4 + 0x2C),
                                            readSize, *(u16*)(entry + 4) + pos + add,
                                            _ssDVDReadAsync_activeChk);
                        } else {
                            s32 aramBase = *(s32*)(sound + 0x21C + *(u16*)(entry + 2) * 4);
                            void* dst = *(void**)(work + (1 - *(u16*)(entry + 0xFC)) * 4 + 0x2C);
                            DCInvalidateRange(dst, 0x8000);
                            ARQPostRequest(entry + 0xB4, work, 1, 0,
                                           *(u16*)(entry + 4) + pos + add + aramBase, dst, readSize,
                                           cache_flush);
                        }
                        *(s32*)(entry + 0xF8) = chunk;
                        *(u16*)entry |= 0x2000;
                        if ((*(u16*)entry & 0x400) != 0) {
                            *(u16*)(entry + 0x104) = 1;
                        }
                    }
                }
            }

            if (*(u16*)(entry + 0x1A) == 2 && *(u16*)(entry + 0x7C) == 0 &&
                *(u16*)(entry + 0x104) == 0 && *(u16*)(entry + 0x74) != *(u16*)(entry + 0xFC)) {
                *(u16*)(entry + 0xFC) = *(u16*)(entry + 0x74);
                *(s32*)(entry + 0xF4) = *(s32*)(entry + 0x6C);
                *(s32*)(entry + 0xE8) = *(s32*)(entry + 0x60);
                *(s32*)(entry + 0xE4) = *(s32*)(entry + 0x5C);
            }

            if (*(u16*)(entry + 0x1A) == 2) {
                if (*(u16*)(sound + 0x214) == 2 && *(u16*)(entry + 0x7C) == 1 &&
                    *(u16*)(entry + 0x104) == 1) {
                    *(u16*)(sound + 0x214) = 0;
                }
            } else if (*(u16*)(sound + 0x214) == 2 && *(u16*)(entry + 0x7C) == 1) {
                *(u16*)(sound + 0x214) = 0;
            }
        }
    }
}

void _ssDVDReadAsync_activeChk(s32 result, void* ptr) {
    extern char sound[];
    extern s32 OSDisableInterrupts(void);
    extern void OSRestoreInterrupts(s32);
    extern void sndStreamActivate(s32);
    extern void sndStreamMixParameter(s32, s32, s32, s32, s32);
    extern const f32 float_63_8042190c;

    u8* entry;
    u8* block;
    s32 matchedAlt;
    s32 i;
    s32 offset;

    if (*(u16*)(sound + 0x20C) == 0) {
        *(s32*)(sound + 0x210) = OSDisableInterrupts();
    }
    *(u16*)(sound + 0x20C) += 1;

    entry = *(u8**)(sound + 0x100);
    for (i = 0; i < 3; i++, entry += 0x138) {
        if (*(u16*)entry != 0) {
            block = NULL;
            matchedAlt = 0;
            if ((void*)(*(s32*)(entry + 0x28) + 0x40) == ptr) {
                block = entry + 0x28;
                matchedAlt = 0;
            }
            if ((void*)(*(s32*)(entry + 0xB0) + 0x40) == ptr) {
                block = entry + 0xB0;
                matchedAlt = 1;
            }

            if (block != NULL) {
                if (result < 0) {
                    goto done;
                }
                *(s32*)(block + 0x34) += *(s32*)(block + 0x48);
                *(s32*)(block + (1 - *(u16*)(block + 0x4C)) * 4 + 0x3C) = *(s32*)(block + 0x48);
                *(u16*)(block + 0x54) = 1;

                if (*(u32*)(block + 0x34) < *(u32*)(block + 0x60)) {
                    if (matchedAlt != 0) {
                        *(u16*)entry &= ~0x2000;
                    } else {
                        *(u16*)entry &= ~0x1000;
                    }
                } else {
                    *(s32*)(block + 0x34) = *(s32*)(block + 0x64);
                    if ((*(u16*)entry & 0x200) == 0) {
                        *(u16*)entry |= 0x4000;
                    } else if (matchedAlt != 0) {
                        *(u16*)entry &= ~0x2000;
                    } else {
                        *(u16*)entry &= ~0x1000;
                    }
                }

                if (*(u16*)(entry + 0x1A) == 2) {
                    if (*(u16*)(sound + 0x214) == 2 && *(u16*)(entry + 0x7C) == 1 &&
                        *(u16*)(entry + 0x104) == 1) {
                        *(u16*)(sound + 0x214) = 0;
                    }
                } else if (*(u16*)(sound + 0x214) == 2 && *(u16*)(entry + 0x7C) == 1) {
                    *(u16*)(sound + 0x214) = 0;
                }
                break;
            }
        }
    }

    offset = 0;
    for (i = 0; i < 3; i++, offset += 0x138) {
        entry = (u8*)(*(s32*)(sound + 0x100) + offset);
        if (*(u16*)entry != 0 && (*(u16*)entry & 8) == 0) {
            if ((void*)(*(s32*)(entry + 0x28) + 0x40) == ptr) {
                *(u16*)(entry + 0x7E) = 1;
            }
            if ((void*)(*(s32*)(entry + 0xB0) + 0x40) == ptr) {
                *(u16*)(entry + 0x106) = 1;
            }

            if (*(u16*)(entry + 0x1A) == 2) {
                if (*(u16*)(entry + 0x7E) == 0) {
                    continue;
                }
                if (*(u16*)(entry + 0x106) == 0) {
                    continue;
                }
            } else if (*(u16*)(entry + 0x7E) == 0) {
                continue;
            }

            *(u16*)(entry + 0x74) = 0;
            *(u16*)(entry + 0xFC) = 0;
            *(u16*)(entry + 0x7C) = 0;
            *(u16*)(entry + 0x104) = 0;
            sndStreamActivate(*(s32*)(entry + 0x4C));
            if (*(u16*)(entry + 0x1A) == 2) {
                sndStreamActivate(*(s32*)(entry + 0xD4));
            }

            {
                u8* mixEntry = (u8*)(*(s32*)(sound + 0x100) + i * 0x138);
                u32 pan = *(u16*)(entry + 6) & 0xFF;
                *(u16*)(mixEntry + 6) = pan;
                if (*(u16*)mixEntry != 0 && (*(u16*)mixEntry & 8) != 0) {
                    if (*(u16*)(mixEntry + 0x1A) == 1) {
                        sndStreamMixParameter(*(s32*)(mixEntry + 0x4C), (s32)*(f32*)(mixEntry + 0x10),
                                              pan, *(u16*)(mixEntry + 8) & 0xFF, 0);
                    } else {
                        f32 left = *(f32*)(mixEntry + 0x10);
                        f32 right = left;
                        if (pan < 0x40) {
                            right = ((f32)pan * left) / float_63_8042190c;
                        }
                        if ((s32)(pan - 0x40) > -1) {
                            left = ((f32)(0x7F - pan) * left) / float_63_8042190c;
                        }
                        sndStreamMixParameter(*(s32*)(mixEntry + 0x4C), (s32)left, 0,
                                              *(u16*)(mixEntry + 8) & 0xFF, 0);
                        sndStreamMixParameter(*(s32*)(mixEntry + 0xD4), (s32)right, 0x7F,
                                              *(u16*)(mixEntry + 8) & 0xFF, 0);
                    }
                }
            }
            *(u16*)entry |= 8;
            goto done;
        }
    }

done:
    if (*(u16*)(sound + 0x20C) != 0) {
        *(u16*)(sound + 0x20C) -= 1;
        if (*(u16*)(sound + 0x20C) == 0) {
            OSRestoreInterrupts(*(s32*)(sound + 0x210));
        }
    }
}

void SoundDVDMain(void) {
    extern char sound[];
    extern u32 AIGetStreamSampleCount(void);
    extern void AISetStreamPlayState(u32 state);
    extern void DVDCancelStream(void* fileInfo);
    extern void DVDClose(void* fileInfo);
    extern void AISetStreamVolLeft(u32 volume);
    extern void AISetStreamVolRight(u32 volume);
    extern const f32 float_0_80421908;
    extern const f32 float_63_8042190c;

    u8* base;
    u8* entry;
    s32 i;
    s32 offset;

    base = *(u8**)(sound + 0xF8);
    offset = 0;
    for (i = 0; i < 2; i++, offset += 0x154) {
        u16 flags;
        entry = base + offset;
        flags = *(u16*)entry;
        if ((flags & 2) == 0) {
            if ((flags & 0x40) != 0) {
                if (float_0_80421908 == *(f32*)(entry + 8)) {
                    *(u32*)(entry + 0x114) = AIGetStreamSampleCount();
                    if ((*(u16*)entry & 3) != 0) {
                        AISetStreamPlayState(0);
                        *(u16*)entry = 0;
                        *(s32*)(sound + 0xFC) = -1;
                        DVDCancelStream(entry + 0x118);
                        DVDClose(entry + 0x118);
                    }
                    *(u16*)entry |= 2;
                    *(u16*)entry &= ~0x40;
                    continue;
                } else {
                    f32 volume;
                    f32 left;
                    f32 right;
                    u32 pan;

                    *(f32*)(entry + 8) -= *(f32*)(entry + 0xC);
                    if (*(f32*)(entry + 8) <= float_0_80421908) {
                        *(f32*)(entry + 8) = float_0_80421908;
                    }
                    pan = *(u16*)(entry + 2) & 0xFF;
                    *(u16*)(base + offset + 2) = pan;
                    volume = *(f32*)(base + offset + 8);
                    left = volume;
                    right = volume;
                    if (pan < 0x40) {
                        right = ((f32)pan * volume) / float_63_8042190c;
                    }
                    if ((s32)(pan - 0x40) > -1) {
                        left = ((f32)(0x7F - pan) * volume) / float_63_8042190c;
                    }
                    AISetStreamVolLeft((u32)left);
                    AISetStreamVolRight((u32)right);
                }
            }

            flags = *(u16*)entry;
            if ((flags & 0x20) != 0) {
                if (float_0_80421908 == *(f32*)(entry + 8)) {
                    *(u16*)entry &= ~0x20;
                    if ((*(u16*)entry & 3) != 0) {
                        AISetStreamPlayState(0);
                        *(u16*)entry = 0;
                        *(s32*)(sound + 0xFC) = -1;
                        DVDCancelStream(entry + 0x118);
                        DVDClose(entry + 0x118);
                    }
                    continue;
                } else {
                    f32 volume;
                    f32 left;
                    f32 right;
                    u32 pan;

                    *(f32*)(entry + 8) -= *(f32*)(entry + 0xC);
                    if (*(f32*)(entry + 8) <= float_0_80421908) {
                        *(f32*)(entry + 8) = float_0_80421908;
                    }
                    pan = *(u16*)(entry + 2) & 0xFF;
                    *(u16*)(base + offset + 2) = pan;
                    volume = *(f32*)(base + offset + 8);
                    left = volume;
                    right = volume;
                    if (pan < 0x40) {
                        right = ((f32)pan * volume) / float_63_8042190c;
                    }
                    if ((s32)(pan - 0x40) > -1) {
                        left = ((f32)(0x7F - pan) * volume) / float_63_8042190c;
                    }
                    AISetStreamVolLeft((u32)left);
                    AISetStreamVolRight((u32)right);
                }
            }

            flags = *(u16*)entry;
            if ((flags & 0x10) != 0) {
                f32 volume;
                f32 left;
                f32 right;
                u32 pan;

                *(f32*)(entry + 8) += *(f32*)(entry + 0xC);
                if (*(f32*)(entry + 4) <= *(f32*)(entry + 8)) {
                    *(f32*)(entry + 8) = *(f32*)(entry + 4);
                    *(f32*)(entry + 0xC) = float_0_80421908;
                    *(u16*)entry &= ~0x10;
                }
                pan = *(u16*)(entry + 2) & 0xFF;
                *(u16*)(base + offset + 2) = pan;
                volume = *(f32*)(base + offset + 8);
                left = volume;
                right = volume;
                if (pan < 0x40) {
                    right = ((f32)pan * volume) / float_63_8042190c;
                }
                if ((s32)(pan - 0x40) > -1) {
                    left = ((f32)(0x7F - pan) * volume) / float_63_8042190c;
                }
                AISetStreamVolLeft((u32)left);
                AISetStreamVolRight((u32)right);
            }
        }
    }
}

s32 SoundSLibLoadDVD(s32 name) {
    extern char sound[];
    extern char str_436[];
    extern char str_PCTs_PCTsPCTs_802e3cd0[];
    extern char str_slib_8042192c[];
    extern char str_stbl_80421934[];
    extern char str_etbl_8042193c[];
    extern char* getMarioStDvdRoot(void);
    extern int sprintf(char* s, char* format, ...);
    extern void* DVDMgrOpen(char* path, s32 mode, s32 unk);
    extern u32 DVDMgrGetLength(void* handle);
    extern s32 DVDMgrRead(void* handle, void* dst, u32 size, u32 offset);
    extern void DVDMgrClose(void* handle);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void __memFree(s32 heap, void* ptr);

    void* handle;
    u32 length;
    u32 size;
    u8* data;
    u8* node;
    u8* scan;
    s16 idx;

    if (*(u32*)(sound + 0xE4) >= 8) {
        return 0;
    }

    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_slib_8042192c);
    handle = DVDMgrOpen(str_436, 2, 0);
    if (handle == NULL) {
        data = NULL;
    } else {
        length = DVDMgrGetLength(handle);
        if (length == 0) {
            data = NULL;
        } else {
            size = (length + 0x1F) & ~0x1F;
            data = __memAlloc(0, size);
            if (data == NULL) {
                data = NULL;
            } else if (DVDMgrRead(handle, data, size, 0) < 1) {
                __memFree(0, data);
                data = NULL;
            } else {
                DVDMgrClose(handle);
            }
        }
    }
    if (data == NULL) {
        return 0;
    }

    *(u8**)(sound + 0x60 + *(u32*)(sound + 0xE4) * 4) = data;
    while (*(s32*)data != -1) {
        node = __memAlloc(0, 0x10);
        if (node == NULL) {
            for (;;) {
            }
        }
        idx = 0;
        scan = *(u8**)(sound + 0xEC);
        if (scan != NULL) {
            u8* prev;
            do {
                prev = scan;
                scan = *(u8**)(prev + 0xC);
                idx++;
            } while (scan != NULL);
            *(u8**)(prev + 0xC) = node;
            scan = *(u8**)(sound + 0xEC);
        } else {
            scan = node;
        }
        *(u8**)(sound + 0xEC) = scan;
        *(u16*)node = 1;
        *(s16*)(node + 2) = idx;
        *(u16*)(node + 4) = 0;
        *(u16*)(node + 0xC) = 0;
        *(u8**)(node + 8) = data + 0x20;
        *(u32*)(node + 0xC) = 0;
        data += *(s32*)data;
    }

    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_stbl_80421934);
    handle = DVDMgrOpen(str_436, 2, 0);
    if (handle == NULL) {
        data = NULL;
    } else {
        length = DVDMgrGetLength(handle);
        if (length == 0) {
            data = NULL;
        } else {
            size = (length + 0x1F) & ~0x1F;
            data = __memAlloc(0, size);
            if (data == NULL) {
                data = NULL;
            } else if (DVDMgrRead(handle, data, size, 0) < 1) {
                __memFree(0, data);
                data = NULL;
            } else {
                DVDMgrClose(handle);
            }
        }
    }
    if (data == NULL) {
        return 0;
    }
    *(u8**)(sound + 0x80 + *(u32*)(sound + 0xE4) * 4) = data;

    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_etbl_8042193c);
    handle = DVDMgrOpen(str_436, 2, 0);
    if (handle == NULL) {
        data = NULL;
    } else {
        length = DVDMgrGetLength(handle);
        if (length == 0) {
            data = NULL;
        } else {
            size = (length + 0x1F) & ~0x1F;
            data = __memAlloc(0, size);
            if (data == NULL) {
                data = NULL;
            } else if (DVDMgrRead(handle, data, size, 0) < 1) {
                __memFree(0, data);
                data = NULL;
            } else {
                DVDMgrClose(handle);
            }
        }
    }
    if (data == NULL) {
        return 0;
    }
    *(u8**)(sound + 0xA0 + *(u32*)(sound + 0xE4) * 4) = data;
    *(u32*)(sound + 0xE4) += 1;
    return 1;
}

void _ssDVDReadAsync_cache_aram(int param_1) {
    extern char sound[];
    extern u32 DVDMgrGetLength(void*);
    extern void DVDMgrReadAsync(void*, void*, u32, u32, void*);
    extern void DCInvalidateRange(void*, u32);
    extern void ARQPostRequest(void*, void*, s32, s32, u32, void*, u32, void*);
    extern void _ssDVDReadAsync_activeChk(s32, void*);
    extern void _ssDVDReadAsync_cache_next(void);
    extern void cache_flush(void*);

    u8* entry;
    u32 length;
    s32 pos;
    s32 chunk;
    u32 readSize;

    entry = (u8*)(*(s32*)(sound + 0x100) + ((*(u32*)((s32)param_1 + 4) & 0xFF) * 0x138));
    length = DVDMgrGetLength(*(void**)(entry + 0x28));

    if (*(u32*)(entry + 0x60) < length) {
        length = DVDMgrGetLength(*(void**)(entry + 0x28));
        pos = *(s32*)(entry + 0x5C);
        chunk = 0x8000;
        if (length <= (u32)(pos + 0x8000)) {
            chunk = length - pos;
        }
        DVDMgrReadAsync(*(void**)(entry + 0x28), *(void**)(entry + 0x54),
                        (chunk + 0x1F) & ~0x1F, pos, _ssDVDReadAsync_cache_next);
        *(s32*)(entry + 0x70) = chunk;
    } else {
        u8* work;
        s32 add;

        *(s32*)(entry + 0x5C) = 0;
        *(s32*)(entry + 0x60) = 0;
        *(s32*)(entry + 0xE4) = 0;
        *(s32*)(entry + 0xE8) = 0;
        *(u16*)(entry + 0x74) = 1;

        work = entry + 0x28;
        pos = *(s32*)(entry + 0x5C);
        length = *(u32*)(entry + 0x88);
        chunk = 0x8000;
        if (length < (u32)(pos + 0x8000)) {
            chunk = length - pos;
        } else {
            s32 limit = 0x10000;
            s32 div = 2;
            s32 count = 4;
            while (count != 0) {
                if (length < (u32)(pos + limit)) {
                    u32 remain = length - (pos + (div - 1) * 0x8000);
                    chunk = 0x8000 - (0x8000 - remain) / div;
                    chunk += 0x20 - (chunk & 0x1F);
                    break;
                }
                div++;
                limit += 0x8000;
                count--;
            }
        }
        readSize = (chunk + 0x1F) & ~0x1F;
        if ((*(u16*)entry & 0x400) == 0) {
            DVDMgrReadAsync(*(void**)work, *(void**)(work + (1 - *(u16*)(entry + 0x74)) * 4 + 0x2C),
                            readSize, *(u16*)(entry + 4) + pos, _ssDVDReadAsync_activeChk);
        } else {
            s32 aramBase = *(s32*)(sound + 0x21C + *(u16*)(entry + 2) * 4);
            void* dst = *(void**)(work + (1 - *(u16*)(entry + 0x74)) * 4 + 0x2C);
            DCInvalidateRange(dst, 0x8000);
            ARQPostRequest(entry + 0x2C, work, 1, 0, *(u16*)(entry + 4) + pos + aramBase,
                           dst, readSize, cache_flush);
        }
        *(s32*)(entry + 0x70) = chunk;

        if (*(u16*)(entry + 0x1A) == 2) {
            *(u16*)(entry + 0xFC) = 1;
            work = entry + 0xB0;
            pos = *(s32*)(entry + 0xE4);
            length = *(u32*)(entry + 0x110);
            add = length + (0x20 - (length & 0x1F));
            chunk = 0x8000;
            if (length < (u32)(pos + 0x8000)) {
                chunk = length - pos;
            } else {
                s32 limit = 0x10000;
                s32 div = 2;
                s32 count = 4;
                while (count != 0) {
                    if (length < (u32)(pos + limit)) {
                        u32 remain = length - (pos + (div - 1) * 0x8000);
                        chunk = 0x8000 - (0x8000 - remain) / div;
                        chunk += 0x20 - (chunk & 0x1F);
                        break;
                    }
                    div++;
                    limit += 0x8000;
                    count--;
                }
            }
            readSize = (chunk + 0x1F) & ~0x1F;
            if ((*(u16*)entry & 0x400) == 0) {
                DVDMgrReadAsync(*(void**)work, *(void**)(work + (1 - *(u16*)(entry + 0xFC)) * 4 + 0x2C),
                                readSize, *(u16*)(entry + 4) + pos + add, _ssDVDReadAsync_activeChk);
            } else {
                s32 aramBase = *(s32*)(sound + 0x21C + *(u16*)(entry + 2) * 4);
                void* dst = *(void**)(work + (1 - *(u16*)(entry + 0xFC)) * 4 + 0x2C);
                DCInvalidateRange(dst, 0x8000);
                ARQPostRequest(entry + 0xB4, work, 1, 0, *(u16*)(entry + 4) + pos + add + aramBase,
                               dst, readSize, cache_flush);
            }
            *(s32*)(entry + 0xF8) = chunk;
        }
    }
}

void SoundEfxMain(void) {
    extern char sound[];
    extern double sin(double);
    extern double cos(double);
    extern void sndUpdateListener(void* listener, void* pos, void* delta, void* dir, void* up, u32 flags);
    extern void sndUpdateEmitter(void* emitter, void* pos, void* delta, u32 flags);
    extern u32 sndEmitterVoiceID(void* emitter);
    extern u32 sndFXCheck(u32 id);
    extern void sndFXKeyOff(u32 id);
    extern void sndRemoveEmitter(void* emitter);
    extern u32 sndCheckEmitter(void* emitter);
    extern const f32 float_0_80421908;
    extern const f32 float_1_80421914;
    extern const f32 float_6p2832_80421924;
    extern const f32 float_360_80421928;

    f32 oldX;
    f32 oldY;
    f32 oldZ;
    f32 angle;
    u8* entry;
    s32 i;

    oldX = *(f32*)(sound + 0x1C4);
    oldY = *(f32*)(sound + 0x1C8);
    oldZ = *(f32*)(sound + 0x1CC);
    angle = (float_6p2832_80421924 * *(f32*)(sound + 0x1F4)) / float_360_80421928;

    *(f32*)(sound + 0x1C4) = *(f32*)(sound + 0x1B8);
    *(f32*)(sound + 0x1C8) = *(f32*)(sound + 0x1BC);
    *(f32*)(sound + 0x1D0) = *(f32*)(sound + 0x1B8) - oldX;
    *(f32*)(sound + 0x1CC) = *(f32*)(sound + 0x1C0);
    *(f32*)(sound + 0x1D4) = *(f32*)(sound + 0x1BC) - oldY;
    *(f32*)(sound + 0x1D8) = *(f32*)(sound + 0x1C0) - oldZ;

    *(f32*)(sound + 0x1DC) = float_0_80421908 + float_1_80421914 * (f32)sin(angle);
    *(f32*)(sound + 0x1E0) = float_0_80421908;
    *(f32*)(sound + 0x1E4) = -(float_1_80421914 * (f32)cos(angle) - float_0_80421908);
    *(f32*)(sound + 0x1E8) = float_0_80421908;
    *(f32*)(sound + 0x1EC) = float_1_80421914;
    *(f32*)(sound + 0x1F0) = float_0_80421908;

    sndUpdateListener(sound + 0x128, sound + 0x1B8, sound + 0x1D0, sound + 0x1DC,
                      sound + 0x1E8, *(u16*)(sound + 0x204) & 0xFF);

    entry = *(u8**)(sound + 0xF4);
    for (i = 0; i < 0x28; i++, entry += 0x88) {
        if (*(u16*)entry != 0 && (*(u16*)entry & 0x10) != 0) {
            *(f32*)(entry + 0x7C) = *(f32*)(entry + 0x64) - *(f32*)(entry + 0x70);
            *(f32*)(entry + 0x80) = *(f32*)(entry + 0x68) - *(f32*)(entry + 0x74);
            *(f32*)(entry + 0x84) = *(f32*)(entry + 0x6C) - *(f32*)(entry + 0x78);
            *(f32*)(entry + 0x70) = *(f32*)(entry + 0x64);
            *(f32*)(entry + 0x74) = *(f32*)(entry + 0x68);
            *(f32*)(entry + 0x78) = *(f32*)(entry + 0x6C);
            sndUpdateEmitter(entry + 0x14, entry + 0x64, entry + 0x7C, *(u16*)(entry + 4) & 0xFF);
        }
    }

    entry = *(u8**)(sound + 0xF4);
    for (i = 0; i < 0x28; i++, entry += 0x88) {
        if (*(u16*)entry != 0) {
            if ((*(u16*)entry & 0x10) == 0) {
                u32 id;
                if ((*(u16*)entry & 0x10) == 0) {
                    id = sndFXCheck(*(u32*)(entry + 0xC));
                } else {
                    id = sndFXCheck(sndEmitterVoiceID(entry + 0x14));
                }
                id = sndFXCheck(id);
                if (id == 0xFFFFFFFF && *(u16*)entry != 0) {
                    sndFXKeyOff(*(u32*)(entry + 0xC));
                    if ((*(u16*)entry & 0x10) != 0) {
                        sndRemoveEmitter(entry + 0x14);
                    }
                    *(u16*)entry = 0;
                }
            } else {
                if (sndCheckEmitter(entry + 0x14) == 0 && *(u16*)entry != 0) {
                    sndFXKeyOff(*(u32*)(entry + 0xC));
                    if ((*(u16*)entry & 0x10) != 0) {
                        sndRemoveEmitter(entry + 0x14);
                    }
                    *(u16*)entry = 0;
                }
                if (*(u16*)(entry + 0xA) == 0) {
                    u32 id;
                    if ((*(u16*)entry & 0x10) == 0) {
                        id = sndFXCheck(*(u32*)(entry + 0xC));
                    } else {
                        id = sndFXCheck(sndEmitterVoiceID(entry + 0x14));
                    }
                    id = sndFXCheck(id);
                    if (id == 0xFFFFFFFF && *(u16*)entry != 0) {
                        sndFXKeyOff(*(u32*)(entry + 0xC));
                        if ((*(u16*)entry & 0x10) != 0) {
                            sndRemoveEmitter(entry + 0x14);
                        }
                        *(u16*)entry = 0;
                    }
                } else {
                    *(u16*)(entry + 0xA) -= 1;
                }
            }
        }
    }
}

s32 SoundLoadDVD2(s32 name) {
    extern char sound[];
    extern char str_436[];
    extern char str_PCTs_PCTsPCTs_802e3cd0[];
    extern char str_pool_80421944[];
    extern char str_proj_8042194c[];
    extern char str_sdir_80421954[];
    extern char str_samp_8042195c[];
    extern char* getMarioStDvdRoot(void);
    extern int sprintf(char* s, char* format, ...);
    extern char* strcpy(char* dst, char* src);
    extern void* DVDMgrOpen(char* path, s32 mode, s32 unk);
    extern u32 DVDMgrGetLength(void* handle);
    extern s32 DVDMgrRead(void* handle, void* dst, u32 size, u32 offset);
    extern void DVDMgrClose(void* handle);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void __memFree(s32 heap, void* ptr);
    extern void sndSetSampleDataUploadCallback(void* callback, s32 size);
    extern void loadDVD_callback(void);

    void* handle;
    u32 length;
    u32 size;
    u8* data;

    if (*(u32*)(sound + 0xE0) >= 8) {
        return 0;
    }

    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_pool_80421944);
    handle = DVDMgrOpen(str_436, 2, 0);
    if (handle == NULL) {
        data = NULL;
    } else {
        length = DVDMgrGetLength(handle);
        if (length == 0) {
            data = NULL;
        } else {
            size = (length + 0x1F) & ~0x1F;
            data = __memAlloc(0, size);
            if (data == NULL) {
                data = NULL;
            } else if (DVDMgrRead(handle, data, size, 0) < 1) {
                __memFree(0, data);
                data = NULL;
            } else {
                DVDMgrClose(handle);
            }
        }
    }
    if (data == NULL) {
        return 0;
    }
    *(u8**)(sound + *(u32*)(sound + 0xE0) * 4) = data;

    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_proj_8042194c);
    handle = DVDMgrOpen(str_436, 2, 0);
    if (handle == NULL) {
        data = NULL;
    } else {
        length = DVDMgrGetLength(handle);
        if (length == 0) {
            data = NULL;
        } else {
            size = (length + 0x1F) & ~0x1F;
            data = __memAlloc(0, size);
            if (data == NULL) {
                data = NULL;
            } else if (DVDMgrRead(handle, data, size, 0) < 1) {
                __memFree(0, data);
                data = NULL;
            } else {
                DVDMgrClose(handle);
            }
        }
    }
    if (data == NULL) {
        return 0;
    }
    *(u8**)(sound + 0x20 + *(u32*)(sound + 0xE0) * 4) = data;

    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_sdir_80421954);
    handle = DVDMgrOpen(str_436, 2, 0);
    if (handle == NULL) {
        data = NULL;
    } else {
        length = DVDMgrGetLength(handle);
        if (length == 0) {
            data = NULL;
        } else {
            size = (length + 0x1F) & ~0x1F;
            data = __memAlloc(0, size);
            if (data == NULL) {
                data = NULL;
            } else if (DVDMgrRead(handle, data, size, 0) < 1) {
                __memFree(0, data);
                data = NULL;
            } else {
                DVDMgrClose(handle);
            }
        }
    }
    if (data == NULL) {
        return 0;
    }
    *(u8**)(sound + 0x40 + *(u32*)(sound + 0xE0) * 4) = data;

    sprintf(str_436, str_PCTs_PCTsPCTs_802e3cd0, getMarioStDvdRoot(), name, str_samp_8042195c);
    strcpy(sound + 0x22A, str_436);
    sndSetSampleDataUploadCallback(loadDVD_callback, 0x20000);
    return 1;
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

