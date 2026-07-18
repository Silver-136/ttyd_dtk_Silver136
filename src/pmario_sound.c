#include "pmario_sound.h"

//.bss
u8 _buf[0x80];
PaperSoundWork psnd;
PaperSoundEnv psenv[16];
PaperSoundEffect pssfx[40];
PaperSoundBGM psbgm[2];

//.data
PaperSoundEffectData pssfxlist[3042];

//local prototypes
s32 __psndSFXOn(s32 lookup, u8 volume, u8 a3, u16 a4, Vec* position, u32 a6, u16 distance, s32 a8);

s32 __psndSFXOn(s32 lookup, u8 volume, u8 a3, u16 a4, Vec* position, u32 a6, u16 distance, s32 a8) {
    extern int strcmp(const char*, const char*);
    extern s32 evtGetValue(void*, s32);
    extern void SoundEfxStop(s32);
    extern void SoundSongStopCh(s32);
    extern void SoundSSStopCh(s32);
    extern void* gp;
    extern const f32 float_1000_804218cc;

    u8* listBase = (u8*)pssfxlist;
    u8* entry;
    PaperSoundEffect* slot;
    PaperSoundEffect* found;
    PaperSoundEffect* scan;
    u32 index;
    u32 ret;
    u32 extra;
    s32 i;

    found = NULL;
    ret = 0;

    if (lookup < 0) {
        for (i = 0; i < 0x1092; i++) {
            if (strcmp(*(char**)(listBase + i * 0x14), (char*)lookup) == 0) {
                break;
            }
        }
        lookup = i;
        if (lookup >= 0x1092) {
            lookup = 0;
        }
    }

    index = lookup & 0x1FFF;
    entry = listBase + index * 0x14;
    if (*(u32*)(entry + 4) == 0) {
        return -1;
    }
    if ((*(u16*)((s32)&psnd + 0x56) & 4) != 0 && a8 == 0) {
        return -1;
    }

    if (index == 0x5B || index == 0x8CA || (index - 0x1BE) < 2 || index == 0x205) {
        if (*(s8*)((s32)&psnd + 0x5C) != 0) {
            return -1;
        }
        *(s8*)((s32)&psnd + 0x5C) = 6;
    }

    if ((s32)evtGetValue(NULL, -170000000) < 3 && index == 0x365) {
        return -1;
    }

    extra = 0;
    scan = pssfx;
    for (i = 0; i < 0x28; i += 2) {
        if (scan[0].listIndex != (u32)-1 && (scan[0].listIndex & 0x1FFF) == index &&
            (scan[0].unk6 & 0x100) != 0 && extra == 0) {
            *(u16*)((s32)&psnd + 0x54) += 8;
            extra = *(u16*)((s32)&psnd + 0x54);
        }
        if (scan[0].listIndex == (u32)-1) {
            ret = i;
            found = &scan[0];
        }
        if (scan[1].listIndex != (u32)-1 && (scan[1].listIndex & 0x1FFF) == index &&
            (scan[1].unk6 & 0x100) != 0 && extra == 0) {
            *(u16*)((s32)&psnd + 0x54) += 8;
            extra = *(u16*)((s32)&psnd + 0x54);
        }
        if (scan[1].listIndex == (u32)-1) {
            ret = i + 1;
            found = &scan[1];
        }
        scan += 2;
    }

    if (found == NULL) {
        scan = pssfx;
        for (i = 0; i < 0x28; i++, scan++) {
            ret = i;
            found = scan;
            if (scan->listIndex == (u32)-1) {
                break;
            }
            if ((u16)*(u8*)(entry + 0x10) < *(u16*)((s32)scan + 0xC)) {
                if (ret != (u32)-1) {
                    u32 sfxIndex = ret & 0xFF;
                    PaperSoundEffect* old = &pssfx[sfxIndex];
                    if (old->listIndex != (u32)-1) {
                        if ((old->unk6 & 1) == 0) {
                            s32 effectId = old->effectId;
                            if (effectId != -1) {
                                u32 flags = pssfxlist[old->listIndex & 0x1FFF].unk4;
                                if ((flags & 0x80000000) != 0) {
                                    SoundSSStopCh(effectId);
                                } else if ((flags & 0x40000000) != 0) {
                                    SoundSongStopCh(effectId);
                                } else {
                                    SoundEfxStop(effectId);
                                }
                            }
                            old->listIndex = (u32)-1;
                            old->effectId = -1;
                        } else {
                            old->listIndex = (u32)-1;
                            old->effectId = -1;
                        }
                    }
                }
                break;
            }
        }
        if (found == NULL) {
            return -1;
        }
    }

    if (volume == 0xFF) {
        volume = 0x7F;
    }
    if (a3 == 0xFF) {
        a3 = *(u8*)(entry + 0xD);
    }

    found->unk6 = 0x101;
    if ((*(u16*)((s32)&psnd + 0x56) & 0x10) != 0) {
        found->unk6 |= 0x10;
    }
    if ((lookup & 0x4000000) != 0) {
        found->unk6 |= 0x20;
    }
    if (index == 0x518 || index == 0x742) {
        found->unk6 |= 0x280;
    }
    if (*(u8*)(entry + 0xE) == 1) {
        found->unk6 |= 0x200;
    }
    if ((lookup & 0x10000) != 0) {
        found->unk6 |= 0x400;
    }
    if ((lookup & 0x20000) != 0) {
        found->unk6 |= 0x800;
    }
    if ((lookup & 0x40000) != 0) {
        found->unk6 |= 0x80;
    }

    found->listIndex = lookup;
    found->volume = volume;
    *(u8*)((s32)found + 9) = *(u8*)(entry + 0xC);
    *(u8*)((s32)found + 0xA) = a3;
    *(s16*)((s32)found + 0xE) =
        (s16)((f32)*(u8*)(entry + 0x11) + (f32)extra +
              ((f32)a4 / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4))));
    *(s16*)((s32)found + 0x10) = (s16)extra * 5;
    *(u8*)((s32)found + 0xB) = *(u8*)(entry + 0xF);
    *(u16*)((s32)found + 0xC) = *(u8*)(entry + 0x10);
    found->pitch = *(u16*)(entry + 0x12);

    if (position != NULL) {
        found->position = *position;
        if (a6 == 0) {
            found->unk6 |= 2;
        } else {
            found->unk6 |= 4;
        }
    }
    *(s16*)((s32)found + 0x12) = distance;
    *(s16*)((s32)found + 0x16) = 32000;
    *(u16*)((s32)&psnd + 0x50) += 1;
    *(u16*)((s32)found + 4) = *(u16*)((s32)&psnd + 0x50);
    return ret | (*(u16*)((s32)&psnd + 0x50) << 8);
}

u32 psndSFXOn_3D(s32 lookup, Vec* position) {
    return __psndSFXOn(lookup, 0xFFu, 0xFFu, 0, position, 0, 0x10u, 0);
}

void psndSFX_vol(u32 index, u8 volume) {
    PaperSoundEffect* sfx;

    if (index != -1) {
        sfx = &pssfx[(u8)index];
        if (sfx->listIndex != -1) {
            sfx->volume = volume;
        }
    }
}

void psndSFX_pit(u32 index, u32 pitch) {
    PaperSoundEffect* sfx;

    if (index != -1) {
        sfx = &pssfx[(u8)index];
        if (sfx->listIndex != -1) {
            if ((sfx->unk6 & 1) != 0) {
                sfx->pitch = pitch;
            }
            else {
                PaperSoundEffectData* sfxdata = &pssfxlist[sfx->listIndex & 0x1FFF];
                if ((sfxdata->unk4 & 0x80000000) == 0 && (sfxdata->unk4 & 0x80000000) == 0) {
                    SoundEfxSetPitch(sfx->effectId, pitch);
                }
            }
        }
    }
}

u32 psndGetFlag(void) {
    return *(u16*)((s32)&psnd + 0x56);
}

void psndClearFlag(u32 flag) {
    *(u16*)((s32)&psnd + 0x56) &= ~flag;
}

void psndBGMOff(s32 id) {
    extern void psndBGMOff_f_d(s32 id, s32 fade, s32 unk);
    psndBGMOff_f_d(id, 1000, 0);
}

void psndENVOff(s32 id) {
    extern void psndENVOff_f_d(s32 id, s32 fade, s32 unk);
    psndENVOff_f_d(id, 1000, 0);
}

void psndENVOn(s32 id, s32 a2) {
    extern void psndENVOn_f_d(s32 id, s32 a2, s32 fade, s32 unk);
    psndENVOn_f_d(id, a2, 1000, 0);
}

void psndBGMOn(s32 id, s32 a2) {
    extern void psndBGMOn_f_d(s32 id, s32 a2, s32 fade, s32 unk1, s32 unk2);
    psndBGMOn_f_d(id, a2, 1000, 750, 0);
}

s32 psndSFXOnVol(s32 lookup, u8 volume) {
    return __psndSFXOn(lookup, volume, 0xFF, 0, 0, 0, 0x10, 0);
}

