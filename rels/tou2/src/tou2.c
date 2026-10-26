#include "tou2.h"
#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "dolphin/gx.h"
#include "driver/swdrv.h"
#include "driver/npcdrv.h"
#include "battle/battle_unit.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_starstone.h"
#include "driver/dispdrv.h"
#include "driver/windowdrv.h"
#include "driver/icondrv.h"
#include "driver/msgdrv.h"
#include "manager/fontmgr.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cmd.h"
#include "battle/battle_monosiri.h"
#include "data/mapdata.h"
#include "mario/mario_pouch.h"
#include "pmario_sound.h"
#include "system.h"
#include "memory.h"
#include "string.h"
#include "stdio.h"
#include "icon_ids.h"
#include "mario/mariost.h"
#include "data/npc_data.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "libc_decls.h"

typedef struct RankEntry {
    u16 flags;
    // pad 2 bytes
    s32 id;
    union {
        s16 total;
        u8 bytes[2];
    } points;
    // pad 2 bytes
} RankEntry;

typedef enum RankFlags {
    RANK_BELOW = 1 << 0, // This fighter is below us in the rankings
    RANK_HIDDEN = 1 << 1, // This fighter is hidden from us
    RANK_PLAYER = 1 << 2, // This is the player
} RankFlags;

typedef struct RankData {
    s32 count;
    RankEntry* entries;
} RankData;

typedef struct FighterData {
    u16 flags;
    // pad 2 bytes
    const char* name;
    const char* title;
    s32 setupId;
    const char* taunt;
    const char* victory;
    s16 field_18; // priority, order, sortId?
    // pad 2 bytes
} FighterData;

// This includes a bunch of "story markers" for the various important fights
typedef enum FighterFlags {
    FIGHT_MAJOR = 1 << 0,
    FIGHT_MINOR = 1 << 1,
    FIGHT_CHAMPION = 1 << 2,
    FIGHT_PLAYER = 1 << 3,
    FIGHT_BANDITS = 1 << 8,
    FIGHT_KOOPAS = 1 << 9,
    FIGHT_GOOMBAS = 1 << 10,
    FIGHT_CLEFTS = 1 << 11,
    FIGHT_KOOPATROL = 1 << 12,
} FighterFlags;

typedef enum FighterId {
    FIGHTER_RAWK_HAWK = 0,
    FIGHTER_KOOPINATOR = 1,
    FIGHTER_CHOMPS = 2,
    FIGHTER_BROS = 3,
    FIGHTER_CRAW = 4,
    FIGHTER_MAGIKOOPA = 5,
    FIGHTER_FUZZY = 6,
    FIGHTER_SHADY_KOOPAS = 7,
    FIGHTER_BRISTLES = 8,
    FIGHTER_SPIKE_TOPS = 9,
    FIGHTER_IRON_CLEFTS = 10,
    FIGHTER_BANDITS = 11,
    FIGHTER_BOB_OMB = 12,
    FIGHTER_BALD_CLEFTS = 13,
    FIGHTER_BOGGLY = 14,
    FIGHTER_LAKITU = 15,
    FIGHTER_DULL_BONES = 16,
    FIGHTER_POKEYS = 17,
    FIGHTER_KP_KOOPAS = 18,
    FIGHTER_GOOMBAS = 19,
    FIGHTER_PLAYER = 20,
    FIGHTER_SWOOPS = 21,
    FIGHTER_SPINIA = 22,
} FighterId;

typedef struct RuleData {
    const char* name;
    const char* desc;
    s32 id;
    s32 field_C;
    s32 field_10;
} RuleData;

typedef struct DispWork {
    f32 scroll;
    f32 target;
    s32 page;
} DispWork;

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
extern NpcSetupBattleInfo btlsetup_tou_tbl[];


static RankData rank_work;
static DispWork disp_work;
static s32 champion_belt_onoff;

