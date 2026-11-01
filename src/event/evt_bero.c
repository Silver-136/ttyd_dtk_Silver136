// Script commands and scripts for map entrances and exits ("bero"): roads, doors, pipes and gates,
// Mario's walk in and out, and the camera changes around them.

#include "version.h"
#include "evt/evt_bero.h"
#include "driver/fadedrv.h"
#include "cam_road.h"
#include "driver/hitdrv.h"
#include "driver/seqdrv.h"
#include "evt/evt_cmd.h"
#include "evt/evt_npc.h"
#include "evt/evt_seq.h"
#include "mario/mario.h"
#include "mario/mario_sbr.h"
#include "mario/mariost.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>
#include "driver/camdrv.h"
#include "driver/animdrv.h"
#include "cam_shift.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "driver/casedrv.h"
#include "evt/evt_fade.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_party.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_hit.h"
#include "system.h"

//.sbss, NOTE: all are reversed because of -inline deferred
s32 BeroEXEC;
s32 BeroNUM, BeroNOWNUM;
s32 BeroCAM; // pipe camera handshake: bero_case_exec_pipe_sub pokes it and polls it (POKE_INT / PEEK_INT)
f32 BeroOX, BeroOY, BeroOZ; //offset position
f32 BeroSX, BeroSY, BeroSZ; //start position
f32 BeroEX, BeroEY, BeroEZ; //end position
#if VERSION <= VERSION_DEMO_US
extern Vec BeroPT, BeroAT;
#else
Vec BeroPT, BeroAT;
#endif
BOOL BeroMarioGO;

/**
 * Stand-in entrance for an arrival with no entrance name: no hit object, normal type,
 * direction and start point worked out automatically. The entrance lookup falls back to it
 * when gp->beroEnterName is empty.
 */
#if VERSION > VERSION_DEMO_US
BeroEntry bero_null[] = {
    {
        NULL, // hitName
        BERO_TYPE_NONE, // type
        0, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, 0, NULL, // exitEvent..nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};
#endif

/** Enter script for entrances without one: LW(0) = 0 so the normal entrance runs. */
EVT_BEGIN(default_entevt)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Exit script for entrances without one: LW(0) = 0 so the exit goes ahead. */
EVT_BEGIN(default_outevt)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/** Arrival through a "none" entrance (type 0xF00): no walk-in, just gives control back to the player. */
#if VERSION > VERSION_DEMO_US
/** Arrival through a "none" entrance (type 0xF00): no walk-in, just gives control back to the player. */
EVT_BEGIN(evt_bero_info_run_none)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
#endif

/** Arrival through a gate entrance (type 3): no walk-in, just gives control back to the player. */
#if VERSION > VERSION_DEMO_US
/** Arrival through a gate entrance (type 3): no walk-in, just gives control back to the player. */
EVT_BEGIN(evt_bero_info_run_gate)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
#endif

// s32 x, s32 y, s32 z (new in retail)
// Descriptive name (not in the symbol maps).
#if VERSION <= VERSION_DEMO_US
BeroEntry bero_null[] = {
    {
        NULL, // hitName
        BERO_TYPE_NONE, // type
        0, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, 0, NULL, // exitEvent..nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
};
#endif
/**
 * Sets where Mario respawns after falling into a bottomless pit; args are x, y, z as whole numbers.
 * Retail only (the demos lack it).
 */
USER_FUNC(evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry) {
    EvtWord* args = event->args;
    s32 x, y, z;

    x = evtGetValue(event, args[0]);
    y = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    marioSetBottomlessRespawnPosOnBeroEntry((f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

/** Sets Mario's bottomless-pit respawn point to where he stands now. No arguments; retail only. */
// no args (new in retail)
// Descriptive name (not in the symbol maps).
USER_FUNC(evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos) {
    MarioWork* wp = marioGetPtr();
    marioSetBottomlessRespawnPosOnBeroEntry(wp->position.x, wp->position.y, wp->position.z);
    return EVT_RETURN_DONE;
}

/**
 * Returns the BeroEXEC state bits; bit 1 is set while an entrance
 * or exit is playing (camdrv and seq_mapchange check it before showing the letterbox).
 */
s32 bero_get_BeroEXEC(void) {
    return BeroEXEC;
}

/** Returns how many entrances the current map registered with evt_bero_get_info. */
s32 bero_get_BeroNUM(void) {
    return BeroNUM;
}

f32 bero_get_BeroSX(void) {
    return BeroSX;
}

f32 bero_get_BeroSY(void) {
    return BeroSY;
}

f32 bero_get_BeroSZ(void) {
    return BeroSZ;
}

f32 bero_get_BeroEX(void) {
    return BeroEX;
}

f32 bero_get_BeroEY(void) {
    return BeroEY;
}

f32 bero_get_BeroEZ(void) {
    return BeroEZ;
}

/** Returns the current map's table of 16 entrance pointers (BeroINFOARR, NULL-terminated). */
BeroEntry** bero_get_ptr(void) {
    return BeroINFOARR;
}

/** Zeroes the saved Mario display offset (BeroOX/OY/OZ) that pipe travel restores; called from mot_yoshi. */
void bero_clear_Offset(void) {
    BeroOX = 0.0f;
    BeroOY = 0.0f;
    BeroOZ = 0.0f;
}

/**
 * Turns an entrance reference into its table index:
 * values 0-15 are already indices, anything else is taken as a hit object name and looked up in BeroINFOARR.
 */
// FAKE: `&newId != NULL` tests (always true); the plain `newId = 0` / `newId++` change the object
s32 bero_id_filter(EvtWord id) {
    BeroEntry** temp, **table = BeroINFOARR;
    s32 newId = id;
    const char *string, *test;

    if (id < 0 || id >= 16) {
        test = (const char*)id;
        if (&newId != NULL) {
            newId = 0;
        }
        temp = table;
        while (1) {
            string = (*temp)->hitName;
            if (!string || !strcmp(string, test)) {
                break;
            }
            temp++;
            if (&newId != NULL) {
                newId++;
            }
        }
    }
    return newId;
}

/** const char** mapName, const char** beroName */
//const char** mapName, const char** beroName
USER_FUNC(evt_bero_mapchange) {
    EvtWord* args = event->args;
    const char *map, *bero;

    map = (const char*)evtGetValue(event, args[0]);
    bero = (const char*)evtGetValue(event, args[1]);
    if (isFirstCall) {
        seqSetSeq(SEQ_MAP_CHANGE, map, bero);
    }
    return EVT_RETURN_BLOCK;
}

/** s32 retIndex */
//s32 retIndex
USER_FUNC(evt_bero_get_entername) {
    evtSetValue(event, *event->args, (EvtWord)&gp->beroEnterName);
    return EVT_RETURN_DONE;
}

/** BOOL* on/off, s32* mask */
//BOOL* on/off, s32* mask
USER_FUNC(evt_bero_exec_onoff) {
    EvtWord* args = event->args;
    BOOL onoff;
    s32 mask, value;

    onoff = evtGetValue(event, args[0]);
    mask = evtGetValue(event, args[1]);
    value = BeroEXEC | mask;
    if (onoff) {
        value = BeroEXEC & ~mask;
    }
    BeroEXEC = value;
    return EVT_RETURN_DONE;
}

/** s32 retIndex */
//s32 retIndex
USER_FUNC(evt_bero_exec_get) {
    evtSetValue(event, *event->args, BeroEXEC);
    return EVT_RETURN_DONE;
}

/** Blocks until any of the bits in the argument mask is set in BeroEXEC (see evt_bero_exec_onoff). */
USER_FUNC(evt_bero_exec_wait) {
    return evtGetValue(event, *event->args) & BeroEXEC ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** f32 retX, f32 retY, f32 retZ */
//f32 retX, f32 retY, f32 retZ
USER_FUNC(evt_bero_get_start_position) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], BeroSX);
    evtSetFloat(event, args[1], BeroSY);
    evtSetFloat(event, args[2], BeroSZ);
    return EVT_RETURN_DONE;
}

/** f32 retX, f32 retY, f32 retZ */
//f32 retX, f32 retY, f32 retZ
USER_FUNC(evt_bero_get_end_position) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], BeroEX);
    evtSetFloat(event, args[1], BeroEY);
    evtSetFloat(event, args[2], BeroEZ);
    return EVT_RETURN_DONE;
}

/** s32* id, s32 retIndex1, s32 retIndex2, s32 retIndex3, s32 retIndex4 */
//s32* id, s32 retIndex1, s32 retIndex2, s32 retIndex3, s32 retIndex4
USER_FUNC(evt_bero_get_info_anime) {
    BeroEntry* bero;
    EvtWord* args = event->args;

    bero = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], bero->enterAnimeType);
    evtSetValue(event, args[2], bero->exitAnimeType);
    evtSetValue(event, args[3], (EvtWord)bero->enterAnimeArg);
    evtSetValue(event, args[4], (EvtWord)bero->exitAnimeArg);
    return EVT_RETURN_DONE;
}

/**
 * args: entrance index, out length (float).
 * Uses the entry's length, or the type default when it is -1:
 * 30 for roads, gates and "none", 60 for doors and pipes.
 */
