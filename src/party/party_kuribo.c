#include "party/party_kuribo.h"

extern s32 msg_ep;
extern s32 marioGetPartyId();
extern void* partyGetPtr();
extern void marioStSystemLevel();
extern void psndClearFlag();

s32 pre_kurio_use(void* pEvt) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void psndSetFlag(u32 flag);
    extern u32 hitGetAttr(void* hit);
    extern void* mobjHitObjPtrToPtr(void* hit);
    extern s32 strcmp(const char* a, const char* b);
    extern s32 pouchGetHammerLv(void);
    extern s32 pouchGetJumpLv(void);
    extern s32 mobjCheckItemboxOpen(void* mobj);
    extern void partyChgPoseId(void* party, s32 pose);
    extern char str_msg_kuri_map_802cb8f0[];
    extern char str_MOBJ_Lv1Block_802cb900[], str_MOBJ_Lv1BigBlock_802cb938[];
    extern char str_MOBJ_Lv1BigBigBlock_802cb94c[], str_MOBJ_Lv2Block_802cb988[];
    extern char str_MOBJ_Lv2BigBlock_802cb998[], str_MOBJ_Lv2BigBigBlock_802cb9e8[];
    extern char str_MOBJ_Lv3BigBigBlock_802cba10[], str_MOBJ_Lv3BigBlock_802cba24[];
    extern char str_MOBJ_Lv3Block_802cba38[], str_MOBJ_Block_802cba60[];
    extern char str_MOBJ_PinkBlock_802cba6c[], str_MOBJ_PowerUpBlock_802cba90[];
    extern char str_MOBJ_HatenaBlock_802cbab4[], str_MOBJ_BadgeBlock_802cbad8[];
    extern char str_MOBJ_TreasureBox_802cbccc[], str_MOBJ_GrayTreasureBox_802cbce0[];
    extern char str_MOBJ_BigTreasureBox_802cbcf8[], str_MOBJ_BlackTreasureBo_802cbd34[];
    extern char str_MOBJ_RedJumpStand_802cbd74[], str_MOBJ_RedJumpStand2_802cbd88[];
    extern char str_MOBJ_BlueJumpStand_802cbd9c[], str_MOBJ_BlueJumpStand2_802cbdb0[];
    extern char str_MOBJ_MapJumpStand_802cbdc4[], str_MOBJ_RedTimerSwitch_802cbde8[];
    extern char str_MOBJ_BlueTimerSwitch_802cbdfc[], str_MOBJ_RedSwitch_802cbe2c[];
    extern char str_MOBJ_BlueSwitch_802cbe3c[], str_MOBJ_WhiteSwitch_802cbe4c[];
    extern char str_MOBJ_BlackSwitch_802cbe60[], str_MOBJ_BreakingFloor_802cbed4[];
    extern char str_MOBJ_BlackBreakingFl_802cbee8[], str_MOBJ_BombRock_802cbf2c[];
    extern char str_MOBJ_Signboard_802cbf50[], str_MOBJ_Arrow_802cbf70[];
    extern char str_MOBJ_Lock_802cbf90[];
    extern char str_obj_hlp_block_y_0_802cb910[], str_obj_hlp_block_y_1_802cb924[];
    extern char str_obj_hlp_block_y_2_802cb960[], str_obj_hlp_block_y_3_802cb974[];
    extern char str_obj_hlp_block_s_0_802cb9ac[], str_obj_hlp_block_s_1_802cb9c0[];
    extern char str_obj_hlp_block_s_2_802cb9d4[], str_obj_hlp_block_s_3_802cb9fc[];
    extern char str_obj_hlp_redyellow_bl_802cba48[], str_obj_hlp_su_block_802cba7c[];
    extern char str_obj_hlp_shine_802cbaa4[], str_obj_hlp_hatena_802cbac8[];
    extern char str_obj_hlp_badge_hatena_802cbae8[], str_obj_hlp_takara_0_802cbd20[];
    extern char str_obj_hlp_takara_1_802cbd0c[], str_obj_hlp_blackbox_0_802cbd60[];
    extern char str_obj_hlp_blackbox_1_802cbd4c[], str_obj_hlp_jump_802cbdd8[];
    extern char str_obj_hlp_timer_switch_802cbe14[], str_obj_hlp_switch_802cbe74[];
    extern char str_obj_hlp_hip_attack_802cbf00[], str_obj_hlp_hip_attack_1_802cbf14[];
    extern char str_obj_hlp_bomb_rock_802cbf3c[], str_obj_hlp_kanban_802cbf60[];
    extern char str_obj_hlp_yajirushi_802cbf7c[], str_obj_hlp_lock_802cbf9c[];
    void* party = partyGetPtr(marioGetPartyId());
    s32* work = (s32*)((s32)pEvt + 0x9C);
    char* message = str_msg_kuri_map_802cb8f0;

    if (party == 0 || (*(u32*)((s32)party + 4) & 0x100) != 0) {
        return 2;
    }
    psndSetFlag(0x80);
    *(u32*)party |= 0x20400000;
    if (work[0] == 0) {
        void* player = *(void**)((s32)party + 0x160);
        void* hit = *(void**)((s32)player + 0x1E0);
        if (hit == 0 || (hitGetAttr(hit) & 0x80000000) == 0) hit = *(void**)((s32)player + 0x1E4);
        if (hit == 0 || (hitGetAttr(hit) & 0x80000000) == 0) hit = *(void**)((s32)player + 0x1E8);
        if (hit != 0 && (hitGetAttr(hit) & 0x80000000) != 0) {
            void* mobj = mobjHitObjPtrToPtr(hit);
            char* name = (char*)mobj + 0x15;
            s32 hammer = pouchGetHammerLv();
            if (!strcmp(name, str_MOBJ_Lv1Block_802cb900) || !strcmp(name, str_MOBJ_Lv1BigBlock_802cb938)) {
                message = hammer < 1 ? str_obj_hlp_block_y_0_802cb910 : str_obj_hlp_block_y_1_802cb924;
            } else if (!strcmp(name, str_MOBJ_Lv1BigBigBlock_802cb94c)) {
                message = hammer < 1 ? str_obj_hlp_block_y_0_802cb910 :
                          hammer < 2 ? str_obj_hlp_block_y_2_802cb960 : str_obj_hlp_block_y_3_802cb974;
            } else if (!strcmp(name, str_MOBJ_Lv2Block_802cb988) || !strcmp(name, str_MOBJ_Lv2BigBlock_802cb998)) {
                message = hammer < 1 ? str_obj_hlp_block_s_0_802cb9ac :
                          hammer < 3 ? str_obj_hlp_block_s_1_802cb9c0 : str_obj_hlp_block_s_2_802cb9d4;
            } else if (!strcmp(name, str_MOBJ_Lv2BigBigBlock_802cb9e8)) {
                message = hammer < 1 ? str_obj_hlp_block_s_0_802cb9ac :
                          hammer < 3 ? str_obj_hlp_block_s_1_802cb9c0 : str_obj_hlp_block_s_3_802cb9fc;
            } else if (!strcmp(name, str_MOBJ_Lv3BigBigBlock_802cba10) ||
                       !strcmp(name, str_MOBJ_Lv3BigBlock_802cba24) || !strcmp(name, str_MOBJ_Lv3Block_802cba38)) {
                message = str_obj_hlp_redyellow_bl_802cba48;
            } else if (!strcmp(name, str_MOBJ_Block_802cba60) || !strcmp(name, str_MOBJ_PinkBlock_802cba6c)) {
                message = str_obj_hlp_su_block_802cba7c;
            } else if (!strcmp(name, str_MOBJ_PowerUpBlock_802cba90)) {
                message = str_obj_hlp_shine_802cbaa4;
            } else if (!strcmp(name, str_MOBJ_HatenaBlock_802cbab4)) {
                message = str_obj_hlp_hatena_802cbac8;
            } else if (!strcmp(name, str_MOBJ_BadgeBlock_802cbad8)) {
                message = str_obj_hlp_badge_hatena_802cbae8;
            } else if (!strcmp(name, str_MOBJ_TreasureBox_802cbccc) ||
                       !strcmp(name, str_MOBJ_GrayTreasureBox_802cbce0) ||
                       !strcmp(name, str_MOBJ_BigTreasureBox_802cbcf8)) {
                message = mobjCheckItemboxOpen(mobj) == 2 ? str_obj_hlp_takara_1_802cbd0c : str_obj_hlp_takara_0_802cbd20;
            } else if (!strcmp(name, str_MOBJ_BlackTreasureBo_802cbd34)) {
                message = mobjCheckItemboxOpen(mobj) == 2 ? str_obj_hlp_blackbox_1_802cbd4c : str_obj_hlp_blackbox_0_802cbd60;
            } else if (!strcmp(name, str_MOBJ_RedJumpStand_802cbd74) ||
                       !strcmp(name, str_MOBJ_RedJumpStand2_802cbd88) ||
                       !strcmp(name, str_MOBJ_BlueJumpStand_802cbd9c) ||
                       !strcmp(name, str_MOBJ_BlueJumpStand2_802cbdb0) ||
                       !strcmp(name, str_MOBJ_MapJumpStand_802cbdc4)) {
                message = str_obj_hlp_jump_802cbdd8;
            } else if (!strcmp(name, str_MOBJ_RedTimerSwitch_802cbde8) || !strcmp(name, str_MOBJ_BlueTimerSwitch_802cbdfc)) {
                message = str_obj_hlp_timer_switch_802cbe14;
            } else if (!strcmp(name, str_MOBJ_RedSwitch_802cbe2c) || !strcmp(name, str_MOBJ_BlueSwitch_802cbe3c) ||
                       !strcmp(name, str_MOBJ_WhiteSwitch_802cbe4c) || !strcmp(name, str_MOBJ_BlackSwitch_802cbe60)) {
                message = str_obj_hlp_switch_802cbe74;
            } else if (!strcmp(name, str_MOBJ_BreakingFloor_802cbed4) || !strcmp(name, str_MOBJ_BlackBreakingFl_802cbee8)) {
                message = pouchGetJumpLv() > 1 ? str_obj_hlp_hip_attack_1_802cbf14 : str_obj_hlp_hip_attack_802cbf00;
            } else if (!strcmp(name, str_MOBJ_BombRock_802cbf2c)) {
                message = str_obj_hlp_bomb_rock_802cbf3c;
            } else if (!strcmp(name, str_MOBJ_Signboard_802cbf50)) {
                message = str_obj_hlp_kanban_802cbf60;
            } else if (!strcmp(name, str_MOBJ_Arrow_802cbf70)) {
                message = str_obj_hlp_yajirushi_802cbf7c;
            } else if (!strcmp(name, str_MOBJ_Lock_802cbf90)) {
                message = str_obj_hlp_lock_802cbf9c;
            }
            work[0] = (s32)message;
            partyChgPoseId(party, 7);
        } else {
            work[0] = (s32)message;
        }
    } else {
        work[0] += 8;
    }
    return 2;
}

