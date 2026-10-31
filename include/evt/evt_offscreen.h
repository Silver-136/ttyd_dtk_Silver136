// evt_offscreen.c: script commands for offscreen render targets (offscreendrv).

#pragma once

#include "evt/evt_cmd.h"
#include <dolphin/types.h>

USER_FUNC(evt_offscreen_entry);
USER_FUNC(evt_offscreen_delete);
USER_FUNC(evt_offscreen_get_boundingbox);
USER_FUNC(evt_offscreen_get_boundingbox2);
USER_FUNC(evt_offscreen_set_texture_fmt);