static FighterData fighterDt[23] = {
    { FIGHT_CHAMPION, "stg3_tou_sensyu_00", "stg3_tou_sensyu2_00", 22, "stg3_tou_400_02", "stg3_tou_400_03", 250 },                // Rawk Hawk
    { FIGHT_KOOPATROL | FIGHT_MAJOR, "stg3_tou_sensyu_01", "stg3_tou_sensyu2_01", 19, "stg3_tou_118_00", "stg3_tou_118_01", 200 }, // The Koopinator
    { FIGHT_MAJOR, "stg3_tou_sensyu_02", "stg3_tou_sensyu2_02", 18, "stg3_tou_118_02", "stg3_tou_118_03", 180 },                   // Chomp Country
    { FIGHT_MAJOR, "stg3_tou_sensyu_03", "stg3_tou_sensyu2_03", 17, "stg3_tou_118_04", "stg3_tou_118_05", 170 },                   // Hamma, Bamma, and Flare
    { FIGHT_MAJOR, "stg3_tou_sensyu_04", "stg3_tou_sensyu2_04", 16, "stg3_tou_118_06", "stg3_tou_118_07", 160 },                   // Craw-Daddy
    { FIGHT_MAJOR, "stg3_tou_sensyu_05", "stg3_tou_sensyu2_05", 15, "stg3_tou_118_08", "stg3_tou_118_09", 150 },                   // The Magikoopa Masters
    { FIGHT_MAJOR, "stg3_tou_sensyu_06", "stg3_tou_sensyu2_06", 14, "stg3_tou_118_10", "stg3_tou_118_11", 140 },                   // The Fuzz
    { FIGHT_MAJOR, "stg3_tou_sensyu_07", "stg3_tou_sensyu2_07", 13, "stg3_tou_118_12", "stg3_tou_118_13", 130 },                   // The Shellshockers
    { FIGHT_MAJOR, "stg3_tou_sensyu_08", "stg3_tou_sensyu2_08", 12, "stg3_tou_118_14", "stg3_tou_118_15", 120 },                   // The Poker Faces
    { FIGHT_MAJOR, "stg3_tou_sensyu_09", "stg3_tou_sensyu2_09", 11, "stg3_tou_118_16", "stg3_tou_118_17", 110 },                   // The Tiny Spinies
    { FIGHT_CLEFTS | FIGHT_MAJOR, "stg3_tou_sensyu_10", "stg3_tou_sensyu2_10", 10, "stg3_tou_118_18", "stg3_tou_118_19", 100 },    // The Armored Harriers
    { FIGHT_BANDITS | FIGHT_MINOR, "stg3_tou_sensyu_11", "stg3_tou_sensyu2_11", 9, "stg3_tou_118_20", "stg3_tou_118_21", 90 },     // The Hand-It-Overs
    { FIGHT_MINOR, "stg3_tou_sensyu_12", "stg3_tou_sensyu2_12", 8, "stg3_tou_118_22", "stg3_tou_118_23", 80 },                     // The Bob-omb Squad
    { FIGHT_MINOR, "stg3_tou_sensyu_13", "stg3_tou_sensyu2_13", 7, "stg3_tou_118_24", "stg3_tou_118_25", 70 },                     // The Punk Rocks
    { FIGHT_MINOR, "stg3_tou_sensyu_14", "stg3_tou_sensyu2_14", 6, "stg3_tou_118_26", "stg3_tou_118_27", 60 },                     // The Mind-Bogglers
    { FIGHT_MINOR, "stg3_tou_sensyu_15", "stg3_tou_sensyu2_15", 5, "stg3_tou_118_28", "stg3_tou_118_29", 50 },                     // Spike Storm
    { FIGHT_MINOR, "stg3_tou_sensyu_16", "stg3_tou_sensyu2_16", 4, "stg3_tou_118_30", "stg3_tou_118_31", 40 },                     // The Dead Bones
    { FIGHT_MINOR, "stg3_tou_sensyu_17", "stg3_tou_sensyu2_17", 3, "stg3_tou_118_32", "stg3_tou_118_33", 30 },                     // The Pokey Triplets
    { FIGHT_KOOPAS | FIGHT_MINOR, "stg3_tou_sensyu_18", "stg3_tou_sensyu2_18", 2, "stg3_tou_118_34", "stg3_tou_118_35", 20 },      // The KP Koopas
    { FIGHT_GOOMBAS | FIGHT_MINOR, "stg3_tou_sensyu_19", "stg3_tou_sensyu2_19", 1, "stg3_tou_118_36", "stg3_tou_118_36_01", 10 },  // The Goomba Bros.
    { FIGHT_PLAYER, "stg3_tou_sensyu_20", "stg3_tou_sensyu2_20", 1, NULL, NULL, 0 },                                               // The Great Gonzales
    { FIGHT_MINOR, "stg3_tou_sensyu_21", "stg3_tou_sensyu2_21", 20, "stg3_tou_118_36_02", "stg3_tou_118_36_03", 5 },               // Wings of Night
    { FIGHT_MINOR, "stg3_tou_sensyu_22", "stg3_tou_sensyu2_22", 21, "stg3_tou_118_36_04", "stg3_tou_118_36_05", 0 },               // The Destructors
};

static FighterData fighterDt_re[21] = {
    { FIGHT_CHAMPION, "stg3_tou_re_315", "stg3_tou_re_338", 22, "stg3_tou_re_289", "stg3_tou_re_290", 250 },                // Rawk Hawk
    { FIGHT_KOOPATROL | FIGHT_MAJOR, "stg3_tou_re_316", "stg3_tou_re_339", 19, "stg3_tou_re_179", "stg3_tou_re_180", 200 }, // The Koopinator
    { FIGHT_MAJOR, "stg3_tou_re_317", "stg3_tou_re_340", 18, "stg3_tou_re_181", "stg3_tou_re_182", 180 },                   // Chomp Country
    { FIGHT_MAJOR, "stg3_tou_re_318", "stg3_tou_re_341", 17, "stg3_tou_re_183", "stg3_tou_re_184", 170 },                   // Hamma, Bamma, and Flare
    { FIGHT_MAJOR, "stg3_tou_re_319", "stg3_tou_re_342", 16, "stg3_tou_re_185", "stg3_tou_re_186", 160 },                   // Craw-Daddy
    { FIGHT_MAJOR, "stg3_tou_re_320", "stg3_tou_re_343", 15, "stg3_tou_re_187", "stg3_tou_re_188", 150 },                   // The Magikoopa Masters
    { FIGHT_MAJOR, "stg3_tou_re_321", "stg3_tou_re_344", 14, "stg3_tou_re_189", "stg3_tou_re_190", 140 },                   // The Fuzz
    { FIGHT_MAJOR, "stg3_tou_re_322", "stg3_tou_re_345", 13, "stg3_tou_re_191", "stg3_tou_re_192", 130 },                   // The Shellshockers
    { FIGHT_MAJOR, "stg3_tou_re_323", "stg3_tou_re_346", 12, "stg3_tou_re_193", "stg3_tou_re_194", 120 },                   // The Poker Faces
    { FIGHT_MAJOR, "stg3_tou_re_324", "stg3_tou_re_347", 11, "stg3_tou_re_195", "stg3_tou_re_196", 110 },                   // The Tiny Spinies
    { FIGHT_CLEFTS | FIGHT_MAJOR, "stg3_tou_re_325", "stg3_tou_re_348", 10, "stg3_tou_re_197", "stg3_tou_re_198", 100 },    // The Armored Harriers
    { FIGHT_BANDITS | FIGHT_MINOR, "stg3_tou_re_326", "stg3_tou_re_349", 9, "stg3_tou_re_199", "stg3_tou_re_200", 90 },     // The Hand-It-Overs
    { FIGHT_MINOR, "stg3_tou_re_327", "stg3_tou_re_350", 8, "stg3_tou_re_201", "stg3_tou_re_202", 80 },                     // The Bob-omb Squad
    { FIGHT_MINOR, "stg3_tou_re_328", "stg3_tou_re_351", 7, "stg3_tou_re_203", "stg3_tou_re_204", 70 },                     // The Punk Rocks
    { FIGHT_MINOR, "stg3_tou_re_329", "stg3_tou_re_352", 6, "stg3_tou_re_205", "stg3_tou_re_206", 60 },                     // The Mind-Bogglers
    { FIGHT_MINOR, "stg3_tou_re_330", "stg3_tou_re_353", 5, "stg3_tou_re_207", "stg3_tou_re_208", 50 },                     // Spike Storm
    { FIGHT_MINOR, "stg3_tou_re_331", "stg3_tou_re_354", 4, "stg3_tou_re_209", "stg3_tou_re_210", 40 },                     // The Dead Bones
    { FIGHT_MINOR, "stg3_tou_re_332", "stg3_tou_re_355", 3, "stg3_tou_re_211", "stg3_tou_re_212", 30 },                     // The Pokey Triplets
    { FIGHT_KOOPAS | FIGHT_MINOR, "stg3_tou_re_333", "stg3_tou_re_356", 2, "stg3_tou_re_213", "stg3_tou_re_214", 20 },      // The KP Koopas
    { FIGHT_GOOMBAS | FIGHT_MINOR, "stg3_tou_re_334", "stg3_tou_re_357", 1, "stg3_tou_re_215", "stg3_tou_re_216", 10 },     // The Goomba Bros.
    { FIGHT_PLAYER, "stg3_tou_re_335", "stg3_tou_re_358", 1, NULL, NULL, 0 },                                               // The Great Gonzales
    //{ FIGHT_MINOR, "stg3_tou_re_336", "stg3_tou_re_359", 20, "stg3_tou_re_217", "stg3_tou_re_218", 5 },                     // Wings of Night (cut)
    //{ FIGHT_MINOR, "stg3_tou_re_337", "stg3_tou_re_360", 21, "stg3_tou_re_219", "stg3_tou_re_220", 0 },                     // The Destructors (cut)
};