void kuribo_use(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 partySearchFrontWall(f64 distance, f64 angle, void* pParty, f32* pos);
    extern void partyChgPoseId(void* party, s32 pose);
    extern s32 partyGetAppearPos3(void* party, void* pos);
    extern void* evtEntryType(void* evt, s32 priority, u32 flags, u32 type);
    extern void marioStSystemLevel(s32 level);
    extern void* fbatNpcTalkCheck(void);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partyChgMot(void* party, s32 motion);
    extern void partyChkGnd(void* party);
    extern f32 float_125_80421578;
    extern f32 float_150_8042157c;
    extern f32 float_0_80421580;
    extern u8 kurio_msg_evt[];

    void* player;
    void* evt;
    VecLocal pos;
    s32 hit;
    s32 timer;
    f32 denom;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 0x8) & 8) != 0) {
        *(u32*)((s32)pParty + 0x8) &= ~8;
        if (PSVECDistance((void*)((s32)player + 0x8C), (void*)((s32)pParty + 0x58)) > float_125_80421578) {
            *(u8*)((s32)pParty + 0x39) = 0;
        } else {
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            hit = partySearchFrontWall(float_150_8042157c,
                                       angleABf(*(f32*)((s32)pParty + 0x58),
                                                *(f32*)((s32)pParty + 0x60),
                                                *(f32*)((s32)player + 0x8C),
                                                *(f32*)((s32)player + 0x94)),
                                       pParty, &pos.x);
            if (hit != 0) {
                if (*(f32*)((s32)pParty + 0xEC) <
                    distABf(*(f32*)((s32)pParty + 0x58),
                            *(f32*)((s32)pParty + 0x60),
                            *(f32*)((s32)player + 0x8C),
                            *(f32*)((s32)player + 0x94))) {
                    *(u8*)((s32)pParty + 0x39) = 0;
                } else {
                    hit = 0;
                }
            }
            if (hit == 0) {
                partyChgPoseId(pParty, 1);
                *(f32*)((s32)pParty + 0x104) = float_0_80421580;
                *(s32*)((s32)pParty + 0x24) = 100;
                *(u8*)((s32)pParty + 0x39) = 10;
            }
        }
    }

    switch (*(u8*)((s32)pParty + 0x39)) {
        case 0:
            if (partyGetAppearPos3(pParty, (void*)((s32)pParty + 0x94)) == 0) {
                *(u32*)pParty |= 0x4000000;
                *(u32*)pParty |= 0x2000000;
            }
            *(s32*)((s32)pParty + 0x24) = 4;
            *(f32*)((s32)pParty + 0xFC) =
                angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                         *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
            *(f32*)((s32)pParty + 0x100) = *(f32*)((s32)pParty + 0xFC);
            *(f32*)((s32)pParty + 0x10C) = *(f32*)((s32)player + 0x1AC);
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)player + 0x1AC);
            denom = (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x104) =
                distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                        *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C)) / denom;
            *(f32*)((s32)pParty + 0x114) = float_0_80421580;
            *(u8*)((s32)pParty + 0x39) = 1;
            break;
        case 1:
            if (partyGetAppearPos3(pParty, (void*)((s32)pParty + 0x94)) == 0) {
                *(u32*)pParty |= 0x4000000;
                *(u32*)pParty |= 0x2000000;
            }
            denom = (f32)*(s32*)((s32)pParty + 0x24);
            *(f32*)((s32)pParty + 0x58) += (*(f32*)((s32)pParty + 0x94) - *(f32*)((s32)pParty + 0x58)) / denom;
            *(f32*)((s32)pParty + 0x5C) += (*(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C)) / denom;
            *(f32*)((s32)pParty + 0x60) += (*(f32*)((s32)pParty + 0x9C) - *(f32*)((s32)pParty + 0x60)) / denom;
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer > 0) {
                return;
            }
            partyChgPoseId(pParty, 1);
            *(f32*)((s32)pParty + 0x104) = float_0_80421580;
            *(s32*)((s32)pParty + 0x24) = 100;
            *(u8*)((s32)pParty + 0x39) = 10;
            break;
        case 10:
            evt = evtEntryType(kurio_msg_evt, 0, 0, 0x1A);
            marioStSystemLevel(1);
            *(void**)((s32)evt + 0x9C) = fbatNpcTalkCheck();
            *(u8*)((s32)pParty + 0x39) = 15;
            break;
        case 20:
            *(s32*)((s32)pParty + 0x24) = 5;
            *(u8*)((s32)pParty + 0x39) = 21;
        case 21:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 1) {
                partyChgRunMode(pParty, 2);
                partyChgMoveMode(pParty, 0);
                partyChgMot(pParty, 0);
                partyChgPoseId(pParty, 1);
                *(u32*)pParty &= ~0x100;
                *(u32*)pParty &= ~0x4000000;
                *(u32*)pParty &= ~0x2000000;
            }
            break;
    }
    partyChkGnd(pParty);
}

