#include "evt/evt_bero.h"
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
Vec BeroPT, BeroAT;
BOOL BeroMarioGO;

BeroEntry bero_null[] = {
    {
        NULL, // hitName
        3840, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
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

EVT_BEGIN(default_entevt)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(default_outevt)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(evt_bero_info_run_none)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_bero_info_run_gate)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

// s32 x, s32 y, s32 z (new in retail)
USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_on_bero_entry) {
    EvtWord* args = event->args;
    s32 x, y, z;

    x = evtGetValue(event, args[0]);
    y = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    marioSetBottomlessRespawnPosOnBeroEntry((f32)x, (f32)y, (f32)z);
    return EVT_RETURN_DONE;
}

// no args (new in retail)
USER_FUNC(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos) {
    MarioWork* wp = marioGetPtr();
    marioSetBottomlessRespawnPosOnBeroEntry(wp->position.x, wp->position.y, wp->position.z);
    return EVT_RETURN_DONE;
}

s32 bero_get_BeroEXEC(void) {
    return BeroEXEC;
}

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

BeroEntry** bero_get_ptr(void) {
    return BeroINFOARR;
}

void bero_clear_Offset(void) {
    BeroOX = 0.0f;
    BeroOY = 0.0f;
    BeroOZ = 0.0f;
}

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

//s32 retIndex
USER_FUNC(evt_bero_get_entername) {
    evtSetValue(event, *event->args, (EvtWord)&gp->beroEnterName);
    return EVT_RETURN_DONE;
}

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

//s32 retIndex
USER_FUNC(evt_bero_exec_get) {
    evtSetValue(event, *event->args, BeroEXEC);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_exec_wait) {
    return evtGetValue(event, *event->args) & BeroEXEC ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

//f32 retX, f32 retY, f32 retZ
USER_FUNC(evt_bero_get_start_position) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], BeroSX);
    evtSetFloat(event, args[1], BeroSY);
    evtSetFloat(event, args[2], BeroSZ);
    return EVT_RETURN_DONE;
}

//f32 retX, f32 retY, f32 retZ
USER_FUNC(evt_bero_get_end_position) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], BeroEX);
    evtSetFloat(event, args[1], BeroEY);
    evtSetFloat(event, args[2], BeroEZ);
    return EVT_RETURN_DONE;
}

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
        case 0:
        case 3:
        case 0xF00:
            defaultLength = 30.0f;
            break;
        case 1:
            defaultLength = 60.0f;
            break;
        case 2:
            defaultLength = 60.0f;
            break;
        default:
            defaultLength = 60.0f;
            break;
    }
    evtSetFloat(event, args[1], defaultLength);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_info_kinddir) {
    BeroEntry* bero;
    EvtWord* args = event->args;

    bero = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], bero->type);
    evtSetValue(event, args[2], bero->sfxId);
    evtSetValue(event, args[3], bero->directionId);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_info_nextarea) {
    BeroEntry* bero;
    EvtWord* args = event->args;

    bero = BeroINFOARR[evtGetValue(event, args[0])];
    evtSetValue(event, args[1], (EvtWord)bero->nextMapName);
    evtSetValue(event, args[2], (EvtWord)bero->nextBeroName);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_set_now_number) {
    BeroNOWNUM = evtGetValue(event, *event->args);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_get_now_number) {
    evtSetValue(event, *event->args, BeroNOWNUM);
    return EVT_RETURN_DONE;
}

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
    case 20000:
        // FAKE: dead test (kind is 20000 in this case); removing it changes the object
        if (kind == 10000) {
            kind = 10000;
        } else {
        switch (bero->type & BERO_TYPE_KIND_MASK) {
            case 0:
            case 1:
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
                i = 10000;
                break;
        }
        kind = i;
        }
        break;
    }
    return kind;
}

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

USER_FUNC(evt_bero_id_filter) {
    EvtWord* args = event->args;
    evtSetValue(event, args[0], bero_id_filter(evtGetValue(event, args[0])));
    return EVT_RETURN_DONE;
}

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
            case 0xF00:
                event->lwData[10] = 0; // CASE_STAND_ON
                break;
            case 0:
                event->lwData[10] = 10; // CASE_PUSH
                break;
            case 1:
                event->lwData[10] = 9; // CASE_FRONT_A
                break;
            case 2:
                event->lwData[10] = 4; // CASE_JUMP_FROM_A
                break;
            case 3:
                event->lwData[10] = 7; // CASE_STAND_ON_STICK_UP
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
    MarioWork* wp = marioGetPtr();
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
    if (pos[0] == 100000) {
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
    marioSetBottomlessRespawnPosOnBeroEntry(end.x, end.y, end.z);
    if (wp->currMotionId == MARIO_MOTION_SHIP) {
        marioReloadMapOnBottomlessOn();
    }
    return EVT_RETURN_DONE;
}