static RuleData ruleDt_2bu[10] = {
    { "stg3_tou_107_01", "stg3_tou_cp_000", 18, 3, 3 },
    { "stg3_tou_107_02", "stg3_tou_cp_001", 1, 1, 1 },
    { "stg3_tou_107_03", "stg3_tou_cp_002", 3, 1, 1 },
    { "stg3_tou_107_04", "stg3_tou_cp_003", 48, 1, 1 },
    { "stg3_tou_107_05", "stg3_tou_cp_004", 5, 1, 1 },
    { "stg3_tou_107_06", "stg3_tou_cp_005", 26, 1, 1 },
    { "stg3_tou_107_07", "stg3_tou_cp_006", 32, 1, 1 },
    { "stg3_tou_107_08", "stg3_tou_cp_007", 59, 5, 5 },
    { "stg3_tou_107_12", "stg3_tou_cp_008", 6, 1, 1 },
    { "stg3_tou_107_16", "stg3_tou_cp_009", 43, 1, 1 },
};

static RuleData ruleDt_1bu[14] = {
    { "stg3_tou_107_01", "stg3_tou_cp_010", 18, 5, 5 },
    { "stg3_tou_107_02", "stg3_tou_cp_011", 1, 1, 1 },
    { "stg3_tou_107_03", "stg3_tou_cp_012", 3, 1, 1 },
    { "stg3_tou_107_04", "stg3_tou_cp_013", 48, 1, 1 },
    { "stg3_tou_107_05", "stg3_tou_cp_014", 5, 1, 1 },
    { "stg3_tou_107_06", "stg3_tou_cp_015", 26, 1, 1 },
    { "stg3_tou_107_07", "stg3_tou_cp_016", 32, 1, 1 },
    { "stg3_tou_107_08", "stg3_tou_cp_017", 59, 5, 5 },
    { "stg3_tou_107_12", "stg3_tou_cp_019", 6, 1, 1 },
    { "stg3_tou_107_13", "stg3_tou_cp_020", 11, 20, 20 },
    { "stg3_tou_107_16", "stg3_tou_cp_021", 43, 3, 3 },
    { "stg3_tou_107_17", "stg3_tou_cp_022", 65, 3, 3 },
    { "stg3_tou_107_19", "stg3_tou_cp_023", 64, 255, 255 },
    { "stg3_tou_107_20", "stg3_tou_cp_024", 63, 255, 255 },
};

static RuleData ruleDt_re[14] = {
    { "stg3_tou_re_362", "stg3_tou_cp_040", 18, 5, 5 },
    { "stg3_tou_re_363", "stg3_tou_cp_041", 1, 1, 1 },
    { "stg3_tou_re_364", "stg3_tou_cp_042", 3, 1, 1 },
    { "stg3_tou_re_365", "stg3_tou_cp_043", 48, 1, 1 },
    { "stg3_tou_re_366", "stg3_tou_cp_044", 5, 1, 1 },
    { "stg3_tou_re_367", "stg3_tou_cp_045", 26, 1, 1 },
    { "stg3_tou_re_368", "stg3_tou_cp_046", 32, 1, 1 },
    { "stg3_tou_re_369", "stg3_tou_cp_047", 59, 5, 5 },
    { "stg3_tou_re_371", "stg3_tou_cp_049", 6, 1, 1 },
    { "stg3_tou_re_372", "stg3_tou_cp_050", 11, 20, 20 },
    { "stg3_tou_re_373", "stg3_tou_cp_051", 43, 3, 3 },
    { "stg3_tou_re_374", "stg3_tou_cp_052", 65, 3, 3 },
    { "stg3_tou_re_375", "stg3_tou_cp_053", 64, 255, 255 },
    { "stg3_tou_re_376", "stg3_tou_cp_054", 63, 255, 255 },
};

static RankData* rank_wp = &rank_work;
static DispWork* dwp = &disp_work;
static const char* suuji[] = { "０", "１", "２", "３", "４", "５", "６", "７", "８", "９" };

