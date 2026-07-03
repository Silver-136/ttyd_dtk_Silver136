#include "action/star/sac_genki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void statusWinOpen(void);
void* BattleGetPartyPtr(void* battleWork);
void main_star0(s32 index);
void main_star1(void);

void disp_2D(void) {
    ;
}

void disp_3D_alpha(void) {
    ;
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(recover_cmd_disable) {
    void* work = _battleWorkPointer;
    void* party;
    *(s32*)((s32)work + 0x19050) = 0;
    *(s32*)((s32)work + 0x19054) = 0;
    *(s32*)((s32)work + 0x19058) = 0;
    party = BattleGetPartyPtr(work);
    if (party != 0) {
        *(u16*)((s32)party + 0x13C) = 0;
        *(u16*)((s32)party + 0x13E) = 0;
    }
    return 2;
}

USER_FUNC(status_on) {
    statusWinOpen();
    return 2;
}

USER_FUNC(star_stone_appear) {
    void* work = get_ptr();
    *(s32*)((s32)work + 0x510) = 1;
    *(s32*)((s32)work + 0x518) = 1;
    return 2;
}

USER_FUNC(star_appear) {
    void* work = get_ptr();
    s32 offset = 0;
    s32 zero = offset;
    s32 one = 1;
    s32 i;

    for (i = 0; i < 7; i++) {
        void* entry = (void*)((s32)work + offset + 0x510);

        offset += 0x64;
        *(s32*)entry = one;
        *(s32*)((s32)entry + 8) = zero;
    }
    return 2;
}

USER_FUNC(start_game) {
    void* wp;

    wp = get_ptr();
    *(s32*)((s32)wp + 0x4) = 1;

    return 2;
}

USER_FUNC(wait_game_end) {
    s32 value = *(s32*)((s32)get_ptr() + 4);
    return value == 3 ? 2 : 0;
}

s32 object_get_num(void) {
    void* work;
    s32 i;
    s32 offset;
    s32 count;
    s32 entryOffset;

    work = get_ptr();
    count = 0;
    offset = 0;

    for (i = 0; i < 10; i++) {
        entryOffset = offset;
        entryOffset += 0x358;

        if (*(s32*)((char*)work + entryOffset) == 2) {
            count++;
        }

        offset += 0x2C;
    }

    return count;
}

void object_entry(void) {
    void* work;
    s32 i;
    s32 offset;
    s32 entryOffset;

    work = get_ptr();
    offset = 0;

    for (i = 0; i < 10; i++) {
        entryOffset = offset + 0x358;

        if (*(s32*)((s32)work + entryOffset) == 0) {
            *(s32*)((s32)work + entryOffset) = 1;
            break;
        }

        offset += 0x2C;
    }
}

USER_FUNC(get_score) {
    void* work;
    s32* args;
    s32 value;

    work = get_ptr();
    args = event->args;

    value = *(s32*)((s32)work + 0x10);
    evtSetValue(event, args[0], value);

    value = *(s32*)((s32)work + 0x14);
    evtSetValue(event, args[1], value);

    value = *(s32*)((s32)work + 0x18);
    evtSetValue(event, args[2], value);

    return 2;
}

void main_star(void) {
    void* work = get_ptr();
    s32 i;

    if (*(s32*)((s32)work + 0xC) == 0) {
        for (i = 0; i < 7; i++) {
            main_star0(i);
        }
    } else {
        main_star1();
    }
}

void weapon_entry(void) {
    extern void* BattleGetMarioPtr(void* battleWork);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern s32 irand(s32 max);
    extern void BtlUnit_SetAnim(void* parts, const char* anim, s32 force);
    extern const char str_M_C_2C_80300dfc[];

    void* work;
    void* mario;
    void* parts;
    s32 ids[10];
    s32 count;
    s32 stackOffset;
    s32 id;
    s32 offset;
    s32 i;
    s32 chosen;
    void* entry;

    work = get_ptr();
    mario = BattleGetMarioPtr(_battleWorkPointer);
    parts = BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario));
    count = 0;
    stackOffset = 0;
    id = 0;
    offset = 0;
    for (i = *(s32*)((s32)work + 0x58); i > 0; i--) {
        if (*(s32*)((s32)work + offset + 0x10C) == 0) {
            *(volatile s32*)((s32)ids + stackOffset) = id;
            count++;
            stackOffset += 4;
        }
        id++;
        offset += 0x54;
    }
    if (count >= 1) {
        chosen = *(s32*)((s32)ids + (irand(count) << 2));
        entry = (void*)((s32)work + chosen * 0x54 + 0x10C);
        *(s32*)entry = 1;
        *(s32*)((s32)entry + 4) = 7;
        BtlUnit_SetAnim(parts, str_M_C_2C_80300dfc, 1);
    }
}