void bero_get_position_pipe_sub(s32 id, s32 a, EvtWord name, s32 kind, Vec* start, Vec* end) {
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 ofs;
    hz = hy = hx = 0.0f;
    dist = 10000.0f;
    ofs = 20.0f;
    switch (kind) {
    case 6:
        a = -a;
        ofs = -ofs;
    case 2:
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
    case 9:
        a = -a;
    case 8:
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
    case 10000:
    default:
        hitObjGetPos((const char*)name, start);
        hitObjGetPos((const char*)name, end);
        break;
    }
}

USER_FUNC(bero_get_position_pipe_pure) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 a = evtGetFloat(event, args[1]);
    s32 kind = event->lwData[5];
    Vec start, end;
    bero_get_position_pipe_sub(id, a, event->lwData[3], kind, &start, &end);
    BeroSX = start.x;
    BeroSY = kind == 8 ? start.y - 37.5f : start.y;
    BeroSZ = start.z;
    BeroEX = end.x;
    BeroEY = (f32)(kind == 8 ? end.y - 37.5f : end.y);
    BeroEZ = end.z;
    marioSetBottomlessRespawnPosOnBeroEntry(BeroEX, BeroEY, BeroEZ);
    return EVT_RETURN_DONE;
}

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

USER_FUNC(evt_bero_case_id_save) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    BeroID[id] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_case_id_load) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], BeroID[id]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_disppos_save) {
    BeroOX = marioGetPtr()->dispOfs.x;
    BeroOY = marioGetPtr()->dispOfs.y;
    BeroOZ = marioGetPtr()->dispOfs.z;
    return EVT_RETURN_DONE;
}

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

USER_FUNC(evt_bero_cam_shift_init) {
    camRoadReset();
    camShiftReset();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_init) {
    BeroMarioGO = FALSE;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go) {
    BeroMarioGO = TRUE;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_mario_go_wait) {
    return BeroMarioGO == FALSE ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(evt_bero_1stcheck) {
    EvtWord* args = event->args;
    if (strcmp((const char*)event->lwData[13], "sys_01") == 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

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
        CASE_OR_EQUAL(0)
        CASE_OR_EQUAL(1)
        CALL(evt_bero_get_info_length, LW(1), LW(0))
        CALL(bero_get_position_normal, 0, LW(0))
        CASE_END
        CASE_OR_EQUAL(3840)
        CASE_OR_EQUAL(3)
        CALL(bero_get_position_normal, 0, FLOAT(0.0f))
        CASE_END
        CASE_EQUAL(2)
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
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
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
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
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

EVT_BEGIN(bero_case_exec_pipe_sub)
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
    CALL(evt_bero_disppos_save)
    CALL(evt_bero_get_info_length, LW(1), LW(10))
    CALL(bero_get_position_pipe_pure, 0, LW(10))
    CALL(bero_set_disp_position_pipe)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_DOKAN1"), LW(0), LW(1), LW(2), 0)
    SWITCH(LW(5))
        CASE_EQUAL(6)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(2)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(8)
        CALL(evt_mario_paper_pose_dokan, 2)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(9)
        CALL(evt_mario_paper_pose_dokan, 3)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
    END_SWITCH
    POKE_INT(1, PTR(&BeroCAM))
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
    CALL(evt_npc_change_fbat_mode, 1)
    CALL(evt_npc_start_for_event)
    CALL(evt_bero_exec_onoff, 0, 65536)
    CALL(evt_bero_exec_onoff, 1, 1)
    RETURN
EVT_END()

EVT_BEGIN(bero_case_exec_pipe)
    CALL(evt_mario_get_mode, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
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
        CASE_EQUAL(6)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(2)
        CALL(evt_mario_paper_pose_dokan, 5)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(8)
        CALL(evt_mario_paper_pose_dokan, 2)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(9)
        CALL(evt_mario_paper_pose_dokan, 3)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
    END_SWITCH
    IF_BITS_CLEAR(LW(4), 8192)
        SWITCH(LW(5))
            CASE_EQUAL(6)
            CALL(evt_fade_set_mapchange_type, 1, 45, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, 45, 0, -1, -1)
            CASE_EQUAL(2)
            CALL(evt_fade_set_mapchange_type, 1, 47, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, 47, 0, -1, -1)
            CASE_EQUAL(8)
            CALL(evt_fade_set_mapchange_type, 1, 41, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, 41, 0, -1, -1)
            CASE_EQUAL(9)
            CALL(evt_fade_set_mapchange_type, 1, 43, 0, -1, -1)
            CALL(evt_fade_set_mapchange_type, 0, 43, 0, -1, -1)
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

EVT_BEGIN(bero_case_exec_gate)
    CALL(evt_mario_key_onoff, 0)
    CALL_CHILD(PTR(bero_case_init_check))
    CALL(evt_mario_key_onoff, 1)
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

EVT_BEGIN(bero_case_entry)
    PEEK_INT(LW(2), PTR(&BeroNUM))
    SET_VALUE(LW(1), 0)
    LOOP(0)
        CALL(evt_bero_get_info_num, LW(1))
        IF_INT_EQUAL(LW(9), -1)
            SET_VALUE(LW(0), LW(4))
            AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
            SWITCH(LW(0))
                CASE_EQUAL(3840)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_none), LW(15))
                CASE_EQUAL(0)
                CALL(evt_hitobj_attr_onoff, 0, 1, LW(3), HIT_ATTR_BERO)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_road), LW(15))
                CASE_EQUAL(1)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_door), LW(15))
                CASE_EQUAL(2)
                CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_pipe), LW(15))
                CASE_EQUAL(3)
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