// disp_proc templates
static const GXColor titleColor = { 0xDC, 0xC8, 0xDC, 0xFF };
static const GXColor numberColor = { 0xDC, 0xC8, 0xDC, 0xFF };
static const GXColor playerColor = { 0xFF, 0x00, 0x00, 0xFF };
static const GXColor nameColor = { 0xDC, 0xC8, 0xDC, 0xFF };
static const Vec cursorPos = { 0.0f, 0.0f, 0.0f };
static const Vec upArrowPos = { 0.0f, 125.0f, 0.0f };
static const Vec upButtonPos = { 0.0f, 75.0f, 0.0f };
static const Vec downArrowPos = { 0.0f, -140.0f, 0.0f };
static const Vec downButtonPos = { 0.0f, -125.0f, 0.0f };

// local prototypes
static f32 camGetRy(void);
f32 atan2f(f32 y, f32 x);
static void disp_proc(CameraId cameraId, void* param);
static void printRanking(void);
static void rankingControll(void);
static void insert(s32* arr, s32 n, s32 from, s32 to);
static void rankingExit(void);
static void rankingInit(void);
USER_FUNC(tou_champion_belt_onoff) {
    champion_belt_onoff = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(tou_champion_belt) {
    NpcEntry* npc;
    EffectEntry* eff;
    AnimationPose* pose;
    Vec* positions;
    EffStarStoneWork** work;
    s32 group;
    f32 angle;

    npc = npcNameToPtr((const char*)evtGetValue(event, event->args[0]));
    if (isFirstCall) {
        eff = effStarStoneEntry(9, 0.0f, 0.0f, 0.0f, 0.6f);
        animPoseWorldPositionEvalOn(npc->poseId);
        event->userdatap[0] = eff;
        champion_belt_onoff = 0;
    }
    pose = animPoseGetAnimPosePtr(npc->poseId);
    positions = *pose->worldPositions;
    group = animPoseGetGroupIdx(npc->poseId, "locator8");
    angle = reviseAngle(camGetRy() - pose->rotationY);
    eff = event->userdatap[0];
    // FAKE: the work is read through a pointer to the userdata slot, so userdata is reloaded at every store (a plain EffStarStoneWork* local: 86.67%, a cast per access: 99.26%)
    work = (EffStarStoneWork**)&eff->userdata;
    (*work)->position.x = positions[group].x;
    (*work)->position.y = positions[group].y;
    (*work)->position.z = positions[group].z;
    if ((angle > 90.0f && angle <= 270.0f) || (angle < -90.0f && angle >= -270.0f)) {
        (*work)->rot.y = 8.0f - (angle + camGetPtr(CAMERA_3D)->viewYaw);
    } else {
        (*work)->rot.y = -(angle + camGetPtr(CAMERA_3D)->viewYaw) - 8.0f;
    }
    if (npc->position.y <= -1000.0f) {
        (*work)->position.x = 0.0f;
        (*work)->position.y = -1000.0f;
        (*work)->position.z = 0.0f;
    }
    if (champion_belt_onoff) {
        (*work)->position.x = 0.0f;
        (*work)->position.y = -1000.0f;
        (*work)->position.z = 0.0f;
    }
    return EVT_RETURN_BLOCK;
}

static f32 camGetRy(void) {
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    f32 ry = reviseAngle(57.29579f * atan2f(-(cam->cameraAt.x - cam->cameraPos.x), -(cam->cameraAt.z - cam->cameraPos.z)));

    if (ry < 0.0f) {
        ry += 360.0f;
    }
    return ry;
}

f32 atan2f(f32 y, f32 x) {
    return atan2(y, x);
}

static USER_FUNC(evt_tou_print_ranking) {
    s32 i;
    s32 rank;

    if (isFirstCall) {
        memset(dwp, 0, sizeof(DispWork));
        rank = 0;
        for (i = 0; i < rank_wp->count; i++) {
            if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
                if (rank_wp->entries[i].flags & RANK_PLAYER) {
                    break;
                }
                rank++;
            }
        }
        dwp->page = rank / 11;
        dwp->scroll = dwp->target = dwp->page * 220;
        printRanking();
        return EVT_RETURN_BLOCK;
    }
    if (keyGetButtonTrg(0) & PAD_BUTTON_B) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_CANCEL1));
        return EVT_RETURN_YIELD;
    }
    if (dwp->page > 0 && (keyGetButtonTrg(0) & PAD_TRIGGER_L)) {
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
        dwp->page--;
    }
    if (dwp->page < 1 && (keyGetButtonTrg(0) & PAD_TRIGGER_R)) {
        dwp->page++;
        psndSFXOn(SFX_NAME_WORD(PSND_SFX_CURSOR_MOVE1));
    }
    dwp->target = dwp->page * 220;
    dwp->scroll += (dwp->target - dwp->scroll) / 8.0f;
    dispEntry(CAMERA_2D, 300.0f, 0, disp_proc, NULL);
    return EVT_RETURN_BLOCK;
}

