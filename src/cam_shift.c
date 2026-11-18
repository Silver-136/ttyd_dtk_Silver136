// Camera shift: eases the field camera's sideways lead from Mario's horizontal stick input.

#include "driver/camdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "system.h"
#include "cam_road.h"
#include "cam_shift.h"

// The early demos use the field clock; JP and later use the render clock.
#if VERSION <= VERSION_DEMO_JP
#define CAM_SHIFT_TIME (gp->renderFieldTime)
#else
#define CAM_SHIFT_TIME (gp->renderTime)
#endif

static CamShiftWork work;

/**
 * State of the sideways camera lead
 * (the camera drifts ahead of where Mario is heading); wp is the pointer the code uses.
 */
static CamShiftWork* wp = &work;

/**
 * Resets the camera shift to no lead, with its default stick threshold (300), top speed (3) and acceleration (0.002).
 */
void camShiftInit(void) {
    wp->current = 0.0f;
    wp->target = 0.0f;
    wp->rate = 0.0f;
    wp->stickAccum = 0.0f;
    wp->active = 0.0f;
    wp->stickLimit = 300.0f;
    wp->maxSpeed = 3.0f;
    wp->accel = 0.002f;
    wp->lastX = 0.0f;
    wp->lastZ = 0.0f;
}

/**
 * Updates the sideways camera lead from Mario's horizontal stick and stores it in *out: holding the stick one way while
 * Mario moves sets the target to the camera road's shift rate that way, and the lead eases toward it at a limited speed
 * while the stick is held. CAMERA_FLAG_ROAD_SHIFT_OFF brings it back to 0.
 */
void camShiftMain(CameraEntry* camera, MarioWork* mario, f32* out) {
    f32 shiftRate;
    f32 stick;
    f32 speed;
    f32 diff;
    BOOL reset = FALSE;

    shiftRate = camRoadGetCurrentShiftXRate();
    if (camera->flags & CAMERA_FLAG_ROAD_SHIFT_OFF) {
        reset = TRUE;
    }
    if (reset) {
        wp->rate = 1.0f;
        stick = 0.0f;
        wp->target = stick;
        wp->active = 1.0f;
    } else {
        stick = mario->wStickDir1;
        if (stick > 0.0f) {
            stick = 25.0f;
        }
        if (stick < 0.0f) {
            stick = -25.0f;
        }
        if (mario->position.x == wp->lastX && mario->position.z == wp->lastZ) {
            stick = 0.0f;
        }
        wp->lastX = mario->position.x;
        wp->lastZ = mario->position.z;
    }

    if (stick) {
        if (stick < 0.0f) {
            if (wp->stickAccum > 0.0f) {
                wp->stickAccum = getV60FPS(stick, wp->prevTime, CAM_SHIFT_TIME);
            } else {
                wp->stickAccum += getV60FPS(stick, wp->prevTime, CAM_SHIFT_TIME);
            }
            if (wp->stickAccum <= -wp->stickLimit) {
                wp->active = 1.0f;
                if (wp->target > 0.0f) {
                    wp->rate = 0.0f;
                }
                wp->target = -shiftRate;
                wp->stickAccum = -wp->stickLimit;
            }
        } else {
            if (wp->stickAccum < 0.0f) {
                wp->stickAccum = getV60FPS(stick, wp->prevTime, CAM_SHIFT_TIME);
            } else {
                wp->stickAccum += getV60FPS(stick, wp->prevTime, CAM_SHIFT_TIME);
            }
            if (wp->stickAccum >= wp->stickLimit) {
                wp->active = 1.0f;
                if (wp->target < 0.0f) {
                    wp->rate = 0.0f;
                }
                wp->target = shiftRate;
                wp->stickAccum = wp->stickLimit;
            }
        }
    }

    if (wp->active) {
        wp->rate += getV60FPS(wp->accel, wp->prevTime, CAM_SHIFT_TIME);
        if (wp->rate > 1.0f) {
            wp->rate = 1.0f;
        }
    }

    if (0.0f == wp->target - wp->current) {
        wp->rate = 0.0f;
        wp->active = 0.0f;
    }

    speed = wp->rate * (diff = wp->target - wp->current);
    if (diff > 0.0f) {
        if (diff < 0.0001f) {
            speed = diff;
        }
        if (speed > wp->maxSpeed) {
            speed = wp->maxSpeed;
        }
    } else {
        if (diff > -0.0001f) {
            speed = diff;
        }
        if (speed < -wp->maxSpeed) {
            speed = -wp->maxSpeed;
        }
    }

    if (stick || reset) {
        wp->current += getV60FPS(speed, wp->prevTime, CAM_SHIFT_TIME);
        if (wp->target < 0.0f) {
            if (wp->current < wp->target) {
                wp->current = wp->target;
            }
        } else if (wp->current > wp->target) {
            wp->current = wp->target;
        }
    } else {
        wp->rate = 0.0f;
    }
    *out = wp->current;
}

/** Records this frame's clock so the next camShiftMain can scale by the time elapsed. */
void camShiftPostMain(void) {
    wp->prevTime = CAM_SHIFT_TIME;
}

/** The camera shift's work. */
// Original name and size (0x8) from the JP demo map (UNUSED there: nothing calls it); returns the unit's work
// pointer, like animGetPtr.
CamShiftWork* camShiftGetWorkPtr(void) {
    return wp;
}

/**
 * Snaps the sideways camera lead back to zero with the default tuning (same as camShiftInit).
 * Called on map changes, door transitions, game over and by camera scripts.
 */
void camShiftReset(void) {
    wp->current = 0.0f;
    wp->target = 0.0f;
    wp->rate = 0.0f;
    wp->stickAccum = 0.0f;
    wp->active = 0.0f;
    wp->stickLimit = 300.0f;
    wp->maxSpeed = 3.0f;
    wp->accel = 0.002f;
    wp->lastX = 0.0f;
    wp->lastZ = 0.0f;
}