s32 post_kurio_use(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party == NULL) {
        return 2;
    }
    *(u8*)((s32)party + 0x39) = 0x14;
    marioStSystemLevel(0);
    psndClearFlag(0x80);
    *(u32*)party &= ~0x4000000;
    *(u32*)party &= ~0x2000000;
    return 2;
}


u8 kuribo_bye(void* pParty) {
    extern void partyGoodbyeInit(void* party);
    extern s32 partyGoodbyeMain(void* party);
    extern void partyChgRunMode(void* party, s32 mode);

    if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
        *(u32*)((s32)pParty + 8) &= ~4;
        partyGoodbyeInit(pParty);
    }
    if (partyGoodbyeMain(pParty) != 0) {
        partyChgRunMode(pParty, 0xE);
    }
}

s32 christineGetStatus(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != NULL && *(s8*)((s32)party + 0x31) == 1 && (*(u32*)party & 0x100)) {
        return 1;
    }
    return 0;
}


u8 kuribo_init(void* pParty) {
    *(u8*)((s32)pParty + 0x33) = 1;
    *(u8*)((s32)pParty + 0x32) = 0;
    *(f32*)((s32)pParty + 0xF0) = 24.0f;
    *(f32*)((s32)pParty + 0xF4) = 26.0f;
    msg_ep = 0;
}


void kuribo_use_post(void* pParty) {
    *(u32*)pParty &= ~0x4000000;
    *(u32*)pParty &= ~0x2000000;
}