static void disp_proc(CameraId cameraId, void* param) {
    FighterData* fighters;
    f32 winW;
    f32 left;
    f32 top;
    f32 x;
    s32 width;
    u16 scissorX;
    u16 scissorW;
    s32 i;
    s32 num;
    f32 y;
    Mtx scale;
    Mtx mtx;
    char entry[0x100];
#ifdef VERSION_G8ME01
    const char* title;
    const char* name;
    f32 nameW;
#endif

    fighters = evtGetValue(NULL, GSW_STORY_PROGRESS) < 172 ? fighterDt : fighterDt_re;
    winW = 250.0f;
    left = -winW / 2.0f;
    windowDispGX_System(0, 0xFF, left - 30.0f, 165.0f, 310.0f, 330.0f);
#ifdef VERSION_G8ME01
    if ((u32)gp->language == 0) {
        title = "\x83\x89\x83\x93\x83\x4C\x83\x93\x83\x4F";
    } else {
        title = msgSearch("stg3_ranking");
    }
    FontDrawStart();
    FontDrawScale(1.0f);
    width = FontGetMessageWidth(title);
#else
    FontDrawStart();
    FontDrawScale(1.0f);
    width = FontGetMessageWidth("\x83\x89\x83\x93\x83\x4C\x83\x93\x83\x4F");
#endif
    {
        GXColor color = titleColor;
        FontDrawColor(color);
    }
    top = 135.0f;
#ifdef VERSION_G8ME01
    FontDrawMessage(-width / 2, top, title);
#else
    FontDrawMessage(-width / 2, top, "\x83\x89\x83\x93\x83\x4C\x83\x93\x83\x4F");
#endif
    {
        f32 sy = 145.0f;
        f32 sh = 220.0f;
        scissorX = 304.0f + left;
        scissorW = winW;
        GXSetScissor(scissorX, (u16)sy, scissorW, (u16)sh);
    }

    num = 0;
    for (i = 0; i < rank_wp->count; i++) {
        x = 10.0f + left;
        y = dwp->scroll + (top - num * 20 - 40.0f);
        if (rank_wp->entries[i].flags & RANK_HIDDEN) {
            continue;
        }
        PSMTXScale(scale, 0.8f, 0.8f, 0.8f);
        if (num != 0) {
#ifdef VERSION_G8ME01
            if ((u32)gp->language == 0) {
                sprintf(entry, "%s%s", num / 10 ? suuji[num / 10] : "", suuji[num % 10]);
            } else {
                sprintf(entry, "%2d", num);
            }
#else
            sprintf(entry, "%s%s", num / 10 ? suuji[num / 10] : "", suuji[num % 10]);
#endif
            FontDrawStart();
            {
                GXColor color = numberColor;
                FontDrawColor(color);
            }
            PSMTXTrans(mtx, x, y, 0.0f);
            PSMTXConcat(mtx, scale, mtx);
            FontDrawMessageMtx(mtx, entry);
        }
        FontDrawStart();
        if (rank_wp->entries[i].flags & RANK_PLAYER) {
            GXColor color = playerColor;
            FontDrawColor(color);
        } else {
            GXColor color = nameColor;
            FontDrawColor(color);
        }
#ifdef VERSION_G8ME01
        name = msgSearch(fighters[rank_wp->entries[i].id].name);
        nameW = FontGetMessageWidth(name);
#endif
        x = 40.0f + x;
        PSMTXTrans(mtx, x, y, 0.0f);
        PSMTXConcat(mtx, scale, mtx);
#ifdef VERSION_G8ME01
        if (nameW > 220.0f) {
            PSMTXScale(scale, 220.0f / nameW, 1.0f, 1.0f);
            PSMTXConcat(mtx, scale, mtx);
        }
        FontDrawMessageMtx(mtx, name);
#else
        FontDrawMessageMtx(mtx, msgSearch(fighters[rank_wp->entries[i].id].name));
#endif
        num++;
    }
    {
        f32 sy = 137.0f;
        f32 sh = 228.0f;
        GXSetScissor(scissorX, (u16)sy, scissorW, (u16)sh);
    }
    {
        f32 y = 95.0f + dwp->scroll;
        Vec pos = cursorPos;
        pos.x = 15.0f + left;
        pos.y = y - 24.0f;
        iconDispGx(&pos, 0x10, ICON_ITEM_CHAMPS_BELT, 0.8f);
    }
    GXSetScissor(0, 0, 608, 480);
    if (dwp->page > 0) {
        {
            Vec pos = upArrowPos;
            pos.x = left + winW;
            iconDispGx(&pos, 0x10, ICON_PAGE_ARROW, -1.0f);
        }
        {
            Vec pos = upButtonPos;
            pos.x = left + winW;
            iconDispGx(&pos, 0x10, ICON_TRIGGER_L, 0.6f);
        }
    }
    if (dwp->page < 1) {
        {
            Vec pos = downArrowPos;
            pos.x = left + winW;
            iconDispGx(&pos, 0x10, ICON_PAGE_ARROW, 1.0f);
        }
        {
            Vec pos = downButtonPos;
            pos.x = left + winW;
            iconDispGx(&pos, 0x10, ICON_TRIGGER_R, 0.6f);
        }
    }
}

static void printRanking(void) {
    s32 i;

    evtGetValue(NULL, GSW_STORY_PROGRESS);
    for (i = 0; i < rank_wp->count; i++) {
    }
}

