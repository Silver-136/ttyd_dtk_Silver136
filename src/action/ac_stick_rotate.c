#include "action/ac_stick_rotate.h"

extern void* _battleWorkPointer;
extern f32 float_neg300_8042683c;
extern f32 float_30_80426840;

f32 intplGetValue(s32 type, s32 current, s32 total, f32 start, f32 end);
s32 BattlePadCheckNow(s32 mask);
void actionCommandDisp(f32 x, f32 y);
void actionCommandDisp2(f32 x, f32 y);

s32 battleAcResult_StickRotate(void* obj) {
    return *(s32*)((s32)obj + 0x1CB8);
}

void battleAcDelete_StickRotate(void* obj) {
    *(s32*)((s32)obj + 0x1C9C) = 0x3EC;
}

s32 _get_icon_id(s8 id) {
    switch (id) {
        case 0:
            return 0x84;
        case 1:
            return 0x85;
        case 2:
            return 0x82;
        case 3:
            return 0x7F;
        case 4:
            return 0x7E;
        case 5:
            return 0x7D;
        case 6:
            return 0x80;
        case 7:
            return 0x83;
        case 8:
        default:
            return 0x81;
    }
}

void battleAcDisp_StickRotate(void* camera, void* obj) {
    void (*disp)(f32, f32);
    register s32 work;
    s32 state;
    s32 timer;

    disp = actionCommandDisp2;
    work = (s32)obj + 0x1F20;
    if (*(s32*)((s32)obj + 0x1CC4) & 2) {
        disp = actionCommandDisp;
    }

    state = *(s32*)((s32)obj + 0x1C9C);
    if (state >= 0x3E8) {
        if (state >= 0x3EF) {
            return;
        }
        if (state >= 0x3EC) {
            goto outro;
        }
        goto steady;
    } else {
        if (state == 0x64) {
            goto steady;
        }
        if (state < 0x63) {
            return;
        }

intro:
        timer = *(s32*)(work + 0x20);
        *(f32*)(work + 0x14) = intplGetValue(4, 0x14 - timer, 0x14, float_neg300_8042683c, float_30_80426840);
        disp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
        timer = *(s32*)(work + 0x20);
        if (timer > 0) {
            *(s32*)(work + 0x20) = timer - 1;
        }
    }
    return;

steady:
    *(f32*)(work + 0x14) = float_30_80426840;
    disp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
    return;

outro:
    timer = *(s32*)(work + 0x20);
    if (timer > 0) {
        *(s32*)(work + 0x20) = timer - 1;
    }
    timer = *(s32*)(work + 0x20);
    if (timer <= 0x28) {
        *(f32*)(work + 0x14) = intplGetValue(4, timer, 0x28, float_neg300_8042683c, float_30_80426840);
    } else {
        *(f32*)(work + 0x14) = float_30_80426840;
    }
    disp(*(f32*)(work + 0x14), *(f32*)(work + 0x18));
}

s32 _CalcRollDir(void) {
    void* battleWork = _battleWorkPointer;
    s8 current = *(s8*)((s32)battleWork + 0x1F4C);
    s8 previous = *(s8*)((s32)battleWork + 0x1F4D);
    s32 diff;

    if (current <= 0) {
        return 0;
    }
    if (previous <= 0) {
        return 0;
    }
    if (current == previous) {
        return *(u8*)((s32)battleWork + 0x1F50);
    }
    if (current == 1 && previous == 8) {
        return 1;
    }
    if (previous == 1 && current == 8) {
        return -1;
    }

    diff = current - previous;
    if (diff < 0) {
        diff = -diff;
    }
    if (diff <= 1) {
        return current - previous;
    }
    return 0;
}

s32 _GetInputDir(void) {
    void* battleWork = _battleWorkPointer;

    if (*(void**)((s32)battleWork + 0x1CD0) != 0 && BattlePadCheckNow((s32)*(void**)((s32)battleWork + 0x1CD0)) == 0) {
        return -1;
    }
    if (BattlePadCheckNow(0x10000) != 0) {
        if (BattlePadCheckNow(0x80000) != 0) {
            return 2;
        }
        if (BattlePadCheckNow(0x40000) != 0) {
            return 8;
        }
        return 1;
    }
    if (BattlePadCheckNow(0x20000) != 0) {
        if (BattlePadCheckNow(0x80000) != 0) {
            return 4;
        }
        if (BattlePadCheckNow(0x40000) != 0) {
            return 6;
        }
        return 5;
    }
    if (BattlePadCheckNow(0x80000) != 0) {
        return 3;
    }
    if (BattlePadCheckNow(0x40000) != 0) {
        return 7;
    }
    return 0;
}


/* CHATGPT STUB FILL: main/action/ac_stick_rotate 20260624_184008 */