EVT_BEGIN(bero_case_entry_sub)
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    SWITCH(LW(0))
        CASE_EQUAL(3840)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_none), LW(15))
        CASE_EQUAL(0)
        CALL(evt_hitobj_attr_onoff, 0, 1, LW(3), HIT_ATTR_BERO)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_road), LW(15))
        CASE_EQUAL(1)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_door), LW(15))
        CASE_EQUAL(2)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_pipe), LW(15))
        CASE_EQUAL(3)
        CALL(evt_run_case_evt_bero, LW(10), 1, LW(3), 0, PTR(bero_case_exec_gate), LW(15))
    END_SWITCH
    CALL(evt_bero_case_id_save, LW(1), LW(15))
    RETURN
EVT_END()

EVT_BEGIN(bero_case_switch_on)
    CALL(evt_bero_switch_on, LW(0))
    CALL(evt_bero_get_info_num, LW(1))
    IF_INT_EQUAL(LW(0), 0)
        DEBUG_PUT_MSG(STRING("\203x\203\215\223o\230^"))
        CALL_CHILD(PTR(bero_case_entry_sub))
        ELSE
        DEBUG_PUT_MSG(STRING("\203x\203\215\202\314\217o\202\304\215s\202\255\203f\201[\203^\202\314\223\361\217d\223o\230^\226h\216~\202\360\202\265\202\304\202\242\202\334\202\267"))
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(bero_case_switch_off)
    CALL(evt_bero_switch_off, LW(0))
    CALL(evt_bero_get_info_num, LW(1))
    IF_INT_EQUAL(LW(0), 0)
        DEBUG_PUT_MSG(STRING("\203x\203\215\215\355\217\234\202\265\202\334\202\265\202\275"))
        CALL(evt_bero_case_id_load, LW(1), LW(15))
        CALL(evt_del_case_evt, 0, LW(15))
        CALL(evt_hitobj_attr_onoff, 0, 0, LW(3), HIT_ATTR_BERO)
        ELSE
        DEBUG_PUT_MSG(STRING("\203x\203\215\202\314\215\355\217\234\227\\\226\361\202\340\202\265\202\255\202\315\203x\203\215\202\314\217o\202\304\215s\202\255\203f\201[\203^\202\314\223\361\217d\215\355\217\234\226h\216~\202\305\202\267"))
    END_IF
    RETURN
EVT_END()

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

EVT_BEGIN(evt_bero_info_run_pipe_sub00)
    CALL(evt_party_stop, 0)
    CALL(evt_party_stop, 1)
    SWITCH(LW(5))
        CASE_EQUAL(6)
        CALL(evt_mario_set_dispdir, 180)
        CALL(evt_mario_paper_pose_dokan, 4)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(2)
        CALL(evt_mario_set_dispdir, 0)
        CALL(evt_mario_paper_pose_dokan, 4)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(9)
        CALL(evt_mario_paper_pose_dokan, 1)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
        CASE_EQUAL(8)
        CALL(evt_mario_paper_pose_dokan, 0)
        CALL(evt_bero_move_mario_speed, FLOAT(60.0f))
    END_SWITCH
    CALL(evt_mario_dokan_end)
    RETURN
