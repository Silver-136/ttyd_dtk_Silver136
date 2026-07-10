#include "motion/mot_walk.h"


u8 mot_dash(void) {
    return 0;
}


u8 mot_walk(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 marioWalkDashSe(void* pHit, u32 interval) {
    extern void* marioGetPtr(void);
    extern u32 hitGetAttr(void* hit);
    extern void psndSFXOn_3D(s32 sfx, void* pos);
    extern s32 strncmp(const char* a, const char* b, u32 n);
    extern void* gp;
    extern char str_mri_804208a4;
    void* mario;
    u32 attr;
    u32 frame;

    mario = marioGetPtr();
    attr = hitGetAttr(pHit);
    frame = *(u32*)((s32)mario + 0x48);

    if ((frame % interval) == 0) {
        if (attr & 0x100) {
            psndSFXOn_3D(0x143, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x200000) {
            psndSFXOn_3D(0x145, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x100000) {
            psndSFXOn_3D(0x147, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x1000) {
            if (strncmp((char*)((s32)gp + 0x12C), &str_mri_804208a4, 3) != 0) {
                psndSFXOn_3D(0x149, (void*)((s32)mario + 0x8C));
            } else {
                psndSFXOn_3D(0x143, (void*)((s32)mario + 0x8C));
            }
        } else if (attr & 0x400000) {
            psndSFXOn_3D(0x14B, (void*)((s32)mario + 0x8C));
        } else {
            psndSFXOn_3D(0x141, (void*)((s32)mario + 0x8C));
        }
    } else if ((frame % (interval >> 1)) == 0) {
        if (attr & 0x100) {
            psndSFXOn_3D(0x142, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x200000) {
            psndSFXOn_3D(0x144, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x100000) {
            psndSFXOn_3D(0x146, (void*)((s32)mario + 0x8C));
        } else if (attr & 0x1000) {
            if (strncmp((char*)((s32)gp + 0x12C), &str_mri_804208a4, 3) != 0) {
                psndSFXOn_3D(0x148, (void*)((s32)mario + 0x8C));
            } else {
                psndSFXOn_3D(0x142, (void*)((s32)mario + 0x8C));
            }
        } else if (attr & 0x400000) {
            psndSFXOn_3D(0x14A, (void*)((s32)mario + 0x8C));
        } else {
            psndSFXOn_3D(0x140, (void*)((s32)mario + 0x8C));
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

f32 marioGetDashSpd(void) {
    extern void* marioGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern f32 float_0p5_8042088c;
    void* mario = marioGetPtr();
    f32 speed = *(f32*)((s32)mario + 0x188);

    if (*(u32*)mario & 0x100000) {
        s32 x = *(s8*)((s32)mario + 0x252);
        s32 y = *(s8*)((s32)mario + 0x253);
        speed = *(f32*)((s32)mario + 0x184);
        if (x * x + y * y <= 0xBD1) {
            speed *= float_0p5_8042088c;
        }
    } else if (marioBgmodeChk() == 1) {
        speed *= float_0p5_8042088c;
    }
    speed *= *(f32*)((s32)mario + 0x228);
    return speed;
}

float marioGetWalkSpd(void) {
    extern void* marioGetPtr(void);
    extern s32 marioBgmodeChk(void);
    extern f32 float_0p5_8042088c;
    void* mario = marioGetPtr();
    f32 speed = *(f32*)((s32)mario + 0x184);

    if (*(u32*)mario & 0x100000) {
        register s32 x = *(s8*)((s32)mario + 0x252);
        register s32 y = *(s8*)((s32)mario + 0x253);
        if (x * x + y * y <= 0xBD1) {
            speed *= float_0p5_8042088c;
        }
    } else if (marioBgmodeChk() == 1) {
        speed *= float_0p5_8042088c;
    }
    speed *= *(f32*)((s32)mario + 0x228);
    return speed;
}

