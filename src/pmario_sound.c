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
    ;
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
    return 0;
}


u8 psndSFXMain(void) {
    return 0;
}


void psndBGMOn_f_d(s32 id, s32 a2, s32 fade, s32 unk1, s32 unk2) {
    ;
}


u8 calc3D(void) {
    return 0;
}


u8 psndBGMMain(void) {
    return 0;
}


void psndENVOn_f_d(s32 name, s32 flags, s32 frames, s32 extra) {
    ;
}


u8 psndENV_stop(u32 param_1) {
    return 0;
}


void psndBGMOff_f_d(s32 name, s32 frames, s32 flags) {
    ;
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