USER_FUNC(end_genki) {
    extern void effDelete(void* effect);
    extern void BattleFree(void* ptr);

    void* work;
    void* entry;
    s32 i;
    s32 offset;
    s32 j;
    s32 objectOffset;
    s32 k;
    s32 childOffset;

    work = get_ptr();
    i = 0;
    offset = 0;
    while (i < 7) {
        entry = (void*)((s32)work + offset + 0x510);
        if (*(void**)((s32)entry + 0x60) != NULL) {
            effDelete(*(void**)((s32)*(void**)((s32)*(void**)((s32)entry + 0x60) + 0xC) + 0x34));
            effDelete(*(void**)((s32)entry + 0x60));
        }
        i++;
        offset += 0x64;
    }

    j = 0;
    objectOffset = 0;
    while (j < 7) {
        entry = (void*)((s32)work + objectOffset + 0x10C);
        if (*(void**)((s32)entry + 0x44) != NULL) {
            effDelete(*(void**)((s32)*(void**)((s32)*(void**)((s32)entry + 0x44) + 0xC) + 0x34));
            effDelete(*(void**)((s32)entry + 0x44));
            k = 0;
            childOffset = 0;
            while (k < 3) {
                effDelete(*(void**)((s32)*(void**)((s32)*(void**)((s32)entry + childOffset + 0x48) + 0xC) + 0x34));
                effDelete(*(void**)((s32)entry + childOffset + 0x48));
                k++;
                childOffset += 4;
            }
        }
        j++;
        objectOffset += 0x54;
    }
    effDelete(*(void**)((s32)work + 0x34));
    effDelete(*(void**)((s32)work + 0x38));
    effDelete(*(void**)((s32)work + 0x3C));
    BattleFree(*(void**)((s32)work + 0x5C));
    return 2;
}



/* CHATGPT STUB FILL: main/action/star/sac_genki 20260624_184929 */

/* stub-fill: disp_3D | missing_definition | ghidra_signature */
u8 disp_3D(void) {
    return 0;
}

/* stub-fill: main_star1 | prototype_only | source_prototype */
void main_star1(void) {
    return;
}

/* stub-fill: main_star0 | prototype_only | source_prototype */
void main_star0(s32 index) {
    return;
}

/* stub-fill: main_object | missing_definition | ghidra_signature */
u8 main_object(u32 param_1) {
    return 0;
}

/* stub-fill: main_weapon | missing_definition | ghidra_signature */
u8 main_weapon(void) {
    return 0;
}

/* stub-fill: main_target | missing_definition | ghidra_signature */
void main_target(void) {
    extern void* get_ptr(void);
    extern f32 float_30_80428318;
    extern f32 float_1p25_80428304;
    extern f32 float_60_804282e8;
    extern f32 float_360_80428380;
    extern f32 float_neg1p25_804283a4;
    extern f32 float_0_8042831c;

    void* work = get_ptr();

    switch (*(s32*)((s32)work + 0x64)) {
        case 1:
            *(s32*)((s32)work + 0x68) = 0;
            *(f32*)((s32)work + 0x6C) = float_30_80428318;
            *(f32*)((s32)work + 0x70) = float_1p25_80428304;
            *(s32*)((s32)work + 0x64) = 2;
            /* fallthrough */
        case 2:
            *(s32*)((s32)work + 0x68) += 1;
            if (*(s32*)((s32)work + 0x68) >= 0x168) {
                *(s32*)((s32)work + 0x68) = (s32)((f32)*(s32*)((s32)work + 0x68) - float_360_80428380);
            }
            *(f32*)((s32)work + 0x6C) += *(f32*)((s32)work + 0x70);
            if (*(f32*)((s32)work + 0x6C) >= float_60_804282e8) {
                *(f32*)((s32)work + 0x6C) = float_60_804282e8;
                *(f32*)((s32)work + 0x70) = float_neg1p25_804283a4;
                *(s32*)((s32)work + 0x64) = 3;
            }
            break;
        case 3:
            *(s32*)((s32)work + 0x68) += 1;
            if (*(s32*)((s32)work + 0x68) >= 0x168) {
                *(s32*)((s32)work + 0x68) = (s32)((f32)*(s32*)((s32)work + 0x68) - float_360_80428380);
            }
            *(f32*)((s32)work + 0x6C) += *(f32*)((s32)work + 0x70);
            if (*(f32*)((s32)work + 0x6C) <= float_0_8042831c) {
                *(f32*)((s32)work + 0x6C) = float_0_8042831c;
                *(f32*)((s32)work + 0x70) = float_1p25_80428304;
                *(s32*)((s32)work + 0x64) = 2;
            }
            break;
    }
}

