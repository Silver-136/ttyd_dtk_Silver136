#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"
#include "driver/npcdrv.h"
#include "driver/effdrv.h"
#include <dolphin/mtx.h>
#include "driver/hitdrv.h"

typedef struct PuniTarget {
    s32 flags;           // 0x0
    Vec pos;             // 0x4
    HitEntry* hit;       // 0x10
} PuniTarget;

#define PUNI_DATA_MAX 211
#define PUNI_TEX_INFO_MAX 200

typedef struct PuniAnimSet {
    s16* table[10]; // 0x0
    s16 count[10];  // 0x28
    s16 end[10];    // 0x3C
} PuniAnimSet;

// PuniData.flags (mri_puni). Kinds: PUNI/TOGE/SPRITE_*/NPC_* (one each). 0x4, 0x2000 and 0x400000 (set, never
// read), 0x10000/0x200000 (scripts only) and 0x80000000 are not named.
typedef enum PuniFlags {
    PUNI_FLAG_PUNI = (1 << 0),               // 0x1
    PUNI_FLAG_TOGE = (1 << 1),               // 0x2
    PUNI_FLAG_SPRITE_PUNI = (1 << 3),        // 0x8
    PUNI_FLAG_SPRITE_TOGE = (1 << 4),        // 0x10
    PUNI_FLAG_NPC_GUIDE = (1 << 5),          // 0x20
    PUNI_FLAG_NPC_GUIDE_SISTER = (1 << 6),   // 0x40
    PUNI_FLAG_NPC_CHIBI_TOGE = (1 << 7),     // 0x80
    PUNI_FLAG_NPC_RIVAL = (1 << 8),          // 0x100
    PUNI_FLAG_MOVE_STOP = (1 << 9),          // 0x200, evt_puni_all_movestop
    PUNI_FLAG_ACTIVE = (1 << 10),            // 0x400
    PUNI_FLAG_HIDDEN = (1 << 11),            // 0x800
    PUNI_FLAG_WALL_CHECK = (1 << 12),        // 0x1000
    PUNI_FLAG_DEAD = (1 << 14),              // 0x4000, evt_mri_puni_win_attack revives by clearing it
    PUNI_FLAG_STRAY = (1 << 15),             // 0x8000, puniModeHagure
    PUNI_FLAG_SCRIPT_ANIM = (1 << 17),       // 0x20000
    PUNI_FLAG_SCRIPT_CONTROL = (1 << 18),    // 0x40000, around cutscenes
    PUNI_FLAG_IN_CHAIN = (1 << 19),          // 0x80000, evt_mri_toge_make_retsu
    PUNI_FLAG_COLLIDE = (1 << 20),           // 0x100000
    PUNI_FLAG_NOT_IN_PARTY = (1 << 23),      // 0x800000
    PUNI_FLAG_OWN_ANIM_TIME = (1 << 24),     // 0x1000000
    PUNI_FLAG_UPDATED = (1 << 25),           // 0x2000000
    PUNI_FLAG_SENSED = (1 << 26),            // 0x4000000
    PUNI_FLAG_DIZZY = (1 << 27),             // 0x8000000
    PUNI_FLAG_PUSHING = (1 << 28),           // 0x10000000
    PUNI_FLAG_NEAR = (1 << 29),              // 0x20000000
    PUNI_FLAG_KEEP = (1 << 30),              // 0x40000000
    PUNI_FLAG_MARIO_UNUSED = (1 << 2),        // 0x4, the Mario stand-in entry Punidat[0]; no code tests this bit
    PUNI_FLAG_KNOCKED_BACK_UNUSED = (1 << 13), // 0x2000, damage hop in progress; no code tests this bit
    PUNI_FLAG_HIT_WALL_UNUSED = (1 << 22),     // 0x400000, the last move hit something; no code tests this bit
} PuniFlags;

