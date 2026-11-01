// evt_win.c: script commands for the unit windows (shop, partner and item list windows, coin display).
#pragma once

#include "evt/evt_cmd.h"

USER_FUNC(evt_unitwin_alloc);
USER_FUNC(evt_unitwin_free);
USER_FUNC(evt_unitwin_selltable_setup);
USER_FUNC(set_new_goods_list);
USER_FUNC(unitwin_get_work_ptr);
USER_FUNC(unitwin_getpartylank);
USER_FUNC(unitwin_lottery_disp_onoff);
USER_FUNC(unitwin_set_bought);
USER_FUNC(unitwin_setmariolevel);
USER_FUNC(unitwin_setpartylank);
USER_FUNC(unitwincoin_disp_onoff);
#include "version.h"
#if VERSION <= VERSION_DEMO_US
USER_FUNC(unitwin_window_select_func);
#endif
USER_FUNC(unitwincoin_wait);