s32 psndSFXOn(s32 lookup) {
    return __psndSFXOn(lookup, 0xFF, 0xFF, 0, 0, 0, 0x10, 0);
}

s32 psndSFXOn_(s32 lookup) {
    return __psndSFXOn(lookup, 0xFF, 0xFF, 0, 0, 0, 0x10, 1);
}

void psndSFX_dist(u32 index, u16 distance) {
    PaperSoundEffect* sfx;
    if (index != -1) {
        sfx = &pssfx[(u8)index];
        if (sfx->listIndex != -1) {
            *(u16*)((s32)sfx + 0x12) = distance;
        }
    }
}

void psndSetReverb(s32 mode) {
    switch (mode) {
    case 0:
        *(s16*)((s32)&psnd + 0x52) = 0;
        break;
    case 1:
        *(s16*)((s32)&psnd + 0x52) = 0x14;
        break;
    case 2:
        *(s16*)((s32)&psnd + 0x52) = 0x28;
        break;
    case 3:
        *(s16*)((s32)&psnd + 0x52) = 0x3C;
        break;
    }
}

void psndSetPosDirListener(Vec* pos, f32 dir) {
    *(Vec*)((s32)&psnd + 0x30) = *pos;
    *(f32*)((s32)&psnd + 0x3C) = dir;
}

u8 psndSFX_get_vol(u32 index) {
    PaperSoundEffect* sfx;
    if (index == -1) {
        return 0;
    }
    sfx = &pssfx[(u8)index];
    if (sfx->listIndex == -1) {
        return 0;
    }
    return sfx->volume;
}

void psndSFX_pos(u32 index, Vec* pos) {
    PaperSoundEffect* sfx;
    if (index != -1) {
        sfx = &pssfx[(u8)index];
        if (sfx->listIndex != -1) {
            sfx->position = *pos;
        }
    }
}

void psndExit(void) {
    extern void SoundDropData(void);
    extern void DVDMgrSetupCallback(void*);
    extern s32 init_f;
    SoundDropData();
    DVDMgrSetupCallback(0);
    init_f = 0;
}

void psndMainInt(void) {
    extern void SoundMainInt(void);
    extern s32 init_f;
    if (init_f != 0) {
        SoundMainInt();
    }
}

void unk_800db778(s32 id, s32 a2, s32 a3, s32 a4) {
    extern void psndBGMOn_f_d(s32, s32, s32, s32, s32);
    psndBGMOn_f_d(id, a2, a3, 750, a4);
}

s32 psndSFXOnEx_3D(s32 lookup, u8 volume, u8 a3, u16 a4, Vec* position, u16 distance) {
    return __psndSFXOn(lookup, a3, a4, (u16)(s32)position, (Vec*)(s32)volume, 0, distance, 0);
}

s32 psndBGMChk(s32 index) {
    s32 value;

    value = *(s32*)((s32)&psbgm[index & 0xF] + 0);
    return (u32)(~((value + 1) | (-1 - value))) >> 31;
}

s32 psndBGMChkSilent(s32 index) {
    PaperSoundBGM* bgm = &psbgm[index & 0xF];
    s32 value = *(s32*)bgm;

    if ((u32)(value + 0x10000) == 0xFFFF) {
        return 0;
    }

    return *(u8*)((s32)bgm + 0x26) == 8;
}

s32 psndBGMStartCheck(s32 index) {
    extern s32 SoundSSCheck(s32 id);
    PaperSoundBGM* bgm = &psbgm[index & 0xF];
    s32 value = *(s32*)bgm;

    if ((u32)(value + 0x10000) == 0xFFFF) {
        return 1;
    }

    return (SoundSSCheck(*(s32*)((s32)bgm + 4)) >> 3) & 1;
}

