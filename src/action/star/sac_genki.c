#include "action/star/sac_genki.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
s32 evtSetValue(EventEntry* event, s32 target, s32 value);
void statusWinOpen(void);
void* BattleGetPartyPtr(void* battleWork);
void* get_ptr(void);
void main_star0(s32 index);
void main_star1(void);

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

/* stub-fill: main_base | missing_definition | ghidra_signature */
void main_base(void) {
    extern void* get_ptr(void);
    extern s32 irand(s32);
    extern void object_entry(void);
    u8* work = get_ptr();
    s32* table = *(s32**)(work + 0x5C);
    s32 i;

    if ((f32)*(s32*)(work + 0x10) < *(f32*)(work + 0x1C)) {
        *(f32*)(work + 0x1C) -= 1.0f;
        *(s32*)(work + 0x28) = 60;
    }
    if (*(f32*)(work + 0x1C) < (f32)*(s32*)(work + 0x10)) {
        *(f32*)(work + 0x1C) += 1.0f;
        *(s32*)(work + 0x28) = 60;
    }
    if ((f32)*(s32*)(work + 0x14) < *(f32*)(work + 0x20)) {
        *(f32*)(work + 0x20) -= 1.0f;
        *(s32*)(work + 0x2C) = 60;
    }
    if (*(f32*)(work + 0x20) < (f32)*(s32*)(work + 0x14)) {
        *(f32*)(work + 0x20) += 1.0f;
        *(s32*)(work + 0x2C) = 60;
    }
    if ((f32)*(s32*)(work + 0x18) < *(f32*)(work + 0x24)) {
        *(f32*)(work + 0x24) -= 1.0f;
        *(s32*)(work + 0x30) = 60;
    }
    if (*(f32*)(work + 0x24) < (f32)*(s32*)(work + 0x18)) {
        *(f32*)(work + 0x24) += 1.0f;
        *(s32*)(work + 0x30) = 60;
    }
    if (*(s32*)(work + 0x28) > 0) (*(s32*)(work + 0x28))--;
    if (*(s32*)(work + 0x2C) > 0) (*(s32*)(work + 0x2C))--;
    if (*(s32*)(work + 0x30) > 0) (*(s32*)(work + 0x30))--;

    if (*(s32*)(work + 4) == 1) {
        s32 pos = 0;
        *(s32*)(work + 4) = 2;
        *(s32*)(work + 0x44) = 1;
        *(s32*)(work + 0x64) = 1;
        *(s32*)(work + 0x60) = 0;
        if (*(s32*)(work + 0xC) == 0) {
            for (i = 0; i < 7; i++) table[pos++] = 0;
            for (i = 0; i < 7; i++) table[pos++] = 2;
            for (i = 0; i < 8; i++) table[pos++] = 4;
            for (i = 0; i < 3; i++) table[pos++] = 6;
            for (i = 0; i < 200; i++) {
                s32 a = irand(25), b = irand(25), temp = table[a];
                table[a] = table[b]; table[b] = temp;
            }
        } else {
            for (i = 0; i < 12; i++) table[pos++] = 0;
            for (i = 0; i < 4; i++) table[pos++] = 1;
            for (i = 0; i < 12; i++) table[pos++] = 2;
            for (i = 0; i < 4; i++) table[pos++] = 3;
            for (i = 0; i < 12; i++) table[pos++] = 4;
            for (i = 0; i < 4; i++) table[pos++] = 5;
            for (i = 0; i < 2; i++) table[pos++] = 6;
            for (i = 0; i < 200; i++) {
                s32 a = irand(50), b = irand(50), temp = table[a];
                table[a] = table[b]; table[b] = temp;
            }
        }
    }
    if (*(s32*)(work + 4) != 2) return;

    *(s32*)(work + 0x4C) -= 1;
    if (*(s32*)(work + 0x4C) < 1) {
        *(s32*)(work + 0x4C) = *(s32*)(work + 0xC) == 0 ? 37 : 18;
        if (*(s32*)(work + 0x40) > 0) object_entry();
    }
    if (*(s32*)(work + 0x40) < 1) {
        s32 stars = *(s32*)(work + 0x58);
        s32 doneStars = 0;
        s32 doneObjects = 0;
        for (i = 0; i < stars; i++) {
            if (*(s32*)(work + 0x10C + i * 0x54) != 0) break;
            doneStars++;
        }
        for (i = 0; i < 10; i++) {
            if (*(s32*)(work + 0x358 + i * 0x2C) != 0) break;
            doneObjects++;
        }
        if (doneStars == stars && doneObjects == 10) *(s32*)(work + 4) = 3;
    } else {
        *(s32*)(work + 0x40) -= 1;
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

/* stub-fill: main_weapon | missing_definition | ghidra_signature */
void main_weapon(s32 index) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* get_ptr(void);
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void*);
    extern s32 BattleAudience_GetAudienceNum(void);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern f64 cos(f64);
    extern f64 sin(f64);
    extern void BtlUnit_snd_se(void*, char*, s32, s16);
    extern void effStardustN64Entry(f32, f32, f32, f32, s32);
    extern void effHitEntry(void);
    extern u32 psndSFXOn_3D(char*, Vec*);
    extern void BattleAudienceSoundCallKind(s32);
    extern void BattleAudienceSoundHandBeat(void);
    extern void BattleAudienceSoundBooingKind(s32);
    extern char str_SFX_BTL_SAC_HEART_SH_80300e10[];
    u8* work = (u8*)get_ptr();
    s32* weapon = (s32*)(work + 0x10C + index * 0x54);
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 audience = BattleAudience_GetAudienceNum();
    f32 angle = 3.1416f * 2.0f * *(f32*)(work + 0x6C) / 360.0f;
    s32 i;

    for (i = 2; i > 0; i--) {
        weapon[5 + i * 3] = weapon[5 + (i - 1) * 3];
        weapon[6 + i * 3] = weapon[6 + (i - 1) * 3];
        weapon[7 + i * 3] = weapon[7 + (i - 1) * 3];
    }
    weapon[5] = weapon[2]; weapon[6] = weapon[3]; weapon[7] = weapon[4];
    if (weapon[0] == 0) {
        *(f32*)&weapon[2] = *(f32*)&weapon[3] = *(f32*)&weapon[4] = 0.0f;
    } else if (weapon[0] == 1) {
        if (--weapon[1] < 0) {
            s32 off;
            weapon[0] = 2;
            BtlUnit_GetPos(mario, (f32*)&weapon[2], (f32*)&weapon[3], (f32*)&weapon[4]);
            *(f32*)&weapon[2] += 20.0f;
            *(f32*)&weapon[3] += 28.0f;
            *(f32*)&weapon[14] = 10.0f * (f32)cos(angle);
            *(f32*)&weapon[15] = 10.0f * (f32)sin(angle);
            *(f32*)&weapon[16] = 0.0f;
            for (off = 0; off < 7; off++) {
                s32* trail = (s32*)(work + 0x80 + off * 0x14);
                if (trail[0] < 1) {
                    trail[0] = 0x1E;
                    trail[1] = *(s32*)(work + 0x74);
                    trail[2] = *(s32*)(work + 0x78);
                    trail[3] = *(s32*)(work + 0x7C);
                    *(f32*)&trail[4] = 2.0f * (f32)*(s32*)(work + 0x68);
                    break;
                }
            }
            BtlUnit_snd_se(mario, str_SFX_BTL_SAC_HEART_SH_80300e10, -250000000, 0);
        }
    }
    if (weapon[0] == 2) {
        f32 marioX, marioY, marioZ;
        s32 nearest = -1;
        f32 nearestDist = 10000000.0f;
        *(f32*)&weapon[2] += *(f32*)&weapon[14];
        *(f32*)&weapon[3] += *(f32*)&weapon[15];
        *(f32*)&weapon[15] -= 0.15f;
        BtlUnit_GetPos(mario, &marioX, &marioY, &marioZ);
        if (*(f32*)&weapon[2] >= 250.0f || *(f32*)&weapon[3] >= 180.0f || *(f32*)&weapon[3] <= marioY) {
            weapon[0] = 3;
        } else {
            for (i = 0; i < 10; i++) {
                s32* object = (s32*)(work + 0x358 + i * 0x2C);
                if (object[0] == 2) {
                    f32 dx = *(f32*)&weapon[2] - *(f32*)&object[3];
                    f32 dy = *(f32*)&weapon[3] - *(f32*)&object[4];
                    f32 dist = dx * dx + dy * dy;
                    if (dist < nearestDist) { nearestDist = dist; nearest = i; }
                }
            }
            if (nearest != -1) {
                s32* object = (s32*)(work + 0x358 + nearest * 0x2C);
                f32 limit;
                switch (object[2]) {
                    case 0: case 2: case 4: limit = 576.0f; break;
                    case 6: limit = 344.0f; break;
                    default: limit = 1296.0f; break;
                }
                if (nearestDist <= limit && *(f32*)&object[4] <= 200.0f) {
                    weapon[0] = 3;
                    object[0] = 3;
                    effStardustN64Entry(*(f32*)&object[3], *(f32*)&object[4], *(f32*)&object[5], 10.0f, 5);
                    effHitEntry();
                    if (object[2] == 0 || object[2] == 1) {
                        *(s32*)(work + 0x10) += object[2] == 0 ? 1 : 3;
                        if (*(s32*)(work + 0x10) > 98) *(s32*)(work + 0x10) = 99;
                    } else if (object[2] == 2 || object[2] == 3) {
                        *(s32*)(work + 0x14) += object[2] == 2 ? 1 : 3;
                        if (*(s32*)(work + 0x14) > 98) *(s32*)(work + 0x14) = 99;
                    } else if (object[2] == 4 || object[2] == 5) {
                        *(s32*)(work + 0x18) += object[2] == 4 ? 1 : 3;
                        if (*(s32*)(work + 0x18) > 98) *(s32*)(work + 0x18) = 99;
                    } else if (object[2] == 6) {
                        *(s32*)(work + 0x44) = 3;
                    }
                    psndSFXOn_3D((char*)(0xA64 + object[2]), (Vec*)&weapon[2]);
                    if (object[2] == 6) {
                        if (audience < 100) BattleAudienceSoundBooingKind(1);
                        else BattleAudienceSoundBooingKind(audience < 150 ? 2 : 3);
                    } else {
                        BattleAudienceSoundCallKind(audience < 100 ? 1 : 2);
                        if ((audience >= 50 && audience < 100) || audience >= 150)
                            BattleAudienceSoundHandBeat();
                    }
                }
            }
        }
    } else if (weapon[0] == 3) {
        weapon[0] = 0;
    }
    if (weapon[17] != 0) {
        u8* ew = *(u8**)((u8*)weapon[17] + 0xC);
        *(f32*)(ew + 8) = *(f32*)&weapon[2];
        *(f32*)(ew + 0xC) = *(f32*)&weapon[3];
        *(f32*)(ew + 0x10) = *(f32*)&weapon[4];
        *(f32*)(ew + 0x14) = 1.125f;
        *(u8*)(ew + 0x5F) = 0xFF;
    }
    for (i = 0; i < 3; i++) {
        u8* ew = *(u8**)((u8*)weapon[18 + i] + 0xC);
        *(f32*)(ew + 8) = *(f32*)&weapon[5 + i * 3];
        *(f32*)(ew + 0xC) = *(f32*)&weapon[6 + i * 3];
        *(f32*)(ew + 0x10) = *(f32*)&weapon[7 + i * 3];
        *(f32*)(ew + 0x14) = 1.125f;
        *(u8*)(ew + 0x5F) = (u8)((i + 1) * -50 - 1);
    }
}

