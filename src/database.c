#include "database.h"
#include "driver/arcdrv.h"
#include "manager/evtmgr.h"
#include "evt/evt_cmd.h"
#include "evt/evt_story.h"
#include "memory.h"
#include "system.h"
#include "driver/npcdrv.h"
#include "data/npc_data.h"
#include "data/mapdata.h"
#include "parse.h"
#include <string.h>
#include "libc_decls.h"
#include "manager/evtmgr_cmd.h"
#include "driver/animdrv.h"
#include "mario/mariost.h"

//.sbss
void* ptr;
u32 size;

void setupDataLoad(const char* mapName) {
    char path[64];
    DVDEntry* dvd;
    u32 filesize;

    ptr = NULL;
    sprintf(path, "%s/m/%s/s", getMarioStDvdRoot(), mapName);
    if (strcmp(mapName, "muj_20") || evtGetValue(NULL, GSW_STORY_PROGRESS) != STG0_02_MARIOS_HOUSE) {
        arcOpen(path, &ptr, &size);
        if (!ptr) {
            dvd = DVDMgrOpen(path, 2, 0);
            if (dvd) {
                filesize = DVDMgrGetLength(dvd);
                size = filesize;
                if (filesize) {
                    ptr = _mapAlloc(OSRoundUp32B(filesize));
                    DVDMgrRead(dvd, ptr, OSRoundUp32B(size), 0);
                }
                DVDMgrClose(dvd);
            }
        }
    }
}

typedef struct MapAttrEntry {
    const char* name;
    s32 area;
    u32 flags;
} MapAttrEntry;

extern MapAttrEntry _map_attr_data[];

BOOL setupDataCheck(void) {
    char tribeName[64];
    BOOL group;

    if (!ptr) {
        return TRUE;
    }
    parseInit(ptr, size);
    while (parsePush("<Npc>")) {
        parseTagGet1("<tribe>", 0, tribeName);
        parsePopNext();
        group = gp->inBattle != 0;
        if (!animGroupBaseAsync(npcGetTribe(tribeName)->modelName, group, NULL)) {
            return FALSE;
        }
    }
    parsePop();
    return TRUE;
}
static inline MapAttrEntry* findMapAttr(const char* name) {
    MapAttrEntry* p = _map_attr_data;
    for (;;) {
        if (!p->name || !strcmp(p->name, name)) {
            break;
        }
        p++;
    }
    return p;
}