USER_FUNC(evt_bero_get_info_length) {
    BeroEntry* bero;
    EvtWord* args = event->args;
    s32 length;
    f32 defaultLength;

    bero = BeroINFOARR[evtGetValue(event, args[0])];
    length = bero->length;
    if (length != -1) {
        evtSetFloat(event, args[1], (f32)length);
        return EVT_RETURN_DONE;
    }
    switch (bero->type & BERO_TYPE_KIND_MASK) {
        case BERO_TYPE_ROAD:
        case BERO_TYPE_GATE:
        case BERO_TYPE_NONE:
            defaultLength = 30.0f;
            break;
        case BERO_TYPE_DOOR:
            defaultLength = 60.0f;
            break;
        case BERO_TYPE_PIPE:
            defaultLength = 60.0f;
            break;
        default:
            defaultLength = 60.0f;
            break;
    }
    evtSetFloat(event, args[1], defaultLength);
    return EVT_RETURN_DONE;
}

/** args: entrance index, then outs for its type, door sound kind (sfxId) and direction. */
USER_FUNC(evt_bero_get_info_kinddir) {
    BeroEntry* bero;
    EvtWord* args = event->args;

    bero = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], bero->type);
    evtSetValue(event, args[2], bero->sfxId);
    evtSetValue(event, args[3], bero->directionId);
    return EVT_RETURN_DONE;
}

/**
 * args: entrance index, then outs for the destination map name and destination entrance name.
 * Also used by evt_movefloor.
 */
USER_FUNC(evt_bero_get_info_nextarea) {
    BeroEntry* bero;
    EvtWord* args = event->args;

    bero = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], (EvtWord)bero->nextMapName);
    evtSetValue(event, args[2], (EvtWord)bero->nextBeroName);
    return EVT_RETURN_DONE;
}

/** Records the index of the entrance being used now (BeroNOWNUM). */
USER_FUNC(evt_bero_set_now_number) {
    BeroNOWNUM = evtGetValue(event, *event->args);
    return EVT_RETURN_DONE;
}

/** Returns the index of the entrance in use (BeroNOWNUM) to args[0]; also used by evt_movefloor. */
USER_FUNC(evt_bero_get_now_number) {
    evtSetValue(event, *event->args, BeroNOWNUM);
    return EVT_RETURN_DONE;
}

/**
 * Loads the map's entrance table from LW(0): fills BeroINFOARR, reports entries whose hit object is missing,
 * marks every exit unregistered (BeroSW -1), sets BeroNUM and clears BeroEXEC.
 */
USER_FUNC(evt_bero_get_info) {
    BeroEntry** table;
    int i;
    s32* switches;

    table = BeroINFOARR;
    for (i = 0; i < 16; i++) {
        *table++ = NULL;
    }
    table = BeroINFOARR;

    {
        BeroEntry* en;
        for (en = (BeroEntry*)event->lwData[0];; en++) {
            HitEntry* hit = hitNameToPtr(en->hitName);
            if (!en->hitName) {
                break;
            }
            if (!hit) {
                mapErrorEntry(1, en->hitName);
            }
        }
    }
    {
        BeroEntry* e = (BeroEntry*)event->lwData[0];
        switches = BeroSW;
        i = 0;
        while (1) {
            if (!e->hitName) {
                break;
            }
            *table = e;
            switches[i] = -1;
            table++;
            i++;
            e++;
        }
    }
    BeroNUM = i;
    BeroEXEC = 0;
    return EVT_RETURN_DONE;
}

// FAKE: walk flag picks a pointer-walk loop (evt_bero_get_info_num) or the index loop (evt_bero_get_into_info); constant-folded per call site
static inline s32 bero_get_kind(BeroEntry* bero, s32 walk) {
    u32 i;
    const char** p;
    const char* dirs[8] = {"n_", "ne_", "e_", "se_", "s_", "sw_", "w_", "nw_"};
    s32 kind = bero->directionId;
    switch (kind) {
    case BERO_DIR_AUTO:
        // FAKE: dead test (kind is 20000 in this case); removing it changes the object
        if (kind == BERO_DIR_NONE) {
            kind = BERO_DIR_NONE;
        } else {
        switch (bero->type & BERO_TYPE_KIND_MASK) {
            case BERO_TYPE_ROAD:
            case BERO_TYPE_DOOR:
                if (walk) {
                    for (i = 0, p = dirs; i < 8; i++, p++) {
                        if (!strncmp(*p, bero->hitName, strlen(*p))) {
                            break;
                        }
                    }
                } else {
                    for (i = 0; i < 8; i++) {
                        if (!strncmp(dirs[i], bero->hitName, strlen(dirs[i]))) {
                            break;
                        }
                    }
                }
                break;
            default:
                i = BERO_DIR_NONE;
                break;
        }
        kind = i;
        }
        break;
    }
    return kind;
}

/**
 * Finds the entrance Mario arrived by (gp->beroEnterName; bero_null when empty) and fills LW(1) index,
 * LW(2) count, LW(3) name, LW(4) type, LW(5) direction, LW(6) position, LW(7) enter script.
 */
USER_FUNC(evt_bero_get_into_info) {
    s32 i;
    BeroEntry* bero;
    EvtWord* evt;

    for (i = 0;; i++) {
        bero = BeroINFOARR[i];
        if (!bero || !strcmp(gp->beroEnterName, "")) {
            i = 0;
            bero = bero_null;
            break;
        }
        if (!strcmp(bero->hitName, gp->beroEnterName)) {
            break;
        }
    }
    if ((gp->flags & GW_FLAG_NO_ENTRANCE) && !strcmp(gp->beroEnterName, "")) {
        bero = BeroINFOARR[0];
        event->lwData[1] = 0;
        event->lwData[2] = BeroNUM;
        event->lwData[3] = (EvtWord)bero->hitName;
        event->lwData[4] = bero->type;
        event->lwData[5] = bero_get_kind(bero, 0);
        event->lwData[6] = (EvtWord)&bero->centerPosition;
        evt = bero->enterEvent;
        if (!evt) {
            evt = default_entevt;
        }
        event->lwData[7] = (EvtWord)evt;
        event->lwData[8] = 0xFFFF0000;
        return EVT_RETURN_DONE;
    } else {
        event->lwData[1] = i;
        event->lwData[2] = BeroNUM;
        event->lwData[3] = (EvtWord)gp->beroEnterName;
        event->lwData[4] = bero->type;
        event->lwData[5] = bero_get_kind(bero, 0);
        event->lwData[6] = (EvtWord)&bero->centerPosition;
        evt = bero->enterEvent;
        if (!evt) {
            evt = default_entevt;
        }
        event->lwData[7] = (EvtWord)evt;
        event->lwData[8] = 0xFFFF0000;
        return EVT_RETURN_DONE;
    }
}

/** Loads Mario's paper pipe-travel animations (p_dokan_x, p_dokan_y), blocking until both are in. */
USER_FUNC(evt_bero_read_mario_pera) {
    if (isFirstCall) {
        event->userdata[0] = 0;
    }
    switch (event->userdata[0]) {
        case 0:
            if (animGroupBaseAsync("p_dokan_x", 0, NULL)) {
                event->userdata[0] = 1;
            }
            break;
        case 1:
            if (animGroupBaseAsync("p_dokan_y", 0, NULL)) {
                event->userdata[0] = 100;
            }
            break;
        case 2:
            break;
        default:
            break;
    }
    return event->userdata[0] == 100 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** Replaces the variable in args[0] (an index or a hit name) with its entrance index, see bero_id_filter. */
USER_FUNC(evt_bero_id_filter) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], bero_id_filter(evtGetValue(event, args[0])));
    return EVT_RETURN_DONE;
}

/**
 * Loads entrance args[0] into LW(3)-LW(14) for the bero scripts:
 * name, type, direction, position, enter script, LW(9) registered state, LW(10) case trigger,
 * LW(11) exit script, LW(13)/LW(14) destination.
 */
USER_FUNC(evt_bero_get_info_num) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    BeroEntry* bero = BeroINFOARR[id];
    EvtWord* evt;

    event->lwData[3] = (EvtWord)bero->hitName;
    event->lwData[4] = bero->type;
    event->lwData[5] = bero_get_kind(bero, 1);
    event->lwData[6] = (EvtWord)&bero->centerPosition;
    evt = bero->enterEvent;
    if (!evt) {
        evt = default_entevt;
    }
    event->lwData[7] = (EvtWord)evt;
    event->lwData[8] = 0xFFFF0000;
    event->lwData[9] = BeroSW[id];
    event->lwData[10] = bero->caseType;
    if (bero->caseType == -1) {
        switch (bero->type & BERO_TYPE_KIND_MASK) {
            case BERO_TYPE_NONE:
                event->lwData[10] = CASE_STAND_ON; // CASE_STAND_ON
                break;
            case BERO_TYPE_ROAD:
                event->lwData[10] = CASE_PUSH; // CASE_PUSH
                break;
            case BERO_TYPE_DOOR:
                event->lwData[10] = CASE_FRONT_A; // CASE_FRONT_A
                break;
            case BERO_TYPE_PIPE:
                event->lwData[10] = CASE_JUMP_FROM_A; // CASE_JUMP_FROM_A
                break;
            case BERO_TYPE_GATE:
                event->lwData[10] = CASE_STAND_ON_STICK_UP; // CASE_STAND_ON_STICK_UP
                break;
        }
    }
    evt = bero->exitEvent;
    if (!evt) {
        evt = default_outevt;
    }
    event->lwData[11] = (EvtWord)evt;
    event->lwData[12] = 0xFFFF0000;
    event->lwData[13] = (EvtWord)bero->nextMapName;
    event->lwData[14] = (EvtWord)bero->nextBeroName;
    return EVT_RETURN_DONE;
}