USER_FUNC(evt_tou_set_rule_result) {
    s32 result = evtGetValue(event, event->args[0]);

    if (result & 4) {
        swSet(2443);
    } else if (result & 2) {
        swClear(2443);
    } else if (result & 1) {
        swClear(2443);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_tou_set_ranking_win_lose) {
    EvtWord* args = event->args;
    s32 lose = evtGetValue(event, args[0]);
    s32 id = evtGetValue(event, args[1]);
    s16 min;
    s16 max;
    s32 i;

    if (lose == 0) {
        for (i = 0; i < rank_wp->count; i++) {
            if (id == rank_wp->entries[i].id) {
                rank_wp->entries[i].flags |= RANK_BELOW;
                break;
            }
        }
        swSet(2442);
    } else {
        for (i = 0; i < rank_wp->count; i++) {
            if (id == rank_wp->entries[i].id) {
                rank_wp->entries[i].flags &= ~RANK_BELOW;
                break;
            }
        }
        swClear(2442);
    }

    min = 1000;
    max = -1000;
    for (i = 0; i < rank_wp->count; i++) {
        if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
            s32 b = 0;
            b += rand() % 4;
            b += rand() % 4;

            rank_wp->entries[i].points.total += b - 3;
            if (rank_wp->entries[i].points.total < min) {
                min = rank_wp->entries[i].points.total;
            }
            if (rank_wp->entries[i].points.total > max) {
                max = rank_wp->entries[i].points.total;
            }
        }
    }
    swByteSet(507, id);
    swSet(2532);
    rankingControll();
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_tou_get_rule_msg2) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 num;
    RuleData* rules;
    s32 i;
    s32 rank;

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 172) {
        rank = 0;
        for (i = 0; i < rank_wp->count; i++) {
            if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
                if (rank_wp->entries[i].flags & RANK_PLAYER) {
                    break;
                }
                rank++;
            }
        }
        if (rank > 10) {
            rules = ruleDt_2bu;
            num = 10;
        } else {
            rules = ruleDt_1bu;
            num = 14;
        }
    } else {
        rules = ruleDt_re;
        num = 14;
    }
    for (i = 0; i < num; i++) {
        if (id == rules[i].id) {
            break;
        }
    }
    if (i == num) {
        i = num - 1;
    }
    evtSetValue(event, args[1], (EvtWord)msgSearch(rules[i].desc));
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_tou_get_rule_info) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    s32 num;
    RuleData* rules;
    s32 i;
    s32 rank;

    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 172) {
        rank = 0;
        for (i = 0; i < rank_wp->count; i++) {
            if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
                if (rank_wp->entries[i].flags & RANK_PLAYER) {
                    break;
                }
                rank++;
            }
        }
        if (rank > 10) {
            rules = ruleDt_2bu;
            num = 10;
        } else {
            rules = ruleDt_1bu;
            num = 14;
        }
    } else {
        rules = ruleDt_re;
        num = 14;
    }
    if (type == 0) {
        for (i = 0; i < num; i++) {
            if (rules[i].id == 6) {
                break;
            }
        }
    } else if (type == 10) {
        while (TRUE) {
            i = rand() % num;
            if (rules[i].id != 48 && rules[i].id != 32 && rules[i].id != 64) {
                break;
            }
        }
    } else {
        i = rand() % num;
    }
    evtSetValue(event, args[1], (EvtWord)rules[i].name);
    evtSetValue(event, args[2], rules[i].id);
    evtSetValue(event, args[3], rules[i].field_C);
    evtSetValue(event, args[4], rules[i].field_10);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_tou_get_fighter_tribe) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 idx = evtGetValue(event, args[1]);
    s32 setupId = fighterDt[id].setupId;
    TattleEntry* tattle = battleGetUnitMonosiriPtr(btlsetup_tou_tbl[setupId].select->groupSetup->enemyData[idx].mUnitKindParams->mUnitType);
    NpcTribeInfo* tribe;
    const char* name;
    BOOL found = FALSE;

    for (tribe = npcTribe; tribe->nameJp != NULL; tribe++) {
        if (strcmp(tribe->modelName, tattle->modelName) == 0) {
            found = TRUE;
            break;
        }
    }
    if (!found) {
        name = "\x83\x4E\x83\x8A\x83\x7B\x81\x5B";
        tribe = npcGetTribe(name);
    } else {
        name = tribe->nameJp;
    }
    if (setupId == 2 && idx == 1) {
        name = "\x83\x70\x83\x5E\x83\x74\x83\x40\x83\x43\x83\x5E\x81\x5B";
        tribe = npcGetTribe(name);
    }
    if (setupId == 10 && idx == 1) {
        name = "\x83\x41\x83\x43\x83\x41\x83\x93\x83\x56\x83\x93\x83\x47\x83\x82\x83\x93\x32";
        tribe = npcGetTribe(name);
    }
    if (setupId == 13 && idx == 1) {
        name = "\x83\x45\x83\x89\x83\x70\x83\x5E";
        tribe = npcGetTribe(name);
    }
    if (setupId == 15 && idx == 2) {
        name = "\x83\x4A\x83\x81\x83\x62\x83\x4E\x97\xCE\x8B\xF3";
        tribe = npcGetTribe(name);
    }
    if (setupId == 11) {
        name = "\x83\x6E\x83\x43\x83\x70\x81\x5B\x83\x67\x83\x51\x83\x81\x83\x62\x83\x67";
        tribe = npcGetTribe(name);
    }
    evtSetValue(event, args[2], (EvtWord)tribe->modelName);
    evtSetValue(event, args[3], (EvtWord)name);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_tou_get_fighter_num) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], btlsetup_tou_tbl[fighterDt[id].setupId].select->groupSetup->numEnemies);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_tou_get_fighter_battle_msg) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    FighterData* fighters = evtGetValue(NULL, GSW_STORY_PROGRESS) < 172 ? fighterDt : fighterDt_re;

    evtSetValue(event, args[1], (EvtWord)fighters[id].taunt);
    evtSetValue(event, args[2], (EvtWord)fighters[id].victory);
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_tou_get_fighter_rank) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 rank = 0;
    s32 i;

    for (i = 0; i < rank_wp->count; i++) {
        RankEntry* entry = &rank_wp->entries[i];
        if (!(entry->flags & RANK_HIDDEN)) {
            if (id == entry->id) {
                break;
            }
            rank++;
        }
    }
    evtSetValue(event, args[1], rank);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_tou_get_fighter_name2) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    FighterData* fighters = evtGetValue(NULL, GSW_STORY_PROGRESS) < 172 ? fighterDt : fighterDt_re;

    evtSetValue(event, args[1], (EvtWord)msgSearch(fighters[id].title));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_tou_get_fighter_info) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    FighterData* fighters = evtGetValue(NULL, GSW_STORY_PROGRESS) < 172 ? fighterDt : fighterDt_re;

    evtSetValue(event, args[1], (EvtWord)msgSearch(fighters[id].name));
    evtSetValue(event, args[2], fighterDt[id].setupId);
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_tou_get_next_match) {
    EvtWord* args = event->args;
    BOOL reverse = evtGetValue(event, GSWF_GLITZ_PIT_WON_LAST_MATCH);
    s32 list[32];
    s32 i;
    s32 rank;
    s32 num;

    num = 0;
    for (i = 0; i < rank_wp->count; i++) {
        if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
            list[num++] = rank_wp->entries[i].id;
        }
    }
    rank = 0;
    for (i = 0; i < rank_wp->count; i++) {
        if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
            if (rank_wp->entries[i].flags & RANK_PLAYER) {
                break;
            }
            rank++;
        }
    }
    if (rank > 0) {
        if (reverse) {
            evtSetValue(event, args[0], list[rank - 1]);
        } else if (rank + 1 < num) {
            evtSetValue(event, args[0], list[rank + 1]);
        } else {
            evtSetValue(event, args[0], list[rank - 1]);
        }
    } else {
        evtSetValue(event, args[0], list[1]);
    }
    printRanking();
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_tou_get_ranking) {
    EvtWord* args = event->args;
    s32 i;
    s32 rank;

    rank = 0;
    for (i = 0; i < rank_wp->count; i++) {
        if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
            if (rank_wp->entries[i].flags & RANK_PLAYER) {
                break;
            }
            rank++;
        }
    }
    evtSetValue(event, args[0], rank);
    return EVT_RETURN_DONE;
}