void setupDataBase(const char* areaName, const char* mapName) {
    char npcName[64];
    char tribeName[64];
    Vec pos;
    f32 dir;
    char aiType[64];
    char terrType[64];
    Vec base;
    Vec loiter;
    Vec homing;
    f32 searchRange, searchAngle, homingRange, homingAngle;
    char btlSetup[64];
    FieldBattleData* fbat;
    MapAttrEntry* attr;
    MapAttrEntry* a;
    MapAttrEntry* b;
    NpcDeadInfo* dead;
    NpcDeadInfo* dead1;
    NpcDeadInfo tmp;
    int i, j;
    NpcTribeInfo* tribe;
    NpcEntry* npc;
    int k;
    int isMarker;
    NpcAiInfo* ai;
    AreaData* area;
    s32 btl;

    fbat = fbatGetPointer();
    dead = fbat->deadInfo;
    dead1 = &fbat->deadInfo[1];
    attr = findMapAttr(mapName);

    for (i = 0; i < 64; i++) {
        if (strcmp(fbat->deadInfo[i].mapName, "") == 0) {
            memset(&fbat->deadInfo[i], 0, sizeof(NpcDeadInfo));
            strcpy(fbat->deadInfo[i].mapName, mapName);
            break;
        }
        if (strcmp(fbat->deadInfo[i].mapName, mapName) == 0) {
            break;
        }
    }
    if (i > 0) {
        tmp = fbat->deadInfo[i];
        for (j = i; j >= 1; j--) {
            fbat->deadInfo[j] = fbat->deadInfo[j - 1];
        }
        fbat->deadInfo[0] = tmp;
    }

    if (attr->flags & 1) {
        a = findMapAttr(mapName);
        b = findMapAttr(dead1->mapName);
        if (a->area != b->area) {
            memset(dead, 0, sizeof(fbat->deadInfo));
            strcpy(dead->mapName, mapName);
        } else if (!(attr->flags & 2) && (attr->flags & 4)) {
            memset(&fbat->deadInfo[2], 0, 0x4d8);
        }
    }

    k = 0;
    isMarker = 0;
    if (attr->flags & 1) {
        isMarker = 1;
    }
    if (ptr) {
        parseInit(ptr, size);
        while (parsePush("<Npc>")) {
            parseTagGet1("<name>", 0, npcName);
            parseTagGet1("<tribe>", 0, tribeName);
            parseTagGet1("<position>", 3, &pos);
            parseTagGet1("<dir>", 2, &dir);
            parseTagGet1("<aitype>", 0, aiType);
            parseTagGet1("<territory_type>", 0, terrType);
            parseTagGet1("<territory_base>", 3, &base);
            parseTagGet1("<territory_loiter>", 3, &loiter);
            parseTagGet1("<territory_homing>", 3, &homing);
            parseTagGet1("<search_range>", 2, &searchRange);
            parseTagGet1("<search_angle>", 2, &searchAngle);
            parseTagGet1("<homing_range>", 2, &homingRange);
            parseTagGet1("<homing_angle>", 2, &homingAngle);
            parseTagGet1("<btl_setup_no>", 0, btlSetup);
            parsePopNext();
            if (!isMarker || !(dead->mask & (1 << (u8)k))) {
                tribe = npcGetTribe(tribeName);
                npcEntry(npcName, tribe->modelName);
                npc = npcNameToPtr(npcName);
                npc->setupIndex = k;
                ai = npc_ai_type_table;
                npc->flags |= NPC_FLAG_RECORD_DEFEAT;
                npc->position = pos;
                npc->direction = dir;
                npc->tribe = tribe;
                npc->width = tribe->width;
                npc->height = tribe->height;
                npc->flags &= ~NPC_FLAG_READY;
                for (;;) {
                    if (!ai->description) {
                        break;
                    }
                    if (strcmp(ai->description, aiType) != 0) {
                        ai++;
                        continue;
                    }
                    npc->flags |= ai->flags;
                    npc->initEvent = ai->initEvent;
                    npc->moveEvent = ai->moveEvent;
                    npc->deadEvent = ai->deadEvent;
                    npc->findEvent = ai->findEvent;
                    npc->lostEvent = ai->lostEvent;
                    npc->returnEvent = ai->returnEvent;
                    npc->blowEvent = ai->blowEvent;
                    break;
                }
                npc->territoryType = dbGetDefData(npc_define_territory_type, terrType);
                npc->territoryBase = base;
                npc->territoryLoiter = loiter;
                npc->territoryHoming = homing;
                npc->searchRange = searchRange;
                npc->searchAngle = searchAngle;
                npc->homingRange = homingRange;
                npc->homingAngle = homingAngle;
                npc->footstepSfxA = tribe->footstepSfxA;
                npc->footstepSfxB = tribe->footstepSfxB;
                npc->jumpSfx = tribe->jumpSfx;
                npc->landSfx = tribe->landSfx;
                npc->footstepStride = tribe->footstepStride;
                npc->footstepToggle = 1;
                npc->footstepAccum = 0.0f;
                btl = -1;
                area = areaDataPtr(areaName);
                if (strcmp(btlSetup, "BTLNO_INVALID")) {
                    btl = dbGetDefData((NpcTerritoryInfo*)area->btlSetupNoTbl, btlSetup);
                }
                npcSetBattleInfo(npc, btl);
                if (tribe->stayAnim) {
                    strcpy(npc->stayAnim, tribe->stayAnim);
                }
                if (tribe->talkAnim) {
                    strcpy(npc->talkAnim, tribe->talkAnim);
                }
                if (tribe->initialAnim) {
                    strcpy(npc->currentAnim, tribe->initialAnim);
                    animPoseSetAnim(npc->poseId, npc->currentAnim, 1);
                }
            }
            k++;
        }
        parsePop();
    }
}

