#include "motion/mot_dokan.h"


void mot_dokan(void) {
    extern void* marioGetPtr(void);
    extern void _partyEntry(s32 mode);
    extern void marioPaperOn(char* name);
    extern void marioPaperOff(void);
    extern void marioPaperLightOff(void);
    extern void marioChgPaper(char* name);
    extern void marioChgPose(char* name);
    extern f32 revise360(f32 angle);
    extern char str_p_dokan_y_802f45fc[];
    extern char str_p_dokan_x_802f4610[];
    extern char str_PM_D_1A_802f4608[];
    extern char str_PM_D_1B_802f461c[];
    extern char str_M_Z_1_80423044[];
    u8* player = marioGetPtr();
    u32 state;
    u8 dokan;
    f32 scale;

    if ((*(u32*)(player + 8) & 2) != 0) {
        *(u32*)(player + 8) &= ~2;
        *(u32*)(player + 4) |= 0x1804;
        *(f32*)(player + 0x214) = 0.0f;
        *(f32*)(player + 0x218) = *(f32*)(player + 0x94);
        *(s32*)(player + 0x20C) = 0;
        *(s32*)(player + 0x1F8) = 0;
        *(f32*)(player + 0x1B8) = 0.0f;
        dokan = *(u8*)(player + 0x3A4);
        if (dokan == 0) {
            _partyEntry(0);
            *(u32*)(player + 0x14) = 30;
        } else if (dokan == 1) {
            marioPaperOn(str_p_dokan_y_802f45fc);
            marioChgPaper(str_PM_D_1A_802f4608);
            marioChgPose(str_M_Z_1_80423044);
            marioPaperLightOff();
            *(u32*)(player + 0x14) = 20;
            *(f32*)(player + 0x210) = 30.0f;
            _partyEntry(1);
        } else if (dokan == 2 || dokan == 3) {
            *(u32*)(player + 0x14) = 10;
        } else {
            marioPaperOn(str_p_dokan_x_802f4610);
            marioPaperLightOff();
            marioChgPaper(str_PM_D_1A_802f4608);
            marioChgPose(str_M_Z_1_80423044);
            *(u32*)(player + 0x14) = 40;
            *(f32*)(player + 0x210) = 30.0f;
            _partyEntry(1);
        }
    }

    state = *(u32*)(player + 0x14);
    if (state == 10) {
        *(f32*)(player + 0x188) = revise360(*(f32*)(player + 0x188) + *(f32*)(player + 0x210));
        scale = *(f32*)(player + 0x144) - 0.005f;
        if (scale < 0.0f) scale = 0.0f;
        *(f32*)(player + 0x144) = scale;
        *(f32*)(player + 0x148) = scale;
        *(f32*)(player + 0x14C) = scale;
    } else if (state == 20 || state == 30) {
        *(f32*)(player + 0x188) = revise360(*(f32*)(player + 0x188) + *(f32*)(player + 0x210));
        scale = *(f32*)(player + 0x144) + 0.05f;
        if (scale > 1.0f) scale = 1.0f;
        *(f32*)(player + 0x144) = scale;
        *(f32*)(player + 0x148) = scale;
        *(f32*)(player + 0x14C) = scale;
    } else if (state == 40) {
        *(f32*)(player + 0x17C) += *(f32*)(player + 0x210);
        scale = *(f32*)(player + 0x144) + 0.03f;
        if (scale > 1.0f) scale = 1.0f;
        *(f32*)(player + 0x144) = scale;
        *(f32*)(player + 0x148) = scale;
        *(f32*)(player + 0x14C) = scale;
    } else if (state == 0x67) {
        marioPaperOff();
        *(u32*)(player + 4) &= ~0x1804;
        marioChgPaper(str_PM_D_1B_802f461c);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void _partyEntry(int param_1) {
    extern void* marioGetPtr(void);
    extern void* partyGetPtr(s32 id);
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 partyEntryPos(s32 party, f32 x, f32 y, f32 z);
    extern s32 partyEntry2Pos(s32 party, f32 x, f32 y, f32 z);
    extern f32 revise360(f32 angle);
    extern char vec3_802f4548[];
    extern char vec3_802f4554[];
    extern f32 float_120_80423084;
    extern f32 float_180_80423080;
    extern f32 float_neg120_80423088;
    void* mario;
    s32 count;
    s32 i;
    char* slot;
    s32 partyId;
    void* party;
    f32 angle;

    mario = marioGetPtr();
    if ((*(u32*)mario & 0x02000000) == 0) {
        count = 0;
        if (partyGetPtr(*(s8*)((s32)mario + 0x245)) != 0) {
            count = 1;
        }
        if (partyGetPtr(*(s8*)((s32)mario + 0x246)) != 0) {
            count++;
        }
        for (i = 0; i < 2; i++) {
            slot = (char*)mario + i;
            partyId = *(s8*)(slot + 0x245);
            if (partyId < 0) {
                if (*(s8*)(slot + 0x247) != 0) {
                    if (i == 0) {
                        partyId = partyEntryPos(*(u8*)(slot + 0x247), *(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x90), *(f32*)((s32)mario + 0x94));
                    } else {
                        partyId = partyEntry2Pos(*(u8*)(slot + 0x247), *(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x90), *(f32*)((s32)mario + 0x94));
                    }
                    if (partyId >= 0) {
                        *(u8*)(slot + 0x245) = partyId;
                        party = partyGetPtr(partyId);
                        partyChgRunMode(party, 10);
                        if (param_1 == 0) {
                            *(s32*)((s32)party + 0x70) = *(s32*)(vec3_802f4548 + 0);
                            *(s32*)((s32)party + 0x74) = *(s32*)(vec3_802f4548 + 4);
                            *(s32*)((s32)party + 0x78) = *(s32*)(vec3_802f4548 + 8);
                        }
                        *(u32*)party |= 0x04000000;
                        *(u32*)party |= 0x02000000;
                        if (count == 1) {
                            angle = float_180_80423080;
                        } else {
                            angle = float_120_80423084;
                        }
                        *(u32*)((s32)party + 4) &= ~1;
                        *(f32*)((s32)party + 0x110) = revise360(*(f32*)((s32)mario + 0x1AC) + angle);
                        *(f32*)((s32)party + 0x10C) = *(f32*)((s32)mario + 0x1AC);
                        *(f32*)((s32)party + 0xFC) = *(f32*)((s32)mario + 0x1A0);
                    }
                }
            } else {
                party = partyGetPtr(partyId);
                partyChgRunMode(party, 10);
                if (param_1 == 0) {
                    *(s32*)((s32)party + 0x70) = *(s32*)(vec3_802f4554 + 0);
                    *(s32*)((s32)party + 0x74) = *(s32*)(vec3_802f4554 + 4);
                    *(s32*)((s32)party + 0x78) = *(s32*)(vec3_802f4554 + 8);
                }
                *(u32*)party |= 0x04000000;
                *(u32*)party |= 0x02000000;
                if (count == 1) {
                    angle = float_180_80423080;
                } else {
                    angle = float_neg120_80423088;
                }
                *(u32*)((s32)party + 4) &= ~1;
                *(f32*)((s32)party + 0x110) = revise360(*(f32*)((s32)mario + 0x1AC) + angle);
                *(f32*)((s32)party + 0x10C) = *(f32*)((s32)mario + 0x1AC);
                *(f32*)((s32)party + 0xFC) = *(f32*)((s32)mario + 0x1A0);
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