static void rankingControll(void) {
    s32 above[32];
    s32 below[32];
    s32 nAbove;
    s32 nBelow;
    s32 i;
    s32 j;
    s32 idx2;
    s32 tmp;
    s32 idx;

    s32 rank;
    RankEntry* entry;
    RankEntry* buf;
    nAbove = 0;
    nBelow = 0;
    for (idx = 0, entry = rank_wp->entries; idx < rank_wp->count; idx++, entry++) {
        if (!(entry->flags & RANK_PLAYER)) {
            if (entry->flags & RANK_BELOW) {
                below[nBelow] = idx;
                nBelow++;
            } else {
                above[nAbove] = idx;
                nAbove++;
            }
        }
    }

    if (nAbove > 0) {
        for (i = 0; i < nAbove - 1; i++) {
            for (j = i + 1; j < nAbove; j++) {
                if (rank_wp->entries[above[i]].points.total < rank_wp->entries[above[j]].points.total) {
                    tmp = above[i];
                    above[i] = above[j];
                    above[j] = tmp;
                }
            }
        }

        for (i = 0; i < nAbove - 1; i++) {
            for (idx2 = i + 1; idx2 < nAbove; idx2++) {
                FighterData* fj = &fighterDt[rank_wp->entries[above[idx2]].id];
                FighterData* fi = &fighterDt[rank_wp->entries[above[i]].id];
                if ((fj->flags & 5) && (fi->flags & 2)) {
                    tmp = above[i];
                    above[i] = above[idx2];
                    above[idx2] = tmp;
                }
            }
        }

        if (nAbove > 0) {
            for (idx = 0; idx < nAbove; idx++) {
                if (fighterDt[rank_wp->entries[above[idx]].id].flags & 4) {
                    break;
                }
            }
            if (idx < nAbove) {
                insert(above, nAbove, idx, 0);
            }
        }

        if (nAbove > 1) {
            for (idx = 0; idx < nAbove; idx++) {
                if (fighterDt[rank_wp->entries[above[idx]].id].flags & 0x1000) {
                    break;
                }
            }
            if (idx < nAbove) {
                insert(above, nAbove, idx, 1);
            }
        }

        if (nAbove > 10) {
            for (idx = 0; idx < nAbove; idx++) {
                if (fighterDt[rank_wp->entries[above[idx]].id].flags & 0x800) {
                    break;
                }
            }
            if (idx < nAbove) {
                insert(above, nAbove, idx, 10);
            }
        }

        if (nAbove > 18) {
            for (idx = 0; idx < nAbove; idx++) {
                if (fighterDt[rank_wp->entries[above[idx]].id].flags & 0x200) {
                    break;
                }
            }
            if (idx < nAbove) {
                insert(above, nAbove, idx, 18);
            }
        }

        if (nAbove > 19) {
            for (idx = 0; idx < nAbove; idx++) {
                if (fighterDt[rank_wp->entries[above[idx]].id].flags & 0x400) {
                    break;
                }
            }
            if (idx < nAbove) {
                insert(above, nAbove, idx, 19);
            }
        }

        if (nAbove > 16) {
            for (idx = 0; idx < nAbove; idx++) {
                if (fighterDt[rank_wp->entries[above[idx]].id].flags & 0x100) {
                    break;
                }
            }
            if (idx < nAbove) {
                insert(above, nAbove, idx, 14);
            }
        }

        // Check if a match is booked (2532), and move our opponent (507) to above us for the fight. 
        if (swGet(2532)) {
            for (idx = 0; idx < nAbove; idx++) {
                if (above[idx] == swByteGet(507)) {
                    break;
                }
            }
            if (idx < nAbove) {
                insert(above, nAbove, idx, nAbove - 1);
            }
        }
    }

    if (nBelow > 0) {
        for (i = 0; i < nBelow - 1; i++) {
            for (j = i + 1; j < nBelow; j++) {
                if (rank_wp->entries[below[i]].points.total < rank_wp->entries[below[j]].points.total) {
                    tmp = below[i];
                    below[i] = below[j];
                    below[j] = tmp;
                }
            }
        }

        for (i = 0; i < nBelow - 1; i++) {
            for (idx2 = i + 1; idx2 < nBelow; idx2++) {
                FighterData* fj = &fighterDt[rank_wp->entries[below[idx2]].id];
                FighterData* fi = &fighterDt[rank_wp->entries[below[i]].id];
                if ((fj->flags & 5) && (fi->flags & 2)) {
                    tmp = below[i];
                    below[i] = below[idx2];
                    below[idx2] = tmp;
                }
            }
        }

        if (nBelow > 0) {
            for (idx = 0; idx < nBelow; idx++) {
                if (fighterDt[rank_wp->entries[below[idx]].id].flags & 4) {
                    break;
                }
            }
            if (idx < nBelow) {
                insert(below, nBelow, idx, 0);
            }
        }
        
        if (nBelow > 0) {
            for (idx = 0; idx < nBelow; idx++) {
                if (fighterDt[rank_wp->entries[below[idx]].id].flags & 0x800) {
                    break;
                }
            }
            if (idx < nBelow) {
                for (tmp = idx, idx = 0; idx < nBelow; idx++) {
                    if (fighterDt[rank_wp->entries[below[idx]].id].flags & 2) {
                        break;
                    }
                }
                if (idx < nBelow && idx > 0) {
                    insert(below, nBelow, tmp, idx - 1);
                }
            }
        }

        // Check if a match is booked (2532), and move our opponent (507) to above us for the fight.
        if (swGet(2532)) {
            for (idx = 0; idx < nBelow; idx++) {
                if (below[idx] == swByteGet(507)) {
                    break;
                }
            }
            if (idx < nBelow) {
                insert(below, nBelow, idx, 0);
            }
        }
    }

    buf = (RankEntry*)__memAlloc(HEAP_DEFAULT, sizeof(RankEntry) * rank_wp->count);
    for (i = 0; i < nAbove; i++) {
        buf[i] = rank_wp->entries[above[i]];
    }
    buf[nAbove].flags = RANK_PLAYER;
    buf[nAbove].id = FIGHTER_PLAYER;
    buf[nAbove].points.total = 0;
    for (i = 0; i < nBelow; i++) {
        j = nAbove + 1 + i;
        buf[j] = rank_wp->entries[below[i]];
    }
    memcpy(rank_wp->entries, buf, sizeof(RankEntry) * rank_wp->count);
    __memFree(HEAP_DEFAULT, buf);

    // Calculate our current rank and store it in 580.
    rank = 0;
    for (i = 0; i < rank_wp->count; i++) {
        if (!(rank_wp->entries[i].flags & RANK_HIDDEN)) {
            if (rank_wp->entries[i].flags & RANK_PLAYER) {
                break;
            }
            rank++;
        }
    }
    swByteSet(580, rank);
}