USER_FUNC(bero_get_position_normal) {
    EvtWord* args = event->args;
#if VERSION > VERSION_DEMO_US
    MarioWork* wp = marioGetPtr();
#endif
    s32 dir = evtGetValue(event, args[0]);
    float halfTurn;
    f32 respawnZ;
    f32 len = evtGetFloat(event, args[1]);
    s32* pos = (s32*)event->lwData[6];
    const char* name = (const char*)event->lwData[3];
    s32 angle = event->lwData[5];
    Vec start;
    f32 *hxPtr;
    Vec end;
    start.x = pos[0];
    start.y = pos[1];
    start.z = pos[2];
    end.x = pos[0];
    end.y = pos[1];
    end.z = pos[2];
    if (pos[0] == BERO_POS_AUTO) {
        hitObjGetPos(name, &start);
    }
    if (len != 0.0f) {
        Vec tmp2;
        Vec tmp;
        f32 nz;
        f32 ny;
        f32 nx;
        f32 dist;
        f32 hz;
        f32 hy;
        f32 hx;
        f32 deg;
        f32 rad;
        hz = hy = hx = 0.0f;
        dist = 1000000.0f;
        // FAKE: two-step product; `deg = (angle * 360.0f) * 0.125f` changes the object
        deg = 0.125f;
        deg = (angle * 360.0f) * deg;
        if (dir == 1) {
            deg += 180.0f;
        }
        rad = (6.2831855f * deg) / 360.0f;
        tmp.x = start.x - (30.0f * (f32)sin(rad));
        tmp.y = start.y;
        // FAKE: block-scoped temporary for cos(rad); the direct expression changes the object
        {
            f32 c0 = (f32) cos(rad);
            tmp.z = start.z - (30.0f * (-c0));
        }
        // FAKE: repeated store of dist and a spare pointer to hx; dropping both changes the object
        dist = 1000000.0f;
        hxPtr = &hx;
        if (hitCheckFilter(NULL, hxPtr, &hy, &hz, &dist, &nx, &ny, &nz, tmp.x, tmp.y, tmp.z, 0.0f, -1.0f, 0.0f)) {
            tmp.y = hy;
        }
        tmp2 = tmp;
        tmp2.x += (30.0f + len) * (f32)sin(rad);
        // FAKE: recomputes rad; `cos(rad)` changes the object
        tmp2.z += (30.0f + len) * (-((f32) cos((6.2831855f * deg) / 360.0f)));
        start = tmp;
        end = tmp2;
    }
    BeroSX = start.x;
    BeroSY = start.y;
    BeroSZ = start.z;
    BeroEX = end.x;
    BeroEY = end.y;
    BeroEZ = end.z;
#if VERSION > VERSION_DEMO_US
    marioSetBottomlessRespawnPosOnBeroEntry(end.x, end.y, end.z);
    if (wp->currMotionId == MARIO_MOTION_SHIP) {
        marioReloadMapOnBottomlessOn();
    }
#endif
    return EVT_RETURN_DONE;
}

/**
 * Start and end points for pipe travel: id 0 goes into the pipe, otherwise out of it;
 * side pipes move a units along X (floor-snapped), up/down pipes a units along Y, others stay at the hit object.
 */
void bero_get_position_pipe_sub(s32 id, s32 a, EvtWord name, s32 kind, Vec* start, Vec* end) {
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 ofs;
    hz = hy = hx = 0.0f;
    dist = 10000.0f;
    ofs = 20.0f;
    switch (kind) {
    case BERO_DIR_W:
        a = -a;
        ofs = -ofs;
    case BERO_DIR_E:
        if (id == 0) {
            hitObjGetPos((const char*)name, start);
            start->x -= ofs;
            end->x = start->x + a;
            end->y = start->y;
            end->z = start->z;
            hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, end->x, end->y, end->z, 0.0f, -1.0f, 0.0f);
        } else {
            hitObjGetPos((const char*)name, end);
            end->x -= ofs;
            start->x = end->x + a;
            start->y = end->y;
            start->z = end->z;
            hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, start->x, start->y, start->z, 0.0f, -1.0f, 0.0f);
        }
        end->y = start->y = hy;
        break;
    case BERO_DIR_DOWN:
        a = -a;
    case BERO_DIR_UP:
        if (id == 0) {
            hitObjGetPos((const char*)name, start);
            end->x = start->x;
            end->y = start->y + a;
            end->z = start->z;
        } else {
            hitObjGetPos((const char*)name, end);
            start->x = end->x;
            start->y = end->y + a;
            start->z = end->z;
        }
        break;
    case BERO_DIR_NONE:
    default:
        hitObjGetPos((const char*)name, start);
        hitObjGetPos((const char*)name, end);
        break;
    }
}

#if VERSION <= VERSION_DEMO_US
static USER_FUNC(bero_get_position_pipe_pure) {
#else
USER_FUNC(bero_get_position_pipe_pure) {
#endif
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 a = evtGetFloat(event, args[1]);
    s32 kind = event->lwData[5];
    Vec start, end;
    bero_get_position_pipe_sub(id, a, event->lwData[3], kind, &start, &end);
    BeroSX = start.x;
    BeroSY = kind == BERO_DIR_UP ? start.y - 37.5f : start.y;
    BeroSZ = start.z;
    BeroEX = end.x;
    BeroEY = (f32)(kind == BERO_DIR_UP ? end.y - 37.5f : end.y);
    BeroEZ = end.z;
#if VERSION > VERSION_DEMO_US
    marioSetBottomlessRespawnPosOnBeroEntry(BeroEX, BeroEY, BeroEZ);
#endif
    return EVT_RETURN_DONE;
}

/**
 * Works out the road camera at the far end of a pipe (args: in/out, length) into BeroPT/BeroAT,
 * for evt_bero_cam3d_change during same-map pipe travel.
 */
USER_FUNC(bero_get_position_pipe2) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 a = evtGetFloat(event, args[1]);
    Vec start, end;
    CameraParams out;
    bero_get_position_pipe_sub(id, a, event->lwData[3], event->lwData[5], &start, &end);
    end.y += 1.0f;
    camRoadReset();
    camShiftReset();
    camRoadMain(&out, end.x, end.y, end.z, end.x, end.y, end.z, 0.0f);
    {
        Vec pt = {0.0f, 0.0f, 0.0f};
        Vec at = {0.0f, 0.0f, 0.0f};
        pt.x = out.pos.x;
        pt.y = out.pos.y;
        pt.z = out.pos.z;
        at.x = out.at.x;
        at.y = out.at.y;
        at.z = out.at.z;
        BeroPT = pt;
        BeroAT = at;
    }
    return EVT_RETURN_DONE;
}

/**
 * Re-registers the pipe exit at LW(3) once Mario has stepped off it, so he does not re-enter at once;
 * waits for motion 3 or 10 only when LW(10) is 6, otherwise re-registers right away.
 */
EVT_BEGIN(evt_bero_nonride_check)
    IF_INT_NOT_EQUAL(LW(10), 6)
        SET_VALUE(LW(0), LW(3))
        CALL_CHILD(PTR(bero_case_switch_on))
        RETURN
    END_IF
    LOOP(0)
        CALL(evt_mario_get_motion, LW(0))
        IF_INT_EQUAL(LW(0), MARIO_MOTION_JUMP)
            BREAK_LOOP
        END_IF
        IF_INT_EQUAL(LW(0), MARIO_MOTION_FALL)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    DEBUG_PUT_MSG(STRING("\223y\212\307\202\251\202\347\215~\202\350\202\275"))
    SET_VALUE(LW(0), LW(3))
    CALL_CHILD(PTR(bero_case_switch_on))
    RETURN
EVT_END()

