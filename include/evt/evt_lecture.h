// evt_lecture.c: script commands for the tutorial (lecture) scenes: system level checks and camera save/load.

#pragma once

#include "evt/evt_cmd.h"
#include <dolphin/types.h>

USER_FUNC(lect_set_systemlevel);
USER_FUNC(lect_test_systemlevel);
USER_FUNC(lect_cam_save);
USER_FUNC(lect_cam_load);
USER_FUNC(lect_juyoitem_rel_set);
USER_FUNC(lect_juyoitem_rel_get);

extern EvtWord all_juyoitem_lecture[];
