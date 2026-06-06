#include "event/evt_shop.h"

extern void* _wp;
extern void* gp;
extern char mapname;
extern void evtSetValue();
extern s32 evtGetValue();
extern void* pouchGetPtr();
extern s32 pouchGetHaveItemCnt();
extern s32 pouchGetKeepItemCnt();
extern s32 pouchGetEmptyHaveItemCnt();
extern s32 pouchGetEmptyKeepItemCnt();
extern s32 strcmp();
extern void winMgrOpen();
extern void winMgrClose();
extern s32 winMgrAction();
extern void winMgrHelpDraw();
extern void winMgrHelpMain();
extern void* msgSearch();
extern u8 itemDataTable[];


u8 list_disp(void) {
    return 0;
}


s32 evt_shop_main_func(void* pEvt) {
    return 0;
}


u8 evt_shop_setup(s32 pEvt) {
    return 0;
}


u8 name_disp(void) {
    return 0;
}


u8 title_disp(void) {
    return 0;
}


u8 point_disp(u32* param_1) {
    return 0;
}


s32 chk_shop_point(void* pEvt) {
    return 0;
}


s32 add_shop_point(void* pEvt) {
    return 0;
}


s32 shop_point_item(void* pEvt) {
    extern s32 shopPointList[];
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 point = evtGetValue(pEvt, args[0]);
    s32* list;

    pouchGetPtr();
    list = shopPointList;
    while (list[0] != -1) {
        if (list[0] == point) {
            evtSetValue(pEvt, args[1], list[1]);
            evtSetValue(pEvt, args[2], msgSearch(*(void**)(itemDataTable + list[1] * 0x28 + 4)));
            return 2;
        }
        list += 2;
    }
    evtSetValue(pEvt, args[1], -1);
    evtSetValue(pEvt, args[2], -1);
    return 2;
}

