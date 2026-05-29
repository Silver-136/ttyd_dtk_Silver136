#include "pmario_sound.h"

//.bss
PaperSoundWork psnd;
PaperSoundEnv psenv[16];
PaperSoundEffect pssfx[40];
PaperSoundBGM psbgm[2];

//.data
PaperSoundEffectData pssfxlist[3042];

//local prototypes
s32 __psndSFXOn(s32 lookup, u8 volume, u8 a3, u16 a4, Vec* position, u32 a6, u16 distance, s32 a8);

s32 __psndSFXOn(s32 lookup, u8 volume, u8 a3, u16 a4, Vec* position, u32 a6, u16 distance, s32 a8) {
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
    return __psndSFXOn(lookup, volume, a3, a4, position, 0, distance, 0);
}

s32 psndBGMChk(s32 index) {
    s32 value = *(s32*)((s32)&psbgm[index & 0xF] + 0);
    return value != -1;
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
