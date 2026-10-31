// Maya-exported effect animation keys: MayaHermitChannel (one Hermite channel) and MayaHermitKey (22 channels).

#pragma once

#include <dolphin/types.h>

/* Maya effect animation key (read by effCalcMayaAnimMatrix through GQR5: s16 values, 6 fraction bits) */
typedef struct MayaHermitChannel {
    s16 value;      // 0x0
    s16 inTangent;  // 0x2
    s16 outTangent; // 0x4
    u8 pad_06;   // 0x6, never accessed
    u8 step;        // 0x7, nonzero: hold value, no interpolation
} MayaHermitChannel;

/* 22 channels: translate xyz, rotate xyz, scale xyz, pivot xyz, translate2 xyz, pivot2 xyz, translate3 xyz, alpha */
typedef struct MayaHermitKey {
    s16 time;                  // 0x0
    MayaHermitChannel ch[22];  // 0x2
} MayaHermitKey;