/* stub-fill: main_object | missing_definition | ghidra_signature */
void main_object(u32 index) {
    extern void* get_ptr(void);
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void*);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern s32 irand(s32);
    extern void* effStardustEntry(f32, f32, f32, f32, f32, s32, s32, s32);

    u8* work = get_ptr();
    s32* object = (s32*)(work + 0x358 + index * 0x2C);
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 choices[3] = {0, 2, 4};
    f32 marioX, marioY, marioZ;
    s32 state = object[0];

    if (state == 1) {
        u8* star;
        s32 threshold = *(s32*)(work + 0xC) == 0 ? 25 : 50;
        object[0] = 2;
        object[1] = 0;
        if (*(s32*)(work + 0x60) < threshold) {
            object[2] = *(s32*)(*(s32*)(work + 0x5C) + *(s32*)(work + 0x60) * 4);
        } else {
            s32 divisor = *(s32*)(work + 0xC) == 0 ? 7 : 14;
            if (irand(divisor) == 0) {
                object[2] = 6;
            } else {
                object[2] = choices[irand(*(s32*)(work + 0x50) == -1 ? 2 : 3)];
                if (irand(5) == 0 && *(s32*)(work + 0xC) == 1) {
                    if (object[2] == 0) object[2] = 1;
                    else if (object[2] == 2) object[2] = 3;
                    else if (object[2] == 4) object[2] = 5;
                }
            }
        }
        if (*(s32*)(work + 0x50) == -1 && object[2] == 2) object[2] = 0;
        if (*(s32*)(work + 0x50) == -1 && object[2] == 3) object[2] = 1;
        *(s32*)(work + 0x60) += 1;
        star = *(s32*)(work + 0xC) == 0
            ? work + 0x510 + irand(7) * 0x64
            : work + 0x510;
        BtlUnit_GetPos(mario, &marioX, &marioY, &marioZ);
        *(f32*)&object[3] = *(f32*)(star + 0xC);
        *(f32*)&object[4] = *(f32*)(star + 0x10);
        *(f32*)&object[5] = marioZ + (f32)index;
        state = 2;
    }
    if (state == 2) {
        if (*(s32*)(work + 0xC) == 0) {
            *(f32*)&object[4] -= 1.32f;
        } else {
            *(f32*)&object[4] -= 1.98f;
        }
        BtlUnit_GetPos(mario, &marioX, &marioY, &marioZ);
        if (*(f32*)&object[4] <= marioY + 15.0f) {
            object[0] = 3;
            effStardustEntry(*(f32*)&object[3], marioY + 15.0f, *(f32*)&object[5],
                             30.0f, 30.0f, 5, 10, 60);
        }
        object[1]++;
        if (object[1] > 120) object[1] = 0;
    } else if (state == 3) {
        object[0] = 0;
    }
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