u8 psndENVMain(void) {
    extern s32 seqGetSeq(void);
    extern s32 irand(u32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void SoundEfxSetLPF(s32, s16);
    extern void* gp;
    u8* snd = (u8*)&psnd;
    u8* env = (u8*)psenv;
    s32 target = ((*(u16*)(snd + 0x56) & 0x20) != 0 && (*(u16*)(snd + 0x56) & 0x40) == 0) ? 80 : 100;
    s32 i;

    if (seqGetSeq() != 3 && (*(u32*)((s32)gp + 0x1C) & 1) != 0) {
        if (*(s8*)(snd + 0x5A) < target) (*(s8*)(snd + 0x5A))++;
        if (*(s8*)(snd + 0x5A) > target) (*(s8*)(snd + 0x5A))--;
    }
    for (i = 0; i < 16; i++, env += 0x24) {
        s16 id = *(s16*)(env + 4);
        u8 mode = *(u8*)(env + 8);
        if (id == 0) continue;
        switch (mode) {
        case 0:
            switch (*(u8*)(env + 0xD)) {
            case 0:
                (*(u16*)(env + 0xE))--;
                if (*(u16*)(env + 0xE) == 0) {
                    (*(u8*)(env + 0xD))++;
                }
                break;
            case 1: {
                s32 volume = (*(u8*)(env + 6) * *(s8*)(snd + 0x5A)) / 100;
                f32 angle = (f32)irand(360) * 6.2832f / 360.0f;
                f32 radius = (f32)(*(u8*)(env + 0xB) * 10 + irand(*(u8*)(env + 0xC) * 10));
                *(f32*)(env + 0x18) += radius * (f32)sin(angle);
                *(f32*)(env + 0x1C) = 0.0f;
                *(f32*)(env + 0x20) -= radius * (f32)cos(angle);
                *(u32*)(env + 0x14) = __psndSFXOn(*(u32*)env | 0x5000000,
                                                   volume, 0xFF, 0, (Vec*)(env + 0x18),
                                                   0, *(u16*)(env + 0x10), 0);
                (*(u8*)(env + 0xD))++;
                break;
            }
            case 2:
                if (*(u32*)(env + 0x14) == 0xFFFFFFFF) {
                    *(u8*)(env + 0xD) = 0;
                    *(u16*)(env + 0xE) = *(u8*)(env + 0xA) * 60 +
                                         irand(*(u8*)(env + 9)) * 60;
                }
                break;
            }
            break;
        case 1:
            if (*(u8*)(env + 0xD) == 0) {
                s32 volume = (*(u8*)(env + 6) * *(s8*)(snd + 0x5A)) / 100;
                *(u32*)(env + 0x14) = __psndSFXOn(*(u32*)env | 0x4000000,
                                                   volume, 0xFF, 0, (Vec*)(env + 0x18),
                                                   0, *(u16*)(env + 0x10), 0);
                (*(u8*)(env + 0xD))++;
            } else if (*(u8*)(env + 0xD) == 1) {
                u32 handle = *(u32*)(env + 0x14);
                if (handle == 0xFFFFFFFF ||
                    (pssfx[handle & 0xFF].effectId == -1 &&
                     (pssfx[handle & 0xFF].unk6 & 1) == 0)) {
                    *(u32*)(env + 0x14) = 0xFFFFFFFF;
                    *(u8*)(env + 0xD) = 0;
                    *(u16*)(env + 0xE) = 0;
                }
            }
            break;
        case 2:
            if (*(u8*)(env + 0xD) == 0) {
                (*(u16*)(env + 0xE))--;
                if (*(u16*)(env + 0xE) == 0) {
                    (*(u8*)(env + 0xD))++;
                }
            } else if (*(u8*)(env + 0xD) == 1) {
                s32 volume = (*(u8*)(env + 6) * *(s8*)(snd + 0x5A)) / 100;
                *(u32*)(env + 0x14) = __psndSFXOn(*(u32*)env | 0x4000000,
                                                   volume, 0xFF, 0, (Vec*)(env + 0x18),
                                                   0, *(u16*)(env + 0x10), 0);
                (*(u8*)(env + 0xD))++;
            } else if (*(u8*)(env + 0xD) == 2) {
                u32 handle = *(u32*)(env + 0x14);
                if (handle == 0xFFFFFFFF ||
                    (pssfx[handle & 0xFF].effectId == -1 &&
                     (pssfx[handle & 0xFF].unk6 & 1) == 0)) {
                    *(u32*)(env + 0x14) = 0xFFFFFFFF;
                    *(u8*)(env + 0xD) = 0;
                    *(u16*)(env + 0xE) = *(u8*)(env + 0xA) * *(s32*)((s32)gp + 4) +
                                         (s16)*(s32*)((s32)gp + 4) *
                                             (s16)irand(*(u8*)(env + 9));
                }
            }
            break;
        case 3:
            if (*(u8*)(env + 0xD) == 0) {
                s32 volume = (*(u8*)(env + 6) * *(s8*)(snd + 0x5A)) / 100;
                *(u32*)(env + 0x14) = __psndSFXOn(*(u32*)env | 0x6000000,
                                                   volume, 0xFF, 0, (Vec*)(env + 0x18),
                                                   0, *(u16*)(env + 0x10), 0);
                (*(u8*)(env + 0xD))++;
            } else if (*(u8*)(env + 0xD) == 1) {
                u32 handle = *(u32*)(env + 0x14);
                if (handle == 0xFFFFFFFF ||
                    (pssfx[handle & 0xFF].effectId == -1 &&
                     (pssfx[handle & 0xFF].unk6 & 1) == 0)) {
                    *(u32*)(env + 0x14) = 0xFFFFFFFF;
                    *(u8*)(env + 0xD) = 0;
                    *(u16*)(env + 0xE) = 0;
                }
            }
            break;
        }
    }

    env = (u8*)psenv;
    for (i = 0; i < 16; i++, env += 0x24) {
        u32 handle;
        u32 sfxIndex;
        s32 volume;
        s16 lpf;

        if (*(u16*)(env + 4) == 0 || *(s32*)(env + 0x14) == -1) {
            continue;
        }
        if ((*(u8*)(env + 8) == 0 && *(u8*)(env + 0xD) != 2) ||
            (*(u8*)(env + 8) == 1 && *(u8*)(env + 0xD) != 1) ||
            (*(u8*)(env + 8) == 2 && *(u8*)(env + 0xD) != 2) ||
            (*(u8*)(env + 8) == 3 && *(u8*)(env + 0xD) != 1)) {
            continue;
        }

        volume = (*(u8*)(env + 6) * *(s8*)(snd + 0x5A)) / 100;
        handle = *(u32*)(env + 0x14);
        sfxIndex = handle & 0xFF;
        if (pssfx[sfxIndex].listIndex != -1) {
            pssfx[sfxIndex].volume = volume;
        }

        lpf = 0;
        if (*(u16*)(snd + 0x18) == *(u16*)(env + 4)) {
            lpf = *(s16*)(snd + 0x1C);
        }
        if (*(u16*)(snd + 0x1A) == *(u16*)(env + 4)) {
            lpf = *(s16*)(snd + 0x1E);
        }
        if (pssfx[sfxIndex].listIndex != -1) {
            *(s16*)((u8*)&pssfx[sfxIndex] + 0x16) = lpf;
            if ((pssfx[sfxIndex].unk6 & 1) == 0 &&
                (pssfxlist[pssfx[sfxIndex].listIndex & 0x1FFF].unk4 & 0x80000000) == 0) {
                SoundEfxSetLPF(pssfx[sfxIndex].effectId, lpf);
            }
        }
    }
    return 0;
}

u8 psndSFXMain(void) {
    extern u32 SoundSSCheck(s32);
    extern u32 SoundSongCheck(s32);
    extern u32 SoundEfxCheck(s32);
    extern void SoundSSSetPanCh(s32, u8);
    extern void SoundSSSetVolCh(s32, u8);
    extern void SoundSSSetSrndPanCh(s32, u8);
    extern void SoundSongSetVolCh(s32, u8);
    extern void SoundEfxSetPan(s32, u8);
    extern void SoundEfxSetVolume(s32, u8);
    extern s32 seqGetSeq(void);
    extern void* gp;
    u8* snd = (u8*)&psnd;
    u8* entry = (u8*)pssfx;
    u8* list = (u8*)pssfxlist;
    s32 i;

    for (i = 0; i < 40; i++, entry += 0x28) {
        s32 listId = *(s32*)entry;
        s32 handle = *(s32*)(entry + 0x18);
        u32 kind;
        s32 volume;
        if (listId == -1 || (*(u16*)(entry + 6) & 1) != 0) continue;
        kind = *(u32*)(list + (listId & 0x1FFF) * 0x14 + 4);
        if ((kind & 0x80000000) != 0) {
            if (SoundSSCheck(handle) == 0) { *(s32*)entry = -1; *(s32*)(entry + 0x18) = -1; continue; }
        } else if ((kind & 0x40000000) != 0) {
            if (SoundSongCheck(handle) == 0) { *(s32*)entry = -1; *(s32*)(entry + 0x18) = -1; continue; }
        } else if (SoundEfxCheck(handle) == 0) {
            *(s32*)entry = -1; *(s32*)(entry + 0x18) = -1; continue;
        }
        volume = (*(u8*)(entry + 8) * *(u8*)(entry + 9)) / 127 - *(u16*)(entry + 0x10);
        if (volume < 0) volume = 0;
        if (volume > 127) volume = 127;
        if ((kind & 0x80000000) != 0) {
            SoundSSSetPanCh(handle, *(u8*)(entry + 0xA));
            SoundSSSetVolCh(handle, volume);
            SoundSSSetSrndPanCh(handle, *(u8*)(entry + 0xB));
        } else if ((kind & 0x40000000) != 0) {
            SoundSongSetVolCh(handle, volume);
        } else {
            SoundEfxSetPan(handle, *(u8*)(entry + 0xA));
            SoundEfxSetVolume(handle, volume);
        }
    }
    if (*(u8*)(snd + 0x24) == 2 && --*(s16*)(snd + 0x28) < 1) *(u8*)(snd + 0x24) = 0;
    if (seqGetSeq() != 3 && (*(u32*)((s32)gp + 0x1C) & 1) != 0) {
        s8 target = ((*(u16*)(snd + 0x56) & 0x20) != 0 && (*(u16*)(snd + 0x56) & 0x40) == 0) ? 120 : 100;
        if (*(s8*)(snd + 0x59) < target) (*(s8*)(snd + 0x59))++;
        if (*(s8*)(snd + 0x59) > target) (*(s8*)(snd + 0x59))--;
    }
    return 0;
}

void psndBGMOn_f_d(s32 id, s32 a2, s32 fade, s32 fadeOut, s32 unused) {
    extern u8 psbgmlist[];
    extern s32 evtGetValue(void* evt, s32 value);
    extern s32 strcmp(const char* a, const char* b);
    extern void SoundSetFadeTime(s16 in, s16 out);
    extern void SoundSongContinueCh(s32 ch);
    extern void SoundSSContinueCh(s32 ch);
    extern void SoundSongFadeoutCh(s32 ch);
    extern void SoundSSFadeoutCh(s32 ch);
    extern u32 SoundSongCheck(s32 ch);
    extern u32 SoundSSCheck(s32 ch);
    extern void SoundSSStopCh(s32 ch);
    extern void SoundSongPlayCh(s32 ch, u32 song);
    extern void SoundSongSetVolCh(s32 ch, u8 volume);
    extern void SoundSSPlayCh(s32 ch, const char* path);
    extern void SoundSSMuteOn(s32 ch);
    extern void SoundSSSetVolCh(s32 ch, u8 volume);
    extern void SoundSSSetPanCh(s32 ch, u8 pan);
    extern void SoundSSSetSrndPanCh(s32 ch, u8 pan);
    extern void psndBGMMain(void);
    extern void psndBGMOff_f_d(s32 id, s32 fade, s32 unk);
    extern char str_mri_00_802e3c70[];
    extern void* gp;
    u8* bgm;
    u8* list;
    s32 story;
    s32 old;
    s32 channel;
    s32 i;
    u32 flags;
    (void)unused;

    if ((*(u16*)((s32)&psnd + 0x56) & 1) != 0) {
        return;
    }
    if (a2 < 0) {
        for (i = 0; i < 0x106; i++) {
            if (strcmp((char*)(psbgmlist + i * 0x10), (char*)a2) == 0) {
                a2 = i;
                break;
            }
        }
        if (i == 0x106) {
            a2 = 0;
        }
    }

    channel = id & 0xF;
    bgm = (u8*)&psbgm[channel];

    story = evtGetValue(0, -170000000);
    if (story > 100) {
        story = evtGetValue(0, -170000000);
        if (story < 0x66 || evtGetValue(0, -170000000) >= 0x6A) {
            if (strcmp((char*)((s32)gp + 0x12C), str_mri_00_802e3c70) != 0 && a2 == 0x4F) {
                a2 = 0x50;
            }
        }
    }

    story = evtGetValue(0, -170000000);
    if (story > 0x100 && evtGetValue(0, -170000000) < 0x104 && (a2 == 0x5A || a2 == 0x5B)) {
        a2 = 0xA4;
    }

    story = evtGetValue(0, -170000000);
    if (story > 0x6C && evtGetValue(0, -170000000) < 0x6F) {
        if (a2 == 0x4F) {
            a2 = 0xA4;
        }
        if (a2 == 0x50) {
            a2 = 0xA4;
        }
        if (a2 == 0x7B || a2 == 0x7D || a2 == 0x7E) {
            return;
        }
        if (channel == 0 && (id & 0x20) != 0 && *(u8*)((s32)&psbgm[0] + 0x20) != 4) {
            return;
        }
        if ((a2 == 0x90 || a2 == 0x91) && *(s32*)&psbgm[0] == 0xA4) {
            return;
        }
        if (a2 == 0x92 && *(s32*)&psbgm[0] == 0xA4) {
            psndBGMOff_f_d(0x400, 0x2EE, 1);
        }
    }

    story = evtGetValue(0, -170000000);
    if (story > 0x7E && evtGetValue(0, -170000000) < 0xB2) {
        if (a2 == 0x8C) {
            return;
        }
        if ((a2 == 0x90 || a2 == 0x91) && *(s32*)&psbgm[0] == 0xC3) {
            return;
        }
        if (a2 == 0 && *(s32*)&psbgm[0] == 0xC3 && (id & 0x20) == 0) {
            return;
        }
        if (a2 == 0x92 && *(s32*)&psbgm[0] == 0xC3) {
            psndBGMOff_f_d(0x400, 0x2EE, 1);
        }
    }

    story = evtGetValue(0, -170000000);
    if (story > 0xA3 && a2 == 0x53) {
        a2 = 0x54;
    }

    if (a2 == 0x90 || a2 == 0x91) {
        s32 bgm1 = *(s32*)&psbgm[1];
        if (bgm1 == 0x80 || (u32)(bgm1 - 0x81) < 0xC || bgm1 == 0x8E || bgm1 == 0x8D) {
            return;
        }
    }

    list = psbgmlist + a2 * 0x10;
    flags = *(u32*)(list + 4);
    if (flags == 0 && (id & 0x20) == 0) {
        return;
    }
    SoundSetFadeTime((s16)fade, (s16)fade);
    old = *(s32*)bgm;
    if (old != -1) {
        if ((id & 0x10) == 0 && (id & 0x20) != 0) {
            u32 oldFlags = *(u32*)(psbgmlist + old * 0x10 + 4);
            if ((oldFlags & 0x80000000) == 0) {
                SoundSongContinueCh(*(s32*)(bgm + 4));
            } else {
                SoundSSContinueCh(*(s32*)(bgm + 4));
            }
            *(u8*)(bgm + 0x20) = (id & 0x100) != 0;
            *(u16*)(bgm + 0x22) = (id & 0x100) != 0 ? fade / (1000 / *(s32*)((s32)gp + 4)) : 1;
            *(u16*)(bgm + 0x24) = (id & 0x100) == 0;
            if ((id & 0x80) != 0 && *(u8*)(bgm + 0x26) == 8) {
                s32 scale;
                *(u8*)(bgm + 0x26) = 0x10;
                *(u16*)(bgm + 0x28) = fade / (1000 / *(s32*)((s32)gp + 4));
                scale = (s16)*(u16*)(bgm + 0x28) * *(u8*)(bgm + 0x27);
                *(u16*)(bgm + 0x2A) = (s16)(scale / 100);
                if ((id & 0x40) != 0) {
                    *(u32*)(bgm + 8) |= 0x40;
                } else {
                    *(u32*)(bgm + 8) &= ~0x40;
                }
            }
            psndBGMMain();
            return;
        }
        if ((id & 0x10) == 0 && old == a2 && *(u8*)(bgm + 0x20) != 2) {
            psndBGMMain();
            return;
        }
        if ((id & 0x10) == 0 && *(u8*)(list + 0xE) != 0 && *(u8*)(list + 0xE) == *(u8*)(bgm + 0xD)) {
            psndBGMMain();
            return;
        }
        if ((*(u32*)(psbgmlist + old * 0x10 + 4) & 0x80000000) == 0) {
            SoundSetFadeTime(0, (s16)fadeOut);
            SoundSongFadeoutCh(*(s32*)(bgm + 4));
        } else {
            SoundSetFadeTime(0, (s16)((id & 0x10) ? 100 : fadeOut));
            SoundSSFadeoutCh(*(s32*)(bgm + 4));
        }
    }
    if (a2 == 0) {
        return;
    }

    *(s32*)bgm = a2;
    *(u32*)(bgm + 8) = id;
    *(u8*)(bgm + 0xD) = *(u8*)(list + 0xE);
    *(u8*)(bgm + 0xE) = *(u8*)(list + 0xC);
    *(u8*)(bgm + 0xF) = *(u8*)(list + 0xD);
    *(u8*)(bgm + 0x10) = *(u8*)(list + 0xF) == 1 ? 0xFF : 0;
    *(u32*)(bgm + 0x30) = *(u32*)((s32)gp + 0x38);
    *(u32*)(bgm + 0x34) = *(u32*)((s32)gp + 0x3C);
    if ((flags & 0x80000000) == 0) {
        for (i = 0; i < 4 && SoundSongCheck(i) != 0; i++) {
        }
        if (i == 4) {
            *(s32*)bgm = -1;
            return;
        }
        *(s32*)(bgm + 4) = i;
        SoundSongPlayCh(i, flags & 0x0FFFFFFF);
        SoundSongSetVolCh(i, *(u8*)(list + 0xC));
    } else {
        for (i = 0; i < 3 && SoundSSCheck(i) != 0; i++) {
        }
        if (i == 3) {
            for (i = 0; i < 3; i++) {
                if ((SoundSSCheck(i) & 0x20) != 0) {
                    SoundSSStopCh(i);
                }
            }
            for (i = 0; i < 3 && SoundSSCheck(i) != 0; i++) {
            }
        }
        if (i == 3) {
            *(s32*)bgm = -1;
            return;
        }
        *(s32*)(bgm + 4) = i;
        SoundSSPlayCh(i, *(char**)(list + 8));
        SoundSSMuteOn(i);
        SoundSSSetVolCh(i, *(u8*)(list + 0xC));
        SoundSSSetPanCh(i, *(u8*)(list + 0xD));
        SoundSSSetSrndPanCh(i, *(u8*)(list + 0xD) == 0x40 ? 0x40 : 0);
    }
    *(u8*)(bgm + 0x20) = (id & 0x100) != 0;
    *(u16*)(bgm + 0x22) = (id & 0x100) ? fade / (1000 / *(s32*)((s32)gp + 4)) : 1;
    *(u16*)(bgm + 0x24) = (id & 0x100) == 0;
    *(u8*)(bgm + 0x26) = 0;
    *(u16*)(bgm + 0x28) = 1;
    *(u16*)(bgm + 0x2A) = 1;
    psndBGMMain();
}

u32 calc3D(u32 flags, Vec* pos, u8 volume, u16 distance) {
    extern f64 angleABf(f64, f64, f64, f64);
    extern f64 cos(f64);
    extern f64 sin(f64);
    extern f64 sqrt(f64);
    extern const f32 float_0_804218d0;
    extern const f32 float_6p2832_804218d4;
    extern const f32 float_360_804218d8;
    extern const f32 float_180_804218e0;
    extern const f32 float_127_804218e4;
    extern const f32 float_2048_804218e8;
    extern const f32 float_64_804218ec;
    extern const f32 float_0p5_804218f0;
    extern const f32 float_200_804218f4;

    Vec local;
    f32 camAngle;
    f32 angle;
    f32 normalized;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 projected;
    f32 panCurve;
    s16 pan;
    s16 surround;
    s32 tmp;

    local = *pos;
    if ((flags & 0x1000000) != 0) {
        local.x += *(f32*)((s32)&psnd + 0x30);
        local.y += *(f32*)((s32)&psnd + 0x34);
        local.z += *(f32*)((s32)&psnd + 0x38);
    }
    if ((flags & 0x2000000) != 0) {
        local.x = *(f32*)((s32)&psnd + 0x30);
        local.y = *(f32*)((s32)&psnd + 0x34);
        local.z = *(f32*)((s32)&psnd + 0x38);
    }

    camAngle = *(f32*)((s32)&psnd + 0x3C);
    angle = (f32)angleABf(*(f32*)((s32)&psnd + 0x30), *(f32*)((s32)&psnd + 0x38), local.x, local.z);
    if (float_180_804218e0 <= __fabsf(angle - camAngle)) {
        if (camAngle <= angle) {
            angle -= float_360_804218d8;
        } else {
            angle += float_360_804218d8;
        }
    }
    normalized = (angle - camAngle) / float_180_804218e0;
    pan = (s16)(float_127_804218e4 * normalized + float_127_804218e4);
    if (pan < 0x40) {
        pan = 0x7F - pan;
    } else if (pan > 0xC4) {
        pan = 0xC4 - (pan - 0xC4);
    }
    pan -= 0x40;
    if (pan < 0) {
        pan = 0;
    }
    if (pan > 0x7F) {
        pan = 0x7F;
    }

    dx = local.x - *(f32*)((s32)&psnd + 0x30);
    dy = local.y - *(f32*)((s32)&psnd + 0x34);
    dz = local.z - *(f32*)((s32)&psnd + 0x38);
    dist = dx * dx + dy * dy + dz * dz;
    if (float_0_804218d0 < dist) {
        dist = (f32)sqrt(dist);
    }

    projected = __fabsf(dist * (f32)cos((f64)(float_6p2832_804218d4 * normalized)));
    if (float_2048_804218e8 < projected) {
        projected = float_2048_804218e8;
    }
    projected = (float_64_804218ec * projected) / float_2048_804218e8;

    if (float_0p5_804218f0 <= __fabsf(normalized)) {
        surround = (s16)(float_64_804218ec + projected);
        if (surround > 0x7F) {
            surround = 0x7F;
        }
    } else {
        surround = (s16)(float_64_804218ec - projected);
        if (surround < 1) {
            surround = 1;
        }
    }

    tmp = surround;
    if (tmp < 0x41) {
        f32 v = (f32)(((0x40 - tmp) * 0x5A) >> 6);
        projected = -(float_64_804218ec * (f32)sin((f64)((float_6p2832_804218d4 * v) / float_360_804218d8)) - float_64_804218ec);
    } else {
        f32 v = (f32)(((tmp - 0x40) * 0x5A) >> 6);
        projected = float_64_804218ec * (f32)sin((f64)((float_6p2832_804218d4 * v) / float_360_804218d8)) + float_64_804218ec;
    }

    tmp = (s16)(s32)projected;
    if (tmp < 0) {
        tmp = 0;
    }
    if (tmp > 0x7F) {
        tmp = 0x7F;
    }

    if (distance != 0) {
        f32 max = (f32)(distance * 100);
        f32 fade = dist - float_200_804218f4;
        if (max < fade) {
            fade = max;
        }
        if (fade < float_0_804218d0) {
            fade = float_0_804218d0;
        }
        volume = (u8)(((f32)volume * ((float_127_804218e4 * (max - fade)) / max)) / float_127_804218e4);
    }
    if (volume > 0x7F) {
        volume = 0x7F;
    }

    return ((u32)volume << 24) | ((u32)(u8)pan << 16) | ((u32)(u8)tmp << 8) | 0;
}

void psndBGMMain(void) {
    extern s32 seqGetSeq(void);
    extern void* gp;
    extern u16 SoundSSCheck(s32);
    extern s32 SoundSongCheck(s32);
    extern void SoundSSMuteOff(s32);
    extern void SoundSongSetVolCh(s32, u8);
    extern void SoundSSSetVolCh(s32, u8);
    extern void SoundSetFadeTime(u16, u16);
    extern void SoundSongFadeinCh(s32);
    extern void SoundSSFadeoutCh(s32);

    s32 targetVol;
    s32 i;
    u8* bgm;

    targetVol = 100;
    if ((*(u16*)((s32)&psnd + 0x56) & 0x20) != 0 && (*(u16*)((s32)&psnd + 0x56) & 0x40) == 0) {
        targetVol = 65;
    }

    if (seqGetSeq() != 3 && (*(u32*)((s32)gp + 0x1C) & 1) != 0) {
        if ((s8)*(u8*)((s32)&psnd + 0x58) < targetVol) {
            *(u8*)((s32)&psnd + 0x58) += 1;
        }
        if (targetVol < (s8)*(u8*)((s32)&psnd + 0x58)) {
            *(u8*)((s32)&psnd + 0x58) -= 1;
        }
    }

    bgm = (u8*)psbgm;
    for (i = 0; i < 2; i++, bgm += 0x38) {
        if (*(s32*)bgm != -1) {
            s32 active;
            s32 base;
            s32 env;
            s32 vol;
            u8* modePtr;

            if ((SoundSSCheck(*(s32*)(bgm + 4)) & 2) != 0) {
                SoundSSMuteOff(*(s32*)(bgm + 4));
            }

            if (*(s32*)bgm == -1) {
                active = 0;
            } else if ((pssfxlist[*(u32*)bgm].unk4 & 0x80000000) == 0) {
                active = SoundSongCheck(*(s32*)(bgm + 4)) != 0;
            } else {
                active = SoundSSCheck(*(s32*)(bgm + 4)) != 0;
            }

            if (active != 0) {
                if (bgm[0x10] != 0 && bgm[0x10] != 0xFF) {
                    bgm[0x10] -= 1;
                }

                base = 100;
                if (bgm[0x10] == 0) {
                    base = (s8)*(u8*)((s32)&psnd + 0x58);
                }

                if (bgm[0x20] == 0) {
                    vol = bgm[0xE];
                } else {
                    vol = ((u32)bgm[0xE] * *(s16*)(bgm + 0x24)) / *(s16*)(bgm + 0x22);
                    vol &= 0xFF;
                }

                if (bgm[0x26] == 0) {
                    env = 0x7F;
                } else {
                    env = (*(s16*)(bgm + 0x2A) * 0x7F) / *(s16*)(bgm + 0x28);
                    env &= 0xFF;
                }

                vol = (vol * env) / 0x7F;
                vol &= 0xFF;

                if (*(s32*)bgm != -1) {
                    s32 out = vol * base;
                    out = out / 100 + (out >> 31);
                    out = out - (out >> 31);
                    if ((pssfxlist[*(u32*)bgm].unk4 & 0x80000000) == 0) {
                        SoundSongSetVolCh(*(s32*)(bgm + 4), (u8)out);
                    } else {
                        SoundSSSetVolCh(*(s32*)(bgm + 4), (u8)out);
                    }
                }

                modePtr = bgm + 0x20;
                if (*modePtr == 1) {
                    *(s16*)(bgm + 0x24) += 1;
                    if (*(s16*)(bgm + 0x22) <= *(s16*)(bgm + 0x24)) {
                        *modePtr = 0;
                    }
                }

                if (*modePtr == 2) {
                    *(s16*)(bgm + 0x24) -= 1;
                    if (*(s16*)(bgm + 0x24) < 1) {
                        if (*(s32*)bgm != -1) {
                            if ((pssfxlist[*(u32*)bgm].unk4 & 0x80000000) == 0) {
                                SoundSetFadeTime(0, 100);
                                SoundSongFadeinCh(*(s32*)(bgm + 4));
                            } else {
                                SoundSetFadeTime(0, 100);
                                SoundSSFadeoutCh(*(s32*)(bgm + 4));
                            }
                        }
                        *modePtr = 0;
                        *(s32*)bgm = -1;
                    }
                }

                if (*modePtr == 4 && *(s16*)(bgm + 0x24) > 0) {
                    *(s16*)(bgm + 0x24) -= 1;
                }

                if (bgm[0x26] == 8) {
                    s16 current = *(s16*)(bgm + 0x2A);
                    s32 target = *(s16*)(bgm + 0x28) * bgm[0x27];
                    target = target / 100 + (target >> 31);
                    target -= target >> 31;
                    if (target < current) {
                        if ((*(u32*)(bgm + 8) & 0x40) == 0) {
                            *(s16*)(bgm + 0x2A) = current - 1;
                        } else {
                            *(s16*)(bgm + 0x2A) = current - 5;
                            if (*(s16*)(bgm + 0x2A) < target) {
                                *(s16*)(bgm + 0x2A) = target;
                            }
                        }
                    }
                }

                if (bgm[0x26] == 0x10) {
                    s16 current = *(s16*)(bgm + 0x2A);
                    if (current < *(s16*)(bgm + 0x28)) {
                        if ((*(u32*)(bgm + 8) & 0x40) == 0) {
                            *(s16*)(bgm + 0x2A) = current + 1;
                        } else {
                            *(s16*)(bgm + 0x2A) = current + 5;
                            if (*(s16*)(bgm + 0x28) < *(s16*)(bgm + 0x2A)) {
                                *(s16*)(bgm + 0x2A) = *(s16*)(bgm + 0x28);
                            }
                        }
                    } else {
                        bgm[0x26] = 0;
                    }
                }
            } else {
                *(s32*)bgm = -1;
            }
        }
    }
}

u32 psndENVOn_f_d(u32 flags, s32 name, s32 frames, s32 extra) {
    typedef struct PSndEnvListEntryLocal {
        u16 envId;
        u16 soundId;
        u8 unk4;
        u8 unk5;
        u8 mode;
        u8 randRange;
        u8 baseTime;
        u8 unk9;
        u8 unkA;
        u8 padB;
        u32 unkC;
        u32 unk10;
        u32 unk14;
        u8 unk18;
        u8 unk19;
        u8 unk1A;
        u8 pad1B;
    } PSndEnvListEntryLocal;
    extern char* psenvlistname[];
    extern PSndEnvListEntryLocal psenvlist[];
    extern void* gp;
    extern const f32 float_1000_804218cc;
    extern int strcmp(const char*, const char*);
    extern s32 irand(s32);
    extern u8 psndENV_stop(u32);

    u8* work = (u8*)&psnd;
    s32 slot = flags & 0xF;
    s32 envOff = slot * 0xC;
    s32 slot2 = slot * 2;
    u8* envSlot = work + envOff;
    u16* activeIdPtr = (u16*)(work + 0x18 + slot2);
    u16 activeId;
    PSndEnvListEntryLocal* list;
    s32 i;

    if (name < 0) {
        for (i = 0; i < 0x76; i++) {
            if (strcmp(psenvlistname[i], (char*)name) == 0) {
                break;
            }
        }
        name = i;
        if (name >= 0x76) {
            name = 0;
        }
    }

    if ((*(u16*)(work + 0x56) & 2) != 0) {
        return 0;
    }

    activeId = *activeIdPtr;
    list = psenvlist;

    if (activeId != 0) {
        if ((flags & 0x10) == 0) {
            if ((flags & 0x20) != 0) {
                if ((flags & 0x100) != 0) {
                    envSlot[0] = 1;
                    *(s16*)(envSlot + 2) = (s16)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
                    *(s16*)(envSlot + 4) = 0;
                } else {
                    envSlot[0] = 0;
                    *(s16*)(envSlot + 2) = 1;
                    *(s16*)(envSlot + 4) = 1;
                }

                if ((flags & 0x80) != 0 && envSlot[6] == 8) {
                    s32 fade;
                    envSlot[6] = 0x10;
                    *(s16*)(envSlot + 8) = (s16)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
                    fade = *(s16*)(envSlot + 8) * envSlot[7];
                    fade = fade / 100 + (fade >> 31);
                    *(s16*)(envSlot + 0xA) = (s16)fade - (s16)(fade >> 31);
                }
                return 1;
            }

            if (activeId == (u16)name && envSlot[0] != 2) {
                return 1;
            }
        }

        psndENV_stop(activeId);
        *activeIdPtr = 0;
        list = psenvlist;
    }

    while (1) {
        u16 listId = list->envId;

        if (listId == 0x76) {
            *activeIdPtr = (u16)name;
            envSlot[0] = 0;
            *(s16*)(work + 0x1C + slot2) = 32000;
            *(s16*)(work + 0x20 + slot2) = 32000;

            if ((flags & 0x100) != 0) {
                envSlot[0] = 1;
                *(s16*)(envSlot + 2) = (s16)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
                *(s16*)(envSlot + 4) = 0;
            } else {
                envSlot[0] = 0;
                *(s16*)(envSlot + 2) = 1;
                *(s16*)(envSlot + 4) = 1;
            }
            *(s16*)(envSlot + 8) = 1;
            *(s16*)(envSlot + 0xA) = 1;
            envSlot[6] = 0;
            *(s16*)(envSlot + 8) = 1;
            *(s16*)(envSlot + 0xA) = 1;
            return 1;
        }

        if (listId == (u16)name) {
            u8* dst = NULL;
            s32 idx;

            for (idx = 0; idx < 0x10; idx++) {
                if (*(s16*)((s32)&psenv[idx] + 4) == 0) {
                    dst = (u8*)&psenv[idx];
                    break;
                }
            }

            if (dst != NULL) {
                *(u16*)(dst + 4) = listId;
                *(u32*)dst = list->soundId;
                dst[6] = list->unk4;
                dst[7] = list->unk5;
                dst[8] = list->mode;
                dst[9] = list->randRange;
                dst[0xA] = list->baseTime;
                *(u32*)(dst + 0x18) = list->unkC;
                *(u32*)(dst + 0x1C) = list->unk10;
                *(u32*)(dst + 0x20) = list->unk14;
                dst[0xB] = list->unk18;
                dst[0xC] = list->unk19;
                *(u16*)(dst + 0x10) = list->unk1A;
                dst[0xD] = 0;
                *(u16*)(dst + 0xE) = 0;

                if (dst[8] == 2 || dst[8] == 0) {
                    *(u16*)(dst + 0xE) = dst[0xA] * *(s16*)((s32)gp + 4);
                    *(s16*)(dst + 0xE) += *(s16*)((s32)gp + 4) * (s16)irand(dst[9]);
                }
            }
        }
        list++;
    }
}

void psndENV_stop(u32 envId) {
    extern void SoundEfxStop(s32);
    extern void SoundSongStopCh(s32);
    extern void SoundSSStopCh(s32);

    s32 i;
    for (i = 0; i < 16; i++) {
        u8* env = (u8*)&psenv[i];
        u8 mode;
        u32 sfxIndex;
        PaperSoundEffect* sfx;

        if (*(u16*)(env + 4) != envId) {
            continue;
        }

        mode = env[8];
#define STOP_ENV_SFX() \
        do { \
            sfxIndex = *(u32*)(env + 0x14); \
            if (sfxIndex != (u32)-1) { \
                sfx = &pssfx[sfxIndex & 0xFF]; \
                if (sfx->listIndex != (u32)-1) { \
                    if ((sfx->unk6 & 1) == 0) { \
                        s32 effId = sfx->effectId; \
                        if (effId != -1) { \
                            u32 flags = pssfxlist[sfx->listIndex & 0x1FFF].unk4; \
                            if ((flags & 0x80000000) != 0) { \
                                SoundSSStopCh(effId); \
                            } else if ((flags & 0x40000000) != 0) { \
                                SoundSongStopCh(effId); \
                            } else { \
                                SoundEfxStop(effId); \
                            } \
                        } \
                        sfx->listIndex = (u32)-1; \
                        sfx->effectId = -1; \
                    } else { \
                        sfx->listIndex = (u32)-1; \
                        sfx->effectId = -1; \
                    } \
                } \
            } \
            *(u16*)(env + 4) = 0; \
        } while (0)

        if (mode == 2) {
            STOP_ENV_SFX();
        } else if (mode < 2) {
            if (mode == 0) {
                STOP_ENV_SFX();
            } else {
                STOP_ENV_SFX();
            }
        } else if (mode < 4) {
            STOP_ENV_SFX();
        }
#undef STOP_ENV_SFX
    }
}

s32 psndBGMOff_f_d(s32 name, s32 frames, s32 flags) {
    extern u32 silent_tbl[];
    extern void* gp;
    extern void SoundSetFadeTime(u16, u16);
    extern s32 evtGetValue(void*, s32);
    extern void psndBGMOn_f_d(s32, s32, s32, s32, s32);
    extern void SoundSongStopCh(s32);
    extern void SoundSSStopCh(s32);
    extern const f32 float_1000_804218cc;

    u8* bgm;
    s32 index;
    s32 volume;
    s32 state;

    index = name & 0xF;
    bgm = (u8*)&psbgm[index];
    volume = silent_tbl[(name >> 10) & 0xF];
    SoundSetFadeTime((u16)frames, (u16)frames);

    if (*(s32*)bgm == -1) {
        return 0;
    }

    if (flags == 0) {
        state = evtGetValue(NULL, -170000000);
        if (state > 0x6C && evtGetValue(NULL, -170000000) < 0x6F && *(u32*)bgm == 0xA4) {
            return 0;
        }
        state = evtGetValue(NULL, -170000000);
        if (state > 0x7E && evtGetValue(NULL, -170000000) < 0xB2 && *(u32*)&psbgm[0] == 0xC3) {
            if (*(u32*)bgm != 0x92) {
                return 0;
            }
            psndBGMOn_f_d(0x120, 0, 0x2EE, 0x2EE, 1);
        }
    }

    if ((name & 0x200) != 0) {
        if (bgm[0x20] != 2) {
            if (bgm[0x20] == 4) {
                bgm[0x20] = 2;
            } else {
                bgm[0x20] = 2;
                *(s16*)(bgm + 0x22) = (s16)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
                *(s16*)(bgm + 0x24) = *(s16*)(bgm + 0x22);
            }
        }
    } else if ((name & 0x400) != 0) {
        if (bgm[0x20] != 2 && bgm[0x20] != 4) {
            bgm[0x20] = 4;
            *(s16*)(bgm + 0x22) = (s16)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
            *(s16*)(bgm + 0x24) = *(s16*)(bgm + 0x22);
        }
    } else if ((name & 0x800) != 0) {
        if (bgm[0x26] != 8) {
            bgm[0x26] = 8;
            bgm[0x27] = volume;
            *(s16*)(bgm + 0x28) = (s16)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
            *(s16*)(bgm + 0x2A) = *(s16*)(bgm + 0x28);
            if ((name & 0x40) == 0 && (*(u32*)((s32)gp + 0x14) & 1) == 0) {
                *(u32*)(bgm + 8) &= ~0x40;
            } else {
                *(u32*)(bgm + 8) |= 0x40;
            }
        }
    } else {
        if ((pssfxlist[*(u32*)bgm].unk4 & 0x80000000) != 0) {
            SoundSSStopCh(*(s32*)(bgm + 4));
        } else {
            SoundSongStopCh(*(s32*)(bgm + 4));
        }
        *(s32*)bgm = -1;
    }

    return 1;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void psndENVOff_f_d(s32 name, s32 frames, s32 flags) {
    extern void* gp;
    extern u32 silent_tbl[];
    extern const f32 float_1000_804218cc;

    u8* work;
    u8* env;
    u16* envIdPtr;
    s32 index;
    s32 fade;
    s32 volume;
    u16 envId;

    work = (u8*)&psnd;
    index = name & 0xF;
    envIdPtr = (u16*)(work + 0x18 + index * 2);
    envId = *envIdPtr;
    volume = silent_tbl[(name >> 10) & 0xF];

    if (envId == 0) {
        return;
    }

    env = work + index * 0xC;
    if ((name & 0x200) != 0) {
        if (env[0] != 2) {
            if (env[0] == 4) {
                env[0] = 2;
            } else {
                env[0] = 2;
                fade = (s32)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
                *(u16*)(env + 2) = fade;
                *(u16*)(env + 4) = *(s16*)(env + 2);
            }
        }
    } else if ((name & 0x400) != 0) {
        if (env[0] != 2 && env[0] != 4) {
            env[0] = 4;
            fade = (s32)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
            *(u16*)(env + 2) = fade;
            *(u16*)(env + 4) = *(s16*)(env + 2);
        }
    } else if ((name & 0x800) != 0) {
        env[6] = 8;
        fade = (s32)((f32)frames / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
        *(u16*)(env + 8) = fade;
        *(u16*)(env + 0xA) = *(s16*)(env + 8);
        env[7] = volume;
        if ((name & 0x40) != 0) {
            *(u16*)(env + 0xA) = ((s32)*(s16*)(env + 8) * (u8)env[7]) / 100;
        }
    } else {
        psndENV_stop(envId);
        *envIdPtr = 0;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2) {
    extern f32 angleABTBL[];
    extern const f32 float_0_804218d0;
    extern const f32 float_2_804218fc;
    extern const f32 float_45_804218f8;
    extern const f32 float_90_80421900;
    extern const f32 float_180_804218e0;
    extern const f32 float_270_80421904;
    extern const f32 float_360_804218d8;
    extern const f64 double_0p5_802e3c98;

    f32 dx;
    f32 dz;
    f32 absX;
    f32 absZ;
    f32 ratio;
    f32 angle;
    f32 indexValue;
    s32 index;

    dx = x2 - x1;
    dz = z2 - z1;
    absX = __fabsf(dx);
    absZ = __fabsf(dz);

    if (absX > absZ) {
        ratio = absZ / absX;
        angle = float_45_804218f8 * ratio;
        indexValue = float_2_804218fc * angle;

        if (indexValue >= float_0_804218d0) {
            index = (s32)(double_0p5_802e3c98 + indexValue);
        } else {
            index = -(s32)(double_0p5_802e3c98 - indexValue);
        }

        angle = angle * angleABTBL[index];
        if (dx >= float_0_804218d0) {
            if (dz >= float_0_804218d0) {
                return float_90_80421900 + angle;
            } else {
                return float_90_80421900 - angle;
            }
        } else {
            if (dz >= float_0_804218d0) {
                return float_270_80421904 - angle;
            } else {
                return float_270_80421904 + angle;
            }
        }
    } else {
        if (absZ == float_0_804218d0) {
            return float_0_804218d0;
        }

        ratio = absX / absZ;
        indexValue = float_45_804218f8 * ratio;
        angle = float_2_804218fc * indexValue;

        if (angle >= float_0_804218d0) {
            index = (s32)(double_0p5_802e3c98 + angle);
        } else {
            index = -(s32)(double_0p5_802e3c98 - angle);
        }

        indexValue = indexValue * angleABTBL[index];
        if (dz >= float_0_804218d0) {
            if (dx >= float_0_804218d0) {
                return float_180_804218e0 - indexValue;
            } else {
                return float_180_804218e0 + indexValue;
            }
        } else {
            if (dx >= float_0_804218d0) {
                return indexValue;
            } else {
                return float_360_804218d8 - indexValue;
            }
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void psndMapChange(void) {
    extern void* gp;
    extern char* strcpy(char* dst, const char* src);
    extern s32 strcmp(const char* s1, const char* s2);
    extern const f32 float_1000_804218cc;
    extern const f32 float_750_804218dc;

    u8* work;
    u8* sfx;
    s32 i;
    s32 fade;
    u16 flags;

    work = (u8*)&psnd;
    if (strcmp((char*)(work + 0x40), (char*)((s32)gp + 0x12C)) != 0) {
        *(u16*)(work + 0x52) = 0;
    }
    strcpy((char*)(work + 0x40), (char*)((s32)gp + 0x12C));

    *(u16*)(work + 0x56) &= ~0x100;

    sfx = (u8*)pssfx;
    i = 8;
    do {
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        i--;
    } while (i != 0);

    if (work[0x24] != 2) {
        work[0x24] = 2;
        fade = (s32)(float_750_804218dc / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
        *(u16*)(work + 0x26) = fade;
        *(u16*)(work + 0x28) = fade;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void psndInit(void) {
    extern void SoundInit(void);
    extern void SoundLoadDVD2(const char* path);
    extern void SoundSLibLoadDVD(const char* path);
    extern char* strcpy(char* dst, const char* src);
    extern const f32 float_0_804218d0;
    extern const char str__802e3c78[];
    extern const char str_sound_proj_pmario_802e3c80[];
    u8* buf;
    u8* sfx;
    u8* env;
    u8* work;
    s32 i;
    s32 neg;
    s32 zero;

    buf = _buf;
    SoundInit();

    neg = -1;
    zero = 0;
    *(s32*)(buf + 0x960) = neg;
    work = buf + 0x960;
    *(u8*)(work + 0x20) = zero;
    *(s32*)(work + 0x38) = neg;
    *(u8*)(work + 0x58) = zero;

    sfx = buf + 0x320;
    i = 5;
    do {
        *(s32*)(sfx + 0x0) = neg;
        *(s32*)(sfx + 0x18) = neg;
        *(u16*)(sfx + 0x6) = zero;
        *(s32*)(sfx + 0x28) = neg;
        *(s32*)(sfx + 0x40) = neg;
        *(u16*)(sfx + 0x2E) = zero;
        *(s32*)(sfx + 0x50) = neg;
        *(s32*)(sfx + 0x68) = neg;
        *(u16*)(sfx + 0x56) = zero;
        *(s32*)(sfx + 0x78) = neg;
        *(s32*)(sfx + 0x90) = neg;
        *(u16*)(sfx + 0x7E) = zero;
        *(s32*)(sfx + 0xA0) = neg;
        *(s32*)(sfx + 0xB8) = neg;
        *(u16*)(sfx + 0xA6) = zero;
        *(s32*)(sfx + 0xC8) = neg;
        *(s32*)(sfx + 0xE0) = neg;
        *(u16*)(sfx + 0xCE) = zero;
        *(s32*)(sfx + 0xF0) = neg;
        *(s32*)(sfx + 0x108) = neg;
        *(u16*)(sfx + 0xF6) = zero;
        *(s32*)(sfx + 0x118) = neg;
        *(s32*)(sfx + 0x130) = neg;
        *(u16*)(sfx + 0x11E) = zero;
        sfx += 0x140;
        i--;
    } while (i != 0);

    env = buf + 0xE0;
    work = buf + 0x80;
    *(u16*)(env + 0x4) = zero;
    *(u16*)(env + 0x28) = zero;
    *(u16*)(env + 0x4C) = zero;
    *(u16*)(env + 0x70) = zero;
    *(u16*)(env + 0x94) = zero;
    *(u16*)(env + 0xB8) = zero;
    *(u16*)(env + 0xDC) = zero;
    *(u16*)(env + 0x100) = zero;
    *(u16*)(env + 0x124) = zero;
    *(u16*)(env + 0x148) = zero;
    *(u16*)(env + 0x16C) = zero;
    *(u16*)(env + 0x190) = zero;
    *(u16*)(env + 0x1B4) = zero;
    *(u16*)(env + 0x1D8) = zero;
    *(u16*)(env + 0x1FC) = zero;
    *(u16*)(env + 0x220) = zero;

    *(u16*)(work + 0x18) = zero;
    work[0] = zero;
    *(u16*)(work + 0x1A) = zero;
    work[0xC] = zero;
    *(f32*)(work + 0x3C) = float_0_804218d0;
    *(u16*)(work + 0x50) = zero;
    *(u16*)(work + 0x54) = zero;
    *(u16*)(work + 0x56) = zero;
    work[0x5C] = zero;
    work[0x5A] = 0x64;
    work[0x59] = 0x64;
    work[0x58] = 0x64;

    strcpy((char*)(work + 0x40), str__802e3c78);
    SoundLoadDVD2(str_sound_proj_pmario_802e3c80);
    SoundSLibLoadDVD(str_sound_proj_pmario_802e3c80);
}

void psndStopAllFadeOut(void) {
    extern void* gp;
    extern const f32 float_1000_804218cc;
    extern const f32 float_750_804218dc;
    u8* sfx;
    u8* work;
    s32 i;
    s32 fade;
    u16 flags;

    work = (u8*)&psnd;
    psndBGMOff_f_d(0x200, 1000, 0);
    psndBGMOff_f_d(0x201, 1000, 0);
    psndENVOff_f_d(0x200, 1000, 0);
    psndENVOff_f_d(0x201, 1000, 0);

    sfx = (u8*)pssfx;
    i = 8;
    do {
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            *(u16*)(sfx + 6) = flags | 0x40;
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            *(u16*)(sfx + 6) = flags | 0x40;
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            *(u16*)(sfx + 6) = flags | 0x40;
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            *(u16*)(sfx + 6) = flags | 0x40;
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            *(u16*)(sfx + 6) = flags | 0x40;
        }
        sfx += 0x28;
        i--;
    } while (i != 0);

    work[0x24] = 2;
    fade = (s32)(float_750_804218dc / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
    *(u16*)(work + 0x26) = fade;
    *(u16*)(work + 0x28) = fade;
}

void psndSFXAllOff(void) {
    extern void* gp;
    extern const f32 float_1000_804218cc;
    extern const f32 float_750_804218dc;
    extern const f64 double_to_int_802e3c60;
    u8* sfx;
    u8* work;
    s32 i;
    s32 fade;
    u16 flags;

    work = (u8*)&psnd;
    sfx = (u8*)pssfx;
    i = 8;
    do {
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        if ((u32)(*(s32*)sfx + 0x10000) != 0xFFFF) {
            flags = *(u16*)(sfx + 6);
            if ((flags & 0xA0) == 0) {
                *(u16*)(sfx + 6) = flags | 0x40;
            }
        }
        sfx += 0x28;
        i--;
    } while (i != 0);

    if (work[0x24] != 2) {
        work[0x24] = 2;
        fade = (s32)(float_750_804218dc / (float_1000_804218cc / (f32)*(s32*)((s32)gp + 4)));
        *(u16*)(work + 0x26) = fade;
        *(u16*)(work + 0x28) = fade;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void L_psndBGM_stop(void) {
    extern void SoundSSStopCh(s32 id);
    extern void SoundSongStopCh(s32 id);
    extern void SoundEfxStop(s32 id);
    PaperSoundEffect* sfx;
    PaperSoundEffect* off;
    PaperSoundEffectData* data;
    s32 i;
    s32 listIndex;
    s32 effectId;
    u32 flags;

    *(u16*)((s32)&psnd + 0x56) &= ~0x10;

    sfx = pssfx;
    i = 0;
    while (i < 0x28) {
        if ((u32)(sfx->listIndex + 0x10000) != 0xFFFF) {
            if ((sfx->unk6 & 0x10) != 0) {
                if ((sfx->unk6 & 0x820) == 0) {
                    if ((u32)(i + 0x10000) != 0xFFFF) {
                        off = &pssfx[(u8)i];
                        listIndex = off->listIndex;
                        if ((u32)(listIndex + 0x10000) != 0xFFFF) {
                            if ((off->unk6 & 1) != 0) {
                                off->listIndex = -1;
                                off->effectId = -1;
                            } else {
                                effectId = off->effectId;
                                data = &pssfxlist[listIndex & 0x1FFF];
                                if ((u32)(effectId + 0x10000) != 0xFFFF) {
                                    flags = data->unk4;
                                    if ((flags & 0x80000000) != 0) {
                                        SoundSSStopCh(effectId);
                                    } else if ((flags & 0x40000000) != 0) {
                                        SoundSongStopCh(effectId);
                                    } else {
                                        SoundEfxStop(effectId);
                                    }
                                }
                                off->listIndex = -1;
                                off->effectId = -1;
                            }
                        }
                    }
                }
            }
        }
        i++;
        sfx++;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void psndSFXOff(s32 id) {
    extern void SoundSSStopCh(s32 id);
    extern void SoundSongStopCh(s32 id);
    extern void SoundEfxStop(s32 id);
    PaperSoundEffect* sfx;
    PaperSoundEffectData* data;
    s32 listIndex;
    s32 effectId;
    u32 flags;

    if ((u32)(id + 0x10000) != 0xFFFF) {
        sfx = &pssfx[(u8)id];
        listIndex = sfx->listIndex;
        if ((u32)(listIndex + 0x10000) != 0xFFFF) {
            if ((sfx->unk6 & 1) != 0) {
                sfx->listIndex = -1;
                sfx->effectId = -1;
            } else {
                effectId = sfx->effectId;
                data = &pssfxlist[listIndex & 0x1FFF];
                if ((u32)(effectId + 0x10000) != 0xFFFF) {
                    flags = data->unk4;
                    if ((flags & 0x80000000) != 0) {
                        SoundSSStopCh(effectId);
                    } else if ((flags & 0x40000000) != 0) {
                        SoundSongStopCh(effectId);
                    } else {
                        SoundEfxStop(effectId);
                    }
                }
                sfx->listIndex = -1;
                sfx->effectId = -1;
            }
        }
    }
}

s32 psndBGMPlayTime(s32 id) {
    extern void* gp;
    PaperSoundBGM* bgm;
    u32 ticks;
    u64 now;
    u64 start;

    bgm = &psbgm[id & 0xF];
    if (*(s32*)bgm == -1) {
        return 600000;
    }

    ticks = (*(u32*)0x800000F8 >> 2) / 1000;
    now = *(u64*)((s32)gp + 0x38);
    start = *(u64*)((s32)bgm + 0x30);
    return (s32)((now - start) / ticks);
}

void psndSetFlag(s32 flag) {
    u16* flags;
    u16 value;

    flags = (u16*)((s32)&psnd + 0x56);
    value = *flags | flag;
    *flags = value;

    if ((value & 1) != 0) {
        psndBGMOff_f_d(0, 1000, 0);
        psndBGMOff_f_d(1, 1000, 0);
    }

    if ((*flags & 2) != 0) {
        psndENVOff_f_d(0, 1000, 0);
        psndENVOff_f_d(1, 1000, 0);
    }
}

s32 psndPushGroup(void) {
    extern s32 _psndPushGroup_trg;
    extern void SoundLoadDVD2Free(void);
    extern void SoundLoadDVD2PushGroup(void* tbl);
    extern s32 init_f;
    static s32 group;
    static u8 tbl[2];

    if (_psndPushGroup_trg != 0) {
        return 1;
    }

    if (group > 0xB) {
        init_f = 1;
        SoundLoadDVD2Free();
        return 0;
    }

    _psndPushGroup_trg = 1;
    tbl[0] = group;
    tbl[1] = 0xFF;
    SoundLoadDVD2PushGroup(tbl);
    _psndPushGroup_trg = 0;
    group++;
    return 1;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 searchPSSFXList(s32 name) {
    extern s32 strcmp(const char* a, const char* b);
    PaperSoundEffectData* entry;
    s32 i;

    entry = pssfxlist;
    if (name >= 0) {
        return name;
    }

    i = 0;
    while (i < 0x1092) {
        if (strcmp(*(char**)entry, (char*)name) == 0) {
            break;
        }
        i++;
        entry++;
    }

    if (i >= 0x1092) {
        i = 0;
    }
    return i;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void psndBGM_rate(u32 param_1, u16 param_2) {
    extern void SoundSSSetPlayFreqCh(s32 id);
    extern u8 psbgmlist[];
    PaperSoundBGM* bgm;
    s32 listIndex;

    bgm = &psbgm[param_1 & 0xF];
    listIndex = *(s32*)bgm;
    if ((u32)(listIndex + 0x10000) != 0xFFFF) {
        if ((*(u32*)(psbgmlist + listIndex * 0x10 + 4) & 0x80000000) != 0) {
            SoundSSSetPlayFreqCh(*(s32*)((s32)bgm + 4));
        }
    }
}

int psndSFXChk(u32 param_1) {
    PaperSoundEffect* sfx;
    s32 listIndex;

    if ((u32)(param_1 + 0x10000) == 0xFFFF) {
        return -1;
    }
    sfx = &pssfx[(u8)param_1];
    if (((param_1 >> 8) & 0xFFFF) != *(u16*)((s32)sfx + 4)) {
        return -1;
    }
    listIndex = *(s32*)sfx;
    return ((listIndex + 1) & ~(-1 - listIndex)) >> 31;
}

void psndBGMScope(u32 param_1, int param_2) {
    PaperSoundBGM* bgm;

    bgm = &psbgm[param_1 & 0xF];
    if (*(s32*)bgm == -1) {
        return;
    }
    if (param_2 > 0) {
        *(u8*)((s32)bgm + 0x10) = 0xFF;
    }
    if (param_2 < 0) {
        *(u8*)((s32)bgm + 0x10) = 0;
    }
    if (param_2 == 0) {
        *(u8*)((s32)bgm + 0x10) = 0x1E;
    }
}

void psndMain(void) {
    extern s32 init_f;
    extern void SoundMain(void);
    u8* work;
    s8 count;

    if (init_f != 0) {
        work = (u8*)&psnd;
        count = *(s8*)(work + 0x5C);
        if (count != 0) {
            work[0x5C] = count - 1;
        }
        psndBGMMain();
        psndENVMain();
        psndSFXMain();
        SoundMain();
    }
}

s32 psndENV_LPF(u32 param_1, short param_2) {
    u8* work;
    u32 offset;

    offset = (param_1 & 0xF) * 2;
    work = (u8*)&psnd;
    if (*(u16*)(work + 0x18 + offset) == 0) {
        return 0;
    }
    if ((u16)param_2 == 0) {
        param_2 = 0x7D00;
    }
    *(u16*)(work + 0x20 + offset) = param_2;
    return 1;
}

