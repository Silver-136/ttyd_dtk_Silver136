// Glitz Pit fighter ranking, rule and fighter tables: types shared by the two RELs that carry a copy of the ranking
// code (tou.c in tou, tou2.c in tou2).

#pragma once

#include "dolphin/types.h"

// RankEntry.flags
typedef enum RankFlags {
    RANK_FLAG_BEATEN = 1 << 0, // Mario beat this fighter (below him in the ranking); saved in GSWF_TOU_FIGHTER_BELOW_BASE + id
    RANK_FLAG_HIDDEN = 1 << 1, // not in the ranking yet
    RANK_FLAG_PLAYER = 1 << 2, // Mario's own entry (id 20, drawn in red)
} RankFlags;

typedef struct RankEntry {
    u16 flags; // 0x0, RANK_FLAG_*
    s32 id;    // 0x4, FighterId
    union {
        s16 total;   // ranking points
        u8 bytes[2]; // the same value as saved bytes (GSW_TOU_FIGHTER_POINTS_BASE + id * 2)
    } points;  // 0x8
} RankEntry;

typedef struct RankWork {
    s32 count;          // 23 before chapter 3 ends, 21 after it
    RankEntry* entries;
} RankWork;

// Scroll state of the two-page ranking window.
typedef struct DispWork {
    f32 scroll; // current list offset, eases toward target
    f32 target; // page * 220
    s32 page;
} DispWork;

typedef struct FighterData {
    u16 flags;           // 0x0, FighterFlags
    const char* name;    // 0x4
    const char* title;   // 0x8
    s32 setupId;         // 0xC, index into btlsetup_tou_tbl
    const char* taunt;   // 0x10, pre-match message
    const char* victory; // 0x14
    s16 startPoints;     // 0x18, ranking points a fighter starts with; copied to points.total when no ranking is saved
                         // (GSWF 2529 off); 250 for Rawk Hawk down to 0
} FighterData;

// FighterData.flags. rankingControll sorts each group by points, puts FIGHT_MAJOR/FIGHT_CHAMPION fighters above
// FIGHT_MINOR ones, then holds the fighters with bits 8-12 at fixed indexes of the list above Mario (the story
// fights): those bits each belong to one fighter.
typedef enum FighterFlags {
    FIGHT_MAJOR = 1 << 0,      // fighters 1-10
    FIGHT_MINOR = 1 << 1,      // fighters 11-19, 21 and 22
    FIGHT_CHAMPION = 1 << 2,   // fighter 0: always first
    FIGHT_PLAYER = 1 << 3,     // fighter 20, Mario's own entry; nothing tests it
    FIGHT_BANDITS = 1 << 8,    // held at list index 14
    FIGHT_KOOPAS = 1 << 9,     // held at list index 18
    FIGHT_GOOMBAS = 1 << 10,   // held at list index 19
    FIGHT_CLEFTS = 1 << 11,    // held at list index 10 (below Mario: just above the first FIGHT_MINOR fighter)
    FIGHT_KOOPATROL = 1 << 12, // held at list index 1
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
    s32 param;  // first rule parameter: evt_tou_get_rule_info returns it; the scripts store it in GSW_GLITZ_PIT_CONDITION_PARAM
    s32 param2; // evt_tou_get_rule_info returns it in args[4]; the scripts store it in GSW_GLITZ_PIT_CONDITION_PARAM2
} RuleData;
