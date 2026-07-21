#include "action/ac_pendulum_crane_timing.h"

u16 _hp_tbl[14];
extern void* _battleWorkPointer;
extern f32 float_0_80426eb8;
extern f32 float_45_80426ebc;
extern f32 float_100_80426ec0;
extern f32 float_90_80426ec4;
extern f32 float_neg300_80426ee4;
extern f32 float_30_80426ee8;

f32 compAngle(f32 a, f32 b);
f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
void actionCommandDisp(f32 x, f32 y);

/* stub-fill: battleAcMain_PendulumCraneTiming | missing_definition | ghidra_signature */
s32 battleAcMain_PendulumCraneTiming(s32 work)  {
    extern void* memset(void*,s32,s32);
    extern void* BattleGetUnitPtr(void*,s32);
    extern void* BtlUnit_GetPartsPtr(void*,s32);
    extern void BtlUnit_SetRotateOffset(void*,f32,f32,f32);
    extern void BtlUnit_GetPos(void*,f32*,f32*,f32*);
    extern void BtlUnit_SetRotate(void*,f32,f32,f32);
    extern f64 sinfd(f64);
    extern f32 reviseAngle(f32);
    extern s32 _get_angle_hp(s32);
    extern s32 _get_angle_rate(f32);
    extern s32 BattlePadCheckTrigger(s32);
    extern void psndSFXOn(char*);
    extern void psndSFXOff(s32);
    extern char str_SFX_AC_GAUGE_MOVE1_802fe508[];
    extern void* _battleWorkPointer;
    void* unit;
    u32 state=*(u32*)(work+0x1C9C);
    s32 total;
    f32 angle;
    f32 sign;
    f32 input=0.0f;
    s32 rate;
    s32 zone;
    s32 oldZone;
    u8 autoBadge=*(u8*)(work+0x1D00);
    unit=BattleGetUnitPtr((void*)work,*(s32*)(work+0x1CC8));
    BtlUnit_GetPartsPtr(unit,*(s32*)(work+0x1CCC));
    if(state==0) {
        memset((void*)(work+0x1F20),0,0x2C);
        *(f32*)(work+0x1F34)=-300.0f;
        *(f32*)(work+0x1F38)=0.0f;
        *(s32*)(work+0x1F40)=20;
        *(s32*)(work+0x1F24)=0;
        *(f32*)(work+0x1F4C)=0.0f;
        *(s32*)(work+0x1F54)=*(s32*)(work+0x1CD4);
        *(s32*)(work+0x1F58)=0;
        *(f32*)(work+0x1F48)=3.0f;
        *(s32*)(work+0x1F68)=-1;
        *(s32*)(work+0x1F20)=0x100;
        *(s32*)(work+0x1CB8)=1;
        *(s32*)(work+0x1CE8)=0;
        *(s32*)(work+0x1CEC)=_get_angle_hp(50);
        *(s32*)(work+0x1CF0)=_get_angle_hp(0);
        *(s32*)(work+0x1F60)=50;
        *(s32*)(work+0x1F6C)=-1;
        *(s32*)(work+0x1C9C)=99;
        return 1;
    }
    if(state==100) {
        BtlUnit_SetRotateOffset(unit,0.0f,(f32)*(s32*)(work+0x1CCC),0.0f);
        BtlUnit_GetPos(unit,(f32*)(work+0x1F34),(f32*)(work+0x1F38),(f32*)(work+0x1F3C));
        *(f32*)(work+0x1F28)=*(f32*)(work+0x1F34);
        *(f32*)(work+0x1F2C)=*(f32*)(work+0x1F38);
        *(f32*)(work+0x1F30)=*(f32*)(work+0x1F3C);
        *(s32*)(work+0x1F24)=0;
        *(f32*)(work+0x1F40)=0.5f;
        *(s32*)(work+0x1F5C)=1;
        *(s32*)(work+0x1C9C)=1000;
        state=1000;
    }
    if(state==1000) {
        (*(s32*)(work+0x1F24))++;
        total=*(s32*)(work+0x1CD4)*2;
        *(f32*)(work+0x1F50)=(f32)((((*(s32*)(work+0x1F54)+*(s32*)(work+0x1F58))%total)*360)/total);
        angle=reviseAngle(45.0f*(f32)sinfd(*(f32*)(work+0x1F50)));
        *(f32*)(work+0x1F4C)=angle;
        sign=(*(f32*)(work+0x1F50)>=90.0f&&*(f32*)(work+0x1F50)<270.0f)?-1.0f:1.0f;
        *(s32*)(work+0x1F58)=(s32)((f32)*(s32*)(work+0x1F58)+input);
        *(s32*)(work+0x1F54)=(s32)((f32)*(s32*)(work+0x1F54)+*(f32*)(work+0x1F48));
        *(f32*)(work+0x1F50)=(f32)((((*(s32*)(work+0x1F54)+*(s32*)(work+0x1F58))%total)*360)/total);
        angle=reviseAngle(45.0f*(f32)sinfd(*(f32*)(work+0x1F50)));
        *(f32*)(work+0x1F4C)=angle;
        BtlUnit_SetRotate(unit,0.0f,0.0f,angle);
        if(sign!=((*(f32*)(work+0x1F50)>=90.0f&&*(f32*)(work+0x1F50)<270.0f)?-1.0f:1.0f))*(s32*)(work+0x1F5C)=0;
        rate=_get_angle_rate(angle);
        *(s32*)(work+0x1CEC)=_get_angle_hp(rate);
        *(s32*)(work+0x1F60)=rate;
        zone=rate<6?0:(rate>94?1:-1);
        oldZone=*(s32*)(work+0x1F68);
        if(zone!=-1&&zone!=oldZone) {
            psndSFXOn(str_SFX_AC_GAUGE_MOVE1_802fe508);
            *(s32*)(work+0x1F68)=zone;
        }
        if((autoBadge&&*(s32*)(work+0x1CEC)>=_get_angle_hp(50))||BattlePadCheckTrigger(*(s32*)(work+0x1F20))) {
            *(s32*)(work+0x1CE8)=1;
            *(s32*)(work+0x1CB8)=2;
            (*(s32*)(work+0x1CB4))++;
            *(s32*)(work+0x1C9C)=1002;
        } else if(*(s32*)(work+0x1CD8)<*(s32*)(work+0x1F24)) {
            *(s32*)(work+0x1CB8)=0;
            *(s32*)(work+0x1C9C)=1002;
        }
        return 1;
    }
    if(state==1002) {
        *(u32*)(work+0x1CB0)|=1;
        *(s32*)(work+0x1F6C)=60;
        if(*(s32*)(work+0x1F64)!=-1)psndSFXOff(*(s32*)(work+0x1F64));
        *(s32*)(work+0x1C9C)=1003;
        return 1;
    }
    if(state==1003) {
        if(--*(s32*)(work+0x1F6C)<=0)*(s32*)(work+0x1C9C)=1004;
        return 1;
    }
    if(state==1004) {
        *(s32*)(work+0x1CA0)=0;
        *(s32*)(work+0x1CA4)=0;
        *(s32*)(work+0x1CA8)=0;
        *(s32*)(work+0x1CAC)=0;
        return 0;
    }
    return 1;
}