MapAttrEntry _map_attr_data[] = {
    { "tik_00", 16, 7 },
    { "tik_01", 16, 7 },
    { "tik_02", 16, 7 },
    { "tik_03", 16, 7 },
    { "tik_04", 16, 7 },
    { "tik_05", 16, 7 },
    { "tik_06", 16, 7 },
    { "tik_07", 16, 7 },
    { "tik_08", 16, 7 },
    { "tik_09", 16, 7 },
    { "tik_10", 16, 7 },
    { "tik_11", 16, 7 },
    { "tik_12", 16, 7 },
    { "tik_13", 16, 7 },
    { "tik_14", 16, 7 },
    { "tik_15", 16, 7 },
    { "tik_16", 16, 7 },
    { "tik_17", 16, 7 },
    { "tik_18", 16, 7 },
    { "tik_19", 16, 7 },
    { "tik_20", 16, 7 },
    { "tik_21", 16, 7 },
    { "hei_00", 1, 5 },
    { "hei_01", 1, 5 },
    { "hei_02", 1, 5 },
    { "hei_03", 1, 5 },
    { "hei_04", 1, 5 },
    { "hei_05", 1, 5 },
    { "hei_06", 1, 5 },
    { "hei_07", 1, 5 },
    { "hei_08", 5, 7 },
    { "hei_09", 5, 7 },
    { "hei_10", 5, 7 },
    { "hei_11", 5, 7 },
    { "hei_12", 5, 7 },
    { "hei_13", 1, 5 },
    { "gon_00", 2, 7 },
    { "gon_01", 2, 7 },
    { "gon_02", 2, 7 },
    { "gon_03", 2, 7 },
    { "gon_04", 2, 7 },
    { "gon_05", 2, 7 },
    { "gon_06", 2, 7 },
    { "gon_07", 2, 7 },
    { "gon_08", 2, 7 },
    { "gon_09", 2, 7 },
    { "gon_10", 2, 7 },
    { "gon_11", 2, 7 },
    { "gon_12", 2, 7 },
    { "gon_13", 2, 7 },
    { "win_00", 3, 5 },
    { "win_01", 3, 5 },
    { "win_02", 3, 5 },
    { "win_03", 3, 5 },
    { "win_04", 3, 5 },
    { "win_05", 3, 5 },
    { "win_06", 3, 5 },
    { "mri_00", 3, 5 },
    { "mri_01", 4, 7 },
    { "mri_02", 4, 7 },
    { "mri_03", 4, 7 },
    { "mri_04", 4, 7 },
    { "mri_05", 4, 7 },
    { "mri_06", 4, 7 },
    { "mri_07", 4, 7 },
    { "mri_08", 4, 7 },
    { "mri_09", 4, 7 },
    { "mri_10", 4, 7 },
    { "mri_11", 4, 7 },
    { "mri_12", 4, 7 },
    { "mri_13", 4, 7 },
    { "mri_14", 4, 7 },
    { "mri_15", 4, 7 },
    { "mri_16", 4, 7 },
    { "mri_17", 4, 7 },
    { "mri_18", 4, 7 },
    { "mri_19", 4, 7 },
    { "mri_20", 4, 7 },
    { "gra_00", 6, 5 },
    { "gra_01", 6, 5 },
    { "gra_02", 6, 5 },
    { "gra_03", 6, 5 },
    { "gra_04", 6, 5 },
    { "gra_05", 6, 5 },
    { "gra_06", 6, 5 },
    { "jin_00", 7, 7 },
    { "jin_01", 7, 7 },
    { "jin_02", 7, 7 },
    { "jin_03", 7, 7 },
    { "jin_04", 7, 7 },
    { "jin_05", 7, 7 },
    { "jin_06", 7, 7 },
    { "jin_07", 7, 7 },
    { "jin_08", 7, 7 },
    { "jin_09", 7, 7 },
    { "jin_10", 7, 7 },
    { "jin_11", 7, 7 },
    { "muj_00", 8, 5 },
    { "muj_01", 8, 5 },
    { "muj_02", 8, 5 },
    { "muj_03", 8, 5 },
    { "muj_04", 8, 5 },
    { "muj_05", 8, 5 },
    { "muj_10", 9, 7 },
    { "muj_11", 9, 7 },
    { "muj_12", 9, 7 },
    { "dou_00", 9, 7 },
    { "dou_01", 9, 7 },
    { "dou_02", 9, 7 },
    { "dou_03", 9, 7 },
    { "dou_04", 9, 7 },
    { "dou_05", 9, 7 },
    { "dou_06", 9, 7 },
    { "dou_07", 9, 7 },
    { "dou_08", 9, 7 },
    { "dou_09", 9, 7 },
    { "dou_10", 9, 7 },
    { "dou_11", 9, 7 },
    { "dou_12", 9, 7 },
    { "dou_13", 9, 7 },
    { "eki_00", 10, 7 },
    { "eki_01", 10, 7 },
    { "eki_02", 10, 7 },
    { "eki_03", 10, 7 },
    { "eki_04", 10, 7 },
    { "eki_05", 10, 7 },
    { "eki_06", 10, 7 },
    { "pik_00", 17, 7 },
    { "pik_01", 17, 7 },
    { "pik_02", 17, 7 },
    { "pik_03", 17, 7 },
    { "pik_04", 17, 7 },
    { "bom_03", 11, 5 },
    { "bom_04", 11, 5 },
    { "eki_00", 12, 7 },
    { "eki_01", 12, 7 },
    { "eki_02", 12, 7 },
    { "eki_03", 12, 7 },
    { "eki_04", 12, 7 },
    { "eki_05", 12, 7 },
    { "eki_06", 12, 7 },
    { "moo_00", 13, 5 },
    { "moo_01", 13, 5 },
    { "moo_02", 13, 5 },
    { "moo_05", 13, 5 },
    { "moo_06", 13, 5 },
    { "moo_07", 13, 5 },
    { "aji_00", 14, 7 },
    { "aji_01", 14, 7 },
    { "aji_02", 14, 7 },
    { "aji_03", 14, 7 },
    { "aji_04", 14, 7 },
    { "aji_05", 14, 7 },
    { "aji_06", 14, 7 },
    { "aji_07", 14, 7 },
    { "aji_08", 14, 7 },
    { "aji_09", 14, 7 },
    { "aji_10", 14, 7 },
    { "aji_11", 14, 7 },
    { "aji_12", 14, 7 },
    { "aji_13", 14, 7 },
    { "aji_14", 14, 7 },
    { "aji_15", 14, 7 },
    { "aji_16", 14, 7 },
    { "aji_17", 14, 7 },
    { "aji_18", 14, 7 },
    { "aji_19", 14, 7 },
    { "las_00", 15, 7 },
    { "las_01", 15, 7 },
    { "las_02", 15, 7 },
    { "las_03", 15, 7 },
    { "las_04", 15, 7 },
    { "las_05", 15, 7 },
    { "las_06", 15, 7 },
    { "las_07", 15, 7 },
    { "las_08", 15, 7 },
    { "las_09", 15, 7 },
    { "las_10", 15, 7 },
    { "las_11", 15, 7 },
    { "las_12", 15, 7 },
    { "las_13", 15, 7 },
    { "las_14", 15, 7 },
    { "las_15", 15, 7 },
    { "las_16", 15, 7 },
    { "las_17", 15, 7 },
    { "las_18", 15, 7 },
    { "las_19", 15, 7 },
    { "las_20", 15, 7 },
    { "las_21", 15, 7 },
    { "las_22", 15, 7 },
    { "las_23", 15, 7 },
    { "las_24", 15, 7 },
    { "las_25", 15, 7 },
    { "las_26", 15, 7 },
    { "las_27", 15, 7 },
    { "las_28", 15, 7 },
    { "las_29", 15, 7 },
    { "las_30", 15, 7 },
    { NULL, -1, 0 },
};

