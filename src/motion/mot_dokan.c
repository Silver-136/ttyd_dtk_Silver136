#include "motion/mot_dokan.h"

extern void* marioGetPtr(void);
extern void* partyGetPtr(s32 id);
extern f32 revise360(f32 angle);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void _partyEntry(int param_1) {
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 partyEntryPos(s32 party, f32 x, f32 y, f32 z);
    extern s32 partyEntry2Pos(s32 party, f32 x, f32 y, f32 z);
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



void mot_dokan(void) {
    extern void _partyEntry(s32 mode);
    extern void marioPaperOn(char* name);
    extern void marioPaperOff(void);
    extern void marioPaperLightOff(void);
    extern void marioChgPaper(char* name);
    extern void marioChgPose(char* name);
    extern s32 marioGetPartyId(void);
    extern s32 marioGetExtraPartyId(void);
    extern void partyDokanEnd(s32 id, s32 dokan);
    extern void marioUpdateCamPos(void);
    extern f32 vec3_802f4548[];
    extern char str_PM_D_1B_802f461c[];
    extern char str_M_Z_1_80423044[];
    u8* player = marioGetPtr();
    u32 state;
    u8 dokan;
    f32 scale;
    u8* party;
    s32 partyId;
    u8* data = (u8*)vec3_802f4548;

    if ((*(u32*)(player + 0xC) & 1) != 0) {
        *(u32*)(player + 0xC) &= ~1;
        *(u32*)(player + 4) |= 0x1000;
        *(u32*)(player + 4) |= 0x8;
        *(u32*)(player + 4) |= 0x100;
        *(f32*)(player + 0x2BC) = 0.0f;
        *(f32*)(player + 0x2C0) = *(f32*)(player + 0x94);
        *(s32*)(player + 0x48) = 0;
        *(s16*)(player + 0x50) = 0;
        *(f32*)(player + 0x180) = 0.0f;
        dokan = *(u8*)(player + 0x3E);
        if (dokan == 0) {
            _partyEntry(0);
            *(u32*)(player + 0x44) = 30;
        } else if (dokan == 1) {
            marioPaperOn((char*)(data + 0xB4));
            marioChgPaper((char*)(data + 0xC0));
            marioChgPose(str_M_Z_1_80423044);
            marioPaperLightOff();
            *(u32*)(player + 0x44) = 20;
            *(f32*)(player + 0x2B8) = 30.0f;
            _partyEntry(1);
        } else if (dokan == 2 || dokan == 3) {
            *(u32*)(player + 0x44) = 10;
        } else {
            marioPaperOn((char*)(data + 0xC8));
            marioPaperLightOff();
            marioChgPaper((char*)(data + 0xC0));
            marioChgPose(str_M_Z_1_80423044);
            *(u32*)(player + 0x44) = 40;
            *(f32*)(player + 0x2B8) = 30.0f;
            _partyEntry(1);
        }
    }

    state = *(u32*)(player + 0x44);
    if (state == 0) {
        *(s32*)(player + 0x48) -= 1;
        if (*(s32*)(player + 0x48) < 1) {
            *(f32*)(player + 0xBC) += *(f32*)(player + 0x2B8);
            scale = *(f32*)(player + 0xC8) - 0.02f;
            if (scale < 0.0f) scale = 0.0f;
            *(f32*)(player + 0xC8) = scale;
            *(f32*)(player + 0xCC) = scale;
            *(f32*)(player + 0xD0) = scale;
            *(f32*)(player + 0xB4) = 16.0f * scale;
            *(f32*)(player + 0x9C) = 16.0f - *(f32*)(player + 0xB4);
            *(f32*)(player + 0xA0) = -3.0f;
        }
    } else if (state == 10) {
        *(f32*)(player + 0x1AC) = revise360(*(f32*)(player + 0x1AC) + *(f32*)(player + 0x2B8));
        scale = *(f32*)(player + 0xC8) - 0.005f;
        if (scale < 0.0f) scale = 0.0f;
        *(f32*)(player + 0xC8) = scale;
        *(f32*)(player + 0xCC) = scale;
        *(f32*)(player + 0xD0) = scale;
    } else if (state == 20 || state == 21 || state == 30) {
        *(f32*)(player + 0x1AC) = revise360(*(f32*)(player + 0x1AC) + *(f32*)(player + 0x2B8));
        scale = *(f32*)(player + 0xC8) + 0.05f;
        if (scale > 1.0f) scale = 1.0f;
        *(f32*)(player + 0xC8) = scale;
        *(f32*)(player + 0xCC) = scale;
        *(f32*)(player + 0xD0) = scale;
    } else if (state == 40) {
        *(f32*)(player + 0xBC) += *(f32*)(player + 0x2B8);
        scale = *(f32*)(player + 0xC8) + 0.03f;
        if (scale > 1.0f) scale = 1.0f;
        *(f32*)(player + 0xC8) = scale;
        *(f32*)(player + 0xCC) = scale;
        *(f32*)(player + 0xD0) = scale;
        *(f32*)(player + 0xB4) = 16.0f * scale;
        *(f32*)(player + 0x9C) = 16.0f - *(f32*)(player + 0xB4);
        *(f32*)(player + 0xA0) = -3.0f;
        partyId = marioGetPartyId();
        party = partyGetPtr(partyId);
        if (party != 0) {
            *(f32*)(party + 0x58) = *(f32*)(player + 0x8C);
            *(f32*)(party + 0x5C) = *(f32*)(player + 0x90);
            *(f32*)(party + 0x60) = *(f32*)(player + 0x94);
            *(f32*)(party + 0x74) = *(f32*)(player + 0xC8);
            *(f32*)(party + 0x78) = *(f32*)(player + 0xCC);
            *(f32*)(party + 0x7C) = *(f32*)(player + 0xD0);
            *(f32*)(party + 0x8C) = *(f32*)(player + 0xB4);
            *(f32*)(party + 0x80) = *(f32*)(player + 0x9C);
            *(f32*)(party + 0x84) = *(f32*)(player + 0xA0);
        }
        partyId = marioGetExtraPartyId();
        party = partyGetPtr(partyId);
        if (party != 0) {
            *(f32*)(party + 0x58) = *(f32*)(player + 0x8C);
            *(f32*)(party + 0x5C) = *(f32*)(player + 0x90);
            *(f32*)(party + 0x60) = *(f32*)(player + 0x94);
            *(f32*)(party + 0x74) = *(f32*)(player + 0xC8);
            *(f32*)(party + 0x78) = *(f32*)(player + 0xCC);
            *(f32*)(party + 0x7C) = *(f32*)(player + 0xD0);
            *(f32*)(party + 0x8C) = *(f32*)(player + 0xB4);
            *(f32*)(party + 0x80) = *(f32*)(player + 0x9C);
            *(f32*)(party + 0x84) = *(f32*)(player + 0xA0);
        }
    } else if (state == 0x65) {
        *(f32*)(player + 0x1AC) += *(f32*)(player + 0x2B8);
        if (*(f32*)(player + 0x1AC) >= 540.0f) {
            marioChgPaper(str_PM_D_1B_802f461c);
            *(s32*)(player + 0x48) = 20;
            *(u32*)(player + 0x44) = 0x66;
            *(f32*)(player + 0xBC) = 0.0f;
            partyDokanEnd(marioGetPartyId(), (s8)*(u8*)(player + 0x3E));
            partyDokanEnd(marioGetExtraPartyId(), (s8)*(u8*)(player + 0x3E));
        }
    } else if (state == 0x66) {
        *(s32*)(player + 0x48) -= 1;
        if (*(s32*)(player + 0x48) < 1) {
            marioUpdateCamPos();
            *(u32*)(player + 0x44) = 0x67;
        }
    } else if (state == 0x67) {
        marioPaperOff();
        *(u32*)(player + 4) &= ~0x1804;
        marioChgPaper(str_PM_D_1B_802f461c);
    }
}

const f32 vec3_802f4548[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f4554[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f4560[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f456c[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f4578[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f4584[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f4590[3] = { 0.0f, 16.0f, 0.0f };
const f32 vec3_802f459c[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f45a8[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f45b4[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f45c0[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f45cc[3] = { 0.0f, 16.0f, 0.0f };
const f32 vec3_802f45d8[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802f45e4[3] = { 1.0f, 1.0f, 1.0f };
const f32 vec3_802f45f0[3] = { 1.0f, 1.0f, 1.0f };

const char str_p_dokan_y_802f45fc[] = "p_dokan_y";
const char str_PM_D_1A_802f4608[] = "PM_D_1A";
const char str_p_dokan_x_802f4610[] = "p_dokan_x";
const char str_PM_D_1B_802f461c[] = "PM_D_1B";

const f32 float_0_80423040 = 0.0f;
const char str_M_Z_1_80423044[] = "M_Z_1";
const f32 float_30_8042304c = 30.0f;
const f32 float_90_80423050 = 90.0f;
const f32 float_270_80423054 = 270.0f;
const f32 float_neg30_80423058 = -30.0f;
const f32 float_neg0p005_8042305c = -0.005f;
const f32 float_neg2_80423060 = -2.0f;
const f32 float_0p03_80423064 = 0.03f;
const f32 float_1_80423068 = 1.0f;
const f32 float_0p05_8042306c = 0.05f;
const f32 float_neg0p02_80423070 = -0.02f;
const f32 float_16_80423074 = 16.0f;
const f32 float_neg3_80423078 = -3.0f;
const f32 float_540_8042307c = 540.0f;
const f32 float_180_80423080 = 180.0f;
const f32 float_120_80423084 = 120.0f;
const f32 float_neg120_80423088 = -120.0f;