// PuniData.flags2. 0x1 is not named.
typedef enum PuniFlags2 {
    PUNI_FLAG2_DIZZY_ICON = (1 << 1),        // 0x2
    PUNI_FLAG2_CROSSING_LEDGE = (1 << 2),    // 0x4
    PUNI_FLAG2_ON_SLOPE = (1 << 3),          // 0x8
    PUNI_FLAG2_DEATH_PENDING = (1 << 4),     // 0x10
    PUNI_FLAG2_OBSTACLE = (1 << 5),          // 0x20
} PuniFlags2;

// PuniWork.flags: the pipe (bero directionId) the floor was entered from, and floor properties
typedef enum PuniWorkFlags {
    PUNI_WORK_FLAG_PIPE_WEST = (1 << 0),     // 0x1, directionId 6
    PUNI_WORK_FLAG_PIPE_EAST = (1 << 1),     // 0x2, directionId 2
    PUNI_WORK_FLAG_PIPE_UP = (1 << 2),       // 0x4, directionId 8
    PUNI_WORK_FLAG_PIPE_DOWN = (1 << 3),     // 0x8, directionId 9
    PUNI_WORK_FLAG_NO_NEXT_MAP = (1 << 4),   // 0x10
    PUNI_WORK_FLAG_BATTLE_FLOOR = (1 << 5),  // 0x20, floors 9 and 12
} PuniWorkFlags;

typedef struct PuniWork {
    s32 systemMode;           // 0x0
    PuniAnimSet anim[2];      // 0x4
    u8 animNames[0x324 - 0xA4];   // 0xA4
    u32 flags;               // 0x324, PuniWorkFlags
    s32 floor;               // 0x328
    f32 posY[4];              // 0x32C
    s32 ymode;                // 0x33C
    s32 mapKind;              // 0x340
    Vec recoverPos;               // 0x344
    f32 stuckTop0;               // 0x350
    f32 stuckBottom0;               // 0x354
    f32 stuckTop1;               // 0x358
    f32 stuckBottom1;               // 0x35C
    struct PuniData* marioPuni;  // 0x360
    struct PuniData* reserved_364;  // 0x364
    struct PuniData* reserved_368;  // 0x368
    struct PuniData* reserved_36C;  // 0x36C
    struct PuniData* reserved_370;  // 0x370
    PuniTarget targets[10];   // 0x374
    s32 allMode;               // 0x43C
    s32 prevAllMode;               // 0x440
    BOOL showPuniCounter; //0x444
    BOOL showRemaining; //0x448
    s32 puniCounterAlpha; //0x44C
    s32 seed0;                // 0x450
    s32 seed1;                // 0x454
    u32 sfx458;               // 0x458
    s32 puniMoveCount;        // 0x45C
    s32 puniMoveLevel;        // 0x460
    u8 pad_464[0x468 - 0x464]; // 0x464
    u32 sfx468;               // 0x468
    s32 togeMoveCount;        // 0x46C
    s32 togeMoveLevel;        // 0x470
    s32 togeMoveLevelPrev;    // 0x474
    u32 sfxTogetti;           // 0x478
    s32 beroMode;             // 0x47C
    Vec beroStart;            // 0x480
    Vec beroEnd;              // 0x48C
    u8 pad_498[0x4B0 - 0x498]; // 0x498
    f32 beroStartRange;       // 0x4B0
    f32 beroEndRange;         // 0x4B4
    u8 pad_4B8[0x4C0 - 0x4B8]; // 0x4B8
    s32 fightSfxCount1;               // 0x4C0
    s32 fightSfxCount2;               // 0x4C4
    u8 pad_4C8[0x4CC - 0x4C8]; // 0x4C8
    s32 windDisabled;               // 0x4CC
} PuniWork;

