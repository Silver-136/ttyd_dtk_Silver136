#include "action/ac_signal_timing.h"

extern void* _battleWorkPointer;
s32 ac_signaltiming_ok_frame_range[4];
s32 ac_signaltiming_ok_frame_range_2[4];

s32 battleAcResult_SignalTiming(void* wp) {
    return *(s32*)((s32)wp + 0x1CB8);
}

void battleAcDelete_SignalTiming(void* wp) {
    *(s32*)((s32)wp + 0x1C9C) = 1004;
}

s32 _get_ok_frame_range(s32 idx) {
    void* wp = _battleWorkPointer;

    switch (*(s32*)((s32)wp + 0x1CCC)) {
        case 1:
            return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range_2[idx];
        default:
            return *(s32*)((s32)wp + 0x1CE4) + ac_signaltiming_ok_frame_range[idx];
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void battleAcDisp_SignalTiming(void* camera, void* wp) {
    typedef struct SignalTimingDispWork {
        u8 pad_00[0x14];
        f32 x;
        f32 y;
        u8 pad_1C[4];
        s32 timer;
    } SignalTimingDispWork;
    SignalTimingDispWork* disp;
    s32 state;
    s32 timer;

    extern f32 float_neg300_80427c70;
    extern f32 float_30_80427c74;
    extern f32 float_0_80427c54;
    extern f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
    extern void actionCommandDisp(f32 x, f32 y);

    disp = (SignalTimingDispWork*)((s32)wp + 0x1F20);
    state = *(s32*)((s32)wp + 0x1C9C);

    switch (state) {
        case 99:
        case 100:
            disp->x = intplGetValue(
                4,
                0x14 - disp->timer,
                0x14,
                float_neg300_80427c70,
                float_30_80427c74);
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer > 0) {
                disp->timer = timer - 1;
            }
            break;
        case 1000:
        case 1001:
        case 1002:
        case 1003:
            actionCommandDisp(float_30_80427c74, float_0_80427c54);
            break;
        case 1004:
        case 1005:
            timer = disp->timer;
            if (timer >= 0x28) {
                disp->x = intplGetValue(
                    4,
                    timer - 0x28,
                    0x14,
                    float_30_80427c74,
                    float_neg300_80427c70);
            } else {
                disp->x = float_30_80427c74;
            }
            actionCommandDisp(disp->x, disp->y);
            timer = disp->timer;
            if (timer < 0x3C) {
                disp->timer = timer + 1;
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/action/ac_signal_timing 20260624_184823 */

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    typedef struct { f32 x, y, z; } Vec;
    extern void* g_BattleWork;
    extern void iconDispGx(f32, Vec*, s32, s32);
    extern void iconNumberDispGx(void*, s32, s32, void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    u8* ac = (u8*)g_BattleWork + 0x1F20;
    u32 flags = *(u32*)ac;
    s32 kind = *(s32*)(ac + 0x10);
    s32 count = *(s32*)(ac + 0x28);
    s32 filled = *(s32*)(ac + 0x54);
    s32 span;
    s32 i;
    Vec pos;
    f32 trans[3][4], scale[3][4], model[3][4];
    u32 color = 0xFFFFFFFF;

    if ((flags & 1) == 0) {
        pos.x = x - 200.0f;
        pos.y = y + 70.0f;
        pos.z = 0.0f;
        iconDispGx(1.0f, &pos, 0x10, *(s32*)(ac + 0x60) == 0 ? 0x6C : 0x6D);
    }
    span = count * 2 + (kind == 1 ? 5 : kind == 3 ? 0xB : kind == 5 ? 0xF : 0xD);
    pos.x = x - 288.0f;
    pos.y = y + 25.0f;
    pos.z = 0.0f;
    iconDispGx(1.0f, &pos, 0x10, 0x97);
    for (i = 1; i < span; i++) {
        pos.x = x + (f32)(i * 16 - 0x120);
        iconDispGx(1.0f, &pos, 0x10, 0x96);
    }
    pos.x = x + (f32)(span * 16 - 0x120);
    iconDispGx(1.0f, &pos, 0x10, 0x98);
    if ((flags & 4) == 0) {
        PSMTXTrans(trans, x - 200.0f + (f32)(kind * 0x30 + count * 0x20 - 0x1A), y + 25.0f, 0.0f);
        PSMTXScale(scale, (flags & 8) ? 1.5f : 1.0f, (flags & 8) ? 1.5f : 1.0f, (flags & 8) ? 1.5f : 1.0f);
        PSMTXConcat(trans, scale, model);
        iconNumberDispGx(model, *(s32*)(ac + 0x48) + *(s32*)(ac + 0x24), 0, &color);
    }
    for (i = 0; i < count; i++) {
        pos.x = x + (f32)(i * 0x21 - 0x102);
        pos.y = y + 25.0f;
        iconDispGx(i < filled ? 1.0f : 0.5f, &pos, 0x10, i < filled ? 0x9A + i : 0x99);
    }
}

/* stub-fill: battleAcMain_SignalTiming | missing_definition | ghidra_signature */
s32 battleAcMain_SignalTiming(int work) {
    extern void* memset(void*,s32,u32);extern s32 BattleActionCommandGetDifficulty(void*);extern s32 BattlePadCheckTrigger(u32);extern s32 BattlePadGetTrigger(void);extern s32 irand(s32);extern s32 _get_ok_frame_range(s32);extern void psndSFXOn(char*);
    extern char str_SFX_AC_COMMAND_NG1_802ff7b0[];extern char str_SFX_AC_COMMAND_OK1_802ff79c[];extern char str_SFX_AC_PI1_802ff780[];extern char str_SFX_AC_PONE1_802ff78c[];
    u8* wp=(u8*)work;u8* extra=wp+0x1F4C;s32 state;u32 trig;s32 valid;s32 frame;s32 start;s32 end;s32 i;s32 button;s32 done=0;
    do{
        state=*(s32*)(wp+0x1C9C);
        switch(state){
        case 0:
            *(s32*)(wp+0x1CB8)=1;memset(wp+0x1F20,0,0x2C);*(f32*)(wp+0x1F34)=-300.0f;*(f32*)(wp+0x1F38)=0.0f;*(s32*)(wp+0x1F40)=0x14;
            *(s32*)(extra+0)=0;*(s32*)(wp+0x1F50)=0;*(s32*)(wp+0x1F5C)=0;*(s32*)(wp+0x1F54)=0;*(s32*)(wp+0x1F58)=0;*(s32*)(wp+0x1F78)=BattleActionCommandGetDifficulty(wp);*(s32*)(wp+0x1F7C)=0;*(s32*)(wp+0x1CEC)=0;
            for(i=0;i<5;i++)extra[0x4C+i]=0;
            if(*(s32*)(wp+0x1CD8)>0&&*(s32*)(wp+0x1CD8)<3){for(i=0;i<*(s32*)(wp+0x1CC8);i++){button=irand(4);*(s32*)(extra+0x38+i*4)=button;}}else{for(i=0;i<*(s32*)(wp+0x1CC8);i++)*(s32*)(extra+0x38+i*4)=0x100;}
            *(s32*)(wp+0x1F80)=*(s32*)(wp+0x1F84);*(s32*)(wp+0x1C9C)=99;break;
        case 99:
            if(*(s16*)(wp+0x1D18)<1){frame=*(s32*)(wp+0x1F54);*(s32*)(wp+0x1F60)=frame**(s32*)(wp+0x1CD0);*(s32*)(wp+0x1F64)=(frame+1)**(s32*)(wp+0x1CD0)-1;*(s32*)(wp+0x1F68)=*(s32*)(wp+0x1F60);*(s32*)(wp+0x1F6C)=*(s32*)(wp+0x1F68)+_get_ok_frame_range(*(s32*)(wp+0x1F78))-1;*(s32*)(wp+0x1C9C)=1000;}else{(*(s16*)(wp+0x1D18))--;*(s32*)(wp+0x1CB8)=2;*(s32*)(wp+0x1C9C)=1002;done=1;}break;
        case 1000:
            *(s32*)(wp+0x1F74)=(*(s32*)(wp+0x1F58)+1)**(s32*)(wp+0x1CD0)-1;(*(s32*)(wp+0x1F58))++;psndSFXOn(*(s32*)(wp+0x1F58)<*(s32*)(wp+0x1CE0)+1?str_SFX_AC_PI1_802ff780:str_SFX_AC_PONE1_802ff78c);*(s32*)(wp+0x1F70)=0;*(s32*)(wp+0x1C9C)=1001;break;
        case 1001:
            (*(s32*)(wp+0x1F5C))++;frame=*(s32*)(wp+0x1F5C);start=*(s32*)(wp+0x1F68);end=*(s32*)(wp+0x1F6C);valid=(frame>=start&&frame<=end&&*(s32*)(wp+0x1F70)==0);*(s32*)(wp+0x1F7C)=valid;
            trig=BattlePadCheckTrigger(*(u32*)(wp+0x1F80));if(trig&&valid){*(s32*)(wp+0x1F70)=1;(*(s32*)extra)++;*(s32*)(wp+0x1CEC)=1;psndSFXOn(str_SFX_AC_COMMAND_OK1_802ff79c);}else if(frame>end&&*(s32*)(wp+0x1F70)==0){*(s32*)(wp+0x1F70)=1;*(s32*)(wp+0x1CEC)=-1;psndSFXOn(str_SFX_AC_COMMAND_NG1_802ff7b0);}
            if(*(s32*)(wp+0x1F70)&&*(s32*)(wp+0x1F54)<*(s32*)(wp+0x1CC8)+*(s32*)(wp+0x1CE0)){(*(s32*)(wp+0x1F54))++;frame=*(s32*)(wp+0x1F54);*(s32*)(wp+0x1F68)=frame**(s32*)(wp+0x1CD0);*(s32*)(wp+0x1F6C)=*(s32*)(wp+0x1F68)+_get_ok_frame_range(*(s32*)(wp+0x1F78))-1;*(s32*)(wp+0x1F70)=0;}
            if(*(s32*)(wp+0x1F58)**(s32*)(wp+0x1CD0)<=frame){*(s32*)(wp+0x1C9C)=1000;}break;
        case 1002:*(s32*)(wp+0x1C9C)=1003;break;
        case 1003:*(u32*)(wp+0x1CC0)|=1;*(s32*)(wp+0x1F7C)=0;*(s32*)(wp+0x1FA0)=60;*(s32*)(wp+0x1C9C)=1005;break;
        case 1005:if(--*(s32*)(wp+0x1FA0)<1)*(s32*)(wp+0x1C9C)=1006;break;
        case 1006:*(s32*)(wp+0x1CA0)=0;*(s32*)(wp+0x1CA8)=0;*(s32*)(wp+0x1CA4)=0;*(s32*)(wp+0x1CAC)=0;break;
        }
        if(!done)return 1;
    }while(1);
}

