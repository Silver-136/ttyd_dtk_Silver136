#include "party/party_kuribo.h"

s32 msg_ep;
u32 gap_08_8041EAE4_sbss;
extern s32 marioGetPartyId();
extern void* partyGetPtr();
extern void marioStSystemLevel();
extern void psndClearFlag();

extern void partyChgPoseId(void* party, s32 pose);
extern void partyChgRunMode(void* party, s32 mode);
extern void evt_msg_print(void);

s32 christineGetStatus(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != NULL && *(s8*)((s32)party + 0x31) == 1 && (*(u32*)party & 0x100)) {
        return 1;
    }
    return 0;
}

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
    extern const char str_msg_kuri_map_802cb8f0[];
    extern const char str_MOBJ_Lv1Block_802cb900[], str_MOBJ_Lv1BigBlock_802cb938[];
    extern const char str_MOBJ_Lv1BigBigBlock_802cb94c[], str_MOBJ_Lv2Block_802cb988[];
    extern const char str_MOBJ_Lv2BigBlock_802cb998[], str_MOBJ_Lv2BigBigBlock_802cb9e8[];
    extern const char str_MOBJ_Lv3BigBigBlock_802cba10[], str_MOBJ_Lv3BigBlock_802cba24[];
    extern const char str_MOBJ_Lv3Block_802cba38[], str_MOBJ_Block_802cba60[];
    extern const char str_MOBJ_PinkBlock_802cba6c[], str_MOBJ_PowerUpBlock_802cba90[];
    extern const char str_MOBJ_HatenaBlock_802cbab4[], str_MOBJ_BadgeBlock_802cbad8[];
    extern const char str_MOBJ_TreasureBox_802cbccc[], str_MOBJ_GrayTreasureBox_802cbce0[];
    extern const char str_MOBJ_BigTreasureBox_802cbcf8[], str_MOBJ_BlackTreasureBo_802cbd34[];
    extern const char str_MOBJ_RedJumpStand_802cbd74[], str_MOBJ_RedJumpStand2_802cbd88[];
    extern const char str_MOBJ_BlueJumpStand_802cbd9c[], str_MOBJ_BlueJumpStand2_802cbdb0[];
    extern const char str_MOBJ_MapJumpStand_802cbdc4[], str_MOBJ_RedTimerSwitch_802cbde8[];
    extern const char str_MOBJ_BlueTimerSwitch_802cbdfc[], str_MOBJ_RedSwitch_802cbe2c[];
    extern const char str_MOBJ_BlueSwitch_802cbe3c[], str_MOBJ_WhiteSwitch_802cbe4c[];
    extern const char str_MOBJ_BlackSwitch_802cbe60[], str_MOBJ_BreakingFloor_802cbed4[];
    extern const char str_MOBJ_BlackBreakingFl_802cbee8[], str_MOBJ_BombRock_802cbf2c[];
    extern const char str_MOBJ_Signboard_802cbf50[], str_MOBJ_Arrow_802cbf70[];
    extern const char str_MOBJ_Lock_802cbf90[];
    extern const char str_obj_hlp_block_y_0_802cb910[], str_obj_hlp_block_y_1_802cb924[];
    extern const char str_obj_hlp_block_y_2_802cb960[], str_obj_hlp_block_y_3_802cb974[];
    extern const char str_obj_hlp_block_s_0_802cb9ac[], str_obj_hlp_block_s_1_802cb9c0[];
    extern const char str_obj_hlp_block_s_2_802cb9d4[], str_obj_hlp_block_s_3_802cb9fc[];
    extern const char str_obj_hlp_redyellow_bl_802cba48[], str_obj_hlp_su_block_802cba7c[];
    extern const char str_obj_hlp_shine_802cbaa4[], str_obj_hlp_hatena_802cbac8[];
    extern const char str_obj_hlp_badge_hatena_802cbae8[], str_obj_hlp_takara_0_802cbd20[];
    extern const char str_obj_hlp_takara_1_802cbd0c[], str_obj_hlp_blackbox_0_802cbd60[];
    extern const char str_obj_hlp_blackbox_1_802cbd4c[], str_obj_hlp_jump_802cbdd8[];
    extern const char str_obj_hlp_timer_switch_802cbe14[], str_obj_hlp_switch_802cbe74[];
    extern const char str_obj_hlp_hip_attack_802cbf00[], str_obj_hlp_hip_attack_1_802cbf14[];
    extern const char str_obj_hlp_bomb_rock_802cbf3c[], str_obj_hlp_kanban_802cbf60[];
    extern const char str_obj_hlp_yajirushi_802cbf7c[], str_obj_hlp_lock_802cbf9c[];
    void* party = partyGetPtr(marioGetPartyId());
    s32* work = (s32*)((s32)pEvt + 0x9C);
    const char* message = str_msg_kuri_map_802cb8f0;

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