u32 disp_list(void* evt, int param_2) {
    extern u8 list_desc[];
    extern void* winMgrEntry(void* desc);
    extern void winMgrCloseAutoDelete(void* win);
    extern s32 keyGetButtonTrg(s32 player);
    extern void psndSFXOn(s32 id);

    if (param_2 != 0) {
        *(void**)((s32)evt + 0x78) = winMgrEntry(list_desc);
        *(void**)((s32)evt + 0x7C) = winMgrEntry(list_desc + 0x28);
        *(void**)((s32)evt + 0x80) = winMgrEntry(list_desc + 0x50);
        winMgrOpen(*(void**)((s32)evt + 0x78));
        winMgrOpen(*(void**)((s32)evt + 0x7C));
        winMgrOpen(*(void**)((s32)evt + 0x80));
        psndSFXOn(0x2A);
    }
    if (keyGetButtonTrg(0) & 0x200) {
        winMgrCloseAutoDelete(*(void**)((s32)evt + 0x78));
        winMgrCloseAutoDelete(*(void**)((s32)evt + 0x7C));
        winMgrCloseAutoDelete(*(void**)((s32)evt + 0x80));
        psndSFXOn(0x2B);
        return 1;
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 get_value(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    void* work = (void*)evtGetValue(pEvt, args[0]);
    s32 index;
    void* items;

    index = *(s32*)((s32)work + 0x2C) * 8;
    items = *(void**)((s32)work + 8);
    evtSetValue(pEvt, args[1], msgSearch(*(void**)(itemDataTable + *(s32*)((s32)items + index) * 0x28 + 4)));
    index = *(s32*)((s32)work + 0x2C) * 8;
    items = *(void**)((s32)work + 8);
    evtSetValue(pEvt, args[2], *(s32*)((s32)items + index));
    index = *(s32*)((s32)work + 0x2C) * 8;
    items = *(void**)((s32)work + 8);
    evtSetValue(pEvt, args[3], *(s32*)((s32)items + index + 4));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 name_price(int param_1) {
    s32* args = *(s32**)(param_1 + 0x18);
    s32 item = evtGetValue((void*)param_1, args[0]);
    s32 offset = item * 0x28;

    evtSetValue((void*)param_1, args[1], msgSearch(*(void**)(itemDataTable + offset + 4)));
    evtSetValue((void*)param_1, args[2], *(u16*)(itemDataTable + offset + 0x1A));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void help_disp(void* work) {
    void* wp = _wp;
    s32 index = *(s32*)((s32)wp + 0x2C) * 8;
    void* items = *(void**)((s32)wp + 8);

    if (winMgrAction(*(void**)((s32)wp + 0x54)) == 0) {
        *(void**)((s32)work + 0x34) = msgSearch(*(void**)(itemDataTable + *(s32*)((s32)items + index) * 0x28 + 8));
        winMgrHelpDraw(work);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 exchange_shop_point(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 shop_flag_onoff(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 on = evtGetValue(event, *(s32*)args);
    s32 flag = evtGetValue(event, *(s32*)((s32)args + 4));
    void* wp = _wp;

    if (on) {
        *(u16*)wp |= flag;
    } else {
        *(u16*)wp &= ~flag;
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 get_msg(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 shop = evtGetValue(event, *(s32*)args);
    s32 index = evtGetValue(event, *(s32*)((s32)args + 4));
    void* item = (void*)((s32)*(void**)((s32)shop + 0xC) + index * 4);

    evtSetValue(event, *(s32*)((s32)args + 8), *(void**)((s32)item + 0xC));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 point_disp_onoff(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    s32 on = evtGetValue(event, *(s32*)args);
    void* pouch = pouchGetPtr();
    void* wp = _wp;

    *(s32*)((s32)wp + 0x38) = *(s16*)((s32)pouch + 0x5B0);
    if (on) {
        winMgrOpen(*(void**)((s32)wp + 0x58));
    } else {
        winMgrClose(*(void**)((s32)wp + 0x58));
    }
    return 2;
}


s32 item_data_db_arrange(void) {
    return 0;
}


s32 evtShopIsActive(void) {
    void* wp;

    if (strcmp((s32)gp + 0x12C, &mapname) == 0) {
        wp = _wp;
        if (*(u16*)wp != 0) {
            return 1;
        }
    }
    return 0;
}


s32 point_wait(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 get_shop_point(void* event) {
    void* args = *(void**)((s32)event + 0x18);
    void* pouch = pouchGetPtr();

    evtSetValue(event, *(s32*)args, *(s16*)((s32)pouch + 0x5B0));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 item_data_db_restore(void) {
    return 0;
}


s32 unkeep_pouchcheck_func(void* work) {
    pouchGetPtr();
    *(s32*)((s32)work + 0x9C) = pouchGetKeepItemCnt();
    *(s32*)((s32)work + 0xA0) = pouchGetEmptyHaveItemCnt();
    return 2;
}


s32 keep_pouchcheck_func(void* work) {
    pouchGetPtr();
    *(s32*)((s32)work + 0x9C) = pouchGetHaveItemCnt();
    *(s32*)((s32)work + 0xA0) = pouchGetEmptyKeepItemCnt();
    return 2;
}


s32 sell_pouchcheck_func(void* work) {
    pouchGetPtr();
    *(s32*)((s32)work + 0x9C) = pouchGetHaveItemCnt();
    return 2;
}


s32 get_fook_evt(void* event) {
    void* wp = _wp;
    void* args = *(void**)((s32)event + 0x18);
    void* data = *(void**)((s32)wp + 0xC);

    evtSetValue(event, *(s32*)args, *(void**)((s32)data + 4));
    return 2;
}


s32 get_buy_evt(void* event) {
    void* wp = _wp;
    void* args = *(void**)((s32)event + 0x18);
    void* data = *(void**)((s32)wp + 0xC);

    evtSetValue(event, *(s32*)args, *(void**)((s32)data + 8));
    return 2;
}


s32 disp_off(void* event) {
    void* obj = (void*)evtGetValue(event, **(s32***)((s32)event + 0x18));

    *(u16*)obj &= ~1;
    return 2;
}


s32 shopper_name(void* event) {
    void* wp = _wp;
    void* args = *(void**)((s32)event + 0x18);
    void* data = *(void**)((s32)wp + 0xC);

    evtSetValue(event, *(s32*)args, *(void**)data);
    return 2;
}


s32 set_buy_item_id(void* event) {
    s32 value = evtGetValue(event, **(s32***)((s32)event + 0x18));
    void* wp = _wp;

    *(s32*)((s32)wp + 0xD4) = value;
    return 2;
}


s32 get_ptr(void* event) {
    evtSetValue(event, **(s32***)((s32)event + 0x18), _wp);
    return 2;
}


s32 reset_shop_point(void) {
    void* pouch = pouchGetPtr();

    *(s16*)((s32)pouch + 0x5B0) = 0;
    *(s32*)((s32)pouch + 0x5B4) = 0;
    return 2;
}


void help_main(void) {
    winMgrHelpMain();
}