/* stub-fill: main_mario | missing_definition | ghidra_signature */
void main_mario(void) {
    extern void* get_ptr(void);
    extern s32 keyGetStickX(s32 pad);
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void* battleWork);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern void BtlUnit_SetAnim(void* parts, const char* anim, s32 force);
    extern s32 object_get_num(void);
    extern void weapon_entry(void);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effStampN64Entry(s32 type, f32 x, f32 y, f32 z);
    extern char str_M_S_1_804283a8[];
    extern char str_M_C_2A_80300e2c[];
    extern char str_M_C_2B_80300e34[];
    extern char str_M_D_1_804283b0[];
    extern char str_M_X_1_804283b8[];
    extern char str_M_I_Y_804282d4[];
    extern f32 float_20_80428310;
    extern f32 float_10_804282ec;

    void* work = get_ptr();
    s32 stick = (s8)keyGetStickX(0);
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    void* parts = BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario));
    f32 x;
    f32 y;
    f32 z;
    void* eff;
    void* effWork;

    if (*(s32*)((s32)work + 0x44) >= 1 && *(s32*)((s32)work + 0x44) <= 4 &&
        *(s32*)((s32)work + 0x40) <= 0 && object_get_num() == 0) {
        *(s32*)((s32)work + 0x44) = 5;
        *(s32*)((s32)work + 0x64) = 4;
        BtlUnit_SetAnim(parts, str_M_S_1_804283a8, 1);
    }

    switch (*(s32*)((s32)work + 0x44)) {
        case 1:
            if (stick < -0x28) {
                *(s32*)((s32)work + 0x44) = 2;
                *(s32*)((s32)work + 0x48) = 0;
                *(s32*)((s32)work + 0x54) = 0xA;
                BtlUnit_SetAnim(parts, str_M_C_2A_80300e2c, 0);
            }
            break;
        case 2:
            if (stick > -0x14) {
                *(s32*)((s32)work + 0x44) = 1;
                weapon_entry();
            } else {
                *(s32*)((s32)work + 0x54) -= 1;
                if (*(s32*)((s32)work + 0x54) == 2) {
                    BtlUnit_SetAnim(parts, str_M_C_2B_80300e34, 1);
                }
                if (*(s32*)((s32)work + 0x54) <= 0 && *(s32*)((s32)work + 0xC) == 1) {
                    *(s32*)((s32)work + 0x54) = 0xA;
                    weapon_entry();
                }
            }
            break;
        case 3:
            *(s32*)((s32)work + 0x44) = 4;
            *(s32*)((s32)work + 0x48) = 0;
            BtlUnit_SetAnim(parts, str_M_D_1_804283b0, 0);
            BtlUnit_GetPos(mario, &x, &y, &z);
            eff = effStampN64Entry(2, x, y + float_20_80428310, z + float_10_804282ec);
            effWork = *(void**)((s32)eff + 0xC);
            *(u8*)((s32)effWork + 0x38) = 0;
            *(u8*)((s32)effWork + 0x39) = 0xC8;
            *(u8*)((s32)effWork + 0x3A) = 0;
            *(u8*)((s32)effWork + 0x3B) = 0;
            *(u8*)((s32)effWork + 0x3C) = 0xA;
            *(u8*)((s32)effWork + 0x3D) = 0;
            break;
        case 4:
            *(s32*)((s32)work + 0x48) += 1;
            if (*(s32*)((s32)work + 0x48) == 0x1E) {
                BtlUnit_SetAnim(parts, str_M_X_1_804283b8, 0);
            }
            if (*(s32*)((s32)work + 0x48) == 0xB4) {
                BtlUnit_SetAnim(parts, str_M_I_Y_804282d4, 0);
                *(s32*)((s32)work + 0x44) = 1;
                *(s32*)((s32)work + 0x48) = 0;
            }
            break;
    }
}