typedef struct PuniData {
    NpcEntry* npc;                      // 0x0
    s32 flags;                          // 0x4, PuniFlags
    s32 flags2;                         // 0x8, PuniFlags2
    u16 mode;                           // 0xC
    u16 prevMode;                       // 0xE
    u8 pad_10[0x114 - 0x10];            // 0x10
    u16 modeStep;                       // 0x114
    s16 index;                          // 0x116
    s16 groupId;                        // 0x118
    s16 depth;                          // 0x11A
    u16 anim;                           // 0x11C
    u16 prevAnim;                       // 0x11E
    u8 animHoldCount;                   // 0x120
    u8 flag1Timer;                      // 0x121
    u8 pad_122[0x124 - 0x122];          // 0x122
    s32 animTime;                       // 0x124
    s32 timer;                          // 0x128
    Vec pos;                            // 0x12C
    Vec prevPos;                        // 0x138
    Vec dispOfs;                        // 0x144, added to pos for the npc/shadow matrix
    Vec savedPos;                       // 0x150, pos saved on a wall hit; intpl target
    Vec vel;                            // 0x15C
    Vec pushDir;                        // 0x168
    f32 maxSpeed;                       // 0x174
    f32 accelStep;                      // 0x178
    u8 pad_17C[0x180 - 0x17C];          // 0x17C
    f32 dir;                            // 0x180
    f32 spin;                           // 0x184
    f32 rotOffsetY;                     // 0x188
    f32 scale;                          // 0x18C, -> npc->scale.x/y and PunidrvPose.scale; ramps 0 -> 1
    Vec radius;                         // 0x190
    f32 followRange;                    // 0x19C
    f32 breathPower;                    // 0x1A0
    f32 marioDist;                      // 0x1A4
    f32 parentDist;                     // 0x1A8
    f32 moveDist;                       // 0x1AC
    f32 prevParentDist;                 // 0x1B0
    s32 massTarget;                     // 0x1B4
    f32 massTargetDistSq;               // 0x1B8
    s32 awayTarget;                     // 0x1BC
    f32 awayTargetDistSq;               // 0x1C0
    u8 pad_1C4[0x1C8 - 0x1C4];          // 0x1C4
    union {
        OSTime startTime;               // 0x1C8, gp->renderTime when the timed move started
        struct {
            s32 startTimeHi;            // 0x1C8
            s32 startTimeLo;            // 0x1CC, low word: elapsed ticks = renderTime - this
        };
    };
    struct PuniData* parent;            // 0x1D0
    struct PuniData* child1;            // 0x1D4
    struct PuniData* child2;            // 0x1D8
    struct PuniData* target;            // 0x1DC
    s32 modeVar0;                       // 0x1E0
    s32 modeVar1;                       // 0x1E4
    s32 modeVar2;                       // 0x1E8
    f32 modeF0;                         // 0x1EC, per-mode float (jump: base height; wind: speed)
    f32 modeF1;                         // 0x1F0, per-mode float (jump: initial vertical term; wind: acceleration)
    f32 modeF2;                         // 0x1F4, per-mode float (jump: linear term; wind: speed cap)
    Vec moveStart;                      // 0x1F8
    Vec moveEnd;                        // 0x204, intpl end point (moveStart = start)
    HitEntry* hitMove;                  // 0x210
    HitEntry* hitCheck;                 // 0x214
    EffectEntry* effect;                // 0x218
    u8 pad_21C[0x2AC - 0x21C];          // 0x21C
    Vec bubblePos;                      // 0x2AC, hagure speech-bubble anchor; passed to effFukidashiEntry as a fake NpcEntry's position
    u8 pad_2B8[0x560 - 0x2B8];          // 0x2B8
    Vec wallNrm;                        // 0x560, wall normal at the last hit; pushDir = -wallNrm
    void (*modeFunc)(struct PuniData*); // 0x56C
    void (*moveFunc)(struct PuniData*); // 0x570
    s16 hp;                             // 0x574
    s16 maxHp;                          // 0x576
    s16 regen;                          // 0x578
    s16 regenTimer;                     // 0x57A
    s16 regenInterval;                  // 0x57C
    s16 attack;                         // 0x57E
    s16 defense;                        // 0x580
    s16 reserved_582;                   // 0x582
    s16 damage;                         // 0x584
    s16 attacked;                       // 0x586
    s16 hitRate;                        // 0x588
    s16 hitRateGuard;                   // 0x58A
    s32 reserved_58C;                   // 0x58C
    Vec reserved_590;                   // 0x590
    Vec reserved_59C;                   // 0x59C
} PuniData;

