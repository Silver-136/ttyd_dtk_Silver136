// The Great Tree (mri): the Puni crowd system's types (PuniWork, PuniData, PuniMap) and its user functions.

#pragma once

#include "version.h"
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

// PuniData.flags (mri_puni). Kinds: PUNI/TOGE/SPRITE_*/NPC_* (one each). Bits set but never read carry _UNUSED;
// 0x10000/0x200000 (used by scripts only) and 0x80000000 are not named.
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

// PuniData.flags2.
typedef enum PuniFlags2 {
    PUNI_FLAG2_HEALED = (1 << 0),            // 0x1, set when its HP rises, cleared 30 frames later (LLCOOLT's name)
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
    char animNames[4][10][16]; // 0xA4, pose names per story Puni (guide, sister, little Jabbi, rival) and animation id
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
    struct PuniData* guidePuni;  // 0x364
    struct PuniData* sisterPuni;  // 0x368
    struct PuniData* jabbiPuni;  // 0x36C
    struct PuniData* rivalPuni;  // 0x370
    PuniTarget targets[10];   // 0x374
    s32 allMode;               // 0x43C
    s32 prevAllMode;               // 0x440
    BOOL showPuniCounter; // 0x444
    BOOL showRemaining; // 0x448
    s32 puniCounterAlpha; // 0x44C
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
    NpcEntry* npc;                      // 0x0, slots 1-210: their PunidrvPose (draw pose) cast, set by puni_puni_init
    s32 flags;                          // 0x4, PuniFlags
    s32 flags2;                         // 0x8, PuniFlags2
    u16 mode;                           // 0xC, PUNI_MODE_*
    u16 prevMode;                       // 0xE, mode before a switch to wind, away jump etc. (dizzy mode returns to it)
#if VERSION <= VERSION_DEMO_JP
    s32 modeHistIdx;   // 0x10, demo: index into modeHist (0-127)
    u16 modeHist[128];
#else
    u8 pad_10[0x114 - 0x10];            // 0x10
#endif
    u16 modeStep;                       // 0x114, step inside the current mode's handler (0 on entering a mode)
    s16 index;                          // 0x116, slot number in Punidat (0 = Mario); staggers animation and searches
    s16 groupId;                        // 0x118, follow-chain group, copied from the parent (-1: not in a chain)
    s16 depth;                          // 0x11A, place in the follow chain: the parent's depth + 1 (0: head or none)
    u16 anim;                           // 0x11C, anim id = animNames index (0 S_1 stand, 1 W_1 walk, 2 R_1 run, ...)
    u16 prevAnim;                       // 0x11E, anim at the last draw (puniAnimation); 1000 on setup
    u8 animHoldCount;                   // 0x120, frames a follower kept its old stand/walk anim (at most 8)
    u8 flag1Timer;                      // 0x121, frames since PUNI_FLAG2_HEALED was set; cleared after 30
    s32 animTime;                       // 0x124, anim frame (counts itself with PUNI_FLAG_OWN_ANIM_TIME)
    s32 timer;                          // 0x128, per-mode frames (follower: time out of reach; others count down)
    Vec pos;                            // 0x12C
    Vec prevPos;                        // 0x138, pos before this frame's mode and move step; moveDist = |pos - prevPos|
    Vec dispOfs;                        // 0x144, added to pos for the npc/shadow matrix
    Vec savedPos;                       // 0x150, pos saved on a wall hit; intpl target
    Vec vel;                            // 0x15C, added to pos by the move step
    Vec pushDir;                        // 0x168, -wallNrm after an NPC-only wall hit (a ledge); it crosses that way
    f32 maxSpeed;                       // 0x174, speed cap of the limited move steps (3 by default)
    f32 accelStep;                      // 0x178, damage hop: vertical speed (5, -0.5 a frame); wind: added to modeF1
    u8 pad_17C[0x180 - 0x17C];          // 0x17C
    f32 dir;                            // 0x180, facing angle (90 or 270 from vel.x) -> npc->direction / pose dir
    f32 spin;                           // 0x184, z rotation in degrees (wind: 10 a frame) -> npc rotation.z, pose rotZ
    f32 rotOffsetY;                     // 0x188, rotation offset (8 while blown) -> npc->rotationOffset.y, pose offsetY
    f32 scale;                          // 0x18C, -> npc->scale.x/y and PunidrvPose.scale; ramps 0 -> 1
    Vec radius;                         // 0x190, x: body radius; y, z: added for the push-apart range (6, 2, 3)
    f32 followRange;                    // 0x19C, an attacker comes this close before it strikes this Puni (10)
    f32 breathPower;                    // 0x1A0, Flurrie's breath here (cloudGetBreathPower); over 10 blows it away
    f32 marioDist;                      // 0x1A4, distance to Mario (1000 when more than 32 units of height apart)
    f32 parentDist;                     // 0x1A8, distance to the parent Puni
    f32 moveDist;                       // 0x1AC, distance moved in the last frame
    f32 prevParentDist;                 // 0x1B0, parentDist of the frame before (closing in resets the lost timer)
    s32 massTarget;                     // 0x1B4, nearest gather target in Punigp->targets (-1: none), puniTargetSearch
    f32 massTargetDistSq;               // 0x1B8, its squared distance (1000000: none)
    s32 awayTarget;                     // 0x1BC, nearest flee target in Punigp->targets (-1: none)
    f32 awayTargetDistSq;               // 0x1C0, its squared distance (1000000: none)
    union {
        OSTime startTime;               // 0x1C8, gp->renderTime when the timed move started
        struct {
            s32 startTimeHi;            // 0x1C8
            s32 startTimeLo;            // 0x1CC, low word: elapsed ticks = renderTime - this
        };
    };
    struct PuniData* parent;            // 0x1D0, the Puni this one follows in the chain
    struct PuniData* child1;            // 0x1D4, first follower
    struct PuniData* child2;            // 0x1D8, second follower
    struct PuniData* target;            // 0x1DC, Puni it chases or fights (fight opponent; Mario's stand-in when blown)
    s32 modeVar0;                       // 0x1E0, per-mode integer (wind: spin direction; fall: the mode after landing)
    s32 modeVar1;                       // 0x1E4, per-mode integer (away jump: second pose set; gather: jitter frames)
    s32 modeVar2;                       // 0x1E8, per-mode integer (a countdown copied from modeVar1)
    f32 modeF0;                         // 0x1EC, per-mode float (jump: base height; wind: speed)
    f32 modeF1;                         // 0x1F0, per-mode float (jump: initial vertical term; wind: acceleration)
    f32 modeF2;                         // 0x1F4, per-mode float (jump: linear term; wind: speed cap)
    Vec moveStart;                      // 0x1F8, intpl start point; wind: blow direction; gather: jitter step
    Vec moveEnd;                        // 0x204, intpl end point (moveStart = start)
    HitEntry* hitMove;                  // 0x210, wall hit by the last move ray (NULL: none)
    HitEntry* hitCheck;                 // 0x214, wall hit by the slide check after it (slide undone; NULL: none)
    EffectEntry* effect;                // 0x218, attached effect (the balloon before an updraft); puniDeleteEffect
    u8 pad_21C[0x2AC - 0x21C];          // 0x21C
    Vec bubblePos;                      // 0x2AC, hagure speech-bubble anchor; passed to effFukidashiEntry as a fake NpcEntry's position
    u8 pad_2B8[0x560 - 0x2B8];          // 0x2B8
    Vec wallNrm;                        // 0x560, wall normal at the last hit; pushDir = -wallNrm
    void (*modeFunc)(struct PuniData*); // 0x56C, mode handler for PUNI_MODE_FUNC (puniModeCustom)
    void (*moveFunc)(struct PuniData*); // 0x570, move step for puniMoveCustom
    s16 hp;                             // 0x574, fight hp; 0 marks it dead (puniParaDamage)
    s16 maxHp;                          // 0x576, maximum hp (60)
    s16 regen;                          // 0x578, hp regained each regenInterval frames at rest (10)
    s16 regenTimer;                     // 0x57A, frames until the next regen; reset while it moves, is blown or dizzy
    s16 regenInterval;                  // 0x57C, frames between regens (60)
    s16 attack;                         // 0x57E, damage dealt = attack - the loser's defense
    s16 defense;                        // 0x580
    s16 reserved_582;                   // 0x582, set to 10 by the init functions, never read
    s16 damage;                         // 0x584, pending damage, subtracted from hp by puniParaDamage
    s16 attacked;                       // 0x586, already in an exchange this frame (puniParaAttack; cleared each frame)
    s16 hitRate;                        // 0x588, hit weight against an opponent that is not escaping (50)
    s16 hitRateGuard;                   // 0x58A, hit weight against an escaping opponent (70)
    s32 recoverReason;                  // 0x58C, puniPositionCheck result (1, 2) of the last put-back; never read
    Vec recoverFromPos;                 // 0x590, pos before the last put-back; never read
    Vec recoverToPos;                   // 0x59C, pos after the last put-back; never read
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

USER_FUNC(evt_puni_mem_init);
void PuniDokuritsu(PuniData* p);
void puniMoveDecel(PuniData* p);
void puniswTrapGetindex(s32 idx, s32* out);
void puniswGetindex(s32 base, s32 idx, s32* val, Vec* pos);
int puniMoveAtari(PuniData* p, Vec start, Vec dir, int reflect, f32 dist);
PuniWork* puniGetPunigp(void);
PuniData* puniGetPunidat(void);
USER_FUNC(evt_puni_all_fall_check);
USER_FUNC(evt_puni_all_movestop);
USER_FUNC(evt_puni_all_save);
USER_FUNC(evt_puni_attack_on);
USER_FUNC(evt_puni_count);
USER_FUNC(evt_puni_count_area);
USER_FUNC(evt_puni_dokan_onoff);
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

// Names from LLCOOLT's tree.
enum {
    PUNI_MODE_IDLE = 0,
    PUNI_MODE_JOIN = 1,
    PUNI_MODE_PARENT = 2,
    PUNI_MODE_SEARCH = 3,
    PUNI_MODE_MASS = 4,
    PUNI_MODE_AWAYJUMP = 5,
    PUNI_MODE_AWAY = 6,
    PUNI_MODE_ATTACK = 7,
    PUNI_MODE_ESCAPE = 8,
    PUNI_MODE_DAMAGE = 9,
    PUNI_MODE_PIYORI = 10,
    PUNI_MODE_RECOVER = 11,
    PUNI_MODE_DEAD = 12,
    PUNI_MODE_WIND = 13,
    PUNI_MODE_STOP = 14,
    PUNI_MODE_CHECKPOS = 15,
    PUNI_MODE_HAGURE = 16,
    PUNI_MODE_FALL = 17,
    PUNI_MODE_PREUPDRAFT = 18,
    PUNI_MODE_UPDRAFT = 19,
    PUNI_MODE_WONDER = 20,
    PUNI_MODE_FUNBARI = 21,
    PUNI_MODE_WATER = 22,
    PUNI_MODE_DODAI = 23,
    PUNI_MODE_TRAP = 24,
    PUNI_MODE_ENTER = 25,
    PUNI_MODE_EXIT = 26,
    PUNI_MODE_POINT = 27,
    PUNI_MODE_DOKAN = 28,
    PUNI_MODE_FUNC = 29,
    PUNI_MODE_INERT = 30,
};

enum {
    PUNI_TARGET_FLAG_ON = 1,
    PUNI_TARGET_FLAG_MASS = 2,
    PUNI_TARGET_FLAG_AWAY = 4,
};
#define PUNI_ALLY_LAST 102
#define PUNI_ENEMY_END 210
#define PUNI_ENEMY_FIRST 110
#define PUNI_LSW_SLOT_SIZE 7
#define PUNI_SOLID_FIRST 103
