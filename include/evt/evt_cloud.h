// evt_cloud.c: script commands for animated cloud overlays.
#pragma once

#include "evt/evt_cmd.h"

// CloudWork.state (evt_cloud_main), returned by evt_cloud_get_mode. wp->state++ steps through 0..4 and 20..21.
typedef enum CloudState {
    CLOUD_STATE_WAIT_BREATH = 0,      // until Flurrie's breath touches the hit object
    CLOUD_STATE_LOAD_POSE = 1,        // load the paper animation, set up the off-screen group
    CLOUD_STATE_CAPTURE = 2,          // capture the group into the image
    CLOUD_STATE_SHOW = 3,             // show the image (A_1), hide the group
    CLOUD_STATE_BLOWING = 4,          // counter grows while the breath lasts
    CLOUD_STATE_RESTORE = 10,         // breath stopped: show the group again
    CLOUD_STATE_BLOWN_AWAY = 20,      // limit passed: endEvt, image A_2, hit object off
    CLOUD_STATE_BLOWN_AWAY_WAIT = 21, // wait out the timer
    CLOUD_STATE_RELEASE = 99,         // free the image and pose, back to WAIT_BREATH
} CloudState;

USER_FUNC(evt_cloud_ent);
USER_FUNC(evt_cloud_get_mode);
USER_FUNC(evt_cloud_init);
USER_FUNC(evt_cloud_set_cap_size);