static void insert(s32* arr, s32 n, s32 from, s32 to) {
    s32 i;

    if (from < to) {
        s32 tmp = arr[from];
        for (i = from; i < to; i++) {
            arr[i] = arr[i + 1];
        }
        arr[to] = tmp;
    } else {
        s32 tmp = arr[from];
        for (i = from; i > to; i--) {
            arr[i] = arr[i - 1];
        }
        arr[to] = tmp;
    }
}

USER_FUNC(tou2_rankingReset) {
    s32 i;

    rankingExit();
    pouchRemoveItem(ITEM_CHAMPS_BELT);
    for (i = 0; i < 20; i++) {
        swClear(i + 2465);
    }
    swClear(2392);
    swClear(2399);
    swClear(2400);
    swClear(2383);
    swClear(2529);
    swClear(2532);
    swClear(2403);
    rankingInit();
    return EVT_RETURN_DONE;
}

static USER_FUNC(evt_rankingReInit) {
    rankingExit();
    rankingInit();
    return EVT_RETURN_DONE;
}

static void rankingExit(void) {
    s32 i;
    s32 j;
    RankEntry* entry;

    for (i = 0, entry = rank_wp->entries; i < rank_wp->count; i++, entry++) {
        if (entry->flags & RANK_BELOW) {
            swSet(entry->id + 2465);
        } else {
            swClear(entry->id + 2465);
        }
        for (j = 0; j < 2; j++) {
            swByteSet(2 * entry->id + 519 + j, entry->points.bytes[j]);
        }
    }
    _mapFree(rank_wp->entries);
}

static void rankingInit(void) {
    s32 temp;
    s32 count;
    int i;
    RankEntry* entry;

    memset(rank_wp, 0, sizeof(RankData));
    // We need to handle the excluded entries if we're doing rematches
    temp = evtGetValue(NULL, GSW_STORY_PROGRESS);
    count = 21;
    if (temp < 172) {
        count = 23;
    }
    rank_wp->count = count;
    rank_wp->entries = _mapAlloc(rank_wp->count * sizeof(RankEntry));
    memset(rank_wp->entries, 0, rank_wp->count * sizeof(RankEntry));

    for (i = 0, entry = rank_wp->entries; i < rank_wp->count; i++, entry++) {
        entry->flags = 0;
        entry->id = i;
        if (swGet(entry->id + 2465)) {
            entry->flags |= RANK_BELOW;
        }
        if (swGet(2529)) {
            int j;
            for (j = 0; j < 2; j++) {
                entry->points.bytes[j] = swByteGet(2 * entry->id + 519 + j);
            }
        } else {
            entry->points.total = fighterDt[entry->id].field_18;
        }
        if (entry->id == FIGHTER_PLAYER) {
            entry->flags |= RANK_PLAYER;
        }
    }

    swSet(2529);

    // Set up the initial run-through
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) < 172) {
        rank_wp->entries[FIGHTER_SWOOPS].flags |= RANK_HIDDEN;
        rank_wp->entries[FIGHTER_SPINIA].flags |= RANK_HIDDEN;

        // If we're past this point in the story, hide King K
        if (evtGetValue(NULL, GSWF(2412))) {
            rank_wp->entries[FIGHTER_KP_KOOPAS].flags |= RANK_HIDDEN;
        }

        // If we're past this story, Sir Swoop has been introduced
        if (evtGetValue(NULL, GSWF(2413))) {
            rank_wp->entries[FIGHTER_SWOOPS].flags &= ~RANK_HIDDEN;

            if (!evtGetValue(NULL, GSWF(2530))) {
                rank_wp->entries[FIGHTER_SWOOPS].flags |= RANK_BELOW;
            }
            evtSetValue(NULL, GSWF(2530), TRUE);
        }

        // If we're past this point in the story, Bandy Andy got "retired" and The Destructors were added.
        if (evtGetValue(NULL, GSWF(2421))) {
            rank_wp->entries[FIGHTER_BANDITS].flags |= RANK_HIDDEN;
            rank_wp->entries[FIGHTER_SPINIA].flags &= ~RANK_HIDDEN;

            if (!evtGetValue(NULL, GSWF(2531))) {
                rank_wp->entries[FIGHTER_SPINIA].flags |= RANK_BELOW;
            }
            evtSetValue(NULL, GSWF(2531), TRUE);
        }
    }
    rankingControll();
}

void _unresolved(void) {
}

void _epilog(void) {
    voidfunctionptr* dtor;

    rankingExit();
    relSetEvtAddr("tou_03", NULL);
    relSetBtlAddr("tou", NULL, NULL);
    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("tou_03", tou_03_init_evt);
    relSetBtlAddr("tou", btlsetup_tou_tbl, btl_tou_setup_no_tbl);
    rankingInit();
}