EVT_BEGIN(bero_open_door_play_se)
    CALL(evt_bero_get_info_num, LW(1))
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(evt_hit_get_position, LW(3), LW(4), LW(5), LW(6))
    END_IF
    CALL(evt_bero_get_info_kinddir, LW(1), LW(10), LW(11), LW(12))
    SWITCH(LW(11))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(3)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD2"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(4)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_WOOD3"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(5)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_IRON1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(6)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_IRON2"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(7)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_GRILLE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(8)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(9)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_TURN1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(10)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_ELEC1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(11)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_STONE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(12)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_CASTLE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(13)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_CASTLE2"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(14)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_METAL1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(15)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AUTO_DOOR1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(16)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_LOCKER_OPEN1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(17)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_SLIDE1_3"), LW(4), LW(5), LW(6), 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(bero_close_door_play_se)
    CALL(evt_bero_get_info_num, LW(1))
    SET_VALUE(LW(4), 0)
    SET_VALUE(LW(5), 0)
    SET_VALUE(LW(6), 0)
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(evt_hit_get_position, LW(3), LW(4), LW(5), LW(6))
    END_IF
    CALL(evt_bero_get_info_kinddir, LW(1), LW(10), LW(11), LW(12))
    SWITCH(LW(11))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(2)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(3)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(4)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_WOOD2"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(5)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_IRON1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(6)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_IRON2"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(7)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_GRILLE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(8)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(9)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_TURN1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(10)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_ELEC1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(11)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_OPEN_STONE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(12)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_CASTLE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(13)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_CASTLE2"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(14)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_METAL1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(15)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AUTO_DOOR1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(16)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_LOCKER_CLOSE1"), LW(4), LW(5), LW(6), 0)
        CASE_EQUAL(17)
        CALL(evt_snd_sfxon_3d, STRING("SFX_DOOR_SHUT_SLIDE1_3"), LW(4), LW(5), LW(6), 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(bero_play_enter_anime)
    CALL(evt_bero_get_info_anime, LW(1), LW(10), LW(11), LW(12), LW(13))
    SWITCH(LW(10))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        BEGIN_INLINE_EVENT
            WAIT_MS(200)
            CALL(evt_bero_mario_go)
        END_INLINE_EVENT
        IF_INT_NOT_EQUAL(LW(12), 0)
            CALL(evt_map_playanim, LW(12), MAP_ANIM_PLAY_ONCE, 0)
            CALL(evt_map_pauseanim, 0, LW(12))
            WAIT_MS(700)
            SPAWN_EVENT(PTR(bero_close_door_play_se))
            CALL(evt_map_replayanim, 0, LW(12))
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(LW(12), 0)
            BREAK_SWITCH
        END_IF
        SET_VALUE(LW(8), LW(12))
        CALL(evt_mapobj_rotate, 1, LW(8), 0, -80, 0)
        CALL(evt_bero_mario_go)
        WAIT_MS(500)
        SPAWN_EVENT(PTR(bero_close_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, -80, 0, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, LW(8), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CASE_EQUAL(4)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SPAWN_EVENT(LW(12))
        END_IF
        CASE_EQUAL(5)
        IF_INT_NOT_EQUAL(LW(12), 0)
            SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
            CALL_CHILD(LW(12))
            CALL(evt_bero_mario_go)
            WAIT_MS(500)
            SPAWN_EVENT(PTR(bero_close_door_play_se))
            SET_VALUE(LW(9), LW(12))
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 180, 0, 500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
                IF_INT_EQUAL(LW(10), 0)
                    SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                    CALL_CHILD(LW(9))
                    BREAK_LOOP
                    ELSE
                    CALL_CHILD(LW(9))
                END_IF
            END_LOOP
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(bero_play_out_anime)
    CALL(evt_bero_get_info_anime, LW(1), LW(10), LW(11), LW(12), LW(13))
    SWITCH(LW(11))
        CASE_EQUAL(0)
        CASE_EQUAL(1)
        IF_INT_NOT_EQUAL(LW(13), 0)
            SPAWN_EVENT(PTR(bero_open_door_play_se))
            CALL(evt_map_playanim, LW(13), MAP_ANIM_PLAY_ONCE, 0)
            BEGIN_INLINE_EVENT
                CALL(evt_map_checkanim, LW(13), LW(8), LW(9))
                WAIT_MS(LW(9))
                CALL(evt_bero_mario_go)
            END_INLINE_EVENT
            ELSE
            BEGIN_INLINE_EVENT
                WAIT_MS(500)
                CALL(evt_bero_mario_go)
            END_INLINE_EVENT
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(LW(12), 0)
            BREAK_SWITCH
        END_IF
        SET_VALUE(LW(8), LW(12))
        SPAWN_EVENT(PTR(bero_open_door_play_se))
        CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 80, 500)
        LOOP(0)
            CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
            CALL(evt_mapobj_rotate, 1, LW(8), 0, LW(0), 0)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_bero_mario_go)
        CASE_EQUAL(4)
        IF_INT_NOT_EQUAL(LW(13), 0)
            SPAWN_EVENT(LW(13))
        END_IF
        CASE_EQUAL(5)
        IF_INT_NOT_EQUAL(LW(13), 0)
            SPAWN_EVENT(PTR(bero_open_door_play_se))
            SET_VALUE(LW(9), LW(13))
            CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 180, 500)
            LOOP(0)
                CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(10))
                IF_INT_EQUAL(LW(10), 0)
                    SET_FLOAT_VALUE(LW(0), FLOAT(180.0f))
                    CALL_CHILD(LW(9))
                    BREAK_LOOP
                    ELSE
                    CALL_CHILD(LW(9))
                END_IF
            END_LOOP
            CALL(evt_bero_mario_go)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

// LW(4): the exit's BeroEntry.type, LW(5): its direction kind
USER_FUNC(bero_set_disp_position_pipe) {
    switch (event->lwData[5]) {
        case 2: case 6:
            if (!(event->lwData[4] & BERO_TYPE_BG_MODE)) {
                marioGetPtr()->dispOfs.y += 3.0f;
                marioGetPtr()->dispOfs.z -= 15.0f;
            }
            break;
        case 7: case 10000:
            break;
        case 8: case 9:
            if (!(event->lwData[4] & BERO_TYPE_BG_MODE)) {
                marioGetPtr()->dispOfs.z -= 13.0f;
            } else {
                marioGetPtr()->dispOfs.z -= 20.0f;
            }
            break;
    }
    return EVT_RETURN_DONE;
}

/**
 * Marks entrance args[0] (index or name) as registered in BeroSW;
 * LW(0) = 1 if it already was (so the caller skips a double registration), LW(1) = its index.
 */
// FAKE: `&newId != NULL` tests (always true); the plain `newId = 0` / `newId++` change the object
USER_FUNC(evt_bero_switch_on) {
    BeroEntry** table = BeroINFOARR;
    EvtWord* args = event->args;
    BeroEntry** temp;
    EvtWord id = evtGetValue(event, *args++);
    s32 newId = id;
    const char *string, *test;
    if (id < 0 || id >= 16) {
        test = (const char*)id;
        if (&newId != NULL) {
            newId = 0;
        }
        temp = table;
        while (1) {
            string = (*temp)->hitName;
            if (!string || !strcmp(string, test)) {
                break;
            }
            temp++;
            if (&newId != NULL) {
                newId++;
            }
        }
    }
    if (BeroSW[newId] == 1) {
        event->lwData[0] = 1;
    } else {
        event->lwData[0] = 0;
    }
    BeroSW[newId] = 1;
    event->lwData[1] = newId;
    return EVT_RETURN_DONE;
}

/** Marks entrance args[0] (index or name) as unregistered; LW(0) = 1 if it already was, LW(1) = its index. */
// FAKE: `&newId != NULL` tests (always true); the plain `newId = 0` / `newId++` change the object
USER_FUNC(evt_bero_switch_off) {
    BeroEntry** table = BeroINFOARR;
    EvtWord* args = event->args;
    BeroEntry** temp;
    EvtWord id = evtGetValue(event, *args++);
    s32 newId = id;
    const char *string, *test;
    if (id < 0 || id >= 16) {
        test = (const char*)id;
        if (&newId != NULL) {
            newId = 0;
        }
        temp = table;
        while (1) {
            string = (*temp)->hitName;
            if (!string || !strcmp(string, test)) {
                break;
            }
            temp++;
            if (&newId != NULL) {
                newId++;
            }
        }
    }
    if (BeroSW[newId] != 1) {
        event->lwData[0] = 1;
    } else {
        event->lwData[0] = 0;
    }
    BeroSW[newId] = 0;
    event->lwData[1] = newId;
    return EVT_RETURN_DONE;
}

/** Puts the index of entrance args[0] (index or hit name) into LW(1). */
// FAKE: `&newId != NULL` tests (always true); the plain `newId = 0` / `newId++` change the object
USER_FUNC(evt_bero_get_number) {
    BeroEntry** table = BeroINFOARR;
    EvtWord* args = event->args;
    BeroEntry** temp;
    EvtWord id = evtGetValue(event, *args++);
    s32 newId = id;
    const char *string, *test;
    if (id < 0 || id >= 16) {
        test = (const char*)id;
        if (&newId != NULL) {
            newId = 0;
        }
        temp = table;
        while (1) {
            string = (*temp)->hitName;
            if (!string || !strcmp(string, test)) {
                break;
            }
            temp++;
            if (&newId != NULL) {
                newId++;
            }
        }
    }
    event->lwData[1] = newId;
    return EVT_RETURN_DONE;
}

/** Stores case ID args[1] for entrance index args[0] (BeroID), for deleting the exit trigger later. */
USER_FUNC(evt_bero_case_id_save) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    BeroID[id] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Returns the case ID saved for entrance index args[0] into args[1]. */
USER_FUNC(evt_bero_case_id_load) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], BeroID[id]);
    return EVT_RETURN_DONE;
}

/** Saves Mario's display offset before pipe travel shifts it (restored by evt_bero_disppos_load). */
USER_FUNC(evt_bero_disppos_save) {
    BeroOX = marioGetPtr()->dispOfs.x;
    BeroOY = marioGetPtr()->dispOfs.y;
    BeroOZ = marioGetPtr()->dispOfs.z;
    return EVT_RETURN_DONE;
}

/** Restores Mario's display offset saved by evt_bero_disppos_save. */
USER_FUNC(evt_bero_disppos_load) {
    marioGetPtr()->dispOfs.x = BeroOX;
    marioGetPtr()->dispOfs.y = BeroOY;
    marioGetPtr()->dispOfs.z = BeroOZ;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_move_mario_speed) {
    EvtWord* args = event->args;
    f32 speed = evtGetFloat(event, args[0]);
    OSTime now = gp->inBattle ? gp->renderTime : gp->renderFieldTime;
    f32 s, c;
    f32 angle;
    f32 dx, dy;
    f32 dist;
    f32 ox, oy;
    BOOL xdone, ydone;
    s32 frames;
    f32 sx;

    if (isFirstCall) {
        event->savedTime = now;
    }
    speed = sysFrame2SecFloat(speed);
    angle = 6.2831855f * reviseAngle(angleABf(BeroSX, BeroSY, BeroEX, BeroEY)) / 360.0f;
    s = sin(angle);
    c = cos(angle);
    frames = sysMsec2Frame((s32)OSTicksToMilliseconds((u64)((s32)now - (s32)event->savedTime)));
    dx = speed * s;
    dy = -(speed * c);
    dist = speed * frames;
    ox = s * dist;
    // FAKE: spare temporary for BeroSX; using BeroSX directly changes the object
    sx = BeroSX;
    marioGetPtr()->position.x = sx + ox;
    oy = -(c * dist);
    marioGetPtr()->position.y = BeroSY + oy;
    marioGetPtr()->position.z = BeroSZ;
    xdone = FALSE;
    if (dx >= 0.0f && BeroEX <= marioGetPtr()->position.x) {
        xdone = TRUE;
    }
    if (dx < 0.0f && BeroEX >= marioGetPtr()->position.x) {
        xdone = TRUE;
    }
    ydone = FALSE;
    if (dy >= 0.0f && BeroEY <= marioGetPtr()->position.y) {
        ydone = TRUE;
    }
    if (dy < 0.0f && BeroEY >= marioGetPtr()->position.y) {
        ydone = TRUE;
    }
    if (xdone + ydone != 2) {
        return EVT_RETURN_BLOCK;
    }
    marioGetPtr()->position.x = BeroEX;
    marioGetPtr()->position.y = BeroEY;
    marioGetPtr()->position.z = BeroEZ;
    return EVT_RETURN_DONE;
}

