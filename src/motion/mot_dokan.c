#include "motion/mot_dokan.h"


u8 mot_dokan(void) {
    return 0;
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

