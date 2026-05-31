#include "event/evt_nannpc.h"

extern void extLoadShadowMtx(void*);
extern u8 extLoadShadowRenderMode(void);
extern u8 extLoadShadowVertex(void);
extern u8 extLoadShadowTev(void);
extern u8 extDrawShadow(void);

typedef struct NanNpcSortEntry {
    f32 unk0;
    f32 z;
} NanNpcSortEntry;


u8 nanNPCOption(void) {
    return 0;
}


u8 evt_nannpc_jump_position(void) {
    return 0;
}


s32 evt_nannpc_init(void* param_1, int param_2) {
    return 0;
}


u8 nannpc_ext_disp(void) {
    return 0;
}


u8 evt_nannpc_move_position2(void) {
    return 0;
}


s32 evt_nannpc_entry(void* param_1) {
    return 0;
}


u8 nannpc_ext_main_sub_fast(void) {
    return 0;
}


s32 evt_nannpc_ppflag_onoff(void* pEvt) {
    return 0;
}


s32 evt_nannpc_flag_onoff(void* pEvt) {
    return 0;
}


s32 evt_nannpc_set_animtbl(int param_1) {
    return 0;
}


s32 evt_nannpc_set_color(int param_1) {
    return 0;
}


s32 evt_nannpc_set_position(void* pEvt) {
    return 0;
}


s32 evt_nannpc_set_dir(void* pEvt) {
    return 0;
}


s32 evt_nannpc_set_hosei_position(void* pEvt) {
    return 0;
}


s32 evt_nannpc_get_position(void* evtEntry) {
    return 0;
}


s32 evt_nannpc_set_work(void* pEvt) {
    return 0;
}


s32 evt_nannpc_set_shadow_position(int param_1) {
    return 0;
}


s32 evt_nannpc_get_work(void* pEvt) {
    return 0;
}


s32 evt_nannpc_set_size(int param_1) {
    return 0;
}


s32 evt_nannpc_set_func(void* pEvt) {
    return 0;
}


u8 nannpc_ext_init(void) {
    return 0;
}


s32 evt_nannpc_set_subfunc(void* pEvt) {
    return 0;
}


u8 nannpc_ext_dispent(void) {
    return 0;
}


s32 evt_nannpcwork_flag_onoff(void* pEvt) {
    return 0;
}


u8 nannpc_ext_main_sub(void) {
    return 0;
}


u8 nannpc_ext_main(void) {
    return 0;
}


u8 evt_nannpc_set_animtbl_sub(int param_1, short* param_2) {
    s32 count = 0;

    *(short**)(param_1 + 0x58) = param_2;
loop:
    if (*param_2 == -1) {
        goto done;
    }
    param_2++;
    count++;
    goto loop;
done:
    *(s16*)(param_1 + 0x5C) = count;
    if (*(s16*)(param_1 + 0x5C) == 0) {
        *(s32*)(param_1 + 0x58) = 0;
    }
}


u8 nannpc_ext_shadow_disp(void) {
    static f32 mt[3][4];

    extLoadShadowMtx(mt);
    extLoadShadowRenderMode();
    extLoadShadowVertex();
    extLoadShadowTev();
    return extDrawShadow();
}


s32 nannpc_zcompare(NanNpcSortEntry* param_1, NanNpcSortEntry* param_2) {
    if (param_1->z < param_2->z) {
        return 1;
    }
    if (param_1->z > param_2->z) {
        return -1;
    }
    return 0;
}
