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