s32 battleAcResult_PendulumCraneTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDisp_PendulumCraneTiming(void) {
    s32 work = (s32)_battleWorkPointer + 0x1F20;
    s32 state = *(s32*)(work - 0x284);
    s32 timer;

    if (state == 0x3E8) {
        goto intro_display;
    }
    if (state >= 0x3E8) {
        if (state >= 0x3EC) {
            return;
        }
        if (state >= 0x3EA) {
            goto outro;
        }
        return;
    }
    if (state == 0x64) {
        goto intro_display;
    }
    if (state >= 0x64) {
        return;
    }
    if (state < 0x63) {
        return;
    }

    *(f32*)(work + 0x14) = intplGetValue(4, 0x14 - *(s32*)(work + 0x20), 0x14, float_neg300_80426ee4, float_30_80426ee8);

intro_display:
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer > 0) {
        *(s32*)(work + 0x20) = timer - 1;
    }
    return;

outro:
    timer = *(s32*)(work + 0x20);
    if (timer >= 0x28) {
        *(f32*)(work + 0x14) = intplGetValue(4, timer - 0x28, 0x14, float_30_80426ee8, float_neg300_80426ee4);
    } else {
        *(f32*)(work + 0x14) = float_30_80426ee8;
    }
    actionCommandDisp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    timer = *(s32*)(work + 0x20);
    if (timer < 0x3C) {
        *(s32*)(work + 0x20) = timer + 1;
    }
}

