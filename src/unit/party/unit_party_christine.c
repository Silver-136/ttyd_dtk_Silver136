#include "unit/party/unit_party_christine.h"


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 krb_get_dir(void* evt) {
    extern f32 evtGetFloat(void* evt, s32 arg);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 float_0_804243cc;

    s32* args = *(s32**)((s32)evt + 0x18);
    f32 x1 = evtGetFloat(evt, args[0]);
    f32 z1 = evtGetFloat(evt, args[1]);
    f32 x2 = evtGetFloat(evt, args[2]);
    f32 z2 = evtGetFloat(evt, args[3]);
    s32 def = evtGetValue(evt, args[4]);
    f32 dx = x2 - x1;
    f32 dz = z2 - z1;
    f32 zero;

    zero = float_0_804243cc;
    if (zero == dx && zero == dz) {
        evtSetValue(evt, args[4], def);
        return 2;
    }
    zero = float_0_804243cc;
    evtSetValue(evt, args[4], (s32)angleABf(dx, dz, zero, zero));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* _battleWorkPointer;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_80393A58[];
    s32 techLv;
    void* party;
    u8* data;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_80393A58;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
    *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x450;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
    *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
        *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
    *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
        msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
    *count = *count + 1;

    if (techLv >= 0) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x510;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 1) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x5D0;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 2) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x690;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 btlevtcmd_get_monosiri_msg_no(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void* battleGetUnitMonosiriPtr(void* unit);
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* mono;
    s32 id = evtGetValue(evt, args[0]);
    evtGetValue(evt, args[1]);
    mono = battleGetUnitMonosiriPtr(*(void**)((s32)BattleGetUnitPtr(_battleWorkPointer, id) + 8));
    evtSetValue(evt, args[2], *(s32*)((s32)mono + 4));
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _monosiri_flag_on(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void battleSetUnitMonosiriFlag(void* unit);

    s32 id = evtGetValue(evt, **(s32**)((s32)evt + 0x18));
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    battleSetUnitMonosiriFlag(*(void**)((s32)unit + 8));
    return 2;
}
s32 _dictionary(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void* monosiriInit(void* unit, f32 x, f32 y);
    extern void monosiriMain(void* work);
    extern void monosiriClose(void* work);
    extern s32 monosiriCloseWait(void* work);
    extern void monosiriExit(void* work);
    extern f32 float_neg50_804243c4;
    extern f32 float_50_804243c8;

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* unit;
    s32 state;
    void* work;
    s32 currentId = BattleTransID(evt, -2);
    s32 targetId = BattleTransID(evt, evtGetValue(evt, args[0]));
    evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, currentId);
    state = *(s32*)((s32)unit + 0x218);
    work = *(void**)((s32)unit + 0x21C);

    switch (state) {
        case 0:
            work = monosiriInit(*(void**)((s32)BattleGetUnitPtr(battleWork, targetId) + 8),
                                float_neg50_804243c4, float_50_804243c8);
            state = 2;
            break;
        case 1:
            monosiriMain(work);
            state = 3;
            break;
        case 2:
            monosiriMain(work);
            break;
        case 3:
            monosiriClose(work);
            monosiriMain(work);
            state = 5;
            break;
        case 4:
            if (monosiriCloseWait(work)) {
                monosiriExit(work);
                state = 6;
            } else {
                monosiriMain(work);
            }
            break;
    }
    *(s32*)((s32)unit + 0x218) = state;
    *(void**)((s32)unit + 0x21C) = work;
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _set_hustle(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    extern void BtlUnit_OnStatusFlag(void* unit, u32 flag);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id = evtGetValue(evt, args[0]);
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    if (unit != 0) {
        if (*(s8*)((s32)unit + 0x22) <= 0) {
            *(u8*)((s32)unit + 0x22) = *(u8*)((s32)unit + 0x22) + 1;
            *(u8*)((s32)unit + 0x20) = 0;
            *(u8*)((s32)unit + 0x311) = 0xFF;
            if (*(s8*)((s32)unit + 0x23) <= 0) {
                *(u8*)((s32)unit + 0x23) = 1;
                if (BtlUnit_CheckStatus(unit, 0x14)) {
                    *(u32*)((s32)unit + 0x27C) |= 0x2000;
                }
            }
        } else {
            BtlUnit_OnStatusFlag(unit, 0x02000000);
        }
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
