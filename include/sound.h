#pragma once

#include <dolphin/types.h>

void SoundInit(void);
void SoundMain(void);
void SoundMainInt(void);
void SoundSetFadeTime(s16 a, s16 b);
s32 SoundLoadDVD2(s32 name);
s32 SoundLoadDVD2PushGroup(u8* data);
void SoundLoadDVD2Free(void);
s32 SoundSLibLoadDVD(s32 name);
s32 SoundDropData(void);
void SoundSetOutputMode(s32 mode);
void SoundOpenCover(void);
void SoundCloseCover(void);
void SoundSongPlayCh(s32 index, u32 value);
void SoundSongContinueCh(s32 index);
void SoundSongStopCh(s32 index);
void SoundSongFadeoutCh(s32 index);
void SoundSongFadeinCh(s32 index);
void SoundSongSetVolCh(s32 index, u8 volume);
s32 SoundSongCheck(s32 index);
void SoundEfxStop(s32 index);
void SoundEfxSetPitch(s32 index, u32 value);
void SoundEfxSetVolume(s32 index, u32 value);
void SoundEfxSetPan(s32 index, u32 value);
void SoundEfxSetLPF(s32 index, s16 value);
s32 SoundEfxCheck(s32 index);
void SoundSSPlayCh(s32 a, s32 b);
void SoundSSStopCh(s32 index);
void SoundSSFadeoutCh(s32 index);
void SoundSSContinueCh(s32 index);
void SoundSSSetPanCh(s32 index, u8 pan);
void SoundSSSetVolCh(s32 index, u8 volume);
void SoundSSSetSrndPanCh(s32 index, u8 pan);
void SoundSSSetPlayFreqCh(s32 index, u16 value);
u16 SoundSSCheck(s32 index);
void SoundSSMuteOn(s32 index);
void SoundSSMuteOff(s32 index);
