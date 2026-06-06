#include "peach.h"

const char str_c_peach_802fd210[] = "c_peach";
const char str_c_zako_n_802fd218[] = "c_zako_n";
f32 float_0_804265b0 = 0.0f;
char str_P_T_1_804265b4[] = "P_T_1";
char str_P_R_1_804265bc[] = "P_R_1";
char str_P_W_1_804265c4[] = "P_W_1";
char str_P_S_1_804265cc[] = "P_S_1";
char str_P_U_1_804265d4[] = "P_U_1";
char str_P_N_1_804265dc[] = "P_N_1";
f32 float_47_804265e4 = 47.0f;
f32 float_20_804265e8 = 20.0f;
char str_S_1_804265ec[] = "S_1";
char str_T_1_804265f0[] = "T_1";
f32 float_30_804265f4 = 30.0f;

void* marioGetPtr(void);
void marioSetCharMode(s32 mode);
void peachChgPose(void* pose);
void animPoseRelease(s32 poseId);
s32 animPoseEntry(const char* name, s32 mode);
void npcSetMarioAutoTalkPose(const char* stay, const char* talk);

void peachPreInit(void) {
}

void peachSetStsAftershower(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x10;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsAngry(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x100;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsCarry(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x400;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsHappy(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x40;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsInvisible(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x20;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsOrdinary(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x200;
    peachChgPose(str_P_S_1_804265cc);
}

void peachSetStsUneasy(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x10) &= ~0x7F0;
    *(u32*)((s32)mario + 0x10) |= 0x80;
    peachChgPose(str_P_S_1_804265cc);
}


void peachChgPose(void* pose) {
}


u8 peach_dash(void) {
    return 0;
}


u8 peach_stay(void) {
    return 0;
}


s32 peachMain(void) {
    return 0;
}


u8 peach_walk(void) {
    return 0;
}


void peachSetStsNormal(void) {
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0x10);

    if (flags & 0x2000) {
        s32 mode;

        mario = marioGetPtr();
        mode = *(u8*)((s32)mario + 0x3C);
        if (mode != 1) {
            marioSetCharMode(1);
        }

        if (*(u32*)((s32)mario + 0x10) & 0x2000) {
            f32 height;
            f32 width;

            *(u32*)((s32)mario + 0x10) &= ~0x2000;
            if (*(s32*)((s32)mario + 0x22C) >= 0) {
                animPoseRelease(*(s32*)((s32)mario + 0x22C));
            }
            *(s32*)((s32)mario + 0x22C) = animPoseEntry(str_c_peach_802fd210, 2);
            npcSetMarioAutoTalkPose(str_P_S_1_804265cc, str_P_T_1_804265b4);
            height = float_47_804265e4;
            width = float_20_804265e8;
            *(f32*)((s32)mario + 0x1BC) = height;
            *(f32*)((s32)mario + 0x1B8) = width;
        }
    } else {
        *(u32*)((s32)mario + 0x10) &= ~0x7F0;
        peachChgPose(str_P_S_1_804265cc);
    }
}

void peachTransformOff(void) {
    void* mario = marioGetPtr();
    s32 mode = *(u8*)((s32)mario + 0x3C);

    if (mode != 1) {
        marioSetCharMode(1);
    }

    if (*(u32*)((s32)mario + 0x10) & 0x2000) {
        f32 height;
        f32 width;

        *(u32*)((s32)mario + 0x10) &= ~0x2000;
        if (*(s32*)((s32)mario + 0x22C) >= 0) {
            animPoseRelease(*(s32*)((s32)mario + 0x22C));
        }
        *(s32*)((s32)mario + 0x22C) = animPoseEntry(str_c_peach_802fd210, 2);
        npcSetMarioAutoTalkPose(str_P_S_1_804265cc, str_P_T_1_804265b4);
        height = float_47_804265e4;
        width = float_20_804265e8;
        *(f32*)((s32)mario + 0x1BC) = height;
        *(f32*)((s32)mario + 0x1B8) = width;
    }
}

void peachTransformOn(void) {
    void* mario = marioGetPtr();
    s32 mode = *(u8*)((s32)mario + 0x3C);

    if (mode != 1) {
        marioSetCharMode(1);
    }

    if (!(*(u32*)((s32)mario + 0x10) & 0x2000)) {
        f32 scale;

        *(u32*)((s32)mario + 0x10) |= 0x2000;
        if (*(s32*)((s32)mario + 0x22C) >= 0) {
            animPoseRelease(*(s32*)((s32)mario + 0x22C));
        }
        *(s32*)((s32)mario + 0x22C) = animPoseEntry(str_c_zako_n_802fd218, 2);
        npcSetMarioAutoTalkPose(str_S_1_804265ec, str_T_1_804265f0);
        scale = float_30_804265f4;
        *(f32*)((s32)mario + 0x1BC) = scale;
        *(f32*)((s32)mario + 0x1B8) = scale;
    }
}

u8 peach_talk(void) {
    return 0;
}