typedef struct PuniMap {
    f32 x;                // 0x0
    f32 z;                // 0x4
    s32 width;            // 0x8
    s32 height;           // 0xC
    s32 size;             // 0x10
    s32 enabled;          // 0x14
    void* src;            // 0x18
    s32 inRange;            // 0x1C
    u16* data;            // 0x20
} PuniMap;

typedef struct PuniTexInfo {
    char* group; // 0x0
    char* anim; // 0x4
    f32 frame;   // 0x8
} PuniTexInfo;

USER_FUNC(evt_puni_mem_init);
void PuniDokuritsu(PuniData* p);
void puniMoveDecel(PuniData* p);
void puniswTrapGetindex(s32 idx, s32* out);
PuniData* puniGetPunidat(void);
USER_FUNC(evt_make_texbuf);
USER_FUNC(evt_puni_all_fall_check);
USER_FUNC(evt_puni_all_movestop);
USER_FUNC(evt_puni_all_save);
USER_FUNC(evt_puni_attack_on);
USER_FUNC(evt_puni_count);
USER_FUNC(evt_puni_count_area);
USER_FUNC(evt_puni_dokan_onoff);
USER_FUNC(evt_puni_exit);
USER_FUNC(evt_puni_fade_onoff);
USER_FUNC(evt_puni_flag_onoff);
USER_FUNC(evt_puni_force_se_off);
USER_FUNC(evt_puni_get_anim);
USER_FUNC(evt_puni_get_dir);
USER_FUNC(evt_puni_get_flag);
USER_FUNC(evt_puni_get_lsw_index);
USER_FUNC(evt_puni_get_mode);
USER_FUNC(evt_puni_get_position);
USER_FUNC(evt_puni_get_systemmode);
USER_FUNC(evt_puni_get_trap_index);
USER_FUNC(evt_puni_init_puni);
USER_FUNC(evt_puni_init_teki);
USER_FUNC(evt_puni_jump_position);
USER_FUNC(evt_puni_korokoro);
USER_FUNC(evt_puni_landon_check);
USER_FUNC(evt_puni_main);
USER_FUNC(evt_puni_move_position);
USER_FUNC(evt_puni_move_position2);
USER_FUNC(evt_puni_pera_onoff);
USER_FUNC(evt_puni_puninum_alpha);
USER_FUNC(evt_puni_puninum_onoff);
USER_FUNC(evt_puni_rnd_pos);
USER_FUNC(evt_puni_set_anim);
USER_FUNC(evt_puni_set_dir);
USER_FUNC(evt_puni_set_hamari);
USER_FUNC(evt_puni_set_lsw_index);
USER_FUNC(evt_puni_set_maze);
USER_FUNC(evt_puni_set_mode);
USER_FUNC(evt_puni_set_position);
USER_FUNC(evt_puni_set_systemmode);
USER_FUNC(evt_puni_set_trap_index);
USER_FUNC(evt_puni_set_ymode);
USER_FUNC(evt_puni_set_yuka);
USER_FUNC(evt_puni_setsudan);
USER_FUNC(evt_puni_setup);
USER_FUNC(evt_puni_wait_systemmode);
USER_FUNC(evt_puni_winddisable_onoff);
USER_FUNC(evt_puni_zenmetsu_check);
USER_FUNC(evt_target_flag_onoff);
USER_FUNC(evt_target_set_position);
USER_FUNC(evt_target_set_soko);
USER_FUNC(puni_disp_info_kassencheck);
USER_FUNC(puni_disp_info_sub);