/* stub-fill: main_base | missing_definition | ghidra_signature */
u8 main_base(void) {
    return 0;
}

/* stub-fill: main_genki | missing_definition | ghidra_signature */
s32 main_genki(EventEntry* event, s32 isFirstCall) {
    extern void* get_ptr(void);
    extern void* memset(void* dest, s32 value, u32 size);
    extern s32 evtGetValue(EventEntry* event, s32 target);
    extern void* BattleAlloc(u32 size);
    extern void* effRecoveryEntry(s32 type, s32 arg, f32 x, f32 y, f32 z);
    extern void* effStarStoneEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void main_base(void);
    extern void main_mario(void);
    extern void main_target(void);
    extern void main_weapon(s32 index);
    extern void main_object(s32 index);
    extern void main_star(void);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void disp_2D(void);
    extern void disp_3D(void);
    extern void disp_3D_alpha(void);
    extern f32 float_0_8042831c;
    extern f32 float_neg1000_80428320;
    extern f32 float_1_8042830c;
    extern f32 float_901_804283c0;

    void* work = get_ptr();
    s32* args;
    s32 i;
    s32 j;

    if (isFirstCall != 0) {
        args = event->args;
        memset(work, 0, 0x7CC);
        *(s32*)((s32)work + 0x50) = evtGetValue(event, args[0]);
        *(s32*)((s32)work + 0xC) = evtGetValue(event, args[1]);
        *(s32*)((s32)work + 0x40) = 0x384;
        *(s32*)((s32)work + 0x10) = 0;
        *(s32*)((s32)work + 0x14) = 0;
        *(s32*)((s32)work + 0x18) = 0;
        *(f32*)((s32)work + 0x1C) = float_0_8042831c;
        *(f32*)((s32)work + 0x20) = float_0_8042831c;
        *(f32*)((s32)work + 0x24) = float_0_8042831c;
        *(s32*)((s32)work + 0x58) = (*(s32*)((s32)work + 0xC) == 0) ? 3 : 7;
        *(void**)((s32)work + 0x5C) = BattleAlloc((*(s32*)((s32)work + 0xC) == 0) ? 0x64 : 0xC8);
        *(void**)((s32)work + 0x34) = effRecoveryEntry(2, 0, float_0_8042831c, float_neg1000_80428320, float_0_8042831c);
        *(void**)((s32)work + 0x38) = effRecoveryEntry(2, 0, float_0_8042831c, float_neg1000_80428320, float_0_8042831c);
        *(void**)((s32)work + 0x3C) = effRecoveryEntry(3, 0, float_0_8042831c, float_neg1000_80428320, float_0_8042831c);
        for (i = 0; i < 7; i++) {
            void* entry = (void*)((s32)work + (i * 0x54));
            *(void**)((s32)entry + 0x150) = effStarStoneEntry(i, float_0_8042831c, float_neg1000_80428320, float_0_8042831c, float_1_8042830c);
            for (j = 0; j < 3; j++) {
                *(void**)((s32)entry + 0x154 + (j * 4)) = effStarStoneEntry(i + 7, float_0_8042831c, float_neg1000_80428320, float_0_8042831c, float_1_8042830c);
            }
        }
    }

    main_base();
    main_mario();
    main_target();
    for (i = 0; i < *(s32*)((s32)work + 0x58); i++) {
        main_weapon(i);
    }
    for (i = 0; i < 10; i++) {
        main_object(i);
    }
    main_star();
    dispEntry(8, 1, disp_2D, float_901_804283c0, 0);
    dispEntry(4, 1, disp_3D, float_0_8042831c, 0);
    dispEntry(4, 2, disp_3D_alpha, float_0_8042831c, 0);
    return 0;
}

