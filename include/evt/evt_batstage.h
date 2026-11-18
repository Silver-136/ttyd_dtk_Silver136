// evt_batstage.c: script commands for the battle stage (stage and audience darkness, player lights).

#pragma once

#include "evt/evt_cmd.h"

USER_FUNC(evt_batstage_light_set_player_off);
USER_FUNC(evt_batstage_light_set_player_on);
USER_FUNC(evt_batstage_light_release);
USER_FUNC(evt_batstage_light_release_all);
USER_FUNC(evt_batstage_light_set_lightcolor);
USER_FUNC(evt_batstage_light_set_width);
USER_FUNC(evt_batstage_light_set_position);
USER_FUNC(evt_batstage_light_set_target);
USER_FUNC(evt_batstage_light_set_upvec);
USER_FUNC(evt_batstage_return_aud_dark_base);
USER_FUNC(evt_batstage_set_aud_dark);
USER_FUNC(evt_batstage_set_aud_dark_base);
USER_FUNC(evt_batstage_return_stg_dark_base);
USER_FUNC(evt_batstage_set_stg_dark);
USER_FUNC(evt_batstage_set_stg_dark_base);