/* stub-fill: main_star0 | prototype_only | source_prototype */
void main_star0(s32 index) {
    extern void* get_ptr(void);
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void*);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern void* effStarStoneEntry(f32, f32, f32, f32, s32);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern s32 rand(void);
    extern s32 irand(s32);
    u8* work = (u8*)get_ptr();
    s32* star = (s32*)(work + 0x510 + index * 100);
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    f32 marioX, marioY, marioZ;

    if (star[0] == 1) {
        star[0] = 2;
        star[1] = 0;
        star[24] = (s32)effStarStoneEntry(0.0f, -1000.0f, 0.0f, 1.0f, index);
        BtlUnit_GetPos(mario, (f32*)&star[3], (f32*)&star[4], (f32*)&star[5]);
        *(f32*)&star[4] += 37.0f;
        star[6] = star[3]; star[7] = star[4]; star[8] = star[5];
        star[9] = star[3]; star[10] = star[4]; star[11] = star[5];
        *(f32*)&star[10] += 30.0f;
        *(f32*)&star[15] = *(f32*)&star[16] = *(f32*)&star[17] = 0.0f;
        *(f32*)&star[18] = *(f32*)&star[19] = *(f32*)&star[20] = 0.0f;
        *(f32*)&star[21] = 0.0f;
        *(f32*)&star[22] = 6.2832f * (f32)index / 7.0f;
        *(f32*)&star[23] = 0.1f;
    }
    if (star[0] == 2) {
        star[1]++;
        BtlUnit_GetPos(mario, &marioX, &marioY, &marioZ);
        if (star[1] < 101) {
            *(f32*)&star[4] = (f32)intplGetValue(*(f32*)&star[7], *(f32*)&star[10], 0, star[1], 100);
            *(f32*)&star[21] = (f32)intplGetValue(0.0, 40.0, 0, star[1], 100);
            *(f32*)&star[22] += *(f32*)&star[23];
            *(f32*)&star[3] = *(f32*)&star[21] * (f32)cos(*(f32*)&star[22]) + marioX;
            *(f32*)&star[5] = *(f32*)&star[21] * (f32)sin(*(f32*)&star[22]) + marioZ;
            *(f32*)&star[15] = *(f32*)&star[16] = *(f32*)&star[17] =
                (f32)intplGetValue(0.0, 0.75, 0, star[1], 100);
        } else {
            *(f32*)&star[4] = *(f32*)&star[10];
            *(f32*)&star[22] += *(f32*)&star[23];
            *(f32*)&star[3] = *(f32*)&star[21] * (f32)cos(*(f32*)&star[22]) + marioX;
            *(f32*)&star[5] = *(f32*)&star[21] * (f32)sin(*(f32*)&star[22]) + marioZ;
            *(f32*)&star[15] = *(f32*)&star[16] = *(f32*)&star[17] = 0.75f;
        }
        *(f32*)&star[19] = (f32)intplGetValue(0.0, 2160.0, 4, star[1], 120);
        if (star[1] > 119) {
            star[0] = 3;
            star[1] = 0;
            star[6] = star[3]; star[7] = star[4]; star[8] = star[5];
            *(f32*)&star[9] = 200.0f * (f32)rand() / 32767.0f;
            *(f32*)&star[10] = 120.0f;
            star[11] = star[8];
            *(f32*)&star[12] = 2.0f;
            *(f32*)&star[13] = (2.0f * (*(f32*)&star[10] - *(f32*)&star[7]) -
                60.0f * 60.0f * -(0.01f * (f32)index - 0.2f)) / 120.0f;
            *(f32*)&star[14] = 0.0f;
        }
    } else if (star[0] == 3) {
        star[1]++;
        *(f32*)&star[3] = (f32)intplGetValue(*(f32*)&star[6], *(f32*)&star[9], 4, star[1], 60);
        *(f32*)&star[4] += *(f32*)&star[13];
        *(f32*)&star[13] += -(0.01f * (f32)index - 0.2f);
        *(f32*)&star[15] = *(f32*)&star[16] = *(f32*)&star[17] = 0.75f;
        *(f32*)&star[19] = 0.0f;
        if (star[1] > 59) {
            star[0] = 4;
            star[1] = 0;
            *(f32*)&star[12] = irand(2) == 0 ? -(0.05f * (f32)index + 2.0f) : 0.05f * (f32)index + 2.0f;
            *(f32*)&star[13] = 0.0f;
        }
    } else if (star[0] == 4) {
        *(f32*)&star[3] += *(f32*)&star[12];
        if (*(f32*)&star[3] < 0.0f) *(f32*)&star[12] = 0.05f * (f32)index + 3.0f;
        if (*(f32*)&star[3] > 200.0f) *(f32*)&star[12] = -(0.05f * (f32)index + 3.0f);
        *(f32*)&star[13] += 0.01f * (f32)index + 0.1f;
        if (*(f32*)&star[13] >= 6.2832f) *(f32*)&star[13] -= 6.2832f;
        *(f32*)&star[4] = 10.0f * (f32)sin(*(f32*)&star[13]) + 180.0f;
        if (*(s32*)(work + 0x40) < 1) star[0] = 5;
    }
    if (star[24] != 0) {
        u8* effectWork = *(u8**)((u8*)star[24] + 0xC);
        *(f32*)(effectWork + 8) = *(f32*)&star[3];
        *(f32*)(effectWork + 0xC) = *(f32*)&star[4];
        *(f32*)(effectWork + 0x10) = *(f32*)&star[5];
        *(f32*)(effectWork + 0x18) = *(f32*)&star[18];
        *(f32*)(effectWork + 0x1C) = *(f32*)&star[19];
        *(f32*)(effectWork + 0x20) = *(f32*)&star[20];
        *(f32*)(effectWork + 0x14) = 1.5f * *(f32*)&star[15];
    }
}