void battleAcDelete_PendulumCraneTiming(void* wp) {
    if ((*(u32*)((s32)wp + 0x1CC0) & 1) != 0) {
        return;
    }
    *(s32*)((s32)wp + 0x1C9C) = 1002;
}



/* CHATGPT STUB FILL: main/action/ac_pendulum_crane_timing 20260624_184128 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* _battleWorkPointer;
    extern u32 vec3_802fe4e8[];
    extern f32 float_neg200_80426ec8;
    extern f32 float_25_80426ecc;
    extern f32 float_1_80426ed0;
    extern f32 float_88_80426ed4;
    extern f32 float_50_80426ed8;
    extern f32 float_100_80426ec0;
    extern f32 float_neg288_80426ee0;
    extern f32 float_45_80426ebc;
    extern void* camGetPtr(s32 cameraId);
    extern void iconDispGx(f32 scale, Vec* pos, s32 flags, s32 iconId);
    extern void btlDispGXInit2DRasta(void);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void btlDispGXQuads2DRasta(double x1, double y1, double x2, double y2, s32 r, s32 g, s32 b, s32 a);
    Vec iconPos;
    Vec markerPos;
    void* cam;
    f32 baseX;
    f32 barY;
    f32 x1;
    f32 x2;
    s32 i;
    s32 intensity;

    camGetPtr(8);
    iconPos = *(Vec*)vec3_802fe4e8;
    baseX = float_neg200_80426ec8 + x;
    iconPos.x = baseX;
    iconPos.y = float_25_80426ecc + y;
    iconDispGx(float_1_80426ed0, &iconPos, 0x10, 0x94);

    cam = camGetPtr(8);
    btlDispGXInit2DRasta();
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);

    x1 = baseX - float_88_80426ed4;
    barY = float_50_80426ed8 + y;
    for (i = 0; i < 7; i++) {
        if (i != 0) {
            x1 = baseX - float_88_80426ed4 + (f32)(_hp_tbl[(i - 1) * 2] * 0xAF) / float_100_80426ec0;
        }
        x2 = baseX - float_88_80426ed4 + (f32)(_hp_tbl[i * 2] * 0xAF) / float_100_80426ec0;
        intensity = 3 - ((3 - i) < 0 ? -(3 - i) : (3 - i));
        btlDispGXQuads2DRasta((double)x1, (double)barY, (double)x2, (double)barY, intensity * 0x3C, 0, intensity * -0x3C - 0x4C, 0xFF);
    }

    markerPos = *(Vec*)(vec3_802fe4e8 + 3);
    markerPos.x = float_neg288_80426ee0 + x + (f32)(*(s32*)((s32)_battleWorkPointer + 0x1CD0) * 0xB0) / float_100_80426ec0;
    markerPos.y = float_45_80426ebc + y;
    iconDispGx(float_1_80426ed0, &markerPos, 0x10, 0x9E);
}

s32 _get_angle_rate(f32 angle) {
    f32 base;
    f32 diff;
    f32 sum;
    f32 scale;
    s32 value;

    base = float_0_80426eb8;
    diff = compAngle(base, angle);
    sum = diff + float_45_80426ebc;
    scale = float_100_80426ec0;
    value = (s32)((scale * sum) / float_90_80426ec4);

    if (value < 0) {
        value = 0;
    }
    if (value > 100) {
        value = 100;
    }
    return value;
}

s32 _get_angle_hp(s32 hp) {
    u16* entry = _hp_tbl;

    while (hp > entry[0]) {
        entry += 2;
    }
    return entry[1];
}