/* stub-fill: actionCommandDisp2 | prototype_only | source_prototype */
void actionCommandDisp2(f32 x, f32 y) {
    extern void iconDispGx(f32* pos, s32 flags, s32 iconId, f32 scale);
    extern f32 float_neg200_80426830;
    extern f32 float_70_80426834;
    extern f32 float_1_80426838;
    void* battle = _battleWorkPointer;
    s32 state = *(s32*)((s32)battle + 0x1C9C);
    s8* extra = (s8*)((s32)battle + 0x1F4C);
    f32 pos[3];
    s32 dir;
    s32 current;
    s32 threshold;

    if (state < 0x3E8) {
        if (state >= 0x63 && state < 0x65) {
            pos[0] = float_neg200_80426830 + x;
            pos[1] = float_70_80426834 + y;
            pos[2] = 0.0f;
            iconDispGx(pos, 0x10, 0x81, float_1_80426838);
        }
        return;
    }

    if (state >= 0x3EF) {
        return;
    }

    if (state < 0x3EA) {
        extra[7]++;
        threshold = (*(s32*)((s32)battle + 0x1CD8) + *(s32*)((s32)battle + 0x1CDC)) / 2;
        if ((u8)extra[7] >= threshold) {
            extra[7] = 0;
            dir = extra[4];
            if (dir == 0) {
                dir = -1;
            }
            current = extra[6];
            if (current == 0) {
                extra[6] = 1;
            } else {
                current += dir;
                extra[6] = current;
                if (extra[6] >= 9) {
                    extra[6] = 1;
                } else if (extra[6] <= 0) {
                    extra[6] = 8;
                }
            }
        }
        pos[0] = float_neg200_80426830 + x;
        pos[1] = float_70_80426834 + y;
        pos[2] = 0.0f;
        iconDispGx(pos, 0x10, _get_icon_id(extra[6]), float_1_80426838);
    } else {
        pos[0] = float_neg200_80426830 + x;
        pos[1] = float_70_80426834 + y;
        pos[2] = 0.0f;
        iconDispGx(pos, 0x10, 0x81, float_1_80426838);
    }
}

/* stub-fill: actionCommandDisp | prototype_only | source_prototype */
void actionCommandDisp(f32 x, f32 y) {
    extern void iconDispGx(f32* pos, s32 flags, s32 iconId, f32 scale);
    extern f32 float_neg200_80426830;
    extern f32 float_70_80426834;
    extern f32 float_1_80426838;
    void* battle = _battleWorkPointer;
    s32 state = *(s32*)((s32)battle + 0x1C9C);
    s8* extra = (s8*)((s32)battle + 0x1F4C);
    f32 pos[3];
    s32 icon;

    if (state == 0x3E8) {
        icon = _get_icon_id(extra[2]);
        pos[0] = float_neg200_80426830 + x;
        pos[1] = float_70_80426834 + y;
        pos[2] = 0.0f;
        iconDispGx(pos, 0x10, icon, float_1_80426838);
        return;
    }

    if (state < 0x3E8) {
        if (state >= 0x63 && state < 0x65) {
            pos[0] = float_neg200_80426830 + x;
            pos[1] = float_70_80426834 + y;
            pos[2] = 0.0f;
            iconDispGx(pos, 0x10, 0x81, float_1_80426838);
        }
        return;
    }

    if (state >= 0x3EF) {
        return;
    }

    if (state < 0x3EA) {
        if (extra[5] < *(s32*)((s32)battle + 0x1CD8)) {
            if (*(u16*)(extra + 8) < 2) {
                icon = _get_icon_id(extra[2]);
            } else {
                icon = _get_icon_id(extra[1]);
            }
        } else {
            icon = _get_icon_id(extra[2]);
        }
        pos[0] = float_neg200_80426830 + x;
        pos[1] = float_70_80426834 + y;
        pos[2] = 0.0f;
        iconDispGx(pos, 0x10, icon, float_1_80426838);
    } else {
        pos[0] = float_neg200_80426830 + x;
        pos[1] = float_70_80426834 + y;
        pos[2] = 0.0f;
        iconDispGx(pos, 0x10, 0x81, float_1_80426838);
    }
}