EVT_END()

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

EVT_BEGIN(evt_bero_info_run)
    CALL(evt_bero_get_into_info)
    CALL(evt_bero_set_now_number, LW(1))
    CALL(evt_bero_read_mario_pera)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_SAVE_LOADED)
        DEBUG_PUT_MSG(STRING("\203Z\201[\203u\203u\203\215\203b\203N\202\251\202\347\202\314\203\215\201[\203h\216\236\202\314\217\210\227\235"))
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
        IF_INT_EQUAL(LW(0), 2)
            CALL(evt_mario_set_prev_party_dokan, 0)
            CALL(evt_mario_set_prev_party_dokan, 1)
            ELSE
            CALL(evt_mario_set_prev_party, 0)
            CALL(evt_mario_set_prev_party, 1)
        END_IF
    END_IF
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    IF_INT_EQUAL(LW(0), 2)
        SWITCH(LW(5))
            CASE_EQUAL(6)
            CALL(evt_mario_dokan_prepare, 4)
            CASE_EQUAL(2)
            CALL(evt_mario_dokan_prepare, 4)
            CASE_EQUAL(9)
            CALL(evt_mario_dokan_prepare, 1)
            CASE_EQUAL(8)
            CALL(evt_mario_dokan_prepare, 0)
        END_SWITCH
    END_IF
    CALL_CHILD(PTR(evt_bero_initial_pos))
    IF_BITS_CLEAR(LW(4), 8192)
        SET_VALUE(LW(0), LW(4))
        AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
        IF_INT_EQUAL(LW(0), 2)
            SWITCH(LW(5))
                CASE_EQUAL(6)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, 46, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, 46, 0)
                CASE_EQUAL(2)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, 48, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, 48, 0)
                CASE_EQUAL(8)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, 42, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, 42, 0)
                CASE_EQUAL(9)
                CALL(evt_fade_set_mapchange_type, 1, -1, -1, 44, 0)
                CALL(evt_fade_set_mapchange_type, 0, -1, -1, 44, 0)
            END_SWITCH
        END_IF
    END_IF
    DEBUG_PUT_MSG(STRING("\202\331\202\251\202\314\217\211\212\372\220\335\222\350\212J\216n"))
    SPAWN_EVENT(PTR(evt_bero_mario_enter))
    RETURN
EVT_END()

EVT_BEGIN(evt_bero_initial_pos)
    CALL(evt_bero_get_into_info)
    SET_VALUE(LW(0), LW(4))
    AND_RAW(LW(0), BERO_TYPE_KIND_MASK)
    SWITCH(LW(0))
        CASE_EQUAL(3840)
        CASE_EQUAL(3)
        CALL(bero_get_position_normal, 0, FLOAT(0.0f))
        PEEK_FLOAT(LW(10), PTR(&BeroSX))
        PEEK_FLOAT(LW(11), PTR(&BeroSY))
        PEEK_FLOAT(LW(12), PTR(&BeroSZ))
        CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 1, LW(10), LW(11), LW(12))
        CASE_END
        CASE_EQUAL(0)
        CALL(evt_bero_get_info_length, LW(1), LW(10))
        CALL(bero_get_position_normal, 1, LW(10))
        PEEK_FLOAT(LW(10), PTR(&BeroSX))
        PEEK_FLOAT(LW(11), PTR(&BeroSY))
        PEEK_FLOAT(LW(12), PTR(&BeroSZ))
        CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 1, LW(10), LW(11), LW(12))
        CASE_EQUAL(1)
        CALL(evt_bero_get_info_length, LW(1), LW(10))
        CALL(bero_get_position_normal, 1, LW(10))
        PEEK_FLOAT(LW(10), PTR(&BeroSX))
        PEEK_FLOAT(LW(11), PTR(&BeroSY))
        PEEK_FLOAT(LW(12), PTR(&BeroSZ))
        CALL(evt_mario_set_pos, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 0, LW(10), LW(11), LW(12))
        CALL(evt_party_set_pos, 1, LW(10), LW(11), LW(12))
        CASE_EQUAL(2)
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
        CASE_EQUAL(3840)
        CALL_CHILD(PTR(evt_bero_info_run_none))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(evt_bero_info_run_road))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(evt_bero_info_run_door))
        CASE_EQUAL(2)
        CALL_CHILD(PTR(evt_bero_info_run_pipe))
        CASE_EQUAL(3)
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