/** Replaces entrance args[0] (index or hit name) in BeroINFOARR with the BeroEntry pointer args[1]. */
// FAKE: `&newId != NULL` tests (always true); the plain `newId = 0` / `newId++` change the object
USER_FUNC(evt_bero_overwrite) {
    BeroEntry** temp, **table = BeroINFOARR;
    EvtWord* args = event->args;
    EvtWord id = evtGetValue(event, args[0]);
    s32 newId = id;
    const char *string, *test;
    if (id < 0 || id >= 16) {
        test = (const char*)id;
        if (&newId != NULL) {
            newId = 0;
        }
        temp = table;
        while (1) {
            string = (*temp)->hitName;
            if (!string || !strcmp(string, test)) {
                break;
            }
            temp++;
            if (&newId != NULL) {
                newId++;
            }
        }
    }
    BeroINFOARR[newId] = (BeroEntry*)evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Moves the 3D camera to BeroPT/BeroAT over args[0] ms with interpolation curve args[1]. */
USER_FUNC(evt_bero_cam3d_change) {
    EvtWord* args = event->args;
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    s32 time = evtGetValue(event, args[0]);
    s32 curve = evtGetValue(event, args[1]);
    cam->presetPos = cam->cameraPos;
    cam->presetAt = cam->cameraAt;
    cam->storedPos = BeroPT;
    cam->storedAt = BeroAT;
    cam->startTimeA = gp->renderTime;
    cam->durationA = (u32)OSMillisecondsToTicks(time);
    cam->stateA = 3;
    cam->curveA = curve;
    return EVT_RETURN_DONE;
}

/** Resets the road camera and camera shift after same-map pipe travel. */
USER_FUNC(evt_bero_cam_shift_init) {
    camRoadReset();
    camShiftReset();
    return EVT_RETURN_DONE;
}

/** Clears the go signal before a door animation starts (see evt_bero_mario_go_wait). */
USER_FUNC(evt_bero_mario_go_init) {
    BeroMarioGO = FALSE;
    return EVT_RETURN_DONE;
}

/** Raised by the door animation scripts once the door is open enough for Mario to walk. */
USER_FUNC(evt_bero_mario_go) {
    BeroMarioGO = TRUE;
    return EVT_RETURN_DONE;
}

/** Blocks until evt_bero_mario_go has been called. */
USER_FUNC(evt_bero_mario_go_wait) {
    return BeroMarioGO == FALSE ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/**
 * args[0] = 1 when the destination map (LW(13)) is sys_01, else 0; the exits then wait a second.
 * sys_01 is the chapter title card ("Prologue", "Chapter 1" ...),
 * so this is the pause before a chapter starts.
 */
USER_FUNC(evt_bero_1stcheck) {
    EvtWord* args = event->args;
    if (strcmp((const char*)event->lwData[13], "sys_01") == 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Shared start of every exit trigger: LW(0) = 1 to abort if another transition runs;
 * otherwise stops NPCs, computes the walk, runs the exit script LW(11),
 * and aborts (restarting NPCs) if it returns LW(0) = 1.
 */
EVT_BEGIN(bero_case_init_check)
    CALL(evt_bero_exec_onoff, 1, 65536)
    CALL(evt_bero_exec_get, LW(0))
    AND_RAW(LW(0), 1)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_npc_change_fbat_mode, 7)
    CALL(evt_npc_stop_for_event)
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    SWITCH(LW(0))
        CASE_OR_EQUAL(BERO_TYPE_ROAD)
        CASE_OR_EQUAL(BERO_TYPE_DOOR)
        CALL(evt_bero_get_info_length, LW(1), LW(0))
        CALL(bero_get_position_normal, 0, LW(0))
        CASE_END
        CASE_OR_EQUAL(BERO_TYPE_NONE)
        CASE_OR_EQUAL(BERO_TYPE_GATE)
        CALL(bero_get_position_normal, 0, FLOAT(0.0f))
        CASE_END
        CASE_EQUAL(BERO_TYPE_PIPE)
        CALL(evt_bero_get_info_length, LW(1), LW(0))
        CALL(bero_get_position_pipe_pure, 0, LW(0))
    END_SWITCH
    CALL(evt_bero_set_now_number, LW(1))
    CALL(evt_bero_exec_onoff, 0, 1)
    SET_VALUE(LW(15), 0)
    CALL_CHILD(LW(11))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_npc_change_fbat_mode, 1)
        CALL(evt_npc_start_for_event)
        CALL(evt_bero_exec_onoff, 0, 65536)
        CALL(evt_bero_exec_onoff, 1, 1)
        SET_VALUE(LW(0), 1)
        RETURN
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(bero_case_exec_none)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(bero_case_init_check)
#else
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
#endif
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_onoff, 0, 65536)
    CALL(evt_bero_exec_onoff, 1, 1)
    CALL(evt_bero_exec_onoff, 0, 8)
    WAIT_FRAMES(2)
    CALL(evt_bero_1stcheck, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    CALL(evt_bero_mapchange, LW(13), LW(14))
    RETURN
EVT_END()

EVT_BEGIN(bero_case_exec_road)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(bero_case_init_check)
#else
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
#endif
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(5))
        MULTIPLY(LW(0), 45)
        CALL(evt_mario_set_force_dir, LW(0))
        CALL(evt_mario_wait_landon)
        CALL(bero_get_position_normal, 0, FLOAT(150.0f))
        PEEK_FLOAT(LW(10), PTR(&BeroEX))
        PEEK_FLOAT(LW(12), PTR(&BeroEZ))
        CALL(evt_mario_mov_pos2, LW(10), LW(12), FLOAT(150.0f))
        CALL(evt_bero_exec_onoff, 0, 65536)
        CALL(evt_bero_exec_onoff, 1, 1)
    END_INLINE_EVENT
    WAIT_MS(150)
    CALL(evt_bero_exec_onoff, 0, 8)
    WAIT_FRAMES(2)
    CALL(evt_bero_1stcheck, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    CALL(evt_bero_mapchange, LW(13), LW(14))
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(bero_case_exec_door)
    CALL_CHILD(bero_case_init_check)
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_attr_onoff, 0, 1, LW(3), HIT_ATTR_BERO)
    CALL(evt_bero_mario_go_init)
    SPAWN_EVENT(bero_play_out_anime)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(5))
        MULTIPLY(LW(0), 45)
        CALL(evt_mario_set_force_dir, LW(0))
        CALL(evt_mario_wait_landon)
        CALL(evt_bero_mario_go_wait)
        CALL(bero_get_position_normal, 0, FLOAT(150.0f))
PEEK_FLOAT(LW(10), PTR(&BeroEX))
        PEEK_FLOAT(LW(12), PTR(&BeroEZ))
        CALL(evt_mario_mov_pos2, LW(10), LW(12), FLOAT(150.0f))
        CALL(evt_bero_exec_onoff, 0, 65536)
        CALL(evt_bero_exec_onoff, 1, 1)
    END_INLINE_EVENT
    CALL(evt_bero_mario_go_wait)
    WAIT_MS(150)
    CALL(evt_bero_exec_onoff, 0, 8)
    WAIT_FRAMES(2)
    CALL(evt_bero_1stcheck, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    CALL(evt_bero_mapchange, LW(13), LW(14))
    RETURN
EVT_END()
#else
EVT_BEGIN(bero_case_exec_door)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_hitobj_attr_onoff, 0, 1, LW(3), HIT_ATTR_BERO)
    CALL(evt_bero_mario_go_init)
    SPAWN_EVENT(PTR(bero_play_out_anime))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), LW(5))
        MULTIPLY(LW(0), 45)
        CALL(evt_mario_set_force_dir, LW(0))
        CALL(evt_mario_wait_landon)
        CALL(evt_bero_mario_go_wait)
#if VERSION == VERSION_EU
        CALL(evt_sub_get_mapname, LW(10))
        IF_STR_EQUAL(LW(10), STRING("tou_04"))
            CALL(bero_get_position_normal, 0, FLOAT(40.0f))
        ELSE
            CALL(bero_get_position_normal, 0, FLOAT(150.0f))
        END_IF
#else
        CALL(bero_get_position_normal, 0, FLOAT(150.0f))