/* stub-fill: battleAcMain_StickRotate | missing_definition | ghidra_signature */
s32 battleAcMain_StickRotate(s32 work)  {
    extern void* memset(void*, s32, s32);
    extern s32 BattlePadCheckNow(s32);
    extern s32 _GetInputDir(void);
    extern s32 _CalcRollDir(void);
    extern s32 irand(s32);
    s32 state;
    s32 dir;
    s32 roll;
    s32 next;
    s32 done = 0;
    s8* current = (s8*)(work + 0x1F4C);
    state=*(s32*)(work+0x1C9C);
    if(state==0) {
        *(s32*)(work+0x1CB8)=1;
        memset((void*)(work+0x1F20),0,0x2C);
        *(f32*)(work+0x1F34)=-300.0f;
        *(f32*)(work+0x1F38)=0.0f;
        *(s32*)(work+0x1F40)=20;
        *(s8*)(work+0x1F4E)=0;
        *(s8*)(work+0x1F4F)=0;
        *(s8*)(work+0x1F52)=0;
        *(s8*)(work+0x1F53)=0;
        *(s16*)(work+0x1F5C)=0;
        *(s8*)(work+0x1F51)=0;
        *(s32*)(work+0x1C9C)=99;
        return 1;
    }
    if(state==99||state==100) {
        if(state==100) {
            *(s32*)(work+0x1F58)=0x78;
            *(s32*)(work+0x1C9C)=1000;
            *(s8*)(work+0x1F50)=0;
            *(s16*)(work+0x1F54)=0;
            *(s32*)(work+0x1CE8)=0;
            if(*(u32*)(work+0x1CC4)&2) {
                *(s8*)(work+0x1F4E)=3;
                *(s8*)(work+0x1F4F)=2;
            }
        }
        dir=_GetInputDir();
        *current=(s8)dir;
        if((dir==-1||(*(s32*)(work+0x1CD0)==0&&dir<1))&&*(s16*)(work+0x1D18)<1) {
            if(--*(s32*)(work+0x1F58)<1) {
                *(s32*)(work+0x1CB8)=0;
                *(s32*)(work+0x1C9C)=1002;
            }
        } else {
            *(s32*)(work+0x1C9C)=1001;
            *(s8*)(work+0x1F4D)=0;
            (*(s32*)(work+0x1CE8))++;
            *(s32*)(work+0x1F58)=*(s32*)(work+0x1CCC);
            if(*(u32*)(work+0x1CC4)&2) {
                *(s8*)(work+0x1F50)=-1;
                *(s8*)(work+0x1F4D)=4;
            }
        }
        return 1;
    }
    if(state==1001) {
        if(*(s16*)(work+0x1D18)>0) {
            (*(s16*)(work+0x1D18))--;
            *(s32*)(work+0x1CB8)=2;
            *(s32*)(work+0x1C9C)=1002;
            *(s32*)(work+0x1CE8)=*(s32*)(work+0x1CC8);
        }
        (*(s32*)(work+0x1F58))--;
        (*(s8*)(work+0x1F51))++;
        if(*(s32*)(work+0x1CD0)!=0&&BattlePadCheckNow(*(s32*)(work+0x1CD0))==0)*(s32*)(work+0x1F58)=0;
        if(*(s32*)(work+0x1F58)<0) {
            if(*(s32*)(work+0x1CD4)<=*(s32*)(work+0x1CE8)&&BattlePadCheckNow(*(s32*)(work+0x1CD0))==0)*(u32*)(work+0x1CB8)|=2;
            done=1;
        } else {
            dir=_GetInputDir();
            *current=(s8)dir;
            if(dir==-1)done=1;
            else if(dir==0) {
                *(s8*)(work+0x1F50)=0;
                if((*(u32*)(work+0x1CC4)&1)&&*(s8*)(work+0x1F4D)!=0)done=1;
            } else if(*(s8*)(work+0x1F4D)!=0) {
                roll=(s8)_CalcRollDir();
                if(roll==0) {
                    *(s8*)(work+0x1F50)=0;
                } else if(*(s8*)(work+0x1F50)==0) {
                    *(s8*)(work+0x1F50)=(s8)roll;
                    next=dir+roll;
                    if(next>8)next=1;
                    if(next<1)next=8;
                    *(s8*)(work+0x1F4E)=(s8)dir;
                    *(s8*)(work+0x1F4F)=(s8)next;
                } else if(*(s8*)(work+0x1F50)!=(s8)roll)done=1;
                if(*(s8*)(work+0x1F4D)!=*current) {
                    (*(s16*)(work+0x1F54))++;
                    *(s32*)(work+0x1CE8)=((u16)*(s16*)(work+0x1F54)+7)>>3;
                    if(*(s32*)(work+0x1CE8)>*(s32*)(work+0x1CC8))*(s32*)(work+0x1CE8)=*(s32*)(work+0x1CC8);
                }
            }
            *(s8*)(work+0x1F4D)=*current;
        }
        if(done) {
            *(u32*)(work+0x1CB8)&=~1;
            *(s32*)(work+0x1C9C)=1002;
        }
        return 1;
    }
    if(state==1002) {
        *(s32*)(work+0x1C9C)=1003;
        return 1;
    }
    if(state==1003) {
        *(s32*)(work+0x1F60)=60;
        *(s32*)(work+0x1F40)=60;
        *(s32*)(work+0x1C9C)=1005;
        return 1;
    }
    if(state==1005) {
        if(--*(s32*)(work+0x1F60)<1)*(s32*)(work+0x1C9C)=1006;
        return 1;
    }
    if(state==1006) {
        *(s32*)(work+0x1CA0)=0;
        *(s32*)(work+0x1CA4)=0;
        *(s32*)(work+0x1CA8)=0;
        *(s32*)(work+0x1CAC)=0;
        return 1;
    }
    if(irand(100)<0) {
        *(s32*)(work+0x1CB8)=2;
        (*(s32*)(work+0x1CB4))++;
    }
    return 0;
}

