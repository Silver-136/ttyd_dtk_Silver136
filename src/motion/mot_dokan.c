#include "motion/mot_dokan.h"

extern void* marioGetPtr(void);
extern void* partyGetPtr(s32 id);
extern f32 revise360(f32 angle);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw on
void _partyEntry(int param_1) {
    typedef struct RawVec3 {
        s32 x;
        s32 y;
        s32 z;
    } RawVec3;
    extern void partyChgRunMode(void* party, s32 mode);
    extern s32 partyEntryPos(s32 party, f32 x, f32 y, f32 z);
    extern s32 partyEntry2Pos(s32 party, f32 x, f32 y, f32 z);
    extern char vec3_802f4548[];
    extern char vec3_802f4554[];
    extern f32 float_120_80423084;
    extern f32 float_180_80423080;
    extern f32 float_neg120_80423088;
    void* mario;
    s32 i;
    s32 count;
    char* slot;
    s32 partyId;
    void* party;
    f32 angle;
    f32 x, y, z;

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
            partyId = *(u8*)(slot + 0x245);
            if ((s8)partyId < 0) {
                partyId = *(s8*)(slot + 0x247);
                x = *(f32*)((s32)mario + 0x8C);
                y = *(f32*)((s32)mario + 0x90);
                z = *(f32*)((s32)mario + 0x94);
                if (partyId != 0) {
                    if (i == 0) {
                        partyId = partyEntryPos(partyId, x, y, z);
                    } else {
                        partyId = partyEntry2Pos(partyId, x, y, z);
                    }
                    if (partyId >= 0) {
                        *(u8*)(slot + 0x245) = partyId;
                        party = partyGetPtr(partyId);
                        partyChgRunMode(party, 10);
                        if (param_1 == 0) {
                            *(RawVec3*)((s32)party + 0x70) = *(RawVec3*)vec3_802f4548;
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
                party = partyGetPtr((s8)partyId);
                partyChgRunMode(party, 10);
                if (param_1 == 0) {
                    *(RawVec3*)((s32)party + 0x70) = *(RawVec3*)vec3_802f4554;
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



#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_dokan(void) {
    typedef struct RawVec3 {
        s32 x;
        s32 y;
        s32 z;
    } RawVec3;
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
    extern void partyShadowOff(void* party);
    extern void marioSlitForceCancel(void);
    extern f32 toMovedir(f32 angle);
    extern f32 float_90_80423050;
    extern f32 float_270_80423054;
    extern f32 float_neg30_80423058;
    extern f32 float_30_8042304c;
    extern void marioOfsRotReset(void);
    extern void marioChgMot(s32 motion);
    extern void camFollowYOn(void);
    extern void camFollowYOff(void);
    extern f32 toMovedirSimple(f32 angle);
    extern f32 float_neg2_80423060;
    extern f32 vec3_802f4548[];
    extern char str_PM_D_1B_802f461c[];
    extern char str_M_Z_1_80423044[];
    extern f32 float_0_80423040;
    extern f32 float_0p03_80423064;
    extern f32 float_0p05_8042306c;
    extern f32 float_16_80423074;
    extern f32 float_1_80423068;
    extern f32 float_540_8042307c;
    extern f32 float_neg0p005_8042305c;
    extern f32 float_neg0p02_80423070;
    extern f32 float_neg3_80423078;
    u8* player;
    s32 state;
    s8 dokan;
    f32 scale;
    u8* party;
    s32 partyId;
    u8* data = (u8*)vec3_802f4548;

    player = marioGetPtr();

    if ((*(u32*)(player + 0xC) & 1) != 0) {
        *(u32*)(player + 0xC) &= ~1;
        *(u32*)(player + 4) |= 0x1000;
        *(u32*)(player + 4) |= 0x8;
        *(u32*)(player + 4) |= 0x100;
        *(f32*)(player + 0x2BC) = float_0_80423040;
        *(f32*)(player + 0x2C0) = *(f32*)(player + 0x94);
        *(s32*)(player + 0x48) = 0;
        *(s16*)(player + 0x50) = 0;
        *(f32*)(player + 0x180) = float_0_80423040;
        dokan = *(u8*)(player + 0x3E);
        if (dokan == 2) {
            *(u32*)(player + 0x44) = 10;
        } else if (dokan == 3) {
            *(u32*)(player + 0x44) = 10;
        } else if (dokan == 1) {
            marioPaperOn((char*)(data + 0xB4));
            marioChgPaper((char*)(data + 0xC0));
            marioChgPose(str_M_Z_1_80423044);
            marioPaperLightOff();
            *(u32*)(player + 4) |= 0x8;
            *(u32*)(player + 4) |= 0x100;
            *(f32*)(player + 0x1AC) = *(f32*)(player + 0x1B0);
            *(u32*)(player + 0x44) = 20;
            *(f32*)(player + 0x2B8) = float_0_80423040;
            *(f32*)(player + 0x2B8) = float_30_8042304c;
            *(RawVec3*)(player + 0xC8) = *(RawVec3*)(data + 0x18);
            _partyEntry(1);
        } else if (dokan == 0) {
            _partyEntry(0);
            *(u32*)(player + 0x44) = 30;
        } else if (dokan == 5) {
            *(u32*)(player + 4) |= 0x1000;
            party = partyGetPtr(0);
            if (party != 0) {
                partyShadowOff(party);
            }
            party = partyGetPtr(1);
            if (party != 0) {
                partyShadowOff(party);
            }
            *(RawVec3*)(player + 0x98) = *(RawVec3*)(data + 0x24);
            *(RawVec3*)(player + 0xA4) = *(RawVec3*)(data + 0x30);
            *(RawVec3*)(player + 0xBC) = *(RawVec3*)(data + 0x3C);
            *(RawVec3*)(player + 0xB0) = *(RawVec3*)(data + 0x48);
            marioSlitForceCancel();
            marioPaperOn((char*)(data + 0xC8));
            marioPaperLightOff();
            marioChgPaper((char*)(data + 0xC0));
            marioChgPose(str_M_Z_1_80423044);
            *(f32*)(player + 0x1AC) = *(f32*)(player + 0x1B0);
            *(f32*)(player + 0x2B8) = float_30_8042304c;
            *(s32*)(player + 0x48) = 20;
            *(u32*)(player + 0x44) = 0;
        } else if (dokan == 4) {
            *(RawVec3*)(player + 0xC8) = *(RawVec3*)(data + 0x54);
            *(RawVec3*)(player + 0x98) = *(RawVec3*)(data + 0x60);
            *(RawVec3*)(player + 0xA4) = *(RawVec3*)(data + 0x6C);
            *(RawVec3*)(player + 0xBC) = *(RawVec3*)(data + 0x78);
            *(RawVec3*)(player + 0xB0) = *(RawVec3*)(data + 0x84);
            *(f32*)(player + 0x1A4) = toMovedir(*(f32*)(player + 0x1AC));
            *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1A4);
            marioPaperOn((char*)(data + 0xC8));
            marioPaperLightOff();
            marioChgPaper((char*)(data + 0xC0));
            marioChgPose(str_M_Z_1_80423044);
            if (*(f32*)(player + 0x1B0) >= float_90_80423050 &&
                *(f32*)(player + 0x1B0) <= float_270_80423054) {
                *(f32*)(player + 0x2B8) = float_30_8042304c;
            } else {
                *(u32*)(player + 4) |= 0x8;
                *(f32*)(player + 0x2B8) = float_neg30_80423058;
            }
            *(u32*)(player + 4) |= 0x100;
            marioPaperLightOff();
            *(f32*)(player + 0x1AC) = *(f32*)(player + 0x1B0);
            *(u32*)(player + 0x44) = 40;
            _partyEntry(1);
            *(u32*)(player + 4) |= 0x1000;
            partyShadowOff(partyGetPtr(marioGetPartyId()));
            partyShadowOff(partyGetPtr(marioGetExtraPartyId()));
        }
    }

    state = *(u32*)(player + 0x44);
    switch (state) {
    case 10:
        *(f32*)(player + 0x1AC) = revise360(*(f32*)(player + 0x1AC) + *(f32*)(player + 0x2B8));
        *(f32*)(player + 0xC8) += float_neg0p005_8042305c;
        if (*(f32*)(player + 0xC8) < float_0_80423040) *(f32*)(player + 0xC8) = float_0_80423040;
        *(f32*)(player + 0xCC) = *(f32*)(player + 0xC8);
        *(f32*)(player + 0xD0) = *(f32*)(player + 0xC8);
        break;
    case 20:
        *(RawVec3*)(player + 0xC8) = *(RawVec3*)(data + 0x90);
        *(u32*)(player + 0x44) = 21;
        if ((*(u32*)player & 0x02000000) != 0) *(f32*)(player + 0xA8) = float_neg2_80423060;
        /* fall through */
    case 21:
        *(f32*)(player + 0x1AC) = revise360(*(f32*)(player + 0x1AC) + *(f32*)(player + 0x2B8));
        if ((*(u32*)player & 0x02000000) != 0) {
            *(f32*)(player + 0xC8) += float_0p03_80423064;
            if (*(f32*)(player + 0xC8) >= float_1_80423068) *(f32*)(player + 0xC8) = float_1_80423068;
            *(f32*)(player + 0xCC) = *(f32*)(player + 0xC8);
            *(f32*)(player + 0xD0) = *(f32*)(player + 0xC8);
        } else {
            *(f32*)(player + 0xC8) += float_0p05_8042306c;
            if (*(f32*)(player + 0xC8) >= float_1_80423068) *(f32*)(player + 0xC8) = float_1_80423068;
            *(f32*)(player + 0xCC) = *(f32*)(player + 0xC8);
            *(f32*)(player + 0xD0) = *(f32*)(player + 0xC8);
        }
        break;
    case 30:
        *(f32*)(player + 0x1AC) = revise360(*(f32*)(player + 0x1AC) + *(f32*)(player + 0x2B8));
        break;
    case 0:
        if (--*(s32*)(player + 0x48) <= 0) {
            *(f32*)(player + 0xBC) += *(f32*)(player + 0x2B8);
            *(f32*)(player + 0xC8) += float_neg0p02_80423070;
            if (*(f32*)(player + 0xC8) < float_0_80423040) *(f32*)(player + 0xC8) = float_0_80423040;
            *(f32*)(player + 0xCC) = *(f32*)(player + 0xC8);
            *(f32*)(player + 0xD0) = *(f32*)(player + 0xC8);
            *(f32*)(player + 0xB4) = float_16_80423074 * *(f32*)(player + 0xC8);
            *(f32*)(player + 0x9C) = float_16_80423074 - *(f32*)(player + 0xB4);
            *(f32*)(player + 0xA0) = float_neg3_80423078;
        }
        break;
    case 40:
        *(f32*)(player + 0xBC) += *(f32*)(player + 0x2B8);
        *(f32*)(player + 0xC8) += float_0p03_80423064;
        if (*(f32*)(player + 0xC8) >= float_1_80423068) *(f32*)(player + 0xC8) = float_1_80423068;
        *(f32*)(player + 0xCC) = *(f32*)(player + 0xC8);
        *(f32*)(player + 0xD0) = *(f32*)(player + 0xC8);
        *(f32*)(player + 0xB4) = float_16_80423074 * *(f32*)(player + 0xC8);
        *(f32*)(player + 0x9C) = float_16_80423074 - *(f32*)(player + 0xB4);
        *(f32*)(player + 0xA0) = float_neg3_80423078;
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            *(RawVec3*)(party + 0x58) = *(RawVec3*)(player + 0x8C);
            *(f32*)(party + 0x70) = *(f32*)(player + 0xC8);
            *(f32*)(party + 0x74) = *(f32*)(player + 0xCC);
            *(f32*)(party + 0x78) = *(f32*)(player + 0xD0);
            *(f32*)(party + 0x8C) = *(f32*)(player + 0xB4);
            *(f32*)(party + 0x80) = *(f32*)(player + 0x9C);
            *(f32*)(party + 0x84) = *(f32*)(player + 0xA0);
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != 0) {
            *(RawVec3*)(party + 0x58) = *(RawVec3*)(player + 0x8C);
            *(f32*)(party + 0x70) = *(f32*)(player + 0xC8);
            *(f32*)(party + 0x74) = *(f32*)(player + 0xCC);
            *(f32*)(party + 0x78) = *(f32*)(player + 0xD0);
            *(f32*)(party + 0x8C) = *(f32*)(player + 0xB4);
            *(f32*)(party + 0x80) = *(f32*)(player + 0x9C);
            *(f32*)(party + 0x84) = *(f32*)(player + 0xA0);
        }
        *(f32*)(player + 0xF0) = *(f32*)(player + 0x90);
        break;
    case 100:
        dokan = *(s8*)(player + 0x3E);
        if (dokan == 1) {
            *(u32*)(player + 0x44) = 110;
        } else if (dokan == 4) {
            marioUpdateCamPos();
            *(f32*)(player + 0xBC) = float_0_80423040;
            marioChgPaper((char*)(data + 0xD4));
            *(s32*)(player + 0x48) = 20;
            *(u32*)(player + 0x44) = 102;
            partyDokanEnd(marioGetPartyId(), *(s8*)(player + 0x3E));
            partyDokanEnd(marioGetExtraPartyId(), *(s8*)(player + 0x3E));
        } else if (dokan == 1) {
            *(u32*)(player + 0x44) = 101;
        } else if (dokan == 0) {
            marioUpdateCamPos();
            *(u32*)(player + 0x44) = 101;
        } else {
            *(u32*)(player + 0x44) = 102;
        }
        break;
    case 101:
        *(f32*)(player + 0x1AC) += *(f32*)(player + 0x2B8);
        if (*(f32*)(player + 0x1AC) >= float_540_8042307c) {
            marioChgPaper((char*)(data + 0xD4));
            *(s32*)(player + 0x48) = 20;
            *(u32*)(player + 0x44) = 102;
            *(f32*)(player + 0xBC) = float_0_80423040;
            partyDokanEnd(marioGetPartyId(), *(s8*)(player + 0x3E));
            partyDokanEnd(marioGetExtraPartyId(), *(s8*)(player + 0x3E));
        }
        break;
    case 102:
        if (--*(s32*)(player + 0x48) <= 0) *(u32*)(player + 0x44) = 103;
        break;
    case 103:
        *(RawVec3*)(player + 0xE0) = *(RawVec3*)(player + 0x8C);
        *(RawVec3*)(player + 0xC8) = *(RawVec3*)(data + 0x9C);
        marioOfsRotReset();
        *(u32*)(player + 4) &= ~0x8;
        *(u32*)(player + 4) &= ~0x100;
        *(u32*)(player + 4) &= ~0x1000;
        marioPaperOff();
        marioChgMot(0);
        *(f32*)(player + 0x1AC) = revise360(*(f32*)(player + 0x1AC));
        *(f32*)(player + 0x1B0) = *(f32*)(player + 0x1AC);
        *(f32*)(player + 0x1A4) = toMovedirSimple(*(f32*)(player + 0x1AC));
        *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1A4);
        if (*(u8*)(player + 0x3E) == 4) {
            camFollowYOn();
            *(f32*)(player + 0xF0) = *(f32*)(player + 0x90);
        }
        *(u32*)player &= ~0x100;
        break;
    }
    switch (*(u32*)(player + 0x44)) {
    case 110:
        if ((*(u32*)player & 0x02000000) == 0) *(f32*)(player + 0xF0) = *(f32*)(player + 0x90);
        marioUpdateCamPos();
        camFollowYOff();
        *(u32*)(player + 0x44) = 111;
        /* fall through */
    case 111:
        *(f32*)(player + 0x1AC) += *(f32*)(player + 0x2B8);
        if (*(f32*)(player + 0x1AC) >= float_540_8042307c) {
            if ((*(u32*)player & 0x02000000) == 0) camFollowYOn();
            marioChgPaper((char*)(data + 0xD4));
            *(s32*)(player + 0x48) = 20;
            *(u32*)(player + 0x44) = 112;
            *(f32*)(player + 0xBC) = float_0_80423040;
            *(f32*)(player + 0x1B0) = revise360(*(f32*)(player + 0x1AC));
            *(f32*)(player + 0x1AC) = *(f32*)(player + 0x1B0);
            *(f32*)(player + 0x1A4) = toMovedirSimple(*(f32*)(player + 0x1AC));
            *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1A4);
            partyDokanEnd(marioGetPartyId(), *(s8*)(player + 0x3E));
            partyDokanEnd(marioGetExtraPartyId(), *(s8*)(player + 0x3E));
        }
        break;
    case 112:
        if (--*(s32*)(player + 0x48) <= 0) *(u32*)(player + 0x44) = 113;
        break;
    case 113:
        *(RawVec3*)(player + 0xE0) = *(RawVec3*)(player + 0x8C);
        *(RawVec3*)(player + 0xC8) = *(RawVec3*)(data + 0xA8);
        marioOfsRotReset();
        *(u32*)(player + 4) &= ~0x8;
        *(u32*)(player + 4) &= ~0x100;
        *(u32*)(player + 4) &= ~0x1000;
        marioPaperOff();
        marioChgMot(0);
        *(f32*)(player + 0x1AC) = revise360(*(f32*)(player + 0x1AC));
        *(f32*)(player + 0x1B0) = *(f32*)(player + 0x1AC);
        *(f32*)(player + 0x1A4) = toMovedirSimple(*(f32*)(player + 0x1AC));
        *(f32*)(player + 0x1A0) = *(f32*)(player + 0x1A4);
        if ((*(u32*)player & 0x02000000) == 0) camFollowYOn();
        *(u32*)player &= ~0x100;
        break;
    }
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

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

/* Distinct target-backed trailing initialized padding. */
const unsigned char gap_09_8042308C_sdata2[4] = {0, 0, 0, 0};