/* stub-fill: main_star1 | prototype_only | source_prototype */
void main_star1(void) {
    extern void* get_ptr(void);
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void*);
    extern void BtlUnit_GetPos(void*, f32*, f32*, f32*);
    extern void* effStarStoneEntry(f32, f32, f32, f32, s32);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern s32 rand(void);
    extern s32 irand(s32);
    extern f64 sin(f64);

    u8* work = get_ptr();
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 state = *(s32*)(work + 0x510);
    s32 timer;

    if (state == 1) {
        *(s32*)(work + 0x510) = 2;
        *(s32*)(work + 0x514) = 0;
        *(void**)(work + 0x570) = effStarStoneEntry(0.0f, -1000.0f, 0.0f, 1.0f, 4);
        BtlUnit_GetPos(mario, (f32*)(work + 0x51C), (f32*)(work + 0x520), (f32*)(work + 0x524));
        *(f32*)(work + 0x520) += *(f32*)((s32)mario + 0x114) * (f32)*(s16*)((s32)mario + 0xCE) + 37.0f;
        *(f32*)(work + 0x528) = *(f32*)(work + 0x51C);
        *(f32*)(work + 0x52C) = *(f32*)(work + 0x520);
        *(f32*)(work + 0x530) = *(f32*)(work + 0x524);
        *(f32*)(work + 0x534) = *(f32*)(work + 0x51C);
        *(f32*)(work + 0x538) = *(f32*)(work + 0x520) + 50.0f;
        *(f32*)(work + 0x53C) = *(f32*)(work + 0x524) - 1.0f;
        *(f32*)(work + 0x54C) = 0.0f;
        *(f32*)(work + 0x550) = 0.0f;
        *(f32*)(work + 0x554) = 0.0f;
        *(f32*)(work + 0x558) = 0.0f;
        *(f32*)(work + 0x55C) = 0.0f;
        *(f32*)(work + 0x560) = 0.0f;
        state = 2;
    }
    if (state == 2) {
        timer = ++*(s32*)(work + 0x514);
        if (timer < 101) {
            *(f32*)(work + 0x51C) = (f32)intplGetValue(*(f32*)(work + 0x528), *(f32*)(work + 0x534), 0, timer, 100);
            *(f32*)(work + 0x520) = (f32)intplGetValue(*(f32*)(work + 0x52C), *(f32*)(work + 0x538), 0, timer, 100);
            *(f32*)(work + 0x524) = (f32)intplGetValue(*(f32*)(work + 0x530), *(f32*)(work + 0x53C), 0, timer, 100);
            *(f32*)(work + 0x54C) = (f32)intplGetValue(0.0, 2.0, 0, timer, 100);
            *(f32*)(work + 0x550) = *(f32*)(work + 0x54C);
            *(f32*)(work + 0x554) = *(f32*)(work + 0x54C);
        } else {
            *(f32*)(work + 0x51C) = *(f32*)(work + 0x534);
            *(f32*)(work + 0x520) = *(f32*)(work + 0x538);
            *(f32*)(work + 0x524) = *(f32*)(work + 0x53C);
            *(f32*)(work + 0x54C) = 2.0f;
            *(f32*)(work + 0x550) = 2.0f;
            *(f32*)(work + 0x554) = 2.0f;
        }
        *(f32*)(work + 0x55C) = (f32)intplGetValue(0.0, 2160.0, 4, timer, 120);
        if (timer > 119) {
            *(s32*)(work + 0x510) = 3;
            *(s32*)(work + 0x514) = 0;
            *(f32*)(work + 0x528) = *(f32*)(work + 0x51C);
            *(f32*)(work + 0x52C) = *(f32*)(work + 0x520);
            *(f32*)(work + 0x530) = *(f32*)(work + 0x524);
            *(f32*)(work + 0x534) = 200.0f * (f32)rand() / 32767.0f;
            *(f32*)(work + 0x538) = 0.0f;
            *(f32*)(work + 0x53C) = *(f32*)(work + 0x530);
            *(f32*)(work + 0x540) = 0.0f;
            *(f32*)(work + 0x544) =
                (2.0f * (*(f32*)(work + 0x538) - *(f32*)(work + 0x52C)) - 720.0f) / 120.0f;
            *(f32*)(work + 0x548) = 0.0f;
        }
    } else if (state == 3) {
        timer = ++*(s32*)(work + 0x514);
        *(f32*)(work + 0x51C) = (f32)intplGetValue(*(f32*)(work + 0x528), *(f32*)(work + 0x534), 4, timer, 60);
        *(f32*)(work + 0x520) += *(f32*)(work + 0x544);
        *(f32*)(work + 0x544) += 0.2f;
        *(f32*)(work + 0x54C) = 2.0f;
        *(f32*)(work + 0x550) = 2.0f;
        *(f32*)(work + 0x554) = 2.0f;
        *(f32*)(work + 0x55C) = 0.0f;
        if (timer > 59) {
            *(s32*)(work + 0x510) = 4;
            *(s32*)(work + 0x514) = 0;
            *(f32*)(work + 0x540) = irand(2) == 0 ? -2.0f : 2.0f;
            *(f32*)(work + 0x544) = 0.0f;
        }
    } else if (state == 4) {
        *(f32*)(work + 0x51C) += *(f32*)(work + 0x540);
        if (*(f32*)(work + 0x51C) < 0.0f) *(f32*)(work + 0x540) = 3.0f;
        if (*(f32*)(work + 0x51C) > 200.0f) *(f32*)(work + 0x540) = -3.0f;
        *(f32*)(work + 0x544) += 0.1f;
        if (*(f32*)(work + 0x544) >= 6.2832f) *(f32*)(work + 0x544) -= 6.2832f;
        *(f32*)(work + 0x520) = 10.0f * (f32)sin(*(f32*)(work + 0x544)) + 180.0f;
        if (*(s32*)(work + 0x40) < 1) {
            *(s32*)(work + 0x510) = 5;
            *(s32*)(work + 0x514) = 0;
            *(f32*)(work + 0x51C) = 0.0f;
            *(f32*)(work + 0x520) = 0.0f;
            *(f32*)(work + 0x524) = 0.0f;
        }
    }

    if (*(void**)(work + 0x570) != 0) {
        u8* effectWork = *(u8**)((s32)*(void**)(work + 0x570) + 0xC);
        *(f32*)(effectWork + 8) = *(f32*)(work + 0x51C);
        *(f32*)(effectWork + 0xC) = *(f32*)(work + 0x520);
        *(f32*)(effectWork + 0x10) = *(f32*)(work + 0x524);
        *(f32*)(effectWork + 0x18) = *(f32*)(work + 0x558);
        *(f32*)(effectWork + 0x1C) = *(f32*)(work + 0x55C);
        *(f32*)(effectWork + 0x20) = *(f32*)(work + 0x560);
        *(f32*)(effectWork + 0x14) = 1.5f * *(f32*)(work + 0x54C);
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

void disp_2D(void) {
    ;
}



/* CHATGPT STUB FILL: main/action/star/sac_genki 20260624_184929 */

/* stub-fill: disp_3D | missing_definition | ghidra_signature */
void disp_3D(void) {
    extern u8* get_ptr(void);
    extern void* g_BattleWork;
    extern void* BattleGetMarioPtr(void*);
    extern void* BattleGetPartyPtr(void*);
    extern void BtlUnit_GetHomePos(void*, f32*, f32*, f32*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXTransApply(void*, void*, f32, f32, f32);
    extern void iconNumberDispGx3D(void*, s32, s32, void*);
    extern void iconDispGxCol(void*, u16, u16, void*);
    u8* work = get_ptr();
    void* mario = BattleGetMarioPtr(g_BattleWork);
    void* party = BattleGetPartyPtr(g_BattleWork);
    f32 mx, my, mz, px, py, pz;
    f32 mtx[3][4];
    u32 color;
    s32 frame;
    f32 x;
    s32 i;

    (void)(3.141592f * *(f32*)(work + 0x6C) / 180.0f);
    if (*(s32*)(work + 4) == 0) {
        return;
    }
    BtlUnit_GetHomePos(mario, &mx, &my, &mz);
    my += 60.0f;
    {
        u8* data = *(u8**)(*(u8**)(work + 0x34) + 0xC);
        *(f32*)(data + 8) = mx;
        *(f32*)(data + 0xC) = my - 10.0f;
        *(f32*)(data + 0x10) = mz + 5.0f;
    }
    x = *(f32*)(work + 0x1C) < 10.0f ? mx : mx + 6.0f;
    PSMTXTrans(mtx, x, my, mz + 8.0f);
    frame = *(s32*)(work + 0x28) % 12;
    if (frame >= 0 && frame < 6) {
        color = 0xFFFFFFFF;
        iconNumberDispGx3D(mtx, (s32)*(f32*)(work + 0x1C), 0, &color);
    }
    if (*(s32*)(work + 0x50) == -1) {
        u8* data;
        BtlUnit_GetHomePos(mario, &mx, &my, &mz);
        my += 90.0f;
        data = *(u8**)(*(u8**)(work + 0x3C) + 0xC);
        *(f32*)(data + 8) = mx;
        *(f32*)(data + 0xC) = my - 10.0f;
        *(f32*)(data + 0x10) = mz + 5.0f;
        x = *(f32*)(work + 0x24) < 10.0f ? mx : mx + 6.0f;
        PSMTXTrans(mtx, x, my, mz + 8.0f);
        frame = *(s32*)(work + 0x30) % 12;
        if (frame >= 0 && frame < 6) {
            color = 0xFFFFFFFF;
            iconNumberDispGx3D(mtx, (s32)*(f32*)(work + 0x24), 0, &color);
        }
    } else {
        u8* data;
        BtlUnit_GetHomePos(party, &px, &py, &pz);
        py += 60.0f;
        data = *(u8**)(*(u8**)(work + 0x38) + 0xC);
        *(f32*)(data + 8) = px;
        *(f32*)(data + 0xC) = py - 10.0f;
        *(f32*)(data + 0x10) = pz + 5.0f;
        x = *(f32*)(work + 0x20) < 10.0f ? px : px + 6.0f;
        PSMTXTrans(mtx, x, py, pz + 8.0f);
        frame = *(s32*)(work + 0x2C) % 12;
        if (frame >= 0 && frame < 6) {
            color = 0xFFFFFFFF;
            iconNumberDispGx3D(mtx, (s32)*(f32*)(work + 0x20), 0, &color);
        }
        BtlUnit_GetHomePos(mario, &mx, &my, &mz);
        BtlUnit_GetHomePos(party, &px, &py, &pz);
        mx = 0.5f * (mx + px);
        my += 90.0f;
        data = *(u8**)(*(u8**)(work + 0x3C) + 0xC);
        *(f32*)(data + 8) = mx;
        *(f32*)(data + 0xC) = my - 10.0f;
        *(f32*)(data + 0x10) = mz + 5.0f;
        x = *(f32*)(work + 0x24) < 10.0f ? mx : mx + 6.0f;
        PSMTXTrans(mtx, x, my, mz + 8.0f);
        frame = *(s32*)(work + 0x30) % 12;
        if (frame >= 0 && frame < 6) {
            color = 0xFFFFFFFF;
            iconNumberDispGx3D(mtx, (s32)*(f32*)(work + 0x24), 0, &color);
        }
    }

    for (i = 0; i < 10; i++) {
        s32* entry = (s32*)(work + 0x358 + i * 0x2C);
        if (entry[0] == 2) {
            s32 kind = entry[2];
            u16 icon = 0;
            u16 flags = 0;
            f32 scale = 1.25f;
            if (kind == 0) {
                icon = 0x1AB;
            } else if (kind == 1) {
                icon = 0x1AB;
                if (entry[1] % 12 >= 0 && entry[1] % 12 < 6) {
                    flags = 0x20;
                    scale = 2.0f;
                }
            } else if (kind == 5) {
                icon = 0x1B9;
                if (entry[1] % 12 >= 0 && entry[1] % 12 < 6) {
                    flags = 0x20;
                    scale = 2.0f;
                }
            } else if (kind == 6) {
                icon = 0x14B;
            }
            PSMTXScale(mtx, scale, scale, 1.0f);
            PSMTXTransApply(mtx, mtx, (f32)entry[3], (f32)entry[4], (f32)entry[5]);
            color = 0xFFFFFFFF;
            iconDispGxCol(mtx, flags, icon, &color);
        }
    }
}

void disp_3D_alpha(void) {
    ;
}

void* get_ptr(void) {
    return (void*)((s32)_battleWorkPointer + 0x1F4C);
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

USER_FUNC(status_on) {
    statusWinOpen();
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

USER_FUNC(star_stone_appear) {
    void* work = get_ptr();
    *(s32*)((s32)work + 0x510) = 1;
    *(s32*)((s32)work + 0x518) = 1;
    return 2;
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