u8 kuribo_init(void* pParty) {
    extern const f32 float_24_80421584;
    extern const f32 float_26_80421588;

    *(u8*)((s32)pParty + 0x33) = 1;
    *(u8*)((s32)pParty + 0x32) = 0;
    *(f32*)((s32)pParty + 0xF0) = float_24_80421584;
    *(f32*)((s32)pParty + 0xF4) = float_26_80421588;
    msg_ep = 0;
}


void kuribo_use_post(void* pParty) {
    *(u32*)pParty &= ~0x4000000;
    *(u32*)pParty &= ~0x2000000;
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
    extern s32 partyGetAppearPos3(void* party, void* pos);
    extern void* evtEntryType(void* evt, s32 priority, u32 flags, u32 type);
    extern void marioStSystemLevel(s32 level);
    extern void* fbatNpcTalkCheck(void);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partyChgMot(void* party, s32 motion);
    extern void partyChkGnd(void* party);
    extern const f32 float_125_80421578;
    extern const f32 float_150_8042157c;
    extern const f32 float_0_80421580;
    extern u32 kurio_msg_evt[];

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


u8 kuribo_bye(void* pParty) {
    extern void partyGoodbyeInit(void* party);
    extern s32 partyGoodbyeMain(void* party);

    if ((*(u32*)((s32)pParty + 8) & 4) != 0) {
        *(u32*)((s32)pParty + 8) &= ~4;
        partyGoodbyeInit(pParty);
    }
    if (partyGoodbyeMain(pParty) != 0) {
        partyChgRunMode(pParty, 0xE);
    }
}

const char str_msg_kuri_map_802cb8f0[] = "msg_kuri_map";
const char str_MOBJ_Lv1Block_802cb900[] = "MOBJ_Lv1Block";
const char str_obj_hlp_block_y_0_802cb910[] = "obj_hlp_block_y_0";
const char str_obj_hlp_block_y_1_802cb924[] = "obj_hlp_block_y_1";
const char str_MOBJ_Lv1BigBlock_802cb938[] = "MOBJ_Lv1BigBlock";
const char str_MOBJ_Lv1BigBigBlock_802cb94c[] = "MOBJ_Lv1BigBigBlock";
const char str_obj_hlp_block_y_2_802cb960[] = "obj_hlp_block_y_2";
const char str_obj_hlp_block_y_3_802cb974[] = "obj_hlp_block_y_3";
const char str_MOBJ_Lv2Block_802cb988[] = "MOBJ_Lv2Block";
const char str_MOBJ_Lv2BigBlock_802cb998[] = "MOBJ_Lv2BigBlock";
const char str_obj_hlp_block_s_0_802cb9ac[] = "obj_hlp_block_s_0";
const char str_obj_hlp_block_s_1_802cb9c0[] = "obj_hlp_block_s_1";
const char str_obj_hlp_block_s_2_802cb9d4[] = "obj_hlp_block_s_2";
const char str_MOBJ_Lv2BigBigBlock_802cb9e8[] = "MOBJ_Lv2BigBigBlock";
const char str_obj_hlp_block_s_3_802cb9fc[] = "obj_hlp_block_s_3";
const char str_MOBJ_Lv3BigBigBlock_802cba10[] = "MOBJ_Lv3BigBigBlock";
const char str_MOBJ_Lv3BigBlock_802cba24[] = "MOBJ_Lv3BigBlock";
const char str_MOBJ_Lv3Block_802cba38[] = "MOBJ_Lv3Block";
const char str_obj_hlp_redyellow_bl_802cba48[] = "obj_hlp_redyellow_block";
const char str_MOBJ_Block_802cba60[] = "MOBJ_Block";
const char str_MOBJ_PinkBlock_802cba6c[] = "MOBJ_PinkBlock";
const char str_obj_hlp_su_block_802cba7c[] = "obj_hlp_su_block";
const char str_MOBJ_PowerUpBlock_802cba90[] = "MOBJ_PowerUpBlock";
const char str_obj_hlp_shine_802cbaa4[] = "obj_hlp_shine";
const char str_MOBJ_HatenaBlock_802cbab4[] = "MOBJ_HatenaBlock";
const char str_obj_hlp_hatena_802cbac8[] = "obj_hlp_hatena";
const char str_MOBJ_BadgeBlock_802cbad8[] = "MOBJ_BadgeBlock";
const char str_obj_hlp_badge_hatena_802cbae8[] = "obj_hlp_badge_hatena";
const char str_MOBJ_green_FlowBlock_802cbb00[] = "MOBJ_green_FlowBlock";
const char str_MOBJ_green_big_FlowB_802cbb18[] = "MOBJ_green_big_FlowBlock";
const char str_MOBJ_green_big02_Flo_802cbb34[] = "MOBJ_green_big02_FlowBlock";
const char str_MOBJ_purple_FlowBloc_802cbb50[] = "MOBJ_purple_FlowBlock";
const char str_MOBJ_purple_big_Flow_802cbb68[] = "MOBJ_purple_big_FlowBlock";
const char str_MOBJ_purple_big02_Fl_802cbb84[] = "MOBJ_purple_big02_FlowBlock";
const char str_MOBJ_orange_FlowBloc_802cbba0[] = "MOBJ_orange_FlowBlock";
const char str_MOBJ_orange_big_Flow_802cbbb8[] = "MOBJ_orange_big_FlowBlock";
const char str_MOBJ_orange_big02_Fl_802cbbd4[] = "MOBJ_orange_big02_FlowBlock";
const char str_obj_hlp_huyuu_802cbbf0[] = "obj_hlp_huyuu";
const char str_MOBJ_green_FlowBlock_802cbc00[] = "MOBJ_green_FlowBlockSwitch";
const char str_MOBJ_purple_FlowBloc_802cbc1c[] = "MOBJ_purple_FlowBlockSwitch";
const char str_MOBJ_orange_FlowBloc_802cbc38[] = "MOBJ_orange_FlowBlockSwitch";
const char str_obj_hlp_huyuu_switch_802cbc54[] = "obj_hlp_huyuu_switch";
const char str_MOBJ_BlueTornadoSwit_802cbc6c[] = "MOBJ_BlueTornadoSwitch";
const char str_MOBJ_RedTornadoSwitc_802cbc84[] = "MOBJ_RedTornadoSwitch";
const char str_obj_hlp_big_switch_1_802cbc9c[] = "obj_hlp_big_switch_1";
const char str_obj_hlp_big_switch_2_802cbcb4[] = "obj_hlp_big_switch_2";
const char str_MOBJ_TreasureBox_802cbccc[] = "MOBJ_TreasureBox";
const char str_MOBJ_GrayTreasureBox_802cbce0[] = "MOBJ_GrayTreasureBox";
const char str_MOBJ_BigTreasureBox_802cbcf8[] = "MOBJ_BigTreasureBox";
const char str_obj_hlp_takara_1_802cbd0c[] = "obj_hlp_takara_1";
const char str_obj_hlp_takara_0_802cbd20[] = "obj_hlp_takara_0";
const char str_MOBJ_BlackTreasureBo_802cbd34[] = "MOBJ_BlackTreasureBox";
const char str_obj_hlp_blackbox_1_802cbd4c[] = "obj_hlp_blackbox_1";
const char str_obj_hlp_blackbox_0_802cbd60[] = "obj_hlp_blackbox_0";
const char str_MOBJ_RedJumpStand_802cbd74[] = "MOBJ_RedJumpStand";
const char str_MOBJ_RedJumpStand2_802cbd88[] = "MOBJ_RedJumpStand2";
const char str_MOBJ_BlueJumpStand_802cbd9c[] = "MOBJ_BlueJumpStand";
const char str_MOBJ_BlueJumpStand2_802cbdb0[] = "MOBJ_BlueJumpStand2";
const char str_MOBJ_MapJumpStand_802cbdc4[] = "MOBJ_MapJumpStand";
const char str_obj_hlp_jump_802cbdd8[] = "obj_hlp_jump";
const char str_MOBJ_RedTimerSwitch_802cbde8[] = "MOBJ_RedTimerSwitch";
const char str_MOBJ_BlueTimerSwitch_802cbdfc[] = "MOBJ_BlueTimerSwitch";
const char str_obj_hlp_timer_switch_802cbe14[] = "obj_hlp_timer_switch";
const char str_MOBJ_RedSwitch_802cbe2c[] = "MOBJ_RedSwitch";
const char str_MOBJ_BlueSwitch_802cbe3c[] = "MOBJ_BlueSwitch";
const char str_MOBJ_WhiteSwitch_802cbe4c[] = "MOBJ_WhiteSwitch";
const char str_MOBJ_BlackSwitch_802cbe60[] = "MOBJ_BlackSwitch";
const char str_obj_hlp_switch_802cbe74[] = "obj_hlp_switch";
const char str_MOBJ_RideSwitchA_802cbe84[] = "MOBJ_RideSwitchA";
const char str_MOBJ_RideSwitchB_802cbe98[] = "MOBJ_RideSwitchB";
const char str_MOBJ_RideSwitchC_802cbeac[] = "MOBJ_RideSwitchC";
const char str_obj_hlp_yuka_switch_802cbec0[] = "obj_hlp_yuka_switch";
const char str_MOBJ_BreakingFloor_802cbed4[] = "MOBJ_BreakingFloor";
const char str_MOBJ_BlackBreakingFl_802cbee8[] = "MOBJ_BlackBreakingFloor";
const char str_obj_hlp_hip_attack_802cbf00[] = "obj_hlp_hip_attack";
const char str_obj_hlp_hip_attack_1_802cbf14[] = "obj_hlp_hip_attack_1";
const char str_MOBJ_BombRock_802cbf2c[] = "MOBJ_BombRock";
const char str_obj_hlp_bomb_rock_802cbf3c[] = "obj_hlp_bomb_rock";
const char str_MOBJ_Signboard_802cbf50[] = "MOBJ_Signboard";
const char str_obj_hlp_kanban_802cbf60[] = "obj_hlp_kanban";
const char str_MOBJ_Arrow_802cbf70[] = "MOBJ_Arrow";
const char str_obj_hlp_yajirushi_802cbf7c[] = "obj_hlp_yajirushi";
const char str_MOBJ_Lock_802cbf90[] = "MOBJ_Lock";
const char str_obj_hlp_lock_802cbf9c[] = "obj_hlp_lock";
const char str_MOBJ_KururinFloor_802cbfac[] = "MOBJ_KururinFloor";

const char str_party_80421570[] = "party";
const f32 float_125_80421578 = 125.0f;
const f32 float_150_8042157c = 150.0f;
const f32 float_0_80421580 = 0.0f;
const f32 float_24_80421584 = 24.0f;
const f32 float_26_80421588 = 26.0f;
const u32 gap_09_8042158C_sdata2 = 0;

u32 kurio_msg_evt[] = {
    0x0001005B, (u32)pre_kurio_use,
    0x00020018, 0xFE363C80, 0,
    0x0001005B, (u32)post_kurio_use,
    0x00000002,
    0x00000021,
    0x0005005B, (u32)evt_msg_print, 0, 0xFE363C80, 0, (u32)(void*)str_party_80421570,
    0x0001005B, (u32)post_kurio_use,
    0x00000002,
    0x00000001,
};
