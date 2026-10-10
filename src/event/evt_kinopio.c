#include "event/evt_kinopio.h"

static void* work;
static void* wp = &work;
void marioPaperOff(void);
void psndSetFlag(s32 flag);
void psndClearFlag(s32 flag);
s32 evtGetValue(EventEntry* event, s32 value);
f32 evtGetFloat(EventEntry* event, s32 value);
f64 sin(f64 value);
f64 cos(f64 value);
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_kinopio_setup(void* pEvt) {
    extern void* npcNameToPtr(s32 name);
    extern s32 animGroupBaseAsync(const char* name, s32 flags, s32 unused);
    extern char str_p_slit_802ed424[];
    extern u32 kinopio_house_talk_evt[];
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* data = (void*)evtGetValue(pEvt, args[0]);
    void* npc = npcNameToPtr(*(s32*)((s32)data + 0x54));

    if (animGroupBaseAsync(str_p_slit_802ed424, 0, 0) == 0) {
        return 0;
    }
    *(void**)((s32)npc + 0x128) = kinopio_house_talk_evt;
    *(void**)wp = data;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 msg_no(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 index = evtGetValue(pEvt, args[0]);
    char* data = *(char**)wp;
    index *= 4;
    data += index;
    {
        s32 value = *(s32*)(data + 0x58);
        evtSetValue(pEvt, args[1], value);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 power_chk(void* pEvt) {
    extern void* pouchGetPtr(void);
    extern s32 pouchGetMaxHP(void);
    extern s32 pouchGetHP(void);
    extern s32 pouchGetMaxFP(void);
    extern s32 pouchGetFP(void);
    extern s32 pouchGetMaxAP(void);
    extern s32 pouchGetAP(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* pouch = pouchGetPtr();
    s32 ok = 1;
    s32 i;

    for (i = 0; i < 2; i++) {
        if ((*(u16*)((s32)pouch + 0x0) & 1) &&
            *(s16*)((s32)pouch + 0x6) != *(s16*)((s32)pouch + 0x2)) {
            ok = 0;
            break;
        }
        if ((*(u16*)((s32)pouch + 0xE) & 1) &&
            *(s16*)((s32)pouch + 0x14) != *(s16*)((s32)pouch + 0x10)) {
            ok = 0;
            break;
        }
        if ((*(u16*)((s32)pouch + 0x1C) & 1) &&
            *(s16*)((s32)pouch + 0x22) != *(s16*)((s32)pouch + 0x1E)) {
            ok = 0;
            break;
        }
        if ((*(u16*)((s32)pouch + 0x2A) & 1) &&
            *(s16*)((s32)pouch + 0x30) != *(s16*)((s32)pouch + 0x2C)) {
            ok = 0;
            break;
        }
        pouch = (void*)((s32)pouch + 0x38);
    }
    if (pouchGetHP() == pouchGetMaxHP() && pouchGetFP() == pouchGetMaxFP() &&
        pouchGetAP() == pouchGetMaxAP() && ok != 0) {
        evtSetValue(pEvt, args[0], 1);
    } else {
        evtSetValue(pEvt, args[0], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 coin_chk(void* pEvt) {
    extern s32 pouchGetCoin(void);
    s32* args = *(s32**)((s32)pEvt + 0x18);

    if (pouchGetCoin() < *(s32*)((s32)*(void**)wp + 0x10)) {
        evtSetValue(pEvt, args[0], 1);
    } else {
        evtSetValue(pEvt, args[0], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(get_ryoukin) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x10));
    return 2;
}
USER_FUNC(get_npcname) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x54));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(get_bed) {
    s32* args = event->args;
    void* data = *(void**)wp;
    evtSetFloat(event, args[0], *(f32*)((s32)data + 0x14));
    data = *(void**)wp;
    evtSetFloat(event, args[1], *(f32*)((s32)data + 0x18));
    data = *(void**)wp;
    evtSetFloat(event, args[2], *(f32*)((s32)data + 0x1C));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(get_bedside) {
    s32* args = event->args;
    void* data = *(void**)wp;
    evtSetFloat(event, args[0], *(f32*)((s32)data + 0x20));
    data = *(void**)wp;
    evtSetFloat(event, args[1], *(f32*)((s32)data + 0x24));
    data = *(void**)wp;
    evtSetFloat(event, args[2], *(f32*)((s32)data + 0x28));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(get_beddeg) {
    void* data = *(void**)wp;
    evtSetFloat(event, event->args[0], *(f32*)((s32)data + 0x44));
    return 2;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(get_npctalk) {
    s32* args = event->args;
    void* data = *(void**)wp;
    evtSetFloat(event, args[0], *(f32*)((s32)data + 0x48));
    data = *(void**)wp;
    evtSetFloat(event, args[1], *(f32*)((s32)data + 0x4C));
    data = *(void**)wp;
    evtSetFloat(event, args[2], *(f32*)((s32)data + 0x50));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 get_cam(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* data = *(void**)wp;
    evtSetFloat(pEvt, args[0], *(f32*)((s32)data + 0x2C));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[1], *(f32*)((s32)data + 0x30));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[2], *(f32*)((s32)data + 0x34));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[3], *(f32*)((s32)data + 0x38));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[4], *(f32*)((s32)data + 0x3C));
    data = *(void**)wp;
    evtSetFloat(pEvt, args[5], *(f32*)((s32)data + 0x40));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


USER_FUNC(get_to_bed_evt) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x90));
    return 2;
}

USER_FUNC(get_from_bed_evt) {
    void* data = *(void**)wp;
    evtSetValue(event, event->args[0], *(s32*)((s32)data + 0x94));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 paper_on(s32 pEvt, s32 isFirstCall) {
    extern void* gp;
    extern void* marioGetPtr(void);
    extern void marioPaperOn(void* name);
    extern void marioChgPose(void* name);
    extern void marioChgPaper(void* name);
    extern s32 strcmp(void* a, void* b);
    extern char str_BGM_FF_GO_SLEEPING1_802ed368[];
    extern char str_M_S_1_80421f24[6];
    extern f32 float_6p2832_80421f18;
    extern f32 float_360_80421f20;
    extern f32 float_0p2_80421f2c;
    s32* args = *(s32**)(pEvt + 0x18);
    char* strings = str_BGM_FF_GO_SLEEPING1_802ed368;
    f32 angle = evtGetFloat((void*)pEvt, args[0]);
    void* mario = marioGetPtr();
    f64 rad;

    if (isFirstCall != 0) {
        marioPaperOn(strings + 0xBC);
        marioChgPose(str_M_S_1_80421f24);
        marioChgPaper(strings + 0xC4);
        *(s32*)(pEvt + 0x78) = 0;
    }
    if (*(s32*)(pEvt + 0x78) < 30) {
        if (strcmp((void*)((s32)gp + 0x12C), strings + 0x60) == 0) {
            rad = (float_6p2832_80421f18 * angle) / float_360_80421f20;
            *(f32*)((s32)mario + 0x8C) -= float_0p2_80421f2c * (f32)cos(rad);
            *(f32*)((s32)mario + 0x94) -= float_0p2_80421f2c * (f32)sin(rad);
        } else {
            rad = (float_6p2832_80421f18 * angle) / float_360_80421f20;
            *(f32*)((s32)mario + 0x8C) += float_0p2_80421f2c * (f32)cos(rad);
            *(f32*)((s32)mario + 0x94) += float_0p2_80421f2c * (f32)sin(rad);
        }
        *(s32*)(pEvt + 0x78) += 1;
    }
    *(f32*)((s32)mario + 0x1B0) = angle;
    *(f32*)((s32)mario + 0x1AC) = angle;
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
USER_FUNC(paper_off) {
    marioPaperOff();
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 offset(s32 pEvt) {
    extern void* camGetPtr(s32 id);
    extern f32 float_6p2832_80421f18;
    extern f32 float_90_80421f1c;
    extern f32 float_360_80421f20;
    s32* args = *(s32**)(pEvt + 0x18);
    f32 dist = evtGetFloat((void*)pEvt, args[0]);
    f32 angle = (float_6p2832_80421f18 * (float_90_80421f1c + *(f32*)((s32)camGetPtr(4) + 0x114))) / float_360_80421f20;

    evtSetFloat((void*)pEvt, args[1], dist * -(f32)sin(angle));
    evtSetFloat((void*)pEvt, args[2], dist * (f32)cos(angle));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 breakfast(void) {
    extern void* itemNameToPtr(void* name);
    extern void itemEntry(void* name, f32 x, f32 y, f32 z, s32 flags, s32 id, s32 unused);
    extern char str_breakfast_802ed418[];
    void* item = itemNameToPtr(str_breakfast_802ed418);
    void* data;

    if (item == NULL || (*(u16*)item & 1) == 0) {
        data = *(void**)wp;
        if (*(s32*)data != 0) {
            itemEntry(str_breakfast_802ed418, *(f32*)((s32)data + 4), *(f32*)((s32)data + 8),
                      *(f32*)((s32)data + 0xC), 0x10, -1, 0);
        }
    }
    return 2;
}

USER_FUNC(snd_off) {
    psndSetFlag(0x80);
    return 2;
}

USER_FUNC(snd_on) {
    psndClearFlag(0x80);
    return 2;
}

s32 N_cameraman_on(void) {
    extern void* npcNameToPtr(const char* name);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern const char str_kinopio_cameraman_802ed37c[];
    void* npc;

    npc = npcNameToPtr(str_kinopio_cameraman_802ed37c);
    animPoseSetMaterialFlagOn(*(s32*)((s32)npc + 0x104), 0x1800);
    return 2;
}

const char str_BGM_FF_GO_SLEEPING1_802ed368[] = "BGM_FF_GO_SLEEPING1";
const char str_kinopio_cameraman_802ed37c[] =
    "\x8F\x68\x89\xAE\x82\xCC\x83\x4A\x83\x81\x83\x89\x83\x7D\x83\x93";
const char str_c_jugem_c_802ed390[] = "c_jugem_c";
const char str_cameraman_802ed39c[] = "\x83\x4A\x83\x81\x83\x89\x83\x7D\x83\x93";
const char str_bom_02_802ed3a8[] = "bom_02";
const char str_gor_01_802ed3b0[] = "gor_01";
const char str_muj_01_802ed3b8[] = "muj_01";
const char str_nok_00_802ed3c0[] = "nok_00";
const char str_pik_04_802ed3c8[] = "pik_04";
const char str_usu_00_802ed3d0[] = "usu_00";
const char str_confetti_802ed3d8[] = "confetti";
const char str_SFX_VOICE_MARIO_FIND_802ed3e4[] = "SFX_VOICE_MARIO_FIND1";
const char str_SFX_EVT_50_HOTEL_SHU_802ed3fc[] = "SFX_EVT_50_HOTEL_SHUTTER1";
const char str_breakfast_802ed418[] = "breakfast";
const char str_p_slit_802ed424[] = "p_slit";
const char str_PM_S_3A_802ed42c[] = "PM_S_3A";

const char str_mario_80421f10[] = "mario";
const f32 float_6p2832_80421f18 = 6.2831855f;
const f32 float_90_80421f1c = 90.0f;
const f32 float_360_80421f20 = 360.0f;
const char str_M_S_1_80421f24[] = "M_S_1";
const f32 float_0p2_80421f2c = 0.2f;

/* Exact US evt_kinopio Event reconstruction dependencies.
 * Definitions are late so existing executable consumers retain their source shape. */
extern s32 evt_cam3d_evt_off();
extern s32 evt_cam3d_evt_set();
extern s32 evt_eff();
extern s32 evt_eff_fukidashi();
extern s32 evt_fade_end_wait();
extern s32 evt_fade_entry();
extern s32 evt_fade_set_spot_pos();
extern s32 evt_mario_adjust_dir();
extern s32 evt_mario_dispflag_onoff();
extern s32 evt_mario_flag_onoff();
extern s32 evt_mario_get_exparty();
extern s32 evt_mario_get_party();
extern s32 evt_mario_get_pos();
extern s32 evt_mario_goodbye_party();
extern s32 evt_mario_jump_pos();
extern s32 evt_mario_key_onoff();
extern s32 evt_mario_mov_pos2();
extern s32 evt_mario_set_dir_npc();
extern s32 evt_mario_set_party_pos();
extern s32 evt_mario_set_pos();
extern s32 evt_msg_print();
extern s32 evt_msg_print_add();
extern s32 evt_msg_print_insert();
extern s32 evt_msg_select();
extern s32 evt_npc_check_delete();
extern s32 evt_npc_entry();
extern s32 evt_npc_flag_onoff();
extern s32 evt_npc_get_position();
extern s32 evt_npc_set_position();
extern s32 evt_npc_set_tribe();
extern s32 evt_party_run();
extern s32 evt_party_set_dir_npc();
extern s32 evt_party_set_pos();
extern s32 evt_pouch_add_coin();
extern s32 evt_pouch_all_party_recovery();
extern s32 evt_pouch_check_item();
extern s32 evt_pouch_mario_recovery();
extern s32 evt_pouch_remove_item();
extern s32 evt_set_dir_to_target();
extern s32 evt_snd_bgmoff();
extern s32 evt_snd_bgmon();
extern s32 evt_snd_sfxon();
extern s32 evt_snd_sfxon_3d();
extern s32 evt_sub_get_coin();
extern s32 evt_sub_get_mapname();
extern s32 evt_win_coin_off();
extern s32 evt_win_coin_on();
extern u32 evt_50[];

/* All Events US + fresh target-object exact payload/relocation reconstruction. */
u32 kinopio_house_talk_evt[628] = {
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000000, 0x0002005B, (u32)get_ryoukin, 0xFE363C8A,
    0x0002005B, (u32)get_npcname, 0xFE363C8B, 0x0003005B, (u32)evt_win_coin_on, 0x00000000,
    0xFE363C8C, 0x0003005B, (u32)msg_no, 0x00000000, 0xFE363C80, 0x0006005B,
    (u32)evt_msg_print_insert, 0x00000000, 0xFE363C80, 0x00000000, 0xFE363C8B, 0xFE363C8A,
    0x0003005B, (u32)msg_no, 0x00000006, 0xFE363C80, 0x0003005B, (u32)evt_msg_select,
    0x00000000, 0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0003005B,
    (u32)msg_no, 0x00000002, 0xFE363C80, 0x0003005B, (u32)evt_msg_print_add, 0x00000000,
    0xFE363C80, 0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x0002005B, (u32)evt_win_coin_off,
    0xFE363C8C, 0x00000002, 0x00000021, 0x0002005B, (u32)power_chk, 0xFE363C80,
    0x00020018, 0xFE363C80, 0x00000001, 0x0003005B, (u32)msg_no, 0x00000001,
    0xFE363C80, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, 0xFE363C80, 0x0003005B,
    (u32)msg_no, 0x00000006, 0xFE363C80, 0x0003005B, (u32)evt_msg_select, 0x00000000,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001, 0x0003005B, (u32)msg_no,
    0x00000002, 0xFE363C80, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, 0xFE363C80,
    0x0002005B, (u32)evt_mario_key_onoff, 0x00000001, 0x0002005B, (u32)evt_win_coin_off, 0xFE363C8C,
    0x00000002, 0x00000021, 0x00000021, 0x00020032, 0xFBD3E280, 0x00000000,
    0x0003005B, (u32)evt_pouch_check_item, 0x000000A4, 0xFE363C80, 0x0002001B, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)msg_no, 0x00000007, 0xFE363C80, 0x0003005B,
    (u32)evt_msg_print_add, 0x00000000, 0xFE363C80, 0x0003005B, (u32)msg_no, 0x00000008,
    0xFE363C80, 0x0003005B, (u32)evt_msg_select, 0x00000000, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000000, 0x00020032, 0xFBD3E280, 0x00000001, 0x0003005B,
    (u32)evt_pouch_remove_item, 0x000000A4, 0xFE363C80, 0x00000021, 0x00000021, 0x00020018,
    0xFBD3E280, 0x00000000, 0x0002005B, (u32)coin_chk, 0xFE363C80, 0x00020018,
    0xFE363C80, 0x00000001, 0x0003005B, (u32)msg_no, 0x00000004, 0xFE363C80,
    0x0003005B, (u32)evt_msg_print_add, 0x00000000, 0xFE363C80, 0x0002005B, (u32)evt_mario_key_onoff,
    0x00000001, 0x0002005B, (u32)evt_win_coin_off, 0xFE363C8C, 0x00000002, 0x00000021,
    0x0002005B, (u32)get_ryoukin, 0xFE363C8A, 0x00020037, 0xFE363C8A, 0xFFFFFFFF,
    0x0002005B, (u32)evt_pouch_add_coin, 0xFE363C8A, 0x0001000A, 0x000003E8, 0x00000021,
    0x0002005B, (u32)evt_win_coin_off, 0xFE363C8C, 0x0003005B, (u32)msg_no, 0x00000003,
    0xFE363C80, 0x0003005B, (u32)evt_msg_print_add, 0x00000000, 0xFE363C80, 0x0002005B,
    (u32)get_npcname, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80,
    0x00000020, 0x0002005B, (u32)evt_mario_get_party, 0xFE363C8B, 0x0002005B, (u32)evt_mario_get_exparty,
    0xFE363C8C, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C8D, 0xFE363C8E,
    0xFE363C8F, 0x0002005B, (u32)evt_mario_goodbye_party, 0x00000001, 0x0002005B, (u32)evt_mario_goodbye_party,
    0x00000000, 0x0002005B, (u32)get_to_bed_evt, 0xFE363C80, 0x00020018, 0xFE363C80,
    0x00000000, 0x0000006B, 0x0001000A, 0x000003E8, 0x0007005B, (u32)get_cam,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0xFE363C85,
    0x0009005B, (u32)evt_cam3d_evt_set, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0xFE363C85, 0x00000000, 0x0000000B, 0x0000006D, 0x0004005B,
    (u32)get_bedside, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_mario_mov_pos2,
    0xFE363C80, 0xFE363C82, 0xF24BE280, 0x00000020, 0x0001006C, 0xFE363C80,
    0x0001005E, 0xFE363C80, 0x0000006D, 0x00010005, 0x00000000, 0x0002006A,
    0xFE363C80, 0xFE363C81, 0x00020018, 0xFE363C81, 0x00000000, 0x00000007,
    0x00000021, 0x00010009, 0x00000001, 0x00000006, 0x0007005B, (u32)get_cam,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C83, 0xFE363C84, 0xFE363C85,
    0x0009005B, (u32)evt_cam3d_evt_set, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C83,
    0xFE363C84, 0xFE363C85, 0x00000000, 0x0000000B, 0x0001000A, 0x000001F4,
    0x00000021, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0006005B, (u32)evt_snd_sfxon_3d, 0x0002009E, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00000000, 0x0003005B, (u32)evt_mario_flag_onoff, 0x00000001, 0x00000700,
    0x0004005B, (u32)get_bed, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0007005B,
    (u32)evt_mario_jump_pos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0xF250BA80, 0x00000000,
    0xFFFFFC18, 0x0004005B, (u32)evt_mario_set_pos, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0001005B, (u32)evt_mario_adjust_dir, 0x0001000A, 0x000000C8, 0x0003005B, (u32)evt_mario_dispflag_onoff,
    0x00000001, 0x00001000, 0x0001006C, 0xFE363C8A, 0x0002005B, (u32)get_beddeg,
    0xFE363C80, 0x0002005B, (u32)paper_on, 0xFE363C80, 0x0000006D, 0x0000006B,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)evt_snd_sfxon_3d, 0x000203BA, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0001000A, 0x000000C8, 0x0006005B, (u32)evt_snd_sfxon_3d, 0x000203BB,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000000, 0x0000006D, 0x0001005B,
    (u32)snd_off, 0x0002005B, (u32)evt_snd_bgmoff, 0x00000400, 0x0003005B, (u32)evt_snd_bgmon,
    0x00000001, (u32)(char*)str_BGM_FF_GO_SLEEPING1_802ed368, 0x0006005B, (u32)evt_fade_entry, 0x00000004, 0x00001770,
    0x00000000, 0x00000000, 0x00000000, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_fade_set_spot_pos, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0001005B, (u32)evt_fade_end_wait, 0x0006005B, (u32)evt_fade_entry,
    0x0000000C, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0001005B,
    (u32)evt_fade_end_wait, 0x0001005B, (u32)evt_pouch_mario_recovery, 0x0001005B, (u32)evt_pouch_all_party_recovery, 0x0001005B,
    (u32)breakfast, 0x0001000A, 0x00000BB8, 0x0001005F, 0xFE363C8A, 0x0001005B,
    (u32)paper_off, 0x0003005B, (u32)evt_mario_dispflag_onoff, 0x00000000, 0x00001000, 0x00020019,
    0xFE363C8B, 0x00000000, 0x0004005B, (u32)get_bed, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0006005B, (u32)evt_mario_set_party_pos, 0x00000000, 0xFE363C8B, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000021, 0x00020019, 0xFE363C8C, 0x00000000,
    0x0004005B, (u32)get_bed, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B,
    (u32)offset, 0xF24AF280, 0xFE363C83, 0xFE363C84, 0x0002003A, 0xFE363C80,
    0xFE363C83, 0x0002003A, 0xFE363C82, 0xFE363C84, 0x0006005B, (u32)evt_mario_set_party_pos,
    0x00000001, 0xFE363C8C, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000021,
    0x0003005B, (u32)evt_mario_flag_onoff, 0x00000000, 0x00000700, 0x0004005B, (u32)get_bedside,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_mario_set_pos, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x0002005B, (u32)get_npcname, 0xFE363C80, 0x0003005B,
    (u32)evt_set_dir_to_target, 0xFE363C80, (u32)(char*)str_mario_80421f10, 0x0002005B, (u32)evt_mario_set_dir_npc, 0xFE363C80,
    0x0003005B, (u32)evt_party_set_dir_npc, 0x00000000, 0xFE363C80, 0x0003005B, (u32)evt_party_set_dir_npc,
    0x00000001, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, 0xFE363C80,
    0x00008000, 0x0003005B, (u32)evt_cam3d_evt_off, 0x00000000, 0x0000000B, 0x00010009,
    0x00000001, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, 0xFE363C80, 0x00008000,
    0x0002005B, (u32)evt_snd_bgmoff, 0x00000201, 0x0003005B, (u32)evt_snd_bgmon, 0x00000120,
    0x00000000, 0x0001005B, (u32)snd_on, 0x00020035, 0xF5DE0181, 0x00000001,
    0x0002001B, 0xF5DE0181, 0x00000064, 0x00020032, 0xF5DE0181, 0x00000064,
    0x00000021, 0x00020018, 0xF5DE0181, 0x00000032, 0x0001005E, (u32)evt_50,
    0x0002005B, (u32)get_npcname, 0xFE363C80, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000,
    0xFE363C80, 0x00000020, 0x00000002, 0x00000021, 0x0001005B, (u32)evt_mario_adjust_dir,
    0x0006005B, (u32)evt_fade_entry, 0x0000000B, 0x000003E8, 0x00000000, 0x00000000,
    0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0002005B, (u32)get_from_bed_evt, 0xFE363C80,
    0x00020019, 0xFE363C80, 0x00000000, 0x0001006C, 0xFE363C8A, 0x0001005E,
    0xFE363C80, 0x0000006D, 0x00010005, 0x00000000, 0x0002006A, 0xFE363C8A,
    0xFE363C8B, 0x00020018, 0xFE363C8B, 0x00000000, 0x00000007, 0x00000021,
    0x00010009, 0x00000001, 0x00000006, 0x00000021, 0x0002005B, (u32)evt_party_run,
    0x00000000, 0x0002005B, (u32)evt_party_run, 0x00000001, 0x0004005B, (u32)get_npctalk,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_mario_mov_pos2, 0xFE363C80,
    0xFE363C82, 0xF24BE280, 0x0001005B, (u32)evt_fade_end_wait, 0x0002005B, (u32)get_npcname,
    0xFE363C8B, 0x0002005B, (u32)evt_mario_set_dir_npc, 0xFE363C8B, 0x0003005B, (u32)evt_party_set_dir_npc,
    0x00000000, 0xFE363C8B, 0x0002005B, (u32)get_npcname, 0xFE363C8B, 0x0003005B,
    (u32)msg_no, 0x00000005, 0xFE363C80, 0x0005005B, (u32)evt_msg_print, 0x00000000,
    0xFE363C80, 0x00000000, 0xFE363C8B, 0x0002005B, (u32)get_npcname, 0xFE363C80,
    0x0004005B, (u32)evt_npc_flag_onoff, 0x00000000, 0xFE363C80, 0x00000020, 0x0002005B,
    (u32)evt_mario_key_onoff, 0x00000001, 0x00000002, 0x00000001,
};

u32 evt_50[375] = {
    0x0002005B, (u32)get_npcname, 0xFE363C8A, 0x0005005B, (u32)evt_npc_get_position, 0xFE363C8A,
    0xFE363C8D, 0xFE363C8E, 0xFE363C8F, 0x0003005B, (u32)evt_npc_entry, (u32)(char*)str_kinopio_cameraman_802ed37c,
    (u32)(char*)str_c_jugem_c_802ed390, 0x0003005B, (u32)evt_npc_set_tribe, (u32)(char*)str_kinopio_cameraman_802ed37c, (u32)(char*)str_cameraman_802ed39c, 0x0001005B,
    (u32)N_cameraman_on, 0x0004005B, (u32)evt_npc_flag_onoff, 0x00000001, (u32)(char*)str_kinopio_cameraman_802ed37c, 0x00008000,
    0x0002005B, (u32)evt_sub_get_mapname, 0xFE363C80, 0x0002000C, 0xFE363C80, (u32)(char*)str_bom_02_802ed3a8,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_kinopio_cameraman_802ed37c, 0xFFFFFE93, 0x0000001E, 0xFFFFFF56,
    0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_gor_01_802ed3b0, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_kinopio_cameraman_802ed37c, 0x000000A8, 0x0000009A, 0xFFFFFF1B, 0x00000021, 0x0002000C,
    0xFE363C80, (u32)(char*)str_muj_01_802ed3b8, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_kinopio_cameraman_802ed37c, 0x0000010E,
    0x00000014, 0xFFFFFF6F, 0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_nok_00_802ed3c0,
    0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_kinopio_cameraman_802ed37c, 0xFFFFFE7F, 0x00000019, 0xFFFFFF6F,
    0x00000021, 0x0002000C, 0xFE363C80, (u32)(char*)str_pik_04_802ed3c8, 0x0005005B, (u32)evt_npc_set_position,
    (u32)(char*)str_kinopio_cameraman_802ed37c, 0x00000136, 0x00000531, 0xFFFFFF9C, 0x0005005B, (u32)evt_npc_set_position,
    0xFE363C8A, 0x00000136, 0x00000531, 0xFFFFFFBF, 0x00000021, 0x0002000C,
    0xFE363C80, (u32)(char*)str_usu_00_802ed3d0, 0x0005005B, (u32)evt_npc_set_position, (u32)(char*)str_kinopio_cameraman_802ed37c, 0x0000010C,
    0x00000000, 0xFFFFFF2E, 0x00000021, 0x0003005B, (u32)evt_set_dir_to_target, (u32)(char*)str_kinopio_cameraman_802ed37c,
    (u32)(char*)str_mario_80421f10, 0x0003005B, (u32)evt_set_dir_to_target, 0xFE363C8A, (u32)(char*)str_mario_80421f10, 0x0002005B,
    (u32)evt_mario_set_dir_npc, (u32)(char*)str_kinopio_cameraman_802ed37c, 0x0003005B, (u32)evt_party_set_dir_npc, 0x00000000, (u32)(char*)str_kinopio_cameraman_802ed37c,
    0x0003005B, (u32)msg_no, 0x00000009, 0xFE363C80, 0x0005005B, (u32)evt_msg_print,
    0x00000000, 0xFE363C80, 0x00000000, 0xFE363C8A, 0x0004005B, (u32)get_bed,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_mario_set_pos, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C82, 0x00000005, 0x0005005B,
    (u32)evt_party_set_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0006005B,
    (u32)evt_fade_entry, 0x00000009, 0x000003E8, 0x00000000, 0x00000000, 0x00000000,
    0x0001005B, (u32)evt_fade_end_wait, 0x0001000A, 0x000001F4, 0x0005005B, (u32)evt_mario_get_pos,
    0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81,
    0x00000064, 0x000F005B, (u32)evt_eff, 0x00000000, (u32)(char*)str_confetti_802ed3d8, 0x00000003,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00000258, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0000006B,
    0x0005005B, (u32)evt_mario_get_pos, 0x00000000, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0006005B, (u32)evt_snd_sfxon_3d, (u32)(char*)str_SFX_VOICE_MARIO_FIND_802ed3e4, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x00000000, 0x0000006D, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x000C005B, (u32)evt_eff_fukidashi, 0x00000003,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0001000A, 0x000003E8,
    0x0003005B, (u32)msg_no, 0x0000000A, 0xFE363C80, 0x0005005B, (u32)evt_msg_print,
    0x00000000, 0xFE363C80, 0x00000000, 0xFE363C8A, 0x0004005B, (u32)get_bedside,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0004005B, (u32)evt_mario_mov_pos2, 0xFE363C80,
    0xFE363C82, 0xF24BE280, 0x0004005B, (u32)get_npctalk, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x0004005B, (u32)evt_mario_mov_pos2, 0xFE363C80, 0xFE363C82, 0xF24BE280,
    0x0002005B, (u32)evt_mario_set_dir_npc, 0xFE363C8A, 0x0003005B, (u32)msg_no, 0x0000000B,
    0xFE363C80, 0x0005005B, (u32)evt_msg_print, 0x00000000, 0xFE363C80, 0x00000000,
    0xFE363C8A, 0x0002005B, (u32)evt_mario_set_dir_npc, (u32)(char*)str_kinopio_cameraman_802ed37c, 0x0003005B, (u32)evt_party_set_dir_npc,
    0x00000000, (u32)(char*)str_kinopio_cameraman_802ed37c, 0x0001000A, 0x0000012C, 0x0003005B, (u32)evt_snd_sfxon,
    (u32)(char*)str_SFX_EVT_50_HOTEL_SHU_802ed3fc, 0x00000000, 0x0006005B, (u32)evt_fade_entry, 0x00000008, 0x00000096,
    0x000000FF, 0x000000FF, 0x000000FF, 0x0005005B, (u32)evt_mario_get_pos, 0x00000000,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x00020035, 0xFE363C81, 0x00000028,
    0x0004005B, (u32)evt_fade_set_spot_pos, 0xFE363C80, 0xFE363C81, 0xFE363C82, 0x0001005B,
    (u32)evt_fade_end_wait, 0x0006005B, (u32)evt_fade_entry, 0x00000007, 0x00000096, 0x000000FF,
    0x000000FF, 0x000000FF, 0x0001005B, (u32)evt_fade_end_wait, 0x0003005B, (u32)msg_no,
    0x0000000C, 0xFE363C80, 0x0005005B, (u32)evt_msg_print, 0x00000000, 0xFE363C80,
    0x00000000, 0xFE363C8A, 0x0002005B, (u32)evt_mario_set_dir_npc, 0xFE363C8A, 0x0003005B,
    (u32)evt_party_set_dir_npc, 0x00000000, 0xFE363C8A, 0x0001000A, 0x0000012C, 0x0002005B,
    (u32)evt_sub_get_coin, 0x000000C8, 0x0003005B, (u32)msg_no, 0x0000000D, 0xFE363C80,
    0x0005005B, (u32)evt_msg_print, 0x00000000, 0xFE363C80, 0x00000000, 0xFE363C8A,
    0x0006005B, (u32)evt_fade_entry, 0x0000000A, 0x000003E8, 0x00000000, 0x00000000,
    0x00000000, 0x0001005B, (u32)evt_fade_end_wait, 0x0002005B, (u32)evt_npc_check_delete, (u32)(char*)str_kinopio_cameraman_802ed37c,
    0x0005005B, (u32)evt_npc_set_position, 0xFE363C8A, 0xFE363C8D, 0xFE363C8E, 0xFE363C8F,
    0x0003005B, (u32)evt_set_dir_to_target, 0xFE363C8A, (u32)(char*)str_mario_80421f10, 0x0006005B, (u32)evt_fade_entry,
    0x00000009, 0x000003E8, 0x00000000, 0x00000000, 0x00000000, 0x0001005B,
    (u32)evt_fade_end_wait, 0x0002005B, (u32)evt_party_run, 0x00000000, 0x0002005B, (u32)evt_mario_key_onoff,
    0x00000001, 0x00000002, 0x00000001,
};