#endif
        PEEK_FLOAT(LW(10), PTR(&BeroEX))
        PEEK_FLOAT(LW(12), PTR(&BeroEZ))
        CALL(evt_mario_mov_pos2, LW(10), LW(12), FLOAT(150.0f))
        CALL(evt_bero_exec_onoff, 0, 65536)
        CALL(evt_bero_exec_onoff, 1, 1)
    END_INLINE_EVENT
    CALL(evt_bero_mario_go_wait)
    WAIT_MS(150)
    CALL(evt_bero_exec_onoff, 0, 8)
    WAIT_FRAMES(2)
    CALL(evt_bero_1stcheck, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    CALL(evt_bero_mapchange, LW(13), LW(14))
    RETURN
EVT_END()
#endif

USER_FUNC(evt_camera_change_event_from_road) {
    MarioWork* mario = marioGetPtr();
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 bank;
    CameraParams out;
    camShiftMain(cam, mario, &bank);
    camRoadMain(&out, mario->position.x, cam->marioPos.y, mario->position.z, mario->position.x, cam->marioPos.y, mario->position.z, bank);
    {
        Vec pt = {0.0f, 0.0f, 0.0f};
        pt.x = out.pos.x;
        pt.y = out.pos.y;
        pt.z = out.pos.z;
        {
            Vec at = {0.0f, 0.0f, 0.0f};
            at.x = out.at.x;
            at.y = out.at.y;
            at.z = out.at.z;
            cam->presetPos = cam->cameraPos;
            cam->presetAt = cam->cameraAt;
            cam->storedPos = pt;
            cam->storedAt = at;
            cam->startTimeA = gp->renderTime;
            cam->durationA = (u32)OSMillisecondsToTicks(500);
            cam->stateA = 3;
            cam->curveA = EASING_SINE_IN_OUT;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Pipe travel within the map (no destination map):
 * Mario goes down one pipe, the camera pans and he comes out of pipe LW(14),
 * switching background mode when the two pipes differ; the exit is re-registered.
 */
EVT_BEGIN(bero_case_exec_pipe_sub)
    // The camera leaves road mode. Alongside, unless Mario goes from a normal pipe into a background pipe,
    // the camera waits until he is inside and then glides over to the destination pipe.
    BEGIN_INLINE_EVENT
        CALL(evt_camera_change_event_from_road)
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(13), 0)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(0), LW(4))
            AND_RAW(LW(0), BERO_TYPE_BG_MODE)
            IF_INT_EQUAL(LW(0), 0)
                SET_VALUE(LF(0), 0)
                ELSE
                SET_VALUE(LF(0), 1)
            END_IF
            CALL(evt_bero_get_number, LW(14))
            CALL(evt_bero_set_now_number, LW(1))
            CALL(evt_bero_get_info_num, LW(1))
            SET_VALUE(LW(0), LW(4))
            AND_RAW(LW(0), BERO_TYPE_BG_MODE)
            IF_INT_EQUAL(LW(0), 0)
                SET_VALUE(LF(1), 0)
                ELSE
                SET_VALUE(LF(1), 1)
            END_IF
            SET_VALUE(LF(2), 0)
            IF_INT_EQUAL(LF(0), 0)
                IF_INT_EQUAL(LF(1), 1)
                    SET_VALUE(LF(2), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LF(2), 0)
                POKE_INT(0, PTR(&BeroCAM))
                CALL(evt_bero_get_info_length, LW(1), LW(10))
                CALL(bero_get_position_pipe2, 1, LW(10))
                LOOP(0)
                    PEEK_INT(LW(0), PTR(&BeroCAM))
                    IF_INT_EQUAL(LW(0), 1)
                        BREAK_LOOP
                    END_IF
                    WAIT_FRAMES(1)
                END_LOOP
                CALL(evt_bero_cam3d_change, 500, EASING_SINE_IN_OUT)
            END_IF
        END_INLINE_EVENT
    END_IF
    // Mario sinks into the pipe with the pipe sound, posed for the pipe's direction.
    CALL(evt_bero_disppos_save)
    CALL(evt_bero_get_info_length, LW(1), LW(10))
    CALL(bero_get_position_pipe_pure, 0, LW(10))
    CALL(bero_set_disp_position_pipe)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_DOKAN1"), LW(0), LW(1), LW(2), 0)
    SWITCH(LW(5))
        CASE_EQUAL(BERO_DIR_W)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(BERO_DIR_E)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(8)
        CALL(evt_mario_paper_pose_dokan, 2)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(9)
        CALL(evt_mario_paper_pose_dokan, 3)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
    END_SWITCH
    // He is inside: let the camera move.
    POKE_INT(1, PTR(&BeroCAM))
    // After a moment the destination pipe's own entry script runs, and Mario is moved under that pipe.
    IF_INT_EQUAL(LW(13), 0)
        SET_VALUE(LW(0), LW(4))
        AND_RAW(LW(0), BERO_TYPE_BG_MODE)
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LF(0), 0)
            ELSE
            SET_VALUE(LF(0), 1)
        END_IF
        WAIT_MS(500)
        CALL(evt_bero_get_number, LW(14))
        CALL(evt_bero_get_info_num, LW(1))
        CALL_CHILD(LW(7))
        SET_VALUE(LW(0), LW(4))
        AND_RAW(LW(0), BERO_TYPE_BG_MODE)
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LF(1), 0)
            ELSE
            SET_VALUE(LF(1), 1)
        END_IF
        CALL(evt_bero_disppos_load)
        CALL(evt_mario_paper_pose_off)
        CALL(evt_bero_disppos_save)
        CALL(evt_bero_exec_get, LW(0))
        IF_BITS_CLEAR(LW(0), 4)
            CALL(evt_bero_exec_onoff, 0, 2)
        END_IF
        CALL(evt_bero_get_info_length, LW(1), LW(10))
        CALL(bero_get_position_pipe_pure, 1, LW(10))
        CALL(bero_set_disp_position_pipe)
        // He pops out of the destination pipe in its layer (background or normal) with the pipe sound,
        // gets control back, and the pipe he left re-arms once he steps off.
        WAIT_MS(500)
        IF_INT_NOT_EQUAL(LF(1), 0)
            CALL(evt_mario_bgmode_on)
            ELSE
            CALL(evt_mario_bgmode_off)
        END_IF
        PEEK_FLOAT(LW(0), PTR(&BeroEX))
        PEEK_FLOAT(LW(1), PTR(&BeroEY))
        PEEK_FLOAT(LW(2), PTR(&BeroEZ))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_DOKAN1"), LW(0), LW(1), LW(2), 0)
        CALL_CHILD(PTR(evt_bero_info_run_pipe_sub00))
        CALL(evt_bero_exec_get, LW(0))
        IF_BITS_CLEAR(LW(0), 4)
            CALL(evt_bero_exec_onoff, 1, 2)
        END_IF
        CALL(evt_bero_disppos_load)
        CALL(evt_mario_cont_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        SET_VALUE(LW(0), LW(3))
        CALL_CHILD(PTR(bero_case_switch_off))
        SPAWN_EVENT(PTR(evt_bero_nonride_check))
        CALL(evt_bero_cam_shift_init)
        CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
        // With a destination map (the pipe exit never spawns this case) he just takes the entrance's layer.
        ELSE
        CALL(evt_bero_disppos_load)
        CALL(evt_mario_paper_pose_off)
        SET_VALUE(LW(0), LW(4))
        AND_RAW(LW(0), BERO_TYPE_BG_MODE)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_mario_bgmode_on)
            ELSE
            CALL(evt_mario_bgmode_off)
        END_IF
    END_IF
    // Field enemies resume and the exits are live again.
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_start_for_event)
    CALL(evt_bero_exec_onoff, 0, 65536)
    CALL(evt_bero_exec_onoff, 1, 1)
    RETURN
EVT_END()

/**
 * Exit trigger for pipes (only in normal mode):
 * Mario slides in, the pipe-direction fade is set unless type bit 0x2000, then the map changes.
 * Pipes with no destination map run bero_case_exec_pipe_sub.
 */
EVT_BEGIN(bero_case_exec_pipe)
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(bero_case_init_check)
#else
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
#endif
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_wait_landon)
    CALL(evt_mario_cont_onoff, 0)
    IF_INT_EQUAL(LW(13), 0)
        SPAWN_EVENT(PTR(bero_case_exec_pipe_sub))
        RETURN
    END_IF
    CALL(evt_mario_cam_off)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_DOKAN1"), LW(0), LW(1), LW(2), 0)
    SWITCH(LW(5))
        CASE_EQUAL(BERO_DIR_W)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(BERO_DIR_E)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(8)
        CALL(evt_mario_paper_pose_dokan, 2)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(9)
        CALL(evt_mario_paper_pose_dokan, 3)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
    END_SWITCH
    IF_BITS_CLEAR(LW(4), BERO_TYPE_FLAG_NO_FADE)
        SWITCH(LW(5))
            CASE_EQUAL(BERO_DIR_W)
            CALL(evt_fade_set_mapchange_type, 1, FADE_PIPE_LEFT_A1, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, FADE_PIPE_LEFT_A1, 0, -1, -1)
            CASE_EQUAL(BERO_DIR_E)
            CALL(evt_fade_set_mapchange_type, 1, FADE_PIPE_RIGHT_A1, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, FADE_PIPE_RIGHT_A1, 0, -1, -1)
            CASE_EQUAL(BERO_DIR_UP)
            CALL(evt_fade_set_mapchange_type, 1, FADE_PIPE_UP_A1, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, FADE_PIPE_UP_A1, 0, -1, -1)
            CASE_EQUAL(BERO_DIR_DOWN)
            CALL(evt_fade_set_mapchange_type, 1, FADE_PIPE_DOWN_A1, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, FADE_PIPE_DOWN_A1, 0, -1, -1)
        END_SWITCH
    END_IF
    CALL(evt_bero_exec_onoff, 0, 8)
    WAIT_FRAMES(2)
    CALL(evt_bero_1stcheck, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    CALL(evt_bero_mapchange, LW(13), LW(14))
    RETURN
EVT_END()

/** Exit trigger for gates: after bero_case_init_check, changes map straight away. */
EVT_BEGIN(bero_case_exec_gate)
#if VERSION <= VERSION_DEMO_US
    CALL_CHILD(bero_case_init_check)
#else
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
#endif
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_bero_exec_onoff, 0, 65536)
    CALL(evt_bero_exec_onoff, 1, 1)
    CALL(evt_bero_exec_onoff, 0, 8)
    WAIT_FRAMES(2)
    CALL(evt_bero_1stcheck, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(1000)
    END_IF
    CALL(evt_bero_mapchange, LW(13), LW(14))
    RETURN
EVT_END()

/**
 * Registers the exit trigger (case event) for every entrance not yet registered, by type,
 * saves the case IDs and marks them registered. Runs once the arrival finishes.
 */
EVT_BEGIN(bero_case_entry)
    PEEK_INT(LW(2), PTR(&BeroNUM))
    SET_VALUE(LW(1), 0)
    LOOP(0)
        CALL(evt_bero_get_info_num, LW(1))
        IF_INT_EQUAL(LW(9), -1)
            SET_VALUE(LW(0), LW(4))
            AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
            SWITCH(LW(0))
                CASE_EQUAL(BERO_TYPE_NONE)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_none), LW(15))
                CASE_EQUAL(BERO_TYPE_ROAD)
                CALL(evt_hitobj_attr_onoff, 0, 1, LW(3), HIT_ATTR_BERO)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_road), LW(15))
                CASE_EQUAL(BERO_TYPE_DOOR)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_door), LW(15))
                CASE_EQUAL(BERO_TYPE_PIPE)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_pipe), LW(15))
                CASE_EQUAL(BERO_TYPE_GATE)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_gate), LW(15))
            END_SWITCH
            CALL(evt_bero_case_id_save, LW(1), LW(15))
            BEGIN_INLINE_EVENT
                CALL(evt_bero_switch_on, LW(1))
            END_INLINE_EVENT
        END_IF
        ADD(LW(1), 1)
        IF_INT_EQUAL(LW(1), LW(2))
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

