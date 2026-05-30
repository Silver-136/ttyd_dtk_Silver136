#include "event/evt_party.h"
#include "event/evt_cmd.h"

extern s32 partyCtrlNo;
extern f32 float_0_80421d4c;

s32 evtGetValue(EventEntry* event, s32 value);
f32 evtSetFloat(EventEntry* event, s32 target, f32 value);
void* partyGetPtr(s32 id);
void partyInitCamId(void* party);
void partySetCamId(void* party, s32 camId);
void partyChgPoseId(void* party, s32 poseId);
void partyCtrlOff(void);
void partyCtrlOn(void);

static void* evt_party_get_arg_party(EventEntry* event) {
    s32* args = event->args;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    return partyGetPtr(partyId);
}

USER_FUNC(evt_party_wait_landon) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    return ((*(u32*)party >> 1) & 1) != 0 ? 2 : 0;
}

USER_FUNC(evt_party_sleep_off) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        partyChgPoseId(party, 1);
        *(u32*)party &= ~0x400000;
    }
    return 2;
}

USER_FUNC(evt_party_sleep_on) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        partyChgPoseId(party, 6);
        *(u32*)party |= 0x400000;
    }
    return 2;
}

USER_FUNC(evt_party_set_breed_pose) {
    s32* args = event->args;
    void* party;
    s32 poseId;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party != 0) {
        poseId = evtGetValue(event, args[1]);
        partyChgPoseId(party, poseId);
    }
    return 2;
}

USER_FUNC(evt_party_get_dispdir) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        evtSetFloat(event, args[1], float_0_80421d4c);
    } else {
        evtSetFloat(event, args[1], *(f32*)((s32)party + 0x10C));
    }
    return 2;
}

USER_FUNC(evt_party_set_camid) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    partySetCamId(party, evtGetValue(event, args[1]));
    return 2;
}

USER_FUNC(evt_party_init_camid) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    partyInitCamId(party);
    return 2;
}

USER_FUNC(evt_party_cont_onoff) {
    s32* args = event->args;
    void* party;
    s32 partyId = 1;
    if (args[0] == 0) {
        partyId = partyCtrlNo;
    }
    party = partyGetPtr(partyId);
    if (party == 0) {
        return 2;
    }
    if (evtGetValue(event, args[1]) == 0) {
        partyCtrlOff();
    } else {
        partyCtrlOn();
    }
    return 2;
}


u8 evt_party_jump_pos(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_party_force_reset_outofscreen(void* pEvt) {
    return 0;
}


u8 evt_party_set_dir(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_party_move_pos2(s32 pEvt, s32 param_2) {
    return 0;
}


u8 evt_party_move_pos(s32 pEvt, s32 param_2) {
    return 0;
}


s32 evt_party_nokotaro_hold_cancel(void* pEvt, int param_2) {
    return 0;
}


u8 evt_party_set_dir_pos(void) {
    return 0;
}


s32 evt_party_yoshi_ride(void* pEvt, int param_2) {
    return 0;
}


u8 evt_party_set_dir_npc(s32 pEvt, s32 param_2) {
    return 0;
}


u32 evt_party_nokotaro_kick_hold(void* pEvt, int param_2) {
    return 0;
}


s32 evt_party_cloud_breathout(void* pEvt, int param_2) {
    return 0;
}


s32 evt_party_yoshi_fly(void* pEvt, int param_2) {
    return 0;
}


s32 evt_party_nokotaro_kick(void* pEvt, int param_2) {
    return 0;
}


s32 evt_party_get_pos(void* pEvt) {
    return 0;
}


s32 evt_party_vivian_hold(void* pEvt, int param_2) {
    return 0;
}


s32 evt_party_thunders_use(void* pEvt, int param_2) {
    return 0;
}


s32 N_evt_party_cloud_lock_animations_on_off(void* pEvt, int param_2) {
    return 0;
}


s32 evt_party_dispflg_onoff(void* pEvt) {
    return 0;
}


s32 evt_party_flg_onoff(void* pEvt) {
    return 0;
}


s32 evt_party_set_homing_dist(void* pEvt) {
    return 0;
}


s32 evt_party_set_hosei_xyz(void* pEvt) {
    return 0;
}


s32 evt_party_set_pos(void* pEvt) {
    return 0;
}


s32 evt_party_outofscreen(void* pEvt) {
    return 0;
}


s32 evt_party_get_name_hitobj_ride(void* pEvt) {
    return 0;
}


s32 evt_party_get_name_hitobj_head(void* pEvt) {
    return 0;
}


u32 evt_party_vivian_unhold(void* pEvt, int param_2) {
    return 0;
}


s32 evt_party_nokotaro_get_status(void* pParty) {
    return 0;
}


s32 L_evt_party_dokan(void* pEvt) {
    return 0;
}


s32 evt_party_get_status(void* pEvt) {
    return 0;
}


s32 evt_party_set_pose(void* pEvt) {
    return 0;
}


s32 evt_party_move_beside_mario(void* pEvt) {
    return 0;
}


s32 evt_party_move_behind_mario(void* pEvt) {
    return 0;
}


s32 evt_party_set_dispdir(int param_1) {
    return 0;
}


s32 L_evt_party_vivian_tail(void* pEvt) {
    return 0;
}


s32 evt_party_run(void* pEvt) {
    return 0;
}


s32 evt_party_stop(void* pEvt) {
    return 0;
}
