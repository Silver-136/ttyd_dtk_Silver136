#pragma once

#include "sound.h"

#include <dolphin/mtx.h>


typedef struct PaperSoundEnv {
    u8 unk0[0x24 - 0x0]; //0x0
} PaperSoundEnv;
STATIC_ASSERT_SIZEOF(PaperSoundEnv, 0x24);

typedef struct PaperSoundEffect {
    u32 listIndex;         //0x0
    u8 unk0[0x6 - 0x4];    //0x4
    u16 unk6;              //0x6
    u8 volume;             //0x8
    u8 unk9[0x14 - 0x9];   //0x9
    u16 pitch;             //0x14
    u8 unk16[0x18 - 0x16]; //0x16
    s32 effectId;          //0x18
    Vec position;          //0x1C
} PaperSoundEffect;
STATIC_ASSERT_SIZEOF(PaperSoundEffect, 0x28);

typedef struct PaperSoundEffectData {
    u8 unk0[0x4 - 0x0];  //0x0
    u32 unk4;            //0x4
    u8 unk8[0x14 - 0x8]; //0x8
} PaperSoundEffectData;
STATIC_ASSERT_SIZEOF(PaperSoundEffectData, 0x14);

typedef struct PaperSoundBGM {
    u8 unk0[0x38 - 0x0]; //0x0
} PaperSoundBGM;
STATIC_ASSERT_SIZEOF(PaperSoundBGM, 0x38);

typedef struct PaperSoundWork {
    u8 unk0[0x60 - 0x0]; //0x0
} PaperSoundWork;
STATIC_ASSERT_SIZEOF(PaperSoundWork, 0x60);

s32 searchPSSFXList(s32 name);
void psndInit(void);
void psndMain(void);
void psndMainInt(void);
void psndExit(void);
s32 psndPushGroup(void);
void psndSetPosDirListener(Vec* pos, f32 dir);
void psndStopAllFadeOut(void);
void psndSetReverb(s32 mode);
void psndSetFlag(s32 flag);
void psndClearFlag(u32 flag);
u32 psndGetFlag(void);
void psndMapChange(void);
void psndSFXAllOff(void);
void L_psndBGM_stop(void);
void psndBGM_rate(u32 value, u16 rate);
void psndBGMOn(s32 id, s32 value);
void unk_800db778(s32 id, s32 a2, s32 a3, s32 a4);
s32 psndBGMOn_f_d(s32 id, s32 value, s32 fade, s32 fadeOut, s32 unused);
void psndBGMOff(s32 id);
s32 psndBGMOff_f_d(s32 name, s32 frames, s32 flags);
s32 psndBGMChk(s32 index);
s32 psndBGMChkSilent(s32 index);
void psndBGMScope(u32 value, int mode);
s32 psndBGMStartCheck(s32 index);
s32 psndBGMPlayTime(s32 id);
s32 psndSFXOn_(s32 lookup);
s32 psndSFXOn(s32 lookup);
s32 psndSFXOnVol(s32 lookup, u8 volume);
u32 psndSFXOn_3D(s32 lookup, Vec* position);
s32 psndSFXOnEx_3D(s32 lookup, u8 volume, u8 pan, u16 pitch, Vec* position, u16 distance);
void psndSFX_vol(u32 index, u8 volume);
void psndSFX_pit(u32 index, u32 pitch);
void psndSFX_pos(u32 index, Vec* pos);
void psndSFX_dist(u32 index, u16 distance);
void psndSFXOff(s32 id);
int psndSFXChk(u32 id);
u8 psndSFX_get_vol(u32 index);
void psndENVOn(s32 id, s32 value);
u32 psndENVOn_f_d(u32 flags, s32 name, s32 frames, s32 extra);
void psndENVOff(s32 id);
void psndENVOff_f_d(s32 name, s32 frames, s32 flags);
s32 psndENV_LPF(u32 id, short value);