/** Registers the exit trigger for the single entrance loaded in LW(1)-LW(15) and saves its case ID. */
EVT_BEGIN(bero_case_entry_sub)
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    SWITCH(LW(0))
        CASE_EQUAL(BERO_TYPE_NONE)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_none), LW(15))
        CASE_EQUAL(BERO_TYPE_ROAD)
        CALL(evt_hitobj_attr_onoff, 0, 1, LW(3), HIT_ATTR_BERO)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_road), LW(15))
        CASE_EQUAL(BERO_TYPE_DOOR)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_door), LW(15))
        CASE_EQUAL(BERO_TYPE_PIPE)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_pipe), LW(15))
        CASE_EQUAL(BERO_TYPE_GATE)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_gate), LW(15))
    END_SWITCH
    CALL(evt_bero_case_id_save, LW(1), LW(15))
    RETURN
EVT_END()

/** Retail: same, with the "registered" debug message only on a real registration. */
EVT_BEGIN(bero_case_switch_on)
    CALL(evt_bero_switch_on, LW(0))
    CALL(evt_bero_get_info_num, LW(1))
#if VERSION <= VERSION_DEMO_US
    DEBUG_PUT_MSG(STRING("ベロ登録"))
#endif
    IF_INT_EQUAL(LW(0), 0)
#if VERSION <= VERSION_DEMO_US
        CALL_CHILD(bero_case_entry_sub)
#else
        DEBUG_PUT_MSG(STRING("\203x\203\215\223o\230^"))
        CALL_CHILD(PTR(bero_case_entry_sub))
#endif
        ELSE
        DEBUG_PUT_MSG(STRING("\203x\203\215\202\314\217o\202\304\215s\202\255\203f\201[\203^\202\314\223\361\217d\223o\230^\226h\216~\202\360\202\265\202\304\202\242\202\334\202\267"))
    END_IF
    RETURN
EVT_END()

/** Retail: same, and also clears hit attribute 0x800000 on the entrance's hit object. */
EVT_BEGIN(bero_case_switch_off)
    CALL(evt_bero_switch_off, LW(0))
    CALL(evt_bero_get_info_num, LW(1))
#if VERSION <= VERSION_DEMO_US
    DEBUG_PUT_MSG(STRING("ベロ削除"))
#endif
    IF_INT_EQUAL(LW(0), 0)
#if VERSION > VERSION_DEMO_US
        DEBUG_PUT_MSG(STRING("\203x\203\215\215\355\217\234\202\265\202\334\202\265\202\275"))
#endif
        CALL(evt_bero_case_id_load, LW(1), LW(15))
        CALL(evt_del_case_evt, 0, LW(15))
#if VERSION > VERSION_DEMO_US
        CALL(evt_hitobj_attr_onoff, 0, 0, LW(3), HIT_ATTR_BERO)
#endif
        ELSE
#if VERSION <= VERSION_DEMO_US
        DEBUG_PUT_MSG(STRING("ベロの出て行くデータの二重削除防止をしています"))
#else
        DEBUG_PUT_MSG(STRING("\203x\203\215\202\314\215\355\217\234\227\\\226\361\202\340\202\265\202\255\202\315\203x\203\215\202\314\217o\202\304\215s\202\255\203f\201[\203^\202\314\223\361\217d\215\355\217\234\226h\216~\202\305\202\267"))
#endif
    END_IF
    RETURN
EVT_END()

/**
 * Arrival by road: Mario walks from the map edge to the end point
 * while partners follow closely (homing distance 20, back to 50 after).
 */
#if VERSION <= VERSION_DEMO_US
/** Arrival through a "none" entrance (type 0xF00): no walk-in, just gives control back to the player. */
EVT_BEGIN(evt_bero_info_run_none)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
#endif
/**
 * Arrival by road: Mario walks from the map edge to the end point
 * while partners follow closely (homing distance 20, back to 50 after).
 */
EVT_BEGIN(evt_bero_info_run_road)
    PEEK_FLOAT(LW(13), PTR(&BeroEX))
    PEEK_FLOAT(LW(14), PTR(&BeroEY))
    PEEK_FLOAT(LW(15), PTR(&BeroEZ))
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_party_set_homing_dist, 1, FLOAT(20.0f))
    CALL(evt_mario_mov_pos2, LW(13), LW(15), FLOAT(150.0f))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_party_set_homing_dist, 0, FLOAT(50.0f))
    CALL(evt_party_set_homing_dist, 1, FLOAT(50.0f))
    RETURN
EVT_END()

/** Arrival through a door: plays the door animation, then Mario and partners walk in. */
EVT_BEGIN(evt_bero_info_run_door)
    CALL(evt_bero_mario_go_init)
    SPAWN_EVENT(PTR(bero_play_enter_anime))
    CALL(evt_bero_mario_go_wait)
    PEEK_FLOAT(LW(13), PTR(&BeroEX))
    PEEK_FLOAT(LW(14), PTR(&BeroEY))
    PEEK_FLOAT(LW(15), PTR(&BeroEZ))
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_party_set_homing_dist, 1, FLOAT(20.0f))
    CALL(evt_mario_party_bero_move, LW(13), LW(15), FLOAT(150.0f))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(evt_party_set_homing_dist, 0, FLOAT(50.0f))
    CALL(evt_party_set_homing_dist, 1, FLOAT(50.0f))
    RETURN
EVT_END()

/** Mario rises, drops or slides out of a pipe by direction LW(5), then ends the pipe pose. */
EVT_BEGIN(evt_bero_info_run_pipe_sub00)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    SWITCH(LW(5))
        CASE_EQUAL(BERO_DIR_W)
        CALL(evt_mario_set_dispdir, 180)
        CALL(evt_mario_paper_pose_dokan, 4)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(BERO_DIR_E)
        CALL(evt_mario_set_dispdir, 0)
        CALL(evt_mario_paper_pose_dokan, 4)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(BERO_DIR_DOWN)
        CALL(evt_mario_paper_pose_dokan, 1)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(BERO_DIR_UP)
        CALL(evt_mario_paper_pose_dokan, 0)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
    END_SWITCH
    CALL(evt_mario_dokan_end)
    RETURN
EVT_END()

/**
 * Arrival from a pipe: after the fade, Mario comes out
 * and the pipe's exit is re-registered once he steps off it (evt_bero_nonride_check).
 */
EVT_BEGIN(evt_bero_info_run_pipe)
    PEEK_FLOAT(LW(0), PTR(&BeroEX))
    PEEK_FLOAT(LW(1), PTR(&BeroEY))
    PEEK_FLOAT(LW(2), PTR(&BeroEZ))
    CALL(evt_cam_road_reset2, LW(0), LW(1), LW(2))
    CALL(evt_mario_cam_off)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_DOKAN1"), LW(0), LW(1), LW(2), 0)
    CALL_CHILD(PTR(evt_bero_info_run_pipe_sub00))
    CALL(evt_mario_cont_onoff, 1)
    SET_VALUE(LW(0), LW(3))
    CALL_CHILD(PTR(bero_case_switch_off))
    SPAWN_EVENT(PTR(evt_bero_nonride_check))
    CALL(evt_mario_cam_on)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map-entry script run after evt_bero_get_info: on a save load just registers exits;
 * otherwise sets up Mario (background mode, partners, pipe pose), places him,
 * sets the pipe fade and spawns the walk-in.
 */
#if VERSION <= VERSION_DEMO_US
/** Arrival through a gate entrance (type 3): no walk-in, just gives control back to the player. */
EVT_BEGIN(evt_bero_info_run_gate)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()
#endif
/**
 * Map-entry script run after evt_bero_get_info: on a save load just registers exits;
 * otherwise sets up Mario (background mode, partners, pipe pose), places him,
 * sets the pipe fade and spawns the walk-in.
 */
EVT_BEGIN(evt_bero_info_run)
    // Find the entrance Mario arrived by and whether he is in paper form.
    CALL(evt_bero_get_into_info)
    CALL(evt_bero_set_now_number, LW(1))
    CALL(evt_bero_read_mario_pera)
    CALL(evt_sub_get_system_flag, LW(0))
    // Loading a save: no walk-in; only arm the exits and start the NPCs.
    IF_BITS_SET(LW(0), GW_FLAG_SAVE_LOADED)
#if VERSION > VERSION_DEMO_US
        DEBUG_PUT_MSG(STRING("\203Z\201[\203u\203u\203\215\203b\203N\202\251\202\347\202\314\203\215\201[\203h\216\236\202\314\217\210\227\235"))
#endif
        CALL(evt_bero_exec_onoff, 0, 1)
        WAIT_FRAMES(1)
        CALL(evt_bero_exec_get, LW(0))
        IF_BITS_CLEAR(LW(0), 4)
            CALL(evt_bero_exec_onoff, 0, 2)
        END_IF
        WAIT_FRAMES(1)
        CALL(evt_bero_exec_get, LW(0))
        IF_BITS_CLEAR(LW(0), 4)
            CALL(evt_bero_exec_onoff, 1, 2)
        END_IF
        CALL(evt_npc_change_fbat_mode, 1)
        CALL(evt_npc_start_for_event)
        CALL(evt_bero_exec_onoff, 1, 1)
        CALL(evt_bero_exec_onoff, 0, 65536)
        CALL_CHILD(PTR(bero_case_entry))
        RETURN
    END_IF
    // Normal arrival: lock the controls. A background entrance hides the partners;
    // otherwise they follow Mario in, out of the pipe for a pipe entrance.
    CALL(evt_bero_exec_get, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_bero_exec_onoff, 0, 1)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_BG_MODE)
    IF_INT_NOT_EQUAL(LW(0), 0)
        DEBUG_PUT_MSG(STRING("\224w\214i\202\311\223\374\202\351\202\306\202\253\202\314\203}\203\212\203I\202\314\217\211\212\372\217\210\227\235"))
        CALL(evt_mario_bgmode_on)
        CALL(evt_mario_kill_party, 0)
        CALL(evt_mario_kill_party, 1)
        CALL(evt_mario_cam_y_off)
        ELSE
        DEBUG_PUT_MSG(STRING("\225\201\222\312\202\311\223\374\202\351\202\306\202\253\202\314\203}\203\212\203I\202\314\217\211\212\372\217\210\227\235"))
        CALL(evt_mario_bgmode_off)
        SET_VALUE(LW(0), LW(4))
        AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
        IF_INT_EQUAL(LW(0), BERO_TYPE_PIPE)
            CALL(evt_mario_set_prev_party_dokan, 0)
            CALL(evt_mario_set_prev_party_dokan, 1)
            ELSE
            CALL(evt_mario_set_prev_party, 0)
            CALL(evt_mario_set_prev_party, 1)
        END_IF
    END_IF
    // Pipe entrance: Mario starts tucked inside the pipe, facing out along its direction.
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    IF_INT_EQUAL(LW(0), BERO_TYPE_PIPE)
        SWITCH(LW(5))
            CASE_EQUAL(BERO_DIR_W)
            CALL(evt_mario_dokan_prepare, 4)
            CASE_EQUAL(BERO_DIR_E)
            CALL(evt_mario_dokan_prepare, 4)
            CASE_EQUAL(BERO_DIR_DOWN)
            CALL(evt_mario_dokan_prepare, 1)
            CASE_EQUAL(BERO_DIR_UP)
            CALL(evt_mario_dokan_prepare, 0)
        END_SWITCH
    END_IF
    // Place Mario and the partners at the entrance, choose the pipe fade-in for its direction
    // and start the walk-in.
    CALL_CHILD(PTR(evt_bero_initial_pos))
    IF_BITS_CLEAR(LW(4), BERO_TYPE_FLAG_NO_FADE)
        SET_VALUE(LW(0), LW(4))
        AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
        IF_INT_EQUAL(LW(0), BERO_TYPE_PIPE)
            SWITCH(LW(5))
                CASE_EQUAL(BERO_DIR_W)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, FADE_PIPE_LEFT_A2, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, FADE_PIPE_LEFT_A2, 0)
                CASE_EQUAL(BERO_DIR_E)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, FADE_PIPE_RIGHT_A2, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, FADE_PIPE_RIGHT_A2, 0)
                CASE_EQUAL(BERO_DIR_UP)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, FADE_PIPE_UP_A2, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, FADE_PIPE_UP_A2, 0)
                CASE_EQUAL(BERO_DIR_DOWN)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, FADE_PIPE_DOWN_A2, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, FADE_PIPE_DOWN_A2, 0)
            END_SWITCH
        END_IF
    END_IF
    DEBUG_PUT_MSG(STRING("\202\331\202\251\202\314\217\211\212\372\220\335\222\350\212J\216n"))
    SPAWN_EVENT(PTR(evt_bero_mario_enter))
    RETURN
EVT_END()

/** Places Mario and both partners at the arrival entrance's start point and resets the road camera. */
EVT_BEGIN(evt_bero_initial_pos)
    CALL(evt_bero_get_into_info)
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    SWITCH(LW(0))
        CASE_EQUAL(BERO_TYPE_NONE)
        CASE_EQUAL(BERO_TYPE_GATE)
        CALL(bero_get_position_normal, 0, FLOAT(0.0f))
        PEEK_FLOAT(LW(10), PTR(&BeroSX))
        PEEK_FLOAT(LW(11), PTR(&BeroSY))
        PEEK_FLOAT(LW(12), PTR(&BeroSZ))
        CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 1, LW(10), LW(11), LW(12))
        CASE_END
        CASE_EQUAL(BERO_TYPE_ROAD)
        CALL(evt_bero_get_info_length, LW(1), LW(10))
        CALL(bero_get_position_normal, 1, LW(10))
        PEEK_FLOAT(LW(10), PTR(&BeroSX))
        PEEK_FLOAT(LW(11), PTR(&BeroSY))
        PEEK_FLOAT(LW(12), PTR(&BeroSZ))
        CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 1, LW(10), LW(11), LW(12))
        CASE_EQUAL(BERO_TYPE_DOOR)
        CALL(evt_bero_get_info_length, LW(1), LW(10))
        CALL(bero_get_position_normal, 1, LW(10))
        PEEK_FLOAT(LW(10), PTR(&BeroSX))
        PEEK_FLOAT(LW(11), PTR(&BeroSY))
        PEEK_FLOAT(LW(12), PTR(&BeroSZ))
        CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 1, LW(10), LW(11), LW(12))
        CASE_EQUAL(BERO_TYPE_PIPE)
        CALL(evt_bero_get_info_length, LW(1), LW(10))
        CALL(bero_get_position_pipe_pure, 1, LW(10))
        PEEK_FLOAT(LW(10), PTR(&BeroSX))
        PEEK_FLOAT(LW(11), PTR(&BeroSY))
        PEEK_FLOAT(LW(12), PTR(&BeroSZ))
        CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 1, LW(10), LW(11), LW(12))
    END_SWITCH
    CALL(evt_cam_road_reset)
    RETURN
EVT_END()

/**
 * Runs the entrance's enter script (LW(0) = 1 skips the walk-in), then the arrival for its type,
 * and finally registers every exit trigger and clears the transition bit.
 */
EVT_BEGIN(evt_bero_mario_enter)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_npc_change_fbat_mode, 7)
    CALL(evt_npc_stop_for_event)
    CALL_CHILD(LW(7))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_mario_key_onoff, 1)
        GOTO(100)
    END_IF
    CALL(evt_bero_exec_get, LW(0))
    IF_BITS_CLEAR(LW(0), 4)
        CALL(evt_bero_exec_onoff, 0, 2)
    END_IF
    CALL(evt_bero_get_into_info)
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    SWITCH(LW(0))
        CASE_EQUAL(BERO_TYPE_NONE)
        CALL_CHILD(PTR(evt_bero_info_run_none))
        CASE_EQUAL(BERO_TYPE_ROAD)
        CALL_CHILD(PTR(evt_bero_info_run_road))
        CASE_EQUAL(BERO_TYPE_DOOR)
        CALL_CHILD(PTR(evt_bero_info_run_door))
        CASE_EQUAL(BERO_TYPE_PIPE)
        CALL_CHILD(PTR(evt_bero_info_run_pipe))
        CASE_EQUAL(BERO_TYPE_GATE)
        CALL_CHILD(PTR(evt_bero_info_run_gate))
        CASE_DEFAULT
        CALL(evt_mario_key_onoff, 1)
    END_SWITCH
    CALL(evt_bero_exec_get, LW(0))
    IF_BITS_CLEAR(LW(0), 4)
        CALL(evt_bero_exec_onoff, 1, 2)
    END_IF
    LABEL(100)
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_start_for_event)
    CALL_CHILD(PTR(bero_case_entry))
    CALL(evt_bero_exec_onoff, 1, 1)
    CALL(evt_bero_exec_onoff, 0, 65536)
    RETURN
EVT_END()

// .bss definitions last (emitted in reverse order: BeroID, BeroSW, BeroINFOARR)
BeroEntry* BeroINFOARR[16];
s32 BeroSW[16];
s32 BeroID[16];
#if VERSION <= VERSION_DEMO_US
Vec BeroPT, BeroAT;
#endif

